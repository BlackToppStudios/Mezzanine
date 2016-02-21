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

#ifndef noisemoduleturbulence_cpp
#define noisemoduleturbulence_cpp

#include "Noise/Module/turbulence.h"

namespace Mezzanine
{
    namespace Noise
    {
        namespace Module
        {
            Turbulence::Turbulence() :
                ModuleBase(this->GetSourceModuleCount()),
                m_power(Module::DEFAULT_TURBULENCE_POWER)
            {
                this->SetSeed(Module::DEFAULT_TURBULENCE_SEED);
                this->SetFrequency(Module::DEFAULT_TURBULENCE_FREQUENCY);
                this->SetRoughness(Module::DEFAULT_TURBULENCE_ROUGHNESS);
            }

            PreciseReal Turbulence::GetFrequency() const
            {
                // Since each Noise::Module::Perlin noise module has the same frequency, it
                // does not matter which module we use to retrieve the frequency.
                return this->m_xDistortModule.GetFrequency();
            }

            void Turbulence::SetSeed(const Integer Seed)
            {
                // Set the Seed of each Noise::Module::Perlin noise modules.  To prevent any
                // sort of weird artifacting, use a slightly different Seed for each noise
                // module.
                this->m_xDistortModule.SetSeed(Seed    );
                this->m_yDistortModule.SetSeed(Seed + 1);
                this->m_zDistortModule.SetSeed(Seed + 2);
            }

            Integer Turbulence::GetSeed() const
            {
                return this->m_xDistortModule.GetSeed ();
            }

            PreciseReal Turbulence::GetValue(const PreciseReal X, const PreciseReal Y, const PreciseReal Z) const
            {
                assert( this->m_pSourceModule[0] != NULL );

                // Get the values from the three Noise::Module::Perlin noise modules and
                // add each value to each coordinate of the input value.  There are also
                // some offsets added to the coordinates of the input values.  This prevents
                // the distortion modules from returning zero if the (X, Y, Z) coordinates,
                // when multiplied by the frequency, are near an integer boundary.  This is
                // due to a property of gradient coherent noise, which returns zero at
                // integer boundaries.
                PreciseReal x0, y0, z0;
                PreciseReal x1, y1, z1;
                PreciseReal x2, y2, z2;
                x0 = X + (12414.0 / 65536.0);
                y0 = Y + (65124.0 / 65536.0);
                z0 = Z + (31337.0 / 65536.0);
                x1 = X + (26519.0 / 65536.0);
                y1 = Y + (18128.0 / 65536.0);
                z1 = Z + (60493.0 / 65536.0);
                x2 = X + (53820.0 / 65536.0);
                y2 = Y + (11213.0 / 65536.0);
                z2 = Z + (44845.0 / 65536.0);
                PreciseReal xDistort = X + (this->m_xDistortModule.GetValue(x0, y0, z0) * this->m_power);
                PreciseReal yDistort = Y + (this->m_yDistortModule.GetValue(x1, y1, z1) * this->m_power);
                PreciseReal zDistort = Z + (this->m_zDistortModule.GetValue(x2, y2, z2) * this->m_power);

                // Retrieve the output value at the offsetted input value instead of the
                // original input value.
                return this->m_pSourceModule[0]->GetValue(xDistort, yDistort, zDistort);
            }
        }//Module
    }//Noise
}//Mezzanine

#endif
