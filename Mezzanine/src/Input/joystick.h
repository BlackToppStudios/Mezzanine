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
#ifndef _inputjoystick_h
#define _inputjoystick_h

#include "Input/inputenumerations.h"
#include "Input/buttondevice.h"

namespace Mezzanine
{
    namespace Input
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class represents a Joystick input device.
        ///////////////////////////////////////
        class MEZZ_LIB Joystick : public ButtonDevice<Input::JOYSTICK_BUTTONMAX>
        {
        public:
            /// @brief Convenience type for the storage of Joystick axis states.
            using AxisContainer = std::array<Int16,Input::JOYSTICK_AXISMAX>;
            /// @brief Convenience type for the storage of Joystick trackball states.
            using TrackballContainer = std::array<Int16,Input::JOYSTICK_TRACKBALLMAX>;
            /// @brief Convenience type for the storage of Joystick hat states.
            using HatContainer = std::array<Input::HatState,Input::JOYSTICK_HATMAX>;
        protected:
            /// @brief Container storing the states for each axis on the Joystick.
            AxisContainer Axes;
            /// @brief Container storing the states for each trackball on the Joystick.
            TrackballContainer Trackballs;
            /// @brief Container storing the states for each hat on the Joystick.
            HatContainer Hats;
            /// @brief The name of the Joystick device.
            String DeviceName;
            /// @brief A pointer to the internal Joystick device.
            SDL_Joystick* Stick;
            /// @brief The index of the Joystick device.
            DeviceIDType DeviceID;

            /// @copydoc ButtonDevice::UpdateImpl(ConstMetaCodeIterator, ConstMetaCodeIterator)
            MetaCodeContainer UpdateImpl(ConstMetaCodeIterator DeltaBegin, ConstMetaCodeIterator DeltaEnd);
        public:
            /// @brief Class constructor.
            /// @param InternalStick A pointer to the internal struct of this Joystick.
            /// @param Index The internal index of this device.
            Joystick(SDL_Joystick* InternalStick, int Index);
            /// @brief Copy constructor.
            /// @param Other The other Joystick to be copied.
            Joystick(const Joystick& Other) = delete;
            /// @brief Move constructor.
            /// @param Other The other Joystick to be moved.
            Joystick(Joystick&& Other) = delete;
            /// @brief Class destructor.
            virtual ~Joystick() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Assignment operator.
            /// @param Other The other Joystick to be copied.
            /// @return Returns a reference to this.
            Joystick& operator=(const Joystick& Other) = delete;
            /// @brief Move assignment operator.
            /// @param Other The other Joystick to be moved.
            /// @return Returns a reference to this.
            Joystick& operator=(Joystick&& Other) = delete;

            ///////////////////////////////////////////////////////////////////////////////
            // Query Methods

            /// @copydoc Device::GetDeviceID() const
            DeviceIDType GetDeviceID() const;
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

            /// @brief Gets the number of axes on this device.
            /// @return Returns a UInt16 representing the number of axes present on this Joystick.
            UInt16 GetNumAxes() const;
            /// @brief This Gets the value of the given Joystick axis.
            /// @param Axis The axis that you want.  This value is expected to be 1-20.
            /// @return An integer with the Value of the Joystick axis.
            Int16 GetAxis(const UInt16 Axis) const;
            /// @brief This Gets the value of the given Joystick axis.
            /// @param Axis An InputCode representing the axis that you want.
            /// @return An integer with the Value of the Joystick axis.
            Int16 GetAxis(const Input::InputCode Axis) const;

            /// @brief Gets the number of Trackballs on this device.
            /// @return Returns a UInt16 representing the number of Trackballs present on this Joystick.
            UInt16 GetNumTrackballs() const;
            /// @brief Gets the delta movement on the requested trackball.
            /// @remarks The layout of the trackball states is XYXY.  So an index of 1 and 3 will get the X axis
            /// state for the first and second trackballs, respectively.  2 and 4 will retrieve the Y axis the
            /// same way.
            /// @param TrackballAxis The trackball axis to query.  This value is expected to be 1-4.
            /// @return Returns a Int16 representing the delta movement for the specified trackball axis.
            Int16 GetTrackballDelta(const UInt16 TrackballAxis) const;
            /// @brief Gets the delta movement on the requested trackball.
            /// @exception Throws an InvalidParametersException if anything other than a Joystick ball value from the @ref Input enum is passed in.
            /// @param TrackballAxis An InputCode representing the trackball axis to query.
            /// @return Returns a Int16 representing the delta movement for the specified trackball.
            Int16 GetTrackballDelta(const Input::InputCode TrackballAxis) const;

            /// @brief Gets the number of Hats on this device.
            /// @return Returns a UInt16 representing the number of Hats present on this Joystick.
            UInt16 GetNumHats() const;
            /// @brief Gets the current state of the requested hat.
            /// @param Hat The hat to query.  This value is expected to be 1-6.
            /// @return Returns a HatState representing the current state of the specified hat.
            Input::HatState GetHatState(const UInt16 Hat) const;
            /// @brief Gets the current state of the requested hat.
            /// @param Hat An InputCode representing the hat to query.
            /// @return Returns a HatState representing the current state of the specified hat.
            Input::HatState GetHatState(const Input::InputCode Hat) const;
            /// @brief Gets whether a specific hat is pressed in a specific direction.
            /// @param WhichWay The hat direction to check for.  This value is expected to be 1-6.
            /// @return Returns true if the specified hat is pressed in the requested direction.
            Boole IsHatPushedInDirection(const UInt16 Hat, const Input::HatState WhichWay) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @brief Gets the internal representation of the Joystick.
            /// @return Returns a pointer to the internal Joystick device.
            SDL_Joystick* _GetInternalDevice() const;
        };//Joystick
    }//Input
}//Mezzanine

#endif
