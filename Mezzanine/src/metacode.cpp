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
#ifndef METACODE_CPP
#define METACODE_CPP

#include "world.h" //only used for logging

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////

#include "exception.h"
#include "metacode.h"
#include "stringtool.h"

#include <assert.h>
#include <ostream>
#include <cstring>
#include <memory>

#include "SDL.h"
#include "serialization.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// MetaCode
///////////////////////////////////////
namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // Creation and Deletion Methods
    ///////////////////////////////////////
    MetaCode::MetaCode():MetaValue(0),Code(KEY_UNKNOWN)
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
        #ifdef MEZZDEBUG
        World::GetWorldPointer()->Log("Entering: MetaCode::Construct(RawEvent);");
        #endif
        switch(RawEvent_.type)
        {
            case SDL_KEYDOWN:
                Construct(BUTTON_PRESSING, GetInputCodeFromSDL_KEY(RawEvent_));
                break;
            case SDL_KEYUP:
                Construct(BUTTON_LIFTING, GetInputCodeFromSDL_KEY(RawEvent_));
                break;
            case SDL_MOUSEBUTTONDOWN:
                Construct(BUTTON_PRESSING, GetInputCodeFromSDL_MOUSE(RawEvent_));
                break;
            case SDL_MOUSEBUTTONUP:
                Construct(BUTTON_LIFTING, GetInputCodeFromSDL_MOUSE(RawEvent_));
                break;
            case SDL_JOYBUTTONDOWN:
                Construct(BUTTON_PRESSING, GetInputCodeFromSDL_JOYSTICK(RawEvent_));
                break;
            case SDL_JOYBUTTONUP:
                Construct(BUTTON_LIFTING, GetInputCodeFromSDL_JOYSTICK(RawEvent_));
                break;

            // Fail when incorrectly constructed
            case SDL_JOYBALLMOTION:     case SDL_JOYHATMOTION:
            case SDL_JOYAXISMOTION:     case SDL_MOUSEMOTION:
                Construct(BUTTON_UP, KEY_FIRST);            //create a safe but gibberish default
                throw ("RawEvent which creates Multiple Metacodes inserted into Metacode");
                break;

            default:
                Construct(BUTTON_UP, KEY_FIRST);            //create a safe but gibberish default
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
        memcpy( &To, &(RawEvent_.key.keysym.scancode), sizeof(RawEvent_.key.keysym.scancode));

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

    MetaCode::InputCode MetaCode::GetInputCodeFromSDL_JOYSTICK(const RawEvent &RawEvent_)
        { return GetJoystickButtonCode(RawEvent_.jbutton.button); }

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

    MetaCode::DirectionalMotionState MetaCode::GetMetaValueAsDirectionalMotionState() const
    {
        if( MetaCode::DIRECTIONALMOTION_DOWNRIGHT <= this->MetaValue && MetaCode::DIRECTIONALMOTION_UPLEFT >= this->MetaValue)
        {
            return (MetaCode::DirectionalMotionState) this->MetaValue;
        }else{
            throw("Invalid DirectionalMotionState in MetaValue");
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
            { MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Unsupported mouse Button."); }
        return Answer;
    }

    MetaCode::InputCode MetaCode::GetJoystickButtonCode(short unsigned int ButtonNumber)
    {
        MetaCode::InputCode Answer = (MetaCode::InputCode)(ButtonNumber + (int)MetaCode::JOYSTICKBUTTON);
        if ( MetaCode::JOYSTICKBUTTON_FIRST > Answer && MetaCode::JOYSTICKBUTTON_LAST < Answer)
            { MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Unsupported Joystick Button."); }
        return Answer;
    }

    MetaCode::InputCode MetaCode::GetJoystickAxisCode(short unsigned int AxisNumber)
    {
        MetaCode::InputCode Answer = (MetaCode::InputCode)(AxisNumber + (int)MetaCode::JOYSTICKAXIS);
        if ( MetaCode::JOYSTICKAXIS_FIRST > Answer && MetaCode::JOYSTICKAXIS_LAST < Answer)
            { MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Unsupported Joystick AXIS."); }
        return Answer;
    }

    bool MetaCode::IsKeyboardButton() const
        { return (MetaCode::KEY_FIRST <= this->Code && this->Code <= MetaCode::KEY_LAST); }

    bool MetaCode::IsAltKey() const
        { return (MetaCode::KEY_LALT == this->Code || this->Code <= MetaCode::KEY_RALT); }

    bool MetaCode::IsCtrlKey() const
        { return (MetaCode::KEY_LCTRL == this->Code || this->Code <= MetaCode::KEY_RCTRL); }

    bool MetaCode::IsShiftKey() const
        { return (MetaCode::KEY_LSHIFT == this->Code || this->Code <= MetaCode::KEY_RSHIFT); }

    bool MetaCode::IsSuperKey() const
        { return (MetaCode::KEY_LSUPER == this->Code || this->Code <= MetaCode::KEY_RSUPER); }

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
    // Mike's portion of adding serializable to the metacode starts here
    #ifdef MEZZXML
    // Serializable
    void MetaCode::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        Mezzanine::xml::Node MetaNode = CurrentRoot.AppendChild(SerializableName());
        MetaNode.SetName(SerializableName());

        Mezzanine::xml::Attribute VersionAttr = MetaNode.AppendAttribute("Version");
        Mezzanine::xml::Attribute MetaValue_ = MetaNode.AppendAttribute("MetaValue");
        Mezzanine::xml::Attribute Code_ = MetaNode.AppendAttribute("Code");

        if( VersionAttr && MetaValue_ && Code_ )
        {
            if( VersionAttr.SetValue("1") && Code_.SetValue(Code) && MetaValue_.SetValue(MetaValue) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values", SerializableName(),true);
            }
        }else{
            SerializeError("Create XML Attributes", SerializableName(),true);
        }
    }

    // DeSerializable
    void MetaCode::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==Mezzanine::String(SerializableName()) )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                SetCode(OneNode.GetAttribute("Code").AsInt());
                MetaValue=OneNode.GetAttribute("MetaValue").AsInt();
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + SerializableName() + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a " + SerializableName() + ", found a " + String(OneNode.Name()) + ".");
        }
    }

    String MetaCode::SerializableName() const
        { return String("MetaCode"); }
    #endif // end MEZZXML
    // end of mike's serializable
}

