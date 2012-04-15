//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#include "stringtool.h"
#include "world.h"
#include "xml.h"

#include "Ogre.h"

namespace Mezzanine
{

    ///////////////////////////////////////////////////////////////////////////////
    // Constructors
    Plane::Plane() : Normal(0,0,0), Distance(0)
        {}

    Plane::Plane(Vector3 Normal_, Real Distance_) : Normal(Normal_),Distance(Distance_)
        {}

    Plane::Plane(Ogre::Plane Plane_)
        { this->ExtractOgrePlane(Plane_); }

    Plane::Plane(const Vector3& rkPoint0, const Vector3& rkPoint1, const Vector3& rkPoint2)
    {
        Vector3 kEdge1 = rkPoint1 - rkPoint0;
		Vector3 kEdge2 = rkPoint2 - rkPoint0;
		this->Normal = kEdge1.CrossProduct(kEdge2);
		this->Normal.Normalize();
		this->Distance = -this->Normal.DotProduct(rkPoint0);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Conversions and adjustments
    Ogre::Plane Plane::GetOgrePlane() const
        { return Ogre::Plane( Normal.GetOgreVector3(), Distance); }

    void Plane::ExtractOgrePlane(const Ogre::Plane& Plane2)
    {
        this->Normal=Plane2.normal;
        this->Distance=Plane2.d;
    }

    void Plane::operator=(const Ogre::Plane& Plane2)
        { this->ExtractOgrePlane(Plane2); }

}
#ifdef MEZZXML
std::ostream& operator << (std::ostream& stream, const Mezzanine::Plane& x)
{
    stream << "<Plane Version=\"1\" Distance=\"" << x.Distance << "\" >" << x.Normal << "</Plane>";
    return stream;
}


std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Plane& x)
{
    Mezzanine::String OneTag( Mezzanine::xml::GetOneTag(stream) );
    std::auto_ptr<Mezzanine::xml::Document> Doc( Mezzanine::xml::PreParseClassFromSingleTag("Mezzanine::", "Plane", OneTag) );

    Doc->GetFirstChild() >> x;

    return stream;
}

Mezzanine::xml::Node& MEZZ_LIB operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::Plane& x)
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
                throw(Mezzanine::Exception("Normal not found while parsing Mezzanine::Plane"));
            }
        }else{
            throw( Mezzanine::Exception("Incompatible XML Version for Plane: Not Version 1"));
        }
    }else{
        throw( Mezzanine::Exception(Mezzanine::StringTool::StringCat("Attempting to deserialize a Plane, found a ", OneNode.Name())));
    }
}

#endif // \Mezz_xml

#endif
