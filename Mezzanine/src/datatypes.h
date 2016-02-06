// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _datatypes_h
#define _datatypes_h

///////////////////////////////////////////////////////////////////////////////
// Any Special data types that we need will get declared right here
/// @file
/// @brief All the definitions for datatypes as well as some basic conversion functions are defined
/// here. Additionally, this is where all of the other singular header inclusions go as well.
///////////////////////////////////////

/// @def MEZZANINE_CORE
/// @brief Only defined in the Mezzanine and none of the Sattelite libraries
/// @details Some parts of the code are in the Mezzanine and other libraries, Only
/// the Mezzanine defines this to allow that code to fully integrate its behaviour.
#define MEZZANINE_CORE 1

#include "crossplatformexport.h"
//#include "pstdint.h" // A reimplimentation of the C99 stdint.h for all compilers

// Standard Headers are not included in SWIG preprocessing
// Most std includes are centralized here to make modifying this list as simple as possible. Other
// standard includes that are not included here are in places that they are required and
// conditionally may not be compiled in. For example,

#ifndef SWIG
    #if defined( _MSC_VER )
        #include "pstdint.h"
    #else
        #include <stdint.h> //Not available in all version of mscv
    #endif
    #include <cstddef>

    #include <algorithm>
    #include <exception>
    #include <istream>
    #include <list>
    #include <map>
    #include <string>
    #include <sstream>
    #include <fstream>
    #include <set>
    #include <vector>
    #include <utility>
    #include <memory>
    #include <ostream>
#endif

#include "swig.h"

