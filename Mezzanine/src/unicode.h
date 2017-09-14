// The UTF8 Conversion Library is a small library aiding in the converstion from raw text to UTF8 text.
// © Copyright 2010 - 2017 BlackTopp Studios Inc.
/* This file is part of The UTF8 Conversion Library.

    The UTF8 Conversion Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The UTF8 Conversion Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The UTF8 Conversion Library.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'doc' folder. See 'gpl.txt'
*/
/* We welcome the use of the UTF8 Conversion Library to anyone, including companies who wish to
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

#ifndef _unicode_h
#define _unicode_h

#include "datatypes.h"
#include <iostream>

/// @file
/// @brief This contains simple tools for indexing with UTF8 characters swiftly

namespace Mezzanine
{
    /// @brief A dumping ground for any feature that only relates to unicode work.
    /// @details Unicode is a series of numbers that correlate to glyphs. These numbers
    /// are seperate from any binary representation. Common binary represenations of the
    /// numbers are UTF8, UTF16, and UTF32. This library supports UTF8, which uses between
    /// 1 and 4 bytes to represent and valid Unicode glyph. The tools provided here allow
    /// conversion between The raw Unicode value, which is useful for algorithms, and
    /// its UTF8 representation, which is useful for storage and transmission.
    namespace Unicode
    {
        const UInt8 High1Bit = (1<<7);            ///< 1xxxxxxx - Is used compared against high 2 bits to determine if in middle of byte
        const UInt8 High2Bit = High1Bit | (1<<6); ///< 11xxxxxx
        const UInt8 High3Bit = High2Bit | (1<<5); ///< 111xxxxx
        const UInt8 High4Bit = High3Bit | (1<<4); ///< 1111xxxx
        const UInt8 High5Bit = High4Bit | (1<<3); ///< 11111xxx
        const UInt8 High6Bit = High5Bit | (1<<2); ///< 111111xx
        const UInt8 High7Bit = High6Bit | (1<<1); ///< 1111111x
        const UInt8 High8Bit = High7Bit | 1;      ///< 11111111

        /// @brief The index of this array corresponds to the amount of high bits that are set.
        const UInt8 IterableHighBits[] = {0, High1Bit, High2Bit, High3Bit, High4Bit, High5Bit, High6Bit, High7Bit, High8Bit};


        const UInt8 Low1Bit = (1);                ///< xxxxxxx1
        const UInt8 Low2Bit = Low1Bit | (1<<1);   ///< xxxxxx11
        const UInt8 Low3Bit = Low2Bit | (1<<2);   ///< xxxxx111
        const UInt8 Low4Bit = Low3Bit | (1<<3);   ///< xxxx1111
        const UInt8 Low5Bit = Low4Bit | (1<<4);   ///< xxx11111
        const UInt8 Low6Bit = Low5Bit | (1<<5);   ///< xx111111
        const UInt8 Low7Bit = Low6Bit | (1<<6);   ///< x1111111
        const UInt8 Low8Bit = Low7Bit | (1<<7);   ///< 11111111

        /// @brief The index of this array corresponds to the amount of low bits that are set.
        const UInt8 IterableLowBits[] = {0, Low1Bit, Low2Bit, Low3Bit, Low4Bit, Low5Bit, Low6Bit, Low7Bit, Low8Bit};

        const Int32 UTF8ByteRange1Max = 127;        ///< The maximum Unicode codepoint that can fit into a single UTF8 byte. Equal to 2^7-1.
        const Int32 UTF8ByteRange2Max = 4097;       ///< The maximum Unicode codepoint that can fit into 2 UTF8 bytes. Equal to 2^11-1.
        const Int32 UTF8ByteRange3Max = 65535;      ///< The maximum Unicode codepoint that can fit into 3 UTF8 bytes. Equal to 2^16-1.
        const Int32 UTF8ByteRange4Max = 2097151;    ///< The maximum Unicode codepoint that can fit into 4 UTF8 bytes. Equal to 2^21-1.

        const UInt32 High1bytes = 0xFF000000;       ///< The Highest byte of an integer on this system
        const UInt32 High2bytes = 0xFFFF0000;       ///< The Highest 2 bytes of an integer on this system
        const UInt32 High3bytes = 0xFFFFFF00;       ///< The Highest 3 bytes of an integer on this system

        const UInt32 UTF8Null2ByteBase = 49280;     ///< This is the numerical representation 0 in a two UTF8 Sequence. Is equal to 11000000 10000000
        const UInt32 UTF8Null3ByteBase = 14712960;  ///< This is the numerical representation 0 in a three UTF8 Sequence. Is equal to 11100000 10000000 10000000
        const UInt32 UTF8Null4ByteBase = 4034953344;///< This is the numerical representation 0 in a four UTF8 Sequence. Is equal to 11110000 10000000 10000000 10000000


        /// @brief A helper function that produces a human readable sequence of ' ', '1' and '0' characters.
        /// @param IntToPrint A 32 bit integer that will be used to create the sequence.
        /// @return A Mezzanine::String containing '1' and '0' characters with a space every eight digits.
        String AsBitString(Int32 IntToPrint);

        /// @brief Get a number suitable for using in an index from a character string.
        /// @param BytesUsed The value of this variable is ignored and overwritten with the amount of bytes consumed from CurrentCharacter.
        /// @param CurrentCharacter a pointer to a c style string.
        /// @return If the character pointer to is the beginning of a valid UTF8 character a number suitable for using in an index is returned, otherwise some negative value is returned.
        Int32 GetIntFromCharacter(Int32& BytesUsed, const char* CurrentCharacter);

        /// @brief Convert a number that represents any valid unicode value into its UTF8 representation.
        /// @param Destination The place to write the results. Never more than 4 bytes will be written. Null terminators are not written.
        /// @param BytesUsable How many byte of the Destination are usable.
        /// @param ByteSequence The integer value to convert to a UTF8 unicode representation. This sequence must be representable in 21 or fewer bits(<4194304) to be valid.
        /// @return The amount of bytes written to destination or -1 on error. This will never be more than 4,
        Int32 GetCharacterFromInt(char* Destination, Int32 BytesUsable, Int32 ByteSequence);
    }//Unicode
}//Mezzanine

#endif
