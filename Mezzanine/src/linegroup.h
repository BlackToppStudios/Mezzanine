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
#ifndef _linegroup_cpp
#define _linegroup_cpp

#include "crossplatformexport.h"
#include "datatypes.h"
#include "vector3.h"

namespace Mezzanine
{
    // Just a few forward declarations to make wrapping some functionality to engine internals easier.
    class World;
    namespace internal
    {
            class Line3D;
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @class LineGroup
    /// @headerfile linegroup.h
    /// @brief This is a group of consectutive line segments to be rendered together.
    /// @details This class stores a listing of points and renders thems as one
    /// object into the world provided.
    class MEZZ_LIB LineGroup
    {
        /// @todo TODO: This class really should support rotation, the underlying implementation does.
        public:
            /// @brief Basic Constructor
            /// @details This creates a basic, empty, LineGroup.
            /// @param Parent_ This is a pointer to the world to render to.
            LineGroup(World *Parent_);

            /// @brief Default Destructor
            /// @details This safely tears down, and removes from the graphics system the LineGroup
            ~LineGroup(void);

            /// @brief This add Either a start pointing, or a line segment to the next point
            /// @details This adds a point that will be rendered as the endpoint of a line
            /// @param p The Point to be added.
            void addPoint(const Vector3 &p);

            /// @brief Access points by order they were added.
            /// @details This returns the point indicated by index. They start at 0, and increment from there
            /// @param index A Whole number which indicates which point to retrieve.
            const Vector3 getPoint(Whole index) const;

            /// @brief Get the amount of points used to define Line Segments
            /// @details This return the amount of points and therefore line segments. There will always be one more point
            /// Than line segments.
            /// @return A Whole Number which indicates the amount of points.
            Whole getNumPoints(void) const;

            /// @brief This changes a specific point
            /// @details This replaces a point specified by index with a new point
            /// @param index The index of the point to replace.
            /// @param value A point to replace the existing point with
            void updatePoint(Whole index, const Vector3 &value);

            /// @brief This adds Two points to the list
            /// @details This could add 2 line segments, be it simply adds two lines to the list, but if you don't care then this is an
            /// easy way to guarantee that a specific line segment be rendered.
            /// @todo TODO: In the future we will add a break in the line segment chain when this is called.
            /// @param start The first point to be added
            /// @param end The first point to be added
            void drawLine(const Vector3 &start, const Vector3 &end);

            /// @brief Renders the line segment chain.
            /// @details This send the Line segment information to the rending subsystem. PrepareForRendering() should be called first
            /// @todo TODO: PrepareForRendering should be rolled into drawLines, but this cannot happen until the physics debug rendererin gets more attention.
            void drawLines(void);

            /// @brief This assists in the rendering process
            /// @details This sends some of the data to the rendering subsystem to aid drawLines
            void PrepareForRendering();

            //Real getSquaredViewDepth(const Camera *cam) const;

            /// @brief How big would a circle need to be to encapsulate this
            /// @details This returns the radius the a circle would need to have to surround this line group.
            /// @return This returns a real number which indicates the radius.
            Real getBoundingRadius(void) const;

        private:
            /// @brief A pointer to the world that this LineGroup will render to.
            World *Parent;

            /// @brief A Pointer to the internal class that actually does the work.
            internal::Line3D *LineData;
    };
}

#endif
