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
#ifndef _inputdevice_h
#define _inputdevice_h

#include "Input/metacode.h"

/// @file
/// @brief The declaration of the User input Device class.

namespace Mezzanine
{
    namespace Input
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a base class for all input devices.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Device
        {
            protected:
                std::vector<Whole> TransitioningIndexes;
                std::vector<Input::ButtonState> Buttons;
                /// @internal
                /// @brief Updates transitioning buttons.
                void UpdateButtonTransitions();
                /// @internal
                /// @brief Internal implementation of the device update.
                virtual void UpdateImpl(std::vector<MetaCode>& Codes) = 0;
            public:
                /// @brief Class constructor.
                Device();
                /// @brief Class destructor.
                virtual ~Device();

                ///////////////////////////////////////////////////////////////////////////////
                // Query Methods

                /// @brief Gets the number of buttons on this device.
                /// @return Returns a UInt16 representing the number of buttons on this device.
                UInt16 GetNumButtons() const;
                /// @brief Gets whether or not a device button is pressed down.
                /// @remarks This function accepts a button number.  Check the number of buttons on this device to get the acceptable range.  1 is the minimum value.
                /// @param Button The button to check the state of.
                /// @return Returns whether or not the requested button is pressed down.
                bool IsButtonPressed(const UInt16 Button) const;
                /// @brief Gets whether or not a device button is pressed down.
                /// @param Button The button to check the state of.
                /// @return Returns whether or not the requested button is pressed down.
                bool IsButtonPressed(const Input::InputCode& Button) const;
                /// @brief Gets whether or not a device button was pressed this frame.
                /// @remarks This function accepts a button number.  Check the number of buttons on this device to get the acceptable range.  1 is the minimum value.
                /// @param Button The button to check the state of.
                /// @return Returns true if the specified button is being pressed this frame, false otherwise.
                bool IsButtonPressing(const UInt16 Button) const;
                /// @brief Gets whether or not a device button was pressed this frame.
                /// @param Button The button to check the state of.
                /// @return Returns true if the specified button is being pressed this frame, false otherwise.
                bool IsButtonPressing(const Input::InputCode& Button) const;
                /// @brief Gets whether or not a device button was lifted this frame.
                /// @remarks This function accepts a button number.  Check the number of buttons on this device to get the acceptable range.  1 is the minimum value.
                /// @param Button The button to check the state of.
                /// @return Returns true if the specified button is being lifted this frame, false otherwise.
                bool IsButtonLifting(const UInt16 Button) const;
                /// @brief Gets whether or not a device button was lifted this frame.
                /// @param Button The button to check the state of.
                /// @return Returns true if the specified button is being lifted this frame, false otherwise.
                bool IsButtonLifting(const Input::InputCode& Button) const;
                /// @brief Gets whether or not a device button was lifted or pressed this frame.
                /// @param Button The button to check the state of.
                /// @return Returns true if the requested button is pressing or lifting, false otherwise.
                bool IsButtonTransitioning(const UInt16 Button) const;
                /// @brief Gets whether or not a device button was lifted or pressed this frame.
                /// @param Button The button to check the state of.
                /// @return Returns true if the requested button is pressing or lifting, false otherwise.
                bool IsButtonTransitioning(const Input::ButtonState& Button) const;
                /// @brief Checks to see if a button on this device is a specific state.
                /// @param Button The button to check the state of.
                /// @param State The button state to check for.
                /// @return Returns true if the requested button is the specified state, false otherwise.
                bool CheckButtonState(const UInt16 Button, const Input::ButtonState& State) const;
                /// @brief Checks to see if a button on this device is a specific state.
                /// @param Button The button to check the state of.
                /// @param State The button state to check for.
                /// @return Returns true if the requested button is the specified state, false otherwise.
                bool CheckButtonState(const Input::InputCode& Button, const Input::ButtonState& State) const;
                /// @brief Gets the state of the requested button.
                /// @remarks This function accepts a button number, and as such expects a number from 1 to 20.
                /// @return Returns the actual state of the requested button.
                virtual const Input::ButtonState& GetButtonState(const UInt16 Button) const = 0;
                /// @brief Gets the state of the requested button.
                /// @return Returns the actual state of the requested button.
                virtual const Input::ButtonState& GetButtonState(const Input::InputCode& Button) const = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @internal
                /// @brief Updates this device with the newest data.
                /// @param Codes A vector of the codes to process and update this device with.
                void _Update(std::vector<MetaCode>& Codes);
        };//Device
    }//Input
}//Mezzanine

#endif
