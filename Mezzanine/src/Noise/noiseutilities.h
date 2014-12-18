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

#ifndef _noiseutilities_h
#define _noiseutilities_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace Noise
    {
        /// @brief Converts latitude/longitude coordinates on a unit sphere into 3D Cartesian coordinates.
        /// @remarks No bounds checking is performed internally, so if the values provided are not within the expected range then you can get some undesired results.
        /// @param Latitude The latitude, in degrees.  Must range from @b -90 to @b +90.
        /// @param Longitude The longitude, in degrees.  Must range from @b -180 to @b +180.
        /// @param X On exit, this parameter contains the @a X coordinate.
        /// @param Y On exit, this parameter contains the @a Y coordinate.
        /// @param Z On exit, this parameter contains the @a Z coordinate.
        void MEZZ_LIB ConvertSphereDegreesToCartesian(const PreciseReal Latitude, const PreciseReal Longitude, PreciseReal& X, PreciseReal& Y, PreciseReal& Z);
        /// @brief Converts latitude/longitude coordinates on a unit sphere into 3D Cartesian coordinates.
        /// @remarks No bounds checking is performed internally, so if the values provided are not within the expected range then you can get some undesired results.
        /// @param Latitude The latitude, in degrees.  Must range from @b -pi*0.5 to @b +pi*0.5.
        /// @param Longitude The longitude, in degrees.  Must range from @b -pi to @b +pi.
        /// @param X On exit, this parameter contains the @a X coordinate.
        /// @param Y On exit, this parameter contains the @a Y coordinate.
        /// @param Z On exit, this parameter contains the @a Z coordinate.
        void MEZZ_LIB ConvertSphereRadiansToCartesian(const PreciseReal Latitude, const PreciseReal Longitude, PreciseReal& X, PreciseReal& Y, PreciseReal& Z);
    }//Noise
}//Mezzanine

#endif
