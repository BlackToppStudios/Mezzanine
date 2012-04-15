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
#ifndef _uilistbox_cpp
#define _uilistbox_cpp

#include "uilistbox.h"
#include "uimanager.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "uicaption.h"
#include "uirectangle.h"
#include "uiscrollbar.h"
#include "uiviewportupdatetool.h"
#include "inputquerytool.h"
#include "metacode.h"
#include "world.h"

#include <cmath>

namespace Mezzanine
{
    namespace UI
    {
        ListBox::ListBox(ConstString& name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollStyle, Layer* PLayer)
            : Widget(name,PLayer),
              Selected(NULL),
              AutoHideScroll(true),
              LastScrollValue(0),
              MaxDisplay(3),
              SelectionsAdded(1)
        {
            /// @todo Currently this class has little support for a border around the selections.
            /// Ideally when the UI system is more complete we'll be able to seemlessly move
            /// objects around different layers and thus have more control over z-order.  When
            /// that happens we should add a rect to this class be be placed over the selections
            /// for use with a border or other kind of decoration.
            Type = Widget::W_ListBox;

            // Set some sane template defaults
            SelectionTemplate.BackgroundColour = ColourValue(1.0,1.0,1.0,1.0);
            SelectionTemplate.TextColour = ColourValue(0.0,0.0,0.0,1.0);
            SelectionTemplate.TextScale = 1.0;
            SelectionTemplate.CursorOffset = 0.0;
            SelectionTemplate.HorizontalAlign = UI::Txt_Middle;
            SelectionTemplate.VerticalAlign = UI::Txt_Center;
            SelectionTemplate.Priority = UI::RP_Medium;

            RenderableRect ScrollRect, BoxRect;
            const Vector2& WinDim = ParentLayer->GetParent()->GetViewportDimensions();
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;

                SelectionTemplate.Size = Rect.Size * WinDim;

                ScrollRect.Position = Vector2((RelPosition.X + RelSize.X) - ((Rect.Size.Y * WinDim.Y) / WinDim.X),RelPosition.Y);
                ScrollRect.Size = Vector2((Rect.Size.Y * WinDim.Y) / WinDim.X,RelSize.Y * MaxDisplay);
                ScrollRect.Relative = Rect.Relative;
            }else{
                RelPosition = Rect.Position / WinDim;
                RelSize = Rect.Size / WinDim;

                SelectionTemplate.Size = Rect.Size;

                ScrollRect.Position = Vector2((Rect.Position.X + Rect.Size.X) - Rect.Size.Y,Rect.Position.Y);
                ScrollRect.Size = Vector2(Rect.Size.Y,Rect.Size.Y * MaxDisplay);
                ScrollRect.Relative = Rect.Relative;
            }
            BoxRect.Position = Rect.Position;
            BoxRect.Size.X = Rect.Size.X;
            BoxRect.Size.Y = Rect.Size.Y * MaxDisplay;
            BoxRect.Relative = Rect.Relative;

            BoxBack = new Rectangle(BoxRect,ParentLayer);
            VertScroll = new Scrollbar(Name+"Scr",ScrollRect,ScrollStyle,ParentLayer);
            VertScroll->Hide();

            AddSubRenderable(0,RenderablePair(BoxBack,NULL));
            AddSubRenderable(1,RenderablePair(NULL,VertScroll));
        }

        ListBox::~ListBox()
        {
            delete BoxBack;
            if(!Selections.empty())
            {
                for( std::vector<Caption*>::iterator it = Selections.begin() ; it != Selections.end() ; it++ )
                {
                    delete (*it);
                }
                Selections.clear();
            }
            VisibleSelections.clear();
        }

        void ListBox::ScrollerSizeCheck()
        {
            Real ScrollerSize = (Real)MaxDisplay / (Real)Selections.size();
            VertScroll->SetScrollerSize(ScrollerSize);
        }

