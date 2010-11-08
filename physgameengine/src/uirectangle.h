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
#ifndef _uirectangle_h
#define _uirectangle_h

#include "datatypes.h"
#include "colourvalue.h"
#include "vector2.h"

namespace Gorilla
{
    class Rectangle;
    class Layer;
}

namespace phys
{
    class UIManager;
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class UIRectangle
        /// @headerfile uirectangle.h
        /// @brief This class is a helper class, specifically for use with background rectangle shapes.
        /// @details
        ///////////////////////////////////////
        class Rectangle
        {
            protected:
                Gorilla::Rectangle* GRectangle;
                Gorilla::Layer* Parent;
                UIManager* Manager;
            public:
                /// @brief Internal constructor
                /// @param GRect The Gorilla Rectangle this rectangle is based on.
                /// @param manager Pointer to the manager that created this rectangle.
                Rectangle(Gorilla::Rectangle* GRect, Gorilla::Layer* GLayer, UIManager* manager);
                /// @brief Class destructor.
                ~Rectangle();
                /// @brief Sets the background colour of the rectangle.
                /// @param Colour A colour value representing the colour to be set.
                void SetBackgroundColour(ColourValue& Colour);
                /// @brief Sets the background image(if provided in the atlas) of the rectangle.
                /// @param Name The name of the sprite to set as the background.
                void SetBackgroundSprite(const String& Name);
                /// @brief Enables a border and sets it's colour.
                /// @param Colour A colour value representing the colour to be set.
                void SetBorder(Real Width, ColourValue& Colour);
                /// @brief Sets the top left position of this rectangle in pixels.
                /// @param Position A Vector2 representing the location of this rectangle.
                void SetPosition(Vector2 Position);
                /// @brief Gets the top left position of this rectangle in pixels.
                /// @return Returns a Vector2 representing the location of this rectangle.
                Vector2 GetPosition();
                /// @brief Sets the size of this rectangle in pixels.
                /// @param Size A vector2 representing the size of this rectangle.
                void SetSize(Vector2 Size);
                /// @brief Gets the size of this rectangle in pixels.
                /// @return Returns a vector2 representing the size of this rectangle.
                Vector2 GetSize();
        };//rectangle
    }//UI
}//phys

#endif
