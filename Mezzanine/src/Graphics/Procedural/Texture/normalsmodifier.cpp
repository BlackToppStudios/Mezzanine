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
#ifndef _graphicsproceduralnormalsmodifier_cpp
#define _graphicsproceduralnormalsmodifier_cpp

#include "Graphics/Procedural/Texture/normalsmodifier.h"

#include "mathtool.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            NormalsModifier::NormalsModifier() :
                Amplify(64)
                {  }

            NormalsModifier::~NormalsModifier()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void NormalsModifier::Modify(TextureBuffer& Buffer)
            {
                Whole Width = static_cast<Whole>( Buffer.GetWidth() );
                Whole Height = static_cast<Whole>( Buffer.GetHeight() );
                Real fAmp = static_cast<Real>( this->Amplify ) * 4.0f / 255.0f;
                TextureBuffer TempBuffer( Buffer );

                for( Whole Y = 0 ; Y < Height ; ++Y )
                {
                    for( Whole X = 0 ; X < Width ; ++X )
                    {
                        Whole XPrev = (X < 1) ? 0 : (X - 1) % Width;
                        Whole XNext = (X + 1) % Width;
                        Whole YPrev = (Y < 1) ? 0 : (Y - 1) % Height;
                        Whole YNext = (Y + 1) % Height;

                        //Y Sobel filter
                        Real fPix = static_cast<Real>( Buffer.GetRedByte(XPrev,YNext) );
                        Real dY  = fPix * -1.0f;
                        fPix = static_cast<Real>( Buffer.GetRedByte(X,YNext) );
                        dY += fPix * -2.0f;
                        fPix = static_cast<Real>( Buffer.GetRedByte(XNext,YNext) );
                        dY += fPix * -1.0f;
                        fPix = static_cast<Real>( Buffer.GetRedByte(XPrev,YPrev) );
                        dY += fPix * 1.0f;
                        fPix = static_cast<Real>( Buffer.GetRedByte(X,YPrev) );
                        dY += fPix * 2.0f;
                        fPix = static_cast<Real>( Buffer.GetRedByte(XNext,YPrev) );
                        dY += fPix * 1.0f;

                        //X Sobel filter
                        fPix = static_cast<Real>( Buffer.GetRedByte(XPrev,YPrev) );
                        Real dX  = fPix * -1.0f;
                        fPix = static_cast<Real>( Buffer.GetRedByte(XPrev,Y) );
                        dX += fPix * -2.0f;
                        fPix = static_cast<Real>( Buffer.GetRedByte(XPrev,YNext) );
                        dX += fPix * -1.0f;
                        fPix = static_cast<Real>( Buffer.GetRedByte(XNext,YPrev) );
                        dX += fPix * 1.0f;
                        fPix = static_cast<Real>( Buffer.GetRedByte(XNext,Y) );
                        dX += fPix * 2.0f;
                        fPix = static_cast<Real>( Buffer.GetRedByte(XNext,YNext) );
                        dX += fPix * 1.0f;

                        Real NormX = -dX * fAmp / 255.0f;
                        Real NormY = -dY * fAmp / 255.0f;
                        Real Norm = MathTools::Sqrt(NormX * NormX + NormY * NormY + 1.0f);
                        if( Norm > (Real)10e-6 ) {
                            Norm = 1.0f / Norm;
                        }else{
                            Norm = 0.0f;
                        }
                        NormX = (NormX * Norm) * 127.0f + 128.0f;
                        NormY = (NormY * Norm) * 127.0f + 128.0f;
                        Real NormZ = Norm * 127.0f + 128.0f;

                        TempBuffer.SetPixel( X, Y, ColourValue(
                                             static_cast<UInt8>( NormX ),
                                             static_cast<UInt8>( NormY ),
                                             static_cast<UInt8>( NormZ ),
                                             Buffer.GetAlphaByte(X,Y) ) );
                    }
                }

                Buffer.SetData(TempBuffer);
            }

            String NormalsModifier::GetName() const
                { return "NormalsModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            NormalsModifier& NormalsModifier::SetAmplify(const UInt8 Amp)
                { this->Amplify = Amp;  return *this; }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
