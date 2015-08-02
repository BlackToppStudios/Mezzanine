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
#ifndef _graphicsproceduralicospheregenerator_cpp
#define _graphicsproceduralicospheregenerator_cpp

#include "Graphics/Procedural/Mesh/icospheregenerator.h"

#include "MathTools/mathtools.h"
#include "exception.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            IcoSphereGenerator::IcoSphereGenerator(const Real Radius, const Whole Iterations) :
                SphereRadius(Radius),
                NumIterations(Iterations)
                {  }

            IcoSphereGenerator::~IcoSphereGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void IcoSphereGenerator::AddToTriangleBuffer(TriangleBuffer& Buffer) const
            {
                std::vector<Vector3> Vertices;
                Integer Offset = 0;

                /// Step 1 : Generate icosahedron
                Real phi = .5f * ( 1.f + sqrt(5.f) );
                Real invnorm = 1 / sqrt( phi * phi + 1 );

                Vertices.push_back( Vector3(-1,  phi, 0) * invnorm );//0
                Vertices.push_back( Vector3( 1,  phi, 0) * invnorm );//1
                Vertices.push_back( Vector3(0,   1,  -phi) * invnorm );//2
                Vertices.push_back( Vector3(0,   1,   phi) * invnorm );//3
                Vertices.push_back( Vector3(-phi,0,  -1) * invnorm );//4
                Vertices.push_back( Vector3(-phi,0,   1) * invnorm );//5
                Vertices.push_back( Vector3( phi,0,  -1) * invnorm );//6
                Vertices.push_back( Vector3( phi,0,   1) * invnorm );//7
                Vertices.push_back( Vector3(0,   -1, -phi) * invnorm );//8
                Vertices.push_back( Vector3(0,   -1,  phi) * invnorm );//9
                Vertices.push_back( Vector3(-1,  -phi,0) * invnorm );//10
                Vertices.push_back( Vector3( 1,  -phi,0) * invnorm );//11

                Integer FirstFaces[] = {0,1,2,
                                        0,3,1,
                                        0,4,5,
                                        1,7,6,
                                        1,6,2,
                                        1,3,7,
                                        0,2,4,
                                        0,5,3,
                                        2,6,8,
                                        2,8,4,
                                        3,5,9,
                                        3,9,7,
                                        11,6,7,
                                        10,5,4,
                                        10,4,8,
                                        10,9,5,
                                        11,8,6,
                                        11,7,9,
                                        10,8,11,
                                        10,11,9};

                std::vector<Integer> Faces(FirstFaces, FirstFaces + sizeof(FirstFaces) / sizeof(*FirstFaces) );
                Integer Size = 60;

                /// Step 2 : tessellate
                for( Whole Iteration = 0 ; Iteration < this->NumIterations ; ++Iteration )
                {
                    Size *= 4;
                    std::vector<Integer> NewFaces;
                    NewFaces.clear();
                    for( Integer Index = 0 ; Index < Size / 12 ; ++Index )
                    {
                        Integer i1 = Faces[ Index * 3 ];
                        Integer i2 = Faces[ Index * 3 + 1 ];
                        Integer i3 = Faces[ Index * 3 + 2 ];
                        Integer i12 = Vertices.size();
                        Integer i23 = i12 + 1;
                        Integer i13 = i12 + 2;
                        Vector3 v1 = Vertices[ i1 ];
                        Vector3 v2 = Vertices[ i2 ];
                        Vector3 v3 = Vertices[ i3 ];
                        //make 1 vertice at the center of each edge and project it onto the sphere
                        Vertices.push_back( ( v1 + v2 ).GetNormal() );
                        Vertices.push_back( ( v2 + v3 ).GetNormal() );
                        Vertices.push_back( ( v1 + v3 ).GetNormal() );
                        //now recreate indices
                        NewFaces.push_back(i1);
                        NewFaces.push_back(i12);
                        NewFaces.push_back(i13);
                        NewFaces.push_back(i2);
                        NewFaces.push_back(i23);
                        NewFaces.push_back(i12);
                        NewFaces.push_back(i3);
                        NewFaces.push_back(i13);
                        NewFaces.push_back(i23);
                        NewFaces.push_back(i12);
                        NewFaces.push_back(i23);
                        NewFaces.push_back(i13);
                    }
                    Faces.swap(NewFaces);
                }

                /// Step 3 : generate texcoords
                std::vector<Vector2> TexCoords;
                for( UInt16 Index = 0 ; Index < Vertices.size() ; Index++ )
                {
                    const Vector3& vec = Vertices[Index];
                    Real u, v;
                    Real r0 = sqrtf( vec.X * vec.X + vec.Z * vec.Z );
                    Real alpha;
                    alpha = atan2f(vec.Z,vec.X);
                    u = alpha / MathTools::GetTwoPi() + .5f;
                    v = atan2f(vec.Y, r0) / MathTools::GetPi() + .5f;
                    TexCoords.push_back( Vector2(u,v) );
                }

                /// Step 4 : fix texcoords
                // find Vertices to split
                std::vector<Integer> IndexToSplit;
                for( Whole Index = 0 ; Index < Faces.size() / 3 ; ++Index )
                {
                    Vector2& t0 = TexCoords[ Faces[ Index * 3 + 0 ] ];
                    Vector2& t1 = TexCoords[ Faces[ Index * 3 + 1 ] ];
                    Vector2& t2 = TexCoords[ Faces[ Index * 3 + 2 ] ];
                    if( MathTools::Abs( t2.X - t0.X ) > 0.5 ) {
                        if( t0.X < 0.5 ) {
                            IndexToSplit.push_back( Faces[ Index * 3 ] );
                        }else{
                            IndexToSplit.push_back( Faces[ Index * 3 + 2 ] );
                        }
                    }
                    if( MathTools::Abs( t1.X - t0.X ) > 0.5 ) {
                        if( t0.X < 0.5 ) {
                            IndexToSplit.push_back( Faces[ Index * 3 ] );
                        }else{
                            IndexToSplit.push_back( Faces[ Index * 3 + 1 ] );
                        }
                    }
                    if( MathTools::Abs( t2.X - t1.X ) > 0.5 ) {
                        if( t1.X < 0.5 ) {
                            IndexToSplit.push_back( Faces[ Index * 3 + 1 ] );
                        }else{
                            IndexToSplit.push_back( Faces[ Index * 3 + 2 ] );
                        }
                    }
                }

                //split Vertices
                for( UInt16 Split = 0 ; Split < IndexToSplit.size() ; Split++ )
                {
                    Integer Index = IndexToSplit[Split];
                    //duplicate vertex
                    Vector3 v = Vertices[Index];
                    Vector2 t = TexCoords[Index] + Vector2(1.0,0.0);
                    Vertices.push_back(v);
                    TexCoords.push_back(t);
                    Integer NewIndex = Vertices.size() - 1;
                    //reassign indices
                    for( UInt16 j = 0; j < Faces.size() ; j++ )
                    {
                        if( Faces[j] == Index ) {
                            Integer index1 = Faces[ ( j + 1 ) % 3 + ( j / 3 ) * 3 ];
                            Integer index2 = Faces[ ( j + 2 ) % 3 + ( j / 3 ) * 3 ];
                            if( ( TexCoords[index1].X > 0.5 ) || ( TexCoords[index2].X > 0.5 ) ) {
                                Faces[j] = NewIndex;
                            }
                        }
                    }
                }

                /// Step 5 : realize
                Buffer.RebaseOffset();
                Buffer.EstimateVertexCount( Vertices.size() );
                Buffer.EstimateIndexCount(Size);

                for( UInt16 Index = 0 ; Index < Vertices.size() ; ++Index )
                {
                    this->AddPoint(Buffer, Vertices[Index] * this->SphereRadius,
                                   Vertices[Index],//note : Vertices are already normalised
                                   Vector2( TexCoords[Index].X, TexCoords[Index].Y ) );
                }
                for( UInt16 Index = 0 ; Index < Size ; ++Index )
                {
                    Buffer.AddIndex( Offset + Faces[Index] );
                }
                Offset += Vertices.size();
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            IcoSphereGenerator& IcoSphereGenerator::SetRadius(const Real Radius)
            {
                if( Radius <= 0.0 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Radius for a generated ico-sphere mesh must be greater than zero.");

                this->SphereRadius = Radius;
                return *this;
            }

            IcoSphereGenerator& IcoSphereGenerator::SetNumIterations(const Whole Iterations)
            {
                if( Iterations == 0 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Number of Iterations for generating ico-sphere mesh much be greater than zero.");

                this->NumIterations = Iterations;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
