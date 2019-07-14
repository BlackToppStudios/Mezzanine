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
#ifndef _filestream_cpp
#define _filestream_cpp

#include "filestream.h"
#include "stringtool.h"
#include "exception.h"

#include <fstream>
#include <algorithm>

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // FileIStream Methods

    FileIStream::FileIStream() :
        IStream(&this->FileBuffer)
        {  }

    FileIStream::FileIStream(const String& Identifier, const Whole SplitIdx, const Whole Flags) :
        IStream(&this->FileBuffer)
        { this->OpenFile(Identifier,SplitIdx,Flags); }

    FileIStream::FileIStream(const String& Identifier, const String& Group, const Whole SplitIdx, const Whole Flags) :
        IStream(&this->FileBuffer)
        { this->OpenFile(Identifier,Group,SplitIdx,Flags); }

    FileIStream::~FileIStream()
        { if( this->IsOpenToFile() ) this->CloseFile(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility Methods

    void FileIStream::OpenFile(const String& Identifier, const Whole SplitIdx, const Whole Flags)
    {
        const Whole FilteredFlags = Flags & ~Mezzanine::SF_Write;
        if( !this->FileBuffer.open(Identifier.c_str(),static_cast<const std::ios_base::openmode>(FilteredFlags)) ) {
            this->setstate(std::ios_base::failbit);
        }else{
            this->clear();
        }

        if( !this->IsOpenToFile() ) {
            StringStream ExceptionStream;
            ExceptionStream << "Unable to create or locate file \"" << Identifier << "\".";
            MEZZ_EXCEPTION(ExceptionBase::IO_FILE_NOT_FOUND_EXCEPTION,ExceptionStream.str());
        }

        this->OpenFileName = Identifier;
        this->SetReadPosition(0,Mezzanine::SO_End);
        this->Size = static_cast<StreamSize>( this->GetReadPosition() );
        this->SetReadPosition(0,Mezzanine::SO_Beginning);
        this->StreamFlags = FilteredFlags;
        this->ArchiveEndIndex = SplitIdx;
    }

    void FileIStream::OpenFile(const String& Identifier, const String& Group, const Whole SplitIdx, const Whole Flags)
    {
        this->OpenFile(Identifier,SplitIdx,Flags);
        this->OpenFileGroup = Group;
    }

    Boole FileIStream::IsOpenToFile() const
    {
        return this->FileBuffer.is_open();
    }

    void FileIStream::CloseFile()
    {
        if( this->IsOpenToFile() ) {
            this->OpenFileName.clear();
            this->OpenFileGroup.clear();
            this->ArchiveEndIndex = 0;
            this->StreamFlags = Mezzanine::SF_None;

            if( !this->FileBuffer.close() ) {
                this->setstate(std::ios_base::failbit);
            }
        }
    }

    const String& FileIStream::GetFullIdentifier() const
        { return this->OpenFileName; }

    Whole FileIStream::GetStreamFlags() const
        { return this->StreamFlags; }

    ///////////////////////////////////////////////////////////////////////////////
    // StreamBase Methods

    String FileIStream::GetIdentifier() const
        { return this->OpenFileName.substr(this->ArchiveEndIndex); }

    String FileIStream::GetGroup() const
        { return this->OpenFileGroup; }

    StreamSize FileIStream::GetSize() const
        { return this->Size; }

    Boole FileIStream::CanSeek() const
        { return true; }

    Boole FileIStream::IsEncrypted() const
        { return false; } // We don't yet support encryption on raw filesystems. May never.

    Boole FileIStream::IsRaw() const
        { return true; }

    ///////////////////////////////////////////////////////////////////////////////
    // FileOStream Methods

    FileOStream::FileOStream() :
        OStream(&this->FileBuffer)
        {  }

    FileOStream::FileOStream(const String& Identifier, const Whole SplitIdx, const Whole Flags) :
        OStream(&this->FileBuffer)
        { this->OpenFile(Identifier,SplitIdx,Flags); }

    FileOStream::FileOStream(const String& Identifier, const String& Group, const Whole SplitIdx, const Whole Flags) :
        OStream(&this->FileBuffer)
        { this->OpenFile(Identifier,Group,SplitIdx,Flags); }

    FileOStream::~FileOStream()
        { if( this->IsOpenToFile() ) this->CloseFile(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility Methods

    void FileOStream::OpenFile(const String& Identifier, const Whole SplitIdx, const Whole Flags)
    {
        const Whole FilteredFlags = Flags & ~Mezzanine::SF_Read;
        if( !this->FileBuffer.open(Identifier.c_str(),static_cast<const std::ios_base::openmode>(FilteredFlags)) ) {
            this->setstate(std::ios_base::failbit);
        }else{
            this->clear();
        }

        if( !this->IsOpenToFile() ) {
            StringStream ExceptionStream;
            ExceptionStream << "Unable to create or locate file \"" << Identifier << "\".";
            MEZZ_EXCEPTION(ExceptionBase::IO_FILE_NOT_FOUND_EXCEPTION,ExceptionStream.str());
        }

        this->OpenFileName = Identifier;
        this->SetWritePosition(0,Mezzanine::SO_End);
        this->Size = static_cast<StreamSize>( this->GetWritePosition() );
        this->SetWritePosition(0,Mezzanine::SO_Beginning);
        this->StreamFlags = FilteredFlags;
        this->ArchiveEndIndex = SplitIdx;
    }

    void FileOStream::OpenFile(const String& Identifier, const String& Group, const Whole SplitIdx, const Whole Flags)
    {
        this->OpenFile(Identifier,SplitIdx,Flags);
        this->OpenFileGroup = Group;
    }

    Boole FileOStream::IsOpenToFile() const
    {
        return this->FileBuffer.is_open();
    }

    void FileOStream::CloseFile()
    {
        if( this->IsOpenToFile() ) {
            this->OpenFileName.clear();
            this->OpenFileGroup.clear();
            this->ArchiveEndIndex = 0;
            this->StreamFlags = Mezzanine::SF_None;

            if( !this->FileBuffer.close() ) {
                this->setstate(std::ios_base::failbit);
            }
        }
    }

    const String& FileOStream::GetFullIdentifier() const
        { return this->OpenFileName; }

    Whole FileOStream::GetStreamFlags() const
        { return this->StreamFlags; }

    ///////////////////////////////////////////////////////////////////////////////
    // StreamBase Methods

    String FileOStream::GetIdentifier() const
        { return this->OpenFileName.substr(this->ArchiveEndIndex); }

    String FileOStream::GetGroup() const
        { return this->OpenFileGroup; }

    StreamSize FileOStream::GetSize() const
        { return this->Size; }

    Boole FileOStream::CanSeek() const
        { return true; }

    Boole FileOStream::IsEncrypted() const
        { return false; } // We don't yet support encryption on raw filesystems. May never.

    Boole FileOStream::IsRaw() const
        { return true; }

    ///////////////////////////////////////////////////////////////////////////////
    // FileStream Methods

    FileStream::FileStream() :
        IOStream(&this->FileBuffer)
        {  }

    FileStream::FileStream(const String& Identifier, const Whole SplitIdx, const Whole Flags) :
        IOStream(&this->FileBuffer)
        { this->OpenFile(Identifier,SplitIdx,Flags); }

    FileStream::FileStream(const String& Identifier, const String& Group, const Whole SplitIdx, const Whole Flags) :
        IOStream(&this->FileBuffer)
        { this->OpenFile(Identifier,Group,SplitIdx,Flags); }

    FileStream::~FileStream()
        { if( this->IsOpenToFile() ) this->CloseFile(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility Methods

    void FileStream::OpenFile(const String& Identifier, const Whole SplitIdx, const Whole Flags)
    {
        if( !this->FileBuffer.open(Identifier.c_str(),static_cast<const std::ios_base::openmode>(Flags)) ) {
            this->setstate(std::ios_base::failbit);
        }else{
            this->clear();
        }

        if( !this->IsOpenToFile() ) {
            StringStream ExceptionStream;
            ExceptionStream << "Unable to create or locate file \"" << Identifier << "\".";
            MEZZ_EXCEPTION(ExceptionBase::IO_FILE_NOT_FOUND_EXCEPTION,ExceptionStream.str());
        }

        this->OpenFileName = Identifier;
        this->SetReadPosition(0,Mezzanine::SO_End);
        this->Size = static_cast<StreamSize>( this->GetReadPosition() );
        this->SetStreamPosition(0);
        this->StreamFlags = Flags;
        this->ArchiveEndIndex = SplitIdx;
    }

    void FileStream::OpenFile(const String& Identifier, const String& Group, const Whole SplitIdx, const Whole Flags)
    {
        this->OpenFile(Identifier,SplitIdx,Flags);
        this->OpenFileGroup = Group;
    }

    Boole FileStream::IsOpenToFile() const
    {
        return this->FileBuffer.is_open();
    }

    void FileStream::CloseFile()
    {
        if( this->IsOpenToFile() ) {
            this->OpenFileName.clear();
            this->OpenFileGroup.clear();
            this->ArchiveEndIndex = 0;
            this->StreamFlags = Mezzanine::SF_None;

            if( !this->FileBuffer.close() ) {
                this->setstate(std::ios_base::failbit);
            }
        }
    }

    const String& FileStream::GetFullIdentifier() const
        { return this->OpenFileName; }

    Whole FileStream::GetStreamFlags() const
        { return this->StreamFlags; }

    ///////////////////////////////////////////////////////////////////////////////
    // StreamBase Methods

    String FileStream::GetIdentifier() const
        { return this->OpenFileName.substr(this->ArchiveEndIndex); }

    String FileStream::GetGroup() const
        { return this->OpenFileGroup; }

    StreamSize FileStream::GetSize() const
        { return this->Size; }

    Boole FileStream::CanSeek() const
        { return true; }

    Boole FileStream::IsEncrypted() const
        { return false; } // We don't yet support encryption on raw filesystems. May never.

    Boole FileStream::IsRaw() const
        { return true; }
}//Mezzanine

#endif
