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
#ifndef _mathtool_cpp
#define _mathtool_cpp

#include "mathtools.h"
#include "sphere.h"
#include "axisalignedbox.h"
#include "ray.h"
#include "plane.h"
#include "linesegment.h"

#include <cmath>
#include <limits>
#include <algorithm>

/// @cond DontDocumentInternal
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
    Mezzanine::Boole CalculateAxis(const Mezzanine::Whole Axis, const Mezzanine::Ray& Cast, const Mezzanine::AxisAlignedBox& Box, SegmentPosPair& PosPair)
    {
        Mezzanine::Vector3 RayDir = Cast.GetNormal();
        Mezzanine::Real Denom = 1 / RayDir[Axis];
        Mezzanine::Real NewStart = ( Box.MinExt[Axis] - Cast.GetOrigin()[Axis] ) * Denom;
        Mezzanine::Real NewEnd = ( Box.MaxExt[Axis] - Cast.GetOrigin()[Axis] ) * Denom;

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
}
/// @endcond

namespace Mezzanine
{
    namespace MathTools
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Geometry Math

        ///////////////////////////////////////////////////////////////////////////////
        // 2D Geometry Intersection Query

        Point2DTestResult Intersects(const LineSegment2D& Line1, const LineSegment2D& Line2)
        {
            Vector2 d1 = Line1.PointB - Line1.PointA;
            Real a1 = d1.Y;
            Real b1 = -d1.X;
            Real g1 = d1.X * Line1.PointA.Y - d1.Y * Line1.PointA.X;

            Vector2 d3 = Line2.PointB - Line2.PointA;
            Real a2 = d3.Y;
            Real b2 = -d3.X;
            Real g2 = d3.X * Line2.PointA.Y - d3.Y * Line2.PointA.X;

            // if both segments are parallel, early out
            if( d1.CrossProduct(d3) == 0.0 )
                return Point2DTestResult(false,Vector2());

            Real IntersectX = ( b2 * g1 - b1 * g2 ) / ( b1 * a2 - b2 * a1 );
            Real IntersectY = ( a2 * g1 - a1 * g2 ) / ( a1 * b2 - a2 * b1 );

            Vector2 Intersect(IntersectX,IntersectY);
            if( ( Intersect - Line1.PointA ).DotProduct( Intersect - Line1.PointB ) < 0 && ( Intersect - Line2.PointA ).DotProduct( Intersect - Line2.PointB ) < 0 ) {
                return Point2DTestResult(true,Intersect);
            }
            return Point2DTestResult(false,Vector2());
        }

        ///////////////////////////////////////////////////////////////////////////////
        // 3D Geometry Intersection Query

        Boole IsInside(const AxisAlignedBox& Box, const Vector3& Point)
        {
            return ( Box.MinExt.X <= Point.X && Box.MaxExt.X >= Point.X &&
                     Box.MinExt.Y <= Point.Y && Box.MaxExt.Y >= Point.Y &&
                     Box.MinExt.Z <= Point.Z && Box.MaxExt.Z >= Point.Z );
        }

        Boole IsInside(const Sphere& Ball, const Vector3& Point)
        {
            return ( Ball.Center.Distance(Point) <= Ball.Radius );
        }

        Boole Overlap(const AxisAlignedBox& Box, const Sphere& Ball)
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

        Boole Overlap(const AxisAlignedBox& Box, const Plane& Surface)
        {
            return ( Plane::S_Both == Surface.GetSide(Box.GetCenter(),Box.GetHalfSize()) );
        }

        Boole Overlap(const Plane& Surface, const Sphere& Ball)
        {
            return ( MathTools::Abs( Surface.GetDistance( Ball.Center ) ) <= Ball.Radius );
        }

        Boole Overlap(const AxisAlignedBox& Box1, const AxisAlignedBox& Box2)
        {
            return !( Box1.MinExt.X > Box2.MaxExt.X || Box1.MinExt.Y > Box2.MaxExt.Y || Box1.MinExt.Z > Box2.MaxExt.Z ||
                      Box1.MaxExt.X < Box2.MinExt.X || Box1.MaxExt.Y < Box2.MinExt.Y || Box1.MaxExt.Z < Box2.MinExt.Z );
        }

