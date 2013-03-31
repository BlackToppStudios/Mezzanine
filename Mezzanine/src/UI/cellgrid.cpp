//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _uicellgrid_cpp
#define _uicellgrid_cpp

#include "UI/cellgrid.h"
#include "UI/cell.h"
#include "UI/rectangle.h"
#include "UI/screen.h"
#include "UI/scrollbar.h"
#include "UI/viewportupdatetool.h"

#include "inputmanager.h"
#include "Input/metacode.h"
#include "Input/mouse.h"

namespace Mezzanine
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

        CellGrid::CellGrid(const String& name, const RenderableRect& Rect, Screen* parent)
            : Widget(name,parent),
              EdgeSpacing(Vector2(0,0)),
              CellSpacing(Vector2(10.0,10.0)),
              FixedCellSize(Vector2(100.0,100.0)),
              Ordering(CellGrid::CG_Horizontal_Vertical_Ascending),
              CellsAdded(0),
              GridDirty(false),
              Selected(NULL)
        {
            Type = Widget::W_CellGrid;
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;

                WorkAreaLimits = Rect.Size * ParentScreen->GetViewportDimensions();
            }else{
                RelPosition = Rect.Position / ParentScreen->GetViewportDimensions();
                RelSize = Rect.Size / ParentScreen->GetViewportDimensions();

                WorkAreaLimits = Rect.Size;
            }

            GridBack = ParentScreen->CreateRectangle(Rect);
            AddSubRenderable(0,GridBack);
            CreateOrDestroyRow(0);
        }

        CellGrid::~CellGrid()
        {
            DestroyAllCells();
            for( Whole X = 0 ; X < TheGrid.size() ; X++ )
                delete TheGrid[X];
            TheGrid.clear();
            ParentScreen->DestroyBasicRenderable(GridBack);
        }

        Whole CellGrid::GetMaxXCellsPresentable()
        {
            Whole MaxColumns = GetMaxColumns();
            Vector2 ActSize = GetActualSize();
            Real Size = 0;
            for( Whole X = 0 ; X < MaxColumns ; ++X )
            {
                if( ActSize.X > Size + FixedCellSize.X + CellSpacing.X ) Size += (FixedCellSize.X + CellSpacing.X);
                else if( ActSize.X > Size + FixedCellSize.X ) return X + 1;
                else return X;
            }
        }

        Whole CellGrid::GetMaxYCellsPresentable()
        {
            Whole MaxRows = GetMaxRows();
            Vector2 ActSize = GetActualSize();
            Real Size = 0;
            for( Whole Y = 0 ; Y < MaxRows ; ++Y )
            {
                if( ActSize.Y > Size + FixedCellSize.Y + CellSpacing.Y ) Size += (FixedCellSize.Y + CellSpacing.Y);
                else if( ActSize.Y > Size + FixedCellSize.Y ) return Y + 1;
                else return Y;
            }
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
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            RegenerateGrid(WinDim);
        }

        void CellGrid::RegenerateGrid(const Vector2& WinDim)
        {
            if(!GridDirty)
                return;
            ClearGrid();
            Vector2 ActPos = GridBack->GetActualPosition();

            Whole CurrentColumn = 0;
            Whole CurrentRow = 0;
            Whole CurrentXposition = (Whole)ActPos.X + (Whole)(EdgeSpacing.X);
            Whole CurrentYposition = (Whole)ActPos.Y + (Whole)(EdgeSpacing.Y);
            Whole StartXposition = CurrentXposition;
            Whole StartYposition = CurrentYposition;
            Whole RowLimit = (Whole)((ActPos.X + WorkAreaLimits.X) - EdgeSpacing.X * 2);
            Whole ColumnLimit = (Whole)((ActPos.Y + WorkAreaLimits.Y) - EdgeSpacing.Y * 2);

            UI::Cell* CurrCell = NULL;

            if( CellGrid::CG_Horizontal_Vertical_Ascending == Ordering || CellGrid::CG_Horizontal_Vertical_Decending == Ordering )
            {
                for( CellList::iterator it = Cells.begin() ; it != Cells.end() ; it++ )
                {
                    CurrCell = (*it);
                    Vector2 CellSize = CurrCell->GetActualSize();
                    if(RowLimit >= CurrentXposition + (Whole)(CellSize.X + CellSpacing.X))
                    {
                        TheGrid.at(CurrentRow)->push_back(CurrCell);
                        CurrentXposition+=CellSize.X + CellSpacing.X;
                        CurrentColumn++;
                    }
                    else if(RowLimit >= CurrentXposition + (Whole)CellSize.X)
                    {
                        TheGrid.at(CurrentRow)->push_back(CurrCell);
                        CurrentXposition+=CellSize.X + CellSpacing.X;
                        CurrentColumn++;
                    }
                    else
                    {
                        if(ColumnLimit < CurrentYposition + (Whole)(CellSize.Y + CellSpacing.Y))
                            break;
                        CurrentXposition = StartXposition;
                        CurrentColumn = 0;
                        CurrentYposition+=(Whole)(FixedCellSize.Y + CellSpacing.Y);
                        CurrentRow++;
                        if(CurrentRow >= TheGrid.size())
                            CreateOrDestroyRow(0);

                        TheGrid.at(CurrentRow)->push_back(CurrCell);
                        CurrentXposition+=CellSize.X + CellSpacing.X;
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
                    if(ColumnLimit >= CurrentYposition + (Whole)(CellSize.Y + CellSpacing.Y))
                    {
                        TheGrid.at(CurrentRow)->push_back(CurrCell);
                        CurrentYposition+=CellSize.Y + CellSpacing.Y;
                        CurrentRow++;
                        if(CurrentRow >= TheGrid.size())
                            CreateOrDestroyRow(0);
                    }
                    else if(ColumnLimit >= CurrentYposition + (Whole)CellSize.Y)
                    {
                        TheGrid.at(CurrentRow)->push_back(CurrCell);
                        CurrentYposition+=CellSize.Y + CellSpacing.Y;
                        CurrentRow++;
                        if(CurrentRow >= TheGrid.size())
                            CreateOrDestroyRow(0);
                    }
                    else
                    {
                        if(RowLimit < CurrentXposition + (Whole)(CellSize.X + CellSpacing.X))
                            break;
                        CurrentYposition = StartYposition;
                        CurrentRow = 0;
                        CurrentXposition+=(Whole)(FixedCellSize.X + CellSpacing.X);
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
            Input::ButtonState State = InputManager::GetSingletonPtr()->GetSystemMouse()->GetButtonState(1);
            if(HoveredSubWidget)
            {
                if(Input::BUTTON_LIFTING == State)
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
                        return true;
                    }
                }
            }
            if(GridBack->CheckMouseHover())
            {
                HoveredSubWidget = NULL;
                return true;
            }
            return false;
        }

        void CellGrid::SetFixedCellSize(const Vector2& FixedSize, bool Relative)
        {
            if(Relative) FixedCellSize = FixedSize * ParentScreen->GetViewportDimensions();
            else FixedCellSize = FixedSize;
        }

        Vector2 CellGrid::GetFixedCellSize(bool Relative)
        {
            if(Relative) return FixedCellSize / ParentScreen->GetViewportDimensions();
            else return FixedCellSize;
        }

        void CellGrid::AddCell(UI::Cell* ToBeAdded)
        {
            CellsAdded++;
            ToBeAdded->SetActualSize(FixedCellSize);
            ToBeAdded->Hide();
            Cells.push_back(ToBeAdded);
            AddSubRenderable(CellsAdded,ToBeAdded);
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
            // Check Visible Cells
            for( std::vector<UI::Cell*>::iterator it = VisibleCells.begin() ; it != VisibleCells.end() ; ++it )
            {
                if( (*it) == ToBeRemoved )
                {
                    VisibleCells.erase(it);
                    break;
                }
            }
            // Check All Cells
            for( std::list<UI::Cell*>::iterator it = Cells.begin() ; it != Cells.end() ; ++it )
            {
                if( (*it) == ToBeRemoved )
                {
                    Cells.erase(it);
                    break;
                }
            }
            // Check Z-Order listing
            for( RenderableIterator it = SubRenderables.begin() ; it != SubRenderables.end() ; ++it )
            {
                if( (*it) == ToBeRemoved )
                {
                    SubRenderables.erase(it);
                    break;
                }
            }
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
            Cells.clear();
            VisibleCells.clear();
            SubRenderables.clear();
            AddSubRenderable(0,GridBack);
            GridDirty = true;
        }

        void CellGrid::GenerateGrid()
        {
            if(!GridDirty)
                return;
            //RegenerateGrid();
            DrawGrid();
        }

        void CellGrid::SetWorkAreaLimits(const Vector2& AreaLimit, bool Relative)
        {
            if(Relative) WorkAreaLimits = AreaLimit * ParentScreen->GetViewportDimensions();
            else WorkAreaLimits = AreaLimit;
        }

        Vector2 CellGrid::GetWorkAreaLimits(bool Relative)
        {
            if(Relative) return WorkAreaLimits / ParentScreen->GetViewportDimensions();
            else return WorkAreaLimits;
        }

        void CellGrid::SetEdgeSpacing(const Vector2& Spacing, bool Relative)
        {
            if(Relative) EdgeSpacing = Spacing * ParentScreen->GetViewportDimensions();
            else EdgeSpacing = Spacing;
        }

        Vector2 CellGrid::GetEdgeSpacing(bool Relative)
        {
            if(Relative) return EdgeSpacing / ParentScreen->GetViewportDimensions();
            else return EdgeSpacing;
        }

        void CellGrid::SetCellSpacing(const Vector2& Spacing, bool Relative)
        {
            if(Relative) CellSpacing = Spacing * ParentScreen->GetViewportDimensions();
            else CellSpacing = Spacing;
        }

        Vector2 CellGrid::GetCellSpacing(bool Relative)
        {
            if(Relative) return CellSpacing / ParentScreen->GetViewportDimensions();
            else return CellSpacing;
        }

        void CellGrid::SetGridOrdering(const CellGrid::OrderingStyle& GridOrdering)
        {
            Ordering = GridOrdering;
        }

        CellGrid::OrderingStyle CellGrid::GetGridOrdering()
        {
            return Ordering;
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
            RelPosition = Position / ParentScreen->GetViewportDimensions();
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
            RelSize = Size / ParentScreen->GetViewportDimensions();
            GridBack->SetActualSize(Size);
            DrawGrid();
        }

        void CellGrid::UpdateDimensions()
        {
            WidgetResult Result = ViewportUpdateTool::UpdateWidget(this);
            RelPosition = Result.first / ViewportUpdateTool::GetNewSize();
            RelSize = Result.second / ViewportUpdateTool::GetNewSize();
            Real Scale = ViewportUpdateTool::GetNewSize().Y / ViewportUpdateTool::GetOldSize().Y;
            GridBack->UpdateDimensions();
            WorkAreaLimits*=Scale;
            FixedCellSize*=Scale;
            EdgeSpacing*=Scale;
            CellSpacing*=Scale;
            for( CellList::iterator it = Cells.begin() ; it != Cells.end() ; it++ )
            {
                (*it)->UpdateDimensions();
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
}//Mezzanine

#endif
