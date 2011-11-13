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
#ifndef _uitextbutton_cpp
#define _uitextbutton_cpp

#include "uitextbutton.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "uimanager.h"
#include "internalGorilla.h.cpp"

namespace phys
{
    namespace UI
    {
        TextButton::TextButton(ConstString& name, const RenderableRect& Rect, const Whole& Glyph, const String& Text, Layer* PLayer)
            : Button(name,Rect,PLayer)
        {
            AutoScaleText = false;
            RelLineHeight = 0.0;
            GorillaButton = Parent->GetGorillaLayer()->createCaption(Glyph,Ogre::Vector2(GorillaRectangle->left(),GorillaRectangle->top()),Ogre::Vector2(GorillaRectangle->width(),GorillaRectangle->height()),Text,*GorillaRectangle->GetNameFile());
            //GorillaButton->size(GorillaRectangle->width(),GorillaRectangle->height());
            GorillaButton->background_colour(Ogre::ColourValue(0,0,0,0));
            GorillaButton->align(Gorilla::TextAlign_Centre);
            GorillaButton->vertical_align(Gorilla::VerticalAlign_Middle);
        }

        TextButton::TextButton(ConstString& name, const RenderableRect& Rect, const Real& LineHeight, const String& Text, Layer* PLayer)
            : Button(name,Rect,PLayer)
        {
            AutoScaleText = true;
            RelLineHeight = LineHeight;
            std::pair<Whole,Real> Result = Manager->SuggestGlyphIndex(LineHeight * Parent->GetParent()->GetViewportDimensions().Y,*GorillaRectangle->GetNameFile());
            GorillaButton = Parent->GetGorillaLayer()->createCaption(Result.first,Ogre::Vector2(GorillaRectangle->left(),GorillaRectangle->top()),Ogre::Vector2(GorillaRectangle->width(),GorillaRectangle->height()),Text,*GorillaRectangle->GetNameFile());
            //GorillaButton->size(GorillaRectangle->width(),GorillaRectangle->height());
            GorillaButton->background_colour(Ogre::ColourValue(0,0,0,0));
            GorillaButton->align(Gorilla::TextAlign_Centre);
            GorillaButton->vertical_align(Gorilla::VerticalAlign_Middle);
            SetTextScale(Result.second);
        }

        TextButton::~TextButton()
        {
            Parent->GetGorillaLayer()->destroyCaption(GorillaButton);
        }

        void TextButton::SetVisible(bool Visible)
        {
            GorillaRectangle->SetVisible(Visible);
            GorillaButton->SetVisible(Visible);
        }

        bool TextButton::IsVisible()
        {
            return GorillaRectangle->IsVisible();
        }

        void TextButton::Show()
        {
            GorillaRectangle->Show();
            GorillaButton->Show();
        }

        void TextButton::Hide()
        {
            GorillaRectangle->Hide();
            GorillaButton->Hide();
        }

        bool TextButton::IsTextButton()
        {
            return true;
        }

        void TextButton::SetText(ConstString& Text)
        {
            GorillaButton->text(Text);
        }

        String TextButton::GetText()
        {
            return GorillaButton->text();
        }

        void TextButton::SetTextScale(const Real& Scale)
        {
            GorillaButton->SetCharScaling(Scale);
        }

        Real TextButton::GetTextScale()
        {
            return GorillaButton->GetCharScaling();
        }

        void TextButton::SetGlyphIndex(const Whole& GlyphIndex)
        {
            Glyphs = GlyphIndex;
            GorillaButton->font(GlyphIndex,*GorillaButton->GetNameFile());
        }

        void TextButton::SetGlyphIndex(const Whole& GlyphIndex, const String& Atlas)
        {
            Glyphs = GlyphIndex;
            GorillaButton->font(GlyphIndex,Atlas);
        }

        Whole TextButton::GetGlyphIndex()
        {
            return Glyphs;
        }

        void TextButton::HorizontallyAlign(const UI::TextHorizontalAlign& Align)
        {
            Gorilla::TextAlignment HA;
            switch (Align)
            {
                case UI::Txt_Left:
                    HA = Gorilla::TextAlign_Left;
                    break;
                case UI::Txt_Right:
                    HA = Gorilla::TextAlign_Right;
                    break;
                case UI::Txt_Middle:
                    HA = Gorilla::TextAlign_Centre;
                    break;
                default:
                    return;
            }
            GorillaButton->align(HA);
        }

