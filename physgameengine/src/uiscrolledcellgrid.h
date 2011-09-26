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
#ifndef _uiscrolledcellgrid_h
#define _uiscrolledcellgrid_h

#include "uicellgrid.h"
#include "uienumerations.h"

namespace phys
{
    namespace UI
    {
        class Scrollbar;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class ScrolledCellGrid
        /// @headerfile uiscrolledcellgrid.h
        /// @brief This is a specialization of the cell grid that allows you to use scrollbars to navigate the cells on the grid.
        /// @details
        ///////////////////////////////////////
        class PHYS_LIB ScrolledCellGrid : public CellGrid
        {
            protected:
                Real HScrollVal;
                Real VScrollVal;
                bool AutoHide;
                UI::Scrollbar* HorizontalScroll;
                UI::Scrollbar* VerticalScroll;
                /// @brief Gets the top row to be rendered based on the scroller position.
                virtual Whole FindTopRow(const Real& ScrollPos);
                /// @brief Gets the left column to be rendered based on the scroller position.
                virtual Whole FindLeftColumn(const Real& ScrollPos);
                /// @brief Draws the grid, used for updating after any change.
                virtual void DrawGrid();
                /// @brief Draws the grid, used for updating after any change.
                virtual void DrawGrid(const Vector2& WinDim);
                /// @brief Determines whether or not the grid needs to be redrawn.
                virtual bool GridNeedsRedraw();
                /// @brief Child specific visibility method.
                virtual void SetVisibleImpl(bool visible);
                /// @brief Child specific mouse hover method.
                virtual bool CheckMouseHoverImpl();
            public:
                /// @brief Class constructor.
                /// @param name The name of the widget.
                /// @param Rect The Rect representing the position and size of the widget.
                /// @param Thickness The width of the vertical scrollbar.  The same amount of actual pixels is used
                /// to determine the height of the horizontal scrollbar.
                /// If the Rect passed in is relative, this will expect Thickness to be relative as well.
                /// @param Style An enum value representing how you want your scrollbar constructed.  See class details for more info.
                /// @param parent The parent layer this widget belongs to.
                ScrolledCellGrid(const String& name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style, Layer* parent);
                /// @brief Class destructor.
                virtual ~ScrolledCellGrid();
                /// @brief Sets whether or not the scrollbars should auto-hide, or always be visible.
                /// @param Auto Whether or not the scrollbars should auto-hide.
                virtual void SetAutoHide(bool Auto);
                /// @brief Gets whether or not the scrollbars are auto-hiding.
                /// @return Returns a bool indicating whether or not scrollbar auto-hide is enabled.
                virtual bool GetAutoHide();
                /// @brief Sets the relative position of this widget.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this widget.
                virtual void SetPosition(const Vector2& Position);
                /// @brief Sets the pixel position of this widget.
                /// @param Position A vector2 representing the pixel position of this widget.
                virtual void SetActualPosition(const Vector2& Position);
                /// @brief Sets the relative size of this widget.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Size A vector2 representing the relative size of this widget.
                virtual void SetSize(const Vector2& Size);
                /// @brief Sets the pixel size of this widget.
                /// @param Size A vector2 representing the pixel size of this widget.
                virtual void SetActualSize(const Vector2& Size);
                /// @brief Updates the dimensions of this widget to match those of the new screen size.
                /// @details This function is called automatically when a viewport changes in size, and shouldn't need to be called manually.
                /// @param OldViewportSize The old size of the viewport.
                virtual void UpdateDimensions(const Vector2& OldViewportSize);
                /// @brief Gets the horizontal scrollbar of this grid, if present.
                /// @return Returns a pointer to the horizontal scrollbar of this Grid, or NULL if it's paged.
                virtual UI::Scrollbar* GetHorizontalScroll();
                /// @brief Gets the vertical scrollbar of this grid, if present.
                /// @return Returns a pointer to the vertical scrollbar of this Grid, or NULL if it's paged.
                virtual UI::Scrollbar* GetVerticalScroll();
        };//scrolledcellgrid
    }//ui
}//phys

#endif
