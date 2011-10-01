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
#ifndef _uidropdownlist_cpp
#define _uidropdownlist_cpp

#include "uidropdownlist.h"
#include "uilistbox.h"
#include "uicaption.h"
#include "uibutton.h"
#include "uirectangle.h"
#include "uiscrollbar.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "uimanager.h"
#include "inputquerytool.h"

namespace phys
{
    namespace UI
    {
        DropDownList::DropDownList(const String& name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle, Layer* parent)
            : Widget(name,parent),
              ToggleActivated(false)
        {
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            std::pair<Whole,Real> Result;
            if(Rect.Relative) Result = Manager->SuggestGlyphIndex((Whole)(LineHeight * WinDim.Y),Parent->GetParent()->GetPrimaryAtlas());
            else Result = Manager->SuggestGlyphIndex((Whole)LineHeight,Parent->GetParent()->GetPrimaryAtlas());

            ConstructDropDownList(Rect,Result.first,ScrollStyle);

            if(1.f != Result.second)
            {
                Selection->SetTextScale(Result.second);
                SelectionList->SetTemplateTextScale(Result.second);
            }
        }

        DropDownList::DropDownList(const String& name, const RenderableRect& Rect, const Whole& Glyph, const UI::ScrollbarStyle& ScrollStyle, Layer* parent)
            : Widget(name,parent),
              ToggleActivated(false)
        {
            ConstructDropDownList(Rect,Glyph,ScrollStyle);
        }

        DropDownList::~DropDownList()
        {
            delete Selection;
            delete ListToggle;
            delete SelectionList;
        }

        void DropDownList::ConstructDropDownList(const RenderableRect& Rect, const Whole& Glyph, const UI::ScrollbarStyle& ScrollStyle)
        {
            Type = Widget::W_DropDownList;
            RenderableRect SelectionRect, ListToggleRect, SelectionListRect;
            Real ScrollbarWidth;
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;

                SelectionRect.Position = Rect.Position;
                SelectionRect.Size.X = Rect.Size.X - ((Rect.Size.Y * WinDim.Y) / WinDim.X);
                SelectionRect.Size.Y = Rect.Size.Y;
                SelectionRect.Relative = Rect.Relative;

                ListToggleRect.Position.X = Rect.Position.X + SelectionRect.Size.X;
                ListToggleRect.Position.Y = Rect.Position.Y;
                ListToggleRect.Size.X = (Rect.Size.Y * WinDim.Y) / WinDim.X;
                ListToggleRect.Size.Y = Rect.Size.Y;
                ListToggleRect.Relative = Rect.Relative;

                ScrollbarWidth = (Rect.Size.Y * WinDim.Y) / WinDim.X;
            }else{
                RelPosition = Rect.Position / WinDim;
                RelSize = Rect.Size / WinDim;

                SelectionRect.Position = Rect.Position;
                SelectionRect.Size.X = Rect.Size.X - Rect.Size.Y;
                SelectionRect.Size.Y = Rect.Size.Y;
                SelectionRect.Relative = Rect.Relative;

                ListToggleRect.Position.X = Rect.Position.X + SelectionRect.Size.X;
                ListToggleRect.Position.Y = Rect.Position.Y;
                ListToggleRect.Size.X = Rect.Size.Y;
                ListToggleRect.Size.Y = Rect.Size.Y;
                ListToggleRect.Relative = Rect.Relative;

                ScrollbarWidth = Rect.Size.Y;
            }
            SelectionListRect.Position.X = Rect.Position.X;
            SelectionListRect.Position.Y = Rect.Position.Y + SelectionRect.Size.Y;
            SelectionListRect.Size = Rect.Size;
            SelectionListRect.Relative = Rect.Relative;

            Selection = new Caption(Name+"Select",SelectionRect,Glyph,"",Parent);
            ListToggle = new Button(Name+"Toggle",ListToggleRect,Parent);
            SelectionList = new UI::ListBox(Name+"List",SelectionListRect,ScrollStyle,Parent);

            SelectionList->SetTemplateGlyphIndex(Glyph);
            SelectionList->SetTemplateRenderPriority(UI::RP_High);
            SelectionList->Hide();
            SelectionList->GetBoxBack()->SetRenderPriority(UI::RP_High);
            SelectionList->GetVertScroll()->GetScrollBack()->SetRenderPriority(UI::RP_High);
            SelectionList->GetVertScroll()->GetScroller()->SetRenderPriority(UI::RP_High);
            SelectionList->GetVertScroll()->GetUpLeftButton()->SetRenderPriority(UI::RP_High);
            SelectionList->GetVertScroll()->GetDownRightButton()->SetRenderPriority(UI::RP_High);
        }

