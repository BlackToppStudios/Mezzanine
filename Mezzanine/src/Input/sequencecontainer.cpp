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

namespace Mezzanine
{
    namespace Input
    {
        SequenceContainer::SequenceContainer() :
            MaxSequenceSize(0),
            DeviceID(std::numeric_limits<UInt32>::max()),
            SequencedInputs(MetaCode::NullCode)
        {
            this->SequenceTimer.SetCurrentTimeInMilliseconds(300);
            this->SequenceTimer.SetCountMode(Mezzanine::CM_CountDown);
        }

        SequenceContainer::SequenceContainer(const UInt32 Device) :
            MaxSequenceSize(0),
            DeviceID(Device),
            SequencedInputs(MetaCode::NullCode)
        {
            this->SequenceTimer.SetCurrentTimeInMilliseconds(300);
            this->SequenceTimer.SetCountMode(Mezzanine::CM_CountDown);
        }

        SequenceContainer::~SequenceContainer()
        {
            this->SequencedInputs.clear();
        }

        MetaCode SequenceContainer::TestCurrentSequence()
        {
            this->CurrSequence.push_back(MetaCode::NullCode);
            SequencedInputIterator SeqIt = this->SequencedInputs.find(this->CurrSequence);
            this->CurrSequence.pop_back();

            if( SeqIt == this->SequencedInputs.end() ) {
                return MetaCode::NullCode;
            }
            return MetaCode(*(*SeqIt).second,Input::COMPOUNDINPUT_CUSTOMSEQUENCE,this->DeviceID);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Sequenced Input Management

        void SequenceContainer::AddInputSequence(const MetaCodeContainer& Codes, const Int32 SequenceID)
        {
            assert(std::numeric_limits<Int32>::max() != SequenceID &&
                   "Attempting to use max value of Int32 as an ID for an input sequence.  This value is reserved for error conditions.");

            if( MetaCode::NullCode != Codes.back() ) {
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Attempting to add a Sequenced Input that is not terminated with a null MetaCode.");
            }
            this->SequencedInputs.insert(Codes,SequenceID);
            this->MaxSequenceSize = std::max(static_cast<size_t>(this->MaxSequenceSize),Codes.size());
        }

        Boole SequenceContainer::InputSequenceExists(const MetaCodeContainer& Codes)
            { return this->SequencedInputs.hasKey(Codes); }

        Int32 SequenceContainer::GetIDofInputSequence(const MetaCodeContainer& Codes)
        {
            SequencedInputIterator SqIt = this->SequencedInputs.find(Codes);
            if( SqIt != this->SequencedInputs.end() ) {
                return *(SqIt->second);
            }else{
                return std::numeric_limits<Int32>::max();
            }
        }

        void SequenceContainer::RemoveInputSequence(const MetaCodeContainer& Codes)
            { this->SequencedInputs.erase(Codes); }

        void SequenceContainer::RemoveAllInputSequences()
            { this->SequencedInputs.clear(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        UInt32 SequenceContainer::GetNumInputSequences() const
        {
            return static_cast<UInt32>( this->SequencedInputs.size() );
        }

        MetaCodeContainer SequenceContainer::Update(ConstMetaCodeIterator DeltaBegin, ConstMetaCodeIterator DeltaEnd)
        {
            MetaCodeContainer Ret;
            // Escape early if we have nothing to do
            if( this->SequencedInputs.empty() ) {
                return Ret;
            }

            // Check our timer and clear our cache if it's been long enough since the last input
            if( this->SequenceTimer.IsStopped() ) {
                this->CurrSequence.clear();
            }else{
                this->SequenceTimer.Reset(300 * 1000);
            }

            // Perform our per-code processing
            while( DeltaBegin != DeltaEnd )
            {
                if( this->CurrSequence.empty() ) {
                    this->CurrSequence.push_back( (*DeltaBegin) );
                    this->CurrSequence.push_back( MetaCode::NullCode );
                    SequencedInputIterator SeqIt = this->SequencedInputs.startsWith( this->CurrSequence );
                    if( SeqIt != this->SequencedInputs.end() ) {
                        this->CurrSequence.pop_back();
                        this->SequenceTimer.Start();
                    }else{
                        this->CurrSequence.clear();
                    }
                }else{
                    this->CurrSequence.push_back( (*DeltaBegin) );
                    if( this->MaxSequenceSize == this->CurrSequence.size() ) {
                        this->CurrSequence.erase( this->CurrSequence.begin() );
                    }
                    MetaCode New = this->TestCurrentSequence();
                    if( MetaCode::NullCode != New ) {
                        Ret.push_back(New);
                    }
                }
                ++DeltaBegin;
            }
            return Ret;
        }
    }//Input
}//Mezzanine

#endif
