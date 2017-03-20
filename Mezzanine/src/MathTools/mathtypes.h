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
#ifndef _mathtypes_h
#define _mathtypes_h

#include "datatypes.h"

namespace Mezzanine
{
    class Vector2;
    class Vector3;
    class LineSegment2D;
    class LineSegment3D;
    namespace MathTools
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Enumerations

        /// @brief An enum used to describe which side of a plane a point, or group of points is on.
        /// @remarks Additional culling may be used based on which side is preferable for a specific effect.
        /// @n @n
        /// Triangles make use of this emum as they are just like planes in many ways, but with limits to their extents.
        enum PlaneSide
        {
            PS_Neither  = 0, ///< Error condition, or a miss in the case of a bounded plane (such as a triangle).
            PS_Positive = 1, ///< The front side of the plane.
            PS_Negative = 2, ///< The back side of the plane.
            PS_Both     = 3  ///< When querying with an area, overlapping with the plane will cause it to be on both sides.
        };

        ///////////////////////////////////////////////////////////////////////////////
        // Types

        /// @brief This is a type used for intersection tests that return a point in 2D space.
        typedef std::pair<Boole,Vector2> Point2DTestResult;
        /// @brief This is a type used for intersection tests that return a point in 3D space.
        typedef std::pair<Boole,Vector3> Point3DTestResult;
        /// @brief This is a type used to represent the portion of a ray that pierces though a plane.
        typedef std::pair<PlaneSide,Vector3> PlaneTestResult;
        /// @brief This is a type used for geometry tests that return a line segment in 2D space.
        typedef std::pair<Boole,LineSegment2D> Line2DTestResult;
        /// @brief This is a type used for geometry tests that return a line segment in 3D space.
        typedef std::pair<Boole,LineSegment3D> Line3DTestResult;
        /// @brief This is a type used to represent the portion of a ray that pierces though geometry.
        typedef std::pair<Boole,LineSegment3D> GeometryRayTestResult;
    }//MathTools
}//Mezzanine

#endif // _mathtypes_h
