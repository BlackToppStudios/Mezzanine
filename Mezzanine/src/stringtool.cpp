// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _stringtool_cpp
#define _stringtool_cpp

#include "stringtool.h"
#include "exception.h"

#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>
//#include <locale>

namespace
{
    using namespace Mezzanine;
    using StrIter = String::const_iterator;

    /// @brief Convenience multiplier used for converting a colour value for a single channel to a scalar value.
    const Real HexConversionMultiplier = Real(1.0 / 255.0);

    /// @brief Converts a string containing hex to a ColourValue channel.
    /// @param Hex The Hex value to be converted.
    /// @return Returns a Real representing the converted Hex string that can be applied to a ColourValue channel.
    Real ConvertHexToColourChannel(const String& Hex)
    {
        if( Hex.size() != 2 ) {
            MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Hex code requires 2 characters to express a ColourValue channel.");
        }

        Real Ret = 0;
        StringStream Converter;
        Converter << std::hex << Hex;
        Converter >> Ret;
        return std::min(Ret *= HexConversionMultiplier,Real(1.0));
    }
    /// @brief Converts a ColourValue channel to Hex.
    /// @param Channel The value to be converted to Hex.
    /// @return Returns a two character string containing the hex expression for the provided channel value.
    String ConvertColourChannelToHex(const Real Channel)
    {
        String Ret;
        StringStream Converter;
        Converter << std::hex << static_cast<UInt8>( Channel * 255.0 );
        Converter >> Ret;

        if( Ret.size() == 1 ) {
            Ret.insert(0,1,'0');
        }
        return Ret;
    }

    // Pattern Matching helper methods

    /// @brief Convenience method to compare two characters insensitive of case.
    /// @param First An iterator to the first character to be compared.
    /// @param Second An iterator to the second character to be compared.
    /// @return Returns true if the two characters are the same later, regardless of case, false otherwise.
    Boole CaseInsensitiveCompare(StrIter First, StrIter Second)
        { return ( ::tolower( *First ) == ::tolower( *Second ) ); }

    /// @brief Searches for a pattern in the source string.
    /// @param SrcIt An iterator to the current position in the source string.
    /// @param SrcEnd An iterator to the end of the source string.
    /// @param PatIt An iterator to the current position in the string containing the pattern to search for.
    /// @param PatEnd An iterator to the end of the string containing the pattern to search for.
    /// @param CaseSensitive Whether or not the pattern to be matched should be matched in the same case.
    /// @return Returns an iterator to the position in the source string the pattern begins if one is found, or SrcEnd if no pattern was found.
    StrIter SearchPattern(StrIter SrcIt, const StrIter SrcEnd, StrIter PatIt, const StrIter PatEnd, const Boole CaseSensitive)
    {
        for(  ;  ; ++SrcIt )
        {
            StrIter SrcCompIt = SrcIt;
            for( StrIter PatCompIt = PatIt ;  ; ++SrcCompIt, ++PatCompIt )
            {
                if( PatCompIt == PatEnd ) {
                    return SrcIt;
                }
                if( SrcCompIt == SrcEnd ) {
                    return SrcEnd;
                }
                if( (*PatCompIt) == '\\' ) {
                    ++PatCompIt;
                }
                if( CaseSensitive ) {
                    if( (*SrcCompIt) != (*PatCompIt) ) {
                        break;
                    }
                }else{
                    if( !CaseInsensitiveCompare(SrcCompIt,PatCompIt) ) {
                        break;
                    }
                }
            }
        }
    }

