// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _resourcememorystream_cpp
#define _resourcememorystream_cpp

#include "Resource/memorystream.h"
#include "stringtool.h"
#include "exception.h"

#include <cstring>
#include <algorithm>

namespace Mezzanine
{
    namespace Resource
    {
#ifdef USENEWDATASTREAM
        /// @todo Implement this
#else //USENEWDATASTREAM
        MemoryStream::MemoryStream(const size_t& BufferSize, Boolean FreeOnClose, Boolean ReadOnly)
            : DataStream( ReadOnly ? DataStream::SF_Read : static_cast<DataStream::StreamFlags>(DataStream::SF_Read | DataStream::SF_Write) ),
              FreeBuffer(FreeOnClose)
        {
            Size = BufferSize;
            BufferStart = new UInt8[BufferSize];
            BufferPos = BufferStart;
            BufferEnd = BufferStart + BufferSize;

            if(BufferEnd <= BufferStart)
                MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Using a zero or negative size buffer");
        }

        MemoryStream::MemoryStream(void* Buffer, const size_t& BufferSize, Boolean FreeOnClose, Boolean ReadOnly)
            : DataStream( ReadOnly ? DataStream::SF_Read : static_cast<DataStream::StreamFlags>(DataStream::SF_Read | DataStream::SF_Write) ),
              FreeBuffer(FreeOnClose)
        {
            BufferStart = BufferPos = static_cast<UInt8*>(Buffer);
            Size = BufferSize;
            BufferEnd = BufferStart + BufferSize;

            if(BufferEnd <= BufferStart)
                MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Using a zero or negative size buffer");
        }

        MemoryStream::~MemoryStream()
        {
            Close();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        UInt8* MemoryStream::GetBufferStart() const
        {
            return BufferStart;
        }

        UInt8* MemoryStream::GetBufferPosition() const
        {
            return BufferPos;
        }

        UInt8* MemoryStream::GetBufferEnd() const
        {
            return BufferEnd;
        }

        void MemoryStream::SetFreeOnClose(Boolean FreeOnClose)
        {
            FreeBuffer = FreeOnClose;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Access and Manipulation

        size_t MemoryStream::Read(void* Buffer, const size_t& Count)
        {
            size_t RetCount = Count;
            if(BufferPos + RetCount > BufferEnd)
                RetCount = BufferEnd - BufferPos;
            if(RetCount == 0)
                return 0;

            //if(RetCount > Count);
            //    MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Cannot read passed end of stream");

            std::memcpy(Buffer,BufferPos,RetCount);
            BufferPos += RetCount;
            return RetCount;
        }

        size_t MemoryStream::Write(const void* Buffer, const size_t& Count)
        {
            size_t Written = 0;
            if(IsWriteable())
            {
                Written = Count;
                if(BufferPos + Written > BufferEnd)
                    Written = BufferEnd - BufferPos;
                if(Written == 0)
                    return 0;

                std::memcpy(BufferPos,Buffer,Written);
                BufferPos += Written;
            }
            return Written;
        }

        void MemoryStream::Advance(const StreamOff Count)
        {
            size_t NewPos = (size_t)( ( BufferPos - BufferStart ) + Count );
            if( BufferStart + NewPos > BufferEnd )
                NewPos = Size;

            BufferPos = BufferStart + NewPos;
        }

        void MemoryStream::SetStreamPosition(StreamPos Position)
        {
            if( BufferStart + Position > BufferEnd || Position < 0 )
                MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Attempting to set position of stream to area outside the bounds of the buffer");

            BufferPos = BufferStart + Position;
        }

        void MemoryStream::SetStreamPosition(StreamOff Offset, SeekOrigin Origin)
        {
            switch(Origin)
            {
                case SO_Beginning:
                {
                    this->SetStreamPosition(Offset);
                    break;
                }
                case SO_Current:
                {
                    if( GetStreamPosition() + Offset < 0 || GetStreamPosition() + Offset >= Size )
                        MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Attempting to set position of stream to area outside the bounds of the buffer");

                    BufferPos = BufferStart + (GetStreamPosition() + Offset);
                    break;
                }
                case SO_End:
                {
                    if(Offset > 0 || Offset <= -Size)
                        MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Attempting to set position of stream to area outside the bounds of the buffer");

                    BufferPos = BufferStart + ((Size - 1) + Offset);
                    break;
                }
            }
        }

        StreamPos MemoryStream::GetStreamPosition(Boolean Read)
        {
            return BufferPos - BufferStart;
        }

        Boolean MemoryStream::EoF() const
        {
            return BufferPos >= BufferEnd;
        }

        void MemoryStream::Close()
        {
            if(FreeBuffer && BufferStart)
            {
                delete[] BufferStart;
                BufferStart = NULL;
                BufferPos = NULL;
                BufferEnd = NULL;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Formatting Methods

        size_t MemoryStream::ReadLine(Char8* Buffer, size_t MaxCount, const String& Delim)
        {
            Boolean TrimCR = false;
            if(Delim.find_first_of('\n') != String::npos)
            {
                TrimCR = true;
            }

            size_t BytesRead = 0;
            while(BytesRead < MaxCount && BufferPos < BufferEnd)
            {
                if(Delim.find(*BufferPos) != String::npos)
                {
                    if(TrimCR && BytesRead && Buffer[BytesRead - 1] == '\r')
                    {
                        --BytesRead;
                    }

                    ++BufferPos;
                    break;
                }
                Buffer[BytesRead++] = *BufferPos++;
            }
            Buffer[BytesRead] = '\0';
            return BytesRead;
        }

        size_t MemoryStream::SkipLine(const String& Delim)
        {
            size_t BytesSkipped = 0;
            while(BufferPos < BufferEnd)
            {
                ++BytesSkipped;
                if(Delim.find(*BufferPos++) != String::npos)
                {
                    break;
                }
            }
            return BytesSkipped;
        }
#endif //USENEWDATASTREAM
    }//Resource
}//Mezzanine

#endif
