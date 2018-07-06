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
#ifndef _resourcefilesystemarchive_cpp
#define _resourcefilesystemarchive_cpp

#include "Resource/filesystemarchive.h"
#include "Resource/resourceutilities.h"

#include "stringtool.h"
#include "datastream.h"
#include "filestream.h"

#include "exception.h"

namespace
{
    using namespace Mezzanine;

    /// @brief Recursively gets the contents of a directory and populated a vector with archive entries.
    /// @param BaseDir The base directory that is being searched.
    /// @param Dir The relative part of the directory to search.
    /// @param Pattern The pattern to use as a filter of what entries should be appended.
    /// @param Contents The listing of contents to append to.
    /// @param OmitDirs Whether or not the entries for subdirectories should not be appended to the container.
    void PopulateDirectoryContentsRecursive(const String& BaseDir, const String& Dir, const String& Pattern, Resource::ArchiveEntryVector& Contents, const Boole OmitDirs)
    {
        String ActualDir = BaseDir + Dir;
        Resource::ArchiveEntryVector CurrDirContents = Resource::GetDirectoryContents(ActualDir);
        for( const Resource::ArchiveEntry& Ent : CurrDirContents )
        {
            String FullName = Dir + Ent.Name;
            if( !StringTools::PatternMatch(FullName,Pattern,false) ) {
                continue;
            }

            if( Ent.EntType == Resource::ET_Directory ) {
                if( !Resource::IsDirectorySeparator( FullName.back() ) ) {
                    FullName.append(1,Resource::GetUniversalDirectorySeparator());
                }

                if( !OmitDirs ) {
                    Contents.emplace_back(Ent);
                    Contents.back().Name = FullName;
                }

                PopulateDirectoryContentsRecursive(BaseDir,FullName,Pattern,Contents,OmitDirs);
            }else{
                Contents.emplace_back(Ent);
                Contents.back().Name = FullName;
            }
        }
    }
}

namespace Mezzanine
{
    namespace Resource
    {
        FileSystemArchive::FileSystemArchive() :
            OpenFlags(0)
            {  }

        FileSystemArchive::~FileSystemArchive()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Open / Close

        void FileSystemArchive::Open(const char* Identifier, const Whole Flags)
        {
            this->Open(Identifier,Flags);
        }

        void FileSystemArchive::Open(const String& Identifier, const Whole Flags)
        {
            Boole DirExists = DirectoryExists(Identifier);
            if( !DirExists ) {
                if( Flags & AOF_Create ) {
                    DirExists = CreateDirectory(Identifier);
                }

                if( !DirExists ) {
                    StringStream ExceptionStream;
                    ExceptionStream << "Directory \"" << Identifier << "\" not found.";
                    MEZZ_EXCEPTION(ExceptionBase::IO_DIRECTORY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
                }
            }

            this->OpenFlags = Flags;
            this->ArchiveIdentifier.assign(Identifier);
            if( !Resource::IsDirectorySeparator( this->ArchiveIdentifier.back() ) ) {
                this->ArchiveIdentifier.append(1,Resource::GetUniversalDirectorySeparator());
            }
        }

        void FileSystemArchive::Open(const String& Identifier, Char8* Buffer, const size_t BufferSize, const Whole Flags, const Boole Owner)
        {
            MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"FileSystem Archives do not support initializing from a memory buffer.");
        }

        Boole FileSystemArchive::IsOpen() const
        {
            return !this->ArchiveIdentifier.empty();
        }

        void FileSystemArchive::Close()
        {
            this->OpenFlags = AOF_None;
            this->ArchiveIdentifier.clear();
        }

        Whole FileSystemArchive::GetFlags() const
        {
            return this->OpenFlags;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Streaming

        DataStreamPtr FileSystemArchive::OpenStream(const char* Identifier, const Whole Flags)
        {
            String StreamIdentifier(Identifier);
            return OpenStream(StreamIdentifier,Flags);
        }

        DataStreamPtr FileSystemArchive::OpenStream(const String& Identifier, const Whole Flags)
        {
            if( Flags & Mezzanine::SF_Write && !( this->OpenFlags & AOF_Write ) ) {
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Attempting to open a stream for writing while parent archive prohibits writing.");
            }
            if( Flags & Mezzanine::SF_Read && !( this->OpenFlags & AOF_Read ) ) {
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Attempting to open a stream for reading while parent archive prohibits reading.");
            }
            FileStreamPtr Ret = std::make_shared<FileStream>();
            Ret->OpenFile(Identifier,Flags);
            return Ret;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Querying

        Int64 FileSystemArchive::GetEntryCount() const
            { return -1; }

        ArchiveEntryPtr FileSystemArchive::GetEntry(const UInt64 Index) const
            { return nullptr; }

        ArchiveEntryPtr FileSystemArchive::GetEntry(const String& FileName) const
        {
            String ActualFileName = Resource::GetBaseName(FileName);
            String DirPath = this->ArchiveIdentifier;
            DirPath.append(Resource::GetDirName(FileName));
            ArchiveEntryVector Contents = Resource::GetDirectoryContents(DirPath);
            ArchiveEntryVector::iterator EntIt = std::find_if(Contents.begin(),Contents.end(),[=](const ArchiveEntry& Ent){
                return ( Ent.Name == ActualFileName );
            });
            if( EntIt != Contents.end() ) {
                ArchiveEntryPtr Ret = std::make_unique<ArchiveEntry>(*EntIt);
                return Ret;
            }else{
                return nullptr;
            }
        }

        ArchiveEntryVector FileSystemArchive::GetEntries(const String& Pattern, const Boole OmitDirs) const
        {
            ArchiveEntryVector Ret;
            PopulateDirectoryContentsRecursive(this->ArchiveIdentifier,StringTools::Blank,Pattern,Ret,OmitDirs);
            return Ret;
        }
    }//Resource
}//Mezzanine

#endif
