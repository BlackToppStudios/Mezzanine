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
#ifndef _binarytool_h
#define _binarytool_h

#include "datatypes.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A grouping of utilities for working with binary data more easily
    ///////////////////////////////////////
    namespace BinaryTools
    {
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief A way to store and pass binary buffers like compiled bytcode.
        /// @details Originally intended for use with @ref ScriptCompilable as a basic way to store and pass bytecode around.
        /// This deletes the passed binary on destruction. to prevent this behavior set the Binary pointer to null.
        /// @n @n
        /// This is designed to be fairly minimalistic and pointer-like so that it can be passed by value to reduce
        /// double pointer indirection and caching costs.
        class BinaryBuffer
        {
            public:

                /// @brief How many bytes is @ref Binary in size. This is set to 0 if @ref Binary is invalid and should be a null pointer.
                Whole Size;

                /// @brief A pointer to the actual binary data.
                UInt8* Binary;

                /// @brief Default constructor, set everything to zero.
                BinaryBuffer() :
                    Size(0),
                    Binary(NULL)
                    {}

                /// @brief Copy constructor
                BinaryBuffer(const BinaryBuffer& Other);

                /// @brief Assignment Operator
                /// @details This deletes the buffer if it si
                /// @param RH The item on the right hand side
                /// @return A reference to the newly assigned binary buffer.
                BinaryBuffer& operator= (const BinaryBuffer& RH);

                /// @brief Verbose constructor, set everything custom on creation.
                /// @param PredeterminedSize The size to set on creation.
                /// @param BinaryPointer A pointer to the first byte in memory, to be set on creation.
                BinaryBuffer(Integer PredeterminedSize, UInt8* BinaryPointer) :
                    Size(PredeterminedSize),
                    Binary(BinaryPointer)
                    {}

                /// @brief Virtual deconstructor calls @ref DeleteBuffer() to clean up whatever has been inserted here
                /// @details IF you do not want the Buffer pointed to by the pointer Binary assign Binary to 0 and
                /// this deconstructor will delete with erase nothing.
                virtual ~BinaryBuffer();

                /// @brief Deletes whatever Binary points at and assigns Size to 0.
                /// @param NewSize If you don't want to just clear the buffer, but rather want to set size to a value and set a new size, you can do that with this
                /// @param NewData If you don't want to just delete the Binary pointer, but want to set a new one, pass the new pointer here.
                void DeleteBuffer(Whole NewSize=0, UInt8* NewData=0);

                /// @brief Get the binary buffer as a base64 string
                /// @return a String contain a base6 encoded version of the binary
                String AsBase64();

                /// @brief This calls @ref DeleteBuffer then Decodes that passed and repopulates the Buffer
                /// @param EncodedBinaryData The Base64 string containing binary data.
                void CreateFromBase64(String EncodedBinaryData);
        };

        /// @brief Is a character a valid Base64 character
        /// @param Character a single char to check if it could possibly be valid base64
        /// @return True if the character could be part of a valid block of Base64 text, false otherwise
        bool IsBase64(unsigned char Char8);

        /// @brief Convert a binary buffer to a Base64 string.
        /// @param BytesToEncode A pointer to the beginning of the buffer.
        /// @param Length The length of the bufferin bytes.
        String Base64Encode(UInt8 const* BytesToEncode, unsigned int Length);

        /// @brief Convert Base64 stuff back to binary
        /// @param EncodedString The results of a previous function like @ref Base64Encode to be converted back to binary
        /// @return A String witht the raw bianry
        /// @todo Return something not a String and more amenable to use as a binary buffer.
        String Base64Decode(String const& EncodedString);

    } //stringtool
}//Mezzanine

#endif
