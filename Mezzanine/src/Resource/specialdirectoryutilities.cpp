// © Copyright 2010 - 2018 BlackTopp Studios Inc.
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
#ifndef _resourcespecialdirectoryutilities_cpp
#define _resourcespecialdirectoryutilities_cpp

#ifdef MEZZ_WINDOWS
    #define _WIN32_WINNT 0x0601
#endif

#include "Resource/specialdirectoryutilities.h"
#include "Resource/pathutilities.h"
#include "exception.h"
#include "stringtool.h"

#ifdef MEZZ_WINDOWS
    #include <Windows.h>
    #include <direct.h>
    #include <errno.h>
    #include <Shlobj.h>
#elif MEZZ_MACOSX
    #include <sys/types.h>
    #include <unistd.h>
    #include <pwd.h>
    #include <mach-o/dyld.h> // for _NSGetExecutablePath
#else
    #include <sys/types.h>
    #include <unistd.h>
    #include <pwd.h>
#endif

#include "Resource/platformundefs.h"

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
        #ifdef MEZZ_LINUX
            MaxInt Length = ::readlink("/proc/self/exe", Results, sizeof(Results)-1);
            if( Length != -1 ) {
                Results[Length] = '\0';
                return GetDirName(String(Results));
            }else{
                return "";
            }
        #endif
        #ifdef MEZZ_WINDOWS
            ::GetModuleFileName( NULL, Results, FILENAME_MAX );
            return GetDirName(String(Results));
        #endif
        #ifdef MEZZ_MACOSX
            uint32_t size = sizeof(Results);
            if( _NSGetExecutablePath(Results, &size) == 0 ) {
                return GetDirName(String(Results));
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
            if( ArgCount > 0 ) {
                if(String("") == GetBaseName(ArgVars[0])) { // No command is clearly bogus, must bail
                    return "";
                }

                String Results(GetDirName(ArgVars[0]));

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
        #ifdef MEZZ_WINDOWS
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
        #ifdef MEZZ_WINDOWS
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
            if(LowerVar == "localappdata") {
                return GetLocalAppDataDir();
            }else if(LowerVar == "shareableappdata") {
                return GetShareableAppDataDir();
            }else if(LowerVar == "currentuserdata") {
                return GetCurrentUserDataDir();
            }else if(LowerVar == "commonuserdata") {
                return GetCommonUserDataDir();
            }else{
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Attempting to retrieve unknown path variable: \"" + PathVar + "\".");
            }
            return String();
        }

        String GetLocalAppDataDir()
        {
        #ifdef MEZZ_WINDOWS
            TCHAR path_local_appdata[MAX_PATH];
            if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA|CSIDL_FLAG_CREATE, NULL, 0, path_local_appdata))) {
                return path_local_appdata;
            }
        #else
            struct passwd* pw = getpwuid(getuid());
            if( pw != nullptr ) {
                return String(pw->pw_dir);
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Could not get user information to retrieve app data directory.");
            }

            // might be some useful MAC OS X code
            /*#elif MEZZ_MACOSX
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
        #ifdef MEZZ_WINDOWS
            TCHAR path_appdata[MAX_PATH];
            if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA|CSIDL_FLAG_CREATE, NULL, 0, path_appdata))) {
                return path_appdata;
            }
        #else
            struct passwd* pw = getpwuid(getuid());
            if( pw != nullptr ) {
                return String(pw->pw_dir);
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Could not get user information to retrieve home directory.");
            }
        #endif
            return "";
        }

        String GetCurrentUserDataDir()
        {
        #ifdef MEZZ_WINDOWS
            TCHAR path_personal[MAX_PATH];
            if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL|CSIDL_FLAG_CREATE, NULL, 0, path_personal))) {
                return path_personal;
            }
        #else
            struct passwd* pw = getpwuid(getuid());
            if( pw != nullptr ) {
                return String(pw->pw_dir);
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Could not get user information to retrieve user data directory.");
            }
        #endif
            return "";
        }

        String GetCommonUserDataDir()
        {
        #ifdef MEZZ_WINDOWS
            TCHAR path_common_personal[MAX_PATH];
            if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_DOCUMENTS|CSIDL_FLAG_CREATE, NULL, 0, path_common_personal))) {
                return path_common_personal;
            }
        #else
            struct passwd* pw = getpwuid(getuid());
            if( pw != nullptr ) {
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
