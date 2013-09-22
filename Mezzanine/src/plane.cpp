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
#ifndef _plane_cpp
#define _plane_cpp

#include <iostream>
#include <memory>

#include "exception.h"
#include "plane.h"
#include "ray.h"
#include "axisalignedbox.h"
#include "sphere.h"
#include "stringtool.h"
#include "mathtool.h"
#ifndef SWIG
    #include "XML/xml.h"
#endif
#include "serialization.h"

#include "Ogre.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // Construction and Destruction

    Plane::Plane() :
        Normal(0,0,0),
        Distance(0)
        {  }

    Plane::Plane(const Plane& Other) :
        Normal(Other.Normal),
        Distance(Other.Distance)
        {  }

    Plane::Plane(const Vector3& Norm, const Real Dist) :
        Normal(Norm),
        Distance(Dist)
        {  }

    Plane::Plane(const Vector3& First, const Vector3& Second, const Vector3& Third)
        { this->Define(First,Second,Third); }

    Plane::Plane(const Ogre::Plane& InternalPlane)
        { this->ExtractOgrePlane(InternalPlane); }

    Plane::~Plane()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void Plane::Define(const Vector3& Norm, const Real Dist)
    {
        this->Normal = Norm;
        this->Distance = Dist;
    }

    void Plane::Define(const Vector3& First, const Vector3& Second, const Vector3& Third)
    {
        Vector3 Edge1 = Second - First;
        Vector3 Edge2 = Third - First;
        this->Normal = Edge1.CrossProduct(Edge2);
        this->Normal.Normalize();
        this->Distance = -(this->Normal.DotProduct(First));
    }

    Plane::Side Plane::GetSide(const Vector3& Point) const
    {
        Real Dist = this->GetDistance(Point);
        if( Dist < 0.0 ) {
            return Plane::S_Negative;
        }else if( Dist > 0.0 ) {
            return Plane::S_Positive;
        }else{
            return Plane::S_None;
        }
    }

    Plane::Side Plane::GetSide(const Vector3& Center, const Vector3& HalfSize) const
    {
        Real CenterDist = this->GetDistance(Center);
        Real MaxDist = MathTools::Fabs( this->Normal.DotProduct(HalfSize) );
        if( CenterDist < -MaxDist ) {
            return Plane::S_Negative;
        }else if( CenterDist > +MaxDist ) {
            return Plane::S_Positive;
        }else{
            return Plane::S_Both;
        }
    }

    Real Plane::GetDistance(const Vector3& Point) const
        { return ( this->Normal.DotProduct(Point) + this->Distance ); }

    Bool Plane::IsOverlapping(const Sphere& ToCheck) const
        { return MathTools::Overlap(*this,ToCheck); }

    Bool Plane::IsOverlapping(const AxisAlignedBox& ToCheck) const
        { return MathTools::Overlap(ToCheck,*this); }

    Bool Plane::IsOverlapping(const Plane& ToCheck) const
        { return MathTools::Overlap(*this,ToCheck); }

    Plane::RayTestResult Plane::Intersects(const Ray& ToCheck) const
        { return MathTools::Intersects(*this,ToCheck); }

    ///////////////////////////////////////////////////////////////////////////////
    // Conversion Methods

    Ogre::Plane Plane::GetOgrePlane() const
        { return Ogre::Plane( Normal.GetOgreVector3(), Distance); }

    void Plane::ExtractOgrePlane(const Ogre::Plane& InternalPlane)
        { this->Normal = InternalPlane.normal;  this->Distance = InternalPlane.d; }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void Plane::ProtoSerialize(XML::Node& ParentNode) const
    {
        XML::Node SelfRoot = ParentNode.AppendChild( Plane::GetSerializableName() );

        if( SelfRoot.AppendAttribute("Version").SetValue("1") &&
            SelfRoot.AppendAttribute("Distance").SetValue( this->Distance ) )
        {
            XML::Node CenterNode = SelfRoot.AppendChild("Normal");
            this->Normal.ProtoSerialize( CenterNode );

            return;
        }else{
            SerializeError("Create XML Attribute Values",Plane::GetSerializableName(),true);
        }
    }

    void Plane::ProtoDeSerialize(const XML::Node& SelfRoot)
    {
        XML::Attribute CurrAttrib;

        if( String(SelfRoot.Name()) == Plane::GetSerializableName() ) {
            if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                CurrAttrib = SelfRoot.GetAttribute("Distance");
                if( !CurrAttrib.Empty() )
                    this->Distance = CurrAttrib.AsReal();

                // Get the properties that need their own nodes
                XML::Node NormalNode = SelfRoot.GetChild("Normal").GetFirstChild();
                if( !NormalNode.Empty() )
                    this->Normal.ProtoDeSerialize(NormalNode);
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + Plane::GetSerializableName() + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,Plane::GetSerializableName() + " was not found in the provided XML node, which was expected.");
        }
    }

    String Plane::GetSerializableName()
    {
        return "Plane";
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Operators

    void Plane::operator=(const Plane& Other)
        { this->Normal = Other.Normal;  this->Distance = Other.Distance; }

    void Plane::operator=(const Ogre::Plane& InternalPlane)
        { this->ExtractOgrePlane(InternalPlane); }

    Bool Plane::operator==(const Plane& Other) const
        { return ( this->Normal == Other.Normal && this->Distance == Other.Distance ); }

    Bool Plane::operator!=(const Plane& Other) const
        { return ( this->Normal != Other.Normal || this->Distance != Other.Distance ); }
}

std::ostream& operator << (std::ostream& stream, const Mezzanine::Plane& x)
{
    stream << "<Plane Version=\"1\" Distance=\"" << x.Distance << "\" >" << x.Normal << "</Plane>";
    return stream;
}


std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Plane& x)
{
    Mezzanine::String OneTag( Mezzanine::XML::GetOneTag(stream) );
    Mezzanine::CountedPtr<Mezzanine::XML::Document> Doc( Mezzanine::XML::PreParseClassFromSingleTag("Mezzanine::", "Plane", OneTag) );

    Doc->GetFirstChild() >> x;

    return stream;
}

void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Plane& x)
{
    x.ProtoDeSerialize(OneNode);
}

#endif
