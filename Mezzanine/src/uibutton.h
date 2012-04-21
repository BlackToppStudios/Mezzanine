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
#ifndef _uibutton_h
#define _uibutton_h

#include "colourvalue.h"
#include "uienumerations.h"
#include "metacode.h"
#include "uirenderablerect.h"
#include "uirectangle.h"

namespace Mezzanine
{
    class UIManager;
    namespace UI
    {
        class Layer;
        class ButtonCallback;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Button
        /// @headerfile uibutton.h
        /// @brief This class is a helper class, specifically for use as a button.
        /// @details Unlike rectangles and captions, this class can be interacted with by clicking.
        /// It is important to understand what you want your space to do when selecting the class to use.
        ///////////////////////////////////////
        class MEZZ_LIB Button : public Rectangle
        {
            protected:
                friend class UIManager;
                Sprite* NormalSprite;
                Sprite* HoveredSprite;
                Sprite* UserSprite;
                ButtonCallback* Callback;
                bool UserSpriteEnabled;
                bool Activated;
                bool MultipleActivations;
                UI::ActivationCondition ActCond;
                std::vector<MetaCode::InputCode> KeyboardActivationKeys;
                std::vector<MetaCode::InputCode> MouseActivationButtons;
                virtual void SetHovered(bool Hovered);
            public:
                /// @brief Internal constructor
                /// @param name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                /// @param Layer Pointer to the Layer that created this button.
                Button(ConstString& name, const RenderableRect& Rect, Layer* PLayer);
                /// @brief Class destructor.
                virtual ~Button();

                /// @brief Gets whether this is a text button.
                /// @return Returns a bool representing whether or not this is a text button.
                virtual bool IsTextButton();

                /// @brief Sets the callback for this button.  See the ButtonCallback class for more info.
                /// @details You can pass in a null pointer to disable a callback.
                /// @param Call A pointer to the callback you wish to have set for this button.
                virtual void SetButtonCallback(ButtonCallback* Call);
                /// @brief Registers a keyboard key or mouse button that can activate this button.
                /// @details In the case of a mouse button, the hover check has to return true to activate the button.
                /// @param Code The input code to register that will trigger activation.
                virtual void BindActivationKeyOrButton(const MetaCode::InputCode& Code);
                /// @brief Removes a previously registered activation key or button.
                /// @param Code The input code to remove.
                virtual void UnbindActivationKeyOrButton(const MetaCode::InputCode& Code);
                /// @brief Clears all keyboard input codes from the list of activation keys.
                virtual void UnbindAllKeyboardActivationKeys();
                /// @brief Clears all mouse input codes from the list of activation buttons.
                virtual void UnbindAllMouseActivationButtons();
                /// @brief Clears all keyboard and mouse input codes from the list of activators.
                virtual void UnbindAllActivationKeysAndButtons();
                /// @brief Sets the condition for activation for this button.
                /// @details See the ActivationCondition enum for more details.
                /// @param Condition The condition to be set.
                virtual void SetActivationCondition(const UI::ActivationCondition& Condition);
                /// @brief Gets the condition for activation for this button.
                /// @details See the ActivationCondition enum for more details.
                /// @return Returns an enum value indicating the condition for activation for this button.
                virtual UI::ActivationCondition GetActivationCondition();
                /// @brief Sets the button as activated, also calling any set callbacks.
                /// @details This shouldn't be called on manually unless you know exactly what you are doing.
                /// @param Activate The state of activation to be applied.
                virtual void SetActivation(bool Activate);
                /// @brief Gets whether or not this button is currently activated.
                /// @details Button activations are cleared every frame by the UI manager.
                /// @return Returns whether or not this button has been activated.
                virtual bool IsActivated();
                /// @brief Enables or disables whether or not the button should be allowed to activate multiple times per frame.
                /// @details In most cases a button will only activate multiple times when using hotkeys, either when there are multiple
                /// keys hotkeyed to the same button, or when the mouse button is pressed over a UI button while a hotkey for it is activated
                /// in the same frame, or both.  If you only want a UI element being triggered once per frame at most, you want this disabled. @n
                /// Default: false.
                /// @param Enable Whether or not to enable this feature.
                virtual void EnableMultipleActivations(bool Enable);
                /// @brief Gets whether or not multiple activations per frame are enabled for this button.
                /// @return Returns a bool indicating whether or not multiple activations are allowed for this button.
                virtual bool IsMultipleActivationsEnabled();

