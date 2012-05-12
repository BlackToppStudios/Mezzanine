//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _stringtool_h
#define _stringtool_h

#include "metacode.h"
#include "vector2.h"
#include "vector3.h"
#include "quaternion.h"
#include "colourvalue.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class StringTool
    /// @headerfile stringtool.h
    /// @brief This is a utility to help perform all kinds of string related tasks.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB StringTool
    {
        protected:
        public:
            /// @brief Class constructor.
            StringTool();
            /// @brief Class destructor.
            ~StringTool();

            ///////////////////////////////////////////////////////////////////////////////
            // String Manipulation and checks

            /// @brief Trims all whitespaces and tabs from a one or both sides of a string.
            /// @param Source The original string to be trimmed.
            /// @param Left Whether or not to trim the left side of the string.
            /// @param Right Whether or not to trim the right side of the string.
            static void Trim(String& Source, bool Left = true, bool Right = true);
            /// @brief Splits a string into multiple substrings based on the specified delimiters.
            /// @param Source The string to be split.
            /// @param Delims The characters to look for and use as split points in the source string.
            /// @param MaxSplits The maximum number of splits to perform on this string.  Value of zero means unlimited splits.
            /// @return Returns a vector containing all the substrings generated from the source string.
            static std::vector<String> Split(const String& Source, const String& Delims = " \t\n", const Whole& MaxSplits = 0);
            /// @brief Converts all lower case characters in a string to their respective upper case.
            /// @param Source The string to be converted.
            static void ToUpperCase(String& Source);
            /// @brief Converts all upper case characters in a string to their respective lower case.
            /// @param Source The string to be converted.
            static void ToLowerCase(String& Source);
            /// @brief Checks a string to see if it starts with a specific pattern.
            /// @param Str The string to check.
            /// @param Pattern The sequence to check for at the start of the string.
            /// @param CaseSensitive If false this function will check lower-case copies for the pattern, otherwise the strings will be checked as is.
            /// @return Returns true if the string starts with the provided pattern, false otherwise.
            static bool StartsWith(const String& Str, const String& Pattern, const bool CaseSensitive);
            /// @brief Checks a string to see if it ends with a specific pattern.
            /// @param Str The string to check.
            /// @param Pattern The sequence to check for at the end of the string.
            /// @param CaseSensitive If false this function will check lower-case copies for the pattern, otherwise the strings will be checked as is.
            /// @return Returns true if the string ends with the provided pattern, false otherwise.
            static bool EndsWith(const String& Str, const String& Pattern, const bool CaseSensitive);
            /// @brief Replaces all instances of multiple consecutive whitespaces with only a single whitespace.
            /// @param Source The string to be altered.
            static void RemoveDuplicateWhitespaces(String& Source);

            ///////////////////////////////////////////////////////////////////////////////
            // Data Class Utilities

            /// @brief Convert two numbers in a string into a Vector2.
            /// @param ToConvert The string to be converted.
            /// @remarks The string is expected to have a certain format.  The format should be "X Y".  If there are not 2 numbers an exception will be thrown.
            /// @return Returns a Vector2 populated with the values from the string passed in.
            static Vector2 ConvertToVector2(const String& ToConvert);
            /// @brief Converts a Vector2 into a string.
            /// @param ToConvert The Vector2 to be converted.
            /// @return Returns a string containing the values from the Vector2 in "X Y" format.
            static String ConvertToString(const Vector2& ToConvert);
            /// @brief Convert three numbers in a string into a Vector3.
            /// @param ToConvert The string to be converted.
            /// @remarks The string is expected to have a certain format.  The format should be "X Y Z".  If there are not 3 numbers an exception will be thrown.
            /// @return Returns a Vector3 populated with the values from the string passed in.
            static Vector3 ConvertToVector3(const String& ToConvert);
            /// @brief Converts a Vector3 into a string.
            /// @param ToConvert The Vector3 to be converted.
            /// @return Returns a string containing the values from the Vector3 in "X Y Z" format.
            static String ConvertToString(const Vector3& ToConvert);
            /// @brief Convert four numbers in a string into a Quaternion.
            /// @param ToConvert The string to be converted.
            /// @remarks The string is expected to have a certain format.  The format should be "X Y Z W".  If there are not 4 numbers an exception will be thrown.
            /// @return Returns a Quaternion populated with the values from the string passed in.
            static Quaternion ConvertToQuaternion(const String& ToConvert);
            /// @brief Converts a Quaternion into a string.
            /// @param ToConvert The Quaternion to be converted.
            /// @return Returns a string containing the values from the Quaternion in "X Y Z W" format.
            static String ConvertToString(const Quaternion& ToConvert);
            /// @brief Convert four numbers in a string into a ColourValue.
            /// @param ToConvert The string to be converted.
            /// @remarks The string is expected to have a certain format.  The format should be "R G B A".  If there are not 4 numbers an exception will be thrown.
            /// @return Returns a ColourValue populated with the values from the string passed in.
            static ColourValue ConvertToColourValue(const String& ToConvert);
            /// @brief Converts a ColourValue into a string.
            /// @param ToConvert The ColourValue to be converted.
            /// @return Returns a string containing the values from the ColourValue in "R G B A" format.
            static String ConvertToString(const ColourValue& ToConvert);

            ///////////////////////////////////////////////////////////////////////////////
            // Convert-To-Data functions

            /// @brief Converts a string into a bool.
            /// @param ToConvert The string to be converted to a bool.
            /// @return Returns a bool with the converted value.
            static bool ConvertToBool(const String& ToConvert, const bool Default = false);
            /// @brief Converts a string into a Real.
            /// @param ToConvert The string to be converted to a Real.
            /// @return Returns a Real with the converted value.
            static Real ConvertToReal(const String& ToConvert);
            /// @brief Converts a string into an Integer.
            /// @param ToConvert The string to be converted to an Integer.
            /// @return Returns an Integer with the converted value.
            static Integer ConvertToInteger(const String& ToConvert);
            /// @brief Converts a string into an Int8.
            /// @param ToConvert The string to be converted to an Int8.
            /// @return Returns an Int8 with the converted value.
            static Int8 ConvertToInt8(const String& ToConvert);
            /// @brief Converts a string into a UInt8.
            /// @param ToConvert The string to be converted to a UInt8.
            /// @return Returns a UInt8 with the converted value.
            static UInt8 ConvertToUInt8(const String& ToConvert);
            /// @brief Converts a string into an Int16.
            /// @param ToConvert The string to be converted to an Int16.
            /// @return Returns an Int16 with the converted value.
            static Int16 ConvertToInt16(const String& ToConvert);
            /// @brief Converts a string into a UInt16.
            /// @param ToConvert The string to be converted to a UInt16.
            /// @return Returns a UInt16 with the converted value.
            static UInt16 ConvertToUInt16(const String& ToConvert);
            /// @brief Converts an string into an Int32.
            /// @param ToConvert The string to be converted to an Int32.
            /// @return Returns an Int32 with the converted value.
            static Int32 ConvertToInt32(const String& ToConvert);
            /// @brief Converts a string into a UInt32.
            /// @param ToConvert The string to be converted to a UInt32.
            /// @return Returns a UInt32 with the converted value.
            static UInt32 ConvertToUInt32(const String& ToConvert);

            ///////////////////////////////////////////////////////////////////////////////
            // Convert-To-String functions

            /// @brief Converts a bool into a string.
            /// @param ToConvert The bool to be converted.
            /// @return Returns "true" if true, or "false" if false.
            static String ConvertToString(const bool ToConvert);
            /// @brief Converts an Real into a string.
            /// @param ToConvert The Real to be converted.
            /// @return Returns a string containing the converted input.
            static String ConvertToString(const Real& ToConvert);
            /// @brief Converts an Integer into a string.
            /// @param ToConvert The Integer to be converted.
            /// @return Returns a string containing the converted input.
            static String ConvertToString(const Integer& ToConvert);
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

            ///////////////////////////////////////////////////////////////////////////////
            // String Catenate functions

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
}//Mezzanine

#endif
