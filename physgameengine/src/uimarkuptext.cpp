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
#ifndef _uimarkuptext_cpp
#define _uimarkuptext_cpp

#include "uimarkuptext.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "uimanager.h"
#include "uiviewportupdatetool.h"
#include "world.h"

#include "internalGorilla.h.cpp"

namespace phys
{
    namespace UI
    {
        MarkupText::MarkupText(ConstString& name, const RenderableRect& Rect, const Whole& Glyph, const String& Text, Layer* PLayer)
            : BasicRenderable(name,PLayer)
        {
            AutoScaleText = false;
            RelLineHeight = 0.0;
            ConstructMarkupText(Rect,Glyph,Text);
        }

        MarkupText::MarkupText(ConstString& name, const RenderableRect& Rect, const Real& LineHeight, const String& Text, Layer* PLayer)
            : BasicRenderable(name,PLayer)
        {
            AutoScaleText = true;
            RelLineHeight = LineHeight;
            std::pair<Whole,Real> Result = Manager->SuggestGlyphIndex(LineHeight * Parent->GetParent()->GetViewportDimensions().Y,Parent->GetParent()->GetPrimaryAtlas());
            ConstructMarkupText(Rect,Result.first,Text);
            SetTextScale(Result.second);
        }

        MarkupText::~MarkupText()
        {
            Parent->GetGorillaLayer()->destroyMarkupText(GMarkup);
        }

        void MarkupText::ConstructMarkupText(const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;

                const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
                GMarkup = Parent->GetGorillaLayer()->createMarkupText(Glyph,(Rect.Position * WinDim).GetOgreVector2(),(Rect.Size * WinDim).GetOgreVector2(),Text,Parent->GetParent()->GetPrimaryAtlas());
            }else{
                RelPosition = Rect.Position / Parent->GetParent()->GetViewportDimensions();

                GMarkup = Parent->GetGorillaLayer()->createMarkupText(Glyph,Rect.Position.GetOgreVector2(),Rect.Size.GetOgreVector2(),Text,Parent->GetParent()->GetPrimaryAtlas());
            }
        }

        void MarkupText::SetVisible(bool Visible)
        {
            GMarkup->SetVisible(Visible);
        }

        bool MarkupText::IsVisible() const
        {
            return GMarkup->IsVisible();
        }

        void MarkupText::Show()
        {
            GMarkup->Show();
        }

        void MarkupText::Hide()
        {
            GMarkup->Hide();
        }

        void MarkupText::SetText(const String& Text)
        {
            GMarkup->text(Text);
        }

        String MarkupText::GetText()
        {
            return GMarkup->text();
        }

        void MarkupText::SetTextScale(const Real& Scale)
        {
            GMarkup->SetCharScaling(Scale);
        }

        Real MarkupText::GetTextScale()
        {
            return GMarkup->GetCharScaling();
        }

        void MarkupText::SetDefaultGlyphIndex(const Whole& DefaultGlyphIndex)
        {
            Glyphs = DefaultGlyphIndex;
            GMarkup->SetDefaultGlyphIndex(DefaultGlyphIndex);
        }

        Whole MarkupText::GetDefaultGlyphIndex()
        {
            return Glyphs;
        }

        /*void MarkupText::HorizontallyAlign(UI::TextHorizontalAlign Align)
        {
            Gorilla::TextAlignment HA;
            switch (Align)
            {
                case UI::Left:
                    HA = Gorilla::TextAlign_Left;
                    break;
                case UI::Right:
                    HA = Gorilla::TextAlign_Right;
                    break;
                case UI::Middle:
                    HA = Gorilla::TextAlign_Centre;
                    break;
                default:
                    return;
            }
            GMarkup->align(HA);
        }

        void MarkupText::VerticallyAlign(UI::TextVerticalAlign Align)
        {
            Gorilla::VerticalAlignment VA;
            switch (Align)
            {
                case UI::Top:
                    VA = Gorilla::VerticalAlign_Top;
                    break;
                case UI::Bottom:
                    VA = Gorilla::VerticalAlign_Bottom;
                    break;
                case UI::Center:
                    VA = Gorilla::VerticalAlign_Middle;
                    break;
                default:
                    return;
            }
            GMarkup->vertical_align(VA);
        }*/

        void MarkupText::SetBackgroundColour(const ColourValue& Colour)
        {
            GMarkup->background_colour(Colour.GetOgreColourValue());
        }

        void MarkupText::SetBackgroundSprite(const String& Name)
        {
            Gorilla::Sprite* GSprite = Parent->GetGorillaLayer()->_getSprite(Name,*GMarkup->GetNameFile());
            GMarkup->background_image(GSprite);
        }

        void MarkupText::SetBackgroundSprite(const String& Name, const String& Atlas)
        {
            Gorilla::Sprite* GSprite = Parent->GetGorillaLayer()->_getSprite(Name,Atlas);
            GMarkup->background_image(GSprite);
        }

        void MarkupText::SetPosition(const Vector2& Position)
        {
            RelPosition = Position;
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            GMarkup->left(WinDim.X * RelPosition.X);
            GMarkup->top(WinDim.Y * RelPosition.Y);
        }

        Vector2 MarkupText::GetPosition() const
        {
            return RelPosition;
        }

        void MarkupText::SetActualPosition(const Vector2& Position)
        {
            GMarkup->left(Position.X);
            GMarkup->top(Position.Y);
        }

        Vector2 MarkupText::GetActualPosition() const
        {
            Vector2 Pos(GMarkup->left(), GMarkup->top());
            return Pos;
        }

        void MarkupText::SetSize(const Vector2& Size)
        {
            RelSize = Size;
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            GMarkup->width(WinDim.X * RelSize.X);
            GMarkup->height(WinDim.Y * RelSize.Y);
        }

        Vector2 MarkupText::GetSize() const
        {
            return RelSize;
        }

        void MarkupText::SetActualSize(const Vector2& Size)
        {
            RelSize = Size / Parent->GetParent()->GetViewportDimensions();
            GMarkup->width(Size.X);
            GMarkup->height(Size.Y);
        }

        Vector2 MarkupText::GetActualSize() const
        {
            Vector2 Pos(GMarkup->width(), GMarkup->height());
            return Pos;
        }

        void MarkupText::SetRenderPriority(const UI::RenderPriority& Priority)
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
            GMarkup->RenderPriority(RP);
        }

        UI::RenderPriority MarkupText::GetRenderPriority() const
        {
            Gorilla::RenderPriority RP = this->GMarkup->RenderPriority();
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

        void MarkupText::SetPrimaryAtlas(const String& Atlas)
        {
            GMarkup->SetNameFile(Atlas);
        }

        String MarkupText::GetPrimaryAtlas() const
        {
            return *GMarkup->GetNameFile();
        }

        void MarkupText::SetAutoScaleText(bool Enable)
        {
            AutoScaleText = Enable;
        }

        bool MarkupText::GetAutoScaleText()
        {
            return AutoScaleText;
        }

        void MarkupText::UpdateDimensions()
        {
            ViewportUpdateTool::UpdateRenderable(this);
            //this->SetActualPosition(RelPosition * Parent->GetParent()->GetViewportDimensions());
            //this->SetActualSize(RelSize * Parent->GetParent()->GetViewportDimensions());
            if(AutoScaleText)
            {
                std::pair<Whole,Real> Result = Manager->SuggestGlyphIndex(RelLineHeight * Parent->GetParent()->GetViewportDimensions().Y,GetPrimaryAtlas());
                SetDefaultGlyphIndex(Result.first);
                SetTextScale(Result.second);
            }
        }
    }//UI
}//phys

#endif
