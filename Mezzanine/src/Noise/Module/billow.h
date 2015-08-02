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
// Copyright (C) 2004 Jason Bevins
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

#ifndef noisemodulebillow_h
#define noisemodulebillow_h

#include "Noise/Module/modulebase.h"
#include "Noise/noiseenumerations.h"

#include "exception.h"

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

            /// Default frequency for the Noise::Module::Billow noise module.
            const PreciseReal DEFAULT_BILLOW_FREQUENCY = 1.0;

            /// Default lacunarity for the the Noise::Module::Billow noise module.
            const PreciseReal DEFAULT_BILLOW_LACUNARITY = 2.0;

            /// Default number of octaves for the the Noise::Module::Billow noise module.
            const Integer DEFAULT_BILLOW_OCTAVE_COUNT = 6;

            /// Default persistence value for the the Noise::Module::Billow noise
            /// module.
            const PreciseReal DEFAULT_BILLOW_PERSISTENCE = 0.5;

            /// Default noise quality for the the Noise::Module::Billow noise module.
            const Noise::NoiseQuality DEFAULT_BILLOW_QUALITY = NQ_Standard;

            /// Default noise Seed for the the Noise::Module::Billow noise module.
            const Integer DEFAULT_BILLOW_SEED = 0;

            /// Maximum number of octaves for the the Noise::Module::Billow noise module.
            const Integer BILLOW_MAX_OCTAVE = 30;

            /// Noise module that outputs three-dimensional "billowy" noise.
            ///
            /// This noise module generates "billowy" noise suitable for clouds and
            /// rocks.
            ///
            /// This noise module is nearly identical to Noise::Module::Perlin except
            /// this noise module modifies each octave with an absolute-value
            /// function.  See the documentation of Noise::Module::Perlin for more
            /// information.
            class Billow : public ModuleBase
            {
            public:
                /// @brief Constructor.
                ///
                /// @details The default frequency is set to
                /// Noise::Module::DEFAULT_BILLOW_FREQUENCY.
                ///
                /// The default lacunarity is set to
                /// Noise::Module::DEFAULT_BILLOW_LACUNARITY.
                ///
                /// The default number of octaves is set to
                /// Noise::Module::DEFAULT_BILLOW_OCTAVE_COUNT.
                ///
                /// The default persistence value is set to
                /// Noise::Module::DEFAULT_BILLOW_PERSISTENCE.
                ///
                /// The default Seed value is set to
                /// Noise::Module::DEFAULT_BILLOW_SEED.
                Billow();

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

                /// Sets the lacunarity of the billowy noise.
                ///
                /// @param Lacunarity The lacunarity of the billowy noise.
                ///
                /// The lacunarity is the frequency multiplier between successive
                /// octaves.
                ///
                /// For best results, set the lacunarity to a number between 1.5 and
                /// 3.5.
                void SetLacunarity(const PreciseReal Lacunarity)
                {
                    this->m_lacunarity = Lacunarity;
                }
                /// Returns the lacunarity of the billowy noise.
                ///
                /// @return Returns the lacunarity of the billowy noise.
                ///
                /// The lacunarity is the frequency multiplier between successive
                /// octaves.
                PreciseReal GetLacunarity() const
                {
                    return this->m_lacunarity;
                }

                /// Sets the quality of the billowy noise.
                ///
                /// @param Quality The quality of the billowy noise.
                ///
                /// See Noise::NoiseQuality for definitions of the various
                /// coherent-noise qualities.
                void SetNoiseQuality(const Noise::NoiseQuality Quality)
                {
                    this->m_noiseQuality = Quality;
                }
                /// Returns the quality of the billowy noise.
                ///
                /// @return Returns the quality of the billowy noise.
                ///
                /// See Noise::NoiseQuality for definitions of the various
                /// coherent-noise qualities.
                Noise::NoiseQuality GetNoiseQuality() const
                {
                    return this->m_noiseQuality;
                }

                /// Sets the number of octaves that generate the billowy noise.
                ///
                /// @param OctaveCount The number of octaves that generate the billowy
                /// noise.
                ///
                /// @pre The number of octaves ranges from 1 to
                /// Noise::Module::BILLOW_MAX_OCTAVE.
                ///
                /// @exception Exception::PARAMETERS_EXCEPTION An invalid parameter was
                /// specified; see the preconditions for more information.
                ///
                /// The number of octaves controls the amount of detail in the billowy
                /// noise.
                ///
                /// The larger the number of octaves, the more time required to
                /// calculate the billowy-noise value.
                void SetOctaveCount(const Integer OctaveCount)
                {
                    if( OctaveCount < 1 || OctaveCount > BILLOW_MAX_OCTAVE ) {
                        MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"The specified amount of octaves for Billow noise is outside the valid range.");
                    }
                    this->m_octaveCount = OctaveCount;
                }
                /// Returns the number of octaves that generate the billowy noise.
                ///
                /// @return Returns the number of octaves that generate the billowy noise.
                ///
                /// The number of octaves controls the amount of detail in the billowy
                /// noise.
                Integer GetOctaveCount() const
                {
                    return this->m_octaveCount;
                }

                /// Sets the persistence value of the billowy noise.
                ///
                /// @param Persistence The persistence value of the billowy noise.
                ///
                /// The persistence value controls the roughness of the billowy noise.
                ///
                /// For best results, set the persistence value to a number between
                /// 0.0 and 1.0.
                void SetPersistence(const PreciseReal Persistence)
                {
                    this->m_persistence = Persistence;
                }
                /// Returns the persistence value of the billowy noise.
                ///
                /// @return Returns the persistence value of the billowy noise.
                ///
                /// The persistence value controls the roughness of the billowy noise.
                PreciseReal GetPersistence() const
                {
                    return this->m_persistence;
                }

                /// @brief Sets the Seed value used by the billowy-noise function.
                /// @param Seed The Seed value.
                void SetSeed(const Integer Seed)
                {
                    this->m_seed = Seed;
                }
                /// @brief Returns the Seed value used by the billowy-noise function.
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
                /// @brief Frequency of the first octave.
                PreciseReal m_frequency;
                /// @internal
                /// @brief Frequency multiplier between successive octaves.
                PreciseReal m_lacunarity;
                /// @internal
                /// @brief Quality of the billowy noise.
                Noise::NoiseQuality m_noiseQuality;
                /// @internal
                /// @brief Total number of octaves that generate the billowy noise.
                Integer m_octaveCount;
                /// @internal
                /// @brief Persistence value of the billowy noise.
                PreciseReal m_persistence;
                /// @internal
                /// @brief Seed value used by the billowy-noise function.
                Integer m_seed;
            };//Billow
            /// @}

            /// @}

            /// @}
        }//Module
    }//Noise
}//Mezzanine

#endif