        Boole Overlap(const Sphere& Ball1, const Sphere& Ball2)
        {
            Real Dist = Ball1.Center.Distance(Ball2.Center);
            return ( Dist <= Ball1.Radius + Ball2.Radius );
        }

        Boole Overlap(const Plane& Surface1, const Plane& Surface2)
        {
            return ( ( Surface1.Normal == Surface2.Normal ? ( Surface1.Distance == Surface2.Distance ) : true ) );
        }

        Point3DTestResult Intersects(const Plane& Surface, const Ray& Cast)
        {
            // Code in this function is based on the equivalent in Ogre
            Real Denom = Surface.Normal.DotProduct( Cast.GetNormal() );// + Surface.Distance;
            if( MathTools::Abs(Denom) < std::numeric_limits<Real>::epsilon() ) {
                return Point3DTestResult( false, Vector3() );
            }else{
                Real Nom = Surface.Normal.DotProduct( Cast.GetOrigin() ) + Surface.Distance;
                Real Distance = -( Nom / Denom );
                return Point3DTestResult( true, Cast.GetOrigin() + ( Cast.GetNormal() * Distance) );
            }
            return Point3DTestResult( false, Vector3() );
        }

        GeometryRayTestResult Intersects(const AxisAlignedBox& Box, const Ray& Cast)
        {
            // Code in this function is based on the equivalent in Ogre
            Vector3 CastDir = Cast.GetNormal();
            Vector3 AbsoluteDir = CastDir;
            AbsoluteDir.X = MathTools::Abs( AbsoluteDir.X );
            AbsoluteDir.Y = MathTools::Abs( AbsoluteDir.Y );
            AbsoluteDir.Z = MathTools::Abs( AbsoluteDir.Z );

            // A small fixed sized constant time sorting algorithm for sorting the length of each axis.
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

            if(IsInside(Box,Cast.Origin))
            {
                Vector3 Intersects;
                Intersects[MinAxis] = 0;
                Intersects[MidAxis] = 0;
                Intersects[MaxAxis] = 1;
                /*Plane Side(Intersects,)
                if(CastDir[MaxAxis]>0)
                {

                }else{

                }
                return GeometryRayTestResult(true,Ray(,Vector3));*/
            }

            SegmentPosPair Distances(0,std::numeric_limits<Real>::infinity());

            ::CalculateAxis(MaxAxis,Cast,Box,Distances);
            if( AbsoluteDir[MidAxis] < std::numeric_limits<Real>::epsilon() ) {
                if( Cast.GetOrigin()[MidAxis] < Box.MinExt[MidAxis] || Cast.GetOrigin()[MidAxis] > Box.MaxExt[MidAxis] ||
                    Cast.GetOrigin()[MinAxis] < Box.MinExt[MinAxis] || Cast.GetOrigin()[MinAxis] > Box.MaxExt[MinAxis] )
                {
                    return GeometryRayTestResult(false,Ray());
                }
            }else{
                ::CalculateAxis(MidAxis,Cast,Box,Distances);
                if( AbsoluteDir[MinAxis] < std::numeric_limits<Real>::epsilon() ) {
                    if( Cast.GetOrigin()[MinAxis] < Box.MinExt[MinAxis] || Cast.GetOrigin()[MinAxis] > Box.MaxExt[MinAxis] ) {
                        return GeometryRayTestResult(false,Ray());
                    }
                }else{
                    ::CalculateAxis(MinAxis,Cast,Box,Distances);
                }
            }

            Ray Ret( Cast.GetOrigin() + (CastDir * Distances.first), Cast.GetOrigin() + (CastDir * Distances.second) );
            return GeometryRayTestResult(true,Ret);
        }

        GeometryRayTestResult Intersects(const Sphere& Ball, const Ray& Cast)
        {
            // Code in this function is based on the equivalent in Ogre
            const Vector3 CastDir = Cast.GetNormal();
            const Vector3 CastOrigin = Cast.GetOrigin() - Ball.Center; // Makes math easier to do this in sphere local coordinates
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

                Ray Ret( Cast.GetOrigin() + (CastDir * NearDist), Cast.GetOrigin() + (CastDir * FarDist) );
                return GeometryRayTestResult(true,Ret);
            }
        }
    }//MathTools
}//Mezzanine

#endif
