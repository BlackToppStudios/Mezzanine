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
#ifndef _graphicsproceduralblurmodifier_cpp
#define _graphicsproceduralblurmodifier_cpp

#include "Graphics/Procedural/Texture/blurmodifier.h"
#include "Graphics/Procedural/Texture/convolutionmodifier.h"

#include "mathtool.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            BlurModifier::BlurModifier() :
                Type(BT_Box),
                BlurBlockSize(5),
                BlurSigma(92)
                {  }

            BlurModifier::~BlurModifier()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void BlurModifier::Modify(TextureBuffer& Buffer)
            {
                ConvolutionModifier Filter;
                switch( this->Type )
                {
                    default:
                    case BT_Box:
                    {
                        Real BlurKernel[25] = {	1, 2, 3, 2, 1, 2, 4, 5, 4, 2, 3, 5, 6, 5, 3, 2, 4, 5, 4, 2, 1, 2, 3, 2, 1 };
                        Filter.SetKernel(5,BlurKernel);
                        break;
                    }
                    case BT_Mean:
                    {
                        Filter.SetKernel( Matrix3x3(1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f) ).CalculateDivisor();
                        break;
                    }
                    case BT_Gaussian:
                    {
                        Real fSigma = 0.5 + ( (3.0 - 0.5 ) / 255.0 ) * (Real)this->BlurSigma;
                        Integer Radius = static_cast<Integer>( this->BlurBlockSize / 2 );
                        Real Min = MathTools::Exp( static_cast<Real>( 2 * Radius * Radius ) / ( -2.0 * fSigma * fSigma ) ) / ( MathTools::GetTwoPi() * fSigma * fSigma );
                        Integer* Kernel = new Integer[this->BlurBlockSize * this->BlurBlockSize];
                        Integer Divisor = 0;
                        Integer Y = -Radius;
                        Integer X = -Radius;
                        for( Integer i = 0 ; i < this->BlurBlockSize ; ++i )
                        {
                            for( Integer j = 0 ; j < this->BlurBlockSize ; ++j )
                            {
                                Kernel[i * this->BlurBlockSize + j] = static_cast<Integer>( ( MathTools::Exp( (Real)( X * X + Y * Y ) / ( -2.0 * fSigma * fSigma ) ) / ( MathTools::GetTwoPi() * fSigma * fSigma ) ) / Min );
                                Divisor += Kernel[i * this->BlurBlockSize + j];
                                ++X;
                            }
                            ++Y;
                        }
                        Filter.SetKernel(this->BlurBlockSize,Kernel).SetDivisor( static_cast<Real>(Divisor) );
                        delete[] Kernel;
                        break;
                    }
                }
                Filter.SetIncludeAlphaChannel(true).Modify(Buffer);
            }

            String BlurModifier::GetName() const
                { return "BlurModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            BlurModifier& BlurModifier::SetBlurType(const BlurType Blur)
            {
                this->Type = Blur;
                return *this;
            }

            BlurModifier& BlurModifier::SetBlockSize(const UInt8 Size)
            {
                this->BlurBlockSize = Size;
                return *this;
            }

            BlurModifier& BlurModifier::SetSigma(const UInt8 Sigma)
            {
                this->BlurSigma = Sigma;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
