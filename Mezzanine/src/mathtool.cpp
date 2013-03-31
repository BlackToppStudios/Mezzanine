//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _mathtool_cpp
#define _mathtool_cpp

#include "mathtool.h"

#include <cmath>

namespace Mezzanine
{
    const Real MathTool::Pi = Real(4.0 * atan(1.0));
    const Real MathTool::TwoPi = Real(2.0 * Pi);
    const Real MathTool::HalfPi = Real(0.5 * Pi);
    const Real MathTool::RadToDegMult = Real(Real(180.0) / Pi);
    const Real MathTool::DegToRadMult = Real(Pi / Real(180.0));

    MathTool::MathTool()
    {
    }

    MathTool::~MathTool()
    {
    }

    Real MathTool::GetPi()
    {
        return Pi;
    }

    Real MathTool::GetTwoPi()
    {
        return TwoPi;
    }

    Real MathTool::GetHalfPi()
    {
        return HalfPi;
    }

    Real MathTool::Ceil(const Real& Val)
    {
        return ceil(Val);
    }

    Real MathTool::Floor(const Real& Val)
    {
        return floor(Val);
    }

    Real MathTool::Pow(const Real& Val, const Real& Power)
    {
        return pow(Val,Power);
    }

    Real MathTool::Sqrt(const Real& Val)
    {
        return sqrt(Val);
    }

    Real MathTool::Fabs(const Real& Val)
    {
        return fabs(Val);
    }

    Real MathTool::Fmod(const Real& Numerator, const Real& Denominator)
    {
        return fmod(Numerator,Denominator);
    }

    bool MathTool::WithinTolerance(const Real& First, const Real& Second, const Real& Tolerance)
    {
        if( Fabs(Second - First) <= Tolerance ) return true;
        else return false;
    }

    Real MathTool::Cos(const Real& Radians)
    {
        return cos(Radians);
    }

    Real MathTool::Sin(const Real& Radians)
    {
        return sin(Radians);
    }

    Real MathTool::Tan(const Real& Radians)
    {
        return tan(Radians);
    }

    Real MathTool::ACos(const Real& Interval)
    {
        return acos(Interval);
    }

    Real MathTool::ASin(const Real& Interval)
    {
        return asin(Interval);
    }

    Real MathTool::ATan(const Real& Interval)
    {
        return atan(Interval);
    }

    Real MathTool::DegreesToRadians(const Real& Degrees)
    {
        return Degrees * DegToRadMult;
    }

    Real MathTool::RadiansToDegrees(const Real& Radians)
    {
        return Radians * RadToDegMult;
    }
}//Mezzanine

#endif
