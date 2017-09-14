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
// Copyright (C) 2004 Jason Bevins
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

#ifndef noisemoduletranslatepoint_h
#define noisemoduletranslatepoint_h

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

            /// Default translation factor applied to the @a X coordinate for the
            /// Noise::Module::TranslatePoint noise module.
            const PreciseReal DEFAULT_TRANSLATE_POINT_X = 0.0;

            /// Default translation factor applied to the @a Y coordinate for the
            /// Noise::Module::TranslatePoint noise module.
            const PreciseReal DEFAULT_TRANSLATE_POINT_Y = 0.0;

            /// Default translation factor applied to the @a Z coordinate for the
            /// Noise::Module::TranslatePoint noise module.
            const PreciseReal DEFAULT_TRANSLATE_POINT_Z = 0.0;

            /// Noise module that moves the coordinates of the input value before
            /// returning the output value from a source module.
            ///
            /// The GetValue() method moves the ( @a X, @a Y, @a Z ) coordinates of
            /// the input value by a translation amount before returning the output
            /// value from the source module.  To set the translation amount, call
            /// the SetTranslation() method.  To set the translation amount to
            /// apply to the individual @a X, @a Y, or @a Z coordinates, call the
            /// SetXTranslation(), SetYTranslation() or SetZTranslation() methods,
            /// respectively.
            ///
            /// This noise module requires one source module.
            class TranslatePoint : public ModuleBase
            {
            public:
                /// @brief Constructor.
                ///
                /// The default translation amount to apply to the @a X coordinate is
                /// set to Noise::Module::DEFAULT_TRANSLATE_POINT_X.
                ///
                /// The default translation amount to apply to the @a Y coordinate is
                /// set to Noise::Module::DEFAULT_TRANSLATE_POINT_Y.
                ///
                /// The default translation amount to apply to the @a Z coordinate is
                /// set to Noise::Module::DEFAULT_TRANSLATE_POINT_Z.
                TranslatePoint();
                
                /// Sets the translation amount to apply to the input value.
                ///
                /// @param Translation The translation amount to apply.
                ///
                /// The GetValue() method moves the ( @a X, @a Y, @a Z ) coordinates
                /// of the input value by a translation amount before returning the
                /// output value from the source module
                void SetTranslation(const PreciseReal Translation)
                {
                    this->m_xTranslation = Translation;
                    this->m_yTranslation = Translation;
                    this->m_zTranslation = Translation;
                }
                /// Sets the translation amounts to apply to the ( @a X, @a Y, @a Z )
                /// coordinates of the input value.
                ///
                /// @param XTranslation The translation amount to apply to the @a x
                /// coordinate.
                /// @param YTranslation The translation amount to apply to the @a y
                /// coordinate.
                /// @param ZTranslation The translation amount to apply to the @a z
                /// coordinate.
                ///
                /// The GetValue() method moves the ( @a X, @a Y, @a Z ) coordinates
                /// of the input value by a translation amount before returning the
                /// output value from the source module
                void SetTranslation(const PreciseReal XTranslation, const PreciseReal YTranslation, const PreciseReal ZTranslation)
                {
                    this->m_xTranslation = XTranslation;
                    this->m_yTranslation = YTranslation;
                    this->m_zTranslation = ZTranslation;
                }

                /// Sets the translation amount to apply to the @a X coordinate of the
                /// input value.
                ///
                /// @param XTranslation The translation amount to apply to the @a x
                /// coordinate.
                ///
                /// The GetValue() method moves the ( @a X, @a Y, @a Z ) coordinates
                /// of the input value by a translation amount before returning the
                /// output value from the source module
                void SetXTranslation(const PreciseReal XTranslation)
                {
                    this->m_xTranslation = XTranslation;
                }
                /// Returns the translation amount to apply to the @a X coordinate of
                /// the input value.
                ///
                /// @return Returns the translation amount to apply to the @a X coordinate.
                PreciseReal GetXTranslation() const
                {
                    return this->m_xTranslation;
                }

                /// Sets the translation amount to apply to the @a Y coordinate of the
                /// input value.
                ///
                /// @param YTranslation The translation amount to apply to the @a y
                /// coordinate.
                ///
                /// The GetValue() method moves the ( @a X, @a Y, @a Z ) coordinates
                /// of the input value by a translation amount before returning the
                /// output value from the source module
                void SetYTranslation(const PreciseReal YTranslation)
                {
                    this->m_yTranslation = YTranslation;
                }
                /// Returns the translation amount to apply to the @a Y coordinate of
                /// the input value.
                ///
                /// @return Returns the translation amount to apply to the @a Y coordinate.
                PreciseReal GetYTranslation() const
                {
                    return this->m_yTranslation;
                }

                /// Sets the translation amount to apply to the @a Z coordinate of the
                /// input value.
                ///
                /// @param ZTranslation The translation amount to apply to the @a z
                /// coordinate.
                ///
                /// The GetValue() method moves the ( @a X, @a Y, @a Z ) coordinates
                /// of the input value by a translation amount before returning the
                /// output value from the source module
                void SetZTranslation(const PreciseReal ZTranslation)
                {
                    this->m_zTranslation = ZTranslation;
                }
                /// Returns the translation amount to apply to the @a Z coordinate of
                /// the input value.
                ///
                /// @return Returns the translation amount to apply to the @a Z coordinate.
                PreciseReal GetZTranslation() const
                {
                    return this->m_zTranslation;
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
                /// @brief Translation amount applied to the @a X coordinate of the input value.
                PreciseReal m_xTranslation;
                /// @internal
                /// @brief Translation amount applied to the @a Y coordinate of the input value.
                PreciseReal m_yTranslation;
                /// @internal
                /// @brief Translation amount applied to the @a Z coordinate of the input value.
                PreciseReal m_zTranslation;
            };//TranslatePoint
            /// @}

            /// @}

            /// @}
        }//Module
    }//Noise
}//Mezzanine

#endif
