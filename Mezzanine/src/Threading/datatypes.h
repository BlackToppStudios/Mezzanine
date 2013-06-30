// The DAGFrameScheduler is a Multi-Threaded lock free and wait free scheduling library.
// © Copyright 2010 - 2013 BlackTopp Studios Inc.
/* This file is part of The DAGFrameScheduler.

    The DAGFrameScheduler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The DAGFrameScheduler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The DAGFrameScheduler.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'doc' folder. See 'gpl.txt'
*/
/* We welcome the use of the DAGFrameScheduler to anyone, including companies who wish to
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
#ifndef _datatypes_h
#define _datatypes_h

/// @file
/// @brief This file declares a few typedefs to allow flexibility, in case a change is required/desired.
/// @details When integrating into the Mezzanine this file is dropped and the datatypes.h in the
/// Mezzanine is used instead.

#include "compilerthreadcompat.h"

#include <string>

#ifdef _MEZZ_CPP11_PARTIAL_
#include <cstdint>
#else
#include <stdint.h>
#endif

namespace Mezzanine
{

    /// @brief A type that any pointer can be converted to and back from, and insures after the conversion back it will be identical.
    #ifdef _MEZZ_CPP11_PARTIAL_
    typedef std::intptr_t ConvertiblePointer;
    #else
    typedef intptr_t ConvertiblePointer;
    #endif

    /// @typedef Int8
    /// @brief An 8-bit integer.
    typedef int8_t Int8;
    /// @typedef UInt8
    /// @brief An 8-bit unsigned integer.
    typedef uint8_t UInt8;
    /// @typedef Int16
    /// @brief An 16-bit integer.
    typedef int16_t Int16;
    /// @typedef UInt16
    /// @brief An 16-bit unsigned integer.
    typedef uint16_t UInt16;
    /// @typedef Int32
    /// @brief An 32-bit integer.
    typedef int32_t Int32;
    /// @typedef UInt32
    /// @brief An 32-bit unsigned integer.
    typedef uint32_t UInt32;
    /// @typedef Int64
    /// @brief An 64-bit integer.
    typedef int64_t Int64;
    /// @typedef UInt64
    /// @brief An 64-bit unsigned integer.
    typedef uint64_t UInt64;


    /// @brief Whole is an unsigned integer, it will be at least 32bits in size. In general it will be the most efficient unsigned type for math.
    typedef unsigned int Whole;
    /// @brief Integer is a signed integer. In general it will be the most efficient signed type for math.
    typedef int Integer;
    /// @brief String will provide most of the functionality of std::string but could be implemented in a variety of ways to optimize it.
    typedef std::string String;


    /// @brief A large integer type suitable for compile time math and long term microsecond time keeping.
    /// @details For reference when this is a 64 bit integer, it can store a number between −9,223,372,036,854,775,808 and 9,223,372,036,854,775,807.
    /// In seconds that is approximately 292,277,000,000 years and the universe is only 14,600,000,000 years old. So this is good for any time between
    /// 20x the age of the universe before and after the beginning of any chosen epoch. Even if used to track nanoseconds it should be good for
    /// 292 years.
    #ifdef _MEZZ_CPP11_PARTIAL_
        typedef intmax_t  MaxInt;
    #else
        typedef long long MaxInt;
    #endif

    /// @brief In case we ever replace the stringstream with another class, this will allow us to swap it out.
    /// @details This will always support <<, str() but may lose support for formatting functions like std::hex.
    typedef std::stringstream Logger;
}//Mezzanine

#endif
