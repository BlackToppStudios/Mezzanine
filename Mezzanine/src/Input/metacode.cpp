// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#include <limits>
#include <ostream>
#include <cstring>
#include <memory>

#include "SDL.h"
#include "serialization.h"

using namespace std;

namespace Mezzanine
{
    namespace Input
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Creation and Deletion Methods

        MetaCode::MetaCode() : MetaValue(0), DeviceIndex( numeric_limits<UInt16>::max() ), Code(Input::KEY_UNKNOWN)
        {}

        MetaCode::MetaCode(const MetaCode& Other) : MetaValue(Other.MetaValue), DeviceIndex(Other.DeviceIndex), Code(Other.Code)
        {}

        MetaCode::MetaCode(const Int32& Value, const Input::InputCode& NewCode) : DeviceIndex( numeric_limits<UInt16>::max() )
        {
            this->Construct(Value, NewCode);
        }

        MetaCode::MetaCode(const Int32& Value, const Input::InputCode& NewCode, const UInt16& Index)
        {
            this->Construct(Value, NewCode);
            this->SetDeviceIndex(Index);
        }

        MetaCode::MetaCode(const RawEvent& RawEvent_)
        {
            this->Construct(RawEvent_);
        }

        void MetaCode::Construct(const RawEvent& RawEvent_)
        {
            switch(RawEvent_.type)
            {
                case SDL_KEYDOWN:
                    this->Construct(Input::BUTTON_PRESSING, GetInputCodeFromSDL_KEY(RawEvent_));
                    break;
                case SDL_KEYUP:
                    this->Construct(Input::BUTTON_LIFTING, GetInputCodeFromSDL_KEY(RawEvent_));
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    this->Construct(Input::BUTTON_PRESSING, GetInputCodeFromSDL_MOUSE(RawEvent_));
                    break;
                case SDL_MOUSEBUTTONUP:
                    this->Construct(Input::BUTTON_LIFTING, GetInputCodeFromSDL_MOUSE(RawEvent_));
                    break;
                case SDL_JOYBUTTONDOWN:
                    this->Construct(Input::BUTTON_PRESSING, GetInputCodeFromSDL_JOYSTICK(RawEvent_));
                    break;
                case SDL_JOYBUTTONUP:
                    this->Construct(Input::BUTTON_LIFTING, GetInputCodeFromSDL_JOYSTICK(RawEvent_));
                    break;

                // Fail when incorrectly constructed
                case SDL_JOYBALLMOTION:     case SDL_JOYHATMOTION:
                case SDL_JOYAXISMOTION:     case SDL_MOUSEMOTION:
                    this->Construct(Input::BUTTON_UP, Input::KEY_FIRST);            // ©reate a safe but gibberish default
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"RawEvent which creates Multiple Metacodes inserted into Metacode");
                    break;

                default:
                    this->Construct(Input::BUTTON_UP, Input::KEY_FIRST);            // ©reate a safe but gibberish default
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Unknown User Input Inserted into Metacode");
                    break;
            }
        }

        void MetaCode::Construct(const Int32& Value, const Input::InputCode& NewCode)
        {
            this->SetMetaValue(Value);
            this->SetCode(NewCode);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Dirty Hacks

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
            { return GetControllerButtonCode(RawEvent_.jbutton.button); }

        ///////////////////////////////////////////////////////////////////////////////
        // Gets and Sets

        void MetaCode::SetCode(const Input::InputCode& NewCode)
            { this->Code = NewCode; }

        void MetaCode::SetCode(const Int32& NewCode)
            { this->Code = (Input::InputCode)NewCode; }

        Input::InputCode MetaCode::GetCode() const
            { return this->Code; }

        void MetaCode::SetMetaValue(const Int32& Value)
            { this->MetaValue = Value; }

        Int32 MetaCode::GetMetaValue() const
            { return this->MetaValue; }

        void MetaCode::SetDeviceIndex(const UInt16& Index)
            { this->DeviceIndex = Index; }

        UInt16 MetaCode::GetDeviceIndex() const
            { return DeviceIndex; }

        void MetaCode::SetNullValues()
        {
            this->Code = Input::KEY_UNKNOWN;
            this->MetaValue = 0;
            this->DeviceIndex = numeric_limits<UInt32>::max();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Conversion and Casting Functions
        Input::ButtonState MetaCode::GetMetaValueAsButtonState() const
        {
            if( Input::BUTTON_LIFTING <= this->MetaValue && Input::BUTTON_DOWN >= this->MetaValue)
            {
                return (Input::ButtonState) this->MetaValue;
            }else{
                MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Invalid ButtonState in MetaValue");
            }
        }

        Input::DirectionalMotionState MetaCode::GetMetaValueAsDirectionalMotionState() const
        {
            if( Input::DIRECTIONALMOTION_DOWNRIGHT <= this->MetaValue && Input::DIRECTIONALMOTION_UPLEFT >= this->MetaValue)
            {
                return (Input::DirectionalMotionState) this->MetaValue;
            }else{
                MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Invalid DirectionalMotionState in MetaValue");
            }
        }

        Input::InputCode MetaCode::GetMouseButtonCode(const UInt16 ButtonNumber)
        {
            Input::InputCode Answer = (Input::InputCode)(ButtonNumber + (UInt16)Input::MOUSEBUTTON);
            if ( Input::MOUSEBUTTON_FIRST > Answer && Input::MOUSEBUTTON_LAST < Answer)
                { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Unsupported mouse Button."); }
            return Answer;
        }

        Input::InputCode MetaCode::GetControllerButtonCode(const UInt16 ButtonNumber)
        {
            Input::InputCode Answer = (Input::InputCode)(ButtonNumber + (UInt16)Input::CONTROLLERBUTTON);
            if ( Input::CONTROLLERBUTTON_FIRST > Answer && Input::CONTROLLERBUTTON_LAST < Answer)
                { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Unsupported Controller Button."); }
            return Answer;
        }

        Input::InputCode MetaCode::GetControllerAxisCode(const UInt16 AxisNumber)
        {
            Input::InputCode Answer = (Input::InputCode)(AxisNumber + (UInt16)Input::CONTROLLERAXIS);
            if ( Input::CONTROLLERAXIS_FIRST > Answer && Input::CONTROLLERAXIS_LAST < Answer)
                { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Unsupported Controller AXIS."); }
            return Answer;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Checks

        bool MetaCode::IsKeyboardButton() const
            { return (Input::KEY_FIRST <= this->Code && Input::KEY_LAST >= this->Code); }

        bool MetaCode::IsMouseButton() const
            { return (Input::MOUSEBUTTON_FIRST <= this->Code && Input::MOUSEBUTTON_LAST >= this->Code); }

        bool MetaCode::IsControllerButton() const
            { return (Input::CONTROLLERBUTTON_FIRST <= this->Code && Input::CONTROLLERBUTTON_LAST >= this->Code); }

        bool MetaCode::IsDeviceButton() const
            { return (this->IsKeyboardButton() || this->IsMouseButton() || this->IsControllerButton()); }

        bool MetaCode::IsKeyboardEvent() const
            { return IsKeyboardButton(); }

        bool MetaCode::IsMouseEvent() const
            { return (Input::MOUSE_FIRST <= this->Code && Input::MOUSE_LAST >= this->Code) || IsMouseMultiClickEvent(); }

        bool MetaCode::IsMouseMotionEvent() const
            { return (Input::MOUSEMOTION_FIRST <= this->Code && Input::MOUSEMOTION_LAST && this->Code); }

        bool MetaCode::IsMouseMultiClickEvent() const
            { return (Input::COMPOUNDINPUT_MOUSEMULTICLICKFIRST <= this->Code && Input::COMPOUNDINPUT_MOUSEMULTICLICKLAST >= this->Code); }

        bool MetaCode::IsMultitouchEvent() const
            { return (Input::MULTITOUCH_FIRST <= this->Code && Input::MULTITOUCH_LAST >= this->Code); }

        bool MetaCode::IsControllerEvent() const
            { return (Input::CONTROLLER_FIRST <= this->Code && Input::CONTROLLER_LAST >= this->Code); }

        bool MetaCode::IsControllerAxisEvent() const
            { return (Input::CONTROLLERAXIS_FIRST <= this->Code && Input::CONTROLLERAXIS_LAST >= this->Code); }

        bool MetaCode::IsControllerHatEvent() const
            { return (Input::CONTROLLERHAT_FIRST <= this->Code && Input::CONTROLLERHAT_LAST >= this->Code); }

        bool MetaCode::IsInputEvent() const
            { return (Input::INPUTEVENT_FIRST <= this->Code && Input::INPUTEVENT_LAST >= this->Code); }

        bool MetaCode::IsAltKey() const
            { return (Input::KEY_LALT == this->Code || Input::KEY_RALT == this->Code); }

        bool MetaCode::IsCtrlKey() const
            { return (Input::KEY_LCTRL == this->Code || Input::KEY_RCTRL == this->Code); }

        bool MetaCode::IsShiftKey() const
            { return (Input::KEY_LSHIFT == this->Code || Input::KEY_RSHIFT == this->Code); }

        bool MetaCode::IsSuperKey() const
            { return (Input::KEY_LSUPER == this->Code || Input::KEY_RSUPER == this->Code); }

        bool MetaCode::IsPollable() const
            { return ( IsDeviceButton() ); }

        Input::InputDevice MetaCode::GetDeviceType() const
        {
            if(IsKeyboardEvent()) return Input::DEVICE_KEYBOARD;
            else if(IsMouseEvent()) return Input::DEVICE_MOUSE;
            else if(IsMultitouchEvent()) return Input::DEVICE_MULTITOUCH;
            else if(IsControllerEvent()) return Input::DEVICE_CONTROLLER;
            else return Input::DEVICE_UNKNOWN;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        MetaCode& MetaCode::operator=(const MetaCode& Other)
        {
            this->Code = Other.Code;
            this->MetaValue = Other.MetaValue;
            this->DeviceIndex = Other.DeviceIndex;
            return *this;
        }

        bool MetaCode::operator==(const MetaCode& Other) const
        {
            return (this->Code == Other.Code &&
                    this->MetaValue == Other.MetaValue &&
                    this->DeviceIndex == Other.DeviceIndex);
        }

        bool MetaCode::operator!=(const MetaCode& Other) const
        {
            return (this->Code != Other.Code ||
                    this->MetaValue != Other.MetaValue ||
                    this->DeviceIndex != Other.DeviceIndex);
        }

        bool MetaCode::operator<(const MetaCode& Other) const
        {
            return (this->Code < Other.Code);
        }

        bool MetaCode::operator>(const MetaCode& Other) const
        {
            return (this->Code > Other.Code);
        }

        // Mike's portion of adding serializable to the metacode starts here
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
    }//Input
}//Mezzanine

std::ostream& operator << (std::ostream& stream, const Mezzanine::Input::MetaCode& x)
{
    //stream << "<MetaCode Version=\"1\" MetaValue=\"" << x.GetMetaValue() << "\"" << " Code=\"" << x.GetCode() <<  "\" />";
    Mezzanine::Serialize(stream,x);
    return stream;
}

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Input::MetaCode& x)
    { return Mezzanine::DeSerialize(stream, x); }

Mezzanine::XML::Node& operator >> (Mezzanine::XML::Node& OneNode, Mezzanine::Input::MetaCode& x)
{
    x.ProtoDeSerialize(OneNode);
    return OneNode;
}

#endif
