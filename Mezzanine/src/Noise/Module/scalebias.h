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

#ifndef noisemodulescalebias_h
#define noisemodulescalebias_h

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

            /// @addtogroup modifiermodules
            /// @{

            /// Default bias for the Noise::Module::ScaleBias noise module.
            const PreciseReal DEFAULT_BIAS = 0.0;

            /// Default scale for the Noise::Module::ScaleBias noise module.
            const PreciseReal DEFAULT_SCALE = 1.0;

            /// Noise module that applies a scaling factor and a bias to the output
            /// value from a source module.
            ///
            /// The GetValue() method retrieves the output value from the source
            /// module, multiplies it with a scaling factor, adds a bias to it, then
            /// outputs the value.
            ///
            /// This noise module requires one source module.
            class ScaleBias : public ModuleBase
            {
            public:
                /// @brief Constructor.
                ///
                /// @details The default bias is set to Noise::Module::DEFAULT_BIAS.
                /// The default scaling factor is set to Noise::Module::DEFAULT_SCALE.
                ScaleBias();

                /// Sets the bias to apply to the scaled output value from the source
                /// module.
                ///
                /// @param Bias The bias to apply.
                ///
                /// The GetValue() method retrieves the output value from the source
                /// module, multiplies it with the scaling factor, adds the bias to
                /// it, then outputs the value.
                void SetBias(const PreciseReal Bias)
                {
                    this->m_bias = Bias;
                }
                /// Returns the bias to apply to the scaled output value from the
                /// source module.
                ///
                /// @return Returns the bias to apply.
                ///
                /// The GetValue() method retrieves the output value from the source
                /// module, multiplies it with the scaling factor, adds the bias to
                /// it, then outputs the value.
                PreciseReal GetBias() const
                {
                    return this->m_bias;
                }

                /// Sets the scaling factor to apply to the output value from the
                /// source module.
                ///
                /// @param Scale The scaling factor to apply.
                ///
                /// The GetValue() method retrieves the output value from the source
                /// module, multiplies it with the scaling factor, adds the bias to
                /// it, then outputs the value.
                void SetScale(const PreciseReal Scale)
                {
                    this->m_scale = Scale;
                }
                /// Returns the scaling factor to apply to the output value from the
                /// source module.
                ///
                /// @return Returns the scaling factor to apply.
                ///
                /// The GetValue() method retrieves the output value from the source
                /// module, multiplies it with the scaling factor, adds the bias to
                /// it, then outputs the value.
                PreciseReal GetScale() const
                {
                    return this->m_scale;
                }

                /// @copydoc ModuleBase::GetSourceModuleCount() const
                virtual Integer GetSourceModuleCount() const
                {
                    return 1;
                }

                /// @copydoc ModuleBase::GetValue(const PreciseReal, const PreciseReal, const PreciseReal) const
                virtual PreciseReal GetValue(const PreciseReal X, const PreciseReal Y, const PreciseReal Z) const;
            protected:
                /// @internal
                /// @brief Bias to apply to the scaled output value from the source module.
                PreciseReal m_bias;
                /// @internal
                /// @brief Scaling factor to apply to the output value from the source module.
                PreciseReal m_scale;
            };//ScaleBias
            /// @}

            /// @}

            /// @}
        }//Module
    }//Noise
}//Mezzanine

#endif
