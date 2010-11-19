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
#ifndef _uitextbutton_cpp
#define _uitextbutton_cpp

#include "uitextbutton.h"
#include "uilayer.h"
#include "uimanager.h"
#include "internalGorilla.h.cpp"

namespace phys
{
    namespace UI
    {
        TextButton::TextButton(String& name, Vector2 Position, Vector2 Size, Whole Glyph, String Text, UILayer* Layer)
            : Button(name,Position,Size,Layer)
        {
            GorillaButton = Parent->GetGorillaLayer()->createCaption(Glyph,GorillaRectangle->left(),GorillaRectangle->top(),Text);
            GorillaButton->size(GorillaRectangle->width(),GorillaRectangle->height());
            GorillaButton->background(Ogre::ColourValue(0,0,0,0));
        }

        TextButton::~TextButton()
        {
            Parent->GetGorillaLayer()->destroyCaption(GorillaButton);
        }

        void TextButton::SetText(String& Text)
        {
            GorillaButton->text(Text);
        }

        String TextButton::GetText()
        {
            return GorillaButton->text();
        }

        void TextButton::HorizontallyAlign(UI::TextHorizontalAlign Align)
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
            GorillaButton->align(HA);
        }

        void TextButton::VerticallyAlign(UI::TextVerticalAlign Align)
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
            GorillaButton->vertical_align(VA);
        }

        void TextButton::SetPosition(Vector2 Position)
        {
            RelPosition = Position;
            Vector2 CurrDim = Manager->GetWindowDimensions();
            GorillaButton->left(CurrDim.X * RelPosition.X);
            GorillaButton->top(CurrDim.Y * RelPosition.Y);
            GorillaRectangle->left(CurrDim.X * RelPosition.X);
            GorillaRectangle->top(CurrDim.Y * RelPosition.Y);
        }

        Vector2 TextButton::GetPosition()
        {
            return RelPosition;
        }

        void TextButton::SetActualPosition(Vector2 Position)
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

        void TextButton::SetSize(Vector2 Size)
        {
            RelSize = Size;
            Vector2 CurrDim = Manager->GetWindowDimensions();
            GorillaButton->left(CurrDim.X * RelSize.X);
            GorillaButton->top(CurrDim.Y * RelSize.Y);
            GorillaRectangle->left(CurrDim.X * RelSize.X);
            GorillaRectangle->top(CurrDim.Y * RelSize.Y);
        }

        Vector2 TextButton::GetSize()
        {
            return RelSize;
        }

        void TextButton::SetActualSize(Vector2 Size)
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

        void TextButton::SetRenderPriority(UI::RenderPriority Priority)
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
    }
}

#endif
