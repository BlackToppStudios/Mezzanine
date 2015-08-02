// © Copyright 2010 - 2015 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _resourceutilities_cpp
#define _resourceutilities_cpp

#include "Resource/resourceutilities.h"

#include "exception.h"
#include "stringtool.h"

#include <dirent.h>
#ifdef WINDOWS
    #include <Windows.h>
    #include <Winuser.h>
    #include <WinBase.h>
    #include <Shlobj.h> // for getting system directories
    // Below includes may not work on MSVC
    #include <direct.h> // for _getcwd
    #include <sys/stat.h>
#elif MACOSX
    #include <CoreServices/CoreServices.h>
    #include <unistd.h>//for sleep and getcwd
    #include <errno.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include "pwd.h"
    #include <mach-o/dyld.h> // for _NSGetExecutablePath
#else
    #include <unistd.h>     // for sleep and getcwd
    #include <errno.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <pwd.h>
    #include <cstring>      // for strerror
#endif

#include <algorithm>
#include <cstdio>

#ifdef CreateDirectory
#undef CreateDirectory
#endif

#ifdef RemoveDirectory
#undef RemoveDirectory
#endif

namespace
{
    /// @brief ArgC as it was passed into Main.
    /// @details This cannot be set statically, it must wait for main(int, char**) to
    /// be initialized, then call the appropriate function to set this.
    int ArgC;
    /// @brief ArgC as it was passed into Main.
    /// @details This cannot be set statically, it must wait for main(int, char**) to
    /// be initialized, then call the appropriate function to set this.
    char** ArgV;
}

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        // MainArg Utilities

        void CacheMainArgs(int ArgCount, char** ArgVars)
        {
            ArgC = ArgCount;
            ArgV = ArgVars;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // File Management

        /*Boole RemoveFile(const String& PathAndFile)
        {
            return ( remove( PathAndFile.c_str() ) == 0 );
        }//*/

        ///////////////////////////////////////////////////////////////////////////////
        // Basic Directory Management

        Boole CreateDirectory(const String& DirectoryPath)
        {
            #ifdef WINDOWS
            if( ::CreateDirectoryA(DirectoryPath.c_str(),NULL) < 0 ) {
                if( ERROR_ALREADY_EXISTS == ::GetLastError() ) {
                    return false;
                }
                StringStream ExceptionStream;
                ExceptionStream << "Unable to create directory.  Error follows:" << std::endl;
                if( ERROR_PATH_NOT_FOUND == ::GetLastError() ) {
                    ExceptionStream << "Path to requested directory does not exist.";
                }else{
                    ExceptionStream << "Error Unknown. :(";
                }
                MEZZ_EXCEPTION(ExceptionBase::IO_DIRECTORY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
            }
            return true;
            #else
            if( ::mkdir(DirectoryPath.c_str(),0777) < 0 ) {
                if( EEXIST == errno ) {
                    return false;
                }
                StringStream ExceptionStream;
                ExceptionStream << "Unable to create directory.  Error follows:" << std::endl;
                ExceptionStream << std::strerror(errno);
                MEZZ_EXCEPTION(ExceptionBase::IO_DIRECTORY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
            }
            return true;
            #endif
        }

        Boole CreateDirectoryPath(const String& DirectoryPath)
        {
            Boole Result = true;
            StringVector FolderNames;
            CountedPtr<StringVector> FolderVec = StringTools::Split(DirectoryPath,"/\\");
            size_t StartIndex = 0;
            String PathAttempt;
            Char8 SysSlash = GetDirectorySeparator();
            #ifdef WINDOWS
            // For windows and windows like machines, see if the first entry is a drive, because attempting to make a drive is silly.
            if(FolderVec->at(0).find(':') != String::npos) {
                PathAttempt.append( FolderVec->at(0) );
                PathAttempt.append( 1, SysSlash );
                StartIndex++;
            }
            #else
            PathAttempt.append( 1, SysSlash );
            #endif
            for( size_t VecIndex = StartIndex ; VecIndex < FolderVec->size() ; ++VecIndex )
            {
                PathAttempt.append( FolderVec->at(VecIndex) );
                PathAttempt.append( 1, SysSlash );
                Result = CreateDirectory( PathAttempt );
            }
            return Result;
        }

        Boole DoesDirectoryExist(const String& DirectoryPath)
        {
            struct stat st;
            if( stat(DirectoryPath.c_str(),&st) == 0 ) {
                return S_ISDIR(st.st_mode);
            }/*else{
                //MEZZ_EXCEPTION(ExceptionBase::IO_DIRECTORY_NOT_FOUND_EXCEPTION,"Unknown error getting directory information.");
                return false;
            }//*/
            return false;
        }

        void RemoveDirectory(const String& DirectoryPath)
        {
            if( !rmdir(DirectoryPath.c_str()) ) {
                return;
            }else{
                MEZZ_EXCEPTION(ExceptionBase::IO_DIRECTORY_NOT_FOUND_EXCEPTION,"Unknown error removing directory.");
            }
        }

        StringVector GetDirContents(const String& Dir)
        {
            StringVector Results;
            DIR *Directory;
            struct dirent *DirEntry;
            if( ( Directory = opendir( Dir.c_str() ) ) ) {
                while( ( DirEntry = readdir(Directory) ) )
                {
                    Results.push_back( DirEntry->d_name );
                    //DirEntry->d_type Later this can be modified to include the type of file entry it is, like a file, block device, directory, socket etc...
                }

                closedir(Directory);
                return Results;
            }else{
                MEZZ_EXCEPTION(ExceptionBase::IO_DIRECTORY_NOT_FOUND_EXCEPTION,String("Error listing directory contents"));
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Path Utilities

        String DirName(const String& FileName)
        {
            Whole Last(FileName.find_last_of("\\/"));
            if(FileName.npos == Last) {
                return String();
            }else{
                if( Last<FileName.size() ) {
                    return FileName.substr(0,Last+1);
                }else{
                    return FileName.substr(0,Last+1);
                }
            }
        }

        String BaseName(const String& FileName)
        {
            Whole Last(FileName.find_last_of("\\/"));
            if(FileName.npos == Last) {
                return FileName;
            }else{
                if(Last<FileName.size()) {
                    return FileName.substr(Last+1,FileName.npos);
                }else{
                    return String("");
                }
            }
        }

        Char8 GetDirectorySeparator()
        {
            #ifdef WINDOWS
            return '\\';
            #else
            return '/';
            #endif
        }

        Char8 GetPathSeparator()
        {
            #ifdef WINDOWS
            return ';';
            #else
            return ':';
            #endif
        }

        String CombinePathAndFileName(const String& FilePath, const String& FileName)
        {
            String FullPath = FilePath;
            String Separator(1,Resource::GetDirectorySeparator());
            if( !StringTools::EndsWith(FilePath,Separator,true) ) {
                FullPath.append(Separator);
            }
            FullPath.append(FileName);
            return FullPath;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // System Path Utilities

        StringVector GetSystemPATH(const String& PATH)
        {
            StringVector Results;
            const Char8 Sep = GetPathSeparator();
            String OneEntry;

            for( String::const_iterator Current = PATH.begin() ; PATH.end()!=Current ; ++Current )
            {
                if(Sep==*Current) {
                    Results.push_back(OneEntry);
                    OneEntry.clear();
                }else{
                    OneEntry += *Current;
                }
            }
            return Results;
        }

        String Which(const String& ExecutableName)
        {
            StringVector PATH( GetSystemPATH() );

            for( StringVector::const_iterator Iter = PATH.begin() ; Iter!=PATH.end() ; ++Iter )
            {
                StringVector Entries( GetDirContents(*Iter) );
                if( std::find(Entries.begin(),Entries.end(),ExecutableName) != Entries.end() ) {
                    return (*Iter) + GetDirectorySeparator();
                }
            }
            return String();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Execution Directory Utilities

        String GetExecutableDir()
        {
            return GetExecutableDir(ArgC,ArgV);
        }

        String GetExecutableDir(int ArgCount, char** ArgVars)
        {
            String Results( GetExecutableDirFromArg(ArgCount,ArgVars) );
            if( String(".") == Results || String("") == Results ) { // Means it might have valid exename
                return GetExecutableDirFromSystem();
            }else{
                return Results;
            }
        }

        String GetExecutableDirFromSystem()
        {
            char Results[FILENAME_MAX];
            #ifdef LINUX
                MaxInt Length = ::readlink("/proc/self/exe", Results, sizeof(Results)-1);
                if( Length != -1 ) {
                    Results[Length] = '\0';
                    return DirName(String(Results));
                }else{
                    return "";
                }
            #endif
            #ifdef WINDOWS
                GetModuleFileName( NULL, Results, FILENAME_MAX );
                return DirName(String(Results));
            #endif
            #ifdef MACOSX
                uint32_t size = sizeof(Results);
                if( _NSGetExecutablePath(Results, &size) == 0 ) {
                    return DirName(String(Results));
                }else{
                    return "";
                }
            #endif
        }

        String GetExecutableDirFromArg()
        {
            return GetExecutableDirFromArg(ArgC,ArgV);
        }

        String GetExecutableDirFromArg(int ArgCount, char** ArgVars)
        {
            if(ArgCount>0) {
                if(String("") == BaseName(ArgVars[0])) { // No command is clearly bogus, must bail
                    return "";
                }

                String Results(DirName(ArgVars[0]));

                // strip ./ ../ .\ ..\ and
                //String::iterator From=Results.begin();
                //for(String::iterator Iter=Results.begin(); Iter!=Results.end();)
                //{}

                if( String("") == Results || String("./") == Results || String(".\\") == Results) {// common cases of exe existing but dir is empty
                    return ".";
                }

                if(String("")!=Results) { // and exe is empty.
                    return Results;
                }
                return String("");
            }else{
                return String("");
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Working Directory Utilities

        void ChangeWorkingDirectory(const String& ChangeTo)
        {
            #ifdef WINDOWS
            if(_chdir(ChangeTo.c_str()))
            #else
            if(chdir(ChangeTo.c_str()))
            #endif
            { MEZZ_EXCEPTION(ExceptionBase::IO_DIRECTORY_NOT_FOUND_EXCEPTION,String("Could not change to directory \"")+ChangeTo+"\" error: "+ToString(errno)); }
        }

        String GetWorkingDirectory()
        {
            char cCurrentPath[FILENAME_MAX];
            // char cCurrentPath[MAXPATHLEN];
            #ifdef WINDOWS
            String Results (_getcwd(cCurrentPath,sizeof(cCurrentPath)));
            #else
            String Results (getcwd(cCurrentPath,sizeof(cCurrentPath)));
            #endif
            return Results;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Application Data Directory Utilities

        String ResolveDataPathFromString(const String& PathVar)
        {
            String LowerVar = PathVar;
            StringTools::ToLowerCase(LowerVar);
            if(LowerVar == "localappdata") return GetLocalAppDataDir();
            else if(LowerVar == "shareableappdata") return GetShareableAppDataDir();
            else if(LowerVar == "currentuserdata") return GetCurrentUserDataDir();
            else if(LowerVar == "commonuserdata") return GetCommonUserDataDir();
            else
            {
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Attempting to retrieve unknown path variable: \"" + PathVar + "\".");
            }
        }

        String GetLocalAppDataDir()
        {
            #ifdef WINDOWS
            TCHAR path_local_appdata[MAX_PATH];
            if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA|CSIDL_FLAG_CREATE, NULL, 0, path_local_appdata))) {
                return path_local_appdata;
            }
            #else
            struct passwd* pw = getpwuid(getuid());
            if(pw) {
                return String(pw->pw_dir);
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Could not get user information to retrieve app data directory.");
            }

            // might be some useful MAC OS X code
            /*#elif MACOSX
            FSRef ref;
            OSType folderType = kApplicationSupportFolderType;
            char path[PATH_MAX];
            FSFindFolder( kUserDomain, folderType, kCreateFolder, &ref );
            FSRefMakePath( &ref, (UInt8*)&path, PATH_MAX );
            return path;*/
            #endif
            return "";
        }

        String GetShareableAppDataDir()
        {
            #ifdef WINDOWS
            TCHAR path_appdata[MAX_PATH];
            if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA|CSIDL_FLAG_CREATE, NULL, 0, path_appdata))) {
                return path_appdata;
            }
            #else
            struct passwd* pw = getpwuid(getuid());
            if(pw) {
                return String(pw->pw_dir);
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Could not get user information to retrieve home directory.");
            }
            #endif
            return "";
        }

        String GetCurrentUserDataDir()
        {
            #ifdef WINDOWS
            TCHAR path_personal[MAX_PATH];
            if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL|CSIDL_FLAG_CREATE, NULL, 0, path_personal))) {
                return path_personal;
            }
            #else
            struct passwd* pw = getpwuid(getuid());
            if(pw) {
                return String(pw->pw_dir);
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Could not get user information to retrieve user data directory.");
            }
            #endif
            return "";
        }

        String GetCommonUserDataDir()
        {
            #ifdef WINDOWS
            TCHAR path_common_personal[MAX_PATH];
            if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_DOCUMENTS|CSIDL_FLAG_CREATE, NULL, 0, path_common_personal))) {
                return path_common_personal;
            }
            #else
            struct passwd* pw = getpwuid(getuid());
            if(pw) {
                return String(pw->pw_dir);
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Could not get user information to retrieve common data directory.");
            }
            #endif
            return "";
        }
    }//Resource
}//Mezzanine

#endif
