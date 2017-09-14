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
#ifndef _datastream_cpp
#define _datastream_cpp

#include "datastream.h"
#include "stringtool.h"
#include "exception.h"

#include <cstring>
#include <cstdio>
#include <algorithm>

#define TEMP_STREAM_SIZE 128

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // IStream Methods

    IStream::IStream(std::streambuf* Buf) :
        std::istream(Buf)
        {  }

    IStream::~IStream()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Stream Base Operations

    Boole IStream::EoF() const
        { return this->eof(); }

    Boole IStream::Bad() const
        { return this->bad(); }

    Boole IStream::Fail() const
        { return this->fail(); }

    Boole IStream::IsValid() const
        { return this->good(); }

    void IStream::ClearErrors()
        { this->clear(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Input methods

    size_t IStream::Read(void* Buffer, StreamSize Size)
    {
        this->read(static_cast<char*>(Buffer),Size);
        return this->gcount();
    }

    void IStream::SetReadPosition(StreamPos Position)
        { this->seekg(Position); }

    void IStream::SetReadPosition(StreamOff Offset, SeekOrigin Origin)
        { this->seekg(Offset,static_cast<std::ios_base::seekdir>(Origin)); }

    StreamPos IStream::GetReadPosition()
        { return this->tellg(); }

    ///////////////////////////////////////////////////////////////////////////////
    // OStream Methods

    OStream::OStream(std::streambuf* Buf) :
        std::ostream(Buf)
        {  }

    OStream::~OStream()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Stream Base Operations

    Boole OStream::EoF() const
        { return this->eof(); }

    Boole OStream::Bad() const
        { return this->bad(); }

    Boole OStream::Fail() const
        { return this->fail(); }

    Boole OStream::IsValid() const
        { return this->good(); }

    void OStream::ClearErrors()
        { this->clear(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Output methods

    size_t OStream::Write(const void* Buffer, StreamSize Size)
    {
        this->write(static_cast<const char*>(Buffer),Size);
        return ( this->fail() ? 0 : Size );
    }

    void OStream::SetWritePosition(StreamPos Position)
        { this->seekp(Position); }

    void OStream::SetWritePosition(StreamOff Offset, SeekOrigin Origin)
        { this->seekp(Offset,static_cast<std::ios_base::seekdir>(Origin)); }

    StreamPos OStream::GetWritePosition()
        { return this->tellp(); }

    ///////////////////////////////////////////////////////////////////////////////
    // IOStream Methods

    IOStream::IOStream(std::streambuf* Buf) :
        std::iostream(Buf)
        {  }

    IOStream::~IOStream()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Stream Base Operations

    Boole IOStream::EoF() const
        { return this->eof(); }

    Boole IOStream::Bad() const
        { return this->bad(); }

    Boole IOStream::Fail() const
        { return this->fail(); }

    Boole IOStream::IsValid() const
        { return this->good(); }

    void IOStream::ClearErrors()
        { this->clear(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Input methods

    size_t IOStream::Read(void* Buffer, StreamSize Size)
    {
        this->read(static_cast<char*>(Buffer),Size);
        return this->gcount();
    }

    void IOStream::SetReadPosition(StreamPos Position)
        { this->seekg(Position); }

    void IOStream::SetReadPosition(StreamOff Offset, SeekOrigin Origin)
        { this->seekg(Offset,static_cast<std::ios_base::seekdir>(Origin)); }

    StreamPos IOStream::GetReadPosition()
        { return this->tellg(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Output methods

    size_t IOStream::Write(const void* Buffer, StreamSize Size)
    {
        this->write(static_cast<const char*>(Buffer),Size);
        return ( this->fail() ? 0 : Size );
    }

    void IOStream::SetWritePosition(StreamPos Position)
        { this->seekp(Position); }

    void IOStream::SetWritePosition(StreamOff Offset, SeekOrigin Origin)
        { this->seekp(Offset,static_cast<std::ios_base::seekdir>(Origin)); }

    StreamPos IOStream::GetWritePosition()
        { return this->tellp(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Input/Output methods

    void IOStream::Advance(const StreamOff Count)
        { this->SetStreamPosition(Count,SO_Current); }

    void IOStream::SetStreamPosition(StreamPos Position)
    {
        this->seekg(Position);
        this->seekp(Position);
    }

    void IOStream::SetStreamPosition(StreamOff Offset, SeekOrigin Origin)
    {
        this->seekg(Offset,static_cast<std::ios_base::seekdir>(Origin));
        this->seekp(Offset,static_cast<std::ios_base::seekdir>(Origin));
    }

    StreamPos IOStream::GetStreamPosition(const Boole Read)
    {
        if(Read) return this->GetReadPosition();
        else return this->GetWritePosition();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Formatting Methods

    String IOStream::GetAsString()
    {
        size_t BufferSize = ( this->GetSize() > 0 ? this->GetSize() : 4096 );
        char* Buffer = new char[BufferSize];

        this->SetStreamPosition(0);
        String Ret;
        while( !this->EoF() )
        {
            size_t BytesRead = Read(Buffer,BufferSize);
            Ret.append(Buffer,BytesRead);
        }
        delete[] Buffer;
        return Ret;
    }

    size_t IOStream::ReadLine(Char8* Buffer, size_t MaxCount, const String& Delim)
    {
        Boole TrimCR = false;
        if( Delim.find_first_of('\n') != String::npos ) {
            TrimCR = true;
        }

        char Temp[TEMP_STREAM_SIZE];
        size_t ChunkSize = std::min(MaxCount,(size_t)TEMP_STREAM_SIZE - 1);
        size_t TotalCount = 0;
        size_t ReadCount = 0;

        while( ChunkSize && ( ReadCount = Read(Temp,ChunkSize) ) != 0 )
        {
            Temp[ReadCount] = '\0';
            size_t Pos = std::strcspn(Temp,Delim.c_str());

            if( Pos < ReadCount ) {
                this->Advance((long)(Pos + 1 - ReadCount));
            }

            if( Buffer ) {
                std::memcpy(Buffer + TotalCount,Temp,Pos);
            }
            TotalCount += Pos;

            if( Pos < ReadCount ) {
                if( TrimCR && TotalCount && Buffer[TotalCount - 1] == '\r' ) {
                    --TotalCount;
                }
                break;
            }

            ChunkSize = std::min(MaxCount - TotalCount,(size_t)TEMP_STREAM_SIZE - 1);
        }
        Buffer[TotalCount] = '\0';
        return TotalCount;
    }

    String IOStream::GetLine(Boole Trim)
    {
        char Temp[TEMP_STREAM_SIZE];
        String Ret;
        size_t ReadCount;

        while( ( ReadCount = Read(Temp,TEMP_STREAM_SIZE - 1) ) != 0 )
        {
            Temp[ReadCount] = '\0';

            char* Pos = std::strchr(Temp,'\n');
            if( Pos != 0 ) {
                this->Advance( (long)(Pos + 1 - Temp - ReadCount) );
                *Pos = '\0';
            }

            Ret += Temp;

            if( Pos != 0 ) {
                if( Ret.length() && Ret[Ret.length() - 1] == '\r' ) {
                    Ret.erase(Ret.length() - 1, 1);
                }
                break;
            }
        }

        if( Trim ) {
            StringTools::Trim(Ret);
        }

        return Ret;
    }

    size_t IOStream::SkipLine(const String& Delim)
    {
        char Temp[TEMP_STREAM_SIZE];
        size_t TotalBytes = 0;
        size_t ReadCount = 0;

        while( ( ReadCount = Read(Temp,TEMP_STREAM_SIZE - 1) ) != 0 )
        {
            Temp[ReadCount] = '\0';
            size_t Position = std::strcspn(Temp,Delim.c_str());

            if( Position < ReadCount ) {
                this->Advance( (long)(Position + 1 - ReadCount) );
                TotalBytes += Position + 1;
                break;
            }

            TotalBytes += ReadCount;
        }

        return TotalBytes;
    }
}//Mezzanine

#endif
