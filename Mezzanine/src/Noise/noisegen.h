// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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

#ifndef noisenoisegen_h
#define noisenoisegen_h

#include "Noise/noiseenumerations.h"

#include "MathTools/arithmetic.h"

namespace Mezzanine
{
    namespace Noise
    {
        /// @addtogroup libnoise
        /// @{

        /// @brief Generates a gradient-coherent-noise value from the coordinates of a three-dimensional input value.
        /// @param X The @a X coordinate of the input value.
        /// @param Y The @a Y coordinate of the input value.
        /// @param Z The @a Z coordinate of the input value.
        /// @param Seed The random number Seed.
        /// @param Quality The quality of the coherent-noise.
        /// @return Returns the generated gradient-coherent-noise value.  The return value ranges from -1.0 to +1.0.
        /// @details For an explanation of the difference between <i>gradient</i> noise and
        /// <i>value</i> noise, see the comments for the GradientNoise3D() function.
        PreciseReal MEZZ_LIB GradientCoherentNoise3D(const PreciseReal X, const PreciseReal Y, const PreciseReal Z, const Integer Seed = 0, const NoiseQuality Quality = NQ_Standard);

        /// @brief Generates a gradient-noise value from the coordinates of a three-dimensional input value and the integer coordinates of a nearby three-dimensional value.
        /// @note The difference between the float and integer components for each of the provided inputs must not exceed 1.
        /// @param fX The Realing-point @a X coordinate of the input value.
        /// @param fY The Realing-point @a Y coordinate of the input value.
        /// @param fZ The Realing-point @a Z coordinate of the input value.
        /// @param iX The integer @a X coordinate of a nearby value.
        /// @param iY The integer @a Y coordinate of a nearby value.
        /// @param iZ The integer @a Z coordinate of a nearby value.
        /// @param Seed The random number Seed.
        /// @return Returns the generated gradient-noise value.  The return value ranges from -1.0 to +1.0.
        /// @details A <i>gradient</i>-noise function generates better-quality noise than a
        /// <i>value</i>-noise function.  Most noise modules use gradient noise for
        /// this reason, although it takes much longer to calculate. @n @n
        ///
        /// This function generates a gradient-noise value by performing the
        /// following steps: @n
        /// - It first calculates a random normalized vector based on the
        ///   nearby integer value passed to this function. @n
        /// - It then calculates a new value by adding this vector to the
        ///   nearby integer value passed to this function. @n
        /// - It then calculates the dot product of the above-generated value
        ///   and the Realing-point input value passed to this function. @n
        /// @n
        /// A noise function differs from a random-number generator because it
        /// always returns the same output value if the same input value is passed
        /// to it.
        PreciseReal MEZZ_LIB GradientNoise3D(const PreciseReal fX, const PreciseReal fY, const PreciseReal fZ, const Integer iX, const Integer iY, const Integer iZ, const Integer Seed = 0);

        /// @brief Generates an integer-noise value from the coordinates of a three-dimensional input value.
        /// @param X The integer @a X coordinate of the input value.
        /// @param Y The integer @a Y coordinate of the input value.
        /// @param Z The integer @a Z coordinate of the input value.
        /// @param Seed A random number Seed.
        /// @return Returns the generated integer-noise value.  The return value ranges from 0 to 2147483647.
        /// @details A noise function differs from a random-number generator because it
        /// always returns the same output value if the same input value is passed
        /// to it.
        Integer MEZZ_LIB IntValueNoise3D(const Integer X, const Integer Y, const Integer Z, const Integer Seed = 0);

        /// @brief Modifies a Realing-point value so that it can be stored in a Int32 variable.
        /// @note This function does not modify @a Num.
        /// @param Num A Realing-point number.
        /// @return Returns the modified Realing-point number.
        /// @details In libnoise, the noise-generating algorithms are all integer-based;
        /// they use variables of type Int32.  Before calling a noise
        /// function, pass the @a X, @a Y, and @a Z coordinates to this function to
        /// ensure that these coordinates can be cast to a Int32 value. @n @n
        ///
        /// Although you could do a straight cast from PreciseReal to Int32, the
        /// resulting value may differ between platforms.  By using this function,
        /// you ensure that the resulting value is identical between platforms.
        inline PreciseReal MakeInt32Range(const PreciseReal Num)
        {
            if( Num >= 1073741824.0 ) {
                return ( 2.0 * MathTools::Mod( Num, 1073741824.0 ) ) - 1073741824.0;
            }else if( Num <= -1073741824.0 ) {
                return ( 2.0 * MathTools::Mod( Num, 1073741824.0 ) ) + 1073741824.0;
            }else{
                return Num;
            }
        }

        /// @brief Generates a value-coherent-noise value from the coordinates of a three-dimensional input value.
        /// @param X The @a X coordinate of the input value.
        /// @param Y The @a Y coordinate of the input value.
        /// @param Z The @a Z coordinate of the input value.
        /// @param Seed The random number Seed.
        /// @param Quality The quality of the coherent-noise.
        /// @return Returns the generated value-coherent-noise value.  The return value ranges from -1.0 to +1.0.
        /// @details For an explanation of the difference between <i>gradient</i> noise and
        /// <i>value</i> noise, see the comments for the GradientNoise3D() function.
        PreciseReal MEZZ_LIB ValueCoherentNoise3D(const PreciseReal X, const PreciseReal Y, const PreciseReal Z, const Integer Seed = 0, const NoiseQuality Quality = NQ_Standard);

        /// @brief Generates a value-noise value from the coordinates of a three-dimensional input value.
        /// @param X The @a X coordinate of the input value.
        /// @param Y The @a Y coordinate of the input value.
        /// @param Z The @a Z coordinate of the input value.
        /// @param Seed A random number Seed.
        /// @return Returns the generated value-noise value.  The return value ranges from -1.0 to +1.0.
        /// @details A noise function differs from a random-number generator because it
        /// always returns the same output value if the same input value is passed
        /// to it.
        PreciseReal MEZZ_LIB ValueNoise3D(const Integer X, const Integer Y, const Integer Z, const Integer Seed = 0);

        /// @}
    }//Noise
}//Mezzanine

#endif
