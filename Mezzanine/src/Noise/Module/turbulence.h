// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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

#ifndef noisemoduleturbulence_h
#define noisemoduleturbulence_h

#include "Noise/Module/perlin.h"

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

            /// @addtogroup transformermodules
            /// @{

            /// Default frequency for the Noise::Module::Turbulence noise module.
            const PreciseReal DEFAULT_TURBULENCE_FREQUENCY = DEFAULT_PERLIN_FREQUENCY;

            /// Default power for the Noise::Module::Turbulence noise module.
            const PreciseReal DEFAULT_TURBULENCE_POWER = 1.0;

            /// Default roughness for the Noise::Module::Turbulence noise module.
            const Integer DEFAULT_TURBULENCE_ROUGHNESS = 3;

            /// Default noise Seed for the Noise::Module::Turbulence noise module.
            const Integer DEFAULT_TURBULENCE_SEED = DEFAULT_PERLIN_SEED;

            /// Noise module that randomly displaces the input value before
            /// returning the output value from a source module.
            ///
            /// @a Turbulence is the pseudo-random displacement of the input value.
            /// The GetValue() method randomly displaces the ( @a X, @a Y, @a Z )
            /// coordinates of the input value before retrieving the output value from
            /// the source module.  To control the turbulence, an application can
            /// modify its frequency, its power, and its roughness.
            ///
            /// The frequency of the turbulence determines how rapidly the
            /// displacement amount changes.  To specify the frequency, call the
            /// SetFrequency() method.
            ///
            /// The power of the turbulence determines the scaling factor that is
            /// applied to the displacement amount.  To specify the power, call the
            /// SetPower() method.
            ///
            /// The roughness of the turbulence determines the roughness of the
            /// changes to the displacement amount.  Low values smoothly change the
            /// displacement amount.  High values roughly change the displacement
            /// amount, which produces more "kinky" changes.  To specify the
            /// roughness, call the SetRoughness() method.
            ///
            /// Use of this noise module may require some trial and error.  Assuming
            /// that you are using a generator module as the source module, you
            /// should first:
            /// - Set the frequency to the same frequency as the source module.
            /// - Set the power to the reciprocal of the frequency.
            ///
            /// From these initial frequency and power values, modify these values
            /// until this noise module produce the desired changes in your terrain or
            /// texture.  For example:
            /// - Low frequency (1/8 initial frequency) and low power (1/8 initial
            ///   power) produces very minor, almost unnoticeable changes.
            /// - Low frequency (1/8 initial frequency) and high power (8 times
            ///   initial power) produces "ropey" lava-like terrain or marble-like
            ///   textures.
            /// - High frequency (8 times initial frequency) and low power (1/8
            ///   initial power) produces a noisy version of the initial terrain or
            ///   texture.
            /// - High frequency (8 times initial frequency) and high power (8 times
            ///   initial power) produces nearly pure noise, which isn't entirely
            ///   useful.
            ///
            /// Displacing the input values result in more realistic terrain and
            /// textures.  If you are generating elevations for terrain height maps,
            /// you can use this noise module to produce more realistic mountain
            /// ranges or terrain features that look like flowing lava rock.  If you
            /// are generating values for textures, you can use this noise module to
            /// produce realistic marble-like or "oily" textures.
            ///
            /// Internally, there are three Noise::Module::Perlin noise modules
            /// that displace the input value; one for the @a X, one for the @a Y,
            /// and one for the @a Z coordinate.
            ///
            /// This noise module requires one source module.
            class Turbulence : public ModuleBase
            {
            public:
                /// @brief Constructor.
                ///
                /// The default frequency is set to
                /// Noise::Module::DEFAULT_TURBULENCE_FREQUENCY.
                ///
                /// The default power is set to
                /// Noise::Module::DEFAULT_TURBULENCE_POWER.
                ///
                /// The default roughness is set to
                /// Noise::Module::DEFAULT_TURBULENCE_ROUGHNESS.
                ///
                /// The default Seed value is set to
                /// Noise::Module::DEFAULT_TURBULENCE_SEED.
                Turbulence();

                /// Sets the frequency of the turbulence.
                ///
                /// @param Frequency The frequency of the turbulence.
                ///
                /// The frequency of the turbulence determines how rapidly the
                /// displacement amount changes.
                void SetFrequency(const PreciseReal Frequency)
                {
                    // Set the frequency of each Perlin-noise module.
                    this->m_xDistortModule.SetFrequency(Frequency);
                    this->m_yDistortModule.SetFrequency(Frequency);
                    this->m_zDistortModule.SetFrequency(Frequency);
                }
                /// Returns the frequency of the turbulence.
                ///
                /// @return Returns the frequency of the turbulence.
                ///
                /// The frequency of the turbulence determines how rapidly the
                /// displacement amount changes.
                PreciseReal GetFrequency() const;

                /// Sets the power of the turbulence.
                ///
                /// @param Power The power of the turbulence.
                ///
                /// The power of the turbulence determines the scaling factor that is
                /// applied to the displacement amount.
                void SetPower(const PreciseReal Power)
                {
                    this->m_power = Power;
                }
                /// Returns the power of the turbulence.
                ///
                /// @return Returns the power of the turbulence.
                ///
                /// The power of the turbulence determines the scaling factor that is
                /// applied to the displacement amount.
                PreciseReal GetPower() const
                {
                    return this->m_power;
                }

                /// Sets the roughness of the turbulence.
                ///
                /// @param Roughness The roughness of the turbulence.
                ///
                /// The roughness of the turbulence determines the roughness of the
                /// changes to the displacement amount.  Low values smoothly change
                /// the displacement amount.  High values roughly change the
                /// displacement amount, which produces more "kinky" changes.
                ///
                /// Internally, there are three Noise::Module::Perlin noise modules
                /// that displace the input value; one for the @a X, one for the @a Y,
                /// and one for the @a Z coordinate.  The roughness value is equal to
                /// the number of octaves used by the Noise::Module::Perlin noise
                /// modules.
                void SetRoughness(const Integer Roughness)
                {
                    // Set the octave count for each Perlin-noise module.
                    this->m_xDistortModule.SetOctaveCount(Roughness);
                    this->m_yDistortModule.SetOctaveCount(Roughness);
                    this->m_zDistortModule.SetOctaveCount(Roughness);
                }
                /// Returns the roughness of the turbulence.
                ///
                /// @return Returns the roughness of the turbulence.
                ///
                /// The roughness of the turbulence determines the roughness of the
                /// changes to the displacement amount.  Low values smoothly change
                /// the displacement amount.  High values roughly change the
                /// displacement amount, which produces more "kinky" changes.
                Integer GetRoughnessCount() const
                {
                    return this->m_xDistortModule.GetOctaveCount();
                }

                /// Sets the Seed value of the internal noise modules that are used to
                /// displace the input values.
                ///
                /// @param Seed The Seed value.
                ///
                /// Internally, there are three Noise::Module::Perlin noise modules
                /// that displace the input value; one for the @a X, one for the @a Y,
                /// and one for the @a Z coordinate.  This noise module assigns the
                /// following Seed values to the Noise::Module::Perlin noise modules:
                /// - It assigns the Seed value (@a Seed + 0) to the @a X noise module.
                /// - It assigns the Seed value (@a Seed + 1) to the @a Y noise module.
                /// - It assigns the Seed value (@a Seed + 2) to the @a Z noise module.
                void SetSeed(const Integer Seed);
                /// Returns the Seed value of the internal Perlin-noise modules that
                /// are used to displace the input values.
                ///
                /// @return Returns the Seed value.
                ///
                /// Internally, there are three Noise::Module::Perlin noise modules
                /// that displace the input value; one for the @a X, one for the @a Y,
                /// and one for the @a Z coordinate.
                Integer GetSeed() const;

                /// @copydoc ModuleBase::GetSourceModuleCount() const
                virtual Integer GetSourceModuleCount() const
                {
                    return 1;
                }

                /// @copydoc ModuleBase::GetValue(const PreciseReal, const PreciseReal, const PreciseReal) const
                virtual PreciseReal GetValue(const PreciseReal X, const PreciseReal Y, const PreciseReal Z) const;
            protected:
                /// @internal
                /// @brief The power (scale) of the displacement.
                PreciseReal m_power;
                /// @internal
                /// @brief Noise module that displaces the @a X coordinate.
                Perlin m_xDistortModule;
                /// @internal
                /// @brief Noise module that displaces the @a Y coordinate.
                Perlin m_yDistortModule;
                /// @internal
                /// @brief Noise module that displaces the @a Z coordinate.
                Perlin m_zDistortModule;
            };//Turbulence
            /// @}

            /// @}

            /// @}
        }//Module
    }//Noise
}//Mezzanine

#endif
