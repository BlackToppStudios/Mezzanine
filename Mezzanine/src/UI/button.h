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
#ifndef _uibutton_h
#define _uibutton_h

#include "colourvalue.h"
#include "timer.h"
#include "Input/metacodekey.h"
#include "UI/uienumerations.h"
#include "UI/widget.h"
#include "UI/widgetfactory.h"

namespace Mezzanine
{
    class UIManager;
    namespace UI
    {
        class Screen;
        class ButtonCallback;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Button
        /// @headerfile button.h
        /// @brief This class is a helper class, specifically for use as a button.
        /// @details Unlike rectangles and captions, this class can be interacted with by clicking.
        /// It is important to understand what you want your space to do when selecting the class to use.
        ///////////////////////////////////////
        class MEZZ_LIB Button : public Widget
        {
        public:
            /// @brief Basic container type for Activation Code storage by this class.
            typedef std::set< Input::MetaCodeKey >           ActivationCodeContainer;
            /// @brief Iterator type for Activation Codes stored by this class.
            typedef ActivationCodeContainer::iterator        ActivationCodeIterator;
            /// @brief Const Iterator type for Activation Codes stored by this class.
            typedef ActivationCodeContainer::const_iterator  ConstActivationCodeIterator;

            /// @enum ActivationState
            /// @brief This enum describes the different possible states for the activation of a button.
            /// @details The first two states should be self explanitory.  Button is made but not interacted with, it's deactivated.
            /// If the button is pressed it becomes activated for the duration of the press.  The third state is a special state that
            /// occurs when one of the criteria for being activated is removed in a way that can promptly return without changing the
            /// other criteria.  Specifically this can happen if a mouse click occurs over the button and is held like that while moving
            /// the mouse cursor around.  If it is moved such that it is no longer hovering over the button, it will go in standby.  The
            /// normal activation will be restored once the cursor is hovered over the button again.
            enum ActivationState
            {
                AS_Deactivated        = 0,    ///< The normal default state.
                AS_Activated          = 1,    ///< This button has been activated via an input device.
                AS_Activation_Standby = 2     ///< This button was activated, but an event occured that made it suspend it's activation.
            };

            /// @brief String containing the type name for this class: "Button".
            static const String TypeName;
            /// @brief Event name for when this activatable widget is activated.
            static const String EventActivated;
            /// @brief Event name for when this activatable widget is put into standby.
            static const String EventStandby;
            /// @brief Event name for when this activatable widget is deactivated.
            static const String EventDeactivated;
        protected:
            friend class ButtonFactory;
            /// @internal
            /// @brief The timer that will be used when a button is locked from activating a second time within a certain period.
            StopWatchTimer LockoutTimer;
            /// @internal
            /// @brief A container of codes that stores the inputs that will trigger this button to be activated.
            ActivationCodeContainer ActivationCodes;
            /// @internal
            /// @brief Stores the current state of this button's activation.  See Button::ActivationState enum for more details.
            ActivationState Activation;
            /// @internal
            /// @brief Stores whether or not the current activation of this button was triggered by a mouse.
            Boolean MouseActivated;

