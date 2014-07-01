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
#ifndef _graphicsproceduralnoise_h
#define _graphicsproceduralnoise_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief The base class for noise generators.
            /// @details Noise generators are used for procedural texture generation and can generate coherent or non-coherent noise. @n
            /// Non-coherent noise generators will generally use random number generators.  Coherent noise generators will return the same
            /// output for the same input (assuming all other configuration values are the same).
            ///////////////////////////////////////
            class MEZZ_LIB NoiseBase
            {
            public:
                /// @brief Class constructor.
                NoiseBase() {  }
                /// @brief Class destructor.
                virtual ~NoiseBase() {  }

                /// @brief Generates noise for a theoretical one dimensional array position.
                /// @remarks Noise generators can generate random, psuedo-random, or non-random noise.  If a generator generates random noise then this method can return
                /// any arbitrary number and the array position passed in will not matter.  However with other types of generators they can and often will follow a
                /// repeating pattern.  Given that an array doesn't actually need to be generated to know how a particular position will turn out for some types of noise.
                /// @param X The position on the array to generate.
                /// @return Returns a Real representing the noise for the specified grid position.
                virtual Real Noise1D(const Whole X) = 0;
                /// @brief Generates noise for a theoretical two dimensional grid position.
                /// @remarks Noise generators can generate random, psuedo-random, or non-random noise.  If a generator generates random noise then this method can return
                /// any arbitrary number and the array position passed in will not matter.  However with other types of generators they can and often will follow a
                /// repeating pattern.  Given that an array doesn't actually need to be generated to know how a particular position will turn out for some types of noise.
                /// @param X The X position on the grid to generate.
                /// @param Y The Y position on the grid to generate.
                /// @return Returns a Real representing
                virtual Real Noise2D(const Whole X, const Whole Y) = 0;

                /// @brief Generates a one dimensional array of noise as configured.
                /// @param WX The size of the array to allocate.
                /// @return Returns an array of UInt8's with noise based on how this noise generated is configured.
                virtual UInt8* Field1D(const Whole WX);
                /// @brief Generated a two dimensional array of noise as configured.
                /// @param WX The X size of the array to allocate.  Array total size is: X * Y.
                /// @param WY The Y size of the array to allocate.  Array total size is: X * Y.
                /// @return Returns an array of UInt8's with noise based on how this noise generated is configured.
                virtual UInt8* Field2D(const Whole WX, const Whole WY);
            };//NoiseBase

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A noise generator for random, non-coherent noise.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB WhiteNoise : public NoiseBase
            {
            protected:
                /// @internal
                /// @brief The seed value to be used with the random number generator.
                Whole NoiseSeed;
            public:
                /// @brief Class constructor.
                /// @param Seed The seed value to be used with the random number generator.
                WhiteNoise(const Whole Seed = 5120);
                /// @brief Class destructor.
                virtual ~WhiteNoise();

                /// @copydoc NoiseBase::Noise1D(const Whole)
                virtual Real Noise1D(const Whole X);
                /// @copydoc NoiseBase::Noise1D(const Whole, const Whole)
                virtual Real Noise2D(const Whole X, const Whole Y);
            };//WhiteNoise

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A noise generator for psuedo-random, conherent noise using the Perlin algorithm.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB PerlinNoise : public NoiseBase
            {
            protected:
                /// @internal
                /// @brief The number of octaves to generate noise with.
                Whole NoiseOctaves;
                /// @internal
                /// @brief A multiplier that determines how quickly the amplitudes diminish for each successive octave.
                Real NoisePersistence;
                /// @internal
                /// @brief The number of cycles per unit length that a specific octave outputs.
                Real NoiseFrequency;
                /// @internal
                /// @brief The maximum absolute value a specific octave can output.
                Real NoiseAmplitude;

                /// @internal
                /// @brief Generates a psuedo-random number from a theoretical position on a one dimensional array.  Rounding errors may apply.
                /// @param X The position on the array to generate.
                /// @return Returns a PreciseReal containing the generated noise.
                PreciseReal MakeNoise(const PreciseReal X);
                /// @internal
                /// @brief Generates a psuedo-random number from a theoretical position on a two dimensional grid.  Rounding errors may apply.
                /// @param X The X position on the grid to generate.
                /// @param Y The Y position on the grid to generate.
                /// @return Returns a PreciseReal containing the generated noise.
                PreciseReal MakeNoise(const PreciseReal X, const PreciseReal Y);
                /// @internal
                /// @brief Similar to "MakeNoise", but will properly interpolate values.
                /// @param X The position on the array to generate.
                /// @return Returns a PreciseReal containing the generated noise.
                PreciseReal MakeSmoothedNoise(const PreciseReal X);
                /// @internal
                /// @brief Similar to "MakeNoise", but will properly interpolate values.
                /// @param X The X position on the grid to generate.
                /// @param Y The Y position on the grid to generate.
                /// @return Returns a PreciseReal containing the generated noise.
                PreciseReal MakeSmoothedNoise(const PreciseReal X, const PreciseReal Y);
                /// @internal
                /// @brief Interpolates noise between two positions on an array.
                /// @param X1 The first array position to interpolate.
                /// @param X2 The second array position to interpolate.
                /// @param Amount The amount to interpolate between the two positions by.
                /// @return Returns a PreciseReal containing the interpolated noise.
                PreciseReal Interpolate(const PreciseReal X1, const PreciseReal X2, const PreciseReal Amount);
            public:
                /// @brief Class constructor.
                /// @param Octaves The number of octaves to generate noise with.
                /// @param Persistence A multiplier that determines how quickly the amplitudes diminish for each successive octave.
                /// @param Frequency The number of cycles per unit length that a specific octave outputs.
                /// @param Amplitude The maximum absolute value a specific octave can output.
                PerlinNoise(const Whole Octaves = 4, const Real Persistence = 0.65, const Real Frequency = 1.0, const Real Amplitude = 1.0);
                /// @brief Class destructor.
                virtual ~PerlinNoise();

                /// @brief Sets the number of octaves to be applied to noise generated.
                /// @remarks The number of octaves control the amount of detail of Perlin noise. Adding more octaves increases the
                /// detail of Perlin noise, with the added drawback of increasing the calculation time.
                /// @param Octaves The number of octaves to generate noise with.
                void SetOctaves(const Whole Octaves);
                /// @brief Gets the number of octaves to be applied to noise generated.
                /// @return Returns a Whole representing the number of octaves used to generate noise.
                Whole GetOctaves() const;
                /// @brief Sets the persistence to be applied to noise generated.
                /// @remarks The amplitude of each successive octave is equal to the product of the previous octave's amplitude and
                /// the persistence value. Increasing the persistence produces "rougher" Perlin noise.
                /// @param Persistence A multiplier that determines how quickly the amplitudes diminish for each successive octave.
                void SetPersistence(const Real Persistence);
                /// @brief Gets the persistence to be applied to noise generated.
                /// @return Returns a Real representing how quickly the amplitudes diminish for each successive octave.
                Whole GetPersistence() const;
                /// @brief Sets the frequency to be applied to noise generated.
                /// @param Frequency The number of cycles per unit length that a specific octave outputs.
                void SetFrequency(const Real Frequency);
                /// @brief Gets the frequency to be applied to noise generated.
                /// @return Returns a Real representing the number of cycles an octave will output per unit length.
                Whole GetFrequency() const;
                /// @brief Sets the amplitude to be applied to noise generated.
                /// @param Amplitude The maximum absolute value a specific octave can output.
                void SetAmplitude(const Real Amplitude);
                /// @brief Gets the amplitude to be applied to noise generated.
                /// @return Returns a Real representing highest allowed absolute value of an octave.
                Whole GetAmplitude() const;

                /// @copydoc NoiseBase::Noise1D(const Whole)
                virtual Real Noise1D(const Whole X);
                /// @copydoc NoiseBase::Noise1D(const Whole, const Whole)
                virtual Real Noise2D(const Whole X, const Whole Y);
            };//PerlinNoise
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
