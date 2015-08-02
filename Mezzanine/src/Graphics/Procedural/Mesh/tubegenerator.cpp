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
#ifndef _graphicsproceduraltubegenerator_cpp
#define _graphicsproceduraltubegenerator_cpp

#include "Graphics/Procedural/Mesh/tubegenerator.h"

#include "MathTools/mathtools.h"
#include "exception.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            TubeGenerator::TubeGenerator(const Real InnerRadius, const Real OuterRadius, const Real Height, const Whole SegCircle, const Whole SegHeight) :
                TubeInnerRadius(InnerRadius),
                TubeOuterRadius(OuterRadius),
                TubeHeight(Height),
                NumSegCircle(SegCircle),
                NumSegHeight(SegHeight)
                {  }

            TubeGenerator::~TubeGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void TubeGenerator::AddToTriangleBuffer(TriangleBuffer& Buffer) const
            {
                Buffer.RebaseOffset();
                Buffer.EstimateVertexCount( ( this->NumSegHeight + 1 ) * ( this->NumSegCircle + 1 ) * 2 + ( this->NumSegCircle + 1 ) * 4 );
                Buffer.EstimateIndexCount( 6 * ( this->NumSegCircle + 1 ) * this->NumSegHeight * 2 + 6 * this->NumSegCircle * 2 );

                Real deltaAngle = ( MathTools::GetTwoPi() / this->NumSegCircle );
                Real deltaHeight = this->TubeHeight / (Real)this->NumSegHeight;
                int Offset = 0;

                for( Whole i = 0 ; i <= this->NumSegHeight ; ++i )
                {
                    for( Whole j = 0 ; j <= this->NumSegCircle ; ++j )
                    {
                        Real x0 = this->TubeOuterRadius * cosf( j * deltaAngle );
                        Real z0 = this->TubeOuterRadius * sinf( j * deltaAngle );
                        this->AddPoint(Buffer, Vector3( x0, i * deltaHeight, z0 ),
                                       Vector3( x0, 0, z0 ).GetNormal(),
                                       Vector2( j / (Real)this->NumSegCircle, i / (Real)this->NumSegHeight ) );

                        if( i != this->NumSegHeight ) {
                            Buffer.AddIndex( Offset + this->NumSegCircle + 1 );
                            Buffer.AddIndex( Offset );
                            Buffer.AddIndex( Offset + this->NumSegCircle );
                            Buffer.AddIndex( Offset + this->NumSegCircle + 1 );
                            Buffer.AddIndex( Offset + 1 );
                            Buffer.AddIndex( Offset );
                        }
                        Offset++;
                    }
                }

                for( Whole i = 0 ; i <= this->NumSegHeight ; ++i )
                {
                    for( Whole j = 0 ; j<= this->NumSegCircle ; ++j )
                    {
                        Real x0 = this->TubeInnerRadius * cosf( j * deltaAngle );
                        Real z0 = this->TubeInnerRadius * sinf( j * deltaAngle );
                        this->AddPoint(Buffer, Vector3( x0, i * deltaHeight, z0 ),
                                       -Vector3( x0, 0, z0 ).GetNormal(),
                                       Vector2( j / (Real)this->NumSegCircle, i / (Real)this->NumSegHeight ) );

                        if( i != this->NumSegHeight ) {
                            Buffer.AddIndex( Offset + this->NumSegCircle + 1 );
                            Buffer.AddIndex( Offset + this->NumSegCircle );
                            Buffer.AddIndex( Offset );
                            Buffer.AddIndex( Offset + this->NumSegCircle + 1 );
                            Buffer.AddIndex( Offset );
                            Buffer.AddIndex( Offset + 1);
                        }
                        Offset++;
                    }
                }

                //low cap
                for( Whole j = 0 ; j <= this->NumSegCircle ; ++j )
                {
                    Real x0 = this->TubeInnerRadius * cosf( j * deltaAngle );
                    Real z0 = this->TubeInnerRadius * sinf( j * deltaAngle );

                    this->AddPoint(Buffer, Vector3( x0, 0.0f, z0 ),
                                   Vector3::Neg_Unit_Y(),
                                   Vector2( j / (Real)this->NumSegCircle, 1. ) );

                    x0 = this->TubeOuterRadius * cosf( j * deltaAngle );
                    z0 = this->TubeOuterRadius * sinf( j * deltaAngle );

                    this->AddPoint(Buffer, Vector3( x0, 0.0f, z0 ),
                                   Vector3::Neg_Unit_Y(),
                                   Vector2( j / (Real)this->NumSegCircle, 0. ) );

                    if( j != this->NumSegCircle ) {
                        Buffer.AddIndex( Offset );
                        Buffer.AddIndex( Offset + 1 );
                        Buffer.AddIndex( Offset + 3 );
                        Buffer.AddIndex( Offset + 2 );
                        Buffer.AddIndex( Offset );
                        Buffer.AddIndex( Offset + 3 );
                    }
                    Offset += 2;
                }


                //high cap
                for( Whole j = 0 ; j <= this->NumSegCircle ; ++j )
                {
                    Real x0 = this->TubeInnerRadius * cosf( j * deltaAngle );
                    Real z0 = this->TubeInnerRadius * sinf( j * deltaAngle );

                    this->AddPoint(Buffer, Vector3( x0, this->TubeHeight, z0 ),
                                   Vector3::Unit_Y(),
                                   Vector2( j / (Real)this->NumSegCircle, 0. ) );

                    x0 = this->TubeOuterRadius * cosf( j * deltaAngle );
                    z0 = this->TubeOuterRadius * sinf( j * deltaAngle );

                    this->AddPoint(Buffer, Vector3( x0, this->TubeHeight, z0 ),
                                   Vector3::Unit_Y(),
                                   Vector2( j / (Real)this->NumSegCircle, 1. ) );

                    if( j != this->NumSegCircle ) {
                        Buffer.AddIndex( Offset + 1 );
                        Buffer.AddIndex( Offset );
                        Buffer.AddIndex( Offset + 3 );
                        Buffer.AddIndex( Offset );
                        Buffer.AddIndex( Offset + 2 );
                        Buffer.AddIndex( Offset + 3 );
                    }
                    Offset += 2;
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            TubeGenerator& TubeGenerator::SetInnerRadius(const Real InnerRadius)
            {
                if( InnerRadius <= 0.0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Radius for a generated tube mesh must be greater than zero.");

                if( this->TubeOuterRadius < InnerRadius )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Outer radius must be larger than inner radius!");

                this->TubeInnerRadius = InnerRadius;
                return *this;
            }

            TubeGenerator& TubeGenerator::SetOuterRadius(const Real OuterRadius)
            {
                if( OuterRadius <= 0.0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Radius for a generated tube mesh must be greater than zero.");

                if( OuterRadius < this->TubeInnerRadius )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Outer radius must be larger than inner radius!");

                this->TubeOuterRadius = OuterRadius;
                return *this;
            }

            TubeGenerator& TubeGenerator::SetHeight(const Real Height)
            {
                if( Height <= 0.0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Height for a generated cylinder mesh must be greater than zero.");

                this->TubeHeight = Height;
                return *this;
            }

            TubeGenerator& TubeGenerator::SetNumSegCircle(const Whole SegCircle)
            {
                if( SegCircle < 3 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Number of segments for circular component of generated tube mesh must be greater than two.");

                this->NumSegCircle = SegCircle;
                return *this;
            }

            TubeGenerator& TubeGenerator::SetNumSegHeight(const Whole SegHeight)
            {
                if( SegHeight == 0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Number of segments for length component of generated tube mesh must be greater than zero.");

                this->NumSegHeight = SegHeight;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