        void ListBox::ScrollHideCheck()
        {
            if(!IsVisible())
            {
                VertScroll->Hide();
                return;
            }
            if(!AutoHideScroll)
            {
                VertScroll->Show();
                return;
            }
            if(Selections.size() > MaxDisplay)
                VertScroll->Show();
            else
                VertScroll->Hide();
        }

        void ListBox::SelectionSizeCheck(UI::Caption* Selection)
        {
            const Vector2& WinDim = ParentLayer->GetParent()->GetViewportDimensions();
            Vector2 CurrSize = Selection->GetActualSize();
            Vector2 TargetSize;
            if(VertScroll->IsVisible())
                TargetSize = Vector2(SelectionTemplate.Size.X - VertScroll->GetActualSize().X,SelectionTemplate.Size.Y);
            else
                TargetSize = SelectionTemplate.Size;
            if(CurrSize != TargetSize)
            {
                Selection->SetActualSize(TargetSize);
            }
        }

        void ListBox::SetArea(const Vector2& Area)
        {
            RelSize = Area / ParentLayer->GetParent()->GetViewportDimensions();
            BoxBack->SetActualSize(Area);
            Vector2 ScrollP((GetActualPosition().X + Area.X) - VertScroll->GetActualSize().X,GetActualPosition().Y);
            Vector2 ScrollS(VertScroll->GetActualSize().X,Area.Y);
            VertScroll->SetActualPosition(ScrollP);
            VertScroll->SetActualSize(ScrollS);
        }

        void ListBox::DrawList()
        {
            if(Selections.empty())
                return;
            ScrollHideCheck();
            VisibleSelections.clear();
            Whole FirstCaption = 0;
            if(MaxDisplay < Selections.size())
            {
                Real ToBeRounded = VertScroll->GetScrollerValue() * (Real)(Selections.size() - MaxDisplay);
                FirstCaption = (Whole)(ToBeRounded + 0.5);
            }
            Vector2 SelectionPos = GetActualPosition();
            Real ActualInc = SelectionTemplate.Size.Y;

            for( Whole w = 0 ; w < FirstCaption ; ++w )
            {
                Selections[w]->SetPosition(GetPosition());
                Selections[w]->Hide();
                SelectionSizeCheck(Selections[w]);
            }
            Whole Displayed = FirstCaption+MaxDisplay > Selections.size() ? Selections.size() : FirstCaption+MaxDisplay;
            for( Whole x = FirstCaption ; x < Displayed ; ++x )
            {
                VisibleSelections.push_back(Selections[x]);
                Selections[x]->SetVisible(this->IsVisible());
            }
            for( Whole y = Displayed ; y < Selections.size() ; ++y )
            {
                Selections[y]->SetPosition(GetPosition());
                Selections[y]->Hide();
                SelectionSizeCheck(Selections[y]);
            }
            for( Whole z = 0 ; z < VisibleSelections.size() ; ++z )
            {
                VisibleSelections[z]->SetActualPosition(SelectionPos);
                SelectionPos.Y+=ActualInc;
                SelectionSizeCheck(VisibleSelections[z]);
            }
        }

        void ListBox::UpdateImpl(bool Force)
        {
            MetaCode::ButtonState State = InputQueryTool::GetMouseButtonState(1);
            if(HoveredCaption)
            {
                if(MetaCode::BUTTON_PRESSING == State)
                {
                    SetSelected(HoveredCaption);
                }
            }
            if(LastScrollValue != VertScroll->GetScrollerValue())
            {
                DrawList();
                LastScrollValue = VertScroll->GetScrollerValue();
            }
        }

        void ListBox::SetVisibleImpl(bool visible)
        {
            BoxBack->SetVisible(visible);
            DrawList();
        }

        bool ListBox::CheckMouseHoverImpl()
        {
            for( std::vector<Caption*>::iterator it = VisibleSelections.begin() ; it != VisibleSelections.end() ; it++ )
            {
                if((*it)->CheckMouseHover())
                {
                    HoveredSubWidget = NULL;
                    HoveredButton = NULL;
                    HoveredCaption = (*it);
                    return true;
                }
            }
            if(VertScroll->CheckMouseHover())
            {
                HoveredSubWidget = VertScroll;
                HoveredButton = VertScroll->GetHoveredButton();
                HoveredCaption = NULL;
                return true;
            }
            else if(BoxBack->CheckMouseHover())
            {
                HoveredSubWidget = NULL;
                HoveredButton = NULL;
                HoveredCaption = NULL;
                return true;
            }
            HoveredCaption = NULL;
            return false;
        }

