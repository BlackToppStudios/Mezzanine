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
#ifndef _graphicsproceduralcapsulegenerator_cpp
#define _graphicsproceduralcapsulegenerator_cpp

#include "Graphics/Procedural/capsulegenerator.h"

#include "mathtools.h"
#include "exception.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            CapsuleGenerator::CapsuleGenerator(const Real Radius, const Real Height, const Whole Rings, const Whole SegCircle, const Whole SegHeight) :
                CapsuleRadius(Radius),
                CapsuleHeight(Height),
                NumRings(Rings),
                NumSegCircle(SegCircle),
                NumSegHeight(SegHeight)
                {  }

            CapsuleGenerator::~CapsuleGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void CapsuleGenerator::AddToTriangleBuffer(TriangleBuffer& Buffer) const
            {
                Buffer.RebaseOffset();
                Buffer.EstimateVertexCount( ( 2 * this->NumRings + 2 ) * ( this->NumSegCircle + 1 ) + ( this->NumSegHeight - 1 ) * ( this->NumSegCircle + 1 ) );
                Buffer.EstimateIndexCount( ( 2 * this->NumRings + 1 ) * ( this->NumSegCircle + 1 ) * 6 + ( this->NumSegHeight - 1 ) * ( this->NumSegCircle + 1 ) * 6 );

                Real fDeltaRingAngle = ( MathTools::GetHalfPi() / this->NumRings );
                Real fDeltaSegAngle = ( MathTools::GetTwoPi() / this->NumSegCircle );

                Real sphereRatio = this->CapsuleRadius / ( 2 * this->CapsuleRadius + this->CapsuleHeight );
                Real cylinderRatio = this->CapsuleHeight / ( 2 * this->CapsuleRadius + this->CapsuleHeight );
                Integer Offset = 0;

                // Top half sphere
                // Generate the group of rings for the sphere
                for( Whole ring = 0 ; ring <= this->NumRings ; ring++ )
                {
                    Real r0 = this->CapsuleRadius * sinf( ring * fDeltaRingAngle );
                    Real y0 = this->CapsuleRadius * cosf( ring * fDeltaRingAngle );

                    // Generate the group of segments for the current ring
                    for( Whole seg = 0; seg <= this->NumSegCircle; seg++ )
                    {
                        Real x0 = r0 * cosf( seg * fDeltaSegAngle );
                        Real z0 = r0 * sinf( seg * fDeltaSegAngle );

                        // Add one vertex to the strip which makes up the sphere
                        this->AddPoint(Buffer, Vector3( x0, 0.5 * this->CapsuleHeight + y0, z0 ),
                                       Vector3( x0, y0, z0 ).GetNormal(),
                                       Vector2( (Real)seg / (Real)this->NumSegCircle, (Real)ring / (Real)this->NumRings * sphereRatio) );

                        // each vertex (except the last) has six indices pointing to it
                        Buffer.AddIndex( Offset + this->NumSegCircle + 1 );
                        Buffer.AddIndex( Offset + this->NumSegCircle );
                        Buffer.AddIndex( Offset );
                        Buffer.AddIndex( Offset + this->NumSegCircle + 1 );
                        Buffer.AddIndex( Offset );
                        Buffer.AddIndex( Offset + 1 );

                        Offset++;
                    }//for each segment
                }//for each ring

                // Cylinder part
                Real deltaAngle = ( MathTools::GetTwoPi() / this->NumSegCircle );
                Real deltaHeight = this->CapsuleHeight / (Real)this->NumSegHeight;

                for( Whole i = 1 ; i < this->NumSegHeight ; i++ )
                {
                    for( Whole j = 0 ; j <= this->NumSegCircle ; j++ )
                    {
                        Real x0 = this->CapsuleRadius * cosf( j * deltaAngle );
                        Real z0 = this->CapsuleRadius * sinf( j * deltaAngle );

                        this->AddPoint(Buffer, Vector3( x0, 0.5 * this->CapsuleHeight - i * deltaHeight, z0 ),
                                       Vector3( x0, 0, z0 ).GetNormal(),
                                       Vector2( j / (Real)this->NumSegCircle, i / (Real)this->NumSegHeight * cylinderRatio + sphereRatio));

                        Buffer.AddIndex( Offset + this->NumSegCircle + 1 );
                        Buffer.AddIndex( Offset + this->NumSegCircle );
                        Buffer.AddIndex( Offset );
                        Buffer.AddIndex( Offset + this->NumSegCircle + 1 );
                        Buffer.AddIndex( Offset );
                        Buffer.AddIndex( Offset + 1 );

                        Offset++;
                    }
                }

                // Bottom half sphere
                // Generate the group of rings for the sphere
                for( Whole ring = 0 ; ring <= this->NumRings ; ring++ )
                {
                    Real r0 = this->CapsuleRadius * sinf( MathTools::GetHalfPi() + ring * fDeltaRingAngle );
                    Real y0 = this->CapsuleRadius * cosf( MathTools::GetHalfPi() + ring * fDeltaRingAngle );

                    // Generate the group of segments for the current ring
                    for( Whole seg = 0 ; seg <= this->NumSegCircle ; seg++ )
                    {
                        Real x0 = r0 * cosf( seg * fDeltaSegAngle );
                        Real z0 = r0 * sinf( seg * fDeltaSegAngle );

                        // Add one vertex to the strip which makes up the sphere
                        this->AddPoint(Buffer, Vector3( x0, -0.5 * this->CapsuleHeight + y0, z0 ),
                                       Vector3( x0, y0, z0 ).GetNormal(),
                                       Vector2( (Real)seg / (Real)this->NumSegCircle, (Real)ring / (Real)this->NumRings * sphereRatio + cylinderRatio + sphereRatio) );

                        if( ring != this->NumRings ) {
                            // each vertex (except the last) has six indices pointing to it
                            Buffer.AddIndex( Offset + this->NumSegCircle + 1 );
                            Buffer.AddIndex( Offset + this->NumSegCircle );
                            Buffer.AddIndex( Offset );
                            Buffer.AddIndex( Offset + this->NumSegCircle + 1 );
                            Buffer.AddIndex( Offset );
                            Buffer.AddIndex( Offset + 1 );
                        }
                        Offset++;
                    }//for each segment
                }//for each ring
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            CapsuleGenerator& CapsuleGenerator::SetRadius(const Real Radius)
            {
                if( Radius <= 0.0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Radius for a generated capsule mesh must be greater than zero.");

                this->CapsuleRadius = Radius;
                return *this;
            }

            CapsuleGenerator& CapsuleGenerator::SetHeight(const Real Height)
            {
                if( Height <= 0.0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Height for a generated capsule mesh must be greater than zero.");
                this->CapsuleHeight = Height;
                return *this;
            }

            CapsuleGenerator& CapsuleGenerator::SetNumRings(const Whole Rings)
            {
                if( Rings == 0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Number of rings for generated capsule mesh must be greater than zero.");

                this->NumRings = Rings;
                return *this;
            }

            CapsuleGenerator& CapsuleGenerator::SetNumSegCircle(const Whole SegCircle)
            {
                if( SegCircle == 0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Number of segments for generated capsule mesh must be greater than zero.");

                this->NumSegCircle = SegCircle;
                return *this;
            }

            CapsuleGenerator& CapsuleGenerator::SetNumSegHeight(const Whole SegHeight)
            {
                if( SegHeight == 0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Number of segments for generated capsule mesh must be greater than zero.");

                this->NumSegHeight = SegHeight;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
