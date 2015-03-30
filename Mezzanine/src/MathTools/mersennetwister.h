// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
/*
   A C-program for MT19937, with initialization improved 2002/2/10.
   Coded by Takuji Nishimura and Makoto Matsumoto.
   This is a faster version by taking Shawn Cokus's optimization,
   Matthe Bellew's simplification, Isaku Wada's real version.

   Before using, initialize the state by using init_genrand(seed)
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.keio.ac.jp/matumoto/emt.html
   email: matumoto@math.keio.ac.jp
*/
/*
   C++ codes by Kohei Takeda (k-tak@letter.or.jp)
   Redistribution terms are the same as the original ones above.
*/
#ifndef _mathtoolsmersennetwister_h
#define _mathtoolsmersennetwister_h

#include "datatypes.h"

#ifndef SWIG
#include <ctime>
#include <cassert>
#endif

namespace Mezzanine
{
    namespace MathTools
    {
        /// @brief A set of types and constants for using a 32-bit Mersenne Twister PRNG.
        struct MT32Traits
        {
            /// @brief This typedef defines the type that should be used for unsigned integer operations.
            typedef UInt32 UIntType;
            /// @brief This typedef defines the type that should be used for signed integer operations.
            typedef Int32 IntType;

            /// @brief The bit size of the Integer type to be worked with.
            static const Integer WordSize = 32;
            /// @brief The number of elements needed in the state array to store 19937 bits.
            static const Whole StateSize = 624;
            /// @brief Elements are transformed using other values in the sequence that are 'ShiftSize' elements away on each twist.
            static const Whole ShiftSize = 397;
            /// @brief This describes the maximum value of a varible casted from this traits Integer type to a double precision float.
            static const UIntType MaxDoubleValue = 4294967295U; //2^32-1
            /// @brief The initialization multiplier used to seed the state when a single value is used as seed.
            static const UIntType InitValue = 1812433253U;
            /// @brief The seed to be used when the Mersenne Twister is initialized with an array.
            static const UIntType ArrayInitSeed = 19650218U;
            /// @brief The multiplier to use when initializing the state array on the first pass.
            static const UIntType ArrayInitValue_1 = 1664525U;
            /// @brief The multiplier to use when initializing the state array on the second pass.
            static const UIntType ArrayInitValue_2 = 1566083941U;

            /// @brief Twists the values of numbers in the state to randomize them on initialization.
            /// @param U The number to be twisted/replaced in the Mersenne state.
            /// @param V The number following 'U' in the Mersenne state.
            /// @return Returns the twisted result as an Integer.
            static UIntType Twist(const UIntType& U, const UIntType& V)
            {
                static UIntType Mag01[2] = {0U, 0x9908b0dfU};
                return ( ( ( ( U & 0x80000000U ) | ( V & 0x7fffffffU ) ) >> 1 ) ^ Mag01[ V & 1 ] );
            }

            /// @brief Scrables a value in the Mersenne state into a returnable value.
            /// @param Y The value from the Mersenne state to be scrambled.
            /// @return Returns the tempered return as an Integer.
            static UIntType Temper(UIntType Y)
            {
                Y ^= ( Y >> 11 );                // Tempering U
                Y ^= ( Y << 7 ) & 0x9d2c5680U;   // Tempering S & Tempering B
                Y ^= ( Y << 15 ) & 0xefc60000U;  // Tempering T & Tempering C
                Y ^= ( Y >> 18 );                // Tempering L

                return Y;
            }
        };//MT32Traits

        /// @brief A set of types and constants for using a 64-bit Mersenne Twister PRNG.
        struct MT64Traits
        {
            /// @brief This typedef defines the type that should be used for unsigned integer operations.
            typedef UInt64 UIntType;
            /// @brief This typedef defines the type that should be used for signed integer operations.
            typedef Int64 IntType;

