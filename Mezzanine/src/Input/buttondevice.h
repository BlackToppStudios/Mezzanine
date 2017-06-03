// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _inputbuttondevice_h
#define _inputbuttondevice_h

#include "Input/metacode.h"
#include "Input/device.h"

namespace Mezzanine
{
    namespace Input
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a base class for all input devices with buttons.
        /// @details The primary responsibility of this class is to store all of the button states for the device,
        /// and to generate or detect input events that are not considered internally.
        ///////////////////////////////////////
        template<size_t NumButtons>
        class MEZZ_LIB ButtonDevice : public Device
        {
        protected:
            /// @brief A container of indexes being tracked due to state transitions.
            std::vector<Whole> TransitioningIndexes;
            /// @brief A container of states for each button on the input device.
            std::array<Input::ButtonState,NumButtons> Buttons;

            /// @brief Internal implementation of the device update.
            /// @param DeltaBegin An iterator to the start of the range of new codes to update this devices state with.
            /// @param DeltaEnd An iterator to the end of the range of new codes to update this devices state with.
            /// @return Returns a vector of the codes this device generated during the update.  Generally sequences and/or button transitions.
            virtual MetaCodeContainer UpdateImpl(ConstMetaCodeIterator DeltaBegin, ConstMetaCodeIterator DeltaEnd) = 0;
            /// @brief Updates transitioning buttons.
            /// @return Returns a container of the MetaCodes that were generated during the update.
            MetaCodeContainer UpdateButtonTransitions()
            {
                MetaCodeContainer GeneratedCodes;
                for( Whole CurrIndex : this->TransitioningIndexes )
                {
                    Input::ButtonState State = this->Buttons.at( CurrIndex );
                    if( Input::BUTTON_PRESSING == State ) {
                        this->Buttons.at( CurrIndex ) = Input::BUTTON_DOWN;
                        Input::InputCode ButtonCode = static_cast<Input::InputCode>(this->GetFirstButtonCode() + CurrIndex);
                        GeneratedCodes.emplace_back(Input::BUTTON_DOWN,ButtonCode,this->GetDeviceID());
                    }else if( Input::BUTTON_LIFTING == State ) {
                        this->Buttons.at( CurrIndex ) = Input::BUTTON_UP;
                    }
                }
                this->TransitioningIndexes.clear();
                return GeneratedCodes;
            }
        public:
            /// @brief Class constructor.
            ButtonDevice() :
                TransitioningIndexes(4)
                {  }
            /// @brief Copy constructor.
            /// @param Other The other ButtonDevice to be copied.
            ButtonDevice(const ButtonDevice& Other) = delete;
            /// @brief Move constructor.
            /// @param Other The other ButtonDevice to be moved.
            ButtonDevice(ButtonDevice&& Other) = delete;
            /// @brief Class destructor.
            virtual ~ButtonDevice() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Assignment operator.
            /// @param Other The other ButtonDevice to be copied.
            /// @return Returns a reference to this.
            ButtonDevice& operator=(const ButtonDevice& Other) = delete;
            /// @brief Move assignment operator.
            /// @param Other The other ButtonDevice to be moved.
            /// @return Returns a reference to this.
            ButtonDevice& operator=(ButtonDevice&& Other) = delete;

            ///////////////////////////////////////////////////////////////////////////////
            // Query Methods