    /// @brief Checks to see if the pattern following the asterisk matches a pattern in the source string.
    /// @param SrcIt An iterator to the current position in the source string.
    /// @param SrcEnd An iterator to the end of the source string.
    /// @param PatIt An iterator to the current position in the pattern string.
    /// @param PatEnd An iterator to the end of the pattern string.
    /// @param CaseSensitive Whether or not the pattern to be matched should be matched in the same case.
    /// @return Returns true if the pattern immediately following the asterisk matches a sequence in the source, false otherwise.
    Boole MatchAsterisk(StrIter& SrcIt, const StrIter SrcEnd, StrIter& PatIt, const StrIter PatEnd, const Boole CaseSensitive)
    {
        const StrIter PatBegin = PatIt;
        const String ControlChars("[*?");

        // Quick Check for the easy case.  An asterisk at the end of the pattern.
        if( PatBegin == ( PatEnd - 1 ) ) {
            SrcIt = SrcEnd;
            PatIt = PatEnd;
            return true;
        }

        // Get the position of the first non-escaped control character.
        StrIter ControlIt = PatBegin;
        do{
            ++ControlIt;
            ControlIt = std::find_first_of(ControlIt,PatEnd,ControlChars.begin(),ControlChars.end());
        }while( *( ControlIt - 1) == '\\' && ControlIt != PatEnd );

        StrIter SrcResult = SearchPattern(SrcIt,SrcEnd,PatBegin + 1,ControlIt,CaseSensitive);
        if( SrcResult != SrcEnd ) {
            SrcIt = SrcResult;
            PatIt = ControlIt;
            return true;
        }else{
            SrcIt = SrcEnd;
            PatIt = PatEnd;
            return false;
        }
    }

    /// @brief Matches any single character in the source string.
    /// @param SrcIt An iterator to the current position in the source string.
    /// @param SrcEnd An iterator to the end of the source string.
    /// @param PatIt An iterator to the current position in the pattern string.
    /// @param PatEnd An iterator to the end of the pattern string.
    /// @return Returns true always.  Question marks are trivial to match.
    Boole MatchQuestionMark(StrIter& SrcIt, const StrIter SrcEnd, StrIter& PatIt, const StrIter PatEnd)
    {
        if( SrcIt != SrcEnd ) {
            ++SrcIt;
        }
        return true;
    }

    /// @brief Matches a single character in the source string with the range specified in the pattern string.
    /// @param SrcIt An iterator to the current position in the source string.
    /// @param SrcEnd An iterator to the end of the source string.
    /// @param PatIt An iterator to the current position in the pattern string.
    /// @param PatEnd An iterator to the end of the pattern string.
    /// @return Returns true if the range is valid and the source character is within it, false otherwise.
    Boole MatchSingleRange(StrIter& SrcIt, const StrIter SrcEnd, StrIter& PatIt, const StrIter PatEnd)
    {
        char LowCheck = *(PatIt - 1);
        char HighCheck = *(PatIt + 1);
        std::advance(PatIt,2);
        if( ( StringTools::IsUpperAlphaLetter( LowCheck ) && StringTools::IsUpperAlphaLetter( HighCheck ) ) ||
            ( StringTools::IsLowerAlphaLetter( LowCheck ) && StringTools::IsLowerAlphaLetter( HighCheck ) ) ||
            ( StringTools::IsDigit( LowCheck ) && StringTools::IsDigit( HighCheck ) ) )
        {
            return ( (*SrcIt) >= LowCheck && (*SrcIt) <= HighCheck );
        }
        return false;
    }

    /// @brief Marches a single character in the source string with one or more ranges specified in the pattern string.
    /// @param SrcIt An iterator to the current position in the source string.
    /// @param SrcEnd An iterator to the end of the source string.
    /// @param PatIt An iterator to the current position in the pattern string.
    /// @param PatEnd An iterator to the end of the pattern string.
    /// @return Returns true if the source character matches any of the valid ranges specified, false otherwise.
    Boole MatchRangeCharacter(StrIter& SrcIt, const StrIter SrcEnd, StrIter& PatIt, const StrIter PatEnd)
    {
        StrIter PatternBegin = PatIt;
        if( (*PatternBegin) != '[' ) {
            return false;
        }

        Boole PassRangeTest = false;
        while( PatIt != PatEnd && (*PatIt) != ']' )
        {
            if( (*PatIt) == '-' ) {
                PassRangeTest = PassRangeTest || MatchSingleRange(SrcIt,SrcEnd,PatIt,PatEnd);
            }else{
                ++PatIt;
            }
        }
        if( PassRangeTest ) {
            ++SrcIt;
        }
        if( PatIt != PatEnd ) {
            ++PatIt;
        }
        return PassRangeTest;
    }
}

