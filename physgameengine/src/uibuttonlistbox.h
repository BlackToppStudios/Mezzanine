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
#ifndef _uibuttonlistbox_h
#define _uibuttonlistbox_h

#include "colourvalue.h"
#include "uiwidget.h"
#include "enumerations.h"

namespace phys
{
    namespace UI
    {
        class Rectangle;
        class Button;
        class Scrollbar;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class ButtonListBox
        /// @headerfile uibuttonlistbox.h
        /// @brief This is a widget for displaying a list of buttons in a box.
        /// @details
        ///////////////////////////////////////
        class PHYS_LIB ButtonListBox : public Widget
        {
            protected:
                Rectangle* BoxBack;
                /// @todo Third instance of needing to include the namespace in the declaration seemingly needlessly.
                UI::Scrollbar* VertScroll;
                Button* Selected;
                std::vector<Button*> Selections;
                std::vector<Button*> VisibleSelections;
                bool AutoHideScroll;
                Real LastScrollValue;
                Real SelectionDist;
                Real BorderWidth;
                Whole TGlyph;
                Whole NumVisible;
                Vector2 TSize;
                ColourValue BorderColour;
                /// @brief Determines how many items can be displayed in the box at once.
                virtual void CalculateVisibleSelections();
                /// @brief Updates the list of Visible buttons and hides the rest.
                virtual void DrawList();
                /// @brief For use with widget update/automation.
                virtual void Update(bool Force = false);
            public:
                /// @brief Standard initialization constructor.
                /// @param name The name of the Button List Box.
                /// @param Position The position of the Button List Box.
                /// @param Size The size of the Button List Box.
                /// @param ScrollbarWidth The relative(to screensize) width of the vertical scrollbar.
                /// If a horizontal scrollbar is needed it'll use the equal value in pixels(to avoid the stretched look.
                /// @param ScrollStyle The style of the scrollbar you want for this Button List Box.  See Scrollbar
                /// class for more information.
                /// @param Layer The parent layer this Button List Box belongs to.
                ButtonListBox(ConstString& name, Vector2 Position, Vector2 Size, Real ScrollbarWidth, UI::ScrollbarStyle ScrollStyle, UILayer* Layer);
                /// @brief Standard destructor.
                ~ButtonListBox();
                /// @brief Sets the visibility of this Button List Box.
                /// @param visible Bool determining whether or not this Button List Box should be visible.
                virtual void SetVisible(bool visible);
                /// @brief Gets the visibility of this Button List Box.
                /// @return Returns a bool representing the visibility of this Button List Box.
                virtual bool IsVisible();
                /// @brief Forces this Button List Box to be shown.
                virtual void Show();
                /// @brief Forces this Button List Box to hide.
                virtual void Hide();
                /// @brief Checks to see if the current mouse position is over this Button List Box.
                /// @return Returns a bool value, true if the mouse is over this Button List Box, false if it's not.
                virtual bool CheckMouseHover();
                /// @brief Sets the desired size and glyph set provided to all buttons created within this widget.
                /// @details This function needs to be called before adding any selections to this widget.
                /// @param Size The size for all buttons in this widget.
                /// @param Glyph The Glyph set to be used for any and all Text Buttons created.  Glyphs are defined in your .gorilla file.
                virtual void SetTemplateParameters(Vector2 Size, Whole Glyph);
                /// @brief Adds a selectable button to the list to be displayed.
                /// @param BackgroundSprite Optional, name of the sprite to set as it's background.  Ignored if the string is empty.
                /// @param TextLabel Optional, will create a text button instead of a regular button and set it's text.  Ignored if
                /// the string is empty.  @n @n Note: If a Text button is created, you can't revert it to a regular button and vice
                /// versa.  You'll have to destroy the selection and make a new one.  In the case of having a text button, you can
                /// however still set it's text to an empty string.
                /// @return Returns a pointer to the created button.
                virtual Button* AddSelection(ConstString& name, ConstString& BackgroundSprite = "", ConstString &TextLabel = "");
                /// @brief Gets a button by name.
                /// @param Name The name of the selectable button you want to get.
                /// @return Returns a pointer to the named button.
                virtual Button* GetSelection(ConstString &Name);
                /// @brief Destroys a selectable button.
                /// @param ToBeDestroyed A pointer to the button you want destroyed and removed from this list.
                virtual void DestroySelection(Button* ToBeDestroyed);
                /// @brief Destroys a selectable button.
                /// @param ToBeDestroyed A string naming the button you want destroyed and removed from this list.
                virtual void DestroySelection(String& ToBeDestroyed);
                /// @brief Sets the distance apart(and from the sides of box) the Selections will be from each other.
                /// @details This function expects a relative value to the screen size(0.0 to 1.0).  This value defaults to 0.025.
                /// @param Dist A relative value for the distance to be used when determining the positions of Selections.
                virtual void SetSelectionDistance(Real Dist);
                /// @brief Eanbles or disables the scrollbar autohide.
                /// @param AutoHide A bool indicating whether or not to auto hide the scrollbar.
                virtual void SetAutoHideScroll(bool AutoHide);
                /// @brief Enables the setting of a border on the button you select.
                /// @param Width The width of the border in pixels.
                /// @param Colour The colour of the border.
                virtual void EnableBorderSelector(Real Width, ColourValue &Colour);
                /// @brief Disables borders on currently selected buttons if one was enabled.
                virtual void DisableBorderSelector();
                /// @brief Sets the relative position of this Button List Box.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this Button List Box.
                virtual void SetPosition(Vector2 Position);
                /// @brief Gets the relative position of this Button List Box.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative position of this Button List Box.
                virtual Vector2 GetPosition();
                /// @brief Sets the pixel position of this Button List Box.
                /// @param Position A vector2 representing the pixel position of this Button List Box.
                virtual void SetActualPosition(Vector2 Position);
                /// @brief Sets the pixel position of this Button List Box.
                /// @return Returns a vector2 representing the pixel position of this Button List Box.
                virtual Vector2 GetActualPosition();
                /// @brief Sets the relative size of this Button List Box.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Size A vector2 representing the relative size of this Button List Box.
                virtual void SetSize(Vector2 Size);
                /// @brief Gets the relative size of this Button List Box.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative size of this Button List Box.
                virtual Vector2 GetSize();
                /// @brief Sets the pixel size of this Button List Box.
                /// @param Size A vector2 representing the pixel size of this Button List Box.
                virtual void SetActualSize(Vector2 Size);
                /// @brief Sets the pixel size of this Button List Box.
                /// @return Returns a vector2 representing the pixel size of this Button List Box.
                virtual Vector2 GetActualSize();
                /// @brief Gets the currently selected button.
                /// @return Returns a pointer to the currently selected button, or NULL if none are selected.
                virtual Button* GetSelected();
                /// @brief Gets the background of this Button List Box.
                /// @return Returns a pointer to the background of this Button List Box.
                virtual Rectangle* GetBoxBack();
                /// @brief Gets the scrollbar used within this Button List Box.
                /// @return Returns a pointer to the scrollbar of this Button List Box.
                virtual UI::Scrollbar* GetVertScroll();
        };
    }//UI
}//phys

#endif
