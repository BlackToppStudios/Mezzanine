// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#include "colourvalue.h"

namespace Mezzanine
{
    // Just a few forward declarations to make wrapping some functionality to engine internals easier.
    class Entresol;
    class World;
    namespace Internal
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
        private:
            World * ParentWorld;
        public:
            /// @brief Basic Constructor.
            LineGroup(World * ParentWorld);
            /// @brief Default Destructor.
            ~LineGroup();

            /// @brief This add Either a start pointing, or a line segment to the next point.
            /// @details This adds a point that will be rendered as the endpoint of a line.
            /// @param NewPoint The Point to be added.
            /// @param Colour The colour to be given to the new point.
            void AddPoint(const Vector3& NewPoint, const ColourValue& Colour);
            /// @brief Access points by order they were added.
            /// @return Returns the point indicated by index. They start at 0, and increment from there.
            /// @param Index A Whole number which indicates which point to retrieve.
            const Vector3 GetPoint(const Whole Index) const;
            /// @brief Get the amount of points used to define Line Segments.
            /// @return A Whole Number which indicates the amount of points used to make the lines in this LineGroup.
            Whole GetNumPoints() const;
            /// @brief This changes a specific point.
            /// @details This replaces a point specified by index with a new point.
            /// @param Index The index of the point to replace.
            /// @param NewValue A point to replace the existing point with.
            void UpdatePoint(const Whole Index, const Vector3& NewValue);
            /// @brief Clears all data pertaining to points in this line group.
            void ClearLines();

            /// @brief This adds Two points to the list.
            /// @param Start The first point to be added.
            /// @param End The second point to be added.
            /// @param Colour The colour of the line being added.
            void DrawLine(const Vector3& Start, const Vector3& End, const ColourValue& Colour);
            /// @brief Updates the render buffers with the needed data to draw the lines in this LineGroup.
            void DrawLines();

            /// @brief Configures this LineGroup to render in the scene.
            void AddToWorld();
            /// @brief Unhooks this LineGroup from the scene, stopping it from rendering.
            void RemoveFromWorld();

            /// @brief How big would a circle need to be to encapsulate this.
            /// @return This returns a real number which indicates the radius.
            Real GetBoundingRadius() const;
        private:
            /// @internal
            /// @brief A Pointer to the internal class that actually does the work.
            Internal::Line3D *LineData;
    };//LineGroup
}//Mezzanine

#endif
