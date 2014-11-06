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
#ifndef _graphicsproceduralbooleanmeshgenerator_cpp
#define _graphicsproceduralbooleanmeshgenerator_cpp

#include "Graphics/Procedural/booleanmeshgenerator.h"
#include "Graphics/Procedural/triangulator.h"
#include "Graphics/Procedural/path.h"

#include "MathTools/mathtools.h"
#include "linesegment.h"
#include "triangle.h"
#include "exception.h"

namespace
{
    // Keeps this section form being documented by doxygen
    /// @cond DontDocumentInternal
    /// @todo Document this anonymous namespace?

    struct Intersect
    {
        Mezzanine::LineSegment3D Segment;
        Mezzanine::Integer Triangle1;
        Mezzanine::Integer Triangle2;

        Intersect(Mezzanine::LineSegment3D seg, Mezzanine::Integer tri1, Mezzanine::Integer tri2) :
            Segment(seg),
            Triangle1(tri1),
            Triangle2(tri2)
            {  }
    };//Intersect

    struct Seg3Comparator
    {
        Mezzanine::Boole operator()(const Mezzanine::LineSegment3D& One, const Mezzanine::LineSegment3D& Two) const
        {
            if ( One.EpsilonEquivalent(Two) )
                return false;

            if ( One.PointA.SquaredDistance(Two.PointA) > 1e-6 )
                return Mezzanine::Vector3LengthCompare()( One.PointA, Two.PointA );

            return Mezzanine::Vector3LengthCompare()( One.PointB, Two.PointB );
        }
    };//Seg3Comparator

    inline Mezzanine::Vector2 ProjectOnAxis(const Mezzanine::Vector3& Input, const Mezzanine::Vector3& Origin, const Mezzanine::Vector3& Axis1, const Mezzanine::Vector3& Axis2)
    {
        return Mezzanine::Vector2( (Input - Origin).DotProduct(Axis1), (Input - Origin).DotProduct(Axis2) );
    }

    inline Mezzanine::Vector3 DeprojectOnAxis(const Mezzanine::Vector2& Input, const Mezzanine::Vector3& Origin, const Mezzanine::Vector3& Axis1, const Mezzanine::Vector3& Axis2)
    {
        return Origin + ( Axis1 * Input.X ) + ( Axis2 * Input.Y );
    }

    inline Mezzanine::LineSegment2D ProjectOnAxis(const Mezzanine::LineSegment3D& Input, const Mezzanine::Vector3& Origin, const Mezzanine::Vector3& Axis1, const Mezzanine::Vector3& Axis2)
    {
        return Mezzanine::LineSegment2D(ProjectOnAxis(Input.PointA, Origin, Axis1, Axis2), ProjectOnAxis(Input.PointB, Origin, Axis1, Axis2));
    }

    typedef Mezzanine::Graphics::Procedural::TriangleBuffer TriangleBuffer;
    typedef Mezzanine::Graphics::Procedural::Vertex Vertex;
    typedef Mezzanine::Graphics::Procedural::IndexContainer IndexContainer;
    typedef Mezzanine::Graphics::Procedural::VertexContainer VertexContainer;
    typedef Mezzanine::Graphics::Procedural::LineSeg2DVec LineSeg2DVec;
    typedef Mezzanine::Graphics::Procedural::LineSeg3DVec LineSeg3DVec;
    typedef Mezzanine::Graphics::Procedural::PathContainer PathContainer;
    typedef std::multimap<Mezzanine::LineSegment3D,Mezzanine::Integer,Seg3Comparator> TriLookup;
    typedef std::pair<TriLookup::iterator,TriLookup::iterator> TriLookupRange;
    typedef std::set<Mezzanine::LineSegment3D,Seg3Comparator> LineSeg3DSet;
    typedef std::map<Mezzanine::Integer,LineSeg3DVec> LineSeg3DMap;
    typedef std::vector<Intersect> IntersectContainer;

    void _RemoveFromTriLookup(Mezzanine::Integer Index, TriLookup& Lookup)
    {
        for ( TriLookup::iterator LookupIt = Lookup.begin() ; LookupIt != Lookup.end() ;  )
        {
            TriLookup::iterator RemoveIt = LookupIt++;
            if( RemoveIt->second == Index )
                Lookup.erase(RemoveIt);
        }
    }

