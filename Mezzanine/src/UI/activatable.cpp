//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _uiactivatable_cpp
#define _uiactivatable_cpp

#include "UI/activatable.h"
#include "exception.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Activatable Methods

        Activatable::Activatable(const String& name, Screen* Parent)
            : Widget(name,Parent),
              Activated(false),
              MultipleActivations(false),
              ActCond(UI::AC_OnLift)
        {
        }

        Activatable::~Activatable()
        {
            for( ListenerIterator It = Listeners.begin() ; It != Listeners.end() ; ++It )
            {
                delete (*It);
            }
            Listeners.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Activation Methods

        void Activatable::SetActivationCondition(const UI::ActivationCondition& Condition)
        {
            ActCond = Condition;
        }

        const UI::ActivationCondition& Activatable::GetActivationCondition() const
        {
            return ActCond;
        }

        bool Activatable::IsPressActivation() const
        {
            return UI::AC_OnPress == ActCond;
        }

        bool Activatable::IsLiftActivation() const
        {
            return UI::AC_OnLift == ActCond;
        }

        bool Activatable::IsActivated() const
        {
            return Activated;
        }

        void Activatable::EnableMultipleActivations(bool Enable)
        {
            MultipleActivations = Enable;
        }

        bool Activatable::IsMultipleActivationsEnabled() const
        {
            return MultipleActivations;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Binding Methods

        void Activatable::BindActivationKeyOrButton(const Input::InputCode& Code)
        {
            if(Input::KEY_FIRST < Code && Input::KEY_LAST > Code)
            {
                for( std::vector<Input::InputCode>::iterator It = KeyboardActivationKeys.begin() ; It != KeyboardActivationKeys.end() ; It++ )
                {
                    if((*It)==Code)
                        return;
                }
                KeyboardActivationKeys.push_back(Code);

            }else if(Input::MOUSE_FIRST < Code && Input::MOUSE_LAST > Code)
            {
                for( std::vector<Input::InputCode>::iterator It = MouseActivationButtons.begin() ; It != MouseActivationButtons.end() ; It++ )
                {
                    if((*It)==Code)
                        return;
                }
                MouseActivationButtons.push_back(Code);
            }else{
                /// @todo Throw an error?
            }
        }

        void Activatable::UnbindActivationKeyOrButton(const Input::InputCode& Code)
        {
            if(Input::KEY_FIRST < Code && Input::KEY_LAST > Code)
            {
                for( std::vector<Input::InputCode>::iterator It = KeyboardActivationKeys.begin() ; It != KeyboardActivationKeys.end() ; It++ )
                {
                    if((*It)==Code)
                    {
                        KeyboardActivationKeys.erase(It);
                        return;
                    }
                }

            }else if(Input::MOUSE_FIRST < Code && Input::MOUSE_LAST > Code)
            {
                for( std::vector<Input::InputCode>::iterator It = MouseActivationButtons.begin() ; It != MouseActivationButtons.end() ; It++ )
                {
                    if((*It)==Code)
                    {
                        MouseActivationButtons.erase(It);
                        return;
                    }
                }
            }
        }

        void Activatable::UnbindAllKeyboardActivationKeys()
        {
            KeyboardActivationKeys.clear();
        }

        void Activatable::UnbindAllMouseActivationButtons()
        {
            MouseActivationButtons.clear();
        }

        void Activatable::UnbindAllActivationKeysAndButtons()
        {
            UnbindAllKeyboardActivationKeys();
            UnbindAllMouseActivationButtons();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Listener Methods

        void Activatable::AddActivatableListener(ActivatableListener* Listener)
        {
            for( ListenerIterator It = Listeners.begin() ; It != Listeners.end() ; ++It )
            {
                if( (*It) == Listener )
                {
                    MEZZ_EXCEPTION(Exception::II_DUPLICATE_IDENTITY_EXCEPTION,"Listener being added to activatable is already registered.");
                }
            }
            Listeners.push_back(Listener);
            Listener->SetCaller(this);
        }

        void Activatable::RemoveActivatableListener(ActivatableListener* Listener)
        {
            for( ListenerIterator It = Listeners.begin() ; It != Listeners.end() ; ++It )
            {
                if( (*It) == Listener )
                {
                    Listeners.erase(It);
                    Listener->SetCaller(NULL);
                    return;
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Fetch Methods

        const std::vector<Input::InputCode>& Activatable::GetKeyboardActivationKeys() const
        {
            return KeyboardActivationKeys;
        }

        const std::vector<Input::InputCode>& Activatable::GetMouseActivationButtons() const
        {
            return MouseActivationButtons;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void Activatable::_SetActivation(bool Activate)
        {
            if( Activate && Activated && !MultipleActivations )
                return;
            if( Activate && Listeners.size() )
            {
                for( ListenerIterator It = Listeners.begin() ; It != Listeners.end() ; ++It )
                {
                    (*It)->DoActivateItems();
                }
            }
            Activated = Activate;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // ActivatableListener Methods

        ActivatableListener::ActivatableListener()
            : Caller(NULL)
        {
        }

        ActivatableListener::~ActivatableListener()
        {
        }
    }//UI
}//Mezzanine

#endif
