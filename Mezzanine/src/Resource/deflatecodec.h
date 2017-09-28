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
#ifndef _resourcedeflatecodec_h
#define _resourcedeflatecodec_h

#include "Resource/archivecodec.h"

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A codec implementation for standard DEFLATE/INFLATE.
        ///////////////////////////////////////
        class MEZZ_LIB DeflateCodec : public ArchiveCodec
        {
        public:
        protected:
            /// @brief The amount of compression to perform when compressing.
            /// @remarks Range is 1-10.  1 is faster compression, 10 is smaller result.
            Integer CompressionLevel;
        public:
            /// @brief Class constructor.
            DeflateCodec();
            /// @brief Class destructor.
            virtual ~DeflateCodec() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            /// @brief Sets the amount of compression to apply when using the ArchiveCodec API.
            /// @remarks Level range is 0-10.  0 means no compression at all, 1 is faster compression, 10 is smaller result.
            /// @param Level The amount of compression to perform when compressing.
            void SetCompressionLevel(const Integer Level);
            /// @brief Gets the amount of compression to apply when using the ArchiveCodec API.
            /// @return Returns an integer representing the amount of compression that will be applied.  -1 is internal default.
            Integer GetCompressionLevel() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Compression/Decompression

            /// @copydoc ArchiveCodec::Compress(const ByteVector&) const
            virtual ByteVector Compress(const ByteVector& Input) const override;
            /// @copydoc ArchiveCodec::Decompress(const ByteVector&, const StreamSize) const
            virtual ByteVector Decompress(const ByteVector& Input, const StreamSize Expected) const override;
        };//DeflateCodec
    }//Resource
}//Mezzanine

#endif
