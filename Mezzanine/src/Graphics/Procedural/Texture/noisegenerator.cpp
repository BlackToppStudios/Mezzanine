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
#ifndef _graphicsproceduralnoisegenerator_cpp
#define _graphicsproceduralnoisegenerator_cpp

#include "Graphics/Procedural/Texture/noisegenerator.h"
#include "Graphics/Procedural/noise.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            NoiseGenerator::NoiseGenerator() :
                GenColour(1.0,1.0,1.0,1.0),
                GenSeed(5120),
                NType(Procedural::NT_White)
                {  }

            NoiseGenerator::~NoiseGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void NoiseGenerator::AddToTextureBuffer(TextureBuffer& Buffer) const
            {
                NoiseBase* NoiseGen = NULL;
                switch( this->NType )
                {
                    case Procedural::NT_Perlin:
                    {
                        NoiseGen = new PerlinNoise();
                        break;
                    }
                    case Procedural::NT_White:
                    default:
                    {
                        NoiseGen = new WhiteNoise(this->GenSeed);
                        break;
                    }
                }

                UInt8* Field = NoiseGen->Field2D( Buffer.GetWidth(), Buffer.GetHeight() );
                for( Whole Y = 0 ; Y < Buffer.GetHeight() ; ++Y )
                {
                    for( Whole X = 0 ; X < Buffer.GetWidth() ; ++X )
                    {
                        Real NoiseVal = (Real)Field[Y * Buffer.GetWidth() + X];
                        Buffer.SetRedByte( X, Y, (UInt8)( NoiseVal * this->GenColour.RedChannel ) );
                        Buffer.SetGreenByte( X, Y, (UInt8)( NoiseVal * this->GenColour.GreenChannel ) );
                        Buffer.SetBlueByte( X, Y, (UInt8)( NoiseVal * this->GenColour.BlueChannel ) );
                        Buffer.SetAlphaByte( X, Y, (UInt8)( this->GenColour.AlphaChannel * 255.0 ) );
                    }
                }

                delete[] Field;
                delete NoiseGen;
            }

            String NoiseGenerator::GetName() const
                { return "NoiseGenerator"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            NoiseGenerator& NoiseGenerator::SetColour(const ColourValue& Colour)
            {
                this->GenColour = Colour;
                return *this;
            }

            NoiseGenerator& NoiseGenerator::SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            NoiseGenerator& NoiseGenerator::SetSeed(const Whole Seed)
            {
                this->GenSeed = Seed;
                return *this;
            }

            NoiseGenerator& NoiseGenerator::SetType(const Procedural::NoiseType Type)
            {
                this->NType = Type;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
