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
#ifndef _graphicsproceduralblitmodifier_cpp
#define _graphicsproceduralblitmodifier_cpp

#include "Graphics/Procedural/Texture/blitmodifier.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            BlitModifier::BlitModifier() :
                BlitSrcTexture(NULL)
                {  }

            BlitModifier::~BlitModifier()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void BlitModifier::Modify(TextureBuffer& Buffer)
            {
                /// @todo This algorithm is pretty basic pixel fetch and makes no attempt at sampling pixels for the proper colour.  This should up updated to do some actual sampling.
                if( this->BlitSrcTexture != NULL ) {
                    WholeRect SrcRect = this->BlitSrcRect;
                    WholeRect DestRect = this->BlitDestRect;
                    SrcRect.ClampToSize(this->BlitSrcTexture->GetWidth(),this->BlitSrcTexture->GetHeight());
                    DestRect.ClampToSize(Buffer.GetWidth(),Buffer.GetHeight());

                    for( Whole Y = DestRect.Top ; Y < DestRect.Bottom ; ++Y )
                    {
                        for( Whole X = DestRect.Left ; X < DestRect.Right ; ++X )
                        {
                            Whole ResX = static_cast<Whole>( static_cast<Real>( X - DestRect.Left ) / static_cast<Real>( DestRect.GetWidth() ) * static_cast<Real>( SrcRect.GetWidth() ) + static_cast<Real>( SrcRect.Left ) );
                            Whole ResY = static_cast<Whole>( static_cast<Real>( Y - DestRect.Top ) / static_cast<Real>( DestRect.GetHeight() ) * static_cast<Real>( SrcRect.GetHeight() ) + static_cast<Real>( SrcRect.Top ) );
                            Buffer.SetPixel(X,Y,this->BlitSrcTexture->GetPixel(ResX,ResY));
                        }
                    }
                }
            }

            String BlitModifier::GetName() const
                { return "BlitModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            BlitModifier& BlitModifier::SetSrcTexture(TextureBuffer* SrcTexture)
            {
                this->BlitSrcTexture = SrcTexture;
                return *this;
            }

            BlitModifier& BlitModifier::SetSrcRect(const WholeRect& Rect)
            {
                this->BlitSrcRect = Rect;
                return *this;
            }

            BlitModifier& BlitModifier::SetSrcRect(const Whole RectLeft, const Whole RectTop, const Whole RectRight, const Whole RectBottom)
            {
                this->BlitSrcRect.SetValues(RectLeft,RectTop,RectRight,RectBottom);
                return *this;
            }

            BlitModifier& BlitModifier::SetDestRect(const WholeRect& Rect)
            {
                this->BlitDestRect = Rect;
                return *this;
            }

            BlitModifier& BlitModifier::SetDestRect(const Whole RectLeft, const Whole RectTop, const Whole RectRight, const Whole RectBottom)
            {
                this->BlitDestRect.SetValues(RectLeft,RectTop,RectRight,RectBottom);
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
