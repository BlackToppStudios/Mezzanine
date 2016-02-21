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
#ifndef _stringtool_h
#define _stringtool_h

#ifndef SWIG
    #include "Input/metacode.h"
#endif
#include "vector2.h"
#include "vector3.h"
#include "quaternion.h"
#include "colourvalue.h"
#include "countedptr.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a utility to help perform all kinds of string related tasks.
    /// @details
    ///////////////////////////////////////
    namespace StringTools
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Globals, mostly used for comparison and such

        /// @brief Convenience String for comparing to return blank Strings where necessary.
        const String Blank = "";

        ///////////////////////////////////////////////////////////////////////////////
        // Character Manipulation and checks

        /// @brief Checks if a character is a decimal digit.
        /// @param ToCheck The character to be checked.
        /// @return Returns true if the character is a decimal digit, false otherwise.
        Boole MEZZ_LIB IsDigit(const Char8 ToCheck);
        /// @brief Checks if a character is a lower-case alphabetic letter.
        /// @param ToCheck The character to be checked.
        /// @return Returns true if the character is a lower-case letter, false otherwise.
        Boole MEZZ_LIB IsLowerAlphaLetter(const Char8 ToCheck);
        /// @brief Checks if a character is a upper-case alphabetic letter.
        /// @param ToCheck The character to be checked.
        /// @return Returns true if the character is a upper-case letter, false otherwise.
        Boole MEZZ_LIB IsUpperAlphaLetter(const Char8 ToCheck);
        /// @brief Checks if a character is a lower or upper case letter.
        /// @param ToCheck The character to be checked.
        /// @return Returns true if the character is a lower or upper case letter, false otherwise.
        Boole MEZZ_LIB IsAlphaLetter(const Char8 ToCheck);
        /// @brief Checks if a character is a lower-case hexadecimal letter.
        /// @param ToCheck The character to be checked.
        /// @return Returns true if the character is a lower-case letter, false otherwise.
        Boole MEZZ_LIB IsLowerHexLetter(const Char8 ToCheck);
        /// @brief Checks if a character is a upper-case hexadecimal letter.
        /// @param ToCheck The character to be checked.
        /// @return Returns true if the character is a upper-case letter, false otherwise.
        Boole MEZZ_LIB IsUpperHexLetter(const Char8 ToCheck);
        /// @brief Checks if a character is a hexadecimal letter.
        /// @param ToCheck The character to be checked.
        /// @return Returns true if the character is a hexadecimal letter, false otherwise.
        Boole MEZZ_LIB IsHexLetter(const Char8 ToCheck);
        /// @brief Checks if a character is a hexadecimal digit.
        /// @param ToCheck The character to be checked.
        /// @return Returns true if the character is a hexadecimal digit, false otherwise.
        Boole MEZZ_LIB IsHexDigit(const Char8 ToCheck);
        /// @brief Checks if a character is a letter or digit.
        /// @param ToCheck The character to be checked.
        /// @return Returns true if the character is a letter or digit, false otherwise.
        Boole MEZZ_LIB IsAlphanumeric(const Char8 ToCheck);

        ///////////////////////////////////////////////////////////////////////////////
        // String Manipulation and Checks

        /// @brief Trims all whitespaces and tabs from a one or both sides of a String.
        /// @param Source The original string to be trimmed.
        /// @param Left Whether or not to trim the left side of the String.
        /// @param Right Whether or not to trim the right side of the String.
        void MEZZ_LIB Trim(String& Source, Boole Left = true, Boole Right = true);
        /// @brief Splits a string into multiple substrings based on the specified delimiters.
        /// @param Source The String to be split.
        /// @param Delims The characters to look for and use as split points in the source String.
        /// @param MaxSplits The maximum number of splits to perform on this String.  Value of zero means unlimited splits.
        /// @return Returns a vector containing all the substrings generated from the source String.
        StringVector MEZZ_LIB Split(const String& Source, const String& Delims = " \t\n", const Whole MaxSplits = 0);
        /// @brief Converts all lower case characters in a string to their respective upper case.
        /// @param Source The String to be converted.
        void MEZZ_LIB ToUpperCase(String& Source);
        /// @brief Create a copy of the String that is upper case.
        /// @param Source The original String to copy.
        /// @return Returns a copy of the String that has been modified to be in all upper case.
        String MEZZ_LIB UpperCaseCopy(String Source);
        /// @brief Converts all upper case characters in a string to their respective lower case.
        /// @param Source The String to be converted.
        void MEZZ_LIB ToLowerCase(String& Source);
        /// @brief Create a copy of the String that is lower case.
        /// @param Source The original String to copy.
        /// @return Returns a copy of the String that has been modified to be in all lower case.
        String MEZZ_LIB LowerCaseCopy(String Source);
        /// @brief Converts the first letter of each word to upper case and all other letters to lower case.
        /// @param Source The String to be converted.
        void MEZZ_LIB ToCamelCase(String& Source);
        /// @brief Create a copy of the String that has the first letter of each word upper case and other letters lower case.
        /// @param Source The original String to copy.
        /// @return Returns a copy of the String that has been modified to be in Camel Case.
        String MEZZ_LIB CamelCaseCopy(String Source);
        /// @brief Checks a String to see if it starts with a specific pattern.
        /// @param Str The String to check.
        /// @param Pattern The sequence to check for at the start of the String.
        /// @param CaseSensitive If false this function will check lower-case copies for the pattern, otherwise the strings will be checked as is.
        /// @return Returns true if the String starts with the provided pattern, false otherwise.
        Boole MEZZ_LIB StartsWith(const String& Str, const String& Pattern, const Boole CaseSensitive);
        /// @brief Checks a String to see if it ends with a specific pattern.
        /// @param Str The String to check.
        /// @param Pattern The sequence to check for at the end of the String.
        /// @param CaseSensitive If false this function will check lower-case copies for the pattern, otherwise the strings will be checked as is.
        /// @return Returns true if the string ends with the provided pattern, false otherwise.
        Boole MEZZ_LIB EndsWith(const String& Str, const String& Pattern, const Boole CaseSensitive);
        /// @brief Replaces all instances of multiple consecutive whitespaces with only a single whitespace.
        /// @param Source The String to be altered.
        void MEZZ_LIB RemoveDuplicateWhitespaces(String& Source);

        ///////////////////////////////////////////////////////////////////////////////
        // Data Class Utilities

        /// @brief Convert two numbers in a string into a Vector2.
        /// @param ToConvert The string to be converted.
        /// @remarks The string is expected to have a certain format.  The format should be "X Y".  If there are not 2 numbers an exception will be thrown.
        /// @return Returns a Vector2 populated with the values from the string passed in.
        Vector2 MEZZ_LIB ConvertToVector2(const String& ToConvert);
        /// @brief Converts a Vector2 into a string.
        /// @param ToConvert The Vector2 to be converted.
        /// @return Returns a string containing the values from the Vector2 in "X Y" format.
        String MEZZ_LIB ConvertToString(const Vector2& ToConvert);
        /// @brief Convert three numbers in a string into a Vector3.
        /// @param ToConvert The string to be converted.
        /// @remarks The string is expected to have a certain format.  The format should be "X Y Z".  If there are not 3 numbers an exception will be thrown.
        /// @return Returns a Vector3 populated with the values from the string passed in.
        Vector3 MEZZ_LIB ConvertToVector3(const String& ToConvert);
        /// @brief Converts a Vector3 into a string.
        /// @param ToConvert The Vector3 to be converted.
        /// @return Returns a string containing the values from the Vector3 in "X Y Z" format.
        String MEZZ_LIB ConvertToString(const Vector3& ToConvert);
        /// @brief Convert four numbers in a string into a Quaternion.
        /// @param ToConvert The string to be converted.
        /// @remarks The string is expected to have a certain format.  The format should be "X Y Z W".  If there are not 4 numbers an exception will be thrown.
        /// @return Returns a Quaternion populated with the values from the string passed in.
        Quaternion MEZZ_LIB ConvertToQuaternion(const String& ToConvert);
        /// @brief Converts a Quaternion into a string.
        /// @param ToConvert The Quaternion to be converted.
        /// @return Returns a string containing the values from the Quaternion in "X Y Z W" format.
        String MEZZ_LIB ConvertToString(const Quaternion& ToConvert);
        /// @brief Convert four numbers in a string into a ColourValue.
        /// @param ToConvert The string to be converted.
        /// @remarks The string is expected to have a certain format.  The format should be "R G B A".  If there are not 4 numbers an exception will be thrown.
        /// @return Returns a ColourValue populated with the values from the string passed in.
        ColourValue MEZZ_LIB ConvertToColourValue(const String& ToConvert);
        /// @brief Converts a ColourValue into a string.
        /// @param ToConvert The ColourValue to be converted.
        /// @return Returns a string containing the values from the ColourValue in "R G B A" format.
        String MEZZ_LIB ConvertToString(const ColourValue& ToConvert);

        /// @brief Converts a Hex code in a string into a ColourValue.
        /// @param ToConvert The string to be converted.
        /// @return Returns a ColourValue populated with the values from the string passed in.
        ColourValue MEZZ_LIB ConvertHexToColourValue(const String& ToConvert);
        /// @brief Converts a ColourValue into a string as a Hex code.
        /// @param ToConvert The ColourValue to be converted.
        /// @return Returns a string containing the values from the string passed in.
        String MEZZ_LIB ConvertToHexString(const ColourValue& ToConvert);

        ///////////////////////////////////////////////////////////////////////////////
        // Convert-To-Data functions

        /// @brief Converts a string into a Boole.
        /// @param ToConvert The string to be converted to a Boole.
        /// @return Returns a Boole with the converted value.
        Boole MEZZ_LIB ConvertToBool(const String& ToConvert, const Boole Default = false);
        /// @brief Converts a string into a Real.
        /// @param ToConvert The string to be converted to a Real.
        /// @return Returns a Real with the converted value.
        Real MEZZ_LIB ConvertToReal(const String& ToConvert);
        /// @brief Converts a string into an Integer.
        /// @param ToConvert The string to be converted to an Integer.
        /// @return Returns an Integer with the converted value.
        Integer MEZZ_LIB ConvertToInteger(const String& ToConvert);
        /// @brief Converts a string into an Whole.
        /// @param ToConvert The string to be converted to an Whole.
        /// @return Returns an Whole with the converted value.
        Whole MEZZ_LIB ConvertToWhole(const String& ToConvert);
        /// @brief Converts a string into an Int8.
        /// @param ToConvert The string to be converted to an Int8.
        /// @return Returns an Int8 with the converted value.
        Int8 MEZZ_LIB ConvertToInt8(const String& ToConvert);
        /// @brief Converts a string into a UInt8.
        /// @param ToConvert The string to be converted to a UInt8.
        /// @return Returns a UInt8 with the converted value.
        UInt8 MEZZ_LIB ConvertToUInt8(const String& ToConvert);
        /// @brief Converts a string into an Int16.
        /// @param ToConvert The string to be converted to an Int16.
        /// @return Returns an Int16 with the converted value.
        Int16 MEZZ_LIB ConvertToInt16(const String& ToConvert);
        /// @brief Converts a string into a UInt16.
        /// @param ToConvert The string to be converted to a UInt16.
        /// @return Returns a UInt16 with the converted value.
        UInt16 MEZZ_LIB ConvertToUInt16(const String& ToConvert);
        /// @brief Converts an string into an Int32.
        /// @param ToConvert The string to be converted to an Int32.
        /// @return Returns an Int32 with the converted value.
        Int32 MEZZ_LIB ConvertToInt32(const String& ToConvert);
        /// @brief Converts a string into a UInt32.
        /// @param ToConvert The string to be converted to a UInt32.
        /// @return Returns a UInt32 with the converted value.
        UInt32 MEZZ_LIB ConvertToUInt32(const String& ToConvert);

        ///////////////////////////////////////////////////////////////////////////////
        // Convert-To-String functions

        /// @brief Converts any into a string.
        /// @param ToConvert Stream class instance to be converted.
        /// @return Returns a string containing the lexicagraphically converted data.
        template<typename T>
        String ConvertToString(const T& ToConvert)
        {
            StringStream converter;
            converter << ToConvert;
            return converter.str();
        }

        // @brief Converts a Boole into a string.
        // @param ToConvert The Boole to be converted.
        // @return Returns "true" if true, or "false" if false.
        //template<>
        //String MEZZ_LIB ConvertToString<Boole>(const Boole& ToConvert)
        //{
        //    if(ToConvert) return "true";
        //    else return "false";
        //}

        /// @brief Converts a Input::InputCode into a string.
        /// @param Code The input code to be converted.
        /// @param ShiftPressed Whether or not the shift modifier key has been pressed.
        /// @return Returns a string(usually with only one character) containing the converted input code.
        String ConvertToString(const Input::InputCode& Code, Boole ShiftPressed);

        /// @brief Converts a Input::InputCode into a string, assuming shift was not pressed.
        /// @details this is good for figuring out what key was pressed
        /// @param Code The input code to be converted.
        /// @return Returns a string(usually with only one character) containing the converted input code.
        //template<>
        //String MEZZ_LIB ConvertToString<Input::InputCode>(const Input::InputCode& Code)
        //    { return ConvertToString(Code, false); }

    } //stringtool
}//Mezzanine

#endif
