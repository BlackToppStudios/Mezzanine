//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _quaternion_cpp
#define _quaternion_cpp

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"

#include "quaternion.h"

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    // Constructors
    Quaternion::Quaternion()
    {
        this->X=0;
        this->Y=0;
        this->Z=0;
        this->W=1;
    }

    Quaternion::Quaternion(const Real &x, const Real &y, const Real &z, const Real &w)
    {
        this->X=x;
        this->Y=y;
        this->Z=z;
        this->W=w;
    }

    Quaternion::Quaternion(const btQuaternion& Other)
        { ExtractBulletQuaternion(Other); }

    Quaternion::Quaternion(const Ogre::Quaternion& Other)
        { ExtractOgreQuaternion(Other); }

    Quaternion::Quaternion(const phys::Quaternion& Other)
    {
        this->X=Other.X;
        this->Y=Other.Y;
        this->Z=Other.Z;
        this->W=Other.W;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Explicit Conversion

    btQuaternion Quaternion::GetBulletQuaternion(bool normalize) const
    {
        btQuaternion Other;
        Other.setX(this->X);
        Other.setY(this->Y);
        Other.setZ(this->Z);
        Other.setW(this->W);
        if(normalize)
            { Other.normalize(); }
        return Other;
    }

    void Quaternion::ExtractBulletQuaternion(const btQuaternion &Other)
    {
        this->X=Other.x();
        this->Y=Other.y();
        this->Z=Other.z();
        this->W=Other.w();
    }

    Ogre::Quaternion Quaternion::GetOgreQuaternion(bool normalize) const
    {
        Ogre::Quaternion Other;
        Other.x=this->X;
        Other.y=this->Y;
        Other.z=this->Z;
        Other.w=this->W;
        if(normalize)
            { Other.normalise(); }
        return Other;
    }

    void Quaternion::ExtractOgreQuaternion(const Ogre::Quaternion &Other)
    {
        this->X=Other.x;
        this->Y=Other.y;
        this->Z=Other.z;
        this->W=Other.w;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Left Hand Basic Arithmetic Operators

    Quaternion Quaternion::operator+ (const phys::Quaternion& Other) const
        { return Quaternion(this->X+Other.X, this->Y+Other.Y, this->Z+Other.Z, this->W+Other.W); }

    Quaternion Quaternion::operator+ (const Ogre::Quaternion& Other) const
        { return Quaternion(this->X+Other.x, this->Y+Other.y, this->Z+Other.z, this->W+Other.w); }

    Quaternion Quaternion::operator+ (const btQuaternion& Other) const
        { return Quaternion(this->X+Other.x(), this->Y+Other.y(), this->Z+Other.z(), this->W+Other.w()); }

    Quaternion Quaternion::operator- (const phys::Quaternion& Other) const
        { return Quaternion(this->X-Other.X, this->Y-Other.Y, this->Z-Other.Z, this->W-Other.W); }

    Quaternion Quaternion::operator- (const Ogre::Quaternion& Other) const
        { return Quaternion(this->X-Other.x, this->Y-Other.y, this->Z-Other.z, this->W-Other.w); }

    Quaternion Quaternion::operator- (const btQuaternion& Other) const
        { return Quaternion(this->X-Other.x(), this->Y-Other.y(), this->Z-Other.z(), this->W-Other.w()); }

    ///////////////////////////////////////////////////////////////////////////////
    // Increment and Decrement Operators

    Quaternion& Quaternion::operator+= (const phys::Quaternion& Other)
    {
        this->X=this->X+Other.X;
        this->Y=this->Y+Other.Y;
        this->Z=this->Z+Other.Z;
        this->W=this->W+Other.W;
        return *this;
    }

    Quaternion& Quaternion::operator+= (const Ogre::Quaternion& Other)
    {
        this->X=this->X+Other.x;
        this->Y=this->Y+Other.y;
        this->Z=this->Z+Other.z;
        this->W=this->W+Other.w;
        return *this;
    }

    Quaternion& Quaternion::operator+= (const btQuaternion& Other)
    {
        this->X=this->X+Other.x();
        this->Y=this->Y+Other.y();
        this->Z=this->Z+Other.z();
        this->W=this->W+Other.w();
        return *this;
    }

    Quaternion& Quaternion::operator-= (const phys::Quaternion& Other)
    {
        this->X=this->X-Other.X;
        this->Y=this->Y-Other.Y;
        this->Z=this->Z-Other.Z;
        this->W=this->W-Other.W;
        return *this;
    }

    Quaternion& Quaternion::operator-= (const Ogre::Quaternion& Other)
    {
        this->X=this->X-Other.x;
        this->Y=this->Y-Other.y;
        this->Z=this->Z-Other.z;
        this->W=this->W-Other.w;
        return *this;
    }

    Quaternion& Quaternion::operator-= (const btQuaternion& Other)
    {
        this->X=this->X-Other.x();
        this->Y=this->Y-Other.y();
        this->Z=this->Z-Other.z();
        this->W=this->W-Other.w();
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Assignment Operators
    Quaternion& Quaternion::operator= (const phys::Quaternion& Other)
    {
        this->X=Other.X;
        this->Y=Other.Y;
        this->Z=Other.Z;
        this->W=Other.W;
    }

    Quaternion& Quaternion::operator= (const btQuaternion& Other)
    {
        ExtractBulletQuaternion(Other);
        return *this;
    }

    Quaternion& Quaternion::operator= (const Ogre::Quaternion& Other)
    {
        ExtractOgreQuaternion(Other);
        return *this;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Right Hand Addition Operators

phys::Quaternion operator+ (const Ogre::Quaternion& Other, const phys::Quaternion& Other2)
    { return Other2+Other; }

phys::Quaternion operator+ (const btQuaternion& Other, const phys::Quaternion& Other2)
    { return Other2+Other; }

phys::Quaternion operator- (const Ogre::Quaternion& Other, const phys::Quaternion& Other2)
    { return phys::Quaternion(Other.x-Other2.X, Other.y-Other2.Y, Other.z-Other2.Z, Other.w-Other2.W); }

phys::Quaternion operator- (const btQuaternion& Other, const phys::Quaternion& Other2)
    { return phys::Quaternion(Other.getX()-Other2.X, Other.getY()-Other2.Y, Other.getZ()-Other2.Z, Other.getW()-Other2.W); }

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

btQuaternion& operator<< ( btQuaternion& Other, const phys::Quaternion& Other2)
{
    Other=Other2.GetBulletQuaternion();
    return Other;
}

btQuaternion& operator<< ( btQuaternion& Other, const Ogre::Quaternion& Other2)
{
    Other.setX(Other2.x);
    Other.setY(Other2.y);
    Other.setZ(Other2.z);
    Other.setW(Other2.w);
    return Other;
}

phys::Quaternion& operator<< ( phys::Quaternion& Other, const Ogre::Quaternion& Other2)
{
    Other=Other2;
    return Other;
}

phys::Quaternion& operator<< ( phys::Quaternion& Other, const btQuaternion& Other2)
{
    Other=Other2;
    return Other;
}

Ogre::Quaternion& operator<< ( Ogre::Quaternion& Other, const phys::Quaternion& Other2)
{
    Other=Other2.GetOgreQuaternion();
    return Other;
}

Ogre::Quaternion& operator<< ( Ogre::Quaternion& Other, const btQuaternion& Other2)
{
    Other.x=Other2.getX();
    Other.y=Other2.getY();
    Other.z=Other2.getZ();
    Other.w=Other2.getW();
}

std::ostream& operator << (std::ostream& stream, const phys::Quaternion& x)
{
    #ifdef PHYSXML
        stream << "<Quaternion Version=\"1\" X=\"" << x.X << "\" Y=\"" << x.Y << "\" Z=\"" << x.Z << "\" W=\"" << x.W << "\" />";
    #else
        stream << "[" << x.X << "," << x.Y << "," << x.Z << "," << x.W << "]";
    #endif // \PHYSXML
    return stream;
}

#ifdef PHYSXML
std::istream& operator >> (std::istream& stream, phys::Quaternion& Ev)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "Quaternion", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

phys::xml::Node& operator >> (const phys::xml::Node& OneNode, phys::Quaternion& Ev)
{
    if ( phys::String(OneNode.Name())==phys::String("Quaternion") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            Ev.X=OneNode.GetAttribute("X").AsReal();
            Ev.Y=OneNode.GetAttribute("Y").AsReal();
            Ev.Z=OneNode.GetAttribute("Z").AsReal();
            Ev.W=OneNode.GetAttribute("W").AsReal();
        }else{
            throw( phys::Exception("Incompatible XML Version for Quaternion: Not Version 1"));
        }
    }else{
        throw( phys::Exception(phys::StringCat("Attempting to deserialize a Quaternion, found a ", OneNode.Name())));
    }
}
#endif // \PHYSXML



#endif
