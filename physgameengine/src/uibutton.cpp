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
    UIButton::UIButton(String& name, Gorilla::Caption* GButton, UILayer* Layer, UIManager* manager)
        : Name(name)
    {
        GorillaButton = GButton;
        GorillaRectangle = Layer->GetGorillaLayer()->createRectangle(GButton->left(),GButton->top(),GButton->width(),GButton->height());
        Parent = Layer;
        Manager = manager;
        Caption = false;
        MouseHover = false;
    }

    UIButton::~UIButton()
    {
        Parent->GetGorillaLayer()->destroyCaption(GorillaButton);
    }

    String& UIButton::GetName()
    {
        return Name;
    }

    void UIButton::SetAsCaption(bool caption)
    {
        this->Caption = caption;
    }

    bool UIButton::IsCaption()
    {
        return Caption;
    }

    void UIButton::SetText(String& Text)
    {
        GorillaButton->text(Text);
    }

    String UIButton::GetText()
    {
        return GorillaButton->text();
    }

    bool UIButton::CheckMouseHover()
    {
        Vector2 MouseLoc = Manager->GetGameWorld()->GetEventManager()->GetMouseCoords();
        if(GorillaButton->intersects(MouseLoc.GetOgreVector2()) && Parent->GetVisible())
        {
            MouseHover = true;
        }else{
            MouseHover = false;
        }
        return MouseHover;
    }

    bool UIButton::GetMouseHover()
    {
        return MouseHover;
    }

    void UIButton::SetBackgroundColour(ColourValue& Colour)
    {
        GorillaButton->background(Colour.GetOgreColourValue());
    }

    void UIButton::SetBackgroundSprite(const String& Name)
    {
        Gorilla::Sprite* GSprite = Parent->GetGorillaLayer()->_getSprite(Name);
        GorillaRectangle->background_image(GSprite);
    }

    void UIButton::SetBorder(Real Width, ColourValue& Colour)
    {
        GorillaRectangle->border(Width, Colour.GetOgreColourValue());
    }

    void UIButton::HorizontallyAlign(HorizontalAlign Align)
    {
        Gorilla::TextAlignment HA;
        switch (Align)
        {
            case Left:
                HA = Gorilla::TextAlign_Left;
                break;
            case Right:
                HA = Gorilla::TextAlign_Right;
                break;
            case Middle:
                HA = Gorilla::TextAlign_Centre;
                break;
            default:
                return;
        }
        GorillaButton->align(HA);
    }

    void UIButton::VerticallyAlign(VerticalAlign Align)
    {
        Gorilla::VerticalAlignment VA;
        switch (Align)
        {
            case Top:
                VA = Gorilla::VerticalAlign_Top;
                break;
            case Bottom:
                VA = Gorilla::VerticalAlign_Bottom;
                break;
            case Center:
                VA = Gorilla::VerticalAlign_Middle;
                break;
            default:
                return;
        }
        GorillaButton->vertical_align(VA);
    }

    void UIButton::SetPosition(Vector2 Position)
    {
        GorillaButton->left(Position.X);
        GorillaButton->top(Position.Y);
        GorillaRectangle->left(Position.X);
        GorillaRectangle->top(Position.Y);
    }

    Vector2 UIButton::GetPosition()
    {
        Vector2 Pos(GorillaButton->left(), GorillaButton->top());
        return Pos;
    }

    void UIButton::SetSize(Vector2 Size)
    {
        GorillaButton->width(Size.X);
        GorillaButton->height(Size.Y);
        GorillaRectangle->width(Size.X);
        GorillaRectangle->height(Size.Y);
    }

    Vector2 UIButton::GetSize()
    {
        Vector2 Pos(GorillaButton->width(), GorillaButton->height());
        return Pos;
    }
}//phys

#endif