            /// @brief The bit size of the int type to be worked with.
            static const Integer WordSize = 64;
            /// @brief The number of elements needed in the state array to store 19937 bits.
            static const Whole StateSize = 312;
            /// @brief Elements are transformed using other values in the sequence that are 'ShiftSize' elements away on each twist.
            static const Whole ShiftSize = 156;
            /// @brief This describes the maximum value of a varible casted from this traits Integer type to a double precision float.
            static const UIntType MaxDoubleValue = 9007199254740991ULL; // 2^53-1
            /// @brief The initialization multiplier used to seed the state when a single value is used as seed.
            static const UIntType InitValue = 6364136223846793005ULL;
            /// @brief The seed to be used when the Mersenne Twister is initialized with an array.
            static const UIntType ArrayInitSeed = 19650218ULL;
            /// @brief The multiplier to use when initializing the state array on the first pass.
            static const UIntType ArrayInitValue_1 = 3935559000370003845ULL;
            /// @brief The multiplier to use when initializing the state array on the second pass.
            static const UIntType ArrayInitValue_2 = 2862933555777941757ULL;

            /// @brief Twists the values of numbers in the state to randomize them on initialization.
            /// @param U The number to be twisted/replaced in the Mersenne state.
            /// @param V The number following 'U' in the Mersenne state.
            /// @return Returns the twisted result as an Integer.
            static UIntType Twist(const UIntType& U, const UIntType& V)
            {
                static UIntType Mag01[2] = {0ULL, 0xB5026F5AA96619E9ULL};
                return ( ( ( ( U & 0xFFFFFFFF80000000ULL ) | ( V & 0x7FFFFFFFULL ) ) >> 1 ) ^ Mag01[ V & 1 ] );
            }

            /// @brief Scrables a value in the Mersenne state into a returnable value.
            /// @param Y The value from the Mersenne state to be scrambled.
            /// @return Returns the tempered return as an Integer.
            static UIntType Temper(UIntType Y)
            {
                Y ^= ( Y >> 29 ) & 0x5555555555555555ULL;
                Y ^= ( Y << 17 ) & 0x71D67FFFEDA60000ULL;
                Y ^= ( Y << 37 ) & 0xFFF7EEE000000000ULL;
                Y ^= ( Y >> 43 );

                return Y;
            }
        };//MT64Traits

        /// @brief
        /// @details
        template <typename Traits>
        class MersenneTwisterGenerator
        {
        public:
            /// @brief The unsigned Integer type to be used by the Mersenne Twister.
            typedef typename Traits::UIntType UIntType;
            /// @brief The signed Integer type to be used by the Mersenne Twister.
            typedef typename Traits::IntType IntType;
        protected:
            /// @internal
            /// @brief A pointer to the collection of numbers to be twisted to generate psuedo random numbers.
            UIntType* State;
            /// @internal
            /// @brief A pointer to the next value in the state to generate a psuedo random number from.
            UIntType* Next;
            /// @internal
            /// @brief The number of elements remaining to the left of the current value in the state.
            Whole Left;

            /// @internal
            /// @brief Generates the series of values to use for generating psuedo random numbers.
            void NextState()
            {
                UIntType* CurrVal = this->State;
                Whole CurrIndex;

                this->Left = Traits::StateSize;
                this->Next = this->State;

                Integer ShiftDist = static_cast<Integer>( Traits::ShiftSize );
                for( CurrIndex = Traits::StateSize - Traits::ShiftSize + 1 ; --CurrIndex ; ++CurrVal )
                {
                    *CurrVal = CurrVal[ShiftDist] ^ Traits::Twist(CurrVal[0],CurrVal[1]);
                }

                ShiftDist = static_cast<Integer>( Traits::ShiftSize ) - static_cast<Integer>( Traits::StateSize );
                for( CurrIndex = Traits::ShiftSize ; --CurrIndex ; ++CurrVal )
                {
                    *CurrVal = CurrVal[ShiftDist] ^ Traits::Twist(CurrVal[0],CurrVal[1]);
                }

                *CurrVal = CurrVal[ShiftDist] ^ Traits::Twist(CurrVal[0],this->State[0]);
            }
        public:
            /// @brief Blank constructor.
            MersenneTwisterGenerator()
            {
                this->Left = 1;
                this->Next = NULL;
                this->State = new UIntType[Traits::StateSize];
                this->Initialize( (UIntType)time(NULL) );
            }
            /// @brief Seeded constructor.
            /// @param Seed The seed to use for twisting the series of numbers generated.
            MersenneTwisterGenerator(const UIntType Seed)
            {
                this->Left = 1;
                this->Next = NULL;
                this->State = new UIntType[Traits::StateSize];
                this->Initialize(Seed);
            }
            /// @brief Array constructor.
            /// @remarks If the end of the InitKeys array is reached (which will happen if it is smaller than the state) then it will wrap around back to the beginning and reuse values.
            /// @param InitKeys An array of modifiers to use for twisting the series of numbers generated.
            /// @param KeyLen The length of the 'InitKeys' array.
            MersenneTwisterGenerator(UIntType InitKeys[], const Whole KeyLen)
            {
                this->Left = 1;
                this->Next = NULL;
                this->State = new UIntType[Traits::StateSize];
                this->Initialize(InitKeys,KeyLen);
            }
            /// @brief Class destructor.
            virtual ~MersenneTwisterGenerator()
            {
                if( this->State ) {
                    delete[] this->State;
                }
            }

