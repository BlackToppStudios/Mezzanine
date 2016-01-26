// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _mathtoolintersectiontools_cpp
#define _mathtoolintersectiontools_cpp

#include "mathtools.h"


/// @cond DontDocumentInternal
namespace
{

}
/// @endcond

namespace Mezzanine
{
    namespace MathTools
    {

        Vector2 AxisAlignedQuad::DropAxisToCreateVector2(const Vector3& Point, const StandardAxis AxisToDrop)
        {
            switch(AxisToDrop)
            {
                case Axis_X: return Vector2(Point.Z, Point.Y);
                case Axis_Y: return Vector2(Point.X, Point.Z);
                case Axis_Z: return Vector2(Point.X, Point.Y);
                default:
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_RANGE_EXCEPTION, "Failed to dropped dimension from Vector3 based on Axis while creating a Vector2");
            }
        }

        Vector3 AxisAlignedQuad::DropAxisToCreateVector2(const Vector2& Point, const StandardAxis AxisToRegain, const Real Missing)
        {
            switch(AxisToRegain)
            {
                case Axis_X: return Vector3(Missing, Point.Y, Point.X);
                case Axis_Y: return Vector3(Point.X, Missing, Point.Y);
                case Axis_Z: return Vector3(Point.X, Point.Y, Missing);
                default:
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_RANGE_EXCEPTION, "");
            }
        }

        AxisAlignedQuad::AxisAlignedQuad(const StandardAxis PlanarAlignment, const Real Distance, const Vector2& Min, const Vector2& Max)
            : AlignedOn(PlanarAlignment),
              DistanceFromOrigin(Distance),
              MinExtents(Min),
              MaxExtents(Max)
        {}

        AxisAlignedQuad::AxisAlignedQuad(const StandardAxis PlanarAlignment, const Vector3& Min, const Vector3& Max)
            : AlignedOn(PlanarAlignment),
              DistanceFromOrigin(Min[PlanarAlignment]),
              MinExtents(AxisAlignedQuad::DropAxisToCreateVector2(Min,PlanarAlignment)),
              MaxExtents(AxisAlignedQuad::DropAxisToCreateVector2(Max,PlanarAlignment))
        {
            if( Min[PlanarAlignment] != Max[PlanarAlignment] )
            {
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_RANGE_EXCEPTION, "Failed to dropped dimension from Vector3 based on Axis while creating a Vector2");
            }
        }

        Boole AxisAlignedQuad::OverlapsWith(const Vector3& Other) const
        {
            if(DistanceFromOrigin == Other[AlignedOn])
                { return OverlapsWith( DropAxisToCreateVector2(Other, AlignedOn) ); }
            return false;
        }

        Boole AxisAlignedQuad::OverlapsWith(const Vector2& Other) const
        {
            if( MinExtents.X <= Other.X && Other.X <= MaxExtents.X &&
                    MinExtents.Y <= Other.Y && Other.Y <= MaxExtents.Y )
                { return true; }
            return false;
        }

        Boole AxisAlignedQuad::operator==(const AxisAlignedQuad& Other) const
        {
            return  AlignedOn == Other.AlignedOn &&
                    DistanceFromOrigin == Other.DistanceFromOrigin &&
                    MinExtents == Other.MinExtents &&
                    MaxExtents == Other.MaxExtents;
        }

    }//MathTools
}//Mezzanine

#endif
