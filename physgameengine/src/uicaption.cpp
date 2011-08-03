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
#ifndef _uicaption_cpp
#define _uicaption_cpp

#include "inputquerytool.h"
#include "uicaption.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "uimanager.h"
#include "eventmanager.h"
#include "world.h"

#include "internalGorilla.h.cpp"

namespace phys
{
    namespace UI
    {
        Caption::Caption(ConstString& name, const RenderableRect& Rect, const Whole& Glyph, const String& Text, Layer* PLayer)
            : Parent(PLayer),
              MouseHover(false),
              Name(name)
        {
            Manager = World::GetWorldPointer()->GetUIManager();
            AutoScaleText = false;
            RelLineHeight = 0.0;
            ConstructCaption(Rect,Glyph,Text);
        }

        Caption::Caption(ConstString& name, const RenderableRect& Rect, const Real& LineHeight, const String& Text, Layer* PLayer)
            : Parent(PLayer),
              MouseHover(false),
              Name(name)
        {
            Manager = World::GetWorldPointer()->GetUIManager();
            AutoScaleText = true;
            RelLineHeight = LineHeight;
            std::pair<Whole,Real> Result = Manager->SuggestGlyphIndex((Whole)(LineHeight * Parent->GetParent()->GetViewportDimensions().Y),Parent->GetParent()->GetPrimaryAtlas());
            ConstructCaption(Rect,Result.first,Text);
            SetTextScale(Result.second);
        }

        Caption::~Caption()
        {
            Parent->GetGorillaLayer()->destroyCaption(GorillaCaption);
            Parent->GetGorillaLayer()->destroyRectangle(GorillaRectangle);
        }

        void Caption::ConstructCaption(const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;

                const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
                GorillaCaption = Parent->GetGorillaLayer()->createCaption(Glyph, Rect.Position.X * WinDim.X, Rect.Position.Y * WinDim.Y, Text, Parent->GetParent()->GetPrimaryAtlas());
                GorillaCaption->size(Rect.Size.X * WinDim.X, Rect.Size.Y * WinDim.Y);
                GorillaRectangle = Parent->GetGorillaLayer()->createRectangle((Rect.Position * WinDim).GetOgreVector2(),(Rect.Size * WinDim).GetOgreVector2());
            }else{
                RelPosition = Rect.Position / Parent->GetParent()->GetViewportDimensions();
                RelSize = Rect.Size / Parent->GetParent()->GetViewportDimensions();

                GorillaCaption = Parent->GetGorillaLayer()->createCaption(Glyph, Rect.Position.X, Rect.Position.Y, Text, Parent->GetParent()->GetPrimaryAtlas());
                GorillaCaption->size(Rect.Size.X, Rect.Size.Y);
                GorillaRectangle = Parent->GetGorillaLayer()->createRectangle(Rect.Position.GetOgreVector2(),Rect.Size.GetOgreVector2());
            }
            GorillaCaption->background(Ogre::ColourValue(0,0,0,0));
            GorillaCaption->align(Gorilla::TextAlign_Centre);
            GorillaCaption->vertical_align(Gorilla::VerticalAlign_Middle);
        }

        void Caption::SetVisible(bool Visible)
        {
            GorillaRectangle->SetVisible(Visible);
            GorillaCaption->SetVisible(Visible);
        }

        bool Caption::IsVisible()
        {
            return GorillaRectangle->IsVisible();
        }

        void Caption::Show()
        {
            GorillaRectangle->Show();
            GorillaCaption->Show();
        }

        void Caption::Hide()
        {
            GorillaRectangle->Hide();
            GorillaCaption->Hide();
        }

        bool Caption::CheckMouseHover()
        {
            if(!GorillaRectangle->IsVisible())
                return false;
            Vector2 MouseLoc = InputQueryTool::GetMouseCoordinates();
            if(GorillaCaption->intersects(MouseLoc.GetOgreVector2()))
            {
                MouseHover = true;
            }else{
                MouseHover = false;
            }
            return MouseHover;
        }

        bool Caption::GetMouseHover()
        {
            return MouseHover;
        }

        ConstString& Caption::GetName()
        {
            return Name;
        }

        void Caption::SetText(ConstString& Text)
        {
            GorillaCaption->text(Text);
        }

        String Caption::GetText()
        {
            return GorillaCaption->text();
        }

        void Caption::SetTextScale(const Real& Scale)
        {
            GorillaCaption->SetCharScaling(Scale);
        }

        Real Caption::GetTextScale()
        {
            return GorillaCaption->GetCharScaling();
        }

        void Caption::SetTextColour(const ColourValue& TextColour)
        {
            GorillaCaption->colour(TextColour.GetOgreColourValue());
        }

        ColourValue Caption::GetTextColour()
        {
            ColourValue Col(GorillaCaption->colour());
            return Col;
        }

