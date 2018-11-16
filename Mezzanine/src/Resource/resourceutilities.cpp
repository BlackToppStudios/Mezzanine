// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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

#ifdef MEZZ_WINDOWS
    #define _WIN32_WINNT 0x0601
#endif

#include "Resource/resourceutilities.h"

#include "exception.h"
#include "stringtool.h"

#ifdef MEZZ_WINDOWS
    #include <Windows.h>
    #include <Winuser.h>
    #include <WinBase.h>
    #include <Shlobj.h> // for getting system directories
    // Below includes may not work on MSVC
    #include <direct.h> // for _getcwd
    #include <sys/stat.h>
    #include <cwchar>
#elif MEZZ_MACOSX
    #include <CoreServices/CoreServices.h>
    #include <unistd.h>//for sleep and getcwd
    #include <errno.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include "pwd.h"
    #include <mach-o/dyld.h> // for _NSGetExecutablePath
    #include <dirent.h>
#else
    #include <unistd.h>     // for sleep and getcwd
    #include <errno.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <pwd.h>
    #include <cstring>      // for strerror
    #include <dirent.h>
#endif

#include <algorithm>
#include <cstdio>

#include "Resource/platformundefs.h"

namespace
{
    using namespace Mezzanine;

    /// @brief ArgC as it was passed into Main.
    /// @details This cannot be set statically, it must wait for main(int, char**) to
    /// be initialized, then call the appropriate function to set this.
    int ArgC;
    /// @brief ArgC as it was passed into Main.
    /// @details This cannot be set statically, it must wait for main(int, char**) to
    /// be initialized, then call the appropriate function to set this.
    char** ArgV;

#ifdef MEZZ_WINDOWS
    /// @brief Converts a system time type to a standard time type.
    /// @param TimeVal the system time type to convert.
    /// @return Returns a converted standard time value.
    UInt64 ConvertTime(const FILETIME TimeVal)
    {
        LARGE_INTEGER Converter;
        Converter.HighPart = TimeVal.dwHighDateTime;
        Converter.LowPart = TimeVal.dwLowDateTime;
        return static_cast<UInt64>(Converter.QuadPart);
    }
    /*/// @brief Converts a standard time type to a system time type.
    /// @param TimeVal The standard time type to convert.
    /// @return Returns a converted system time value.
    FILETIME ConvertTime(const UInt64 TimeVal)
    {
        FILETIME Ret;
        LARGE_INTEGER Converter;
        Converter.QuadPart = TimeVal;
        Ret.dwHighDateTime = Converter.HighPart;
        Ret.dwLowDateTime = Converter.LowPart;
        return Ret;
    }//*/
    /// @brief Converts a narrow (8-bit) string to a wide (16-bit) string.
    /// @param Narrow The string to be converted.
    /// @return Returns a wide string with the converted contents.
    WideString ConvertToWideString(const String& Narrow)
    {
        WideString Ret;
        if( !Narrow.empty() ) {
            size_t WideLength = ::MultiByteToWideChar(CP_UTF8,0,Narrow.data(),Narrow.size(),0,0);
            Ret.resize(WideLength,L'\0');
            ::MultiByteToWideChar(CP_UTF8,0,Narrow.data(),Narrow.size(),&Ret[0],Ret.size());
        }
        return Ret;
    }
    /// @brief Converts a wide (16-bit) string to a narrow (8-bit) string.
    /// @param Wide The string to be converted.
    /// @return Returns a narrow string with the converted contents.
    String ConvertToNarrowString(const wchar_t* Wide)
    {
        String Ret;
        size_t WideLength = std::wcslen(Wide);
        if( WideLength > 0 ) {
            size_t NarrowLength = ::WideCharToMultiByte(CP_UTF8,0,Wide,WideLength,0,0,NULL,NULL);
            Ret.resize(NarrowLength,'\0');
            ::WideCharToMultiByte(CP_UTF8,0,Wide,WideLength,&Ret[0],Ret.size(),NULL,NULL);
        }
        return Ret;
    }
#endif // MEZZ_WINDOWS
#if defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
    /// @brief Converts posix file permissions to Mezzanine permissions.
    /// @param Perms The permissions to be converted.
    /// @return Returns a UInt32 bitmask containing the converted file permissions.
    UInt32 ConvertPermissions(const mode_t Perms)
    {
        UInt32 Ret = 0;
        Ret |= static_cast<UInt32>( Perms & S_IRWXO );
        Ret |= static_cast<UInt32>( Perms & S_IRWXG );
        Ret |= static_cast<UInt32>( Perms & S_IRWXU );
        return Ret;
    }
    /// @brief Converts Mezzanine file permissions to posix permissions
    /// @param Perms The permissions to be converted.
    /// @return Returns a mode_t bitmask containing the converted file permissions.
    mode_t ConvertPermissions(const UInt32 Perms)
    {
        mode_t Ret = 0;
        Ret |= static_cast<mode_t>( Perms & Resource::FP_Other );
        Ret |= static_cast<mode_t>( Perms & Resource::FP_Group );
        Ret |= static_cast<mode_t>( Perms & Resource::FP_Owner );
        return Ret;
    }
#endif // MEZZ_MACOSX || MEZZ_LINUX
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

