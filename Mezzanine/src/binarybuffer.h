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
        /// @brief A way to store and pass binary buffers, for example compiled bytecode.
        /// @details Originally intended for use with ScriptCompilable as a basic way to store and pass bytecode around.
        /// This deletes the passed binary on destruction. To prevent this behavior set the Binary pointer to null.
        /// @n @n
        /// This is designed to be fairly minimalistic but passing by value causes the buffer to be copied.
        /// @n @n
        /// Where possible this class performs no speculative allocation unless explicitly requested to. In other words
        /// this should have allocated exactly as many bytes are indicated by the member Size, no more and no less.
        /// This will tend to not allocate memory unless an operation on it is specified that it does so.
        /// @n @n
        /// Whenever this needs to allocated memory it will use the Size member for determining the amount to allocate. If that
        /// is 0 an InvalidStateException exception is thrown. Bounds checking, if performed, only occurs when
        /// MEZZ_DEBUG is enabled.
        class MEZZ_LIB BinaryBuffer
        {
            public:

                /// @brief The type of data this buffer can hold, it is intended to be some type one byte in length, but doesn't have to be
                typedef Int8 Byte;
#ifndef SWIG
                /// @brief How many bytes is @ref Binary in size. This is set to 0 if @ref Binary is invalid and should be a null pointer.
                Whole Size;

                /// @brief A pointer to the actual binary data.
                Byte* Binary;
#endif
                /// @brief Default constructor, set everything to zero. Doesn't allocate anything
                BinaryBuffer() :
                    Size(0),
                    Binary(NULL)
                    {}

                /// @brief Copy constructor.
                /// @param Other The buffer to copy to make
                /// @details Allocates identical amount of memory as other buffer then copies the other buffer into
                /// the allocated space. Each BinaryBuffer retains ownership of their respective buffers.
                BinaryBuffer(const BinaryBuffer& Other);

                /// @brief Base64 decoding Constructor
                /// @details Performs exactly one allocation of the amount required to store the decoded base64,
                /// Then starts filling it. If there is an error with the data as it is beign process the rest of the Buffer will
                /// be filled with gibberish, and everything before the error will be properly decoded.
                /// @param DataString A Base64 string to be decode and used as a binary buffer, or a string to be used a buffer if IsBase64 is false
                /// @param IsBase64 Is the String passed Base64 encoded
                explicit BinaryBuffer(const String& DataString, Boole IsBase64 = true);

                /// @brief Assignment Operator
                /// @details This deletes the buffer if it is not null, and allocates a fresh one of
                /// the size in RH, then copies it.
                /// @param RH The item on the right hand side
                /// @return A reference to the newly assigned binary buffer.
                BinaryBuffer& operator= (const BinaryBuffer& RH);

                /// @brief Verbose constructor, set everything custom on creation.
                /// @details If passed a pointer this assumes ownship of that pointer, otherwise this allocates the amount of space requested.
                /// @param BinaryPointer A pointer to the first byte in memory, if this is null the buffer is created. Ownership of this Pointer will be assumed.
                /// @param PredeterminedSize The size to set on creation.
                BinaryBuffer(Byte* BinaryPointer, Integer PredeterminedSize) :
                    Size(PredeterminedSize),
                    Binary(BinaryPointer ? BinaryPointer : new Byte[PredeterminedSize])
                    {}

                /// @brief Terse constructor, set a custom size and allocates space (filled with gibberish).
                /// @param PredeterminedSize The size to set on creation.
                explicit BinaryBuffer(Whole PredeterminedSize) :
                    Size(PredeterminedSize),
                    Binary(new Byte[PredeterminedSize])
                    {}

                /// @brief Virtual deconstructor calls @ref DeleteBuffer() to clean up whatever has been inserted here
                /// @details If you do not want the Buffer pointed to by the pointer Binary assign Binary to 0 and
                /// this deconstructor will delete with erase nothing.
                virtual ~BinaryBuffer();

                /// @brief Deletes whatever Binary points at and assigns Size to 0.
                /// @param NewSize If you don't want to just clear the buffer, but rather want to set size to a value and set a new size, you can do that with this
                void DeleteBuffer(Whole NewSize=0);

                /// @brief This will create a buffer with size matching the this->Size and point this->Binary to that Buffer
                /// @warning This does not delete an old buffer, delete that before calling this.
                void CreateBuffer();

                /// @brief Get the binary buffer as a base64 string
                /// @return a String contain a base6 encoded version of the binary
                String ToBase64String();

                /// @brief Get the contents of this crudely converted to a c style string then stuff it in a string
                /// @return A String with the value stored in binary copied into it.
                String ToString();

                /// @brief This calls deallocates any space, allocates fresh space of the size requestedthen the Decodes the passed and repopulates the Buffer
                /// @param EncodedBinaryData The Base64 string containing binary data.
                void CreateFromBase64(const String &EncodedBinaryData);

                /// @brief Access a part of the buffer
                /// @param Index How from from the 0 aligned beginning of the buffer would you like to access.
                /// @return A Reference to the specific Byte the Index passed refers to
                /// @note When compiled as Debug this can throw a @ref MemoryOutOfBoundsException if the index is to high (or cast from a negative
                Byte& operator[] (Whole Index);

                /// @brief Append another group of arbitrary data onto this one.
                /// @details Allocates space equal to the size of both buffers, Copies this Buffers data
                /// into it, then copies the other buffers data, then deallocates any space this buffer
                /// may have had allocated preivously.
                /// @param OtherBuffer A pointer to a region of memory to be copied
                /// @param ByteSize How big in bytes is the Buffer
                void Concatenate(const Byte* OtherBuffer, Whole ByteSize);

                /// @brief Concatenate another buffer onto this one
                /// @details This calls @ref Concatenate(const Byte*, Whole)
                /// @param BufferFromAnotherMother A buffer to copy and append onto this one.
                void Concatenate(const BinaryBuffer BufferFromAnotherMother);

                /// @details An easier way to call @ref Concatenate(const Byte*, Whole)
                /// @param RH The other Buffer to copy/append.
                /// @return A Reference to this buffer to allow operator chaining.
                BinaryBuffer& operator+=(const BinaryBuffer& RH);

                /// @brief Even though this class is intended to have its internals modifieddirectly in some cases, In normal cases accessor are nice to have.
                /// @return Get the size as a whole
                Whole GetSize() const;

        };

        /// @brief Is a character a valid Base64 character
        /// @param Char8 a single char to check if it could possibly be valid base64
        /// @return True if the character could be part of a valid block of Base64 text, false otherwise
        Boole MEZZ_LIB IsBase64(unsigned char Char8);

        /// @brief Converts the contents of a String into a String containing a base64 encoded String
        /// @param Unencoded A String/binary to be encoded
        /// @return A string containing base64.
        String MEZZ_LIB Base64Encode(String const& Unencoded);

        /// @brief Convert a binary buffer to a base64 String.
        /// @param Buffer A BinaryBuffer to base64 encode.
        /// @return A string containing base64.
        String MEZZ_LIB Base64Encode(BinaryBuffer const& Buffer);

        /// @brief Convert a binary buffer to a Base64 string.
        /// @param BytesToEncode A pointer to the beginning of the buffer.
        /// @param Length The length of the bufferin bytes.
        /// @return A String containing the base64 encoded binary
        String MEZZ_LIB Base64Encode(UInt8 const* BytesToEncode, unsigned int Length);

        /// @brief Convert Base64 stuff back to binary
        /// @param EncodedString The results of a previous function like @ref Base64Encode to be converted back to binary
        /// @return A String with the raw bianry
        BinaryBuffer MEZZ_LIB Base64Decode(String const& EncodedString);

        /// @brief From an encoded string get the exact size of the decode binary in 8bit bytes
        /// @param EncodedString The base64 encoded string
        /// @return This returns the exact length of the result once it is decoded. The binary is about 3/4 of the base64 size, but that can be off by just enough to make memory allocation an issue if not calcualated carefully.
        Whole MEZZ_LIB PredictBinarySizeFromBase64String(String const& EncodedString);

        /// @brief From the size of a binary get the exact size in 8bit bytes.
        /// @param Length The bytelength of the item before Base64 encoding.
        /// @return This returns the exact length of the result once it is encoded. The Base64 is about 133% of the binary size, but that can be off by just enough to make memory allocation an issue if not calcualated carefully.
        Whole MEZZ_LIB PredictBase64StringSizeFromBinarySize(Whole Length);

    } //stringtool
}//Mezzanine

#endif
