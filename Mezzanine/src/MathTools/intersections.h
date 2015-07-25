// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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

        typedef std::pair<Boole,Ray> GeometryRayTestResult;

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

        ///////////////////////////////////////////////////////////////////////////////
        // 2D Geometry Intersection Query

        /// @brief Checks to see if two 2D line segments intersect.
        /// @param Line1 The first line segment to test.
        /// @param Line2 The second line segment to test.
        /// @return Returns a std::pair containing whether or not the was an intersection, and the point in 2D space where the intersection occured.
        Point2DTestResult MEZZ_LIB Intersects(const LineSegment2D& Line1, const LineSegment2D& Line2);

        ///////////////////////////////////////////////////////////////////////////////
        // 3D Geometry Intersection Query

        /// @brief Checks to see if a point in 3D space is within an AABB.
        /// @param Box The AABB to check if the point is inside.
        /// @param Point The point in 3D space to check if it is within the AABB.
        /// @return Returns true if the provided point is within the AABB, false otherwise.
        Boole MEZZ_LIB IsInside(const AxisAlignedBox& Box, const Vector3& Point);
        /// @brief Checks to see if a point in 3D space is within a Sphere.
        /// @param Ball The Sphere to check if the point is inside.
        /// @param Point The point in 3D space to check if it is within the Sphere.
        /// @return Returns true if the provided point is within the sphere, false otherwise.
        Boole MEZZ_LIB IsInside(const Sphere& Ball, const Vector3& Point);

        /// @brief Checks to see if an AABB and Sphere are overlapping.
        /// @param Box The AABB to check for overlap.
        /// @param Ball The Sphere to check for overlap.
        /// @return Returns true if the AABB and Sphere overlap, false otherwise.
        Boole MEZZ_LIB Overlap(const AxisAlignedBox& Box, const Sphere& Ball);
        /// @brief Checks to see if an AANN and Plane are overlapping.
        /// @param Box The AABB to check for overlap.
        /// @param Surface The Plane to check for overlap.
        /// @return Returns true if the AABB and Pland overlap, false otherwise.
        Boole MEZZ_LIB Overlap(const AxisAlignedBox& Box, const Plane& Surface);
        /// @brief Checks to see if a Plane and Sphere are overlapping.
        /// @param Surface The Plane to check for overlap.
        /// @param Ball The Sphere to check for overlap.
        /// @return Returns true if the Plane and Sphere overlap, false otherwise.
        Boole MEZZ_LIB Overlap(const Plane& Surface, const Sphere& Ball);

        /// @brief Checks to see if two AABBs are overlapping.
        /// @param Box1 The first AABB to check for overlap.
        /// @param Box2 The second AABB to check for overlap.
        /// @return Returns true if the two AABBs are overlapping, false otherwise.
        Boole MEZZ_LIB Overlap(const AxisAlignedBox& Box1, const AxisAlignedBox& Box2);
        /// @brief Checks to see if two Spheres are overlapping.
        /// @param Ball1 The first Sphere to check for overlap.
        /// @param Ball2 The second Sphere to check for overlap.
        /// @return Returns true if the two Spheres are overlapping, false otherwise.
        Boole MEZZ_LIB Overlap(const Sphere& Ball1, const Sphere& Ball2);
        /// @brief Checks to see if two Planes are overlapping.
        /// @param Surface1 The first Plane to check for overlap.
        /// @param Surface2 The second Plane to check for overlap.
        /// @return Returns true if the two Planes are overlapping, false otherwise.
        Boole MEZZ_LIB Overlap(const Plane& Surface1, const Plane& Surface2);

        /// @brief Checks to see if a ray intersects with a Plane.
        /// @param Surface The Plane to check for intersection.
        /// @param Cast The casted ray to check for intersection.
        /// @return Returns a std::pair containing whether of not the ray hit, and the point in 3D space where it hit if it did.
        Point3DTestResult MEZZ_LIB Intersects(const Plane& Surface, const Ray& Cast);
        /// @brief Checks to see if a ray intersects with an AABB.
        /// @param Box The AABB to check for intersection.
        /// @param Cast The casted ray to check for intersection.
        /// @return Returns a std::pair containing whether or not the ray hit, and if it did also a ray that is the subsection of the casted ray that went through the AABB.
        GeometryRayTestResult MEZZ_LIB Intersects(const AxisAlignedBox& Box, const Ray& Cast);
        /// @brief Checks to see if a ray intersects with a Sphere.
        /// @param Ball The Sphere to check for intersection.
        /// @param Cast The casted ray to check for intersection.
        /// @return Returns a std::pair containing whether or not the ray hit, and if it did also a ray that is the subsection of the casted ray that went through the Sphere.
        GeometryRayTestResult MEZZ_LIB Intersects(const Sphere& Ball, const Ray& Cast);



    }//MathTools
}//Mezzanine

#endif
