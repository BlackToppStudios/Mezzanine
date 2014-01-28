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
#ifndef _inputcontroller_h
#define _inputcontroller_h

#include "vector2.h"
#include "Input/inputenumerations.h"
#include "Input/buttondevice.h"

namespace Mezzanine
{
    namespace Input
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Controller
        /// @headerfile controller.h
        /// @brief This class represents a controller input device, such as a gamepad or joystick.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Controller : public ButtonDevice
        {
        protected:
            UInt16 Index;
            String DeviceName;
            std::vector<Int16> Axes;
            std::vector<Vector2> Trackballs;
            std::vector<Input::HatState> Hats;
            /// @copydoc ButtonDevice::UpdateImpl(const MetaCodeContainer& DeltaCodes, MetaCodeContainer& GeneratedCodes)
            void UpdateImpl(const MetaCodeContainer& DeltaCodes, MetaCodeContainer& GeneratedCodes);
            /// @copydoc Device::VerifySequenceImpl(const MetaCodeContainer& Sequence)
            virtual void VerifySequenceImpl(const MetaCodeContainer& Sequence) const;
            /// @copydoc Device::AddPressedButtons(MetaCodeContainer& GeneratedCodes) const
            virtual void AddPressedButtons(MetaCodeContainer& GeneratedCodes) const;
        public:
            /// @brief Class constructor.
            /// @param InternalControl A pointer to the internal struct of this controller.
            /// @remarks A void pointer is necessary because SDL forward declares it's own struct already and this won't
            /// compile with two forward declares.
            Controller(void* InternalControl, int Count);
            /// @brief Class destructor.
            virtual ~Controller();

            ///////////////////////////////////////////////////////////////////////////////
            // Query Methods

            /// @brief Gets the device index of this controller.
            /// @return Returns a UInt16 representing the device index for this controller.
            UInt16 GetDeviceIndex() const;
            /// @brief Gets the name of this device.
            /// @return Returns a const string reference containing the name of this device.
            const String& GetDeviceName() const;
            /// @brief Gets the number of Axes on this device.
            /// @return Returns a UInt16 representing the number of Axes present on this controller.
            UInt16 GetNumAxes() const;
            /// @brief Gets the number of Trackballs on this device.
            /// @return Returns a UInt16 representing the number of Trackballs present on this controller.
            UInt16 GetNumTrackballs() const;
            /// @brief Gets the number of Hats on this device.
            /// @return Returns a UInt16 representing the number of Hats present on this controller.
            UInt16 GetNumHats() const;
            /// @brief Gets whether a specific hat is pressed in a specific direction.
            /// @param WhichWay The hat direction to check for.  This value is expected to be 1-6.
            /// @return Returns true if the specified hat is pressed in the requested direction.
            bool IsHatPushedInDirection(const UInt16 Hat, const Input::HatState& WhichWay) const;
            /// @brief This Gets the value of the given joystick axis.
            /// @param Axis The axis that you want.  This value is expected to be 1-20.
            /// @return An integer with the Value of the joystick axis.
            Int16 GetAxis(const UInt16 Axis) const;
            /// @brief Gets the delta movement on the requested trackball.
            /// @param Trackball The trackball to query.  This value is expected to be at least 1.
            /// @return Returns a vector2 representing the delta movement for the specified trackball.
            Vector2 GetTrackballDelta(const UInt16 Trackball) const;
            /// @brief Gets the delta movement on the X axis on the requested trackball.
            /// @param Trackball The trackball to query.  This value is expected to be at least 1.
            /// @return Returns a Real representing the delta movement on the X axis for the specified trackball.
            Real GetTrackballDeltaX(const UInt16 Trackball) const;
            /// @brief Gets the delta movement on the Y axis on the requested trackball.
            /// @param Trackball The trackball to query.  This value is expected to be at least 1.
            /// @return Returns a Real representing the delta movement on the Y axis for the specified trackball.
            Real GetTrackballDeltaY(const UInt16 Trackball) const;
            /// @brief Gets the current state of the requested hat.
            /// @param Hat The hat to query.  This value is expected to be 1-6.
            /// @return Returns a HatState representing the current state of the specified hat.
            const Input::HatState& GetHatState(const UInt16 Hat) const;
            /// @brief This Gets the value of the given joystick axis.
            /// @param Axis An InputCode representing the axis that you want.
            /// @return An integer with the Value of the joystick axis.
            Int16 GetAxis(const Input::InputCode& Axis) const;
            /// @brief Gets the delta movement on the requested trackball.
            /// @param Trackball An InputCode representing the trackball to query.
            /// @return Returns a vector2 representing the delta movement for the specified trackball.
            /// @throw A @ref InvalidParametersException is thrown if anything other than a controllerball value from the @ref Input enum is passed.
            Real GetTrackballDelta(const Input::InputCode& Trackball) const;
            /// @copydoc ButtonDevice::GetButtonState(const UInt16 Button) const
            virtual const Input::ButtonState& GetButtonState(const UInt16 Button) const;
            /// @copydoc ButtonDevice::GetButtonState(const Input::InputCode& Button) const
            virtual const Input::ButtonState& GetButtonState(const Input::InputCode& Button) const;
            /// @brief Gets the current state of the requested hat.
            /// @param Hat An InputCode representing the hat to query.
            /// @return Returns a HatState representing the current state of the specified hat.
            const Input::HatState& GetHatState(const Input::InputCode& Hat) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration Methods

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods
        };// Controller
    }//Input
}//Mezzanine

#endif
