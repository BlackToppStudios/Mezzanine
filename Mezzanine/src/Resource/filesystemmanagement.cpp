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
#ifndef _filesystemmanagement_cpp
#define _filesystemmanagement_cpp

#ifdef MEZZ_WINDOWS
    #define _WIN32_WINNT 0x0601
#endif

#include "Resource/filesystemmanagement.h"
#include "Resource/pathutilities.h"
#include "exception.h"
#include "stringtool.h"

#ifdef MEZZ_WINDOWS
    #include <Windows.h>
#elif MEZZ_MACOSX
    #include <stdio.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>
#else
    #include <stdio.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>
#endif

#include "Resource/platformundefs.h"

namespace
{
    using namespace Mezzanine;

#ifdef MEZZ_WINDOWS
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
#endif // MEZZ_WINDOWS
}

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Basic File Management

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
            Char8 SysSlash = GetDirectorySeparator_Host();
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
    }//Resource
}//Mezzanine

#endif
