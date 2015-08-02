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
#ifndef _graphicsproceduralshape_cpp
#define _graphicsproceduralshape_cpp

#include "Graphics/Procedural/shape.h"
#include "Graphics/Procedural/multishape.h"
#include "Graphics/Procedural/path.h"

#include "Graphics/meshmanager.h"

#include "MathTools/mathtools.h"
#include "exception.h"

#include "Ogre.h"

namespace
{
    /// @brief Sorter method used to sort directions generated during a boolean operation.
    /// @param One The first pair to compare.
    /// @param Two The second pair to compare.
    /// @return Returns true if the first pair should be ordered before the second pair, false otherwise.
    Mezzanine::Boole _SortAngles(std::pair<Mezzanine::Real,Mezzanine::UInt8> One, std::pair<Mezzanine::Real,Mezzanine::UInt8> Two)
    {
        return ( One.first < Two.first );
    }
}

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief Convenience class storing data on the point in a 2D shape where multiple segments intersect.
            /// @details
            ///////////////////////////////////////
            struct IntersectionInShape
            {
                /// @brief An Array storing indexes to points that are the first vertices of segments that intersect for each of the two shapes being processed.
                Whole Index[2];
                /// @brief An Array storing whether the intersection is on or very near a vertex belonging to either shape being processed.
                Boole OnVertex[2];
                /// @brief The position of the intersection.
                Vector2 Position;

                /// @brief Class constructor.
                /// @param Index0 The index of the first point.
                /// @param Index1 The index of the second point.
                /// @param Intersect The position of the intersection.
                IntersectionInShape(const Whole Index0, const Whole Index1, Vector2 Intersect) :
                    Position(Intersect)
                {
                    this->Index[0] = Index0;
                    this->Index[1] = Index1;
                    this->OnVertex[0] = false;
                    this->OnVertex[1] = false;
                }
            };//IntersectionInShape

            ///////////////////////////////////////////////////////////////////////////////
            // Shape Methods

            Shape::Shape() :
                OutSide(Procedural::SS_Right),
                Closed(false)
                {  }

            Shape::~Shape()
                {  }

            MultiShape Shape::_BooleanOperation(const Shape& Other, const Procedural::BooleanOperation OpType) const
            {
                if( !this->Closed || this->Points.size() < 2 ) {
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"2D shapes must be closed and must contain at least 2 points.");
                }
                if( !Other.Closed || Other.Points.size() < 2 ) {
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"2D shapes must be closed and must contain at least 2 points.");
                }

                // Compute the intersection between the 2 shapes
                IntersectionContainer Intersections;
                this->_FindAllIntersections(Other, Intersections);

                // Build the resulting shape
                if( Intersections.empty() ) {
                    if( this->IsPointInside( Other.GetPoint(0) ) ) {
                        // Shape B is completely inside shape A
                        if( OpType == Procedural::BO_Union ) {
                            return *this;
                        }else if( OpType == Procedural::BO_Intersection ) {
                            return Other;
                        }else if( OpType == Procedural::BO_Difference ) {
                            MultiShape ms;
                            ms.AddShape(*this);
                            ms.AddShape(Other);
                            ms.GetShape(1).SwitchSide();
                            return ms;
                        }
                    }else if( Other.IsPointInside( this->GetPoint(0) ) ) {
                        // Shape A is completely inside shape B
                        if( OpType == Procedural::BO_Union ) {
                            return Other;
                        }else if( OpType == Procedural::BO_Intersection ) {
                            return *this;
                        }else if( OpType == Procedural::BO_Difference ) {
                            MultiShape ms;
                            ms.AddShape(*this);
                            ms.AddShape(Other);
                            ms.GetShape(0).SwitchSide();
                            return ms;
                        }
                    }else{
                        if( OpType == Procedural::BO_Union ) {
                            MultiShape ms;
                            ms.AddShape( *this );
                            ms.AddShape( Other );
                            return ms;
                        }else if( OpType == Procedural::BO_Intersection ) {
                            return Shape();//empty result
                        }else if( OpType == Procedural::BO_Difference ) {
                            return Shape();//empty result
                        }
                    }
                }
                MultiShape outputMultiShape;

                const Shape* InputShapes[2];
                InputShapes[0] = this;
                InputShapes[1] = &Other;

                while( !Intersections.empty() )
                {
                    Shape outputShape;
                    UInt8 shapeSelector = 0; // 0 : first shape, 1 : second shape

                    Vector2 currentPosition = Intersections.begin()->Position;
                    IntersectionInShape firstIntersection = *Intersections.begin();
                    Whole currentSegment = firstIntersection.Index[ shapeSelector ];
                    Intersections.erase( Intersections.begin() );
                    outputShape.AddPoint( currentPosition );

                    Char8 IsIncreasing = 0;// +1 if increasing, -1 if decreasing, 0 if undefined

                    if( !this->_FindWhereToGo(InputShapes, OpType, firstIntersection, shapeSelector, IsIncreasing, currentSegment) ) {
                        // That intersection is located on a place where the resulting shape won't go => discard
                        continue;
                    }

                    while( true )
                    {
                        // find the closest intersection on the same segment, in the correct direction
                        IntersectionIterator found_next_intersection = Intersections.end();
                        Real distanceToNextIntersection = std::numeric_limits<Real>::max();

                        Whole nextPoint = currentSegment + ( IsIncreasing == 1 ? 1 : 0 );
                        Boole nextPointIsOnIntersection = false;

                        for( IntersectionIterator it = Intersections.begin() ; it != Intersections.end() ; ++it )
                        {
                            if( currentSegment == it->Index[shapeSelector] ) {
                                if( ( ( it->Position - currentPosition ).DotProduct( it->Position - InputShapes[shapeSelector]->GetPoint(nextPoint) ) < 0 ) ||
                                    ( it->OnVertex[shapeSelector] && nextPoint == it->Index[shapeSelector] ) )
                                {
                                    // found an intersection between the current one and the next segment point
                                    Real d = ( it->Position - currentPosition ).Length();
                                    if( d < distanceToNextIntersection ) {
                                        // check if we have the nearest intersection
                                        found_next_intersection = it;
                                        distanceToNextIntersection = d;
                                    }
                                }
                            }
                            if( nextPoint == it->Index[shapeSelector] && it->OnVertex[shapeSelector] )
                                nextPointIsOnIntersection = true;
                        }

                        // stop condition
                        if( currentSegment == firstIntersection.Index[shapeSelector] ) {
                            // we found ourselves on the same segment as the first intersection and no other
                            if( ( firstIntersection.Position - currentPosition ).DotProduct( firstIntersection.Position - InputShapes[shapeSelector]->GetPoint(nextPoint) ) < 0 ) {
                                Real d = ( firstIntersection.Position - currentPosition ).Length();
                                if( d > 0. && d < distanceToNextIntersection ) {
                                    outputShape.Close();
                                    break;
                                }
                            }
                        }

                        // We actually found the next intersection => change direction and add current intersection to the list
                        if( found_next_intersection != Intersections.end() ) {
                            IntersectionInShape currentIntersection = *found_next_intersection;
                            Intersections.erase(found_next_intersection);
                            outputShape.AddPoint(currentIntersection.Position);
                            Boole result = this->_FindWhereToGo(InputShapes, OpType, currentIntersection, shapeSelector, IsIncreasing, currentSegment);
                            if( !result ) {
                                OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "We should not be here!", "Procedural::Shape::_booleanOperation(const Procedural::Shape&, Procedural::BooleanOperationType)");
                            }
                        }else{
                            // no intersection found for the moment => just continue on the current segment
                            if( !nextPointIsOnIntersection ) {
                                if( IsIncreasing == 1 ) {
                                    currentPosition = InputShapes[shapeSelector]->GetPoint( currentSegment + 1 );
                                }else{
                                    currentPosition = InputShapes[shapeSelector]->GetPoint( currentSegment );
                                }

                                outputShape.AddPoint(currentPosition);
                            }
                            currentSegment = MathTools::WrappedModulo( currentSegment + IsIncreasing,InputShapes[shapeSelector]->GetSegCount() );
                        }
                    }

                    outputMultiShape.AddShape(outputShape);
                }
                return outputMultiShape;
            }

            Boole Shape::_IsLookingForOutside(const Procedural::BooleanOperation OpType, const Char8 ShapeSelector) const
            {
                switch( OpType )
                {
                    case Procedural::BO_Union:          return true;
                    case Procedural::BO_Intersection:   return false;
                    case Procedural::BO_Difference:     return ( ShapeSelector == 0 );
                    default:                            return true;
                }
            }

            Char8 Shape::_IsIncreasing(const Real Dot, const Procedural::BooleanOperation OpType, const Char8 ShapeSelector) const
            {
                if( Dot < 0 && OpType == Procedural::BO_Union )
                    return -1;
                if( Dot > 0 && OpType == Procedural::BO_Intersection )
                    return -1;
                if( OpType == Procedural::BO_Difference ) {
                    if( ( Dot < 0 && ShapeSelector == 0) || ( Dot > 0 && ShapeSelector == 1 ) )
                        return -1;
                }
                return 1;
            }

            Boole Shape::_FindWhereToGo(const Shape* InputShapes[], const Procedural::BooleanOperation OpType, const IntersectionInShape& Intersection, UInt8& ShapeSelector, Char8& IsIncreasing, Whole& CurrentSegment) const
            {
                if( Intersection.OnVertex[0] || Intersection.OnVertex[1] ) {
                    // determine 4 directions with normal info
                    // if 2 normals "face each other" then you have the couple of outside directions
                    Vector2 Directions[4];
                    Char8 Sides[4];
                    UInt8 IncomingDirection;

                    // fill-in the incoming arrays
                    if( IsIncreasing == 0 ) {
                        IncomingDirection = 255;
                    }else{
                        IncomingDirection = ShapeSelector + ( IsIncreasing == 1 ? 2 : 0 );
                    }
                    for( UInt8 i = 0 ; i < 2 ; i++ )
                    {
                        if( Intersection.OnVertex[i] ) {
                            Directions[ i ] = InputShapes[i]->GetDirectionBefore( Intersection.Index[i] );
                            Directions[ 2 + i ] = -InputShapes[i]->GetDirectionAfter( Intersection.Index[i] );
                        }else{
                            Directions[ 2 + i ] = -InputShapes[i]->GetDirectionAfter( Intersection.Index[i] );
                            Directions[ i ] = -Directions[ 2 + i ];
                        }
                    }
                    for( UInt8 i = 0 ; i < 4 ; i++ )
                    {
                        Sides[i] = ( i / 2 == 0 ? -1 : 1 ) * ( InputShapes[ i % 2 ]->OutSide == Procedural::SS_Right ? -1 : 1 );
                    }

                    Boole IsOutside[4];
                    std::pair<Real,UInt8> SortedDirections[4];

                    // sort by angle
                    for( Integer i = 0 ; i < 4 ; i++ )
                    {
                        if( i == 0 ) {
                            SortedDirections[i].first = 0;
                        }else{
                            SortedDirections[i].first = Sides[0] * Directions[0].AngleTo( Directions[i] );
                        }
                        SortedDirections[i].second = i;
                    }

                    std::sort(SortedDirections, SortedDirections + 4, _SortAngles);

                    //find which segments are outside
                    if( Sides[0] != Sides[ SortedDirections[1].second ] ) {
                        IsOutside[0] = IsOutside[ SortedDirections[1].second ] = true;
                        IsOutside[ SortedDirections[2].second ] = IsOutside[ SortedDirections[3].second ] = false;
                    }else{
                        IsOutside[ SortedDirections[1].second ] = IsOutside[ SortedDirections[2].second ] = true;
                        IsOutside[ SortedDirections[3].second ] = IsOutside[ SortedDirections[0].second ] = false;
                    }

                    //find first eligible segment that is not the current segment
                    for( unsigned short i = 0 ; i < 4 ; i++ )
                    {
                        if( ( IsOutside[i] == this->_IsLookingForOutside( OpType, i % 2 ) ) && ( i != IncomingDirection ) ) {
                            ShapeSelector = i % 2;
                            IsIncreasing = i / 2 == 0 ? 1 : -1;
                            CurrentSegment = Intersection.Index[ ShapeSelector ];
                            return true;
                        }
                    }
                    // if we reach here, it means that no segment is eligible! (it should only happen with difference opereation
                    return false;
                }else{
                    // determine which way to go
                    Integer NextShapeSelector = ( ShapeSelector + 1 ) % 2;

                    Real Dot = InputShapes[NextShapeSelector]->GetDirectionAfter( Intersection.Index[NextShapeSelector] ).DotProduct( InputShapes[ShapeSelector]->GetNormalAfter(CurrentSegment) );
                    IsIncreasing = this->_IsIncreasing( Dot, OpType, NextShapeSelector );

                    ShapeSelector = NextShapeSelector;

                    CurrentSegment = Intersection.Index[ ShapeSelector ];
                    return true;
                }
            }

            void Shape::_FindAllIntersections(const Shape& Other, IntersectionContainer& Intersections) const
            {
                for( unsigned short SelfSeg = 0 ; SelfSeg < this->GetSegCount() ; ++SelfSeg )
                {
                    LineSegment2D Seg1( this->GetPoint( SelfSeg ), this->GetPoint( SelfSeg + 1 ) );

                    for( unsigned short OtherSeg = 0 ; OtherSeg < Other.GetSegCount() ; ++OtherSeg )
                    {
                        LineSegment2D Seg2( Other.GetPoint( OtherSeg ), Other.GetPoint( OtherSeg + 1 ) );

                        LineSegment2D::IntersectionTestResult Result = Seg1.Intersects(Seg2);
                        if( Result.first ) {
                            IntersectionInShape Inter( SelfSeg, OtherSeg, Result.second );
                            // check if intersection is "borderline" : too near to a vertex
                            if( Seg1.PointA.SquaredDistance( Result.second ) < 1e-8 ) {
                                Inter.OnVertex[0] = true;
                            }
                            if( Seg1.PointB.SquaredDistance( Result.second ) < 1e-8 ) {
                                Inter.OnVertex[0] = true;
                                Inter.Index[0]++;
                            }
                            if( Seg2.PointA.SquaredDistance( Result.second ) < 1e-8 ) {
                                Inter.OnVertex[1] = true;
                            }
                            if( Seg2.PointB.SquaredDistance( Result.second ) < 1e-8 ) {
                                Inter.OnVertex[1] = true;
                                Inter.Index[1]++;
                            }

                            Intersections.push_back( Inter );
                        }
                    }
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            Mesh* Shape::GenerateMesh(const String& Name, const String& Group) const
            {
                Ogre::ManualObject* TempMan = new Ogre::ManualObject("TempMan");
                TempMan->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OT_LINE_STRIP);

                this->_AppendToManualObject(TempMan);

                TempMan->end();
                Mesh* NewMesh = MeshManager::GetSingletonPtr()->_WrapInternalMesh( TempMan->convertToMesh(Name,Group) );
                delete TempMan;
                return NewMesh;
            }

            Shape& Shape::AppendShape(const Shape& Other)
            {
                this->Points.insert(this->Points.end(), Other.Points.begin(), Other.Points.end());
                return *this;
            }

            Shape& Shape::AppendShapeRel(const Shape& Other)
            {
                if( this->Points.empty() ) {
                    this->AppendShape(Other);
                }else{
                    Vector2 refVector = *( this->Points.end() - 1 );
                    Point2DContainer pointList( Other.Points.begin(), Other.Points.end() );
                    for( Point2DIterator it = pointList.begin() ; it != pointList.end() ; ++it )
                        { *it += refVector; }
                    this->Points.insert( this->Points.end(), pointList.begin(), pointList.end() );
                }
                return *this;
            }

            Shape Shape::ExtractSubShape(const Whole First, const Whole Last)
            {
                Shape Ret;
                for( Whole i = First ; i <= Last ; ++i )
                    { Ret.AddPoint( this->Points[i] ); }
                Ret.SetOutSide( this->OutSide );
                if( this->Closed ) {
                    Ret.Close();
                }
                return Ret;
            }

            Whole Shape::GetSegCount() const
                { return ( this->Points.size() - 1 ) + ( this->Closed ? 1 : 0 ); }

            Real Shape::GetTotalLength() const
            {
                Real Length = 0;
                for( Whole Index = 0 ; Index < this->Points.size() - 1 ; ++Index )
                    { Length += ( this->Points[ Index + 1 ] - this->Points[Index] ).Length(); }
                if( this->Closed ) {
                    Length += ( this->Points.back() - *(this->Points.begin()) ).Length();
                }
                return Length;
            }

            Boole Shape::IsPointInside(const Vector2& Point) const
            {
                // Draw a horizontal lines that goes through "point"
                // Using the closest intersection, find whethe the point is actually inside
                Integer ClosestSegmentIndex = -1;
                Real ClosestSegmentDistance = std::numeric_limits<Real>::max();
                Vector2 ClosestSegmentIntersection;

                for( Whole CurrSeg = 0 ; CurrSeg < this->GetSegCount() ; ++CurrSeg )
                {
                    Vector2 A = this->GetPoint( CurrSeg );
                    Vector2 B = this->GetPoint( CurrSeg + 1 );
                    if( A.Y != B.Y && ( A.Y - Point.Y ) * ( B.Y - Point.Y ) <= 0.0 ) {
                        Vector2 Intersect( A.X + ( Point.Y - A.Y ) * ( B.X - A.X ) / ( B.Y - A.Y ), Point.Y );
                        Real Dist = MathTools::Abs( Point.X - Intersect.X );
                        if( Dist < ClosestSegmentDistance ) {
                            ClosestSegmentIndex = CurrSeg;
                            ClosestSegmentDistance = Dist;
                            ClosestSegmentIntersection = Intersect;
                        }
                    }
                }
                if( ClosestSegmentIndex != -1 ) {
                    if( this->GetNormalAfter( ClosestSegmentIndex ).X * ( Point.X - ClosestSegmentIntersection.X ) < 0 ) {
                        return true;
                    }else{
                        return false;
                    }
                }
                if( this->FindRealOutSide() == this->OutSide ) {
                    return false;
                }else{
                    return true;
                }
            }

            Real Shape::FindBoundingRadius() const
            {
                Real sqRadius = 0.0;
                for( Whole Index = 0 ; Index < this->Points.size() ; ++Index )
                    { sqRadius = std::max( sqRadius, this->Points[Index].SquaredLength() ); }
                return MathTools::Sqrt(sqRadius);
            }

            Vector2 Shape::GetPosition(const Whole Index, const Real Coord) const
            {
                if( !this->Closed || Index >= this->Points.size() )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Specified point index is out of bounds.");
                if( Coord < 0.0 || Coord > 1.0 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Coord must be between 0 and 1.");
                Vector2 A = this->GetPoint( Index );
                Vector2 B = this->GetPoint( Index + 1 );
                return A + ( ( B - A ) * Coord );
            }

            Vector2 Shape::GetPosition(Real Coord) const
            {
                if( this->Points.size() < 2 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION, "The shape must contain at least 2 points.");
                Whole Index = 0;
                while( true )
                {
                    Real NextLen = ( this->GetPoint( Index + 1 ) - this->GetPoint( Index ) ).Length();
                    if( Coord > NextLen ) {
                        Coord -= NextLen;
                    }else{
                        return this->GetPosition(Index,Coord);
                    }
                    if( !this->Closed && Index >= this->Points.size() - 2 ) {
                        return this->Points.back();
                    }
                    Index++;
                }
            }

            Shape& Shape::Mirror(Boole Flip)
            {
                return this->MirrorAroundPoint(Vector2(0,0),Flip);
            }

            Shape& Shape::MirrorAroundPoint(const Real X, const Real Y, Boole Flip)
            {
                return this->MirrorAroundPoint(Vector2(X,Y),Flip);
            }

            Shape& Shape::MirrorAroundPoint(const Vector2& Point, Boole Flip)
            {
                Integer NumPoints = (Integer)this->Points.size();
                if( Flip ) {
                    for( Integer CurrPoint = NumPoints - 1 ; CurrPoint >= 0 ; CurrPoint-- )
                    {
                        Vector2 Pos = this->Points.at(CurrPoint) - Point;
                        this->Points.push_back( ( Pos * -1.0 ) + Point );
                    }
                }else{
                    for( Integer CurrPoint = 0 ; CurrPoint < NumPoints ; CurrPoint++ )
                    {
                        Vector2 Pos = this->Points.at(CurrPoint) - Point;
                        this->Points.push_back( ( Pos * -1.0 ) + Point );
                    }
                }
                return *this;
            }

            Shape& Shape::MirrorAroundAxis(const Vector2& Axis, Boole Flip)
            {
                Integer NumPoints = (Integer)this->Points.size();
                Vector2 Normal = Axis.Perpendicular().GetNormal();
                if( Flip ) {
                    for( Integer CurrPoint = 0 ; CurrPoint < NumPoints ; CurrPoint++ )
                    {
                        Vector2 Pos = this->Points.at(CurrPoint);
                        Pos = Pos.Reflect(Normal);
                        if( Pos != this->Points.at(CurrPoint) ) {
                            this->Points.push_back(Pos);
                        }
                    }
                }else{
                    for( Integer CurrPoint = NumPoints - 1 ; CurrPoint >= 0 ; CurrPoint-- )
                    {
                        Vector2 Pos = this->Points.at(CurrPoint);
                        Pos = Pos.Reflect(Normal);
                        if( Pos != this->Points.at(CurrPoint) ) {
                            this->Points.push_back(Pos);
                        }
                    }
                }
                return *this;
            }

            Shape& Shape::Reflect(const Vector2& Normal)
            {
                for( Point2DIterator it = this->Points.begin() ; it != this->Points.end() ; ++it )
                    { *it = it->Reflect(Normal); }
                return *this;
            }

            Shape& Shape::Close()
            {
                this->Closed = true;
                return *this;
            }

            Boole Shape::IsClosed() const
            {
                return this->Closed;
            }

            Shape& Shape::Reverse()
            {
                std::reverse(this->Points.begin(), this->Points.end());
                this->SwitchSide();
                return *this;
            }

            Shape& Shape::Reset()
            {
                this->Points.clear();
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Point Management

            Shape& Shape::AddPoint(const Vector2& Point)
            {
                this->Points.push_back(Point);
                return *this;
            }

            Shape& Shape::AddPoint(const Real X, const Real Y)
            {
                this->Points.push_back(Vector2(X,Y));
                return *this;
            }

            Shape& Shape::AddPointRel(const Vector2& Point)
            {
                if( this->Points.empty() ) {
                    this->Points.push_back( Point );
                }else{
                    this->Points.push_back( Point + *( this->Points.end() - 1 ) );
                }
                return *this;
            }

            Shape& Shape::AddPointRel(const Real X, const Real Y)
            {
                if( this->Points.empty() ) {
                    this->Points.push_back( Vector2(X,Y) );
                }else{
                    this->Points.push_back( Vector2(X,Y) + *( this->Points.end() - 1 ) );
                }
                return *this;
            }

            Shape& Shape::InsertPoint(const Whole Index, const Real X, const Real Y)
            {
                this->Points.insert( this->Points.begin() + Index, Vector2(X,Y) );
                return *this;
            }

            Shape& Shape::InsertPoint(const Whole Index, const Vector2& Point)
            {
                this->Points.insert( this->Points.begin() + Index, Point );
                return *this;
            }

            const Vector2& Shape::GetPoint(const Integer Index) const
            {
                return this->Points[ this->GetBoundedIndex(Index) ];
            }

            Whole Shape::GetPointCount() const
            {
                return this->Points.size();
            }

            Whole Shape::GetBoundedIndex(const Integer Index) const
            {
                if( this->Closed ) {
                    return MathTools::WrappedModulo( Index, this->Points.size() );
                }
                return MathTools::Clamp( Index, Integer(0), Integer(this->Points.size() - 1) );
            }

            Point2DContainer Shape::GetPoints() const
                { return this->Points; }

            Point2DContainer& Shape::GetPointsReference()
                { return this->Points; }

            const Point2DContainer& Shape::GetPointsReference() const
                { return this->Points; }

            ///////////////////////////////////////////////////////////////////////////////
            // Transform

            Shape& Shape::Translate(const Vector2& Trans)
            {
                for( Point2DIterator it = this->Points.begin() ; it != this->Points.end() ; ++it )
                    *it += Trans;
                return *this;
            }

            Shape& Shape::Translate(const Real TransX, const Real TransY)
            {
                return this->Translate( Vector2(TransX,TransY) );
            }

            Shape& Shape::Rotate(const Real Angle)
            {
                Real c = MathTools::Cos( Angle );
                Real s = MathTools::Sin( Angle );
                for( Point2DIterator it = this->Points.begin() ; it != this->Points.end() ; ++it )
                {
                    Real x = it->X;
                    Real y = it->Y;
                    it->X = c * x - s * y;
                    it->Y = s * x + c * y;
                }
                return *this;
            }

            Shape& Shape::Scale(const Real Scaling)
            {
                return this->Scale(Scaling,Scaling);
            }

            Shape& Shape::Scale(const Real ScaleX, const Real ScaleY)
            {
                for( Point2DIterator it = this->Points.begin() ; it != this->Points.end() ; ++it )
                {
                    it->X *= ScaleX;
                    it->Y *= ScaleY;
                }
                return *this;
            }

            Shape& Shape::Scale(const Vector2& Scaling)
            {
                return this->Scale(Scaling.X,Scaling.Y);
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Directions and Normals

            Vector2 Shape::GetDirectionAfter(const Whole Index) const
            {
                // If the path isn't closed, we get a different calculation at the end, because
                // the tangent shall not be null
                if( !this->Closed && Index == this->Points.size() - 1 && Index > 0) {
                    return ( this->Points[Index] - this->Points[ Index - 1 ] ).GetNormal();
                }else{
                    return ( this->GetPoint( Index + 1 ) - this->GetPoint(Index) ).GetNormal();
                }
            }

            Vector2 Shape::GetDirectionBefore(const Whole Index) const
            {
                // If the path isn't closed, we get a different calculation at the end, because
                // the tangent shall not be null
                if( !this->Closed && Index == 1) {
                    return ( this->Points[1] - this->Points[0] ).GetNormal();
                }else{
                    return ( this->GetPoint(Index) - this->GetPoint( Index - 1 ) ).GetNormal();
                }
            }

            Vector2 Shape::GetAvgDirection(const Whole Index) const
            {
                return ( this->GetDirectionAfter(Index) + this->GetDirectionBefore(Index) ).GetNormal();
            }

            Vector2 Shape::GetNormalAfter(const Whole Index) const
            {
                if( this->OutSide == Procedural::SS_Right ) {
                    return -( this->GetDirectionAfter(Index).Perpendicular() );
                }
                return this->GetDirectionAfter(Index).Perpendicular();
            }

            Vector2 Shape::GetNormalBefore(const Whole Index) const
            {
                if( this->OutSide == Procedural::SS_Right ) {
                    return -( this->GetDirectionBefore(Index).Perpendicular() );
                }
                return this->GetDirectionBefore(Index).Perpendicular();
            }

            Vector2 Shape::GetAvgNormal(const Whole Index) const
            {
                if( this->OutSide == Procedural::SS_Right ) {
                    return -( this->GetAvgDirection(Index).Perpendicular() );
                }
                return this->GetAvgDirection(Index).Perpendicular();
            }

            ///////////////////////////////////////////////////////////////////////////////
            // OutSide

            Shape& Shape::SetOutSide(const Procedural::ShapeSide Side)
            {
                this->OutSide = Side;
                return *this;
            }

            Procedural::ShapeSide Shape::GetOutSide() const
            {
                return this->OutSide;
            }

            Shape& Shape::SwitchSide()
            {
                this->OutSide = ( this->OutSide == Procedural::SS_Left ) ? Procedural::SS_Right : Procedural::SS_Left;
                return *this;
            }

            Procedural::ShapeSide Shape::FindRealOutSide() const
            {
                Real X = this->Points[0].X;
                Integer Index = 0;
                for( unsigned short CurrPoint = 1 ; CurrPoint < this->Points.size() ; ++CurrPoint )
                {
                    if( X < this->Points[CurrPoint].X ) {
                        X = this->Points[CurrPoint].X;
                        Index = CurrPoint;
                    }
                }
                Real Alpha1 = Vector2::Unit_Y().AngleTo( this->GetDirectionAfter(Index) );
                Real Alpha2 = Vector2::Unit_Y().AngleTo( -( this->GetDirectionBefore(Index) ) );
                if( Alpha1 < Alpha2 ) {
                    return Procedural::SS_Right;
                }else{
                    return Procedural::SS_Left;
                }
            }

            Boole Shape::IsOutsideRealOutside() const
            {
                return ( this->FindRealOutSide() == this->OutSide );
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Conversion

            Path Shape::ConvertToPath() const
            {
                Path RetPath;
                ConstPoint2DIterator BeginIt = this->Points.begin();
                for( ConstPoint2DIterator PointIt = BeginIt ; PointIt != this->Points.end() ; ++PointIt )
                    { RetPath.AddPoint( Vector3( (*PointIt).X, 0, (*PointIt).Y ) ); }
                if( this->Closed && this->Points.size() > 2 ) {
                    RetPath.AddPoint( Vector3( (*BeginIt).X, 0, (*BeginIt).Y ) );
                }
                return RetPath;
            }

            MultiShape Shape::Thicken(const Real Amount)
            {
                if( !this->Closed ) {
                    Shape TempShape;
                    TempShape.SetOutSide( this->OutSide );
                    for( Whole Index = 0 ; Index < this->Points.size() ; Index++ )
                        { TempShape.AddPoint( this->Points[Index] + ( this->GetAvgNormal(Index) * Amount ) ); }
                    for( Integer Index = this->Points.size() - 1 ; Index >= 0 ; Index-- )
                        { TempShape.AddPoint( this->Points[Index] - ( this->GetAvgNormal(Index) * Amount ) ); }
                    TempShape.Close();
                    return MultiShape().AddShape(TempShape);
                }else{
                    MultiShape RetShape;
                    Shape TempShape1;
                    for( Whole Index = 0 ; Index < this->Points.size() ; Index++ )
                        { TempShape1.AddPoint( this->Points[Index] + ( this->GetAvgNormal(Index) * Amount ) ); }
                    TempShape1.Close();
                    TempShape1.SetOutSide( this->OutSide );
                    RetShape.AddShape(TempShape1);
                    Shape TempShape2;
                    for( Whole Index = 0 ; Index < this->Points.size() ; Index++ )
                        { TempShape2.AddPoint( this->Points[Index] - ( this->GetAvgNormal(Index) * Amount ) ); }
                    TempShape2.Close();
                    TempShape2.SetOutSide( this->OutSide == Procedural::SS_Left ? Procedural::SS_Right : Procedural::SS_Left );
                    RetShape.AddShape(TempShape2);
                    return RetShape;
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Boolean Operations

            MultiShape Shape::BooleanIntersect(const Shape& Other) const
                { return this->_BooleanOperation(Other,Procedural::BO_Union); }

            MultiShape Shape::BooleanUnion(const Shape& Other) const
                { return this->_BooleanOperation(Other,Procedural::BO_Intersection); }

            MultiShape Shape::BooleanDifference(const Shape& Other) const
                { return this->_BooleanOperation(Other,Procedural::BO_Difference); }

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            void Shape::_AppendToManualObject(Ogre::ManualObject* Object) const
            {

            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
