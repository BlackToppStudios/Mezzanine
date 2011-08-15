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
#ifndef _uilistbox_cpp
#define _uilistbox_cpp

#include "uilistbox.h"
#include "uimanager.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "uicaption.h"
#include "uirectangle.h"
#include "uiscrollbar.h"
#include "inputquerytool.h"
#include "metacode.h"
#include "world.h"

#include <cmath>

namespace phys
{
    namespace UI
    {
        ListBox::ListBox(ConstString& name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollStyle, Layer* PLayer)
            : Widget(name,PLayer),
              Selected(NULL),
              AutoHideScroll(true),
              LastScrollValue(0),
              NumVisible(0),
              MaxDisplay(3)
        {
            /// @todo Currently this class has little support for a border around the selections.
            /// Ideally when the UI system is more complete we'll be able to seemlessly move
            /// objects around different layers and thus have more control over z-order.  When
            /// that happens we should add a rect to this class be be placed over the selections
            /// for use with a border or other kind of decoration.
            Type = Widget::ListBox;

            // Set some sane template defaults
            SelectionTemplate.Size = Rect.Size;
            SelectionTemplate.BackgroundColour = ColourValue(1.0,1.0,1.0,1.0);
            SelectionTemplate.TextColour = ColourValue(0.0,0.0,0.0,1.0);
            SelectionTemplate.TextScale = 1.0;
            SelectionTemplate.CursorOffset = 0.0;
            SelectionTemplate.HorizontalAlign = UI::Txt_Middle;
            SelectionTemplate.VerticalAlign = UI::Txt_Center;

            RenderableRect ScrollRect;
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;
                const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();

                ScrollRect.Position = Vector2((RelPosition.X + RelSize.X) - ((Rect.Size.Y * WinDim.Y) / WinDim.X),RelPosition.Y);
                ScrollRect.Size = Vector2((Rect.Size.Y * WinDim.Y) / WinDim.X,RelSize.Y * 3);
                ScrollRect.Relative = Rect.Relative;
            }else{
                RelPosition = Rect.Position / Parent->GetParent()->GetViewportDimensions();
                RelSize = Rect.Size / Parent->GetParent()->GetViewportDimensions();

                ScrollRect.Position = Vector2((Rect.Position.X + Rect.Size.X) - Rect.Size.Y,Rect.Position.Y);
                ScrollRect.Size = Vector2(Rect.Size.Y,Rect.Size.Y * 3);
                ScrollRect.Relative = Rect.Relative;
            }

            BoxBack = new Rectangle(Rect,Parent);
            /// @todo Fourth instance of needing to include the namespace in the declaration seemingly needlessly.
            VertScroll = new UI::Scrollbar(Name+"Scr",ScrollRect,ScrollStyle,Parent);
            VertScroll->Hide();
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

        void ListBox::CalculateVisibleSelections()
        {
            NumVisible = (Whole)(RelSize.Y / SelectionTemplate.Size.Y);
            if(Selections.size() > NumVisible)
            {
                //if(Selections.size() > 0)
                VertScroll->SetScrollerSize((Real)NumVisible / (Real)Selections.size());
                if(AutoHideScroll)
                    VertScroll->Show();
            }else{
                if(AutoHideScroll)
                    VertScroll->Hide();
            }
        }

