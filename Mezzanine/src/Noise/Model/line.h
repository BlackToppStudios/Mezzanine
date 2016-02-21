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
// Copyright (C) 2004 Keith Davies
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

#ifndef noisemodelline_h
#define noisemodelline_h

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

            /// Model that defines the displacement of a line segment.
            ///
            /// This model returns an output value from a noise module given the
            /// one-dimensional coordinate of an input value located on a line
            /// segment, which can be used as displacements.
            ///
            /// This class is useful for creating:
            ///  - roads and rivers
            ///  - disaffected college students
            ///
            /// To generate an output value, pass an input value between 0.0 and 1.0
            /// to the GetValue() method.  0.0 represents the start position of the
            /// line segment and 1.0 represents the end position of the line segment.
            class Line
            {
            public:
                /// @brief Blank constructor.
                Line();
                /// @brief Module constructor.
                /// @param Module The noise module that is used to generate the output values.
                Line(const Module::ModuleBase& Module);

                /// @brief Sets a flag indicating that the output value is to be attenuated
                /// (moved toward 0.0) as the ends of the line segment are approached.
                /// @param Att A flag that specifies whether the output value is to be attenuated.
                void SetAttenuate(const Boole Att)
                {
                    this->m_attenuate = Att;
                }
                /// @brief Returns a flag indicating whether the output value is to be
                /// attenuated (moved toward 0.0) as the ends of the line segment are
                /// approached by the input value.
                /// @return Returns
                /// - @a true if the value is to be attenuated
                /// - @a false if not.
                Boole GetAttenuate() const
                {
                    return this->m_attenuate;
                }

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

                /// @brief Sets the position ( @a X, @a Y, @a Z ) of the start of the line
                /// segment to choose values along.
                /// @param X x coordinate of the start position.
                /// @param Y y coordinate of the start position.
                /// @param Z z coordinate of the start position.
                void SetStartPoint(const PreciseReal X, const PreciseReal Y, const PreciseReal Z)
                {
                    this->m_x0 = X;
                    this->m_y0 = Y;
                    this->m_z0 = Z;
                }
                /// @brief Sets the position ( @a X, @a Y, @a Z ) of the end of the line
                /// segment to choose values along.
                /// @param X x coordinate of the end position.
                /// @param Y y coordinate of the end position.
                /// @param Z z coordinate of the end position.
                void SetEndPoint(const PreciseReal X, const PreciseReal Y, const PreciseReal Z)
                {
                    this->m_x1 = X;
                    this->m_y1 = Y;
                    this->m_z1 = Z;
                }

                /// Returns the output value from the noise module given the
                /// one-dimensional coordinate of the specified input value located
                /// on the line segment.
                ///
                /// @param Pos The distance along the line segment (ranges from 0.0 to 1.0)
                ///
                /// @return Returns the output value from the noise module.
                ///
                /// @pre A noise module was passed to the SetModule() method.
                /// @pre The start and end points of the line segment were specified.
                ///
                /// The output value is generated by the noise module passed to the
                /// SetModule() method.  This value may be attenuated (moved toward
                /// 0.0) as @a p approaches either end of the line segment; this is
                /// the default behavior.
                ///
                /// If the value is not to be attenuated, @a p can safely range
                /// outside the 0.0 to 1.0 range; the output value will be
                /// extrapolated along the line that this segment is part of.
                PreciseReal GetValue(const PreciseReal Pos) const;
            protected:
                /// @internal
                /// @brief A flag that specifies whether the value is to be attenuated
                /// (moved toward 0.0) as the ends of the line segment are approached.
                Boole m_attenuate;
                /// @internal
                /// @brief A pointer to the noise module used to generate the output values.
                const Module::ModuleBase* m_pModule;
                /// @internal
                /// @brief @a X coordinate of the start of the line segment.
                PreciseReal m_x0;
                /// @internal
                /// @brief @a X coordinate of the end of the line segment.
                PreciseReal m_x1;
                /// @internal
                /// @brief @a Y coordinate of the start of the line segment.
                PreciseReal m_y0;
                /// @internal
                /// @brief @a Y coordinate of the end of the line segment.
                PreciseReal m_y1;
                /// @internal
                /// @brief @a Z coordinate of the start of the line segment.
                PreciseReal m_z0;
                /// @internal
                /// @brief @a Z coordinate of the end of the line segment.
                PreciseReal m_z1;
            };//Line

            /// @}

            /// @}
        }//Model
    }//Noise
}//Mezzanine

#endif