    void _RecursiveAddNeighbour(TriangleBuffer& Result, const TriangleBuffer& Source, Mezzanine::Integer TriNumber, TriLookup& Lookup, const LineSeg3DSet& Limits, Mezzanine::Boole Inverted)
    {
        if( TriNumber == -1 )
            return;
        const IndexContainer& Indices = Source.GetIndices();
        const VertexContainer& Vertices = Source.GetVertices();
        Result.RebaseOffset();
        if( Inverted ) {
            Result.AddTriangle(0, 2, 1);
            Vertex Vert = Vertices[ Indices[ TriNumber * 3 ] ];
            Vert.Normal = -Vert.Normal;
            Result.AddVertex(Vert);
            Vert = Vertices[ Indices[ TriNumber * 3 + 1 ] ];
            Vert.Normal = -Vert.Normal;
            Result.AddVertex(Vert);
            Vert = Vertices[ Indices[ TriNumber * 3 + 2 ] ];
            Vert.Normal = -Vert.Normal;
            Result.AddVertex(Vert);
        }else{
            Result.AddTriangle(0, 1, 2);
            Result.AddVertex( Vertices[ Indices[ TriNumber * 3 ] ] );
            Result.AddVertex( Vertices[ Indices[ TriNumber * 3 + 1 ] ] );
            Result.AddVertex( Vertices[ Indices[ TriNumber * 3 + 2 ] ] );
        }

        TriLookup::iterator it;

        Mezzanine::Integer NextTriangle1 = -1;
        Mezzanine::Integer NextTriangle2 = -1;
        Mezzanine::Integer NextTriangle3 = -1;
        it = Lookup.find( Mezzanine::LineSegment3D( Vertices[ Indices[ TriNumber * 3 ] ].Position, Vertices[ Indices[ TriNumber * 3 + 1 ] ].Position ).GetOrderedCopy() );
        //if( it != Lookup.end() && Limits.find( it->first.GetOrderedCopy() ) != Limits.end() )

        if( it != Lookup.end() && Limits.find( it->first.GetOrderedCopy() ) == Limits.end() ) {
            NextTriangle1 = it->second;
            _RemoveFromTriLookup(NextTriangle1,Lookup);
        }
        it = Lookup.find( Mezzanine::LineSegment3D( Vertices[ Indices[ TriNumber * 3 + 1 ] ].Position, Vertices[ Indices[ TriNumber * 3 + 2 ] ].Position ).GetOrderedCopy() );
        //if( it != Lookup.end() && Limits.find( it->first.GetOrderedCopy() ) != Limits.end() )

        if( it != Lookup.end() && Limits.find( it->first.GetOrderedCopy() ) == Limits.end() ) {
            NextTriangle2 = it->second;
            _RemoveFromTriLookup(NextTriangle2,Lookup);
        }
        it = Lookup.find( Mezzanine::LineSegment3D( Vertices[ Indices[ TriNumber * 3 ] ].Position, Vertices[ Indices[ TriNumber * 3 + 2 ] ].Position ).GetOrderedCopy());
        //if( it != Lookup.end() && Limits.find( it->first.GetOrderedCopy() ) != Limits.end() )
        if( it != Lookup.end() && Limits.find( it->first.GetOrderedCopy() ) == Limits.end() ) {
            NextTriangle3 = it->second;
            _RemoveFromTriLookup(NextTriangle3,Lookup);
        }

        _RecursiveAddNeighbour(Result,Source,NextTriangle1,Lookup,Limits,Inverted);
        _RecursiveAddNeighbour(Result,Source,NextTriangle2,Lookup,Limits,Inverted);
        _RecursiveAddNeighbour(Result,Source,NextTriangle3,Lookup,Limits,Inverted);
    }

