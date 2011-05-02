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
#ifndef _uitextbutton_h
#define _uitextbutton_h

#include "crossplatformexport.h"
#include "uibutton.h"

namespace phys
{
    namespace UI
    {
        class Layer;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class TextButton
        /// @headerfile uitextbutton.h
        /// @brief This is a button class that provides text capabilities.
        /// @details
        ///////////////////////////////////////
        class PHYS_LIB TextButton : public Button
        {
            protected:
                Gorilla::Caption* GorillaButton;
            public:
                /// @brief Internal constructor
                /// @param name The name of the button.
                /// @param Position The top left position of the button.
                /// @param Size The size of the Button.
                /// @param Glyph One of the glyphs specified in your gorilla file.  Must be valid.
                /// @param Text Any text you want printed on the button.
                /// @param Layer Pointer to the Layer that created this button.
                TextButton(ConstString& name, const Vector2 Position, const Vector2 Size, const Whole Glyph, String Text, Layer* PLayer);
                /// @brief Class destructor.
                ~TextButton();
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
                /// @brief Gets whether this is a text button.
                /// @return Returns a bool representing whether or not this is a text button.
                virtual bool IsTextButton();
                /// @brief Sets the text displayed within the button.
                /// @param Text The text to be displayed.
                virtual void SetText(ConstString& Text);
                /// @brief Gets the text displayed within the button.
                /// @return Returns the text being displayed.
                virtual String GetText();
                /// @brief Aligns the text of the button.
                /// @details Default value for this is UI::Txt_Middle.
                /// @param Align The enum value representing the horizontal alignment to be set.
                virtual void HorizontallyAlign(UI::TextHorizontalAlign Align);
                /// @brief Aligns the text of the button.
                /// @details Default value for this is UI::Txt_Center.
                /// @param Align The enum value representing the vertical alignment to be set.
                virtual void VerticallyAlign(UI::TextVerticalAlign Align);
                /// @brief Sets the relative top left position of this button.
                /// @param Position A Vector2 representing the location of this button.
                virtual void SetPosition(const Vector2 Position);
                /// @brief Gets the relative top left position of this button.
                /// @return Returns a Vector2 representing the location of this button.
                virtual Vector2 GetPosition();
                /// @brief Sets the top left position of this button in pixels.
                /// @param Position A Vector2 representing the location of this button.
                virtual void SetActualPosition(const Vector2 Position);
                /// @brief Gets the top left position of this button in pixels.
                /// @return Returns a Vector2 representing the location of this button.
                virtual Vector2 GetActualPosition();
                /// @brief Sets the relative size of this button.
                /// @param Size A vector2 representing the size of this button.
                virtual void SetSize(const Vector2 Size);
                /// @brief Gets the relative size of this button.
                /// @return Returns a vector2 representing the size of this button.
                virtual Vector2 GetSize();
                /// @brief Sets the size of this button in pixels.
                /// @param Size A vector2 representing the size of this button.
                virtual void SetActualSize(const Vector2 Size);
                /// @brief Gets the size of this button in pixels.
                /// @return Returns a vector2 representing the size of this button.
                virtual Vector2 GetActualSize();
                /// @brief Sets the priority this button should be rendered with.
                /// @details The default value for this is Medium.
                /// @param Priority The priority level to be used when rendering this button.
                virtual void SetRenderPriority(UI::RenderPriority Priority);
                /// @brief Gets the priority this button should be rendered with.
                /// @return Returns an enum value representing this button's priority level.
                virtual UI::RenderPriority GetRenderPriority();
        };
    }//UI
}//phys

#endif
