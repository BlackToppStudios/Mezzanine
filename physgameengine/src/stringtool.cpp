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
#ifndef _stringtool_cpp
#define _stringtool_cpp

#include "stringtool.h"
#include <sstream>

namespace phys
{
    StringTool::StringTool()
    {
    }

    StringTool::~StringTool()
    {
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
}//phys

#endif
