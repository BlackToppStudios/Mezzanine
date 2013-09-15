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
#include "sphere.h"
#include "axisalignedbox.h"
#include "ray.h"
#include "plane.h"

#include <cmath>

namespace Mezzanine
{
    namespace MathTools
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Real Math & Check Functions

        Real Ceil(const Real& Val)
            { return std::ceil(Val); }

        Real Floor(const Real& Val)
            { return std::floor(Val); }

        Real Pow(const Real& Val, const Real& Power)
            { return std::pow(Val,Power); }

        Real Sqrt(const Real& Val)
            { return std::sqrt(Val); }

        Real Fabs(const Real& Val)
            { return std::fabs(Val); }

        Real Fmod(const Real& Numerator, const Real& Denominator)
            { return std::fmod(Numerator,Denominator); }

        bool WithinTolerance(const Real& First, const Real& Second, const Real& Tolerance)
            { return ( MathTools::Fabs(Second - First) <= Tolerance ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Angle Calculation Functions

        Real Cos(const Real& Radians)
            { return std::cos(Radians); }

        Real Sin(const Real& Radians)
            { return std::sin(Radians); }

        Real Tan(const Real& Radians)
            { return std::tan(Radians); }

        Real ACos(const Real& Interval)
            { return std::acos(Interval); }

        Real ASin(const Real& Interval)
            { return std::asin(Interval); }

        Real ATan(const Real& Interval)
            { return std::atan(Interval); }

        ///////////////////////////////////////////////////////////////////////////////
        // Angle Conversion Functions

        Real DegreesToRadians(const Real& Degrees)
            { return Degrees * MathTools::DegToRadMult; }

        Real RadiansToDegrees(const Real& Radians)
            { return Radians * MathTools::RadToDegMult; }

        ///////////////////////////////////////////////////////////////////////////////
        // Geometry Math

        ///////////////////////////////////////////////////////////////////////////////
        // Intersection Query

        Bool IsInside(const AxisAlignedBox& Box, const Vector3& Point)
        {

        }

        Bool IsInside(const Sphere& Ball, const Vector3& Point)
        {
            return ( Ball.Center.Distance(Point) <= Ball.Radius );
        }

        Bool Overlap(const AxisAlignedBox& Box, const Sphere& Ball)
        {
            // Arvo's algorithm
            Real SquareVar, Dist = 0;
            for( Whole X = 0 ; X < 3 ; ++X )
            {
                if ( Ball.Center[X] < Box.MinExt[X] ) {
                    SquareVar = Ball.Center[X] - Box.MinExt[X];
                    Dist += SquareVar * SquareVar;
                }else if( Ball.Center[X] > Box.MaxExt[X] ) {
                    SquareVar = Ball.Center[X] - Box.MaxExt[X];
                    Dist += SquareVar * SquareVar;
                }
            }
            return ( Dist <= Ball.Radius * Ball.Radius );
        }

        Bool Overlap(const AxisAlignedBox& Box, const Plane& Surface)
        {

        }

        Bool Overlap(const Plane& Surface, const Sphere& Ball)
        {

        }

        Bool Overlap(const AxisAlignedBox& Box1, const AxisAlignedBox& Box2)
        {

        }

        Bool Overlap(const Sphere& Ball1, const Sphere& Ball2)
        {
            Real Dist = Ball1.Center.Distance(Ball2.Center);
            return ( Dist <= Ball1.Radius || Dist <= Ball2.Radius );
        }

        Bool Overlap(const Plane& Surface1, const Plane& Surface2)
        {

        }

        Bool Intersects(const Plane& Surface, const Ray& Cast)
        {

        }

        GeometryRayTestResult Intersects(const AxisAlignedBox& Box, const Ray& Cast)
        {

        }

        GeometryRayTestResult Intersects(const Sphere& Ball, const Ray& Cast)
        {

        }
    }//MathTools
}//Mezzanine

#endif
