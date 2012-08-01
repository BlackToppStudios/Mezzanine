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
#ifndef _uilistbox_h
#define _uilistbox_h

#include "colourvalue.h"
#include "uienumerations.h"
#include "UI/widget.h"

namespace Mezzanine
{
    namespace UI
    {
        struct TemplateParams
        {
            UI::TextHorizontalAlign HorizontalAlign;
            UI::TextVerticalAlign VerticalAlign;
            UI::RenderPriority Priority;
            Whole GlyphIndex;
            Whole CursorOffset;
            Real TextScale;
            Vector2 Size;
            ColourValue TextColour;
            ColourValue BackgroundColour;
        };

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
        class MEZZ_LIB ListBox : public Widget
        {
            public:
               TemplateParams Params;
            protected:
                Rectangle* BoxBack;
                Scrollbar* VertScroll;
                Caption* Selected;
                Caption* HoveredCaption;
                std::vector<Caption*> Selections;
                std::vector<Caption*> VisibleSelections;
                bool AutoHideScroll;
                Real LastScrollValue;
                Whole MaxDisplay;
                Whole SelectionsAdded;
                UI::TemplateParams SelectionTemplate;
                /// @brief Checks to make sure the Scroller is the appropriate size.
                virtual void ScrollerSizeCheck();
                /// @brief Checks to see if the scrollbar should be hidden or not.
                virtual void ScrollHideCheck();
                /// @brief Checks to see if the current size of the selection needs adjusting.
                virtual void SelectionSizeCheck(UI::Caption* Selection);
                /// @brief Internal utility to set the size of this widget.
                virtual void SetArea(const Vector2& Area);
                /// @brief Updates the list of Visible buttons and hides the rest.
                virtual void DrawList();
                /// @brief Child specific update method.
                virtual void UpdateImpl(bool Force = false);
                /// @brief Child specific visibility method.
                virtual void SetVisibleImpl(bool visible);
                /// @brief Child specific mouse hover method.
                virtual bool CheckMouseHoverImpl();
            public:
                /// @brief Standard initialization constructor.
                /// @param name The name of the List Box.
                /// @param Rect The Rect representing the position and size of the List Box.
                /// The size portion of this rect would be in reference to the size of a single selection.
                /// The actual size of this widget will be a result of how many selections are currently being displayed.
                /// @param ScrollStyle The style of the scrollbar you want for this List Box.  See Scrollbar
                /// class for more information.
                /// @param PScreen The parent screen this List Box belongs to.
                ListBox(ConstString& name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollStyle, Screen* PScreen);
                /// @brief Standard destructor.
                virtual ~ListBox();
                /// @brief Sets the size to be applied to all created Selections.
                /// @param Size The size of the caption to make.  Applied to all captions created by this class.
                /// @param Relative Whether or not the size provided is in relative units(rather then pixels).
                /// @return Returns a reference to this List Box.
                virtual ListBox& SetTemplateSize(const Vector2& Size, bool Relative = true);
                /// @brief Sets the glyph index to be applied to all created Selections.
                /// @details This needs to be called prior to the creation of any selections(captions).
                /// This defaults to 0, which may not be a valid font index in your mta file.  In which case it will throw an exception.
                /// @param Glyph The Glyph index to use for the text generated by all captions in this widget.
                /// @return Returns a reference to this List Box.
                virtual ListBox& SetTemplateGlyphIndex(const Whole& Glyph);
                /// @brief Sets the text colour to be applied to all created Selections.
                /// @details Optional parameter.
                /// @param TextColour The Colour of the text.  This defaults to black.
                /// @return Returns a reference to this List Box.
                virtual ListBox& SetTemplateTextColour(const ColourValue& TextColour);
                /// @brief Sets the text scale to be applied to all created Selections.
                /// @details Optional parameter.
                /// @param Scale The scale you wish to apply to the text in all created selections.
                /// @return Returns a reference to this List Box.
                virtual ListBox& SetTemplateTextScale(const Real& Scale);
                /// @brief Set the cursor offset to be applied to all created Selections.
                /// @details Optional parameter.
                /// @param Offset The offset in pixels to adjust where text starts appearing in the caption.
                /// @return Returns a reference to this List Box.
                virtual ListBox& SetTemplateCursorOffset(const Whole& Offset);
                /// @brief Sets the background colour to be applied to all created Selections.
                /// @details Optional parameter.
                /// @param BackgroundColour The colour to set for the captions background.  This defaults to white.
                /// @return Returns a reference to this List Box.
                virtual ListBox& SetTemplateBackgroundColour(const ColourValue& BackgroundColour);
                /// @brief Sets the horizontal alignment to be applied to all created Selections.
                /// @details Optional parameter.
                /// @param HorAlign The horizontal text alignment.  This defaults to UI::Middle.
                /// @return Returns a reference to this List Box.
                virtual ListBox& SetTemplateHorizontalAlign(const UI::TextHorizontalAlign& HorAlign);
                /// @brief Sets the vertical alignment to be applied to all created Selections.
                /// @details Optional parameter.
                /// @param VertAlign The Vertical text alignment.  This defaults to UI::Center.
                /// @return Returns a reference to this List Box.
                virtual ListBox& SetTemplateVerticalAlign(const UI::TextVerticalAlign& VertAlign);
                /// @brief Sets the render priority to be applied to all created Selections.
                /// @details Optional parameter.
                /// @param Priority The priority with which to render the image.  This defaults to UI::RP_Medium.
                /// @return Returns a reference to this List Box.
                virtual ListBox& SetTemplateRenderPriority(const UI::RenderPriority& Priority);
                /// @brief Gets the struct containing all the current template parameters used when creating a new selections.
                /// @return Returns a const reference to the structure containing all the template parameters.
                virtual const UI::TemplateParams& GetTemplateInfo();
                /// @brief Adds a selectable caption to the list to be displayed.
                /// @details If a colour other then white was set as the template and you try to set a background sprite, it will
                /// attempt to blend the colour and sprite.  Pure white colour will cause the sprite to look normal.
                /// @param name A name for the selection.
                /// @param Text The text to be displayed within the created caption.
                /// @param BackgroundSprite Optional, name of the sprite to set as it's background.  Ignored if the string is empty.
                virtual Caption* AddSelection(ConstString& name, ConstString &Text, ConstString& BackgroundSprite = "");
                /// @brief Gets a caption by name.
                /// @param Name The name of the selectable caption you want to get.
                /// @return Returns a pointer to the named caption.
                virtual Caption* GetSelection(ConstString &Name);
                /// @brief Destroys a selectable caption.
                /// @param ToBeDestroyed A pointer to the caption you want destroyed and removed from this list.
                virtual void DestroySelection(Caption* ToBeDestroyed);
                /// @brief Sets the selection within this list to be "selected".
                /// @param ToBeSelected A pointer to the caption to be set as "selected".
                virtual void SetSelected(Caption* ToBeSelected);
                /// @brief Sets the maximum number of selections to display at once before the scrollbar appears.
                /// @param MaxSelections The maximum number of selections to display at once before requiring a scrollbar.
                /// This indirectly controls the max size of this widget.
                virtual void SetMaxDisplayedSelections(const Whole& MaxSelections);
                /// @brief Eanbles or disables the scrollbar autohide.
                /// @param AutoHide A bool indicating whether or not to auto hide the scrollbar.
                virtual void SetAutoHideScroll(bool AutoHide);
                /// @brief Sets the relative position of this List Box.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this List Box.
                virtual void SetPosition(const Vector2& Position);
                /// @brief Sets the pixel position of this List Box.
                /// @param Position A vector2 representing the pixel position of this List Box.
                virtual void SetActualPosition(const Vector2& Position);
                /// @brief Sets the relative size of this List Box.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Size A vector2 representing the relative size of this List Box.
                virtual void SetSize(const Vector2& Size);
                /// @brief Sets the pixel size of this List Box.
                /// @param Size A vector2 representing the pixel size of this List Box.
                virtual void SetActualSize(const Vector2& Size);
                /// @brief Updates the dimensions of this widget to match those of the new screen size.
                /// @details This function is called automatically when a viewport changes in size, and shouldn't need to be called manually.
                virtual void UpdateDimensions();
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
}//Mezzanine

#endif
