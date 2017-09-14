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
#ifndef _memorystream_cpp
#define _memorystream_cpp

#include "memorystream.h"
#include "stringtool.h"
#include "exception.h"

#include <cstring>
#include <cassert>
#include <algorithm>

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // MemoryStreamBuffer Methods

    MemoryStreamBuffer::MemoryStreamBuffer() :
        BufferStart(NULL),
        /*BufferReadPos(NULL),
        BufferWritePos(NULL),// */
        BufferEnd(NULL),
        FreeBuffer(true)
        {  }

    MemoryStreamBuffer::~MemoryStreamBuffer()
        { this->DestroyBuffer(); }

    std::streampos MemoryStreamBuffer::CheckStreamPosition(const std::streampos Position)
    {
        std::streampos BufferSize = this->BufferEnd - this->BufferStart;
        if( Position < 0 ) {
            return 0;
        }
        if( Position > BufferSize ) {
            return BufferSize;
        }
        return Position;
    }

    std::streamoff MemoryStreamBuffer::CheckStreamOffset(const std::streamoff CursorOffset, const std::streamoff Adjust)
    {
        std::streampos BufferSize = this->BufferEnd - this->BufferStart;
        if( CursorOffset + Adjust < 0 ) {
            return -CursorOffset;
        }
        if( CursorOffset + Adjust > BufferSize ) {
            return BufferSize - CursorOffset;
        }
        return Adjust;
    }

    std::streampos MemoryStreamBuffer::GetReadPosition() const
        { return static_cast<std::streampos>( this->gptr() - this->eback() ); }

    std::streampos MemoryStreamBuffer::GetWritePosition() const
        { return static_cast<std::streampos>( this->pptr() - this->pbase() ); }

    ///////////////////////////////////////////////////////////////////////////////
    // Locales

    void MemoryStreamBuffer::imbue(const std::locale& loc)
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Buffer Management and Positioning

    std::streambuf* MemoryStreamBuffer::setbuf(char* s, std::streamsize n)
    {
        if( s != NULL && n > 0 ) {
            this->DestroyBuffer();

            this->BufferStart = s;
            this->BufferEnd = this->BufferStart + n;
            /*this->BufferReadPos = this->BufferStart;
            this->BufferWritePos = this->BufferStart;// */
            this->setg(this->BufferStart,this->BufferStart,this->BufferEnd);
            this->setp(this->BufferStart,this->BufferEnd);// */
            return this;
        }
        return NULL;
    }

    std::streampos MemoryStreamBuffer::seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which)
    {
        std::streampos BufferSize = this->BufferEnd - this->BufferStart;
        // Read
        if( which & std::ios_base::in ) {
            std::streampos ReadPosition = this->GetReadPosition();
            switch( way )
            {
                case std::ios_base::beg:  this->gbump( this->CheckStreamPosition( off ) - ReadPosition );                break;
                case std::ios_base::end:  this->gbump( this->CheckStreamPosition( BufferSize + off ) - ReadPosition );   break;
                case std::ios_base::cur:
                default:                  this->gbump( this->CheckStreamOffset(ReadPosition,off) );                      break;
            }
        }
        // Write
        if( which & std::ios_base::out ) {
            std::streampos WritePosition = this->GetWritePosition();
            switch( way )
            {
                case std::ios_base::beg:  this->pbump( this->CheckStreamPosition( off ) - WritePosition );               break;
                case std::ios_base::end:  this->pbump( this->CheckStreamPosition( BufferSize + off ) - WritePosition );  break;
                case std::ios_base::cur:
                default:                  this->pbump( this->CheckStreamOffset(WritePosition,off) );                     break;
            }
        }
        return ( which & std::ios_base::in ? this->GetReadPosition() : this->GetWritePosition() );
    }

    std::streampos MemoryStreamBuffer::seekpos(std::streampos sp, std::ios_base::openmode which)
    {
        // Read
        if( which & std::ios_base::in ) {
            std::streampos ReadPosition = this->GetReadPosition();
            this->gbump( this->CheckStreamPosition( sp ) - ReadPosition );
        }
        // Write
        if( which & std::ios_base::out ) {
            std::streampos WritePosition = this->GetWritePosition();
            this->pbump( this->CheckStreamPosition( sp ) - WritePosition );
        }
        return ( which & std::ios_base::in ? this->GetReadPosition() : this->GetWritePosition() );
    }

    int MemoryStreamBuffer::sync()
        { return 0; }

    ///////////////////////////////////////////////////////////////////////////////
    // Input Functions (get/read)

    std::streamsize MemoryStreamBuffer::showmanyc()
    {
        if( this->BufferStart != NULL ) {
            assert( std::less_equal<Char8*>()(this->gptr(),this->egptr()) );
            //return this->BufferEnd - this->BufferReadPos;
            return this->egptr() - this->gptr();
        }
        return 0;
    }

    std::streamsize MemoryStreamBuffer::xsgetn(char* s, std::streamsize n)
    {
        if( this->OpenMode & std::ios_base::in ) {
            return std::streambuf::xsgetn(s,n);
        }
        return 0;
    }

    int MemoryStreamBuffer::underflow()
    {
        if( this->gptr() == this->egptr() ) {
        //if( this->BufferReadPos == this->BufferEnd ) {
            return traits_type::eof();
        }

        return traits_type::to_int_type( *(this->egptr()) );
    }

    int MemoryStreamBuffer::uflow()
    {
        return std::streambuf::uflow();
    }

    int MemoryStreamBuffer::pbackfail(int c)
    {
        //if( this->BufferReadPos == this->BufferStart || ( c != traits_type::eof() && c != ( this->BufferReadPos - 1 ) ) ) {
        if( this->gptr() == this->eback() || ( c != traits_type::eof() && c != *( this->gptr() - 1 ) ) ) {
            return traits_type::eof();
        }

        //return traits_type::to_int_type( *(--this->BufferReadPos) );
        this->gbump(-1);
        return this->sgetc();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Output Functions (put/write)

    std::streamsize MemoryStreamBuffer::xsputn(const char* s, std::streamsize n)
    {
        if( this->OpenMode & std::ios_base::out ) {
            return std::streambuf::xsputn(s,n);
        }
        return 0;
    }

    int MemoryStreamBuffer::overflow(int c)
    {
        //if( this->pptr() == this->epptr() )
        return traits_type::eof();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void MemoryStreamBuffer::CreateBuffer(const Whole Size)
    {
        if( Size > 0 ) {
            this->setbuf( new Char8[Size], Size );
            this->FreeBuffer = true;
        }
    }

    void MemoryStreamBuffer::SetBuffer(char* Buffer, const Whole BufferSize, const Boole FreeBuf)
    {
        if( this->setbuf(Buffer,BufferSize) ) {
            this->ConfigureBuffer(0);
            this->FreeBuffer = FreeBuf;
        }
    }

    void MemoryStreamBuffer::CopyBuffer(const char* Buffer, const Whole BufferSize)
    {
        char* NewBuf = new char[BufferSize];
        memcpy(NewBuf,Buffer,BufferSize);
        this->SetBuffer(NewBuf,BufferSize,true);
    }

    Char8* MemoryStreamBuffer::GetBufferStart() const
    {
        // Gonna use the std api for consistency, but could return raw members as well
        return this->eback();
    }

    Char8* MemoryStreamBuffer::GetBufferEnd() const
    {
        // Gonna use the std api for consistency, but could return raw members as well
        return this->egptr();
    }

    std::streamsize MemoryStreamBuffer::GetBufferSize() const
    {
        return this->egptr() - this->eback();
    }

    void MemoryStreamBuffer::ConfigureBuffer(const std::streampos Pos, std::ios_base::openmode Mode)
    {
        if( this->BufferStart != NULL ) {
            if( Mode & std::ios_base::in ) {
                //this->BufferReadPos = this->BufferStart + Pos;
                this->setg(this->BufferStart,this->BufferStart + Pos,this->BufferEnd);
            }
            if( Mode & std::ios_base::out ) {
                //this->BufferWritePos = this->BufferStart + Pos;
                this->setp(this->BufferStart,this->BufferEnd);
            }
            this->OpenMode = Mode;
            this->seekpos(Pos,Mode);
        }
    }

    void MemoryStreamBuffer::DestroyBuffer()
    {
        if( this->BufferStart != NULL && this->FreeBuffer ) {
            delete[] this->BufferStart;
            this->BufferStart = NULL;
            /*this->BufferReadPos = NULL;
            this->BufferWritePos = NULL;// */
            this->BufferEnd = NULL;
        }
    }

    void MemoryStreamBuffer::SetFreeBuffer(const Boole FreeBuf)
        { this->FreeBuffer = FreeBuf; }

    Boole MemoryStreamBuffer::GetFreeBuffer() const
        { return this->FreeBuffer; }

    ///////////////////////////////////////////////////////////////////////////////
    // MemoryStream Methods

    MemoryStream::MemoryStream() :
        IOStream(&this->Buffer)
        { /*this->init(&this->Buffer);*/ }

    MemoryStream::MemoryStream(const Whole BufferSize, const Whole Mode) :
        IOStream(&this->Buffer)
        { /*this->init(&this->Buffer);*/  this->CreateBuffer(BufferSize,Mode); }

    MemoryStream::MemoryStream(const void* Buffer, const Whole BufferSize, const Whole Mode) :
        IOStream(&this->Buffer)
        { /*this->init(&this->Buffer);*/  this->CopyBuffer(Buffer,BufferSize,Mode); }

    MemoryStream::MemoryStream(void* Buffer, const Whole BufferSize, const Boole FreeAfter, const Whole Mode) :
        IOStream(&this->Buffer)
        { /*this->init(&this->Buffer);*/  this->SetBuffer(Buffer,BufferSize,FreeAfter,Mode); }

    MemoryStream::~MemoryStream()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void MemoryStream::CreateBuffer(const Whole Size, const Whole Mode)
        { this->Buffer.CreateBuffer(Size);  this->Buffer.ConfigureBuffer(0,static_cast<std::ios_base::openmode>(Mode)); }

    void MemoryStream::SetBuffer(void* Buffer, const Whole BufferSize, const Boole FreeBuf, const Whole Mode)
        { this->Buffer.SetBuffer(static_cast<char*>(Buffer),BufferSize,FreeBuf);  this->Buffer.ConfigureBuffer(0,static_cast<std::ios_base::openmode>(Mode)); }

    void MemoryStream::CopyBuffer(const void* Buffer, const Whole BufferSize, const Whole Mode)
        { this->Buffer.CopyBuffer(static_cast<const char*>(Buffer),BufferSize);  this->Buffer.ConfigureBuffer(0,static_cast<std::ios_base::openmode>(Mode)); }

    Char8* MemoryStream::GetBufferStart() const
        { return this->Buffer.GetBufferStart(); }

    Char8* MemoryStream::GetBufferEnd() const
        { return this->Buffer.GetBufferEnd(); }

    void MemoryStream::SetFreeBuffer(const Boole FreeBuf)
        { this->Buffer.SetFreeBuffer(FreeBuf); }

    Boole MemoryStream::GetFreeBuffer() const
        { return this->Buffer.GetFreeBuffer(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Stream Base Operations

    StreamSize MemoryStream::GetSize() const
        { return this->Buffer.GetBufferSize(); }
}//Mezzanine

#endif
