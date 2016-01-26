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
/*
 -----------------------------------------------------------------------------
 This source file is part of ogre-procedural

 For the latest info, see http://code.google.com/p/ogre-procedural/

 Copyright (c) 2010-2013 Michael Broutin

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */
#ifndef _graphicsproceduraltriangulator_h
#define _graphicsproceduraltriangulator_h

#include "Graphics/Procedural/proceduraldatatypes.h"
#include "Graphics/Procedural/Mesh/meshgenerator.h"

#include "linesegment.h"
#include "triangle.h"
#include "track.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            class Shape;
            class MultiShape;

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A segment of 2 points in a Point2DContainer.
            /// @details
            ///////////////////////////////////////
            struct DelaunaySegment
            {
                ///////////////////////////////////////////////////////////////////////////////
                // Public Data Members

                /// @brief The first index of the segment.
                /// @note These indexes refer to Vector2's in a Point2DContainer.
                Integer Index1;
                /// @brief The second index of the segment.
                /// @note These indexes refer to Vector2's in a Point2DContainer.
                Integer Index2;

                ///////////////////////////////////////////////////////////////////////////////
                // Construction and Destruction

                /// @brief Class constructor.
                /// @param First The first index of the segment.
                /// @param Second The second index of the segment.
                DelaunaySegment(const Integer First, const Integer Second);
                /// @brief Class destructor.
                ~DelaunaySegment();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Gets the inverse of this segment.
                /// @return Returns a copy of this DelaunaySegment with it's indexes reversed.
                DelaunaySegment Inverse() const;
                /// @brief Less-than compare operator.
                /// @param Other The other DelaunaySegment to compare with.
                /// @return Returns true if this DelaunaySegment is less than the other DelaunaySegment, false otherwise.
                Boole operator<(const DelaunaySegment& Other) const;
            };//DelaunaySegment

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A triangle formed from 3 points in a Point2DContainer.
            /// @details
            ///////////////////////////////////////
            struct DelaunayTriangle
            {
                ///////////////////////////////////////////////////////////////////////////////
                // Public Data Members and Types

                /// @enum InsideType
                /// @brief An enum used to describe the proximity of a point to the edge of a Circumcircle generated by the Delaunay Triangulation algorithm.
                enum InsideType
                {
                    IT_Inside             = 0,  ///< Returned when the specified point is inside the circumcircle.
                    IT_Outside            = 1,  ///< Returned when the specified point is outside the circumcircle.
                    IT_BorderlineOutside  = 2   ///< Returned when the specified point is exceedingly close to being outside the circumcircle.
                };//InsideType

                /// @brief An array of indexes forming the triangle.
                /// @note These indexes refer to Vector2's in a Point2DContainer.
                Integer Indexes[3];
                /// @brief An array of Vector2's that for all the triangles/segments being operated on.
                const Point2DContainer* Points;

                ///////////////////////////////////////////////////////////////////////////////
                // Construction and Destruction

                /// @brief Class constructor.
                /// @param List A pointer to the Point2DContainer this triangle resides in.
                DelaunayTriangle(const Point2DContainer* List);
                /// @brief Class destructor.
                ~DelaunayTriangle();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Sets the points that form this triangle.
                /// @param Index0 The first point in the Point2DContainer forming this triangle.
                /// @param Index1 The second point in the Point2DContainer forming this triangle.
                /// @param Index2 The third point in the Point2DContainer forming this triangle.
                void SetVertices(const Integer Index0, const Integer Index1, const Integer Index2);
                /// @brief Will conditionally re-arrange the vertices of this triangle.
                void MakeDirectIfNeeded();

                /// @brief Gets a point in the Point2DContainer by index.
                /// @param Point The index of the point to retrieve.
                /// @return Returns the point at the specified index.
                Vector2 GetPoint(const Integer Point) const;
                /// @brief Gets the central point of this triangle.
                /// @return Returns a Vector2 containing the central point of this triangle.
                Vector2 GetMidPoint() const;
                /// @brief Gets the index of a segment within this triangle.
                /// @exception If a valid segment index is not found then an INTERNAL_EXCEPTION will be thrown.
                /// @param Index0 The first index of the segment to search for.
                /// @param Index1 The second index of the segment to search for.
                /// @return Returns 0-2 if a valid segment is found.
                Integer FindSegNumber(const Integer Index0, const Integer Index1) const;
                /// @brief Checks to see of the segment specified via Indexes is a part of this triangle.
                /// @param Index0 The first point of the segment to check for.
                /// @param Index1 The second point of the segment to check for.
                /// @return Returns true if the specified segment is a part of this triangle, false otherwise.
                Boole ContainsSegment(const Integer Index0, const Integer Index1) const;
                /// @brief Checks to see if a point is inside this triangle.
                /// @param Point The point to check.
                /// @return Returns true if the specified point is inside this triangle, false otherwise.
                Boole IsPointInside(const Vector2& Point) const;
                /// @brief Checks to see if a point is inside the Circumcircle generated from this triangle.
                /// @param Point The point to check.
                /// @return Returns true if the the specified point is inside the Circumcircle generated by the Delaunay Triangulation algorithm from this triangle, false otherwise.
                InsideType IsPointInsideCircumcircle(const Vector2& Point) const;
                /// @brief Checks if this triangle is degenerate.
                /// @return Returns true if this triangle is degenerate, false otherwise.
                Boole IsDegenerate() const;
                /// @brief Gets a string description of this triangle.
                /// @return Returns a String containing a description of this triangle.
                String GetDebugDescription() const;

                /// @brief Equality comparison operator.
                /// @param Other The other DelaunayTriangle to compare with.
                /// @return Returns true if ths other DelaunayTriangle buffer is equal to this one, false otherwise.
                Boole operator==(const DelaunayTriangle& Other) const;
            };//DelaunayTriangle

            /// @typedef DelaunayTriangleBuffer
            /// @brief A container type for the storage of DelaunayTriangles.
            typedef std::list<DelaunayTriangle> DelaunayTriangleBuffer;

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A triangle convenience class for some comparison operations.
            /// @details
            ///////////////////////////////////////
            struct TouchSuperTriangle
            {
                ///////////////////////////////////////////////////////////////////////////////
                // Public Data Members

                /// @brief The first index of the segment.
                /// @note These indexes refer to Vector2's in a Point2DContainer.
                Integer Index0;
                /// @brief The second index of the segment.
                /// @note These indexes refer to Vector2's in a Point2DContainer.
                Integer Index1;
                /// @brief The third index of the segment.
                /// @note These indexes refer to Vector2's in a Point2DContainer.
                Integer Index2;

                ///////////////////////////////////////////////////////////////////////////////
                // Construction and Destruction

                /// @brief Class constructor.
                /// @param First The first index of the triangle.
                /// @param Second The second index of the triangle.
                /// @param Third The third index of the triangle.
                TouchSuperTriangle(const Integer First, const Integer Second, const Integer Third);
                /// @brief Class destructor.
                ~TouchSuperTriangle();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Functor operator.
                /// @param Tri The triangle to check.
                /// @return Returns true if this TouchSuperTriangle has any points in common with the specified DelaunayTriangle, false otherwise.
                Boole operator()(const DelaunayTriangle& Tri);
            };//SuperTouchTriangle

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A generator class that implements the Delaunay Triangulation algorithm.
            /// @details This generator works with 2D geometry to generate triangles for 3D meshes.
            ///////////////////////////////////////
            class MEZZ_LIB Triangulator : public MeshGenerator<Triangulator>
            {
            public:
                /// @brief Convenience typedef for the storage of segments to be processed by this class.
                typedef std::vector<LineSegment2D> SegmentContainer;
                /// @brief Convenience typedef for the storage of delaunay segments to be processed by this class.
                typedef std::vector<DelaunaySegment> DelaunaySegmentContainer;
            protected:
                /// @internal
                /// @brief A pointer to the Shape to be triangulated.
                /// @note If this is valid, then the MultiShape and SegmentList pointers will be NULL.  Only one can be triangulated.
                const Shape* ShapeToTriangulate;
                /// @internal
                /// @brief A pointer to the MultiShape to be triangulated.
                /// @note If this is valid, then the Shape and SegmentList pointers will be NULL.  Only one can be triangulated.
                const MultiShape* MultiShapeToTriangulate;
                /// @internal
                /// @brief A pointer to a container of segments forming the shape to triangulate.
                /// @note If this is valid, then the Shape and MultiShape pointers will be NULL.  Only one can be triangulated.
                SegmentContainer* SegmentListToTriangulate;
                /// @internal
                /// @brief A pointer to an optional triangle that encompasses all points to be triangulated.
                /// @note If this is not set, the algorithm will make an educated guess.  This is slower than providing one if the information is available.
                Triangle2D* ManualSuperTriangle;
                /// @internal
                /// @brief Stores whether or not the super triangle should be removed at the end of triangulation.
                Boole RemoveOutside;

                /// @internal
                /// @brief Performs a Delaunay Triangulation on a provided list of points in 2D space and populates a triangle buffer with the results.
                /// @param List The container of Vector2's to perform the triangulation on.
                /// @param TriBuf The triangle buffer storing the results of the triangulation.
                void Delaunay(Point2DContainer& List, DelaunayTriangleBuffer& TriBuf) const;
                /// @internal
                /// @brief Forces specific segments to be present in the resulting triangulation.
                /// @param TriBuf The triangule buffer storing the results of the triangulation.
                /// @param List The container of Vector2's to perform the triangulation on.
                /// @param SegmentListIndices A container of indexes to segments that will be forced to exist in the resulting triangulation.
                void _AddConstraints(DelaunayTriangleBuffer& TriBuf, const Point2DContainer& List, const IndexContainer& SegmentListIndices) const;
                /// @internal
                /// @brief Recursively generates a Delaunay triangle from a container of points.
                /// @param CuttingSeg The segment used to generate the previous delaunay triangle that is a part of the provided available points.
                /// @param InputPoints The remaining available points to generate a triangle from.
                /// @param TriBuf The triangule buffer storing the results of the triangulation.
                /// @param List The container of Vector2's to perform the triangulation on.
                void _RecursiveTriangulatePolygon(const DelaunaySegment& CuttingSeg, const IndexContainer& InputPoints, DelaunayTriangleBuffer& TriBuf, const Point2DContainer& List) const;
            public:
                /// @brief Class constructor.
                Triangulator();
                /// @brief Class destructor.
                virtual ~Triangulator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc MeshGenerator::AddToTriangleBuffer(TriangleBuffer&) const
                virtual void AddToTriangleBuffer(TriangleBuffer& Buffer) const;
                /// @brief Executes the Constrained Delaunay Triangulation algorithm.
                /// @exception An INVALID_STATE_EXCEPTION will be thrown if either a Shape, MultiShape, or Segment List is not set when this is called.
                /// @param Indexes A vector of indexes where is outputed the resulting triangle indexes.
                /// @param Vertices A vector of vertices where is outputed the resulting triangle vertices.
                void Triangulate(IndexContainer& Indexes, Point2DContainer& Vertices) const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets shape to triangulate.
                /// @note A Shape, MultiShape, or SegmentList must be available in order to triangulate and only one will be processed.  If one gets set, the other two will be NULL'd out.
                /// @param TriShape The shape that will be triangulated.
                /// @return Returns a reference to this.
                Triangulator& SetShapeToTriangulate(const Shape* TriShape);
                /// @brief Sets multi shape to triangulate.
                /// @note A Shape, MultiShape, or SegmentList must be available in order to triangulate and only one will be processed.  If one gets set, the other two will be NULL'd out.
                /// @param TriMultiShape The multi-shape that will be triangulated.
                /// @return Returns a reference to this.
                Triangulator& SetMultiShapeToTriangulate(const MultiShape* TriMultiShape);
                /// @brief Sets segment list to triangulate.
                /// @note A Shape, MultiShape, or SegmentList must be available in order to triangulate and only one will be processed.  If one gets set, the other two will be NULL'd out.
                /// @param Segments A pointer to the container of line segments that will be triangulated.
                /// @return Returns a reference to this.
                Triangulator& SetSegmentListToTriangulate(SegmentContainer* Segments);
                /// @brief Sets manual super triangle.
                /// @note If this is not set, the algorithm will make an educated guess.  This is slower than providing one if the information is available.
                /// @param Tri The super triangle to manually set for triangulation.
                /// @return Returns a reference to this.
                Triangulator& SetManualSuperTriangle(Triangle2D* Tri);
                /// @brief Sets if the outside of the shape must be removed.
                /// @param RemoveOutside Whether or not the outside of the shape should be removed during triangulation.
                /// @return Returns a reference to this.
                Triangulator& SetRemoveOutside(const Boole Remove);
            };//Triangulator
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
