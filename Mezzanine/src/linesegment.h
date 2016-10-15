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
#ifndef _linesegment_h
#define _linesegment_h

#ifndef SWIG
    #include "MathTools/mathtypes.h"
#endif

#include "vector2.h"
#include "vector3.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A geometry math class for expressing a line connecting 2 points in 2D space.
    ///////////////////////////////////////
    class MEZZ_LIB LineSegment2D
    {
    public:
        /// @brief This is a type used for the return of a intersection test.
        /// @details This type provides more verbose return data that can be used for further tests.  @n @n
        /// The first member stores whether or not there was a hit.  The second member stores the point in 2D space where the segments intersect.
        typedef MathTools::Point2DTestResult IntersectionTestResult;

        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief The first point defining the segment.
        Vector2 PointA;
        /// @brief The second point defining the segment.
        Vector2 PointB;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Blank constructor.
        LineSegment2D();
        /// @brief Descriptive constructor.
        /// @param A The first point defining the segment.
        /// @param B The second point defining the segment.
        LineSegment2D(const Vector2& A, const Vector2& B);
        /// @brief Copy constructor.
        /// @param Other The other line segment to copy from.
        LineSegment2D(const LineSegment2D& Other);

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets whether or not another line segment intersects with this one.
        /// @param Other The other line segment to compare with.
        /// @return Returns true if this line segment intersects with the other segment, false otherwise.
        IntersectionTestResult Intersects(const LineSegment2D& Other) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other line segment to copy from.
        void operator=(const LineSegment2D& Other);
        /// @brief Equality comparison operator.
        /// @param Other The other line segment to compare with.
        /// @return Returns true if the two line segments are equal, false otherwise.
        Boole operator==(const LineSegment2D& Other) const;
        /// @brief Inequality comparison operator.
        /// @param Other The other line segment to compare with.
        /// @return Returns true if the two line segments are not equal, false otherwise.
        Boole operator!=(const LineSegment2D& Other) const;
    };//LineSegment2D

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A geometry math class for expressing a line connecting 2 points in 3D space.
    ///////////////////////////////////////
    class MEZZ_LIB LineSegment3D
    {
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief The first point defining the segment.
        Vector3 PointA;
        /// @brief The second point defining the segment.
        Vector3 PointB;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Blank constructor.
        LineSegment3D();
        /// @brief Descriptive constructor.
        /// @param A The first point defining the segment.
        /// @param B The second point defining the segment.
        LineSegment3D(const Vector3& A, const Vector3& B);
        /// @brief Copy constructor.
        /// @param Other The other line segment to copy from.
        LineSegment3D(const LineSegment3D& Other);

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Equality comparison with some slack.
        /// @param Other The other LineSegment3D to compare with.
        /// @return Returns true if the two line segments are equal within the epsilon.
        Boole EpsilonEquivalent(const LineSegment3D& Other) const;
        /// @brief Gets a copy of this line segment with it's members ordered by length.
        /// @return Returns a LineSegment3D with the same values as this, but with PointA guaranteed to be the vector with the shorter length.
        LineSegment3D GetOrderedCopy() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other line segment to copy from.
        void operator=(const LineSegment3D& Other);
        /// @brief Equality comparison operator.
        /// @param Other The other line segment to compare with.
        /// @return Returns true if the two line segments are equal, false otherwise.
        Boole operator==(const LineSegment3D& Other) const;
        /// @brief Inequality comparison operator.
        /// @param Other The other line segment to compare with.
        /// @return Returns true if the two line segments are not equal, false otherwise.
        Boole operator!=(const LineSegment3D& Other) const;
    };//LineSegment3D
}//Mezzanine

#ifndef SWIG
/// @brief Streaming output operator.
/// @details This converts the data of the LineSegment2D into a stream Ideal for sending to a log or cout.
/// @param Stream This is the stream we send our data to.
/// @param Data The data to be streamed.
/// @return This returns an std::ostream which now contains our data.
std::ostream& MEZZ_LIB operator << (std::ostream& Stream, const Mezzanine::LineSegment2D& Data);
/// @brief Streaming output operator.
/// @details This converts the data of the LineSegment3D into a stream Ideal for sending to a log or cout.
/// @param Stream This is the stream we send our data to.
/// @param Data The data to be streamed.
/// @return This returns an std::ostream which now contains our data.
std::ostream& MEZZ_LIB operator << (std::ostream& Stream, const Mezzanine::LineSegment3D& Data);
#endif

#endif
