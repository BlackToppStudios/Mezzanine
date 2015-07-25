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
#ifndef _graphicsproceduralcellgenerator_cpp
#define _graphicsproceduralcellgenerator_cpp

#include "Graphics/Procedural/Texture/cellgenerator.h"

#include "vector3.h"
#include "MathTools/mathtools.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            CellGenerator::CellGenerator() :
                GenColour(1.0,1.0,1.0,1.0),
                GenSeed(5120),
                GenRegularity(128),
                GenDensity(8),
                GenMode(CellGenerator::CM_Grid),
                GenPattern(CellGenerator::CP_Both)
                {  }

            CellGenerator::~CellGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void CellGenerator::AddToTextureBuffer(TextureBuffer& Buffer) const
            {
                Boole cfc;
                Real Coeff;

                Whole TargetWidth = Buffer.GetWidth();
                Whole TargetHeight = Buffer.GetHeight();

                const Real Regularity = this->GenRegularity / 255.0;
                std::vector<Vector3> CellPoints( this->GenDensity * this->GenDensity );
                MathTools::MersenneTwisterGenerator32 NumGen(this->GenSeed);

                for( Whole Y = 0 ; Y < this->GenDensity ; ++Y )
                {
                    for( Whole X = 0 ; X < this->GenDensity ; ++X )
                    {
                        Real RandNum1 = NumGen.GeneratePositiveReal();//static_cast<Real>( NumGen.GenerateInt() ) / 65536.0;
                        Real RandNum2 = NumGen.GeneratePositiveReal();//static_cast<Real>( NumGen.GenerateInt() ) / 65536.0;
                        CellPoints[X + Y * this->GenDensity].X = ( X + 0.5 + ( RandNum1 - 0.5 ) * ( 1 - Regularity ) ) / this->GenDensity - 1.0 / TargetWidth;
                        CellPoints[X + Y * this->GenDensity].Y = ( Y + 0.5 + ( RandNum2 - 0.5 ) * ( 1 - Regularity ) ) / this->GenDensity - 1.0 / TargetHeight;
                        CellPoints[X + Y * this->GenDensity].Z = 0;
                    }
                }

                for( Whole Y = 0 ; Y < TargetHeight ; ++Y )
                {
                    for( Whole X = 0 ; X < TargetWidth ; ++X )
                    {
                        Vector3 PixelPos;
                        PixelPos.X = static_cast<Real>( X ) / static_cast<Real>( TargetWidth ),
                        PixelPos.Y = static_cast<Real>( Y ) / static_cast<Real>( TargetHeight );
                        PixelPos.Z = 0.0;

                        Real MinDist = 10.0;
                        Real NextMinDist = MinDist;
                        Integer Xo = X * this->GenDensity / TargetWidth;
                        Integer Yo = Y * this->GenDensity / TargetHeight;
                        for( Integer V = -1 ; V < 2 ; ++V )
                        {
                            Integer Vo = ( ( Yo + this->GenDensity + V ) % this->GenDensity ) * this->GenDensity;
                            for( Integer U = -1 ; U < 2 ; ++U )
                            {
                                Vector3 CellPos = CellPoints[ ( ( Xo + this->GenDensity + U ) % this->GenDensity ) + Vo ];
                                if( U == -1 && X * this->GenDensity < TargetWidth )
                                    { CellPos.X -= 1; }
                                if( V == -1 && Y * this->GenDensity < TargetHeight )
                                    { CellPos.Y -= 1; }
                                if( U == 1 && X * this->GenDensity >= TargetWidth * ( this->GenDensity - 1 ) )
                                    { CellPos.X += 1; }
                                if( V == 1 && Y * this->GenDensity >= TargetHeight * ( this->GenDensity - 1 ) )
                                    { CellPos.Y += 1; }

                                Real Norm = PixelPos.Distance(CellPos);
                                if( Norm < MinDist ) {
                                    NextMinDist = MinDist;
                                    MinDist = Norm;
                                }else if( Norm < NextMinDist ) {
                                    NextMinDist = Norm;
                                }
                            }
                        }

                        switch( this->GenPattern )
                        {
                            case CellGenerator::CP_Cross:
                            {
                                MinDist = MathTools::Clamp( 2 * NextMinDist * this->GenDensity - 1, Real(0.0), Real(1.0) );
                                break;
                            }
                            case CellGenerator::CP_Cone:
                            {
                                MinDist = MathTools::Clamp( 1 - MinDist * this->GenDensity, Real(0.0), Real(1.0) );
                                break;
                            }
                            default:
                            case CellGenerator::CP_Both:
                            {
                                MinDist = MathTools::Clamp( ( NextMinDist - MinDist ) * this->GenDensity, Real(0.0), Real(1.0) );
                                break;
                            }
                        }

                        switch( this->GenMode )
                        {
                            case CellGenerator::CM_Chessboard:
                            {
                                cfc = ( ( Xo & 1 ) ^ ( Yo & 1 ) ) != 0;
                                Coeff = ( 1 - 2 * cfc ) / 2.5;
                                Buffer.SetRedByte( X, Y, static_cast<UInt8>( ( cfc + Coeff * MinDist ) * this->GenColour.RedChannel * 255.0 ) );
                                Buffer.SetGreenByte( X, Y, static_cast<UInt8>( ( cfc + Coeff * MinDist ) * this->GenColour.GreenChannel * 255.0 ) );
                                Buffer.SetBlueByte( X, Y, static_cast<UInt8>( ( cfc + Coeff * MinDist ) * this->GenColour.BlueChannel * 255.0 ) );
                                break;
                            }
                            default:
                            case CellGenerator::CM_Grid:
                            {
                                Buffer.SetRedByte( X, Y, static_cast<UInt8>( MinDist * this->GenColour.RedChannel * 255.0 ) );
                                Buffer.SetGreenByte( X, Y, static_cast<UInt8>( MinDist * this->GenColour.GreenChannel * 255.0 ) );
                                Buffer.SetBlueByte( X, Y, static_cast<UInt8>( MinDist * this->GenColour.BlueChannel * 255.0 ) );
                                break;
                            }
                        }
                        Buffer.SetAlphaReal( X, Y, this->GenColour.AlphaChannel );
                    }
                }
            }

            String CellGenerator::GetName() const
                { return "CellGenerator"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            CellGenerator& CellGenerator::SetColour(const ColourValue& Colour)
            {
                this->GenColour = Colour;
                return *this;
            }

            CellGenerator& CellGenerator::SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            CellGenerator& CellGenerator::SetSeed(const Whole Seed)
            {
                this->GenSeed = Seed;
                return *this;
            }

            CellGenerator& CellGenerator::SetRegularity(const UInt8 Regularity)
            {
                this->GenRegularity = Regularity;
                return *this;
            }

            CellGenerator& CellGenerator::SetDensity(const Whole Density)
            {
                this->GenDensity = Density;
                return *this;
            }

            CellGenerator& CellGenerator::SetMode(const CellMode Mode)
            {
                this->GenMode = Mode;
                return *this;
            }

            CellGenerator& CellGenerator::SetPattern(const CellPattern Pattern)
            {
                this->GenPattern = Pattern;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
