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
#ifndef _graphicsproceduralgradientgenerator_cpp
#define _graphicsproceduralgradientgenerator_cpp

#include "Graphics/Procedural/Texture/gradientgenerator.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            GradientGenerator::GradientGenerator() :
                GenTopLeftColour(0.0,0.0,1.0,1.0),
                GenTopRightColour(0.0,1.0,0.0,1.0),
                GenBottomLeftColour(1.0,0.0,0.0,1.0),
                GenBottomRightColour(0.0,1.0,1.0,1.0)
                {  }

            GradientGenerator::~GradientGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void GradientGenerator::AddToTextureBuffer(TextureBuffer& Buffer) const
            {
                Real finv_WH = 1.0 / static_cast<Real>( Buffer.GetWidth() * Buffer.GetHeight() );
                for( Whole Y = 0 ; Y < Buffer.GetHeight() ; ++Y )
                {
                    for( Whole X = 0 ; X < Buffer.GetWidth() ; ++X )
                    {
                        Real A = static_cast<Real>( ( Buffer.GetWidth() - X ) * (Buffer.GetHeight() - Y ) ) * finv_WH;
                        Real B = static_cast<Real>( X * (Buffer.GetHeight() - Y ) ) * finv_WH;
                        Real C = static_cast<Real>( ( Buffer.GetWidth() - X ) * Y ) * finv_WH;
                        Real D = static_cast<Real>( X * Y ) * finv_WH;

                        Buffer.SetRedByte( X, Y, (UInt8)( ( ( this->GenTopLeftColour.RedChannel * A ) + ( this->GenTopRightColour.RedChannel * B ) + ( this->GenBottomLeftColour.RedChannel * C ) + ( this->GenBottomRightColour.RedChannel * D ) ) * 255.0 ) );
                        Buffer.SetGreenByte( X, Y, (UInt8)( ( ( this->GenTopLeftColour.GreenChannel * A ) + ( this->GenTopRightColour.GreenChannel * B ) + ( this->GenBottomLeftColour.GreenChannel * C ) + ( this->GenBottomRightColour.GreenChannel * D ) ) * 255.0 ) );
                        Buffer.SetBlueByte( X, Y, (UInt8)( ( ( this->GenTopLeftColour.BlueChannel * A ) + ( this->GenTopRightColour.BlueChannel * B ) + ( this->GenBottomLeftColour.BlueChannel * C ) + ( this->GenBottomRightColour.BlueChannel * D ) ) * 255.0 ) );
                        Buffer.SetAlphaByte( X, Y, (UInt8)( ( ( this->GenTopLeftColour.AlphaChannel * A ) + ( this->GenTopRightColour.AlphaChannel * B ) + ( this->GenBottomLeftColour.AlphaChannel * C ) + ( this->GenBottomRightColour.AlphaChannel * D ) ) * 255.0 ) );
                    }
                }
            }

            String GradientGenerator::GetName() const
                { return "GradientGenerator"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            GradientGenerator& GradientGenerator::SetTopLeftColour(const ColourValue& Colour)
            {
                this->GenTopLeftColour = Colour;
                return *this;
            }

            GradientGenerator& GradientGenerator::SetTopLeftColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenTopLeftColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            GradientGenerator& GradientGenerator::SetTopRightColour(const ColourValue& Colour)
            {
                this->GenTopRightColour = Colour;
                return *this;
            }

            GradientGenerator& GradientGenerator::SetTopRightColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenTopRightColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            GradientGenerator& GradientGenerator::SetBottomLeftColour(const ColourValue& Colour)
            {
                this->GenBottomLeftColour = Colour;
                return *this;
            }

            GradientGenerator& GradientGenerator::SetBottomLeftColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenBottomLeftColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            GradientGenerator& GradientGenerator::SetBottomRightColour(const ColourValue& Colour)
            {
                this->GenBottomRightColour = Colour;
                return *this;
            }

            GradientGenerator& GradientGenerator::SetBottomRightColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenBottomRightColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            GradientGenerator& GradientGenerator::SetColours(const ColourValue& TopLeft, const ColourValue& TopRight, const ColourValue& BottomLeft, const ColourValue& BottomRight)
            {
                this->GenTopLeftColour = TopLeft;
                this->GenTopRightColour = TopRight;
                this->GenBottomLeftColour = BottomLeft;
                this->GenBottomRightColour = BottomRight;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
