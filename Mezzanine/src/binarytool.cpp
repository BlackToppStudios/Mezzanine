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
#ifndef _binarytool_cpp
#define _binarytool_cpp

#include "binarytool.h"


namespace Mezzanine
{

    /*
     * The following 3 functions and 1 variable were taken from http://www.adp-gmbh.ch/cpp/common/base64.html
     * for the functions IsBase64(unsigned char c),  Base64Encode(UInt8 const* BytesToEncode, unsigned int Length)
     *     and Base64Decode(String const& EncodedString)
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

    #include <string.h>

    BinaryTools::BinaryBuffer::BinaryBuffer(const BinaryBuffer& Other)
    {
        if (this == &Other)
            { return; }
        this->Size = Other.Size;
        this->Binary = new UInt8[this->Size];
        memcpy(this->Binary,Other.Binary,this->Size);
    }





    BinaryTools::BinaryBuffer::~BinaryBuffer()
        { DeleteBuffer(); }

    void BinaryTools::BinaryBuffer::DeleteBuffer()
    {
        delete[] Binary;
        Size = 0;
    }

    String BinaryTools::BinaryBuffer::AsBase64()
        { return Base64Encode(Binary,Size); }

    void BinaryTools::BinaryBuffer::CreateFromBase64(String EncodedBinaryData)
    {
        DeleteBuffer();
        String Raw(Base64Decode(EncodedBinaryData));
        Binary = new UInt8[Raw.size()];
        memcpy(Binary,Raw.c_str(),Raw.size());
    }

    // Code change to Match BTS naming conventions and formatting
    static const String Base64Chars =
                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                 "abcdefghijklmnopqrstuvwxyz"
                 "0123456789+/";

    // Code change to Match BTS naming conventions and formatting
    bool BinaryTools::IsBase64(unsigned char Character)
    {
      return (isalnum(Character) || (Character == '+') || (Character == '/'));
    }

    // Code change to Match BTS naming conventions and formatting
    String BinaryTools::Base64Encode(UInt8 const* BytesToEncode, unsigned int Length)
    {
        String ret;
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
                    { ret += Base64Chars[char_array_4[i]]; }
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
                { ret += Base64Chars[char_array_4[j]]; }

            while((i++ < 3))
            { ret += '='; }
        }

        return ret;
    }

    // Code change to Match BTS naming conventions and formatting
    String BinaryTools::Base64Decode(String const& EncodedString)
    {
        int in_len = EncodedString.size();
        int i = 0;
        int j = 0;
        int in_ = 0;
        unsigned char char_array_4[4], char_array_3[3];
        String ret;

        while (in_len-- && ( EncodedString[in_] != '=') && IsBase64(EncodedString[in_]))
        {
            char_array_4[i++] = EncodedString[in_]; in_++;
            if (i ==4)
            {
                for (i = 0; i <4; i++)
                    { char_array_4[i] = Base64Chars.find(char_array_4[i]); }

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++)
                    { ret += char_array_3[i]; }
                i = 0;
            }
        }

        if (i)
        {
            for (j = i; j <4; j++)
                { char_array_4[j] = 0; }

            for (j = 0; j <4; j++)
                { char_array_4[j] = Base64Chars.find(char_array_4[j]); }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++)
                { ret += char_array_3[j]; }
        }

        return ret;
    }


}//Mezzanine

#endif
