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
#ifndef _graphicsproceduraltexturebuffer_cpp
#define _graphicsproceduraltexturebuffer_cpp

#include "Graphics/Procedural/texturebuffer.h"

#include "Graphics/texture.h"
#include "Graphics/image.h"

#include "mathtool.h"

#include <cstring>

#ifdef LoadImage
#undef LoadImage
#endif

/// @todo Change this so we're detecting the endianness in Mezzanine code, and adjusting this approproately.
namespace
{
    /// @enum ProceduralColour
    /// @brief Convenience enum that stores the colour placement in a word according to endianness.
    enum ProceduralColour
    {
#if MEZZ_LITTLE_ENDIAN
        PC_Red   = 3,
        PC_Green = 2,
        PC_Blue  = 1,
        PC_Alpha = 0
#endif
#if MEZZ_BIG_ENDIAN
        PC_Red   = 0,
        PC_Green = 1,
        PC_Blue  = 2,
        PC_Alpha = 3
#endif
    };
}

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            TextureBuffer::TextureBuffer(const Whole SquareSize) :
                Width( std::max(SquareSize,Whole(8)) ),
                Height( std::max(SquareSize,Whole(8)) )
            {
                this->Pixels = new UInt8[this->Width * this->Height * 4];
                memset(this->Pixels, 0, this->Width * this->Height * 4 * sizeof(UInt8));
                for( Whole Y = 0 ; Y < this->Height ; Y++ )
                {
                    for( Whole X = 0 ; X < this->Width ; X++ )
                    {
                        this->SetAlphaByte( X, Y, (UInt8)255 );
                    }
                }
            }

            TextureBuffer::TextureBuffer(const Whole TexWidth, const Whole TexHeight) :
                Width( std::max(TexWidth,Whole(8)) ),
                Height( std::max(TexHeight,Whole(8)) )
            {
                this->Pixels = new UInt8[this->Width * this->Height * 4];
                memset(this->Pixels, 0, this->Width * this->Height * 4 * sizeof(UInt8));
                for( Whole Y = 0 ; Y < this->Height ; Y++ )
                {
                    for( Whole X = 0 ; X < this->Width ; X++ )
                    {
                        this->SetAlphaByte( X, Y, (UInt8)255 );
                    }
                }
            }

            TextureBuffer::TextureBuffer(const TextureBuffer& Other)
            {
                this->Width = Other.GetWidth();
                this->Height = Other.GetHeight();

                this->Pixels = new UInt8[this->Width * this->Height * 4];
                memcpy(this->Pixels, Other.Pixels, this->Width * this->Height * 4 * sizeof(UInt8));
            }

            TextureBuffer::~TextureBuffer()
                { delete[] Pixels; }

            UInt8& TextureBuffer::GetPixel(const Whole X, const Whole Y, const UInt16 Component)
            {
                if( X >= this->Width || Y >= this->Height ) {
                    MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Requesting pixel position in TextureBuffer that is out of bounds.");
                }

                return this->Pixels[ Y * this->Width * 4 + X * 4 + Component ];
            }

            const UInt8& TextureBuffer::GetPixel(const Whole X, const Whole Y, const UInt16 Component) const
            {
                if( X >= this->Width || Y >= this->Height ) {
                    MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Requesting pixel position in TextureBuffer that is out of bounds.");
                }

                return this->Pixels[ Y * this->Width * 4 + X * 4 + Component ];
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void TextureBuffer::SetData(const Whole BuffWidth, const Whole BuffHeight, const UInt8* Buffer)
            {
                if( Buffer != NULL && this->Width == BuffWidth && this->Height == BuffHeight ) {
                    memcpy(this->Pixels,Buffer,this->Width * this->Height * 4 * sizeof(UInt8));
                }
            }

            void TextureBuffer::SetData(const TextureBuffer& Other)
            {
                if( Other.Pixels != NULL && this->Width == Other.Width && this->Height == Other.Height ) {
                    memcpy(this->Pixels,Other.Pixels,this->Width * this->Height * 4 * sizeof(UInt8));
                }
            }

            Whole TextureBuffer::GetWidth() const
                { return this->Width; }

            Whole TextureBuffer::GetHeight() const
                { return this->Height; }

            Image* TextureBuffer::GenerateImage() const
            {
                Whole ImageSize = this->Width * this->Height * 4;
                UInt8* NewBuff = new UInt8[ ImageSize ];
                memcpy( NewBuff, this->Pixels, ImageSize );
                Image* NewImage = new Image(NewBuff,this->Width,this->Height,Graphics::PF_R8G8B8A8,true);
                return NewImage;
            }

            Texture* TextureBuffer::GenerateTexture(const String& TexName, const String& TexGroup) const
            {
                /// @todo Implement this.
                MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Converting to an Texture is reliant on the completion of the Texture class.");
                return NULL;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Pixel Configuration

            void TextureBuffer::SetPixel(const Whole X, const Whole Y, const ColourValue& Colour)
            {
                this->SetPixelReal(X,Y,Colour.RedChannel,Colour.GreenChannel,Colour.BlueChannel,Colour.AlphaChannel);
            }

            void TextureBuffer::SetPixelByte(const Whole X, const Whole Y, const UInt8 Red, const UInt8 Green, const UInt8 Blue, const UInt8 Alpha)
            {
                this->SetRedByte(X,Y,Red);
                this->SetGreenByte(X,Y,Green);
                this->SetBlueByte(X,Y,Blue);
                this->SetAlphaByte(X,Y,Alpha);
            }

            void TextureBuffer::SetPixelReal(const Whole X, const Whole Y, const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->SetRedReal(X,Y,Red);
                this->SetGreenReal(X,Y,Green);
                this->SetBlueReal(X,Y,Blue);
                this->SetAlphaReal(X,Y,Alpha);
            }

            ColourValue TextureBuffer::GetPixel(const Whole X, const Whole Y) const
                { return ColourValue( this->GetRedReal(X,Y), this->GetGreenReal(X,Y), this->GetBlueReal(X,Y), this->GetAlphaReal(X,Y) ); }

            void TextureBuffer::SetRedByte(const Whole X, const Whole Y, const UInt8 Red)
                { this->GetPixel(X,Y,PC_Red) = Red; }

            void TextureBuffer::SetRedReal(const Whole X, const Whole Y, const Real Red)
            {
                if( Red > 1.0 || Red < 0.0 )
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Red colour component to be set is outside the expected range [0.0-1.0]."); }
                this->GetPixel(X,Y,PC_Red) = static_cast<UInt8>( MathTools::Clamp(Red * Real(255.0),Real(0.0),Real(255.0)) );
            }

            UInt8 TextureBuffer::GetRedByte(const Whole X, const Whole Y) const
                { return this->GetPixel(X,Y,PC_Red); }

            Real TextureBuffer::GetRedReal(const Whole X, const Whole Y) const
                { return ( static_cast<Real>( this->GetPixel(X,Y,PC_Red) ) / 255.0 ); }

            void TextureBuffer::SetGreenByte(const Whole X, const Whole Y, const UInt8 Green)
                { this->GetPixel(X,Y,PC_Green) = Green; }

            void TextureBuffer::SetGreenReal(const Whole X, const Whole Y, const Real Green)
            {
                if( Green > 1.0 || Green < 0.0 )
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Green colour component to be set is outside the expected range [0.0-1.0]."); }
                this->GetPixel(X,Y,PC_Green) = static_cast<UInt8>( MathTools::Clamp(Green * Real(255.0),Real(0.0),Real(255.0)) );
            }

            UInt8 TextureBuffer::GetGreenByte(const Whole X, const Whole Y) const
                { return this->GetPixel(X,Y,PC_Green); }

            Real TextureBuffer::GetGreenReal(const Whole X, const Whole Y) const
                { return ( static_cast<Real>( this->GetPixel(X,Y,PC_Green) ) / 255.0 ); }

            void TextureBuffer::SetBlueByte(const Whole X, const Whole Y, const UInt8 Blue)
                { this->GetPixel(X,Y,PC_Blue) = Blue; }

            void TextureBuffer::SetBlueReal(const Whole X, const Whole Y, const Real Blue)
            {
                if( Blue > 1.0 || Blue < 0.0 )
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Blue colour component to be set is outside the expected range [0.0-1.0]."); }
                this->GetPixel(X,Y,PC_Blue) = static_cast<UInt8>( MathTools::Clamp(Blue * Real(255.0),Real(0.0),Real(255.0)) );
            }

            UInt8 TextureBuffer::GetBlueByte(const Whole X, const Whole Y) const
                { return this->GetPixel(X,Y,PC_Blue); }

            Real TextureBuffer::GetBlueReal(const Whole X, const Whole Y) const
                { return ( static_cast<Real>( this->GetPixel(X,Y,PC_Blue) ) / 255.0 ); }

            void TextureBuffer::SetAlphaByte(const Whole X, const Whole Y, const UInt8 Alpha)
                { this->GetPixel(X,Y,PC_Alpha) = Alpha; }

            void TextureBuffer::SetAlphaReal(const Whole X, const Whole Y, const Real Alpha)
            {
                if( Alpha > 1.0 || Alpha < 0.0 )
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Alpha colour component to be set is outside the expected range [0.0-1.0]."); }
                this->GetPixel(X,Y,PC_Alpha) = static_cast<UInt8>( MathTools::Clamp(Alpha * Real(255.0),Real(0.0),Real(255.0)) );
            }

            UInt8 TextureBuffer::GetAlphaByte(const Whole X, const Whole Y) const
                { return this->GetPixel(X,Y,PC_Alpha); }

            Real TextureBuffer::GetAlphaReal(const Whole X, const Whole Y) const
                { return ( static_cast<Real>( this->GetPixel(X,Y,PC_Alpha) ) / 255.0 ); }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
