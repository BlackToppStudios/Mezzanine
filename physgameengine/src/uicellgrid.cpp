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
#ifndef _uicellgrid_cpp
#define _uicellgrid_cpp

#include "uicellgrid.h"
#include "uicell.h"
#include "uirectangle.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "uiscrollbar.h"

#include "metacode.h"
#include "inputquerytool.h"

namespace phys
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @struct CellPrioritySorter
        /// @headerfile uicellgrid.h
        /// @brief This is a widget used to determine spacing and ordering of other widgets(Cells) on a grid like surface.
        /// @details
        ///////////////////////////////////////
        struct CellPrioritySorter
        {
            bool operator()(Cell* First, Cell* Second)
            {
                return First->GetPriority() < Second->GetPriority();
            }
        };

        CellGrid::CellGrid(const String& name, const RenderableRect& Rect, Layer* parent)
            : Widget(name,parent),
              EdgeSpacing(Vector2(0,0)),
              CellSpacing(Vector2(0.01,0.01)),
              FixedCellSize(Vector2(0.1,0.1)),
              Ordering(CellGrid::CG_Horizontal_Vertical_Ascending),
              GridDirty(false),
              Selected(NULL)
        {
            Type = Widget::W_CellGrid;
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;

                WorkAreaLimits = Rect.Size * Parent->GetParent()->GetViewportDimensions();
            }else{
                RelPosition = Rect.Position / Parent->GetParent()->GetViewportDimensions();
                RelSize = Rect.Size / Parent->GetParent()->GetViewportDimensions();

                WorkAreaLimits = Rect.Size;
            }

            GridBack = new Rectangle(Rect,Parent);
            CreateOrDestroyRow(0);
        }

        CellGrid::~CellGrid()
        {
            DestroyAllCells();
            for( Whole X = 0 ; X < TheGrid.size() ; X++ )
                delete TheGrid[X];
            TheGrid.clear();
            delete GridBack;
        }

        Whole CellGrid::GetMaxRows()
        {
            return TheGrid.size();
        }

        Whole CellGrid::GetMaxColumns()
        {
            Whole CurrentBest = 0;
            for( std::vector<CellVector*>::iterator it = TheGrid.begin() ; it != TheGrid.end() ; it++ )
            {
                if(CurrentBest < (*it)->size())
                    CurrentBest = (*it)->size();
            }
            return CurrentBest;
        }

        Whole CellGrid::FindTallestInRow(const Whole& RowIndex)
        {
            if(TheGrid.size() < RowIndex)
                return 0;
            if(TheGrid.at(RowIndex)->empty())
                return 0;
            Whole CurrentBest = 0;
            for( CellVector::iterator Cit = TheGrid.at(RowIndex)->begin() ; Cit != TheGrid.at(RowIndex)->end() ; Cit++ )
            {
                Whole CellHeight = (*Cit)->GetActualSize().Y;
                if(CellHeight > CurrentBest)
                    CurrentBest = CellHeight;
            }
            return CurrentBest;
        }

        Whole CellGrid::FindWidestInColumn(const Whole& ColumnIndex)
        {
            Whole CurrentBest = 0;
            for( std::vector<CellVector*>::iterator it = TheGrid.begin() ; it != TheGrid.end() ; it++ )
            {
                if((*it)->size() >= ColumnIndex)
                {
                    UI::Cell* CurrCell = (*it)->at(ColumnIndex);
                    Whole CellWidth = CurrCell->GetActualSize().X;
                    if(CellWidth > CurrentBest)
                        CurrentBest = CellWidth;
                }
            }
            return CurrentBest;
        }

        void CellGrid::CreateOrDestroyRow(CellVector* ToDestroy)
        {
            if(!ToDestroy)
            {
                TheGrid.push_back(new CellVector());
                return;
            }
            for( std::vector<CellVector*>::iterator it = TheGrid.begin() ; it != TheGrid.end() ; it++ )
            {
                if(ToDestroy == (*it))
                {
                    for( CellVector::iterator Cit = (*it)->begin() ; Cit != (*it)->end() ; Cit++ )
                        delete (*Cit);
                    (*it)->clear();
                    delete ToDestroy;
                    TheGrid.erase(it);
                    return;
                }
            }
        }

        void CellGrid::ClearGrid()
        {
            for( std::vector<CellVector*>::iterator it = TheGrid.begin() ; it != TheGrid.end() ; it++ )
                (*it)->clear();
        }

        void CellGrid::RegenerateGrid()
        {
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            RegenerateGrid(WinDim);
        }

        void CellGrid::RegenerateGrid(const Vector2& WinDim)
        {
            if(!GridDirty)
                return;
            ClearGrid();
            Vector2 ActPos = GridBack->GetActualPosition();
            Vector2 ActEdge = EdgeSpacing * WinDim;
            Vector2 ActCell = CellSpacing * WinDim;
            Vector2 ActFixedSize = FixedCellSize * WinDim;

            Whole CurrentColumn = 0;
            Whole CurrentRow = 0;
            Whole CurrentXposition = (Whole)ActPos.X + (Whole)(ActEdge.X);
            Whole CurrentYposition = (Whole)ActPos.Y + (Whole)(ActEdge.Y);
            Whole StartXposition = CurrentXposition;
            Whole StartYposition = CurrentYposition;
            Whole RowLimit = (Whole)((ActPos.X + WorkAreaLimits.X) - ActEdge.X * 2);
            Whole ColumnLimit = (Whole)((ActPos.Y + WorkAreaLimits.Y) - ActEdge.Y * 2);

            UI::Cell* CurrCell = NULL;

            if( CellGrid::CG_Horizontal_Vertical_Ascending == Ordering || CellGrid::CG_Horizontal_Vertical_Decending == Ordering )
            {
                for( CellList::iterator it = Cells.begin() ; it != Cells.end() ; it++ )
                {
                    CurrCell = (*it);
                    Vector2 CellSize = CurrCell->GetActualSize();
                    if(RowLimit >= CurrentXposition + (Whole)(CellSize.X + ActCell.X))
                    {
                        TheGrid.at(CurrentRow)->push_back(CurrCell);
                        CurrentXposition+=CellSize.X + ActCell.X;
                        CurrentColumn++;
                    }
                    else if(RowLimit >= CurrentXposition + (Whole)CellSize.X)
                    {
                        TheGrid.at(CurrentRow)->push_back(CurrCell);
                        CurrentXposition+=CellSize.X + ActCell.X;
                        CurrentColumn++;
                    }
                    else
                    {
                        if(ColumnLimit < CurrentYposition + (Whole)(CellSize.Y + ActCell.Y))
                            break;
                        CurrentXposition = StartXposition;
                        CurrentColumn = 0;
                        CurrentYposition+=(Whole)(ActFixedSize.Y + ActCell.Y);
                        CurrentRow++;
                        if(CurrentRow >= TheGrid.size())
                            CreateOrDestroyRow(0);

                        TheGrid.at(CurrentRow)->push_back(CurrCell);
                        CurrentXposition+=CellSize.X + ActCell.X;
                        CurrentColumn++;
                    }
                }
            }
            else if( CellGrid::CG_Vertical_Horizontal_Ascending == Ordering || CellGrid::CG_Vertical_Horizontal_Decending == Ordering )
            {
                for( CellList::iterator it = Cells.begin() ; it != Cells.end() ; it++ )
                {
                    CurrCell = (*it);
                    Vector2 CellSize = CurrCell->GetActualSize();
                    if(ColumnLimit >= CurrentYposition + (Whole)(CellSize.Y + ActCell.Y))
                    {
                        TheGrid.at(CurrentRow)->push_back(CurrCell);
                        CurrentYposition+=CellSize.Y + ActCell.Y;
                        CurrentRow++;
                        if(CurrentRow >= TheGrid.size())
                            CreateOrDestroyRow(0);
                    }
                    else if(ColumnLimit >= CurrentYposition + (Whole)CellSize.Y)
                    {
                        TheGrid.at(CurrentRow)->push_back(CurrCell);
                        CurrentYposition+=CellSize.Y + ActCell.Y;
                        CurrentRow++;
                        if(CurrentRow >= TheGrid.size())
                            CreateOrDestroyRow(0);
                    }
                    else
                    {
                        if(RowLimit < CurrentXposition + (Whole)(CellSize.X + ActCell.X))
                            break;
                        CurrentYposition = StartYposition;
                        CurrentRow = 0;
                        CurrentXposition+=(Whole)(ActFixedSize.X + ActCell.X);
                        CurrentColumn++;

                        TheGrid.at(CurrentRow)->push_back(CurrCell);
                        CurrentRow++;
                        if(CurrentRow >= TheGrid.size())
                            CreateOrDestroyRow(0);
                    }
                }
            }
            GridDirty = false;
        }

        void CellGrid::UpdateImpl(bool Force)
        {
            MetaCode::ButtonState State = InputQueryTool::GetMouseButtonState(1);
            if(HoveredSubWidget)
            {
                if(MetaCode::BUTTON_LIFTING == State)
                {
                    if(Widget::W_Cell == HoveredSubWidget->GetType() && HoveredSubWidget != Selected)
                    {
                        if(Selected)
                        {
                            Selected->SetSelected(false);
                            Selected = NULL;
                        }
                        Selected = static_cast<UI::Cell*>(HoveredSubWidget);
                        Selected->SetSelected(true);
                    }
                }
            }
            if(GridNeedsRedraw())
                DrawGrid();
        }

        void CellGrid::SetVisibleImpl(bool visible)
        {
            GridBack->SetVisible(visible);
            for( CellVector::iterator it = VisibleCells.begin() ; it != VisibleCells.end() ; it++ )
            {
                (*it)->SetVisible(visible);
            }
        }

        bool CellGrid::CheckMouseHoverImpl()
        {
            for( CellVector::iterator it = VisibleCells.begin() ; it != VisibleCells.end() ; it++ )
            {
                if((*it)->IsVisible())
                {
                    if((*it)->CheckMouseHover())
                    {
                        HoveredSubWidget = (*it);
                        HoveredButton = HoveredSubWidget->GetHoveredButton();
                        return true;
                    }
                }
            }
            if(GridBack->CheckMouseHover())
            {
                HoveredSubWidget = NULL;
                HoveredButton = NULL;
                return true;
            }
            return false;
        }

        void CellGrid::SetFixedCellSize(const Vector2& FixedSize)
        {
            FixedCellSize = FixedSize;
        }

        Vector2 CellGrid::GetFixedCellSize()
        {
            return FixedCellSize;
        }

        void CellGrid::AddCell(UI::Cell* ToBeAdded)
        {
            ToBeAdded->SetSize(FixedCellSize);
            ToBeAdded->Hide();
            Cells.push_back(ToBeAdded);
            GridDirty = true;
        }

        UI::Cell* CellGrid::GetCell(const String& Name)
        {
            if(Cells.empty())
                return 0;
            UI::Cell* CurrCell = NULL;
            for( CellList::iterator it = Cells.begin() ; it != Cells.end() ; it++ )
            {
                CurrCell = (*it);
                if(Name == CurrCell->GetName())
                    return CurrCell;
            }
            return 0;
        }

        UI::Cell* CellGrid::GetCell(const Whole& Row, const Whole& Column)
        {
            if(Row >= TheGrid.size())
                return 0;

            CellVector* CellRow = TheGrid.at(Row);

            if(Column >= CellRow->size())
                return 0;

            return CellRow->at(Column);
        }

        void CellGrid::RemoveCell(UI::Cell* ToBeRemoved)
        {
            GridDirty = true;
        }

        void CellGrid::DestroyAllCells()
        {
            for( std::vector<CellVector*>::iterator it = TheGrid.begin() ; it != TheGrid.end() ; it++ )
            {
                for( CellVector::iterator Cit = (*it)->begin() ; Cit != (*it)->end() ; Cit++ )
                    delete (*Cit);
                (*it)->clear();
            }
            GridDirty = true;
        }

        void CellGrid::GenerateGrid()
        {
            if(!GridDirty)
                return;
            //RegenerateGrid();
            DrawGrid();
        }

        void CellGrid::SetWorkAreaLimits(const Vector2& AreaLimit)
        {
            WorkAreaLimits = AreaLimit;
        }

        Vector2 CellGrid::GetWorkAreaLimits()
        {
            return WorkAreaLimits;
        }

        void CellGrid::SetEdgeSpacing(const Vector2& Spacing)
        {
            EdgeSpacing = Spacing;
        }

        Vector2 CellGrid::GetEdgeSpacing()
        {
            return EdgeSpacing;
        }

        void CellGrid::SetCellSpacing(const Vector2& Spacing)
        {
            CellSpacing = Spacing;
        }

        Vector2 CellGrid::GetCellSpacing()
        {
            return CellSpacing;
        }

        bool CellGrid::CellsOffGrid()
        {
            Whole GridCount = 0;
            for( std::vector<CellVector*>::iterator it = TheGrid.begin() ; it != TheGrid.end() ; it++ )
                GridCount+=(*it)->size();
            return GridCount < Cells.size();
        }

        void CellGrid::SortGrid()
        {
            Cells.sort(CellPrioritySorter());
            if( CellGrid::CG_Horizontal_Vertical_Decending == Ordering || CellGrid::CG_Vertical_Horizontal_Decending == Ordering )
                Cells.reverse();
        }

        void CellGrid::ClearSelected()
        {
            Selected->SetSelected(false);
            Selected = NULL;
        }

        void CellGrid::SetPosition(const Vector2& Position)
        {
            RelPosition = Position;
            GridBack->SetPosition(Position);
            DrawGrid();
        }

        void CellGrid::SetActualPosition(const Vector2& Position)
        {
            RelPosition = Position / Parent->GetParent()->GetViewportDimensions();
            GridBack->SetActualPosition(Position);
            DrawGrid();
        }

        void CellGrid::SetSize(const Vector2& Size)
        {
            RelSize = Size;
            GridBack->SetSize(Size);
            DrawGrid();
        }

        void CellGrid::SetActualSize(const Vector2& Size)
        {
            RelSize = Size / Parent->GetParent()->GetViewportDimensions();
            GridBack->SetActualSize(Size);
            DrawGrid();
        }

        void CellGrid::UpdateDimensions(const Vector2& OldViewportSize)
        {
            GridBack->UpdateDimensions();
            WorkAreaLimits = (WorkAreaLimits / OldViewportSize) * Parent->GetParent()->GetViewportDimensions();
            for( CellList::iterator it = Cells.begin() ; it != Cells.end() ; it++ )
            {
                (*it)->UpdateDimensions(OldViewportSize);
            }
        }

        Rectangle* CellGrid::GetGridBack()
        {
            return GridBack;
        }

        UI::Cell* CellGrid::GetSelected()
        {
            return Selected;
        }
    }//ui
}//phys

#endif
