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
#ifndef _uiscrolledcellgrid_cpp
#define _uiscrolledcellgrid_cpp

#include "uiscrolledcellgrid.h"
#include "uicell.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "uiscrollbar.h"

namespace phys
{
    namespace UI
    {
        ScrolledCellGrid::ScrolledCellGrid(const String& name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style, Layer* parent)
            : CellGrid(name,Rect,parent),
              HScrollVal(0.0),
              VScrollVal(0.0),
              AutoHide(true)
        {
            Paging = CG_Scrolled;
            RenderableRect HoriRect;
            RenderableRect VertRect;
            if(Rect.Relative)
            {
                const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
                Real ActThick = Thickness * WinDim.X;

                HoriRect.Position = Vector2(RelPosition.X,(RelPosition.Y + RelSize.Y) - ActThick / WinDim.Y);
                HoriRect.Size = Vector2(RelSize.X - (ActThick / WinDim.X),ActThick / WinDim.Y);
                HoriRect.Relative = Rect.Relative;
                VertRect.Position = Vector2((RelPosition.X + RelSize.X) - Thickness,RelPosition.Y);
                VertRect.Size = Vector2(Thickness,RelSize.Y - (ActThick / WinDim.Y));
                VertRect.Relative = Rect.Relative;
            }else{
                HoriRect.Position = Vector2(Rect.Position.X,(Rect.Position.Y + Rect.Size.Y) - Thickness);
                HoriRect.Size = Vector2(Rect.Size.X - Thickness,Thickness);
                HoriRect.Relative = Rect.Relative;
                VertRect.Position = Vector2((Rect.Position.X + Rect.Size.X) - Thickness,Rect.Position.Y);
                VertRect.Size = Vector2(Thickness,Rect.Size.Y - Thickness);
                VertRect.Relative = Rect.Relative;
            }

            HorizontalScroll = new UI::Scrollbar(Name+"HoriScr",HoriRect,Style,Parent);
            VerticalScroll = new UI::Scrollbar(Name+"VertScr",VertRect,Style,Parent);
            HorizontalScroll->Hide();
            VerticalScroll->Hide();
        }

        ScrolledCellGrid::~ScrolledCellGrid()
        {
            delete HorizontalScroll;
            delete VerticalScroll;
        }

        Whole ScrolledCellGrid::FindTopRow(const Real& ScrollPos)
        {
            Whole TargetYPos = (WorkAreaLimits.Y - GetActualSize().Y) * ScrollPos;
            Whole CurrentPos = 0;
            for( Whole X = 0 ; X < TheGrid.size() ; X++ )
            {
                CurrentPos+=FixedCellSize.Y;
                if(CurrentPos >= TargetYPos)
                    return X;
            }
        }

        Whole ScrolledCellGrid::FindLeftColumn(const Real& ScrollPos)
        {
            Whole TargetXPos = (WorkAreaLimits.X - GetActualSize().X) * ScrollPos;
            Whole CurrentPos = 0;
            Whole MaxColumns = 0;
            for( std::vector<CellVector*>::iterator it = TheGrid.begin() ; it != TheGrid.end() ; it++ )
            {
                if(MaxColumns < (*it)->size())
                    MaxColumns = (*it)->size();
            }
            for( Whole X = 0 ; X < MaxColumns ; X++ )
            {
                CurrentPos+=FixedCellSize.X;
                if(CurrentPos >= TargetXPos)
                    return X;
            }
        }

        void ScrolledCellGrid::DrawGrid()
        {
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            DrawGrid(WinDim);
        }

