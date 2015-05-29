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
#ifndef _inputmetacode_cpp
#define _inputmetacode_cpp

#include "entresol.h" //only used for logging

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////

#include "exception.h"
#include "Input/metacode.h"
#include "stringtool.h"
#include "serialization.h"
#include "unicode.h"

#include <assert.h>
#include <limits>
#include <ostream>
#include <cstring>
#include <memory>

#include "SDL.h"

using namespace std;

namespace Mezzanine
{
    namespace Input
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Creation and Deletion Methods

        MetaCode::MetaCode() :
            MetaValue(0),
            DeviceIndex(std::numeric_limits<Int32>::max()),
            Code(Input::KEY_UNKNOWN)
            {  }

        MetaCode::MetaCode(const MetaCode& Other) :
            MetaValue(Other.MetaValue),
            DeviceIndex(Other.DeviceIndex),
            Code(Other.Code)
            {  }

        MetaCode::MetaCode(const Int32 Value, const Input::InputCode NewCode) :
            MetaValue(Value),
            DeviceIndex(std::numeric_limits<Int32>::max()),
            Code(NewCode)
            {  }

        MetaCode::MetaCode(const Int32 Value, const Input::InputCode NewCode, const Int32 Index) :
            MetaValue(Value),
            DeviceIndex(Index),
            Code(NewCode)
            {  }

        MetaCode::~MetaCode()
            {  }

        MetaCode::MetaCodeContainer MetaCode::CreateMetaCodes(const RawEvent& Raw)
        {
            MetaCodeContainer Ret;
            switch(Raw.type)
            {
                case SDL_KEYDOWN:
                {
                    Ret.push_back( MetaCode(Input::BUTTON_PRESSING,GetInputCodeFromSDL_KEY(Raw)) );
                    break;
                }
                case SDL_KEYUP:
                {
                    Ret.push_back( MetaCode(Input::BUTTON_LIFTING,GetInputCodeFromSDL_KEY(Raw)) );
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    Ret.push_back( MetaCode(Input::BUTTON_PRESSING,GetInputCodeFromSDL_MOUSE(Raw)) );
                    break;
                }
                case SDL_MOUSEBUTTONUP:
                {
                    Ret.push_back( MetaCode(Input::BUTTON_LIFTING,GetInputCodeFromSDL_MOUSE(Raw)) );
                    break;
                }
                case SDL_JOYBUTTONDOWN:
                {
                    Ret.push_back( MetaCode(Input::BUTTON_PRESSING,GetInputCodeFromSDL_JOYSTICK(Raw),Raw.jbutton.which) );
                    break;
                }
                case SDL_JOYBUTTONUP:
                {
                    Ret.push_back( MetaCode(Input::BUTTON_LIFTING,GetInputCodeFromSDL_JOYSTICK(Raw),Raw.jbutton.which) );
                    break;
                }
                case SDL_MOUSEMOTION:
                {
                    Ret.push_back( MetaCode(Raw.motion.x,Input::MOUSEABSOLUTEHORIZONTAL) );
                    Ret.push_back( MetaCode(Raw.motion.y,Input::MOUSEABSOLUTEVERTICAL) );

                    if( Raw.motion.xrel != 0 ) {
                        Ret.push_back( MetaCode(Raw.motion.xrel,Input::MOUSEHORIZONTAL) );
                    }
                    if( Raw.motion.yrel != 0 ) {
                        Ret.push_back( MetaCode(Raw.motion.yrel,Input::MOUSEVERTICAL) );
                    }
                    break;
                }
                case SDL_JOYBALLMOTION:
                {
                    if( 0 == Raw.jball.ball ) {
                        if( Raw.jball.yrel != 0 ) {
                            Ret.push_back( MetaCode(Raw.jball.yrel,Input::CONTROLLERBALL_1_VERTICAL,Raw.jball.which) );
                        }
                        if( Raw.jball.xrel != 0 ) {
                            Ret.push_back( MetaCode(Raw.jball.xrel,Input::CONTROLLERBALL_1_HORIZONTAL,Raw.jball.which) );
                        }
                    }else if( 1 == Raw.jball.ball ) {
                        if( Raw.jball.yrel != 0 ) {
                            Ret.push_back( MetaCode(Raw.jball.yrel,Input::CONTROLLERBALL_2_VERTICAL,Raw.jball.which) );
                        }
                        if( Raw.jball.xrel != 0 ) {
                            Ret.push_back( MetaCode(Raw.jball.xrel,Input::CONTROLLERBALL_2_HORIZONTAL,Raw.jball.which) );
                        }
                    }else{
                        MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"More then 2 trackballs is currently not supported.  Perhaps we should expand our enum.");
                    }
                    break;
                }
                case SDL_JOYHATMOTION:
                {
                    Ret.push_back( MetaCode(Raw.jhat.value,MetaCode::GetControllerHatCode(Raw.jhat.hat + 1),Raw.jhat.which) );
                    break;
                }
                case SDL_JOYAXISMOTION:
                {
                    Ret.push_back( MetaCode(Raw.jaxis.value,MetaCode::GetControllerAxisCode(Raw.jaxis.axis + 1),Raw.jaxis.which) );
                    break;
                }
                case SDL_TEXTINPUT:
                {
                    Integer Position = 0;
                    Integer BytesAdvance = 0;
                    while( Raw.text.text[Position] != '\0' )
                    {
                        Int32 GlyphID = Unicode::GetIntFromCharacter(BytesAdvance,&(Raw.text.text[Position]));
                        if( GlyphID < 0 ) {
                            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Non-UTF8 encoded text generated from TextInput event.");
                        }
                        Ret.push_back( MetaCode(GlyphID,Input::OSTEXTINPUT) );
                        Position += BytesAdvance;
                    }
                    break;
                }
                default:
                {
                    // Create a safe but gibberish default
                    Ret.push_back( MetaCode(Input::BUTTON_UP,Input::KEY_FIRST) );
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Unknown User Input Inserted into Metacode");
                    break;
                }
            }
            return Ret;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Dirty Hacks

