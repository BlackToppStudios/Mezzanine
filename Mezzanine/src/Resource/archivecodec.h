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
#ifndef _resourcearchivecodec_h
#define _resourcearchivecodec_h

#include "datatypes.h"
#include "Resource/archivestream.h"

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A base class for data compression/decompression algorithms to implement.
        ///////////////////////////////////////
        class MEZZ_LIB ArchiveCodec
        {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Public Types

            /// @brief Convenience type for passing around byte buffers.
            using ByteVector = std::vector<UInt8>;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Class constructor.
            ArchiveCodec() = default;
            /// @brief Class destructor.
            virtual ~ArchiveCodec() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Compression

            /// @brief Compresses an entire buffer of data.
            /// @param Input A buffer to be compressed.
            /// @return Returns a vector containing the compressed result of the input buffer.
            virtual ByteVector Compress(const ByteVector& Input) const = 0;
/*
            /// @brief Compresses data from an input buffer into a stream.
            /// @remarks The current position in the stream will be used as the start of the data range
            /// to compress and will use everything until the end of the stream.
            /// @param Input A buffer to be compressed.
            /// @param ToCompress The stream providing the data to be compressed.
            /// @return Returns the resulting size of the buffer after compression.
            virtual Whole Compress(const ByteVector& Input, ArchiveOStreamPtr ToStream) = 0;
            /// @brief Compresses data from an input buffer into a stream.
            /// @remarks The current position in the stream will be used as the start of the data range
            /// to compress and will use everything until NumBytes have been read.
            /// @param Input A buffer to be compressed.
            /// @param ToCompress The stream providing the data to be compressed.
            /// @param NumBytes The number of bytes from the provided stream to compress.
            /// @return Returns the resulting size of the buffer after compression.
            virtual Whole Compress(const ByteVector& Input, Whole NumBytes, ArchiveOStreamPtr ToCompress) = 0;
*/

            ///////////////////////////////////////////////////////////////////////////////
            // Decompression

            /// @brief Decompresses an entire buffer of data.
            /// @param Input A buffer to be decompressed.
            /// @param Expected The expected size of the decompressed data.
            /// @return Returns a vector containing the decompressed result of the input buffer.
            virtual ByteVector Decompress(const ByteVector& Input, const StreamSize Expected) const = 0;
/*
            /// @brief Decompresses data from a stream to an output buffer.
            /// @param ToDecompress The stream providing the data to be decompressed.
            /// @return Returns a vector containing the decompressed result of the input stream.
            virtual ByteVector Decompress(ArchiveIStreamPtr ToDecompress) = 0;
            /// @brief Decompresses data from a stream to an output buffer.
            /// @param ToDecompress The stream providing the data to be decompressed.
            /// @param NumBytes The number of bytes from the provided stream to decompress.
            /// @return Returns a vector containing the decompressed result of the input stream.
            virtual ByteVector Decompress(ArchiveIStreamPtr ToDecompress, Whole NumBytes) = 0;
*/
        };//ArchiveCodec
    }//Resource
}//Mezzanine

#endif
