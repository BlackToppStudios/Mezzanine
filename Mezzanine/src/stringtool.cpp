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
#ifndef _stringtool_cpp
#define _stringtool_cpp

#include "stringtool.h"
#include "exception.h"

#include <sstream>
#include <algorithm>
#include <cctype>
//#include <locale>

namespace
{
    /// @internal
    /// @brief Convenience multiplier used for converting a colour value for a single channel to a scalar value.
    const Mezzanine::Real HexConversionMultiplier = Mezzanine::Real(1.0 / 255.0);

    /// @internal
    /// @brief Converts a string containing hex to a ColourValue channel.
    /// @param Hex The Hex value to be converted.
    /// @return Returns a Real representing the converted Hex string that can be applied to a ColourValue channel.
    Mezzanine::Real ConvertHexToColourChannel(const Mezzanine::String& Hex)
    {
        if( Hex.size() != 2 ) {
            MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Hex code requires 2 characters to express a ColourValue channel.");
        }

        Mezzanine::Real Ret = 0;
        Mezzanine::StringStream Converter;
        Converter << std::hex << Hex;
        Converter >> Ret;
        return std::min(Ret *= HexConversionMultiplier,Mezzanine::Real(1.0));
    }
    /// @internal
    /// @brief Converts a ColourValue channel to Hex.
    /// @param Channel The value to be converted to Hex.
    /// @return Returns a two character string containing the hex expression for the provided channel value.
    Mezzanine::String ConvertColourChannelToHex(const Mezzanine::Real Channel)
    {
        Mezzanine::String Ret;
        Mezzanine::StringStream Converter;
        Converter << std::hex << static_cast<Mezzanine::UInt8>( Channel * 255.0 );
        Converter >> Ret;

        if( Ret.size() == 1 ) {
            Ret.insert(0,1,'0');
        }
        return Ret;
    }
}

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // String Manipulation and checks

    void StringTools::Trim(String& Source, Boole Left, Boole Right)
    {
        static const String Delims = " \t\r";
        if(Right)
            Source.erase(Source.find_last_not_of(Delims)+1);
        if(Left)
            Source.erase(0,Source.find_first_not_of(Delims));
    }

    CountedPtr<StringVector> StringTools::Split(const String& Source, const String& Delims, const Whole& MaxSplits)
    {
        CountedPtr<StringVector> Ret(new StringVector);
        Ret->reserve( MaxSplits ? MaxSplits+1 : 10 );
        Whole Splits = 0;

        size_t Start = 0;
        size_t Pos = 0;

        do
        {
            Pos = Source.find_first_of(Delims,Start);
            if(Pos == Start)
            {
                Start = Pos + 1;
            }
            else if(Pos == String::npos || (MaxSplits && Splits == MaxSplits))
            {
                Ret->push_back(Source.substr(Start));
                break;
            }
            else
            {
                Ret->push_back(Source.substr(Start,Pos - Start));
                Start = Pos + 1;
            }
            Start = Source.find_first_not_of(Delims,Start);
            ++Splits;
        }while(Pos != String::npos);

        return Ret;
    }

    void StringTools::ToUpperCase(String& Source)
    {
        std::transform(Source.begin(),Source.end(),Source.begin(),::toupper);
    }

    String StringTools::UpperCaseCopy(String Source)
    {
        ToUpperCase(Source);
        return Source;
    }

    void StringTools::ToLowerCase(String& Source)
        { std::transform(Source.begin(),Source.end(),Source.begin(),::tolower); }

    String StringTools::LowerCaseCopy(String Source)
    {
        ToLowerCase(Source);
        return Source;
    }

    Boole StringTools::StartsWith(const String& Str, const String& Pattern, const Boole CaseSensitive)
    {
        size_t StrLen = Str.length();
        size_t PatternLen = Pattern.length();

        if(PatternLen > StrLen || PatternLen == 0)
            return false;

        String Start = Str.substr(0,PatternLen);

        if(CaseSensitive)
        {
            String LowerPattern = Pattern;
            ToLowerCase(Start);
            ToLowerCase(LowerPattern);
            return (Start == LowerPattern);
        }

        return (Start == Pattern);
    }

    Boole StringTools::EndsWith(const String& Str, const String& Pattern, const Boole CaseSensitive)
    {
        size_t StrLen = Str.length();
        size_t PatternLen = Pattern.length();

        if(PatternLen > StrLen || PatternLen == 0)
            return false;

        String End = Str.substr(StrLen - PatternLen,PatternLen);

        if( !CaseSensitive ) {
            String LowerPattern = Pattern;
            ToLowerCase(End);
            ToLowerCase(LowerPattern);
            return (End == LowerPattern);
        }

        return (End == Pattern);
    }

    void StringTools::RemoveDuplicateWhitespaces(String& Source)
    {
        for( size_t CurrIndex = Source.find_first_of("  ") ; CurrIndex != String::npos ; CurrIndex = Source.find_first_of("  ",CurrIndex) )
        {
            size_t EndIndex = CurrIndex;
            while( Source[EndIndex] == ' ' ) EndIndex++;
            Source.replace(CurrIndex,EndIndex-CurrIndex," ");
            CurrIndex++;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Data Class Utilities

    Vector2 StringTools::ConvertToVector2(const String& ToConvert)
    {
        CountedPtr<StringVector> Digits = Split(ToConvert);
        if(2 == Digits->size())
        {
            return Vector2(ConvertToReal(Digits->at(0)),ConvertToReal(Digits->at(1)));
        }else{
            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"String does not contain 2 digits when attempting to convert.");
        }
    }

    String StringTools::ConvertToString(const Vector2& ToConvert)
    {
        std::stringstream converter;
        converter << ToConvert.X << " " << ToConvert.Y;
        return converter.str();
    }

    Vector3 StringTools::ConvertToVector3(const String& ToConvert)
    {
        CountedPtr<StringVector> Digits = Split(ToConvert);
        if(3 == Digits->size())
        {
            return Vector3(ConvertToReal(Digits->at(0)),ConvertToReal(Digits->at(1)),ConvertToReal(Digits->at(2)));
        }else{
            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"String does not contain 3 digits when attempting to convert.");
        }
    }

    String StringTools::ConvertToString(const Vector3& ToConvert)
    {
        std::stringstream converter;
        converter << ToConvert.X << " " << ToConvert.Y << " " << ToConvert.Z;
        return converter.str();
    }

    Quaternion StringTools::ConvertToQuaternion(const String& ToConvert)
    {
        CountedPtr<StringVector> Digits = Split(ToConvert);
        if(4 == Digits->size())
        {
            return Quaternion(ConvertToReal(Digits->at(0)),ConvertToReal(Digits->at(1)),ConvertToReal(Digits->at(2)),ConvertToReal(Digits->at(3)));
        }else{
            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"String does not contain 4 digits when attempting to convert.");
        }
    }

    String StringTools::ConvertToString(const Quaternion& ToConvert)
    {
        std::stringstream converter;
        converter << ToConvert.X << " " << ToConvert.Y << " " << ToConvert.Z << " " << ToConvert.W;
        return converter.str();
    }

    ColourValue StringTools::ConvertToColourValue(const String& ToConvert)
    {
        CountedPtr<StringVector> Digits = Split(ToConvert);
        if(4 == Digits->size())
        {
            return ColourValue(ConvertToReal(Digits->at(0)),ConvertToReal(Digits->at(1)),ConvertToReal(Digits->at(2)),ConvertToReal(Digits->at(3)));
        }else{
            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"String does not contain 4 digits when attempting to convert.");
        }
    }

    String StringTools::ConvertToString(const ColourValue& ToConvert)
    {
        std::stringstream converter;
        converter << ToConvert.RedChannel << " " << ToConvert.GreenChannel << " " << ToConvert.BlueChannel << " " << ToConvert.AlphaChannel;
        return converter.str();
    }

    ColourValue StringTools::ConvertHexToColourValue(const String& ToConvert)
    {
        if( ToConvert.size() < 6 ) {
            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Hex code requires a minimum of 6 characters to express a ColourValue instance.");
        }

        ColourValue Ret;
        Ret.RedChannel = ConvertHexToColourChannel( ToConvert.substr(0,2) );
        Ret.GreenChannel = ConvertHexToColourChannel( ToConvert.substr(2,2) );
        Ret.BlueChannel = ConvertHexToColourChannel( ToConvert.substr(4,2) );

        if( ToConvert.size() == 6 ) Ret.AlphaChannel = 1.0;
        else Ret.AlphaChannel = ConvertHexToColourChannel( ToConvert.substr(6,2) );

        return Ret;
    }

    String StringTools::ConvertToHexString(const ColourValue& ToConvert)
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

    Boole StringTools::ConvertToBool(const String& ToConvert, const Boole Default)
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

    Real StringTools::ConvertToReal(const String& ToConvert)
    {
        StringStream converter(ToConvert);
        Real Result;
        converter >> Result;
        return Result;
    }

    Integer StringTools::ConvertToInteger(const String& ToConvert)
    {
        StringStream converter(ToConvert);
        Integer Result;
        converter >> Result;
        return Result;
    }

    Whole StringTools::ConvertToWhole(const String& ToConvert)
    {
        StringStream converter(ToConvert);
        Whole Result;
        converter >> Result;
        return Result;
    }

    Int8 StringTools::ConvertToInt8(const String& ToConvert)
    {
        StringStream converter(ToConvert);
        Int8 Result;
        converter >> Result;
        return Result;
    }

    UInt8 StringTools::ConvertToUInt8(const String& ToConvert)
    {
        StringStream converter(ToConvert);
        UInt8 Result;
        converter >> Result;
        return Result;
    }

    Int16 StringTools::ConvertToInt16(const String& ToConvert)
    {
        StringStream converter(ToConvert);
        Int16 Result;
        converter >> Result;
        return Result;
    }

    UInt16 StringTools::ConvertToUInt16(const String& ToConvert)
    {
        StringStream converter(ToConvert);
        UInt16 Result;
        converter >> Result;
        return Result;
    }

    Int32 StringTools::ConvertToInt32(const String& ToConvert)
    {
        StringStream converter(ToConvert);
        Int32 Result;
        converter >> Result;
        return Result;
    }

    UInt32 StringTools::ConvertToUInt32(const String& ToConvert)
    {
        StringStream converter(ToConvert);
        UInt32 Result;
        converter >> Result;
        return Result;
    }

    String StringTools::ConvertToString(const Input::InputCode& Code, Boole ShiftPressed)
    {
        /// @todo Get this ( StringTools::ConvertToString ) to support non us keyboards
        switch(Code)
        {
            case Input::KEY_A:             return ( ShiftPressed ? "A" : "a" );        break;
            case Input::KEY_B:             return ( ShiftPressed ? "B" : "b" );        break;
            case Input::KEY_C:             return ( ShiftPressed ? "C" : "c" );        break;
            case Input::KEY_D:             return ( ShiftPressed ? "D" : "d" );        break;
            case Input::KEY_E:             return ( ShiftPressed ? "E" : "e" );        break;
            case Input::KEY_F:             return ( ShiftPressed ? "F" : "f" );        break;
            case Input::KEY_G:             return ( ShiftPressed ? "G" : "g" );        break;
            case Input::KEY_H:             return ( ShiftPressed ? "H" : "h" );        break;
            case Input::KEY_I:             return ( ShiftPressed ? "I" : "i" );        break;
            case Input::KEY_J:             return ( ShiftPressed ? "J" : "j" );        break;
            case Input::KEY_K:             return ( ShiftPressed ? "K" : "k" );        break;
            case Input::KEY_L:             return ( ShiftPressed ? "L" : "l" );        break;
            case Input::KEY_M:             return ( ShiftPressed ? "M" : "m" );        break;
            case Input::KEY_N:             return ( ShiftPressed ? "N" : "n" );        break;
            case Input::KEY_O:             return ( ShiftPressed ? "O" : "o" );        break;
            case Input::KEY_P:             return ( ShiftPressed ? "P" : "p" );        break;
            case Input::KEY_Q:             return ( ShiftPressed ? "Q" : "q" );        break;
            case Input::KEY_R:             return ( ShiftPressed ? "R" : "r" );        break;
            case Input::KEY_S:             return ( ShiftPressed ? "S" : "s" );        break;
            case Input::KEY_T:             return ( ShiftPressed ? "T" : "t" );        break;
            case Input::KEY_U:             return ( ShiftPressed ? "U" : "u" );        break;
            case Input::KEY_V:             return ( ShiftPressed ? "V" : "v" );        break;
            case Input::KEY_W:             return ( ShiftPressed ? "W" : "w" );        break;
            case Input::KEY_X:             return ( ShiftPressed ? "X" : "x" );        break;
            case Input::KEY_Y:             return ( ShiftPressed ? "Y" : "y" );        break;
            case Input::KEY_Z:             return ( ShiftPressed ? "Z" : "z" );        break;
            case Input::KEY_1:             return ( ShiftPressed ? "!" : "1" );        break;
            case Input::KEY_2:             return ( ShiftPressed ? "@" : "2" );        break;
            case Input::KEY_3:             return ( ShiftPressed ? "#" : "3" );        break;
            case Input::KEY_4:             return ( ShiftPressed ? "$" : "4" );        break;
            case Input::KEY_5:             return ( ShiftPressed ? "%" : "5" );        break;
            case Input::KEY_6:             return ( ShiftPressed ? "^" : "6" );        break;
            case Input::KEY_7:             return ( ShiftPressed ? "&" : "7" );        break;
            case Input::KEY_8:             return ( ShiftPressed ? "*" : "8" );        break;
            case Input::KEY_9:             return ( ShiftPressed ? "(" : "9" );        break;
            case Input::KEY_0:             return ( ShiftPressed ? ")" : "0" );        break;
            case Input::KEY_MINUS:         return ( ShiftPressed ? "_" : "-" );        break;
            case Input::KEY_EQUALS:        return ( ShiftPressed ? "+" : "=" );        break;
            case Input::KEY_LEFTBRACKET:   return ( ShiftPressed ? "{" : "[" );        break;
            case Input::KEY_RIGHTBRACKET:  return ( ShiftPressed ? "}" : "]" );        break;
            case Input::KEY_BACKSLASH:     return ( ShiftPressed ? "\\" : "|" );       break;
            case Input::KEY_SEMICOLON:     return ( ShiftPressed ? ":" : ";" );        break;
            case Input::KEY_APOSTROPHE:    return ( ShiftPressed ? "\"" : "'" );       break;
            case Input::KEY_GRAVE:         return ( ShiftPressed ? "~" : "`" );        break;
            case Input::KEY_COMMA:         return ( ShiftPressed ? "<" : "," );        break;
            case Input::KEY_PERIOD:        return ( ShiftPressed ? ">" : "." );        break;
            case Input::KEY_SLASH:         return ( ShiftPressed ? "?" : "/" );        break;
            case Input::KEY_KP_1:          return "1";                                 break;
            case Input::KEY_KP_2:          return "2";                                 break;
            case Input::KEY_KP_3:          return "3";                                 break;
            case Input::KEY_KP_4:          return "4";                                 break;
            case Input::KEY_KP_5:          return "5";                                 break;
            case Input::KEY_KP_6:          return "6";                                 break;
            case Input::KEY_KP_7:          return "7";                                 break;
            case Input::KEY_KP_8:          return "8";                                 break;
            case Input::KEY_KP_9:          return "9";                                 break;
            case Input::KEY_KP_0:          return "0";                                 break;
            case Input::KEY_KP_PERIOD:     return ".";                                 break;
            default:                       return "";                                  break;
        }
    } // StringTools::ConvertToString


}//Mezzanine

#endif
