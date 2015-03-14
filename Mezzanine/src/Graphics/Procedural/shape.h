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
#ifndef _graphicsproceduralshape_h
#define _graphicsproceduralshape_h

#include "Graphics/Procedural/proceduraldatatypes.h"
#include "Graphics/Procedural/proceduralenumerations.h"

#include "linesegment.h"
#include "track.h"

namespace Ogre
{
    class ManualObject;
}

namespace Mezzanine
{
    namespace Graphics
    {
        class Mesh;
        namespace Procedural
        {
            class Path;
            class MultiShape;
            struct IntersectionInShape;
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A collection of interconnected 2D points used to express an arbitrary 2D shape.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB Shape
            {
            public:
                /// @brief Basic container type for the storage of Intersections in this class.
                typedef std::vector<IntersectionInShape>          IntersectionContainer;
                /// @brief Iterator type for Intersections stored in this class.
                typedef IntersectionContainer::iterator           IntersectionIterator;
                /// @brief Const Iterator type for Intersections stored in this class.
                typedef IntersectionContainer::const_iterator     ConstIntersectionIterator;
            protected:
                /// @internal
                /// @brief Container storing all of the points that form this shape.
                Point2DContainer Points;
                /// @internal
                /// @brief Sets which extreme side of this shape is to be considered the outside of the shape.  Useful when a shape is not closed.
                Procedural::ShapeSide OutSide;
                /// @internal
                /// @brief Whether or not the first point and last point should be connected, closing the shape.
                Boole Closed;

                /// @internal
                /// @brief Performs a boolean operation between this shape and another shape.
                /// @param Other The other shape to perform the boolean operation with.
                /// @param OpType The type of boolean operation to perform.
                /// @return Returns a MultiShape containing the results of the boolean operation.
                MultiShape _BooleanOperation(const Shape& Other, const Procedural::BooleanOperation OpType) const;
                /// @internal
                /// @brief Gets whether or not the current boolean operation is looking for outside edges of the two shapes being processed.
                /// @param OpType The type of boolean operation being performed.
                /// @param ShapeSelector A 0 or 1 describing which shape the current edge being processed belongs to.
                /// @return Returns true if the current operation is seeking the outside edges of the two shapes, or false if it's seeking the inner edges.
                Boole _IsLookingForOutside(const Procedural::BooleanOperation OpType, const Char8 ShapeSelector) const;
                /// @internal
                /// @brief Gets whether or not the segment being operated on is to be incremented.
                /// @param Dot The Dot Product of the directions from the two points in an intersection.
                /// @param OpType The type of boolean operation being performed.
                /// @param ShapeSelector A 0 or 1 describing which shape the current edge being processed belongs to.
                /// @return Returns 1 if the shape is being increased, or -1 if the shape is being decreased.
                Char8 _IsIncreasing(const Real Dot, const Procedural::BooleanOperation OpType, const Char8 ShapeSelector) const;
                /// @internal
                /// @brief Determines which segment to follow when an intersection is encountered.
                /// @param InputShapes An array of shapes being operated on.
                /// @param OpType The type of boolean operation being performed.
                /// @param Intersection The intersection that was encountered.
                /// @param ShapeSelector A 0 or 1 that is the index of the shape that owns the segment to advance along.
                /// @param IsIncreasing A -1 or 1 indicating whether the segment being operated on is to be incremented.
                /// @param CurrentSegment The index of the line segment in the intersection to advance along.
                /// @return Returns true if a valid segment to follow was found, or false if not viable segments were found.
                Boole _FindWhereToGo(const Shape* InputShapes[], const Procedural::BooleanOperation OpType, const IntersectionInShape& Intersection, UInt8& ShapeSelector, Char8& IsIncreasing, Whole& CurrentSegment) const;
                /// @internal
                /// @brief Finds all detectable intersections between this shape and another shape.
                /// @param Other The other shape to compare with this one.
                /// @param Intersections A container of to be populated with the known intersections between this shape and another.
                void _FindAllIntersections(const Shape& Other, IntersectionContainer& Intersections) const;
            public:
                /// @brief Class constructor.
                Shape();
                /// @brief Class destructor.
                ~Shape();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Outputs a mesh representing this shape.
                /// @param Name The name of the mesh to be generated.
                /// @param Group The name of the resource group to create the mesh in.
                /// @return Returns a pointer to the new mesh.
                Mesh* GenerateMesh(const String& Name, const String& Group) const;

                /// @brief Appends all the points from another shape to this shape.
                /// @param Other The other shape to append to this shape.
                /// @return Returns a reference to this.
                Shape& AppendShape(const Shape& Other);
                /// @brief Appends all the points from another shape to this shape with their positions relative to the position of the last point in this shape.
                /// @param Other The other shape to append to this shape.
                /// @return Returns a reference to this.
                Shape& AppendShapeRel(const Shape& Other);
                /// @brief Extracts a part of the shape as a new shape.
                /// @param First The first index of this shape to be in the new shape.
                /// @param Last The last index of this shape to be in the new shape.
                /// @return Returns a new shape containing the specified points from this shape.
                Shape ExtractSubShape(const Whole First, const Whole Last);

                /// @brief Appends the contents of a 2D track to this shape.
                /// @remarks Low values for NumPoints will cause curves to look blocky.  How how it should be depends on the resolution of the curve you want.
                /// @param Curve The 2D curved track of points to append to this shape.
                /// @param NumPoints The number of points along the curve to generate for this shape.
                template< template<class> class Interpolator >
                void AppendTrack(const Track< Interpolator< Vector2 > >& Curve, const Whole NumPoints)
                {
                    // Minus one because we want to include the end of the curve without going over the number of points requested
                    for( Whole CurrPoint = 0 ; CurrPoint < NumPoints ; ++CurrPoint )
                    {
                        Real InterpolateVal = static_cast<Real>( CurrPoint ) / static_cast<Real>( NumPoints - 1 );
                        Vector2 CurvePoint = Curve.GetInterpolated(InterpolateVal);
                        this->AddPoint(CurvePoint);
                    }
                }

                /// @brief Gets the number of segments in this shape.
                /// @return Returns a Whole containing the number of segments that form this shape.
                Whole GetSegCount() const;
                /// @brief Gets the total length of all segments in this shape.
                /// @return Returns a Real representing the combined length of each segment in this shape.
                Real GetTotalLength() const;
                /// @brief Tells whether a point is inside a shape or not.
                /// @param Point The point to check.
                /// @return Returns true if the point is inside this shape, false otherwise.
                Boole IsPointInside(const Vector2& Point) const;
                /// @brief Computes the radius of a bounding circle centered on the origin of this shape.
                /// @return Returns a Real representing the bounding radius of this shape.
                Real FindBoundingRadius() const;

                /// @brief Gets the position on a segment.
                /// @exception If the provided index is out of bounds or the Coord is outside of the range 0-1 then a PARAMETERS_EXCEPTION will be thrown.
                /// @param Index The index of the point starting the segment to retrieve the point from.
                /// @param Coord The relative point on the specified segment to get the position of.
                /// @return Returns a Vector2 containing the point at the specified position.
                Vector2 GetPosition(const Whole Index, const Real Coord) const;
                /// @brief Gets the position at the specified length along the shape.
                /// @exception If the shape has less than 2 points then a PARAMETERS_EXCEPTION will be thrown.
                /// @param Coord The length position to get the 2D position of.
                /// @return Returns a Vector2 containing the position of the point at the specified length.
                Vector2 GetPosition(Real Coord) const;

                /// @brief Create a symetric copy of the points in this shape at the origin point.
                /// @param Flip If true then this will process the points in this shape in reverse order.
                /// @return Returns a reference to this.
                Shape& Mirror(Boole Flip = false);
                /// @brief Create a symetric copy of the points in this shape at an arbitrary point.
                /// @param X The position on the X axis to mirror at.
                /// @param Y The position on the Y axis to mirror at.
                /// @param Flip If true then this will process the points in this shape in reverse order.
                /// @return Returns a reference to this.
                Shape& MirrorAroundPoint(const Real X, const Real Y, Boole Flip = false);
                /// @brief Create a symetric copy of the points in this shape at an arbitrary point.
                /// @param Point The position to mirror at.
                /// @param Flip If true then this will process the points in this shape in reverse order.
                /// @return Returns a reference to this.
                Shape& MirrorAroundPoint(const Vector2& Point, Boole Flip = false);
                /// @brief Create a symetric copy of the points in this shape at an arbitrary axis.
                /// @param Axis The axis to mirror at.
                /// @param Flip If true then this will process the points in this shape in reverse order.
                /// @return Returns a reference to this.
                Shape& MirrorAroundAxis(const Vector2& Axis, Boole Flip = false);

                /// @brief Reflect all points in this shape against a zero-origined line with a given normal.
                /// @param Normal The normal to reflect all points on.
                /// @return Returns a reference to this.
                Shape& Reflect(const Vector2& Normal);

                /// @brief Makes this a closed shape, connecting the last point to the first point.
                /// @return Returns a reference to this.
                Shape& Close();
                /// @brief Gets whether or not this shape is closed.
                /// @return Returns true if the last point is connected to the first, false otherwise.
                Boole IsClosed() const;

                /// @brief Reverses direction/ordering of the segments in this shape.
                /// @return Returns a reference to this.
                Shape& Reverse();
                /// @brief Clears all points from this shape.
                /// @return Returns a reference to this.
                Shape& Reset();

                ///////////////////////////////////////////////////////////////////////////////
                // Point Management

                /// @brief Adds a point to the shape.
                /// @param Point The point to be added.
                /// @return Returns a reference to this.
                Shape& AddPoint(const Vector2& Point);
                /// @brief Adds a point to the shape.
                /// @param X The position of the point to add on the X axis.
                /// @param Y The position of the point to add on the Y axis.
                /// @return Returns a reference to this.
                Shape& AddPoint(const Real X, const Real Y);
                /// @brief Adds a point to the shape, relative to the last position added.
                /// @param Point The position of the point to be added that is in local space of the last position added.
                /// @return Returns a reference to this.
                Shape& AddPointRel(const Vector2& Point);
                /// @brief Adds a point to the shape, relative to the last position added.
                /// @param X The relative position of the point to add on the X axis.
                /// @param Y The relative position of the point to add on the Y axis.
                /// @return Returns a reference to this.
                Shape& AddPointRel(const Real X, const Real Y);
                /// @brief Inserts a point to the shape.
                /// @param Index the index before the inserted point.
                /// @param X The position of the point to insert on the X axis.
                /// @param Y The position of the point to insert on the Y axis.
                /// @return Returns a reference to this.
                Shape& InsertPoint(const Whole Index, const Real X, const Real Y);
                /// @brief Inserts a point to the shape.
                /// @param Index the index before the inserted point.
                /// @param Point The point to be inserted.
                /// @return Returns a reference to this.
                Shape& InsertPoint(const Whole Index, const Vector2& Point);

                /// @brief Gets a point by index which can be out of bounds and will wrap.
                /// @param Index The index of the point to retrieve.
                /// @return Returns a const reference to the point at the specified index.
                const Vector2& GetPoint(const Integer Index) const;
                /// @brief Gets the number of points in this shape.
                /// @return Returns a Whole representing the number of points in this shape.
                Whole GetPointCount() const;
                /// @brief Converts a potentially unsafe Integer index into a safe Whole index.
                /// @param Index The index to be converted.
                /// @return Returns the converted safe index.
                Whole GetBoundedIndex(const Integer Index) const;

                /// @brief Gets a copy of raw vector data of this shape.
                /// @return Returns a new Point2DContainer storing a copy of all the points in this shape.
                Point2DContainer GetPoints() const;
                /// @brief Gets raw vector data of this shape as a non-const reference.
                /// @return Returns a reference to the container storing all of the points in this shape.
                Point2DContainer& GetPointsReference();
                /// @brief Gets raw vector data of this shape as a const reference.
                /// @return Returns a const reference to the container storing all of the points in this shape.
                const Point2DContainer& GetPointsReference() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Transform

                /// @brief Applies the given translation to all the points in this shape.
                /// @note This will only transfrom the points currently in this shape, not any additional points made after calling this.
                /// @param Trans The amount of translation to apply.
                /// @return Returns a reference to this.
                Shape& Translate(const Vector2& Trans);
                /// @brief Applies the given translation to all the points in this shape.
                /// @note This will only transfrom the points currently in this shape, not any additional points made after calling this.
                /// @param TransX The amount of translation to apply on the X axis.
                /// @param TransY The amount of translation to apply on the Y axis.
                /// @return Returns a reference to this.
                Shape& Translate(const Real TransX, const Real TransY);
                /// @brief Applies the given rotation to all the points in this shape.
                /// @note This will only transfrom the points currently in this shape, not any additional points made after calling this.
                /// @param Angle The amount of rotation to be applied in radians.
                /// @return Returns a reference to this.
                Shape& Rotate(const Real Angle);
                /// @brief Applies the given scale to all the points in this shape.
                /// @note This will only transfrom the points currently in this shape, not any additional points made after calling this.
                /// @param Scaling The amount of scaling to apply to each axis.
                /// @return Returns a reference to this.
                Shape& Scale(const Real Scaling);
                /// @brief Applies the given scale to all the points in this shape.
                /// @note This will only transfrom the points currently in this shape, not any additional points made after calling this.
                /// @param ScaleX The amount of scaling to be applied on the X axis.
                /// @param ScaleY The amount of scaling to be applied on the Y axis.
                /// @return Returns a reference to this.
                Shape& Scale(const Real ScaleX, const Real ScaleY);
                /// @brief Applies the given scale to all the points in this shape.
                /// @note This will only transfrom the points currently in this shape, not any additional points made after calling this.
                /// @param Scaling The amount of scaling to apply to each axis.
                /// @return Returns a reference to this.
                Shape& Scale(const Vector2& Scaling);

                ///////////////////////////////////////////////////////////////////////////////
                // Directions and Normals

                /// @brief Gets the direction of a point to the next point in the sequence.
                /// @param Index The index of the point to retrieve the direction for.
                /// @return Returns a Vector2 containing the direction from the specified point to the next point.
                Vector2 GetDirectionAfter(const Whole Index) const;
                /// @brief Gets the direction from a point to the previous point in the sequence.
                /// @param Index The index of the point to retrieve the direction for.
                /// @return Returns a Vector2 containing the direction from the specified point to the previous point.
                Vector2 GetDirectionBefore(const Whole Index) const;
                /// @brief Gets the averaged direction from the specified point to both the next and previous points in the sequence.
                /// @param Index The index of the point to retrieve the direction for.
                /// @return Returns a Vector2 containing the averaged direction for both the next and previous points from the specified point.
                Vector2 GetAvgDirection(const Whole Index) const;

                /// @brief Gets the normal of the segment after the specified point.
                /// @param Index The index of the point to retrieve the normal for.
                /// @return Returns the normal of the segment after the specified index.
                Vector2 GetNormalAfter(const Whole Index) const;
                /// @brief Gets the normal of the segment before the specified point.
                /// @param Index The index of the point to retrieve the normal for.
                /// @return Returns the normal of the segment before the specified index.
                Vector2 GetNormalBefore(const Whole Index) const;
                /// @brief Gets the averaged normal of the segments before and after the specified point.
                /// @param Index The index of the point to retrieve the normal for.
                /// @return Returns the averaged normal of the segment before and after the specified index.
                Vector2 GetAvgNormal(const Whole Index) const;

                ///////////////////////////////////////////////////////////////////////////////
                // OutSide

                /// @brief Sets which side is the OutSide of this shape.
                /// @note This is used for such things as normal generation.  Default is right, which corresponds to placing points anti-clockwise.
                /// @param Side A ShapeSide enum value describing which side of this shape is the OutSide.
                /// @return Returns a reference to this.
                Shape& SetOutSide(const Procedural::ShapeSide Side);
                /// @brief Gets which side is the OutSide of this shape.
                /// @return Returns a ShapeSide enum value describing which side of this shape is the OutSide.
                Procedural::ShapeSide GetOutSide() const;
                /// @brief Toggles the currently set "OutSide".
                /// @return Returns a reference to this.
                Shape& SwitchSide();
                /// @brief On a closed shape, find if the outside is located on the right or on the left.
                /// @note If the outside can easily be guessed in your context, you'd rather use SetOutside(), which doesn't need any computation.  @n @n
                /// Also note that this is a guess made by the algorithm.
                /// @return Returns a ShapeSide enum value describing which side of this shape is the OutSide.
                Procedural::ShapeSide FindRealOutSide() const;
                /// @brief Gets whether the currently set OutSide is the real Outside.
                /// @return Returns true if the detected OutSide matches the currently set OutSide, false otherwise.
                Boole IsOutsideRealOutside() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Conversion

                /// @brief Converts the shape to a 3D path.
                /// @return Returns a path where: 3D.X = 2D.X, 3D.Y = 0, 3D.Z = 2D.Y for each point in this shape.
                Path ConvertToPath() const;
                /// @brief Applies a "thickness" to a shape, ie a bit like the extruder, but in 2D.
                /// @param Amount The amount of thickness to be applied to this shape.
                /// @return Returns a MultiShape instance that is the thickened shape.
                MultiShape Thicken(const Real Amount);

                ///////////////////////////////////////////////////////////////////////////////
                // Boolean Operations

                /// @brief Computes the intersection between this shape and another one.
                /// @remarks Both shapes must be closed.
                /// @exception This will throw an exception if either shape is not closed or contains less than 2 points.  A INVALID_STATE_EXCEPTION
                /// will be thrown if the calling shape is invalid, or a PARAMETERS_EXCEPTION if the parameter shape is invalid.
                /// @param Other The shape against which the intersection is computed.
                /// @return The intersection of two shapes, as a new shape.
                MultiShape BooleanIntersect(const Shape& Other) const;
                /// @brief Computes the union between this shape and another one.
                /// @remarks Both shapes must be closed.
                /// @exception This will throw an exception if either shape is not closed or contains less than 2 points.  A INVALID_STATE_EXCEPTION
                /// will be thrown if the calling shape is invalid, or a PARAMETERS_EXCEPTION if the parameter shape is invalid.
                /// @param Other The shape against which the union is computed.
                /// @return The union of two shapes, as a new shape.
                MultiShape BooleanUnion(const Shape& Other) const;
                /// @brief Computes the difference between this shape and another one.
                /// @remarks Both shapes must be closed.
                /// @exception This will throw an exception if either shape is not closed or contains less than 2 points.  A INVALID_STATE_EXCEPTION
                /// will be thrown if the calling shape is invalid, or a PARAMETERS_EXCEPTION if the parameter shape is invalid.
                /// @param Other The shape against which the diffenrence is computed.
                /// @return The difference of two shapes, as a new shape.
                MultiShape BooleanDifference(const Shape& Other) const;

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @brief Appends the shape vertices to a manual object being edited.
                /// @param Object The manual object to append to.
                void _AppendToManualObject(Ogre::ManualObject* Object) const;
            };//Shape
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
