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
#ifndef _graphicsproceduralcrackmodifier_cpp
#define _graphicsproceduralcrackmodifier_cpp

#include "Graphics/Procedural/Texture/crackmodifier.h"

#include "MathTools/mathtools.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            CrackModifier::CrackModifier() :
                CrackColour(1.0,1.0,1.0,1.0),
                NormalsTexture(NULL),
                CrackCount(100),
                GeneratorSeed(5120),
                CrackLengthMode(CrackModifier::LM_Constant),
                CrackQualityMode(CrackModifier::QM_LowQuality),
                CrackLength(255),
                CrackVariation(64)
                {  }

            CrackModifier::~CrackModifier()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void CrackModifier::Modify(TextureBuffer& Buffer)
            {
                if( this->NormalsTexture == NULL ) {
                    return;
                }

                srand( this->GeneratorSeed );

                Whole TargetWidth = Buffer.GetWidth();
                Whole TargetHeight = Buffer.GetHeight();
                Whole NormalsWidth = this->NormalsTexture->GetWidth();
                Whole NormalsHeight = this->NormalsTexture->GetHeight();

                if( NormalsWidth < TargetWidth || NormalsHeight < TargetHeight ) {
                    return;
                }

                TextureBuffer TempBuffer( Buffer );

                for( Whole CurrCrack = 0 ; CurrCrack < this->CrackCount ; ++CurrCrack )
                {
                    Real X = ( static_cast<Real>( rand() ) / RAND_MAX ) * static_cast<Real>( TargetWidth );
                    Real Y = ( static_cast<Real>( rand() ) / RAND_MAX ) * static_cast<Real>( TargetHeight );
                    Real Angle = MathTools::GetTwoPi() * ( static_cast<Real>( rand() ) / RAND_MAX );
                    Whole PixelCount = static_cast<Whole>( this->CrackLength );
                    ColourValue NormalsPixel = this->NormalsTexture->GetPixel( static_cast<Whole>(X), static_cast<Whole>(Y) );

                    if( NormalsWidth && this->CrackLengthMode == CrackModifier::LM_NormalBased ) {
                        Vector2 Normal(NormalsPixel.RedChannel * 255.0 - 127.0, NormalsPixel.GreenChannel * 255.0 - 127.0);
                        Real NormalLength = Normal.SquaredLength();
                        NormalLength = ( NormalLength > 0 ) ? MathTools::Sqrt(NormalLength) : 0;
                        PixelCount = std::min<Whole>( static_cast<Whole>( PixelCount * NormalLength * NormalLength / 8.0 ), static_cast<Whole>( this->CrackLength ) );
                    }else if( this->CrackLengthMode == CrackModifier::LM_Random ) {
                        PixelCount = static_cast<Whole>( PixelCount * ( static_cast<Real>( rand() ) / RAND_MAX ) * 2.0 );
                    }

                    while( --PixelCount >= 0 )
                    {
                        Angle += static_cast<Real>( this->CrackVariation ) / 256.0 * ( 2.0 * (static_cast<Real>( rand() ) / RAND_MAX) - 1.0 );

                        X = X + MathTools::Cos( Angle );
                        Y = Y + MathTools::Sin( Angle );
                        if( static_cast<Whole>(X) >= TargetWidth || static_cast<Whole>(Y) >= TargetHeight )
                            break;

                        if( NormalsWidth ) {
                            Vector2 Normal(127.0 - NormalsPixel.RedChannel * 255.0, NormalsPixel.GreenChannel * 255.0 - 127.0 );
                            if( Normal.X == 0.0 ) {
                                if( Normal.Y > 0.0 ) {
                                    Angle = MathTools::GetPi();
                                }else{
                                    Angle = MathTools::GetTwoPi();
                                }
                            }else if( Normal.X < 0 ) {
                                Angle = MathTools::ATan( Normal.Y / Normal.X ) + 1.5 * MathTools::GetPi();
                            }else if( Normal.Y < 0 ) {
                                Angle = MathTools::ATan( Normal.Y / Normal.X ) + 2.5 * MathTools::GetPi();
                            }else{
                                Angle = MathTools::ATan( Normal.Y / Normal.X ) + MathTools::GetHalfPi();
                            }
                            Real NormalLength = Normal.SquaredLength();
                            NormalLength = ( NormalLength > 0 ) ? MathTools::Sqrt(NormalLength) : 0;
                            if( NormalLength < ( 255.0 - NormalsPixel.AlphaChannel * 255.0 ) / 4.0 ) {
                                continue;
                            }
                        }

                        switch( this->CrackQualityMode )
                        {
                            case CrackModifier::QM_SubPixel:
                            {
                                ColourValue x1, y1, x2, y2;
                                Real cy2, cy1, cx2, cx1;
                                Whole NextX, NextY;

                                cy2 = ( X - MathTools::Floor(X) ) * ( Y - MathTools::Floor(Y) );
                                cy1 = ( Y - MathTools::Floor(Y) ) * ( MathTools::Ceil(X) - X );
                                cx2 = ( X - MathTools::Floor(X) ) * ( MathTools::Ceil(Y) - Y );
                                cx1 = 1 - ( cx2 + cy1 + cy2 );
                                NextX = std::min( static_cast<Whole>(X) + 1, TargetWidth );
                                NextY = std::min( static_cast<Whole>(Y) + 1, TargetHeight );

                                x1 = Buffer.GetPixel( static_cast<Whole>(X), static_cast<Whole>(Y) );
                                y1 = Buffer.GetPixel( static_cast<Whole>(X), NextY );
                                x2 = Buffer.GetPixel( NextX, static_cast<Whole>(Y) );
                                y2 = Buffer.GetPixel( NextX, NextY );

                                x1 *= ( 1 - cx1 );
                                x2 *= ( 1 - cx2 );
                                y1 *= ( 1 - cy1 );
                                y2 *= ( 1 - cy2 );

                                x1 += this->CrackColour * cx1;
                                y1 += this->CrackColour * cy1;
                                x2 += this->CrackColour * cx2;
                                y2 += this->CrackColour * cy2;

                                TempBuffer.SetPixel( static_cast<Whole>(X), static_cast<Whole>(Y), x1 );
                                TempBuffer.SetPixel( static_cast<Whole>(X), NextY, y1 );
                                TempBuffer.SetPixel( NextX, static_cast<Whole>(Y), x2 );
                                TempBuffer.SetPixel( NextX, NextY, y2 );
                                break;
                            }
                            case CrackModifier::QM_Blend:
                            {
                                TempBuffer.SetPixel( static_cast<Whole>(X), static_cast<Whole>(Y), Buffer.GetPixel( static_cast<Whole>(X), static_cast<Whole>(Y) ) + this->CrackColour );
                                break;
                            }
                            default:
                            {
                                TempBuffer.SetPixel( static_cast<Whole>(X), static_cast<Whole>(Y), this->CrackColour );
                                break;
                            }
                        }
                    }
                }

                Buffer.SetData(TempBuffer);
            }

            String CrackModifier::GetName() const
                { return "CrackModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            CrackModifier& CrackModifier::SetNormalsTexture(TextureBuffer* Normals)
            {
                this->NormalsTexture = Normals;
                return *this;
            }

            CrackModifier& CrackModifier::SetColour(const ColourValue& Colour)
            {
                this->CrackColour = Colour;
                return *this;
            }

            CrackModifier& CrackModifier::SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->CrackColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            CrackModifier& CrackModifier::SetCrackCount(const Whole Count)
            {
                this->CrackCount = Count;
                return *this;
            }

            CrackModifier& CrackModifier::SetCrackLengthMode(const LengthMode Length)
            {
                this->CrackLengthMode = Length;
                return *this;
            }

            CrackModifier& CrackModifier::SetCrackQualityMode(const QualityMode Quality)
            {
                this->CrackQualityMode = Quality;
                return *this;
            }

            CrackModifier& CrackModifier::SetCrackLength(const UInt8 Length)
            {
                this->CrackLength = Length;
                return *this;
            }

            CrackModifier& CrackModifier::SetCrackVariation(const UInt8 Variation)
            {
                this->CrackVariation = Variation;
                return *this;
            }

            CrackModifier& CrackModifier::SetGeneratorSeed(const Whole Seed)
            {
                this->GeneratorSeed = Seed;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
