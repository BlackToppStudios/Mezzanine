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
#ifndef _ray_cpp
#define _ray_cpp

#include "ray.h"
#include "mathtool.h"
#include "axisalignedbox.h"
#include "plane.h"
#include "sphere.h"
#include "exception.h"
#include "serialization.h"

#include <Ogre.h>

#include <ostream>

namespace Mezzanine
{
    Ray::Ray() :
        Origin(0,0,0),
        Destination(0,1,0)
        {  }

    Ray::Ray(const Ray& Other) :
        Origin(Other.Origin),
        Destination(Other.Destination)
        {  }

    Ray::Ray(const Vector3& To) :
        Destination(To)
        {  }

    Ray::Ray(const Vector3& From, const Vector3& To) :
        Origin(From),
        Destination(To)
        {  }

    Ray::Ray(const Ogre::Ray& InternalRay)
        { this->ExtractOgreRay(InternalRay); }

    Ray::~Ray()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Real Ray::Length() const
        { return this->Origin.Distance(this->Destination); }

    Vector3 Ray::GetDirection() const
        { return ( this->Destination - this->Origin ).Normalize(); }

    Ray Ray::GetNormal() const
    {
        Real TempLength = this->Length();
        if( 0 != TempLength ) {
            return (*this) / TempLength;
        }
        return Ray(*this);
    }

    Ray& Ray::Normalize()
    {
        Real TempLength = this->Length();
        if( 0 != TempLength ) {
            return (*this) /= this->Length();
        }
        return *this;
    }

    Ray::PlaneRayTestResult Ray::Intersects(const Plane& ToCheck) const
        { return MathTools::Intersects(ToCheck,*this); }

    Ray::GeometryRayTestResult Ray::Intersects(const Sphere& ToCheck) const
        { return MathTools::Intersects(ToCheck,*this); }

    Ray::GeometryRayTestResult Ray::Intersects(const AxisAlignedBox& ToCheck) const
        { return MathTools::Intersects(ToCheck,*this); }

    ///////////////////////////////////////////////////////////////////////////////
    // Conversion Methods

    void Ray::ExtractOgreRay(const Ogre::Ray& InternalRay)
        { this->Origin = InternalRay.getOrigin();  this->Destination = InternalRay.getPoint(1); }

    Ogre::Ray Ray::GetOgreRay() const
        { return Ogre::Ray(this->Origin.GetOgreVector3(),this->GetDirection().GetOgreVector3()); }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void Ray::ProtoSerialize(XML::Node& ParentNode) const
    {
        XML::Node SelfRoot = ParentNode.AppendChild( Ray::GetSerializableName() );

        if( SelfRoot.AppendAttribute("Version").SetValue("1") )
        {
            XML::Node OriginNode = SelfRoot.AppendChild("Origin");
            this->Origin.ProtoSerialize( OriginNode );

            XML::Node DestinationNode = SelfRoot.AppendChild("Destination");
            this->Destination.ProtoSerialize( DestinationNode );

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

                XML::Node DestinationNode = SelfRoot.GetChild("Destination").GetFirstChild();
                if( !DestinationNode.Empty() )
                    this->Destination.ProtoDeSerialize(DestinationNode);
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + Ray::GetSerializableName() + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,Ray::GetSerializableName() + " was not found in the provided XML node, which was expected.");
        }
    }

    String Ray::GetSerializableName()
    {
        return "Ray";
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Operators

    void Ray::operator=(const Ray& Other)
        { this->Origin = Other.Origin;  this->Destination = Other.Destination; }

    Ray Ray::operator*(const Real Factor) const
        { return Ray( this->Origin, ( ( this->Destination - this->Origin ) * Factor ) + this->Origin ); }

    Ray Ray::operator/(const Real Factor) const
        { return Ray( this->Origin, ( ( this->Destination - this->Origin ) / Factor ) + this->Origin ); }

    Ray& Ray::operator*=(const Real Factor)
        { this->Destination = ( ( this->Destination - this->Origin ) * Factor ) + this->Origin;  return *this; }

    Ray& Ray::operator/=(const Real Factor)
        { this->Destination = ( ( this->Destination - this->Origin ) / Factor ) + this->Origin;  return *this; }

    Bool Ray::operator==(const Ray& Other) const
        { return ( this->Origin == Other.Origin && this->Destination == Other.Destination ); }

    Bool Ray::operator!=(const Ray& Other) const
        { return ( this->Origin != Other.Origin || this->Destination != Other.Destination ); }
}

std::ostream& operator << (std::ostream& stream, const Mezzanine::Ray& x)
{
    stream << "[" << x.Origin << "," << x.Destination << "]";
    return stream;
}

#endif