namespace Mezzanine
{
    namespace StringTools
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Character Manipulation and Checks

        Boole IsSpace(const Char8 ToCheck)
            { return ( ToCheck == ' ' ); }

        Boole IsTab(const Char8 ToCheck)
            { return ( ToCheck == '\t' || ToCheck == '\v' ); }

        Boole IsNewline(const Char8 ToCheck)
            { return ( ToCheck == '\r' || ToCheck == '\n' ); }

        Boole IsWhitespace(const Char8 ToCheck)
            { return ( IsSpace(ToCheck) || IsTab(ToCheck) || IsNewline(ToCheck) ); }

        Boole IsDigit(const Char8 ToCheck)
            { return ( ToCheck >= '0' && ToCheck <= '9' ); }

        Boole IsLowerAlphaLetter(const Char8 ToCheck)
            { return ( ToCheck >= 'a' && ToCheck <= 'z' ); }

        Boole IsUpperAlphaLetter(const Char8 ToCheck)
            { return ( ToCheck >= 'A' && ToCheck <= 'Z' ); }

        Boole IsAlphaLetter(const Char8 ToCheck)
            { return ( IsLowerAlphaLetter(ToCheck) || IsUpperAlphaLetter(ToCheck) ); }

        Boole IsLowerHexLetter(const Char8 ToCheck)
            { return ( ToCheck >= 'a' && ToCheck <= 'f' ); }

        Boole IsUpperHexLetter(const Char8 ToCheck)
            { return ( ToCheck >= 'A' && ToCheck <= 'F' ); }

        Boole IsHexLetter(const Char8 ToCheck)
            { return ( IsLowerHexLetter(ToCheck) || IsUpperHexLetter(ToCheck) ); }

        Boole IsHexDigit(const Char8 ToCheck)
            { return ( IsDigit(ToCheck) || IsHexLetter(ToCheck) ); }

        Boole IsAlphanumeric(const Char8 ToCheck)
            { return ( IsDigit(ToCheck) || IsAlphaLetter(ToCheck) ); }

        ///////////////////////////////////////////////////////////////////////////////
        // String Manipulation and Checks

        void Trim(String& Source, const Boole Left, const Boole Right)
        {
            const String Delims = " \t\r";
            if( Left ) {
                Whole Forward = 0;
                while( Forward < Source.size() )
                {
                    if( Delims.find( Source[Forward] ) == String::npos ) {
                        break;
                    }
                    ++Forward;
                }
                Source.erase(0,Forward);
            }
            if( Right ) {
                Whole Reverse = Source.size();
                while( Reverse > 0 )
                {
                    if( Delims.find( Source[Reverse-1] ) == String::npos ) {
                        break;
                    }
                    --Reverse;
                }
                Source.erase(Reverse);
            }
        }

        StringVector Split(const String& Source, const String& Delims, const Whole MaxSplits)
        {
            StringVector Ret;
            Ret.reserve( MaxSplits ? MaxSplits + 1 : 10 );
            Whole Splits = 0;

            size_t Start = 0;
            size_t Pos = 0;

            do{
                Pos = Source.find_first_of(Delims,Start);
                if( Pos == Start ) {
                    Start = Pos + 1;
                }else if( Pos == String::npos || ( MaxSplits && Splits == MaxSplits ) ) {
                    Ret.push_back(Source.substr(Start));
                    break;
                }else{
                    Ret.push_back(Source.substr(Start,Pos - Start));
                    Start = Pos + 1;
                    ++Splits;
                }
                Start = Source.find_first_not_of(Delims,Start);
            }while(Pos != String::npos);

            return Ret;
        }

