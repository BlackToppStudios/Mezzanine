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

#ifndef noisemodulecurve_h
#define noisemodulecurve_h

#include "Noise/Module/modulebase.h"

namespace Mezzanine
{
    namespace Noise
    {
        namespace Module
        {
            /// @addtogroup libnoise
            /// @{

            /// This structure defines a control point.
            ///
            /// Control points are used for defining splines.
            struct ControlPoint
            {
                /// @brief The input value.
                PreciseReal InputValue;
                /// @brief The output value that is mapped from the input value.
                PreciseReal OutputValue;
            };//ControlPoint

            /// @addtogroup modules
            /// @{

            /// @addtogroup modifiermodules
            /// @{

            /// Noise module that maps the output value from a source module onto an
            /// arbitrary function curve.
            ///
            /// This noise module maps the output value from the source module onto an
            /// application-defined curve.  This curve is defined by a number of
            /// <i>control points</i>; each control point has an <i>input value</i>
            /// that maps to an <i>output value</i>.  Refer to the following
            /// illustration:
            ///
            /// To add the control points to this curve, call the AddControlPoint()
            /// method.
            ///
            /// Since this curve is a cubic spline, an application must add a minimum
            /// of four control points to the curve.  If this is not done, the
            /// GetValue() method fails.  Each control point can have any input and
            /// output value, although no two control points can have the same input
            /// value.  There is no limit to the number of control points that can be
            /// added to the curve.
            ///
            /// This noise module requires one source module.
            class Curve : public ModuleBase
            {
            public:
                /// @brief Constructor.
                Curve ();
                /// Destructor.
                ~Curve ();

                /// Adds a control point to the curve.
                ///
                /// @param InputValue The input value stored in the control point.
                /// @param OutputValue The output value stored in the control point.
                ///
                /// @pre No two control points have the same input value.
                ///
                /// @exception Exception::PARAMETERS_EXCEPTION An invalid parameter was
                /// specified; see the preconditions for more information.
                ///
                /// It does not matter which order these points are added.
                void AddControlPoint(const PreciseReal InputValue, const PreciseReal OutputValue);

                /// Deletes all the control points on the curve.
                ///
                /// @post All points on the curve are deleted.
                void ClearAllControlPoints();

                /// Returns a pointer to the array of control points on the curve.
                ///
                /// @return Returns a pointer to the array of control points.
                ///
                /// Before calling this method, call GetControlPointCount() to
                /// determine the number of control points in this array.
                ///
                /// It is recommended that an application does not store this pointer
                /// for later use since the pointer to the array may change if the
                /// application calls another method of this object.
                const ControlPoint* GetControlPointArray() const
                {
                    return this->m_pControlPoints;
                }

                /// Returns the number of control points on the curve.
                ///
                /// @return Returns the number of control points on the curve.
                Integer GetControlPointCount() const
                {
                    return this->m_controlPointCount;
                }

                /// @copydoc ModuleBase::GetSourceModuleCount() const
                virtual Integer GetSourceModuleCount() const
                {
                    return 1;
                }

                /// @copydoc ModuleBase::GetValue(const PreciseReal, const PreciseReal, const PreciseReal) const
                virtual PreciseReal GetValue(const PreciseReal X, const PreciseReal Y, const PreciseReal Z) const;
            protected:
                /// Determines the array index in which to insert the control point
                /// into the internal control point array.
                ///
                /// @param InputValue The input value of the control point.
                ///
                /// @return Returns the array index in which to insert the control point.
                ///
                /// @pre No two control points have the same input value.
                ///
                /// @exception Exception::PARAMETERS_EXCEPTION An invalid parameter was
                /// specified; see the preconditions for more information.
                ///
                /// By inserting the control point at the returned array index, this
                /// class ensures that the control point array is sorted by input
                /// value.  The code that maps a value onto the curve requires a
                /// sorted control point array.
                Integer FindInsertionPos(const PreciseReal InputValue);
                /// Inserts the control point at the specified position in the
                /// internal control point array.
                ///
                /// @param InsertionPos The zero-based array position in which to
                /// insert the control point.
                /// @param InputValue The input value stored in the control point.
                /// @param OutputValue The output value stored in the control point.
                ///
                /// To make room for this new control point, this method reallocates
                /// the control point array and shifts all control points occurring
                /// after the insertion position up by one.
                ///
                /// Because the curve mapping algorithm used by this noise module
                /// requires that all control points in the array must be sorted by
                /// input value, the new control point should be inserted at the
                /// position in which the order is still preserved.
                void InsertAtPos(const Integer InsertionPos, const PreciseReal InputValue, const PreciseReal OutputValue);

                /// @internal
                /// @brief Number of control points on the curve.
                Integer m_controlPointCount;
                /// @internal
                /// @brief Array that stores the control points.
                ControlPoint* m_pControlPoints;
            };//Curve
            /// @}

            /// @}

            /// @}
        }//Module
    }//Noise
}//Mezzanine

#endif
