// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#include <limits>
#include <algorithm>

/// @cond 0
namespace
{
    /// @typedef SegmentPosPair
    /// @brief This is an std::pair that stores the near and far segement points for ray-aabb intersections.
    typedef std::pair<Mezzanine::Real,Mezzanine::Real> SegmentPosPair;

    /// @brief Helper fucntion for calculating ray-aabb intersections.
    /// @param Axis The axis to be calculated.
    /// @param Cast The ray being casted for the intersection.
    /// @param Box The AABB to check for intersection.
    /// @param PosPair A pair for the near and far distances in the check so far.
    /// @return Returns false if the check has succeeded in ruling out an intersection, true otherwise.
    Mezzanine::Bool CalculateAxis(const Mezzanine::Whole Axis, const Mezzanine::Ray& Cast, const Mezzanine::AxisAlignedBox& Box, SegmentPosPair& PosPair)
    {
        Mezzanine::Vector3 RayDir = Cast.GetDirection();
        Mezzanine::Real Denom = 1 / RayDir[Axis];
        Mezzanine::Real NewStart = ( Box.MinExt[Axis] - Cast.Origin[Axis] ) * Denom;
        Mezzanine::Real NewEnd = ( Box.MaxExt[Axis] - Cast.Origin[Axis] ) * Denom;

        if( NewStart > NewEnd )
            std::swap(NewStart,NewEnd);

        if( NewStart > PosPair.second || NewEnd < PosPair.first )
            return false;

        if( NewStart > PosPair.first )
            PosPair.first = NewStart;

        if( NewEnd < PosPair.second )
            PosPair.second = NewEnd;

        return true;
    }

    const Mezzanine::Real Pi = Mezzanine::Real( 4.0 * Mezzanine::MathTools::ATan(1.0) );
    const Mezzanine::Real TwoPi = ( Mezzanine::Real( 4.0 * Mezzanine::MathTools::ATan(1.0) ) ) * 2.0;
    const Mezzanine::Real HalfPi = ( Mezzanine::Real( 4.0 * Mezzanine::MathTools::ATan(1.0) ) ) * 0.5;
    const Mezzanine::Real QuarterPi = ( Mezzanine::Real( 4.0 * Mezzanine::MathTools::ATan(1.0) ) ) * 0.25;
    const Mezzanine::Real RadToDegMult = Mezzanine::Real( Mezzanine::Real(180.0) / Mezzanine::Real( 4.0 * Mezzanine::MathTools::ATan(1.0) ) );
    const Mezzanine::Real DegToRadMult = Mezzanine::Real( Mezzanine::Real( 4.0 * Mezzanine::MathTools::ATan(1.0) ) / Mezzanine::Real(180.0) );
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
            { return Degrees * DegToRadMult; }

        Real RadiansToDegrees(const Real& Radians)
            { return Radians * RadToDegMult; }

        ///////////////////////////////////////////////////////////////////////////////
        // Geometry Math

        ///////////////////////////////////////////////////////////////////////////////
        // Intersection Query

        Bool IsInside(const AxisAlignedBox& Box, const Vector3& Point)
        {
            return ( Box.MinExt.X <= Point.X && Box.MaxExt.X >= Point.X &&
                     Box.MinExt.Y <= Point.Y && Box.MaxExt.Y >= Point.Y &&
                     Box.MinExt.Z <= Point.Z && Box.MaxExt.Z >= Point.Z );
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
            return ( Plane::S_Both == Surface.GetSide(Box.GetCenter(),Box.GetHalfSize()) );
        }

        Bool Overlap(const Plane& Surface, const Sphere& Ball)
        {
            return ( MathTools::Fabs( Surface.GetDistance( Ball.Center ) ) <= Ball.Radius );
        }

        Bool Overlap(const AxisAlignedBox& Box1, const AxisAlignedBox& Box2)
        {
            return !( Box1.MinExt.X > Box2.MaxExt.X || Box1.MinExt.Y > Box2.MaxExt.Y || Box1.MinExt.Z > Box2.MaxExt.Z ||
                      Box1.MaxExt.X < Box2.MinExt.X || Box1.MaxExt.Y < Box2.MinExt.Y || Box1.MaxExt.Z < Box2.MinExt.Z );
        }

        Bool Overlap(const Sphere& Ball1, const Sphere& Ball2)
        {
            Real Dist = Ball1.Center.Distance(Ball2.Center);
            return ( Dist <= Ball1.Radius + Ball2.Radius );
        }

        Bool Overlap(const Plane& Surface1, const Plane& Surface2)
        {
            return ( ( Surface1.Normal == Surface2.Normal ? ( Surface1.Distance == Surface2.Distance ) : true ) );
        }

