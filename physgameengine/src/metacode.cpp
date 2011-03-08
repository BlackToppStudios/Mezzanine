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

#include "world.h" //only used for logging

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
        Construct(BUTTON_UP, KEY_FIRST);        //create a safe but gibberish default
        /// @todo TODO: Actually process each event
        switch(RawEvent_.type)
        {
            case SDL_KEYDOWN:
                Construct(BUTTON_PRESSING, GetInputCodeFromSDL_KEY(RawEvent_));
                break;
            case SDL_KEYUP:
                Construct(BUTTON_LIFTING, GetInputCodeFromSDL_KEY(RawEvent_));
                break;
            case SDL_MOUSEBUTTONDOWN: //in some case will generate a mousewheel and a mouse button event
                Construct(BUTTON_LIFTING, GetInputCodeFromSDL_MOUSE(RawEvent_));
                break;
            case SDL_MOUSEBUTTONUP: //in some case will generate a mousewheel and a mouse button event
                Construct(BUTTON_LIFTING, GetInputCodeFromSDL_MOUSE(RawEvent_));
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

        phys::World::GetWorldPointer()->LogStream << "SDLK_ESCAPE:" << SDLK_ESCAPE;

        //This Whole thing will only work with SDL Keyboard events. If we switch out event subsystems this is one of those that must change it.
        MetaCode::InputCode To;

        assert( sizeof(To)==sizeof(RawEvent_.key.keysym.sym) );
        memcpy( &To, &(RawEvent_.key.keysym.sym), sizeof(RawEvent_.key.keysym.sym));

        return To;
    }

    MetaCode::InputCode MetaCode::GetInputCodeFromSDL_MOUSE(const RawEvent &RawEvent_)
    {
        switch (RawEvent_.button.button )
        {
            case SDL_BUTTON_LEFT: return MetaCode::MOUSEBUTTON_1;
            case SDL_BUTTON_RIGHT: return MetaCode::MOUSEBUTTON_2;
            case SDL_BUTTON_MIDDLE: return MetaCode::MOUSEBUTTON_3;
            case SDL_BUTTON_X1: return MetaCode::MOUSEBUTTON_4;
            case SDL_BUTTON_X2: return MetaCode::MOUSEBUTTON_5;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Conversion and Casting Functions
    ///////////////////////////////////////
    MetaCode::ButtonState MetaCode::GetMetaValueAsButtonState() const
    {
        if( MetaCode::BUTTON_LIFTING <= this->MetaValue && MetaCode::BUTTON_DOWN >= this->MetaValue)
        {
            return (MetaCode::ButtonState) this->MetaValue;
        }else{
            throw("Invalid ButtonState in MetaValue");
        }
    }

    MetaCode::MouseWheelState MetaCode::GetMetaValueAsMouseWheelState() const
    {
        if( MetaCode::MOUSEWHEEL_DOWN <= this->MetaValue && MetaCode::MOUSEWHEEL_UP >= this->MetaValue)
        {
            return (MetaCode::MouseWheelState) this->MetaValue;
        }else{
            throw("Invalid MouseWheelState in MetaValue");
        }
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

    MetaCode::InputCode MetaCode::GetMouseButtonCode(short unsigned int ButtonNumber)
    {
        MetaCode::InputCode Answer = (MetaCode::InputCode)(ButtonNumber + (int)MetaCode::MOUSEBUTTON);
        if ( MetaCode::MOUSEBUTTON_FIRST > Answer && MetaCode::MOUSEBUTTON_LAST < Answer)
            { throw (Exception("Unsupported mouse Button.")); }
        return Answer;
    }

    bool MetaCode::IsKeyboardButton() const
        { return (MetaCode::KEY_FIRST <= this->Code && this->Code <= MetaCode::KEY_LAST); }

    bool MetaCode::IsMouseButton() const
        { return (MetaCode::MOUSEBUTTON_FIRST <= this->Code && this->Code <= MetaCode::MOUSEBUTTON_LAST); }

    bool MetaCode::IsPollable() const
        { return ( IsKeyboardButton() || IsMouseButton() || this->Code==MetaCode::JOYSTICKBUTTON); }

    bool MetaCode::IsJoyStickEvent() const
        { return (MetaCode::JOYSTICK_FIRST <= this->Code && this->Code <= MetaCode::JOYSTICK_LAST); }

    bool MetaCode::IsOtherInputEvent() const
        { return (MetaCode::INPUTEVENT_FIRST <= this->Code && this->Code <= MetaCode::INPUTEVENT_LAST); }

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
