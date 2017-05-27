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
#ifndef _inputcontroller_cpp
#define _inputcontroller_cpp

#include "Input/controller.h"

#include "exception.h"

#include "SDL.h"

namespace Mezzanine
{
    namespace Input
    {
        Controller::Controller(SDL_GameController* InternalControl, int Index) :
            Control(InternalControl)
        {
            //this->Axes.resize( SDL_JoystickNumAxes(InternalControl), 0 );
            //this->Buttons.resize( SDL_JoystickNumButtons(InternalControl), Input::BUTTON_UP );
            SDL_Joystick* InternalStick = SDL_GameControllerGetJoystick(InternalControl);
            this->DeviceName = SDL_GameControllerName(InternalControl);
            this->DeviceID = SDL_JoystickInstanceID(InternalStick);
        }

        MetaCodeContainer Controller::UpdateImpl(ConstMetaCodeIterator DeltaBegin, ConstMetaCodeIterator DeltaEnd)
        {
            for( ConstMetaCodeIterator CurrDelta = DeltaBegin ; CurrDelta != DeltaEnd ; ++CurrDelta )
            {
                const MetaCode& CurrCode = (*CurrDelta);
                Input::InputCode ICode = CurrCode.GetCode();
                if( ICode >= Input::CONTROLLERAXIS_FIRST && ICode <= Input::CONTROLLERAXIS_LAST ) {
                    this->Axes.at( ICode - Input::CONTROLLERAXIS_FIRST ) = CurrCode.GetMetaValue();
                }else if( ICode >= Input::CONTROLLERBUTTON_FIRST && ICode <= Input::CONTROLLERBUTTON_LAST ) {
                    this->TransitioningIndexes.push_back( ICode - Input::CONTROLLERBUTTON_FIRST );
                    this->Buttons.at( ICode - Input::CONTROLLERBUTTON_FIRST ) = static_cast<Input::ButtonState>(CurrCode.GetMetaValue());
                }
            }
            return std::move( this->Sequences.DetectSequence(DeltaBegin,DeltaEnd) );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        ///////////////////////////////////////////////////////////////////////////////
        // Query Methods

        DeviceIDType Controller::GetDeviceID() const
            { return this->DeviceID; }

        Input::InputDevice Controller::GetDeviceType() const
            { return Input::DEVICE_CONTROLLER; }

        Input::ButtonState Controller::GetButtonState(const UInt16 Button) const
            { return this->Buttons.at( Button - 1 ); }

        Input::ButtonState Controller::GetButtonState(const Input::InputCode Button) const
            { return this->Buttons.at( Button - Input::CONTROLLERBUTTON_FIRST ); }

        Input::InputCode Controller::GetFirstButtonCode() const
            { return Input::CONTROLLERBUTTON_FIRST; }

        const String& Controller::GetDeviceName() const
            { return this->DeviceName; }

        UInt16 Controller::GetNumAxes() const
            { return this->Axes.size(); }

        Int16 Controller::GetAxis(const UInt16 Axis) const
            { return this->Axes.at( Axis - 1 ); }

        Int16 Controller::GetAxis(const Input::InputCode Axis) const
            { return this->Axes.at( Axis - Input::CONTROLLERAXIS_FIRST ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        SDL_GameController* Controller::_GetInternalDevice() const
            { return this->Control; }
    }//Input
}//Mezzanine

#endif
