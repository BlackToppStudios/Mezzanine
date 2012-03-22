//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
                    DS_Read = 1,
                    DS_Write = 2
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
                DataStream(const AccessMode& Mode = DS_Read);
                /// @brief Named constructor.
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
            public:
                /// @brief
                MemoryDataStream();
                /// @brief
                MemoryDataStream();
                /// @brief
                MemoryDataStream();
                /// @brief
                MemoryDataStream();
                /// @brief Class destructor.
                virtual ~MemoryDataStream();
        };//MemoryDataStream
    }//Resource
}//Mezzanine

#endif
