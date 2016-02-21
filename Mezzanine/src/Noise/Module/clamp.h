// clamp.h
//
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

#ifndef noisemoduleclamp_h
#define noisemoduleclamp_h

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

            /// @brief Default lower bound of the clamping range for the Noise::Module::Clamp noise module.
            const PreciseReal DEFAULT_CLAMP_LOWER_BOUND = -1.0;

            /// @brief Default upper bound of the clamping range for the Noise::Module::Clamp noise module.
            const PreciseReal DEFAULT_CLAMP_UPPER_BOUND = 1.0;

            /// Noise module that clamps the output value from a source module to a
            /// range of values.
            ///
            /// The range of values in which to clamp the output value is called the
            /// <i>clamping range</i>.
            ///
            /// If the output value from the source module is less than the lower
            /// bound of the clamping range, this noise module clamps that value to
            /// the lower bound.  If the output value from the source module is
            /// greater than the upper bound of the clamping range, this noise module
            /// clamps that value to the upper bound.
            ///
            /// To specify the upper and lower bounds of the clamping range, call the
            /// SetBounds() method.
            ///
            /// This noise module requires one source module.
            class Clamp: public ModuleBase
            {
            public:
                /// @brief Constructor.
                ///
                /// The default lower bound of the clamping range is set to
                /// Noise::Module::DEFAULT_CLAMP_LOWER_BOUND.
                ///
                /// The default upper bound of the clamping range is set to
                /// Noise::Module::DEFAULT_CLAMP_UPPER_BOUND.
                Clamp();

                /// Sets the lower and upper bounds of the clamping range.
                ///
                /// @param LowerBound The lower bound.
                /// @param UpperBound The upper bound.
                ///
                /// @pre The lower bound must be less than or equal to the
                /// upper bound.
                ///
                /// If the output value from the source module is less than the lower
                /// bound of the clamping range, this noise module clamps that value
                /// to the lower bound.  If the output value from the source module
                /// is greater than the upper bound of the clamping range, this noise
                /// module clamps that value to the upper bound.
                void SetBounds(const PreciseReal LowerBound, const PreciseReal UpperBound);

                /// Returns the upper bound of the clamping range.
                ///
                /// @return Returns the upper bound.
                ///
                /// If the output value from the source module is greater than the
                /// upper bound of the clamping range, this noise module clamps that
                /// value to the upper bound.
                PreciseReal GetUpperBound() const
                {
                    return this->m_upperBound;
                }

                /// Returns the lower bound of the clamping range.
                ///
                /// @return Returns the lower bound.
                ///
                /// If the output value from the source module is less than the lower
                /// bound of the clamping range, this noise module clamps that value
                /// to the lower bound.
                PreciseReal GetLowerBound() const
                {
                    return this->m_lowerBound;
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
                /// @brief Lower bound of the clamping range.
                PreciseReal m_lowerBound;
                /// @internal
                /// @brief Upper bound of the clamping range.
                PreciseReal m_upperBound;
            };//Clamp
            /// @}

            /// @}

            /// @}
        }//Module
    }//Noise
}//Mezzanine

#endif
