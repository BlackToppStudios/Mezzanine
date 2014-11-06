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
#ifndef _graphicsproceduraltorusgenerator_cpp
#define _graphicsproceduraltorusgenerator_cpp

#include "Graphics/Procedural/torusgenerator.h"

#include "MathTools/mathtools.h"
#include "exception.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            TorusGenerator::TorusGenerator(const Real PoloidalRadius, const Real ToroidalRadius, const Whole PoloidalSeg, const Whole ToroidalSeg) :
                TorusPoloidalRadius(PoloidalRadius),
                TorusToroidalRadius(ToroidalRadius),
                NumPoloidalSeg(PoloidalSeg),
                NumToroidalSeg(ToroidalSeg)
                {  }

            TorusGenerator::~TorusGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void TorusGenerator::AddToTriangleBuffer(TriangleBuffer& Buffer) const
            {
                Buffer.RebaseOffset();
                Buffer.EstimateVertexCount( ( this->NumPoloidalSeg + 1 ) * ( this->NumToroidalSeg + 1 ) );
                Buffer.EstimateIndexCount( ( this->NumPoloidalSeg ) * ( this->NumToroidalSeg + 1 ) * 6 );

                Real deltaSection = ( MathTools::GetTwoPi() / this->NumToroidalSeg );
                Real deltaCircle = ( MathTools::GetTwoPi() / this->NumPoloidalSeg );
                Integer Offset = 0;

                for( Whole i = 0 ; i <= this->NumPoloidalSeg ; i++ )
                {
                    for( Whole j = 0 ; j<= this->NumToroidalSeg ; j++ )
                    {
                        Vector3 c0( this->TorusPoloidalRadius, 0.0, 0.0 );
                        Vector3 v0( this->TorusPoloidalRadius + this->TorusToroidalRadius * cosf( j * deltaSection ), this->TorusToroidalRadius * sinf( j * deltaSection ), 0.0 );
                        Quaternion q( i * deltaCircle, Vector3::Unit_Y() );
                        Vector3 v = q * v0;
                        Vector3 c = q * c0;
                        this->AddPoint( Buffer, v,
                                        ( v - c ).GetNormal(),
                                        Vector2( i / (Real)this->NumPoloidalSeg, j / (Real)this->NumToroidalSeg ) );

                        if( i != this->NumPoloidalSeg ) {
                            Buffer.AddIndex( Offset + this->NumToroidalSeg + 1 );
                            Buffer.AddIndex( Offset );
                            Buffer.AddIndex( Offset + this->NumToroidalSeg );
                            Buffer.AddIndex( Offset + this->NumToroidalSeg + 1 );
                            Buffer.AddIndex( Offset + 1 );
                            Buffer.AddIndex( Offset );
                        }
                        Offset++;
                    }
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            TorusGenerator& TorusGenerator::SetPoloidalRadius(const Real PoloidalRadius)
            {
                if( PoloidalRadius <= 0.0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Radius for Poloidal ring of a generated torus mesh must be greater than zero.");

                this->TorusPoloidalRadius = PoloidalRadius;
                return *this;
            }

            TorusGenerator& TorusGenerator::SetToroidalRadius(const Real ToroidalRadius)
            {
                if( ToroidalRadius <= 0.0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Radius for Toroidal ring of a generated torus mesh must be greater than zero.");

                this->TorusToroidalRadius = ToroidalRadius;
                return *this;
            }

            TorusGenerator& TorusGenerator::SetNumPoloidalSeg(const Whole PoloidalSeg)
            {
                if( PoloidalSeg < 3 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Number of segments for Poloidal ring of generated torus mesh must be greater than two.");

                this->NumPoloidalSeg = PoloidalSeg;
                return *this;
            }

            TorusGenerator& TorusGenerator::SetNumToroidalSeg(const Whole ToroidalSeg)
            {
                if( ToroidalSeg < 3 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Number of segments for Toroidal ring of generated torus mesh must be greater than two.");

                this->NumToroidalSeg = ToroidalSeg;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