            /// @brief Gets the number of buttons on this device.
            /// @return Returns a UInt16 representing the number of buttons on this device.
            UInt16 GetNumButtons() const
                { return this->Buttons.size(); }
            /// @brief Gets whether or not a device button is pressed down.
            /// @remarks This function accepts a button number.  Check the number of buttons on this device to get the acceptable range.  1 is the minimum value.
            /// @param Button The button to check the state of.
            /// @return Returns whether or not the requested button is pressed down.
            Boole IsButtonPressed(const UInt16 Button) const
                { return Input::BUTTON_UP < this->GetButtonState(Button); }
            /// @brief Gets whether or not a device button is pressed down.
            /// @param Button The button to check the state of.
            /// @return Returns whether or not the requested button is pressed down.
            Boole IsButtonPressed(const Input::InputCode Button) const
                { return Input::BUTTON_UP < this->GetButtonState(Button); }
            /// @brief Gets whether or not a device button was pressed this frame.
            /// @remarks This function accepts a button number.  Check the number of buttons on this device to get the acceptable range.  1 is the minimum value.
            /// @param Button The button to check the state of.
            /// @return Returns true if the specified button is being pressed this frame, false otherwise.
            Boole IsButtonPressing(const UInt16 Button) const
                { return Input::BUTTON_PRESSING == this->GetButtonState(Button); }
            /// @brief Gets whether or not a device button was pressed this frame.
            /// @param Button The button to check the state of.
            /// @return Returns true if the specified button is being pressed this frame, false otherwise.
            Boole IsButtonPressing(const Input::InputCode Button) const
                { return Input::BUTTON_PRESSING == this->GetButtonState(Button); }
            /// @brief Gets whether or not a device button was lifted this frame.
            /// @remarks This function accepts a button number.  Check the number of buttons on this device to get the acceptable range.  1 is the minimum value.
            /// @param Button The button to check the state of.
            /// @return Returns true if the specified button is being lifted this frame, false otherwise.
            Boole IsButtonLifting(const UInt16 Button) const
                { return Input::BUTTON_LIFTING == this->GetButtonState(Button); }
            /// @brief Gets whether or not a device button was lifted this frame.
            /// @param Button The button to check the state of.
            /// @return Returns true if the specified button is being lifted this frame, false otherwise.
            Boole IsButtonLifting(const Input::InputCode Button) const
                { return Input::BUTTON_LIFTING == this->GetButtonState(Button); }
            /// @brief Gets whether or not a device button was lifted or pressed this frame.
            /// @param Button The button to check the state of.
            /// @return Returns true if the requested button is pressing or lifting, false otherwise.
            Boole IsButtonTransitioning(const UInt16 Button) const
            {
                Input::ButtonState State = this->GetButtonState(Button);
                return (Input::BUTTON_LIFTING == State || Input::BUTTON_PRESSING == State);
            }
            /// @brief Gets whether or not a device button was lifted or pressed this frame.
            /// @param Button The button to check the state of.
            /// @return Returns true if the requested button is pressing or lifting, false otherwise.
            Boole IsButtonTransitioning(const Input::ButtonState Button) const
            {
                Input::ButtonState State = this->GetButtonState(Button);
                return (Input::BUTTON_LIFTING == State || Input::BUTTON_PRESSING == State);
            }
            /// @brief Checks to see if a button on this device is a specific state.
            /// @param Button The button to check the state of.
            /// @param State The button state to check for.
            /// @return Returns true if the requested button is the specified state, false otherwise.
            Boole CheckButtonState(const UInt16 Button, const Input::ButtonState State) const
                { return State == this->GetButtonState(Button); }
            /// @brief Checks to see if a button on this device is a specific state.
            /// @param Button The button to check the state of.
            /// @param State The button state to check for.
            /// @return Returns true if the requested button is the specified state, false otherwise.
            Boole CheckButtonState(const Input::InputCode Button, const Input::ButtonState State) const
                { return State == this->GetButtonState(Button); }
            /// @brief Gets the state of the requested button.
            /// @remarks This function accepts a button number, and as such expects a number from 1 to the
            /// maximum number of buttons supported for the device.  Most devices support up to 20 buttons.
            /// @return Returns the actual state of the requested button.
            virtual Input::ButtonState GetButtonState(const UInt16 Button) const = 0;
            /// @brief Gets the state of the requested button.
            /// @return Returns the actual state of the requested button.
            virtual Input::ButtonState GetButtonState(const Input::InputCode Button) const = 0;
            /// @brief Gets the InputCode of this devices first button.
            /// @return Returns the InputCode enum value of the first button on this device.
            virtual Input::InputCode GetFirstButtonCode() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @copydoc Device::_Update(ConstMetaCodeIterator, ConstMetaCodeIterator)
            MetaCodeContainer _Update(ConstMetaCodeIterator DeltaBegin, ConstMetaCodeIterator DeltaEnd)
            {
                MetaCodeContainer Ret = std::move( this->UpdateButtonTransitions() );
                MetaCodeContainer Temp = std::move( this->UpdateImpl(DeltaBegin,DeltaEnd) );
                Ret.insert(Ret.begin(),Temp.begin(),Temp.end());
                return Ret;
            }
        };//ButtonDevice
    }//Input
}//Mezzanine

#endif