        PlaneRayTestResult Intersects(const Plane& Surface, const Ray& Cast)
        {
            // Code in this function is based on the equivalent in Ogre
            Real Denom = Surface.Normal.DotProduct( Cast.GetDirection() );// + Surface.Distance;
            if( MathTools::Fabs(Denom) < std::numeric_limits<Real>::epsilon() ) {
                return PlaneRayTestResult( false, Vector3() );
            }else{
                Real Nom = Surface.Normal.DotProduct( Cast.Origin ) + Surface.Distance;
                Real Distance = -( Nom / Denom );
                if( Distance > Cast.Length() ) {
                    return PlaneRayTestResult( false, Vector3() );
                }else{
                    return PlaneRayTestResult( true, Cast.Origin + ( Cast.GetDirection() * Distance) );
                }
            }
        }

        GeometryRayTestResult Intersects(const AxisAlignedBox& Box, const Ray& Cast)
        {
            // Code in this function is based on the equivalent in Ogre
            Vector3 CastDir = Cast.GetDirection();
            Vector3 AbsoluteDir = CastDir;
            AbsoluteDir.X = MathTools::Fabs( AbsoluteDir.X );
            AbsoluteDir.Y = MathTools::Fabs( AbsoluteDir.Y );
            AbsoluteDir.Z = MathTools::Fabs( AbsoluteDir.Z );

            Whole MaxAxis = 0, MidAxis = 1, MinAxis = 2;
            if( AbsoluteDir[0] < AbsoluteDir[2] ) {
                MaxAxis = 2;
                MinAxis = 1;
            }else if( AbsoluteDir[1] < AbsoluteDir[MinAxis] ) {
                MidAxis = MinAxis;
                MinAxis = 1;
            }else if( AbsoluteDir[1] > AbsoluteDir[MaxAxis] ) {
                MidAxis = MaxAxis;
                MaxAxis = 1;
            }

            SegmentPosPair Distances(0,std::numeric_limits<Real>::infinity());

            ::CalculateAxis(MaxAxis,Cast,Box,Distances);
            if( AbsoluteDir[MidAxis] < std::numeric_limits<Real>::epsilon() ) {
                if( Cast.Origin[MidAxis] < Box.MinExt[MidAxis] || Cast.Origin[MidAxis] > Box.MaxExt[MidAxis] ||
                    Cast.Origin[MinAxis] < Box.MinExt[MinAxis] || Cast.Origin[MinAxis] > Box.MaxExt[MinAxis] )
                {
                    return GeometryRayTestResult(false,Ray());
                }
            }else{
                ::CalculateAxis(MidAxis,Cast,Box,Distances);
                if( AbsoluteDir[MinAxis] < std::numeric_limits<Real>::epsilon() ) {
                    if( Cast.Origin[MinAxis] < Box.MinExt[MinAxis] || Cast.Origin[MinAxis] > Box.MaxExt[MinAxis] ) {
                        return GeometryRayTestResult(false,Ray());
                    }
                }else{
                    ::CalculateAxis(MinAxis,Cast,Box,Distances);
                }
            }

            Ray Ret( Cast.Origin + (CastDir * Distances.first), Cast.Origin + (CastDir * Distances.second) );
            return GeometryRayTestResult(true,Ret);
        }

        GeometryRayTestResult Intersects(const Sphere& Ball, const Ray& Cast)
        {
            // Code in this function is based on the equivalent in Ogre
            const Vector3 CastDir = Cast.GetDirection();
            const Vector3 CastOrigin = Cast.Origin - Ball.Center; // Makes math easier to do this in sphere local coordinates
            const Real Radius = Ball.Radius;

            // Build coefficients for our formula
            // t = (-b +/- sqrt(b*b + 4ac)) / 2a
            Real ACoEff = CastDir.DotProduct(CastDir);
            Real BCoEff = 2 * CastOrigin.DotProduct(CastDir);
            Real CCoEff = CastOrigin.DotProduct(CastOrigin) - ( Radius * Radius );

            // Get the Determinate
            Real Determinate = ( BCoEff * BCoEff ) - ( 4 * ACoEff * CCoEff );
            if( Determinate < 0 ) {
                return GeometryRayTestResult(false,Ray());
            }else{
                Real NearDist = ( -BCoEff - MathTools::Sqrt( Determinate ) ) / ( 2 * ACoEff );
                Real FarDist = ( -BCoEff + MathTools::Sqrt( Determinate ) ) / ( 2 * ACoEff );

                Ray Ret( Cast.Origin + (CastDir * NearDist), Cast.Origin + (CastDir * FarDist) );
                return GeometryRayTestResult(true,Ret);
            }
        }
    }//MathTools
}//Mezzanine

#endif
