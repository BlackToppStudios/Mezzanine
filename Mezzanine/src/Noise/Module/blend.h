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

#ifndef noisemoduleblend_h
#define noisemoduleblend_h

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

            /// @defgroup selectormodules Selector Modules
            /// @addtogroup selectormodules
            /// @{

            /// Noise module that outputs a weighted blend of the output values from
            /// two source modules given the output value supplied by a control module.
            ///
            /// Unlike most other noise modules, the index value assigned to a source
            /// module determines its role in the blending operation:
            /// - Source module 0 (upper left in the diagram) outputs one of the
            ///   values to blend.
            /// - Source module 1 (lower left in the diagram) outputs one of the
            ///   values to blend.
            /// - Source module 2 (bottom of the diagram) is known as the <i>control
            ///   module</i>.  The control module determines the weight of the
            ///   blending operation.  Negative values weigh the blend towards the
            ///   output value from the source module with an index value of 0.
            ///   Positive values weigh the blend towards the output value from the
            ///   source module with an index value of 1.
            ///
            /// An application can pass the control module to the SetControlModule()
            /// method instead of the SetSourceModule() method.  This may make the
            /// application code easier to read.
            ///
            /// This noise module uses linear interpolation to perform the blending
            /// operation.
            ///
            /// This noise module requires three source modules.
            class Blend : public ModuleBase
            {
            public:
                /// @brief Constructor.
                Blend();

                /// Sets the control module.
                ///
                /// @param ControlModule The control module.
                ///
                /// The control module determines the weight of the blending
                /// operation.  Negative values weigh the blend towards the output
                /// value from the source module with an index value of 0.  Positive
                /// values weigh the blend towards the output value from the source
                /// module with an index value of 1.
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
                /// The control module determines the weight of the blending
                /// operation.  Negative values weigh the blend towards the output
                /// value from the source module with an index value of 0.  Positive
                /// values weigh the blend towards the output value from the source
                /// module with an index value of 1.
                const ModuleBase& GetControlModule() const
                {
                    if( this->m_pSourceModule == NULL || this->m_pSourceModule[2] == NULL ) {
                        MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to retrieve control module that does not exist.");
                    }
                    return *(this->m_pSourceModule[2]);
                }

                /// @copydoc ModuleBase::GetSourceModuleCount() const
                virtual Integer GetSourceModuleCount() const
                {
                    return 3;
                }

                /// @copydoc ModuleBase::GetValue(const PreciseReal, const PreciseReal, const PreciseReal) const
                virtual PreciseReal GetValue(const PreciseReal X, const PreciseReal Y, const PreciseReal Z) const;
            };//Blend
            /// @}

            /// @}

            /// @}
        }//Module
    }//Noise
}//Mezzanine

#endif
