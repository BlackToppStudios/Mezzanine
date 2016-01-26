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
#ifndef _graphicsproceduralflaremodifier_cpp
#define _graphicsproceduralflaremodifier_cpp

#include "Graphics/Procedural/Texture/flaremodifier.h"

#include "MathTools/mathtools.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            FlareModifier::FlareModifier() :
                FlareColour(1.0,1.0,1.0,1.0),
                FlareCenter(0.5,0.5),
                FlareRadius(0.5,0.5),
                FlareBrightness(1.0)
                {  }

            FlareModifier::~FlareModifier()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void FlareModifier::Modify(TextureBuffer& Buffer)
            {
                Integer TargetWidth = static_cast<Integer>( Buffer.GetWidth() );
                Integer TargetHeight = static_cast<Integer>( Buffer.GetHeight() );
                Integer CenterX	= static_cast<Integer>( this->FlareCenter.X * static_cast<Real>( TargetWidth ) );
                Integer CenterY	= static_cast<Integer>( this->FlareCenter.Y * static_cast<Real>( TargetHeight ) );
                Integer RadiusX	= static_cast<Integer>( this->FlareRadius.X * static_cast<Real>( TargetWidth ) );
                Integer RadiusY	= static_cast<Integer>( this->FlareRadius.Y * static_cast<Real>( TargetHeight ) );
                Real FlareRed = this->FlareColour.RedChannel * 255.0;
                Real FlareGreen = this->FlareColour.GreenChannel * 255.0;
                Real FlareBlue = this->FlareColour.BlueChannel * 255.0;
                Real InverseRadiusX = 1.0 / static_cast<Real>( RadiusX );
                Real InverseRadiusY = 1.0 / static_cast<Real>( RadiusY );

                for( Integer Y = 0 ; Y < TargetHeight ; ++Y )
                {
                    Real DeltaY = static_cast<Real>( Y - CenterY ) * InverseRadiusY;

                    for( Integer X = 0 ; X < TargetWidth ; ++X )
                    {
                        Real DeltaX = static_cast<Real>( X - CenterX ) * InverseRadiusX;
                        Real Distance = MathTools::Sqrt(DeltaX * DeltaX + DeltaY * DeltaY);
                        if( Distance >= 1.0 ) {
                            Distance = 0.0;
                        }else{
                            Distance = 1.0 - Distance;
                        }

                        Whole Red = static_cast<Whole>( static_cast<Real>( Buffer.GetRedByte(X,Y) ) + ( ( Distance * FlareRed ) * this->FlareBrightness ) );
                        Whole Green = static_cast<Whole>( static_cast<Real>( Buffer.GetGreenByte(X,Y) ) + ( ( Distance * FlareGreen ) * this->FlareBrightness ) );
                        Whole Blue = static_cast<Whole>( static_cast<Real>( Buffer.GetBlueByte(X,Y) ) + ( ( Distance * FlareBlue ) * this->FlareBrightness ) );
                        UInt8 Alpha = Buffer.GetAlphaByte(X,Y);

                        Buffer.SetPixelByte(X,Y, static_cast<UInt8>( (Red < 255) ? Red : 255 ),static_cast<UInt8>( (Green < 255) ? Green : 255 ),static_cast<UInt8>( (Blue < 255) ? Blue : 255 ),Alpha);
                    }
                }
            }

            String FlareModifier::GetName() const
                { return "FlareModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            FlareModifier& FlareModifier::SetColour(const ColourValue& Colour)
            {
                this->FlareColour = Colour;
                return *this;
            }

            FlareModifier& FlareModifier::SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->FlareColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            FlareModifier& FlareModifier::SetFlareCenter(const Vector2& Center)
            {
                this->FlareCenter = Center;
                return *this;
            }

            FlareModifier& FlareModifier::SetFlareCenter(const Real X, const Real Y)
            {
                this->FlareCenter.X = X;
                this->FlareCenter.Y = Y;
                return *this;
            }

            FlareModifier& FlareModifier::SetFlareCenterX(const Real X)
            {
                this->FlareCenter.X = X;
                return *this;
            }

            FlareModifier& FlareModifier::SetFlareCenterY(const Real Y)
            {
                this->FlareCenter.Y = Y;
                return *this;
            }

            FlareModifier& FlareModifier::SetFlareRadius(const Vector2& Radius)
            {
                this->FlareRadius = Radius;
                return *this;
            }

            FlareModifier& FlareModifier::SetFlareRadius(const Real X, const Real Y)
            {
                this->FlareRadius.X = X;
                this->FlareRadius.Y = Y;
                return *this;
            }

            FlareModifier& FlareModifier::SetFlareRadiusX(const Real X)
            {
                this->FlareRadius.X = X;
                return *this;
            }

            FlareModifier& FlareModifier::SetFlareRadiusY(const Real Y)
            {
                this->FlareRadius.Y = Y;
                return *this;
            }

            FlareModifier& FlareModifier::SetFlareBrightness(const Real Brightness)
            {
                this->FlareBrightness = Brightness;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
