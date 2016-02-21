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
#ifndef _graphicsproceduralspheregenerator_cpp
#define _graphicsproceduralspheregenerator_cpp

#include "Graphics/Procedural/Mesh/spheregenerator.h"

#include "MathTools/mathtools.h"
#include "exception.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            SphereGenerator::SphereGenerator(const Real Radius, const Whole Rings, const Whole SegCircle) :
                SphereRadius(Radius),
                NumRings(Rings),
                NumSegCircle(SegCircle)
                {  }

            SphereGenerator::~SphereGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void SphereGenerator::AddToTriangleBuffer(TriangleBuffer& Buffer) const
            {
                Buffer.RebaseOffset();
                Buffer.EstimateVertexCount( ( this->NumRings + 1 ) * ( this->NumSegCircle + 1 ) );
                Buffer.EstimateIndexCount( this->NumRings * ( this->NumSegCircle + 1 ) * 6 );

                Real fDeltaRingAngle = ( MathTools::GetPi() / this->NumRings );
                Real fDeltaSegAngle = ( MathTools::GetTwoPi() / this->NumSegCircle );
                Integer Offset = 0;

                // Generate the group of rings for the sphere
                for( Whole Ring = 0 ; Ring <= this->NumRings ; ++Ring )
                {
                    Real r0 = this->SphereRadius * sinf( Ring * fDeltaRingAngle );
                    Real y0 = this->SphereRadius * cosf( Ring * fDeltaRingAngle );

                    // Generate the group of segments for the current ring
                    for( Whole Segment = 0 ; Segment <= this->NumSegCircle ; ++Segment )
                    {
                        Real x0 = r0 * sinf( Segment * fDeltaSegAngle );
                        Real z0 = r0 * cosf( Segment * fDeltaSegAngle );

                        // Add one vertex to the strip which makes up the sphere
                        this->AddPoint(Buffer, Vector3(x0, y0, z0),
                                       Vector3(x0, y0, z0).GetNormal(),
                                       Vector2( (Real)Segment / (Real)this->NumSegCircle, (Real)Ring / (Real)this->NumRings ) );

                        if( Ring != this->NumRings ) {
                            if( Segment != this->NumSegCircle ) {
                                // each vertex (except the last) has six indices pointing to it
                                if( Ring != this->NumRings - 1 ) {
                                    Buffer.AddTriangle(Offset + this->NumSegCircle + 2, Offset, Offset + this->NumSegCircle + 1);
                                }
                                if( Ring != 0 ) {
                                    Buffer.AddTriangle(Offset + this->NumSegCircle + 2, Offset + 1, Offset);
                                }
                            }
                            Offset++;
                        }
                    }// for each segment
                }// for each ring
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            SphereGenerator& SphereGenerator::SetRadius(const Real Radius)
            {
                if( Radius <= 0.0 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Radius for a generated sphere mesh must be greater than zero.");

                this->SphereRadius = Radius;
                return *this;
            }

            SphereGenerator& SphereGenerator::SetNumRings(const Whole Rings)
            {
                if( Rings == 0 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Number of rings for generated sphere mesh must be greater than zero.");

                this->NumRings = Rings;
                return *this;
            }

            SphereGenerator& SphereGenerator::SetNumSegCircle(const Whole SegCircle)
            {
                if( SegCircle < 3 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Number of segments for circular component of generated cylinder mesh must be greater than two.");

                this->NumSegCircle = SegCircle;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
