//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _resourcedatastream_h
#define _resourcedatastream_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class DataStream
        /// @headerfile resourcedatastream.h
        /// @brief This represents a stream to a piece of data, usually a file.
        /// @details This is a base class that can be overriden to read from a variey of sources including
        /// data in memory, and archive files.
        ///////////////////////////////////////
        class MEZZ_LIB DataStream
        {
            public:
                /// @enum AccessMode
                /// @brief This enum describes the type of access to the resource.
                enum AccessMode
                {
                    DS_Read  = 1,
                    DS_Write = 2
                };
                /// @enum StreamFlags
                /// @brief This enum describes the flags that control certain behaviors of a stream.
                /// @details It is important to note that not all of these flags are used by all streams.
                enum StreamFlags
                {
                    SF_None         = 0,
                    // Used by the File DataStreams
                    SF_CreateOnFail = 1,  ///< In the event of a failure to find the file, create it.
                    SF_Binary       = 2,  ///< Tell the stream that the file in question is Binary.
                    SF_Truncate     = 4   ///< Clear the contents of the file when opening.  Note that this will also create the file if it's not found.
                };
            protected:
                /// @brief Name of the resource.  If this is a stream to a file, this should be the filename.
                String ResourceName;
                /// @brief The type of access this stream has to the resource.
                AccessMode Access;
                /// @brief The size of the stream.
                size_t Size;
            public:
                /// @brief Class constructor.
                /// @param Mode The access mode for the stream.
                DataStream(const AccessMode& Mode = DS_Read);
                /// @brief Named constructor.
                /// @param Name The name of the stream.
                /// @param Mode The access mode for the stream.
                DataStream(const String& Name, const AccessMode& Mode = DS_Read);
                /// @brief Class destructor.
                virtual ~DataStream();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility
                /// @brief Gets the name of this resource.
                /// @return Returns a const reference to a string containing the name of this resource if one was assigned.
                ConstString& GetName() const;
                /// @brief Gets the currently set access mode for this data stream.
                /// @return Returns an enum value representing the current access mode for this data stream.
                AccessMode GetAccessMode() const;
                /// @brief Gets the size of the stream.
                /// @return Returns the size of this stream in bytes.
                size_t GetSize() const;
                /// @brief Gets whether this stream can be read.
                /// @return Returns true if this stream is in reading mode, false otherwise.
                virtual bool IsReadable() const;
                /// @brief Gets whether this stream can be written to.
                /// @return Returns true if this stream is in writing mode, false otherwise.
                virtual bool IsWriteable() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Stream Access and Manipulation
                /// @brief Gets the contents of the stream as a string.
                /// @return Returns a string with the contents of the stream.
                virtual String GetAsString();
                /// @brief Reads a single line from a string.
                //// @remarks This function should only be used if a file was opened in binary mode.
                /// @param Buffer Pointer to the buffer to copy to.
                /// @param MaxCount The maximum number of bytes to read.  Usually you want this to be your buffer size.
                /// @param Delim The character that marks the end of a line.
                /// @return Returns the number of bytes actually read, not including the Delimiter.
                virtual size_t ReadLine(Character* Buffer, size_t MaxCount, const String& Delim = "\n");
                /// @brief Gets the contents of the current line in the stream.
                //// @remarks This function should only be used if a file was opened in binary mode.
                /// @param Trim Whether or not to trim whitespaces on both sides of the string.
                /// @return Returns a string containing characters from the current position in the stream to the end of the line.
                virtual String GetLine(bool Trim = true);
                /// @brief Moves the current position to the start of the next line.
                //// @remarks This function should only be used if a file was opened in binary mode.
                /// @param Delim The character that marks the end of a line.
                /// @return Returns the number of bytes skipped.
                virtual size_t SkipLine(const String& Delim = "\n");
                /// @brief Reads data from the stream, copying to a buffer.
                /// @param Buffer The buffer to place data from the stream.
                /// @param Count The number of bytes to read from the stream.
                /// @return Returns the number of bytes read.
                virtual size_t Read(void* Buffer, const size_t& Count) = 0;
                /// @brief Writes data from the stream, copying from the provided buffer.
                /// @param Buffer The buffer of data to be written.
                /// @param Count The number of bytes to write from the buffer to the stream.
                /// @return Returns the number of bytes written.
                virtual size_t Write(const void* Buffer, const size_t& Count) = 0;
                /// @brief Advances the position in the stream.
                /// @param Count The number of bytes to skip/advance in the stream from the current position.
                virtual void Skip(const Integer& Count) = 0;
                /// @brief Sets the current position in the stream.
                /// @param Position The position in the stream to be set as current.
                virtual void Seek(const size_t& Position) = 0;
                /// @brief Gets the offset of the current position from the start position.
                /// @return Returns the number of bytes the current position is from the beginning of the stream.
                virtual size_t Tell() = 0;
                /// @brief Gets whether or not the current position is at the end of the file/stream.
                /// @return Returns true if the current position has reached the end of the stream, false otherwise.
                virtual bool EoF() const = 0;
                /// @brief Closes the stream to the resource.
                virtual void Close() = 0;
        };//DataStream

        ///////////////////////////////////////////////////////////////////////////////
        /// @class MemoryDataStream
        /// @headerfile resourcedatastream.h
        /// @brief This represents a stream to a buffer in memory.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB MemoryDataStream : public DataStream
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
                MemoryDataStream(const size_t& BufferSize, bool FreeOnClose = true, bool ReadOnly = false);
                /// @brief Named buffer creation constructor.
                /// @param Name The name of the stream.
                /// @param BufferSize The size of the buffer to be created.
                /// @param FreeOnClose If true this will delete the memory buffer when the stream is closed.
                /// @param ReadOnly If true, writing operations on this stream will be prohibited.
                MemoryDataStream(const String& Name, const size_t& BufferSize, bool FreeOnClose = true, bool ReadOnly = false);
                /// @brief Pre-made buffer constructor.
                /// @param Buffer The premade buffer to stream from.
                /// @param BufferSize The size of the buffer to stream to/from.
                /// @param FreeOnClose If true this will delete the memory buffer when the stream is closed.
                /// @param ReadOnly If true, writing operations on this stream will be prohibited.
                MemoryDataStream(void* Buffer, const size_t& BufferSize, bool FreeOnClose = false, bool ReadOnly = false);
                /// @brief Named pre-made buffer constructor.
                /// @param Name The name of the stream.
                /// @param Buffer The premade buffer to stream from.
                /// @param BufferSize The size of the buffer to stream to/from.
                /// @param FreeOnClose If true this will delete the memory buffer when the stream is closed.
                /// @param ReadOnly If true, writing operations on this stream will be prohibited.
                MemoryDataStream(const String& Name, void* Buffer, const size_t& BufferSize, bool FreeOnClose = false, bool ReadOnly = false);
                /// @brief Class destructor.
                virtual ~MemoryDataStream();

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

                /// @copydoc DataStream::ReadLine
                virtual size_t ReadLine(Character* Buffer, size_t MaxCount, const String& Delim = "\n");
                /// @copydoc DataStream::SkipLine
                virtual size_t SkipLine(const String& Delim = "\n");
                /// @copydoc DataStream::Read
                virtual size_t Read(void* Buffer, const size_t& Count);
                /// @copydoc DataStream::Write
                virtual size_t Write(const void* Buffer, const size_t& Count);
                /// @copydoc DataStream::Skip
                virtual void Skip(const Integer& Count);
                /// @copydoc DataStream::Seek
                virtual void Seek(const size_t& Position);
                /// @copydoc DataStream::Tell
                virtual size_t Tell();
                /// @copydoc DataStream::EoF
                virtual bool EoF() const;
                /// @copydoc DataStream::Close
                virtual void Close();
        };//MemoryDataStream

        ///////////////////////////////////////////////////////////////////////////////
        /// @class FileHandleDataStream
        /// @headerfile resourcedatastream.h
        /// @brief This represents a stream to a file on disk using the C file API.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB FileHandleDataStream : public DataStream
        {
            protected:
                FILE* FileHandle;
            public:
                /// @brief Class constructor.
                /// @param Handle The handle to the file being streamed to/from.
                /// @param Mode The access mode for the stream.
                FileHandleDataStream(FILE* Handle, const DataStream::AccessMode& Mode = DataStream::DS_Read);
                /// @brief Named constructor.
                /// @param Name The name of the stream.
                /// @param Handle The handle to the file being streamed to/from.
                /// @param Mode The access mode for the stream.
                FileHandleDataStream(const String& Name, FILE* Handle, const DataStream::AccessMode& Mode = DataStream::DS_Read);
                /// @brief Self loading constructor.
                /// @param Name The name of the file to be loaded.  This will also become the name of the stream.
                /// @param Path The path to the file being loaded.
                /// @param Flags Additional conditions to apply when opening the file.  See DataStream::StreamFlags enum for more information.
                /// @param Mode The access mode for the stream.
                FileHandleDataStream(const String& Name, const String& Path, const DataStream::StreamFlags& Flags = DataStream::SF_None, const DataStream::AccessMode& Mode = DataStream::DS_Read);
                /// @brief Class destructor.
                virtual ~FileHandleDataStream();

                ///////////////////////////////////////////////////////////////////////////////
                // Stream Access and Manipulation

                /// @copydoc DataStream::Read
                virtual size_t Read(void* Buffer, const size_t& Count);
                /// @copydoc DataStream::Write
                virtual size_t Write(const void* Buffer, const size_t& Count);
                /// @copydoc DataStream::Skip
                virtual void Skip(const Integer& Count);
                /// @copydoc DataStream::Seek
                virtual void Seek(const size_t& Position);
                /// @copydoc DataStream::Tell
                virtual size_t Tell();
                /// @copydoc DataStream::EoF
                virtual bool EoF() const;
                /// @copydoc DataStream::Close
                virtual void Close();
        };//FileHandleDataStream

        ///////////////////////////////////////////////////////////////////////////////
        /// @class FileStreamDataStream
        /// @headerfile resourcedatastream.h
        /// @brief This represents a stream to a file on disk using the C++ file stream API.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB FileStreamDataStream : public DataStream
        {
            protected:
                std::fstream* FileStream;
            public:
                /// @brief Class constructor.
                /// @param Stream The stream to the file being streamed to/from.
                /// @param Mode The access mode for the stream.
                FileStreamDataStream(std::fstream* Stream, const DataStream::AccessMode& Mode = DataStream::DS_Read);
                /// @brief Named constructor.
                /// @param Name The name of the stream.
                /// @param Stream The stream to the file being streamed to/from.
                /// @param Mode The access mode for the stream.
                FileStreamDataStream(const String& Name, std::fstream* Stream, const DataStream::AccessMode& Mode = DataStream::DS_Read);
                /// @brief Self loading constructor.
                /// @param Name The name of the file to be loaded.  This will also become the name of the stream.
                /// @param Path The path to the file being loaded.
                /// @param Flags Additional conditions to apply when opening the file.  See DataStream::StreamFlags enum for more information.
                /// @param Mode The access mode for the stream.
                FileStreamDataStream(const String& Name, const String& Path, const DataStream::StreamFlags& Flags = DataStream::SF_None, const DataStream::AccessMode& Mode = DataStream::DS_Read);
                /// @brief Class destructor.
                virtual ~FileStreamDataStream();

                ///////////////////////////////////////////////////////////////////////////////
                // Stream Access and Manipulation

                /// @copydoc DataStream::Read
                virtual size_t Read(void* Buffer, const size_t& Count);
                /// @copydoc DataStream::Write
                virtual size_t Write(const void* Buffer, const size_t& Count);
                /// @copydoc DataStream::Skip
                virtual void Skip(const Integer& Count);
                /// @copydoc DataStream::Seek
                virtual void Seek(const size_t& Position);
                /// @copydoc DataStream::Tell
                virtual size_t Tell();
                /// @copydoc DataStream::EoF
                virtual bool EoF() const;
                /// @copydoc DataStream::Close
                virtual void Close();
        };//FileStreamDataStream
    }//Resource
}//Mezzanine

#endif