        void ToUpperCase(String& Source)
        {
            std::transform(Source.begin(),Source.end(),Source.begin(),::toupper);
        }

        String UpperCaseCopy(String Source)
        {
            StringTools::ToUpperCase(Source);
            return Source;
        }

        void ToLowerCase(String& Source)
        {
            std::transform(Source.begin(),Source.end(),Source.begin(),::tolower);
        }

        String LowerCaseCopy(String Source)
        {
            StringTools::ToLowerCase(Source);
            return Source;
        }

        void ToCamelCase(String& Source)
        {
            Boole PrevCharIsWhitespace = true;
            for( String::iterator CurrIt = Source.begin() ; CurrIt != Source.end() ; ++CurrIt )
            {
                Boole CurrCharIsLowerLetter = StringTools::IsLowerAlphaLetter( *CurrIt );
                Boole CurrCharIsUpperLetter = StringTools::IsUpperAlphaLetter( *CurrIt );
                if( PrevCharIsWhitespace && CurrCharIsLowerLetter ) {
                    (*CurrIt) -= 32;
                }else if( !PrevCharIsWhitespace && CurrCharIsUpperLetter ) {
                    (*CurrIt) += 32;
                }
                PrevCharIsWhitespace = StringTools::IsWhitespace( *CurrIt );
            }
        }

        String CamelCaseCopy(String Source)
        {
            StringTools::ToCamelCase(Source);
            return Source;
        }

        Boole StartsWith(const String& Str, const String& Pattern, const Boole CaseSensitive)
        {
            size_t StrLen = Str.length();
            size_t PatternLen = Pattern.length();

            String Start = Str.substr(0,PatternLen);

            if( CaseSensitive ) {
                return ( Start == Pattern );
            }

            if( PatternLen > StrLen || PatternLen == 0 ) {
                return false;
            }

            String LowerPattern = Pattern;
            StringTools::ToLowerCase(Start);
            StringTools::ToLowerCase(LowerPattern);
            return (Start == LowerPattern);
        }

        Boole EndsWith(const String& Str, const String& Pattern, const Boole CaseSensitive)
        {
            size_t StrLen = Str.length();
            size_t PatternLen = Pattern.length();
            if( PatternLen > StrLen )
                return false;

            String End = Str.substr(StrLen - PatternLen,PatternLen);

            if( CaseSensitive ) {
                return ( End == Pattern );
            }

            if( PatternLen > StrLen || PatternLen == 0 ) {
                return false;
            }

            String LowerPattern = Pattern;
            StringTools::ToLowerCase(End);
            StringTools::ToLowerCase(LowerPattern);
            return ( End == LowerPattern );
        }

        void RemoveDuplicateWhitespaces(String& Source)
        {
            for( size_t CurrIndex = Source.find_first_of("  ") ; CurrIndex != String::npos ; CurrIndex = Source.find_first_of("  ",CurrIndex) )
            {
                size_t EndIndex = CurrIndex;
                while( Source[EndIndex] == ' ' ) EndIndex++;
                Source.replace(CurrIndex,EndIndex-CurrIndex," ");
                CurrIndex++;
            }
        }

