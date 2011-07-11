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
#ifndef _uipagedcellgrid_h
#define _uipagedcellgrid_h

#include "uicellgrid.h"
#include "enumerations.h"

namespace phys
{
    namespace UI
    {
        class Spinner;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class PagedCellGrid
        /// @headerfile uipagedcellgrid.h
        /// @brief This is a specialization of the cell grid that allows you to use a spinner to navigate through pages of cells on the grid.
        /// @details Unlike scrollbars, the location of a spinner that refers to this widget can be anywhere
        ///////////////////////////////////////
        class PHYS_LIB PagedCellGrid : public CellGrid
        {
            protected:
                Whole CurrentPage;
                Whole XCellsPerPage;
                Whole YCellsPerPage;
                Whole MaxPages;
                UI::Spinner* PageSpinner;
                /// @brief Calculates the cells per page with existing parameters.
                virtual void CalculateCellsPerPage();
                /// @brief Finds the top left of the passed in page.
                virtual std::pair<Whole,Whole> FindTopLeftOfPage(const Whole& Page);
                /// @brief Draws the grid, used for updating after any change.
                virtual void DrawGrid();
                /// @brief Draws the grid, used for updating after any change.
                virtual void DrawGrid(const Vector2& WinDim);
                /// @brief Determines whether or not the grid needs to be redrawn.
                virtual bool GridNeedsRedraw();
            public:
                /// @brief Class constructor.
                /// @param name The name of the widget.
                /// @param Position The position of the widget.
                /// @param Size The size of the widget.
                /// @param SpnPosition The position of the spinner created alongside this widget.
                /// @param SpnSize The size of the spinner created alongside this widget.
                /// @param SStyle The style of spinner to create.
                /// @param GlyphHeight The desired lineheight of the glyphs to be used with the spinner.
                /// @param parent The parent layer this widget belongs to.
                PagedCellGrid(const String& name, const Vector2& Position, const Vector2& Size, const Vector2& SpnPosition, const Vector2& SpnSize, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight, Layer* parent);
                /// @brief Class destructor.
                virtual ~PagedCellGrid();
                /// @brief Sets the visibility of this widget.
                /// @param visible Bool determining whether or not this widget should be visible.
                virtual void SetVisible(bool visible);
                /// @brief Forces this widget to be shown.
                virtual void Show();
                /// @brief Forces this widget to hide.
                virtual void Hide();
                /// @brief Sets the fixed size cells in this grid are to have.
                /// @param FixedSize The fixed size to apply to cells.
                virtual void SetFixedCellSize(const Vector2& FixedSize);
                /// @brief Sets the limits of the grid, to determine when and where objects need to be wrapped.
                /// @details This does not alter the actual size of the widget, just the logical area the widget has to work with.
                /// @param Area The new limits to the grid.
                virtual void SetWorkAreaLimits(const Vector2& AreaLimit);
                /// @brief Sets the spacing from the edge of the entire grid area that no cell should overlap.
                /// @details Default: 0,0.
                /// @param Spacing The minimum amount of space (in relative units) from the edge Cells should be.
                virtual void SetEdgeSpacing(const Vector2& Spacing);
                /// @brief Sets the spacing from each cell around any given cell there should be.
                /// @details Default 0.01,0.01.
                /// @param Spacing The minimum amount of space (in relative units) from each cell a given cell should be.
                virtual void SetCellSpacing(const Vector2& Spacing);
                /// @brief Checks to see if the current mouse position is over this widget.
                /// @return Returns a bool value, true if the mouse is over this widget, false if it's not.
                virtual bool CheckMouseHover();
                /// @brief Updates the dimensions of this widget to match those of the new screen size.
                /// @details This function is called automatically when a viewport changes in size, and shouldn't need to be called manually.
                /// @param OldViewportSize The old size of the viewport.
                virtual void UpdateDimensions(const Vector2& OldViewportSize);
                /// @brief Gets the spinner in use by this widget.
                /// @return Returns a pointer to the spinner being used by this widget.
                virtual UI::Spinner* GetPageSpinner();
        };//pagedcellgrid
    }//ui
}//phys

#endif
