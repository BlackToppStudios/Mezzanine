// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
            SequenceContainer Sequences;
            /// @internal
            /// @brief Provides the device specific logic for filtering code sequences.
            /// @param Sequence The MetaCode sequence to be verified.
            virtual void VerifySequenceImpl(const MetaCodeContainer& Sequence) const = 0;
        public:
            /// @brief Class constructor.
            Device();
            /// @brief Class destructor.
            virtual ~Device();

            ///////////////////////////////////////////////////////////////////////////////
            // Query Methods

            /// @brief Gets the device index of this controller.
            /// @return Returns a UInt16 representing the device index for this controller.
            virtual UInt16 GetDeviceIndex() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Sequenced Input Management

            /// @copydoc SequenceContainer::AddInputSequence(const MetaCodeContainer& Codes, const Int32& SequenceID)
            void AddInputSequence(const MetaCodeContainer& Codes, const Int32& SequenceID);
            /// @copydoc SequenceContainer::InputSequenceExists(const MetaCodeContainer& Codes)
            bool InputSequenceExists(const MetaCodeContainer& Codes);
            /// @copydoc SequenceContainer::GetIDofInputSequence(const MetaCodeContainer& Codes)
            Int32 GetIDofInputSequence(const MetaCodeContainer& Codes);
            /// @copydoc SequenceContainer::RemoveInputSequence(const MetaCodeContainer& Codes)
            void RemoveInputSequence(const MetaCodeContainer& Codes);
            /// @copydoc SequenceContainer::RemoveAllInputSequences()
            void RemoveAllInputSequences();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Updates this device with the newest data.
            /// @param DeltaCodes A vector of the codes to process and update this device with.
            /// @param GeneratedCodes A vector to which generated codes (sequence or otherwise) will be added.
            virtual void _Update(const MetaCodeContainer& DeltaCodes, MetaCodeContainer& GeneratedCodes) = 0;
        };//Device
    }//Input
}//Mezzanine

#endif
