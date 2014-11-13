// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _mathtoolintersections_h
#define _mathtoolintersections_h

#include "vector2.h"
#include "vector3.h"
#include "plane.h"

namespace Mezzanine
{
    class AxisAlignedBox;
    class Plane;
    class Ray;
    class Sphere;
    class LineSegment2D;
    class LineSegment3D;
    /// @namespace Mezzanine::MathTools
    /// @brief This namespace is the home of a number of utility variables and methods to facilitate various math related tasks.
    namespace MathTools
    {
        /// @brief This is a type used for intersection tests that return a point in 2D space.
        /// @details This type provides more verbose return data that can be used for further tests.
        typedef std::pair<Boole,Vector2> Point2DTestResult;
        /// @brief This is a type used for intersection tests that return a point in 3D space.
        /// @details This type provides more verbose return data that can be used for further tests.
        typedef std::pair<Boole,Vector3> Point3DTestResult;
        /// @brief This is a type used for geometry tests that return a line segment in 2D space.
        /// @details This type provides more verbose return data that can be used for further tests.
        typedef std::pair<Boole,LineSegment2D> Line2DTestResult;
        /// @brief This is a type used for geometry tests that return a line segment in 3D space.
        /// @details This type provides more verbose return data that can be used for further tests.
        typedef std::pair<Boole,LineSegment3D> Line3DTestResult;

        /// @brief This is a type used for the return of a ray intersection test.
        /// @details This type provides more verbose return data that can be used for further tests.
        class PointPointInterection
        {
            public:
                Whole IntersectionCount;
                Vector3 FirstIntersection;
                Vector3 SecondIntersection;

                PointPointInterection(Whole HitCount = 0, Vector3 FirstHit = Vector3(), Vector3 SecondHit = Vector3())
                    : IntersectionCount(HitCount),
                      FirstIntersection(FirstHit),
                      SecondIntersection(SecondHit)
                    {}
#ifndef SWIG
                operator bool()
                    { return IntersectionCount != 0; }
#endif
        };

        class PlaneAlignedQuad
        {
            private:
                StandardAxis AlignedOn;
                Real DistanceFromOrigin;
                Vector2 MinExtents;
                Vector2 MaxExtents;
            public:

                PlaneAlignedQuad(const StandardAxis& PlanarAlignment = Axis_X,
                                 const Real& Distance = 0.0,
                                 const Vector2& Min = Vector2(),
                                 const Vector2& Max = Vector2())
                    : AlignedOn(PlanarAlignment),
                      MinExtents(Min),
                      MaxExtents(Max)
                {}

                static Vector2 DropAxisToConvert(Vector3 Point, StandardAxis AxisToDrop)
                {

                }

                bool OverlapsWith(Vector3 Other) const
                {
                    if(DistanceFromOrigin == Other[AlignedOn])
                    {
                        return OverlapsWithPointOnPl();
                    }
                    /*witch(StandardAxis)
                    {
                        case Axis_X:

                        case Axis_Y:
                        case Axis_Z:
                    }*/

                    return false;
                }

                bool OverlapsWithPointOnPl() const
                {

                }
        };

        typedef std::pair<Boole,Ray> GeometryRayTestResult;
    }//MathTools
}//Mezzanine

#endif
