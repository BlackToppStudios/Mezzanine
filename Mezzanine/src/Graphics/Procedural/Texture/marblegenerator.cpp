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
#ifndef _graphicsproceduralmarblegenerator_cpp
#define _graphicsproceduralmarblegenerator_cpp

#include "Graphics/Procedural/Texture/marblegenerator.h"

#include "Noise/Module/perlin.h"

#include "MathTools/mathtools.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            MarbleGenerator::MarbleGenerator() :
                GenColour(1.0,1.0,1.0,1.0),
                GenTurbulence(2.0),
                GenSeed(5120)
                {  }

            MarbleGenerator::~MarbleGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void MarbleGenerator::AddToTextureBuffer(TextureBuffer& Buffer) const
            {
                MathTools::MersenneTwisterGenerator32 NumGen(this->GenSeed);
                Whole RandNum = NumGen.GenerateUInt();
                Noise::Module::Perlin Noise;
                Noise.SetFrequency(1.0 / 32.0);
                Noise.SetLacunarity(1.5);
                Noise.SetOctaveCount(2);
                Noise.SetPersistence(0.15);
                Real XFact = ( 1.0 / 96.0 );
                Real YFact = ( 1.0 / 48.0 );

                for( Whole Y = 0 ; Y < Buffer.GetHeight() ; ++Y )
                {
                    for( Whole X = 0 ; X < Buffer.GetWidth() ; ++X )
                    {
                        Real NoiseVal = std::min( Real(1.0), MathTools::Abs( MathTools::Sin( X * XFact + Y * YFact + this->GenTurbulence * Noise.GetValue( X + RandNum, Y + RandNum, 0 ) ) * MathTools::GetPi() ) );
                        Buffer.SetRedByte( X, Y, (UInt8)std::min<Real>( this->GenColour.RedChannel * 255.0 * NoiseVal, 255.0 ) );
                        Buffer.SetGreenByte( X, Y, (UInt8)std::min<Real>( this->GenColour.GreenChannel * 255.0 * NoiseVal, 255.0 ) );
                        Buffer.SetBlueByte( X, Y, (UInt8)std::min<Real>( this->GenColour.BlueChannel * 255.0 * NoiseVal, 255.0 ) );
                        Buffer.SetAlphaReal( X, Y, this->GenColour.AlphaChannel );
                    }
                }
            }

            String MarbleGenerator::GetName() const
                { return "MarbleGenerator"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            MarbleGenerator& MarbleGenerator::SetColour(const ColourValue& Colour)
            {
                this->GenColour = Colour;
                return *this;
            }

            MarbleGenerator& MarbleGenerator::SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            MarbleGenerator& MarbleGenerator::SetTurbulence(const Real Turb)
            {
                this->GenTurbulence = Turb;
                return *this;
            }

            MarbleGenerator& MarbleGenerator::SetSeed(const Whole Seed)
            {
                this->GenSeed = Seed;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