        Boole PatternMatch(const String& Source, const String& Pattern, const Boole CaseSensitive)
        {
            String::const_iterator SrcIt = Source.begin();
            String::const_iterator PatIt = Pattern.begin();
            const String::const_iterator SrcEnd = Source.end();
            const String::const_iterator PatEnd = Pattern.end();

            Boole EscapedChar = false;
            while( PatIt != PatEnd && SrcIt != SrcEnd )
            {
                Boole MatchResult = false;
                if( EscapedChar ) {
                    EscapedChar = false;
                    // If we're escaped, doesn't matter what it is.  Process as normal char.
                    MatchResult = ( (*PatIt) == (*SrcIt) );
                    ++PatIt;
                    ++SrcIt;
                }else if( (*PatIt) == '*' ) {
                    EscapedChar = false;
                    MatchResult = MatchAsterisk(SrcIt,SrcEnd,PatIt,PatEnd,CaseSensitive);
                }else if( (*PatIt) == '[' ) {
                    EscapedChar = false;
                    MatchResult = MatchRangeCharacter(SrcIt,SrcEnd,PatIt,PatEnd);
                }else if( (*PatIt) == '?' ) {
                    EscapedChar = false;
                    MatchResult = MatchQuestionMark(SrcIt,SrcEnd,PatIt,PatEnd);
                }else if( (*PatIt) == '\\' ) {
                    EscapedChar = true;
                    ++PatIt;
                    continue;
                }else{
                    EscapedChar = false;
                    if( CaseSensitive ) {
                        MatchResult = ( (*PatIt) == (*SrcIt) );
                    }else{
                        MatchResult = CaseInsensitiveCompare(PatIt,SrcIt);
                    }
                    ++PatIt;
                    ++SrcIt;
                }

                if( !MatchResult ) {
                    return false;
                }
            }
            return true;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Data Class Utilities

        Vector2 ConvertToVector2(const String& ToConvert)
        {
            StringVector Digits = Split(ToConvert);
            if( 2 == Digits.size() ) {
                return Vector2(ConvertToReal(Digits.at(0)),ConvertToReal(Digits.at(1)));
            }else{
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"String does not contain 2 digits when attempting to convert.");
            }
        }

        String ConvertToString(const Vector2& ToConvert)
        {
            StringStream converter;
            converter << ToConvert.X << " " << ToConvert.Y;
            return converter.str();
        }

