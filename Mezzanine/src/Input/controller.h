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
#ifndef _inputcontroller_h
#define _inputcontroller_h

#include "Input/inputenumerations.h"
#include "Input/buttondevice.h"

namespace Mezzanine
{
    namespace Input
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class represents a controller input device.
        ///////////////////////////////////////
        class MEZZ_LIB Controller : public ButtonDevice<Input::CONTROLLER_BUTTONMAX>
        {
        public:
            /// @brief Convenience type for the storage of controller axis states.
            using AxisContainer = std::array<Int16,Input::CONTROLLER_AXISMAX>;
        protected:
            /// @brief Container storing the states for each axis on the controller.
            AxisContainer Axes;
            /// @brief The name of the controller device.
            String DeviceName;
            /// @brief A pointer to the internal Controller device.
            SDL_GameController* Control;
            /// @brief The index of the controller device.
            UInt16 DeviceIndex;

            /// @copydoc ButtonDevice::UpdateImpl(ConstMetaCodeIterator, ConstMetaCodeIterator)
            MetaCodeContainer UpdateImpl(ConstMetaCodeIterator DeltaBegin, ConstMetaCodeIterator DeltaEnd);
        public:
            /// @brief Class constructor.
            /// @param InternalControl A pointer to the internal struct of this Controller.
            /// @param Index The internal index of this device.
            Controller(SDL_GameController* InternalControl, int Index);
            /// @brief Copy constructor.
            /// @param Other The other Controller to be copied.
            Controller(const Controller& Other) = delete;
            /// @brief Move constructor.
            /// @param Other The other Controller to be moved.
            Controller(Controller&& Other) = delete;
            /// @brief Class destructor.
            virtual ~Controller() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Assignment operator.
            /// @param Other The other Controller to be copied.
            /// @return Returns a reference to this.
            Controller& operator=(const Controller& Other) = delete;
            /// @brief Move assignment operator.
            /// @param Other The other Controller to be moved.
            /// @return Returns a reference to this.
            Controller& operator=(Controller&& Other) = delete;

            ///////////////////////////////////////////////////////////////////////////////
            // Query Methods

            /// @copydoc Device::GetDeviceIndex() const
            UInt16 GetDeviceIndex() const;
            /// @copydoc Device::GetDeviceType() const
            Input::InputDevice GetDeviceType() const;
            /// @copydoc ButtonDevice::GetButtonState(const UInt16) const
            virtual Input::ButtonState GetButtonState(const UInt16 Button) const;
            /// @copydoc ButtonDevice::GetButtonState(const Input::InputCode) const
            virtual Input::ButtonState GetButtonState(const Input::InputCode Button) const;
            /// @copydoc ButtonDevice::GetFirstButtonCode() const
            Input::InputCode GetFirstButtonCode() const;

            /// @brief Gets the name of this device.
            /// @return Returns a const String reference containing the name of this device.
            const String& GetDeviceName() const;

            /// @brief Gets the number of Axes on this device.
            /// @return Returns a UInt16 representing the number of Axes present on this Controller.
            UInt16 GetNumAxes() const;
            /// @brief This Gets the value of the given Controller axis.
            /// @param Axis The axis that you want.  This value is expected to be 1-20.
            /// @return An integer with the Value of the Controller axis.
            Int16 GetAxis(const UInt16 Axis) const;
            /// @brief This Gets the value of the given Controller axis.
            /// @param Axis An InputCode representing the axis that you want.
            /// @return An integer with the Value of the Controller axis.
            Int16 GetAxis(const Input::InputCode Axis) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @brief Gets the internal representation of the Controller.
            /// @return Returns a pointer to the internal Controller device.
            SDL_GameController* _GetInternalDevice() const;
        };//Controller
    }//Input
}//Mezzanine

#endif
