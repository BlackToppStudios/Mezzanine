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
#ifndef _resourcezipstream_h
#define _resourcezipstream_h

#include "archivestream.h"

/// @file
/// @brief Declaration of ZipStreams and ZipStreamBuffer.

// Internal Forward Declare
struct zip;
struct zip_file;

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A stream buffer object to a file in z zip archive.
        ///////////////////////////////////////
        class MEZZ_LIB ZipStreamBuffer : public std::streambuf
        {
        protected:
            /// @brief The size of the automatically allocated internal buffer.
            static const UInt32 BufferSize = 1 * 1024;

            /// @brief The internal buffer string get/put areas.
            Char8 Buffer[BufferSize];
            /// @brief A pointer to the internal archive.
            zip* InternalArchive;
            /// @brief A pointer to the internal file in a zip archive.
            zip_file* InternalFile;
            /// @brief The index of the file that is open.
            Int64 FileIndex;
            /// @brief A bitfield describing the settings this buffer was opened with.
            Whole StreamFlags;
            /// @brief Whether or not this stream should perform decompression or stream raw from the archive.
            Boole RawStream;

            /// @brief Called after a stream is opened to see if there is an error.  If an error is found an exception is thrown.
            /// @exception If the stream fails to find the file specified, a IO_FILE_NOT_FOUND_EXCEPTION will be thrown. @n
            /// If an archive password isn't specified or is wrong, a IO_FILE_PERMISSION_EXCEPTION will be thrown. @n
            /// If the stream fails to read or seek when being initialized, a IO_FILE_READ_EXCEPTION will be thrown. @n
            /// If ZLIB fails for any reason, a INTERNAL_EXCEPTION will be thrown. @n
            /// If the compression method or encryption method aren't supported, or any other failure occurs, a IO_FILE_EXCEPTION will be thrown.
            void CheckOpenError();
            /// @brief Gets the value at the current read position without incrementing the read position.
            /// @details This will also update the internal buffers with new data if the end of the current read buffer has been reached.
            /// @return Returns the value at the current read position, or EOF if no more data is available.
            int underflow();
        public:
            /// @brief Class constructor.
            /// @param Arch A pointer to the internal archive.
            ZipStreamBuffer(zip* Arch);
            /// @brief Class destructor.
            virtual ~ZipStreamBuffer();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Opens this stream to a file in an archive.
            /// @exception If the stream fails to find the file specified, a IO_FILE_NOT_FOUND_EXCEPTION will be thrown. @n
            /// If an archive password isn't specified or is wrong, a IO_FILE_PERMISSION_EXCEPTION will be thrown. @n
            /// If the stream fails to read or seek when being initialized, a IO_FILE_READ_EXCEPTION will be thrown. @n
            /// If ZLIB fails for any reason, a INTERNAL_EXCEPTION will be thrown. @n
            /// If the compression method or encryption method aren't supported, or any other failure occurs, a IO_FILE_EXCEPTION will be thrown.
            /// @param File The combined name and path to the file to be opened.
            /// @param Flags The configuration to open the file with.  Use StreamFlags enum values for this field.
            /// @param Raw Whether or not the data in the opened file should be decompressed when read.  False to decompress on the fly, true to read data as it exists on disk.
            void OpenFile(const String& Identifier, const Whole Flags, const Boole Raw);
            /// @brief Opens this stream to a password protected file in an archive.
            /// @exception If the stream fails to find the file specified, a IO_FILE_NOT_FOUND_EXCEPTION will be thrown. @n
            /// If an archive password isn't specified or is wrong, a IO_FILE_PERMISSION_EXCEPTION will be thrown. @n
            /// If the stream fails to read or seek when being initialized, a IO_FILE_READ_EXCEPTION will be thrown. @n
            /// If ZLIB fails for any reason, a INTERNAL_EXCEPTION will be thrown. @n
            /// If the compression method or encryption method aren't supported, or any other failure occurs, a IO_FILE_EXCEPTION will be thrown.
            /// @param File The combined name and path to the file to be opened.
            /// @param Password The password to access the file.
            /// @param Flags The configuration to open the file with.  Use StreamFlags enum values for this field.
            /// @param Raw Whether or not the data in the opened file should be decompressed when read.  False to decompress on the fly, true to read data as it exists on disk.
            void OpenEncryptedFile(const String& Identifier, const String& Password, const Whole Flags, const Boole Raw);
            /// @brief Gets whether or not this stream is currently open to a archive.
            /// @return Returns true if this is streaming to/from a archive.  False otherwise.
            Boole IsOpenToFile() const;
            /// @brief Closes the archive that is currently opened.
            void CloseFile();

            /// @brief Gets the flags that were used to open the archive.
            /// @remarks If this stream is not open to a archive it will return Resource::SF_None.
            /// @return Returns a bitfield describing the flags used to open the archive.
            Whole GetStreamFlags() const;

            /// @brief Gets the compressed size of the data in the stream.
            /// @return Returns a StreamSize indicating the raw size of the stream in bytes.
            StreamSize GetCompressedSize() const;
            /// @brief Gets the uncompressed size of the data in the stream.
            /// @return Returns a StreamSize indicating the size of the data in the stream before it was compressed.
            StreamSize GetUncompressedSize() const;

            /// @brief Gets the path and name of the file that this stream is currently open to.
            /// @return Returns a const String reference containing the path and name of the currently open file.
            String GetStreamIdentifier() const;
            /// @brief Gets whether or not seeking can be performed on the stream to/from the file.
            /// @return Returns true if this buffer allows seeking throughout the stream, false otherwise.
            Boole CanSeek() const;
            /// @brief Gets the size of the stream after extraction.
            /// @return Returns a StreamSize containing the number of bytes in this stream.
            StreamSize GetSize() const;
        };//ZipStreamBuffer

        using ZipIStream = ArchiveIStream<ZipStreamBuffer>;
        using ZipIStreamPtr = std::shared_ptr<ZipIStream>;
        //using ZipOStream =
        //using ZipStream =
    }//Resource
}//Mezzanine

#endif
