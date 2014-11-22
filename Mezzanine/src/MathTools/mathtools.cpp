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

    }//MathTools
}//Mezzanine

#endif
