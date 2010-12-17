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
#ifndef _uicheckbox_cpp
#define _uicheckbox_cpp

#include "uicheckbox.h"
#include "uimanager.h"
#include "uilayer.h"
#include "uimarkuptext.h"
#include "uibutton.h"
#include "inputquerytool.h"
#include "metacode.h"
#include "world.h"

namespace phys
{
    namespace UI
    {
        CheckBox::CheckBox(ConstString& name, Vector2 Position, Vector2 Size, Whole Glyph, ConstString &LabelText, UILayer* Layer)
            : Widget(name,Layer),
              GlyphIndex(Glyph),
              Checked(false),
              CheckLock(true)
        {
            Type = Widget::CheckBox;
            RelPosition = Position;
            RelSize = Size;

            Box = new Button(Name+"CB",Position,Size,Layer);
            if(LabelText.empty())
            {
                Label = NULL;
            }else{
                Position.X+=Size.X;
                Label = new MarkupText(Name+"CM",Position,GlyphIndex,LabelText,Layer);
            }
        }

        CheckBox::~CheckBox()
        {
            delete Box;
            if(Label)
                delete Label;
        }

        void CheckBox::SetSpriteSet(std::pair<std::string,std::string>& SpriteSet)
        {
            Box->SetBackgroundSprite(SpriteSet.first);
            if(!SpriteSet.second.empty())
                Box->SetHoveredSprite(SpriteSet.second);
        }

        /*void CheckBox::SetUncheckedSprites()
        {
            Box->SetBackgroundSprite(UncheckedNorm);
            if(!UncheckedHovered.empty())
                Box->SetHoveredSprite(UncheckedHovered);
        }

        void CheckBox::SetCheckedSprites()
        {
            Box->SetBackgroundSprite(CheckedNorm);
            if(!CheckedHovered.empty())
                Box->SetHoveredSprite(CheckedHovered);
        }*/

        void CheckBox::Update(bool Force)
        {
            if(HoveredButton || Force)
            {
                MetaCode::ButtonState State = Manager->GetInputQueryer()->GetMouseButtonState(1);
                if(MetaCode::BUTTON_PRESSING == State)
                {
                    CheckLock = false;
                }
                else if(MetaCode::BUTTON_LIFTING && !CheckLock)
                {
                    Checked = !Checked;
                    if(Checked)
                    {
                        SetSpriteSet(CheckedSet);
                    }else{
                        SetSpriteSet(UncheckedSet);
                    }
                    CheckLock = true;
                }
            }
        }

        void CheckBox::SetVisible(bool visible)
        {
            Box->SetVisible(visible);
            if(Label)
                Label->SetVisible(visible);
            Visible = visible;
        }

        bool CheckBox::IsVisible()
        {
            return Visible;
        }

        void CheckBox::Show()
        {
            Box->Show();
            if(Label)
                Label->Show();
            Visible = true;
        }

        void CheckBox::Hide()
        {
            Box->Hide();
            if(Label)
                Label->Hide();
            Visible = false;
        }

        bool CheckBox::IsChecked()
        {
            return Checked;
        }

        void CheckBox::ManualCheck(bool Check)
        {
            if(Checked==Check)
                return;
            Checked = Check;
            if(Checked)
            {
                SetSpriteSet(CheckedSet);
            }else{
                SetSpriteSet(UncheckedSet);
            }
        }

        bool CheckBox::CheckMouseHover()
        {
            if(!Visible)
                return false;
            if(Box->CheckMouseHover())
            {
                HoveredButton = Box;
                //Update();
                return true;
            }
            HoveredButton = NULL;
            return false;
        }

        void CheckBox::SetLabelText(String &LabelText)
        {
            if(!Label)
            {
                Vector2 Position = Box->GetPosition();
                Position.X+=Box->GetSize().X;
                Label = new MarkupText(Name+"CM",Position,GlyphIndex,LabelText,Parent);
                Label->SetVisible(Box->IsVisible());
            }else{
                Label->SetText(LabelText);
            }
        }

        String CheckBox::GetLabelText()
        {
            if(!Label)
            {
                String Text = "";
                return Text;
            }else{
                return Label->GetText();
            }
        }

        void CheckBox::SetUncheckedSprite(ConstString& Unchecked, ConstString& Hovered)
        {
            UncheckedSet.first = Unchecked;
            UncheckedSet.second = Hovered;
            //UncheckedNorm = Unchecked;
            //UncheckedHovered = Hovered;
        }

        void CheckBox::SetCheckedSprite(ConstString& Checked, ConstString& Hovered)
        {
            CheckedSet.first = Checked;
            CheckedSet.second = Hovered;
            //CheckedNorm = Checked;
            //CheckedHovered = Hovered;
        }

        void CheckBox::SetPosition(Vector2 Position)
        {
            RelPosition = Position;
            Box->SetPosition(Position);
            if(Label)
            {
                Position.X+=Box->GetSize().X;
                Label->SetPosition(Position);
            }
        }

        Vector2 CheckBox::GetPosition()
        {
            return RelPosition;
        }

        void CheckBox::SetActualPosition(Vector2 Position)
        {
            RelPosition = Position / Manager->GetWindowDimensions();
            Box->SetActualPosition(Position);
            if(Label)
            {
                Position.X+=Box->GetSize().X;
                Label->SetActualPosition(Position);
            }
        }

        Vector2 CheckBox::GetActualPosition()
        {
            return RelPosition * Manager->GetWindowDimensions();
        }

        void CheckBox::SetSize(Vector2 Size)
        {
            RelSize = Size;
            Box->SetSize(Size);
            if(Label)
            {
                Size.X+=Box->GetPosition().X;
                Label->SetPosition(Size);
            }
        }

        Vector2 CheckBox::GetSize()
        {
            return RelSize;
        }

        void CheckBox::SetActualSize(Vector2 Size)
        {
            RelSize = Size / Manager->GetWindowDimensions();
            Box->SetActualSize(Size);
            if(Label)
            {
                Size.X+=Box->GetActualPosition().X;
                Label->SetActualPosition(Size);
            }
        }

        Vector2 CheckBox::GetActualSize()
        {
            return RelSize * Manager->GetWindowDimensions();
        }

        Button* CheckBox::GetCheckBoxButton()
        {
            return Box;
        }

        MarkupText* CheckBox::GetCheckBoxLabel()
        {
            return Label;
        }
    }//UT
}//phys

#endif