            /// @brief Initializes the state for the number generator.
            /// @param Seed The seed to use for twisting the series of numbers generated.
            void Initialize(const UIntType Seed)
            {
                assert(sizeof(UIntType) * 8 == (size_t)Traits::WordSize);

                this->State[0] = Seed;
                for( Whole CurrIndex = 1 ; CurrIndex < Traits::StateSize ; ++CurrIndex )
                {
                    this->State[CurrIndex] = ( Traits::InitValue * ( this->State[ CurrIndex - 1 ] ^ ( this->State[ CurrIndex - 1 ] >> ( Traits::WordSize - 2 ) ) ) + static_cast<UIntType>(CurrIndex) );
                }
                this->Left = 1;
            }
            /// @brief Initializes the state for the number generator.
            /// @remarks If the end of the InitKeys array is reached (which will happen if it is smaller than the state) then it will wrap around back to the beginning and reuse values.
            /// @param InitKeys An array of modifiers to use for twisting the series of numbers generated.
            /// @param KeyLen The length of the 'InitKeys' array.
            void Initialize(UIntType InitKeys[], const Whole KeyLen)
            {
                this->Initialize(Traits::ArrayInitSeed);

                Whole StateIndex = 1;
                Whole KeyIndex = 0;
                Whole LoopCount = ( Traits::StateSize > KeyLen ? Traits::StateSize : KeyLen );

                for(  ; LoopCount ; LoopCount-- )
                {
                    this->State[StateIndex] = ( this->State[StateIndex] ^ ( ( this->State[ StateIndex - 1 ] ^ ( this->State[ StateIndex - 1 ] >> ( Traits::WordSize - 2 ) ) ) * Traits::ArrayInitValue_1 ) )
                                              + InitKeys[KeyIndex] + static_cast<UIntType>(KeyIndex); /* non linear */

                    StateIndex++;
                    KeyIndex++;

                    if( StateIndex >= Traits::StateSize ) {
                        this->State[0] = this->State[ Traits::StateSize - 1 ];
                        StateIndex = 1;
                    }

                    if( KeyIndex >= KeyLen ) {
                        KeyIndex = 0;
                    }
                }

                for( LoopCount = Traits::StateSize - 1 ; LoopCount ; LoopCount-- )
                {
                    this->State[StateIndex] = ( this->State[StateIndex] ^ ( ( this->State[ StateIndex - 1 ] ^ ( this->State[ StateIndex - 1 ] >> ( Traits::WordSize - 2 ) ) ) * Traits::ArrayInitValue_2 ) )
                                              - static_cast<UIntType>(StateIndex); /* non linear */

                    StateIndex++;

                    if( StateIndex >= Traits::StateSize ) {
                        this->State[0] = this->State[ Traits::StateSize - 1 ];
                        StateIndex = 1;
                    }
                }

                /* MSB is 1; assuring non-zero initial array */
                this->State[0] = static_cast<UIntType>(1) << ( Traits::WordSize - 1 );
                this->Left = 1;
            }

