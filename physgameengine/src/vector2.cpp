//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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

#include "vector2.h"

#include <memory>

#include <Ogre.h>

namespace phys
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

    bool Vector2::operator== (const phys::Vector2 &Vec2)
    {
        if ( Vec2.X == this->X && Vec2.Y == this->Y )
            { return true; }
        return false;
    }

    bool Vector2::operator!= (const phys::Vector2 &Vec2)
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
    // Serialization
#ifdef PHYSXML
    // Serializable
    void Vector2::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        phys::xml::Node VecNode = CurrentRoot.AppendChild(SerializableName());
        VecNode.SetName(SerializableName());

        phys::xml::Attribute VersionAttr = VecNode.AppendAttribute("Version");
        phys::xml::Attribute XAttr = VecNode.AppendAttribute("X");
        phys::xml::Attribute YAttr = VecNode.AppendAttribute("Y");
        if( VersionAttr && XAttr && YAttr )
        {
            if( VersionAttr.SetValue("1") && XAttr.SetValue(this->X) && YAttr.SetValue(this->Y) )
            {
                return;
            }else{
                throw(Exception(StringCat("Could not Stream",SerializableName()," XML Attribute Values.")));
            }
        }else{
            throw(Exception(StringCat("Could not Stream ",SerializableName()," XML Attribute Names.")));
        }
    }

    // DeSerializable
    void Vector2::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( phys::String(OneNode.Name())==phys::String(SerializableName()) )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                this->X=OneNode.GetAttribute("X").AsReal();
                this->Y=OneNode.GetAttribute("Y").AsReal();
            }else{
                throw( phys::Exception(StringCat("Incompatible XML Version for ",SerializableName(),": Not Version 1")) );
            }
        }else{
            throw( phys::Exception(phys::StringCat("Attempting to deserialize a ",SerializableName(),", found a ", OneNode.Name())));
        }
    }

    String Vector2::SerializableName() const
        { return String("Vector3"); }

#endif

}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
std::ostream& operator << (std::ostream& stream, const phys::Vector2& x)
{
    #ifdef PHYSXML
        //stream << "<Vector2 Version=\"1\" X=\"" << x.X << "\" Y=\"" << x.Y << "\" />";
        Serialize(stream,x);
    #else
        stream << "[" << x.X << "," << x.Y << "]";
    #endif // \PHYSXML
    return stream;
}

#ifdef PHYSXML
std::istream& PHYS_LIB operator >> (std::istream& stream, phys::Vector2& Vec)
    { return DeSerialize(stream, Vec); }

phys::xml::Node& operator >> (const phys::xml::Node& OneNode, phys::Vector2& Vec)
    { return Vec.ProtoDeSerialize(OneNode); }
#endif // \PHYSXML

#endif
