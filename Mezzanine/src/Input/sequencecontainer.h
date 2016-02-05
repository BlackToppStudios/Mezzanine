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
#ifndef _inputsequencecontainer_h
#define _inputsequencecontainer_h

#include "trie.h"
#include "Input/metacode.h"

namespace Mezzanine
{
    class StopWatchTimer;
    namespace Input
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class SequenceContainer
        /// @headerfile sequencecontainer.h
        /// @brief This class stores and provides utilities for sequenced inputs and their operations.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB SequenceContainer
        {
        public:
            typedef Trie<Input::MetaCode,Int32>          SequencedInputContainer;
            typedef SequencedInputContainer::iterator    SequencedInputIterator;
        protected:
            UInt16 MaxSequenceSize;
            const UInt32 DeviceIndex;
            StopWatchTimer* SequenceTimer;
            MetaCodeContainer CurrSequenceCache;
            SequencedInputContainer SequencedInputs;
            /// @internal
            /// @brief Verify's a sequence of MetaCode's is terminated with a null MetaCode.
            /// @param Codes The vector of MetaCode's to verify.
            virtual void VerifyInputSequence(const MetaCodeContainer& Codes) const;
            /// @internal
            /// @brief Verify's a numer is valid to be used as an ID for an input sequence being inserted.
            /// @param ID The identification number to verify.
            void VerifyInputID(const Int32& ID) const;
            /// @internal
            /// @brief Processes a sequence(as defined by an iterator range) to see if any action needs to be taken.
            /// @param First An iterator to the first metacode in the sequence.
            /// @param OneAfterLast An iterator to the space after the last metacode in the sequence.
            /// @return Returns a new metacode for a completed sequence, or a "Null" metacode if no sequence was completed.
            MetaCode ProcessSequence(MetaCodeIterator First, MetaCodeIterator OneAfterLast);
        public:
            /// @brief Class constructor.
            SequenceContainer();
            /// @brief Controller constructor.
            /// @param Device The index of the controller device this container belongs to.
            SequenceContainer(const UInt32& Device);
            /// @brief Class destructor.
            virtual ~SequenceContainer();

            ///////////////////////////////////////////////////////////////////////////////
            // Sequenced Input Management

            /// @brief Adds a custom sequence of inputs that this system will look for and generate MetaCode's for when they occur.
            /// @exception If the vector of MetaCode's doesn't end with a null MetaCode, an exception will be thrown.  An exception can also be thrown if the ID provided is the max value of an Int32.
            /// @param Codes A vector containing the sequence of MetaCode's to be added.
            /// @param SequenceID A unique UInt32 to be used as the identifier for this sequence when a MetaCode is generated.
            void AddInputSequence(const MetaCodeContainer& Codes, const Int32& SequenceID);
            /// @brief Checks to see if the provided sequence of MetaCode's is already being checked for.
            /// @exception If the vector of MetaCode's doesn't end with a null MetaCode, an exception will be thrown.
            /// @param Codes A vector containing the sequence of MetaCode's to check for.
            /// @return Returns true if the sequence already exists, false otherwise.
            Boole InputSequenceExists(const MetaCodeContainer& Codes);
            /// @brief Gets the ID of the provided sequence of MetaCode's.
            /// @exception If the vector of MetaCode's doesn't end with a null MetaCode, an exception will be thrown.
            /// @param Codes A vector containing the sequence of MetaCode's to get the ID for.
            /// @return Returns the ID of the provided sequence, or the max value of an Int32 if the sequence does not exist.
            Int32 GetIDofInputSequence(const MetaCodeContainer& Codes);
            /// @brief Removes the specified custom sequence of MetaCode's.
            /// @exception If the vector of MetaCode's doesn't end with a null MetaCode, an exception will be thrown.
            /// @param Codes A vector containing the sequence of MetaCode's to be removed.
            void RemoveInputSequence(const MetaCodeContainer& Codes);
            /// @brief Removes all stored input sequences.
            void RemoveAllInputSequences();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the number of input sequences stored in this sequence container.
            /// @return Returns a UInt32 containing the number of sequences stored.
            UInt32 GetNumInputSequences() const;
            /// @brief Adds provided codes to the cache if necessary and checks for sequences that have been met.
            /// @param NormalCodes A vector of the codes to add and/or check as a part of another sequence.
            /// @param SequenceCodes A vector to which generated sequence codes will be added if they are generated.
            void Update(const MetaCodeContainer& NormalCodes, MetaCodeContainer& SequenceCodes);
        };//SequenceContainer
    }//Input
}//Mezzanine

#endif
