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
#ifndef _graphicsproceduraltriangulator_cpp
#define _graphicsproceduraltriangulator_cpp

#include "Graphics/Procedural/triangulator.h"
#include "Graphics/Procedural/shape.h"
#include "Graphics/Procedural/multishape.h"

#include "MathTools/mathtools.h"
#include "circle.h"
#include "stringtool.h"
#include "exception.h"
#include "matrix4x4.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            // DelaunaySegment Methods

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            DelaunaySegment::DelaunaySegment(const Integer First, const Integer Second) :
                Index1(First),
                Index2(Second)
                {  }

            DelaunaySegment::~DelaunaySegment()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            DelaunaySegment DelaunaySegment::Inverse() const
                { return DelaunaySegment(this->Index2,this->Index1); }

            Boole DelaunaySegment::operator<(const DelaunaySegment& Other) const
            {
                if( this->Index1 != Other.Index1 ) {
                    return ( this->Index1 < Other.Index1 );
                }else{
                    return ( this->Index2 < Other.Index2 );
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // DelaunayTriangle Methods

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            DelaunayTriangle::DelaunayTriangle(const Point2DContainer* List) :
                Points(List)
                {  }

            DelaunayTriangle::~DelaunayTriangle()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void DelaunayTriangle::SetVertices(const Integer Index0, const Integer Index1, const Integer Index2)
            {
                this->Indexes[0] = Index0;
                this->Indexes[1] = Index1;
                this->Indexes[2] = Index2;
            }

            void DelaunayTriangle::MakeDirectIfNeeded()
            {
                if( ( this->GetPoint(1) - this->GetPoint(0) ).CrossProduct( this->GetPoint(2) - this->GetPoint(0) ) < 0 ) {
                    std::swap( this->Indexes[0], this->Indexes[1] );
                }
            }

            Vector2 DelaunayTriangle::GetPoint(const Integer Point) const
            {
                return (*Points)[ this->Indexes[ Point ] ];
            }

            Vector2 DelaunayTriangle::GetMidPoint() const
            {
                return ( ( this->GetPoint(0) + this->GetPoint(1) + this->GetPoint(2) ) * 1.f / 3.f );
            }

            Integer DelaunayTriangle::FindSegNumber(const Integer Index0, const Integer Index1) const
            {
                if( ( Index0 == this->Indexes[0] && Index1 == this->Indexes[1] ) || ( Index0 == this->Indexes[1] && Index1 == this->Indexes[0] ) )
                    return 2;
                if( ( Index0 == this->Indexes[1] && Index1 == this->Indexes[2] ) || ( Index0 == this->Indexes[2] && Index1 == this->Indexes[1] ) )
                    return 0;
                if( ( Index0 == this->Indexes[2] && Index1 == this->Indexes[0] ) || ( Index0 == this->Indexes[0] && Index1 == this->Indexes[2] ) )
                    return 1;
                MEZZ_EXCEPTION(ExceptionBase::INTERNAL_EXCEPTION,"Specified segment was not found.  If the algorithm is working properly, you should never see this message.");
                return -1;
            }

            Boole DelaunayTriangle::ContainsSegment(const Integer Index0, const Integer Index1) const
            {
                return ( ( Index0 == this->Indexes[0] || Index0 == this->Indexes[1] || Index0 == this->Indexes[2] ) &&
                         ( Index1 == this->Indexes[0] || Index1 == this->Indexes[1] || Index1 == this->Indexes[2] ) );
            }

            Boole DelaunayTriangle::IsPointInside(const Vector2& Point) const
            {
                // Compute vectors
                Vector2 v0 = this->GetPoint(2) - this->GetPoint(0);
                Vector2 v1 = this->GetPoint(1) - this->GetPoint(0);
                Vector2 v2 = Point - this->GetPoint(0);

                // Compute dot products
                Real dot00 = v0.SquaredLength();
                Real dot01 = v0.DotProduct(v1);
                Real dot02 = v0.DotProduct(v2);
                Real dot11 = v1.SquaredLength();
                Real dot12 = v1.DotProduct(v2);

                // Compute barycentric coordinates
                Real invDenom = 1.0 / ( dot00 * dot11 - dot01 * dot01 );
                Real u = ( dot11 * dot02 - dot01 * dot12 ) * invDenom;
                Real v = ( dot00 * dot12 - dot01 * dot02 ) * invDenom;

                // Check if point is in triangle
                return ( u >= 0 ) && ( v >= 0 ) && ( u + v - 1 <= 0 );
            }

            DelaunayTriangle::InsideType DelaunayTriangle::IsPointInsideCircumcircle(const Vector2& Point) const
            {
                Vector2 v0 = this->GetPoint(0);
                Vector2 v1 = this->GetPoint(1);
                Vector2 v2 = this->GetPoint(2);
                Matrix4x4 Mat( v0.X, v0.Y, v0.SquaredLength(), 1.0,
                               v1.X, v1.Y, v1.SquaredLength(), 1.0,
                               v2.X, v2.Y, v2.SquaredLength(), 1.0,
                               Point.X, Point.Y, Point.SquaredLength(), 1.0 );
                Real det = Mat.GetDeterminant();
                if( det >= 0 )
                    return IT_Inside;
                if( det > -1e-3 )
                    return IT_BorderlineOutside;
                return IT_Outside;
            }

            Boole DelaunayTriangle::IsDegenerate() const
            {
                if( MathTools::Abs( ( this->GetPoint(1) - this->GetPoint(0) ).CrossProduct( this->GetPoint(2) - this->GetPoint(0) ) ) < 1e-4 )
                    return true;
                return false;
            }

            String DelaunayTriangle::GetDebugDescription() const
            {
                return "(" + StringTools::ConvertToString( this->Indexes[0] ) + ","
                       + StringTools::ConvertToString( this->Indexes[1] ) + "," + StringTools::ConvertToString( this->Indexes[2] ) + ") <"
                       "(" + StringTools::ConvertToString( this->GetPoint(0) ) + ","
                       + StringTools::ConvertToString( this->GetPoint(1) ) + "," + StringTools::ConvertToString( this->GetPoint(2) ) + ">";
            }

            Boole DelaunayTriangle::operator==(const DelaunayTriangle& Other) const
            {
                return ( this->Indexes[0] == Other.Indexes[0] && this->Indexes[1] == Other.Indexes[1] && this->Indexes[2] == Other.Indexes[2] );
            }

            ///////////////////////////////////////////////////////////////////////////////
            // TouchSuperTriangle Methods

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            TouchSuperTriangle::TouchSuperTriangle(const Integer First, const Integer Second, const Integer Third) :
                Index0(First),
                Index1(Second),
                Index2(Third)
                {  }

            TouchSuperTriangle::~TouchSuperTriangle()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Triangulator Methods

            Boole TouchSuperTriangle::operator()(const DelaunayTriangle& Tri)
            {
                for( Integer Index = 0 ; Index < 3 ; ++Index )
                {
                    if( Tri.Indexes[Index] == this->Index0 || Tri.Indexes[Index] == this->Index1 || Tri.Indexes[Index] == this->Index2 )
                        return true;
                }
                return false;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            Triangulator::Triangulator()
                {  }

            Triangulator::~Triangulator()
                {  }

            void Triangulator::Delaunay(Point2DContainer& List, DelaunayTriangleBuffer& TriBuf) const
            {
                // Compute super triangle or insert manual super triangle
                if( !this->ManualSuperTriangle ) {
                    Real MaxTriangleSize = 0.0;
                    for( Point2DIterator PointIt = List.begin() ; PointIt != List.end() ; ++PointIt )
                    {
                        MaxTriangleSize = std::max<Real>( MaxTriangleSize, MathTools::Abs( PointIt->X ) );
                        MaxTriangleSize = std::max<Real>( MaxTriangleSize, MathTools::Abs( PointIt->Y ) );
                    }

                    List.push_back( Vector2( -3 * MaxTriangleSize, -3 * MaxTriangleSize ) );
                    List.push_back( Vector2( 3 * MaxTriangleSize, -3 * MaxTriangleSize ) );
                    List.push_back( Vector2( 0.0, 3 * MaxTriangleSize) );

                    Integer MaxTriangleIndex = List.size() - 3;
                    DelaunayTriangle SuperTriangle( &List );
                    SuperTriangle.Indexes[0] = MaxTriangleIndex;
                    SuperTriangle.Indexes[1] = MaxTriangleIndex + 1;
                    SuperTriangle.Indexes[2] = MaxTriangleIndex + 2;
                    TriBuf.push_back( SuperTriangle );
                }

                // Point insertion loop
                for( UInt16 PointIndex = 0 ; PointIndex < List.size() - 3 ; ++PointIndex )
                {
                    std::list< std::list<DelaunayTriangle>::iterator > BorderlineTriangles;
                    // Insert 1 point, find all triangles for which the point is in circumcircle
                    Vector2& CurrPoint = List[ PointIndex ];
                    std::set<DelaunaySegment> Segments;
                    for( DelaunayTriangleBuffer::iterator TriIt = TriBuf.begin() ; TriIt != TriBuf.end() ;  )
                    {
                        DelaunayTriangle::InsideType IsInside = TriIt->IsPointInsideCircumcircle( CurrPoint );
                        if( IsInside == DelaunayTriangle::IT_Inside ) {
                            if( !TriIt->IsDegenerate() ) {
                                for( Integer TriPointIndex = 0 ; TriPointIndex < 3 ; ++TriPointIndex )
                                {
                                    DelaunaySegment DSeg1( TriIt->Indexes[ TriPointIndex ], TriIt->Indexes[ ( TriPointIndex + 1 ) % 3 ] );
                                    if( Segments.find(DSeg1) != Segments.end() ) {
                                        Segments.erase(DSeg1);
                                    }else if( Segments.find( DSeg1.Inverse() ) != Segments.end() ) {
                                        Segments.erase( DSeg1.Inverse() );
                                    }else{
                                        Segments.insert(DSeg1);
                                    }
                                }
                            }
                            TriIt = TriBuf.erase(TriIt);
                        }else if( IsInside == DelaunayTriangle::IT_BorderlineOutside ) {
                            BorderlineTriangles.push_back(TriIt);
                            ++TriIt;
                        }else{
                            ++TriIt;
                        }
                    }

                    // Robustification of the standard algorithm : if one triangle's circumcircle was borderline against the new point,
                    // test whether that triangle is intersected by new segments or not (normal situation : it should not)
                    // If intersected, the triangle is considered having the new point in its circumc
                    std::set<DelaunaySegment> CopySegment = Segments;
                    for( std::list< std::list<DelaunayTriangle>::iterator >::iterator BorderItIt = BorderlineTriangles.begin() ; BorderItIt != BorderlineTriangles.end() ; BorderItIt++ )
                    {
                        DelaunayTriangleBuffer::iterator BorderTriIt = (*BorderItIt);
                        Boole TriRemoved = false;
                        for( std::set<DelaunaySegment>::iterator SegIt = CopySegment.begin() ; SegIt != CopySegment.end() && !TriRemoved ; ++SegIt )
                        {
                            Boole IsTriangleIntersected = false;
                            for( Integer SegIndex = 0 ; SegIndex < 2 ; ++SegIndex )
                            {
                                Integer Index1 = ( SegIndex == 0 ) ? SegIt->Index1 : SegIt->Index2;
                                Integer Index2 = PointIndex;
                                for( Integer TriCheckIndex = 0 ; TriCheckIndex < 3 ; ++TriCheckIndex )
                                {
                                    //Early out if 2 points are in fact the same
                                    if( BorderTriIt->Indexes[ TriCheckIndex ] == Index1 ||
                                        BorderTriIt->Indexes[ TriCheckIndex ] == Index2 ||
                                        BorderTriIt->Indexes[ ( TriCheckIndex + 1 ) % 3 ] == Index1 ||
                                        BorderTriIt->Indexes[ ( TriCheckIndex + 1 ) % 3 ] == Index2 )
                                        { continue; }
                                    LineSegment2D Seg2( BorderTriIt->GetPoint( TriCheckIndex ), BorderTriIt->GetPoint( ( TriCheckIndex + 1 ) % 3 ) );
                                    LineSegment2D Seg1( List[Index1], List[Index2] );
                                    LineSegment2D::IntersectionTestResult Result = Seg1.Intersects(Seg2);
                                    if( Result.first ) {
                                        IsTriangleIntersected = true;
                                        break;
                                    }
                                }
                            }
                            if( IsTriangleIntersected ) {
                                if( !BorderTriIt->IsDegenerate() ) {
                                    for( Integer TriPointIndex = 0 ; TriPointIndex < 3 ; ++TriPointIndex )
                                    {
                                        DelaunaySegment DSeg1( BorderTriIt->Indexes[ TriPointIndex ], BorderTriIt->Indexes[ ( TriPointIndex + 1 ) % 3 ] );
                                        if( Segments.find(DSeg1) != Segments.end() ) {
                                            Segments.erase(DSeg1);
                                        }else if( Segments.find( DSeg1.Inverse() ) != Segments.end() ) {
                                            Segments.erase( DSeg1.Inverse() );
                                        }else{
                                            Segments.insert(DSeg1);
                                        }
                                    }
                                }
                                TriBuf.erase(BorderTriIt);
                                TriRemoved = true;
                            }
                        }
                    }

                    // Find all the non-interior edges
                    for( std::set<DelaunaySegment>::iterator EdgeIt = Segments.begin() ; EdgeIt != Segments.end() ; ++EdgeIt )
                    {
                        DelaunayTriangle EdgeTriangle( &List );
                        EdgeTriangle.SetVertices( EdgeIt->Index1, EdgeIt->Index2, PointIndex );
                        EdgeTriangle.MakeDirectIfNeeded();
                        TriBuf.push_back(EdgeTriangle);

                    }
                }

                // NB : Don't remove super triangle here, because all outer triangles are already removed in the addconstraints method.
                //      Uncomment that code if delaunay triangulation ever has to be unconstrained...
                /*TouchSuperTriangle TouchSuperTriangle( MaxTriangleIndex, MaxTriangleIndex + 1, MaxTriangleIndex + 2 );
                TriBuf.remove_if( TouchSuperTriangle );
                List.pop_back();
                List.pop_back();
                List.pop_back();*/
            }

            void Triangulator::_AddConstraints(DelaunayTriangleBuffer& TriBuf, const Point2DContainer& List, const IndexContainer& SegmentListIndices) const
            {
                DelaunaySegmentContainer DelSegList;

                // First, list all the segments that are not already in one of the delaunay triangles
                for( IndexContainer::const_iterator SegIndexIt = SegmentListIndices.begin() ; SegIndexIt != SegmentListIndices.end() ; ++SegIndexIt )
                {
                    Integer Index1 = *SegIndexIt;
                    SegIndexIt++;
                    Integer Index2 = *SegIndexIt;
                    Boole IsAlreadyIn = false;
                    for( DelaunayTriangleBuffer::iterator TriIt = TriBuf.begin() ; TriIt != TriBuf.end() ; ++TriIt )
                    {
                        if( TriIt->ContainsSegment(Index1,Index2) ) {
                            IsAlreadyIn = true;
                            break;
                        }
                    }
                    // only do something for segments not already in DT
                    if( !IsAlreadyIn ) {
                        DelSegList.push_back( DelaunaySegment(Index1,Index2) );
                    }
                }

                // Re-Triangulate according to the new segments
                for( DelaunaySegmentContainer::iterator DelSegIt = DelSegList.begin() ; DelSegIt != DelSegList.end() ; DelSegIt++ )
                {
                    // Remove all triangles intersecting the segment and keep a list of outside edges
                    std::set<DelaunaySegment> Segments;
                    LineSegment2D Seg1( List[DelSegIt->Index1], List[DelSegIt->Index2] );
                    for( DelaunayTriangleBuffer::iterator DelTriIt = TriBuf.begin() ; DelTriIt != TriBuf.end() ;  )
                    {
                        Boole IsTriangleIntersected = false;
                        Integer DegenIndex;
                        for( Integer TriPoint = 0 ; TriPoint < 3 ; ++TriPoint )
                        {
                            //Early out if 2 points are in fact the same
                            if( DelTriIt->Indexes[ TriPoint ] == DelSegIt->Index1 ||
                                DelTriIt->Indexes[ TriPoint ] == DelSegIt->Index2 ||
                                DelTriIt->Indexes[ ( TriPoint + 1 ) % 3 ] == DelSegIt->Index1 ||
                                DelTriIt->Indexes[ ( TriPoint + 1 ) % 3 ] == DelSegIt->Index2 )
                            {
                                if( DelTriIt->IsDegenerate() ) {
                                    if( DelTriIt->Indexes[ TriPoint ] == DelSegIt->Index1 || DelTriIt->Indexes[ ( TriPoint + 1 ) % 3 ] == DelSegIt->Index1 ) {
                                        DegenIndex = DelSegIt->Index1;
                                    }else if( DelTriIt->Indexes[ TriPoint ] == DelSegIt->Index2 || DelTriIt->Indexes[ ( TriPoint + 1 ) % 3 ] == DelSegIt->Index2 ) {
                                        DegenIndex = DelSegIt->Index2;
                                    }
                                    IsTriangleIntersected = true;
                                }else{
                                    continue;
                                }
                            }
                            LineSegment2D Seg2( DelTriIt->GetPoint( TriPoint ), DelTriIt->GetPoint( ( TriPoint + 1 ) % 3 ) );
                            LineSegment2D::IntersectionTestResult Result = Seg1.Intersects(Seg2);
                            if( Result.first ) {
                                IsTriangleIntersected = true;
                                break;
                            }
                        }
                        if( IsTriangleIntersected ) {
                            for( Integer TriPoint = 0 ; TriPoint < 3 ; ++TriPoint )
                            {
                                DelaunaySegment DelSeg1( DelTriIt->Indexes[ TriPoint ], DelTriIt->Indexes[ ( TriPoint + 1 ) % 3 ] );
                                if( Segments.find(DelSeg1) != Segments.end() ) {
                                    Segments.erase(DelSeg1);
                                }else if( Segments.find( DelSeg1.Inverse() ) != Segments.end() ) {
                                    Segments.erase( DelSeg1.Inverse() );
                                }else{
                                    Segments.insert(DelSeg1);
                                }
                            }
                            DelTriIt = TriBuf.erase( DelTriIt );
                        }else{
                            DelTriIt++;
                        }
                    }

                    // Divide the list of points (coming from remaining segments) in 2 groups : "above" and "below"
                    IndexContainer PointsAbove;
                    IndexContainer PointsBelow;
                    Integer Point = DelSegIt->Index1;
                    Boole IsAbove = true;
                    while( Segments.size() > 0 )
                    {
                        //find next point
                        for( std::set<DelaunaySegment>::iterator SegSetIt = Segments.begin() ; SegSetIt != Segments.end() ; ++SegSetIt )
                        {
                            if( SegSetIt->Index1 == Point || SegSetIt->Index2 == Point ) {
                                if( SegSetIt->Index1 == Point ) {
                                    Point = SegSetIt->Index2;
                                }else{
                                    Point = SegSetIt->Index1;
                                }
                                Segments.erase(SegSetIt);
                                if( Point == DelSegIt->Index2 ) {
                                    IsAbove = false;
                                }else if( Point != DelSegIt->Index1 ) {
                                    if( IsAbove ) {
                                        PointsAbove.push_back(Point);
                                    }else{
                                        PointsBelow.push_back(Point);
                                    }
                                }
                                break;
                            }
                        }
                    }

                    // Recursively triangulate both polygons
                    this->_RecursiveTriangulatePolygon(*DelSegIt,PointsAbove,TriBuf,List);
                    this->_RecursiveTriangulatePolygon(DelSegIt->Inverse(),PointsBelow,TriBuf,List);
                }

                // Clean up segments outside of multishape
                if( this->RemoveOutside ) {
                    if( this->MultiShapeToTriangulate && this->MultiShapeToTriangulate->IsClosed() ) {
                        for( DelaunayTriangleBuffer::iterator TriIt = TriBuf.begin() ; TriIt != TriBuf.end() ;  )
                        {
                            Boole IsTriangleOut = !this->MultiShapeToTriangulate->IsPointInside( TriIt->GetMidPoint() );

                            if( IsTriangleOut ) {
                                TriIt = TriBuf.erase(TriIt);
                            }else{
                                ++TriIt;
                            }
                        }
                    }else if( this->ShapeToTriangulate && this->ShapeToTriangulate->IsClosed() ) {
                        for( DelaunayTriangleBuffer::iterator TriIt = TriBuf.begin() ; TriIt != TriBuf.end() ;  )
                        {
                            Boole IsTriangleOut = !this->ShapeToTriangulate->IsPointInside( TriIt->GetMidPoint() );

                            if( IsTriangleOut ) {
                                TriIt = TriBuf.erase(TriIt);
                            }else{
                                ++TriIt;
                            }
                        }
                    }
                }
            }

            void Triangulator::_RecursiveTriangulatePolygon(const DelaunaySegment& CuttingSeg, const IndexContainer& InputPoints, DelaunayTriangleBuffer& TriBuf, const Point2DContainer& List) const
            {
                if( InputPoints.size() == 0 )
                    return;
                if( InputPoints.size() ==1 ) {
                    DelaunayTriangle Tri(&List);
                    Tri.SetVertices( CuttingSeg.Index1, CuttingSeg.Index2, *InputPoints.begin() );
                    Tri.MakeDirectIfNeeded();
                    TriBuf.push_back(Tri);
                    return;
                }
                // Find a point which, when associated with seg.i1 and seg.i2, builds a Delaunay triangle
                IndexContainer::const_iterator CurrentPoint = InputPoints.begin();
                Boole Found = false;
                while( !Found )
                {
                    Boole IsDelaunay = true;
                    Circle Cir( List[*CurrentPoint], List[CuttingSeg.Index1], List[CuttingSeg.Index2] );
                    for( IndexContainer::const_iterator it = InputPoints.begin() ; it != InputPoints.end() ; ++it )
                    {
                        if( Cir.IsInside( List[*it] ) && (*it != *CurrentPoint) ) {
                            IsDelaunay = false;
                            CurrentPoint = it;
                            break;
                        }
                    }
                    if( IsDelaunay ) {
                        Found = true;
                    }
                }

                // Insert current triangle
                DelaunayTriangle Tri(&List);
                Tri.SetVertices(*CurrentPoint, CuttingSeg.Index1, CuttingSeg.Index2);
                Tri.MakeDirectIfNeeded();
                TriBuf.push_back(Tri);

                // Recurse
                DelaunaySegment NewCut1( CuttingSeg.Index1, *CurrentPoint );
                DelaunaySegment NewCut2( CuttingSeg.Index2, *CurrentPoint );
                IndexContainer Set1( InputPoints.begin(), CurrentPoint );
                IndexContainer Set2( CurrentPoint + 1, InputPoints.end() );

                if( !Set1.empty() ) {
                    this->_RecursiveTriangulatePolygon(NewCut1,Set1,TriBuf,List);
                }
                if( !Set2.empty() ) {
                    this->_RecursiveTriangulatePolygon(NewCut2,Set2,TriBuf,List);
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void Triangulator::AddToTriangleBuffer(TriangleBuffer& Buffer) const
            {
                Point2DContainer Points;
                IndexContainer IndexBuffer;
                this->Triangulate(IndexBuffer,Points);
                for( size_t CurrPoint = 0 ; CurrPoint < Points.size() ; ++CurrPoint )
                {
                    Vector2 VP2 = Points[CurrPoint];
                    Vector3 VP(VP2.X,VP2.Y,0);
                    Vector3 Normal = -Vector3::Unit_Z();

                    this->AddPoint(Buffer,VP,Normal,Vector2(VP2.X,VP2.Y));
                }

                for( size_t CurrIndex = 0 ; CurrIndex < IndexBuffer.size() / 3 ; ++CurrIndex )
                {
                    Buffer.AddIndex( IndexBuffer[ CurrIndex * 3 ] );
                    Buffer.AddIndex( IndexBuffer[ CurrIndex * 3 + 2 ] );
                    Buffer.AddIndex( IndexBuffer[ CurrIndex * 3 + 1 ] );
                }
            }

            void Triangulator::Triangulate(IndexContainer& Indexes, Point2DContainer& Vertices) const
            {
                if( this->ShapeToTriangulate == NULL && this->MultiShapeToTriangulate == NULL && this->SegmentListToTriangulate == NULL )
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"A Shape, MultiShape, or SegmentList to triangulate must be set.");

                DelaunayTriangleBuffer DelaunayTriBuf;
                // Do the Delaunay triangulation
                IndexContainer SegmentListIndices;

                if( this->ShapeToTriangulate ) {
                    Vertices = this->ShapeToTriangulate->GetPoints();
                    for( Whole SegIndex = 0 ; SegIndex < this->ShapeToTriangulate->GetSegCount() ; ++SegIndex )
                    {
                        SegmentListIndices.push_back( SegIndex );
                        SegmentListIndices.push_back( this->ShapeToTriangulate->GetBoundedIndex( SegIndex + 1 ) );
                    }
                }else if( this->MultiShapeToTriangulate ) {
                    Vertices = this->MultiShapeToTriangulate->GetPoints();
                    Whole TotalIndex = 0;
                    for( Whole ShapeIndex = 0 ; ShapeIndex < this->MultiShapeToTriangulate->GetNumShapes() ; ++ShapeIndex )
                    {
                        const Shape& CurrShape = this->MultiShapeToTriangulate->GetShape(ShapeIndex);
                        for( Whole SegIndex = 0 ; SegIndex < CurrShape.GetSegCount() ; ++SegIndex )
                        {
                            SegmentListIndices.push_back( TotalIndex + SegIndex );
                            SegmentListIndices.push_back( TotalIndex + CurrShape.GetBoundedIndex( SegIndex + 1 ) );
                        }
                        TotalIndex += CurrShape.GetSegCount();
                    }
                }else if( this->SegmentListToTriangulate ) {
                    typedef std::map<Vector2,Integer,Vector2LengthCompare> PointIndexMap;
                    PointIndexMap PointMap;

                    for( SegmentContainer::iterator SegIt = this->SegmentListToTriangulate->begin() ; SegIt != this->SegmentListToTriangulate->end() ; SegIt++ )
                    {
                        if( SegIt->PointA.SquaredDistance( SegIt->PointB ) < 1e-6 ) {
                            continue;
                        }

                        PointIndexMap::iterator PointIt = PointMap.find( SegIt->PointA );
                        if( PointIt != PointMap.end() ) {
                            SegmentListIndices.push_back( PointIt->second );
                        }else{
                            PointMap[ SegIt->PointA ] = Vertices.size();
                            SegmentListIndices.push_back( Vertices.size() );
                            Vertices.push_back( SegIt->PointA );
                        }

                        PointIt = PointMap.find( SegIt->PointB );
                        if( PointIt != PointMap.end() ) {
                            SegmentListIndices.push_back( PointIt->second );
                        }else{
                            PointMap[ SegIt->PointB ] = Vertices.size();
                            SegmentListIndices.push_back( Vertices.size() );
                            Vertices.push_back( SegIt->PointB );
                        }
                    }

                    if( this->ManualSuperTriangle ) {
                        DelaunayTriangle SuperTriangle(&Vertices);
                        for( Whole PointIndex = 0 ; PointIndex < 3 ; PointIndex++ )
                        {
                            PointIndexMap::iterator TriPointIt = PointMap.find( (*this->ManualSuperTriangle)[PointIndex] );
                            if( TriPointIt != PointMap.end() ) {
                                //SegmentListIndices.push_back(it->second);
                                SuperTriangle.Indexes[PointIndex] = TriPointIt->second;
                            }else{
                                PointMap[ (*this->ManualSuperTriangle)[PointIndex] ] = Vertices.size();
                                //SegmentListIndices.push_back(Vertices.size());
                                SuperTriangle.Indexes[PointIndex] = Vertices.size();
                                Vertices.push_back( (*this->ManualSuperTriangle)[PointIndex] );
                            }
                        }

                        DelaunayTriBuf.push_back(SuperTriangle);
                    }
                }
                this->Delaunay(Vertices,DelaunayTriBuf);
                // Add contraints
                this->_AddConstraints(DelaunayTriBuf,Vertices,SegmentListIndices);
                //Outputs index buffer
                for( DelaunayTriangleBuffer::iterator TriIt = DelaunayTriBuf.begin() ; TriIt != DelaunayTriBuf.end() ; ++TriIt )
                {
                    if( !TriIt->IsDegenerate() ) {
                        Indexes.push_back( TriIt->Indexes[0] );
                        Indexes.push_back( TriIt->Indexes[1] );
                        Indexes.push_back( TriIt->Indexes[2] );
                    }
                }

                // Remove super triangle
                if( this->RemoveOutside ) {
                    Vertices.pop_back();
                    Vertices.pop_back();
                    Vertices.pop_back();
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            Triangulator& Triangulator::SetShapeToTriangulate(const Shape* TriShape)
            {
                this->ShapeToTriangulate = TriShape;
                this->MultiShapeToTriangulate = NULL;
                this->SegmentListToTriangulate = NULL;
                return *this;
            }

            Triangulator& Triangulator::SetMultiShapeToTriangulate(const MultiShape* TriMultiShape)
            {
                this->ShapeToTriangulate = NULL;
                this->MultiShapeToTriangulate = TriMultiShape;
                this->SegmentListToTriangulate = NULL;
                return *this;
            }

            Triangulator& Triangulator::SetSegmentListToTriangulate(SegmentContainer* Segments)
            {
                this->ShapeToTriangulate = NULL;
                this->MultiShapeToTriangulate = NULL;
                this->SegmentListToTriangulate = Segments;
                return *this;
            }

            Triangulator& Triangulator::SetManualSuperTriangle(Triangle2D* Tri)
            {
                this->ManualSuperTriangle = Tri;
                return *this;
            }

            Triangulator& Triangulator::SetRemoveOutside(const Boole Remove)
            {
                this->RemoveOutside = Remove;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
