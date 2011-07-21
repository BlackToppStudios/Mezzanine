//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#include "crossplatformexport.h"
#include "datatypes.h"
#include "enumerations.h"
#include "metacode.h"
#include "uirenderablerect.h"

namespace Gorilla
{
    class Caption;
    class Layer;
    class Rectangle;
    class Sprite;
}

namespace phys
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
        class PHYS_LIB Button
        {
            protected:
                Gorilla::Rectangle* GorillaRectangle;
                Layer* Parent;
                UIManager* Manager;
                Gorilla::Sprite* NormalSprite;
                Gorilla::Sprite* HoveredSprite;
                Gorilla::Sprite* UserSprite;
                ButtonCallback* Callback;
                bool MouseHover;
                bool Activated;
                bool MultipleActivations;
                Vector2 RelPosition;
                Vector2 RelSize;
                String Name;
                std::vector<MetaCode::InputCode> KeyboardActivationKeys;
                std::vector<MetaCode::InputCode> MouseActivationButtons;
                UI::ActivationCondition ActCond;
                void SetHovered(bool Hovered);
            public:
                /// @brief Internal constructor
                /// @param name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                /// @param Layer Pointer to the Layer that created this button.
                Button(ConstString& name, const RenderableRect& Rect, Layer* PLayer);
                /// @brief Class destructor.
                virtual ~Button();
                /// @brief Sets the visibility of this button.
                /// @param Visible Bool determining whether or not this button should be visible.
                virtual void SetVisible(bool Visible);
                /// @brief Gets the visibility of this button.
                /// @return Returns a bool representing the visibility of this button.
                virtual bool IsVisible();
                /// @brief Forces this button to be shown.
                virtual void Show();
                /// @brief Forces this button to hide.
                virtual void Hide();
                /// @brief Gets the name of this button.
                /// @return Returns a string containing the name of this button.
                virtual ConstString& GetName();
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
                /// @brief Determines whether the mouse is over this button.
                /// @details While this can be called manually, it'll provide the same result if called more the once per frame.
                /// Currently the UIManager calls this on it's own once per frame, so there isn't much point in calling this manually.
                /// @return Returns a bool indicating whether the mouse is over this button.
                virtual bool CheckMouseHover();
                /// @brief Gets the stored value of whether or not the mouse is over the button.
                /// @details This function does not perform any checks.  If you want to do a manual check, call CheckMouseHover().
                /// @return Returns the stored value of whether or not the mouse is over the button.
                virtual bool GetMouseHover();
                /// @brief Sets the background colour of the button.
                /// @param Colour A colour value representing the colour to be set.
                virtual void SetBackgroundColour(const ColourValue& Colour);
                /// @brief Sets the background image(if provided in the atlas) of the button.
                /// @param Name The name of the sprite to set as the background.
                virtual void SetBackgroundSprite(const String& Name);
                /// @brief Sets the background image(if provided in the atlas) of the button from another atlas then the one currently set.
                /// @param Name The name of the sprite to set as the background.
                /// @param Atlas The Atlas to load the sprite from.
                virtual void SetBackgroundSprite(const String& Name, const String& Atlas);
                /// @brief Sets an alternate background image that will be applied when the mouse is over this button.
                /// @param Name The name of the sprite to set as the alternate background.
                virtual void SetHoveredSprite(const String& Name);
                /// @brief Sets an alternate background image that will be applied when the mouse is over this button from another atlas then the one currently set.
                /// @param Name The name of the sprite to set as the alternate background.
                /// @param Atlas The Atlas to load the sprite from.
                virtual void SetHoveredSprite(const String& Name, const String& Atlas);
                /// @brief Sets an alternate background image that is stored and can be quickly swapped with the active sprite.
                /// @param Name The name of the sprite to set as the alternate background.
                virtual void SetUserSprite(const String& Name);
                /// @brief Sets an alternate background image that is stored and can be quickly swapped with the active sprite from another atlas then the one currently set.
                /// @param Name The name of the sprite to set as the alternate background.
                /// @param Atlas The Atlas to load the sprite from.
                virtual void SetUserSprite(const String& Name, const String& Atlas);
                /// @brief Enables(or disables) the currently set User sprite.
                /// @param Enable If true, this will swap the current sprite with the user sprite, if false
                /// it will swap the User sprite for the normal sprite.
                virtual void EnableUserSprite(bool Enable);
                /// @brief Enables a border and sets it's colour.
                /// @param Colour A colour value representing the colour to be set.
                virtual void SetBorder(const Real& Width, const ColourValue& Colour);
                /// @brief Disables any border set on this rectangle if one is currently set.
                virtual void NoBorder();
                /// @brief Sets the relative top left position of this button.
                /// @param Position A Vector2 representing the location of this button.
                virtual void SetPosition(const Vector2& Position);
                /// @brief Gets the relative top left position of this button.
                /// @return Returns a Vector2 representing the location of this button.
                virtual Vector2 GetPosition();
                /// @brief Sets the top left position of this button in pixels.
                /// @param Position A Vector2 representing the location of this button.
                virtual void SetActualPosition(const Vector2& Position);
                /// @brief Gets the top left position of this button in pixels.
                /// @return Returns a Vector2 representing the location of this button.
                virtual Vector2 GetActualPosition();
                /// @brief Sets the relative size of this button.
                /// @param Size A vector2 representing the size of this button.
                virtual void SetSize(const Vector2& Size);
                /// @brief Gets the relative size of this button.
                /// @return Returns a vector2 representing the size of this button.
                virtual Vector2 GetSize();
                /// @brief Sets the size of this button in pixels.
                /// @param Size A vector2 representing the size of this button.
                virtual void SetActualSize(const Vector2& Size);
                /// @brief Gets the size of this button in pixels.
                /// @return Returns a vector2 representing the size of this button.
                virtual Vector2 GetActualSize();
                /// @brief Sets the priority this button should be rendered with.
                /// @details The default value for this is Medium.
                /// @param Priority The priority level to be used when rendering this button.
                virtual void SetRenderPriority(const UI::RenderPriority& Priority);
                /// @brief Gets the priority this button should be rendered with.
                /// @return Returns an enum value representing this button's priority level.
                virtual UI::RenderPriority GetRenderPriority();
                /// @brief Sets the Atlas to be assumed when one isn't provided for atlas related tasks.
                /// @param Atlas The name of the atlas to be used.
                virtual void SetPrimaryAtlas(const String& Atlas);
                /// @brief Gets the currently set primary atlas.
                /// @return Returns a string containing the name of the primary atlas that is set, or an empty string if none.
                virtual String GetPrimaryAtlas();
                /// @brief Gets a vector with all the keyboard input codes used to activate this button.
                /// @return Returns a pointer to an std::vector containing all the keyboard keys that will activate this button.
                virtual std::vector<MetaCode::InputCode>* GetKeyboardActivationKeys();
                /// @brief Gets a vector with all the mouse input codes used to activate this button.
                /// @return Returns a pointer to an std::vector containing all the mouse buttons that will activate this button.
                virtual std::vector<MetaCode::InputCode>* GetMouseActivationButtons();
                /// @brief Updates the dimensions of this button to match those of the new screen size.
                /// @details This function is called automatically when a viewport changes in size, and shouldn't need to be called manually.
                virtual void UpdateDimensions();
        };//button

        ///////////////////////////////////////////////////////////////////////////////
        /// @class ButtonCallback
        /// @headerfile uibutton.h
        /// @brief This class provides customizable functionality to the button class.
        /// @details This is a pure virtual class that must be inherited from for use with specialized
        /// behaviors when working with buttons.
        ///////////////////////////////////////
        class PHYS_LIB ButtonCallback
        {
            protected:
                Button* Caller;
            public:
                /// @brief Class constructor.
                /// @param CallerButton The button to which this callback belongs.
                ButtonCallback(Button* CallerButton);
                /// @brief Class Destructor.
                virtual ~ButtonCallback();
                /// @brief The hover function for this callback.  This will be called every time the
                /// button is hovered over by the mouse.
                virtual void DoHoverItems() = 0;
                /// @brief The activation function for this callback.  This will be called every time the
                /// button is activated by the mouse or keyboard.
                virtual void DoActivateItems() = 0;
        };//buttoncallback
    }//UI
}//phys

#endif
