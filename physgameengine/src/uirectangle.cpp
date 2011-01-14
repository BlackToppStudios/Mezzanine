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
#ifndef _uirectangle_cpp
#define _uirectangle_cpp

#include "uirectangle.h"
#include "uimanager.h"
#include "uilayer.h"
#include "world.h"
#include "eventmanager.h"

#include "internalGorilla.h.cpp"

namespace phys
{
    namespace UI
    {
        Rectangle::Rectangle(const Vector2 Position, const Vector2 Size, UILayer* Layer)
            : Parent(Layer),
              MouseHover(false),
              RelPosition(Position),
              RelSize(Size)
        {
            Manager = World::GetWorldPointer()->GetUIManager();

            Vector2 Window = Manager->GetWindowDimensions();
            GRectangle = Parent->GetGorillaLayer()->createRectangle((Position * Window).GetOgreVector2(),(Size * Window).GetOgreVector2());
        }

        Rectangle::~Rectangle()
        {
            Parent->GetGorillaLayer()->destroyRectangle(GRectangle);
        }

        void Rectangle::SetVisible(bool Visible)
        {
            GRectangle->SetVisible(Visible);
        }

        bool Rectangle::IsVisible()
        {
            return GRectangle->IsVisible();
        }

        void Rectangle::Show()
        {
            GRectangle->Show();
        }

        void Rectangle::Hide()
        {
            GRectangle->Hide();
        }

        bool Rectangle::CheckMouseHover()
        {
            if(!GRectangle->IsVisible())
                return false;
            Vector2 MouseLoc = Manager->GetGameWorld()->GetEventManager()->GetMouseCoords();
            if(GRectangle->intersects(MouseLoc.GetOgreVector2()))
            {
                MouseHover = true;
            }else{
                MouseHover = false;
            }
            return MouseHover;
        }

        bool Rectangle::GetMouseHover()
        {
            return MouseHover;
        }

        void Rectangle::SetBackgroundColour(const ColourValue& Colour)
        {
            GRectangle->background_colour(Colour.GetOgreColourValue());
        }

        void Rectangle::SetBackgroundSprite(const String& Name)
        {
            Gorilla::Sprite* GSprite = Parent->GetGorillaLayer()->_getSprite(Name);
            GRectangle->background_image(GSprite);
        }

        void Rectangle::SetBorder(const Real Width, const ColourValue& Colour)
        {
            GRectangle->border(Width, Colour.GetOgreColourValue());
        }

        void Rectangle::NoBorder()
        {
            GRectangle->no_border();
        }

        void Rectangle::SetPosition(const Vector2 Position)
        {
            RelPosition = Position;
            Vector2 CurrDim = Manager->GetWindowDimensions();
            GRectangle->position((CurrDim * RelPosition).GetOgreVector2());
        }

        Vector2 Rectangle::GetPosition()
        {
            return RelPosition;
        }

        void Rectangle::SetActualPosition(const Vector2 Position)
        {
            GRectangle->position(Position.GetOgreVector2());
        }

        Vector2 Rectangle::GetActualPosition()
        {
            Vector2 Pos(GRectangle->left(), GRectangle->top());
            return Pos;
        }

        void Rectangle::SetSize(const Vector2 Size)
        {
            RelSize = Size;
            Vector2 CurrDim = Manager->GetWindowDimensions();
            GRectangle->width(CurrDim.X * RelSize.X);
            GRectangle->height(CurrDim.Y * RelSize.Y);
        }

        Vector2 Rectangle::GetSize()
        {
            return RelSize;
        }

        void Rectangle::SetActualSize(const Vector2 Size)
        {
            GRectangle->width(Size.X);
            GRectangle->height(Size.Y);
        }

        Vector2 Rectangle::GetActualSize()
        {
            Vector2 Pos(GRectangle->width(), GRectangle->height());
            return Pos;
        }

        void Rectangle::SetRenderPriority(UI::RenderPriority Priority)
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
            GRectangle->RenderPriority(RP);
        }

        UI::RenderPriority Rectangle::GetRenderPriority()
        {
            Gorilla::RenderPriority RP = this->GRectangle->RenderPriority();
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
