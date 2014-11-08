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
#ifndef _graphicsproceduralnoise_cpp
#define _graphicsproceduralnoise_cpp

#include "Graphics/Procedural/noise.h"

#include "MathTools/mathtools.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            // NoiseBase Methods

            UInt8* NoiseBase::Field1D(const Whole WX)
            {
                UInt8* RetArray = new UInt8[WX];
                // Set up what will be used as the known range of values for the noise
                PreciseReal MinNoise = 9999999999.9;
                PreciseReal MaxNoise = -999999999.9;
                Real* TempField = new Real[WX];
                // For each spot in the array, generate the noise and then assign known limits appropriately
                for( Whole XDim = 0 ; XDim < WX ; ++XDim )
                {
                    Real NoiseVal = this->Noise1D(XDim);
                    if( NoiseVal < MinNoise )
                        MinNoise = NoiseVal;
                    if( NoiseVal > MaxNoise )
                        MaxNoise = NoiseVal;
                    TempField[XDim] = NoiseVal;
                }
                // For each spot in the return array, convert the generated noise value into a value between 0-255 based on it's position in the known generated range
                for( Whole XDim = 0 ; XDim < WX ; ++XDim )
                {
                    RetArray[XDim] = static_cast<UInt8>( ( 255.0 / ( MaxNoise - MinNoise ) ) * ( TempField[XDim] - MinNoise ) );
                }
                // Clean up and return
                delete[] TempField;
                return RetArray;
            }

            UInt8* NoiseBase::Field2D(const Whole WX, const Whole WY)
            {
                UInt8* RetArray = new UInt8[WX * WY];
                // Set up what will be used as the known range of values for the noise
                PreciseReal MinNoise = 9999999999.9;
                PreciseReal MaxNoise = -999999999.9;
                Real* TempField = new Real[WY * WX];
                // For each spot in the array, generate the noise and then assign known limits appropriately
                for( Whole YDim = 0 ; YDim < WY ; ++YDim )
                {
                    for( Whole XDim = 0 ; XDim < WX ; ++XDim )
                    {
                        Real NoiseVal = this->Noise2D(XDim,YDim);
                        if( NoiseVal < MinNoise )
                            MinNoise = NoiseVal;
                        if( NoiseVal > MaxNoise )
                            MaxNoise = NoiseVal;
                        // Place the generated noise value in the temporary Real array
                        TempField[YDim * WX + XDim] = NoiseVal;
                    }
                }
                // For each spot in the return array, convert the generated noise value into a value between 0-255 based on it's position in the known generated range
                for( Whole YDim = 0 ; YDim < WY ; ++YDim )
                {
                    for( Whole XDim = 0 ; XDim < WX ; ++XDim )
                    {
                        RetArray[YDim * WX + XDim] = static_cast<UInt8>( ( 255.0 / ( MaxNoise - MinNoise ) ) * ( TempField[YDim * WX + XDim] - MinNoise ) );
                    }
                }
                // Clean up and return
                delete[] TempField;
                return RetArray;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // WhiteNoise Methods

            WhiteNoise::WhiteNoise(const Whole Seed) :
                NoiseSeed(Seed)
                { srand(Seed); }

            WhiteNoise::~WhiteNoise()
                {  }

            Real WhiteNoise::Noise1D(const Whole X)
                { srand(this->NoiseSeed);  return static_cast<Real>( rand() / RAND_MAX ); }

            Real WhiteNoise::Noise2D(const Whole X, const Whole Y)
                { return this->Noise1D(X * Y); }

            ///////////////////////////////////////////////////////////////////////////////
            // PerlinNoise Methods

            PerlinNoise::PerlinNoise(const Whole Octaves, const Real Persistence, const Real Frequency, const Real Amplitude) :
                NoiseOctaves( MathTools::Clamp(Octaves,Whole(1),Whole(32)) ),
                NoisePersistence( Persistence ),
                NoiseFrequency( Frequency ),
                NoiseAmplitude( Amplitude )
                {  }

            PerlinNoise::~PerlinNoise()
                {  }

            PreciseReal PerlinNoise::MakeNoise(const PreciseReal X)
            {
                Integer n = ( (Integer)X << 13 ) ^ (Integer)X;
                return 1.0 - ( ( n * ( n * n * 15731 + 789221 ) + 1376312589 ) & 0x7fffffff ) / 1073741824.0;
            }

            PreciseReal PerlinNoise::MakeNoise(const PreciseReal X, const PreciseReal Y)
            {
                Integer n = (Integer)( X + Y * 57.0 );
                n = ( n << 13 ) ^ n;
                return 1.0 - ( ( n * ( n * n * 15731 + 789221 ) + 1376312589 ) & 0x7fffffff ) / 1073741824.0;
            }

            PreciseReal PerlinNoise::MakeSmoothedNoise(const PreciseReal X)
            {
                Integer XInt = static_cast<Integer>(X);
                PreciseReal XFrac = X - static_cast<PreciseReal>(XInt);

                return this->Interpolate( this->MakeNoise(XInt), this->MakeNoise(XInt + 1), XFrac );
            }

            PreciseReal PerlinNoise::MakeSmoothedNoise(const PreciseReal X, const PreciseReal Y)
            {
                Integer XInt = static_cast<Integer>(X);
                Integer YInt = static_cast<Integer>(Y);
                PreciseReal XFrac = X - static_cast<PreciseReal>(XInt);
                PreciseReal YFrac = Y - static_cast<PreciseReal>(YInt);

                PreciseReal n00 = this->MakeNoise(XInt    , YInt    );
                PreciseReal n10 = this->MakeNoise(XInt + 1, YInt    );
                PreciseReal n01 = this->MakeNoise(XInt    , YInt + 1);
                PreciseReal n11 = this->MakeNoise(XInt + 1, YInt + 1);

                PreciseReal i1 = this->Interpolate(n00, n10, XFrac);
                PreciseReal i2 = this->Interpolate(n01, n11, XFrac);

                return this->Interpolate(i1, i2, YFrac);
            }

            PreciseReal PerlinNoise::Interpolate(const PreciseReal X1, const PreciseReal X2, const PreciseReal Amount)
            {
                PreciseReal f = 1 - MathTools::Cos( static_cast<Real>( Amount * MathTools::GetPi() ) ) * 0.5;
                return X1 * ( 1 - f ) + X2 * f;
            }

            void PerlinNoise::SetOctaves(const Whole Octaves)
                { this->NoiseOctaves = MathTools::Clamp(Octaves,Whole(1),Whole(32)); }

            Whole PerlinNoise::GetOctaves() const
                { return this->NoiseOctaves; }

            void PerlinNoise::SetPersistence(const Real Persistence)
                { this->NoisePersistence = Persistence; }

            Whole PerlinNoise::GetPersistence() const
                { return this->NoisePersistence; }

            void PerlinNoise::SetFrequency(const Real Frequency)
                { this->NoiseFrequency = Frequency; }

            Whole PerlinNoise::GetFrequency() const
                { return this->NoiseFrequency; }

            void PerlinNoise::SetAmplitude(const Real Amplitude)
                { this->NoiseAmplitude = Amplitude; }

            Whole PerlinNoise::GetAmplitude() const
                { return this->NoiseAmplitude; }

            Real PerlinNoise::Noise1D(const Whole X)
            {
                PreciseReal PerlinFreq = this->NoiseFrequency;
                PreciseReal PerlinAmp = this->NoiseAmplitude;
                PreciseReal PerlinSum = 0.0;

                for( Whole CurrOctave = 0 ; CurrOctave < this->NoiseOctaves ; ++CurrOctave )
                {
                    PerlinSum += this->MakeSmoothedNoise( static_cast<PreciseReal>(X) * PerlinFreq ) * PerlinAmp;

                    PerlinAmp *= this->NoisePersistence;
                    PerlinFreq *= 2;
                }

                return static_cast<Real>(PerlinSum);
            }

            Real PerlinNoise::Noise2D(const Whole X, const Whole Y)
            {
                PreciseReal PerlinFreq = this->NoiseFrequency;
                PreciseReal PerlinAmp = this->NoiseAmplitude;
                PreciseReal PerlinSum = 0.0;

                for( Whole CurrOctave = 0 ; CurrOctave < this->NoiseOctaves ; ++CurrOctave )
                {
                    PerlinSum += this->MakeSmoothedNoise( static_cast<PreciseReal>(X) * PerlinFreq, static_cast<PreciseReal>(Y) * PerlinFreq ) * PerlinAmp;

                    PerlinAmp *= this->NoisePersistence;
                    PerlinFreq *= 2;
                }

                return static_cast<Real>(PerlinSum);
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