            /// @copydoc Widget::HandleInputImpl(const Input::MetaCode& Code)
            virtual Boolean HandleInputImpl(const Input::MetaCode& Code);
            /// @internal
            /// @brief Contains all the common necessary startup initializations for this class.
            virtual void ConstructButton();
            /// @internal
            /// @brief Verifies the provided to code is valid for this button.
            /// @param Code The code to check.
            /// @return Returns true if this code is valid, false otherwise.
            virtual Boolean VertifyActivationCode(const Input::InputCode Code);
            /// @internal
            /// @brief Attempts to activate this button.
            /// @return Returns true if this button was successfully activated.
            virtual Boolean Activate();
            /// @internal
            /// @brief Attempts to deactivate this button.
            /// @return Returns true if this button was successfully deactivated.
            virtual Boolean Deactivate();
            /// @internal
            /// @brief Attempts to put this button into standby.
            /// @return Returns true if this button was successfully put into standby.
            virtual Boolean Standby();
        //public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            Button(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param Parent The parent Screen that created this widget.
            Button(const String& RendName, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Parent The parent screen that created this renderable.
            Button(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created Button will belong to.
            Button(const XML::Node& XMLNode, Screen* Parent);
            /// @brief Standard destructor.
            virtual ~Button();
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Sets a timer preventing multiple activations for a period of time.
            /// @param Milliseconds The amount of time to lock out additional activations, in milliseconds.
            void SetLockoutTime(const UInt32& Milliseconds);
            /// @brief Gets this activatables lockout timer.
            /// @return Returns a pointer to the Lockout timer for this button, or NULL if one hasn't been set.
            const StopWatchTimer& GetLockoutTimer() const;

            /// @brief Gets whether or not this button can be activated again.
            /// @return Returns true if this button is not ready to be activated again.
            Boolean IsActivationLocked() const;
            /// @brief Gets whether or not this button is currently activated.
            /// @return Returns true if this button is currently activated, false otherwise.
            Boolean IsActivated() const;
            /// @brief Gets whether or not this button is currently on standby.
            /// @return Returns true if this button is currently on activation standby, false otherwise.
            Boolean IsOnStandby() const;
            /// @brief Gets whether or not this button is currently deactivated.
            /// @return Retruns true if this button is currently deactivated, false otherwise.
            Boolean IsDeactivated() const;

            /// @copydoc Widget::GetTypeName() const
            virtual const String& GetTypeName() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Binding Methods

            /// @brief Registers a keyboard key or mouse button that can activate this button.
            /// @details In the case of a mouse button, the hover check has to return true to activate the button.
            /// @param Code The input code to register that will trigger activation.
            virtual void BindActivationKeyOrButton(const Input::MetaCode& Code);
            /// @brief Removes a previously registered activation code.
            /// @param Code The input code to remove.
            virtual void UnbindActivationKeyOrButton(const Input::MetaCode& Code);
            /// @brief Clears all keyboard input codes from the set of activation codes.
            virtual void UnbindAllKeyboardActivationKeys();
            /// @brief Clears all mouse input codes from the set of activation codes.
            virtual void UnbindAllMouseActivationButtons();
            /// @brief Clears all controller input codes from the set of activation codes.
            virtual void UnbindAllControllerActivationButtons();
            /// @brief Clears all keyboard and mouse input codes from the list of activators.
            virtual void UnbindAllActivationKeysAndButtons();

            ///////////////////////////////////////////////////////////////////////////////
            // Fetch Methods

            /// @brief Gets a set with all the activation codes used to activate this button.
            /// @return Returns a pointer to an std::set containing all the activation codes that will activate this button.
            const ActivationCodeContainer& GetActivationCodes() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc Renderable::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @copydoc Renderable::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc Renderable::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Event Methods

            /// @copydoc Widget::_OnMouseEnter()
            virtual void _OnMouseEnter();
            /// @copydoc Widget::_OnMouseExit()
            virtual void _OnMouseExit();
            /// @brief Self logic to be executed when this button is activated.
            virtual void _OnActivate();
            /// @brief Self logic to be executed when this button is put into standby.
            virtual void _OnStandby();
            /// @brief Self logic to be executed when this button is deactivated.
            virtual void _OnDeactivate();
        };//Button

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the factory implementation for Button widgets.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB ButtonFactory : public WidgetFactory
        {
        public:
            /// @brief Class constructor.
            ButtonFactory() {  }
            /// @brief Class destructor.
            virtual ~ButtonFactory() {  }

            /// @copydoc WidgetFactory::GetWidgetTypeName() const
            virtual String GetWidgetTypeName() const;

            /// @brief Creates a new Button.
            /// @param RendName The name to be given to the created Button.
            /// @param Parent The screen the created Button will belong to.
            /// @return Returns a pointer to the created Button.
            virtual Button* CreateButton(const String& RendName, Screen* Parent);
            /// @brief Creates a new Button.
            /// @param RendName The name to be given to the created Button.
            /// @param RendRect The dimensions that will be assigned to the created Button.
            /// @param Parent The screen the created Button will belong to.
            /// @return Returns a pointer to the created Button.
            virtual Button* CreateButton(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief Creates a new Button.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created Button will belong to.
            /// @return Returns a pointer to the created Button.
            virtual Button* CreateButton(const XML::Node& XMLNode, Screen* Parent);

            /// @copydoc WidgetFactory::CreateWidget(Screen*)
            virtual Widget* CreateWidget(Screen* Parent);
            /// @copydoc WidgetFactory::CreateWidget(const String&, const NameValuePairMap&, Screen*)
            virtual Widget* CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent);
            /// @copydoc WidgetFactory::CreateWidget(const String&, const UnifiedRect&, const NameValuePairMap&, Screen*)
            virtual Widget* CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent);
            /// @copydoc WidgetFactory::CreateWidget(const XML::Node&, Screen*)
            virtual Widget* CreateWidget(const XML::Node& XMLNode, Screen* Parent);
            /// @copydoc WidgetFactory::DestroyWidget(Widget*)
            virtual void DestroyWidget(Widget* ToBeDestroyed);
        };//ButtonFactory
    }//UI
}//Mezzanine

#endif
