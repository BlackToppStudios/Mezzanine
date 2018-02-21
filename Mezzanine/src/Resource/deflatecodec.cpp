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
#ifndef _resourcedeflatecodec_cpp
#define _resourcedeflatecodec_cpp

#include "Resource/deflatecodec.h"

#include "exception.h"

#include <miniz.h>

namespace Mezzanine
{
    namespace Resource
    {
        DeflateCodec::DeflateCodec() :
            CompressionLevel(MZ_DEFAULT_COMPRESSION)
            {  }

        DeflateCodec::DeflateCodec(const Integer Compression) :
            CompressionLevel(Compression)
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration

        void DeflateCodec::SetCompressionLevel(const Integer Level)
            { this->CompressionLevel = Level; }

        Integer DeflateCodec::GetCompressionLevel() const
            { return this->CompressionLevel; }

        ///////////////////////////////////////////////////////////////////////////////
        // Static Compression

        DeflateCodec::ByteVector DeflateCodec::Deflate(const ByteVector& Input, const Integer CompressionLevel)
        {
            const mz_ulong BoundSize = compressBound( Input.size() );
            mz_ulong RetSize = BoundSize;
            ByteVector Ret(RetSize);
            int ErrorCode = mz_compress2(Ret.data(),&RetSize,Input.data(),Input.size(),CompressionLevel);

            if( ErrorCode != MZ_OK ) {
                MEZZ_EXCEPTION(ExceptionBase::IO_EXCEPTION,mz_error(ErrorCode));
            }

            if( RetSize < BoundSize ) {
                Ret.resize(RetSize);
            }
            return Ret;
        }

        DeflateCodec::ByteVector DeflateCodec::Inflate(const ByteVector& Input, const StreamSize Expected)
        {
            mz_ulong RetSize = static_cast<mz_ulong>(Expected);
            ByteVector Ret(RetSize);
            int ErrorCode = mz_uncompress(Ret.data(),&RetSize,Input.data(),Input.size());

            if( ErrorCode != MZ_OK ) {
                MEZZ_EXCEPTION(ExceptionBase::IO_EXCEPTION,mz_error(ErrorCode));
            }

            if( RetSize < Expected ) {
                Ret.resize(RetSize);
            }
            return Ret;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Compression/Decompression

        DeflateCodec::ByteVector DeflateCodec::Compress(const ByteVector& Input) const
            { return DeflateCodec::Deflate(Input,this->CompressionLevel); }

        DeflateCodec::ByteVector DeflateCodec::Decompress(const ByteVector& Input, const StreamSize Expected) const
            { return DeflateCodec::Inflate(Input,Expected); }
    }//Resource
}//Mezzanine

#endif
