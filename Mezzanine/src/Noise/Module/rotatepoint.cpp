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

#ifndef noisemodulerotatepoint_cpp
#define noisemodulerotatepoint_cpp

#include "Noise/Module/rotatepoint.h"

#include "MathTools/constants.h"
#include "MathTools/trigonometry.h"

namespace Mezzanine
{
    namespace Noise
    {
        namespace Module
        {
            RotatePoint::RotatePoint() :
                ModuleBase(this->GetSourceModuleCount())
            {
                this->SetAngles(Module::DEFAULT_ROTATE_X, Module::DEFAULT_ROTATE_Y, Module::DEFAULT_ROTATE_Z);
            }

            PreciseReal RotatePoint::GetValue(const PreciseReal X, const PreciseReal Y, const PreciseReal Z) const
            {
                assert( this->m_pSourceModule[0] != NULL );

                PreciseReal nx = (this->m_x1Matrix * X) + (this->m_y1Matrix * Y) + (this->m_z1Matrix * Z);
                PreciseReal ny = (this->m_x2Matrix * X) + (this->m_y2Matrix * Y) + (this->m_z2Matrix * Z);
                PreciseReal nz = (this->m_x3Matrix * X) + (this->m_y3Matrix * Y) + (this->m_z3Matrix * Z);
                return this->m_pSourceModule[0]->GetValue(nx, ny, nz);
            }

            void RotatePoint::SetAngles(const PreciseReal XAngle, const PreciseReal YAngle, const PreciseReal ZAngle)
            {
                PreciseReal xCos, yCos, zCos, xSin, ySin, zSin;
                xCos = MathTools::Cos( XAngle );
                yCos = MathTools::Cos( YAngle );
                zCos = MathTools::Cos( ZAngle );
                xSin = MathTools::Sin( XAngle );
                ySin = MathTools::Sin( YAngle );
                zSin = MathTools::Sin( ZAngle );

                this->m_x1Matrix = ySin * xSin * zSin + yCos * zCos;
                this->m_y1Matrix = xCos * zSin;
                this->m_z1Matrix = ySin * zCos - yCos * xSin * zSin;
                this->m_x2Matrix = ySin * xSin * zCos - yCos * zSin;
                this->m_y2Matrix = xCos * zCos;
                this->m_z2Matrix = -yCos * xSin * zCos - ySin * zSin;
                this->m_x3Matrix = -ySin * xCos;
                this->m_y3Matrix = xSin;
                this->m_z3Matrix = yCos * xCos;

                this->m_xAngle = XAngle;
                this->m_yAngle = YAngle;
                this->m_zAngle = ZAngle;
            }
        }//Module
    }//Noise
}//Mezzanine

#endif
