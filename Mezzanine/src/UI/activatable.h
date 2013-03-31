//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _uiactivatable_h
#define _uiactivatable_h

#include "Input/metacode.h"
#include "UI/widget.h"

namespace Mezzanine
{
    namespace UI
    {
        class ActivatableListener;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Activatable
        /// @headerfile activatable.h
        /// @brief This is a helper class that encapsulates the functionality of an object that can be activated to take some effect.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Activatable : public Widget
        {
            public:
                typedef std::vector<ActivatableListener*> ListenerContainer;
                typedef ListenerContainer::iterator ListenerIterator;
                typedef ListenerContainer::const_iterator ConstListenerIterator;
            protected:
                bool Activated;
                bool MultipleActivations;
                UI::ActivationCondition ActCond;
                ListenerContainer Listeners;
                std::vector<Input::InputCode> KeyboardActivationKeys;
                std::vector<Input::InputCode> MouseActivationButtons;
            //public:
                /// @brief Class constructor.
                /// @param Parent The parent Screen that created this widget.
                /// @param name The Name for the Widget.
                Activatable(const String& name, Screen* Parent);
                /// @brief Class destructor.
                virtual ~Activatable();
            public:
                ///////////////////////////////////////////////////////////////////////////////
                // Activation Methods

                /// @brief Sets the condition for activation for this activatable.
                /// @details See the ActivationCondition enum for more details.
                /// @param Condition The condition to be set.
                virtual void SetActivationCondition(const UI::ActivationCondition& Condition);
                /// @brief Gets the condition for activation for this activatable.
                /// @details See the ActivationCondition enum for more details.
                /// @return Returns an enum value indicating the condition for activation for this activatable.
                virtual const UI::ActivationCondition& GetActivationCondition() const;
                /// @brief Gets whether or not this activatable activates on press.
                /// @return Returns true if this activates on press, false otherwise.
                virtual bool IsPressActivation() const;
                /// @brief Gets whether or not this activatable activates on lift.
                /// @return Returns true if this activates on lift, false otherwise.
                virtual bool IsLiftActivation() const;
                /// @brief Gets whether or not this activatable is currently activated.
                /// @details Button activations are cleared every frame by the UI manager.
                /// @return Returns whether or not this activatable has been activated.
                virtual bool IsActivated() const;
                /// @brief Enables or disables whether or not the activatable should be allowed to activate multiple times per frame.
                /// @details In most cases a activatable will only activate multiple times when using hotkeys, either when there are multiple
                /// keys hotkeyed to the same activatable, or when the mouse activatable is pressed over a UI activatable while a hotkey for it is activated
                /// in the same frame, or both.  If you only want a UI element being triggered once per frame at most, you want this disabled. @n
                /// Default: false.
                /// @param Enable Whether or not to enable this feature.
                virtual void EnableMultipleActivations(bool Enable);
                /// @brief Gets whether or not multiple activations per frame are enabled for this activatable.
                /// @return Returns a bool indicating whether or not multiple activations are allowed for this activatable.
                virtual bool IsMultipleActivationsEnabled() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Binding Methods

                /// @brief Registers a keyboard key or mouse activatable that can activate this activatable.
                /// @details In the case of a mouse activatable, the hover check has to return true to activate the activatable.
                /// @param Code The input code to register that will trigger activation.
                virtual void BindActivationKeyOrButton(const Input::InputCode& Code);
                /// @brief Removes a previously registered activation key or activatable.
                /// @param Code The input code to remove.
                virtual void UnbindActivationKeyOrButton(const Input::InputCode& Code);
                /// @brief Clears all keyboard input codes from the list of activation codes.
                virtual void UnbindAllKeyboardActivationKeys();
                /// @brief Clears all mouse input codes from the list of activation codes.
                virtual void UnbindAllMouseActivationButtons();
                /// @brief Clears all keyboard and mouse input codes from the list of activators.
                virtual void UnbindAllActivationKeysAndButtons();

                ///////////////////////////////////////////////////////////////////////////////
                // Listener Methods

                /// @brief Sets the listener to be used by this activatable.
                /// @param Listener The listener to be set for this activatable.
                virtual void AddActivatableListener(ActivatableListener* Listener);
                /// @brief Removes a listener currently being used by this activatable.
                /// @param Listener The listener to be removed from this activatable.
                virtual void RemoveActivatableListener(ActivatableListener* Listener);

                ///////////////////////////////////////////////////////////////////////////////
                // Fetch Methods

                /// @brief Gets a vector with all the keyboard input codes used to activate this activatable.
                /// @return Returns a pointer to an std::vector containing all the keyboard keys that will activate this activatable.
                virtual const std::vector<Input::InputCode>& GetKeyboardActivationKeys() const;
                /// @brief Gets a vector with all the mouse input codes used to activate this activatable.
                /// @return Returns a pointer to an std::vector containing all the mouse buttons that will activate this activatable.
                virtual const std::vector<Input::InputCode>& GetMouseActivationButtons() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @internal
                /// @brief Sets the activatable as activated, also calling any set callbacks.
                /// @param Activate The state of activation to be applied.
                virtual void _SetActivation(bool Activate);
        };//Activatable

        ///////////////////////////////////////////////////////////////////////////////
        /// @class ActivatableListener
        /// @headerfile uibutton.h
        /// @brief This class provides customizable functionality to the activatable class.
        /// @details This is a pure virtual class that must be inherited from for use with specialized
        /// behaviors when working with activatables.
        ///////////////////////////////////////
        class MEZZ_LIB ActivatableListener : public WidgetListener
        {
            protected:
                Activatable* Caller;
            public:
                /// @brief Class constructor.
                ActivatableListener();
                /// @brief Class Destructor.
                virtual ~ActivatableListener();
                /// @brief The activation function for this callback.  This will be called every time the
                /// activatable is activated by the mouse or keyboard.
                virtual void DoActivateItems() = 0;
        };//buttoncallback
    }//UI
}//Mezzanine

#endif
