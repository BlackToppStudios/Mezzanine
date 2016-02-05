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
#ifndef _graphicsproceduralconvolutionmodifier_cpp
#define _graphicsproceduralconvolutionmodifier_cpp

#include "Graphics/Procedural/Texture/convolutionmodifier.h"

#include <cstring>

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ConvolutionModifier::ConvolutionModifier() :
                KernelData(NULL),
                Divisor(1.0),
                KernelSize(3),
                Threshold(128),
                CalculateEdgeDivisor(true),
                IncludeAlphaChannel(false)
            {
                Integer Mid = 2;
                if( ( this->KernelSize % 2 ) == 0 ) {
                    Mid = this->KernelSize / 2;
                }else{
                    Mid = ( ( this->KernelSize - 1 ) / 2 ) + 1;
                }
                Whole SquaredSize = this->KernelSize * this->KernelSize;
                this->KernelData = new Real[ SquaredSize ];
                memset(this->KernelData,0,SquaredSize * sizeof(Real));
                this->KernelData[ this->KernelSize * Mid + Mid ] = 1.0;
            }

            ConvolutionModifier::~ConvolutionModifier()
            {
                if( this->KernelData != NULL ) {
                    delete[] this->KernelData;
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void ConvolutionModifier::Modify(TextureBuffer& Buffer)
            {
                Whole TargetWidth = Buffer.GetWidth();
                Whole TargetHeight = Buffer.GetHeight();
                Integer Radius = static_cast<Integer>( this->KernelSize ) >> 1;
                TextureBuffer TempBuffer( Buffer );

                for( Whole Y = 0 ; Y < TargetHeight ; ++Y )
                {
                    for( Whole X = 0 ; X < TargetWidth ; ++X )
                    {
                        Integer Red = 0;
                        Integer Green = 0;
                        Integer Blue = 0;
                        Integer Alpha = 0;
                        Integer Div = 0;
                        Integer ProcessedKernelSize = 0;

                        for( UInt8 i = 0 ; i < this->KernelSize ; ++i )
                        {
                            Integer iRad = i - Radius;
                            Integer YPos = static_cast<Integer>(Y) + iRad;

                            if( YPos < 0 )
                                continue;
                            if( YPos >= static_cast<Integer>( TargetHeight ) )
                                break;

                            for( UInt8 j = 0 ; j < this->KernelSize ; ++j )
                            {
                                Integer jRad = j - Radius;
                                Integer XPos = static_cast<Integer>(X) + jRad;

                                if( XPos < 0 )
                                    continue;
                                if( XPos < static_cast<Integer>( TargetWidth ) ) {
                                    Real KernelVal = this->KernelData[ i * this->KernelSize + j ];
                                    ColourValue Pixel = Buffer.GetPixel(XPos,YPos);
                                    Div += static_cast<Integer>( KernelVal );
                                    KernelVal *= 255.0;
                                    Red += static_cast<Integer>( KernelVal * Pixel.RedChannel );
                                    Green += static_cast<Integer>( KernelVal * Pixel.GreenChannel );
                                    Blue += static_cast<Integer>( KernelVal * Pixel.BlueChannel );
                                    Alpha += static_cast<Integer>( KernelVal * Pixel.AlphaChannel );

                                    ProcessedKernelSize++;
                                }
                            }
                        }

                        if( ProcessedKernelSize == ( this->KernelSize * this->KernelSize ) ) {
                            Div = static_cast<Integer>( this->Divisor );
                        }else{
                            if( !this->CalculateEdgeDivisor ) {
                                Div = static_cast<Integer>( this->Divisor );
                            }
                        }

                        if( Div != 0 ) {
                            Red /= Div;
                            Green /= Div;
                            Blue /= Div;
                            Alpha /= Div;
                        }
                        Red += ( static_cast<Integer>( this->Threshold ) - 128 );
                        Green += ( static_cast<Integer>( this->Threshold ) - 128 );
                        Blue += ( static_cast<Integer>( this->Threshold ) - 128 );
                        if( this->IncludeAlphaChannel ) {
                            Alpha += ( static_cast<Integer>( this->Threshold ) - 128 );
                        }else{
                            Alpha = static_cast<Integer>( Buffer.GetAlphaByte(X,Y) );
                        }

                        /// @todo The X and Y here was originally swapped when I transposed the code from Ogre Procedural.  It didn't seem right so I changed it.  Who is wronger?
                        TempBuffer.SetPixelByte( X, Y,
                                            static_cast<UInt8>( (Red > 255) ? 255 : ( (Red < 0) ? 0 : Red ) ),
                                            static_cast<UInt8>( (Green > 255) ? 255 : ( (Green < 0) ? 0 : Green ) ),
                                            static_cast<UInt8>( (Blue > 255) ? 255 : ( (Blue < 0) ? 0 : Blue ) ),
                                            static_cast<UInt8>( (Alpha > 255) ? 255 : ( (Alpha < 0) ? 0 : Alpha ) ) );
                    }
                }

                Buffer.SetData(TempBuffer);
            }

            String ConvolutionModifier::GetName() const
                { return "ConvolutionModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            ConvolutionModifier& ConvolutionModifier::SetKernel(const UInt8 Size, Real* Data)
            {
                if( Size < 3 || Size % 2 == 0 ) {
                    return *this;
                }
                delete[] this->KernelData;
                this->KernelSize = Size;
                this->KernelData = new Real[this->KernelSize * this->KernelSize];
                for( Integer Y = 0 ; Y < this->KernelSize ; ++Y )
                {
                    for( Integer X = 0 ; X < this->KernelSize ; ++X )
                    {
                        this->KernelData[ Y * this->KernelSize + X ] = Data[ Y * this->KernelSize + X ];
                    }
                }
                this->CalculateDivisor();
                return *this;
            }

            ConvolutionModifier& ConvolutionModifier::SetKernel(const UInt8 Size, Integer* Data)
            {
                if( Size < 3 || Size % 2 == 0 ) {
                    return *this;
                }
                delete[] this->KernelData;
                this->KernelSize = Size;
                this->KernelData = new Real[ this->KernelSize * this->KernelSize ];
                for( Integer Y = 0 ; Y < this->KernelSize ; ++Y )
                {
                    for( Integer X = 0 ; X < this->KernelSize ; ++X )
                    {
                        this->KernelData[ Y * this->KernelSize + X ] = static_cast<Real>( Data[ Y * this->KernelSize + X ] );
                    }
                }
                this->CalculateDivisor();
                return *this;
            }

            ConvolutionModifier& ConvolutionModifier::SetKernel(const Matrix3x3& Data)
            {
                delete[] this->KernelData;
                this->KernelSize = 3;
                this->KernelData = new Real[ this->KernelSize * this->KernelSize ];
                for( Integer Y = 0 ; Y < this->KernelSize ; ++Y )
                {
                    for( Integer X = 0 ; X < this->KernelSize ; ++X )
                    {
                        this->KernelData[ Y * this->KernelSize + X ] = Data[Y][X];
                    }
                }
                this->CalculateDivisor();
                return *this;
            }

            ConvolutionModifier& ConvolutionModifier::SetDivisor(const Real Div)
                { this->Divisor = Div;  return *this; }

            ConvolutionModifier& ConvolutionModifier::SetThreshold(const UInt8 Thresh)
                { this->Threshold = Thresh;  return *this; }

            ConvolutionModifier& ConvolutionModifier::SetCalculateEdgeDivisor(const Boole CalculateEdge)
                { this->CalculateEdgeDivisor = CalculateEdge;  return *this; }

            ConvolutionModifier& ConvolutionModifier::SetIncludeAlphaChannel(const Boole IncludeAlpha)
                { this->IncludeAlphaChannel = IncludeAlpha;  return *this; }

            ConvolutionModifier& ConvolutionModifier::CalculateDivisor()
            {
                this->Divisor = 0.0;
                for( Integer Y = 0 ; Y < this->KernelSize ; ++Y )
                {
                    for( Integer X = 0 ; X < this->KernelSize ; ++X )
                    {
                        this->Divisor += this->KernelData[ Y * this->KernelSize + X ];
                    }
                }
                if( this->Divisor == 0.0 ) {
                    this->Divisor = 1.0;
                }

                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
