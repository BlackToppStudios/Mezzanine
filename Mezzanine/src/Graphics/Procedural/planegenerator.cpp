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
#ifndef _graphicsproceduralplanegenerator_cpp
#define _graphicsproceduralplanegenerator_cpp

#include "Graphics/Procedural/planegenerator.h"

#include "exception.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            PlaneGenerator::PlaneGenerator() :
                PlaneNormal( Vector3::Unit_Y() ),
                PlaneSize(1,1),
                NumSegX(1),
                NumSegY(1)
                {  }

            PlaneGenerator::PlaneGenerator(const Vector3& Norm, const Vector2& Size) :
                PlaneNormal(Norm),
                PlaneSize(Size),
                NumSegX(1),
                NumSegY(1)
                {  }

            PlaneGenerator::~PlaneGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void PlaneGenerator::AddToTriangleBuffer(TriangleBuffer& Buffer) const
            {
                Buffer.RebaseOffset();
                Buffer.EstimateVertexCount( ( this->NumSegX + 1 ) * ( this->NumSegY + 1 ) );
                Buffer.EstimateIndexCount( this->NumSegX * this->NumSegY * 6 );
                Integer Offset = 0;

                Vector3 vX = this->PlaneNormal.Perpendicular();
                Vector3 vY = this->PlaneNormal.CrossProduct(vX);
                Vector3 delta1 = vX * ( this->PlaneSize.X / (Real)this->NumSegX );
                Vector3 delta2 = vY * ( this->PlaneSize.Y / (Real)this->NumSegY );
                // build one corner of the square
                Vector3 orig = ( vX * ( -0.5f * this->PlaneSize.X ) ) - ( vY * ( 0.5f * this->PlaneSize.Y ) );

                for( Whole i1 = 0 ; i1 <= this->NumSegX ; i1++ )
                {
                    for( Whole i2 = 0 ; i2 <= this->NumSegY ; i2++ )
                    {
                        this->AddPoint(Buffer, orig + ( delta1 * Real(i1) ) + ( delta2 * Real(i2) ),
                                       this->PlaneNormal,
                                       Vector2( i1 / (Real)this->NumSegX, i2 / (Real)this->NumSegY ) );
                    }
                }

                Boole Reverse = false;
                if( delta1.CrossProduct(delta2).DotProduct(this->PlaneNormal) > 0 )
                    Reverse = true;

                for( Whole n1 = 0 ; n1 < this->NumSegX ; n1++ )
                {
                    for( Whole n2 = 0 ; n2 < this->NumSegY ; n2++ )
                    {
                        if(Reverse) {
                            Buffer.AddIndex( Offset + 0 );
                            Buffer.AddIndex( Offset + ( this->NumSegY + 1 ) );
                            Buffer.AddIndex( Offset + 1 );
                            Buffer.AddIndex( Offset + 1 );
                            Buffer.AddIndex( Offset + ( this->NumSegY + 1 ) );
                            Buffer.AddIndex( Offset + ( this->NumSegY + 1 ) + 1 );
                        }else{
                            Buffer.AddIndex( Offset + 0 );
                            Buffer.AddIndex( Offset + 1 );
                            Buffer.AddIndex( Offset + ( this->NumSegY + 1 ) );
                            Buffer.AddIndex( Offset + 1 );
                            Buffer.AddIndex( Offset + ( this->NumSegY + 1 ) + 1 );
                            Buffer.AddIndex( Offset + ( this->NumSegY + 1 ) );
                        }
                        Offset++;
                    }
                    Offset++;
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            PlaneGenerator& PlaneGenerator::SetNormal(const Vector3& Norm)
            {
                if( this->PlaneNormal.IsZeroLength() )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Facing direction of a plane cannot be zero length.");

                this->PlaneNormal = Norm;
                return *this;
            }

            PlaneGenerator& PlaneGenerator::SetSizeX(const Real SizeX)
            {
                if( SizeX <= 0.0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Size on an axis for a generated plane mesh must be greater than zero.");

                this->PlaneSize.X = SizeX;
                return *this;
            }

            PlaneGenerator& PlaneGenerator::SetSizeY(const Real SizeY)
            {
                if( SizeY <= 0.0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Size on an axis for a generated plane mesh must be greater than zero.");

                this->PlaneSize.Y = SizeY;
                return *this;
            }

            PlaneGenerator& PlaneGenerator::SetSize(const Vector2 Size)
            {
                this->SetSizeX(Size.X);
                this->SetSizeY(Size.Y);
                return *this;
            }

            PlaneGenerator& PlaneGenerator::SetNumSegX(const Whole SegX)
            {
                if( SegX == 0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Number of segments for generated plane mesh must be greater than zero.");

                this->NumSegX = SegX;
                return *this;
            }

            PlaneGenerator& PlaneGenerator::SetNumSegY(const Whole SegY)
            {
                if( SegY == 0 )
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Number of segments for generated plane mesh must be greater than zero.");

                this->NumSegY = SegY;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
