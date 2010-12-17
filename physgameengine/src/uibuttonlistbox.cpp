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
#ifndef _uibuttonlistbox_cpp
#define _uibuttonlistbox_cpp

#include "uibuttonlistbox.h"
#include "uimanager.h"
#include "uilayer.h"
#include "uirectangle.h"
#include "uibutton.h"
#include "uitextbutton.h"
#include "inputquerytool.h"
#include "metacode.h"
#include "world.h"

#include <cmath>

namespace phys
{
    namespace UI
    {
        ButtonListBox::ButtonListBox(ConstString& name, Vector2 Position, Vector2 Size, Real ScrollbarWidth, Scrollbar::BarStyle ScrollbarStyle, UILayer* Layer)
            : Widget(name,Layer),
              Selected(NULL),
              AutoHideScroll(true),
              LastScrollValue(0),
              NumVisible(0),
              SelectionDist(0.025),
              BorderWidth(0),
              BorderColour(ColourValue(0,0,0,0))
        {
            Type = Widget::ButtonListBox;
            RelPosition = Position;
            RelSize = Size;

            BoxBack = new Rectangle(Position,Size,Layer);
            Vector2 ScrollP((RelPosition.X + RelSize.X) - ScrollbarWidth,RelPosition.Y);
            Vector2 ScrollS(ScrollbarWidth,RelSize.Y);
            /// @todo Fourth instance of needing to include the namespace in the declaration seemingly needlessly.
            VertScroll = new UI::Scrollbar(Name+"Scr",ScrollP,ScrollS,ScrollbarStyle,Layer);
            VertScroll->Hide();
        }

        ButtonListBox::~ButtonListBox()
        {
            delete BoxBack;
            if(!Selections.empty())
            {
                for( std::vector<Button*>::iterator it = Selections.begin() ; it != Selections.end() ; it++ )
                {
                    delete (*it);
                }
                Selections.clear();
            }
            VisibleSelections.clear();
        }

        void ButtonListBox::CalculateVisibleSelections()
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

        void ButtonListBox::DrawList()
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
            Whole FirstButton = (Whole)(Remainder >= 0.5 ? ToBeRounded + (One - Remainder) : ToBeRounded - Remainder);
            Vector2 SelectionPos = GetActualPosition();
            Real ActualDist = SelectionDist * Manager->GetWindowDimensions().Y;
            Real ActualInc = ActualDist + (TSize.Y * Manager->GetWindowDimensions().Y);
            SelectionPos.X+=ActualDist;
            SelectionPos.Y+=ActualDist;
            for( Whole w = 0 ; w < FirstButton ; w++ )
            {
                Selections[w]->SetPosition(GetPosition());
                Selections[w]->Hide();
                if(0 != NewSize.X && 0 != NewSize.Y)
                    Selections[w]->SetSize(NewSize);
            }
            Whole Displayed = FirstButton+NumVisible > Selections.size() ? Selections.size() : FirstButton+NumVisible;
            for( Whole x = FirstButton ; x < Displayed ; x++ )
            {
                VisibleSelections.push_back(Selections[x]);
                Selections[x]->Show();
            }
            for( Whole y = FirstButton+NumVisible ; y < Selections.size() ; y++ )
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