    void _Retriangulate(TriangleBuffer& NewBuff, const TriangleBuffer& InputBuff, const IntersectContainer& IntersectionList, Mezzanine::Boole First)
    {
        const VertexContainer& Vertices = InputBuff.GetVertices();
        const IndexContainer& Indices = InputBuff.GetIndices();
        // Triangulate
        //  Group intersections by triangle indice
        LineSeg3DMap MeshIntersects;
        for( IntersectContainer::const_iterator IntersectIt = IntersectionList.begin() ; IntersectIt != IntersectionList.end() ; ++IntersectIt )
        {
            LineSeg3DMap::iterator SegMapIt;
            if( First ) {
                SegMapIt = MeshIntersects.find( IntersectIt->Triangle1 );
            }else{
                SegMapIt = MeshIntersects.find( IntersectIt->Triangle2 );
            }
            if( SegMapIt != MeshIntersects.end() ) {
                SegMapIt->second.push_back( IntersectIt->Segment );
            }else{
                LineSeg3DVec Segments;
                Segments.push_back( IntersectIt->Segment );
                if( First ) {
                    MeshIntersects[ IntersectIt->Triangle1 ] = Segments;
                }else{
                    MeshIntersects[ IntersectIt->Triangle2 ] = Segments;
                }
            }
        }
        // Build a new TriangleBuffer holding non-intersected triangles and retriangulated-intersected triangles
        for( VertexContainer::const_iterator it = Vertices.begin() ; it != Vertices.end() ; ++it )
            { NewBuff.AddVertex(*it); }
        for( Mezzanine::Integer i = 0 ; i < (Mezzanine::Integer)Indices.size() / 3 ; i++ )
        {
            if( MeshIntersects.find(i) == MeshIntersects.end() ) {
                NewBuff.AddTriangle( Indices[i * 3], Indices[i * 3 + 1], Indices[i * 3 + 2] );
            }
        }
        //Mezzanine::Integer numNonIntersected1 = NewBuff.GetIndices().size();
        for( LineSeg3DMap::iterator SegMapIt = MeshIntersects.begin() ; SegMapIt != MeshIntersects.end() ; ++SegMapIt )
        {
            LineSeg3DVec& Segments = SegMapIt->second;
            Mezzanine::Integer TriIndex = SegMapIt->first;
            Mezzanine::Vector3 v1 = Vertices[ Indices[ TriIndex * 3 ] ].Position;
            Mezzanine::Vector3 v2 = Vertices[ Indices[ TriIndex * 3 + 1 ] ].Position;
            Mezzanine::Vector3 v3 = Vertices[ Indices[ TriIndex * 3 + 2 ] ].Position;
            Mezzanine::Vector3 TriNormal = ( ( v2 - v1 ).CrossProduct( v3 - v1 ) ).GetNormal();
            Mezzanine::Vector3 xAxis = TriNormal.Perpendicular();
            Mezzanine::Vector3 yAxis = TriNormal.CrossProduct(xAxis);
            Mezzanine::Vector3 PlaneOrigin = Vertices[ Indices[ TriIndex * 3 ] ].Position;

            // Project intersection segments onto triangle plane
            LineSeg2DVec Segments2;

            for( LineSeg3DVec::iterator SegVec1It = Segments.begin() ; SegVec1It != Segments.end() ; SegVec1It++ )
                { Segments2.push_back( ProjectOnAxis(*SegVec1It, PlaneOrigin, xAxis, yAxis) ); }
            for( LineSeg2DVec::iterator SegVec2It = Segments2.begin() ; SegVec2It != Segments2.end() ;  )
            {
                if( ( SegVec2It->PointA - SegVec2It->PointB ).SquaredLength() < 1e-5 ) {
                    SegVec2It = Segments2.erase(SegVec2It);
                }else{
                    SegVec2It++;
                }
            }

            // Triangulate
            Mezzanine::Graphics::Procedural::Triangulator Triangulate;
            Mezzanine::Triangle2D Tri(ProjectOnAxis( Vertices[ Indices[ TriIndex * 3] ].Position, PlaneOrigin, xAxis, yAxis),
                                      ProjectOnAxis( Vertices[ Indices[ TriIndex * 3 + 1] ].Position, PlaneOrigin, xAxis, yAxis),
                                      ProjectOnAxis( Vertices[ Indices[ TriIndex * 3 + 2] ].Position, PlaneOrigin, xAxis, yAxis));
            Mezzanine::Graphics::Procedural::Point2DContainer outPointList;
            IndexContainer outIndice;
            Triangulate.SetManualSuperTriangle(&Tri).SetRemoveOutside(false).SetSegmentListToTriangulate(&Segments2).Triangulate(outIndice, outPointList);

            // Deproject and add to triangleBuffer
            NewBuff.RebaseOffset();
            for( IndexContainer::iterator IndexIt = outIndice.begin() ; IndexIt != outIndice.end() ; ++IndexIt )
                { NewBuff.AddIndex(*IndexIt); }

            Mezzanine::Real x1 = Tri.PointA.X;
            Mezzanine::Real y1 = Tri.PointA.Y;
            Mezzanine::Vector2 uv1 = Vertices[ Indices[ TriIndex * 3 ] ].UV;
            Mezzanine::Real x2 = Tri.PointB.X;
            Mezzanine::Real y2 = Tri.PointB.Y;
            Mezzanine::Vector2 uv2 = Vertices[ Indices[ TriIndex * 3 + 1 ] ].UV;
            Mezzanine::Real x3 = Tri.PointC.X;
            Mezzanine::Real y3 = Tri.PointC.Y;
            Mezzanine::Vector2 uv3 = Vertices[ Indices[ TriIndex * 3 + 2 ] ].UV;
            Mezzanine::Real DET = x1 * y2 - x2 * y1 + x2 * y3 - x3 * y2 + x3 * y1 - x1 * y3;
            Mezzanine::Vector2 A = ( uv1 * (y2 - y3) + uv2 * (y3 - y1) + uv3 * (y1 - y2) ) / DET;
            Mezzanine::Vector2 B = ( uv1 * (x3 - x2) + uv2 * (x1 - x3) + uv3 * (x2 - x1) ) / DET;
            Mezzanine::Vector2 C = ( uv1 * (x2 * y3 - x3 * y2) + uv2 * (x3 * y1 - x1 * y3) + uv3 * (x1 * y2 - x2 * y1) ) / DET;

            for( std::vector<Mezzanine::Vector2>::iterator PointIt = outPointList.begin() ; PointIt != outPointList.end() ; ++PointIt )
            {
                Mezzanine::Vector2 uv = A * PointIt->X + B * PointIt->Y + C;
                NewBuff.AddVertex(DeprojectOnAxis(*PointIt, PlaneOrigin, xAxis, yAxis),
                                  TriNormal,
                                  uv);
            }
        }
    }

