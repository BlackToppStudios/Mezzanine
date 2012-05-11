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
#ifndef _uilinelist_h
#define _uilinelist_h

#include "colourvalue.h"
#include "uienumerations.h"
#include "UI/basicrenderable.h"

namespace Mezzanine
{
    class UIManager;
    namespace UI
    {
        class Layer;
        class VertexData;
        class ScreenVertexData;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class LineList
        /// @headerfile uilinelist.h
        /// @brief This is an object comprised of a series of lines.
        /// @details This class isn't an object, but rather just a series of lines.  As such it
        /// doesn't have a position or size.  The position functions exist only to create additional
        /// points for the lines to connect.
        ///////////////////////////////////////
        class MEZZ_LIB LineList : public BasicRenderable
        {
            protected:
                friend class Layer;
                Layer* Parent;
                UIManager* Manager;
                bool IsClosed;
                Real Thickness;
                ColourValue Colour;
                std::vector<Vector2> Positions;
            public:
                /// @brief Class constructor.
                /// @param name The name to give to this Linelist.
                /// @param Layer Pointer to the parent Layer that created this rectangle.
                LineList(const String& name, Layer* PLayer);
                /// @brief Class destructor.
                virtual ~LineList();

                /// @brief Starts a new line list.
                /// @details If this function is called while lines have already been defined, it will
                /// clear the current list of lines and start a new list.
                /// @param LineThickness The thickness of the line to draw in pixels.
                /// @param LineColour The colour of the line.
                void Begin(const Whole& LineThickness, const ColourValue& LineColour);
                /// @brief Adds a new point/line to the list via 2 reals.
                /// @param X Relative coordinate on the X vector.
                /// @param Y Relative coordinate on the Y vector.
                void AddPoint(const Real& X, const Real& Y);
                /// @brief Adds a new point/line to the list via a vector2.
                /// @param Position A vector2 representing the relative position on screen.
                void AddPoint(const Vector2& Position);
                /// @brief Adds a new point/line to the list via 2 reals.
                /// @param X Coordinate on the X vector.
                /// @param Y Coordinate on the Y vector.
                void AddActualPoint(const Real& X, const Real& Y);
                /// @brief Adds a new point/line to the list via a vector2.
                /// @param Position A vector2 representing the position on screen.
                void AddActualPoint(const Vector2& Position);
                /// @brief Finalizes the list and prepares it for rendering.
                /// @param Closed Whether or not the line list connects back to it's starting position.  If
                /// true this will create one last line connecting the last provided position with the first.
                void End(bool Closed = false);

                /// @brief Updates the dimensions of this renderable to match those of the new screen size.
                /// @details This function is called automatically when a viewport changes in size, and shouldn't need to be called manually.
                virtual void UpdateDimensions();
                ///////////////////////////////////////////////////////////////////////////////
                // Internal Functions
                ///////////////////////////////////////
                /// @copydoc UI::BasicRenderable::_Redraw()
                virtual void _Redraw();
                /// @copydoc UI::BasicRenderable::_AppendVertices()
                virtual void _AppendVertices(ScreenVertexData& Vertices);
        };//listlist
    }//UI
}//Mezzanine

#endif
