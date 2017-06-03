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
#ifndef _datastream_h
#define _datastream_h

#include "datatypes.h"
#include "countedptr.h"

#include <iostream>

/// @file
/// @brief Declaration of DataStream
/// @todo Investigate how required these stream implementations are

namespace Mezzanine
{
    /// @brief Convenience define for the stream position datatype.
    typedef std::streampos StreamPos;
    /// @brief Convenience define for the stream offset datatype.
    typedef std::streamoff StreamOff;
    /// @brief Convenience define for the stream size datatype.
    typedef std::streamsize StreamSize;

    /// @brief This enum describes the flags that control certain behaviors of a stream.
    /// @details It is important to note that not all of these flags are used by all streams.
    enum StreamFlags
    {
        SF_None         = 0,                     ///< Error/no special initialization.
        SF_Read         = std::ios_base::in,     ///< Permit read operations on the stream.
        SF_Write        = std::ios_base::out,    ///< Permit write operations on the stream.
        SF_Append       = std::ios_base::app,    ///< All write operations on the stream are done at the end of the stream.
        SF_AtEnd        = std::ios_base::ate,    ///< Moves the starting position of the stream to the end upon initialization.
        SF_Binary       = std::ios_base::binary, ///< Tell the stream that the file in question is Binary.
        SF_Truncate     = std::ios_base::trunc   ///< Clear the contents of the file when opening.  Note that this will also create the file if it's not found.
    };

    /// @brief An enum describing which position should be considered the origin for changing the current position in a stream.
    enum SeekOrigin
    {
        SO_Beginning = std::ios_base::beg,  ///< The beginning of the stream.
        SO_Current   = std::ios_base::cur,  ///< The current position for read/write operations in the stream.
        SO_End       = std::ios_base::end   ///< The end of the stream.
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Base class interface for resource streams.
    ///////////////////////////////////////
    class iStreamBase
    {
    public:
        /// @brief Class constructor.
        iStreamBase() {  }
        /// @brief Class destructor.
        virtual ~iStreamBase() {  }

        /// @brief Gets the size of the stream.
        /// @return Returns the size of this stream in bytes.
        virtual StreamSize GetSize() const = 0;
        /// @brief Gets whether or not the current position is at the end of the stream.
        /// @return Returns true if the current position has reached the end of the stream, false otherwise.
        virtual Boole EoF() const = 0;
        /// @brief Gets whether or not a critical error was detected in a previous operation in the stream.
        /// @return Returns true if a critical error has occurred and the stream integrity may be invalid, false otherwise.
        virtual Boole Bad() const = 0;
        /// @brief Gets whether or not an otherwise silent and recoverable error was detected in a previous operation in the stream.
        /// @return Returns true if a non-critical operation failed (such as seek), false otherwise.
        virtual Boole Fail() const = 0;
        /// @brief Gets whether or not this stream is intact and ready for operations.
        /// @return Returns true if no failures have been detected, false otherwise.
        virtual Boole IsValid() const = 0;
        /// @brief Clears any stored error state on the stream.
        /// @remarks This is useful for non-critical errors such as ones that cause "EoF()" or "Fail()" to return true but not
        /// "Bad()".  Using this to clear critical errors is not advised.
        virtual void ClearErrors() = 0;
    };//iStreamBase

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Interface class for input (read) streams.
    ///////////////////////////////////////
    class iInStream : virtual public iStreamBase
    {
    public:
        /// @brief Class constructor.
        iInStream() {  }
        /// @brief Class destructor.
        virtual ~iInStream() {  }

        /// @brief Reads from the stream and copies that data to a buffer.
        /// @param Buffer The buffer to be populated with the read data.
        /// @param Size The number of bytes to read from the stream.
        /// @return Returns the number of bytes successfully read.
        virtual size_t Read(void* Buffer, StreamSize Size) = 0;

        /// @brief Sets the position of the read cursor explicitly.
        /// @param Position The position to be set.
        virtual void SetReadPosition(StreamPos Position) = 0;
        /// @brief Sets the position of the read cursor.
        /// @param Offset The number of bytes to move the read cursor back(if negative) or forward(if positive).
        /// @param Origin The starting point to be considered for the offset.
        virtual void SetReadPosition(StreamOff Offset, SeekOrigin Origin) = 0;
        /// @brief Gets the current read position in this stream.
        /// @return Returns a StreamPos representing the current read position.
        virtual StreamPos GetReadPosition() = 0;
    };//iInStream

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Interface class for output (write) streams.
    ///////////////////////////////////////
    class iOutStream : virtual public iStreamBase
    {
    public:
        /// @brief Class constructor.
        iOutStream() {  }
        /// @brief Class destructor.
        virtual ~iOutStream() {  }

