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
#ifndef _graphicsproceduralpath_cpp
#define _graphicsproceduralpath_cpp

#include "Graphics/Procedural/path.h"
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
            Path::Path() :
                Closed(false)
                {  }

            Path::Path(Point3DIterator Begin, Point3DIterator End) :
                Points(Begin,End),
                Closed(false)
                {  }

            Path::Path(const Point3DContainer& DataSet) :
                Points(DataSet),
                Closed(false)
                {  }

            Path::~Path()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void Path::BuildFromSegmentSoup(const LineSeg3DVec& Segments, PathContainer& GeneratedPaths)
            {
                typedef std::multimap<Vector3,Vector3,Vector3LengthCompare> Vec3MultiMap;
                typedef Vec3MultiMap::iterator Vec3MapIterator;
                typedef std::pair<Vec3MapIterator, Vec3MapIterator> Vec3MapRange;
                typedef std::pair<Vector3,Vector3> Vec3Pair;

                Vec3MultiMap SegmentMap;
                for( ConstLineSeg3DIterator SegIt = Segments.begin() ; SegIt != Segments.end() ; ++SegIt )
                {
                    SegmentMap.insert( Vec3Pair( SegIt->PointA, SegIt->PointB ) );
                    SegmentMap.insert( Vec3Pair( SegIt->PointB, SegIt->PointA ) );
                }
                while( !SegmentMap.empty() )
                {
                    Vector3 HeadFirst = SegmentMap.begin()->first;
                    Vector3 HeadSecond = SegmentMap.begin()->second;
                    Path CurrPath;
                    CurrPath.AddPoint(HeadFirst);
                    CurrPath.AddPoint(HeadSecond);
                    Vec3MapIterator FirstSeg = SegmentMap.begin();
                    Vec3MapRange Correspondants2 = SegmentMap.equal_range(HeadSecond);
                    for( Vec3MapIterator CorrIt = Correspondants2.first ; CorrIt != Correspondants2.second ;  )
                    {
                        Vec3MapIterator RemoveIt = CorrIt++;
                        if( ( RemoveIt->second - FirstSeg->first ).SquaredLength() < 1e-8 ) {
                            SegmentMap.erase(RemoveIt);
                        }
                    }
                    SegmentMap.erase(FirstSeg);
                    Boole FoundSomething = true;
                    while( !SegmentMap.empty() && FoundSomething )
                    {
                        FoundSomething = false;
                        Vec3MapIterator Next = SegmentMap.find(HeadSecond);
                        if( Next != SegmentMap.end() ) {
                            FoundSomething = true;
                            HeadSecond = Next->second;
                            CurrPath.AddPoint(HeadSecond);
                            Vec3MapRange Correspondants = SegmentMap.equal_range(HeadSecond);
                            for( Vec3MapIterator CorrIt = Correspondants.first ; CorrIt != Correspondants.second ;  )
                            {
                                Vec3MapIterator RemoveIt = CorrIt++;
                                if( ( RemoveIt->second - Next->first ).SquaredLength() < 1e-8 ) {
                                    SegmentMap.erase(RemoveIt);
                                }
                            }
                            SegmentMap.erase(Next);
                        }
                        Vec3MapIterator Previous = SegmentMap.find(HeadFirst);
                        if( Previous != SegmentMap.end() ) {
                            FoundSomething = true;
                            CurrPath.InsertPoint(0, Previous->second);
                            HeadFirst = Previous->second;
                            Vec3MapRange Correspondants = SegmentMap.equal_range(HeadFirst);
                            for( Vec3MapIterator CorrIt = Correspondants.first ; CorrIt != Correspondants.second ;  )
                            {
                                Vec3MapIterator RemoveIt = CorrIt++;
                                if( ( RemoveIt->second - Previous->first ).SquaredLength() < 1e-8 ) {
                                    SegmentMap.erase(RemoveIt);
                                }
                            }
                            SegmentMap.erase(Previous);
                        }
                    }
                    // This code section is only relevant if closing of a path could be toggled without changing types.
                    if( CurrPath.GetPoint(0).SquaredDistance( CurrPath.GetPoint( CurrPath.GetSegCount() + 1 ) ) < 1e-6 ) {
                        CurrPath.GetPoints().pop_back();
                        CurrPath.Close();
                    }
                    GeneratedPaths.push_back(CurrPath);
                }
            }

            Mesh* Path::GenerateMesh(const String& Name, const String& Group) const
            {
                Ogre::ManualObject* TempMan = new Ogre::ManualObject("TempMan");
                TempMan->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OT_LINE_STRIP);

                for( ConstPoint3DIterator PointIt = this->Points.begin() ; PointIt != this->Points.end() ; ++PointIt )
                    { TempMan->position( (*PointIt).GetOgreVector3() ); }

                TempMan->end();
                Mesh* NewMesh = MeshManager::GetSingletonPtr()->_WrapInternalMesh( TempMan->convertToMesh(Name,Group) );
                delete TempMan;
                return NewMesh;
            }

            Path& Path::AppendPath(const Path& Other)
            {
                this->Points.insert( this->Points.end(), Other.Points.begin(), Other.Points.end() );
                return *this;
            }

            Path& Path::AppendPathRel(const Path& Other)
            {
                if( this->Points.empty() ) {
                    this->AppendPath(Other);
                }else{
                    Vector3 refVector = *( this->Points.end() - 1 );
                    Point3DContainer Points( Other.Points.begin(), Other.Points.end() );
                    for( Point3DIterator PointIt = Points.begin() ; PointIt != Points.end() ; ++PointIt )
                        { (*PointIt) += refVector; }
                    this->Points.insert( this->Points.end(), Points.begin(), Points.end() );
                }
                return *this;
            }

            Path Path::ExtractSubPath(const Whole First, const Whole Last)
            {
                Path Ret;
                for( Whole Index = First ; Index < Last ; ++Index )
                    { Ret.AddPoint( this->Points[Index] ); }
                return Ret;
            }

            Integer Path::GetSegCount() const
            {
                return ( this->Points.size() - 1 ) + ( this->Closed ? 1 : 0 );
            }

            Real Path::GetTotalLength() const
            {
                Real Length = 0;
                for( Whole Index = 0 ; Index < this->Points.size() ; ++Index )
                    { Length += ( this->Points[ Index + 1 ] - this->Points[ Index ] ).Length(); }
                if( this->Closed ) {
                    Length += ( this->Points.back() - *(this->Points.begin()) ).Length();
                }
                return Length;
            }

            Real Path::GetLengthAtPoint(const Whole PointIndex) const
            {
                Real Length = 0;
                for( Whole Index = 0 ; Index < PointIndex ; ++Index )
                    { Length += ( this->Points[ Index + 1 ] - this->Points[ Index ] ).Length(); }
                if( this->Closed ) {
                    Length += ( this->Points.back() - *(this->Points.begin()) ).Length();
                }
                return Length;
            }

            Path& Path::Reflect(const Vector3& Normal)
            {
                for( Point3DIterator PointIt = this->Points.begin() ; PointIt != this->Points.end() ; ++PointIt )
                    { (*PointIt) = PointIt->Reflect(Normal); }
                return *this;
            }

            Path& Path::Reverse()
            {
                std::reverse( this->Points.begin(), this->Points.end() );
                return *this;
            }

            Path& Path::Close()
            {
                this->Closed = true;
                return *this;
            }

            Boole Path::IsClosed() const
            {
                return this->Closed;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Point Management

            Path& Path::AddPoint(const Vector3& ToAdd)
            {
                this->Points.push_back(ToAdd);
                return *this;
            }

            Path& Path::AddPoint(const Real X, const Real Y, const Real Z)
            {
                this->Points.push_back(Vector3(X,Y,Z));
                return *this;
            }

            Path& Path::InsertPoint(const Whole Index, const Real X, const Real Y, const Real Z)
            {
                this->Points.insert( this->Points.begin() + Index, Vector3(X,Y,Z) );
                return *this;
            }

            Path& Path::InsertPoint(const Whole Index, const Vector3& Point)
            {
                this->Points.insert( this->Points.begin() + Index, Point );
                return *this;
            }

            const Vector3& Path::GetPoint(const Integer Index) const
            {
                if( this->Closed ) {
                    return this->Points.at( MathTools::WrappedModulo(Index, this->Points.size()) );
                }else{
                    return this->Points.at( MathTools::Clamp( Index, 0, Integer(this->Points.size()) - 1 ) );
                }
            }

            Point3DContainer& Path::GetPoints()
                { return this->Points; }

            const Point3DContainer& Path::GetPoints() const
                { return this->Points; }

            ///////////////////////////////////////////////////////////////////////////////
            // Transform

            Path& Path::Translate(const Vector3& Trans)
            {
                for( Point3DIterator PointIt = this->Points.begin() ; PointIt != this->Points.end() ; ++PointIt )
                    { (*PointIt) += Trans; }
                return *this;
            }

            Path& Path::Translate(const Real TransX, const Real TransY, const Real TransZ)
            {
                return this->Translate( Vector3(TransX,TransY,TransZ) );
            }

            Path& Path::Scale(const Real Scaling)
            {
                return this->Scale(Scaling,Scaling,Scaling);
            }

            Path& Path::Scale(const Real ScaleX, const Real ScaleY, const Real ScaleZ)
            {
                for( Point3DIterator PointIt = this->Points.begin() ; PointIt != this->Points.end() ; ++PointIt )
                {
                    (*PointIt).X *= ScaleX;
                    (*PointIt).Y *= ScaleY;
                    (*PointIt).Z *= ScaleZ;
                }
                return *this;
            }

            Path& Path::Scale(const Vector3& Scaling)
            {
                return this->Scale(Scaling.X,Scaling.Y,Scaling.Z);
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Directions and Normals

            Vector3 Path::GetDirectionAfter(const Whole Index) const
            {
                // If the path isn't closed, we get a different calculation at the end, because
                // the tangent shall not be null
                if( !this->Closed && Index == this->Points.size() - 1 && Index > 0 ) {
                    return ( this->Points[ Index ] - this->Points[ Index - 1 ] ).GetNormal();
                }else{
                    return ( this->GetPoint( Index + 1 ) - this->GetPoint( Index ) ).GetNormal();
                }
            }

            Vector3 Path::GetDirectionBefore(const Whole Index) const
            {
                // If the path isn't closed, we get a different calculation at the end, because
                // the tangent shall not be null
                if( !this->Closed && Index == 1 ) {
                    return ( this->Points[1] - this->Points[0] ).GetNormal();
                }else{
                    return ( this->GetPoint( Index ) - this->GetPoint( Index - 1 ) ).GetNormal();
                }
            }

            Vector3 Path::GetAvgDirection(const Whole Index) const
            {
                return ( this->GetDirectionAfter(Index) + this->GetDirectionBefore(Index) ).GetNormal();
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Conversion

            Shape Path::ConvertToShape() const
            {
                Shape RetShape;
                for( ConstPoint3DIterator PointIt = this->Points.begin() ; PointIt != this->Points.end() ; ++PointIt )
                    { RetShape.AddPoint( PointIt->X, PointIt->Y ); }
                if( this->Closed ) {
                    RetShape.Close();
                }
                return RetShape;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
