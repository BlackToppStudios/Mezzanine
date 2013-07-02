//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _resourcememorystream_h
#define _resourcememorystream_h

#include "Resource/datastream.h"

/// @file
/// @brief Declaration of MemoryStream

namespace Mezzanine
{
    namespace Resource
    {
#ifdef USENEWDATASTREAM
        /// @todo Implement this
#else //USENEWDATASTREAM
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This represents a stream to a buffer in memory.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB MemoryStream : public DataStream
        {
            protected:
                bool FreeBuffer;
                UInt8* BufferStart;
                UInt8* BufferPos;
                UInt8* BufferEnd;
            public:
                /// @brief Buffer creation constructor.
                /// @param BufferSize The size of the buffer to be created.
                /// @param FreeOnClose If true this will delete the memory buffer when the stream is closed.
                /// @param ReadOnly If true, writing operations on this stream will be prohibited.
                MemoryStream(const size_t& BufferSize, bool FreeOnClose = true, bool ReadOnly = false);
                /// @brief Pre-made buffer constructor.
                /// @param Buffer The premade buffer to stream from.
                /// @param BufferSize The size of the buffer to stream to/from.
                /// @param FreeOnClose If true this will delete the memory buffer when the stream is closed.
                /// @param ReadOnly If true, writing operations on this stream will be prohibited.
                MemoryStream(void* Buffer, const size_t& BufferSize, bool FreeOnClose = false, bool ReadOnly = false);
                /// @brief Class destructor.
                virtual ~MemoryStream();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Gets a pointer to the start of the memory buffer used by this stream.
                /// @return Returns a pointer to the start of the memory buffer.
                UInt8* GetBufferStart() const;
                /// @brief Gets a pointer to the current position in the memory buffer used by this stream.
                /// @return Returns a pointer to the current position in the memory buffer.
                UInt8* GetBufferPosition() const;
                /// @brief Gets a pointer to the end of the memory buffer used by this stream.
                /// @return Returns a pointer to the end of the memory buffer.
                UInt8* GetBufferEnd() const;
                /// @brief Sets whether or not you want this stream to free the memory buffer when it closes.
                /// @param True if you want this stream to free the buffer when it closes, false if you want it preserved.
                void SetFreeOnClose(bool FreeOnClose);

                ///////////////////////////////////////////////////////////////////////////////
                // Stream Access and Manipulation

                /// @copydoc DataStream::Read(void* Buffer, const size_t& Count)
                virtual size_t Read(void* Buffer, const size_t& Count);
                /// @copydoc DataStream::Write(const void* Buffer, const size_t& Count)
                virtual size_t Write(const void* Buffer, const size_t& Count);
                /// @copydoc DataStream::Advance(const StreamOff Count)
                virtual void Advance(const StreamOff Count);
                /// @copydoc DataStream::SetStreamPosition(StreamPos Position)
                virtual void SetStreamPosition(StreamPos Position);
                /// @copydoc DataStream::SetStreamPosition(StreamOff Offset, SeekOrigin Origin)
                virtual void SetStreamPosition(StreamOff Offset, SeekOrigin Origin);
                /// @copydoc DataStream::GetStreamPosition(bool Read = true)
                virtual StreamPos GetStreamPosition(bool Read = true);
                /// @copydoc DataStream::EoF() const
                virtual bool EoF() const;
                /// @copydoc DataStream::Close()
                virtual void Close();

                ///////////////////////////////////////////////////////////////////////////////
                // Formatting Methods

                /// @copydoc DataStream::ReadLine(Char8* Buffer, size_t MaxCount, const String& Delim = "\n")
                virtual size_t ReadLine(Char8* Buffer, size_t MaxCount, const String& Delim = "\n");
                /// @copydoc DataStream::SkipLine(const String& Delim = "\n")
                virtual size_t SkipLine(const String& Delim = "\n");
        };//MemoryStream
#endif //USENEWDATASTREAM
    }//Resource
}//Mezzanine

#endif
