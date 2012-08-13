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
#ifndef _uipagedcellgrid_cpp
#define _uipagedcellgrid_cpp

#include "UI/pagedcellgrid.h"
#include "UI/spinner.h"
#include "UI/cell.h"
#include "UI/screen.h"
#include "UI/viewportupdatetool.h"

#include "mathtool.h"

namespace Mezzanine
{
    namespace UI
    {
        PagedCellGrid::PagedCellGrid(const String& name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight, Screen* parent)
            : CellGrid(name,Rect,parent),
              CurrentPage(1),
              XCellsPerPage(1),
              YCellsPerPage(1),
              MaxPages(1)
        {
            PageSpinner = ParentScreen->CreateSpinner(Name+"Spn",SpnRect,SStyle,GlyphHeight);
            PageSpinner->SetSpinnerValue((int)CurrentPage);
            PageSpinner->SetValueLimits(1,1);
            CellsAdded = 1;
            AddSubRenderable(1,PageSpinner);
        }

        PagedCellGrid::~PagedCellGrid()
        {
            ParentScreen->DestroyWidget(PageSpinner);
        }

        void PagedCellGrid::CalculateCellsPerPage()
        {
            Real XSpace = RelSize.X - (EdgeSpacing.X * 2);
            Real YSpace = RelSize.Y - (EdgeSpacing.Y * 2);
            XCellsPerPage = 1;
            YCellsPerPage = 1;

            Real XCursor = FixedCellSize.X;
            Real YCursor = FixedCellSize.Y;
            bool DoNotBreak = true;
            while(DoNotBreak)
            {
                if(XCursor + (FixedCellSize.X + CellSpacing.X) < XSpace)
                {
                    XCursor+=(FixedCellSize.X + CellSpacing.X);
                    XCellsPerPage++;
                }
                else if(XCursor + FixedCellSize.X < XSpace)
                {
                    XCursor+=FixedCellSize.X;
                    XCellsPerPage++;
                }
                else
                {
                    DoNotBreak = false;
                }
            }
            DoNotBreak = true;
            while(DoNotBreak)
            {
                if(YCursor + (FixedCellSize.Y + CellSpacing.Y) < YSpace)
                {
                    YCursor+=(FixedCellSize.Y + CellSpacing.Y);
                    YCellsPerPage++;
                }
                else if(YCursor + FixedCellSize.Y < YSpace)
                {
                    YCursor+=FixedCellSize.Y;
                    YCellsPerPage++;
                }
                else
                {
                    DoNotBreak = false;
                }
            }
        }

        std::pair<Whole,Whole> PagedCellGrid::FindTopLeftOfPage(const Whole& Page)
        {
            Whole TotalRows = TheGrid.size();
            Whole TotalColumns = TheGrid.at(0)->size();

            Whole XPages = TotalColumns / XCellsPerPage;
            if(TotalColumns % XCellsPerPage)
                XPages++;
            Whole YPages = TotalRows / YCellsPerPage;
            if(TotalRows % YCellsPerPage)
                YPages++;

            MaxPages = XPages * YPages;

            Whole RRow = 0;
            Whole RColumn = 0;
            Whole page = Page;
            bool DoNotBreak = true;
            while(DoNotBreak)
            {
                if(page > XPages)
                {
                    page-=XPages;
                    RRow+=YCellsPerPage;
                }else{
                    //if(1==Page)
                    //    RColumn = 0;
                    //else
                        RColumn+=((XCellsPerPage * page) - XCellsPerPage);
                    DoNotBreak = false;
                }
            }
            return std::pair<Whole,Whole>(RRow,RColumn);
        }

        void PagedCellGrid::DrawGrid()
        {
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            DrawGrid(WinDim);
        }

