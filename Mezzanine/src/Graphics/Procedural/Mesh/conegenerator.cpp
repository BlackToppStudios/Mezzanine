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
#ifndef _graphicsproceduralconegenerator_cpp
#define _graphicsproceduralconegenerator_cpp

#include "Graphics/Procedural/Mesh/conegenerator.h"

#include "MathTools/mathtools.h"
#include "exception.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ConeGenerator::ConeGenerator(const Real Radius, const Real Height, const Whole SegCircle, const Whole SegHeight) :
                ConeRadius(Radius),
                ConeHeight(Height),
                NumSegCircle(SegCircle),
                NumSegHeight(SegHeight)
                {  }

            ConeGenerator::~ConeGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void ConeGenerator::AddToTriangleBuffer(TriangleBuffer& Buffer) const
            {
                Buffer.RebaseOffset();
                Buffer.EstimateVertexCount( ( this->NumSegHeight + 1 ) * ( this->NumSegCircle + 1 ) + this->NumSegCircle + 2 );
                Buffer.EstimateIndexCount( this->NumSegHeight * this->NumSegCircle * 6 + 3 * this->NumSegCircle );

                Real deltaAngle = ( MathTools::GetTwoPi() / this->NumSegCircle );
                Real deltaHeight = this->ConeHeight / (Real)this->NumSegHeight;
                Integer Offset = 0;

                Vector3 refNormal = Vector3( this->ConeRadius, this->ConeHeight, 0.f ).GetNormal();
                Quaternion q;

                for( Whole i = 0 ; i <= this->NumSegHeight ; i++ )
                {
                    Real r0 = this->ConeRadius * ( 1 - i / (Real)this->NumSegHeight );
                    for( Whole j = 0; j <= this->NumSegCircle ; j++ )
                    {
                        Real x0 = r0 * cosf( j * deltaAngle );
                        Real z0 = r0 * sinf( j * deltaAngle );

                        q.SetFromAxisAngle( -deltaAngle * j, Vector3::Unit_Y() );

                        this->AddPoint(Buffer, Vector3( x0, i * deltaHeight, z0 ),
                                       q * refNormal,
                                       Vector2( j / (Real)this->NumSegCircle, i / (Real)this->NumSegHeight ) );

                        if( i != this->NumSegHeight && j != this->NumSegCircle ) {
                            Buffer.AddIndex( Offset + this->NumSegCircle + 2 );
                            Buffer.AddIndex( Offset );
                            Buffer.AddIndex( Offset + this->NumSegCircle + 1 );
                            Buffer.AddIndex( Offset + this->NumSegCircle + 2 );
                            Buffer.AddIndex( Offset + 1 );
                            Buffer.AddIndex( Offset );
                        }

                        Offset++;
                    }
                }

                //low cap
                Integer CenterIndex = Offset;
                this->AddPoint(Buffer, Vector3(0.0,0.0,0.0),
                               Vector3::Neg_Unit_Y(),
                               Vector2(0.0,1.0) );
                Offset++;
                for( Whole j = 0 ; j <= this->NumSegCircle ; j++ )
                {
                    Real x0 = this->ConeRadius * cosf( j * deltaAngle );
                    Real z0 = this->ConeRadius * sinf( j * deltaAngle );

                    this->AddPoint(Buffer, Vector3(x0, 0.0f, z0),
                                   Vector3::Neg_Unit_Y(),
                                   Vector2( j / (Real)this->NumSegCircle, 0.0 ) );

                    if( j != this->NumSegCircle ) {
                        Buffer.AddIndex( CenterIndex );
                        Buffer.AddIndex( Offset );
                        Buffer.AddIndex( Offset + 1 );
                    }
                    Offset++;
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            ConeGenerator& ConeGenerator::SetRadius(const Real Radius)
            {
                if( Radius <= 0.0 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Radius for a generated cone mesh must be greater than zero.");

                this->ConeRadius = Radius;
                return *this;
            }

            ConeGenerator& ConeGenerator::SetHeight(const Real Height)
            {
                if( Height <= 0.0 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Height for a generated cone mesh must be greater than zero.");

                this->ConeHeight = Height;
                return *this;
            }

            ConeGenerator& ConeGenerator::SetNumSegCircle(const Whole SegCircle)
            {
                if( SegCircle < 3 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Number of segments for circular component of generated cone mesh must be greater than two.");

                this->NumSegCircle = SegCircle;
                return *this;
            }

            ConeGenerator& ConeGenerator::SetNumSegHeight(const Whole SegHeight)
            {
                if( SegHeight == 0 )
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Number of segments for length component of generated cone mesh must be greater than zero.");

                this->NumSegHeight = SegHeight;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
