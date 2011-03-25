//© Copyright 2010 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
//Any Special data types that we need will get declared right here
///////////////////////////////////////
#include "crossplatformexport.h"

#include <string>
#include <sstream>
#include <vector>
#include <map>

/// @internal
/// @brief Forward declaration for SDL compatibilty
union SDL_Event;

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    // Datatypes
    ///////////////////////////////////////

    /// @internal
    /// @brief Forward declaration for use with soundset
    class Sound;

    /// @typedef Real
    /// @brief A Datatype used to represent a real floating point number
    /// @details This Datatype is currently a typedef to a float, This is to match
    /// our compilations of Ogre (rendering subsystem ogre::Real), and Bullet (physics
    /// subsystem, btScalar). With a recompilation of all the subsystems and  this
    /// there is no theoretical reason why this could not be changed to a
    /// double, or even something more extreme like a GMP datatype. Most likely this
    /// switch would require atleast some troubleshooting.
    typedef float Real;

    /// @typedef Whole
    /// @brief A Datatype used to represent an postive integer numbers
    /// @details This is a typedef to unsigned Long. but could be smaller in some situations
    typedef unsigned long Whole;

    /// @typedef Integer
    /// @brief A datatype use to represent any integer close to
    /// @details This is a typedef to int, but could int16 or smaller to improve performance in some situtations
    /// handheld platforms
    typedef int Integer;

    /// @typedef String
    /// @brief A Datatype used to a series of characters
    /// @details This is a typedef to std::string, but could change particularly if UTF16 or UTF32 support is desired. If this is
    /// changed, The Character typedef should be adjusted accordingly.
    typedef std::string String;

    /// @typedef ConstString
    /// @brief A Datatype used to a series of imutable characters
    /// @details This is a typedef to const String, but could change.
    typedef const String ConstString;

    /// @typedef Character
    /// @brief A datatype to represent one character.
    /// @details This should be a char if String is an std::string. The XML parser expects this to be either char or wchar_t
    /// and has not be test with external types. The XML Parser expects XML_WCHAR_MODE to be defined if wchar_t is used. A remarked
    /// definition of this exists near the Character typedef in datatypes.h
    typedef char Character;
    // #define XML_WCHAR_MODE   //UNREMARK ME if changing the above line to typedef wchar_t Character;

    /// @internal
    /// @typedef RawEvent
    /// @brief This is an internal datatype use to communicate with the User input Subsystem
    /// @details This is a typedef to SDL_Event. See the SDL Documentation for more details
    typedef SDL_Event RawEvent;

    /// @typedef SoundSet
    /// @brief This is a vector that stores sounds.
    /// @details This is a vector and can be use to store sounds that can be grouped together
    /// for similiar purposes or similiar content for easy tracking.
    typedef std::vector< Sound* > SoundSet;

    ///////////////////////////////////////////////////////////////////////////////
    // Simple conversion functions
    ///////////////////////////////////////

    /// @brief Converts whatever to a String as long as a streaming operator is available for it
    /// @param Datum The whatever to be converted
    /// @return A String with the converted data
    template<class T> String ToString( const T& Datum )
    {
        std::stringstream Converter;
        Converter << Datum;
        return Converter.str();
    }

    /// @brief Converts whatever to a Whole as long as the proper streaming operators are available for it
    /// @param Datum The whatever to be converted
    /// @return A Whole with the converted data
    template<class T> Whole ToWhole( const T& Datum )
    {
        std::stringstream Converter;
        Converter << Datum;
        Whole Results;
        Converter >> Results;
        return Results;
    }

    /// @brief Converts whatever to an Integer as long as the proper streaming operators are available for it
    /// @param Datum The whatever to be converted
    /// @return An Integer with the converted data
    template<class T> Integer ToInteger( const T& Datum )
    {
        std::stringstream Converter;
        Converter << Datum;
        Integer Results;
        Converter >> Results;
        return Results;
    }

    /// @brief Converts whatever to an int as long as the proper streaming operators are available for it
    /// @param Datum The whatever to be converted
    /// @return An int with the converted data
    /// @details This exists for interacting with other libraies, in situations where changing the Integer Typedef could
    /// break things
    template<class T> int Toint( const T& Datum )
    {
        std::stringstream Converter;
        Converter << Datum;
        int Results;
        Converter >> Results;
        return Results;
    }

    /// @brief Converts whatever to an unsigned int as long as the proper streaming operators are available for it
    /// @param Datum The whatever to be converted
    /// @return An unsigned int with the converted data
    /// @details This exists for interacting with other libraies, in situations where changing the Integer Typedef could
    /// break things
    template<class T> unsigned int Tounsignedint( const T& Datum )
    {
        std::stringstream Converter;
        Converter << Datum;
        unsigned int Results;
        Converter >> Results;
        return Results;
    }

    /// @brief Converts whatever to a Real as long as the proper streaming operators are available for it
    /// @param Datum The whatever to be converted
    /// @return A Real with the converted data
    template<class T> Real ToReal( const T& Datum )
    {
        std::stringstream Converter;
        Converter << Datum;
        Real Results;
        Converter >> Results;
        return Results;
    }

    /// @brief Converts whatever to a float as long as the proper streaming operators are available for it
    /// @param Datum The whatever to be converted
    /// @return A float with the converted data
    /// @details This exists for interacting with other libraies, in situations where changing the Real Typedef could
    /// break things
    template<class T> float Tofloat( const T& Datum )
    {
        std::stringstream Converter;
        Converter << Datum;
        float Results;
        Converter >> Results;
        return Results;
    }

    /// @brief Converts whatever to a double as long as the proper streaming operators are available for it
    /// @param Datum The whatever to be converted
    /// @return A double with the converted data
    /// @details This exists for interacting with other libraies, in situations where changing the Typedefs could break things
    template<class T> double Todouble( const T& Datum )
    {
        std::stringstream Converter;
        Converter << Datum;
        double Results;
        Converter >> Results;
        return Results;
    }

    /// @brief Concatenates 2 Strings
    /// @param Front The first String
    /// @param Back The last String
    /// @return A string containing the other Strings passed attached end to end
    String StringCat(const String& Front, const String& Back);

    /// @brief Concatenates 3 Strings
    /// @param Front The first String
    /// @param Middle A String in the middle
    /// @param Back The last String
    /// @return A string containing the other Strings passed attached end to end
    String StringCat(const String& Front, const String& Middle, const String& Back);

    /// @brief Concatenates 3 Strings
    /// @param Front The first String
    /// @param Middle1 The second string
    /// @param Middle2 The third string
    /// @param Back The last String
    /// @return A string containing the other Strings passed attached end to end
    String StringCat(const String& Front, const String& Middle1, const String& Middle2, const String& Back);

} // \phys

#endif
