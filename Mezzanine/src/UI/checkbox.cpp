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
#ifndef _uicheckbox_cpp
#define _uicheckbox_cpp

#include "uimanager.h"
#include "UI/checkbox.h"
#include "UI/screen.h"
#include "UI/caption.h"
#include "UI/button.h"
#include "UI/viewportupdatetool.h"
#include "inputmanager.h"
#include "Input/metacode.h"
#include "Input/mouse.h"
#include "entresol.h"

namespace Mezzanine
{
    namespace UI
    {
        CheckBox::CheckBox(ConstString& name, const RenderableRect& Rect, const Real& LineHeight, ConstString& LabelText, Screen* PScreen)
            : Widget(name,PScreen),
              Checked(false),
              CheckLock(true)
        {
            Type = Widget::W_CheckBox;
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            RenderableRect BoxRect, LabelRect;
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;

                BoxRect.Position = Rect.Position;
                BoxRect.Size.X = (Rect.Size.Y * WinDim.Y) / WinDim.X;
                BoxRect.Size.Y = Rect.Size.Y;
                BoxRect.Relative = Rect.Relative;
            }else{
                RelPosition = Rect.Position / WinDim;
                RelSize = Rect.Size / WinDim;

                BoxRect.Position = Rect.Position;
                BoxRect.Size.X = Rect.Size.Y;
                BoxRect.Size.Y = Rect.Size.Y;
                BoxRect.Relative = Rect.Relative;
            }
            LabelRect.Position.X = Rect.Position.X + BoxRect.Size.X;
            LabelRect.Position.Y = Rect.Position.Y;
            LabelRect.Size.X = Rect.Size.X - BoxRect.Size.X;
            LabelRect.Size.Y = Rect.Size.Y;
            LabelRect.Relative = Rect.Relative;

            Box = ParentScreen->CreateButton(Name+"CB",BoxRect);
            Label = ParentScreen->CreateCaption(Name+"CM",LabelRect,LineHeight,LabelText);
            AddSubRenderable(0,Box);
            AddSubRenderable(1,Label);
        }

        CheckBox::CheckBox(ConstString& name, const RenderableRect& Rect, const Whole& Glyph, ConstString& LabelText, Screen* PScreen)
            : Widget(name,PScreen),
              GlyphIndex(Glyph),
              Checked(false),
              CheckLock(true)
        {
            Type = Widget::W_CheckBox;
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            RenderableRect BoxRect, LabelRect;
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;

                BoxRect.Position = Rect.Position;
                BoxRect.Size.X = (Rect.Size.Y * WinDim.Y) / WinDim.X;
                BoxRect.Size.Y = Rect.Size.Y;
                BoxRect.Relative = Rect.Relative;
            }else{
                RelPosition = Rect.Position / WinDim;
                RelSize = Rect.Size / WinDim;

                BoxRect.Position = Rect.Position;
                BoxRect.Size.X = Rect.Size.Y;
                BoxRect.Size.Y = Rect.Size.Y;
                BoxRect.Relative = Rect.Relative;
            }
            LabelRect.Position.X = Rect.Position.X + BoxRect.Size.X;
            LabelRect.Position.Y = Rect.Position.Y;
            LabelRect.Size.X = Rect.Size.X - BoxRect.Size.X;
            LabelRect.Size.Y = Rect.Size.Y;
            LabelRect.Relative = Rect.Relative;

            Box = ParentScreen->CreateButton(Name+"CB",BoxRect);
            Label = ParentScreen->CreateCaption(Name+"CM",LabelRect,Glyph,LabelText);
            AddSubRenderable(0,Box);
            AddSubRenderable(1,Label);
        }

        CheckBox::~CheckBox()
        {
            ParentScreen->DestroyWidget(Box);
            ParentScreen->DestroyBasicRenderable(Label);
        }

        void CheckBox::SetSpriteSet(std::pair<std::string,std::string>& SpriteSet)
        {
            Box->GetClickable()->SetBackgroundSprite(SpriteSet.first);
            if(!SpriteSet.second.empty())
                Box->GetClickable()->SetHoveredSprite(SpriteSet.second);
        }

        void CheckBox::UpdateImpl(bool Force)
        {
            if( HoveredSubWidget && (Widget::W_Button == HoveredSubWidget->GetType()) )
            {
                Input::ButtonState State = InputManager::GetSingletonPtr()->GetSystemMouse()->GetButtonState(1);
                if(Input::BUTTON_PRESSING == State)
                {
                    CheckLock = false;
                }
                else if(Input::BUTTON_LIFTING && !CheckLock)
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

        void CheckBox::SetVisibleImpl(bool visible)
        {
            Box->SetVisible(visible);
            Label->SetVisible(visible);
        }

        bool CheckBox::CheckMouseHoverImpl()
        {
            if(Box->CheckMouseHover())
            {
                HoveredSubWidget = Box;
                return true;
            }
            return false;
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

        void CheckBox::SetLabelText(String& LabelText)
        {
            Label->SetText(LabelText);
        }

        String CheckBox::GetLabelText()
        {
            return Label->GetText();
        }

        void CheckBox::SetUncheckedSprite(ConstString& Unchecked, ConstString& Hovered)
        {
            UncheckedSet.first = Unchecked;
            UncheckedSet.second = Hovered;
            SetSpriteSet(UncheckedSet);
        }

        void CheckBox::SetCheckedSprite(ConstString& Checked, ConstString& Hovered)
        {
            CheckedSet.first = Checked;
            CheckedSet.second = Hovered;
        }

        void CheckBox::SetPosition(const Vector2& Position)
        {
            RelPosition = Position;
            Box->SetPosition(Position);

            Vector2 Adjusted = Position;
            Adjusted.X+=Box->GetSize().X;
            Label->SetPosition(Adjusted);
        }

        void CheckBox::SetActualPosition(const Vector2& Position)
        {
            RelPosition = Position / ParentScreen->GetViewportDimensions();
            Box->SetActualPosition(Position);

            Vector2 Adjusted = Position;
            Adjusted.X+=Box->GetActualSize().X;
            Label->SetActualPosition(Adjusted);
        }

        void CheckBox::SetSize(const Vector2& Size)
        {
            RelSize = Size;
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            Box->SetSize(Vector2((Size.Y * WinDim.Y) / WinDim.X,Size.Y));
            Label->SetSize(Vector2(Size.X - ((Size.Y * WinDim.Y) / WinDim.X),Size.Y));
            this->SetPosition(GetPosition());
        }

        void CheckBox::SetActualSize(const Vector2& Size)
        {
            RelSize = Size / ParentScreen->GetViewportDimensions();
            Box->SetActualSize(Vector2(Size.Y,Size.Y));
            Label->SetActualSize(Vector2(Size.X - Size.Y,Size.Y));
            this->SetActualPosition(GetActualPosition());
        }

        void CheckBox::UpdateDimensions()
        {
            WidgetResult Result = ViewportUpdateTool::UpdateWidget(this);
            RelPosition = Result.first / ViewportUpdateTool::GetNewSize();
            RelSize = Result.second / ViewportUpdateTool::GetNewSize();
            Box->UpdateDimensions();
            Label->UpdateDimensions();
            SetPosition(RelPosition);
        }

        Button* CheckBox::GetCheckBoxButton()
        {
            return Box;
        }

        Caption* CheckBox::GetLabel()
        {
            return Label;
        }
    }//UT
}//Mezzanine

#endif