        void TextButton::VerticallyAlign(const UI::TextVerticalAlign& Align)
        {
            Gorilla::VerticalAlignment VA;
            switch (Align)
            {
                case UI::Txt_Top:
                    VA = Gorilla::VerticalAlign_Top;
                    break;
                case UI::Txt_Bottom:
                    VA = Gorilla::VerticalAlign_Bottom;
                    break;
                case UI::Txt_Center:
                    VA = Gorilla::VerticalAlign_Middle;
                    break;
                default:
                    return;
            }
            GorillaButton->vertical_align(VA);
        }

        void TextButton::SetPosition(const Vector2& Position)
        {
            RelPosition = Position;
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            GorillaButton->left(WinDim.X * RelPosition.X);
            GorillaButton->top(WinDim.Y * RelPosition.Y);
            GorillaRectangle->left(WinDim.X * RelPosition.X);
            GorillaRectangle->top(WinDim.Y * RelPosition.Y);
        }

        Vector2 TextButton::GetPosition()
        {
            return RelPosition;
        }

        void TextButton::SetActualPosition(const Vector2& Position)
        {
            GorillaButton->left(Position.X);
            GorillaButton->top(Position.Y);
            GorillaRectangle->left(Position.X);
            GorillaRectangle->top(Position.Y);
        }

        Vector2 TextButton::GetActualPosition()
        {
            Vector2 Pos(GorillaButton->left(), GorillaButton->top());
            return Pos;
        }

        void TextButton::SetSize(const Vector2& Size)
        {
            RelSize = Size;
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            GorillaButton->left(WinDim.X * RelSize.X);
            GorillaButton->top(WinDim.Y * RelSize.Y);
            GorillaRectangle->left(WinDim.X * RelSize.X);
            GorillaRectangle->top(WinDim.Y * RelSize.Y);
        }

        Vector2 TextButton::GetSize()
        {
            return RelSize;
        }

        void TextButton::SetActualSize(const Vector2& Size)
        {
            GorillaButton->width(Size.X);
            GorillaButton->height(Size.Y);
            GorillaRectangle->width(Size.X);
            GorillaRectangle->height(Size.Y);
        }

        Vector2 TextButton::GetActualSize()
        {
            Vector2 Pos(GorillaButton->width(), GorillaButton->height());
            return Pos;
        }

        void TextButton::SetRenderPriority(const UI::RenderPriority& Priority)
        {
            Gorilla::RenderPriority RP;
            switch(Priority)
            {
                case UI::RP_Low:
                    RP = Gorilla::RP_Low;
                    break;
                case UI::RP_Medium:
                    RP = Gorilla::RP_Medium;
                    break;
                case UI::RP_High:
                    RP = Gorilla::RP_High;
                    break;
                default:
                    break;
            }
            GorillaButton->RenderPriority(RP);
            GorillaRectangle->RenderPriority(RP);
        }

        UI::RenderPriority TextButton::GetRenderPriority()
        {
            Gorilla::RenderPriority RP = this->GorillaRectangle->RenderPriority();
            switch(RP)
            {
                case Gorilla::RP_Low:
                    return UI::RP_Low;
                    break;
                case Gorilla::RP_Medium:
                    return UI::RP_Medium;
                    break;
                case Gorilla::RP_High:
                    return UI::RP_High;
                    break;
                default:
                    break;
            }
            return UI::RP_Medium;
        }

        void TextButton::SetAutoScaleText(bool Enable)
        {
            AutoScaleText = Enable;
        }

        bool TextButton::GetAutoScaleText()
        {
            return AutoScaleText;
        }

        void TextButton::UpdateDimensions()
        {
            Button::UpdateDimensions();
            if(AutoScaleText)
            {
                std::pair<Whole,Real> Result = Manager->SuggestGlyphIndex(RelLineHeight * Parent->GetParent()->GetViewportDimensions().Y,GetPrimaryAtlas());
                SetGlyphIndex(Result.first);
                SetTextScale(Result.second);
            }
        }
    }
}

#endif
