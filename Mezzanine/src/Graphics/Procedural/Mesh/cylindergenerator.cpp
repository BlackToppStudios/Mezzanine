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
#ifndef _graphicsproceduralcylindergenerator_cpp
#define _graphicsproceduralcylindergenerator_cpp

#include "Graphics/Procedural/Mesh/cylindergenerator.h"

#include "MathTools/mathtools.h"
#include "exception.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            CylinderGenerator::CylinderGenerator(const Real Radius, const Real Height, const Whole SegCircle, const Whole SegHeight, const Boole Capped) :
                CylinderRadius(Radius),
                CylinderHeight(Height),
                NumSegCircle(SegCircle),
                NumSegHeight(SegHeight),
                CapEnds(Capped)
                {  }

            CylinderGenerator::~CylinderGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void CylinderGenerator::AddToTriangleBuffer(TriangleBuffer& Buffer) const
            {
                const Vector2 ZeroVec2(0,0);
                const Vector3 ZeroVec3(0,0,0);
                Buffer.RebaseOffset();
                if( this->CapEnds ) {
                    Buffer.EstimateVertexCount( ( this->NumSegHeight + 1 ) * ( this->NumSegCircle + 1 ) + 2 * ( this->NumSegCircle + 1 ) + 2 );
                    Buffer.EstimateIndexCount( this->NumSegHeight * ( this->NumSegCircle + 1 ) * 6 + 6 * this->NumSegCircle );
                }else{
                    Buffer.EstimateVertexCount( ( this->NumSegHeight + 1 ) * ( this->NumSegCircle + 1 ) );
                    Buffer.EstimateIndexCount( this->NumSegHeight * ( this->NumSegCircle + 1 ) * 6 );
                }


                Real deltaAngle = ( MathTools::GetTwoPi() / this->NumSegCircle );
                Real deltaHeight = this->CylinderHeight / (Real)this->NumSegHeight;
                Integer Offset = 0;

                for( Whole i = 0 ; i <= this->NumSegHeight ; i++ )
                {
                    for( Whole j = 0 ; j<= this->NumSegCircle ; j++ )
                    {
                        Real x0 = this->CylinderRadius * cosf( j * deltaAngle );
                        Real z0 = this->CylinderRadius * sinf( j * deltaAngle );

                        this->AddPoint(Buffer, Vector3(x0, i * deltaHeight, z0),
                                       Vector3(x0,0,z0).GetNormal(),
                                       Vector2(j/(Real)this->NumSegCircle, i/(Real)this->NumSegHeight));

                        if( i != this->NumSegHeight ) {
                            Buffer.AddIndex(Offset + this->NumSegCircle + 1);
                            Buffer.AddIndex(Offset);
                            Buffer.AddIndex(Offset + this->NumSegCircle);
                            Buffer.AddIndex(Offset + this->NumSegCircle + 1);
                            Buffer.AddIndex(Offset + 1);
                            Buffer.AddIndex(Offset);
                        }
                        Offset++;
                    }
                }

                if( this->CapEnds ) {
                    //low cap
                    Integer CenterIndex = Offset;
                    this->AddPoint(Buffer, ZeroVec3,
                                   Vector3::Neg_Unit_Y(),
                                   ZeroVec2);
                    Offset++;
                    for( Whole j = 0 ; j <= this->NumSegCircle ; j++ )
                    {
                        Real x0 = cosf( j * deltaAngle );
                        Real z0 = sinf( j * deltaAngle );

                        this->AddPoint(Buffer, Vector3(this->CylinderRadius * x0, 0.0f, this->CylinderRadius * z0),
                                       Vector3::Neg_Unit_Y(),
                                       Vector2(x0, z0));
                        if( j != this->NumSegCircle ) {
                            Buffer.AddIndex(CenterIndex);
                            Buffer.AddIndex(Offset);
                            Buffer.AddIndex(Offset + 1);
                        }
                        Offset++;
                    }
                    // high cap
                    CenterIndex = Offset;
                    this->AddPoint(Buffer, Vector3(0,this->CylinderHeight,0),
                                   Vector3::Unit_Y(),
                                   ZeroVec2);
                    Offset++;
                    for( Whole j = 0 ; j <= this->NumSegCircle ; j++ )
                    {
                        Real x0 = cosf( j * deltaAngle );
                        Real z0 = sinf( j * deltaAngle );

                        this->AddPoint(Buffer, Vector3(x0 * this->CylinderRadius, this->CylinderHeight, this->CylinderRadius * z0),
                                       Vector3::Unit_Y(),
                                       Vector2(x0, z0));

                        if( j != this->NumSegCircle ) {
                            Buffer.AddIndex(CenterIndex);
                            Buffer.AddIndex(Offset + 1);
                            Buffer.AddIndex(Offset);
                        }
                        Offset++;
                    }
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            CylinderGenerator& CylinderGenerator::SetRadius(const Real Radius)
            {
                if( Radius <= 0.0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Radius for a generated cylinder mesh must be greater than zero.");

                this->CylinderRadius = Radius;
                return *this;
            }

            CylinderGenerator& CylinderGenerator::SetHeight(const Real Height)
            {
                if( Height <= 0.0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Height for a generated cylinder mesh must be greater than zero.");

                this->CylinderHeight = Height;
                return *this;
            }

            CylinderGenerator& CylinderGenerator::SetCapped(const Boole Capped)
            {
                this->CapEnds = Capped;
                return *this;
            }

            CylinderGenerator& CylinderGenerator::SetNumSegCircle(const Whole SegCircle)
            {
                if( SegCircle < 3 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Number of segments for circular component of generated cylinder mesh must be greater than two.");

                this->NumSegCircle = SegCircle;
                return *this;
            }

            CylinderGenerator& CylinderGenerator::SetNumSegHeight(const Whole SegHeight)
            {
                if( SegHeight == 0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Number of segments for length component of generated cylinder mesh must be greater than zero.");

                this->NumSegHeight = SegHeight;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
