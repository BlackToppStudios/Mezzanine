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
#ifndef _graphicsproceduralsegmentmodifier_cpp
#define _graphicsproceduralsegmentmodifier_cpp

#include "Graphics/Procedural/Texture/segmentmodifier.h"

#include <cstring>

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            SegmentModifier::SegmentModifier() :
                ColourSource(NULL),
                Threshold(128)
                {  }

            SegmentModifier::~SegmentModifier()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void SegmentModifier::Modify(TextureBuffer& Buffer)
            {
                Whole Width = Buffer.GetWidth();
                Whole Height = Buffer.GetHeight();

                if( this->ColourSource == NULL || ( this->ColourSource->GetWidth() < Width || this->ColourSource->GetHeight() < Height ) ) {
                    return;
                }

                UInt8* Coverage = new UInt8[Width * Height];
                memset(Coverage, 0, Width * Height);
                IntVector2* Stack = new IntVector2[Width * Height * 4];
                TextureBuffer TempBuffer( Buffer );

                size_t StackPtr = 0;
                for( Whole Y = 0 ; Y < Height ; ++Y )
                {
                    for( Whole X = 0 ; X < Width ; ++X )
                    {
                        ColourValue PixelA = Buffer.GetPixel(X,Y);
                        ColourValue PixelB = this->ColourSource->GetPixel(X,Y);

                        if( ( PixelA.RedChannel + PixelA.GreenChannel + PixelA.BlueChannel ) * 255.0 > static_cast<Real>(this->Threshold) * 3.0 ) {
                            Stack[StackPtr].X = X;
                            Stack[StackPtr].Y = Y;
                            StackPtr++;
                        }

                        while( StackPtr > 0 )
                        {
                            IntVector2 Current = Stack[--StackPtr];
                            if( Coverage[Current.X + Current.Y * Width] != 0 ) {
                                continue;
                            }

                            PixelA = Buffer.GetPixel(Current.X, Current.Y);
                            if( ( PixelA.RedChannel + PixelA.GreenChannel + PixelA.BlueChannel ) * 255.0 > static_cast<Real>(this->Threshold) * 3.0 ) {
                                Stack[StackPtr].X = Current.X;
                                Stack[StackPtr].Y = (Current.Y + Height - 1) % Height;
                                StackPtr++;
                                Stack[StackPtr].X = Current.X;
                                Stack[StackPtr].Y = (Current.Y + 1) % Height;
                                StackPtr++;
                                Stack[StackPtr].X = (Current.X + 1) % Width;
                                Stack[StackPtr].Y = Current.Y ;
                                StackPtr++;
                                Stack[StackPtr].X = (Current.X + Width - 1) % Width;
                                Stack[StackPtr].Y = Current.Y ;
                                StackPtr++;

                                Coverage[Current.X + Current.Y * Width] = 1;
                                TempBuffer.SetPixel(Current.X, Current.Y, PixelB);
                            }
                        }

                        if( Coverage[X + Y * Width] == 0 ) {
                            TempBuffer.SetPixel(X,Y,ColourValue::Black());
                        }
                    }
                }

                Buffer.SetData(TempBuffer);
                delete[] Coverage;
                delete[] Stack;
            }

            String SegmentModifier::GetName() const
                { return "SegmentModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            SegmentModifier& SegmentModifier::SetColourSource(TextureBuffer* Source)
                { this->ColourSource = Source;  return *this; }

            SegmentModifier& SegmentModifier::SetThreshold(const UInt8 Thresh)
                { this->Threshold = Thresh;  return *this; }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
