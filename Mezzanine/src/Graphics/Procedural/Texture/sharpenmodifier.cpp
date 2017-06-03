// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _graphicsproceduralsharpenmodifier_cpp
#define _graphicsproceduralsharpenmodifier_cpp

#include "Graphics/Procedural/Texture/sharpenmodifier.h"
#include "Graphics/Procedural/Texture/convolutionmodifier.h"

#include "MathTools/mathtools.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            SharpenModifier::SharpenModifier() :
                Type(ST_Normal),
                SharpenBlockSize(5),
                SharpenSigma(92)
                {  }

            SharpenModifier::~SharpenModifier()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void SharpenModifier::Modify(TextureBuffer& Buffer)
            {
                ConvolutionModifier Filter;
                switch(this->Type)
                {
                    default:
                    case ST_Normal:
                    {
                        Filter.SetKernel( Matrix3x3(0.0f,-1.0f,0.0f,-1.0f,5.0f,-1.0f,0.0f,-1.0f,0.0f) ).CalculateDivisor();
                        break;
                    }
                    case ST_Gaussian:
                    {
                        Real fSigma = 0.5f + ( (5.0f - 0.5f) / 255.0f ) * static_cast<Real>(this->SharpenSigma);
                        Integer Radius = static_cast<Integer>(this->SharpenBlockSize) / 2;
                        Real Min = MathTools::Exp( static_cast<Real>(2 * Radius * Radius) / (-2.0 * fSigma * fSigma) ) / (MathTools::GetTwoPi() * fSigma * fSigma);
                        Integer* Kernel = new Integer[ this->SharpenBlockSize * this->SharpenBlockSize ];
                        Integer Sum = 0;
                        Integer Y = -Radius;
                        Integer X = -Radius;
                        for( Integer i = 0 ; i < this->SharpenBlockSize ; i++ )
                        {
                            for( Integer j = 0 ; j < this->SharpenBlockSize ; j++ )
                            {
                                Kernel[ i * this->SharpenBlockSize + j ] = static_cast<Integer>( ( MathTools::Exp( static_cast<Real>( X * X + Y * Y ) / ( -2.0 * fSigma * fSigma ) ) / ( MathTools::GetTwoPi() * fSigma * fSigma ) ) / Min );
                                Sum += Kernel[ i * this->SharpenBlockSize + j ];
                                X++;
                            }
                            Y++;
                        }
                        Integer c = static_cast<Integer>(this->SharpenBlockSize) >> 1;
                        Integer Divisor = 0;
                        for( Integer i = 0 ; i < this->SharpenBlockSize ; i++ )
                        {
                            for( Integer j = 0 ; j < this->SharpenBlockSize ; j++ )
                            {
                                if( (i == c) && (j == c) ) {
                                    Kernel[ i * this->SharpenBlockSize + j ] = 2 * Sum - Kernel[ i * this->SharpenBlockSize + j ];
                                }else{
                                    Kernel[ i * this->SharpenBlockSize + j ] = -Kernel[ i * this->SharpenBlockSize + j ];
                                }
                                Divisor += Kernel[ i * this->SharpenBlockSize + j ];
                            }
                        }
                        Filter.SetKernel(this->SharpenBlockSize,Kernel).SetDivisor( static_cast<Real>(Divisor) );
                        delete[] Kernel;
                        break;
                    }
                }
                Filter.SetIncludeAlphaChannel(true).Modify(Buffer);
            }

            String SharpenModifier::GetName() const
                { return "SharpenModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            SharpenModifier& SharpenModifier::SetSharpenType(const SharpenType Sharpen)
                { this->Type = Sharpen;  return *this; }

            SharpenModifier& SharpenModifier::SetBlockSize(const UInt8 Size)
                { this->SharpenBlockSize = Size;  return *this; }

            SharpenModifier& SharpenModifier::SetSigma(const UInt8 Sigma)
                { this->SharpenSigma = Sigma;  return *this; }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
