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
#ifndef _inputdevice_h
#define _inputdevice_h

#include "Input/sequencecontainer.h"
#include "Input/inputtypes.h"
#include "exception.h"

/// @file
/// @brief The declaration of the User input Device class.

struct _SDL_Joystick;
typedef _SDL_Joystick SDL_Joystick;
struct _SDL_GameController;
typedef _SDL_GameController SDL_GameController;
namespace Mezzanine
{
    namespace Input
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a base class for all input devices.
        ///////////////////////////////////////
        class MEZZ_LIB Device
        {
        protected:
            /// @brief A container for storing and detecting input sequences for an input device.
            SequenceContainer Sequences;
        public:
            /// @brief Class constructor.
            Device() = default;
            /// @brief Copy constructor.
            /// @param Other The other Device to be copied.
            Device(const Device& Other) = delete;
            /// @brief Move constructor.
            /// @param Other The other Device to be moved.
            Device(Device&& Other) = delete;
            /// @brief Class destructor.
            virtual ~Device() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Assignment operator.
            /// @param Other The other Device to be copied.
            /// @return Returns a reference to this.
            Device& operator=(const Device& Other) = delete;
            /// @brief Move assignment operator.
            /// @param Other The other Device to be moved.
            /// @return Returns a reference to this.
            Device& operator=(Device&& Other) = delete;

            ///////////////////////////////////////////////////////////////////////////////
            // Query Methods

            /// @brief Gets the device index of this device.
            /// @remarks Keyboards and mice do not have device ID's, and the ID returned will be the maximum representable value.
            /// @return Returns a DeviceIDType representing the device index for this device.
            virtual DeviceIDType GetDeviceID() const = 0;
            /// @brief Gets the type of input device this is.
            /// @return Returns an InputDevice enum value representing the type of input device this is.
            virtual Input::InputDevice GetDeviceType() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Sequenced Input Management

            /// @copydoc SequenceContainer::AddInputSequence(const MetaCodeContainer&, const Int32)
            void AddInputSequence(const MetaCodeContainer& Codes, const Int32 SequenceID)
            {
                for( const MetaCode& CurrCode : Codes )
                {
                    if( CurrCode.GetDeviceType() != this->GetDeviceType() ) {
                        MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Mismatched device on MetaCode detected when attempting to insert an Input Sequence.");
                    }
                }
                this->Sequences.AddInputSequence(Codes,SequenceID);
            }
            /// @copydoc SequenceContainer::InputSequenceExists(const MetaCodeContainer&)
            Boole InputSequenceExists(const MetaCodeContainer& Codes)
                { return this->Sequences.InputSequenceExists(Codes); }
            /// @copydoc SequenceContainer::GetIDofInputSequence(const MetaCodeContainer&)
            Int32 GetIDofInputSequence(const MetaCodeContainer& Codes)
                { return this->Sequences.InputSequenceExists(Codes); }
            /// @copydoc SequenceContainer::RemoveInputSequence(const MetaCodeContainer&)
            void RemoveInputSequence(const MetaCodeContainer& Codes)
                { this->Sequences.RemoveInputSequence(Codes); }
            /// @copydoc SequenceContainer::RemoveAllInputSequences()
            void RemoveAllInputSequences()
                { this->Sequences.RemoveAllInputSequences(); }

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @brief Updates this device with the newest data.
            /// @param DeltaBegin An iterator to the start of the range of new codes to update this devices state with.
            /// @param DeltaEnd An iterator to the end of the range of new codes to update this devices state with.
            /// @return Returns a vector of the codes this device generated during the update.  Generally sequences and/or button transitions.
            virtual MetaCodeContainer _Update(ConstMetaCodeIterator DeltaBegin, ConstMetaCodeIterator DeltaEnd) = 0;
        };//Device
    }//Input
}//Mezzanine

#endif
