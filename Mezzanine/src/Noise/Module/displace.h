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

#ifndef noisemoduledisplace_h
#define noisemoduledisplace_h

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

            /// @defgroup transformermodules Transformer Modules
            /// @addtogroup transformermodules
            /// @{

            /// Noise module that uses three source modules to displace each
            /// coordinate of the input value before returning the output value from
            /// a source module.
            ///
            /// Unlike most other noise modules, the index value assigned to a source
            /// module determines its role in the displacement operation:
            /// - Source module 0 (left in the diagram) outputs a value.
            /// - Source module 1 (lower left in the diagram) specifies the offset to
            ///   apply to the @a X coordinate of the input value.
            /// - Source module 2 (lower center in the diagram) specifies the
            ///   offset to apply to the @a Y coordinate of the input value.
            /// - Source module 3 (lower right in the diagram) specifies the offset
            ///   to apply to the @a Z coordinate of the input value.
            ///
            /// The GetValue() method modifies the ( @a X, @a Y, @a Z ) coordinates of
            /// the input value using the output values from the three displacement
            /// modules before retrieving the output value from the source module.
            ///
            /// The Noise::Module::Turbulence noise module is a special case of the
            /// displacement module; internally, there are three Perlin-noise modules
            /// that perform the displacement operation.
            ///
            /// This noise module requires four source modules.
            class Displace : public ModuleBase
            {
            public:
                /// @brief Constructor.
                Displace();

                /// Sets the @a X, @a Y, and @a Z displacement modules.
                ///
                /// @param XDisplaceModule Displacement module that displaces the @a x
                /// coordinate of the input value.
                /// @param YDisplaceModule Displacement module that displaces the @a y
                /// coordinate of the input value.
                /// @param ZDisplaceModule Displacement module that displaces the @a z
                /// coordinate of the input value.
                ///
                /// The GetValue() method displaces the input value by adding the output
                /// value from each of the displacement modules to the corresponding
                /// coordinates of the input value before returning the output value
                /// from the source module.
                ///
                /// This method assigns an index value of 1 to the @a X displacement
                /// module, an index value of 2 to the @a Y displacement module, and an
                /// index value of 3 to the @a Z displacement module.
                ///
                /// These displacement modules must exist throughout the lifetime of
                /// this noise module unless another displacement module replaces it.
                void SetDisplaceModules(const ModuleBase& XDisplaceModule, const ModuleBase& YDisplaceModule, const ModuleBase& ZDisplaceModule)
                {
                    this->SetXDisplaceModule(XDisplaceModule);
                    this->SetYDisplaceModule(YDisplaceModule);
                    this->SetZDisplaceModule(ZDisplaceModule);
                }

                /// Sets the @a X displacement module.
                ///
                /// @param XDisplaceModule Displacement module that displaces the @a x
                /// coordinate.
                ///
                /// The GetValue() method displaces the input value by adding the output
                /// value from this displacement module to the @a X coordinate of the
                /// input value before returning the output value from the source
                /// module.
                ///
                /// This method assigns an index value of 1 to the @a X displacement
                /// module.  Passing this displacement module to this method produces
                /// the same results as passing this displacement module to the
                /// SetSourceModule() method while assigning it an index value of 1.
                ///
                /// This displacement module must exist throughout the lifetime of this
                /// noise module unless another displacement module replaces it.
                void SetXDisplaceModule(const ModuleBase& XDisplaceModule)
                {
                    assert( this->m_pSourceModule != NULL );
                    this->m_pSourceModule[1] = &XDisplaceModule;
                }
                /// Returns the @a X displacement module.
                ///
                /// @return Returns a reference to the @a X displacement module.
                ///
                /// @pre This displacement module has been added to this noise module
                /// via a call to SetSourceModule() or SetXDisplaceModule().
                ///
                /// @exception Exception::INVALID_STATE_EXCEPTION See the preconditions for more
                /// information.
                ///
                /// The GetValue() method displaces the input value by adding the output
                /// value from this displacement module to the @a X coordinate of the
                /// input value before returning the output value from the source
                /// module.
                const ModuleBase& GetXDisplaceModule() const
                {
                    if( this->m_pSourceModule == NULL || this->m_pSourceModule[1] == NULL ) {
                        MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to retrieve displace module that does not exist.");
                    }
                    return *(this->m_pSourceModule[1]);
                }

                /// Sets the @a Y displacement module.
                ///
                /// @param YDisplaceModule Displacement module that displaces the @a y
                /// coordinate.
                ///
                /// The GetValue() method displaces the input value by adding the output
                /// value from this displacement module to the @a Y coordinate of the
                /// input value before returning the output value from the source
                /// module.
                ///
                /// This method assigns an index value of 2 to the @a Y displacement
                /// module.  Passing this displacement module to this method produces
                /// the same results as passing this displacement module to the
                /// SetSourceModule() method while assigning it an index value of 2.
                ///
                /// This displacement module must exist throughout the lifetime of this
                /// noise module unless another displacement module replaces it.
                void SetYDisplaceModule(const ModuleBase& YDisplaceModule)
                {
                    assert( this->m_pSourceModule != NULL );
                    this->m_pSourceModule[2] = &YDisplaceModule;
                }
                /// Returns the @a Y displacement module.
                ///
                /// @return Returns a reference to the @a Y displacement module.
                ///
                /// @pre This displacement module has been added to this noise module
                /// via a call to SetSourceModule() or SetYDisplaceModule().
                ///
                /// @exception Exception::INVALID_STATE_EXCEPTION See the preconditions for more
                /// information.
                ///
                /// The GetValue() method displaces the input value by adding the output
                /// value from this displacement module to the @a Y coordinate of the
                /// input value before returning the output value from the source
                /// module.
                const ModuleBase& GetYDisplaceModule() const
                {
                    if( this->m_pSourceModule == NULL || this->m_pSourceModule[2] == NULL ) {
                        MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to retrieve displace module that does not exist.");
                    }
                    return *(this->m_pSourceModule[2]);
                }

                /// Sets the @a Z displacement module.
                ///
                /// @param ZDisplaceModule Displacement module that displaces the @a z
                /// coordinate.
                ///
                /// The GetValue() method displaces the input value by adding the output
                /// value from this displacement module to the @a Z coordinate of the
                /// input value before returning the output value from the source
                /// module.
                ///
                /// This method assigns an index value of 3 to the @a Z displacement
                /// module.  Passing this displacement module to this method produces
                /// the same results as passing this displacement module to the
                /// SetSourceModule() method while assigning it an index value of 3.
                ///
                /// This displacement module must exist throughout the lifetime of this
                /// noise module unless another displacement module replaces it.
                void SetZDisplaceModule(const ModuleBase& ZDisplaceModule)
                {
                    assert( this->m_pSourceModule != NULL );
                    this->m_pSourceModule[3] = &ZDisplaceModule;
                }
                /// Returns the @a Z displacement module.
                ///
                /// @return Returns a reference to the @a Z displacement module.
                ///
                /// @pre This displacement module has been added to this noise module
                /// via a call to SetSourceModule() or SetZDisplaceModule().
                ///
                /// @exception Exception::INVALID_STATE_EXCEPTION See the preconditions for more
                /// information.
                ///
                /// The GetValue() method displaces the input value by adding the output
                /// value from this displacement module to the @a Z coordinate of the
                /// input value before returning the output value from the source
                /// module.
                const ModuleBase& GetZDisplaceModule() const
                {
                    if( this->m_pSourceModule == NULL || this->m_pSourceModule[3] == NULL ) {
                        MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to retrieve displace module that does not exist.");
                    }
                    return *(this->m_pSourceModule[3]);
                }

                /// @copydoc ModuleBase::GetSourceModuleCount() const
                virtual Integer GetSourceModuleCount() const
                {
                    return 4;
                }

                /// @copydoc ModuleBase::GetValue(const PreciseReal, const PreciseReal, const PreciseReal) const
                virtual PreciseReal GetValue(const PreciseReal X, const PreciseReal Y, const PreciseReal Z) const;
            };//Displace
            /// @}

            /// @}

            /// @}
        }//Module
    }//Noise
}//Mezzanine

#endif
