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
#ifndef _graphicsproceduralconvolutionmodifier_h
#define _graphicsproceduralconvolutionmodifier_h

#include "Graphics/Procedural/Texture/texturemodifier.h"

#include "matrix3x3.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A modifier that will calculate each pixel in an image to be a weighed sum of itself and it's neighbors.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB ConvolutionModifier : public TextureModifier
            {
            protected:
                /// @internal
                /// @brief The matricy to use for per-pixel processing.
                Real* KernelData;
                /// @internal
                /// @brief The custom value to divide the pixel result by.
                Real Divisor;
                /// @internal
                /// @brief The size of the set kernel.
                UInt8 KernelSize;
                /// @internal
                /// @brief A value over 128 will force a colour closer to White, lower will force a colour closer to black.
                UInt8 Threshold;
                /// @internal
                /// @brief Whether or not a dynamic divisor from pixel processing is to be used or the explicitly set divisor.
                Boole CalculateEdgeDivisor;
                /// @internal
                /// @brief Whether or not the Alpha channel is to be processed with the rest of the pixel.
                Boole IncludeAlphaChannel;
            public:
                /// @brief Blank constructor.
                ConvolutionModifier();
                /// @brief Class destructor.
                virtual ~ConvolutionModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the matrix that determines how each pixel neighbor is to be weighed when processing each pixel.
                /// @remarks This method calls CalculateDivisor() after updating the kernel.  If you want to have a custom divisor set be sure to call
                /// CalculateDivisor() AFTER the call to this method.
                /// @param Size The row size of the square kernel to be set.
                /// @param Data The matricy to use for per-pixel processing.
                /// @return Returns a reference to this.
                ConvolutionModifier& SetKernel(const UInt8 Size, Real* Data);
                /// @brief Sets the matrix that determines how each pixel neighbor is to be weighed when processing each pixel.
                /// @remarks This method calls CalculateDivisor() after updating the kernel.  If you want to have a custom divisor set be sure to call
                /// CalculateDivisor() AFTER the call to this method.
                /// @param Size The row size of the square kernel to be set.
                /// @param Data The matricy to use for per-pixel processing.
                /// @return Returns a reference to this.
                ConvolutionModifier& SetKernel(const UInt8 Size, Integer* Data);
                /// @brief Sets the matrix that determines how each pixel neighbor is to be weighed when processing each pixel.
                /// @remarks This method calls CalculateDivisor() after updating the kernel.  If you want to have a custom divisor set be sure to call
                /// CalculateDivisor() AFTER the call to this method.
                /// @param Data A 3x3 matrix to use for per-pixel processing.
                /// @return Returns a reference to this.
                ConvolutionModifier& SetKernel(const Matrix3x3& Data);
                /// @brief Sets a custom divisor that will be applied to the result of the pixel to help keep the value within a range.
                /// @param Div The custom value to divide the pixel result by.
                /// @return Returns a reference to this.
                ConvolutionModifier& SetDivisor(const Real Div);
                /// @brief Sets a threshold that will force the colours of each pixel further to white or black.
                /// @param Thresh The threshold to use.  Values over 128 will force a colour closer to White, lower will force a colour closer to black.
                /// @return Returns a reference to this.
                ConvolutionModifier& SetThreshold(const UInt8 Thresh);
                /// @brief Sets if a custom divisor should be use or one automatically generated during pixel processing.
                /// @param CalculateEdge Whether or not a dynamic divisor from pixel processing is to be used or the explicitly set divisor.
                /// @return Returns a reference to this.
                ConvolutionModifier& SetCalculateEdgeDivisor(const Boole CalculateEdge);
                /// @brief Sets if the alpha channel of each pixel should be processed.
                /// @param IncludeAlpha Whether or not the Alpha channel is to be processed with the rest of the pixel.
                /// @return Returns a reference to this.
                ConvolutionModifier& SetIncludeAlphaChannel(const Boole IncludeAlpha);
                /// @brief Automatically generates a sane divisor for pixel processing.
                /// @remarks This method will loop over every element of the kernal and assign the sum of all it's elements to the divisor.
                /// If for any reason the sum is zero a value of 1.0 will be assigned.
                /// @return Returns a reference to this.
                ConvolutionModifier& CalculateDivisor();
            };//ConvolutionModifier
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