    typedef std::multimap<Mezzanine::LineSegment3D,Mezzanine::Integer,Seg3Comparator> TriLookup;
    typedef std::pair<Mezzanine::LineSegment3D,Mezzanine::Integer> TriLookupPair;

    void _BuildTriLookup(TriLookup& Lookup, const TriangleBuffer& NewMesh)
    {
        const std::vector<Mezzanine::Graphics::Procedural::Vertex>& Verts = NewMesh.GetVertices();
        const Mezzanine::Graphics::Procedural::IndexContainer& Inds = NewMesh.GetIndices();
        for( Mezzanine::Integer Index = 0 ; Index < (Mezzanine::Integer)Inds.size() / 3 ; ++Index )
        {
            Lookup.insert( TriLookupPair( Mezzanine::LineSegment3D( Verts[ Inds[ Index * 3 ] ].Position, Verts[ Inds[ Index * 3 + 1 ] ].Position ).GetOrderedCopy(), Index ) );
            Lookup.insert( TriLookupPair( Mezzanine::LineSegment3D( Verts[ Inds[ Index * 3 ] ].Position, Verts[ Inds[ Index * 3 + 2 ] ].Position ).GetOrderedCopy(), Index ) );
            Lookup.insert( TriLookupPair( Mezzanine::LineSegment3D( Verts[ Inds[ Index * 3 + 1 ] ].Position, Verts[ Inds[ Index * 3 + 2 ] ].Position ).GetOrderedCopy(), Index ) );
        }
    }

