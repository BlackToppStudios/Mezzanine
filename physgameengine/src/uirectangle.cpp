//� Copyright 2010 BlackTopp Studios Inc.
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

#include "internalGorilla.h.cpp"

namespace phys
{
    UIRectangle::UIRectangle(Gorilla::Rectangle* GRect, Gorilla::Layer* GLayer, UIManager* manager)
    {
        GRectangle = GRect;
        Parent = GLayer;
        Manager = manager;
    }

    UIRectangle::~UIRectangle()
    {
        Parent->destroyRectangle(GRectangle);
    }

    void UIRectangle::SetBackgroundColour(ColourValue& Colour)
    {
        GRectangle->background_colour(Colour.GetOgreColourValue());
    }

    void UIRectangle::SetBackgroundSprite(const String& Name)
    {
        Gorilla::Sprite* GSprite = Parent->_getSprite(Name);
        GRectangle->background_image(GSprite);
    }

    void UIRectangle::SetBorder(Real Width, ColourValue& Colour)
    {
        GRectangle->border(Width, Colour.GetOgreColourValue());
    }

    void UIRectangle::SetPosition(Vector2 Position)
    {
        GRectangle->position(Position.GetOgreVector2());
    }

    Vector2 UIRectangle::GetPosition()
    {
        Vector2 Pos(GRectangle->left(), GRectangle->top());
        return Pos;
    }

    void UIRectangle::SetSize(Vector2 Size)
    {
        GRectangle->width(Size.X);
        GRectangle->height(Size.Y);
    }

    Vector2 UIRectangle::GetSize()
    {
        Vector2 Pos(GRectangle->width(), GRectangle->height());
        return Pos;
    }
}

#endif
