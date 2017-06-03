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

#ifndef _unicode_cpp
#define _unicode_cpp

#include "unicode.h"

using namespace std;

/// @file
/// @brief This contains simple tools for indexing with UTF8 characters swiftly

namespace Mezzanine
{
    namespace Unicode
    {
        namespace
        {
            /// @internal
            /// Checks following bytes for UTF8 validity.
            Boole CheckAsFollowingBytes(Int32 HowMany, const char* FirstChar)
            {
                for(Int32 Count = 1; HowMany>Count; Count++)
                {
                    if ( (*(FirstChar+Count) & High2Bit) != High1Bit)
                        { return false; }
                }
                return true;
            }
        }

        String AsBitString(Int32 IntToPrint)
        {
            string Results;
            for(int Counter=31; Counter>=0; Counter--)
            {
                if ( ((Counter+1)%8)==0 && Counter!=31)
                    { Results += " "; }
                if ( (1<<Counter) & IntToPrint )
                    { Results += "1"; }
                else
                    { Results += "0"; }
            }
            return Results;
        }

        Int32 GetIntFromCharacter(Int32& BytesUsed, const char* CurrentCharacter)
        {

            if( (*CurrentCharacter & High1Bit) == 0)
            {
                BytesUsed=1;
                char Results = *CurrentCharacter;
                return Results;
            }

            for(Int32 Counter=2; Counter<7; Counter++)
            {
                if((*CurrentCharacter & IterableHighBits[Counter+1]) == IterableHighBits[Counter])
                {
                    if( CheckAsFollowingBytes(Counter-1, CurrentCharacter) )
                    {
                        BytesUsed=Counter;
                        Int32 Results = (CurrentCharacter[0] & IterableLowBits[8-Counter]) << (6*(Counter-1));
                        for(Int32 Remains = 1; Remains<Counter; Remains++)
                            { Results |= (CurrentCharacter[Remains] & Low6Bit) << (6*(Counter-Remains-1)); }
                        return Results;
                    }
                    return -1;
                }
            }

            return -1;
        }

        ///
        Int32 GetCharacterFromInt(char* Destination, Int32 BytesUsable, Int32 ByteSequence)
        {
            ///
            if(ByteSequence<UTF8ByteRange1Max)
            {
                //1 byte or invalid
                if(0>ByteSequence)
                    { return -1; }
                else
                {
                    // 1 byte
                    if (1>BytesUsable)
                        { return -1; }
                    char Ascii = (char)ByteSequence;
                    Destination[0]=Ascii;
                    return 1;
                }
            }
            else
            {
                //more than one
                if(ByteSequence<UTF8ByteRange2Max)
                {
                    // 2 bytes
                    if (2>BytesUsable)
                        { return -1; }
                    Int32 Results = UTF8Null2ByteBase;
                    Results |= (ByteSequence & Int32(Low6Bit));
                    Results |= (ByteSequence & (Int32(Low5Bit)<<6) ) << 2;
                    char* Bytes = (char*)&Results;
                    Destination[0]=Bytes[1];
                    Destination[1]=Bytes[0];
                    return 2;
                }
                else
                {
                    if(ByteSequence<UTF8ByteRange3Max)
                    {
                        // 3 or more
                        if (3>BytesUsable)
                            { return -1; }
                        Int32 Results = UTF8Null3ByteBase;
                        Results |= (ByteSequence & Int32(Low6Bit));
                        Results |= (ByteSequence & (Int32(Low6Bit)<<6) ) << 2;
                        Results |= (ByteSequence & (Int32(Low4Bit)<<12) ) << 4;
                        char* Bytes = (char*)&Results;
                        Destination[0]=Bytes[2];
                        Destination[1]=Bytes[1];
                        Destination[2]=Bytes[0];
                        return 3;
                    }
                    else
                    {
                        if(ByteSequence<UTF8ByteRange4Max)
                        {
                            // 4 or more
                            if (4>BytesUsable)
                                { return -1; }
                            Int32 Results = UTF8Null4ByteBase;
                            Results |= (ByteSequence & Int32(Low6Bit));
                            Results |= (ByteSequence & (Int32(Low6Bit)<<6) ) << 2;
                            Results |= (ByteSequence & (Int32(Low6Bit)<<12) ) << 4;
                            Results |= (ByteSequence & (Int32(Low3Bit)<<18) ) << 6;
                            char* Bytes = (char*)&Results;
                            Destination[0]=Bytes[3];
                            Destination[1]=Bytes[2];
                            Destination[2]=Bytes[1];
                            Destination[3]=Bytes[0];
                            return 4;
                        }
                        else
                            { return -1; }

                    }
                }
            }
            return -1;
        }
    }//Unicode
}//Mezzanine

#endif