        void DropDownList::UpdateImpl(bool Force)
        {
            MetaCode::ButtonState State = InputQueryTool::GetMouseButtonState(1);
            if(HoveredButton == ListToggle)
            {
                if(MetaCode::BUTTON_PRESSING == State)
                {
                    if(!ToggleActivated)
                    {
                        SelectionList->Show();
                        ToggleActivated = true;
                    }else{
                        SelectionList->Hide();
                        ToggleActivated = false;
                    }
                }
            }
            else if(HoveredSubWidget == SelectionList)
            {
                if(MetaCode::BUTTON_PRESSING == State && !SelectionList->GetHoveredSubWidget())
                {
                    Selection->SetText(SelectionList->GetSelected()->GetText());
                    SelectionList->Hide();
                    ToggleActivated = false;
                }
            }
        }

        void DropDownList::SetVisibleImpl(bool visible)
        {
            Selection->SetVisible(visible);
            ListToggle->SetVisible(visible);
            if(ToggleActivated)
                SelectionList->SetVisible(visible);
        }

        bool DropDownList::CheckMouseHoverImpl()
        {
            if(Selection->CheckMouseHover())
            {
                HoveredSubWidget = NULL;
                HoveredButton = NULL;
                return true;
            }
            else if(ListToggle->CheckMouseHover())
            {
                HoveredSubWidget = NULL;
                HoveredButton = ListToggle;
                return true;
            }
            else if(SelectionList->CheckMouseHover())
            {
                HoveredSubWidget = SelectionList;
                HoveredButton = SelectionList->GetHoveredButton();
                return true;
            }
            return false;
        }

        void DropDownList::SetSelection(Caption* ToBeSelected)
        {
            Selection->SetText(ToBeSelected->GetText());
            SelectionList->SetSelected(ToBeSelected);
        }

        void DropDownList::SetSelection(const String& ToBeSelected)
        {
            SetSelection(SelectionList->GetSelection(ToBeSelected));
        }

        void DropDownList::SetPosition(const Vector2& Position)
        {
            RelPosition = Position;
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            Selection->SetPosition(Position);
            ListToggle->SetPosition(Position + Vector2(RelSize.X - ((RelSize.Y * WinDim.Y) / WinDim.X),0));
            SelectionList->SetPosition(Position + Vector2(0,RelSize.Y));
        }

        void DropDownList::SetActualPosition(const Vector2& Position)
        {
            RelPosition = Position / Parent->GetParent()->GetViewportDimensions();
            Vector2 CurrSize = GetActualSize();
            Selection->SetActualPosition(Position);
            ListToggle->SetActualPosition(Position + Vector2(CurrSize.X - CurrSize.Y,0));
            SelectionList->SetActualPosition(Position + Vector2(0,CurrSize.Y));
        }

        void DropDownList::SetSize(const Vector2& Size)
        {
            RelSize = Size;
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            Selection->SetSize(Vector2(Size.X - ((Size.Y * WinDim.Y) / WinDim.X),Size.Y));
            ListToggle->SetSize(Vector2((Size.Y * WinDim.Y) / WinDim.X,Size.Y));
            SelectionList->SetTemplateSize(Size);
            SetPosition(RelPosition);
        }

        void DropDownList::SetActualSize(const Vector2& Size)
        {
            RelSize = Size / Parent->GetParent()->GetViewportDimensions();
            Selection->SetActualSize(Vector2(Size.X - Size.Y,Size.Y));
            ListToggle->SetActualSize(Vector2(Size.Y,Size.Y));
            SelectionList->SetTemplateSize(Size,false);
            SetActualPosition(GetActualPosition());
        }

        void DropDownList::UpdateDimensions(const Vector2& OldViewportSize)
        {
            Selection->UpdateDimensions();
            ListToggle->UpdateDimensions();
            SelectionList->UpdateDimensions(OldViewportSize);
            SetPosition(RelPosition);
        }

        Caption* DropDownList::GetSelection()
        {
            return Selection;
        }

        Button* DropDownList::GetListToggle()
        {
            return ListToggle;
        }

        UI::ListBox* DropDownList::GetSelectionList()
        {
            return SelectionList;
        }
    }//ui
}//phys

#endif
