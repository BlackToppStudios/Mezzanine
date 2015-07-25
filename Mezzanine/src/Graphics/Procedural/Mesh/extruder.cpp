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
#ifndef _graphicsproceduralextruder_cpp
#define _graphicsproceduralextruder_cpp

#include "Graphics/Procedural/Mesh/extruder.h"
#include "Graphics/Procedural/Mesh/vertextransformmodifier.h"
#include "Graphics/Procedural/Mesh/vertexuvmodifier.h"

#include "Graphics/Procedural/path.h"
#include "Graphics/Procedural/shape.h"
#include "Graphics/Procedural/triangulator.h"

#include "MathTools/mathtools.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            Extruder::Extruder() :
                Capped(true)
                {  }

            Extruder::~Extruder()
                {  }

            Vector2 Extruder::GenerateUVs(const Whole PathIndex, const Whole ShapeIndex, const Real PathCoord, const Real ShapeCoord) const
            {
                Vector2 Ret(PathCoord,ShapeCoord);
                ConstTrackMapIterator PathUIt = this->PathUTextureTracks.find(PathIndex);
                if( PathUIt != this->PathUTextureTracks.end() ) {
                    Ret.X = (*PathUIt).second->GetInterpolated(PathCoord);
                }
                ConstTrackMapIterator ShapeVIt = this->ShapeVTextureTracks.find(ShapeIndex);
                if( ShapeVIt != this->ShapeVTextureTracks.end() ) {
                    Ret.Y = (*ShapeVIt).second->GetInterpolated(ShapeCoord);
                }
                return Ret;
            }

            void Extruder::GenerateExtrusionShape(TriangleBuffer& Buffer, const Whole PathIndex, const Whole ShapeIndex, const Real PathCoord, const Vector3& Position, const Quaternion& LeftOri,
                                                  const Quaternion& RightOri, const Real Scale, const Real ScaleLeftCorrect, const Real ScaleRightCorrect, const Boole JoinWithNextShape) const
            {
                const Shape& ExtrudedShape = this->ShapesToExtrude.GetShape(ShapeIndex);
                Real LineicShapePosition = 0.0;
                Real TotalShapeLength = ExtrudedShape.GetTotalLength();
                Whole NumShapeSegments = ExtrudedShape.GetSegCount();
                // Insert new points
                for( Whole PointIndex = 0 ; PointIndex <= NumShapeSegments ; ++PointIndex )
                {
                    Vector2 Point2D = ExtrudedShape.GetPoint(PointIndex);
                    Vector2 Point2DNorm = ExtrudedShape.GetAvgNormal(PointIndex);
                    Quaternion Orientation = ( Point2D.X > 0 ) ? RightOri : LeftOri;
                    Vector3 Point3D = ( Point2D.X > 0 ) ? Vector3(ScaleRightCorrect * Point2D.X, Point2D.Y, 0) : Vector3(ScaleLeftCorrect * Point2D.X, Point2D.Y, 0);
                    Vector3 Normal(Point2DNorm.X, Point2DNorm.Y, 0);
                    Buffer.RebaseOffset();
                    Vector3 NewPoint = Position + ( Orientation * ( Point3D * Scale ) );
                    if( PointIndex > 0 ) {
                        LineicShapePosition += ( Point2D - ExtrudedShape.GetPoint( PointIndex - 1 ) ).Length();
                    }

                    Buffer.AddVertex(NewPoint, Orientation * Normal, this->GenerateUVs(PathIndex,ShapeIndex,PathCoord,LineicShapePosition / TotalShapeLength));

                    if( PointIndex < NumShapeSegments && JoinWithNextShape ) {
                        if( ExtrudedShape.GetOutSide() == Procedural::SS_Left ) {
                            Buffer.AddTriangle(NumShapeSegments + 1, NumShapeSegments + 2, 0);
                            Buffer.AddTriangle(0, NumShapeSegments + 2, 1);
                        }else{
                            Buffer.AddTriangle(NumShapeSegments + 2, NumShapeSegments + 1, 0);
                            Buffer.AddTriangle(NumShapeSegments + 2, 0, 1);
                        }
                    }
                }
            }

            void Extruder::GenerateExtrusionCaps(TriangleBuffer& Buffer) const
            {
                IndexContainer CapIndexes;
                Point2DContainer CapPoints;

                Triangulator CapTri;
                CapTri.SetMultiShapeToTriangulate(&this->ShapesToExtrude);
                CapTri.Triangulate(CapIndexes,CapPoints);

                for( Whole PathIndex = 0 ; PathIndex < this->PathsToExtrude.GetNumPaths() ; ++PathIndex )
                {
                    const Path& CurrPath = this->PathsToExtrude.GetPath(PathIndex);
                    const ParameterTrack* CurrScaleTrack = NULL;
                    ConstTrackMapIterator ScaleIt = this->ScaleTracks.find(PathIndex);
                    if( ScaleIt != this->ScaleTracks.end() ) {
                        CurrScaleTrack = (*ScaleIt).second;
                    }
                    const ParameterTrack* CurrRotationTrack = NULL;
                    ConstTrackMapIterator RotationIt = this->RotationTracks.find(PathIndex);
                    if( RotationIt != this->RotationTracks.end() ) {
                        CurrRotationTrack = (*RotationIt).second;
                    }

                    const MultiPath::IntersectionMap& PathIntersections = this->PathsToExtrude.GetIntersectionsMap();
                    // Begin Cap
                    if( PathIntersections.find( PathCoordinate(PathIndex,0) ) == PathIntersections.end() ) {
                        Buffer.RebaseOffset();
                        Buffer.EstimateIndexCount(CapIndexes.size());
                        Buffer.EstimateVertexCount(CapPoints.size());

                        Quaternion QuatEnd(CurrPath.GetDirectionAfter(0),Vector3::Unit_Y());
                        if( CurrRotationTrack ) {
                            Real Angle = CurrRotationTrack->GetInterpolated(0.0);
                            QuatEnd = QuatEnd * Quaternion(Angle,Vector3::Unit_Z());
                        }

                        Real ScaleBegin = 1.0;
                        if( CurrScaleTrack ) {
                            ScaleBegin = CurrScaleTrack->GetInterpolated(0.0);
                        }

                        for( Whole CurrPoint = 0 ; CurrPoint < CapPoints.size() ; ++CurrPoint )
                        {
                            Vector2 Point2D = CapPoints[CurrPoint];
                            Vector3 Location( Point2D.X, Point2D.Y, 0 );
                            Vector3 Normal = Vector3::Neg_Unit_Z();

                            Vector3 VertLoc = CurrPath.GetPoint(0) + ( QuatEnd * ( Location * ScaleBegin ) );
                            Buffer.AddVertex( VertLoc, QuatEnd * Normal, Point2D );
                        }

                        for( Whole CurrIndex = 0 ; CurrIndex < ( CapIndexes.size() / 3 ) ; ++CurrIndex )
                        {
                            Buffer.AddIndex( CapIndexes[ CurrIndex * 9 ] );
                            Buffer.AddIndex( CapIndexes[ CurrIndex * 9 + 2 ] );
                            Buffer.AddIndex( CapIndexes[ CurrIndex * 9 + 1 ] );
                        }
                    }
                    // End Cap
                    if( PathIntersections.find( PathCoordinate(PathIndex,CurrPath.GetSegCount()) ) == PathIntersections.end() ) {
                        Buffer.RebaseOffset();
                        Buffer.EstimateIndexCount(CapIndexes.size());
                        Buffer.EstimateVertexCount(CapPoints.size());

                        Quaternion QuatEnd(CurrPath.GetDirectionBefore(CurrPath.GetSegCount()),Vector3::Unit_Y());
                        if( CurrRotationTrack ) {
                            Real Angle = CurrRotationTrack->GetInterpolated(1.0);
                            QuatEnd = QuatEnd * Quaternion(Angle,Vector3::Unit_Z());
                        }

                        Real ScaleEnd = 1.0;
                        if( CurrScaleTrack ) {
                            ScaleEnd = CurrScaleTrack->GetInterpolated(1.0);
                        }

                        for( Whole CurrPoint = 0 ; CurrPoint < CapPoints.size() ; ++CurrPoint )
                        {
                            Vector2 Point2D = CapPoints[CurrPoint];
                            Vector3 Location( Point2D.X, Point2D.Y, 0 );
                            Vector3 Normal = Vector3::Unit_Z();

                            Vector3 VertLoc = CurrPath.GetPoint(0) + ( QuatEnd * ( Location * ScaleEnd ) );
                            Buffer.AddVertex( VertLoc, QuatEnd * Normal, Point2D );
                        }

                        for( Whole CurrIndex = 0 ; CurrIndex < ( CapIndexes.size() / 3 ) ; ++CurrIndex )
                        {
                            Buffer.AddIndex( CapIndexes[ CurrIndex * 9 ] );
                            Buffer.AddIndex( CapIndexes[ CurrIndex * 9 + 1 ] );
                            Buffer.AddIndex( CapIndexes[ CurrIndex * 9 + 2 ] );
                        }
                    }
                }
            }

            void Extruder::GenerateExtrusionSegment(TriangleBuffer& Buffer, const Whole ShapeIndex, const Whole PathIndex, const Whole PathSegBegin, const Whole PathSegEnd) const
            {
                const Path& CurrPath = this->PathsToExtrude.GetPath(PathIndex);
                const Shape& CurrShape = this->ShapesToExtrude.GetShape(ShapeIndex);

                Whole NumSegmentsToExtrude = PathSegEnd - PathSegBegin;
                Whole NumSegmentsInShape = CurrShape.GetSegCount();

                if( NumSegmentsToExtrude == 0 || NumSegmentsInShape == 0 ) {
                    MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"The shape and path provided must contain at least 2 points to extrude.");
                }

                // Lineic Position variables
                Real TotalPathLength = CurrPath.GetTotalLength();
                //Real TotalShapeLength = CurrShape.GetTotalLength();
                Real LineicPosition = CurrPath.GetLengthAtPoint(PathSegBegin);

                // Iterators for later
                ConstTrackMapIterator RotationTrackResult = this->RotationTracks.find(PathIndex);
                ConstTrackMapIterator ScaleTrackResult = this->ScaleTracks.find(PathIndex);

                // Prepare the buffer
                Buffer.RebaseOffset();
                Buffer.EstimateVertexCount( ( NumSegmentsInShape + 1 ) * ( NumSegmentsToExtrude + 1 ) );
                Buffer.EstimateIndexCount( NumSegmentsInShape * NumSegmentsToExtrude * 6 );

                // Should this initialize to the Y axis instead of zero?
                Vector3 OldUp;
                for( Whole PointIndex = PathSegBegin ; PointIndex <= PathSegEnd ; ++PointIndex )
                {
                    Vector3 CurrPoint = CurrPath.GetPoint(PointIndex);
                    Vector3 Direction = CurrPath.GetAvgDirection(PointIndex);

                    // Lineic Position
                    if( PointIndex > PathSegBegin ) {
                        LineicPosition += ( CurrPoint - CurrPath.GetPoint( PointIndex - 1 ) ).Length();
                    }
                    Real RelativeLineicPosition = LineicPosition / TotalPathLength;

                    // Rotation Prep
                    Quaternion PointRotation(Direction,Vector3::Unit_Y());
                    Real Angle = ( PointRotation * Vector3::Unit_Y() ).AngleBetween(OldUp);
                    if( PointIndex > PathSegBegin && Angle > ( MathTools::GetHalfPi() * 0.5 ) ) {
                        PointRotation.SetFromAxisToZ(Direction,OldUp);
                    }
                    OldUp = PointRotation * Vector3::Unit_Y();

                    // Rotation
                    if( RotationTrackResult != this->RotationTracks.end() ) {
                        Real TempAngle = (*RotationTrackResult).second->GetInterpolated(RelativeLineicPosition);
                        PointRotation = PointRotation * Quaternion(TempAngle,Vector3::Unit_Z());
                    }

                    // Scale
                    Real PointScale = 1.0;
                    if( ScaleTrackResult != this->ScaleTracks.end() ) {
                        PointScale = (*ScaleTrackResult).second->GetInterpolated(RelativeLineicPosition);
                    }

                    // Generate Points
                    this->GenerateExtrusionShape(Buffer,PathIndex,ShapeIndex,RelativeLineicPosition,CurrPoint,PointRotation,PointRotation,PointScale,1.0,1.0,PointIndex < PathSegEnd);
                }
            }

            void Extruder::GenerateExtrusionIntersection(TriangleBuffer& Buffer, const MultiPath::PathIntersection& Intersection, const Whole ShapeIndex) const
            {
                Quaternion FirstOrientation(this->PathsToExtrude.GetPath(Intersection[0].PathIndex).GetDirectionBefore(Intersection[0].PointIndex),Vector3::Unit_Y());
                Vector3 RefX = FirstOrientation * Vector3::Unit_X();
                Vector3 RefZ = FirstOrientation * Vector3::Unit_Z();

                Point2DContainer Points2D;
                MultiPath::PathIntersection Coords;
                std::vector<Real> Directions;

                for( Whole CoordIndex = 0 ; CoordIndex < Intersection.size() ; ++CoordIndex )
                {
                    const Path& IntersectionPath = this->PathsToExtrude.GetPath(Intersection[CoordIndex].PathIndex);
                    Whole PointIndex = Intersection[CoordIndex].PointIndex;
                    if( PointIndex > 0 || IntersectionPath.IsClosed() ) {
                        Vector3 VecB = IntersectionPath.GetDirectionBefore(PointIndex);
                        Vector2 VecB2D = Vector2(VecB.DotProduct(RefX), VecB.DotProduct(RefZ));
                        Points2D.push_back(VecB2D);
                        Coords.push_back(Intersection[CoordIndex]);
                        Directions.push_back(1);
                    }
                    if( PointIndex < static_cast<Whole>( IntersectionPath.GetSegCount() ) || IntersectionPath.IsClosed() ) {
                        Vector3 VecA = -IntersectionPath.GetDirectionAfter(PointIndex);
                        Vector2 VecA2D = Vector2(VecA.DotProduct(RefX), VecA.DotProduct(RefZ));
                        Points2D.push_back(VecA2D);
                        Coords.push_back(Intersection[CoordIndex]);
                        Directions.push_back(-1);
                    }
                }

                std::map<Real,Whole> Angles;
                for( Whole Point2DIndex = 1 ; Point2DIndex < Points2D.size() ; ++Point2DIndex )
                {
                    Angles[ Points2D[0].AngleTo( Points2D[Point2DIndex] ) ] = Point2DIndex;
                }

                std::vector<size_t> OrderedIndices;
                OrderedIndices.push_back(0);
                for( std::map<Real,Whole>::iterator IndiciesIt = Angles.begin() ; IndiciesIt != Angles.end() ; ++IndiciesIt )
                {
                    OrderedIndices.push_back(IndiciesIt->second);
                }

                for( size_t OrderedIndex = 0 ; OrderedIndex < OrderedIndices.size() ; ++OrderedIndex )
                {
                    size_t CoordIndex = OrderedIndices[OrderedIndex];
                    size_t CoordIndexBefore = OrderedIndices[ MathTools::WrappedModulo( OrderedIndex - 1, OrderedIndices.size() ) ];
                    size_t CoordIndexAfter = OrderedIndices[ MathTools::WrappedModulo( OrderedIndex + 1, OrderedIndices.size() ) ];
                    Real AngleBefore = ( Points2D[CoordIndex].AngleBetween( Points2D[CoordIndexBefore] ) - MathTools::GetPi() ) / 2;
                    Real AngleAfter = ( MathTools::GetPi() - ( Points2D[CoordIndex].AngleBetween( Points2D[CoordIndexAfter] ) ) ) / 2;

                    Integer PointIndex = Coords[CoordIndex].PointIndex - Directions[CoordIndex];
                    const Path& CurrPath = this->PathsToExtrude.GetPath(Coords[CoordIndex].PathIndex);

                    Quaternion PointRotation(CurrPath.GetAvgDirection(PointIndex) * Directions[CoordIndex],Vector3::Unit_Y());
                    Real RelPathPos = CurrPath.GetLengthAtPoint(PointIndex) / CurrPath.GetTotalLength();

                    // Shape making the joint with "standard extrusion"
                    this->GenerateExtrusionShape(Buffer,Coords[CoordIndex].PathIndex,ShapeIndex,RelPathPos,CurrPath.GetPoint(PointIndex),PointRotation,PointRotation,1.0,1.0,1.0,true);

                    // Modified shape at the intersection
                    PointRotation = ( Directions[CoordIndex] > 0 ? Quaternion(CurrPath.GetDirectionBefore(Coords[CoordIndex].PointIndex),Vector3::Unit_Y()) : Quaternion(-CurrPath.GetDirectionAfter(Coords[CoordIndex].PointIndex),Vector3::Unit_Y()) );
                    Quaternion RotationLeft = PointRotation * Quaternion(AngleBefore,Vector3::Unit_Y());
                    Quaternion RotationRight = PointRotation * Quaternion(AngleAfter,Vector3::Unit_Y());
                    Real ScaleLeft = 1.0 / MathTools::Abs( MathTools::Cos(AngleBefore) );
                    Real ScaleRight = 1.0 / MathTools::Abs( MathTools::Cos(AngleAfter) );

                    RelPathPos = CurrPath.GetLengthAtPoint(Coords[CoordIndex].PointIndex) / CurrPath.GetTotalLength();
                    this->GenerateExtrusionShape(Buffer,Coords[CoordIndex].PathIndex,ShapeIndex,RelPathPos,CurrPath.GetPoint(Coords[CoordIndex].PointIndex),RotationLeft,RotationRight,1.0,ScaleLeft,ScaleRight,false);
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void Extruder::AddToTriangleBuffer(TriangleBuffer& Buffer) const
            {
                if( this->ShapesToExtrude.GetNumShapes() == 0 ) {
                    MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"At least one shape must be defined to perform an Extrude.");
                }

                TriangleBuffer ExtrudeBuffer;

                if( this->Capped && this->ShapesToExtrude.IsClosed() ) {
                    this->GenerateExtrusionCaps(ExtrudeBuffer);
                }

                for( Whole PathIndex = 0 ; PathIndex < this->ShapesToExtrude.GetNumShapes() ; ++PathIndex )
                {
                    MultiPath::PathSegmentContainer Segments = this->PathsToExtrude.GetNoIntersectionParts(PathIndex);
                    for( MultiPath::PathSegmentContainer::iterator SegIt = Segments.begin() ; SegIt != Segments.end() ; ++SegIt )
                    {
                        for( Whole ShapeIndex = 0 ; ShapeIndex < this->ShapesToExtrude.GetNumShapes() ; ++ShapeIndex )
                        {
                            this->GenerateExtrusionSegment(ExtrudeBuffer,ShapeIndex,PathIndex,(*SegIt).first,(*SegIt).second);
                        }
                    }

                    const MultiPath::IntersectionVector& Intersections = this->PathsToExtrude.GetIntersections();
                    for( MultiPath::IntersectionVector::const_iterator IntersectIt = Intersections.begin() ; IntersectIt != Intersections.end() ; ++IntersectIt )
                    {
                        for( Whole ShapeIndex = 0 ; ShapeIndex < this->ShapesToExtrude.GetNumShapes() ; ++ShapeIndex )
                        {
                            this->GenerateExtrusionIntersection(ExtrudeBuffer,(*IntersectIt),ShapeIndex);
                        }
                    }
                }

                VertexTransformModifier().SetTranslation(this->Position).SetRotation(this->Orientation).SetScaling(this->Scale).Modify(ExtrudeBuffer);
                VertexUVModifier().SetUVTranslate(this->UVOrigin).SetUVTile(this->UTile,this->VTile).SetSwitchUV(this->GeneratorOpts | GO_SwitchUV).Modify(ExtrudeBuffer);
                Buffer.AppendBuffer(ExtrudeBuffer);
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            Extruder& Extruder::SetPathToExtrude(const Path& ToExtrude)
            {
                this->PathsToExtrude.RemoveAllPaths();
                this->PathsToExtrude.AddPath(ToExtrude);
                this->PathsToExtrude.CalculateIntersections();
                return *this;
            }

            Extruder& Extruder::SetMultiPathToExtrude(const MultiPath& ToExtrude)
            {
                this->PathsToExtrude.RemoveAllPaths();
                this->PathsToExtrude.AddMultiPath(ToExtrude);
                this->PathsToExtrude.CalculateIntersections();
                return *this;
            }

            Extruder& Extruder::SetShapeToExtrude(const Shape& ToExtrude)
            {
                this->ShapesToExtrude.RemoveAllShapes();
                this->ShapesToExtrude.AddShape(ToExtrude);
                return *this;
            }

            Extruder& Extruder::SetMultiShapeToExtrude(const MultiShape& ToExtrude)
            {
                this->ShapesToExtrude.RemoveAllShapes();
                this->ShapesToExtrude.AddMultiShape(ToExtrude);
                return *this;
            }

            Extruder& Extruder::SetScaleTrack(const Whole PathIndex, ParameterTrack* Scale)
            {
                if( Scale != NULL ) {
                    this->ScaleTracks[PathIndex] = Scale;
                }
                return *this;
            }

            Extruder& Extruder::SetRotationTrack(const Whole PathIndex, ParameterTrack* Rotation)
            {
                if( Rotation != NULL ) {
                    this->RotationTracks[PathIndex] = Rotation;
                }
                return *this;
            }

            Extruder& Extruder::SetPathUTextureTrack(const Whole PathIndex, ParameterTrack* PathU)
            {
                if( PathU != NULL ) {
                    this->PathUTextureTracks[PathIndex] = PathU;
                }
                return *this;
            }

            Extruder& Extruder::SetShapeVTextureTrack(const Whole ShapeIndex, ParameterTrack* ShapeV)
            {

                if( ShapeV != NULL ) {
                    this->ShapeVTextureTracks[ShapeIndex] = ShapeV;
                }
                return *this;
            }

            Extruder& Extruder::SetCapped(const Boole Cap)
            {
                this->Capped = Cap;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif

