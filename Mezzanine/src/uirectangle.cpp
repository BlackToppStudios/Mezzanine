//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _uirectangle_cpp
#define _uirectangle_cpp

#include "inputquerytool.h"
#include "uirectangle.h"
#include "uimanager.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "uiviewportupdatetool.h"
#include "world.h"
#include "eventmanager.h"

#include "internalGorilla.h.cpp"

namespace Mezzanine
{
    namespace UI
    {
        Rectangle::Rectangle(const RenderableRect& Rect, Layer* PLayer)
            : BasicRenderable("Rectangle",PLayer),
              MouseHover(false)
        {
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;

                const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
                GRectangle = Parent->GetGorillaLayer()->createRectangle((Rect.Position * WinDim).GetOgreVector2(),(Rect.Size * WinDim).GetOgreVector2());
            }else{
                RelPosition = Rect.Position / Parent->GetParent()->GetViewportDimensions();
                RelSize = Rect.Size / Parent->GetParent()->GetViewportDimensions();

                GRectangle = Parent->GetGorillaLayer()->createRectangle(Rect.Position.GetOgreVector2(),Rect.Size.GetOgreVector2());
            }
            GRectangle->SetNameFile(Parent->GetParent()->GetPrimaryAtlas());
        }

        Rectangle::~Rectangle()
        {
            Parent->GetGorillaLayer()->destroyRectangle(GRectangle);
        }

        void Rectangle::SetVisible(bool Visible)
        {
            GRectangle->SetVisible(Visible);
        }

        bool Rectangle::IsVisible() const
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
            Vector2 MouseLoc = InputQueryTool::GetMouseCoordinates();
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
            Gorilla::Sprite* GSprite = Parent->GetGorillaLayer()->_getSprite(Name,*GRectangle->GetNameFile());
            GRectangle->background_image(GSprite);
        }

        void Rectangle::SetBackgroundSprite(const String& Name, const String& Atlas)
        {
            Gorilla::Sprite* GSprite = Parent->GetGorillaLayer()->_getSprite(Name,Atlas);
            GRectangle->background_image(GSprite);
        }

        void Rectangle::SetBorder(const Real& Width, const ColourValue& Colour)
        {
            GRectangle->border(Width, Colour.GetOgreColourValue());
        }

        void Rectangle::NoBorder()
        {
            GRectangle->no_border();
        }

        void Rectangle::SetPosition(const Vector2& Position)
        {
            RelPosition = Position;
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            GRectangle->position((WinDim * RelPosition).GetOgreVector2());
        }

        Vector2 Rectangle::GetPosition() const
        {
            return RelPosition;
        }

        void Rectangle::SetActualPosition(const Vector2& Position)
        {
            RelPosition = Position / Parent->GetParent()->GetViewportDimensions();
            GRectangle->position(Position.GetOgreVector2());
        }

        Vector2 Rectangle::GetActualPosition() const
        {
            Vector2 Pos(GRectangle->left(), GRectangle->top());
            return Pos;
        }

        void Rectangle::SetSize(const Vector2& Size)
        {
            RelSize = Size;
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            GRectangle->width(WinDim.X * RelSize.X);
            GRectangle->height(WinDim.Y * RelSize.Y);
        }

        Vector2 Rectangle::GetSize() const
        {
            return RelSize;
        }

        void Rectangle::SetActualSize(const Vector2& Size)
        {
            RelSize = Size / Parent->GetParent()->GetViewportDimensions();
            GRectangle->width(Size.X);
            GRectangle->height(Size.Y);
        }

        Vector2 Rectangle::GetActualSize() const
        {
            Vector2 Pos(GRectangle->width(), GRectangle->height());
            return Pos;
        }

        void Rectangle::SetRenderPriority(const UI::RenderPriority& Priority)
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

        UI::RenderPriority Rectangle::GetRenderPriority() const
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

        void Rectangle::SetPrimaryAtlas(const String& Atlas)
        {
            GRectangle->SetNameFile(Atlas);
        }

        String Rectangle::GetPrimaryAtlas() const
        {
            return *GRectangle->GetNameFile();
        }

        void Rectangle::UpdateDimensions()
        {
            //this->SetActualPosition(RelPosition * Parent->GetParent()->GetViewportDimensions());
            //this->SetActualSize(RelSize * Parent->GetParent()->GetViewportDimensions());
            ViewportUpdateTool::UpdateRenderable(this);
        }
    }//UI
}//Mezzanine

#endif
