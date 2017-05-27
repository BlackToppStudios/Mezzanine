// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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

namespace
{
    using namespace Mezzanine;

    /// @brief Converts an internal keyboard input event into a Mezzanine keyboard InputCode.
    /// @param Raw The internal event to be converted into a keyboard InputCode.
    /// @return Returns the converted Mezzanine InputCode.
    Input::InputCode GetInputCodeFromSDL_KEY(const RawEvent& Raw)
    {
        //This Whole thing will only work with SDL Keyboard events. If we switch out event subsystems this is one of those that must change it.
        Input::InputCode To;
        assert( sizeof(To)==sizeof(Raw.key.keysym.sym) );
        memcpy( &To, &(Raw.key.keysym.scancode), sizeof(Raw.key.keysym.scancode));
        return To;
    }
    /// @brief Converts an internal mouse input event into a Mezzanine mouse InputCode.
    /// @param Raw The internal event to be converted into a mouse InputCode.
    /// @return Returns the converted Mezzanine InputCode.
    Input::InputCode GetInputCodeFromSDL_MOUSE(const RawEvent& Raw)
    {
        switch( Raw.button.button )
        {
            case SDL_BUTTON_LEFT:   return Input::MOUSEBUTTON_1;
            case SDL_BUTTON_RIGHT:  return Input::MOUSEBUTTON_2;
            case SDL_BUTTON_MIDDLE: return Input::MOUSEBUTTON_3;
            case SDL_BUTTON_X1:     return Input::MOUSEBUTTON_4;
            case SDL_BUTTON_X2:     return Input::MOUSEBUTTON_5;
        }
        return Input::KEY_UNKNOWN;
    }
    /// @brief Converts an internal controller input event into a Mezzanine controller InputCode.
    /// @param Raw The internal event to be converted into a controller InputCode.
    /// @return Returns the converted Mezzanine InputCode.
    Input::InputCode GetInputCodeFromSDL_JOYSTICK(const RawEvent& Raw)
        { return Input::MetaCode::GetControllerButtonCode(Raw.jbutton.button); }
}

namespace Mezzanine
{
    namespace Input
    {
        const MetaCode MetaCode::NullCode = MetaCode();

        ///////////////////////////////////////////////////////////////////////////////
        // Creation and Deletion Methods

        MetaCode::MetaCode() :
            MetaValue(0),
            DeviceID(std::numeric_limits<Int32>::max()),
            Code(Input::KEY_UNKNOWN)
            {  }

        MetaCode::MetaCode(const Int32 Value, const Input::InputCode NewCode) :
            MetaValue(Value),
            DeviceID(std::numeric_limits<Int32>::max()),
            Code(NewCode)
            {  }

        MetaCode::MetaCode(const Int32 Value, const Input::InputCode NewCode, const DeviceIDType ID) :
            MetaValue(Value),
            DeviceID(ID),
            Code(NewCode)
            {  }

