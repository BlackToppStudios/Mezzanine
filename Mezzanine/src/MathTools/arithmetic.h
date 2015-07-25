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
#ifndef _mathtool_arithmetic_h
#define _mathtool_arithmetic_h

#include "datatypes.h"
#include "swig.h"

namespace Mezzanine
{
    namespace MathTools
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Real Math & Check Functions

        /// @brief Rounds a Real up.
        /// @param Val The value to be rounded.
        /// @return Returns the result of the rounding.
        Real MEZZ_LIB Ceil(const Real Val);
        /// @brief Rounds a Real down.
        /// @param Val The value to be rounded.
        /// @return Returns the result of the rounding.
        Real MEZZ_LIB Floor(const Real Val);
        /// @brief Raises a Real to a power.
        /// @param Val The base number to be raised.
        /// @param Power The power (or exponent) to raise the Real by.
        /// @return Returns the Raised Eeal.
        Real MEZZ_LIB Pow(const Real Val, const Real Power);
        /// @brief Raises Base-e to the exponent specified.
        /// @remarks In case you are like me and don't know what Base-e is, it's essentially an irrational number (like Pi) that is used
        /// in a number of exponential functions.  It's exact value can't be expressed, but it's value out to 20 decimal places is as follows: @n
        /// 2.71828182845904523536 @n @n
        /// So this method is really: return = (Base-e)^Val
        /// @param Val The exponent to raise Base-e by.
        /// @return Returns a Real that is the computed value of Base-e to the specified power.
        Real MEZZ_LIB Exp(const Real Val);
        /// @brief Gets the square root of a Real.
        /// @param Val The number to be calculated.
        /// @return Returns the calculated square root.
        Real MEZZ_LIB Sqrt(const Real Val);
        /// @brief Gets the absolute alue of a Real.
        /// @param Val The value to get the absolute value of.
        /// @return Returns the absolute value of the Real.
        Real MEZZ_LIB Abs(const Real Val);
        /// @brief Modulo, fo' Reals.  Returns the remainder of the division between two Reals.
        /// @param Numerator The number on top.
        /// @param Denominator The number on bottom.
        /// @return Returns the remainder of division between the two Reals passed in.
        Real MEZZ_LIB Mod(const Real Numerator, const Real Denominator);

        /// @brief Checks to see if two Reals are within a certain range of each other.
        /// @param First The first of two Reals to compare.
        /// @param Second The second of two Reals to compare.
        /// @param Tolerance The leeway allowed in how far the two can be apart.
        /// @return Returns true if the two numbers are within the amount specified by Tolerance of each other, false otherwise.
        Boole MEZZ_LIB WithinTolerance(const Real First, const Real Second, const Real Tolerance);

        /// @brief Extended version of the standard modulo that will wrap negative values.
        /// @param Num The number to operate on.
        /// @param Cap The numerical cap for this operation.
        /// @return Returns the result of the operation, guarenteed to be in the range of 0 to (Cap-1).
        Integer MEZZ_LIB WrappedModulo(const Integer Num, const Integer Cap);

        /// @brief Convenience method that will clamp a type to be within a specified range.
        /// @param Val The value to clamp.
        /// @param Min The minimum allowed value.
        /// @param Max The maximum allowed value.
        /// @return Returns the clamped value.
        template<typename NumType>
        NumType Clamp(const NumType Val, const NumType Min, const NumType Max)
        {
            /// @todo Add an assert?
            return std::max( std::min( Val, Max ), Min );
        }
    }//MathTools
}//Mezzanine

#endif
