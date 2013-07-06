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
#ifndef _consolestringmanipulation_h
#define _consolestringmanipulation_h

/// @file
/// @brief Some string manipulation functions geared towards console output for use in the Unit Tests.

#include "datatypes.h"

namespace Mezzanine
{
    namespace Testing
    {
        /// @brief Take the whitespace off the end of a String
        /// @param t The Text to clean up.
        /// @param If " asdf " is passed " asdf" is returned. Also removes carriage returns, newlines and tabs. Does not use the locale.
        Mezzanine::String rtrim(const Mezzanine::String &t);

        /// @brief Creates some blank spaces, useful for controlling the vertical location of console text.
        /// @param Leader The string on the beginning of the line.
        /// @param Column The desired column that the padding should lead toward
        /// @return If "asdf" and 6 are passed this will return "  " (Two spaces).
        Mezzanine::String MakePadding(Mezzanine::String Leader, unsigned int Column);

        /// @brief Makes a c style stron all lowercase with respect to the current locale
        /// @param StringToConvert This string is actually changed.
        /// @return for convience purposes the string is also returned.
        char* AllLower(char* StringToConvert);

        /// @brief Get a String Corresponding to a passed bool
        /// @param i A bool
        /// @return "True" if true is passed, "False" if false is passed.
        Mezzanine::String BoolToString(bool i);

    }// Testing
}// Mezzanine

#endif