        MetaCode::MetaCodeContainer MetaCode::CreateMetaCodes(const RawEvent& Raw)
        {
            MetaCodeContainer Ret;
            switch(Raw.type)
            {
                case SDL_KEYDOWN:
                {
                    Ret.emplace_back( Input::BUTTON_PRESSING,GetInputCodeFromSDL_KEY(Raw) );
                    break;
                }
                case SDL_KEYUP:
                {
                    Ret.emplace_back( Input::BUTTON_LIFTING,GetInputCodeFromSDL_KEY(Raw) );
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
                            MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Non-UTF8 encoded text generated from TextInput event.");
                        }
                        Ret.emplace_back( GlyphID,Input::OSTEXTINPUT );
                        Position += BytesAdvance;
                    }
                    break;
                }
                case SDL_MOUSEMOTION:
                {
                    Ret.emplace_back( Raw.motion.x,Input::MOUSEABSOLUTEHORIZONTAL );
                    Ret.emplace_back( Raw.motion.y,Input::MOUSEABSOLUTEVERTICAL );

                    if( Raw.motion.xrel != 0 ) {
                        Ret.emplace_back( Raw.motion.xrel,Input::MOUSEHORIZONTAL );
                    }
                    if( Raw.motion.yrel != 0 ) {
                        Ret.emplace_back( Raw.motion.yrel,Input::MOUSEVERTICAL );
                    }
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    Ret.emplace_back( Input::BUTTON_PRESSING,GetInputCodeFromSDL_MOUSE(Raw) );
                    break;
                }
                case SDL_MOUSEBUTTONUP:
                {
                    Ret.emplace_back( Input::BUTTON_LIFTING,GetInputCodeFromSDL_MOUSE(Raw) );
                    break;
                }
                case SDL_MOUSEWHEEL:
                {
                    Ret.emplace_back( Raw.wheel.x,Input::MOUSEWHEELHORIZONTAL );
                    Ret.emplace_back( Raw.wheel.y,Input::MOUSEWHEELVERTICAL );
                    break;
                }
                case SDL_JOYAXISMOTION:
                {
                    Ret.emplace_back( Raw.jaxis.value,MetaCode::GetControllerAxisCode(Raw.jaxis.axis + 1),Raw.jaxis.which );
                    break;
                }
                case SDL_JOYBALLMOTION:
                {
                    if( 0 == Raw.jball.ball ) {
                        if( Raw.jball.yrel != 0 ) {
                            Ret.emplace_back( Raw.jball.yrel,Input::JOYSTICKBALL_1_VERTICAL,Raw.jball.which );
                        }
                        if( Raw.jball.xrel != 0 ) {
                            Ret.emplace_back( Raw.jball.xrel,Input::JOYSTICKBALL_1_HORIZONTAL,Raw.jball.which );
                        }
                    }else if( 1 == Raw.jball.ball ) {
                        if( Raw.jball.yrel != 0 ) {
                            Ret.emplace_back( Raw.jball.yrel,Input::JOYSTICKBALL_2_VERTICAL,Raw.jball.which );
                        }
                        if( Raw.jball.xrel != 0 ) {
                            Ret.emplace_back( Raw.jball.xrel,Input::JOYSTICKBALL_2_HORIZONTAL,Raw.jball.which );
                        }
                    }else{
                        MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"More then 2 trackballs is currently not supported.  Perhaps we should expand our enum.");
                    }
                    break;
                }
                case SDL_JOYHATMOTION:
                {
                    Ret.emplace_back( Raw.jhat.value,MetaCode::GetJoystickHatCode(Raw.jhat.hat + 1),Raw.jhat.which );
                    break;
                }
                case SDL_JOYBUTTONDOWN:
                {
                    Ret.emplace_back( Input::BUTTON_PRESSING,GetInputCodeFromSDL_JOYSTICK(Raw),Raw.jbutton.which );
                    break;
                }
                case SDL_JOYBUTTONUP:
                {
                    Ret.emplace_back( Input::BUTTON_LIFTING,GetInputCodeFromSDL_JOYSTICK(Raw),Raw.jbutton.which );
                    break;
                }
                case SDL_CONTROLLERAXISMOTION:
                {
                    Ret.emplace_back( Raw.caxis.value,MetaCode::GetControllerAxisCode(Raw.caxis.axis + 1),Raw.caxis.which );
                    break;
                }
                case SDL_CONTROLLERBUTTONDOWN:
                {
                    Ret.emplace_back( Input::BUTTON_PRESSING,GetInputCodeFromSDL_JOYSTICK(Raw),Raw.cbutton.which );
                    break;
                }
                case SDL_CONTROLLERBUTTONUP:
                {
                    Ret.emplace_back( Input::BUTTON_LIFTING,GetInputCodeFromSDL_JOYSTICK(Raw),Raw.cbutton.which );
                    break;
                }
                default:
                    { break; }
            }
            return Ret;
        }

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

        void MetaCode::SetDeviceID(const DeviceIDType ID)
            { this->DeviceID = ID; }

        DeviceIDType MetaCode::GetDeviceID() const
            { return this->DeviceID; }

        void MetaCode::SetNullValues()
        {
            this->Code = Input::KEY_UNKNOWN;
            this->MetaValue = 0;
            this->DeviceID = std::numeric_limits<Int32>::max();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Conversion and Casting Functions

        Input::ButtonState MetaCode::GetMetaValueAsButtonState() const
        {
            if( Input::BUTTON_LIFTING <= this->MetaValue && Input::BUTTON_DOWN >= this->MetaValue) {
                return static_cast<Input::ButtonState>( this->MetaValue );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Invalid ButtonState in MetaValue");
            }
        }

        Input::DirectionalMotionState MetaCode::GetMetaValueAsDirectionalMotionState() const
        {
            if( Input::DIRECTIONALMOTION_DOWNRIGHT <= this->MetaValue && Input::DIRECTIONALMOTION_UPLEFT >= this->MetaValue) {
                return static_cast<Input::DirectionalMotionState>( this->MetaValue );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Invalid DirectionalMotionState in MetaValue");
            }
        }

        Input::InputCode MetaCode::GetMouseButtonCode(const UInt16 ButtonNumber)
        {
            Input::InputCode Answer = static_cast<Input::InputCode>(ButtonNumber + Input::MOUSEBUTTON_FIRST);
            if( Input::MOUSEBUTTON_FIRST > Answer && Input::MOUSEBUTTON_LAST < Answer )
                { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Unsupported mouse Button."); }
            return Answer;
        }

        Input::InputCode MetaCode::GetJoystickButtonCode(const UInt16 ButtonNumber)
        {
            Input::InputCode Answer = static_cast<Input::InputCode>(ButtonNumber + Input::JOYSTICKBUTTON_FIRST);
            if( Input::JOYSTICKBUTTON_FIRST > Answer || Input::JOYSTICKBUTTON_LAST < Answer )
                { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Unsupported Joystick Button."); }
            return Answer;
        }

        Input::InputCode MetaCode::GetJoystickAxisCode(const UInt16 AxisNumber)
        {
            Input::InputCode Answer = static_cast<Input::InputCode>(AxisNumber + Input::JOYSTICKAXIS_FIRST);
            if( Input::JOYSTICKAXIS_FIRST > Answer || Input::JOYSTICKAXIS_LAST < Answer )
                { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Unsupported Joystick Axis."); }
            return Answer;
        }

        Input::InputCode MetaCode::GetJoystickHatCode(const UInt16 HatNumber)
        {
            Input::InputCode Answer = static_cast<Input::InputCode>(HatNumber + Input::JOYSTICKHAT_FIRST);
            if( Input::JOYSTICKHAT_FIRST > Answer || Input::JOYSTICKHAT_LAST < Answer )
                { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Unsupported Joystick Hat."); }
            return Answer;
        }

        Input::InputCode MetaCode::GetControllerButtonCode(const UInt16 ButtonNumber)
        {
            Input::InputCode Answer = static_cast<Input::InputCode>(ButtonNumber + Input::CONTROLLERBUTTON_FIRST);
            if( Input::CONTROLLERBUTTON_FIRST > Answer || Input::CONTROLLERBUTTON_LAST < Answer )
                { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Unsupported Controller Button."); }
            return Answer;
        }

        Input::InputCode MetaCode::GetControllerAxisCode(const UInt16 AxisNumber)
        {
            Input::InputCode Answer = static_cast<Input::InputCode>(AxisNumber + Input::CONTROLLERAXIS_FIRST);
            if( Input::CONTROLLERAXIS_FIRST > Answer || Input::CONTROLLERAXIS_LAST < Answer )
                { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Unsupported Controller Axis."); }
            return Answer;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Checks

        Boole MetaCode::IsKeyboardEvent() const
            { return this->IsKeyboardButton(); }

        Boole MetaCode::IsKeyboardButton() const
            { return (Input::KEY_FIRST <= this->Code && Input::KEY_LAST >= this->Code); }

        Boole MetaCode::IsMouseEvent() const
            { return (Input::MOUSE_FIRST <= this->Code && Input::MOUSE_LAST >= this->Code) || this->IsMouseMultiClick(); }

        Boole MetaCode::IsMouseButton() const
            { return (Input::MOUSEBUTTON_FIRST <= this->Code && Input::MOUSEBUTTON_LAST >= this->Code); }

        Boole MetaCode::IsMouseMotion() const
            { return (Input::MOUSEMOTION_FIRST <= this->Code && Input::MOUSEMOTION_LAST && this->Code); }

        Boole MetaCode::IsMouseMultiClick() const
            { return (Input::COMPOUNDINPUT_MOUSEMULTICLICK_FIRST <= this->Code && Input::COMPOUNDINPUT_MOUSEMULTICLICK_LAST >= this->Code); }

        Boole MetaCode::IsJoystickEvent() const
            { return (Input::JOYSTICK_FIRST <= this->Code && Input::JOYSTICK_LAST >= this->Code); }

        Boole MetaCode::IsJoystickButton() const
            { return (Input::JOYSTICKBUTTON_FIRST <= this->Code && Input::JOYSTICKBUTTON_LAST >= this->Code); }

        Boole MetaCode::IsJoystickAxis() const
            { return (Input::JOYSTICKAXIS_FIRST <= this->Code && Input::JOYSTICKAXIS_LAST >= this->Code); }

        Boole MetaCode::IsJoystickBall() const
            { return (Input::JOYSTICKBALL_FIRST <= this->Code && Input::JOYSTICKBALL_LAST >= this->Code); }

        Boole MetaCode::IsJoystickHat() const
            { return (Input::JOYSTICKHAT_FIRST <= this->Code && Input::JOYSTICKHAT_LAST >= this->Code); }

        Boole MetaCode::IsControllerEvent() const
            { return (Input::CONTROLLER_FIRST <= this->Code && Input::CONTROLLER_LAST >= this->Code); }

        Boole MetaCode::IsControllerButton() const
            { return (Input::CONTROLLERBUTTON_FIRST <= this->Code && Input::CONTROLLERBUTTON_LAST > this->Code); }

        Boole MetaCode::IsControllerAxis() const
            { return (Input::CONTROLLERAXIS_FIRST <= this->Code && Input::CONTROLLERAXIS_LAST >= this->Code); }

        Boole MetaCode::IsDeviceButton() const
            { return (this->IsKeyboardButton() || this->IsMouseButton() || this->IsControllerButton()); }

        Boole MetaCode::IsMultitouchEvent() const
            { return (Input::MULTITOUCH_FIRST <= this->Code && Input::MULTITOUCH_LAST >= this->Code); }

        Boole MetaCode::IsAltKey() const
            { return (Input::KEY_LALT == this->Code || Input::KEY_RALT == this->Code); }

        Boole MetaCode::IsCtrlKey() const
            { return (Input::KEY_LCTRL == this->Code || Input::KEY_RCTRL == this->Code); }

        Boole MetaCode::IsShiftKey() const
            { return (Input::KEY_LSHIFT == this->Code || Input::KEY_RSHIFT == this->Code); }

        Boole MetaCode::IsSuperKey() const
            { return (Input::KEY_LSUPER == this->Code || Input::KEY_RSUPER == this->Code); }

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

        Boole MetaCode::operator==(const MetaCode& Other) const
        {
            return (this->Code == Other.Code &&
                    this->MetaValue == Other.MetaValue &&
                    this->DeviceID == Other.DeviceID);
        }

        Boole MetaCode::operator!=(const MetaCode& Other) const
        {
            return (this->Code != Other.Code ||
                    this->MetaValue != Other.MetaValue ||
                    this->DeviceID != Other.DeviceID);
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
                SelfNode.AppendAttribute("DeviceID").SetValue( this->DeviceID ) &&
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

                    CurrAttrib = OneNode.GetAttribute("DeviceID");
                    if( !CurrAttrib.Empty() )
                        this->DeviceID = static_cast<DeviceIDType>( CurrAttrib.AsInt() );

                    CurrAttrib = OneNode.GetAttribute("Code");
                    if( !CurrAttrib.Empty() )
                        this->Code = static_cast<Input::InputCode>( CurrAttrib.AsUint() );
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + MetaCode::GetSerializableName() + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,MetaCode::GetSerializableName() + " was not found in the provided XML node, which was expected.");
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