        ListBox& ListBox::SetTemplateSize(const Vector2& Size, bool Relative)
        {
            const Vector2& WinDim = ParentLayer->GetParent()->GetViewportDimensions();
            if(Relative)
            {
                this->SelectionTemplate.Size = Size * WinDim;
                Vector2 NewSize = Vector2(Size.X,Size.Y * MaxDisplay);
                SetArea(NewSize * WinDim);
            }else{
                this->SelectionTemplate.Size = Size;
                Vector2 NewSize = Vector2(Size.X,Size.Y * MaxDisplay);
                SetArea(NewSize);
            }
            return *this;
        }

        ListBox& ListBox::SetTemplateGlyphIndex(const Whole& Glyph)
        {
            this->SelectionTemplate.GlyphIndex = Glyph;
            return *this;
        }

        ListBox& ListBox::SetTemplateTextColour(const ColourValue& TextColour)
        {
            this->SelectionTemplate.TextColour = TextColour;
            return *this;
        }

        ListBox& ListBox::SetTemplateTextScale(const Real& Scale)
        {
            this->SelectionTemplate.TextScale = Scale;
            return *this;
        }

        ListBox& ListBox::SetTemplateCursorOffset(const Whole& Offset)
        {
            this->SelectionTemplate.CursorOffset = Offset;
            return *this;
        }

        ListBox& ListBox::SetTemplateBackgroundColour(const ColourValue& BackgroundColour)
        {
            this->SelectionTemplate.BackgroundColour = BackgroundColour;
            return *this;
        }

        ListBox& ListBox::SetTemplateHorizontalAlign(const UI::TextHorizontalAlign& HorAlign)
        {
            this->SelectionTemplate.HorizontalAlign = HorAlign;
            return *this;
        }

        ListBox& ListBox::SetTemplateVerticalAlign(const UI::TextVerticalAlign& VertAlign)
        {
            this->SelectionTemplate.VerticalAlign = VertAlign;
            return *this;
        }

        ListBox& ListBox::SetTemplateRenderPriority(const UI::RenderPriority& Priority)
        {
            this->SelectionTemplate.Priority = Priority;
            return *this;
        }

        const UI::TemplateParams& ListBox::GetTemplateInfo()
        {
            return this->SelectionTemplate;
        }

        Caption* ListBox::AddSelection(ConstString& name, ConstString &Text, ConstString& BackgroundSprite)
        {
            SelectionsAdded++;
            RenderableRect SelectionRect(RelPosition,SelectionTemplate.Size / ParentLayer->GetParent()->GetViewportDimensions(),true);
            Caption* Select = new Caption(name,SelectionRect,SelectionTemplate.GlyphIndex,Text,ParentLayer);
            if(!BackgroundSprite.empty())
                Select->SetBackgroundSprite(BackgroundSprite);
            if(SelectionTemplate.BackgroundColour != ColourValue(1.0,1.0,1.0,1.0))
                Select->SetBackgroundColour(SelectionTemplate.BackgroundColour);
            if(SelectionTemplate.CursorOffset != 0)
                Select->SetCursorOffset(SelectionTemplate.CursorOffset);
            if(SelectionTemplate.TextScale != 1)
                Select->SetTextScale(SelectionTemplate.TextScale);
            Select->SetTextColour(SelectionTemplate.TextColour);
            Select->HorizontallyAlign(SelectionTemplate.HorizontalAlign);
            Select->VerticallyAlign(SelectionTemplate.VerticalAlign);
            Select->SetRenderPriority(SelectionTemplate.Priority);
            Select->Hide();
            Selections.push_back(Select);
            AddSubRenderable(SelectionsAdded,RenderablePair(Select,NULL));
            ScrollerSizeCheck();
            DrawList();
            return Select;
        }

