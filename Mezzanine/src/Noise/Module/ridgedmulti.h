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
// Copyright (C) 2003, 2004 Jason Bevins
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License (COPYING.txt) for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//

#ifndef noisemoduleridgemulti_h
#define noisemoduleridgemulti_h

#include "Noise/Module/modulebase.h"

#include "Noise/noiseenumerations.h"

namespace Mezzanine
{
    namespace Noise
    {
        namespace Module
        {
            /// @addtogroup libnoise
            /// @{

            /// @addtogroup modules
            /// @{

            /// @addtogroup generatormodules
            /// @{

            /// Default frequency for the Noise::Module::RidgedMulti noise module.
            const PreciseReal DEFAULT_RIDGED_FREQUENCY = 1.0;

            /// Default lacunarity for the Noise::Module::RidgedMulti noise module.
            const PreciseReal DEFAULT_RIDGED_LACUNARITY = 2.0;

            /// Default number of octaves for the Noise::Module::RidgedMulti noise
            /// module.
            const Integer DEFAULT_RIDGED_OCTAVE_COUNT = 6;

            /// Default noise quality for the Noise::Module::RidgedMulti noise
            /// module.
            const Noise::NoiseQuality DEFAULT_RIDGED_QUALITY = NQ_Standard;

            /// Default noise Seed for the Noise::Module::RidgedMulti noise module.
            const Integer DEFAULT_RIDGED_SEED = 0;

            /// Maximum number of octaves for the Noise::Module::RidgedMulti noise
            /// module.
            const Integer RIDGED_MAX_OCTAVE = 30;

            /// Noise module that outputs 3-dimensional ridged-multifractal noise.
            ///
            /// This noise module, heavily based on the Perlin-noise module, generates
            /// ridged-multifractal noise.  Ridged-multifractal noise is generated in
            /// much of the same way as Perlin noise, except the output of each octave
            /// is modified by an absolute-value function.  Modifying the octave
            /// values in this way produces ridge-like formations.
            ///
            /// Ridged-multifractal noise does not use a persistence value.  This is
            /// because the persistence values of the octaves are based on the values
            /// generated from from previous octaves, creating a feedback loop (or
            /// that's what it looks like after reading the code.)
            ///
            /// This noise module outputs ridged-multifractal-noise values that
            /// usually range from -1.0 to +1.0, but there are no guarantees that all
            /// output values will exist within that range.
            ///
            /// @note For ridged-multifractal noise generated with only one octave,
            /// the output value ranges from -1.0 to 0.0.
            ///
            /// Ridged-multifractal noise is often used to generate craggy mountainous
            /// terrain or marble-like textures.
            ///
            /// This noise module does not require any source modules.
            ///
            /// <b>Octaves</b>
            ///
            /// The number of octaves control the <i>amount of detail</i> of the
            /// ridged-multifractal noise.  Adding more octaves increases the detail
            /// of the ridged-multifractal noise, but with the drawback of increasing
            /// the calculation time.
            ///
            /// An application may specify the number of octaves that generate
            /// ridged-multifractal noise by calling the SetOctaveCount() method.
            ///
            /// <b>Frequency</b>
            ///
            /// An application may specify the frequency of the first octave by
            /// calling the SetFrequency() method.
            ///
            /// <b>Lacunarity</b>
            ///
            /// The lacunarity specifies the frequency multipler between successive
            /// octaves.
            ///
            /// The effect of modifying the lacunarity is subtle; you may need to play
            /// with the lacunarity value to determine the effects.  For best results,
            /// set the lacunarity to a number between 1.5 and 3.5.
            ///
            /// <b>References &amp; Acknowledgments</b>
            ///
            /// <a href=http://www.texturingandmodeling.com/Musgrave.html>F.
            /// Kenton "Doc Mojo" Musgrave's texturing page</a> - This page contains
            /// links to source code that generates ridged-multfractal noise, among
            /// other types of noise.  The source file <a
            /// href=http://www.texturingandmodeling.com/CODE/MUSGRAVE/CLOUD/fractal.c>
            /// fractal.c</a> contains the code I used in my ridged-multifractal class
            /// (see the @a RidgedMultifractal() function.)  This code was written by F.
            /// Kenton Musgrave, the person who created
            /// <a href=http://www.pandromeda.com/>MojoWorld</a>.  He is also one of
            /// the authors in <i>Texturing and Modeling: A Procedural Approach</i>
            /// (Morgan Kaufmann, 2002. ISBN 1-55860-848-6.)
            class RidgedMulti : public ModuleBase
            {
            public:
                /// @brief Constructor.
                ///
                /// The default number of octaves is set to
                /// Noise::Module::DEFAULT_RIDGED_OCTAVE_COUNT.
                ///
                /// The default frequency is set to
                /// Noise::Module::DEFAULT_RIDGED_FREQUENCY.
                ///
                /// The default lacunarity is set to
                /// Noise::Module::DEFAULT_RIDGED_LACUNARITY.
                ///
                /// The default Seed value is set to
                /// Noise::Module::DEFAULT_RIDGED_SEED.
                RidgedMulti();

