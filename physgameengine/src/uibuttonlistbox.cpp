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

namespace phys
{
    namespace UI
    {
        ButtonListBox::ButtonListBox(String& name, Vector2 Position, Vector2 Size, Real ScrollbarWidth, Scrollbar::BarStyle ScrollbarStyle, UILayer* Layer)
            : Widget(name,Layer),
              AutoHideScroll(true),
              SelectionDist(0.025),
              BorderWidth(0),
              BorderColour(ColourValue(0,0,0,0))
        {
            Type = Widget::ButtonListBox;
            RelPosition = Position;
            RelSize = Size;

            BoxBack = new Rectangle(Position,Size,Layer);
            Vector2 ScrollP(RelPosition.X - ScrollbarWidth,RelPosition.Y);
            Vector2 ScrollS(ScrollbarWidth,RelSize.Y);
            /// @todo Fourth instance of needing to include the namespace in the declaration seemingly needlessly.
            VertScroll = new UI::Scrollbar(Name+"Scr",ScrollP,ScrollS,ScrollbarStyle,Parent);
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
                VertScroll->SetScrollerSize(NumVisible / Selections.size());
                if(AutoHideScroll)
                    VertScroll->Show();
            }else{
                if(AutoHideScroll)
                    VertScroll->Hide();
            }
        }

        void ButtonListBox::DrawList()
        {

        }

        void ButtonListBox::Update(bool Force)
        {

        }

        void ButtonListBox::SetVisible(bool Visible)
        {
            BoxBack->SetVisible(Visible);
        }

        bool ButtonListBox::IsVisible()
        {
            return BoxBack->IsVisible();
        }

        void ButtonListBox::Show()
        {
            BoxBack->Show();
        }

        void ButtonListBox::Hide()
        {
            BoxBack->Hide();
        }

        bool ButtonListBox::CheckMouseHover()
        {
            if(VertScroll->CheckMouseHover())
            {
                HoveredSubWidget = VertScroll;
                HoveredButton = NULL;
                Update();
                return true;
            }
            else if(BoxBack->CheckMouseHover())
            {
                HoveredSubWidget = NULL;
                HoveredButton = NULL;
                Update();
                return true;
            }
            return false;
        }

        void ButtonListBox::SetTemplateParameters(Vector2 Size, Whole Glyph)
        {
            TSize = Size;
            TGlyph = Glyph;
        }

        void ButtonListBox::AddSelection(String& name, String& BackgroundSprite, String &TextLabel)
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
        }

        void ButtonListBox::SetSelectionDistance(Real Dist)
        {
            SelectionDist = Dist;
        }

        void ButtonListBox::SetAutoHideScroll(bool AutoHide)
        {
            AutoHideScroll = AutoHide;
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
            BoxBack->SetPosition(Position);
        }

        Vector2 ButtonListBox::GetPosition()
        {
            return RelPosition;
        }

        void ButtonListBox::SetActualPosition(Vector2 Position)
        {
            RelPosition = Position / Manager->GetWindowDimensions();
            BoxBack->SetActualPosition(Position);
        }

        Vector2 ButtonListBox::GetActualPosition()
        {
            return RelPosition * Manager->GetWindowDimensions();
        }

        void ButtonListBox::SetSize(Vector2 Size)
        {
            RelSize = Size;
            BoxBack->SetSize(Size);
        }

        Vector2 ButtonListBox::GetSize()
        {
            return RelSize;
        }

        void ButtonListBox::SetActualSize(Vector2 Size)
        {
            RelSize = Size / Manager->GetWindowDimensions();
            BoxBack->SetActualSize(Size);
        }

        Vector2 ButtonListBox::GetActualSize()
        {
            return RelSize * Manager->GetWindowDimensions();
        }

        Rectangle* ButtonListBox::GetBoxBack()
        {
            return BoxBack;
        }
    }//UI
}//phys

#endif
