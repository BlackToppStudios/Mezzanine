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
#ifndef _consolestringmanipulation_cpp
#define _consolestringmanipulation_cpp

/// @file
/// @brief The implementation of the string manipulation functions the unit tests use

#include "mezzanine.h"

#include "testenumerations.h"

using namespace Mezzanine;

namespace Mezzanine
{
    namespace Testing
    {
        String rtrim(const String &t)
        {
            String str = t;
            size_t found;
            found = str.find_last_not_of(" \n\r\t");
            if (found != String::npos)
                { str.erase(found+1); }
            else
                { str.clear(); }            // str is all whitespace

            return str;
        }

        // Used for padding spaces, after a piece of leader text, such that it always ends at teh expected colum
        String MakePadding(String Leader, unsigned int Column)
        {
            String Spaces(" ");
            for (unsigned int c=Leader.length(); c<Column;++c)
                { Spaces+=" "; }
            return Spaces;
        }

        char* AllLower(char* StringToConvert)
        {
            std::locale loc;
            for(int c=0; StringToConvert[c]!='\0'; ++c)
            {
                StringToConvert[c]=tolower(StringToConvert[c],loc);
            }
            return StringToConvert;
        }

        Mezzanine::String BoolToString(bool i)
            { return i?"True":"False" ; }


    }// Testing
}// Mezzanine

#endif
