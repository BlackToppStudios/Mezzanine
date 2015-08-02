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
#ifndef _graphicsproceduralcombinemodifier_cpp
#define _graphicsproceduralcombinemodifier_cpp

#include "Graphics/Procedural/Texture/combinemodifier.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            CombineModifier::CombineModifier() :
                CombineFilterColour(1.0,1.0,1.0,1.0)
                {  }

            CombineModifier::~CombineModifier()
                {  }

            void CombineModifier::ProcessLayer(const CombineLayer& Layer, TextureBuffer& Buffer)
            {
                Whole Width = Buffer.GetWidth();
                Whole Height = Buffer.GetHeight();

                if( Layer.TextureLayer->GetWidth() < Width || Layer.TextureLayer->GetHeight() < Height ) {
                    return;
                }

                Whole RedPercent = static_cast<Whole>( this->CombineFilterColour.RedChannel * 255.0 );
                Whole GreenPercent = static_cast<Whole>( this->CombineFilterColour.GreenChannel * 255.0 );
                Whole BluePercent = static_cast<Whole>( this->CombineFilterColour.BlueChannel * 255.0 );
                Whole AlphaPercent = static_cast<Whole>( this->CombineFilterColour.AlphaChannel * 255.0 );

                switch( Layer.Mode )
                {
                    case CombineModifier::CM_Add_Clamp:
                    {
                        for( Whole Y = 0 ; Y < Height ; ++Y )
                        {
                            for( Whole X = 0 ; X < Width ; ++X )
                            {
                                ColourValue LayerColour = Layer.TextureLayer->GetPixel(X,Y);
                                ColourValue TargetColour = Buffer.GetPixel(X,Y);
                                Whole Red = static_cast<Whole>( TargetColour.RedChannel * 255.0 ) + ( ( static_cast<Whole>( LayerColour.RedChannel * 255.0 ) * RedPercent ) >> 8 );
                                Whole Green = static_cast<Whole>( TargetColour.GreenChannel * 255.0 ) + ( ( static_cast<Whole>( LayerColour.GreenChannel * 255.0 ) * GreenPercent ) >> 8 );
                                Whole Blue = static_cast<Whole>( TargetColour.BlueChannel * 255.0 ) + ( ( static_cast<Whole>( LayerColour.BlueChannel * 255.0 ) * BluePercent ) >> 8 );
                                Buffer.SetPixelByte(X,Y, static_cast<UInt8>((Red < 255) ? Red : 255), static_cast<UInt8>((Green < 255) ? Green : 255), static_cast<UInt8>((Blue < 255) ? Blue : 255), static_cast<UInt8>( TargetColour.AlphaChannel * 255.0 ));
                            }
                        }
                        break;
                    }
                    case CombineModifier::CM_Add_Wrap:
                    {
                        for( Whole Y = 0 ; Y < Height ; ++Y )
                        {
                            for( Whole X = 0 ; X < Width ; ++X )
                            {
                                ColourValue LayerColour = Layer.TextureLayer->GetPixel(X,Y);
                                ColourValue TargetColour = Buffer.GetPixel(X,Y);
                                Whole Red = static_cast<Whole>( TargetColour.RedChannel * 255.0 ) + ( ( static_cast<Whole>( LayerColour.RedChannel * 255.0 ) * RedPercent ) >> 8 );
                                Whole Green = static_cast<Whole>( TargetColour.GreenChannel * 255.0 ) + ( ( static_cast<Whole>( LayerColour.GreenChannel * 255.0 ) * GreenPercent ) >> 8 );
                                Whole Blue = static_cast<Whole>( TargetColour.BlueChannel * 255.0 ) + ( ( static_cast<Whole>( LayerColour.BlueChannel * 255.0 ) * BluePercent ) >> 8 );
                                Buffer.SetPixelByte(X,Y, static_cast<UInt8>(Red % 255), static_cast<UInt8>(Green % 255), static_cast<UInt8>(Blue % 255), static_cast<UInt8>( TargetColour.AlphaChannel * 255.0 ));
                            }
                        }
                        break;
                    }
                    case CombineModifier::CM_Sub_Clamp:
                    {
                        for( Whole Y = 0 ; Y < Height ; ++Y )
                        {
                            for( Whole X = 0 ; X < Width ; ++X )
                            {
                                ColourValue LayerColour = Layer.TextureLayer->GetPixel(X,Y);
                                ColourValue TargetColour = Buffer.GetPixel(X,Y);
                                Integer Red = static_cast<Integer>( TargetColour.RedChannel * 255.0 ) - ( ( static_cast<Integer>( LayerColour.RedChannel * 255.0 ) * RedPercent ) >> 8 );
                                Integer Green = static_cast<Integer>( TargetColour.GreenChannel * 255.0 ) - ( ( static_cast<Integer>( LayerColour.GreenChannel * 255.0 ) * GreenPercent ) >> 8 );
                                Integer Blue = static_cast<Integer>( TargetColour.BlueChannel * 255.0 ) - ( ( static_cast<Integer>( LayerColour.BlueChannel * 255.0 ) * BluePercent ) >> 8 );
                                Buffer.SetPixelByte(X,Y, static_cast<UInt8>((Red > 0) ? Red : 0), static_cast<UInt8>((Green > 0) ? Green : 0), static_cast<UInt8>((Blue > 0) ? Blue : 0), static_cast<UInt8>( TargetColour.AlphaChannel * 255.0 ));
                            }
                        }
                        break;
                    }
                    case CombineModifier::CM_Sub_Wrap:
                    {
                        for( Whole Y = 0 ; Y < Height ; ++Y )
                        {
                            for( Whole X = 0 ; X < Width ; ++X )
                            {
                                ColourValue LayerColour = Layer.TextureLayer->GetPixel(X,Y);
                                ColourValue TargetColour = Buffer.GetPixel(X,Y);
                                Integer Red = static_cast<Integer>( TargetColour.RedChannel * 255.0 ) - ( ( static_cast<Integer>( LayerColour.RedChannel * 255.0 ) * RedPercent ) >> 8 );
                                Integer Green = static_cast<Integer>( TargetColour.GreenChannel * 255.0 ) - ( ( static_cast<Integer>( LayerColour.GreenChannel * 255.0 ) * GreenPercent ) >> 8 );
                                Integer Blue = static_cast<Integer>( TargetColour.BlueChannel * 255.0 ) - ( ( static_cast<Integer>( LayerColour.BlueChannel * 255.0 ) * BluePercent ) >> 8 );
                                Buffer.SetPixelByte(X,Y, static_cast<UInt8>(Red % 255), static_cast<UInt8>(Green % 255), static_cast<UInt8>(Blue % 255), static_cast<UInt8>( TargetColour.AlphaChannel * 255.0 ));
                            }
                        }
                        break;
                    }
                    case CombineModifier::CM_Multiply:
                    {
                        for( Whole Y = 0 ; Y < Height ; ++Y )
                        {
                            for( Whole X = 0 ; X < Width ; ++X )
                            {
                                ColourValue LayerColour = Layer.TextureLayer->GetPixel(X,Y);
                                ColourValue TargetColour = Buffer.GetPixel(X,Y);
                                Whole Red = static_cast<Whole>( TargetColour.RedChannel * 255.0 ) * ( ( static_cast<Whole>( LayerColour.RedChannel * 255.0 ) * RedPercent ) >> 8 );
                                Whole Green = static_cast<Whole>( TargetColour.GreenChannel * 255.0 ) * ( ( static_cast<Whole>( LayerColour.GreenChannel * 255.0 ) * GreenPercent ) >> 8 );
                                Whole Blue = static_cast<Whole>( TargetColour.BlueChannel * 255.0 ) * ( ( static_cast<Whole>( LayerColour.BlueChannel * 255.0 ) * BluePercent ) >> 8 );
                                Buffer.SetPixelByte(X,Y, static_cast<UInt8>(Red >> 8), static_cast<UInt8>(Green >> 8), static_cast<UInt8>(Blue >> 8), static_cast<UInt8>( TargetColour.AlphaChannel * 255.0 ));
                            }
                        }
                        break;
                    }
                    case CombineModifier::CM_Multiply_Double:
                    {
                        for( Whole Y = 0 ; Y < Height ; ++Y )
                        {
                            for( Whole X = 0 ; X < Width ; ++X )
                            {
                                ColourValue LayerColour = Layer.TextureLayer->GetPixel(X,Y);
                                ColourValue TargetColour = Buffer.GetPixel(X,Y);
                                Whole Red = static_cast<Whole>( TargetColour.RedChannel * 255.0 ) * ( ( static_cast<Whole>( LayerColour.RedChannel * 255.0 ) * RedPercent ) >> 8 );
                                Red >>= 7;
                                Whole Green = static_cast<Whole>( TargetColour.GreenChannel * 255.0 ) * ( ( static_cast<Whole>( LayerColour.GreenChannel * 255.0 ) * GreenPercent ) >> 8 );
                                Green >>= 7;
                                Whole Blue = static_cast<Whole>( TargetColour.BlueChannel * 255.0 ) * ( ( static_cast<Whole>( LayerColour.BlueChannel * 255.0 ) * BluePercent ) >> 8 );
                                Blue >>= 7;
                                Buffer.SetPixelByte(X,Y, static_cast<UInt8>((Red < 255) ? Red : 255), static_cast<UInt8>((Green < 255) ? Green : 255), static_cast<UInt8>((Blue < 255) ? Blue : 255), static_cast<UInt8>( TargetColour.AlphaChannel * 255.0 ));
                            }
                        }
                        break;
                    }
                    case CombineModifier::CM_Blend:
                    {
                        for( Whole Y = 0 ; Y < Height ; ++Y )
                        {
                            for( Whole X = 0 ; X < Width ; ++X )
                            {
                                ColourValue LayerColour = Layer.TextureLayer->GetPixel(X,Y);
                                ColourValue TargetColour = Buffer.GetPixel(X,Y);
                                Whole Red = static_cast<Whole>( TargetColour.RedChannel * 255.0 ) + ( ( static_cast<Whole>( LayerColour.RedChannel * 255.0 ) * RedPercent ) >> 8 );
                                Whole Green = static_cast<Whole>( TargetColour.GreenChannel * 255.0 ) + ( ( static_cast<Whole>( LayerColour.GreenChannel * 255.0 ) * GreenPercent ) >> 8 );
                                Whole Blue = static_cast<Whole>( TargetColour.BlueChannel * 255.0 ) + ( ( static_cast<Whole>( LayerColour.BlueChannel * 255.0 ) * BluePercent ) >> 8 );
                                Buffer.SetPixelByte(X,Y, static_cast<UInt8>(Red >> 1), static_cast<UInt8>(Green >> 1), static_cast<UInt8>(Blue >> 1), static_cast<UInt8>( TargetColour.AlphaChannel * 255.0 ));
                            }
                        }
                        break;
                    }
                    case CombineModifier::CM_Alpha:
                    {
                        for( Whole Y = 0 ; Y < Height ; ++Y )
                        {
                            for( Whole X = 0 ; X < Width ; ++X )
                            {
                                ColourValue LayerColour = Layer.TextureLayer->GetPixel(X,Y);
                                ColourValue TargetColour = Buffer.GetPixel(X,Y);
                                Whole Alpha = static_cast<Whole>( TargetColour.AlphaChannel * 255.0 ) + ( ( static_cast<Whole>( LayerColour.AlphaChannel * 255.0 ) * AlphaPercent ) >> 8 );
                                Buffer.SetAlphaByte(X,Y,static_cast<UInt8>( Alpha >> 1 ));
                            }
                        }
                        break;
                    }
                    default:
                    case CombineModifier::CM_Layer:
                    {
                        for( Whole Y = 0 ; Y < Height ; ++Y )
                        {
                            for( Whole X = 0 ; X < Width ; ++X )
                            {
                                ColourValue LayerColour = Layer.TextureLayer->GetPixel(X,Y);
                                ColourValue TargetColour = Buffer.GetPixel(X,Y);
                                Buffer.SetPixelByte(X,Y,
                                                  static_cast<UInt8>( LayerColour.RedChannel * LayerColour.AlphaChannel * 255.0 + TargetColour.RedChannel * 255.0 * ( 1.0 - LayerColour.AlphaChannel ) ),
                                                  static_cast<UInt8>( LayerColour.GreenChannel * LayerColour.AlphaChannel * 255.0 + TargetColour.GreenChannel * 255.0 * ( 1.0 - LayerColour.AlphaChannel ) ),
                                                  static_cast<UInt8>( LayerColour.BlueChannel * LayerColour.AlphaChannel * 255.0 + TargetColour.BlueChannel * 255.0 * ( 1.0 - LayerColour.AlphaChannel ) ),
                                                  static_cast<UInt8>( ( TargetColour.AlphaChannel - TargetColour.AlphaChannel * LayerColour.AlphaChannel ) * 255.0f + LayerColour.AlphaChannel * 255.0 ) );
                            }
                        }
                        break;
                    }
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void CombineModifier::Modify(TextureBuffer& Buffer)
            {
                for( CombineLayerIterator LayerIt = this->CombineLayers.begin() ; LayerIt != this->CombineLayers.end() ; ++LayerIt )
                {
                    CombineLayer& CurrLayer = (*LayerIt);
                    this->ProcessLayer(CurrLayer,Buffer);
                }
            }

            String CombineModifier::GetName() const
                { return "CombineModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            CombineModifier& CombineModifier::AddTexture(TextureBuffer* Layer, const CombineMode Mode)
            {
                if( Layer != NULL ) {
                    CombineLayer NewLayer;
                    NewLayer.TextureLayer = Layer;
                    NewLayer.Mode = Mode;
                    this->CombineLayers.push_back(NewLayer);
                }
                return *this;
            }

            CombineModifier& CombineModifier::RemoveAllTextures()
            {
                this->CombineLayers.clear();
                return *this;
            }

            CombineModifier& CombineModifier::SetFilterColour(const ColourValue& Colour)
            {
                this->CombineFilterColour = Colour;
                return *this;
            }

            CombineModifier& CombineModifier::SetFilterColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->CombineFilterColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
