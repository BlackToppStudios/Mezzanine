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
#ifndef _graphicsprocedurallathe_cpp
#define _graphicsprocedurallathe_cpp

#include "Graphics/Procedural/Mesh/lathe.h"

//#include "Graphics/Procedural/proceduraldatatypes.h"
#include "Graphics/Procedural/shape.h"
#include "Graphics/Procedural/multishape.h"
#include "Graphics/Procedural/triangulator.h"

#include "MathTools/constants.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            Lathe::Lathe() :
                ShapeToLathe(NULL),
                MultiShapeToLathe(NULL),
                NumSegments(16),
                AngleBegin(0),
                AngleEnd(MathTools::GetTwoPi()),
                Closed(true),
                Capped(true)
                {  }

            Lathe::Lathe(Shape* ToLathe, const Whole Segments) :
                ShapeToLathe(ToLathe),
                MultiShapeToLathe(NULL),
                NumSegments(Segments > 2 ? Segments : 3),
                AngleBegin(0),
                AngleEnd(MathTools::GetTwoPi()),
                Closed(true),
                Capped(true)
                {  }

            Lathe::Lathe(MultiShape* ToLathe, const Whole Segments) :
                ShapeToLathe(NULL),
                MultiShapeToLathe(ToLathe),
                NumSegments(Segments > 2 ? Segments : 3),
                AngleBegin(0),
                AngleEnd(MathTools::GetTwoPi()),
                Closed(true),
                Capped(true)
                {  }

            Lathe::~Lathe()
                {  }

            void Lathe::GenerateCaps(TriangleBuffer& Buffer) const
            {
                IndexContainer Indexes;
                Point2DContainer Points;

                Buffer.RebaseOffset();

                Triangulator Tri;
                Shape ShapeCopy;
                MultiShape MultishapeCopy;

                if( this->ShapeToLathe ) {
                    ShapeCopy = *this->ShapeToLathe;
                    ShapeCopy.Close();
                    Tri.SetShapeToTriangulate(&ShapeCopy);
                }else{
                    MultishapeCopy = *this->MultiShapeToLathe;
                    MultishapeCopy.Close();
                    Tri.SetMultiShapeToTriangulate(this->MultiShapeToLathe);
                }
                Tri.Triangulate(Indexes,Points);
                Buffer.EstimateIndexCount(2*Indexes.size());
                Buffer.EstimateVertexCount(2*Points.size());

                // begin cap
                Buffer.RebaseOffset();
                Quaternion Quat;
                Quat.SetFromAxisAngle(this->AngleBegin,Vector3::Unit_Y());
                for( Whole CurrPoint = 0 ; CurrPoint < Points.size() ; ++CurrPoint )
                {
                    Vector2 Point2D = Points[CurrPoint];
                    Vector3 Point3D(Point2D.X,Point2D.Y,0);
                    Vector3 Normal = Vector3::Unit_Z();

                    this->AddPoint(Buffer, Quat * Point3D, Quat * Normal, Point2D);
                }

                for( Whole CurrIndex = 0 ; CurrIndex < Indexes.size() / 3 ; ++CurrIndex )
                {
                    Buffer.AddIndex( Indexes[CurrIndex*3] );
                    Buffer.AddIndex( Indexes[CurrIndex*3+1] );
                    Buffer.AddIndex( Indexes[CurrIndex*3+2] );
                }
                // end cap
                Buffer.RebaseOffset();
                Quat.SetFromAxisAngle(this->AngleEnd,Vector3::Unit_Y());
                for( Whole CurrPoint = 0 ; CurrPoint < Points.size() ; ++CurrPoint )
                {
                    Vector2 Point2D = Points[CurrPoint];
                    Vector3 Point3D(Point2D.X,Point2D.Y,0);
                    Vector3 Normal = Vector3::Neg_Unit_Z();

                    this->AddPoint(Buffer, Quat * Point3D, Quat * Normal, Point2D);
                }

                for( Whole CurrIndex = 0 ; CurrIndex < Indexes.size() / 3 ; ++CurrIndex )
                {
                    Buffer.AddIndex( Indexes[CurrIndex*3] );
                    Buffer.AddIndex( Indexes[CurrIndex*3+2] );
                    Buffer.AddIndex( Indexes[CurrIndex*3+1] );
                }
            }

            void Lathe::GenerateBody(TriangleBuffer& Buffer, const Shape* ToLathe) const
            {
                if( ToLathe == NULL ) {
                    MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Invalid shape detected when performing a Lathe.");
                }
                Integer NumShapeSegments = ToLathe->GetSegCount();
                if( this->NumSegments < 3 ) {
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"The number of segments when Lathe'ing a shape cannot be less than 3.");
                }
                Integer Offset = 0;

                //Integer NumLatheSegments = this->Closed ? this->NumSegments + 1 : this->NumSegments;
                Integer NumLatheSegments = this->NumSegments + 1;
                Buffer.RebaseOffset();
                Buffer.EstimateIndexCount( NumLatheSegments * NumShapeSegments * 6 );
                Buffer.EstimateVertexCount( ( NumShapeSegments + 1 ) * ( NumLatheSegments + 1 ) );

                Real TempEnd = this->AngleEnd;
                if( this->AngleBegin > this->AngleEnd ) {
                    TempEnd += MathTools::GetTwoPi();
                }

                for( Integer LatheSegIndex = 0 ; LatheSegIndex < NumLatheSegments ; ++LatheSegIndex )
                {
                    Real Angle = 0;
                    if( this->Closed ) {
                        Angle = LatheSegIndex / static_cast<Real>( this->NumSegments ) * MathTools::GetTwoPi();
                    }else{
                        Angle = this->AngleBegin + LatheSegIndex / static_cast<Real>( this->NumSegments ) * ( TempEnd - this->AngleBegin );
                    }
                    Quaternion Quat;
                    Quat.SetFromAxisAngle(Angle,Vector3::Unit_Y());

                    for( Integer ShapeSegIndex = 0 ; ShapeSegIndex <= NumShapeSegments ; ++ShapeSegIndex )
                    {
                        const Vector2& SegStart = ToLathe->GetPoint(ShapeSegIndex);
                        const Vector2& SegDirection = ToLathe->GetAvgDirection(ShapeSegIndex);
                        Vector2 SegNormal = SegDirection.Perpendicular();
                        Vector3 VertPos(SegStart.X,SegStart.Y,0);
                        Vector3 Normal3D(SegNormal.X,SegNormal.Y,0);
                        Normal3D.Normalize();
                        if( ToLathe->GetOutSide() == Procedural::SS_Right ) {
                            Normal3D = -Normal3D;
                        }

                        this->AddPoint(Buffer, Quat * VertPos, Quat * Normal3D, Vector2( LatheSegIndex / static_cast<Real>( this->NumSegments ), ShapeSegIndex / static_cast<Real>( NumShapeSegments ) ) );

                        if( ShapeSegIndex < NumShapeSegments && LatheSegIndex < NumLatheSegments - 1 ) {
                            if( ToLathe->GetOutSide() == Procedural::SS_Right ) {
                                Buffer.AddTriangle(Offset + NumShapeSegments + 2, Offset, Offset + NumShapeSegments + 1);
                                Buffer.AddTriangle(Offset + NumShapeSegments + 2, Offset + 1, Offset);
                            }else{
                                Buffer.AddTriangle(Offset + NumShapeSegments + 2, Offset + NumShapeSegments + 1, Offset);
                                Buffer.AddTriangle(Offset + NumShapeSegments + 2, Offset, Offset + 1);
                            }
                        }
                        Offset++;
                    }
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void Lathe::AddToTriangleBuffer(TriangleBuffer& Buffer) const
            {
                if( this->ShapeToLathe == NULL && this->MultiShapeToLathe == NULL ) {
                    MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"A shape or multishape must be defined in order to perform a lathe.");
                }

                // Triangulate the begin and end caps
                if( !this->Closed && this->Capped ) {
                    this->GenerateCaps(Buffer);
                }

                // Extrudes the body
                if( this->ShapeToLathe ) {
                    this->GenerateBody(Buffer,this->ShapeToLathe);
                }else{
                    for( Whole Index = 0 ; Index < this->MultiShapeToLathe->GetNumShapes() ; ++Index )
                    {
                        this->GenerateBody(Buffer,&this->MultiShapeToLathe->GetShape(Index));
                    }
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            Lathe& Lathe::SetShapeToLathe(Shape* ToLathe)
            {
                this->ShapeToLathe = ToLathe;
                this->MultiShapeToLathe = NULL;
                return *this;
            }

            Lathe& Lathe::SetMultiShapeToLathe(MultiShape* ToLathe)
            {
                this->ShapeToLathe = NULL;
                this->MultiShapeToLathe = ToLathe;
                return *this;
            }

            Lathe& Lathe::SetNumSegments(const Whole Segments)
            {
                if( Segments < 3 ) {
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"The number of segments when Lathe'ing a shape cannot be less than 3.");
                }
                this->NumSegments = Segments;
                return *this;
            }

            Lathe& Lathe::SetAngleBegin(const Real Begin)
            {
                this->AngleBegin = Begin;
                this->Closed = false;
                return *this;
            }

            Lathe& Lathe::SetAngleEnd(const Real End)
            {
                this->AngleEnd = End;
                this->Closed = false;
                return *this;
            }

            Lathe& Lathe::SetClosed(const Boole ShapeClosed)
            {
                this->Closed = ShapeClosed;
                return *this;
            }

            Lathe& Lathe::SetCapped(const Boole ShapeCapped)
            {
                this->Capped = ShapeCapped;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif

