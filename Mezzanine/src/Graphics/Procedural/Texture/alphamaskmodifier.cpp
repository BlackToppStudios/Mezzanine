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
#ifndef _graphicsproceduralalphamaskmodifier_cpp
#define _graphicsproceduralalphamaskmodifier_cpp

#include "Graphics/Procedural/Texture/alphamaskmodifier.h"

#include "vector3.h"
#include "exception.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            AlphaMaskModifier::AlphaMaskModifier() :
                MaskTexture(NULL),
                ColourMask(false)
                {  }

            AlphaMaskModifier::~AlphaMaskModifier()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void AlphaMaskModifier::Modify(TextureBuffer& Buffer)
            {
                Whole MaskWidth = this->MaskTexture->GetWidth();
                Whole MaskHeight = this->MaskTexture->GetHeight();
                Whole TargetWidth = Buffer.GetWidth();
                Whole TargetHeight = Buffer.GetHeight();

                if( MaskWidth != TargetWidth && MaskHeight != TargetHeight ) {
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"The MaskTexture size and the target texture size must be the same.");
                }

                for( Whole Y = 0 ; Y < TargetHeight ; ++Y )
                {
                    for( Whole X = 0 ; X < TargetWidth ; ++X )
                    {
                        if( this->MaskTexture != NULL ) {
                            if( this->ColourMask ) {
                                ColourValue PixelA = Buffer.GetPixel(X,Y);
                                ColourValue PixelB = this->MaskTexture->GetPixel(X,Y);
                                Vector3 Colour1( PixelA.RedChannel * 255.0, PixelA.GreenChannel * 255.0, PixelA.BlueChannel * 255.0 );
                                Vector3 Colour2( PixelB.RedChannel * 255.0, PixelB.GreenChannel * 255.0, PixelB.BlueChannel * 255.0 );

                                Colour1.Normalize();
                                Colour2.Normalize();

                                Real Correctness = 0;

                                if( !Colour1.IsZeroLength() && !Colour2.IsZeroLength() ) {
                                    Correctness = Colour1.X * Colour2.X + Colour1.Y * Colour2.Y + Colour1.Z * Colour2.Z;
                                }

                                Buffer.SetAlphaByte(X,Y,static_cast<UInt8>(PixelA.AlphaChannel * Correctness));
                            }else{
                                ColourValue Pixel = this->MaskTexture->GetPixel(X,Y);
                                Real Alpha = ( Pixel.RedChannel + Pixel.GreenChannel + Pixel.BlueChannel ) / 3.0;
                                Buffer.SetAlphaReal(X,Y,Buffer.GetAlphaReal(X,Y) * Alpha);
                            }
                        }else{
                            UInt8 Alpha = Buffer.GetAlphaByte(X,Y);
                            Buffer.SetPixelByte(X,Y,Alpha,Alpha,Alpha,255);
                        }
                    }
                }
            }

            String AlphaMaskModifier::GetName() const
                { return "AlphaMaskModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            AlphaMaskModifier& AlphaMaskModifier::SetMaskTexture(TextureBuffer* Mask)
                { this->MaskTexture = Mask;  return *this; }

            AlphaMaskModifier& AlphaMaskModifier::SetColourMask(const Boole UseColour)
                { this->ColourMask = UseColour;  return *this; }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
