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
#ifndef _uibutton_cpp
#define _uibutton_cpp

#include "uibutton.h"
#include "uilayer.h"
#include "uimanager.h"
#include "eventmanager.h"
#include "world.h"
#include "internalGorilla.h.cpp"

namespace phys
{
    namespace UI
    {
        Button::Button(String& name, Gorilla::Caption* GButton, UILayer* Layer)
            : RelPosition(Vector2(0,0)),
              RelSize(Vector2(0,0)),
              Name(name)
        {
            GorillaButton = GButton;
            GorillaRectangle = Layer->GetGorillaLayer()->createRectangle(GButton->left(),GButton->top(),GButton->width(),GButton->height());
            Parent = Layer;
            Manager = World::GetWorldPointer()->GetUIManager();
            MouseHover = false;
            NormalSprite = NULL;
            HoveredSprite = NULL;
        }

        Button::~Button()
        {
            Parent->GetGorillaLayer()->destroyCaption(GorillaButton);
            Parent->GetGorillaLayer()->destroyRectangle(GorillaRectangle);
        }

        String& Button::GetName()
        {
            return Name;
        }

        void Button::SetText(String& Text)
        {
            GorillaButton->text(Text);
        }

        String Button::GetText()
        {
            return GorillaButton->text();
        }

        bool Button::CheckMouseHover()
        {
            Vector2 MouseLoc = Manager->GetGameWorld()->GetEventManager()->GetMouseCoords();
            if(GorillaButton->intersects(MouseLoc.GetOgreVector2()) && Parent->GetVisible())
            {
                if(!MouseHover && HoveredSprite)
                {
                    GorillaRectangle->background_image(HoveredSprite);
                }
                MouseHover = true;
            }else{
                if(MouseHover && HoveredSprite)
                {
                    GorillaRectangle->background_image(NormalSprite);
                }
                MouseHover = false;
            }
            return MouseHover;
        }

        bool Button::GetMouseHover()
        {
            return MouseHover;
        }

        void Button::SetBackgroundColour(ColourValue& Colour)
        {
            GorillaButton->background(Colour.GetOgreColourValue());
        }

        void Button::SetBackgroundSprite(const String& Name)
        {
            Gorilla::Sprite* GSprite = Parent->GetGorillaLayer()->_getSprite(Name);
            NormalSprite = GSprite;
            GorillaRectangle->background_image(GSprite);
        }

        void Button::SetHoveredSprite(const String& Name)
        {
            Gorilla::Sprite* GSprite = Parent->GetGorillaLayer()->_getSprite(Name);
            HoveredSprite = GSprite;
        }

        void Button::SetBorder(Real Width, ColourValue& Colour)
        {
            GorillaRectangle->border(Width, Colour.GetOgreColourValue());
        }

        void Button::HorizontallyAlign(UI::TextHorizontalAlign Align)
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

        void Button::VerticallyAlign(UI::TextVerticalAlign Align)
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

        void Button::SetPosition(Vector2 Position)
        {
            RelPosition = Position;
            Vector2 CurrDim = Manager->GetWindowDimensions();
            GorillaButton->left(CurrDim.X * RelPosition.X);
            GorillaButton->top(CurrDim.Y * RelPosition.Y);
            GorillaRectangle->left(CurrDim.X * RelPosition.X);
            GorillaRectangle->top(CurrDim.Y * RelPosition.Y);
        }

        Vector2 Button::GetPosition()
        {
            return RelPosition;
        }

        void Button::SetActualPosition(Vector2 Position)
        {
            GorillaButton->left(Position.X);
            GorillaButton->top(Position.Y);
            GorillaRectangle->left(Position.X);
            GorillaRectangle->top(Position.Y);
        }

        Vector2 Button::GetActualPosition()
        {
            Vector2 Pos(GorillaButton->left(), GorillaButton->top());
            return Pos;
        }

        void Button::SetSize(Vector2 Size)
        {
            RelSize = Size;
            Vector2 CurrDim = Manager->GetWindowDimensions();
            GorillaButton->left(CurrDim.X * RelSize.X);
            GorillaButton->top(CurrDim.Y * RelSize.Y);
            GorillaRectangle->left(CurrDim.X * RelSize.X);
            GorillaRectangle->top(CurrDim.Y * RelSize.Y);
        }

        Vector2 Button::GetSize()
        {
            return RelSize;
        }

        void Button::SetActualSize(Vector2 Size)
        {
            GorillaButton->width(Size.X);
            GorillaButton->height(Size.Y);
            GorillaRectangle->width(Size.X);
            GorillaRectangle->height(Size.Y);
        }

        Vector2 Button::GetActualSize()
        {
            Vector2 Pos(GorillaButton->width(), GorillaButton->height());
            return Pos;
        }
    }//UI
}//phys

#endif