        //This function assumes the RawEvent is a valid SDL Keyevent
        Input::InputCode MetaCode::GetInputCodeFromSDL_KEY(const RawEvent& Raw)
        {
            //This Whole thing will only work with SDL Keyboard events. If we switch out event subsystems this is one of those that must change it.
            Input::InputCode To;

            assert( sizeof(To)==sizeof(Raw.key.keysym.sym) );
            memcpy( &To, &(Raw.key.keysym.scancode), sizeof(Raw.key.keysym.scancode));

            return To;
        }

        Input::InputCode MetaCode::GetInputCodeFromSDL_MOUSE(const RawEvent& Raw)
        {
            switch (Raw.button.button )
            {
                case SDL_BUTTON_LEFT: return Input::MOUSEBUTTON_1;
                case SDL_BUTTON_RIGHT: return Input::MOUSEBUTTON_2;
                case SDL_BUTTON_MIDDLE: return Input::MOUSEBUTTON_3;
                case SDL_BUTTON_X1: return Input::MOUSEBUTTON_4;
                case SDL_BUTTON_X2: return Input::MOUSEBUTTON_5;
            }
            return Input::KEY_UNKNOWN;
        }

        Input::InputCode MetaCode::GetInputCodeFromSDL_JOYSTICK(const RawEvent& Raw)
            { return GetControllerButtonCode(Raw.jbutton.button); }

        ///////////////////////////////////////////////////////////////////////////////
        // Gets and Sets

        void MetaCode::SetCode(const Input::InputCode NewCode)
            { this->Code = NewCode; }

        void MetaCode::SetCode(const Int32 NewCode)
            { this->Code = (Input::InputCode)NewCode; }

        Input::InputCode MetaCode::GetCode() const
            { return this->Code; }

        void MetaCode::SetMetaValue(const Int32 Value)
            { this->MetaValue = Value; }

        Int32 MetaCode::GetMetaValue() const
            { return this->MetaValue; }

        void MetaCode::SetDeviceIndex(const Int32 Index)
            { this->DeviceIndex = Index; }

        Int32 MetaCode::GetDeviceIndex() const
            { return DeviceIndex; }

