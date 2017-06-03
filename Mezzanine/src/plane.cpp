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
#include "MathTools/mathtools.h"
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

    Plane::Plane(const Vector3& Norm, const Real Constant) :
        Normal(Norm),
        Distance(-Constant)
        {  }

    Plane::Plane(const Vector3& Norm, const Vector3& Point)
        { this->Define(Norm,Point); }

    Plane::Plane(const Vector3& First, const Vector3& Second, const Vector3& Third)
        { this->Define(First,Second,Third); }

    Plane::Plane(const Ogre::Plane& InternalPlane)
        { this->ExtractOgrePlane(InternalPlane); }

    Plane::~Plane()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void Plane::Define(const Vector3& Norm, const Real Constant)
    {
        this->Normal = Norm;
        this->Distance = -Constant;
    }

    void Plane::Define(const Vector3& Norm, const Vector3& Point)
    {
        this->Normal = Norm;
        this->Distance = -(Norm.DotProduct(Point));
    }

    void Plane::Define(const Vector3& First, const Vector3& Second, const Vector3& Third)
    {
        Vector3 Edge1 = Second - First;
        Vector3 Edge2 = Third - First;
        this->Normal = Edge1.CrossProduct(Edge2);
        this->Normal.Normalize();
        this->Distance = -(this->Normal.DotProduct(First));
    }

    MathTools::PlaneSide Plane::GetSide(const Vector3& Point) const
    {
        Real Dist = this->GetDistance(Point);
        if( Dist < 0.0 ) {
            return MathTools::PS_Negative;
        }else if( Dist > 0.0 ) {
            return MathTools::PS_Positive;
        }else{
            return MathTools::PS_Neither;
        }
    }

    MathTools::PlaneSide Plane::GetSide(const Vector3& Center, const Vector3& HalfSize) const
    {
        Real CenterDist = this->GetDistance(Center);
        Real MaxDist = MathTools::Abs( this->Normal.DotProduct(HalfSize) );
        if( CenterDist < -MaxDist ) {
            return MathTools::PS_Negative;
        }else if( CenterDist > +MaxDist ) {
            return MathTools::PS_Positive;
        }else{
            return MathTools::PS_Both;
        }
    }

    Real Plane::GetDistance(const Vector3& Point) const
        { return ( this->Normal.DotProduct(Point) + this->Distance ); }

    Ray Plane::GetOverlap(const Plane& Other) const
    {
        //TODO : handle the case where the plane is perpendicular to T
        Vector3 point1;
        Vector3 direction = this->Normal.CrossProduct( Other.Normal );
        if( direction.SquaredLength() < 1e-08 )
            return Ray();

        Real cp = this->Normal.X * Other.Normal.Y - Other.Normal.X * this->Normal.Y;
        if( cp != 0 ) {
            Real denom = 1.0 / cp;
            point1.X = ( this->Normal.Y * Other.Distance - Other.Normal.Y * this->Distance ) * denom;
            point1.Y = ( Other.Normal.X * this->Distance - this->Normal.X * Other.Distance ) * denom;
            point1.Z = 0;
        }else if( ( cp = this->Normal.Y * Other.Normal.Z - Other.Normal.Y * this->Normal.Z ) != 0 ) {
            //special case #1
            Real denom = 1.0 / cp;
            point1.X = 0.0;
            point1.Y = ( this->Normal.Z * Other.Distance - Other.Normal.Z * this->Distance ) * denom;
            point1.Z = ( Other.Normal.Y * this->Distance - this->Normal.Y * Other.Distance ) * denom;
        }else if( ( cp = this->Normal.X * Other.Normal.Z - Other.Normal.X * this->Normal.Z ) != 0 ) {
            //special case #2
            Real denom = 1.0 / cp;
            point1.X = ( this->Normal.Z * Other.Distance - Other.Normal.Z * this->Distance ) * denom;
            point1.Y = 0.0;
            point1.Z = ( Other.Normal.X * this->Distance - this->Normal.X * Other.Distance ) * denom;
        }

        return Ray(point1,direction);
    }

    Boole Plane::IsOverlapping(const Sphere& ToCheck) const
        { return MathTools::Overlap(*this,ToCheck); }

    Boole Plane::IsOverlapping(const AxisAlignedBox& ToCheck) const
        { return MathTools::Overlap(ToCheck,*this); }

    Boole Plane::IsOverlapping(const Plane& ToCheck) const
        { return MathTools::Overlap(*this,ToCheck); }

    Plane::RayTestResult Plane::Intersects(const Ray& ToCheck) const
        { return MathTools::Intersects(*this,ToCheck); }

    ///////////////////////////////////////////////////////////////////////////////
    // Conversion Methods

    Ogre::Plane Plane::GetOgrePlane() const
    {
        Ogre::Plane Ret;
        Ret.normal = this->Normal.GetOgreVector3();
        Ret.d = this->Distance;
        return Ret;
    }

    void Plane::ExtractOgrePlane(const Ogre::Plane& InternalPlane)
    {
        this->Normal = InternalPlane.normal;
        this->Distance = InternalPlane.d;
    }

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
                MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + Plane::GetSerializableName() + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,Plane::GetSerializableName() + " was not found in the provided XML node, which was expected.");
        }
    }

    String Plane::GetSerializableName()
    {
        return "Plane";
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Operators

    void Plane::operator=(const Ogre::Plane& InternalPlane)
        { this->ExtractOgrePlane(InternalPlane); }

    Boole Plane::operator==(const Plane& Other) const
        { return ( this->Normal == Other.Normal && this->Distance == Other.Distance ); }

    Boole Plane::operator!=(const Plane& Other) const
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
