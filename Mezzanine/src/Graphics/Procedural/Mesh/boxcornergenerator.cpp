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
#ifndef _graphicsproceduralboxcornergenerator_cpp
#define _graphicsproceduralboxcornergenerator_cpp

#include "Graphics/Procedural/Mesh/boxcornergenerator.h"

#include "exception.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            BoxCornerGenerator::BoxCornerGenerator(const Vector3& Size, const Real CornerThickness)
            {
                this->SetHalfExtents(Size);
                this->SetCornerThickness(CornerThickness);
            }

            BoxCornerGenerator::BoxCornerGenerator(const Real HalfSizeX, const Real HalfSizeY, const Real HalfSizeZ, const Real CornerThickness)
            {
                this->SetHalfSizeX(HalfSizeX);
                this->SetHalfSizeY(HalfSizeY);
                this->SetHalfSizeZ(HalfSizeZ);
                this->SetCornerThickness(CornerThickness);
            }

            BoxCornerGenerator::~BoxCornerGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void BoxCornerGenerator::AddToTriangleBuffer(TriangleBuffer& Buffer) const
            {
                Vector3 Half = this->BoxHalf;
                Vector3 Full = this->BoxHalf * 2;

                Real Length = 0.0;
                Real FirstSizeCheck = Half .X < Half .Y ? Half .X : Half .Y;
                Real Smallest = FirstSizeCheck < Half .Z ? FirstSizeCheck : Half .Z;
                if( Smallest * 2 <= FirstSizeCheck ) {
                    Length = Smallest * 0.5;
                }else{
                    Length = Smallest * 0.25;
                }
                Buffer.RebaseOffset();
                Buffer.EstimateVertexCount( 54 * 8 );  //Verts per corner times the number of corners
                Buffer.EstimateIndexCount( 54 * 8 );  //Verts per corner times the number of corners

                // Create a list of data we can play around with
                std::vector<Procedural::Vertex> BoxVertices;
                //Vertex                                                                                                           //Normal             //TextureCoord
                // Forward Face // 0 /
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y,Half.Z),                                                     Vector3(0,0,1),      Vector2(0,0) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y - Length,Half.Z),                                            Vector3(0,0,1),      Vector2(0,0.5) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - Length,Half.Z),                           Vector3(0,0,1),      Vector2(this->BoxThick / Full.X,0.5) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - this->BoxThick,Half.Z),                   Vector3(0,0,1),      Vector2(this->BoxThick / Full.X,this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + Length,Half.Y - this->BoxThick,Half.Z),                           Vector3(0,0,1),      Vector2(0.5,this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + Length,Half.Y,Half.Z),                                            Vector3(0,0,1),      Vector2(0.5,0) ) );
                // Upward Face // 6 */
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y,Half.Z),                                                     Vector3(0,1,0),      Vector2(0,1) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + Length,Half.Y,Half.Z),                                            Vector3(0,1,0),      Vector2(0.5,1) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + Length,Half.Y,Half.Z - this->BoxThick),                           Vector3(0,1,0),      Vector2(0.5,1 - (this->BoxThick / Full.Y) ) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y,Half.Z - this->BoxThick),                   Vector3(0,1,0),      Vector2(this->BoxThick / Full.X,1 - (this->BoxThick / Full.Y) ) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y,Half.Z - Length),                           Vector3(0,1,0),      Vector2(this->BoxThick / Full.X,0.5) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y,Half.Z - Length),                                            Vector3(0,1,0),      Vector2(0,0.5) ) );
                // Left Face // 12 */
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y,Half.Z),                                                     Vector3(-1,0,0),     Vector2(1,0) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y,Half.Z - Length),                                            Vector3(-1,0,0),     Vector2(0.5,0) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y - this->BoxThick,Half.Z - Length),                           Vector3(-1,0,0),     Vector2(0.5,this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y - this->BoxThick,Half.Z - this->BoxThick),                   Vector3(-1,0,0),     Vector2(1 - (this->BoxThick / Full.X),this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y - Length,Half.Z - this->BoxThick),                           Vector3(-1,0,0),     Vector2(1 - (this->BoxThick / Full.X),0.5) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y - Length,Half.Z),                                            Vector3(-1,0,0),     Vector2(1,0.5) ) );
                // Backward Faces // 18 */
                BoxVertices.push_back( Vertex( Vector3(-Half.X + Length,Half.Y,Half.Z - this->BoxThick),                           Vector3(0,0,-1),     Vector2(0.5,0) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + Length,Half.Y - this->BoxThick,Half.Z - this->BoxThick),          Vector3(0,0,-1),     Vector2(0.5,this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - this->BoxThick,Half.Z - this->BoxThick),  Vector3(0,0,-1),     Vector2(1 - (this->BoxThick / Full.X),this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y,Half.Z - this->BoxThick),                   Vector3(0,0,-1),     Vector2(1 - (this->BoxThick / Full.X),0) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y,Half.Z - Length),                           Vector3(0,0,-1),     Vector2(1 - (this->BoxThick / Full.X),0) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - this->BoxThick,Half.Z - Length),          Vector3(0,0,-1),     Vector2(1 - (this->BoxThick / Full.X),this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y - this->BoxThick,Half.Z - Length),                           Vector3(0,0,-1),     Vector2(1,this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y,Half.Z - Length),                                            Vector3(0,0,-1),     Vector2(1,0) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - this->BoxThick,Half.Z - this->BoxThick),  Vector3(0,0,-1),     Vector2(1 - (this->BoxThick / Full.X),this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - Length,Half.Z - this->BoxThick),          Vector3(0,0,-1),     Vector2(1 - (this->BoxThick / Full.X),0.5) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y - Length,Half.Z - this->BoxThick),                           Vector3(0,0,-1),     Vector2(1,0.5) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y - this->BoxThick,Half.Z - this->BoxThick),                   Vector3(0,0,-1),     Vector2(1,this->BoxThick / Full.Y) ) );
                // Downward Faces // 30 */
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - this->BoxThick,Half.Z),                   Vector3(0,-1,0),     Vector2(this->BoxThick / Full.X,0) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - this->BoxThick,Half.Z - this->BoxThick),  Vector3(0,-1,0),     Vector2(this->BoxThick / Full.X,this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + Length,Half.Y - this->BoxThick,Half.Z - this->BoxThick),          Vector3(0,-1,0),     Vector2(0.5,this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + Length,Half.Y - this->BoxThick,Half.Z),                           Vector3(0,-1,0),     Vector2(0.5,0) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y - Length,Half.Z),                                            Vector3(0,-1,0),     Vector2(0,0) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y - Length,Half.Z - this->BoxThick),                           Vector3(0,-1,0),     Vector2(0,this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - Length,Half.Z - this->BoxThick),          Vector3(0,-1,0),     Vector2(this->BoxThick / Full.X,this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - Length,Half.Z),                           Vector3(0,-1,0),     Vector2(this->BoxThick / Full.X,0) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y - this->BoxThick,Half.Z - this->BoxThick),                   Vector3(0,-1,0),     Vector2(0,this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X,Half.Y - this->BoxThick,Half.Z - Length),                           Vector3(0,-1,0),     Vector2(0,0.5) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - this->BoxThick,Half.Z - Length),          Vector3(0,-1,0),     Vector2(this->BoxThick / Full.X,0.5) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - this->BoxThick,Half.Z - this->BoxThick),  Vector3(0,-1,0),     Vector2(this->BoxThick / Full.X,this->BoxThick / Full.Y) ) );
                // Right Faces // 42 */
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y,Half.Z - this->BoxThick),                   Vector3(1,0,0),      Vector2(this->BoxThick / Full.X,0) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - this->BoxThick,Half.Z - this->BoxThick),  Vector3(1,0,0),      Vector2(this->BoxThick / Full.X,this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - this->BoxThick,Half.Z - Length),          Vector3(1,0,0),      Vector2(0.5,this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y,Half.Z - Length),                           Vector3(1,0,0),      Vector2(0.5,0) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + Length,Half.Y,Half.Z),                                            Vector3(1,0,0),      Vector2(0,0) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + Length,Half.Y - this->BoxThick,Half.Z),                           Vector3(1,0,0),      Vector2(0,this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + Length,Half.Y - this->BoxThick,Half.Z - this->BoxThick),          Vector3(1,0,0),      Vector2(this->BoxThick / Full.X,this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + Length,Half.Y,Half.Z - this->BoxThick),                           Vector3(1,0,0),      Vector2(this->BoxThick / Full.X,0) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - this->BoxThick,Half.Z),                   Vector3(1,0,0),      Vector2(0,this->BoxThick / Full.Y) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - Length,Half.Z),                           Vector3(1,0,0),      Vector2(0,0.5) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - Length,Half.Z - this->BoxThick),          Vector3(1,0,0),      Vector2(this->BoxThick / Full.X,0.5) ) );
                BoxVertices.push_back( Vertex( Vector3(-Half.X + this->BoxThick,Half.Y - this->BoxThick,Half.Z - this->BoxThick),  Vector3(1,0,0),      Vector2(this->BoxThick / Full.X,this->BoxThick / Full.Y) ) );

                // Each of the flips we need to make
                std::vector<Vector3> Flips;
                Flips.push_back( Vector3(1,1,1) );
                Flips.push_back( Vector3(1,1,-1) );
                Flips.push_back( Vector3(1,-1,1) );
                Flips.push_back( Vector3(-1,1,1) );
                Flips.push_back( Vector3(1,-1,-1) );
                Flips.push_back( Vector3(-1,1,-1) );
                Flips.push_back( Vector3(-1,-1,1) );
                Flips.push_back( Vector3(-1,-1,-1) );

                // Flip this set of points against the list of flips
                for( std::vector<Vector3>::iterator CurrentFlip = Flips.begin() ; CurrentFlip != Flips.end() ; ++CurrentFlip )
                {
                    for( std::vector<Procedural::Vertex>::iterator CurrentVertex = BoxVertices.begin() ; CurrentVertex != BoxVertices.end() ; ++CurrentVertex )
                    {
                        this->AddPoint( Buffer,
                                        (*CurrentVertex).Position * (*CurrentFlip),
                                        (*CurrentVertex).Normal * (*CurrentFlip),
                                        (*CurrentVertex).UV );
                    }
                }

                // Create Index's
                for( Whole FlipCount = 0 ; FlipCount<Flips.size() ; ++FlipCount )
                {
                    Whole ShapeIndex = FlipCount * BoxVertices.size();

                    // Top-Left-Front Forward Face
                    if( 1 == Flips.at(FlipCount).Z ) { // draw them in backward order if this shape is flipped on the Z axis
                        //Forward Face Unflipped
                        Buffer.AddTriangle( 0 + ShapeIndex, 1 + ShapeIndex, 2 + ShapeIndex );   // These things could be replaced by a series of loops, but this was simpler
                        Buffer.AddTriangle( 0 + ShapeIndex, 2 + ShapeIndex, 3 + ShapeIndex );
                        Buffer.AddTriangle( 0 + ShapeIndex, 3 + ShapeIndex, 4 + ShapeIndex );
                        Buffer.AddTriangle( 0 + ShapeIndex, 4 + ShapeIndex, 5 + ShapeIndex );
                        //Backward Faces Unflipped
                        Buffer.AddTriangle( 18 + ShapeIndex, 19 + ShapeIndex, 20 + ShapeIndex );
                        Buffer.AddTriangle( 18 + ShapeIndex, 20 + ShapeIndex, 21 + ShapeIndex );
                        Buffer.AddTriangle( 22 + ShapeIndex, 23 + ShapeIndex, 24 + ShapeIndex );
                        Buffer.AddTriangle( 22 + ShapeIndex, 24 + ShapeIndex, 25 + ShapeIndex );
                        Buffer.AddTriangle( 26 + ShapeIndex, 27 + ShapeIndex, 28 + ShapeIndex );
                        Buffer.AddTriangle( 26 + ShapeIndex, 28 + ShapeIndex, 29 + ShapeIndex );
                    }else{
                        //Forward Face Flipped
                        Buffer.AddTriangle( 0 + ShapeIndex, 1 + ShapeIndex, 2 + ShapeIndex );
                        Buffer.AddTriangle( 0 + ShapeIndex, 3 + ShapeIndex, 4 + ShapeIndex );
                        Buffer.AddTriangle( 0 + ShapeIndex, 2 + ShapeIndex, 3 + ShapeIndex );
                        Buffer.AddTriangle( 0 + ShapeIndex, 4 + ShapeIndex, 5 + ShapeIndex );
                        //Backward Faces Flipped
                        Buffer.AddTriangle( 26 + ShapeIndex, 28 + ShapeIndex, 29 + ShapeIndex );
                        Buffer.AddTriangle( 26 + ShapeIndex, 27 + ShapeIndex, 28 + ShapeIndex );
                        Buffer.AddTriangle( 22 + ShapeIndex, 24 + ShapeIndex, 25 + ShapeIndex );
                        Buffer.AddTriangle( 22 + ShapeIndex, 23 + ShapeIndex, 24 + ShapeIndex );
                        Buffer.AddTriangle( 18 + ShapeIndex, 20 + ShapeIndex, 21 + ShapeIndex );
                        Buffer.AddTriangle( 18 + ShapeIndex, 19 + ShapeIndex, 20 + ShapeIndex );
                    }

                    if( 1 == Flips.at(FlipCount).Y ) { // draw them in backward order if this shape is flipped on the Y axis
                        // Upward Face Unflipped
                        Buffer.AddTriangle( 6 + ShapeIndex, 7 + ShapeIndex, 8 + ShapeIndex );
                        Buffer.AddTriangle( 6 + ShapeIndex, 8 + ShapeIndex, 9 + ShapeIndex );
                        Buffer.AddTriangle( 6 + ShapeIndex, 9 + ShapeIndex, 10 + ShapeIndex );
                        Buffer.AddTriangle( 6 + ShapeIndex, 10 + ShapeIndex, 11 + ShapeIndex );
                        // Downward Faces Unflipped
                        Buffer.AddTriangle( 30 + ShapeIndex, 31 + ShapeIndex, 32 + ShapeIndex );
                        Buffer.AddTriangle( 30 + ShapeIndex, 32 + ShapeIndex, 33 + ShapeIndex );
                        Buffer.AddTriangle( 34 + ShapeIndex, 35 + ShapeIndex, 36 + ShapeIndex );
                        Buffer.AddTriangle( 34 + ShapeIndex, 36 + ShapeIndex, 37 + ShapeIndex );
                        Buffer.AddTriangle( 38 + ShapeIndex, 39 + ShapeIndex, 40 + ShapeIndex );
                        Buffer.AddTriangle( 38 + ShapeIndex, 40 + ShapeIndex, 41 + ShapeIndex );
                    }else{
                        // Upward Face Flipped
                        Buffer.AddTriangle( 6 + ShapeIndex, 10 + ShapeIndex, 11 + ShapeIndex );
                        Buffer.AddTriangle( 6 + ShapeIndex, 9 + ShapeIndex, 10 + ShapeIndex );
                        Buffer.AddTriangle( 6 + ShapeIndex, 8 + ShapeIndex, 9 + ShapeIndex );
                        Buffer.AddTriangle( 6 + ShapeIndex, 7 + ShapeIndex, 8 + ShapeIndex );
                        // Downward Faces Flipped
                        Buffer.AddTriangle( 38 + ShapeIndex, 40 + ShapeIndex, 41 + ShapeIndex );
                        Buffer.AddTriangle( 38 + ShapeIndex, 39 + ShapeIndex, 40 + ShapeIndex );
                        Buffer.AddTriangle( 34 + ShapeIndex, 36 + ShapeIndex, 37 + ShapeIndex );
                        Buffer.AddTriangle( 34 + ShapeIndex, 35 + ShapeIndex, 36 + ShapeIndex );
                        Buffer.AddTriangle( 30 + ShapeIndex, 32 + ShapeIndex, 33 + ShapeIndex );
                        Buffer.AddTriangle( 30 + ShapeIndex, 31 + ShapeIndex, 32 + ShapeIndex );
                    }

                    if( 1 == Flips.at(FlipCount).X ) { // draw them in backward order if this shape is flipped on the X axis
                        // Left Face Unflipped
                        Buffer.AddTriangle( 12 + ShapeIndex, 13 + ShapeIndex, 14 + ShapeIndex );
                        Buffer.AddTriangle( 12 + ShapeIndex, 14 + ShapeIndex, 15 + ShapeIndex );
                        Buffer.AddTriangle( 12 + ShapeIndex, 15 + ShapeIndex, 16 + ShapeIndex );
                        Buffer.AddTriangle( 12 + ShapeIndex, 16 + ShapeIndex, 17 + ShapeIndex );
                        // Right Faces Unflipped
                        Buffer.AddTriangle( 42 + ShapeIndex, 43 + ShapeIndex, 44 + ShapeIndex );
                        Buffer.AddTriangle( 42 + ShapeIndex, 44 + ShapeIndex, 45 + ShapeIndex );
                        Buffer.AddTriangle( 46 + ShapeIndex, 47 + ShapeIndex, 48 + ShapeIndex );
                        Buffer.AddTriangle( 46 + ShapeIndex, 48 + ShapeIndex, 49 + ShapeIndex );
                        Buffer.AddTriangle( 50 + ShapeIndex, 51 + ShapeIndex, 52 + ShapeIndex );
                        Buffer.AddTriangle( 50 + ShapeIndex, 52 + ShapeIndex, 53 + ShapeIndex );
                    }else{
                        // Left Face Unflipped
                        Buffer.AddTriangle( 12 +ShapeIndex, 16 + ShapeIndex, 17 + ShapeIndex );
                        Buffer.AddTriangle( 12 +ShapeIndex, 15 + ShapeIndex, 16 + ShapeIndex );
                        Buffer.AddTriangle( 12 +ShapeIndex, 14 + ShapeIndex, 15 + ShapeIndex );
                        Buffer.AddTriangle( 12 +ShapeIndex, 13 + ShapeIndex, 14 + ShapeIndex );
                        // Right Faces Unflipped
                        Buffer.AddTriangle( 50 + ShapeIndex, 52 + ShapeIndex, 53 + ShapeIndex );
                        Buffer.AddTriangle( 50 + ShapeIndex, 51 + ShapeIndex, 52 + ShapeIndex );
                        Buffer.AddTriangle( 46 + ShapeIndex, 48 + ShapeIndex, 49 + ShapeIndex );
                        Buffer.AddTriangle( 46 + ShapeIndex, 47 + ShapeIndex, 48 + ShapeIndex );
                        Buffer.AddTriangle( 42 + ShapeIndex, 44 + ShapeIndex, 45 + ShapeIndex );
                        Buffer.AddTriangle( 42 + ShapeIndex, 43 + ShapeIndex, 44 + ShapeIndex );
                    }
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            BoxCornerGenerator& BoxCornerGenerator::SetHalfSizeX(const Real HalfSizeX)
            {
                if( HalfSizeX <= 0.0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Size on an axis for a generated box-corner mesh must be greater than zero.");

                this->BoxHalf.X = HalfSizeX;
                return *this;
            }

            BoxCornerGenerator& BoxCornerGenerator::SetHalfSizeY(const Real HalfSizeY)
            {
                if( HalfSizeY <= 0.0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Size on an axis for a generated box-corner mesh must be greater than zero.");

                this->BoxHalf.Y = HalfSizeY;
                return *this;
            }

            BoxCornerGenerator& BoxCornerGenerator::SetHalfSizeZ(const Real HalfSizeZ)
            {
                if( HalfSizeZ <= 0.0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Size on an axis for a generated box-corner mesh must be greater than zero.");

                this->BoxHalf.Z = HalfSizeZ;
                return *this;
            }

            BoxCornerGenerator& BoxCornerGenerator::SetHalfExtents(const Vector3& HalfExtents)
            {
                this->SetHalfSizeX(HalfExtents.X);
                this->SetHalfSizeY(HalfExtents.Y);
                this->SetHalfSizeZ(HalfExtents.Z);
                return *this;
            }

            BoxCornerGenerator& BoxCornerGenerator::SetCornerThickness(const Real CornerThickness)
            {
                if( CornerThickness == 0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Number of segments for generated box-corner mesh must be greater than zero.");

                this->BoxThick = CornerThickness;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
