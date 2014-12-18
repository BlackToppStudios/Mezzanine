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

#ifndef noiseinterpolate_h
#define noiseinterpolate_h

namespace Mezzanine
{
    namespace Noise
    {
        /// @addtogroup libnoise
        /// @{

        /// @brief Performs cubic interpolation between two values bound between two other values.
        /// @param n0 The value before the first value.
        /// @param n1 The first value.
        /// @param n2 The second value.
        /// @param n3 The value after the second value.
        /// @param Alpha The alpha value.  This should range from 0.0 to 1.0.
        /// @return Returns the interpolated value.
        /// @details The alpha value should range from 0.0 to 1.0.  If the alpha value is
        /// 0.0, this function returns @a n1.  If the alpha value is 1.0, this
        /// function returns @a n2.
        inline PreciseReal CubicInterpolate(const PreciseReal n0, const PreciseReal n1, const PreciseReal n2, const PreciseReal n3, const PreciseReal Alpha)
        {
            PreciseReal p = (n3 - n2) - (n0 - n1);
            PreciseReal q = (n0 - n1) - p;
            PreciseReal r = n2 - n0;
            PreciseReal s = n1;
            return p * Alpha * Alpha * Alpha + q * Alpha * Alpha + r * Alpha + s;
        }

        /// @brief Performs linear interpolation between two values.
        /// @param n0 The first value.
        /// @param n1 The second value.
        /// @param Alpha The alpha value.  This should range from 0.0 to 1.0.
        /// @return Returns the interpolated value.
        /// @details The alpha value should range from 0.0 to 1.0.  If the alpha value is
        /// 0.0, this function returns @a n0.  If the alpha value is 1.0, this
        /// function returns @a n1.
        inline PreciseReal LinearInterpolate(const PreciseReal n0, const PreciseReal n1, const PreciseReal Alpha)
        {
            return ((1.0 - Alpha) * n0) + (Alpha * n1);
        }

        /// @brief Maps a value onto a cubic S-curve.
        /// @param Alpha The value to map onto a cubic S-curve.  This should range from 0.0 to 1.0.
        /// @return Returns the mapped value.
        /// @details The derivitive of a cubic S-curve is zero at @a a = 0.0 and @a a = 1.0.
        inline PreciseReal SCurve3(const PreciseReal Alpha)
        {
            return (Alpha * Alpha * (3.0 - 2.0 * Alpha));
        }

        /// @brief Maps a value onto a quintic S-curve.
        /// @param Alpha The value to map onto a quintic S-curve.  This should range from 0.0 to 1.0.
        /// @return Returns the mapped value.
        /// @details The first derivitive of a quintic S-curve is zero at @a a = 0.0 and
        /// @a a = 1.0. @n @n
        ///
        /// The second derivitive of a quintic S-curve is zero at @a a = 0.0 and
        /// @a a = 1.0.
        inline PreciseReal SCurve5(const PreciseReal Alpha)
        {
            PreciseReal a3 = Alpha * Alpha * Alpha;
            PreciseReal a4 = a3 * Alpha;
            PreciseReal a5 = a4 * Alpha;
            return (6.0 * a5) - (15.0 * a4) + (10.0 * a3);
        }
        // @}
    }//Noise
}//Mezzanine

#endif
