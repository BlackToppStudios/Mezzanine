//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _uicheckbox_h
#define _uicheckbox_h

#include "uiwidget.h"
#include <utility>

namespace phys
{
    namespace UI
    {
        class Button;
        class MarkupText;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class CheckBox
        /// @headerfile uicheckbox.h
        /// @brief This is a simple widget for storing a bool value.
        /// @details
        ///////////////////////////////////////
        class CheckBox : public Widget
        {
            protected:
                friend class UIManager;
                Button* Box;
                MarkupText* Label;
                std::pair<String,String> UncheckedSet;
                std::pair<String,String> CheckedSet;
                Whole GlyphIndex;
                bool Checked;
                bool CheckLock;
                void SetSpriteSet(std::pair<String,String>& SpriteSet);
                virtual void Update(bool Force = false);
            public:
                /// @brief Class constructor
                /// @details The dimensions provided
                /// @param name The name of the button.
                /// @param Position The top left position of the button.
                /// @param Size The size of the Button.
                /// @param Layer Pointer to the Layer that created this button.
                CheckBox(String& name, Vector2 Position, Vector2 Size, Whole Glyph, String &LabelText, UILayer* Layer);
                /// @brief Class destructor.
                virtual ~CheckBox();
                /// @brief Gets whether this checkbox is checked or not.
                /// @return Returns a bool representing whether or not this checkbox is checked.
                virtual bool IsChecked();
                /// @brief Checks to see if the current mouse position is over this widget.
                /// @return Returns a bool value, true if the mouse is over this widget, false if it's not.
                virtual bool CheckMouseHover();
                /// @brief Sets the text label to be displayed with this widget.
                /// @details If a label hasn't been set when this is called, this funtion will create a
                /// new one and set it.
                /// @param Label The text message to display with this widget.
                virtual void SetLabelText(String &LabelText);
                /// @brief Gets the currently set label being displayed with this widget.
                /// @return Returns a string containing the currently set label.  If no label is set this will
                /// return an empty string.
                virtual String GetLabelText();
                /// @brief Sets the unchecked status sprite and an optional unchecked hovered sprite.
                /// @param Unchecked The name of the sprite in the Atlas you wish to set as the unchecked status sprite.
                /// @param Hovered The name of the sprite in the Atlas you with to sed on the unchecked/hovered sprite.
                /// Leaving this to default or passing in a blank string will cause it to ignore this parameter.
                virtual void SetUncheckedSprite(String& Unchecked, String& Hovered = "");
                /// @brief Sets the checked status sprite and an optional checked hovered sprite.
                /// @param Unchecked The name of the sprite in the Atlas you wish to set as the checked status sprite.
                /// @param Hovered The name of the sprite in the Atlas you with to sed on the checked/hovered sprite.
                /// Leaving this to default or passing in a blank string will cause it to ignore this parameter.
                virtual void SetCheckedSprite(String& Checked, String& Hovered = "");
                /// @brief Sets the relative position of this widget.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this widget.
                virtual void SetPosition(Vector2 Position);
                /// @brief Gets the relative position of this widget.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative position of this widget.
                virtual Vector2 GetPosition();
                /// @brief Sets the pixel position of this widget.
                /// @param Position A vector2 representing the pixel position of this widget.
                virtual void SetActualPosition(Vector2 Position);
                /// @brief Sets the pixel position of this widget.
                /// @return Returns a vector2 representing the pixel position of this widget.
                virtual Vector2 GetActualPosition();
                /// @brief Sets the relative size of this widget.
                /// @param Size A vector2 representing the relative size of this widget.
                virtual void SetSize(Vector2 Size);
                /// @brief Gets the relative size of this widget.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative size of this widget.
                virtual Vector2 GetSize();
                /// @brief Sets the pixel size of this widget.
                /// @param Size A vector2 representing the pixel size of this widget.
                virtual void SetActualSize(Vector2 Size);
                /// @brief Sets the pixel size of this widget.
                /// @return Returns a vector2 representing the pixel size of this widget.
                virtual Vector2 GetActualSize();
                /// @brief Gets the Button this checkbox is based on.
                /// @return Returns a pointer to the button this checkbox is based on.
                Button* GetCheckBoxButton();
                /// @brief Gets the label for this checkbox.
                /// @return Returns a pointer to the label for this checkbox.
                MarkupText* GetCheckBoxLabel();
        };//Checkbox
    }//UI
}//phys

#endif
