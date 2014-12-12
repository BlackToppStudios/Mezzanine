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
#ifndef _mathtool_constants_cpp
#define _mathtool_constants_cpp

#include "MathTools/constants.h"
#include "mathtools.h" //TODO: change this to a more limited include at some point, ATan is required not everything

/// @cond DontDocumentInternal
namespace
{
    const Mezzanine::Real Pi = Mezzanine::Real( 4.0 * Mezzanine::MathTools::ATan(1.0) );
    const Mezzanine::Real TwoPi = ( Mezzanine::Real( 4.0 * Mezzanine::MathTools::ATan(1.0) ) ) * 2.0;
    const Mezzanine::Real HalfPi = ( Mezzanine::Real( 4.0 * Mezzanine::MathTools::ATan(1.0) ) ) * 0.5;
    const Mezzanine::Real QuarterPi = ( Mezzanine::Real( 4.0 * Mezzanine::MathTools::ATan(1.0) ) ) * 0.25;
    const Mezzanine::Real RadToDegMult = Mezzanine::Real( Mezzanine::Real(180.0) / Mezzanine::Real( 4.0 * Mezzanine::MathTools::ATan(1.0) ) );
    const Mezzanine::Real DegToRadMult = Mezzanine::Real( Mezzanine::Real( 4.0 * Mezzanine::MathTools::ATan(1.0) ) / Mezzanine::Real(180.0) );
    const Mezzanine::Real Sqrt2 = Mezzanine::Real( Mezzanine::MathTools::Sqrt(2.0) );
    const Mezzanine::Real Sqrt3 = Mezzanine::Real( Mezzanine::MathTools::Sqrt(3.0) );
}
/// @endcond

namespace Mezzanine
{
    namespace MathTools
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Predefined Constants

        Real GetPi()
            { return Pi; }

        Real GetTwoPi()
            { return TwoPi; }

        Real GetHalfPi()
            { return HalfPi; }

        Real GetQuarterPi()
            { return QuarterPi; }

        Real GetRadToDegMultiplier()
            { return RadToDegMult; }

        Real GetDegToRadMultiplier()
            { return DegToRadMult; }

    }//MathTools
}//Mezzanine

#endif