        void ScrolledCellGrid::DrawGrid(const Vector2& WinDim)
        {
            RegenerateGrid();
            for( Whole X = 0 ; X < VisibleCells.size() ; X++ )
                VisibleCells[X]->Hide();
            VisibleCells.clear();

            HScrollVal = HorizontalScroll->GetScrollerValue();
            VScrollVal = VerticalScroll->GetScrollerValue();

            Whole CurrentRow = FindTopRow(VScrollVal);
            Whole CurrentColumn = FindLeftColumn(HScrollVal);
            const Whole StartRow = CurrentRow;
            const Whole StartColumn = CurrentColumn;

            Vector2 ActPos = GetActualPosition();
            Vector2 ActSize = GetActualSize();
            Vector2 ActEdge = EdgeSpacing * WinDim;
            Vector2 ActCell = CellSpacing * WinDim;
            Vector2 ActFixedSize = FixedCellSize * WinDim;
            Whole CursorXPos = (Whole)(ActPos.X + ActEdge.X);
            Whole CursorYPos = (Whole)(ActPos.Y + ActEdge.Y);
            const Whole StartXPos = CursorXPos;
            const Whole StartYPos = CursorYPos;

            UI::Cell* CurrentCell = NULL;
            bool StillHasRoom = true;

            switch (Ordering)
            {
                case CG_Horizontal_Vertical_Ascending:
                case CG_Horizontal_Vertical_Decending:
                {
                    if(ActSize.Y < (ActFixedSize.Y + (ActEdge.Y * 2)))
                        return;
                    while(StillHasRoom)
                    {
                        CurrentCell = GetCell(CurrentRow,CurrentColumn);
                        if(!CurrentCell)
                        {
                            CursorYPos+=ActFixedSize.Y + ActCell.Y;
                            CurrentRow++;
                            CursorXPos = StartXPos;
                            CurrentColumn = StartColumn;
                            if(CurrentRow >= GetMaxRows())
                                StillHasRoom = false;
                            continue;
                        }
                        Vector2 CellSize = CurrentCell->GetActualSize();
                        if((Real)CursorXPos + CellSize.X > (ActPos.X + ActSize.X) - ActEdge.X)
                        {
                            CursorYPos+=ActFixedSize.Y + ActCell.Y;
                            CurrentRow++;
                            CursorXPos = StartXPos;
                            CurrentColumn = StartColumn;
                            if(CursorYPos+ActFixedSize.Y > (ActPos.Y + ActSize.Y) - ActEdge.Y)
                                StillHasRoom = false;
                            continue;
                        }
                        CurrentCell->SetActualPosition(Vector2((Real)CursorXPos,(Real)CursorYPos));
                        if(Visible)
                            CurrentCell->Show();
                        VisibleCells.push_back(CurrentCell);
                        CursorXPos+=(Whole)(CellSize.X + ActCell.X);
                        CurrentColumn++;
                    }
                    break;
                }
                case CG_Vertical_Horizontal_Ascending:
                case CG_Vertical_Horizontal_Decending:
                {
                    if(ActSize.X < (ActFixedSize.X + (ActEdge.X * 2)))
                        return;
                    while(StillHasRoom)
                    {
                        CurrentCell = GetCell(CurrentRow,CurrentColumn);
                        if(!CurrentCell)
                        {
                            CursorXPos+=ActFixedSize.X + ActCell.X;
                            CurrentColumn++;
                            CursorYPos = StartYPos;
                            CurrentRow = StartRow;
                            if(CurrentColumn >= GetMaxColumns())
                                StillHasRoom = false;
                            continue;
                        }
                        Vector2 CellSize = CurrentCell->GetActualSize();
                        if((Real)CursorYPos + CellSize.Y > (ActPos.Y + ActSize.Y) - ActEdge.Y)
                        {
                            CursorXPos+=ActFixedSize.X + ActCell.X;
                            CurrentColumn++;
                            CursorYPos = StartYPos;
                            CurrentRow = StartRow;
                            if(CursorXPos+ActFixedSize.X > (ActPos.X + ActSize.X) - ActEdge.X)
                                StillHasRoom = false;
                            continue;
                        }
                        CurrentCell->SetActualPosition(Vector2((Real)CursorXPos,(Real)CursorYPos));
                        if(Visible)
                            CurrentCell->Show();
                        VisibleCells.push_back(CurrentCell);
                        CursorYPos+=(Whole)(CellSize.Y + ActCell.Y);
                        CurrentRow++;
                    }
                    break;
                }
            }
        }

        bool ScrolledCellGrid::GridNeedsRedraw()
        {
            if(TheGrid.empty())
                return false;
            return (HScrollVal != HorizontalScroll->GetScrollerValue() || VScrollVal != VerticalScroll->GetScrollerValue());
        }

