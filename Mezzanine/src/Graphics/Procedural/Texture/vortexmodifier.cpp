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
#ifndef _graphicsproceduralvortexmodifier_cpp
#define _graphicsproceduralvortexmodifier_cpp

#include "Graphics/Procedural/Texture/vortexmodifier.h"

#include "MathTools/mathtools.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            VortexModifier::VortexModifier() :
                TwistAngle(0.0)
                {  }

            VortexModifier::~VortexModifier()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void VortexModifier::Modify(TextureBuffer& Buffer)
            {
                Whole Width = Buffer.GetWidth();
                Whole Height = Buffer.GetHeight();
                Whole dwCenterX	= static_cast<Whole>( this->VortexCenter.X * static_cast<Real>(Width) );
                Whole dwCenterY	= static_cast<Whole>( this->VortexCenter.Y * static_cast<Real>(Height) );
                Whole dwRadiusX	= static_cast<Whole>( this->VortexRadius.X * static_cast<Real>(Width) );
                Whole dwRadiusY	= static_cast<Whole>( this->VortexRadius.Y * static_cast<Real>(Height) );
                Real f1_RadiusX = 1.0 / static_cast<Real>(dwRadiusX);
                Real f1_RadiusY = 1.0 / static_cast<Real>(dwRadiusY);
                TextureBuffer TempBuffer( Buffer );

                for( Whole Y = 0 ; Y < Height ; ++Y )
                {
                    Real dY = static_cast<Real>( Y - dwCenterY ) * f1_RadiusY;

                    for( Whole X = 0 ; X < Width ; ++X )
                    {
                        Real dX = static_cast<Real>( X - dwCenterX ) * f1_RadiusX;
                        Real d = MathTools::Sqrt( dX * dX + dY * dY );

                        if( d > 1.0 ) {
                            TempBuffer.SetPixel( X, Y, Buffer.GetPixel(X,Y) );
                        }else{
                            d = MathTools::Cos( d * MathTools::GetHalfPi() - MathTools::GetHalfPi() );
                            d = 1.0 - d;
                            Real nX = static_cast<Real>( X - dwCenterX );
                            Real nY = static_cast<Real>( Y - dwCenterY );
                            Real rad = this->TwistAngle * d;

                            Real bX = nX;
                            nX = bX * MathTools::Cos(rad) - nY * MathTools::Sin(rad) + dwCenterX;
                            nY = bX * MathTools::Sin(rad) + nY * MathTools::Cos(rad) + dwCenterY;

                            if( nX >= Width ) nX = nX - Width;
                            if( nY >= Height ) nY = nY - Height;
                            if( nX < 0 ) nX = Width + nX;
                            if( nY < 0 ) nY = Height + nY;

                            Integer iX = static_cast<Integer>(nX);
                            Integer iY = static_cast<Integer>(nY);

                            Real fracX = nX - iX;
                            Real fracY = nY - iY;

                            Real ul = ( 1.0 - fracX ) * ( 1.0 - fracY );
                            Real ll = ( 1.0 - fracX ) * fracY;
                            Real ur = fracX * ( 1.0 - fracY );
                            Real lr = fracX * fracY;

                            Integer WrapX = ( iX + 1 ) % Width;
                            Integer WrapY = ( iY + 1 ) % Height;
                            ColourValue PixelUL = Buffer.GetPixel( iX, iY );
                            ColourValue PixelUR = Buffer.GetPixel( WrapX, iY );
                            ColourValue PixelLL = Buffer.GetPixel( iX, WrapY );
                            ColourValue PixelLR = Buffer.GetPixel( WrapX, WrapY );

                            TempBuffer.SetPixel( X, Y, ColourValue(
                                                 ( ul * PixelUL.RedChannel * 255.0 + ll * PixelLL.RedChannel * 255.0 + ur * PixelUR.RedChannel * 255.0 + lr * PixelLR.RedChannel * 255.0 ),
                                                 ( ul * PixelUL.GreenChannel * 255.0 + ll * PixelLL.GreenChannel * 255.0 + ur * PixelUR.GreenChannel * 255.0 + lr * PixelLR.GreenChannel * 255.0 ),
                                                 ( ul * PixelUL.BlueChannel * 255.0 + ll * PixelLL.BlueChannel * 255.0 + ur * PixelUR.BlueChannel * 255.0 + lr * PixelLR.BlueChannel * 255.0 ),
                                                 ( ul * PixelUL.AlphaChannel * 255.0 + ll * PixelLL.AlphaChannel * 255.0 + ur * PixelUR.AlphaChannel * 255.0 + lr * PixelLR.AlphaChannel * 255.0 ) ) );
                        }
                    }
                }

                Buffer.SetData(TempBuffer);
            }

            String VortexModifier::GetName() const
                { return "VortexModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            VortexModifier& VortexModifier::SetVortexCenter(const Vector2& Center)
                { this->VortexCenter = Center;  return *this; }

            VortexModifier& VortexModifier::SetVortexCenterX(const Real X)
                { this->VortexCenter.X = X;  return *this; }

            VortexModifier& VortexModifier::SetVortexCenterY(const Real Y)
                { this->VortexCenter.Y = Y;  return *this; }

            VortexModifier& VortexModifier::SetVortexRadius(const Vector2& Radius)
                { this->VortexRadius = Radius;  return *this; }

            VortexModifier& VortexModifier::SetVortexRadiusX(const Real X)
                { this->VortexRadius.X = X;  return *this; }

            VortexModifier& VortexModifier::SetVortexRadiusY(const Real Y)
                { this->VortexRadius.Y = Y;  return *this; }

            VortexModifier& VortexModifier::SetVortexTwist(const Real Twist)
                { this->TwistAngle = Twist;  return *this; }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
