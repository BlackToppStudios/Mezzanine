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
#ifndef _uiwindow_h
#define _uiwindow_h

#include "uiwidget.h"
#include "uiresizinginfo.h"

#include <utility>

namespace phys
{
    namespace UI
    {
        //forward declares
        class Button;
        class TextButton;
        class Rectangle;
        class Caption;
        class MarkupText;
        class LineList;
        class Scrollbar;
        class CheckBox;
        class ButtonListBox;
        class ListBox;
        //typedefs
        typedef ResizingInfo<Button*> OffsetButtonInfo;
        typedef ResizingInfo<Rectangle*> OffsetRectangleInfo;
        typedef ResizingInfo<Caption*> OffsetCaptionInfo;
        typedef ResizingInfo<MarkupText*> OffsetMarkupTextInfo;
        typedef ResizingInfo<Widget*> OffsetWidgetInfo;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Window
        /// @headerfile uiwindow.h
        /// @brief This is a container widget capable of holding any other widget.
        /// @details
        ///////////////////////////////////////
        class PHYS_LIB Window : public Widget
        {
            protected:
                Rectangle* WindowBack;
                Caption* WindowTitle;
                std::vector<OffsetButtonInfo> Buttons;
                std::vector<OffsetRectangleInfo> Rectangles;
                std::vector<OffsetCaptionInfo> Captions;
                std::vector<OffsetMarkupTextInfo> MarkupTexts;
                std::vector<OffsetWidgetInfo> Widgets;
                /// @brief For use with widget update/automation.
                void Update(bool Force = false);
                /// @brief Internal function for setting the location(position) of this widget.
                void SetLocation(Vector2 &Position);
                /// @brief Internal function for setting the area(size) of this widget.
                void SetArea(Vector2 &Size);
                /// @brief Calculates a new offset for an individual element.
                Vector2 CalculateOffset(Vector2 NewSize, Vector2 OldSize, Vector2 Offset, UI::ResizeableAnchor Anchor);
            public:
                /// @brief Standard initialization constructor.
                /// @param name The name of the window.
                /// @param Position The position of the window.
                /// @param Size The size of the window.
                /// @param Layer The parent layer this window belongs to.
                Window(ConstString& name, Vector2 Position, Vector2 Size, UILayer* Layer);
                /// @brief Standard destructor.
                virtual ~Window();
                /// @brief Sets the visibility of this window.
                /// @param visible Bool determining whether or not this window should be visible.
                virtual void SetVisible(bool visible);
                /// @brief Gets the visibility of this window.
                /// @return Returns a bool representing the visibility of this window.
                virtual bool IsVisible();
                /// @brief Forces this window to be shown.
                virtual void Show();
                /// @brief Forces this window to hide.
                virtual void Hide();
                /// @brief Checks to see if the current mouse position is over this window.
                /// @return Returns a bool value, true if the mouse is over this window, false if it's not.
                virtual bool CheckMouseHover();
                /// @brief Sets the relative position of this window.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this window.
                virtual void SetPosition(Vector2 Position);
                /// @brief Gets the relative position of this window.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative position of this window.
                virtual Vector2 GetPosition();
                /// @brief Sets the pixel position of this window.
                /// @param Position A vector2 representing the pixel position of this window.
                virtual void SetActualPosition(Vector2 Position);
                /// @brief Sets the pixel position of this window.
                /// @return Returns a vector2 representing the pixel position of this window.
                virtual Vector2 GetActualPosition();
                /// @brief Sets the relative size of this window.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Size A vector2 representing the relative size of this window.
                virtual void SetSize(Vector2 Size);
                /// @brief Gets the relative size of this window.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative size of this window.
                virtual Vector2 GetSize();
                /// @brief Sets the pixel size of this window.
                /// @param Size A vector2 representing the pixel size of this window.
                virtual void SetActualSize(Vector2 Size);
                /// @brief Sets the pixel size of this window.
                /// @return Returns a vector2 representing the pixel size of this window.
                virtual Vector2 GetActualSize();
        };//window
    }//UI
}//phys

#endif
