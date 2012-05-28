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
#ifndef _vector2_cpp
#define _vector2_cpp

#include "serialization.h"
#include "stringtool.h"
#include "vector2.h"
#include "mathtool.h"

//#include <memory>

#include <Ogre.h>

namespace Mezzanine
{
    Vector2::Vector2()
    {
        X = 0;
        Y = 0;
    }

    Vector2::Vector2(Real x, Real y)
    {
        X = x;
        Y = y;
    }

    Vector2::Vector2(Ogre::Vector2 Vec)
    {
        ExtractOgreVector2(Vec);
    }

    Ogre::Vector2 Vector2::GetOgreVector2() const
    {
        Ogre::Vector2 Theirs;
        Theirs.x=this->X;
        Theirs.y=this->Y;
        return Theirs;
    }

    void Vector2::ExtractOgreVector2(Ogre::Vector2 Ours)
    {
        this->X=Ours.x;
        this->Y=Ours.y;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Equality Comparison operators

    bool Vector2::operator== (const Mezzanine::Vector2 &Vec2)
    {
        if ( Vec2.X == this->X && Vec2.Y == this->Y )
            { return true; }
        return false;
    }

    bool Vector2::operator!= (const Mezzanine::Vector2 &Vec2)
    {
        if ( Vec2.X != this->X || Vec2.Y != this->Y )
            { return true; }
        return false;
    }

    bool Vector2::operator== (const Ogre::Vector2 &Vec2)
    {
        if ( Vec2.x == this->X && Vec2.y == this->Y )
            { return true; }
        return false;
    }

    bool Vector2::operator!= (const Ogre::Vector2 &Vec2)
    {
        if ( Vec2.x != this->X || Vec2.y != this->Y )
            { return true; }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Vector2 Arithmetic with Real
    Vector2 Vector2::operator* (const Real &scalar) const
    {
        return Vector2(
            this->X * scalar,
            this->Y * scalar
        );
    }

    Vector2 Vector2::operator/ (const Real &scalar) const
    {
        return Vector2(
            this->X / scalar,
            this->Y / scalar
        );
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Vector2 Arithmetic and assignment with Real
    void Vector2::operator*= (const Real &scalar)
    {
        this->X *= scalar;
        this->Y *= scalar;
    }

    void Vector2::operator/= (const Real &scalar)
    {
        this->X /= scalar;
        this->Y /= scalar;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Arithmetic Operators

    Vector2 Vector2::operator+ (const Vector2 &Vec2) const
    {
        Vector2 Temp(X,Y);
        Temp.X+=Vec2.X;
        Temp.Y+=Vec2.Y;
        return Temp;
    }

    Vector2 Vector2::operator- (const Vector2 &Vec2) const
    {
        Vector2 Temp(X,Y);
        Temp.X-=Vec2.X;
        Temp.Y-=Vec2.Y;
        return Temp;
    }

    Vector2 Vector2::operator* (const Vector2 &Vec2) const
    {
        Vector2 Temp(X,Y);
        Temp.X*=Vec2.X;
        Temp.Y*=Vec2.Y;
        return Temp;
    }

    Vector2 Vector2::operator/ (const Vector2 &Vec2) const
    {
        Vector2 Temp(X,Y);
        Temp.X/=Vec2.X;
        Temp.Y/=Vec2.Y;
        return Temp;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Fancy Math

    Vector2 Vector2::Perpendicular() const
    {
        return Vector2(-Y,X);
    }

    Vector2& Vector2::Normalize()
    {
        Real Length = MathTool::Sqrt( X * X + Y * Y);

        if ( Length > 1e-08 )
        {
            Real InvLength = 1.0 / Length;
            X *= InvLength;
            Y *= InvLength;
        }

        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization
#ifdef MEZZXML
    // Serializable
    void Vector2::ProtoSerialize(XML::Node& CurrentRoot) const
    {
        Mezzanine::XML::Node VecNode = CurrentRoot.AppendChild(SerializableName());
        VecNode.SetName(SerializableName());

        Mezzanine::XML::Attribute VersionAttr = VecNode.AppendAttribute("Version");
        Mezzanine::XML::Attribute XAttr = VecNode.AppendAttribute("X");
        Mezzanine::XML::Attribute YAttr = VecNode.AppendAttribute("Y");
        if( VersionAttr && XAttr && YAttr )
        {
            if( VersionAttr.SetValue("1") && XAttr.SetValue(this->X) && YAttr.SetValue(this->Y) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values", SerializableName(),true);
            }
        }else{
            SerializeError("Create XML Attributes", SerializableName(),true);
        }
    }

    // DeSerializable
    void Vector2::ProtoDeSerialize(const XML::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==Mezzanine::String(SerializableName()) )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                this->X=OneNode.GetAttribute("X").AsReal();
                this->Y=OneNode.GetAttribute("Y").AsReal();
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + SerializableName() + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a " + SerializableName() + ", found a " + String(OneNode.Name()) + ".");
        }
    }

    String Vector2::SerializableName()
        { return String("Vector2"); }

#endif

}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
std::ostream& operator << (std::ostream& stream, const Mezzanine::Vector2& x)
{
    #ifdef MEZZXML
        //stream << "<Vector2 Version=\"1\" X=\"" << x.X << "\" Y=\"" << x.Y << "\" />";
        Serialize(stream,x);
    #else
        stream << "[" << x.X << "," << x.Y << "]";
    #endif // \MEZZXML
    return stream;
}

#ifdef MEZZXML
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Vector2& Vec)
    { return DeSerialize(stream, Vec); }

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Vector2& Vec)
    { Vec.ProtoDeSerialize(OneNode); }
#endif // \MEZZXML

#endif