        Caption* ListBox::GetSelection(ConstString &Name)
        {
            for ( std::vector<Caption*>::iterator it = Selections.begin() ; it != Selections.end() ; it++ )
            {
                if ( Name == (*it)->GetName() )
                {
                    UI::Caption* button = (*it);
                    return button;
                }
            }
            return 0;
        }

        void ListBox::DestroySelection(Caption* ToBeDestroyed)
        {
            for ( std::vector<Caption*>::iterator it = Selections.begin() ; it != Selections.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it) )
                {
                    Selections.erase(it);
                    break;
                }
            }
            for ( RenderableMap::iterator it = SubRenderables.begin() ; it != SubRenderables.end() ; ++it )
            {
                if( (*it).second.first == ToBeDestroyed )
                {
                    SubRenderables.erase(it);
                    break;
                }
            }
            delete ToBeDestroyed;
        }

        void ListBox::DestroySelection(String& ToBeDestroyed)
        {
            for ( std::vector<Caption*>::iterator it = Selections.begin() ; it != Selections.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it)->GetName() )
                {
                    delete (*it);
                    Selections.erase(it);
                    return;
                }
            }
        }

        void ListBox::SetSelected(Caption* ToBeSelected)
        {
            /// @todo Maybe add a checker to verify the caption belongs to this widget?
            Selected = ToBeSelected;
        }

        void ListBox::SetMaxDisplayedSelections(const Whole& MaxSelections)
        {
            MaxDisplay = MaxSelections;
            Vector2 NewBackSize = SelectionTemplate.Size;
            NewBackSize.Y*=MaxDisplay;
            SetArea(NewBackSize);
            ScrollerSizeCheck();
        }

        void ListBox::SetAutoHideScroll(bool AutoHide)
        {
            AutoHideScroll = AutoHide;
            ScrollHideCheck();
        }

        void ListBox::SetPosition(const Vector2& Position)
        {
            RelPosition = Position;
            Vector2 ScrollOffset = VertScroll->GetPosition() - RelPosition;
            BoxBack->SetPosition(Position);
            VertScroll->SetPosition(Position + ScrollOffset);
            DrawList();
        }

        void ListBox::SetActualPosition(const Vector2& Position)
        {
            RelPosition = Position / ParentLayer->GetParent()->GetViewportDimensions();
            Vector2 ScrollOffset = VertScroll->GetActualPosition() - (RelPosition * ParentLayer->GetParent()->GetViewportDimensions());
            BoxBack->SetActualPosition(Position);
            VertScroll->SetActualPosition(Position + ScrollOffset);
            DrawList();
        }

        void ListBox::SetSize(const Vector2& Size)
        {
            // Size is set implicitly
        }

        void ListBox::SetActualSize(const Vector2& Size)
        {
            // Size is set implicitly
        }

        void ListBox::UpdateDimensions()
        {
            /*const Vector2& WinDim = ParentLayer->GetParent()->GetViewportDimensions();
            SetArea(RelSize * WinDim);
            SetActualPosition(RelPosition * WinDim);*/

            WidgetResult Result = ViewportUpdateTool::UpdateWidget(this);
            RelPosition = Result.first / ViewportUpdateTool::GetNewSize();
            RelSize = Result.second / ViewportUpdateTool::GetNewSize();
            Real Scale = ViewportUpdateTool::GetNewSize().Y / ViewportUpdateTool::GetOldSize().Y;
            BoxBack->UpdateDimensions();
            VertScroll->UpdateDimensions();
            SelectionTemplate.Size*=Scale;
            DrawList();
            SetPosition(RelPosition);
        }

        Caption* ListBox::GetSelected()
        {
            return Selected;
        }

        Rectangle* ListBox::GetBoxBack()
        {
            return BoxBack;
        }

        UI::Scrollbar* ListBox::GetVertScroll()
        {
            return VertScroll;
        }
    }//UI
}//Mezzanine

#endif
