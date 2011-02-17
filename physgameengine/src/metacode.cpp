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
#ifndef METACODE_CPP
#define METACODE_CPP
///////////////////////////////////////////////////////////////////////////////
// This will expose all keyboard and mouse, joystick and other userinput events
// to developers, we are using the SDL keymap to get us started, large portions
// are directly copy/pasta'd, so we included their license too
///////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////

#include "exception.h"
#include "metacode.h"

#include <assert.h>
#include <ostream>
#include <memory>

#include "SDL.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// MetaCode
///////////////////////////////////////
namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    // Creation and Deletion Methods
    ///////////////////////////////////////
    MetaCode::MetaCode()
    {}

    MetaCode::MetaCode(const int &MetaValue_, const MetaCode::InputCode &Code_)
    {
        Construct(MetaValue_, Code_);
    }

    MetaCode::MetaCode(const RawEvent &RawEvent_)
    {
        Construct(RawEvent_);
    }

    void MetaCode::Construct(const RawEvent &RawEvent_)
    {
        //create a safe but gibberish default
        Construct(BUTTON_UP, KEY_FIRST);
        /// @todo TODO: Actually process each event
        switch(RawEvent_.type)
        {
            case SDL_KEYDOWN:
                //    Construct(MetaValue_, ID_, Code_);
                Construct(BUTTON_PRESSING, GetInputCodeFromSDL_KEY(RawEvent_));
                break;
            case SDL_KEYUP:
                Construct(BUTTON_LIFTING, GetInputCodeFromSDL_KEY(RawEvent_));
                break;

            case SDL_JOYAXISMOTION:/// @todo TODO determine if Joystick RawEvents will emit 1 or multiple Metacodes
                break;
            case SDL_JOYBUTTONDOWN:
                break;
            case SDL_JOYBUTTONUP:
                break;
            case SDL_JOYBALLMOTION:
                break;
            case SDL_JOYHATMOTION:
                break;

            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                throw ("RawEvent which creates Multiple Metacodes inserted into Metacode");
                break;

            default:
                throw ("Unknown User Input Inserted into Metacode");
                break;
        }
    }

    void MetaCode::Construct(const int &MetaValue_, const MetaCode::InputCode &Code_)
    {
        SetMetaValue(MetaValue_);
        SetCode(Code_);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Dirty Hacks
    ///////////////////////////////////////

    //This function assumes the RawEvent is a valid SDL Keyevent
    MetaCode::InputCode MetaCode::GetInputCodeFromSDL_KEY(const RawEvent &RawEvent_)
    {
        //This Whole thing will only work with SDL Keyboard events. If we switch out event subsystems this is one of those that must change it.
        MetaCode::InputCode To;

        assert( sizeof(To)==sizeof(RawEvent_.key.keysym.sym) );
        memcpy( &To, &(RawEvent_.key.keysym.sym), sizeof(RawEvent_.key.keysym.sym));

        return To;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Gets and Sets
    ///////////////////////////////////////
    int MetaCode::GetMetaValue() const
    {
        return this->MetaValue;
    }

    MetaCode::InputCode MetaCode::GetCode() const
    {
        return this->Code;
    }

    void MetaCode::SetMetaValue(const int &MetaValue_)
    {
        this->MetaValue=MetaValue_;
    }

    void MetaCode::SetCode(const MetaCode::InputCode &Code_)
    {
        this->Code=Code_;
    }

    void MetaCode::SetCode(int Code_)
    {
        this->Code=(MetaCode::InputCode)Code_;
    }

/*    void MetaCode::SetID(const short unsigned int &ID_)
    {
        this->ID=ID_;
    }*/


    MetaCode::InputCode MetaCode::GetMouseButtonCode(int ButtonNumber)
    {
        MetaCode::InputCode Answer = (MetaCode::InputCode)(ButtonNumber + (int)MetaCode::MOUSEBUTTON);
        if ( MetaCode::MOUSEBUTTON_FIRST > Answer && MetaCode::MOUSEBUTTON_LAST < Answer)
            { throw (Exception("Unsupported mouse Button.")); }
        return Answer;

        // I will hang to this just incase we want the maximum in type safety.
        /*switch(ButtonNumber)
        {
            case 0: return MetaCode::MOUSEBUTTON;
            case 1: return MetaCode::MOUSEBUTTON_1;
            case 2: return MetaCode::MOUSEBUTTON_2;
            case 3: return MetaCode::MOUSEBUTTON_3;
            case 4: return MetaCode::MOUSEBUTTON_4;
            case 5: return MetaCode::MOUSEBUTTON_5;
            case 6: return MetaCode::MOUSEBUTTON_6;
            case 7: return MetaCode::MOUSEBUTTON_7;
            case 8: return MetaCode::MOUSEBUTTON_8;
            case 9: return MetaCode::MOUSEBUTTON_9;
            case 10: return MetaCode::MOUSEBUTTON_10;
            case 11: return MetaCode::MOUSEBUTTON_11;
            case 12: return MetaCode::MOUSEBUTTON_12;
            case 13: return MetaCode::MOUSEBUTTON_13;
            case 14: return MetaCode::MOUSEBUTTON_14;
            case 15: return MetaCode::MOUSEBUTTON_15;
            case 16: return MetaCode::MOUSEBUTTON_16;
            case 17: return MetaCode::MOUSEBUTTON_17;
            case 18: return MetaCode::MOUSEBUTTON_18;
            case 19: return MetaCode::MOUSEBUTTON_19;
            case 20: return MetaCode::MOUSEBUTTON_20;
            default:
                throw (Exception("Unsupported mouse Button."));
                return MetaCode::MOUSEBUTTON;
        }*/
    }

    bool MetaCode::IsKeyboardButton() const
    {
        return (MetaCode::KEY_FIRST <= this->Code && this->Code <= MetaCode::KEY_LAST);
    }

    bool MetaCode::IsMouseButton() const
    {
        return (MetaCode::MOUSEBUTTON_FIRST <= this->Code && this->Code <= MetaCode::MOUSEBUTTON_LAST);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Operators
    ///////////////////////////////////////

    bool MetaCode::operator==(const MetaCode &other) const
    {
        if(this->Code == other.Code && this->MetaValue == other.MetaValue)
        {
            return true;
        }else{
            return false;
        }
    }
}

std::ostream& operator << (std::ostream& stream, const phys::MetaCode& x)
{
    stream << "<MetaCode Version=\"1\" MetaValue=\"" << x.GetMetaValue() << "\"" << " Code=\"" << x.GetCode() <<  "\" />";
    return stream;
}

#ifdef PHYSXML
std::istream& PHYS_LIB operator >> (std::istream& stream, phys::MetaCode& x)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "MetaCode", OneTag) );

    Doc->GetFirstChild() >> x;

    return stream;
}

phys::xml::Node& operator >> (const phys::xml::Node& OneNode, phys::MetaCode& x)
{
    if(OneNode.GetAttribute("Version").AsInt() == 1)
    {
        x.SetMetaValue(OneNode.GetAttribute("MetaValue").AsInt());
        x.SetCode(OneNode.GetAttribute("Code").AsInt());
    }else{
        throw( phys::Exception("Incompatible XML Version for MetaCode: Not Version 1"));
    }
}
#endif // \PHYSXML

#endif
