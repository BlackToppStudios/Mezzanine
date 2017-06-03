// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _ray_cpp
#define _ray_cpp

#include "ray.h"
#include "MathTools/mathtools.h"
#include "axisalignedbox.h"
#include "plane.h"
#include "sphere.h"
#include "linesegment.h"
#include "exception.h"
#include "serialization.h"

#include <OgreRay.h>

#include <ostream>

namespace Mezzanine
{
    Ray::Ray() :
        Origin(0,0,0),
        Normal(0,1,0)
        {  }

    Ray::Ray(const Ray& Other) :
        Origin(Other.Origin),
        Normal(Other.Normal)
        {  }

    Ray::Ray(const Vector3& Dir) :
        Normal(Dir)
        { this->Normal.Normalize(); }

    Ray::Ray(const Vector3& From, const Vector3& Dir) :
        Origin(From),
        Normal(Dir)
        { this->Normal.Normalize(); }

    Ray::Ray(const Ogre::Ray& InternalRay)
        { this->ExtractOgreRay(InternalRay); }

    Ray::~Ray()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    const Vector3& Ray::GetNormal() const
        { return this->Normal; }

    void Ray::SetNormal(const Vector3& FreshNormal)
        { this->Normal = FreshNormal.GetNormal(); }

    const Vector3& Ray::GetOrigin() const
        { return this->Origin; }

    void Ray::SetOrigin(const Vector3& FreshOrigin)
        { this->Origin = FreshOrigin; }

    Vector3 Ray::GetPointAtDistance(const Real& Distance) const
        { return ( this->Origin + ( this->GetNormal() * Distance ) ); }

    Ray::PlaneRayTestResult Ray::Intersects(const Plane& ToCheck) const
        { return MathTools::Intersects(ToCheck,*this); }

    Ray::GeometryRayTestResult Ray::Intersects(const Sphere& ToCheck) const
        { return MathTools::Intersects(ToCheck,*this); }

    Ray::GeometryRayTestResult Ray::Intersects(const AxisAlignedBox& ToCheck) const
        { return MathTools::Intersects(ToCheck,*this); }

    ///////////////////////////////////////////////////////////////////////////////
    // Conversion Methods

    void Ray::ExtractOgreRay(const Ogre::Ray& InternalRay)
        { this->Origin = InternalRay.getOrigin();  this->Normal = InternalRay.getDirection(); }

    Ogre::Ray Ray::GetOgreRay() const
        { return Ogre::Ray(this->Origin.GetOgreVector3(),this->GetNormal().GetOgreVector3()); }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void Ray::ProtoSerialize(XML::Node& ParentNode) const
    {
        XML::Node SelfRoot = ParentNode.AppendChild( Ray::GetSerializableName() );

        if( SelfRoot.AppendAttribute("Version").SetValue("1") )
        {
            XML::Node OriginNode = SelfRoot.AppendChild("Origin");
            this->Origin.ProtoSerialize( OriginNode );

            XML::Node NormalNode = SelfRoot.AppendChild("Normal");
            this->Normal.ProtoSerialize( NormalNode );

            return;
        }else{
            SerializeError("Create XML Attribute Values",Ray::GetSerializableName(),true);
        }
    }

    void Ray::ProtoDeSerialize(const XML::Node& SelfRoot)
    {
        if( String(SelfRoot.Name()) == Ray::GetSerializableName() ) {
            if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                // Get the properties that need their own nodes
                XML::Node OriginNode = SelfRoot.GetChild("Origin").GetFirstChild();
                if( !OriginNode.Empty() )
                    this->Origin.ProtoDeSerialize(OriginNode);

                XML::Node NormalNode = SelfRoot.GetChild("Normal").GetFirstChild();
                if( !NormalNode.Empty() )
                    this->Normal.ProtoDeSerialize(NormalNode);
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + Ray::GetSerializableName() + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,Ray::GetSerializableName() + " was not found in the provided XML node, which was expected.");
        }
    }

    String Ray::GetSerializableName()
    {
        return "Ray";
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Operators

    void Ray::operator=(const Ray& Other)
        { this->Origin = Other.Origin;  this->Normal = Other.Normal; }

    Boole Ray::operator==(const Ray& Other) const
        { return ( this->Origin == Other.Origin && this->Normal == Other.Normal ); }

    Boole Ray::operator!=(const Ray& Other) const
        { return ( this->Origin != Other.Origin || this->Normal != Other.Normal ); }
}//Mezzanine

std::ostream& operator << (std::ostream& Stream, const Mezzanine::Ray& Data)
{
    Stream << "[" << Data.GetOrigin() << "," << Data.GetNormal() << "]";
    return Stream;
}

#endif