        Boole FileExists(const String& PathAndFile)
        {
        #ifdef MEZZ_WINDOWS
            WideString ConvertedPath = std::move( ConvertToWideString(PathAndFile) );
            DWORD dwAttrib = GetFileAttributesW(ConvertedPath.c_str());
            return ( dwAttrib != INVALID_FILE_ATTRIBUTES && !( dwAttrib & FILE_ATTRIBUTE_DIRECTORY ) );
        #else
            //return ( ::access(PathAndName.c_str(),F_OK) );
            struct stat st;
            if( stat(DirectoryPath.c_str(),&st) == 0 ) {
                return S_ISREG(st.st_mode);
            }
            return false;
        #endif
        }

        Boole CopyFile(const String& OldPathAndFile, const String& NewPathAndFile, const Boole FailIfExists)
        {
        #ifdef MEZZ_WINDOWS
            DWORD CopyFlags = COPY_FILE_COPY_SYMLINK;
            if( FailIfExists ) {
                CopyFlags |= COPY_FILE_FAIL_IF_EXISTS;
            }
            WideString ConvertedOldPath = std::move( ConvertToWideString(OldPathAndFile) );
            WideString ConvertedNewPath = std::move( ConvertToWideString(NewPathAndFile) );
            return ( ::CopyFileExW(ConvertedOldPath.c_str(),ConvertedNewPath.c_str(),NULL,NULL,NULL,CopyFlags) != 0 );
        #else
            if( FailIfExists && FileExists(NewPathAndFile.c_str()) ) {
                return false;
            }
            std::ifstream SrcStream;
            std::ofstream DestStream;

            SrcStream.open(OldPathAndFile,std::ios::binary | std::ios_base::in);
            DestStream.open(NewPathAndFile,std::ios::binary | std::ios_base::out);

            DestStream << SrcStream.rdbuf();
            return true;
        #endif
        }

        Boole MoveFile(const String& OldPathAndFile, const String& NewPathAndFile, const Boole FailIfExists)
        {
        #ifdef MEZZ_WINDOWS
            DWORD MoveFlags = MOVEFILE_COPY_ALLOWED;
            if( !FailIfExists ) {
                MoveFlags |= MOVEFILE_REPLACE_EXISTING;
            }
            WideString ConvertedOldPath = std::move( ConvertToWideString(OldPathAndFile) );
            WideString ConvertedNewPath = std::move( ConvertToWideString(NewPathAndFile) );
            return ( ::MoveFileExW(ConvertedOldPath.c_str(),ConvertedNewPath.c_str(),MoveFlags) != 0 );
        #else
            if( FailIfExists && FileExists(NewPathAndFile.c_str()) ) {
                return false;
            }
            return ( ::rename(OldPathAndFile.c_str(),NewPathAndFile.c_str()) == 0 );
        #endif
        }

