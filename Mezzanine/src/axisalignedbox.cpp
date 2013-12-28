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
#ifndef _axisalignedbox_cpp
#define _axisalignedbox_cpp

/// @file
/// @brief This file contains the implementation for the AxisAlignedBox class for representing AABB's of objects in the world.

#include "axisalignedbox.h"
#include "mathtool.h"
#include "plane.h"
#include "ray.h"
#include "exception.h"
#include "serialization.h"

#include <Ogre.h>

#include <algorithm>

namespace Mezzanine
{
    AxisAlignedBox::AxisAlignedBox()
        {  }

    AxisAlignedBox::AxisAlignedBox(const AxisAlignedBox& Other) :
        MinExt(Other.MinExt),
        MaxExt(Other.MaxExt)
        {  }

    AxisAlignedBox::AxisAlignedBox(const Vector3& Min, const Vector3& Max) :
        MinExt(Min),
        MaxExt(Max)
        {  }

    AxisAlignedBox::AxisAlignedBox(const Ogre::AxisAlignedBox& InternalAABB)
        { this->ExtractOgreAABB(InternalAABB); }

    AxisAlignedBox::~AxisAlignedBox()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Boolean AxisAlignedBox::IsZero() const
    {
        return ( this->MinExt.X >= this->MaxExt.X &&
                 this->MinExt.Y >= this->MaxExt.Y &&
                 this->MinExt.Z >= this->MaxExt.Z );
    }

    Real AxisAlignedBox::GetVolume() const
    {
        Vector3 Diff = this->MaxExt - this->MinExt;
        return ( Diff.X * Diff.Y * Diff.Z );
    }

    AxisAlignedBox AxisAlignedBox::GetOverlap(const AxisAlignedBox& Other) const
    {
        Vector3 NewMin = this->MinExt, NewMax = this->MaxExt;
        NewMin.Ceil(Other.MinExt);
        NewMax.Floor(Other.MaxExt);

        if( NewMin.X < NewMax.X && NewMin.Y < NewMax.Y && NewMin.Z < NewMax.Z ) {
            return AxisAlignedBox(NewMin,NewMax);
        }else{
            return AxisAlignedBox();
        }
    }

    void AxisAlignedBox::Expand(const Vector3& Point)
    {
        this->MinExt.Floor(Point);
        this->MaxExt.Ceil(Point);
    }

    void AxisAlignedBox::Expand(const AxisAlignedBox& Other)
    {
        this->MinExt.Floor(Other.MinExt);
        this->MaxExt.Ceil(Other.MaxExt);
    }

    Boolean AxisAlignedBox::IsInside(const Vector3& ToCheck) const
        { return MathTools::IsInside(*this,ToCheck); }

    Boolean AxisAlignedBox::IsOverlapping(const Sphere& ToCheck) const
        { return MathTools::Overlap(*this,ToCheck); }

    Boolean AxisAlignedBox::IsOverlapping(const AxisAlignedBox& ToCheck) const
        { return MathTools::Overlap(*this,ToCheck); }

    Boolean AxisAlignedBox::IsOverlapping(const Plane& ToCheck) const
        { return MathTools::Overlap(*this,ToCheck); }

    AxisAlignedBox::RayTestResult AxisAlignedBox::Intersects(const Ray& ToCheck) const
        { return MathTools::Intersects(*this,ToCheck); }

    ///////////////////////////////////////////////////////////////////////////////
    // Extents query

    void AxisAlignedBox::SetExtents(const Vector3& Min, const Vector3& Max)
        { this->MinExt = Min;  this->MaxExt = Max; }

    Vector3 AxisAlignedBox::GetSize() const
        { return ( this->MaxExt - this->MinExt ); }

    Vector3 AxisAlignedBox::GetHalfSize() const
        { return ( this->GetSize() * 0.5 ); }

    Vector3 AxisAlignedBox::GetCenter() const
        { return ( this->MaxExt + this->MinExt ) * 0.5; }