        void ScrolledCellGrid::SetVisibleImpl(bool visible)
        {
            /// @todo Currently this disregards auto-hides.  Something should be implemented to account for that.
            HorizontalScroll->SetVisible(visible);
            VerticalScroll->SetVisible(visible);
            CellGrid::SetVisibleImpl(visible);
        }

        bool ScrolledCellGrid::CheckMouseHoverImpl()
        {
            if(CellGrid::CheckMouseHoverImpl())
                return true;
            if(HorizontalScroll->CheckMouseHover())
            {
                HoveredSubWidget = HorizontalScroll;
                HoveredButton = HorizontalScroll->GetHoveredButton();
                return true;
            }
            if(VerticalScroll->CheckMouseHover())
            {
                HoveredSubWidget = VerticalScroll;
                HoveredButton = VerticalScroll->GetHoveredButton();
                return true;
            }
            return false;
        }

        void ScrolledCellGrid::SetAutoHide(bool Auto)
        {
            AutoHide = Auto;
        }

        bool ScrolledCellGrid::GetAutoHide()
        {
            return AutoHide;
        }

        void ScrolledCellGrid::SetPosition(const Vector2& Position)
        {
            Vector2 HScrollOffset = VerticalScroll->GetPosition() - Position;
            HorizontalScroll->SetPosition(Position + HScrollOffset);

            Vector2 VScrollOffset = VerticalScroll->GetPosition() - Position;
            VerticalScroll->SetPosition(Position + VScrollOffset);

            CellGrid::SetPosition(Position);
        }

        void ScrolledCellGrid::SetActualPosition(const Vector2& Position)
        {
            Vector2 HScrollOffset = VerticalScroll->GetActualPosition() - Position;
            HorizontalScroll->SetActualPosition(Position + HScrollOffset);

            Vector2 VScrollOffset = VerticalScroll->GetActualPosition() - Position;
            VerticalScroll->SetActualPosition(Position + VScrollOffset);

            CellGrid::SetActualPosition(Position);
        }

        void ScrolledCellGrid::SetSize(const Vector2& Size)
        {
            Vector2 HScrollP((RelPosition.X + Size.X) - HorizontalScroll->GetSize().X,RelPosition.Y);
            Vector2 HScrollS(HorizontalScroll->GetSize().X,Size.Y);
            HorizontalScroll->SetPosition(HScrollP);
            HorizontalScroll->SetSize(HScrollS);

            Vector2 VScrollP((RelPosition.X + Size.X) - VerticalScroll->GetSize().X,RelPosition.Y);
            Vector2 VScrollS(VerticalScroll->GetSize().X,Size.Y);
            VerticalScroll->SetPosition(VScrollP);
            VerticalScroll->SetSize(VScrollS);

            CellGrid::SetSize(Size);
        }

        void ScrolledCellGrid::SetActualSize(const Vector2& Size)
        {
            Vector2 HScrollP((GetActualPosition().X + Size.X) - HorizontalScroll->GetActualSize().X,GetActualPosition().Y);
            Vector2 HScrollS(HorizontalScroll->GetActualSize().X,Size.Y);
            HorizontalScroll->SetActualPosition(HScrollP);
            HorizontalScroll->SetActualSize(HScrollS);

            Vector2 VScrollP((GetActualPosition().X + Size.X) - VerticalScroll->GetActualSize().X,GetActualPosition().Y);
            Vector2 VScrollS(VerticalScroll->GetActualSize().X,Size.Y);
            VerticalScroll->SetActualPosition(VScrollP);
            VerticalScroll->SetActualSize(VScrollS);

            CellGrid::SetActualSize(Size);
        }

        void ScrolledCellGrid::UpdateDimensions(const Vector2& OldViewportSize)
        {
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            CellGrid::UpdateDimensions(OldViewportSize);
            HorizontalScroll->UpdateDimensions(OldViewportSize);
            VerticalScroll->UpdateDimensions(OldViewportSize);
            GridDirty = true;
            RegenerateGrid(WinDim);
            DrawGrid(WinDim);
        }

        UI::Scrollbar* ScrolledCellGrid::GetHorizontalScroll()
        {
            return HorizontalScroll;
        }

        UI::Scrollbar* ScrolledCellGrid::GetVerticalScroll()
        {
            return VerticalScroll;
        }
    }//ui
}//phys

#endif
