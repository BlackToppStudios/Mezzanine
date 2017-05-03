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
#ifndef _inputbuttondevice_cpp
#define _inputbuttondevice_cpp

#include "Input/buttondevice.h"

namespace Mezzanine
{
    namespace Input
    {
        /*ButtonDevice::ButtonDevice() :
            TransitioningIndexes(4)
            {  }

        MetaCodeContainer ButtonDevice::UpdateButtonTransitions()
        {
            MetaCodeContainer GeneratedCodes;
            for( Whole CurrIndex : this->TransitioningIndexes )
            {
                Input::ButtonState State = this->Buttons.at( CurrIndex );
                if( Input::BUTTON_PRESSING == State ) {
                    this->Buttons.at( CurrIndex ) = Input::BUTTON_DOWN;
                    Input::InputCode ButtonCode = static_cast<Input::InputCode>(this->GetFirstButtonCode() + CurrIndex);
                    GeneratedCodes.emplace_back(Input::BUTTON_DOWN,ButtonCode,this->GetDeviceIndex());
                }else if( Input::BUTTON_LIFTING == State ) {
                    this->Buttons.at( CurrIndex ) = Input::BUTTON_UP;
                }
            }
            this->TransitioningIndexes.clear();
            return GeneratedCodes;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Query Methods

        UInt16 ButtonDevice::GetNumButtons() const
            { return this->Buttons.size(); }

        Boole ButtonDevice::IsButtonPressed(const UInt16 Button) const
            { return Input::BUTTON_UP < this->GetButtonState(Button); }

        Boole ButtonDevice::IsButtonPressed(const Input::InputCode Button) const
            { return Input::BUTTON_UP < this->GetButtonState(Button); }

        Boole ButtonDevice::IsButtonPressing(const UInt16 Button) const
            { return Input::BUTTON_PRESSING == this->GetButtonState(Button); }

        Boole ButtonDevice::IsButtonPressing(const Input::InputCode Button) const
            { return Input::BUTTON_PRESSING == this->GetButtonState(Button); }

        Boole ButtonDevice::IsButtonLifting(const UInt16 Button) const
            { return Input::BUTTON_LIFTING == this->GetButtonState(Button); }

        Boole ButtonDevice::IsButtonLifting(const Input::InputCode Button) const
            { return Input::BUTTON_LIFTING == this->GetButtonState(Button); }

        Boole ButtonDevice::IsButtonTransitioning(const UInt16 Button) const
        {
            Input::ButtonState State = this->GetButtonState(Button);
            return (Input::BUTTON_LIFTING == State || Input::BUTTON_PRESSING == State);
        }

        Boole ButtonDevice::IsButtonTransitioning(const Input::ButtonState Button) const
        {
            Input::ButtonState State = this->GetButtonState(Button);
            return (Input::BUTTON_LIFTING == State || Input::BUTTON_PRESSING == State);
        }

        Boole ButtonDevice::CheckButtonState(const UInt16 Button, const Input::ButtonState State) const
            { return State == this->GetButtonState(Button); }

        Boole ButtonDevice::CheckButtonState(const Input::InputCode Button, const Input::ButtonState State) const
            { return State == this->GetButtonState(Button); }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        MetaCodeContainer ButtonDevice::_Update(ConstMetaCodeIterator DeltaBegin, ConstMetaCodeIterator DeltaEnd)
        {
            MetaCodeContainer Ret = std::move( this->UpdateButtonTransitions() );
            MetaCodeContainer Temp = std::move( this->UpdateImpl(DeltaBegin,DeltaEnd) );
            Ret.insert(Ret.begin(),Temp.begin(),Temp.end());
            return Ret;
        }//*/
    }//Input
}//Mezzanine

#endif