            /// @brief Generates a random unsigned Integer.
            /// @remarks The range in which the number can be generated is the range of values allowed by the type 'UIntType'. @n
            /// [0,2^Traits::WordSize-1].
            /// @return Returns a randomly generated UIntType.
            UIntType GenerateUInt()
            {
                if( --this->Left == 0 ) {
                    this->NextState();
                }
                return Traits::Temper( *(this->Next)++ );
            }
            /// @brief Generates a random signed Integer.
            /// @remarks The range in which the number can be generated is 0 to +max for type 'IntType'. @n
            /// [0,2^(Traits::WordSize-1)-1].
            /// @return Returns a randomly generated UIntType.
            IntType GenerateInt()
            {
                if( --this->Left == 0 ) {
                    this->NextState();
                }
                return static_cast<IntType>( Traits::Temper( *(this->Next)++ ) >> 1 );
            }

            /// @brief Generates a random PreciseReal.
            /// @remarks The range in which the number can be generated is the range of values allowed by the type 'PreciseReal'. @n
            /// (0,1).
            /// @return Returns a randomly generated PreciseReal.
            PreciseReal GenerateReal()
            {
                if( --this->Left == 0) {
                    this->NextState();
                }
                if( Traits::WordSize > 52 ) {
                    return ( ( static_cast<PreciseReal>( Traits::Temper( *(this->Next)++ ) >> ( Traits::WordSize - 52 ) ) + 0.5 ) * ( 1.0 / 4503599627370496.0 ) );
                } else {
                    return ( ( static_cast<PreciseReal>( Traits::Temper( *(this->Next)++ ) ) + 0.5 ) * ( 1.0 / ( static_cast<PreciseReal>( Traits::MaxDoubleValue ) + 1.0 ) ) );
                }
            }
            /// @brief Generates a random PreciseReal.
            /// @remarks The range in which the number can be generated is 0.0 to 1.0. @n
            /// [0,1].
            /// @return Returns a randomly generated PreciseReal.
            PreciseReal GenerateScaledReal()
            {
                if( --this->Left == 0 ) {
                    this->NextState();
                }
                if( Traits::WordSize > 53 ) {
                    return ( static_cast<PreciseReal>( Traits::Temper( *(this->Next)++ ) >> ( Traits::WordSize - 53 ) ) * ( 1.0 / 9007199254740991.0 ) );
                }else{
                    return ( static_cast<PreciseReal>( Traits::Temper( *(this->Next)++ ) ) * ( 1.0 / static_cast<PreciseReal>( Traits::MaxDoubleValue ) ) );
                }
            }
            /// @brief Generates a random PreciseReal.
            /// @remarks The range in which the number can be generated is 0.0 to +max for type 'PreciseReal'. @n
            /// [0,1).
            /// @return Returns a randomly generated PreciseReal.
            PreciseReal GeneratePositiveReal()
            {
                if( --this->Left == 0 ) {
                    this->NextState();
                }
                if( Traits::WordSize > 53 ) {
                    return ( static_cast<PreciseReal>( Traits::Temper( *(this->Next)++ ) >> ( Traits::WordSize - 53 ) ) * ( 1.0 / 9007199254740992.0 ) );
                } else {
                    return ( static_cast<PreciseReal>( Traits::Temper( *(this->Next)++ ) ) * ( 1.0 / ( static_cast<PreciseReal>( Traits::MaxDoubleValue ) + 1.0 ) ) );
                }
            }
        };//MersenneTwisterGenerator

        /// @brief Convenience type for a 32-bit Mersenne Twister.
        typedef MersenneTwisterGenerator<MT32Traits> MersenneTwisterGenerator32;
        /// @brief Convenience type for a 64-bit Mersenne Twister.
        typedef MersenneTwisterGenerator<MT64Traits> MersenneTwisterGenerator64;
    }//MathTools
}//Mezzanine

#endif