        void PagedCellGrid::DrawGrid(const Vector2& WinDim)
        {
            if(Cells.empty())
                return;
            RegenerateGrid();
            for( Whole X = 0 ; X < VisibleCells.size() ; X++ )
                VisibleCells[X]->Hide();
            VisibleCells.clear();

            std::pair<Whole,Whole> Result = FindTopLeftOfPage(CurrentPage);
            PageSpinner->SetValueLimits(1,MaxPages);
            Whole CurrentRow = Result.first;
            Whole CurrentColumn = Result.second;
            const Whole StartRow = CurrentRow;
            const Whole StartColumn = CurrentColumn;

            Vector2 ActPos = GetActualPosition();
            Vector2 ActSize = GetActualSize();
            Whole CursorXPos = (Whole)(ActPos.X + EdgeSpacing.X);
            Whole CursorYPos = (Whole)(ActPos.Y + EdgeSpacing.Y);
            const Whole StartXPos = CursorXPos;
            const Whole StartYPos = CursorYPos;

            UI::Cell* CurrentCell = NULL;
            bool StillHasRoom = true;

            switch (Ordering)
            {
                case CG_Horizontal_Vertical_Ascending:
                case CG_Horizontal_Vertical_Decending:
                {
                    if(ActSize.Y < (FixedCellSize.Y + (EdgeSpacing.Y * 2)))
                        return;
                    while(StillHasRoom)
                    {
                        CurrentCell = GetCell(CurrentRow,CurrentColumn);
                        if(!CurrentCell)
                        {
                            CursorYPos+=FixedCellSize.Y + CellSpacing.Y;
                            CurrentRow++;
                            CursorXPos = StartXPos;
                            CurrentColumn = StartColumn;
                            if(CurrentRow >= GetMaxRows())
                                StillHasRoom = false;
                            continue;
                        }
                        Vector2 CellSize = CurrentCell->GetActualSize();
                        if((Real)CursorXPos + CellSize.X > (ActPos.X + ActSize.X) - EdgeSpacing.X)
                        {
                            CursorYPos+=FixedCellSize.Y + CellSpacing.Y;
                            CurrentRow++;
                            CursorXPos = StartXPos;
                            CurrentColumn = StartColumn;
                            if(CursorYPos+FixedCellSize.Y > (ActPos.Y + ActSize.Y) - EdgeSpacing.Y)
                                StillHasRoom = false;
                            continue;
                        }
                        CurrentCell->SetActualPosition(Vector2((Real)CursorXPos,(Real)CursorYPos));
                        if(Visible)
                            CurrentCell->Show();
                        VisibleCells.push_back(CurrentCell);
                        CursorXPos+=(Whole)(CellSize.X + CellSpacing.X);
                        CurrentColumn++;
                    }
                    break;
                }
                case CG_Vertical_Horizontal_Ascending:
                case CG_Vertical_Horizontal_Decending:
                {
                    if(ActSize.X < (FixedCellSize.X + (EdgeSpacing.X * 2)))
                        return;
                    while(StillHasRoom)
                    {
                        CurrentCell = GetCell(CurrentRow,CurrentColumn);
                        if(!CurrentCell)
                        {
                            CursorXPos+=FixedCellSize.X + CellSpacing.X;
                            CurrentColumn++;
                            CursorYPos = StartYPos;
                            CurrentRow = StartRow;
                            if(CurrentColumn >= GetMaxColumns())
                                StillHasRoom = false;
                            continue;
                        }
                        Vector2 CellSize = CurrentCell->GetActualSize();
                        if((Real)CursorYPos + CellSize.Y > (ActPos.Y + ActSize.Y) - EdgeSpacing.Y)
                        {
                            CursorXPos+=FixedCellSize.X + CellSpacing.X;
                            CurrentColumn++;
                            CursorYPos = StartYPos;
                            CurrentRow = StartRow;
                            if(CursorXPos+FixedCellSize.X > (ActPos.X + ActSize.X) - EdgeSpacing.X)
                                StillHasRoom = false;
                            continue;
                        }
                        CurrentCell->SetActualPosition(Vector2((Real)CursorXPos,(Real)CursorYPos));
                        if(Visible)
                            CurrentCell->Show();
                        VisibleCells.push_back(CurrentCell);
                        CursorYPos+=(Whole)(CellSize.Y + CellSpacing.Y);
                        CurrentRow++;
                    }
                    break;
                }
            }
        }

        bool PagedCellGrid::GridNeedsRedraw()
        {
            if(Cells.empty())
                return false;
            return (int)CurrentPage != PageSpinner->GetSpinnerValue();
        }

        void PagedCellGrid::SetVisibleImpl(bool visible)
        {
            PageSpinner->SetVisible(visible);
            CellGrid::SetVisibleImpl(visible);
        }

        bool PagedCellGrid::CheckMouseHoverImpl()
        {
            if(PageSpinner->CheckMouseHover())
            {
                HoveredSubWidget = PageSpinner;
                return true;
            }
            if(CellGrid::CheckMouseHoverImpl())
                return true;
            return false;
        }

        void PagedCellGrid::SetFixedCellSize(const Vector2& FixedSize)
        {
            CellGrid::SetFixedCellSize(FixedSize);
            CalculateCellsPerPage();
        }

        void PagedCellGrid::SetWorkAreaLimits(const Vector2& AreaLimit)
        {
            CellGrid::SetWorkAreaLimits(AreaLimit);
            CalculateCellsPerPage();
        }

        void PagedCellGrid::SetEdgeSpacing(const Vector2& Spacing)
        {
            CellGrid::SetEdgeSpacing(Spacing);
            CalculateCellsPerPage();
        }

        void PagedCellGrid::SetCellSpacing(const Vector2& Spacing)
        {
            CellGrid::SetCellSpacing(Spacing);
            CalculateCellsPerPage();
        }

        void PagedCellGrid::DestroyAllCells()
        {
            CellGrid::DestroyAllCells();
            AddSubRenderable(1,PageSpinner);
        }

        void PagedCellGrid::UpdateDimensions()
        {
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            CellGrid::UpdateDimensions();
            PageSpinner->UpdateDimensions();
            GridDirty = true;
            RegenerateGrid(WinDim);
            DrawGrid(WinDim);
        }

        Spinner* PagedCellGrid::GetPageSpinner()
        {
            return PageSpinner;
        }
    }//ui
}//Mezzanine

#endif