/// @internal
/// @brief Forward declaration for SDL compatibilty
union SDL_Event;

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // Datatypes
    ///////////////////////////////////////

    #ifdef _MEZZ_CPP11_PARTIAL_
        #ifndef SWIG
            //#include <cstdint>
        #endif
        /// @brief A type that any pointer can be converted to and back from, and insures after the
        /// conversion back it will be identical.
        typedef std::intptr_t ConvertiblePointer;
    #else
        #ifndef SWIG
            //#include <stdint.h>
        #endif
        /// @brief A type that any pointer can be converted to and back from, and insures after the
        /// conversion back it will be identical.
        typedef intptr_t ConvertiblePointer;
    #endif

    /// @brief An 8-bit integer.
    typedef int8_t Int8;
    /// @brief An 8-bit unsigned integer.
    typedef uint8_t UInt8;
    /// @brief An 16-bit integer.
    typedef int16_t Int16;
    /// @brief An 16-bit unsigned integer.
    typedef uint16_t UInt16;
    /// @brief An 32-bit integer.
    typedef int32_t Int32;
    /// @brief An 32-bit unsigned integer.
    typedef uint32_t UInt32;
    /// @brief An 64-bit integer.
    typedef int64_t Int64;
    /// @brief An 64-bit unsigned integer.
    typedef uint64_t UInt64;

    /// @brief A Datatype used to represent a real floating point number.
    /// @details This Datatype is currently a typedef to a float, This is to match
    /// our compilations of Ogre (rendering subsystem ogre::Real), and Bullet (physics
    /// subsystem, btScalar). With a recompilation of all the subsystems and  this
    /// there is no theoretical reason why this could not be changed to a
    /// double, or even something more extreme like a GMP datatype. Most likely this
    /// switch would require atleast some troubleshooting.
    /// @n @n
    /// This type will be word aligned and fast
    typedef float Real;
    /// @brief A Real number that is at least as precise as the Real and could be considerably
    /// moreso, perhaps Doubly precise.
    /// @n @n
    /// This type might be poorly aligned but very precise.
    typedef double PreciseReal;

    /// @brief Whole is an unsigned integer, it will be at least 32bits in size.
    /// @details This is a typedef to unsigned Long. but could be smaller in some situations.  In
    /// general it will be the most efficient unsigned type for math.
    typedef unsigned long Whole;
    /// @brief A datatype used to represent any integer close to.
    /// @details This is a typedef to int, but could int16 or smaller to improve performance in some situtations, In general it will be the most efficient signed type for math.
    typedef int Integer;

    /// @brief A datatype used to a series of characters.
    /// @details This is a typedef to std::string, but could change particularly if UTF16 or UTF32 support is desired. If this is
    /// changed, The Character typedef should be adjusted accordingly.
    typedef std::string String;
    /// @brief A wide version of the String typedef.
    /// @details Wide strings are seldom used, but often come in handy when dealing with Unicode strings.
    typedef std::wstring WideString;
    /// @brief A Datatype used to a series of imutable characters.
    /// @details This is a typedef to const String, but could change.
    typedef const String ConstString;

    /// @brief A datatype to represent one character.
    /// @details This should be a char if String is an std::string.
    typedef char Char8;

    /// @brief Generally acts a single bit, true or false
    /// @details Normally just a bool, but on some platform alignment matters more than size, so this could be as large as one cpu word in size.
    typedef bool Boole;

    /// @brief A Datatype used for streaming operations with strings.
    typedef std::stringstream StringStream;

    /// @brief In case we ever replace the stringstream with another class, this will allow us to swap it out.
    /// @details This will always support <<, str() but may lose support for formatting functions like std::hex.
    typedef std::stringstream Logger;

    /// @brief A large integer type suitable for compile time math and long term microsecond time keeping.
    /// @details For reference when this is a 64 bit integer, it can store a number between −9,223,372,036,854,775,808 and 9,223,372,036,854,775,807.
    /// In seconds that is approximately 292,277,000,000 years and the universe is only 14,600,000,000 years old. So this is good for any time between
    /// 20x the age of the universe before and after the beginning of any chosen epoch. Even if used to track nanoseconds it should be good for
    /// 292 years.
    #ifdef _MEZZ_CPP11_PARTIAL_
        typedef intmax_t MaxInt;
    #else
        typedef long long MaxInt;
    #endif

    /// @brief A datatype used to indicate a specific point in time, or a timestamp.
    /// @details This is made into it's own datatype in case we want to tweak the possible size for a timestamp.
    typedef UInt32 TimeMarker;

    ///////////////////////////////////////////////////////////////////////////////
    // Complex Data types
    ///////////////////////////////////////

    /// @brief This is a pair for the generic storage of a value and it's associated name.
    typedef std::pair< String, String > NameValuePair;

    /// @brief This is a datatype mostly used for describing settings or parameters that can't be declared in advance.
    /// @details This datatype uses the std::list container for it's storage.
    typedef std::list< NameValuePair > NameValuePairList;
    /// @brief This is a datatype mostly used for describing settings or parameters that can't be declared in advance.
    /// @details This datatype uses the std::map container for it's storage.
    typedef std::map< String, String > NameValuePairMap;

    /// @brief This is a simple datatype for a vector container of strings.
    typedef std::vector< String > StringVector;
    /// @brief This is a simple datatype for a set container of strings.
    typedef std::set< String > StringSet;

    /// @internal
    /// @brief This is an internal datatype use to communicate with the User input Subsystem.
    /// @details This is a typedef to SDL_Event. See the SDL Documentation for more details.
    typedef SDL_Event RawEvent;

    ///////////////////////////////////////////////////////////////////////////////
    // Simple conversion functions
    ///////////////////////////////////////

    /// @brief Catch all Lexigraphical Conversion.
    /// @param Datum A value of any type that will be converted.
    /// @return The value as if 'read' into the target type.
    /// @todo Overload ConvertTo for ToString Conversions.
    template<typename To, typename From>
    To ConvertTo(const From& Datum)
    {
        std::stringstream Converter;
        Converter << Datum;
        To Results;
        Converter >> Results;
        return Results;
    }

    /// @brief Converts whatever to a String as long as a streaming operator is available for it.
    /// @param Datum The whatever to be converted.
    /// @return A String with the converted data.
    template<class T> String ToString( const T& Datum )
    {
        std::stringstream Converter;
        Converter << Datum;
        return Converter.str();
    }

    /// @brief Converts whatever to a Whole as long as the proper streaming operators are available for it.
    /// @param Datum The whatever to be converted.
    /// @return A Whole with the converted data.
    template<class T> Whole ToWhole( const T& Datum )
        { return ConvertTo<Whole>(Datum); }

    /// @brief Converts whatever to an Integer as long as the proper streaming operators are available for it.
    /// @param Datum The whatever to be converted.
    /// @return An Integer with the converted data.
    template<class T> Integer ToInteger( const T& Datum )
        { return ConvertTo<Integer>(Datum); }

    /// @brief Converts whatever to an int as long as the proper streaming operators are available for it.
    /// @param Datum The whatever to be converted.
    /// @return An int with the converted data.
    /// @details This exists for interacting with other libraies, in situations where changing the Integer Typedef could
    /// break things.
    template<class T> int Toint( const T& Datum )
        { return ConvertTo<int>(Datum); }

    /// @brief Converts whatever to an unsigned int as long as the proper streaming operators are available for it.
    /// @param Datum The whatever to be converted.
    /// @return An unsigned int with the converted data.
    /// @details This exists for interacting with other libraies, in situations where changing the Integer Typedef could
    /// break things.
    template<class T> unsigned int Tounsignedint( const T& Datum )
        { return ConvertTo<unsigned int>(Datum); }

    /// @brief Converts whatever to a Real as long as the proper streaming operators are available for it.
    /// @param Datum The whatever to be converted.
    /// @return A Real with the converted data.
    template<class T> Real ToReal( const T& Datum )
        { return ConvertTo<Real>(Datum); }

    /// @brief Converts whatever to a Boole as long as the proper streaming operators are available for it
    /// @param Datum The whatever to be converted
    /// @return A Bool with the converted data
    template<class T> Boole ToBool( const T& Datum )
        { return ConvertTo<Boole>(Datum); }

    /// @brief Converts whatever to a float as long as the proper streaming operators are available for it
    /// @param Datum The whatever to be converted
    /// @return A float with the converted data
    /// @details This exists for interacting with other libraies, in situations where changing the Real Typedef could
    /// break things.
    template<class T> float Tofloat( const T& Datum )
        { return ConvertTo<float>(Datum); }

    /// @brief Converts whatever to a double as long as the proper streaming operators are available for it.
    /// @param Datum The whatever to be converted.
    /// @return A double with the converted data.
    /// @details This exists for interacting with other libraies, in situations where changing the Typedefs could break things.
    template<class T> double Todouble( const T& Datum )
        { return ConvertTo<double>(Datum); }
}//Mezzanine

#endif