        /// @brief Writes data to the stream.
        /// @param Buffer The memory buffer to write to this stream.
        /// @param Size The size of the buffer being passed in.
        /// @return Returns the number of bytes successfully written.
        virtual size_t Write(const void* Buffer, StreamSize Size) = 0;

        /// @brief Sets the position of the write cursor explicitly.
        /// @param Position The position to be set.
        virtual void SetWritePosition(StreamPos Position) = 0;
        /// @brief Sets the position of the write cursor.
        /// @param Offset The number of bytes to move the write cursor back(if negative) or forward(if positive).
        /// @param Origin The starting point to be considered for the offset.
        virtual void SetWritePosition(StreamOff Offset, SeekOrigin Origin) = 0;
        /// @brief Gets the current write position in this stream.
        /// @return Returns a StreamPos representing the current write position.
        virtual StreamPos GetWritePosition() = 0;
    };//iOutStream

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Base class for input (read) streams with minimal implementation.
    ///////////////////////////////////////
    class IStream : public iInStream, public std::istream
    {
    public:
        /// @brief Class constructor.
        /// @param Buf A pointer to the buffer that will be streamed.
        IStream(std::streambuf* Buf);
        /// @brief Class destructor.
        virtual ~IStream();

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Base Operations

        /// @copydoc iStreamBase::EoF() const
        virtual Boole EoF() const;
        /// @copydoc iStreamBase::Bad() const
        virtual Boole Bad() const;
        /// @copydoc iStreamBase::Fail() const
        virtual Boole Fail() const;
        /// @copydoc iStreamBase::IsValid() const
        virtual Boole IsValid() const;
        /// @copydoc iStreamBase::ClearErrors()
        virtual void ClearErrors();

        ///////////////////////////////////////////////////////////////////////////////
        // Input methods

        /// @copydoc iInStream::Read(void*, StreamSize)
        virtual size_t Read(void* Buffer, StreamSize Size);

        /// @copydoc iInStream::SetReadPosition(StreamPos)
        virtual void SetReadPosition(StreamPos Position);
        /// @copydoc iInStream::SetReadPosition(StreamOff, SeekOrigin)
        virtual void SetReadPosition(StreamOff Offset, SeekOrigin Origin);
        /// @copydoc iInStream::GetReadPosition()
        virtual StreamPos GetReadPosition();
    };//IStream

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Base class for output (write) streams with minimal implementation.
    ///////////////////////////////////////
    class OStream : public iOutStream, public std::ostream
    {
    public:
        /// @brief Class constructor.
        /// @param Buf A pointer to the buffer that will be streamed.
        OStream(std::streambuf* Buf);
        /// @brief Class destructor.
        virtual ~OStream();

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Base Operations

        /// @copydoc iStreamBase::EoF() const
        virtual Boole EoF() const;
        /// @copydoc iStreamBase::Bad() const
        virtual Boole Bad() const;
        /// @copydoc iStreamBase::Fail() const
        virtual Boole Fail() const;
        /// @copydoc iStreamBase::IsValid() const
        virtual Boole IsValid() const;
        /// @copydoc iStreamBase::ClearErrors()
        virtual void ClearErrors();

        ///////////////////////////////////////////////////////////////////////////////
        // Output methods

        /// @copydoc iOutStream::Write(const void*, StreamSize)
        virtual size_t Write(const void* Buffer, StreamSize Size);

        /// @copydoc iOutStream::SetWritePosition(StreamPos)
        virtual void SetWritePosition(StreamPos Position);
        /// @copydoc iOutStream::SetWritePosition(StreamOff, SeekOrigin)
        virtual void SetWritePosition(StreamOff Offset, SeekOrigin Origin);
        /// @copydoc iOutStream::GetWritePosition()
        virtual StreamPos GetWritePosition();
    };//OStream

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Base class for streams that support both read and write operations.
    ///////////////////////////////////////
    class IOStream : public iInStream, public iOutStream, public std::iostream
    {
    public:
        /// @brief Class constructor.
        /// @param Buf A pointer to the buffer that will be streamed.
        IOStream(std::streambuf* Buf);
        /// @brief Class destructor.
        virtual ~IOStream();

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Base Operations

        /// @copydoc iStreamBase::EoF() const
        virtual Boole EoF() const;
        /// @copydoc iStreamBase::Bad() const
        virtual Boole Bad() const;
        /// @copydoc iStreamBase::Fail() const
        virtual Boole Fail() const;
        /// @copydoc iStreamBase::IsValid() const
        virtual Boole IsValid() const;
        /// @copydoc iStreamBase::ClearErrors()
        virtual void ClearErrors();

        ///////////////////////////////////////////////////////////////////////////////
        // Input methods

