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

#ifndef noisemodulespheres_h
#define noisemodulespheres_h

#include "Noise/Module/modulebase.h"

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

            /// Default frequency value for the Noise::Module::Spheres noise module.
            const PreciseReal DEFAULT_SPHERES_FREQUENCY = 1.0;

            /// Noise module that outputs concentric spheres.
            ///
            /// This noise module outputs concentric spheres centered on the origin
            /// like the concentric rings of an onion.
            ///
            /// The first sphere has a radius of 1.0.  Each subsequent sphere has a
            /// radius that is 1.0 unit larger than the previous sphere.
            ///
            /// The output value from this noise module is determined by the distance
            /// between the input value and the the nearest spherical surface.  The
            /// input values that are located on a spherical surface are given the
            /// output value 1.0 and the input values that are equidistant from two
            /// spherical surfaces are given the output value -1.0.
            ///
            /// An application can change the frequency of the concentric spheres.
            /// Increasing the frequency reduces the distances between spheres.  To
            /// specify the frequency, call the SetFrequency() method.
            ///
            /// This noise module, modified with some low-frequency, low-power
            /// turbulence, is useful for generating agate-like textures.
            ///
            /// This noise module does not require any source modules.
            class Spheres : public ModuleBase
            {
            public:
                /// @brief Constructor.
                ///
                /// The default frequency is set to
                /// Noise::Module::DEFAULT_SPHERES_FREQUENCY.
                Spheres();

                /// Sets the frequenct of the concentric spheres.
                ///
                /// @param Frequency The frequency of the concentric spheres.
                ///
                /// Increasing the frequency increases the density of the concentric
                /// spheres, reducing the distances between them.
                void SetFrequency(const PreciseReal Frequency)
                {
                    this->m_frequency = Frequency;
                }
                /// Returns the frequency of the concentric spheres.
                ///
                /// @return Returns the frequency of the concentric spheres.
                ///
                /// Increasing the frequency increases the density of the concentric
                /// spheres, reducing the distances between them.
                PreciseReal GetFrequency() const
                {
                    return this->m_frequency;
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
                /// @brief Frequency of the concentric spheres.
                PreciseReal m_frequency;
            };//Spheres
            /// @}

            /// @}

            /// @}
        }//Module
    }//Noise
}//Mezzanine

#endif
