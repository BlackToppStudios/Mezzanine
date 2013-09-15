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
#include "XML/xml.h"

#include "Ogre.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // Construction and Destruction

    Plane::Plane() :
        Normal(0,0,0),
        Distance(0)
        {  }

    Plane::Plane(const Vector3& Norm, const Real Dist) :
        Normal(Norm),
        Distance(Dist)
        {  }

    Plane::Plane(const Vector3& First, const Vector3& Second, const Vector3& Third)
    {
        Vector3 Edge1 = Second - First;
		Vector3 Edge2 = Third - First;
		this->Normal = Edge1.CrossProduct(Edge2);
		this->Normal.Normalize();
		this->Distance = -(this->Normal.DotProduct(First));
    }

    Plane::Plane(const Ogre::Plane& InternalPlane)
        { this->ExtractOgrePlane(InternalPlane); }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Bool Plane::IsOverlapping(const Sphere& ToCheck) const
        { return MathTools::Overlap(*this,ToCheck); }

    Bool Plane::IsOverlapping(const AxisAlignedBox& ToCheck) const
        { return MathTools::Overlap(ToCheck,*this); }

    Bool Plane::IsOverlapping(const Plane& ToCheck) const
        { return MathTools::Overlap(*this,ToCheck); }

    Bool Plane::Intersects(const Ray& ToCheck) const
        { return MathTools::Intersects(*this,ToCheck); }

    ///////////////////////////////////////////////////////////////////////////////
    // Conversion Methods

    Ogre::Plane Plane::GetOgrePlane() const
        { return Ogre::Plane( Normal.GetOgreVector3(), Distance); }

    void Plane::ExtractOgrePlane(const Ogre::Plane& InternalPlane)
        { this->Normal = InternalPlane.normal;  this->Distance = InternalPlane.d; }

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

Mezzanine::XML::Node& MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Plane& x)
{
    if ( Mezzanine::String(OneNode.Name())==Mezzanine::String("Plane") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            x.Distance=OneNode.GetAttribute("Distance").AsReal();
            if(OneNode.GetFirstChild())
            {
                OneNode.GetFirstChild() >> x.Normal;
            }else{
                MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Normal not found while parsing Mezzanine::Plane.");
            }
        }else{
            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for Plane: Not Version 1.");
        }
    }else{
        MEZZ_EXCEPTION(Mezzanine::Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a Plane, found a " + Mezzanine::String(OneNode.Name()));
    }
}

#endif
