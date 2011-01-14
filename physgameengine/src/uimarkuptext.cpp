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
#ifndef _uimarkuptext_cpp
#define _uimarkuptext_cpp

#include "uimarkuptext.h"
#include "uilayer.h"
#include "uimanager.h"
#include "world.h"

#include "internalGorilla.h.cpp"

namespace phys
{
    namespace UI
    {
        MarkupText::MarkupText(ConstString& name, const Vector2 Position, const Whole Glyph, String Text, UILayer* Layer)
            : Parent(Layer),
              RelPosition(Position),
              RelSize(Vector2(0,0)),
              Name(name)
        {
            Manager = World::GetWorldPointer()->GetUIManager();

            Vector2 Window = Manager->GetWindowDimensions();
            GMarkup = Parent->GetGorillaLayer()->createMarkupText(Glyph,Position.X * Window.X,Position.Y * Window.Y,Text);
        }

        MarkupText::~MarkupText()
        {
            Parent->GetGorillaLayer()->destroyMarkupText(GMarkup);
        }

        void MarkupText::SetVisible(bool Visible)
        {
            GMarkup->SetVisible(Visible);
        }

        bool MarkupText::IsVisible()
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

        String& MarkupText::GetName()
        {
            return Name;
        }

        void MarkupText::SetText(String& Text)
        {
            GMarkup->text(Text);
        }

        String MarkupText::GetText()
        {
            return GMarkup->text();
        }

        void MarkupText::SetDefaultGlyphIndex(const Whole DefaultGlyphIndex)
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

        void MarkupText::SetPosition(const Vector2 Position)
        {
            RelPosition = Position;
            Vector2 CurrDim = Manager->GetWindowDimensions();
            GMarkup->left(CurrDim.X * RelPosition.X);
            GMarkup->top(CurrDim.Y * RelPosition.Y);
        }

        Vector2 MarkupText::GetPosition()
        {
            return RelPosition;
        }

        void MarkupText::SetActualPosition(const Vector2 Position)
        {
            GMarkup->left(Position.X);
            GMarkup->top(Position.Y);
        }

        Vector2 MarkupText::GetActualPosition()
        {
            Vector2 Pos(GMarkup->left(), GMarkup->top());
            return Pos;
        }

        void MarkupText::SetMaxSize(const Vector2 Size)
        {
            RelSize = Size;
            Vector2 CurrDim = Manager->GetWindowDimensions();
            GMarkup->width(CurrDim.X * RelSize.X);
            GMarkup->height(CurrDim.Y * RelSize.Y);
        }

        Vector2 MarkupText::GetMaxSize()
        {
            return RelSize;
        }

        void MarkupText::SetMaxActualSize(const Vector2 Size)
        {
            RelSize = Size / Manager->GetWindowDimensions();
            GMarkup->width(Size.X);
            GMarkup->height(Size.Y);
        }

        Vector2 MarkupText::GetMaxActualSize()
        {
            Vector2 Pos(GMarkup->width(), GMarkup->height());
            return Pos;
        }

        void MarkupText::SetRenderPriority(UI::RenderPriority Priority)
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

        UI::RenderPriority MarkupText::GetRenderPriority()
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
    }//UI
}//phys

#endif
