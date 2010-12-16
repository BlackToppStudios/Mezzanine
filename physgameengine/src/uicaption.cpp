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
#ifndef _uicaption_cpp
#define _uicaption_cpp

#include "uicaption.h"
#include "uilayer.h"
#include "uimanager.h"
#include "eventmanager.h"
#include "world.h"

#include "internalGorilla.h.cpp"

namespace phys
{
    namespace UI
    {
        Caption::Caption(String& name, Vector2 Position, Vector2 Size, Whole Glyph, String Text, UILayer* Layer)
            : Parent(Layer),
              MouseHover(false),
              RelPosition(Position),
              RelSize(Size),
              Name(name)
        {
            Manager = World::GetWorldPointer()->GetUIManager();

            Vector2 Window = Manager->GetWindowDimensions();
            GorillaCaption = Parent->GetGorillaLayer()->createCaption(Glyph, Position.X * Window.X, Position.Y * Window.Y, Text);
            GorillaCaption->size(Size.X * Window.X, Size.Y * Window.Y);
            GorillaRectangle = Parent->GetGorillaLayer()->createRectangle((Position * Window).GetOgreVector2(),(Size * Window).GetOgreVector2());
            GorillaCaption->background(Ogre::ColourValue(0,0,0,0));
        }

        Caption::~Caption()
        {
            Parent->GetGorillaLayer()->destroyCaption(GorillaCaption);
            Parent->GetGorillaLayer()->destroyRectangle(GorillaRectangle);
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
            Vector2 MouseLoc = Manager->GetGameWorld()->GetEventManager()->GetMouseCoords();
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

        String& Caption::GetName()
        {
            return Name;
        }

        void Caption::SetText(String& Text)
        {
            GorillaCaption->text(Text);
        }

        String Caption::GetText()
        {
            return GorillaCaption->text();
        }

        void Caption::SetTextColour(ColourValue& TextColour)
        {
            GorillaCaption->colour(TextColour.GetOgreColourValue());
        }

        ColourValue Caption::GetTextColour()
        {
            ColourValue Col(GorillaCaption->colour());
            return Col;
        }

        void Caption::SetGlyphIndex(Whole GlyphIndex)
        {
            Glyphs = GlyphIndex;
            GorillaCaption->SetGlyphIndex(GlyphIndex);
        }

        Whole Caption::GetGlyphIndex()
        {
            return Glyphs;
        }

        void Caption::SetBackgroundColour(ColourValue& Colour)
        {
            GorillaRectangle->background_colour(Colour.GetOgreColourValue());
        }

        void Caption::SetBackgroundSprite(const String& Name)
        {
            Gorilla::Sprite* GSprite = Parent->GetGorillaLayer()->_getSprite(Name);
            GorillaRectangle->background_image(GSprite);
            //GorillaCaption->background(Ogre::ColourValue(0,0,0,0));
        }

        void Caption::HorizontallyAlign(UI::TextHorizontalAlign Align)
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
            GorillaCaption->align(HA);
        }

        void Caption::VerticallyAlign(UI::TextVerticalAlign Align)
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
            GorillaCaption->vertical_align(VA);
        }

        void Caption::SetPosition(Vector2 Position)
        {
            RelPosition = Position;
            Vector2 CurrDim = Manager->GetWindowDimensions();
            GorillaCaption->left(CurrDim.X * RelPosition.X);
            GorillaCaption->top(CurrDim.Y * RelPosition.Y);
            GorillaRectangle->left(CurrDim.X * RelPosition.X);
            GorillaRectangle->top(CurrDim.Y * RelPosition.Y);
        }

        Vector2 Caption::GetPosition()
        {
            return RelPosition;
        }

        void Caption::SetActualPosition(Vector2 Position)
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

        void Caption::SetSize(Vector2 Size)
        {
            RelSize = Size;
            Vector2 CurrDim = Manager->GetWindowDimensions();
            GorillaCaption->left(CurrDim.X * RelSize.X);
            GorillaCaption->top(CurrDim.Y * RelSize.Y);
            GorillaRectangle->left(CurrDim.X * RelSize.X);
            GorillaRectangle->top(CurrDim.Y * RelSize.Y);
        }

        Vector2 Caption::GetSize()
        {
            return RelSize;
        }

        void Caption::SetActualSize(Vector2 Size)
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

        void Caption::SetRenderPriority(UI::RenderPriority Priority)
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
    }//UT
}//phys

#endif
