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
#ifndef _graphicsproceduralcoloursmodifier_cpp
#define _graphicsproceduralcoloursmodifier_cpp

#include "Graphics/Procedural/Texture/coloursmodifier.h"

#include "MathTools/mathtools.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ColoursModifier::ColoursModifier() :
                ColourBase(0.0,0.0,0.0,1.0),
                ColourPercent(1.0,1.0,1.0,1.0),
                ColourBrightness(127),
                ColourContrast(127),
                ColourSaturation(127),
                ColourAlpha(127)
                {  }

            ColoursModifier::~ColoursModifier()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void ColoursModifier::Modify(TextureBuffer& Buffer)
            {
                Whole Width = Buffer.GetWidth();
                Whole Height = Buffer.GetHeight();
                Integer Brightness = ( ( static_cast<Integer>(this->ColourBrightness) ) * 2 ) - 256;
                Integer Contrast = static_cast<Integer>( this->ColourContrast );
                Real fConstrast = static_cast<Real>( this->ColourContrast ) / 128.0;
                fConstrast = fConstrast * fConstrast * fConstrast;
                Contrast = static_cast<Integer>( fConstrast * 256.0 );
                UInt8 MinAlpha = ( this->ColourAlpha > 127 ) ? static_cast<UInt8>( ( this->ColourAlpha - 127 ) * 2.0 - ( this->ColourAlpha - 127 ) / 128.0 ) : 0;
                UInt8 MaxAlpha = ( this->ColourAlpha < 127 ) ? static_cast<UInt8>( this->ColourAlpha * 2.0 + this->ColourAlpha / 127.0 ) : 255;
                Real AlphaMult = static_cast<Real>( MaxAlpha - MinAlpha ) / 255.0;

                for( Whole Y = 0 ; Y < Height ; ++Y )
                {
                    for( Whole X = 0 ; X < Width ; ++X )
                    {
                        Integer Red = static_cast<Integer>( this->ColourBase.RedChannel * 255.0 ) + ( ( static_cast<Integer>( Buffer.GetRedByte(X,Y) ) * static_cast<Integer>( this->ColourPercent.RedChannel * 255.0 ) ) >> 8 ) + Brightness;
                        Integer Green = static_cast<Integer>( this->ColourBase.GreenChannel * 255.0 ) + ( ( static_cast<Integer>( Buffer.GetGreenByte(X,Y) ) * static_cast<Integer>( this->ColourPercent.GreenChannel * 255.0 ) ) >> 8 ) + Brightness;
                        Integer Blue = static_cast<Integer>( this->ColourBase.BlueChannel * 255.0 ) + ( ( static_cast<Integer>( Buffer.GetBlueByte(X,Y) ) * static_cast<Integer>( this->ColourPercent.BlueChannel * 255.0 ) ) >> 8 ) + Brightness;

                        Integer Colour = static_cast<Integer>( ( ( Red - 127 ) * Contrast ) >> 8 ) + 127;
                        Red = (Colour < 0x00) ? 0x00 : (Colour > 0xff) ? 0xff : Colour;

                        Colour = static_cast<Integer>( ( ( Green - 127 ) * Contrast ) >> 8 ) + 127;
                        Green = (Colour < 0x00) ? 0x00 : (Colour > 0xff) ? 0xff : Colour;

                        Colour = static_cast<Integer>( ( ( Blue - 127 ) * Contrast ) >> 8 ) + 127;
                        Blue = (Colour < 0x00) ? 0x00 : (Colour > 0xff) ? 0xff : Colour;

                        if( this->ColourSaturation != 127 ) {
                            Integer Intensity = Red + Green + Blue;
                            Integer U = ( 3 * Red - Intensity ) * this->ColourSaturation / 127;
                            Integer V = ( 3 * Blue - Intensity ) * this->ColourSaturation / 127;
                            Red = ( U + Intensity ) / 3;
                            Green = ( Intensity - ( U + V ) ) / 3;
                            Blue = ( V + Intensity ) / 3;
                        }

                        Buffer.SetRedByte(X,Y,static_cast<UInt8>( MathTools::Clamp(Red,Integer(0),Integer(255)) ));
                        Buffer.SetGreenByte(X,Y,static_cast<UInt8>( MathTools::Clamp(Green,Integer(0),Integer(255)) ));
                        Buffer.SetBlueByte(X,Y,static_cast<UInt8>( MathTools::Clamp(Blue,Integer(0),Integer(255)) ));
                        Buffer.SetAlphaByte(X,Y,static_cast<UInt8>( MathTools::Clamp( ( static_cast<Real>( Buffer.GetAlphaByte(X,Y) ) * AlphaMult ) + static_cast<Real>( MinAlpha ),Real(0),Real(255)) ));
                    }
                }
            }

            String ColoursModifier::GetName() const
                { return "ColoursModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            ColoursModifier& ColoursModifier::SetColourBase(const ColourValue& Colour)
            {
                this->ColourBase = Colour;
                return *this;
            }

            ColoursModifier& ColoursModifier::SetColourBase(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->ColourBase.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            ColoursModifier& ColoursModifier::SetColourPercent(const ColourValue& Colour)
            {
                this->ColourPercent = Colour;
                return *this;
            }

            ColoursModifier& ColoursModifier::SetColourPercent(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->ColourPercent.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            ColoursModifier& ColoursModifier::SetBrightness(const UInt8 Brightness)
            {
                this->ColourBrightness = Brightness;
                return *this;
            }

            ColoursModifier& ColoursModifier::SetContrast(const UInt8 Contrast)
            {
                this->ColourContrast = Contrast;
                return *this;
            }

            ColoursModifier& ColoursModifier::SetSaturation(const UInt8 Saturation)
            {
                this->ColourSaturation = Saturation;
                return *this;
            }

            ColoursModifier& ColoursModifier::SetAlpha(const UInt8 Alpha)
            {
                this->ColourAlpha = Alpha;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