    /// @endcond
}

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            BooleanGenerator::BooleanGenerator() :
                BoolOp(Procedural::BO_Union),
                FirstBuffer(NULL),
                SecondBuffer(NULL)
                {  }

            BooleanGenerator::BooleanGenerator(const BooleanOperation Op) :
                BoolOp(Op),
                FirstBuffer(NULL),
                SecondBuffer(NULL)
                {  }

            BooleanGenerator::BooleanGenerator(TriangleBuffer* First, TriangleBuffer* Second, const BooleanOperation Op) :
                BoolOp(Op),
                FirstBuffer(First),
                SecondBuffer(Second)
                {  }

            BooleanGenerator::~BooleanGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void BooleanGenerator::AddToTriangleBuffer(TriangleBuffer& Buffer) const
            {
                const VertexContainer& Vertices1 = this->FirstBuffer->GetVertices();
                const IndexContainer& Indexes1 = this->FirstBuffer->GetIndices();
                const VertexContainer& Vertices2 = this->SecondBuffer->GetVertices();
                const IndexContainer& Indexes2 = this->SecondBuffer->GetIndices();

                LineSegment3D IntersectionResult;
                IntersectContainer IntersectionList;

                // Find all intersections between FirstBuffer and SecondBuffer
                Integer FirstIndex = 0;
                for( IndexContainer::const_iterator FirstBufferIt = Indexes1.begin() ; FirstBufferIt != Indexes1.end() ; FirstIndex++ )
                {
                    Triangle3D Tri1( Vertices1[ *FirstBufferIt++ ].Position, Vertices1[ *FirstBufferIt++ ].Position, Vertices1[ *FirstBufferIt++ ].Position );

                    Integer SecondIndex = 0;
                    for( IndexContainer::const_iterator SecondBufferIt = Indexes2.begin() ; SecondBufferIt != Indexes2.end() ; SecondIndex++ )
                    {
                        Triangle3D Tri2( Vertices2[ *SecondBufferIt++ ].Position, Vertices2[ *SecondBufferIt++ ].Position, Vertices2[ *SecondBufferIt++ ].Position );

                        IntersectionResult = Tri1.GetOverlap(Tri2);
                        if( IntersectionResult.PointA != IntersectionResult.PointB ) {
                            Intersect Inter(IntersectionResult,FirstIndex,SecondIndex);
                            IntersectionList.push_back(Inter);
                        }
                    }
                }
                // Remove all intersection segments too small to be relevant
                for( IntersectContainer::iterator InterIt = IntersectionList.begin() ; InterIt != IntersectionList.end() ;  )
                {
                    if( ( InterIt->Segment.PointB - InterIt->Segment.PointA ).SquaredLength() < 1e-8 ) {
                        InterIt = IntersectionList.erase(InterIt);
                    }else{
                        ++InterIt;
                    }
                }

                // Retriangulate
                TriangleBuffer NewMesh1, NewMesh2;
                _Retriangulate(NewMesh1,*(this->FirstBuffer),IntersectionList,true);
                _Retriangulate(NewMesh2,*(this->SecondBuffer),IntersectionList,false);

                //Buffer.append(NewMesh1);
                //Buffer.append(NewMesh2);
                //return;

                // Trace contours
                PathContainer Contours;
                LineSeg3DVec SegmentSoup;
                for( IntersectContainer::iterator InterIt = IntersectionList.begin() ; InterIt != IntersectionList.end() ; ++InterIt )
                    { SegmentSoup.push_back( InterIt->Segment ); }
                Path::BuildFromSegmentSoup(SegmentSoup,Contours);

                // Build a lookup from segment to triangle
                TriLookup TriLookup1, TriLookup2;
                _BuildTriLookup( TriLookup1, NewMesh1 );
                _BuildTriLookup( TriLookup2, NewMesh2 );

                LineSeg3DSet Limits;
                for( LineSeg3DVec::iterator SegSoupIt = SegmentSoup.begin() ; SegSoupIt != SegmentSoup.end() ; ++SegSoupIt )
                    { Limits.insert( SegSoupIt->GetOrderedCopy() ); }
                // Build resulting mesh
                for( PathContainer::iterator PathIt = Contours.begin() ; PathIt != Contours.end() ; ++PathIt )
                {
                    // Find 2 seed triangles for each contour
                    LineSegment3D FirstSeg( PathIt->GetPoint(0), PathIt->GetPoint(1) );

                    TriLookupRange It2mesh1 = TriLookup1.equal_range( FirstSeg.GetOrderedCopy() );
                    TriLookupRange It2mesh2 = TriLookup2.equal_range( FirstSeg.GetOrderedCopy() );
                    Integer Mesh1Seed1, Mesh1Seed2, Mesh2Seed1, Mesh2Seed2;

                    if( It2mesh1.first != TriLookup1.end() && It2mesh2.first != TriLookup2.end() ) {
                        // check which of seed1 and seed2 must be included (it can be 0, 1 or both)
                        Mesh1Seed1 = It2mesh1.first->second;
                        Mesh1Seed2 = (--It2mesh1.second)->second;
                        Mesh2Seed1 = It2mesh2.first->second;
                        Mesh2Seed2 = (--It2mesh2.second)->second;
                        if( Mesh1Seed1 == Mesh1Seed2 ) {
                            Mesh1Seed2 = -1;
                        }
                        if( Mesh2Seed1 == Mesh2Seed2 ) {
                            Mesh2Seed2 = -1;
                        }

                        Vector3 vMesh1, nMesh1, vMesh2, nMesh2;
                        for( Integer i = 0 ; i < 3 ; i++ )
                        {
                            const Vector3& Position = NewMesh1.GetVertices()[ NewMesh1.GetIndices()[ Mesh1Seed1 * 3 + i ] ].Position;
                            if( Position.SquaredDistance( FirstSeg.PointA ) > 1e-6 && Position.SquaredDistance( FirstSeg.PointB ) > 1e-6 ) {
                                vMesh1 = Position;
                                nMesh1 = NewMesh1.GetVertices()[ NewMesh1.GetIndices()[ Mesh1Seed1 * 3 + i ] ].Normal;
                                break;
                            }
                        }

                        for( Integer i = 0 ; i < 3 ; i++ )
                        {
                            const Vector3& Position = NewMesh2.GetVertices()[ NewMesh2.GetIndices()[ Mesh2Seed1 * 3 + i ] ].Position;
                            if( Position.SquaredDistance( FirstSeg.PointA ) > 1e-6 && Position.SquaredDistance( FirstSeg.PointB ) > 1e-6 ) {
                                vMesh2 = Position;
                                nMesh2 = NewMesh2.GetVertices()[ NewMesh2.GetIndices()[ Mesh2Seed1 * 3 + i ] ].Normal;
                                break;
                            }
                        }

                        Boole M2S1InsideM1 = ( nMesh1.DotProduct( vMesh2 - FirstSeg.PointA ) < 0 );
                        Boole M1S1InsideM2 = ( nMesh2.DotProduct( vMesh1 - FirstSeg.PointA ) < 0 );

                        _RemoveFromTriLookup( Mesh1Seed1, TriLookup1 );
                        _RemoveFromTriLookup( Mesh2Seed1, TriLookup2 );
                        _RemoveFromTriLookup( Mesh1Seed2, TriLookup1 );
                        _RemoveFromTriLookup( Mesh2Seed2, TriLookup2 );

                        // Recursively add all neighbours of these triangles
                        // Stop when a contour is touched
                        switch( this->BoolOp )
                        {
                            case BO_Union:
                            {
                                if( M1S1InsideM2 ) {
                                    _RecursiveAddNeighbour(Buffer,NewMesh1,Mesh1Seed2,TriLookup1,Limits,false);
                                }else{
                                    _RecursiveAddNeighbour(Buffer,NewMesh1,Mesh1Seed1,TriLookup1,Limits,false);
                                }
                                if( M2S1InsideM1 ) {
                                    _RecursiveAddNeighbour(Buffer,NewMesh2,Mesh2Seed2,TriLookup2,Limits,false);
                                }else{
                                    _RecursiveAddNeighbour(Buffer,NewMesh2,Mesh2Seed1,TriLookup2,Limits,false);
                                }
                                break;
                            }
                            case BO_Intersection:
                            {
                                if( M1S1InsideM2 ) {
                                    _RecursiveAddNeighbour(Buffer,NewMesh1,Mesh1Seed1,TriLookup1,Limits,false);
                                }else{
                                    _RecursiveAddNeighbour(Buffer,NewMesh1,Mesh1Seed2,TriLookup1,Limits,false);
                                }
                                if( M2S1InsideM1 ) {
                                    _RecursiveAddNeighbour(Buffer,NewMesh2,Mesh2Seed1,TriLookup2,Limits,false);
                                }else{
                                    _RecursiveAddNeighbour(Buffer,NewMesh2,Mesh2Seed2,TriLookup2,Limits,false);
                                }
                                break;
                            }
                            case BO_Difference:
                            {
                                if( M1S1InsideM2 ) {
                                    _RecursiveAddNeighbour(Buffer,NewMesh1,Mesh1Seed2,TriLookup1,Limits,false);
                                }else{
                                    _RecursiveAddNeighbour(Buffer,NewMesh1,Mesh1Seed1,TriLookup1,Limits,false);
                                }
                                if( M2S1InsideM1 ) {
                                    _RecursiveAddNeighbour(Buffer,NewMesh2,Mesh2Seed1,TriLookup2,Limits,true);
                                }else{
                                    _RecursiveAddNeighbour(Buffer,NewMesh2,Mesh2Seed2,TriLookup2,Limits,true);
                                }
                                break;
                            }
                        }
                    }
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            BooleanGenerator& BooleanGenerator::SetFirstBuffer(TriangleBuffer* First)
            {
                this->FirstBuffer = First;
                return *this;
            }

            BooleanGenerator& BooleanGenerator::SetSecondBuffer(TriangleBuffer* Second)
            {
                this->SecondBuffer = Second;
                return *this;
            }

            BooleanGenerator& BooleanGenerator::SetBooleanOperation(const BooleanOperation Op)
            {
                this->BoolOp = Op;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
