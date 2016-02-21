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

#ifndef noisemodulescalepoint_h
#define noisemodulescalepoint_h

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

            /// @addtogroup transformermodules
            /// @{

            /// Default scaling factor applied to the @a X coordinate for the
            /// Noise::Module::ScalePoint noise module.
            const PreciseReal DEFAULT_SCALE_POINT_X = 1.0;

            /// Default scaling factor applied to the @a Y coordinate for the
            /// Noise::Module::ScalePoint noise module.
            const PreciseReal DEFAULT_SCALE_POINT_Y = 1.0;

            /// Default scaling factor applied to the @a Z coordinate for the
            /// Noise::Module::ScalePoint noise module.
            const PreciseReal DEFAULT_SCALE_POINT_Z = 1.0;

            /// Noise module that scales the coordinates of the input value before
            /// returning the output value from a source module.
            ///
            /// The GetValue() method multiplies the ( @a X, @a Y, @a Z ) coordinates
            /// of the input value with a scaling factor before returning the output
            /// value from the source module.  To set the scaling factor, call the
            /// SetScale() method.  To set the scaling factor to apply to the
            /// individual @a X, @a Y, or @a Z coordinates, call the SetXScale(),
            /// SetYScale() or SetZScale() methods, respectively.
            ///
            /// This noise module requires one source module.
            class ScalePoint : public ModuleBase
            {
            public:
                /// @brief Constructor.
                ///
                /// The default scaling factor applied to the @a X coordinate is set
                /// to Noise::Module::DEFAULT_SCALE_POINT_X.
                ///
                /// The default scaling factor applied to the @a Y coordinate is set
                /// to Noise::Module::DEFAULT_SCALE_POINT_Y.
                ///
                /// The default scaling factor applied to the @a Z coordinate is set
                /// to Noise::Module::DEFAULT_SCALE_POINT_Z.
                ScalePoint();

                /// Sets the scaling factor to apply to the input value.
                ///
                /// @param Scale The scaling factor to apply.
                ///
                /// The GetValue() method multiplies the ( @a X, @a Y, @a Z )
                /// coordinates of the input value with a scaling factor before
                /// returning the output value from the source module.
                void SetScale(const PreciseReal Scale)
                {
                    this->m_xScale = Scale;
                    this->m_yScale = Scale;
                    this->m_zScale = Scale;
                }
                /// Sets the scaling factor to apply to the ( @a X, @a Y, @a Z )
                /// coordinates of the input value.
                ///
                /// @param XScale The scaling factor to apply to the @a X coordinate.
                /// @param YScale The scaling factor to apply to the @a Y coordinate.
                /// @param ZScale The scaling factor to apply to the @a Z coordinate.
                ///
                /// The GetValue() method multiplies the ( @a X, @a Y, @a Z )
                /// coordinates of the input value with a scaling factor before
                /// returning the output value from the source module.
                void SetScale(const PreciseReal XScale, const PreciseReal YScale, const PreciseReal ZScale)
                {
                    this->m_xScale = XScale;
                    this->m_yScale = YScale;
                    this->m_zScale = ZScale;
                }

                /// Sets the scaling factor to apply to the @a X coordinate of the
                /// input value.
                ///
                /// @param XScale The scaling factor to apply to the @a X coordinate.
                ///
                /// The GetValue() method multiplies the ( @a X, @a Y, @a Z )
                /// coordinates of the input value with a scaling factor before
                /// returning the output value from the source module.
                void SetXScale(const PreciseReal XScale)
                {
                    this->m_xScale = XScale;
                }
                /// Returns the scaling factor applied to the @a X coordinate of the
                /// input value.
                ///
                /// @return Returns the scaling factor applied to the @a X coordinate.
                PreciseReal GetXScale() const
                {
                    return this->m_xScale;
                }

                /// Sets the scaling factor to apply to the @a Y coordinate of the
                /// input value.
                ///
                /// @param YScale The scaling factor to apply to the @a Y coordinate.
                ///
                /// The GetValue() method multiplies the ( @a X, @a Y, @a Z )
                /// coordinates of the input value with a scaling factor before
                /// returning the output value from the source module.
                void SetYScale(const PreciseReal YScale)
                {
                    this->m_yScale = YScale;
                }
                /// Returns the scaling factor applied to the @a Y coordinate of the
                /// input value.
                ///
                /// @return Returns the scaling factor applied to the @a Y coordinate.
                PreciseReal GetYScale() const
                {
                    return this->m_yScale;
                }

                /// Sets the scaling factor to apply to the @a Z coordinate of the
                /// input value.
                ///
                /// @param ZScale The scaling factor to apply to the @a Z coordinate.
                ///
                /// The GetValue() method multiplies the ( @a X, @a Y, @a Z )
                /// coordinates of the input value with a scaling factor before
                /// returning the output value from the source module.
                void SetZScale(const PreciseReal ZScale)
                {
                    this->m_zScale = ZScale;
                }
                /// Returns the scaling factor applied to the @a Z coordinate of the
                /// input value.
                ///
                /// @return Returns the scaling factor applied to the @a Z coordinate.
                PreciseReal GetZScale() const
                {
                    return this->m_zScale;
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
                /// @brief Scaling factor applied to the @a X coordinate of the input value.
                PreciseReal m_xScale;
                /// @internal
                /// @brief Scaling factor applied to the @a Y coordinate of the input value.
                PreciseReal m_yScale;
                /// @internal
                /// @brief Scaling factor applied to the @a Z coordinate of the input value.
                PreciseReal m_zScale;
            };//ScalePoint
            /// @}

            /// @}

            /// @}
        }//Module
    }//Noise
}//Mezzanine

#endif
