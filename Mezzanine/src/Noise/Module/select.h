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

#ifndef noisemoduleselect_h
#define noisemoduleselect_h

#include "Noise/Module/modulebase.h"

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

            /// @addtogroup selectormodules
            /// @{

            /// Default edge-falloff value for the Noise::Module::Select noise module.
            const PreciseReal DEFAULT_SELECT_EDGE_FALLOFF = 0.0;

            /// Default lower bound of the selection range for the
            /// Noise::Module::Select noise module.
            const PreciseReal DEFAULT_SELECT_LOWER_BOUND = -1.0;

            /// Default upper bound of the selection range for the
            /// Noise::Module::Select noise module.
            const PreciseReal DEFAULT_SELECT_UPPER_BOUND = 1.0;

            /// Noise module that outputs the value selected from one of two source
            /// modules chosen by the output value from a control module.
            ///
            /// Unlike most other noise modules, the index value assigned to a source
            /// module determines its role in the selection operation:
            /// - Source module 0 (upper left in the diagram) outputs a value.
            /// - Source module 1 (lower left in the diagram) outputs a value.
            /// - Source module 2 (bottom of the diagram) is known as the <i>control
            ///   module</i>.  The control module determines the value to select.  If
            ///   the output value from the control module is within a range of values
            ///   known as the <i>selection range</i>, this noise module outputs the
            ///   value from the source module with an index value of 1.  Otherwise,
            ///   this noise module outputs the value from the source module with an
            ///   index value of 0.
            ///
            /// To specify the bounds of the selection range, call the SetBounds()
            /// method.
            ///
            /// An application can pass the control module to the SetControlModule()
            /// method instead of the SetSourceModule() method.  This may make the
            /// application code easier to read.
            ///
            /// By default, there is an abrupt transition between the output values
            /// from the two source modules at the selection-range boundary.  To
            /// smooth the transition, pass a non-zero value to the SetEdgeFalloff()
            /// method.  Higher values result in a smoother transition.
            ///
            /// This noise module requires three source modules.
            class Select : public ModuleBase
            {
            public:
                /// @brief Constructor.
                ///
                /// The default falloff value at the edge transition is set to
                /// Noise::Module::DEFAULT_SELECT_EDGE_FALLOFF.
                ///
                /// The default lower bound of the selection range is set to
                /// Noise::Module::DEFAULT_SELECT_LOWER_BOUND.
                ///
                /// The default upper bound of the selection range is set to
                /// Noise::Module::DEFAULT_SELECT_UPPER_BOUND.
                Select();

                /// Sets the control module.
                ///
                /// @param ControlModule The control module.
                ///
                /// The control module determines the output value to select.  If the
                /// output value from the control module is within a range of values
                /// known as the <i>selection range</i>, the GetValue() method outputs
                /// the value from the source module with an index value of 1.
                /// Otherwise, this method outputs the value from the source module
                /// with an index value of 0.
                ///
                /// This method assigns the control module an index value of 2.
                /// Passing the control module to this method produces the same
                /// results as passing the control module to the SetSourceModule()
                /// method while assigning that noise module an index value of 2.
                ///
                /// This control module must exist throughout the lifetime of this
                /// noise module unless another control module replaces that control
                /// module.
                void SetControlModule(const ModuleBase& ControlModule)
                {
                    assert( this->m_pSourceModule != NULL );
                    this->m_pSourceModule[2] = &ControlModule;
                }
                /// Returns the control module.
                ///
                /// @return Returns a reference to the control module.
                ///
                /// @pre A control module has been added to this noise module via a
                /// call to SetSourceModule() or SetControlModule().
                ///
                /// @exception Exception::INVALID_STATE_EXCEPTION See the preconditions for more
                /// information.
                ///
                /// The control module determines the output value to select.  If the
                /// output value from the control module is within a range of values
                /// known as the <i>selection range</i>, the GetValue() method outputs
                /// the value from the source module with an index value of 1.
                /// Otherwise, this method outputs the value from the source module
                /// with an index value of 0.
                const ModuleBase& GetControlModule() const
                {
                    if( this->m_pSourceModule == NULL || this->m_pSourceModule[2] == NULL ) {
                        MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to retrieve control module that does not exist.");
                    }
                    return *(this->m_pSourceModule[2]);
                }
                
                /// Sets the lower and upper bounds of the selection range.
                ///
                /// @param LowerBound The lower bound.
                /// @param UpperBound The upper bound.
                ///
                /// @pre The lower bound must be less than or equal to the upper
                /// bound.
                ///
                /// If the output value from the control module is within the
                /// selection range, the GetValue() method outputs the value from the
                /// source module with an index value of 1.  Otherwise, this method
                /// outputs the value from the source module with an index value of 0.
                void SetBounds(const PreciseReal LowerBound, const PreciseReal UpperBound);
                /// Returns the lower bound of the selection range.
                ///
                /// @return Returns the lower bound of the selection range.
                ///
                /// If the output value from the control module is within the
                /// selection range, the GetValue() method outputs the value from the
                /// source module with an index value of 1.  Otherwise, this method
                /// outputs the value from the source module with an index value of 0.
                PreciseReal GetLowerBound() const
                {
                    return this->m_lowerBound;
                }
                /// Returns the upper bound of the selection range.
                ///
                /// @return Returns the upper bound of the selection range.
                ///
                /// If the output value from the control module is within the
                /// selection range, the GetValue() method outputs the value from the
                /// source module with an index value of 1.  Otherwise, this method
                /// outputs the value from the source module with an index value of 0.
                PreciseReal GetUpperBound() const
                {
                    return this->m_upperBound;
                }

                /// Sets the falloff value at the edge transition.
                ///
                /// @param EdgeFalloff The falloff value at the edge transition.
                ///
                /// The falloff value is the width of the edge transition at either
                /// edge of the selection range.
                ///
                /// By default, there is an abrupt transition between the values from
                /// the two source modules at the boundaries of the selection range.
                ///
                /// For example, if the selection range is 0.5 to 0.8, and the edge
                /// falloff value is 0.1, then the GetValue() method outputs:
                /// - the output value from the source module with an index value of 0
                ///   if the output value from the control module is less than 0.4
                ///   ( = 0.5 - 0.1).
                /// - a linear blend between the two output values from the two source
                ///   modules if the output value from the control module is between
                ///   0.4 ( = 0.5 - 0.1) and 0.6 ( = 0.5 + 0.1).
                /// - the output value from the source module with an index value of 1
                ///   if the output value from the control module is between 0.6
                ///   ( = 0.5 + 0.1) and 0.7 ( = 0.8 - 0.1).
                /// - a linear blend between the output values from the two source
                ///   modules if the output value from the control module is between
                ///   0.7 ( = 0.8 - 0.1 ) and 0.9 ( = 0.8 + 0.1).
                /// - the output value from the source module with an index value of 0
                ///   if the output value from the control module is greater than 0.9
                ///   ( = 0.8 + 0.1).
                void SetEdgeFalloff(const PreciseReal EdgeFalloff);
                /// Returns the falloff value at the edge transition.
                ///
                /// @return Returns the falloff value at the edge transition.
                ///
                /// The falloff value is the width of the edge transition at either
                /// edge of the selection range.
                ///
                /// By default, there is an abrupt transition between the output
                /// values from the two source modules at the selection-range
                /// boundary.
                PreciseReal GetEdgeFalloff() const
                {
                    return this->m_edgeFalloff;
                }

                /// @copydoc ModuleBase::GetSourceModuleCount() const
                virtual Integer GetSourceModuleCount() const
                {
                    return 3;
                }

                /// @copydoc ModuleBase::GetValue(const PreciseReal, const PreciseReal, const PreciseReal) const
                virtual PreciseReal GetValue(const PreciseReal X, const PreciseReal Y, const PreciseReal Z) const;
            protected:
                /// @internal
                /// @brief Edge-falloff value.
                PreciseReal m_edgeFalloff;
                /// @internal
                /// @brief Lower bound of the selection range.
                PreciseReal m_lowerBound;
                /// @internal
                /// @brief Upper bound of the selection range.
                PreciseReal m_upperBound;
            };//Select
            /// @}

            /// @}

            /// @}
        }//Module
    }//Noise
}//Mezzanine

#endif
