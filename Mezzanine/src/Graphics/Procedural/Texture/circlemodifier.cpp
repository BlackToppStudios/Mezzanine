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
#ifndef _graphicsproceduralcirclemodifier_cpp
#define _graphicsproceduralcirclemodifier_cpp

#include "Graphics/Procedural/Texture/circlemodifier.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            CircleModifier::CircleModifier() :
                CircleColour(ColourValue::White()),
                CircleRadius(0),
                CircleX(0),
                CircleY(0)
                {  }

            CircleModifier::~CircleModifier()
                {  }

            void CircleModifier::PutPixel(const Whole DeltaX, const Whole DeltaY, TextureBuffer& Buffer)
            {
                if( this->CircleX + DeltaX < 0 || this->CircleX + DeltaX >= Buffer.GetWidth() ) {
                    return;
                }
                if( this->CircleY + DeltaY < 0 || this->CircleY + DeltaY >= Buffer.GetWidth() ) {
                    return;
                }
                Buffer.SetPixel(this->CircleX + DeltaX,this->CircleY + DeltaY,this->CircleColour);
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void CircleModifier::Modify(TextureBuffer& Buffer)
            {
                Whole X = 0;
                Whole Y = this->CircleRadius;
                Whole P = 3 - ( 2 * this->CircleRadius );
                while( X <= Y )
                {
                    for( Whole DeltaY = -Y ; DeltaY <= Y ; DeltaY++ )
                    {
                        this->PutPixel(+X,DeltaY,Buffer);
                        this->PutPixel(-X,DeltaY,Buffer);
                    }
                    for( Whole DeltaX = -X ; DeltaX <= X ; DeltaX++ )
                    {
                        this->PutPixel(+Y,DeltaX,Buffer);
                        this->PutPixel(-Y,DeltaX,Buffer);
                    }
                    if( P < 0 ) {
                        P += 4 * X++ + 6;
                    }else{
                        P += 4 * ( X++ - Y-- ) + 10;
                    }
                }
            }

            String CircleModifier::GetName() const
                { return "CircleModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            CircleModifier& CircleModifier::SetColour(const ColourValue& Colour)
            {
                this->CircleColour = Colour;
                return *this;
            }

            CircleModifier& CircleModifier::SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->CircleColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            CircleModifier& CircleModifier::SetRadius(const Whole Radius)
            {
                this->CircleRadius = Radius;
                return *this;
            }

            CircleModifier& CircleModifier::SetPosition(const Whole X, const Whole Y)
            {
                this->CircleX = X;
                this->CircleY = Y;
                return *this;
            }

            CircleModifier& CircleModifier::SetPositionX(const Whole X)
            {
                this->CircleX = X;
                return *this;
            }

            CircleModifier& CircleModifier::SetPositionY(const Whole Y)
            {
                this->CircleY = Y;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
