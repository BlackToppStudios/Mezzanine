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
#ifndef _graphicsproceduralrotationzoommodifier_cpp
#define _graphicsproceduralrotationzoommodifier_cpp

#include "Graphics/Procedural/Texture/rotationzoommodifier.h"

#include "MathTools/mathtools.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            RotationZoomModifier::RotationZoomModifier() :
                RotationCenter(0.5,0.5),
                ZoomFactor(1.0,1.0),
                Rotation(0.0)
                {  }

            RotationZoomModifier::~RotationZoomModifier()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void RotationZoomModifier::Modify(TextureBuffer& Buffer)
            {
                Whole Width = Buffer.GetWidth();
                Whole Height = Buffer.GetHeight();
                TextureBuffer TempBuffer( Buffer );

                Real fZoomX = MathTools::Pow( 0.5, this->ZoomFactor.X - 1 );
                Real fZoomY = MathTools::Pow( 0.5, this->ZoomFactor.Y - 1 );
                Real RotSin = MathTools::Sin( this->Rotation );
                Real RotCos = MathTools::Cos( this->Rotation );
                Real HalfWidth = static_cast<Real>(Width) * 0.5;
                Real HalfHeight = static_cast<Real>(Height) * 0.5;
                Real XSin = RotSin * -HalfWidth;
                Real XCos = RotCos * -HalfWidth;
                Real YSin = RotSin * -HalfHeight;
                Real YCos = RotCos * -HalfHeight;

                for( Whole Y = 0 ; Y < Buffer.GetHeight() ; ++Y )
                {
                    Real TexU = ( ( XCos - YSin ) * fZoomX ) + ( this->RotationCenter.X * static_cast<Real>(Width) );
                    Real TexV = ( ( XSin + YCos ) * fZoomY ) + ( this->RotationCenter.Y * static_cast<Real>(Height) );
                    for( Whole X = 0 ; X < Buffer.GetWidth() ; ++X )
                    {
                        Real uf = (TexU >= 0) ? (TexU - (Integer)TexU) : 1 + (TexU - (Integer)TexU);
                        Real vf = (TexV >= 0) ? (TexV - (Integer)TexV) : 1 + (TexV - (Integer)TexV);
                        Whole ut = (TexU >= 0) ? (Whole)TexU : (Whole)TexU - 1;
                        Whole vt = (TexV >= 0) ? (Whole)TexV : (Whole)TexV - 1;

                        ColourValue Texel = Buffer.GetPixel( vt % Height, ut % Width ) * ( 1.0 - uf ) * ( 1.0 - vf );
                        Texel += Buffer.GetPixel( vt % Height, ( ut + 1 ) % Width ) * uf * ( 1.0 - vf );
                        Texel += Buffer.GetPixel( ( vt + 1 ) % Height, ut % Width ) * ( 1.0 - uf ) * vf;
                        Texel += Buffer.GetPixel( ( vt + 1 ) % Height, ( ut + 1 ) % Width ) * uf * vf;
                        TempBuffer.SetPixel(X,Y,Texel);
                        TexU += RotCos * fZoomX;
                        TexV += RotSin * fZoomY;
                    }
                    YSin += RotSin;
                    YCos += RotCos;
                }

                Buffer.SetData(TempBuffer);
            }

            String RotationZoomModifier::GetName() const
                { return "RotationZoomModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            RotationZoomModifier& RotationZoomModifier::SetRotationCenter(const Vector2& Center)
                { this->RotationCenter = Center;  return *this; }

            RotationZoomModifier& RotationZoomModifier::SetRotationCenterX(const Real X)
                { this->RotationCenter.X = X;  return *this; }

            RotationZoomModifier& RotationZoomModifier::SetRotationCenterY(const Real Y)
                { this->RotationCenter.Y = Y;  return *this; }

            RotationZoomModifier& RotationZoomModifier::SetZoomFactor(const Vector2& Zoom)
                { this->ZoomFactor = Zoom;  return *this; }

            RotationZoomModifier& RotationZoomModifier::SetZoomFactorX(const Real X)
                { this->ZoomFactor.X = X;  return *this; }

            RotationZoomModifier& RotationZoomModifier::SetZoomFactorY(const Real Y)
                { this->ZoomFactor.Y = Y;  return *this; }

            RotationZoomModifier& RotationZoomModifier::SetRotation(const Real Rot)
                { this->Rotation = Rot;  return *this; }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
