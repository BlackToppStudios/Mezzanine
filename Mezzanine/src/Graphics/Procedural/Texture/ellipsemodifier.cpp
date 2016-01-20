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
#ifndef _graphicsproceduralellipsemodifier_cpp
#define _graphicsproceduralellipsemodifier_cpp

#include "Graphics/Procedural/Texture/ellipsemodifier.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            EllipseModifier::EllipseModifier() :
                FillColour(1.0,1.0,1.0,1.0),
                PositionXRel(0.5),
                PositionYRel(0.5),
                RadiusXRel(0.5),
                RadiusYRel(0.5),
                PositionXAdj(0),
                PositionYAdj(0),
                RadiusXAdj(0),
                RadiusYAdj(0)
                {  }

            EllipseModifier::~EllipseModifier()
                {  }

            void EllipseModifier::PutPixel(const Integer XPos, const Integer YPos, TextureBuffer& Buffer)
            {
                if( XPos < 0 || XPos >= static_cast<Integer>( Buffer.GetWidth() ) ) {
                    return;
                }
                if( YPos < 0 || YPos >= static_cast<Integer>( Buffer.GetHeight() ) ) {
                    return;
                }
                Buffer.SetPixel(XPos,YPos,this->FillColour);
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void EllipseModifier::Modify(TextureBuffer& Buffer)
            {
                const Integer TargetWidth = static_cast<Integer>( Buffer.GetWidth() );
                const Integer TargetHeight = static_cast<Integer>( Buffer.GetHeight() );
                const Integer XPos = ( ( this->PositionXRel * TargetWidth ) + this->PositionXAdj );
                const Integer YPos = ( ( this->PositionYRel * TargetHeight ) + this->PositionYAdj );
                const Integer XRadius = std::max( static_cast<Integer>( this->RadiusXRel * TargetWidth ) + this->RadiusXAdj, 0 );
                const Integer YRadius = std::max( static_cast<Integer>( this->RadiusYRel * TargetHeight ) + this->RadiusYAdj, 0 );
                const Integer SquaredXRadius = XRadius * XRadius;
                const Integer SquaredYRadius = YRadius * YRadius;

                // StackOverflow Logic
                {
                    const Integer SquaredDimensions = SquaredXRadius * SquaredYRadius;
                    Integer OuterMostX = XRadius;
                    Integer PrevDelta = 0;
                    // do the horizontal diameter
                    for( Integer CurrX = -XRadius ; CurrX <= XRadius ; ++CurrX )
                    {
                        this->PutPixel(XPos + CurrX,XPos,Buffer);
                    }

                    // now do both halves at the same time, away from the diameter
                    for( Integer CurrY = 1 ; CurrY <= YRadius ; ++CurrY )
                    {
                        Integer OuterMostXCheck = OuterMostX - ( PrevDelta - 1 );  // try slopes of dx - 1 or more
                        for(  ; OuterMostXCheck > 0 ; --OuterMostXCheck )
                        {
                            if( OuterMostXCheck * OuterMostXCheck * SquaredYRadius + CurrY * CurrY * SquaredXRadius <= SquaredDimensions ) {
                                break;
                            }
                        }
                        PrevDelta = OuterMostX - OuterMostXCheck;  // current approximation of the slope
                        OuterMostX = OuterMostXCheck;

                        for( Integer CurrX = -OuterMostX ; CurrX <= OuterMostX ; ++CurrX )
                        {
                            this->PutPixel(XPos + CurrX,YPos - CurrY,Buffer);
                            this->PutPixel(XPos + CurrX,YPos + CurrY,Buffer);
                        }
                    }
                }

                // Ogre Procedural Logic
                /*{
                    Integer DeltaX = 0;
                    Integer DeltaY = YRadius;
                    Integer err = SquaredYRadius - ( 2 * YRadius - 1 ) * SquaredXRadius;
                    Integer e2 = 0;

                    do
                    {
                        for( Integer CurrY = -DeltaY ; CurrY <= DeltaY ; ++CurrY )
                        {
                            this->PutPixel( XPos + (+DeltaX), YPos + CurrY, Buffer );
                            this->PutPixel( XPos + (-DeltaX), YPos + CurrY, Buffer );
                        }

                        e2 = 2 * err;
                        if( e2 <  ( 2 * DeltaX + 1 ) * SquaredYRadius ) {
                            DeltaX++;
                            err += ( 2 * DeltaX + 1 ) * SquaredYRadius;
                        }
                        if( e2 > -( 2 * DeltaY - 1 ) * SquaredXRadius ) {
                            DeltaY--;
                            err -= ( 2 * DeltaY - 1 ) * SquaredXRadius;
                        }
                    }
                    while( DeltaY >= 0 );

                    while( DeltaX++ < XRadius )
                    {
                        this->PutPixel( XPos + (+DeltaX), YPos, Buffer );
                        this->PutPixel( XPos + (-DeltaX), YPos, Buffer );
                    }
                }// */
            }

            String EllipseModifier::GetName() const
                { return "EllipseModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            EllipseModifier& EllipseModifier::SetColour(const ColourValue& Colour)
                { this->FillColour = Colour;  return *this; }

            EllipseModifier& EllipseModifier::SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
                { this->FillColour.SetValues(Red,Green,Blue,Alpha);  return *this; }

            EllipseModifier& EllipseModifier::SetColour(const UInt8 Red, const UInt8 Green, const UInt8 Blue, const UInt8 Alpha)
                { this->FillColour.SetValues(static_cast<Real>(Red) / 255.0,static_cast<Real>(Green) / 255.0,static_cast<Real>(Blue) / 255.0,static_cast<Real>(Alpha) / 255.0);  return *this; }

            EllipseModifier& EllipseModifier::SetEllipse(const Real XPosition, const Real YPosition, const Real XRadius, const Real YRadius)
            {
                this->PositionXRel = XPosition;
                this->PositionYRel = YPosition;
                this->RadiusXRel = XRadius;
                this->RadiusYRel = YRadius;
                return *this;
            }

            EllipseModifier& EllipseModifier::SetEllipse(const Integer XPosition, const Integer YPosition, const Integer XRadius, const Integer YRadius)
            {
                this->PositionXAdj = XPosition;
                this->PositionYAdj = YPosition;
                this->RadiusXAdj = XRadius;
                this->RadiusYAdj = YRadius;
                return *this;
            }

            EllipseModifier& EllipseModifier::SetEllipse(const Real XPositionRel, const Real YPositionRel, const Real XRadiusRel, const Real YRadiusRel, const Integer XPositionAdj, const Integer YPositionAdj, const Integer XRadiusAdj, const Integer YRadiusAdj)
            {
                this->PositionXRel = XPositionRel;
                this->PositionYRel = YPositionRel;
                this->RadiusXRel = XRadiusRel;
                this->RadiusYRel = YRadiusRel;
                this->PositionXAdj = XPositionAdj;
                this->PositionYAdj = YPositionAdj;
                this->RadiusXAdj = XRadiusAdj;
                this->RadiusYAdj = YRadiusAdj;
                return *this;
            }

            EllipseModifier& EllipseModifier::SetPosition(const Real XPositionRel, const Real YPositionRel, const Integer XPositionAdj, const Integer YPositionAdj)
            {
                this->PositionXRel = XPositionRel;
                this->PositionYRel = YPositionRel;
                this->PositionXAdj = XPositionAdj;
                this->PositionYAdj = YPositionAdj;
                return *this;
            }

            EllipseModifier& EllipseModifier::SetRadius(const Real XRadiusRel, const Real YRadiusRel, const Integer XRadiusAdj, const Integer YRadiusAdj)
            {
                this->RadiusXRel = XRadiusRel;
                this->RadiusYRel = YRadiusRel;
                this->RadiusXAdj = XRadiusAdj;
                this->RadiusYAdj = YRadiusAdj;
                return *this;
            }

            EllipseModifier& EllipseModifier::SetXPositionRel(const Real X)
                { this->PositionXRel = X;  return *this; }

            EllipseModifier& EllipseModifier::SetYPositionRel(const Real Y)
                { this->PositionYRel = Y;  return *this; }

            EllipseModifier& EllipseModifier::SetXRadiusRel(const Real X)
                { this->RadiusXRel = X;  return *this; }

            EllipseModifier& EllipseModifier::SetYRadiusRel(const Real Y)
                { this->RadiusYRel = Y;  return *this; }

            EllipseModifier& EllipseModifier::SetXPositionAdj(const Integer X)
                { this->PositionXAdj = X;  return *this; }

            EllipseModifier& EllipseModifier::SetYPositionAdj(const Integer Y)
                { this->PositionYAdj = Y;  return *this; }

            EllipseModifier& EllipseModifier::SetXRadiusAdj(const Integer X)
                { this->RadiusXAdj = X;  return *this; }

            EllipseModifier& EllipseModifier::SetYRadiusAdj(const Integer Y)
                { this->RadiusYAdj = Y;  return *this; }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