        Boole RemoveFile(const String& PathAndFile)
        {
        #ifdef MEZZ_WINDOWS
            WideString ConvertedPath = std::move( ConvertToWideString(PathAndFile) );
            return ( ::DeleteFileW(ConvertedPath.c_str()) != 0 );
        #else
            return ( ::unlink(PathAndFile.c_str()) == 0 );
        #endif
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Symlinks

        Boole CreateSymlink(const String& SymPath, const String& TargetPath)
        {
        #ifdef MEZZ_WINDOWS
            DWORD LinkFlags = 0;
            WideString ConvertedSymPath = std::move( ConvertToWideString(SymPath) );
            WideString ConvertedTargetPath = std::move( ConvertToWideString(TargetPath) );
            return ( ::CreateSymbolicLinkW(ConvertedSymPath.c_str(),ConvertedTargetPath.c_str(),LinkFlags) != 0 );
        #else
            return ( ::symlink(SymPath.c_str(),TargetPath.c_str()) == 0 );
        #endif
        }

        Boole CreateDirectorySymlink(const String& SymPath, const String& TargetPath)
        {
        #ifdef MEZZ_WINDOWS
            DWORD LinkFlags = SYMBOLIC_LINK_FLAG_DIRECTORY;
            WideString ConvertedSymPath = std::move( ConvertToWideString(SymPath) );
            WideString ConvertedTargetPath = std::move( ConvertToWideString(TargetPath) );
            return ( ::CreateSymbolicLinkW(ConvertedSymPath.c_str(),ConvertedTargetPath.c_str(),LinkFlags) != 0 );
        #else
            return ( ::symlink(SymPath.c_str(),TargetPath.c_str()) == 0 );
        #endif
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Basic Directory Management

        Boole DirectoryExists(const String& DirectoryPath)
        {
        #ifdef MEZZ_WINDOWS
            WideString ConvertedPath = std::move( ConvertToWideString(DirectoryPath) );
            DWORD dwAttrib = GetFileAttributesW(ConvertedPath.c_str());
            return ( dwAttrib != INVALID_FILE_ATTRIBUTES && ( dwAttrib & FILE_ATTRIBUTE_DIRECTORY ) );
        #else
            struct stat st;
            if( stat(DirectoryPath.c_str(),&st) == 0 ) {
                return S_ISDIR(st.st_mode);
            }
            return false;
        #endif
        }

        Boole CreateDirectory(const String& DirectoryPath)
        {
        #ifdef MEZZ_WINDOWS
            WideString ConvertedPath = std::move( ConvertToWideString(DirectoryPath) );
            if( ::CreateDirectoryW(ConvertedPath.c_str(),NULL) < 0 ) {
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
            StringVector FolderVec = StringTools::Split(DirectoryPath,"/\\");
            size_t StartIndex = 0;
            String PathAttempt;
            Char8 SysSlash = GetHostDirectorySeparator();
        #ifdef MEZZ_WINDOWS
            // For windows and windows like machines, see if the first entry is a drive, because attempting to make a drive is silly.
            if( FolderVec.at(0).find(':') != String::npos ) {
                PathAttempt.append( FolderVec.at(0) );
                PathAttempt.append( 1, SysSlash );
                StartIndex++;
            }
        #else
            PathAttempt.append( 1, SysSlash );
        #endif
            for( size_t VecIndex = StartIndex ; Result && VecIndex < FolderVec.size() ; ++VecIndex )
            {
                PathAttempt.append( FolderVec.at(VecIndex) );
                PathAttempt.append( 1, SysSlash );
                Result = CreateDirectory( PathAttempt );
            }
            return Result;
        }

        Boole RemoveDirectory(const String& DirectoryPath)
        {
        #ifdef MEZZ_WINDOWS
            WideString ConvertedPath = std::move( ConvertToWideString(DirectoryPath) );
            return ( ::RemoveDirectoryW(ConvertedPath.c_str()) != 0 );
        #else
            return ( ::rmdir(DirectoryPath.c_str()) == 0 );
        #endif
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Directory Traversal

        StringVector GetDirectoryContentNames(const String& DirectoryPath)
        {
            StringVector Ret;
        #ifdef MEZZ_WINDOWS
            WIN32_FIND_DATAW FileData;
            HANDLE FileHandle = INVALID_HANDLE_VALUE;

            WideString ConvertedPath = std::move( ConvertToWideString(DirectoryPath) );
            if( ConvertedPath.back() != L'/' && ConvertedPath.back() != L'\\' ) {
                ConvertedPath.append(1,L'\\');
            }
            ConvertedPath.append(1,L'*');
            FileHandle = ::FindFirstFileW( ConvertedPath.c_str(), &FileData );
            if( FileHandle == INVALID_HANDLE_VALUE ) {
                return Ret;
            }

            do{
                String EntryName = std::move( ConvertToNarrowString( FileData.cFileName ) );
                if( EntryName == "." || EntryName == ".." ) {
                    continue;
                }
                Ret.push_back( EntryName );
            }while( ::FindNextFileW( FileHandle, &FileData ) );

            ::FindClose(FileHandle);
        #else
            struct dirent* DirEntry;
            DIR* Directory = ::opendir( DirectoryPath.c_str() );
            if( Directory ) {
                stat FileStat;
                while( ( DirEntry = ::readdir(Directory) ) )
                {
                    String EntryName = DirEntry->d_name;
                    if( EntryName == "." || EntryName == ".." ) {
                        continue;
                    }

                    Ret.push_back( std::move(EntryName) );
                }

                ::closedir(Directory);
            }
        #endif
            return Ret;
        }

        ArchiveEntryVector GetDirectoryContents(const String& DirectoryPath)
        {
            ArchiveEntryVector Ret;
        #ifdef MEZZ_WINDOWS
            WIN32_FIND_DATAW FileData;
            LARGE_INTEGER FileSizeConverter;
            HANDLE FileHandle = INVALID_HANDLE_VALUE;

            WideString ConvertedPath = std::move( ConvertToWideString(DirectoryPath) );
            if( ConvertedPath.back() != L'/' && ConvertedPath.back() != L'\\' ) {
                ConvertedPath.append(1,L'\\');
            }
            ConvertedPath.append(1,L'*');
            FileHandle = ::FindFirstFileW( ConvertedPath.c_str(), &FileData );
            if( FileHandle == INVALID_HANDLE_VALUE ) {
                return Ret;
            }

            do{
                String EntryName = std::move( ConvertToNarrowString( FileData.cFileName ) );
                if( EntryName == "." || EntryName == ".." ) {
                    continue;
                }

                ArchiveEntry NewEntry;
                NewEntry.ArchType = Resource::AT_FileSystem;
                NewEntry.Name = std::move( EntryName );
                NewEntry.CreateTime = ConvertTime( FileData.ftCreationTime );
                NewEntry.AccessTime = ConvertTime( FileData.ftLastAccessTime );
                NewEntry.ModifyTime = ConvertTime( FileData.ftLastWriteTime );
                NewEntry.Attributes = Resource::FP_Read;
                if( ( FileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY ) == 0 ) {
                    NewEntry.Attributes |= Resource::FP_Write;
                }
                size_t DotPos = NewEntry.Name.find_last_of('.');
                if( DotPos < NewEntry.Name.size() ) {
                    String Extension = NewEntry.Name.substr( NewEntry.Name.find_last_of('.') );
                    StringTools::ToLowerCase(Extension);
                    if( Extension == ".exe" || Extension == ".bat" || Extension == ".com" || Extension == ".cmd" ) {
                        NewEntry.Attributes |= Resource::FP_Execute;
                    }
                }
                if( FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
                    NewEntry.EntType = Resource::ET_Directory;
                }else{
                    if( FileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT ) {
                        NewEntry.EntType = Resource::ET_Symlink;
                    }else{ // if( FileData.dwFileAttributes &  ) {
                        NewEntry.EntType = Resource::ET_File;
                    }

                    FileSizeConverter.LowPart = FileData.nFileSizeLow;
                    FileSizeConverter.HighPart = FileData.nFileSizeHigh;
                    NewEntry.Size = FileSizeConverter.QuadPart;
                }
                Ret.push_back(NewEntry);
            }while( ::FindNextFileW( FileHandle, &FileData ) );

            ::FindClose(FileHandle);
        #else
            struct dirent* DirEntry;
            DIR* Directory = ::opendir( DirectoryPath.c_str() );
            if( Directory ) {
                stat FileStat;
                while( ( DirEntry = ::readdir(Directory) ) )
                {
                    if( ::stat(DirEntry->d_name,&FileStat) ) {
                        continue;
                    }
                    String EntryName = DirEntry->d_name;
                    if( EntryName == "." || EntryName == ".." ) {
                        continue;
                    }

                    ArchiveEntry NewEntry;
                    NewEntry.ArchType = Resource::AT_FileSystem;
                    NewEntry.Name = std::move( EntryName );
                    NewEntry.CreateTime = ConvertTime( FileStat.st_ctime );
                    NewEntry.AccessTime = ConvertTime( FileStat.st_atime );
                    NewEntry.ModifyTime = ConvertTime( FileStat.st_mtime );
                    NewEntry.Attributes = ConvertPermissions(FileStat.st_mode);

                    if( S_ISDIR(FileStat.st_mode) ) {
                        NewEntry.EntType = Resource::ET_Directory;
                    }else{
                        if( S_ISLNK(FileStat.st_mode) ) {
                            NewEntry.EntType = Resource::ET_Symlink;
                        }else if( S_ISREG(FileStat.st_mode) ) { // if( FileData.dwFileAttributes &  ) {
                            NewEntry.EntType = Resource::ET_File;
                        }else{
                            // I dunno wtf we found
                            continue;
                        }

                        NewEntry.Size = FileStat.st_size;
                    }
                    Ret.push_back(NewEntry);
                }

                ::closedir(Directory);
            }
        #endif
            return Ret;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Path Utilities

        String GetDirName(const char* FileName)
        {
            size_t SlashPos = String::npos, CurrPos = 0;
            while( FileName[CurrPos] != '\0' )
            {
                if( FileName[CurrPos] == '\\' || FileName[CurrPos] == '/' ) {
                    SlashPos = CurrPos;
                }
                ++CurrPos;
            }
            if( SlashPos != String::npos ) {
                return String(FileName,SlashPos + 1);
            }
            return String();
        }

        String GetDirName(const String& FileName)
        {
            size_t SlashPos = FileName.find_last_of("\\/");
            if( FileName.npos == SlashPos ) {
                return String();
            }else{
                return FileName.substr(0,SlashPos + 1);
            }
        }

        String GetBaseName(const char* FileName)
        {
            size_t SlashPos = String::npos, CurrPos = 0;
            while( FileName[CurrPos] != '\0' )
            {
                if( FileName[CurrPos] == '\\' || FileName[CurrPos] == '/' ) {
                    SlashPos = CurrPos;
                }
                ++CurrPos;
            }
            if( SlashPos != String::npos ) {
                return String(FileName + (SlashPos + 1),(CurrPos - SlashPos) - 1);
            }
            return String(FileName,CurrPos);
        }

        String GetBaseName(const String& FileName)
        {
            size_t SlashPos = FileName.find_last_of("\\/");
            if( FileName.npos == SlashPos ) {
                return FileName;
            }else{
                return FileName.substr(SlashPos + 1,FileName.npos);
            }
        }

        Boole IsDirectorySeparator(const Char8 ToCheck)
        {
            return ( ToCheck == '\\' || ToCheck == '/' );
        }

        Boole IsHostDirectorySeparator(const Char8 ToCheck)
        {
        #ifdef MEZZ_WINDOWS
            return ( ToCheck == '\\' );
        #else
            return ( ToCheck == '/' );
        #endif
        }

        Char8 GetHostDirectorySeparator()
        {
        #ifdef MEZZ_WINDOWS
            return '\\';
        #else
            return '/';
        #endif
        }

        Char8 GetUniversalDirectorySeparator()
        {
            return '/';
        }

        Char8 GetSystemPathSeparator()
        {
        #ifdef MEZZ_WINDOWS
            return ';';
        #else
            return ':';
        #endif
        }

        Boole IsPathAbsolute(const String& ToCheck)
        {
            return IsPathAbsolute_Windows(ToCheck) || IsPathAbsolute_Posix(ToCheck);
        }

        Boole IsPathAbsolute_Host(const String& ToCheck)
        {
        #ifdef MEZZ_WINDOWS
            return IsPathAbsolute_Windows(ToCheck);
        #else
            return IsPathAbsolute_Posix(ToCheck);
        #endif
        }

        Boole IsPathAbsolute_Posix(const String& ToCheck)
        {
            if( !ToCheck.empty() ) {
                return ( ToCheck[0] == '/' );
            }
            return false;
        }

        Boole IsPathAbsolute_Windows(const String& ToCheck)
        {
            if( ToCheck.size() >= 3 ) {
                return ( StringTools::IsAlphaLetter(ToCheck[0]) &&
                         ToCheck[1] == ':' &&
                         ( ToCheck[2] == '\\' || ToCheck[2] == '/' ) );
            }
            return false;
        }

        Boole IsPathRelative(const String& ToCheck)
        {
            return !IsPathAbsolute(ToCheck);
        }

        Boole IsPathRelative_Host(const String& ToCheck)
        {
            return !IsPathAbsolute_Host(ToCheck);
        }

        Boole IsPathRelative_Posix(const String& ToCheck)
        {
            return !IsPathAbsolute_Posix(ToCheck);
        }

        Boole IsPathRelative_Windows(const String& ToCheck)
        {
            return !IsPathAbsolute_Windows(ToCheck);
        }

        Integer GetDirectoryDepth(const String& ToCheck, const Boole ExitIfNegative)
        {
            Integer Depth = 0;
            String DirStr;
            for( Char8 CurrChar : ToCheck )
            {
                if( CurrChar == '/' || CurrChar == '\\' ) {
                    if( DirStr == ".." ) {
                        Depth -= 1;
                    }else if( DirStr != "." ) {
                        Depth += 1;
                    }
                    DirStr.clear();

                    if( ExitIfNegative && Depth < 0 ) {
                        break;
                    }
                }else{
                    DirStr.append(1,CurrChar);
                }
            }
            return Depth;
        }

        Boole IsSubPath(const String& BasePath, const String& CheckPath)
        {
            Boole BaseIsAbsolute = IsPathAbsolute(BasePath);
            Boole CheckIsAbsolute = IsPathAbsolute(CheckPath);

            if( BaseIsAbsolute == CheckIsAbsolute ) {
                String NormBasePath = RemoveDotSegments(BasePath);
                String NormCheckPath = RemoveDotSegments(CheckPath);
                String::const_iterator BaseIt = NormBasePath.begin();
                String::const_iterator BaseEnd = NormBasePath.end();
                String::const_iterator CheckIt = NormCheckPath.begin();
                String::const_iterator CheckEnd = NormCheckPath.begin();

                if( IsPathAbsolute_Windows(BasePath) ) {
                    std::advance(BaseIt,2);
                }

                while( BaseIt != BaseEnd )
                {
                    if( CheckIt == CheckEnd || (*BaseIt) != (*CheckIt) ) {
                        return false;
                    }

                    ++BaseIt;
                    ++CheckIt;
                }

                String CheckRemains(CheckIt,CheckEnd);
                return ( GetDirectoryDepth(CheckRemains,true) > 0 );
            }

            if( BaseIsAbsolute && !CheckIsAbsolute ) {
                return ( GetDirectoryDepth(CheckPath,true) > 0 );
            }

            if( !BaseIsAbsolute && CheckIsAbsolute ) {
                String WorkingDir = GetWorkingDirectory();
                if( IsDirectorySeparator(WorkingDir.back()) ) {
                    Whole StartIdx = ( IsDirectorySeparator(BasePath.front()) ? 1 : 0 );
                    WorkingDir.append(BasePath,StartIdx,String::npos);
                }else{
                    if( !IsDirectorySeparator(BasePath.front()) ) {
                        WorkingDir.append(1,GetUniversalDirectorySeparator());
                    }
                    WorkingDir.append(BasePath);
                }
                return IsSubPath(WorkingDir,CheckPath);
            }

            return false;
        }

        String BuildPath(const StringVector& ToBuild, const Boole EndWithSlash)
        {
            Whole CharCount = ToBuild.size();
            StringVector::const_iterator BuildIt = ToBuild.begin();
            // Get a count of the characters we'll be using
            std::for_each(BuildIt,ToBuild.end(),[&CharCount](const String& CurrStr) {
                CharCount += CurrStr.length();
            });
            // Create the variables we'll be using
            String Ret;
            Ret.reserve(CharCount);
            const Char8 UnivSeparator = GetUniversalDirectorySeparator();
            // Build the path
            while( BuildIt != ToBuild.end() )
            {
                Ret.append( 1, UnivSeparator );
                Ret.append( *BuildIt );
                ++BuildIt;
            }
            if( EndWithSlash ) {
                Ret.append( 1, UnivSeparator );
            }
            return Ret;
        }

        String RemoveDotSegments(const String& ToRemove)
        {
            Boole EndsWithSlash = IsDirectorySeparator(ToRemove.back());
            StringVector RetSegments;
            StringVector SplitPath = StringTools::Split(ToRemove,"/\\",std::numeric_limits<Whole>::max());
            for( StringVector::iterator VecIt = SplitPath.begin() ; VecIt != SplitPath.end() ; ++VecIt )
            {
                if( (*VecIt) == ".." ) {
                    RetSegments.pop_back();
                }else if( (*VecIt) != "." ) {
                    RetSegments.push_back(*VecIt);
                }
            }
            return std::move( BuildPath(RetSegments,EndsWithSlash) );
        }

        String CombinePathAndFileName(const String& FilePath, const String& FileName)
        {
            String FullPath = FilePath;
            String Separator(1,Resource::GetUniversalDirectorySeparator());
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
            const Char8 Sep = GetSystemPathSeparator();
            String OneEntry;

            for( String::const_iterator Current = PATH.begin() ; PATH.end() != Current ; ++Current )
            {
                if( Sep == *Current ) {
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
            StringVector PATH = std::move( GetSystemPATH() );
            for( StringVector::const_iterator Iter = PATH.begin() ; Iter!=PATH.end() ; ++Iter )
            {
                StringVector Entries = std::move( GetDirectoryContentNames(*Iter) );
                if( std::find(Entries.begin(),Entries.end(),ExecutableName) != Entries.end() ) {
                    return (*Iter) + GetHostDirectorySeparator();
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
            GetModuleFileName( NULL, Results, FILENAME_MAX );
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
            if(ArgCount>0) {
                if(String("") == GetBaseName(ArgVars[0])) { // No command is clearly bogus, must bail
                    return "";
                }

                String Results(GetDirName(ArgVars[0]));

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
        #ifdef MEZZ_WINDOWS
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
        #ifdef MEZZ_WINDOWS
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
        #ifdef MEZZ_WINDOWS
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