std::ostream& operator << (std::ostream& stream, const Mezzanine::MetaCode& x)
{
    //stream << "<MetaCode Version=\"1\" MetaValue=\"" << x.GetMetaValue() << "\"" << " Code=\"" << x.GetCode() <<  "\" />";
    Serialize(stream,x);
    return stream;
}

#ifdef MEZZXML
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::MetaCode& x)
/*{
    Mezzanine::String OneTag( Mezzanine::xml::GetOneTag(stream) );
    std::auto_ptr<Mezzanine::xml::Document> Doc( Mezzanine::xml::PreParseClassFromSingleTag("Mezzanine::", "MetaCode", OneTag) );

    Doc->GetFirstChild() >> x;

    return stream;
}*/
{ return DeSerialize(stream, x); }

Mezzanine::xml::Node& operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::MetaCode& x)
/*{
    if ( Mezzanine::String(OneNode.Name())==Mezzanine::String("MetaCode") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            x.SetMetaValue(OneNode.GetAttribute("MetaValue").AsInt());
            x.SetCode(OneNode.GetAttribute("Code").AsInt());
        }else{
            throw( Mezzanine::Exception("Incompatible XML Version for MetaCode: Not Version 1"));
        }
    }else{
        throw( Mezzanine::Exception(Mezzanine::StringCat("Attempting to deserialize a MetaCode, found a ", OneNode.Name())));
    }
}*/
{x.ProtoDeSerialize(OneNode); }
#endif // \MEZZXML

#endif
