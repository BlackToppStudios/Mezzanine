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
#ifndef _uilistbox_h
#define _uilistbox_h

#include "colourvalue.h"
#include "enumerations.h"
#include "uiwidget.h"

namespace phys
{
    namespace UI
    {
        class Rectangle;
        class Button;
        class Caption;
        class Scrollbar;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class ListBox
        /// @headerfile uilistbox.h
        /// @brief This is a widget for displaying a list of captions in a box.
        /// @details
        ///////////////////////////////////////
        class PHYS_LIB ListBox : public Widget
        {
            protected:
                Rectangle* BoxBack;
                /// @todo Third instance of needing to include the namespace in the declaration seemingly needlessly.
                UI::Scrollbar* VertScroll;
                Caption* Selected;
                Caption* HoveredCaption;
                std::vector<Caption*> Selections;
                std::vector<Caption*> VisibleSelections;
                bool AutoHideScroll;
                Real LastScrollValue;
                Real SelectionDist;
                Whole NumVisible;
                UI::TextHorizontalAlign THAlign;
                UI::TextVerticalAlign TVAlign;
                Whole TGlyph;
                Vector2 TSize;
                ColourValue TTColour;
                ColourValue TBGColour;
                ColourValue SelectColour;
                /// @brief Determines how many items can be displayed in the box at once.
                virtual void CalculateVisibleSelections();
                /// @brief Updates the list of Visible buttons and hides the rest.
                virtual void DrawList();
                /// @brief For use with widget update/automation.
                virtual void Update(bool Force = false);
            public:
                /// @brief Standard initialization constructor.
                /// @param name The name of the List Box.
                /// @param Position The position of the List Box.
                /// @param Size The size of the List Box.
                /// @param ScrollbarWidth The relative(to screensize) width of the vertical scrollbar.
                /// If a horizontal scrollbar is needed it'll use the equal value in pixels(to avoid the stretched look.
                /// @param ScrollbarStyle The style of the scrollbar you want for this List Box.  See Scrollbar
                /// class for more information.
                /// @param Layer The parent layer this List Box belongs to.
                ListBox(ConstString& name, Vector2 Position, Vector2 Size, Real ScrollbarWidth, UI::ScrollbarStyle ScrollStyle, UILayer* Layer);
                /// @brief Standard destructor.
                ~ListBox();
                /// @brief Sets the visibility of this List Box.
                /// @param visible Bool determining whether or not this List Box should be visible.
                virtual void SetVisible(bool visible);
                /// @brief Gets the visibility of this List Box.
                /// @return Returns a bool representing the visibility of this List Box.
                virtual bool IsVisible();
                /// @brief Forces this List Box to be shown.
                virtual void Show();
                /// @brief Forces this List Box to hide.
                virtual void Hide();
                /// @brief Checks to see if the current mouse position is over this List Box.
                /// @return Returns a bool value, true if the mouse is over this List Box, false if it's not.
                virtual bool CheckMouseHover();
                /// @brief Sets the desired size and glyph set provided to all captions created within this widget.
                /// @details This function needs to be called before adding any selections to this widget.
                /// @param Size The size for all captions in this widget.
                /// @param Glyph The Glyph set to be used for all captions created.  Glyphs are defined in your .gorilla file.
                virtual void SetBasicTemplateParameters(Vector2 Size, Whole Glyph);
                /// @brief Sets additional optional perameters to the creation of each caption.
                /// @details This function does not need to be called before creating captions, but if you plan to use this, it
                /// would be a good idea to call this function prior to any creation.
                /// @param BackgroundColour The colour to set for the captions background.
                /// @param HorAlign The horizontal text alignment.  This defaults to UI::Middle.
                /// @param VertAlign The Vertical text alignment.  This defaults to UI::Center.
                virtual void SetMoreTemplateParameters(ColourValue TextColour = ColourValue(0.0,0.0,0.0,1.0),
                                                       ColourValue BackgroundColour = ColourValue(1.0,1.0,1.0,1.0),
                                                       UI::TextHorizontalAlign HorAlign = UI::Txt_Middle,
                                                       UI::TextVerticalAlign VertAlign = UI::Txt_Center);
                /// @brief Adds a selectable caption to the list to be displayed.
                /// @details If a colour other then white was set as the template and you try to set a background sprite, it will
                /// attempt to blend the colour and sprite.  Pure white colour will cause the sprite to look normal.
                /// @param TextLabel The text to be displayed within the created caption.
                /// @param BackgroundSprite Optional, name of the sprite to set as it's background.  Ignored if the string is empty.
                virtual Caption* AddSelection(ConstString& name, ConstString &Text, ConstString& BackgroundSprite = "");
                /// @brief Gets a caption by name.
                /// @param Name The name of the selectable caption you want to get.
                /// @return Returns a pointer to the named caption.
                virtual Caption* GetSelection(ConstString &Name);
                /// @brief Destroys a selectable caption.
                /// @param ToBeDestroyed A pointer to the caption you want destroyed and removed from this list.
                virtual void DestroySelection(Caption* ToBeDestroyed);
                /// @brief Destroys a selectable caption.
                /// @param ToBeDestroyed A string naming the caption you want destroyed and removed from this list.
                virtual void DestroySelection(String& ToBeDestroyed);
                /// @brief Sets the distance apart(and from the sides of box) the Selections will be from each other.
                /// @details This function expects a relative value to the screen size(0.0 to 1.0).  This value defaults to 0.
                /// @param Dist A relative value for the distance to be used when determining the positions of Selections.
                virtual void SetSelectionDistance(Real Dist);
                /// @brief Eanbles or disables the scrollbar autohide.
                /// @param AutoHide A bool indicating whether or not to auto hide the scrollbar.
                virtual void SetAutoHideScroll(bool AutoHide);
                /// @brief Enables the setting of a background colour or sprite on the caption you select.
                /// @details The background colour of captions default to white.
                /// @param Colour The colour of the border.
                virtual void EnableBackgroundSelector(ColourValue &Colour);
                /// @brief Disables borders on currently selected buttons if one was enabled.
                virtual void DisableBackgroundSelector();
                /// @brief Sets the relative position of this List Box.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this List Box.
                virtual void SetPosition(Vector2 Position);
                /// @brief Gets the relative position of this List Box.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative position of this List Box.
                virtual Vector2 GetPosition();
                /// @brief Sets the pixel position of this List Box.
                /// @param Position A vector2 representing the pixel position of this List Box.
                virtual void SetActualPosition(Vector2 Position);
                /// @brief Sets the pixel position of this List Box.
                /// @return Returns a vector2 representing the pixel position of this List Box.
                virtual Vector2 GetActualPosition();
                /// @brief Sets the relative size of this List Box.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Size A vector2 representing the relative size of this List Box.
                virtual void SetSize(Vector2 Size);
                /// @brief Gets the relative size of this List Box.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative size of this List Box.
                virtual Vector2 GetSize();
                /// @brief Sets the pixel size of this List Box.
                /// @param Size A vector2 representing the pixel size of this List Box.
                virtual void SetActualSize(Vector2 Size);
                /// @brief Sets the pixel size of this List Box.
                /// @return Returns a vector2 representing the pixel size of this List Box.
                virtual Vector2 GetActualSize();
                /// @brief Gets the currently selected caption.
                /// @return Returns a pointer to the currently selected caption, or NULL if none are selected.
                virtual Caption* GetSelected();
                /// @brief Gets the background of this List Box.
                /// @return Returns a pointer to the background of this List Box.
                virtual Rectangle* GetBoxBack();
                /// @brief Gets the scrollbar used within this List Box.
                /// @return Returns a pointer to the scrollbar of this List Box.
                virtual UI::Scrollbar* GetVertScroll();
        };
    }//UI
}//phys

#endif
