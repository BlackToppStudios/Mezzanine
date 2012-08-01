//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _uiarearenderable_h
#define _uiarearenderable_h

#include "UI/basicrenderable.h"

namespace Mezzanine
{
    namespace UI
    {
        class Screen;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class AreaRenderable
        /// @headerfile uiarearenderable.h
        /// @brief This class defines a renderable that covers an area, such as a rectangle or circle.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB AreaRenderable : public BasicRenderable
        {
            protected:
                bool MouseHover;
                Vector2 ActPosition;
                Vector2 ActSize;
                Vector2 RelPosition;
                Vector2 RelSize;
                /// @brief Sets the hovered state of this renderable.
                virtual void SetHovered(bool Hovered);
            public:
                /// @brief Class constructor.
                /// @param name The name to be given to this renderable.
                /// @param Parent The parent screen that created this renderable.
                AreaRenderable(const String& name, Screen* Parent);
                /// @brief Class destructor.
                virtual ~AreaRenderable();

                ///////////////////////////////////////////////////////////////////////////////
                // Hover Functions

                /// @brief Determines whether the mouse is over this renderable.
                /// @return Returns a bool indicating whether the mouse is over this renderable.
                virtual bool CheckMouseHover() = 0;
                /// @brief Gets the stored value of whether or not the mouse is over the renderable.
                /// @details This function does not perform any checks.  If you want to do a manual check, call CheckMouseHover().
                /// @return Returns the stored value of whether or not the mouse is over the renderable.
                bool GetMouseHover();

                ///////////////////////////////////////////////////////////////////////////////
                // Transform Functions

                /// @brief Sets the relative top left position of this renderable.
                /// @param Position A Vector2 representing the location of this renderable.
                virtual void SetPosition(const Vector2& Position) = 0;
                /// @brief Gets the relative top left position of this renderable.
                /// @return Returns a Vector2 representing the location of this renderable.
                virtual Vector2 GetPosition() const;
                /// @brief Sets the top left position of this renderable in pixels.
                /// @param Position A Vector2 representing the location of this renderable.
                virtual void SetActualPosition(const Vector2& Position) = 0;
                /// @brief Gets the top left position of this renderable in pixels.
                /// @return Returns a Vector2 representing the location of this renderable.
                virtual Vector2 GetActualPosition() const;
                /// @brief Sets the relative size of this renderable.
                /// @param Size A vector2 representing the size of this renderable.
                virtual void SetSize(const Vector2& Size) = 0;
                /// @brief Gets the relative size of this renderable.
                /// @return Returns a vector2 representing the size of this renderable.
                virtual Vector2 GetSize() const;
                /// @brief Sets the size of this renderable in pixels.
                /// @param Size A vector2 representing the size of this renderable.
                virtual void SetActualSize(const Vector2& Size) = 0;
                /// @brief Gets the size of this renderable in pixels.
                /// @return Returns a vector2 representing the size of this renderable.
                virtual Vector2 GetActualSize() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Utility Functions

                /// @copydoc BasicRenderable::UpdateDimensions()
                virtual void UpdateDimensions() = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Functions

                /// @copydoc BasicRenderable::_Redraw()
                virtual void _Redraw() = 0;
                /// @copydoc BasicRenderable::_AppendVertices(ScreenVertexData&)
                virtual void _AppendVertices(ScreenVertexData& Vertices) = 0;
        };//AreaRenderable
    }//UI
}//Mezzanine

#endif