        void Caption::SetGlyphIndex(const Whole& GlyphIndex)
        {
            Glyphs = GlyphIndex;
            GorillaCaption->font(GlyphIndex,*GorillaCaption->GetNameFile());
        }

        void Caption::SetGlyphIndex(const Whole& GlyphIndex, const String& Atlas)
        {
            Glyphs = GlyphIndex;
            GorillaCaption->font(GlyphIndex,Atlas);
        }

        Whole Caption::GetGlyphIndex()
        {
            return Glyphs;
        }

        void Caption::SetCursorOffset(const Whole& Offset)
        {
            GorillaCaption->SetCursorOffset((Real)Offset);
        }

        Whole Caption::GetCursorOffset()
        {
            return (Whole)GorillaCaption->GetCursorOffset();
        }

        void Caption::SetBackgroundColour(const ColourValue& Colour)
        {
            GorillaRectangle->background_colour(Colour.GetOgreColourValue());
        }

        void Caption::SetBackgroundSprite(const String& Name)
        {
            Gorilla::Sprite* GSprite = Parent->GetGorillaLayer()->_getSprite(Name,*GorillaCaption->GetNameFile());
            GorillaRectangle->background_image(GSprite);
        }

        void Caption::SetBackgroundSprite(const String& Name, const String& Atlas)
        {
            Gorilla::Sprite* GSprite = Parent->GetGorillaLayer()->_getSprite(Name,Atlas);
            GorillaRectangle->background_image(GSprite);
        }

        void Caption::HorizontallyAlign(const UI::TextHorizontalAlign& Align)
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
            GorillaCaption->align(HA);
        }

        void Caption::VerticallyAlign(const UI::TextVerticalAlign& Align)
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
            GorillaCaption->vertical_align(VA);
        }

        void Caption::SetPosition(const Vector2& Position)
        {
            RelPosition = Position;
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            GorillaCaption->left(WinDim.X * RelPosition.X);
            GorillaCaption->top(WinDim.Y * RelPosition.Y);
            GorillaRectangle->left(WinDim.X * RelPosition.X);
            GorillaRectangle->top(WinDim.Y * RelPosition.Y);
        }

        Vector2 Caption::GetPosition()
        {
            return RelPosition;
        }

        void Caption::SetActualPosition(const Vector2& Position)
        {
            GorillaCaption->left(Position.X);
            GorillaCaption->top(Position.Y);
            GorillaRectangle->left(Position.X);
            GorillaRectangle->top(Position.Y);
        }

        Vector2 Caption::GetActualPosition()
        {
            Vector2 Pos(GorillaCaption->left(), GorillaCaption->top());
            return Pos;
        }

        void Caption::SetSize(const Vector2& Size)
        {
            RelSize = Size;
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            GorillaCaption->width(WinDim.X * RelSize.X);
            GorillaCaption->height(WinDim.Y * RelSize.Y);
            GorillaRectangle->width(WinDim.X * RelSize.X);
            GorillaRectangle->height(WinDim.Y * RelSize.Y);
        }

        Vector2 Caption::GetSize()
        {
            return RelSize;
        }

        void Caption::SetActualSize(const Vector2& Size)
        {
            GorillaCaption->width(Size.X);
            GorillaCaption->height(Size.Y);
            GorillaRectangle->width(Size.X);
            GorillaRectangle->height(Size.Y);
        }

        Vector2 Caption::GetActualSize()
        {
            Vector2 Pos(GorillaCaption->width(), GorillaCaption->height());
            return Pos;
        }

        void Caption::SetRenderPriority(const UI::RenderPriority& Priority)
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
            GorillaCaption->RenderPriority(RP);
            GorillaRectangle->RenderPriority(RP);
        }

        UI::RenderPriority Caption::GetRenderPriority()
        {
            Gorilla::RenderPriority RP = this->GorillaCaption->RenderPriority();
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

        void Caption::SetPrimaryAtlas(const String& Atlas)
        {
            GorillaCaption->SetNameFile(Atlas);
        }

        String Caption::GetPrimaryAtlas()
        {
            return *GorillaCaption->GetNameFile();
        }

        void Caption::SetAutoScaleText(bool Enable)
        {
            AutoScaleText = Enable;
        }

        bool Caption::GetAutoScaleText()
        {
            return AutoScaleText;
        }

        void Caption::UpdateDimensions()
        {
            this->SetActualPosition(RelPosition * Parent->GetParent()->GetViewportDimensions());
            this->SetActualSize(RelSize * Parent->GetParent()->GetViewportDimensions());
            if(AutoScaleText)
            {
                std::pair<Whole,Real> Result = Manager->SuggestGlyphIndex(RelLineHeight * Parent->GetParent()->GetViewportDimensions().Y,GetPrimaryAtlas());
                SetGlyphIndex(Result.first);
                SetTextScale(Result.second);
            }
        }
    }//UI
}//phys

#endif
