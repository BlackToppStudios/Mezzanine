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
// Copyright (C) 2004 Owen Jacobson
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
// The developer's email is ojacobson@lionsanctuary.net
//

#ifndef noisemodelplane_h
#define noisemodelplane_h

#include <assert.h>
#include "Noise/Module/modulebase.h"

namespace Mezzanine
{
    namespace Noise
    {
        namespace Model
        {
            /// @addtogroup libnoise
            /// @{

            /// @addtogroup models
            /// @{

            /// Model that defines the surface of a plane.
            ///
            /// This model returns an output value from a noise module given the
            /// coordinates of an input value located on the surface of an ( @a X,
            /// @a Z ) plane.
            ///
            /// To generate an output value, pass the ( @a X, @a Z ) coordinates of
            /// an input value to the GetValue() method.
            ///
            /// This model is useful for creating:
            /// - two-dimensional textures
            /// - terrain height maps for local areas
            ///
            /// This plane extends infinitely in both directions.
            class Plane
            {
            public:
                /// @brief Blank constructor.
                Plane();
                /// @brief Module constructor
                /// @param Module The noise module that is used to generate the output values.
                Plane(const Module::ModuleBase& Module);

                /// @brief Sets the noise module that is used to generate the output values.
                /// @param Module The noise module that is used to generate the output values.
                /// @details This noise module must exist for the lifetime of this object,
                /// until you pass a new noise module to this method.
                void SetModule(const Module::ModuleBase& Module)
                {
                    this->m_pModule = &Module;
                }
                /// @brief Returns the noise module that is used to generate the output values.
                /// @return Returns a reference to the noise module.
                /// @pre A noise module was passed to the SetModule() method.
                const Module::ModuleBase& GetModule() const
                {
                    assert( this->m_pModule != NULL );
                    return *(this->m_pModule);
                }

                /// Returns the output value from the noise module given the
                /// ( @a X, @a Z ) coordinates of the specified input value located
                /// on the surface of the plane.
                ///
                /// @param X The @a X coordinate of the input value.
                /// @param Z The @a Z coordinate of the input value.
                ///
                /// @return Returns the output value from the noise module.
                ///
                /// @pre A noise module was passed to the SetModule() method.
                ///
                /// This output value is generated by the noise module passed to the
                /// SetModule() method.
                PreciseReal GetValue(const PreciseReal X, const PreciseReal Z) const;
            protected:
                /// @internal
                /// @brief A pointer to the noise module used to generate the output values.
                const Module::ModuleBase* m_pModule;
            };//Plane
            /// @}

            /// @}
        }//Model
    }//Noise
}//Mezzanine

#endif
