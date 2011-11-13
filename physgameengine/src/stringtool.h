//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _stringtool_h
#define _stringtool_h

#include "datatypes.h"
#include "metacode.h"

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class StringTool
    /// @headerfile stringtool.h
    /// @brief This is a utility to help perform all kinds of string related tasks.
    /// @details
    ///////////////////////////////////////
    class PHYS_LIB StringTool
    {
        protected:
        public:
            /// @brief Class constructor.
            StringTool();
            /// @brief Class destructor.
            ~StringTool();
            /// @brief Converts an Int8 into a string.
            /// @param ToConvert The Int8 to be converted.
            /// @return Returns a string containing the converted input.
            static String ConvertToString(const Int8& ToConvert);
            /// @brief Converts a UInt8 into a string.
            /// @param ToConvert The UInt8 to be converted.
            /// @return Returns a string containing the converted input.
            static String ConvertToString(const UInt8& ToConvert);
            /// @brief Converts an Int16 into a string.
            /// @param ToConvert The Int16 to be converted.
            /// @return Returns a string containing the converted input.
            static String ConvertToString(const Int16& ToConvert);
            /// @brief Converts a UInt16 into a string.
            /// @param ToConvert The UInt16 to be converted.
            /// @return Returns a string containing the converted input.
            static String ConvertToString(const UInt16& ToConvert);
            /// @brief Converts an Int32 into a string.
            /// @param ToConvert The Int32 to be converted.
            /// @return Returns a string containing the converted input.
            static String ConvertToString(const Int32& ToConvert);
            /// @brief Converts a UInt32 into a string.
            /// @param ToConvert The UInt32 to be converted.
            /// @return Returns a string containing the converted input.
            static String ConvertToString(const UInt32& ToConvert);
            /// @brief Converts a MetaCode::InputCode into a string.
            /// @param Code The input code to be converted.
            /// @param ShiftPressed Whether or not the shift modifier key has been pressed.
            /// @return Returns a string(usually with only one character) containing the converted input code.
            static String ConvertToString(const MetaCode::InputCode& Code, bool ShiftPressed);
            /// @brief Concatenates 2 Strings
            /// @param Front The first String
            /// @param Back The last String
            /// @return A string containing the other Strings passed attached end to end
            static String StringCat(const String& Front, const String& Back);
            /// @brief Concatenates 3 Strings
            /// @param Front The first String
            /// @param Middle A String in the middle
            /// @param Back The last String
            /// @return A string containing the other Strings passed attached end to end
            static String StringCat(const String& Front, const String& Middle, const String& Back);
            /// @brief Concatenates 4 Strings
            /// @param Front The first String
            /// @param Middle1 The second string
            /// @param Middle2 The third string
            /// @param Back The last String
            /// @return A string containing the other Strings passed attached end to end
            static String StringCat(const String& Front, const String& Middle1, const String& Middle2, const String& Back);
            /// @brief Concatenates 5 Strings
            /// @param Front The first String
            /// @param Middle1 The second string
            /// @param Middle2 The third string
            /// @param Middle3 The fourth string
            /// @param Back The last String
            /// @return A string containing the other Strings passed attached end to end
            static String StringCat(const String& Front, const String& Middle1, const String& Middle2, const String& Middle3, const String& Back);
            /// @brief Concatenates 6 Strings
            /// @param Front The first String
            /// @param Middle1 The second string
            /// @param Middle2 The third string
            /// @param Middle3 The fourth string
            /// @param Middle4 The fifth string
            /// @param Back The last String
            /// @return A string containing the other Strings passed attached end to end
            static String StringCat(const String& Front, const String& Middle1, const String& Middle2, const String& Middle3, const String& Middle4, const String& Back);
    };//stringtool
}//phys

#endif
