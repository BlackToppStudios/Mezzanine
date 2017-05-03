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
#ifndef _inputkeyboard_h
#define _inputkeyboard_h

#include "datatypes.h"
#include "Input/inputenumerations.h"
#include "Input/buttondevice.h"

namespace Mezzanine
{
    namespace Input
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class represents the keyboard input device.
        ///////////////////////////////////////
        class MEZZ_LIB Keyboard : public ButtonDevice<Input::KEYBOARD_BUTTONMAX>
        {
        protected:
            /// @copydoc ButtonDevice::UpdateImpl(ConstMetaCodeIterator, ConstMetaCodeIterator)
            MetaCodeContainer UpdateImpl(ConstMetaCodeIterator DeltaBegin, ConstMetaCodeIterator DeltaEnd);
        public:
            /// @brief Class constructor.
            Keyboard();
            /// @brief Copy constructor.
            /// @param Other The other Keyboard to be copied.
            Keyboard(const Keyboard& Other) = delete;
            /// @brief Move constructor.
            /// @param Other The other Keyboard to be moved.
            Keyboard(Keyboard&& Other) = delete;
            /// @brief Class destructor.
            virtual ~Keyboard() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Assignment operator.
            /// @param Other The other Keyboard to be copied.
            /// @return Returns a reference to this.
            Keyboard& operator=(const Keyboard& Other) = delete;
            /// @brief Move assignment operator.
            /// @param Other The other Keyboard to be moved.
            /// @return Returns a reference to this.
            Keyboard& operator=(Keyboard&& Other) = delete;

            ///////////////////////////////////////////////////////////////////////////////
            // Query Methods

            /// @copydoc Device::GetDeviceIndex() const
            UInt16 GetDeviceIndex() const;
            /// @copydoc Device::GetDeviceType() const
            Input::InputDevice GetDeviceType() const;
            /// @copydoc ButtonDevice::GetButtonState(const UInt16) const
            Input::ButtonState GetButtonState(const UInt16 Button) const;
            /// @copydoc ButtonDevice::GetButtonState(const Input::InputCode) const
            Input::ButtonState GetButtonState(const Input::InputCode Button) const;
            /// @copydoc ButtonDevice::GetFirstButtonCode() const
            Input::InputCode GetFirstButtonCode() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration Methods

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods
        };//Keyboard
    }//Input
}//Mezzanine

#endif
