// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _triangle_h
#define _triangle_h

#include "vector2.h"
#include "vector3.h"
#include "MathTools/mathtypes.h"

namespace Mezzanine
{
    class Ray;
    class LineSegment2D;
    class LineSegment3D;
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A geometry math class for expressing a triangle in 2D space.
    ///////////////////////////////////////
    class MEZZ_LIB Triangle2D
    {
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief The first point in space making the triangle.
        Vector2 PointA;
        /// @brief The second point in space making the triangle.
        Vector2 PointB;
        /// @brief The third point in space making the triangle.
        Vector2 PointC;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Blank constructor.
        Triangle2D() = default;
        /// @brief Copy constructor.
        /// @param Other The other triangle to be copied.
        Triangle2D(const Triangle2D& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other triangle to be moved.
        Triangle2D(Triangle2D&& Other) = default;
        /// @brief Descriptive constructor.
        /// @param A The first point in space making the triangle.
        /// @param B The second point in space making the triangle.
        /// @param C The third point in space making the triangle.
        Triangle2D(const Vector2& A, const Vector2& B, const Vector2& C);

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment Operator.
        /// @param Other The other Triangle2D to copy.
        /// @return Returns a reference to this.
        Triangle2D& operator=(const Triangle2D& Other) = default;
        /// @brief Move assignment operator.
        /// @param Other The other Triangle2D to move.
        /// @return Returns a reference to this.
        Triangle2D& operator=(Triangle2D&& Other) = default;

        /// @brief Gets the point in this triangle corresponding to the specified index.
        /// @exception If the index passed in is greater than 2, a PARAMETERS_RANGE_EXCEPTION will be thrown.
        /// @param Index The index of the point to retrieve.
        /// @return Returns a reference to a Vector2 containing the position of the specified point.
        Vector2& operator[](const Whole& Index);
        /// @brief Gets the point in this triangle corresponding to the specified index.
        /// @exception If the index passed in is greater than 2, a PARAMETERS_RANGE_EXCEPTION will be thrown.
        /// @param Index The index of the point to retrieve.
        /// @return Returns a const reference to a Vector2 containing the position of the specified point.
        const Vector2& operator[](const Whole& Index) const;
    };//Triangle2D

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A geometry math class for expressing a triangle in 3D space.
    ///////////////////////////////////////
    class MEZZ_LIB Triangle3D
    {
    public:
        /// @brief This is a type used for the return of a ray intersection test.
        /// @details This type provides more verbose return data that can be used for further tests.  @n @n
        /// The first member stores whether or not there was a hit.  The second member stores the point where it was hit.
        typedef MathTools::PlaneTestResult RayTestResult;

        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief The first point in space making the triangle.
        Vector3 PointA;
        /// @brief The second point in space making the triangle.
        Vector3 PointB;
        /// @brief The third point in space making the triangle.
        Vector3 PointC;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Blank constructor.
        Triangle3D() = default;
        /// @brief Copy constructor.
        /// @param Other The other triangle to be copied.
        Triangle3D(const Triangle3D& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other triangle to be moved.
        Triangle3D(Triangle3D&& Other) = default;
        /// @brief Descriptive constructor.
        /// @param A The first point in space making the triangle.
        /// @param B The second point in space making the triangle.
        /// @param C The third point in space making the triangle.
        Triangle3D(const Vector3& A, const Vector3& B, const Vector3& C);

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the overlap of two triangles.
        /// @param Other The other triangle to compare with.
        /// @return Returns a line segment expressing the length where the two triangles overlap, or a zero'd out line segment if there is no overlap.
        LineSegment3D GetOverlap(const Triangle3D& Other) const;

        /// @brief Checks to see if a ray intersects this triangle.
        /// @param ToCheck The ray to check for a hit.
        /// @return Returns true if the ray intersects this triangle, false otherwise.
        RayTestResult Intersects(const Ray& ToCheck) const;

        /// @brief Calculates a normal for the triangle.
        /// @return Returns a normal indicating the "front" facing direction of the triangle.
        Vector3 CalculateBasicNormal() const;
        /// @brief Calculates a normal for the triangle.
        /// @param A The first point in space making the triangle.
        /// @param B The second point in space making the triangle.
        /// @param C The third point in space making the triangle.
        /// @return Returns a normal indicating the "front" facing direction of the triangle.
        static Vector3 CalculateBasicNormal(const Vector3& A, const Vector3& B, const Vector3& C);

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment Operator.
        /// @param Other The other Triangle3D to copy.
        /// @return Returns a reference to this.
        Triangle3D& operator=(const Triangle3D& Other) = default;
        /// @brief Move assignment operator.
        /// @param Other The other Triangle3D to move.
        /// @return Returns a reference to this.
        Triangle3D& operator=(Triangle3D&& Other) = default;
        /// @brief Equality comparison operator.
        /// @param Other The other Triangle3D to compare to.
        /// @return Returns true if the two Triangle3D's are equal, false otherwise.
        Boole operator==(const Triangle3D& Other) const;
        /// @brief Inequality comparison operator.
        /// @param Other The other Triangle3D to compare to.
        /// @return Returns true if the two Triangle3D's are not equal, false otherwise.
        Boole operator!=(const Triangle3D& Other) const;

        /// @brief Gets the point in this triangle corresponding to the specified index.
        /// @exception If the index passed in is greater than 2, a PARAMETERS_RANGE_EXCEPTION will be thrown.
        /// @param Index The index of the point to retrieve.
        /// @return Returns a reference to a Vector2 containing the position of the specified point.
        Vector3& operator[](const Whole Index);
        /// @brief Gets the point in this triangle corresponding to the specified index.
        /// @exception If the index passed in is greater than 2, a PARAMETERS_RANGE_EXCEPTION will be thrown.
        /// @param Index The index of the point to retrieve.
        /// @return Returns a const reference to a Vector2 containing the position of the specified point.
        const Vector3& operator[](const Whole Index) const;
    };//Triangle3D
}//Mezzanine

#endif
