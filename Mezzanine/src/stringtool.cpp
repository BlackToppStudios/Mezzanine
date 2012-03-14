//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#include "world.h"
#include <sstream>
#include <algorithm>
#include <cctype>
//#include <locale>

namespace Mezzanine
{
    StringTool::StringTool()
    {
    }

    StringTool::~StringTool()
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    // String Manipulation and checks

    void StringTool::Trim(String& Source, bool Left, bool Right)
    {
        static const String Delims = " \t\r";
        if(Right)
            Source.erase(Source.find_last_not_of(Delims)+1);
        if(Left)
            Source.erase(0,Source.find_first_not_of(Delims));
    }

    std::vector<String> StringTool::Split(const String& Source, const String& Delims, const Whole& MaxSplits)
    {
        std::vector<String> Ret;
        Ret.reserve( MaxSplits ? MaxSplits+1 : 10 );
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
                Ret.push_back(Source.substr(Start));
                break;
            }
            else
            {
                Ret.push_back(Source.substr(Start,Pos - Start));
                Start = Pos + 1;
            }
            Start = Source.find_first_not_of(Delims,Start);
            ++Splits;
        }while(Pos != String::npos);

        return Ret;
    }

    void StringTool::ToUpperCase(String& Source)
    {
        std::transform(Source.begin(),Source.end(),Source.begin(),::toupper);
    }

    void StringTool::ToLowerCase(String& Source)
    {
        std::transform(Source.begin(),Source.end(),Source.begin(),::tolower);
    }

    bool StringTool::StartsWith(const String& Str, const String& Pattern, const bool CaseSensitive)
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

    bool StringTool::EndsWith(const String& Str, const String& Pattern, const bool CaseSensitive)
    {
        size_t StrLen = Str.length();
        size_t PatternLen = Pattern.length();

        if(PatternLen > StrLen || PatternLen == 0)
            return false;

        String End = Str.substr(StrLen - PatternLen,PatternLen);

        if(CaseSensitive)
        {
            String LowerPattern = Pattern;
            ToLowerCase(End);
            ToLowerCase(LowerPattern);
            return (End == LowerPattern);
        }

        return (End == Pattern);
    }

    void StringTool::RemoveDuplicateWhitespaces(String& Source)
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

    Vector2 StringTool::ConvertToVector2(const String& ToConvert)
    {
        std::vector<String> Digits = Split(ToConvert);
        if(2 == Digits.size())
        {
            return Vector2(ConvertToReal(Digits[0]),ConvertToReal(Digits[1]));
        }else{
            std::stringstream logstream;
            logstream << "String does not contain 2 digits when attempting to convert in StringTool::ConvertToVector2.";
            World::GetWorldPointer()->LogAndThrow(Exception(logstream.str()));
        }
    }

    String StringTool::ConvertToString(const Vector2& ToConvert)
    {
        std::stringstream converter;
        converter << ToConvert.X << " " << ToConvert.Y;
        return converter.str();
    }

    Vector3 StringTool::ConvertToVector3(const String& ToConvert)
    {
        std::vector<String> Digits = Split(ToConvert);
        if(3 == Digits.size())
        {
            return Vector3(ConvertToReal(Digits[0]),ConvertToReal(Digits[1]),ConvertToReal(Digits[2]));
        }else{
            std::stringstream logstream;
            logstream << "String does not contain 3 digits when attempting to convert in StringTool::ConvertToVector3.";
            World::GetWorldPointer()->LogAndThrow(Exception(logstream.str()));
        }
    }

    String StringTool::ConvertToString(const Vector3& ToConvert)
    {
        std::stringstream converter;
        converter << ToConvert.X << " " << ToConvert.Y << " " << ToConvert.Z;
        return converter.str();
    }

    Quaternion StringTool::ConvertToQuaternion(const String& ToConvert)
    {
        std::vector<String> Digits = Split(ToConvert);
        if(4 == Digits.size())
        {
            return Quaternion(ConvertToReal(Digits[0]),ConvertToReal(Digits[1]),ConvertToReal(Digits[2]),ConvertToReal(Digits[3]));
        }else{
            std::stringstream logstream;
            logstream << "String does not contain 4 digits when attempting to convert in StringTool::ConvertToQuaternion.";
            World::GetWorldPointer()->LogAndThrow(Exception(logstream.str()));
        }
    }

    String StringTool::ConvertToString(const Quaternion& ToConvert)
    {
        std::stringstream converter;
        converter << ToConvert.X << " " << ToConvert.Y << " " << ToConvert.Z << " " << ToConvert.W;
        return converter.str();
    }

    ColourValue StringTool::ConvertToColourValue(const String& ToConvert)
    {
        std::vector<String> Digits = Split(ToConvert);
        if(4 == Digits.size())
        {
            return ColourValue(ConvertToReal(Digits[0]),ConvertToReal(Digits[1]),ConvertToReal(Digits[2]),ConvertToReal(Digits[3]));
        }else{
            std::stringstream logstream;
            logstream << "String does not contain 4 digits when attempting to convert in StringTool::ConvertToColourValue.";
            World::GetWorldPointer()->LogAndThrow(Exception(logstream.str()));
        }
    }

    String StringTool::ConvertToString(const ColourValue& ToConvert)
    {
        std::stringstream converter;
        converter << ToConvert.R << " " << ToConvert.G << " " << ToConvert.B << " " << ToConvert.A;
        return converter.str();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Convert-To-Data functions

    bool StringTool::ConvertToBool(const String& ToConvert, const bool Default)
    {
        String StrCopy = ToConvert;
        StringTool::ToLowerCase(StrCopy);
        if("true" == StrCopy) return true;
        else if("yes" == StrCopy) return true;
        else if("1" == StrCopy) return true;
        else if("false" == StrCopy) return false;
        else if("no" == StrCopy) return false;
        else if("0" == StrCopy) return false;
        else return Default;
    }

    Real StringTool::ConvertToReal(const String& ToConvert)
    {
        std::stringstream converter(ToConvert);
        Real Result;
        converter >> Result;
        return Result;
    }

    Int8 StringTool::ConvertToInt8(const String& ToConvert)
    {
        std::stringstream converter(ToConvert);
        Int8 Result;
        converter >> Result;
        return Result;
    }

    UInt8 StringTool::ConvertToUInt8(const String& ToConvert)
    {
        std::stringstream converter(ToConvert);
        UInt8 Result;
        converter >> Result;
        return Result;
    }

    Int16 StringTool::ConvertToInt16(const String& ToConvert)
    {
        std::stringstream converter(ToConvert);
        Int16 Result;
        converter >> Result;
        return Result;
    }

    UInt16 StringTool::ConvertToUInt16(const String& ToConvert)
    {
        std::stringstream converter(ToConvert);
        UInt16 Result;
        converter >> Result;
        return Result;
    }

    Int32 StringTool::ConvertToInt32(const String& ToConvert)
    {
        std::stringstream converter(ToConvert);
        Int32 Result;
        converter >> Result;
        return Result;
    }

    UInt32 StringTool::ConvertToUInt32(const String& ToConvert)
    {
        std::stringstream converter(ToConvert);
        UInt32 Result;
        converter >> Result;
        return Result;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Convert-To-String functions

    String StringTool::ConvertToString(const Real& ToConvert)
    {
        std::stringstream converter;
        converter << ToConvert;
        return converter.str();
    }

    String StringTool::ConvertToString(const Int8& ToConvert)
    {
        std::stringstream converter;
        converter << ToConvert;
        return converter.str();
    }

    String StringTool::ConvertToString(const UInt8& ToConvert)
    {
        std::stringstream converter;
        converter << ToConvert;
        return converter.str();
    }

    String StringTool::ConvertToString(const Int16& ToConvert)
    {
        std::stringstream converter;
        converter << ToConvert;
        return converter.str();
    }

    String StringTool::ConvertToString(const UInt16& ToConvert)
    {
        std::stringstream converter;
        converter << ToConvert;
        return converter.str();
    }

    String StringTool::ConvertToString(const Int32& ToConvert)
    {
        std::stringstream converter;
        converter << ToConvert;
        return converter.str();
    }

    String StringTool::ConvertToString(const UInt32& ToConvert)
    {
        std::stringstream converter;
        converter << ToConvert;
        return converter.str();
    }

    String StringTool::ConvertToString(const MetaCode::InputCode& Code, bool ShiftPressed)
    {
        switch(Code)
        {
            case MetaCode::KEY_A:
            {
                if(ShiftPressed) return "A";
                else return "a";
                break;
            }
            case MetaCode::KEY_B:
            {
                if(ShiftPressed) return "B";
                else return "b";
                break;
            }
            case MetaCode::KEY_C:
            {
                if(ShiftPressed) return "C";
                else return "c";
                break;
            }
            case MetaCode::KEY_D:
            {
                if(ShiftPressed) return "D";
                else return "d";
                break;
            }
            case MetaCode::KEY_E:
            {
                if(ShiftPressed) return "E";
                else return "e";
                break;
            }
            case MetaCode::KEY_F:
            {
                if(ShiftPressed) return "F";
                else return "f";
                break;
            }
            case MetaCode::KEY_G:
            {
                if(ShiftPressed) return "G";
                else return "g";
                break;
            }
            case MetaCode::KEY_H:
            {
                if(ShiftPressed) return "H";
                else return "h";
                break;
            }
            case MetaCode::KEY_I:
            {
                if(ShiftPressed) return "I";
                else return "i";
                break;
            }
            case MetaCode::KEY_J:
            {
                if(ShiftPressed) return "J";
                else return "j";
                break;
            }
            case MetaCode::KEY_K:
            {
                if(ShiftPressed) return "K";
                else return "k";
                break;
            }
            case MetaCode::KEY_L:
            {
                if(ShiftPressed) return "L";
                else return "l";
                break;
            }
            case MetaCode::KEY_M:
            {
                if(ShiftPressed) return "M";
                else return "m";
                break;
            }
            case MetaCode::KEY_N:
            {
                if(ShiftPressed) return "N";
                else return "n";
                break;
            }
            case MetaCode::KEY_O:
            {
                if(ShiftPressed) return "O";
                else return "o";
                break;
            }
            case MetaCode::KEY_P:
            {
                if(ShiftPressed) return "P";
                else return "p";
                break;
            }
            case MetaCode::KEY_Q:
            {
                if(ShiftPressed) return "Q";
                else return "q";
                break;
            }
            case MetaCode::KEY_R:
            {
                if(ShiftPressed) return "R";
                else return "r";
                break;
            }
            case MetaCode::KEY_S:
            {
                if(ShiftPressed) return "S";
                else return "s";
                break;
            }
            case MetaCode::KEY_T:
            {
                if(ShiftPressed) return "T";
                else return "t";
                break;
            }
            case MetaCode::KEY_U:
            {
                if(ShiftPressed) return "U";
                else return "u";
                break;
            }
            case MetaCode::KEY_V:
            {
                if(ShiftPressed) return "V";
                else return "v";
                break;
            }
            case MetaCode::KEY_W:
            {
                if(ShiftPressed) return "W";
                else return "w";
                break;
            }
            case MetaCode::KEY_X:
            {
                if(ShiftPressed) return "X";
                else return "x";
                break;
            }
            case MetaCode::KEY_Y:
            {
                if(ShiftPressed) return "Y";
                else return "y";
                break;
            }
            case MetaCode::KEY_Z:
            {
                if(ShiftPressed) return "Z";
                else return "z";
                break;
            }
            case MetaCode::KEY_1:
            {
                if(ShiftPressed) return "!";
                else return "1";
                break;
            }
            case MetaCode::KEY_2:
            {
                if(ShiftPressed) return "@";
                else return "2";
                break;
            }
            case MetaCode::KEY_3:
            {
                if(ShiftPressed) return "#";
                else return "3";
                break;
            }
            case MetaCode::KEY_4:
            {
                if(ShiftPressed) return "$";
                else return "4";
                break;
            }
            case MetaCode::KEY_5:
            {
                if(ShiftPressed) return "%";
                else return "5";
                break;
            }
            case MetaCode::KEY_6:
            {
                if(ShiftPressed) return "^";
                else return "6";
                break;
            }
            case MetaCode::KEY_7:
            {
                if(ShiftPressed) return "&";
                else return "7";
                break;
            }
            case MetaCode::KEY_8:
            {
                if(ShiftPressed) return "*";
                else return "8";
                break;
            }
            case MetaCode::KEY_9:
            {
                if(ShiftPressed) return "(";
                else return "9";
                break;
            }
            case MetaCode::KEY_0:
            {
                if(ShiftPressed) return ")";
                else return "0";
                break;
            }
            case MetaCode::KEY_MINUS:
            {
                if(ShiftPressed) return "_";
                else return "-";
                break;
            }
            case MetaCode::KEY_EQUALS:
            {
                if(ShiftPressed) return "+";
                else return "=";
                break;
            }
            case MetaCode::KEY_LEFTBRACKET:
            {
                if(ShiftPressed) return "{";
                else return "[";
                break;
            }
            case MetaCode::KEY_RIGHTBRACKET:
            {
                if(ShiftPressed) return "}";
                else return "]";
                break;
            }
            case MetaCode::KEY_BACKSLASH:
            {
                if(ShiftPressed) return "\\";
                else return "|";
                break;
            }
            case MetaCode::KEY_SEMICOLON:
            {
                if(ShiftPressed) return ":";
                else return ";";
                break;
            }
            case MetaCode::KEY_APOSTROPHE:
            {
                if(ShiftPressed) return "\"";
                else return "'";
                break;
            }
            case MetaCode::KEY_GRAVE:
            {
                if(ShiftPressed) return "~";
                else return "`";
                break;
            }
            case MetaCode::KEY_COMMA:
            {
                if(ShiftPressed) return "<";
                else return ",";
                break;
            }
            case MetaCode::KEY_PERIOD:
            {
                if(ShiftPressed) return ">";
                else return ".";
                break;
            }
            case MetaCode::KEY_SLASH:
            {
                if(ShiftPressed) return "?";
                else return "/";
                break;
            }
            case MetaCode::KEY_KP_1:
            {
                return "1";
                break;
            }
            case MetaCode::KEY_KP_2:
            {
                return "2";
                break;
            }
            case MetaCode::KEY_KP_3:
            {
                return "3";
                break;
            }
            case MetaCode::KEY_KP_4:
            {
                return "4";
                break;
            }
            case MetaCode::KEY_KP_5:
            {
                return "5";
                break;
            }
            case MetaCode::KEY_KP_6:
            {
                return "6";
                break;
            }
            case MetaCode::KEY_KP_7:
            {
                return "7";
                break;
            }
            case MetaCode::KEY_KP_8:
            {
                return "8";
                break;
            }
            case MetaCode::KEY_KP_9:
            {
                return "9";
                break;
            }
            case MetaCode::KEY_KP_0:
            {
                return "0";
                break;
            }
            case MetaCode::KEY_KP_PERIOD:
            {
                return ".";
                break;
            }
            default:
                return "";
                break;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // String Catenate functions

    String StringTool::StringCat(const String& Front, const String& Back)
    {
        std::stringstream Adder;
        Adder << Front << Back;
        return Adder.str();
    }

    String StringTool::StringCat(const String& Front, const String& Middle, const String& Back)
    {
        std::stringstream Adder;
        Adder << Front  << Middle << Back;
        return Adder.str();
    }

    String StringTool::StringCat(const String& Front, const String& Middle1, const String& Middle2, const String& Back)
    {
        std::stringstream Adder;
        Adder << Front  << Middle1 << Middle2 << Back;
        return Adder.str();
    }

    String StringTool::StringCat(const String& Front, const String& Middle1, const String& Middle2, const String& Middle3, const String& Back)
    {
        std::stringstream Adder;
        Adder << Front  << Middle1 << Middle2 << Middle3 << Back;
        return Adder.str();
    }

    String StringTool::StringCat(const String& Front, const String& Middle1, const String& Middle2, const String& Middle3, const String& Middle4, const String& Back)
    {
        std::stringstream Adder;
        Adder << Front  << Middle1 << Middle2 << Middle3 << Middle4 << Back;
        return Adder.str();
    }
}//Mezzanine

#endif
