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
#ifndef _mathtool_trigonometry_h
#define _mathtool_trigonometry_h

#include "datatypes.h"
#include "swig.h"

#include "MathTools/constants.h"
#include "MathTools/intersections.h"


namespace Mezzanine
{
    namespace MathTools
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Angle Calculation Functions and Values

        /// @brief Calculates the Cosine of an angle.
        /// @param Radians The angle to be computed in Radians.
        /// @return Returns a Real representing the Cosine of the angle provided.
        Real MEZZ_LIB Cos(const Real& Radians);
        /// @brief Calculates the Sine of an angle.
        /// @param Radians The angle to be computed in Radians.
        /// @return Returns a Real representing the Sine of the angle provided.
        Real MEZZ_LIB Sin(const Real& Radians);
        /// @brief Calculates the Tangent of an angle.
        /// @param Radians The angle to be computed in Radians.
        /// @return Returns a Real representing the Tangent of the angle provided.
        Real MEZZ_LIB Tan(const Real& Radians);
        /// @brief Calculates the principal value of the arc Cosine of an angle.
        /// @param Interval The Interval to calculate the arc Cosine.  Range is -1.0 to 1.0.
        /// @return Returns a Real representing the principal value of the arc Cosine of the angle provided in Radians.
        Real MEZZ_LIB ACos(const Real& Interval);
        /// @brief Calculates the principal value of the arc Sine of an angle.
        /// @param Interval The Interval to calculate the arc Cosine.  Range is -1.0 to 1.0.
        /// @return Returns a Real representing the principal value of the arc Sine of the angle provided in Radians.
        Real MEZZ_LIB ASin(const Real& Interval);
        /// @brief Calculates the principal value of the arc Tangent of an angle.
        /// @param Interval The Interval to calculate the arc Cosine.  Range is -1.0 to 1.0.
        /// @return Returns a Real representing the principal value of the arc Tangent of the angle provided in Radians.
        Real MEZZ_LIB ATan(const Real& Interval);

        ///////////////////////////////////////////////////////////////////////////////
        // Angle Conversion Functions and Values

        /// @brief Converts an angle in Degree's to the same angle Radians.
        /// @param Degrees The number of Degree's to convert.
        /// @return Returns the converted number in Radians.
        Real MEZZ_LIB DegreesToRadians(const Real& Degrees);
        /// @brief Converts an angle in Radians's to the same angle Degrees.
        /// @param Radians The number of Radians's to convert.
        /// @return Returns the converted number in Degrees.
        Real MEZZ_LIB RadiansToDegrees(const Real& Radians);

    }//MathTools
}//Mezzanine

#endif
