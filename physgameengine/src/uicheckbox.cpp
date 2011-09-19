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
#ifndef _uicheckbox_cpp
#define _uicheckbox_cpp

#include "uicheckbox.h"
#include "uimanager.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "uicaption.h"
#include "uibutton.h"
#include "inputquerytool.h"
#include "metacode.h"
#include "world.h"

namespace phys
{
    namespace UI
    {
        CheckBox::CheckBox(ConstString& name, const RenderableRect& Rect, const Real& LineHeight, ConstString& LabelText, Layer* PLayer)
            : Widget(name,PLayer),
              Checked(false),
              CheckLock(true)
        {
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            std::pair<Whole,Real> Result;
            if(Rect.Relative) Result = Manager->SuggestGlyphIndex((Whole)(LineHeight * WinDim.Y),Parent->GetParent()->GetPrimaryAtlas());
            else Result = Manager->SuggestGlyphIndex((Whole)LineHeight,Parent->GetParent()->GetPrimaryAtlas());
            GlyphIndex = Result.first;

            ConstructCheckBox(Rect,LabelText);
        }

        CheckBox::CheckBox(ConstString& name, const RenderableRect& Rect, const Whole& Glyph, ConstString& LabelText, Layer* PLayer)
            : Widget(name,PLayer),
              GlyphIndex(Glyph),
              Checked(false),
              CheckLock(true)
        {
            ConstructCheckBox(Rect,LabelText);
        }

        CheckBox::~CheckBox()
        {
            delete Box;
            if(Label)
                delete Label;
        }

        void CheckBox::ConstructCheckBox(const RenderableRect& Rect, ConstString& LabelText)
        {
            Type = Widget::W_CheckBox;
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
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

            Box = new Button(Name+"CB",BoxRect,Parent);
            Label = new Caption(Name+"CM",LabelRect,GlyphIndex,LabelText,Parent);
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

        void CheckBox::UpdateImpl(bool Force)
        {
            if(HoveredButton)
            {
                MetaCode::ButtonState State = InputQueryTool::GetMouseButtonState(1);
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

        void CheckBox::SetVisibleImpl(bool visible)
        {
            Box->SetVisible(visible);
            Label->SetVisible(visible);
        }

        bool CheckBox::CheckMouseHoverImpl()
        {
            if(Box->CheckMouseHover())
            {
                HoveredButton = Box;
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
            //UncheckedNorm = Unchecked;
            //UncheckedHovered = Hovered;
            SetSpriteSet(UncheckedSet);
        }

        void CheckBox::SetCheckedSprite(ConstString& Checked, ConstString& Hovered)
        {
            CheckedSet.first = Checked;
            CheckedSet.second = Hovered;
            //CheckedNorm = Checked;
            //CheckedHovered = Hovered;
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
            RelPosition = Position / Parent->GetParent()->GetViewportDimensions();
            Box->SetActualPosition(Position);

            Vector2 Adjusted = Position;
            Adjusted.X+=Box->GetActualSize().X;
            Label->SetActualPosition(Adjusted);
        }

        void CheckBox::SetSize(const Vector2& Size)
        {
            RelSize = Size;
            Box->SetSize(Size);

            Vector2 Adjusted = Size;
            Adjusted.X+=Box->GetPosition().X;
            Label->SetPosition(Adjusted);
        }

        void CheckBox::SetActualSize(const Vector2& Size)
        {
            RelSize = Size / Parent->GetParent()->GetViewportDimensions();
            Box->SetActualSize(Size);

            Vector2 Adjusted = Size;
            Adjusted.X+=Box->GetActualPosition().X;
            Label->SetActualPosition(Adjusted);
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
}//phys

#endif
