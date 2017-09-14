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

#ifndef noisemoduleterrace_h
#define noisemoduleterrace_h

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

            /// Noise module that maps the output value from a source module onto a
            /// terrace-forming curve.
            ///
            /// This noise module maps the output value from the source module onto a
            /// terrace-forming curve.  The start of this curve has a slope of zero;
            /// its slope then smoothly increases.  This curve also contains
            /// <i>control points</i> which resets the slope to zero at that point,
            /// producing a "terracing" effect.  
            ///
            /// To add a control point to this noise module, call the
            /// AddControlPoint() method.
            ///
            /// An application must add a minimum of two control points to the curve.
            /// If this is not done, the GetValue() method fails.  The control points
            /// can have any value, although no two control points can have the same
            /// value.  There is no limit to the number of control points that can be
            /// added to the curve.
            ///
            /// This noise module clamps the output value from the source module if
            /// that value is less than the value of the lowest control point or
            /// greater than the value of the highest control point.
            ///
            /// This noise module is often used to generate terrain features such as
            /// your stereotypical desert canyon.
            ///
            /// This noise module requires one source module.
            class Terrace : public ModuleBase
            {
            public:
                /// @brief Constructor.
                Terrace ();
                /// Destructor.
                ~Terrace ();

                /// Adds a control point to the terrace-forming curve.
                ///
                /// @param Value The value of the control point to add.
                ///
                /// @pre No two control points have the same value.
                ///
                /// @exception Exception::PARAMETERS_EXCEPTION An invalid parameter was
                /// specified; see the preconditions for more information.
                ///
                /// Two or more control points define the terrace-forming curve.  The
                /// start of this curve has a slope of zero; its slope then smoothly
                /// increases.  At the control points, its slope resets to zero.
                ///
                /// It does not matter which order these points are added.
                void AddControlPoint(const PreciseReal Value);
                /// Returns a pointer to the array of control points on the
                /// terrace-forming curve.
                ///
                /// @return Returns a pointer to the array of control points in this noise
                /// module.
                ///
                /// Two or more control points define the terrace-forming curve.  The
                /// start of this curve has a slope of zero; its slope then smoothly
                /// increases.  At the control points, its slope resets to zero.
                ///
                /// Before calling this method, call GetControlPointCount() to
                /// determine the number of control points in this array.
                ///
                /// It is recommended that an application does not store this pointer
                /// for later use since the pointer to the array may change if the
                /// application calls another method of this object.
                const PreciseReal* GetControlPointArray() const
                {
                    return this->m_pControlPoints;
                }
                /// Returns the number of control points on the terrace-forming curve.
                ///
                /// @return Returns the number of control points on the terrace-forming
                /// curve.
                Integer GetControlPointCount() const
                {
                    return this->m_controlPointCount;
                }
                /// Deletes all the control points on the terrace-forming curve.
                ///
                /// @post All control points on the terrace-forming curve are deleted.
                void ClearAllControlPoints();
                
                /// Creates a number of equally-spaced control points that range from
                /// -1 to +1.
                ///
                /// @param ControlPointCount The number of control points to generate.
                ///
                /// @pre The number of control points must be greater than or equal to
                /// 2.
                ///
                /// @post The previous control points on the terrace-forming curve are
                /// deleted.
                ///
                /// @exception Exception::PARAMETERS_EXCEPTION An invalid parameter was
                /// specified; see the preconditions for more information.
                ///
                /// Two or more control points define the terrace-forming curve.  The
                /// start of this curve has a slope of zero; its slope then smoothly
                /// increases.  At the control points, its slope resets to zero.
                void MakeControlPoints(const Integer ControlPointCount);

                /// Enables or disables the inversion of the terrace-forming curve
                /// between the control points.
                ///
                /// @param Invert Specifies whether to invert the curve between the
                /// control points.
                void InvertTerraces(const Boole Invert = true)
                {
                    this->m_invertTerraces = Invert;
                }
                /// Determines if the terrace-forming curve between the control
                /// points is inverted.
                ///
                /// @returns
                /// - @a true if the curve between the control points is inverted.
                /// - @a false if the curve between the control points is not
                ///   inverted.
                Boole IsTerracesInverted() const
                {
                    return this->m_invertTerraces;
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
                  /// @brief Determines the array index in which to insert the control point into the internal control point array.
                  ///
                  /// @param Value The value of the control point.
                  ///
                  /// @return Returns the array index in which to insert the control point.
                  ///
                  /// @pre No two control points have the same value.
                  ///
                  /// @exception Exception::PARAMETERS_EXCEPTION An invalid parameter was
                  /// specified; see the preconditions for more information.
                  ///
                  /// By inserting the control point at the returned array index, this
                  /// class ensures that the control point array is sorted by value.
                  /// The code that maps a value onto the curve requires a sorted
                  /// control point array.
                  Integer FindInsertionPos(const PreciseReal Value);
                  /// @internal
                  /// @brief Inserts the control point at the specified position in the internal control point array.
                  ///
                  /// @param InsertionPos The zero-based array position in which to
                  /// insert the control point.
                  /// @param Value The value of the control point.
                  ///
                  /// To make room for this new control point, this method reallocates
                  /// the control point array and shifts all control points occurring
                  /// after the insertion position up by one.
                  ///
                  /// Because the curve mapping algorithm in this noise module requires
                  /// that all control points in the array be sorted by value, the new
                  /// control point should be inserted at the position in which the
                  /// order is still preserved.
                  void InsertAtPos(const Integer InsertionPos, const PreciseReal Value);
                  
                  /// @internal
                  /// @brief Number of control points stored in this noise module.
                  Integer m_controlPointCount;
                  /// @internal
                  /// @brief Determines if the terrace-forming curve between all control points is inverted.
                  Boole m_invertTerraces;
                  /// @internal
                  /// @brief Array that stores the control points.
                  PreciseReal* m_pControlPoints;
            };//Terrace
            /// @}

            /// @}

            /// @}
        }//Module
    }//Noise
}//Mezzanine

#endif
