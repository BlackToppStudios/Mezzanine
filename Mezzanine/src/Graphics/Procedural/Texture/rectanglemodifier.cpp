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
#ifndef _graphicsproceduralrectanglemodifier_cpp
#define _graphicsproceduralrectanglemodifier_cpp

#include "Graphics/Procedural/Texture/rectanglemodifier.h"

#include "MathTools/mathtools.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            RectangleModifier::RectangleModifier() :
                FillColour(1.0,1.0,1.0,1.0),
                PositionXRel(0.0),
                PositionYRel(0.0),
                SizeXRel(1.0),
                SizeYRel(1.0),
                PositionXAdj(0),
                PositionYAdj(0),
                SizeXAdj(0),
                SizeYAdj(0)
                {  }

            RectangleModifier::~RectangleModifier()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void RectangleModifier::Modify(TextureBuffer& Buffer)
            {
                const Real Width = static_cast<Real>( Buffer.GetWidth() );
                const Real Height = static_cast<Real>( Buffer.GetHeight() );
                const Integer LeftMost = MathTools::Clamp( static_cast<Integer>( Width * this->PositionXRel ) + this->PositionXAdj, 0, static_cast<Integer>( Buffer.GetWidth() ) );
                const Integer RightMost = MathTools::Clamp( LeftMost + ( static_cast<Integer>( Width * this->SizeXRel ) + this->PositionYAdj ), 0, static_cast<Integer>( Buffer.GetWidth() ) );
                const Integer TopMost = MathTools::Clamp( static_cast<Integer>( Height * this->PositionYRel ) + this->SizeXAdj, 0, static_cast<Integer>( Buffer.GetHeight() ) );
                const Integer BottomMost = MathTools::Clamp( TopMost + ( static_cast<Integer>( Height * this->SizeYRel ) + this->SizeYAdj ), 0, static_cast<Integer>( Buffer.GetHeight() ) );

                for( Integer Y = TopMost ; Y <= BottomMost ; ++Y )
                {
                    for( Integer X = LeftMost ; X <= RightMost ; ++X )
                    {
                        Buffer.SetPixel(X,Y,this->FillColour);
                    }
                }
            }

            String RectangleModifier::GetName() const
                { return "RectangleModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            RectangleModifier& RectangleModifier::SetColour(const ColourValue& Colour)
                { this->FillColour = Colour;  return *this; }

            RectangleModifier& RectangleModifier::SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
                { this->FillColour.SetValues(Red,Green,Blue,Alpha);  return *this; }

            RectangleModifier& RectangleModifier::SetColour(const UInt8 Red, const UInt8 Green, const UInt8 Blue, const UInt8 Alpha)
                { this->FillColour.SetValues(static_cast<Real>(Red) / 255.0,static_cast<Real>(Green) / 255.0,static_cast<Real>(Blue) / 255.0,static_cast<Real>(Alpha) / 255.0);  return *this; }

            RectangleModifier& RectangleModifier::SetXPositionRel(const Real X)
                { this->PositionXRel = X;  return *this; }

            RectangleModifier& RectangleModifier::SetYPositionRel(const Real Y)
                { this->PositionYRel = Y;  return *this; }

            RectangleModifier& RectangleModifier::SetXSizeRel(const Real X)
                { this->SizeXRel = X;  return *this; }

            RectangleModifier& RectangleModifier::SetYSizeRel(const Real Y)
                { this->SizeYRel = Y;  return *this; }

            RectangleModifier& RectangleModifier::SetXPositionAdj(const Integer X)
                { this->PositionXAdj = X;  return *this; }

            RectangleModifier& RectangleModifier::SetYPositionAdj(const Integer Y)
                { this->PositionYAdj = Y;  return *this; }

            RectangleModifier& RectangleModifier::SetXSizeAdj(const Integer X)
                { this->SizeXAdj = X;  return *this; }

            RectangleModifier& RectangleModifier::SetYSizeAdj(const Integer Y)
                { this->SizeYAdj = Y;  return *this; }

            RectangleModifier& RectangleModifier::SetRectangle(const Real XPosition, const Real YPosition, const Real XSize, const Real YSize)
            {
                this->PositionXRel = XPosition;
                this->PositionYRel = YPosition;
                this->SizeXRel = XSize;
                this->SizeYRel = YSize;
                return *this;
            }

            RectangleModifier& RectangleModifier::SetRectangle(const Integer XPosition, const Integer YPosition, const Integer XSize, const Integer YSize)
            {
                this->PositionXAdj = XPosition;
                this->PositionYAdj = YPosition;
                this->SizeXAdj = XSize;
                this->SizeYAdj = YSize;
                return *this;
            }

            RectangleModifier& RectangleModifier::SetRectangle(const Real XPositionRel, const Real YPositionRel, const Real XSizeRel, const Real YSizeRel, const Integer XPositionAdj, const Integer YPositionAdj, const Integer XSizeAdj, const Integer YSizeAdj)
            {
                this->PositionXRel = XPositionRel;
                this->PositionYRel = YPositionRel;
                this->SizeXRel = XSizeRel;
                this->SizeYRel = YSizeRel;
                this->PositionXAdj = XPositionAdj;
                this->PositionYAdj = YPositionAdj;
                this->SizeXAdj = XSizeAdj;
                this->SizeYAdj = YSizeAdj;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