        void ListBox::DrawList()
        {
            if(Selections.empty())
                return;
            if(!Visible)
                return;
            Vector2 NewSize;
            if(VertScroll->IsVisible())
            {
                Real MaxWidth = RelSize.X - VertScroll->GetSize().X;
                if(SelectionTemplate.Size.X > MaxWidth)
                {
                    NewSize.X = MaxWidth;
                    NewSize.Y = SelectionTemplate.Size.Y;
                }
            }else{
                if(SelectionTemplate.Size.X > Selections[0]->GetSize().X)
                {
                    NewSize = SelectionTemplate.Size;
                }
            }
            VisibleSelections.clear();
            Real ToBeRounded = VertScroll->GetScrollerValue() * (Real)Selections.size();
            Real Remainder = fmod(ToBeRounded,(Real)1.0);
            Whole FirstCaption = (Whole)(Remainder >= 0.5 ? ToBeRounded + (1.0 - Remainder) : ToBeRounded - Remainder);
            Vector2 SelectionPos = GetActualPosition();
            Real ActualInc = SelectionTemplate.Size.Y * Parent->GetParent()->GetViewportDimensions().Y;

            for( Whole w = 0 ; w < FirstCaption ; w++ )
            {
                Selections[w]->SetPosition(GetPosition());
                Selections[w]->Hide();
                if(0 != NewSize.X && 0 != NewSize.Y)
                    Selections[w]->SetSize(NewSize);
            }
            Whole Displayed = FirstCaption+NumVisible > Selections.size() ? Selections.size() : FirstCaption+NumVisible;
            for( Whole x = FirstCaption ; x < Displayed ; x++ )
            {
                VisibleSelections.push_back(Selections[x]);
                Selections[x]->Show();
            }
            for( Whole y = FirstCaption+NumVisible ; y < Selections.size() ; y++ )
            {
                Selections[y]->SetPosition(GetPosition());
                Selections[y]->Hide();
                if(0 != NewSize.X && 0 != NewSize.Y)
                    Selections[y]->SetSize(NewSize);
            }
            for( Whole z = 0 ; z < VisibleSelections.size() ; z++ )
            {
                VisibleSelections[z]->SetActualPosition(SelectionPos);
                SelectionPos.Y+=ActualInc;
                if(0 != NewSize.X && 0 != NewSize.Y)
                    VisibleSelections[z]->SetSize(NewSize);
            }
        }

        void ListBox::Update(bool Force)
        {
            if(!Force)
                SubWidgetUpdate();
            MetaCode::ButtonState State = InputQueryTool::GetMouseButtonState(1);
            if(HoveredCaption)
            {
                if(MetaCode::BUTTON_PRESSING == State)
                {
                    Selected = HoveredCaption;
                }
            }
            else if(HoveredSubWidget)
            {
                if(MetaCode::BUTTON_PRESSING == State)
                {
                    SubWidgetFocus = HoveredSubWidget;
                }
            }
            if(SubWidgetFocus && (SubWidgetFocus != HoveredSubWidget))
            {
                SubWidgetFocusUpdate(true);
            }
            else if(MetaCode::BUTTON_DOWN == State && Force)
            {
                SubWidgetFocusUpdate(Force);
            }
            if(LastScrollValue != VertScroll->GetScrollerValue())
            {
                DrawList();
                LastScrollValue = VertScroll->GetScrollerValue();
            }
            if(MetaCode::BUTTON_LIFTING == State)
            {
                SubWidgetFocus = NULL;
            }
        }

        void ListBox::SetVisible(bool visible)
        {
            BoxBack->SetVisible(visible);
            if(!AutoHideScroll)
                VertScroll->SetVisible(visible);
            for(Whole x=0 ; x < VisibleSelections.size() ; x++)
                VisibleSelections[x]->SetVisible(visible);
            Visible = visible;
        }

        bool ListBox::IsVisible()
        {
            return Visible && Parent->IsVisible() && Parent->GetParent()->IsVisible();
        }

        void ListBox::Show()
        {
            BoxBack->Show();
            if(!AutoHideScroll)
                VertScroll->Show();
            for(Whole x=0 ; x < VisibleSelections.size() ; x++)
                VisibleSelections[x]->Show();
            Visible = true;
        }

        void ListBox::Hide()
        {
            BoxBack->Hide();
            if(!AutoHideScroll)
                VertScroll->Hide();
            for(Whole x=0 ; x < VisibleSelections.size() ; x++)
                VisibleSelections[x]->Hide();
            Visible = false;
        }

        bool ListBox::CheckMouseHover()
        {
            if(!IsVisible())
                return false;
            for( std::vector<Caption*>::iterator it = VisibleSelections.begin() ; it != VisibleSelections.end() ; it++ )
            {
                if((*it)->CheckMouseHover())
                {
                    HoveredSubWidget = NULL;
                    HoveredCaption = (*it);
                    return true;
                }
            }
            if(VertScroll->CheckMouseHover())
            {
                HoveredSubWidget = VertScroll;
                HoveredCaption = NULL;
                return true;
            }
            else if(BoxBack->CheckMouseHover())
            {
                HoveredSubWidget = NULL;
                HoveredCaption = NULL;
                return true;
            }
            HoveredSubWidget = NULL;
            HoveredCaption = NULL;
            return false;
        }

