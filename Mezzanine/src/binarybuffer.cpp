// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#ifndef _binarytool_cpp
#define _binarytool_cpp

/// @file
/// @brief The implementation of some tools for making working with binary stuff easier.

#include "binarybuffer.h"
#include "exception.h"

#include <string.h>
#include <algorithm>

namespace Mezzanine
{
    namespace BinaryTools
    {
        /*
         * Some of the following functions and 1 variable were taken from http://www.adp-gmbh.ch/cpp/common/base64.html
         * for the functions IsBase64(unsigned char c),  Base64Encode(UInt8 const* BytesToEncode, unsigned int Length),
         *     Base64Decode(String const& EncodedString) and Base64Chars and maybe a few others in this file or the
         *     tests for this file
         * with written permission as follows.


           Copyright (C) 2004-2008 René Nyffenegger

           This source code is provided 'as-is', without any express or implied
           warranty. In no event will the author be held liable for any damages
           arising from the use of this software.

           Permission is granted to anyone to use this software for any purpose,
           including commercial applications, and to alter it and redistribute it
           freely, subject to the following restrictions:

           1. The origin of this source code must not be misrepresented; you must not
              claim that you wrote the original source code. If you use this source code
              in a product, an acknowledgment in the product documentation would be
              appreciated but is not required.

           2. Altered source versions must be plainly marked as such, and must not be
              misrepresented as being the original source code.

           3. This notice may not be removed or altered from any source distribution.

           René Nyffenegger rene.nyffenegger@adp-gmbh.ch

        */

        namespace
        {
            // Code change to Match BTS naming conventions and formatting
            static const String Base64Chars =
                         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                         "abcdefghijklmnopqrstuvwxyz"
                         "0123456789+/";

            /// @brief The actual implementation of the Base64 decoding
            /// @param EncodedString The String to decode
            /// @param Results A reference to a @ref BinaryBuffer with the Size set correctly and a Buffer Allocated, this will be the output.
            void Base64DecodeImpl(const String& EncodedString, BinaryBuffer& Results)
            {
                if(Results.Binary==0 || Results.Size==0)
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION, "Cannot encode an empty buffer."); }

                String::const_iterator Progress = EncodedString.begin();
                Whole Output = 0;

                //Initializing this RAM really slows it down like 30% to double onto execution time.
                //for(Whole c=0; c<Results.Size; c++)
                    //{ Results[c] = 0; }

                unsigned char First;
                unsigned char Second;
                unsigned char Third;
                unsigned char Fourth;

                while(Progress<EncodedString.end())
                {
                    // cout << *(Progress+0) << *(Progress+1) << *(Progress+2) << *(Progress+3) << endl;
                    // cout << !IsBase64(*Progress) << !IsBase64(*(Progress+1)) << !IsBase64(*(Progress+2)) << !IsBase64(*(Progress+3)) <<endl;
                    if(!IsBase64(*Progress) || !IsBase64(*(Progress+1)) || !IsBase64(*(Progress+2)) || !IsBase64(*(Progress+3)))
                        { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION, "Base64 contains an invalid character and cannot be decoded."); }

                    First = Base64Chars.find(*(Progress+0));
                    Second = Base64Chars.find(*(Progress+1));
                    Third = *(Progress+2)=='=' ? 0 : Base64Chars.find(*(Progress+2));

                    #ifdef MEZZDEBUG
                    if(Output+1>Results.Size)
                        { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION, "Output of base64 Decoding is larger than it should be."); }
                    #endif