        Vector3 ConvertToVector3(const String& ToConvert)
        {
            StringVector Digits = Split(ToConvert);
            if( 3 == Digits.size() ) {
                return Vector3(ConvertToReal(Digits.at(0)),ConvertToReal(Digits.at(1)),ConvertToReal(Digits.at(2)));
            }else{
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"String does not contain 3 digits when attempting to convert.");
            }
        }

        String ConvertToString(const Vector3& ToConvert)
        {
            StringStream converter;
            converter << ToConvert.X << " " << ToConvert.Y << " " << ToConvert.Z;
            return converter.str();
        }

        Quaternion ConvertToQuaternion(const String& ToConvert)
        {
            StringVector Digits = Split(ToConvert);
            if( 4 == Digits.size() ) {
                return Quaternion(ConvertToReal(Digits.at(0)),ConvertToReal(Digits.at(1)),ConvertToReal(Digits.at(2)),ConvertToReal(Digits.at(3)));
            }else{
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"String does not contain 4 digits when attempting to convert.");
            }
        }

        String ConvertToString(const Quaternion& ToConvert)
        {
            StringStream converter;
            converter << ToConvert.X << " " << ToConvert.Y << " " << ToConvert.Z << " " << ToConvert.W;
            return converter.str();
        }

        ColourValue ConvertToColourValue(const String& ToConvert)
        {
            StringVector Digits = Split(ToConvert);
            if( 4 == Digits.size() ) {
                return ColourValue(ConvertToReal(Digits.at(0)),ConvertToReal(Digits.at(1)),ConvertToReal(Digits.at(2)),ConvertToReal(Digits.at(3)));
            }else{
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"String does not contain 4 digits when attempting to convert.");
            }
        }

        String ConvertToString(const ColourValue& ToConvert)
        {
            StringStream converter;
            converter << ToConvert.RedChannel << " " << ToConvert.GreenChannel << " " << ToConvert.BlueChannel << " " << ToConvert.AlphaChannel;
            return converter.str();
        }

        ColourValue ConvertHexToColourValue(const String& ToConvert)
        {
            if( ToConvert.size() < 6 ) {
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Hex code requires a minimum of 6 characters to express a ColourValue instance.");
            }

            ColourValue Ret;
            Ret.RedChannel = ConvertHexToColourChannel( ToConvert.substr(0,2) );
            Ret.GreenChannel = ConvertHexToColourChannel( ToConvert.substr(2,2) );
            Ret.BlueChannel = ConvertHexToColourChannel( ToConvert.substr(4,2) );

            if( ToConvert.size() == 6 ) Ret.AlphaChannel = 1.0;
            else Ret.AlphaChannel = ConvertHexToColourChannel( ToConvert.substr(6,2) );

            return Ret;
        }

        String ConvertToHexString(const ColourValue& ToConvert)
        {
            String Ret;
            Ret.append( ConvertColourChannelToHex(ToConvert.RedChannel) );
            Ret.append( ConvertColourChannelToHex(ToConvert.GreenChannel) );
            Ret.append( ConvertColourChannelToHex(ToConvert.BlueChannel) );
            Ret.append( ConvertColourChannelToHex(ToConvert.AlphaChannel) );
            return Ret;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Convert-To-Data functions

        Boole ConvertToBool(const String& ToConvert, const Boole Default)
        {
            String StrCopy = ToConvert;
            StringTools::ToLowerCase(StrCopy);
            if("true" == StrCopy) return true;
            else if("yes" == StrCopy) return true;
            else if("1" == StrCopy) return true;
            else if("false" == StrCopy) return false;
            else if("no" == StrCopy) return false;
            else if("0" == StrCopy) return false;
            else return Default;
        }

        Real ConvertToReal(const String& ToConvert)
        {
            StringStream converter(ToConvert);
            Real Result;
            converter >> Result;
            return Result;
        }

        Integer ConvertToInteger(const String& ToConvert)
        {
            StringStream converter(ToConvert);
            Integer Result;
            converter >> Result;
            return Result;
        }

        Whole ConvertToWhole(const String& ToConvert)
        {
            StringStream converter(ToConvert);
            Whole Result;
            converter >> Result;
            return Result;
        }

        Int8 ConvertToInt8(const String& ToConvert)
        {
            StringStream converter(ToConvert);
            int Result;
            converter >> Result;
            return static_cast<Int8>(Result);
        }

        UInt8 ConvertToUInt8(const String& ToConvert)
        {
            StringStream converter(ToConvert);
            unsigned int Result;
            converter >> Result;
            return static_cast<UInt8>(Result);
        }

        Int16 ConvertToInt16(const String& ToConvert)
        {
            StringStream converter(ToConvert);
            Int16 Result;
            converter >> Result;
            return Result;
        }

        UInt16 ConvertToUInt16(const String& ToConvert)
        {
            StringStream converter(ToConvert);
            UInt16 Result;
            converter >> Result;
            return Result;
        }

        Int32 ConvertToInt32(const String& ToConvert)
        {
            StringStream converter(ToConvert);
            Int32 Result;
            converter >> Result;
            return Result;
        }

        UInt32 ConvertToUInt32(const String& ToConvert)
        {
            StringStream converter(ToConvert);
            UInt32 Result;
            converter >> Result;
            return Result;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Convert-To-String functions

        template<>
        String ConvertToString<Boole>(const Boole& ToConvert)
        {
            if(ToConvert) return "true";
            else return "false";
        }

        template<>
        String ConvertToString<Int8>(const Int8& ToConvert)
        {
            Integer Convert = ToConvert;
            StringStream Converter;
            Converter << Convert;
            return Converter.str();
        }

        template<>
        String ConvertToString<UInt8>(const UInt8& ToConvert)
        {
            Whole Convert = ToConvert;
            StringStream Converter;
            Converter << Convert;
            return Converter.str();
        }
    }//StringTools
}//Mezzanine

#endif
