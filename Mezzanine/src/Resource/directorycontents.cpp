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
#ifndef _resourcedirectorycontents_cpp
#define _resourcedirectorycontents_cpp

#ifdef MEZZ_WINDOWS
    #define _WIN32_WINNT 0x0601
#endif

#include "Resource/directorycontents.h"
#include "stringtool.h"

#ifdef MEZZ_WINDOWS
    #include <Windows.h>
#elif MEZZ_MACOSX
    #include <stdio.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <dirent.h>
    #include <unistd.h>
#else
    #include <stdio.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <dirent.h>
    #include <unistd.h>
#endif

#include "Resource/platformundefs.h"

namespace
{
    using namespace Mezzanine;

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
        // Directory Contents

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
    }//Resource
}//Mezzanine

#endif