        ListBox& ListBox::SetTemplateSize(const Vector2& Size, bool Relative)
        {
            if(Relative) this->SelectionTemplate.Size = Size;
            else this->SelectionTemplate.Size = Size / Parent->GetParent()->GetViewportDimensions();
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

        const ListBox::TemplateParams& ListBox::GetTemplateInfo()
        {
            return this->SelectionTemplate;
        }

        Caption* ListBox::AddSelection(ConstString& name, ConstString &Text, ConstString& BackgroundSprite)
        {
            RenderableRect SelectionRect(GetPosition(),SelectionTemplate.Size,true);
            Caption* Select = new Caption(name,SelectionRect,SelectionTemplate.GlyphIndex,Text,Parent);
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
            Select->Hide();
            Selections.push_back(Select);
            CalculateVisibleSelections();
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
                    delete (*it);
                    Selections.erase(it);
                    CalculateVisibleSelections();
                    return;
                }
            }
        }

        void ListBox::DestroySelection(String& ToBeDestroyed)
        {
            for ( std::vector<Caption*>::iterator it = Selections.begin() ; it != Selections.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it)->GetName() )
                {
                    delete (*it);
                    Selections.erase(it);
                    CalculateVisibleSelections();
                    return;
                }
            }
        }

        void ListBox::SetMaxDisplayedSelections(const Whole& MaxSelections)
        {
            MaxDisplay = MaxSelections;
        }

        void ListBox::SetAutoHideScroll(bool AutoHide)
        {
            AutoHideScroll = AutoHide;
            if(!AutoHide)
                VertScroll->Show();
        }

        void ListBox::SetPosition(const Vector2& Position)
        {
            RelPosition = Position;
            Vector2 ScrollOffset = VertScroll->GetPosition() - RelPosition;
            BoxBack->SetPosition(Position);
            VertScroll->SetPosition(Position + ScrollOffset);
            DrawList();
        }

        Vector2 ListBox::GetPosition()
        {
            return RelPosition;
        }

        void ListBox::SetActualPosition(const Vector2& Position)
        {
            RelPosition = Position / Parent->GetParent()->GetViewportDimensions();
            Vector2 ScrollOffset = VertScroll->GetActualPosition() - RelPosition * Parent->GetParent()->GetViewportDimensions();
            BoxBack->SetActualPosition(Position);
            VertScroll->SetPosition(Position + ScrollOffset);
            DrawList();
        }

        Vector2 ListBox::GetActualPosition()
        {
            return RelPosition * Parent->GetParent()->GetViewportDimensions();
        }

        void ListBox::SetSize(const Vector2& Size)
        {
            RelSize = Size;
            BoxBack->SetSize(Size);
            Vector2 ScrollP((RelPosition.X + Size.X) - VertScroll->GetSize().X,RelPosition.Y);
            Vector2 ScrollS(VertScroll->GetSize().X,Size.Y);
            VertScroll->SetPosition(ScrollP);
            VertScroll->SetSize(ScrollS);
            CalculateVisibleSelections();
            DrawList();
        }

        Vector2 ListBox::GetSize()
        {
            return RelSize;
        }

        void ListBox::SetActualSize(const Vector2& Size)
        {
            RelSize = Size / Parent->GetParent()->GetViewportDimensions();
            BoxBack->SetActualSize(Size);
            Vector2 ScrollP((GetActualPosition().X + Size.X) - VertScroll->GetActualSize().X,GetActualPosition().Y);
            Vector2 ScrollS(VertScroll->GetActualSize().X,Size.Y);
            VertScroll->SetActualPosition(ScrollP);
            VertScroll->SetActualSize(ScrollS);
            CalculateVisibleSelections();
            DrawList();
        }

        Vector2 ListBox::GetActualSize()
        {
            return RelSize * Parent->GetParent()->GetViewportDimensions();
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
}//phys

#endif
