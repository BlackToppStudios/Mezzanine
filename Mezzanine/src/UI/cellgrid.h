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
#ifndef _uicellgrid_h
#define _uicellgrid_h

#include "UI/widget.h"

namespace Mezzanine
{
    namespace UI
    {
        class Rectangle;
        class Caption;
        class Cell;
        class CellPrioritySorter;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class CellGrid
        /// @headerfile uicellgrid.h
        /// @brief This is a widget used to determine spacing and ordering of other widgets(Cells) on a grid like surface.
        /// @details This grid does support cells of different sizes, but all the cells of the same row/column will behave
        /// as if they share the size as the largest cell, in terms of spacing.  The cells themselves will not be altered in
        /// size, and only the appropriate dimention will be applied(I.E. Each column will be spaced according to the largest
        /// width+spacing, and each row will be spaced according to the largest height+spacing).  Thus it is ideal to have
        /// each cell be the same size, but not required.
        ///////////////////////////////////////
        class MEZZ_LIB CellGrid : public Widget
        {
            public:
                enum PageStyle
                {
                    CG_Paged,   ///<--- Contents of the grid will be shown as pages.  Ideally used with "Horizontal_Vertical" ordering.
                    CG_Scrolled ///<--- Scrollbars will be made available to scroll through the grid, both horizontal and vertical as necessary.
                };
                enum OrderingStyle
                {
                    CG_Horizontal_Vertical_Ascending, ///<--- Objects on the grid will be ordered based on their priority from left to right, top to bottom, in increasing order.
                    CG_Horizontal_Vertical_Decending, ///<--- Objects on the grid will be ordered based on their priority from left to right, top to bottom, in decreasing order.
                    CG_Vertical_Horizontal_Ascending, ///<--- Objects on the grid will be ordered based on their priority from top to bottom, left to right, in increasing order.
                    CG_Vertical_Horizontal_Decending  ///<--- Objects on the grid will be ordered based on their priority from top to bottom, left to right, in decreasing order.
                };
            protected:
                friend class RenderableFactory;
                typedef std::list<UI::Cell*> CellList;
                typedef std::vector<UI::Cell*> CellVector;
                Rectangle* GridBack;
                bool GridDirty;
                Vector2 FixedCellSize;
                Vector2 EdgeSpacing;
                Vector2 CellSpacing;
                Vector2 WorkAreaLimits;
                Whole CellsAdded;
                CellGrid::PageStyle Paging;
                CellGrid::OrderingStyle Ordering;
                UI::Cell* Selected;
                CellList Cells;
                CellVector VisibleCells;
                std::vector<CellVector*> TheGrid;
                /// @brief Gets the maximum cells that can be shown with the current dimensions.
                virtual Whole GetMaxXCellsPresentable();
                /// @brief Gets the maximum cells that can be shown with the current dimensions.
                virtual Whole GetMaxYCellsPresentable();
                /// @brief Gets the max number of rows in the grid.
                virtual Whole GetMaxRows();
                /// @brief Gest the max number of columns in the grid.
                virtual Whole GetMaxColumns();
                /// @brief Finds the Tallest cell in a row.
                virtual Whole FindTallestInRow(const Whole& RowIndex);
                /// @brief Finds the Widest cell in a column.
                virtual Whole FindWidestInColumn(const Whole& ColumnIndex);
                /// @brief Creates(if null) or destroys(if pointer is valid) a CellVector.
                virtual void CreateOrDestroyRow(CellVector* ToDestory);
                /// @brief Removes all Cells from the grid.
                virtual void ClearGrid();
                /// @brief Re-populates the grid and the positions of it's elements.
                virtual void RegenerateGrid();
                /// @brief Re-populates the grid and the positions of it's elements.
                virtual void RegenerateGrid(const Vector2& WinDim);
                /// @brief Draws the grid, used for updating after any change.
                virtual void DrawGrid() = 0;
                /// @brief Determines whether or not the grid needs to be redrawn.
                virtual bool GridNeedsRedraw() = 0;
                /// @brief Child specific update method.
                virtual void UpdateImpl(bool Force = false);
                /// @brief Child specific visibility method.
                virtual void SetVisibleImpl(bool visible);
                /// @brief Child specific mouse hover method.
                virtual bool CheckMouseHoverImpl();
            //public:
                /// @brief Class constructor.
                /// @param name The name of the widget.
                /// @param Rect The Rect representing the position and size of the widget.
                /// @param parent The parent screen this widget belongs to.
                CellGrid(const String& name, const RenderableRect& Rect, Screen* parent);
                /// @brief Class destructor.
                virtual ~CellGrid();
            public:
                /// @brief Sets the fixed size cells in this grid are to have.
                /// @param FixedSize The fixed size to apply to cells.
                /// @param Relative Whether or not the vector 2 being passed in is in relative units.
                virtual void SetFixedCellSize(const Vector2& FixedSize, bool Relative = true);
                /// @brief Gets the currently set fixed size applied to all cells in this widget.
                /// @param Relative Optional parameter for getting the size in relative units.
                /// @return Returns a Vector2 containing the size applied to all cells in this widget.
                virtual Vector2 GetFixedCellSize(bool Relative = true);
                /// @brief Addes a Cell to this widget that will be rendered.
                /// @param ToBeAdded The Cell to be added to this widget.
                virtual void AddCell(UI::Cell* ToBeAdded);
                /// @brief Gets a Cell on the grid by name.
                /// @param Name The name of the Cell to get.
                /// @return Returns a pointer to the Cell requested, or NULL if not found.
                virtual UI::Cell* GetCell(const String& Name);
                /// @brief Gets a Cell at the specified row/column.
                /// @param Row The Row to check.  Specified a point on the grid when used along side "Column" param.
                /// @param Column The Column to check.  Specifies a point on the grid when used along side "Row" param.
                /// @return Returns a pointer to the cell requested, or NULL if nothing is at the requested row/column.
                virtual UI::Cell* GetCell(const Whole& Row, const Whole& Column);
                /// @brief Removes a Cell from this widget.
                /// @param ToBeRemoved The Cell to be removed from this widget.
                virtual void RemoveCell(UI::Cell* ToBeRemoved);
                /// @brief Destroys all Cells currently stored within this widget.
                virtual void DestroyAllCells();
                /// @brief Forces a reconstruction of the grid.
                virtual void GenerateGrid();
                /// @brief Sets the limits of the grid, to determine when and where objects need to be wrapped.
                /// @details This does not alter the actual size of the widget, just the logical area the widget has to work with.
                /// @param Area The new limits to the grid.
                /// @param Relative Whether or not the vector 2 being passed in is in relative units.
                virtual void SetWorkAreaLimits(const Vector2& AreaLimit, bool Relative = true);
                /// @brief Gets the currently set logical limits for the grid.
                /// @param Relative Optional parameter for getting the limits in relative units.
                /// @return Returns a Vector2 representing the logical limits of the grid.
                virtual Vector2 GetWorkAreaLimits(bool Relative = true);
                /// @brief Sets the spacing from the edge of the entire grid area that no cell should overlap.
                /// @details Default: 0,0.
                /// @param Spacing The minimum amount of space from the edge Cells should be.
                /// @param Relative Whether or not the vector 2 being passed in is in relative units.
                virtual void SetEdgeSpacing(const Vector2& Spacing, bool Relative = true);
                /// @brief Gets the currently set edge spacing.
                /// @param Relative Optional parameter for getting the spacing in relative units.
                /// @return Returns a Vector2 representing the spacing to be used on edges of this widget.
                virtual Vector2 GetEdgeSpacing(bool Relative = true);
                /// @brief Sets the spacing from each cell around any given cell there should be.
                /// @details Default 10.0,10.0.
                /// @param Spacing The minimum amount of space from each cell a given cell should be.
                /// @param Relative Whether or not the vector 2 being passed in is in relative units.
                virtual void SetCellSpacing(const Vector2& Spacing, bool Relative = true);
                /// @brief Gets the currently set cell spacing.
                /// @param Relative Optional parameter for getting the spacing in relative units.
                /// @return Returns a Vector2 representing the spacing to be used between Cells of this widget.
                virtual Vector2 GetCellSpacing(bool Relative = true);
                /// @brief Sets how cells in this grid should be ordered.
                /// @param GridOrdering The manner in which the cells in this grid should be sorted.
                virtual void SetGridOrdering(const CellGrid::OrderingStyle& GridOrdering);
                /// @brief Gets the current setting for how to sort the cells in the grid.
                /// @return Returns an enum value representing the current order cells have in the grid.
                virtual CellGrid::OrderingStyle GetGridOrdering();
                /// @brief Checks to see if there are any remaining cells that aren't on the grid due to a lack of space.
                /// @return Returns a bool indicating whether or not this widgets work area needs to be increased.
                virtual bool CellsOffGrid();
                /// @brief Sorts the grid based on the priority set in the cells.
                virtual void SortGrid();
                /// @brief Sets the selected pointer to NULL, so no cells are selected.
                virtual void ClearSelected();
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
                virtual void UpdateDimensions();
                /// @brief Gets the background to this Grid.
                /// @return Returns a pointer to the background of this grid.
                virtual Rectangle* GetGridBack();
                /// @brief Gets the currently selected Cell.
                /// @return Returns a pointer to the selected Cell.
                virtual UI::Cell* GetSelected();
        };//cellgrid
    }//ui
}//Mezzanine

#endif