                /// @brief Sets the frequency of the first octave.
                /// @param Frequency The frequency of the first octave.
                void SetFrequency(const PreciseReal Frequency)
                {
                    this->m_frequency = Frequency;
                }
                /// @brief Returns the frequency of the first octave.
                /// @return Returns the frequency of the first octave.
                PreciseReal GetFrequency() const
                {
                    return this->m_frequency;
                }

                /// Sets the lacunarity of the ridged-multifractal noise.
                ///
                /// @param Lacunarity The lacunarity of the ridged-multifractal noise.
                ///
                /// The Lacunarity is the frequency multiplier between successive
                /// octaves.
                ///
                /// For best results, set the lacunarity to a number between 1.5 and
                /// 3.5.
                void SetLacunarity(const PreciseReal Lacunarity)
                {
                    this->m_lacunarity = Lacunarity;
                    this->CalcSpectralWeights();
                }
                /// Returns the lacunarity of the ridged-multifractal noise.
                ///
                /// @return Returns the lacunarity of the ridged-multifractal noise.
                ///
                /// The lacunarity is the frequency multiplier between successive
                /// octaves.
                PreciseReal GetLacunarity() const
                {
                    return this->m_lacunarity;
                }

                /// Sets the quality of the ridged-multifractal noise.
                ///
                /// @param Quality The quality of the ridged-multifractal noise.
                ///
                /// See Noise::NoiseQuality for definitions of the various
                /// coherent-noise qualities.
                void SetNoiseQuality(const Noise::NoiseQuality Quality)
                {
                    this->m_noiseQuality = Quality;
                }
                /// Returns the quality of the ridged-multifractal noise.
                ///
                /// @return Returns the quality of the ridged-multifractal noise.
                ///
                /// See Noise::NoiseQuality for definitions of the various
                /// coherent-noise qualities.
                Noise::NoiseQuality GetNoiseQuality() const
                {
                    return this->m_noiseQuality;
                }

                /// Sets the number of octaves that generate the ridged-multifractal
                /// noise.
                ///
                /// @param OctaveCount The number of octaves that generate the
                /// ridged-multifractal noise.
                ///
                /// @pre The number of octaves ranges from 1 to
                /// Noise::Module::RIDGED_MAX_OCTAVE.
                ///
                /// @exception Exception::PARAMETERS_EXCEPTION An invalid parameter was
                /// specified; see the preconditions for more information.
                ///
                /// The number of octaves controls the amount of detail in the
                /// ridged-multifractal noise.
                ///
                /// The larger the number of octaves, the more time required to
                /// calculate the ridged-multifractal-noise value.
                void SetOctaveCount(const Integer OctaveCount)
                {
                    if( OctaveCount > Module::RIDGED_MAX_OCTAVE ) {
                        MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"The specified amount of octaves for Perlin noise is outside the valid range.");
                    }
                    this->m_octaveCount = OctaveCount;
                }
                /// Returns the number of octaves that generate the
                /// ridged-multifractal noise.
                ///
                /// @return Returns the number of octaves that generate the
                /// ridged-multifractal noise.
                ///
                /// The number of octaves controls the amount of detail in the
                /// ridged-multifractal noise.
                Integer GetOctaveCount() const
                {
                    return this->m_octaveCount;
                }

                /// @brief Sets the Seed value used by the ridged-multifractal-noise function.
                /// @param Seed The Seed value.
                void SetSeed(const Integer Seed)
                {
                    this->m_seed = Seed;
                }
                /// @brief Returns the Seed value used by the ridged-multifractal-noise function.
                /// @return Returns the Seed value.
                Integer GetSeed() const
                {
                    return this->m_seed;
                }

                /// @copydoc ModuleBase::GetSourceModuleCount() const
                virtual Integer GetSourceModuleCount() const
                {
                    return 0;
                }

                /// @copydoc ModuleBase::GetValue(const PreciseReal, const PreciseReal, const PreciseReal) const
                virtual PreciseReal GetValue(const PreciseReal X, const PreciseReal Y, const PreciseReal Z) const;
            protected:
                /// @internal
                /// @brief Calculates the spectral weights for each octave.
                /// @remarks This method is called when the lacunarity changes.
                void CalcSpectralWeights();

                /// @internal
                /// @brief Frequency of the first octave.
                PreciseReal m_frequency;
                /// @internal
                /// @brief Frequency multiplier between successive octaves.
                PreciseReal m_lacunarity;
                /// @internal
                /// @brief Quality of the ridged-multifractal noise.
                Noise::NoiseQuality m_noiseQuality;
                /// @internal
                /// @brief Total number of octaves that generate the ridged-multifractal noise.
                Integer m_octaveCount;
                /// @internal
                /// @brief Contains the spectral weights for each octave.
                PreciseReal m_pSpectralWeights[Module::RIDGED_MAX_OCTAVE];
                /// @internal
                /// @brief Seed value used by the ridged-multfractal-noise function.
                Integer m_seed;
            };//RigedMulti

            /// @}

            /// @}

            /// @}
        }//Module
    }//Noise
}//Mezzanine

#endif
