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
#ifndef _inputsequencecontainer_cpp
#define _inputsequencecontainer_cpp

#include "Input/sequencecontainer.h"
#include "timer.h"
#include "exception.h"

#include <limits>
#include <cassert>

namespace
{
    using namespace Mezzanine;
    enum SequenceConstants
    {
        InvalidSequenceID = std::numeric_limits<Int32>::max()
    };
}

namespace Mezzanine
{
    namespace Input
    {
        SequenceContainer::SequenceContainer() :
            SequencedInputs(MetaCode::NullCode),
            SequenceTimeout(300),
            MaxSequenceSize(0),
            DeviceID(std::numeric_limits<UInt32>::max())
        {
            this->SequenceTimer.SetCurrentTimeInMilliseconds(this->SequenceTimeout);
            this->SequenceTimer.SetCountMode(Mezzanine::CM_CountDown);
        }

        SequenceContainer::SequenceContainer(const UInt32 Device) :
            SequencedInputs(MetaCode::NullCode),
            SequenceTimeout(300),
            MaxSequenceSize(0),
            DeviceID(Device)
        {
            this->SequenceTimer.SetCurrentTimeInMilliseconds(this->SequenceTimeout);
            this->SequenceTimer.SetCountMode(Mezzanine::CM_CountDown);
        }

        SequenceContainer::~SequenceContainer()
        {
            this->SequencedInputs.clear();
        }

        void SequenceContainer::CheckTimeout()
        {
            // Check our timer and clear our cache if it's been long enough since the last input
            if( this->SequenceTimer.IsStopped() ) {
                this->CurrSequence.clear();
            }else{
                this->SequenceTimer.Reset(this->SequenceTimeout * 1000);
            }
        }

        void SequenceContainer::TruncateSequenceCache()
        {
            Whole CurrentSize = this->CurrSequence.size();
            if( CurrentSize > this->MaxSequenceSize ) {
                Whole NewStartIndex = CurrentSize - this->MaxSequenceSize;
                MetaCodeIterator EraseIt = this->CurrSequence.begin();
                this->CurrSequence.erase(EraseIt,EraseIt + NewStartIndex);
            }
        }

        Int32 SequenceContainer::TestSequenceSingle(ConstMetaCodeIterator TestBegin, ConstMetaCodeIterator TestEnd)
        {
            MetaCodeContainer TestContainer(TestEnd,TestBegin);
            TestContainer.push_back( MetaCode::NullCode );
            SequencedInputIterator SeqIt = this->SequencedInputs.find(TestContainer);
            /// @todo The line below is how the logic SHOULD go, if our Trie allowed us to give it arbitrary
            /// iterators and used those to determine the range, rather than a pointer with the end symbol at
            /// the end of the range.  The above code is horrible for performance.  Grumble, Grumble.
            //SequencedInputIterator SeqIt = this->SequencedInputs.find(TestBegin,TestEnd);
            if( SeqIt == this->SequencedInputs.end() ) {
                return InvalidSequenceID;
            }
            return *(*SeqIt).second;
        }

        Int32 SequenceContainer::TestCurrentSequence()
        {
            if( this->CurrSequence.empty() ) {
                return InvalidSequenceID;
            }

            ConstMetaCodeIterator BeginIt = this->CurrSequence.begin();
            ConstMetaCodeIterator EndIt = this->CurrSequence.end();
            for( ConstMetaCodeIterator SeqBeginIt = BeginIt ; SeqBeginIt != EndIt ; ++SeqBeginIt )
            {
                for( ConstMetaCodeIterator SeqEndIt = SeqBeginIt + 1 ; SeqEndIt != EndIt ; ++SeqEndIt )
                {
                    Int32 Result = this->TestSequenceSingle(SeqBeginIt,SeqEndIt);
                    if( Result != InvalidSequenceID ) {
                        this->CurrSequence.erase(BeginIt,SeqEndIt);
                        return Result;
                    }
                }
            }
            return InvalidSequenceID;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Sequenced Input Management

        void SequenceContainer::AddInputSequence(const MetaCodeContainer& Codes, const Int32 SequenceID)
        {
            assert(InvalidSequenceID != SequenceID &&
                   "Attempting to use max value of Int32 as an ID for an input sequence.  This value is reserved for error conditions.");

            if( MetaCode::NullCode != Codes.back() ) {
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Attempting to add a Sequenced Input that is not terminated with a null MetaCode.");
            }
            this->SequencedInputs.insert(Codes,SequenceID);
            this->MaxSequenceSize = std::max(static_cast<size_t>(this->MaxSequenceSize),Codes.size());
            this->CurrSequence.reserve(this->MaxSequenceSize);
        }

        Boole SequenceContainer::InputSequenceExists(const MetaCodeContainer& Codes)
            { return this->SequencedInputs.hasKey(Codes); }

        Int32 SequenceContainer::GetIDofInputSequence(const MetaCodeContainer& Codes)
        {
            SequencedInputIterator SqIt = this->SequencedInputs.find(Codes);
            if( SqIt != this->SequencedInputs.end() ) {
                return *(SqIt->second);
            }else{
                return InvalidSequenceID;
            }
        }

        void SequenceContainer::RemoveInputSequence(const MetaCodeContainer& Codes)
            { this->SequencedInputs.erase(Codes); }

        void SequenceContainer::RemoveAllInputSequences()
            { this->SequencedInputs.clear(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void SequenceContainer::SetSequenceTimeout(const Whole Timeout)
            { this->SequenceTimeout = Timeout; }

        Whole SequenceContainer::GetSequenceTimeout() const
            { return this->SequenceTimeout; }

        UInt32 SequenceContainer::GetNumInputSequences() const
        {
            return static_cast<UInt32>( this->SequencedInputs.size() );
        }

        MetaCode SequenceContainer::DetectSequence(const MetaCode& Delta)
        {
            // Escape early if we have nothing to do
            if( this->SequencedInputs.empty() ) {
                return MetaCode::NullCode;
            }

            this->CheckTimeout();
            this->CurrSequence.push_back( Delta );
            Int32 Result = this->TestCurrentSequence();
            this->TruncateSequenceCache();
            if( Result != InvalidSequenceID ) {
                return MetaCode(Result,Input::COMPOUNDINPUT_CUSTOMSEQUENCE,this->DeviceID);
            }
            return MetaCode::NullCode;
        }

        MetaCodeContainer SequenceContainer::DetectSequence(ConstMetaCodeIterator DeltaBegin, ConstMetaCodeIterator DeltaEnd)
        {
            MetaCodeContainer Ret;
            // Escape early if we have nothing to do
            if( this->SequencedInputs.empty() ) {
                return Ret;
            }

            this->CheckTimeout();
            this->CurrSequence.insert(this->CurrSequence.end(),DeltaBegin,DeltaEnd);
            Int32 Result = InvalidSequenceID;
            do{
                Result = this->TestCurrentSequence();
                if( Result != InvalidSequenceID ) {
                    Ret.emplace_back(Result,Input::COMPOUNDINPUT_CUSTOMSEQUENCE,this->DeviceID);
                }
            }while( Result != InvalidSequenceID );
            this->TruncateSequenceCache();
            return Ret;
        }
    }//Input
}//Mezzanine

#endif
