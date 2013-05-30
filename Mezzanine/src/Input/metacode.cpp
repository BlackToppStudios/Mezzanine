//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _inputmetacode_cpp
#define _inputmetacode_cpp

#include "entresol.h" //only used for logging

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////

#include "exception.h"
#include "Input/metacode.h"
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
    MetaCode::MetaCode() : MetaValue(0), Code(Input::KEY_UNKNOWN), DeviceIndex(-1)
    {}

    MetaCode::MetaCode(const int& MetaValue_, const Input::InputCode& Code_) : DeviceIndex(-1)
    {
        Construct(MetaValue_, Code_);
    }

    MetaCode::MetaCode(const int& MetaValue_, const Input::InputCode& Code_, const UInt16& DeviceIndex_)
    {
        Construct(MetaValue_, Code_);
        SetDeviceIndex(DeviceIndex_);
    }

    MetaCode::MetaCode(const RawEvent& RawEvent_)
    {
        Construct(RawEvent_);
    }

    void MetaCode::Construct(const RawEvent& RawEvent_)
    {
        #ifdef MEZZDEBUG
        Entresol::GetSingletonPtr()->Log("Entering: MetaCode::Construct(RawEvent);");
        #endif
        switch(RawEvent_.type)
        {
            case SDL_KEYDOWN:
                Construct(Input::BUTTON_PRESSING, GetInputCodeFromSDL_KEY(RawEvent_));
                break;
            case SDL_KEYUP:
                Construct(Input::BUTTON_LIFTING, GetInputCodeFromSDL_KEY(RawEvent_));
                break;
            case SDL_MOUSEBUTTONDOWN:
                Construct(Input::BUTTON_PRESSING, GetInputCodeFromSDL_MOUSE(RawEvent_));
                break;
            case SDL_MOUSEBUTTONUP:
                Construct(Input::BUTTON_LIFTING, GetInputCodeFromSDL_MOUSE(RawEvent_));
                break;
            case SDL_JOYBUTTONDOWN:
                Construct(Input::BUTTON_PRESSING, GetInputCodeFromSDL_JOYSTICK(RawEvent_));
                break;
            case SDL_JOYBUTTONUP:
                Construct(Input::BUTTON_LIFTING, GetInputCodeFromSDL_JOYSTICK(RawEvent_));
                break;

            // Fail when incorrectly constructed
            case SDL_JOYBALLMOTION:     case SDL_JOYHATMOTION:
            case SDL_JOYAXISMOTION:     case SDL_MOUSEMOTION:
                Construct(Input::BUTTON_UP, Input::KEY_FIRST);            //create a safe but gibberish default
                throw ("RawEvent which creates Multiple Metacodes inserted into Metacode");
                break;

            default:
                Construct(Input::BUTTON_UP, Input::KEY_FIRST);            //create a safe but gibberish default
                throw ("Unknown User Input Inserted into Metacode");
                break;
        }
    }

    void MetaCode::Construct(const int &MetaValue_, const Input::InputCode &Code_)
    {
        SetMetaValue(MetaValue_);
        SetCode(Code_);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Dirty Hacks
    ///////////////////////////////////////

    //This function assumes the RawEvent is a valid SDL Keyevent
    Input::InputCode MetaCode::GetInputCodeFromSDL_KEY(const RawEvent &RawEvent_)
    {
        //This Whole thing will only work with SDL Keyboard events. If we switch out event subsystems this is one of those that must change it.
        Input::InputCode To;

        assert( sizeof(To)==sizeof(RawEvent_.key.keysym.sym) );
        memcpy( &To, &(RawEvent_.key.keysym.scancode), sizeof(RawEvent_.key.keysym.scancode));

        return To;
    }

    Input::InputCode MetaCode::GetInputCodeFromSDL_MOUSE(const RawEvent &RawEvent_)
    {
        switch (RawEvent_.button.button )
        {
            case SDL_BUTTON_LEFT: return Input::MOUSEBUTTON_1;
            case SDL_BUTTON_RIGHT: return Input::MOUSEBUTTON_2;
            case SDL_BUTTON_MIDDLE: return Input::MOUSEBUTTON_3;
            case SDL_BUTTON_X1: return Input::MOUSEBUTTON_4;
            case SDL_BUTTON_X2: return Input::MOUSEBUTTON_5;
        }
    }

    Input::InputCode MetaCode::GetInputCodeFromSDL_JOYSTICK(const RawEvent &RawEvent_)
        { return GetJoystickButtonCode(RawEvent_.jbutton.button); }

    ///////////////////////////////////////////////////////////////////////////////
    // Conversion and Casting Functions
    ///////////////////////////////////////
    Input::ButtonState MetaCode::GetMetaValueAsButtonState() const
    {
        if( Input::BUTTON_LIFTING <= this->MetaValue && Input::BUTTON_DOWN >= this->MetaValue)
        {
            return (Input::ButtonState) this->MetaValue;
        }else{
            throw("Invalid ButtonState in MetaValue");
        }
    }

    Input::DirectionalMotionState MetaCode::GetMetaValueAsDirectionalMotionState() const
    {
        if( Input::DIRECTIONALMOTION_DOWNRIGHT <= this->MetaValue && Input::DIRECTIONALMOTION_UPLEFT >= this->MetaValue)
        {
            return (Input::DirectionalMotionState) this->MetaValue;
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

    Input::InputCode MetaCode::GetCode() const
    {
        return this->Code;
    }

    void MetaCode::SetMetaValue(const int &MetaValue_)
    {
        this->MetaValue=MetaValue_;
    }

    void MetaCode::SetCode(const Input::InputCode &Code_)
    {
        this->Code=Code_;
    }

    void MetaCode::SetCode(int Code_)
    {
        this->Code=(Input::InputCode)Code_;
    }

    void MetaCode::SetDeviceIndex(const UInt16& Index)
    {
        DeviceIndex = Index;
    }

    UInt16 MetaCode::GetDeviceIndex() const
    {
        return DeviceIndex;
    }

    Input::InputCode MetaCode::GetMouseButtonCode(short unsigned int ButtonNumber)
    {
        Input::InputCode Answer = (Input::InputCode)(ButtonNumber + (int)Input::MOUSEBUTTON);
        if ( Input::MOUSEBUTTON_FIRST > Answer && Input::MOUSEBUTTON_LAST < Answer)
            { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Unsupported mouse Button."); }
        return Answer;
    }

    Input::InputCode MetaCode::GetJoystickButtonCode(short unsigned int ButtonNumber)
    {
        Input::InputCode Answer = (Input::InputCode)(ButtonNumber + (int)Input::CONTROLLERBUTTON);
        if ( Input::CONTROLLERBUTTON_FIRST > Answer && Input::CONTROLLERBUTTON_LAST < Answer)
            { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Unsupported Joystick Button."); }
        return Answer;
    }

    Input::InputCode MetaCode::GetJoystickAxisCode(short unsigned int AxisNumber)
    {
        Input::InputCode Answer = (Input::InputCode)(AxisNumber + (int)Input::CONTROLLERAXIS);
        if ( Input::CONTROLLERAXIS_FIRST > Answer && Input::CONTROLLERAXIS_LAST < Answer)
            { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Unsupported Joystick AXIS."); }
        return Answer;
    }

    bool MetaCode::IsKeyboardButton() const
        { return (Input::KEY_FIRST <= this->Code && this->Code <= Input::KEY_LAST); }

    bool MetaCode::IsAltKey() const
        { return (Input::KEY_LALT == this->Code || this->Code <= Input::KEY_RALT); }

    bool MetaCode::IsCtrlKey() const
        { return (Input::KEY_LCTRL == this->Code || this->Code <= Input::KEY_RCTRL); }

    bool MetaCode::IsShiftKey() const
        { return (Input::KEY_LSHIFT == this->Code || this->Code <= Input::KEY_RSHIFT); }

    bool MetaCode::IsSuperKey() const
        { return (Input::KEY_LSUPER == this->Code || this->Code <= Input::KEY_RSUPER); }

    bool MetaCode::IsMouseButton() const
        { return (Input::MOUSEBUTTON_FIRST <= this->Code && this->Code <= Input::MOUSEBUTTON_LAST); }

    bool MetaCode::IsPollable() const
        { return ( IsKeyboardButton() || IsMouseButton() || this->Code==Input::CONTROLLERBUTTON); }

    bool MetaCode::IsJoyStickEvent() const
        { return (Input::CONTROLLER_FIRST <= this->Code && this->Code <= Input::CONTROLLER_LAST); }

    bool MetaCode::IsOtherInputEvent() const
        { return (Input::INPUTEVENT_FIRST <= this->Code && this->Code <= Input::INPUTEVENT_LAST); }

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
    // Serializable
    void MetaCode::ProtoSerialize(XML::Node& CurrentRoot) const
    {
        Mezzanine::XML::Node MetaNode = CurrentRoot.AppendChild(SerializableName());
        MetaNode.SetName(SerializableName());

        Mezzanine::XML::Attribute VersionAttr = MetaNode.AppendAttribute("Version");
        Mezzanine::XML::Attribute MetaValue_ = MetaNode.AppendAttribute("MetaValue");
        Mezzanine::XML::Attribute Code_ = MetaNode.AppendAttribute("Code");

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
    void MetaCode::ProtoDeSerialize(const XML::Node& OneNode)
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
    // end of mike's serializable
}

std::ostream& operator << (std::ostream& stream, const Mezzanine::MetaCode& x)
{
    //stream << "<MetaCode Version=\"1\" MetaValue=\"" << x.GetMetaValue() << "\"" << " Code=\"" << x.GetCode() <<  "\" />";
    Serialize(stream,x);
    return stream;
}

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::MetaCode& x)
    { return DeSerialize(stream, x); }

Mezzanine::XML::Node& operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::MetaCode& x)
    {x.ProtoDeSerialize(OneNode); }

#endif