        /// @copydoc iInStream::Read(void*, StreamSize)
        virtual size_t Read(void* Buffer, StreamSize Size);

        /// @copydoc iInStream::SetReadPosition(StreamPos)
        virtual void SetReadPosition(StreamPos Position);
        /// @copydoc iInStream::SetReadPosition(StreamOff, SeekOrigin)
        virtual void SetReadPosition(StreamOff Offset, SeekOrigin Origin);
        /// @copydoc iInStream::GetReadPosition()
        virtual StreamPos GetReadPosition();

        ///////////////////////////////////////////////////////////////////////////////
        // Output methods

        /// @copydoc iOutStream::Write(const void*, StreamSize)
        virtual size_t Write(const void* Buffer, StreamSize Size);

        /// @copydoc iOutStream::SetWritePosition(StreamPos)
        virtual void SetWritePosition(StreamPos Position);
        /// @copydoc iOutStream::SetWritePosition(StreamOff, SeekOrigin)
        virtual void SetWritePosition(StreamOff Offset, SeekOrigin Origin);
        /// @copydoc iOutStream::GetWritePosition()
        virtual StreamPos GetWritePosition();

        ///////////////////////////////////////////////////////////////////////////////
        // Input/Output methods

        /// @brief Advances the position in the stream.
        /// @param Count The number of bytes to skip/advance in the stream from the current position.
        virtual void Advance(const StreamOff Count);
        /// @brief Sets the position of the read and write cursors explicitly.
        /// @param Position The position to be set.
        virtual void SetStreamPosition(StreamPos Position);
        /// @brief Sets the position of the read and write cursors.
        /// @param Offset The number of bytes to move the cursors back(if negative) or forward(if positive).
        /// @param Origin The starting point to be considered for the offset.
        virtual void SetStreamPosition(StreamOff Offset, SeekOrigin Origin);
        /// @brief Gets the current position in this stream.
        /// @param Read Whether or not to get the Read position.  If false this will get the write position instead.
        /// @return Returns a StreamPos representing the current position specified from the beginning of the stream.
        virtual StreamPos GetStreamPosition(const Boole Read = true);

        ///////////////////////////////////////////////////////////////////////////////
        // Formatting Methods

        /// @brief Gets the contents of the stream as a string.
        /// @return Returns a string with the contents of the stream.
        virtual String GetAsString();
        /// @brief Reads a single line from a string.
        /// @param Buffer Pointer to the buffer to copy to.
        /// @param MaxCount The maximum number of bytes to read.  Usually you want this to be your buffer size.
        /// @param Delim The character that marks the end of a line.
        /// @return Returns the number of bytes actually read, not including the Delimiter.
        virtual size_t ReadLine(Char8* Buffer, size_t MaxCount, const String& Delim = "\n");
        /// @brief Gets the contents of the current line in the stream.
        /// @param Trim Whether or not to trim whitespaces on both sides of the string.
        /// @return Returns a string containing characters from the current position in the stream to the end of the line.
        virtual String GetLine(Boole Trim = true);
        /// @brief Moves the current position to the start of the next line.
        /// @param Delim The character that marks the end of a line.
        /// @return Returns the number of bytes skipped.
        virtual size_t SkipLine(const String& Delim = "\n");
    };//IOStream

    ///////////////////////////////////////////////////////////////////////////////
    // Compatibility Types

    /// @brief Convenience type for compatibility.
    typedef IOStream DataStream;

    ///////////////////////////////////////////////////////////////////////////////
    // Convenience Standard Types

    /// @brief Convenience type for a standard input stream.
    typedef std::istream StdInputStream;
    /// @brief Convenience type for a standard output stream.
    typedef std::ostream StdOutputStream;
    /// @brief Convenience type for a standard input/output stream.
    typedef std::iostream StdStream;

    ///////////////////////////////////////////////////////////////////////////////
    // Convenience Pointer Types

    /// @brief Convenience type for a standard input stream in a CountedPtr.
    typedef CountedPtr<StdInputStream> StdInputStreamPtr;
    /// @brief Convenience type for a standard output stream in a CountedPtr.
    typedef CountedPtr<StdOutputStream> StdOutputStreamPtr;
    /// @brief Convenience type for a standard input/output stream in a CountedPtr.
    typedef CountedPtr<StdStream> StdStreamPtr;

    /// @brief Convenience type for a standard input stream in a CountedPtr.
    typedef CountedPtr<IStream> IStreamPtr;
    /// @brief Convenience type for a standard output stream in a CountedPtr.
    typedef CountedPtr<OStream> OStreamPtr;
    /// @brief This is a convenience type for a data stream in a counted pointer.
    typedef CountedPtr<DataStream> DataStreamPtr;
}//Mezzanine

#endif
