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
#ifndef _uicheckbox_h
#define _uicheckbox_h

#include "datatypes.h"
#include "UI/widget.h"

namespace Mezzanine
{
    namespace UI
    {
        class Button;
        class Caption;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class CheckBox
        /// @headerfile uicheckbox.h
        /// @brief This is a simple widget for storing a bool value.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB CheckBox : public Widget
        {
            protected:
                friend class UIManager;
                Button* Box;
                Caption* Label;
                std::pair<String,String> UncheckedSet;
                std::pair<String,String> CheckedSet;
                Whole GlyphIndex;
                bool Checked;
                bool CheckLock;
                /// @brief Sets both the background and hovered sprites of a set.
                void SetSpriteSet(std::pair<std::string,std::string>& SpriteSet);
                //void SetUncheckedSprites();
                //void SetCheckedSprites();
                /// @brief Child specific update method.
                virtual void UpdateImpl(bool Force = false);
                /// @brief Child specific visibility method.
                virtual void SetVisibleImpl(bool visible);
                /// @brief Child specific mouse hover method.
                virtual bool CheckMouseHoverImpl();
            public:
                /// @brief Class constructor
                /// @param name The name of the checkbox.
                /// @param Rect The Rect representing the position and size of the checkbox and label.  The checkbox itself will always be as wide as it is tall, with remaining space going to the label.
                /// @param LineHeight The lineheight you want the text to have. If the Rect passed in is relative, this will expect LineHeight to be relative as well.
                /// @param LabelText The text to put into the label.
                /// @param PLayer Pointer to the Layer that created this checkbox.
                CheckBox(ConstString& name, const RenderableRect& Rect, const Real& LineHeight, ConstString& LabelText, Layer* PLayer);
                /// @brief Class constructor
                /// @param name The name of the checkbox.
                /// @param Rect The Rect representing the position and size of the checkbox.
                /// @param Glyph The glyph index to use for the text in the label.
                /// @param LabelText The text to put into the label.
                /// @param PLayer Pointer to the Layer that created this checkbox.
                CheckBox(ConstString& name, const RenderableRect& Rect, const Whole& Glyph, ConstString& LabelText, Layer* PLayer);
                /// @brief Class destructor.
                virtual ~CheckBox();
                /// @brief Gets whether this checkbox is checked or not.
                /// @return Returns a bool representing whether or not this checkbox is checked.
                virtual bool IsChecked();
                /// @brief Manually check or uncheck this checkbox.
                /// @param Check The value to set the status of this checkbox.
                virtual void ManualCheck(bool Check);
                /// @brief Sets the text label to be displayed with this checkbox.
                /// @details If a label hasn't been set when this is called, this funtion will create a
                /// new one and set it.
                /// @param Label The text message to display with this checkbox.
                virtual void SetLabelText(String& LabelText);
                /// @brief Gets the currently set label being displayed with this checkbox.
                /// @return Returns a string containing the currently set label.  If no label is set this will
                /// return an empty string.
                virtual String GetLabelText();
                /// @brief Sets the unchecked status sprite and an optional unchecked hovered sprite.
                /// @param Unchecked The name of the sprite in the Atlas you wish to set as the unchecked status sprite.
                /// @param Hovered The name of the sprite in the Atlas you with to sed on the unchecked/hovered sprite.
                /// Leaving this to default or passing in a blank string will cause it to ignore this parameter.
                virtual void SetUncheckedSprite(ConstString& Unchecked, ConstString& Hovered = "");
                /// @brief Sets the checked status sprite and an optional checked hovered sprite.
                /// @param Unchecked The name of the sprite in the Atlas you wish to set as the checked status sprite.
                /// @param Hovered The name of the sprite in the Atlas you with to sed on the checked/hovered sprite.
                /// Leaving this to default or passing in a blank string will cause it to ignore this parameter.
                virtual void SetCheckedSprite(ConstString& Checked, ConstString& Hovered = "");
                /// @brief Sets the relative position of this checkbox.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this checkbox.
                virtual void SetPosition(const Vector2& Position);
                /// @brief Sets the pixel position of this checkbox.
                /// @param Position A vector2 representing the pixel position of this checkbox.
                virtual void SetActualPosition(const Vector2& Position);
                /// @brief Sets the relative size of this checkbox.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Size A vector2 representing the relative size of this checkbox.
                virtual void SetSize(const Vector2& Size);
                /// @brief Sets the pixel size of this checkbox.
                /// @param Size A vector2 representing the pixel size of this checkbox.
                virtual void SetActualSize(const Vector2& Size);
                /// @brief Updates the dimensions of this widget to match those of the new screen size.
                /// @details This function is called automatically when a viewport changes in size, and shouldn't need to be called manually.
                virtual void UpdateDimensions();
                /// @brief Gets the Button this checkbox is based on.
                /// @return Returns a pointer to the button this checkbox is based on.
                Button* GetCheckBoxButton();
                /// @brief Gets the label for this checkbox.
                /// @return Returns a pointer to the label for this checkbox.
                Caption* GetLabel();
        };//Checkbox
    }//UI
}//Mezzanine

#endif
