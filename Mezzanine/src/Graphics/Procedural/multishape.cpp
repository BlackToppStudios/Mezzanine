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
#ifndef _graphicsproceduralmultishape_cpp
#define _graphicsproceduralmultishape_cpp

#include "Graphics/Procedural/multishape.h"
#include "Graphics/Procedural/shape.h"

#include "Graphics/meshmanager.h"

#include "MathTools/mathtools.h"

#include "Ogre.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            MultiShape::MultiShape()
                {  }

            MultiShape::MultiShape(const Shape& ToAdd)
                { this->Shapes.push_back(ToAdd); }

            MultiShape::MultiShape(const ShapeContainer& ToAdd)
                { this->Shapes.insert(this->Shapes.end(),ToAdd.begin(),ToAdd.end()); }

            MultiShape::~MultiShape()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            Mesh* MultiShape::GenerateMesh(const String& Name, const String& Group)
            {
                Ogre::ManualObject* TempMan = new Ogre::ManualObject("TempMan");

                for( ShapeIterator It = this->Shapes.begin() ; It != this->Shapes.end() ; ++It )
                {
                    TempMan->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OT_LINE_STRIP);
                    (*It)._AppendToManualObject(TempMan);
                    TempMan->end();
                }

                Mesh* NewMesh = MeshManager::GetSingletonPtr()->_WrapInternalMesh( TempMan->convertToMesh(Name,Group) );
                delete TempMan;
                return NewMesh;
            }

            void MultiShape::BuildFromSegmentSoup(const LineSegmentContainer& Segments)
            {
                typedef std::multimap<Vector2,Vector2,Vector2LengthCompare> Vec2MultiMap;
                typedef std::pair<Vector2,Vector2> Vec2Pair;

                Vec2MultiMap SegMap;
                for( ConstLineSegmentIterator SegIt = Segments.begin() ; SegIt != Segments.end() ; ++SegIt )
                {
                    SegMap.insert( Vec2Pair(SegIt->PointA,SegIt->PointB) );
                    SegMap.insert( Vec2Pair(SegIt->PointB,SegIt->PointA) );
                }
                while( !SegMap.empty() )
                {
                    Vector2 headFirst = SegMap.begin()->first;
                    Vector2 headSecond = SegMap.begin()->second;

                    Shape s;
                    s.AddPoint(headFirst).AddPoint(headSecond);

                    Vec2MultiMap::iterator firstSeg = SegMap.begin();
                    std::pair<Vec2MultiMap::iterator,Vec2MultiMap::iterator> correspondants2 = SegMap.equal_range(headSecond);

                    for( Vec2MultiMap::iterator Vec2It = correspondants2.first ; Vec2It != correspondants2.second ;  )
                    {
                        Vec2MultiMap::iterator RemoveIt = ++Vec2It;
                        if( ( RemoveIt->second - firstSeg->first ).SquaredLength() < 1e-8 )
                            SegMap.erase(RemoveIt);
                    }

                    SegMap.erase(firstSeg);
                    Boole FoundSomething = true;
                    while( !SegMap.empty() && FoundSomething )
                    {
                        FoundSomething = false;
                        Vec2MultiMap::iterator next = SegMap.find(headSecond);
                        if( next != SegMap.end() ) {
                            FoundSomething = true;
                            headSecond = next->second;
                            s.AddPoint(headSecond);
                            std::pair<Vec2MultiMap::iterator, Vec2MultiMap::iterator> correspondants = SegMap.equal_range(headSecond);
                            for( Vec2MultiMap::iterator it = correspondants.first ; it != correspondants.second ;  )
                            {
                                Vec2MultiMap::iterator RemoveIt = ++it;
                                if( ( RemoveIt->second - next->first ).SquaredLength() < 1e-8)
                                    SegMap.erase(RemoveIt);
                            }
                            SegMap.erase(next);
                        }
                        Vec2MultiMap::iterator previous = SegMap.find(headFirst);
                        if( previous != SegMap.end() ) {
                            FoundSomething = true;
                            s.InsertPoint(0, previous->second);
                            headFirst = previous->second;
                            std::pair<Vec2MultiMap::iterator,Vec2MultiMap::iterator> correspondants = SegMap.equal_range(headFirst);
                            for( Vec2MultiMap::iterator it = correspondants.first ; it != correspondants.second ;  )
                            {
                                Vec2MultiMap::iterator RemoveIt = ++it;
                                if( ( RemoveIt->second - previous->first ).SquaredLength() < 1e-8 )
                                    SegMap.erase(RemoveIt);
                            }
                            SegMap.erase(previous);
                        }
                    }
                    if( s.GetPoint(0).SquaredDistance( s.GetPoint( s.GetSegCount() + 1 ) ) < 1e-6 ) {
                        s.GetPointsReference().pop_back();
                        s.Close();
                    }
                    this->AddShape(s);
                }
            }

            Point2DContainer MultiShape::GetPoints() const
            {
                Point2DContainer Ret;
                for( Whole Index = 0 ; Index < this->Shapes.size() ; Index++ )
                {
                    Point2DContainer Points = this->Shapes[Index].GetPoints();
                    Ret.insert( Ret.end(), Points.begin(), Points.end() );
                }
                return Ret;
            }

            Boole MultiShape::IsPointInside(const Vector2& Point) const
            {
                // Draw a horizontal lines that goes through "point"
                // Using the closest intersection, find whether the point is actually inside
                Integer ClosestSegmentIndex = -1;
                Real ClosestSegmentDistance = std::numeric_limits<Real>::max();
                Vector2 ClosestSegmentIntersection;
                const Shape* ClosestSegmentShape = 0;

                for( Whole ShapeIndex = 0 ; ShapeIndex < this->Shapes.size() ; ShapeIndex++ )
                {
                    const Shape& CurrShape = this->Shapes[ShapeIndex];
                    for( Whole SegmentIndex = 0 ; SegmentIndex < CurrShape.GetSegCount() ; SegmentIndex++ )
                    {
                        Vector2 A = CurrShape.GetPoint(SegmentIndex);
                        Vector2 B = CurrShape.GetPoint(SegmentIndex + 1);
                        if( A.Y != B.Y && ( A.Y - Point.Y ) * ( B.Y - Point.Y ) <= 0.0 ) {
                            Vector2 Intersect( A.X + ( Point.Y - A.Y ) * ( B.X - A.X ) / ( B.Y - A.Y ), Point.Y );
                            Real Dist = MathTools::Abs( Point.X - Intersect.X );
                            if( Dist < ClosestSegmentDistance ) {
                                ClosestSegmentIndex = SegmentIndex;
                                ClosestSegmentDistance = Dist;
                                ClosestSegmentIntersection = Intersect;
                                ClosestSegmentShape = &CurrShape;
                            }
                        }
                    }
                }
                if( ClosestSegmentIndex != -1 ) {
                    Integer EdgePoint = -1;
                    if( ClosestSegmentIntersection.SquaredDistance( ClosestSegmentShape->GetPoint(ClosestSegmentIndex) ) < 1e-8 ) {
                        EdgePoint = ClosestSegmentIndex;
                    }else if( ClosestSegmentIntersection.SquaredDistance( ClosestSegmentShape->GetPoint(ClosestSegmentIndex + 1) ) < 1e-8) {
                        EdgePoint = ClosestSegmentIndex + 1;
                    }
                    if( EdgePoint > -1 ) {
                        Real Alpha1 = ( Point - ClosestSegmentShape->GetPoint(EdgePoint) ).AngleBetween( ClosestSegmentShape->GetDirectionAfter(EdgePoint) );
                        Real Alpha2 = ( Point - ClosestSegmentShape->GetPoint(EdgePoint) ).AngleBetween( -ClosestSegmentShape->GetDirectionBefore(EdgePoint) );
                        if( Alpha1 < Alpha2 ) {
                            ClosestSegmentIndex = EdgePoint;
                        }else{
                            ClosestSegmentIndex = EdgePoint - 1;
                        }
                    }
                    return ( ClosestSegmentShape->GetNormalAfter(ClosestSegmentIndex).X * ( Point.X - ClosestSegmentIntersection.X ) < 0 );
                }
                // We're in the case where the point is on the "real outside" of the multishape
                // So, if the real outside == user defined outside, then the point is "user-defined outside"
                return !this->IsOutsideRealOutside();
            }

            Boole MultiShape::IsOutsideRealOutside() const
            {
                Real X = std::numeric_limits<Real>::min();
                Integer PointIndex = 0;
                Integer ShapeIndex = 0;
                for( Whole SIndex = 0 ; SIndex < this->Shapes.size() ; SIndex++ )
                {
                    const Shape& CurrShape = this->Shapes[SIndex];
                    const std::vector<Vector2>& Points = CurrShape.GetPointsReference();
                    for( Whole PIndex = 0 ; PIndex < Points.size() ; PIndex++ )
                    {
                        if( X < Points[PIndex].X ) {
                            X = Points[PIndex].X;
                            PointIndex = PIndex;
                            ShapeIndex = SIndex;
                        }
                    }
                }
                Real Alpha1 = Vector2::Unit_Y().AngleTo( this->Shapes[ShapeIndex].GetDirectionAfter(PointIndex) );
                Real Alpha2 = Vector2::Unit_Y().AngleTo( -this->Shapes[ShapeIndex].GetDirectionBefore(PointIndex) );
                Procedural::ShapeSide Side;
                if( Alpha1 < Alpha2 ) {
                    Side = Procedural::SS_Right;
                }else{
                    Side = Procedural::SS_Left;
                }
                return ( Side == this->Shapes[ShapeIndex].GetOutSide() );
            }

            MultiShape& MultiShape::Close()
            {
                for( ShapeIterator It = this->Shapes.begin() ; It != this->Shapes.end(); ++It )
                    { It->Close(); }
                return *this;
            }

            Boole MultiShape::IsClosed() const
            {
                for( ConstShapeIterator It = this->Shapes.begin() ; It != this->Shapes.end() ; ++It )
                {
                    if( !It->IsClosed() )
                        return false;
                }
                return true;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Shape Management

            MultiShape& MultiShape::AddShape(const Shape& ToAdd)
            {
                this->Shapes.push_back(ToAdd);
                return *this;
            }

            MultiShape& MultiShape::AddMultiShape(const MultiShape& Other)
            {
                for( ConstShapeIterator it = Other.Shapes.begin() ; it != Other.Shapes.end() ; ++it )
                    { this->Shapes.push_back(*it); }
                return *this;
            }

            Shape& MultiShape::GetShape(const Whole Index)
                { return this->Shapes[Index]; }

            const Shape& MultiShape::GetShape(const Whole Index) const
                { return this->Shapes[Index]; }

            Whole MultiShape::GetNumShapes() const
                { return this->Shapes.size(); }

            void MultiShape::RemoveAllShapes()
                { this->Shapes.clear(); }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