        void ButtonListBox::Update(bool Force)
        {
            if(!Force)
                SubWidgetUpdate();
            MetaCode::ButtonState State = Manager->GetInputQueryer()->GetMouseButtonState(1);
            if(HoveredButton)
            {
                if(MetaCode::BUTTON_PRESSING == State)
                {
                    if(Selected)
                        Selected->NoBorder();
                    if(BorderWidth > 0)
                        HoveredButton->SetBorder(BorderWidth,BorderColour);
                    Selected = HoveredButton;
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

        void ButtonListBox::SetVisible(bool visible)
        {
            BoxBack->SetVisible(visible);
            if(!AutoHideScroll)
                VertScroll->SetVisible(visible);
            for(Whole x=0 ; x < VisibleSelections.size() ; x++)
                VisibleSelections[x]->SetVisible(visible);
            Visible = visible;
        }

        bool ButtonListBox::IsVisible()
        {
            return Visible;
        }

        void ButtonListBox::Show()
        {
            BoxBack->Show();
            if(!AutoHideScroll)
                VertScroll->Show();
            for(Whole x=0 ; x < VisibleSelections.size() ; x++)
                VisibleSelections[x]->Show();
            Visible = true;
        }

        void ButtonListBox::Hide()
        {
            BoxBack->Hide();
            if(!AutoHideScroll)
                VertScroll->Hide();
            for(Whole x=0 ; x < VisibleSelections.size() ; x++)
                VisibleSelections[x]->Hide();
            Visible = false;
        }

        bool ButtonListBox::CheckMouseHover()
        {
            if(!Visible)
                return false;
            for( std::vector<Button*>::iterator it = VisibleSelections.begin() ; it != VisibleSelections.end() ; it++ )
            {
                if((*it)->CheckMouseHover())
                {
                    HoveredSubWidget = NULL;
                    HoveredButton = (*it);
                    //Update();
                    return true;
                }
            }
            if(VertScroll->CheckMouseHover())
            {
                HoveredSubWidget = VertScroll;
                HoveredButton = NULL;
                //Update();
                return true;
            }
            else if(BoxBack->CheckMouseHover())
            {
                HoveredSubWidget = NULL;
                HoveredButton = NULL;
                //Update();
                return true;
            }
            HoveredSubWidget = NULL;
            HoveredButton = NULL;
            return false;
        }

        void ButtonListBox::SetTemplateParameters(Vector2 Size, Whole Glyph)
        {
            if(Size.X > RelSize.X)
                Size.X = RelSize.X;
            if(Size.Y > RelSize.Y)
                Size.Y = RelSize.Y;
            TSize = Size;
            TGlyph = Glyph;
        }

        Button* ButtonListBox::AddSelection(ConstString& name, ConstString& BackgroundSprite, ConstString &TextLabel)
        {
            Button* Select = NULL;
            if(TextLabel.empty())
            {
                Select = new Button(name,GetPosition(),TSize,Parent);
            }else{
                Select = new TextButton(name,GetPosition(),TSize,TGlyph,TextLabel,Parent);
            }
            if(!BackgroundSprite.empty())
            {
                Select->SetBackgroundSprite(BackgroundSprite);
            }
            Select->Hide();
            Selections.push_back(Select);
            CalculateVisibleSelections();
            DrawList();
            return Select;
        }

        Button* ButtonListBox::GetSelection(ConstString &Name)
        {
            for ( std::vector<Button*>::iterator it = Selections.begin() ; it != Selections.end() ; it++ )
            {
                if ( Name == (*it)->GetName() )
                {
                    UI::Button* button = (*it);
                    return button;
                }
            }
            return 0;
        }

        void ButtonListBox::DestroySelection(Button* ToBeDestroyed)
        {
            for ( std::vector<Button*>::iterator it = Selections.begin() ; it != Selections.end() ; it++ )
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

        void ButtonListBox::DestroySelection(String& ToBeDestroyed)
        {
            for ( std::vector<Button*>::iterator it = Selections.begin() ; it != Selections.end() ; it++ )
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

        void ButtonListBox::SetSelectionDistance(Real Dist)
        {
            SelectionDist = Dist;
        }

        void ButtonListBox::SetAutoHideScroll(bool AutoHide)
        {
            AutoHideScroll = AutoHide;
            if(!AutoHide)
                VertScroll->Show();
        }

        void ButtonListBox::EnableBorderSelector(Real Width, ColourValue &Colour)
        {
            BorderWidth = Width;
            BorderColour = Colour;
        }

        void ButtonListBox::DisableBorderSelector()
        {
            BorderWidth = 0;
            BorderColour = ColourValue(0,0,0,0);
        }

        void ButtonListBox::SetPosition(Vector2 Position)
        {
            RelPosition = Position;
            Vector2 ScrollOffset = VertScroll->GetPosition() - RelPosition;
            BoxBack->SetPosition(Position);
            VertScroll->SetPosition(Position + ScrollOffset);
            DrawList();
        }

        Vector2 ButtonListBox::GetPosition()
        {
            return RelPosition;
        }

        void ButtonListBox::SetActualPosition(Vector2 Position)
        {
            RelPosition = Position / Manager->GetWindowDimensions();
            Vector2 ScrollOffset = VertScroll->GetActualPosition() - RelPosition * Manager->GetWindowDimensions();
            BoxBack->SetActualPosition(Position);
            VertScroll->SetPosition(Position + ScrollOffset);
            DrawList();
        }

        Vector2 ButtonListBox::GetActualPosition()
        {
            return RelPosition * Manager->GetWindowDimensions();
        }

        void ButtonListBox::SetSize(Vector2 Size)
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

        Vector2 ButtonListBox::GetSize()
        {
            return RelSize;
        }

        void ButtonListBox::SetActualSize(Vector2 Size)
        {
            RelSize = Size / Manager->GetWindowDimensions();
            BoxBack->SetActualSize(Size);
            Vector2 ScrollP((GetActualPosition().X + Size.X) - VertScroll->GetActualSize().X,GetActualPosition().Y);
            Vector2 ScrollS(VertScroll->GetActualSize().X,Size.Y);
            VertScroll->SetPosition(ScrollP);
            VertScroll->SetSize(ScrollS);
            CalculateVisibleSelections();
            DrawList();
        }

        Vector2 ButtonListBox::GetActualSize()
        {
            return RelSize * Manager->GetWindowDimensions();
        }

        Button* ButtonListBox::GetSelected()
        {
            return Selected;
        }

        Rectangle* ButtonListBox::GetBoxBack()
        {
            return BoxBack;
        }

        UI::Scrollbar* ButtonListBox::GetVertScroll()
        {
            return VertScroll;
        }
    }//UI
}//phys

#endif
