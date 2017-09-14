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

#ifndef noisemodulerotatepoint_h
#define noisemodulerotatepoint_h

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

            /// Default @a X rotation angle for the Noise::Module::RotatePoint noise
            /// module.
            const PreciseReal DEFAULT_ROTATE_X = 0.0;

            /// Default @a Y rotation angle for the Noise::Module::RotatePoint noise
            /// module.
            const PreciseReal DEFAULT_ROTATE_Y = 0.0;

            /// Default @a Z rotation angle for the Noise::Module::RotatePoint noise
            /// module.
            const PreciseReal DEFAULT_ROTATE_Z = 0.0;

            /// Noise module that rotates the input value around the origin before
            /// returning the output value from a source module.
            ///
            /// The GetValue() method rotates the coordinates of the input value
            /// around the origin before returning the output value from the source
            /// module.  To set the rotation angles, call the SetAngles() method.  To
            /// set the rotation angle around the individual @a X, @a Y, or @a Z axes,
            /// call the SetXAngle(), SetYAngle() or SetZAngle() methods,
            /// respectively.
            ///
            /// The coordinate system of the input value is assumed to be
            /// "left-handed" (@a X increases to the right, @a Y increases upward,
            /// and @a Z increases inward.)
            ///
            /// This noise module requires one source module.
            class RotatePoint : public ModuleBase
            {
            public:
                /// @brief Constructor.
                ///
                /// The default rotation angle around the @a X axis, in radians, is
                /// set to Noise::Module::DEFAULT_ROTATE_X.
                ///
                /// The default rotation angle around the @a Y axis, in radians, is
                /// set to Noise::Module::DEFAULT_ROTATE_Y.
                ///
                /// The default rotation angle around the @a Z axis, in radians, is
                /// set to Noise::Module::DEFAULT_ROTATE_Z.
                RotatePoint();

                /// Sets the rotation angles around all three axes to apply to the
                /// input value.
                ///
                /// @param XAngle The rotation angle around the @a X axis, in radians.
                /// @param YAngle The rotation angle around the @a Y axis, in radians.
                /// @param ZAngle The rotation angle around the @a Z axis, in radians.
                ///
                /// The GetValue() method rotates the coordinates of the input value
                /// around the origin before returning the output value from the
                /// source module.
                void SetAngles(const PreciseReal XAngle, const PreciseReal YAngle, const PreciseReal ZAngle);

                /// Sets the rotation angle around the @a X axis to apply to the input
                /// value.
                ///
                /// @param XAngle The rotation angle around the @a X axis, in radians.
                ///
                /// The GetValue() method rotates the coordinates of the input value
                /// around the origin before returning the output value from the
                /// source module.
                void SetXAngle(const PreciseReal XAngle)
                {
                    this->SetAngles(XAngle, this->m_yAngle, this->m_zAngle);
                }
                /// Returns the rotation angle around the @a X axis to apply to the
                /// input value.
                ///
                /// @return Returns the rotation angle around the @a X axis, in radians.
                PreciseReal GetXAngle() const
                {
                    return this->m_xAngle;
                }

                /// Sets the rotation angle around the @a Y axis to apply to the input
                /// value.
                ///
                /// @param YAngle The rotation angle around the @a Y axis, in radians.
                ///
                /// The GetValue() method rotates the coordinates of the input value
                /// around the origin before returning the output value from the
                /// source module.
                void SetYAngle(const PreciseReal YAngle)
                {
                    this->SetAngles(this->m_xAngle, YAngle, this->m_zAngle);
                }
                /// Returns the rotation angle around the @a Y axis to apply to the
                /// input value.
                ///
                /// @return Returns the rotation angle around the @a Y axis, in radians.
                PreciseReal GetYAngle() const
                {
                    return this->m_yAngle;
                }

                /// Sets the rotation angle around the @a Z axis to apply to the input
                /// value.
                ///
                /// @param ZAngle The rotation angle around the @a Z axis, in radians.
                ///
                /// The GetValue() method rotates the coordinates of the input value
                /// around the origin before returning the output value from the
                /// source module.
                void SetZAngle(const PreciseReal ZAngle)
                {
                    this->SetAngles(this->m_xAngle, this->m_yAngle, ZAngle);
                }
                /// Returns the rotation angle around the @a Z axis to apply to the
                /// input value.
                ///
                /// @return Returns the rotation angle around the @a Z axis, in radians.
                PreciseReal GetZAngle() const
                {
                    return this->m_zAngle;
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
                /// An entry within the 3x3 rotation matrix used for rotating the input value.
                PreciseReal m_x1Matrix;
                /// @internal
                /// An entry within the 3x3 rotation matrix used for rotating the input value.
                PreciseReal m_x2Matrix;
                /// @internal
                /// An entry within the 3x3 rotation matrix used for rotating the input value.
                PreciseReal m_x3Matrix;
                /// @internal
                /// @a X rotation angle applied to the input value, in degrees.
                PreciseReal m_xAngle;
                /// @internal
                /// An entry within the 3x3 rotation matrix used for rotating the input value.
                PreciseReal m_y1Matrix;
                /// @internal
                /// An entry within the 3x3 rotation matrix used for rotating the input value.
                PreciseReal m_y2Matrix;
                /// @internal
                /// An entry within the 3x3 rotation matrix used for rotating the input value.
                PreciseReal m_y3Matrix;
                /// @internal
                /// @a Y rotation angle applied to the input value, in degrees.
                PreciseReal m_yAngle;
                /// @internal
                /// An entry within the 3x3 rotation matrix used for rotating the input value.
                PreciseReal m_z1Matrix;
                /// @internal
                /// An entry within the 3x3 rotation matrix used for rotating the input value.
                PreciseReal m_z2Matrix;
                /// @internal
                /// An entry within the 3x3 rotation matrix used for rotating the input value.
                PreciseReal m_z3Matrix;
                /// @internal
                /// @a Z rotation angle applied to the input value, in degrees.
                PreciseReal m_zAngle;
            };//RotatePoint
            /// @}

            /// @}

            /// @}
        }//Module
    }//Noise
}//Mezzanine

#endif
