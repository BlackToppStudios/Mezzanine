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
#ifndef _axisalignedbox_cpp
#define _axisalignedbox_cpp

/// @file
/// @brief This file contains the implementation for the AxisAlignedBox class for representing AABB's of objects in the world.

#include "axisalignedbox.h"

namespace Mezzanine
{
    AxisAlignedBox::AxisAlignedBox()
        { this->AABBMin.Zero(); this->AABBMax.Zero(); }

    AxisAlignedBox::AxisAlignedBox(const Vector3& Min, const Vector3& Max) :
        AABBMin(Min),
        AABBMax(Max)
        {  }

    AxisAlignedBox::~AxisAlignedBox()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Bool AxisAlignedBox::Intersects(const Vector3& ToCheck) const
    {

    }

    Bool AxisAlignedBox::Intersects(const AxisAlignedBox& ToCheck) const
    {

    }

    Bool AxisAlignedBox::Intersects(const Plane& ToCheck) const
    {

    }

    AxisAlignedBox::RayTestResult AxisAlignedBox::Intersects(const Ray& ToCheck) const
    {

    }

    ///////////////////////////////////////////////////////////////////////////////
    // Extents query

    void AxisAlignedBox::SetExtents(const Vector3& Min, const Vector3& Max)
    {
        this->SetMin(Min);
        this->SetMax(Max);
    }

    void AxisAlignedBox::SetMin(const Vector3& Min)
        { this->AABBMin = Min; }

    const Vector3& AxisAlignedBox::GetMin() const
        { return this->AABBMin; }

    void AxisAlignedBox::SetMax(const Vector3& Max)
        { this->AABBMax = Max; }

    const Vector3& AxisAlignedBox::GetMax() const
        { return this->AABBMax; }

    Vector3 AxisAlignedBox::GetSize() const
        { return ( this->AABBMax - this->AABBMin ); }

    Vector3 AxisAlignedBox::GetCenter() const
        { return ( this->AABBMax + this->AABBMin ) * 0.5; }

    Vector3 AxisAlignedBox::GetCorner(const AxisAlignedBox::AxisExtent XEx, const AxisAlignedBox::AxisExtent YEx, const AxisAlignedBox::AxisExtent ZEx) const
    {
        return Vector3( ( XEx == AE_Min ? this->AABBMin.X : this->AABBMax.X ),
                        ( YEx == AE_Min ? this->AABBMin.Y : this->AABBMax.Y ),
                        ( ZEx == AE_Min ? this->AABBMin.Z : this->AABBMax.Z ) );
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Operators

    Bool AxisAlignedBox::operator==(const AxisAlignedBox& Other) const
        { return ( this->AABBMin == Other.AABBMin && this->AABBMax == Other.AABBMax ); }

    Bool AxisAlignedBox::operator!=(const AxisAlignedBox& Other) const
        { return ( this->AABBMin != Other.AABBMin || this->AABBMax != Other.AABBMax ); }
}//Mezzanine

#endif
