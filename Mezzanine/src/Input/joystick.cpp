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
#ifndef _inputjoystick_cpp
#define _inputjoystick_cpp

#include "Input/joystick.h"

#include "exception.h"

#include "SDL.h"

namespace Mezzanine
{
    namespace Input
    {
        Joystick::Joystick(SDL_Joystick* InternalStick, int Index) :
            Stick(InternalStick)
        {
            //this->Axes.resize( SDL_JoystickNumAxes(InternalStick), 0 );
            //this->Trackballs.resize( SDL_JoystickNumBalls(InternalStick), Vector2(0,0) );
            //this->Buttons.resize( SDL_JoystickNumButtons(InternalStick), Input::BUTTON_UP );
            //this->Hats.resize( SDL_JoystickNumHats(InternalStick), Input::CONTROLLERHAT_CENTERED );
            this->DeviceName = SDL_JoystickName(InternalStick);
            this->DeviceID = SDL_JoystickInstanceID(InternalStick);
        }

        MetaCodeContainer Joystick::UpdateImpl(ConstMetaCodeIterator DeltaBegin, ConstMetaCodeIterator DeltaEnd)
        {
            for( ConstMetaCodeIterator CurrDelta = DeltaBegin ; CurrDelta != DeltaEnd ; ++CurrDelta )
            {
                const MetaCode& CurrCode = (*CurrDelta);
                Input::InputCode ICode = CurrCode.GetCode();
                if( ICode >= Input::JOYSTICKAXIS_FIRST && ICode <= Input::JOYSTICKAXIS_LAST ) {
                    this->Axes.at( ICode - Input::CONTROLLERAXIS_FIRST ) = CurrCode.GetMetaValue();
                }else if( ICode >= Input::JOYSTICKBALL_FIRST && ICode <= Input::JOYSTICKBALL_LAST ) {
                    this->Trackballs.at( ICode - Input::JOYSTICKBALL_FIRST ) = CurrCode.GetMetaValue();
                }else if( ICode >= Input::JOYSTICKBUTTON_FIRST && ICode <= Input::JOYSTICKBUTTON_LAST ) {
                    this->TransitioningIndexes.push_back( ICode - Input::CONTROLLERBUTTON_FIRST );
                    this->Buttons.at( ICode - Input::JOYSTICKBUTTON_FIRST ) = static_cast<Input::ButtonState>(CurrCode.GetMetaValue());
                }else if( ICode >= Input::JOYSTICKHAT_FIRST && ICode <= Input::JOYSTICKHAT_LAST ) {
                    this->Hats.at( ICode - Input::JOYSTICKHAT_FIRST ) = static_cast<Input::HatState>(CurrCode.GetMetaValue());
                }
            }
            return std::move( this->Sequences.DetectSequence(DeltaBegin,DeltaEnd) );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        ///////////////////////////////////////////////////////////////////////////////
        // Query Methods

        DeviceIDType Joystick::GetDeviceID() const
            { return this->DeviceID; }

        Input::InputDevice Joystick::GetDeviceType() const
            { return Input::DEVICE_JOYSTICK; }

        Input::ButtonState Joystick::GetButtonState(const UInt16 Button) const
            { return this->Buttons.at( Button - 1 ); }

        Input::ButtonState Joystick::GetButtonState(const Input::InputCode Button) const
            { return this->Buttons.at( Button - Input::JOYSTICKBUTTON_FIRST ); }

        Input::InputCode Joystick::GetFirstButtonCode() const
            { return Input::JOYSTICKBUTTON_FIRST; }

        const String& Joystick::GetDeviceName() const
            { return this->DeviceName; }

        UInt16 Joystick::GetNumAxes() const
            { return this->Axes.size(); }

        Int16 Joystick::GetAxis(const UInt16 Axis) const
            { return this->Axes.at( Axis - 1 ); }

        Int16 Joystick::GetAxis(const Input::InputCode Axis) const
            { return this->Axes.at( Axis - Input::CONTROLLERAXIS_FIRST ); }

        UInt16 Joystick::GetNumTrackballs() const
            { return this->Trackballs.size(); }

        Int16 Joystick::GetTrackballDelta(const UInt16 TrackballAxis) const
            { return this->Trackballs.at( TrackballAxis - 1 ); }

        Int16 Joystick::GetTrackballDelta(const Input::InputCode TrackballAxis) const
            { return this->Trackballs.at( TrackballAxis - Input::JOYSTICKBALL_FIRST ); }

        UInt16 Joystick::GetNumHats() const
            { return this->Hats.size(); }

        Input::HatState Joystick::GetHatState(const UInt16 Hat) const
            { return this->Hats.at( Hat - 1 ); }

        Input::HatState Joystick::GetHatState(const Input::InputCode Hat) const
            { return this->Hats.at( Hat - Input::JOYSTICKHAT_FIRST ); }

        Boole Joystick::IsHatPushedInDirection(const UInt16 Hat, const Input::HatState WhichWay) const
            { return this->GetHatState(Hat) == WhichWay; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        SDL_Joystick* Joystick::_GetInternalDevice() const
            { return this->Stick; }
    }//Input
}//Mezzanine

#endif
