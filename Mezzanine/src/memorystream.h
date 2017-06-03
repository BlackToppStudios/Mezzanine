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
#ifndef _memorystream_h
#define _memorystream_h

#include "datastream.h"

/// @file
/// @brief Declaration of MemoryStream

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A stream buffer object to a chunk of memory.
    ///////////////////////////////////////
    class MEZZ_LIB MemoryStreamBuffer : public std::streambuf
    {
    protected:
        /// @internal
        /// @brief A pointer to the start of this memory buffer.
        Char8* BufferStart;
        /*/// @internal
        /// @brief The current position for read operations on this buffer.
        Char8* BufferReadPos;
        /// @internal
        /// @brief The current position for write operations on this buffer.
        Char8* BufferWritePos;// */
        /// @internal
        /// @brief A pointer to one passed the last element in the range of this buffer.
        Char8* BufferEnd;
        /// @internal
        /// @brief A bitfield describing the settings this buffer was opened with.
        Whole OpenMode;
        /// @internal
        /// @brief Stores whether or not the memory allocated will be free'd when this buffer is deleted.
        Boole FreeBuffer;

        /// @brief Verifies a position in this stream is valid.
        /// @param Position The position in this stream to check.
        /// @return Returns an std::streampos containing the clamped position for this stream.
        std::streampos CheckStreamPosition(const std::streampos Position);
        /// @brief Verifies an offset in this stream is valid for a read or write cursor.
        /// @param CursorOffset The current read/write position in this stream.
        /// @param Adjust The amount of offset to be checked/applied to the provided read/write position.
        /// @return Returns an std::streamoff containing the clamped offset for the given cursor in this stream.
        std::streamoff CheckStreamOffset(const std::streamoff CursorOffset, const std::streamoff Adjust);
        /// @brief Gets the current position of the read cursor.
        /// @return Returns a std::streampos representing the offset of the read position from the start of the stream.
        std::streampos GetReadPosition() const;
        /// @brief Gets the current position of the write cursor.
        /// @return Returns a std::streampos representing the offset of the write position from the start of the stream.
        std::streampos GetWritePosition() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Locales

        /// @brief Notifies this class that a new locale is being imbued to this streambuf.
        /// @remarks Does nothing, implemented for ease of future coding.
        /// @param loc The new locale being imbued.
        virtual void imbue(const std::locale& loc);

        ///////////////////////////////////////////////////////////////////////////////
        // Buffer Management and Positioning

        /// @brief Sets the buffer to be streamed from.
        /// @param s The buffer to be wrapped.
        /// @param n The size of the buffer being wrapped.
        /// @return Returns this on success, or NULL if there is a failure.
        virtual std::streambuf* setbuf(char* s, std::streamsize n);
        /// @brief Moves the read and/or write pointers in the stream.
        /// @remarks Out of bounds positions will be clamped to the closest limit.
        /// @param off The amount of advance the current position for the operation specified.  Negative values will reverse the position.
        /// @param way The base point in the stream to apply the offset to.
        /// @param which Whether this should move the read pointer, write pointer, or both.
        /// @return Returns the updated read or write position, based on which was provided.  Returns the Read position if both were specified.
        virtual std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
        /// @brief Moves the read and/or write pointers to the specified position in the stream.
        /// @remarks Out of bounds positions will be clamped to the closest limit.
        /// @param sp The index the current position will be moved to.
        /// @param which Whether this should move the read pointer, write pointer, or both.
        /// @return Returns the updated read or write position, based on which was provided.  Returns the Read position if both were specified.
        virtual std::streampos seekpos(std::streampos sp, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
        /// @brief Syncs this stream buffer with it's source.
        /// @remarks This buffer is the source for this type of stream, so syncing isn't required; making this an empty implementation.
        virtual int sync();

        ///////////////////////////////////////////////////////////////////////////////
        // Input Functions (get/read)

        /// @brief Gets the number of characters remaining that can be safely extracted from this buffer.
        /// @return Returns either the number of characters between the current read position and the end of the buffer, or -1 if no buffer is allocated.
        virtual std::streamsize showmanyc();
        /// @brief Gets a number of characters starting at the read position and populates an array with them.
        /// @param s A pointer to the array where the read characters will be placed.
        /// @param n The number of characters to read.
        /// @return This returns the amount of characters retrieved
        virtual std::streamsize xsgetn(char* s, std::streamsize n);
        /// @brief Gets the value at the current read position without incrementing the read position.
        /// @remarks This method us used by the std::streambuf implementation for some limited amount of error checking for when the end of a stream is
        /// reached and it may be possible to access more (such as streaming a file from disk).  This functionality doesn't make sense for a buffer
        /// located entirely in memory, so we must return the End-Of-File value each time we're at the end of the buffer.
        /// @return Returns the current character at the read position or EOF.
        virtual int underflow();
        /// @brief Gets the value at the current read position and then increments the read position.
        /// @remarks Under normal circumstances this method will be used along with underflow to get characters at the end of the sequence safely in the
        /// event that more data needs to be buffered.  However the spec requires it to always return EOF if underflow also returns EOF.  So because
        /// of how we have to implement underflow, this methods behavior is quite limited.
        /// @return Returns EOF, always.
        virtual int uflow();
        /// @brief Reverse the last character extraction operation by decrementing the read position and placing the specified character in it's place.
        /// @param c The character to place into the stream after decrementing the read position.
        /// @return Returns the current character at the read position after the operation completes, or EOF in the event of a failure.
        virtual int pbackfail(int c = traits_type::eof());

        ///////////////////////////////////////////////////////////////////////////////
        // Output Functions (put/write)

        /// @brief Writes a number of characters from a provided buffer to the stream.
        /// @param s The buffer of characters that will be written to the stream.
        /// @param n The number of characters from the provided buffer that will be written to the stream.
        /// @return Returns the number of characters successfully written.
        virtual std::streamsize xsputn(const char* s, std::streamsize n);
        /// @brief Writes a character at the current write position without advancing the write position.
        /// @remarks Generally this method is called when the available writing positions in the stream have been depleted and additional writing spaces can
        /// made.  In our case, we can't buffer additional data so in the event the stream is expecting more, we should return the error result of EOF.
        /// @param c The character to be written to the stream.
        /// @return Returns the written character, or EOF if there was a failure.
        virtual int overflow(int c = traits_type::eof());
    public:
        /// @brief Class constructor.
        MemoryStreamBuffer();
        /// @brief Class destructor.
        virtual ~MemoryStreamBuffer();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Convenience method for creating a new buffer to be used by this stream.
        /// @remarks No changes will be made if the Size is set to 0 or less.
        /// @param Size The size of the buffer to be allocated.
        /// @param Mode An open mode describing if this buffer will be configured for reading, writing, or both.
        void CreateBuffer(const Whole Size);
        /// @brief Wraps an external buffer for streaming.
        /// @param Buffer The buffer to be wrapped.
        /// @param BufferSize The size of the buffer being wrapped.
        /// @param FreeBuf Whether or not the buffer passed in should be deleted when this stream is destroyed.
        void SetBuffer(char* Buffer, const Whole BufferSize, const Boole FreeBuf);
        /// @brief Copies a pre-existing buffer to this buffer for streaming.
        /// @param Buffer The buffer to be copied.
        /// @param BufferSize The size of the buffer to be copied.
        void CopyBuffer(const char* Buffer, const Whole BufferSize);

        /// @brief Gets the start of this buffer.
        /// @return Returns a pointer to the first valid element in this buffer.
        Char8* GetBufferStart() const;
        /// @brief Gets the end of this buffer.
        /// @return Returns a pointer to one passed the last valid element in this buffer.
        Char8* GetBufferEnd() const;
        /// @brief Gets the size of the current buffer being streamed to/from.
        /// @return Returns a std::streamsize representing the size of the current buffer, or 0 if one hasn't been initialized.
        std::streamsize GetBufferSize() const;

        /// @brief Sets the get and put pointers for the streambuffer and assigns the current position to the specified offset.
        /// @param Pos The offset to set the current read/write position to.
        /// @param Mode An open mode describing if this buffer will be configured for reading, writing, or both.
        void ConfigureBuffer(const std::streampos Pos, std::ios_base::openmode Mode = std::ios_base::in | std::ios_base::out);
        /// @brief Convenience method for deleting the existing buffer being streamed.
        void DestroyBuffer();

        /// @brief Sets if this should delete it's internal buffer when it is destroyed.
        /// @param FreeBuf Whether or not the buffer passed in should be deleted when this stream is destroyed.
        void SetFreeBuffer(const Boole FreeBuf);
        /// @brief Gets if this should delete it's internal buffer when it is destroyed.
        /// @return Returns true if the internal buffer will be deleted when this is destroyed.
        Boole GetFreeBuffer() const;
    };//MemoryStreamBuffer

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief An I/O stream to a buffer of memory.
    ///////////////////////////////////////
    class MEZZ_LIB MemoryStream : public Mezzanine::IOStream
    {
    protected:
        /// @internal
        /// @brief The buffer object being streamed to/from.
        MemoryStreamBuffer Buffer;
    public:
        /// @brief Blank constructor.
        MemoryStream();
        /// @brief Buffer initialization constructor.
        /// @param BufferSize The size of the buffer being wrapped.
        /// @param Mode The configuration to open the memory buffer with.
        MemoryStream(const Whole BufferSize, const Whole Mode = Mezzanine::SF_Read | Mezzanine::SF_Write);
        /// @brief Buffer copy constructor.
        /// @param BufferSize The size of the buffer being wrapped.
        /// @param Mode The configuration to open the memory buffer with.
        MemoryStream(const void* Buffer, const Whole BufferSize, const Whole Mode = Mezzanine::SF_Read | Mezzanine::SF_Write);
        /// @brief Buffer wrapper constructor.
        /// @param Buffer The buffer to be wrapped.
        /// @param BufferSize The size of the buffer being wrapped.
        /// @param FreeAfter Whether or not the buffer passed in should be deleted when this stream is destroyed.
        /// @param Mode The configuration to open the memory buffer with.
        MemoryStream(void* Buffer, const Whole BufferSize, const Boole FreeAfter, const Whole Mode = Mezzanine::SF_Read | Mezzanine::SF_Write);
        /// @brief Class destructor.
        virtual ~MemoryStream();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Creates a new memory buffer to stream to/from.
        /// @remarks No changes will be made if the Size is set to 0 or less.
        /// @param Size The size of the buffer to be allocated.
        /// @param Mode An open mode describing if this buffer will be configured for reading, writing, or both.
        void CreateBuffer(const Whole Size, const Whole Mode = Mezzanine::SF_Read | Mezzanine::SF_Write);
        /// @brief Wraps an external buffer for streaming.
        /// @param Buffer The buffer to be wrapped.
        /// @param BufferSize The size of the buffer being wrapped.
        /// @param FreeBuf Whether or not the buffer passed in should be deleted when this stream is destroyed.
        /// @param Mode An open mode describing if this buffer will be configured for reading, writing, or both.
        void SetBuffer(void* Buffer, const Whole BufferSize, const Boole FreeBuf, const Whole Mode = Mezzanine::SF_Read | Mezzanine::SF_Write);
        /// @brief Copies a pre-existing buffer to this buffer for streaming.
        /// @param Buffer The buffer to be copied.
        /// @param BufferSize The size of the buffer to be copied.
        /// @param Mode An open mode describing if this buffer will be configured for reading, writing, or both.
        void CopyBuffer(const void* Buffer, const Whole BufferSize, const Whole Mode = Mezzanine::SF_Read | Mezzanine::SF_Write);

        /// @brief Gets the start of this buffer.
        /// @return Returns a pointer to the first valid element in this buffer.
        Char8* GetBufferStart() const;
        /// @brief Gets the end of this buffer.
        /// @return Returns a pointer to one passed the last valid element in this buffer.
        Char8* GetBufferEnd() const;

        /// @brief Sets if this should delete it's internal buffer when it is destroyed.
        /// @param FreeBuf Whether or not the buffer passed in should be deleted when this stream is destroyed.
        void SetFreeBuffer(const Boole FreeBuf);
        /// @brief Gets if this should delete it's internal buffer when it is destroyed.
        /// @return Returns true if the internal buffer will be deleted when this is destroyed.
        Boole GetFreeBuffer() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Base Operations

        /// @copydoc StreamBase::GetSize() const
        virtual StreamSize GetSize() const;
    };//MemoryStream
}//Mezzanine

#endif
