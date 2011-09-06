//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _mathtool_h
#define _mathtool_h

#include "datatypes.h"

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class MathTool
    /// @headerfile mathtool.h
    /// @brief This provides a number of generic math related utilities.
    /// @details
    ///////////////////////////////////////
    class PHYS_LIB MathTool
    {
        protected:
            static const Real Pi;
            static const Real TwoPi;
            static const Real HalfPi;
        public:
            /// @brief Class constructor.
            MathTool();

            /// @brief Class destructor.
            ~MathTool();

            /// @brief Gets Pi.
            /// @return Returns a real representing Pi.
            static Real GetPi();

            /// @brief Gets Pi * 2.
            /// @return Returns a real representing Pi * 2.
            static Real GetTwoPi();

            /// @brief Gets Pi * 0.5.
            /// @return Returns a real representing Pi * 0.5.
            static Real GetHalfPi();

            /// @brief Rounds a Real up.
            /// @param Val The value to be rounded.
            /// @return Returns the result of the rounding.
            static Real Ceil(const Real& Val);

            /// @brief Rounds a Real down.
            /// @param Val The value to be rounded.
            /// @return Returns the result of the rounding.
            static Real Floor(const Real& Val);

            /// @brief Raises a Real to a power.
            /// @param Val The base number to be raised.
            /// @param Power The power (or exponent) to raise the Real by.
            /// @return Returns the Raised Eeal.
            static Real Pow(const Real& Val, const Real& Power);

            /// @brief Gets the square root of a Real.
            /// @param Val The number to be calculated.
            /// @return Returns the calculated square root.
            static Real Sqrt(const Real& Val);

            /// @brief Gets the absolute value of a Real.
            /// @param Val The value to get the absolute value of.
            /// @return Returns the absolute value of the Real.
            static Real Fabs(const Real& Val);

            /// @brief Modulo, for Reals.  Returns the remainder of the division between two Reals.
            /// @param Numerator The number on top.
            /// @param Denominator The number on bottom.
            /// @return Returns the remainder of division between the two Reals passed in.
            static Real Fmod(const Real& Numerator, const Real& Denominator);

            /// @brief Calculates the Cosine of an angle.
            /// @param Radians The angle to be computed in Radians.
            /// @return Returns a Real representing the Cosine of the angle provided.
            static Real Cos(const Real& Radians);

            /// @brief Calculates the Sine of an angle.
            /// @param Radians The angle to be computed in Radians.
            /// @return Returns a Real representing the Sine of the angle provided.
            static Real Sin(const Real& Radians);

            /// @brief Calculates the Tangent of an angle.
            /// @param Radians The angle to be computed in Radians.
            /// @return Returns a Real representing the Tangent of the angle provided.
            static Real Tan(const Real& Radians);

            /// @brief Calculates the principal value of the arc Cosine of an angle.
            /// @param Interval The Interval to calculate the arc Cosine.  Range is -1.0 to 1.0.
            /// @return Returns a Real representing the principal value of the arc Cosine of the angle provided in Radians.
            static Real ACos(const Real& Interval);

            /// @brief Calculates the principal value of the arc Sine of an angle.
            /// @param Interval The Interval to calculate the arc Cosine.  Range is -1.0 to 1.0.
            /// @return Returns a Real representing the principal value of the arc Sine of the angle provided in Radians.
            static Real ASin(const Real& Interval);

            /// @brief Calculates the principal value of the arc Tangent of an angle.
            /// @param Interval The Interval to calculate the arc Cosine.  Range is -1.0 to 1.0.
            /// @return Returns a Real representing the principal value of the arc Tangent of the angle provided in Radians.
            static Real ATan(const Real& Interval);
    };//mathtool
}//phys

#endif