                    *(Results.Binary+Output+0) = (First << 2) + ((Second & 0x30) >> 4);
                    *(Results.Binary+Output+1) = ((Second & 0xf) << 4) + ((Third & 0x3c) >> 2);
                    if(*(Progress+3)!='=')
                    {
                        #ifdef MEZZDEBUG
                        if(Output+2>Results.Size)
                            { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION, "Output of base64 Decoding is larger than it should be."); }
                        #endif
                        Fourth = Base64Chars.find(*(Progress+3));
                        *(Results.Binary+Output+2) = ((Third & 0x3) << 6) + Fourth;
                    }

                    #ifdef MEZZDEBUG
                    if(Progress>EncodedString.end())
                        { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION, "Gone past the end of the input while decoding a base64 string."); }
                    #endif
                    Output+=3;
                    Progress+=4;
                }
            }

        }

        BinaryBuffer::BinaryBuffer(const BinaryBuffer& Other)
        {
            this->Size = Other.Size;
            if(Other.Size && Other.Binary)
            {
                this->Binary = new Byte[this->Size*sizeof(Byte)];
                memcpy(this->Binary,Other.Binary,this->Size);
            }else{
                this->Binary = 0;
            }
        }

        BinaryBuffer::BinaryBuffer(const String& DataString, Boole IsBase64)
        {
            if(IsBase64)
            {
                Binary=0;
                this->CreateFromBase64(DataString);
            }
            else
            {
                this->Size = DataString.size();
                CreateBuffer();
                memcpy(this->Binary, DataString.c_str(), this->Size);
            }
        }

        BinaryBuffer& BinaryBuffer::operator= (const BinaryBuffer& RH)
        {
            if (RH.Binary == this->Binary)
                { MEZZ_EXCEPTION(Exception::INVALID_ASSIGNMENT, "Attempted a self assignment of a BinaryBuffer"); }
            DeleteBuffer(RH.Size);
            if(RH.Size && RH.Binary)
            {
                CreateBuffer();
                memcpy(this->Binary,RH.Binary,this->Size);
            }else{
                this->Binary = 0;
            }
            return *this;
        }

        BinaryBuffer::~BinaryBuffer()
            { DeleteBuffer(); }

        void BinaryBuffer::DeleteBuffer(Whole NewSize)
        {
            if(Binary)
                { delete[] Binary; }
            Binary=0;
            Size = NewSize;
        }

        void BinaryBuffer::CreateBuffer()
        {
            if(Size)
            {
                this->Binary = new Byte[this->Size*sizeof(Byte)];
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION, "Cannot create a 0 length Buffer.");
            }
        }

        String BinaryBuffer::ToBase64String()
        {
            if(!Size || !Binary)
            {
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION, "Cannot encode 0 length or missing buffer.");
            }
            return Base64Encode((UInt8*)Binary,Size*sizeof(Byte));
        }

        String BinaryBuffer::ToString()
        {
            if(!Size || !Binary)
            {
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION, "Cannot construct string from 0 length or missing buffer.");
            }
            return String((char*)(this->Binary),this->Size*sizeof(Byte));
        }

        void BinaryBuffer::CreateFromBase64(const String& EncodedBinaryData)
        {
            if(Binary)
                { delete[] Binary; }
            Size = PredictBinarySizeFromBase64String(EncodedBinaryData);
            Binary = new Byte[Size*sizeof(Byte)];
            Base64DecodeImpl(EncodedBinaryData,*this);
        }

        BinaryBuffer::Byte& BinaryBuffer::operator[] (Whole Index)
        {
            #ifdef MEZZDEBUG
            if(Index>=Size)
                { MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION, "Attempted access beyond range of Binary Buffer"); }
            #endif
            return *(Binary+Index);
        }

        void BinaryBuffer::Concatenate(const Byte* OtherBuffer, Whole ByteSize)
        {
            Whole NewSize = (this->Size + ByteSize) * sizeof(Byte);
            Byte* TargetBuffer = new Byte[NewSize];

            memcpy(TargetBuffer, this->Binary, this->Size);
            memcpy(TargetBuffer+this->Size, OtherBuffer, ByteSize);

            DeleteBuffer(NewSize);
            this->Binary = TargetBuffer;
        }

        void BinaryBuffer::Concatenate(const BinaryBuffer BufferFromAnotherMother)
            { Concatenate(BufferFromAnotherMother.Binary, BufferFromAnotherMother.Size); }


        BinaryBuffer& BinaryBuffer::operator+=(const BinaryBuffer& RH)
        {
            Concatenate(RH);
            return *this;
        }

        Whole BinaryBuffer::GetSize() const
            { return Size; }

        // Code change to Match BTS naming conventions and formatting
        Boole IsBase64(unsigned char Character)
            { return (isalnum(Character) || (Character == '+') || (Character == '/') || (Character == '=')); }

        String Base64Encode(String const& Unencoded)
            { return Base64Encode((UInt8 const*)Unencoded.c_str(), Unencoded.size()); }

        String Base64Encode(const BinaryBuffer &Buffer)
            { return Base64Encode((const UInt8*) Buffer.Binary,Buffer.Size); }

        // Code change to Match BTS naming conventions and formatting
        String Base64Encode(UInt8 const* BytesToEncode, unsigned int Length)
        {
            String Results;
            Results.reserve(PredictBase64StringSizeFromBinarySize(Length));

            int i = 0;
            int j = 0;
            unsigned char char_array_3[3];
            unsigned char char_array_4[4];

            while (Length--)
            {
                char_array_3[i++] = *(BytesToEncode++);
                if (i == 3)
                {
                    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                    char_array_4[3] = char_array_3[2] & 0x3f;

                    for(i = 0; (i <4) ; i++)
                        { Results += Base64Chars[char_array_4[i]]; }
                    i = 0;
                }
            }

            if (i)
            {
                for(j = i; j < 3; j++)
                    { char_array_3[j] = '\0'; }

                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for (j = 0; (j < i + 1); j++)
                    { Results += Base64Chars[char_array_4[j]]; }

                while((i++ < 3))
                { Results += '='; }
            }

            return Results;
        }

        // Code change to Match BTS naming conventions and formatting, then eventually refactored algorithm to adjust return type and switch from Olog(n) to O(n).
        // This one is guaranteed exactly one memory allocation(+= on String likely only doubles allocation repeatidly), less copying, and in general is simpler
        BinaryBuffer Base64Decode(String const& EncodedString)
        {
            BinaryBuffer Results(PredictBinarySizeFromBase64String(EncodedString));

            Base64DecodeImpl(EncodedString, Results);

            return Results;
        }

        Whole PredictBinarySizeFromBase64String(String const& EncodedString)
        {
            if(EncodedString.size()<4)
                { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION, "It is not possible to have a base64 string less than 4 bytes in length, but one was received.") }

            return EncodedString.size()/4*3 - ( EncodedString.at(EncodedString.size()-2)=='=' ?1:0) - ( EncodedString.at(EncodedString.size()-1)=='=' ?1:0);
        }

        Whole PredictBase64StringSizeFromBinarySize(Whole Length)
            { return (Length+2)/3*4; }


    } // BinaryTools
}//Mezzanine

#endif
