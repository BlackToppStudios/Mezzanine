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

#ifndef noisemoduleperlin_h
#define noisemoduleperlin_h

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

            /// Default frequency for the Noise::Module::Perlin noise module.
            const PreciseReal DEFAULT_PERLIN_FREQUENCY = 1.0;

            /// Default lacunarity for the Noise::Module::Perlin noise module.
            const PreciseReal DEFAULT_PERLIN_LACUNARITY = 2.0;

            /// Default number of octaves for the Noise::Module::Perlin noise module.
            const Integer DEFAULT_PERLIN_OCTAVE_COUNT = 6;

            /// Default persistence value for the Noise::Module::Perlin noise module.
            const PreciseReal DEFAULT_PERLIN_PERSISTENCE = 0.5;

            /// Default noise quality for the Noise::Module::Perlin noise module.
            const Noise::NoiseQuality DEFAULT_PERLIN_QUALITY = NQ_Standard;

            /// Default noise Seed for the Noise::Module::Perlin noise module.
            const Integer DEFAULT_PERLIN_SEED = 0;

            /// Maximum number of octaves for the Noise::Module::Perlin noise module.
            const Integer PERLIN_MAX_OCTAVE = 30;

            /// Noise module that outputs 3-dimensional Perlin noise.
            ///
            /// Perlin noise is the sum of several coherent-noise functions of
            /// ever-increasing frequencies and ever-decreasing amplitudes.
            ///
            /// An important property of Perlin noise is that a small change in the
            /// input value will produce a small change in the output value, while a
            /// large change in the input value will produce a random change in the
            /// output value.
            ///
            /// This noise module outputs Perlin-noise values that usually range from
            /// -1.0 to +1.0, but there are no guarantees that all output values will
            /// exist within that range.
            ///
            /// For a better description of Perlin noise, see the links in the
            /// <i>References and Acknowledgments</i> section.
            ///
            /// This noise module does not require any source modules.
            ///
            /// <b>Octaves</b>
            ///
            /// The number of octaves control the <i>amount of detail</i> of the
            /// Perlin noise.  Adding more octaves increases the detail of the Perlin
            /// noise, but with the drawback of increasing the calculation time.
            ///
            /// An octave is one of the coherent-noise functions in a series of
            /// coherent-noise functions that are added together to form Perlin
            /// noise.
            ///
            /// An application may specify the frequency of the first octave by
            /// calling the SetFrequency() method.
            ///
            /// An application may specify the number of octaves that generate Perlin
            /// noise by calling the SetOctaveCount() method.
            ///
            /// These coherent-noise functions are called octaves because each octave
            /// has, by default, PreciseReal the frequency of the previous octave.  Musical
            /// tones have this property as well; a musical C tone that is one octave
            /// higher than the previous C tone has PreciseReal its frequency.
            ///
            /// <b>Frequency</b>
            ///
            /// An application may specify the frequency of the first octave by
            /// calling the SetFrequency() method.
            ///
            /// <b>Persistence</b>
            ///
            /// The persistence value controls the <i>roughness</i> of the Perlin
            /// noise.  Larger values produce rougher noise.
            ///
            /// The persistence value determines how quickly the amplitudes diminish
            /// for successive octaves.  The amplitude of the first octave is 1.0.
            /// The amplitude of each subsequent octave is equal to the product of the
            /// previous octave's amplitude and the persistence value.  So a
            /// persistence value of 0.5 sets the amplitude of the first octave to
            /// 1.0; the second, 0.5; the third, 0.25; etc.
            ///
            /// An application may specify the persistence value by calling the
            /// SetPersistence() method.
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
            /// <b>References &amp; acknowledgments</b>
            ///
            /// <a href=http://www.noisemachine.com/talk1/>The Noise Machine</a> -
            /// From the master, Ken Perlin himself.  This page contains a
            /// presentation that describes Perlin noise and some of its variants.
            /// He won an Oscar for creating the Perlin noise algorithm!
            ///
            /// <a
            /// href=http://freespace.virgin.net/hugo.elias/models/m_perlin.htm>
            /// Perlin Noise</a> - Hugo Elias's webpage contains a very good
            /// description of Perlin noise and describes its many applications.  This
            /// page gave me the inspiration to create libnoise in the first place.
            /// Now that I know how to generate Perlin noise, I will never again use
            /// cheesy subdivision algorithms to create terrain (unless I absolutely
            /// need the speed.)
            ///
            /// <a
            /// href=http://www.robo-murito.net/code/perlin-noise-math-faq.html>The
            /// Perlin noise math FAQ</a> - A good page that describes Perlin noise in
            /// plain English with only a minor amount of math.  During development of
            /// libnoise, I noticed that my coherent-noise function generated terrain
            /// with some "regularity" to the terrain features.  This page describes a
            /// better coherent-noise function called <i>gradient noise</i>.  This
            /// version of Noise::Module::Perlin uses gradient coherent noise to
            /// generate Perlin noise.
            class Perlin : public ModuleBase
            {
            public:
                /// @brief Constructor.
                ///
                /// The default frequency is set to
                /// Noise::Module::DEFAULT_PERLIN_FREQUENCY.
                ///
                /// The default lacunarity is set to
                /// Noise::Module::DEFAULT_PERLIN_LACUNARITY.
                ///
                /// The default number of octaves is set to
                /// Noise::Module::DEFAULT_PERLIN_OCTAVE_COUNT.
                ///
                /// The default persistence value is set to
                /// Noise::Module::DEFAULT_PERLIN_PERSISTENCE.
                ///
                /// The default Seed value is set to
                /// Noise::Module::DEFAULT_PERLIN_SEED.
                Perlin();

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

                /// Sets the lacunarity of the Perlin noise.
                ///
                /// @param Lacunarity The lacunarity of the Perlin noise.
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
                /// Returns the lacunarity of the Perlin noise.
                ///
                /// @return Returns the lacunarity of the Perlin noise.
                ///
                /// The lacunarity is the frequency multiplier between successive
                /// octaves.
                PreciseReal GetLacunarity() const
                {
                    return this->m_lacunarity;
                }

                /// Sets the quality of the Perlin noise.
                ///
                /// @param Quality The quality of the Perlin noise.
                ///
                /// See Noise::NoiseQuality for definitions of the various
                /// coherent-noise qualities.
                void SetNoiseQuality(const Noise::NoiseQuality Quality)
                {
                    this->m_noiseQuality = Quality;
                }
                /// Returns the quality of the Perlin noise.
                ///
                /// @return Returns the quality of the Perlin noise.
                ///
                /// See Noise::NoiseQuality for definitions of the various
                /// coherent-noise qualities.
                Noise::NoiseQuality GetNoiseQuality() const
                {
                    return this->m_noiseQuality;
                }

                /// Sets the number of octaves that generate the Perlin noise.
                ///
                /// @param OctaveCount The number of octaves that generate the Perlin
                /// noise.
                ///
                /// @pre The number of octaves ranges from 1 to
                /// Noise::Module::PERLIN_MAX_OCTAVE.
                ///
                /// @exception Exception::PARAMETERS_EXCEPTION An invalid parameter was
                /// specified; see the preconditions for more information.
                ///
                /// The number of octaves controls the amount of detail in the Perlin
                /// noise.
                ///
                /// The larger the number of octaves, the more time required to
                /// calculate the Perlin-noise value.
                void SetOctaveCount(const Integer OctaveCount)
                {
                    if( OctaveCount < 1 || OctaveCount > Module::PERLIN_MAX_OCTAVE ) {
                        MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"The specified amount of octaves for Perlin noise is outside the valid range.");
                    }
                    this->m_octaveCount = OctaveCount;
                }
                /// Returns the number of octaves that generate the Perlin noise.
                ///
                /// @return Returns the number of octaves that generate the Perlin noise.
                ///
                /// The number of octaves controls the amount of detail in the Perlin
                /// noise.
                Integer GetOctaveCount() const
                {
                    return this->m_octaveCount;
                }

                /// Sets the persistence value of the Perlin noise.
                ///
                /// @param Persistence The persistence value of the Perlin noise.
                ///
                /// The persistence value controls the roughness of the Perlin noise.
                ///
                /// For best results, set the persistence to a number between 0.0 and
                /// 1.0.
                void SetPersistence(const PreciseReal Persistence)
                {
                    this->m_persistence = Persistence;
                }
                /// Returns the persistence value of the Perlin noise.
                ///
                /// @return Returns the persistence value of the Perlin noise.
                ///
                /// The persistence value controls the roughness of the Perlin noise.
                PreciseReal GetPersistence() const
                {
                    return this->m_persistence;
                }

                /// @brief Sets the Seed value used by the Perlin-noise function.
                /// @param Seed The Seed value.
                void SetSeed(const Integer Seed)
                {
                    this->m_seed = Seed;
                }
                /// @brief Returns the Seed value used by the Perlin-noise function.
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
                /// @brief Quality of the Perlin noise.
                Noise::NoiseQuality m_noiseQuality;
                /// @internal
                /// @brief Total number of octaves that generate the Perlin noise.
                Integer m_octaveCount;
                /// @internal
                /// @brief Persistence of the Perlin noise.
                PreciseReal m_persistence;
                /// @internal
                /// @brief Seed value used by the Perlin-noise function.
                Integer m_seed;
            };//Perlin
            /// @}

            /// @}

            /// @}
        }//Module
    }//Noise
}//Mezzanine

#endif