                /// @brief Sets the background image(if provided in the atlas) of the renderable.
                /// @param PSprite A pointer to the sprite to set as the background.
                virtual void SetBackgroundSprite(Sprite* PSprite);
                /// @brief Sets the background image(if provided in the atlas) of the renderable.
                /// @param SpriteName The name of the sprite to set as the background.
                virtual void SetBackgroundSprite(const String& SpriteName);
                /// @brief Sets the background image(if provided in the atlas) of the renderable.
                /// @param SpriteName The name of the sprite to set as the background.
                /// @param Atlas The Atlas to load the sprite from.
                virtual void SetBackgroundSprite(const String& SpriteName, const String& Atlas);
                /// @brief Sets an alternate background image that will be applied when the mouse is over this button.
                /// @param PSprite A pointer to the sprite to set as the alternate background.
                virtual void SetHoveredSprite(Sprite* PSprite);
                /// @brief Sets an alternate background image that will be applied when the mouse is over this button.
                /// @param SpriteName The name of the sprite to set as the alternate background.
                virtual void SetHoveredSprite(const String& SpriteName);
                /// @brief Sets an alternate background image that will be applied when the mouse is over this button.
                /// @param SpriteName The name of the sprite to set as the alternate background.
                /// @param Atlas The Atlas to load the sprite from.
                virtual void SetHoveredSprite(const String& SpriteName, const String& Atlas);
                /// @brief Sets an alternate background image that is stored and can be quickly swapped with the active sprite from another atlas then the one currently set.
                /// @param PSprite A pointer to the sprite to set as the alternate background.
                virtual void SetUserSprite(Sprite* PSprite);
                /// @brief Sets an alternate background image that is stored and can be quickly swapped with the active sprite from another atlas then the one currently set.
                /// @param SpriteName The name of the sprite to set as the alternate background.
                virtual void SetUserSprite(const String& SpriteName);
                /// @brief Sets an alternate background image that is stored and can be quickly swapped with the active sprite from another atlas then the one currently set.
                /// @param SpriteName The name of the sprite to set as the alternate background.
                /// @param Atlas The Atlas to load the sprite from.
                virtual void SetUserSprite(const String& SpriteName, const String& Atlas);
                /// @brief Enables(or disables) the currently set User sprite.
                /// @param Enable If true, this will swap the current sprite with the user sprite, if false
                /// it will swap the User sprite for the normal sprite.
                virtual void EnableUserSprite(bool Enable);

                /// @brief Gets a vector with all the keyboard input codes used to activate this button.
                /// @return Returns a pointer to an std::vector containing all the keyboard keys that will activate this button.
                virtual std::vector<MetaCode::InputCode>* GetKeyboardActivationKeys();
                /// @brief Gets a vector with all the mouse input codes used to activate this button.
                /// @return Returns a pointer to an std::vector containing all the mouse buttons that will activate this button.
                virtual std::vector<MetaCode::InputCode>* GetMouseActivationButtons();
        };//button

        ///////////////////////////////////////////////////////////////////////////////
        /// @class ButtonCallback
        /// @headerfile uibutton.h
        /// @brief This class provides customizable functionality to the button class.
        /// @details This is a pure virtual class that must be inherited from for use with specialized
        /// behaviors when working with buttons.
        ///////////////////////////////////////
        class MEZZ_LIB ButtonCallback
        {
            protected:
                Button* Caller;
            public:
                /// @brief Class constructor.
                /// @param CallerButton The button to which this callback belongs.
                ButtonCallback();
                /// @brief Class Destructor.
                virtual ~ButtonCallback();
                /// @brief Sets the button this callback belongs to.
                virtual void SetCaller(Button* Caller);
                /// @brief The hover function for this callback.  This will be called every time the
                /// button is hovered over by the mouse.
                virtual void DoHoverItems() = 0;
                /// @brief The activation function for this callback.  This will be called every time the
                /// button is activated by the mouse or keyboard.
                virtual void DoActivateItems() = 0;
        };//buttoncallback
    }//UI
}//Mezzanine

#endif