        void MetaCode::SetNullValues()
        {
            this->Code = Input::KEY_UNKNOWN;
            this->MetaValue = 0;
            this->DeviceIndex = numeric_limits<Int32>::max();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Conversion and Casting Functions

        Input::ButtonState MetaCode::GetMetaValueAsButtonState() const
        {
            if( Input::BUTTON_LIFTING <= this->MetaValue && Input::BUTTON_DOWN >= this->MetaValue) {
                return (Input::ButtonState) this->MetaValue;
            }else{
                MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Invalid ButtonState in MetaValue");
            }
        }

        Input::DirectionalMotionState MetaCode::GetMetaValueAsDirectionalMotionState() const
        {
            if( Input::DIRECTIONALMOTION_DOWNRIGHT <= this->MetaValue && Input::DIRECTIONALMOTION_UPLEFT >= this->MetaValue) {
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
                { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Unsupported Controller Axis."); }
            return Answer;
        }

        Input::InputCode MetaCode::GetControllerHatCode(const UInt16 HatNumber)
        {
            Input::InputCode Answer = (Input::InputCode)(HatNumber + (UInt16)Input::CONTROLLERHAT);
            if ( Input::CONTROLLERHAT_FIRST > Answer && Input::CONTROLLERHAT_LAST < Answer)
                { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Unsupported Controller Hat."); }
            return Answer;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Checks

        Boole MetaCode::IsKeyboardButton() const
            { return (Input::KEY_FIRST <= this->Code && Input::KEY_LAST >= this->Code); }

        Boole MetaCode::IsMouseButton() const
            { return (Input::MOUSEBUTTON_FIRST <= this->Code && Input::MOUSEBUTTON_LAST >= this->Code); }

        Boole MetaCode::IsControllerButton() const
            { return (Input::CONTROLLERBUTTON_FIRST <= this->Code && Input::CONTROLLERBUTTON_LAST >= this->Code); }

        Boole MetaCode::IsDeviceButton() const
            { return (this->IsKeyboardButton() || this->IsMouseButton() || this->IsControllerButton()); }

        Boole MetaCode::IsKeyboardEvent() const
            { return this->IsKeyboardButton(); }

        Boole MetaCode::IsMouseEvent() const
            { return (Input::MOUSE_FIRST <= this->Code && Input::MOUSE_LAST >= this->Code) || this->IsMouseMultiClickEvent(); }

        Boole MetaCode::IsMouseMotionEvent() const
            { return (Input::MOUSEMOTION_FIRST <= this->Code && Input::MOUSEMOTION_LAST && this->Code); }

        Boole MetaCode::IsMouseMultiClickEvent() const
            { return (Input::COMPOUNDINPUT_MOUSEMULTICLICKFIRST <= this->Code && Input::COMPOUNDINPUT_MOUSEMULTICLICKLAST >= this->Code); }

        Boole MetaCode::IsMultitouchEvent() const
            { return (Input::MULTITOUCH_FIRST <= this->Code && Input::MULTITOUCH_LAST >= this->Code); }

        Boole MetaCode::IsControllerEvent() const
            { return (Input::CONTROLLER_FIRST <= this->Code && Input::CONTROLLER_LAST >= this->Code); }

        Boole MetaCode::IsControllerAxisEvent() const
            { return (Input::CONTROLLERAXIS_FIRST <= this->Code && Input::CONTROLLERAXIS_LAST >= this->Code); }

        Boole MetaCode::IsControllerHatEvent() const
            { return (Input::CONTROLLERHAT_FIRST <= this->Code && Input::CONTROLLERHAT_LAST >= this->Code); }

        Boole MetaCode::IsInputEvent() const
            { return (Input::INPUTEVENT_FIRST <= this->Code && Input::INPUTEVENT_LAST >= this->Code); }

        Boole MetaCode::IsAltKey() const
            { return (Input::KEY_LALT == this->Code || Input::KEY_RALT == this->Code); }

        Boole MetaCode::IsCtrlKey() const
            { return (Input::KEY_LCTRL == this->Code || Input::KEY_RCTRL == this->Code); }

        Boole MetaCode::IsShiftKey() const
            { return (Input::KEY_LSHIFT == this->Code || Input::KEY_RSHIFT == this->Code); }

        Boole MetaCode::IsSuperKey() const
            { return (Input::KEY_LSUPER == this->Code || Input::KEY_RSUPER == this->Code); }

        Boole MetaCode::IsPollable() const
            { return ( this->IsDeviceButton() ); }

        Input::InputDevice MetaCode::GetDeviceType() const
        {
            if( this->IsKeyboardEvent() ) return Input::DEVICE_KEYBOARD;
            else if( this->IsMouseEvent() ) return Input::DEVICE_MOUSE;
            else if( this->IsMultitouchEvent() ) return Input::DEVICE_MULTITOUCH;
            else if( this->IsControllerEvent() ) return Input::DEVICE_CONTROLLER;
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

        Boole MetaCode::operator==(const MetaCode& Other) const
        {
            return (this->Code == Other.Code &&
                    this->MetaValue == Other.MetaValue &&
                    this->DeviceIndex == Other.DeviceIndex);
        }

        Boole MetaCode::operator!=(const MetaCode& Other) const
        {
            return (this->Code != Other.Code ||
                    this->MetaValue != Other.MetaValue ||
                    this->DeviceIndex != Other.DeviceIndex);
        }

        Boole MetaCode::operator<(const MetaCode& Other) const
            { return (this->Code < Other.Code); }

        Boole MetaCode::operator>(const MetaCode& Other) const
            { return (this->Code > Other.Code); }

        void MetaCode::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node SelfNode = CurrentRoot.AppendChild( MetaCode::GetSerializableName() );

            if( SelfNode.AppendAttribute("Version").SetValue("1") &&
                SelfNode.AppendAttribute("MetaValue").SetValue( this->MetaValue ) &&
                SelfNode.AppendAttribute("DeviceIndex").SetValue( this->DeviceIndex ) &&
                SelfNode.AppendAttribute("Code").SetValue( this->Code ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",MetaCode::GetSerializableName(),true);
            }
        }

        void MetaCode::ProtoDeSerialize(const XML::Node& OneNode)
        {
            XML::Attribute CurrAttrib;

            if( String(OneNode.Name()) == MetaCode::GetSerializableName() ) {
                if(OneNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = OneNode.GetAttribute("MetaValue");
                    if( !CurrAttrib.Empty() )
                        this->MetaValue = CurrAttrib.AsInt();

                    CurrAttrib = OneNode.GetAttribute("DeviceIndex");
                    if( !CurrAttrib.Empty() )
                        this->DeviceIndex = CurrAttrib.AsInt();

                    CurrAttrib = OneNode.GetAttribute("Code");
                    if( !CurrAttrib.Empty() )
                        this->Code = static_cast<Input::InputCode>( CurrAttrib.AsUint() );
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + MetaCode::GetSerializableName() + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,MetaCode::GetSerializableName() + " was not found in the provided XML node, which was expected.");
            }
        }

        String MetaCode::GetSerializableName() const
            { return "MetaCode"; }
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
