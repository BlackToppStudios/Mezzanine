//© Copyright 2010 BlackTopp Studios Inc.
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
        ListBox::ListBox(ConstString& name, const Vector2 Position, const Vector2 Size, const Real ScrollbarWidth, UI::ScrollbarStyle ScrollStyle, UILayer* Layer)
            : Widget(name,Layer),
              Selected(NULL),
              AutoHideScroll(true),
              LastScrollValue(0),
              NumVisible(0),
              SelectionDist(0.0),
              THAlign(UI::Txt_Middle),
              TVAlign(UI::Txt_Center),
              TTColour(ColourValue(0.0,0.0,0.0,1.0)),
              TBGColour(ColourValue(1.0,1.0,1.0,1.0)),
              SelectColour(ColourValue(1.0,1.0,1.0,1.0))
        {
            Type = Widget::ListBox;
            RelPosition = Position;
            RelSize = Size;

            BoxBack = new Rectangle(Position,Size,Layer);
            Vector2 ScrollP((RelPosition.X + RelSize.X) - ScrollbarWidth,RelPosition.Y);
            Vector2 ScrollS(ScrollbarWidth,RelSize.Y);
            /// @todo Fourth instance of needing to include the namespace in the declaration seemingly needlessly.
            VertScroll = new UI::Scrollbar(Name+"Scr",ScrollP,ScrollS,ScrollStyle,Layer);
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
            NumVisible = (Whole)(RelSize.Y / (TSize.Y + SelectionDist));
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
                Real MaxWidth = RelSize.X - (VertScroll->GetSize().X + SelectionDist);
                if(TSize.X > MaxWidth)
                {
                    NewSize.X = MaxWidth;
                    NewSize.Y = TSize.Y;
                }
            }else{
                if(TSize.X > Selections[0]->GetSize().X)
                {
                    NewSize = TSize;
                }
            }
            VisibleSelections.clear();
            Real One = 1;
            Real ToBeRounded = VertScroll->GetScrollerValue() * (Real)Selections.size();
            Real Remainder = fmod(ToBeRounded,One);
            Whole FirstCaption = (Whole)(Remainder >= 0.5 ? ToBeRounded + (One - Remainder) : ToBeRounded - Remainder);
            Vector2 SelectionPos = GetActualPosition();
            Real ActualDist = SelectionDist * Parent->GetParent()->GetViewportDimensions().Y;
            Real ActualInc = ActualDist + (TSize.Y * Parent->GetParent()->GetViewportDimensions().Y);
            SelectionPos.X+=ActualDist;
            SelectionPos.Y+=ActualDist;
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
            MetaCode::ButtonState State = Manager->GetInputQueryer()->GetMouseButtonState(1);
            if(HoveredCaption)
            {
                if(MetaCode::BUTTON_PRESSING == State)
                {
                    if(Selected)
                        Selected->SetBackgroundColour(TBGColour);
                    if(SelectColour != ColourValue(1.0,1.0,1.0,1.0))
                        HoveredCaption->SetBackgroundColour(SelectColour);
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
            return Visible;
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
            if(!Visible)
                return false;
            for( std::vector<Caption*>::iterator it = VisibleSelections.begin() ; it != VisibleSelections.end() ; it++ )
            {
                if((*it)->CheckMouseHover())
                {
                    HoveredSubWidget = NULL;
                    HoveredCaption = (*it);
                    //Update();
                    return true;
                }
            }
            if(VertScroll->CheckMouseHover())
            {
                HoveredSubWidget = VertScroll;
                HoveredCaption = NULL;
                //Update();
                return true;
            }
            else if(BoxBack->CheckMouseHover())
            {
                HoveredSubWidget = NULL;
                HoveredCaption = NULL;
                //Update();
                return true;
            }
            HoveredSubWidget = NULL;
            HoveredCaption = NULL;
            return false;
        }

        void ListBox::SetBasicTemplateParameters(Vector2 Size, const Whole Glyph)
        {
            if(Size.X > RelSize.X)
                Size.X = RelSize.X;
            if(Size.Y > RelSize.Y)
                Size.Y = RelSize.Y;
            TSize = Size;
            TGlyph = Glyph;
        }

        void ListBox::SetMoreTemplateParameters(const ColourValue& TextColour, const ColourValue& BackgroundColour, UI::TextHorizontalAlign HorAlign, UI::TextVerticalAlign VertAlign)
        {
            TTColour = TextColour;
            TBGColour = BackgroundColour;
            THAlign = HorAlign;
            TVAlign = VertAlign;
        }

        Caption* ListBox::AddSelection(ConstString& name, ConstString &Text, ConstString& BackgroundSprite)
        {
            Caption* Select = new Caption(name,GetPosition(),TSize,TGlyph,Text,Parent);
            if(!BackgroundSprite.empty())
                Select->SetBackgroundSprite(BackgroundSprite);
            if(TBGColour != ColourValue(1.0,1.0,1.0,1.0))
                Select->SetBackgroundColour(TBGColour);
            Select->SetTextColour(TTColour);
            Select->HorizontallyAlign(THAlign);
            Select->VerticallyAlign(TVAlign);
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

        void ListBox::SetSelectionDistance(const Real Dist)
        {
            SelectionDist = Dist;
        }

        void ListBox::SetAutoHideScroll(bool AutoHide)
        {
            AutoHideScroll = AutoHide;
            if(!AutoHide)
                VertScroll->Show();
        }

        void ListBox::EnableBackgroundSelector(const ColourValue &Colour)
        {
            SelectColour = Colour;
        }

        void ListBox::DisableBackgroundSelector()
        {
            SelectColour = ColourValue(1.0,1.0,1.0,1.0);
        }

        void ListBox::SetPosition(const Vector2 Position)
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

        void ListBox::SetActualPosition(const Vector2 Position)
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

        void ListBox::SetSize(const Vector2 Size)
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

        void ListBox::SetActualSize(const Vector2 Size)
        {
            RelSize = Size / Parent->GetParent()->GetViewportDimensions();
            BoxBack->SetActualSize(Size);
            Vector2 ScrollP((GetActualPosition().X + Size.X) - VertScroll->GetActualSize().X,GetActualPosition().Y);
            Vector2 ScrollS(VertScroll->GetActualSize().X,Size.Y);
            VertScroll->SetPosition(ScrollP);
            VertScroll->SetSize(ScrollS);
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