    Vector3 AxisAlignedBox::GetCorner(const AxisAlignedBox::AxisExtent XEx, const AxisAlignedBox::AxisExtent YEx, const AxisAlignedBox::AxisExtent ZEx) const
    {
        return Vector3( ( XEx == AE_Min ? this->MinExt.X : this->MaxExt.X ),
                        ( YEx == AE_Min ? this->MinExt.Y : this->MaxExt.Y ),
                        ( ZEx == AE_Min ? this->MinExt.Z : this->MaxExt.Z ) );
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Conversion Methods

    void AxisAlignedBox::ExtractOgreAABB(const Ogre::AxisAlignedBox& InternalAABB)
        { this->MinExt.ExtractOgreVector3( InternalAABB.getMinimum() );  this->MaxExt.ExtractOgreVector3( InternalAABB.getMaximum() ); }

    Ogre::AxisAlignedBox AxisAlignedBox::GetOgreAABB() const
        { return Ogre::AxisAlignedBox(this->MinExt.GetOgreVector3(),this->MaxExt.GetOgreVector3()); }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void AxisAlignedBox::ProtoSerialize(XML::Node& ParentNode) const
    {
        XML::Node SelfRoot = ParentNode.AppendChild( AxisAlignedBox::GetSerializableName() );

        if( SelfRoot.AppendAttribute("Version").SetValue("1") )
        {
            XML::Node MinimumNode = SelfRoot.AppendChild("MinExt");
            this->MinExt.ProtoSerialize( MinimumNode );

            XML::Node MaximumNode = SelfRoot.AppendChild("MaxExt");
            this->MaxExt.ProtoSerialize( MaximumNode );

            return;
        }else{
            SerializeError("Create XML Attribute Values",AxisAlignedBox::GetSerializableName(),true);
        }
    }

    void AxisAlignedBox::ProtoDeSerialize(const XML::Node& SelfRoot)
    {
        if( String(SelfRoot.Name()) == AxisAlignedBox::GetSerializableName() ) {
            if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                // Get the properties that need their own nodes
                XML::Node MinimumNode = SelfRoot.GetChild("MinExt").GetFirstChild();
                if( !MinimumNode.Empty() )
                    this->MinExt.ProtoDeSerialize(MinimumNode);

                XML::Node MaximumNode = SelfRoot.GetChild("MaxExt").GetFirstChild();
                if( !MaximumNode.Empty() )
                    this->MaxExt.ProtoDeSerialize(MaximumNode);
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + AxisAlignedBox::GetSerializableName() + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,AxisAlignedBox::GetSerializableName() + " was not found in the provided XML node, which was expected.");
        }
    }

    String AxisAlignedBox::GetSerializableName()
    {
        return "AxisAlignedBox";
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Operators

    void AxisAlignedBox::operator=(const AxisAlignedBox& Other)
        { this->MinExt = Other.MinExt;  this->MaxExt = Other.MaxExt; }

    void AxisAlignedBox::operator=(const Ogre::AxisAlignedBox& InternalAABB)
        { this->ExtractOgreAABB(InternalAABB); }

    Boolean AxisAlignedBox::operator>(const AxisAlignedBox& Other) const
        { return ( this->GetVolume() > Other.GetVolume() ); }

    Boolean AxisAlignedBox::operator<(const AxisAlignedBox& Other) const
        { return ( this->GetVolume() < Other.GetVolume() ); }

    Boolean AxisAlignedBox::operator>=(const AxisAlignedBox& Other) const
        { return ( this->GetVolume() >= Other.GetVolume() ); }

    Boolean AxisAlignedBox::operator<=(const AxisAlignedBox& Other) const
        { return ( this->GetVolume() <= Other.GetVolume() ); }

    Boolean AxisAlignedBox::operator==(const AxisAlignedBox& Other) const
        { return ( this->MinExt == Other.MinExt && this->MaxExt == Other.MaxExt ); }

    Boolean AxisAlignedBox::operator!=(const AxisAlignedBox& Other) const
        { return ( this->MinExt != Other.MinExt || this->MaxExt != Other.MaxExt ); }
}//Mezzanine

#endif
