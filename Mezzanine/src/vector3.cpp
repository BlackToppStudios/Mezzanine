//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _vector3_cpp
#define _vector3_cpp

#include "vector3.h"
#include "exception.h"
#include "serialization.h"
#include "stringtool.h"
#include "world.h"          // Needed for Error logging in streaming
#include "xml.h"            // Needed for streaming to xml

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include <cAudio.h>

#include <memory>

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // The Essentials

    Real Vector3::GetAxisValue(StandardAxis Axis) const
    {
        switch(Axis)
        {
            case 0: return this->X;
            case 1: return this->Y;
            case 2: return this->Z;
            default: throw(Exception("Cannot retrieve invalid StandardAxis in Vector3::GetAxisValue"));
        }
    }

    Real Vector3::GetAxisValue(Integer Axis) const
        { return this->GetAxisValue((StandardAxis)Axis); }

    Real& Vector3::GetAxisValue(const StandardAxis& Axis)
    {
        switch(Axis)
        {
            case 0: return this->X;
            case 1: return this->Y;
            case 2: return this->Z;
            default: throw(Exception("Cannot retrieve invalid StandardAxis in Vector3::GetAxisValue"));
        }
    }

    Real& Vector3::GetAxisValue(const Whole& Axis)
        { return this->GetAxisValue((StandardAxis)Axis); }

    Real Vector3::operator[] (const StandardAxis& Axis) const
        { return this->GetAxisValue(Axis); }

    Real Vector3::operator[] (const Whole& Axis) const
        { return this->GetAxisValue((StandardAxis)Axis); }

    Real& Vector3::operator[] (const StandardAxis& Axis)
        { return this->GetAxisValue(Axis); }

    Real& Vector3::operator[] (const Whole& Axis)
        { return this->GetAxisValue((StandardAxis)Axis); }
    ///////////////////////////////////////////////////////////////////////////////
    // Constructors
    Vector3::Vector3()
    {
        X=0;
        Y=0;
        Z=0;
    }

    Vector3::Vector3(const Real& x, const Real& y, const Real& z)
    {
        X=x;
        Y=y;
        Z=z;
    }

    Vector3::Vector3(const Ogre::Vector3& Vec)
        { this->ExtractOgreVector3(Vec); }

    Vector3::Vector3(const btVector3& Vec)
        { this->ExtractBulletVector3(Vec); }

    Vector3::Vector3(const cAudio::cVector3& Vec)
        { this->ExtractcAudioVector3(Vec); }

    Vector3::Vector3(const Mezzanine::Vector3& Vec)
        { *this = Vec; }

#ifdef MEZZXML
    Vector3::Vector3(xml::Node OneNode)
        { this->ProtoDeSerialize(OneNode); }
#endif

    ///////////////////////////////////////////////////////////////////////////////
    // Prebuilt Vectors

    Vector3 Vector3::Unit_X()
        { return Vector3(1,0,0); }

    Vector3 Vector3::Unit_Y()
        { return Vector3(0,1,0);}

    Vector3 Vector3::Unit_Z()
        { return Vector3(0,0,1); }

    Vector3 Vector3::Neg_Unit_X()
        { return Vector3(-1,0,0); }

    Vector3 Vector3::Neg_Unit_Y()
        { return Vector3(0,-1,0); }

    Vector3 Vector3::Neg_Unit_Z()
        { return Vector3(0,0,-1); }

    Vector3 Vector3::UnitOnAxis(StandardAxis Axis)
    {
        switch(Axis)
        {
            case 0: return Vector3::Unit_X();
            case 1: return Vector3::Unit_Y();
            case 2: return Vector3::Unit_Z();
            default: throw(Exception("Cannot convert invalid StandardAxis in Vector3::UnitOnAxis"));
        }
    }

    StandardAxis Vector3::IsStandardUnitAxis() const
    {
        if (1.0==this->X && 0.0==this->Y && 0.0==this->Z)
        {
            return Axis_X;
        } else if (0.0==this->X) {                                         // Not Unit_X
            if (1.0==this->Y && 0.0==this->Z)
            {
                return Axis_Y;
            } else if (0.0==this->Y && 1.0==this->Z) {                                     // Not Unit_Y so hopefully it is Z
                return Axis_Y;
            }
        }
        throw(Exception("Cannot convert Vector3 to StandardAxis in Vector3::IsStandardUnitAxis, Vector3 may not be Axis Aligned or may not be Unit Length."));
    }


    ///////////////////////////////////////////////////////////////////////////////
    // Assignment Operators
    Vector3& Vector3::operator= (const btVector3 &Vec)
    {
        this->X=Vec.getX();
        this->Y=Vec.getY();
        this->Z=Vec.getZ();
    }

    Vector3& Vector3::operator= (const Ogre::Vector3 &Vec)
    {
        this->X=Vec.x;
        this->Y=Vec.y;
        this->Z=Vec.z;
    }

    Vector3& Vector3::operator= (const cAudio::cVector3 &Vec)
    {
        this->X=Vec.x;
        this->Y=Vec.y;
        this->Z=Vec.z;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Unary Operators
    Vector3 Vector3::operator- ()
        { return Vector3(-X,-Y,-Z); }

    ///////////////////////////////////////////////////////////////////////////////
    // Vector3 Arithmetic with Real
    Vector3 Vector3::operator* (const Real &scalar) const
        { return Vector3(this->X * scalar, this->Y * scalar, this->Z * scalar); }

    Vector3 Vector3::operator/ (const Real &scalar) const
        { return Vector3(this->X / scalar, this->Y / scalar, this->Z / scalar); }

    ///////////////////////////////////////////////////////////////////////////////
    // Vector3 Arithmetic and assignment with Real
    Vector3& Vector3::operator*= (const Real &scalar)
    {
        this->X *= scalar;
        this->Y *= scalar;
        this->Z *= scalar;
        return *this;
    }

    Vector3& Vector3::operator/= (const Real &scalar)
    {
        this->X /= scalar;
        this->Y /= scalar;
        this->Z /= scalar;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Equality Comparison operators
    bool Vector3::operator== (const Vector3 &Vec) const
        { return( Vec.X == this->X && Vec.Y == this->Y && Vec.Z == this->Z ); }

    bool Vector3::operator== (const btVector3 &Vec) const
        { return( Vec.getX() == this->X && Vec.getY() == this->Y && Vec.getZ() == this->Z ); }

    bool Vector3::operator== (const Ogre::Vector3 &Vec) const
        { return ( Vec.x == this->X && Vec.y == this->Y && Vec.z == this->Z ); }

    bool Vector3::operator== (const cAudio::cVector3 &Vec) const
        { return ( Vec.x == this->X && Vec.y == this->Y && Vec.z == this->Z ); }


    bool Vector3::operator!= (const Vector3 &Vec) const
        { return ( Vec.X != this->X || Vec.Y != this->Y || Vec.Z != this->Z ); }

    bool Vector3::operator!= (const btVector3 &Vec) const
        { return ( Vec.getX() != this->X || Vec.getY() != this->Y || Vec.getZ() != this->Z ); }

    bool Vector3::operator!= (const Ogre::Vector3 &Vec) const
        { return ( Vec.x != this->X || Vec.y != this->Y || Vec.z != this->Z ); }

    bool Vector3::operator!= (const cAudio::cVector3 &Vec) const
        { return ( Vec.x != this->X || Vec.y != this->Y || Vec.z != this->Z ); }

    ///////////////////////////////////////////////////////////////////////////////
    // Arithmetic Operators

    Vector3 Vector3::operator+ (const Vector3 &Vec) const
        { return Vector3(X+Vec.X, Y+Vec.Y, Z+Vec.Z ); }

    Vector3 Vector3::operator- (const Vector3 &Vec) const
        { return Vector3(X-Vec.X, Y-Vec.Y, Z-Vec.Z ); }

    Vector3 Vector3::operator* (const Vector3 &Vec) const
        { return Vector3(X*Vec.X, Y*Vec.Y, Z*Vec.Z ); }

    Vector3 Vector3::operator/ (const Vector3 &Vec) const
        { return Vector3(X/Vec.X, Y/Vec.Y, Z/Vec.Z ); }

    ///////////////////////////////////////////////////////////////////////////////
    // Arithmetic Operators with btVector3

    Vector3 Vector3::operator+ (const btVector3  &Vec) const
        { return Vector3(X+Vec.getX(), Y+Vec.getY(), Z+Vec.getZ()); }

    Vector3 Vector3::operator- (const btVector3  &Vec) const
        { return Vector3(X-Vec.getX(), Y-Vec.getY(), Z-Vec.getZ()); }

    Vector3 Vector3::operator* (const btVector3  &Vec) const
        { return Vector3(X*Vec.getX(), Y*Vec.getY(), Z*Vec.getZ()); }

    Vector3 Vector3::operator/ (const btVector3  &Vec) const
        { return Vector3(X/Vec.getX(), Y/Vec.getY(), Z/Vec.getZ()); }

    ///////////////////////////////////////////////////////////////////////////////
    // Arithmetic Operators with Ogre::Vector3

    Vector3 Vector3::operator+ (const Ogre::Vector3 &Vec) const
        { return Vector3(X+Vec.x, Y+Vec.y, Z+Vec.z); }

    Vector3 Vector3::operator- (const Ogre::Vector3 &Vec) const
        { return Vector3(X-Vec.x, Y-Vec.y, Z-Vec.z); }

    Vector3 Vector3::operator* (const Ogre::Vector3 &Vec) const
        { return Vector3(X*Vec.x, Y*Vec.y, Z*Vec.z); }

    Vector3 Vector3::operator/ (const Ogre::Vector3 &Vec) const
        { return Vector3(X/Vec.x, Y/Vec.y, Z/Vec.z); }

    ///////////////////////////////////////////////////////////////////////////////
    // Arithmetic Operators with cAudio::cVector3

    Vector3 Vector3::operator+ (const cAudio::cVector3 &Vec) const
        { return Vector3(X+Vec.x, Y+Vec.y, Z+Vec.z); }

    Vector3 Vector3::operator- (const cAudio::cVector3 &Vec) const
        { return Vector3(X-Vec.x, Y-Vec.y, Z-Vec.z); }

    Vector3 Vector3::operator* (const cAudio::cVector3 &Vec) const
        { return Vector3(X*Vec.x, Y*Vec.y, Z*Vec.z); }

    Vector3 Vector3::operator/ (const cAudio::cVector3 &Vec)  const
        { return Vector3(X/Vec.x, Y/Vec.y, Z/Vec.z); }

    ///////////////////////////////////////////////////////////////////////////////
    // Fancy Math

    Vector3 Vector3::CrossProduct( const Vector3& Vec ) const
    {
        return Vector3(                                        // 1,2,3 . 4,5,6
                this->Y * Vec.Z - this->Z * Vec.Y,             // 2*6-3*5 = -3
                this->Z * Vec.X - this->X * Vec.Z,             // 3*4-1*6 = 6
                this->X * Vec.Y - this->Y * Vec.X              // 1*5-2*4 = -3
            );
    }

    Real Vector3::DotProduct(const Vector3& Vec) const
    {
        return this->X * Vec.X + this->Y * Vec.Y + this->Z * Vec.Z;
    }

    Vector3& Vector3::Normalize()
    {
        Real TempLength = this->Distance(Vector3(0.0f,0.0f,0.0f));
        if (0!=TempLength)
        {
             (*this) /= TempLength;
        }else{
            throw (Mezzanine::Exception ("Cannot Normalize Vector3(0,0,0)"));
        }
        return *this;
    }

    Vector3 Vector3::GetNormal() const
    {
        Real TempLength = this->Distance(Vector3(0.0f,0.0f,0.0f));
        if (0!=TempLength)
        {
            return (*this) / TempLength;
        }else{
            throw (Mezzanine::Exception ("Cannot Get the Normal of Vector3(0,0,0)"));
        }
    }

    Vector3 Vector3::GetDirection(const Vector3& Destination) const
    {
        Vector3 Dir = Destination - *this;
        Dir.Normalize();
        return Dir;
    }

    Vector3 Vector3::Inverse()
    {
        if (X!=0)
            X=1/X;
        if (Y!=0)
            Y=1/Y;
        if (Z!=0)
            Z=1/Z;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Manual Conversions
    btVector3 Vector3::GetBulletVector3() const
    {
        btVector3 Theirs;
        Theirs.setX(this->X);
        Theirs.setY(this->Y);
        Theirs.setZ(this->Z);
        Theirs.setW(0);
        return Theirs;
    }

    void Vector3::ExtractBulletVector3(const btVector3& Ours)
    {
        this->X=Ours.getX();
        this->Y=Ours.getY();
        this->Z=Ours.getZ();
    }

    Ogre::Vector3 Vector3::GetOgreVector3() const
    {
        Ogre::Vector3 Theirs;
        Theirs.x=this->X;
        Theirs.y=this->Y;
        Theirs.z=this->Z;
        return Theirs;
    }

    void Vector3::ExtractOgreVector3(const Ogre::Vector3& Ours)
    {
        this->X=Ours.x;
        this->Y=Ours.y;
        this->Z=Ours.z;
    }

    cAudio::cVector3 Vector3::GetcAudioVector3() const
    {
        cAudio::cVector3 Theirs;
        Theirs.x=this->X;
        Theirs.y=this->Y;
        Theirs.z=this->Z;
        return Theirs;
    }

    void Vector3::ExtractcAudioVector3(const cAudio::cVector3& Ours)
    {
        this->X=Ours.x;
        this->Y=Ours.y;
        this->Z=Ours.z;
    }

    Real Vector3::Distance(const Vector3 &OtherVec) const
    {
        float Xdelta = Ogre::Math::Abs(this->X - OtherVec.X);
        float Ydelta = Ogre::Math::Abs(this->Y - OtherVec.Y);
        float Zdelta = Ogre::Math::Abs(this->Z - OtherVec.Z);
        return Ogre::Math::Sqrt( Xdelta*Xdelta + Ydelta*Ydelta + Zdelta*Zdelta );
    }

#ifdef MEZZXML
        // Serializable
        void Vector3::ProtoSerialize(xml::Node& CurrentRoot) const
        {
            Mezzanine::xml::Node VecNode = CurrentRoot.AppendChild(SerializableName());
            VecNode.SetName(SerializableName());

            Mezzanine::xml::Attribute VersionAttr = VecNode.AppendAttribute("Version");
            Mezzanine::xml::Attribute XAttr = VecNode.AppendAttribute("X");
            Mezzanine::xml::Attribute YAttr = VecNode.AppendAttribute("Y");
            Mezzanine::xml::Attribute ZAttr = VecNode.AppendAttribute("Z");
            if( VersionAttr && XAttr && YAttr && ZAttr )
            {
                if( VersionAttr.SetValue("1") && XAttr.SetValue(this->X) && YAttr.SetValue(this->Y) && ZAttr.SetValue(this->Z))
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
        void Vector3::ProtoDeSerialize(const xml::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==Mezzanine::String(SerializableName()) )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    this->X=OneNode.GetAttribute("X").AsReal();
                    this->Y=OneNode.GetAttribute("Y").AsReal();
                    this->Z=OneNode.GetAttribute("Z").AsReal();
                }else{
                    throw( Mezzanine::Exception(StringTool::StringCat("Incompatible XML Version for ",SerializableName(),": Not Version 1")) );
                }
            }else{
                throw( Mezzanine::Exception(Mezzanine::StringTool::StringCat("Attempting to deserialize a ",SerializableName(),", found a ", OneNode.Name())));
            }
        }

        String Vector3::SerializableName()
            { return String("Vector3"); }
#endif

}

Mezzanine::Vector3 operator+ (const btVector3  &Vec, const Mezzanine::Vector3& lhs)
    { return lhs + Vec; }
Mezzanine::Vector3 operator- (const btVector3  &Vec, const Mezzanine::Vector3& lhs)
    { return Mezzanine::Vector3(Vec.getX()-lhs.X, Vec.getY()-lhs.Y, Vec.getZ()-lhs.Z); }
Mezzanine::Vector3 operator* (const btVector3  &Vec, const Mezzanine::Vector3& lhs)
    { return lhs * Vec; }
Mezzanine::Vector3 operator/ (const btVector3  &Vec, const Mezzanine::Vector3& lhs)
    { return Mezzanine::Vector3(Vec.getX()/lhs.X, Vec.getY()/lhs.Y, Vec.getZ()/lhs.Z); }

Mezzanine::Vector3 operator+ (const Ogre::Vector3 &Vec, const Mezzanine::Vector3& lhs)
    { return lhs + Vec; }
Mezzanine::Vector3 operator- (const Ogre::Vector3 &Vec, const Mezzanine::Vector3& lhs)
    { return Mezzanine::Vector3(Vec.x-lhs.X, Vec.y-lhs.Y, Vec.z-lhs.Z); }
Mezzanine::Vector3 operator* (const Ogre::Vector3 &Vec, const Mezzanine::Vector3& lhs)
    { return lhs * Vec; }
Mezzanine::Vector3 operator/ (const Ogre::Vector3 &Vec, const Mezzanine::Vector3& lhs)
    { return Mezzanine::Vector3(Vec.x/lhs.X, Vec.y/lhs.Y, Vec.z/lhs.Z); }

Mezzanine::Vector3 operator+ (const cAudio::cVector3 &Vec, const Mezzanine::Vector3& lhs)
    { return lhs + Vec; }
Mezzanine::Vector3 operator- (const cAudio::cVector3 &Vec, const Mezzanine::Vector3& lhs)
    { return Mezzanine::Vector3(Vec.x-lhs.X, Vec.y-lhs.Y, Vec.z-lhs.Z); }
Mezzanine::Vector3 operator* (const cAudio::cVector3 &Vec, const Mezzanine::Vector3& lhs)
    { return lhs * Vec; }
Mezzanine::Vector3 operator/ (const cAudio::cVector3 &Vec, const Mezzanine::Vector3& lhs)
    { return Mezzanine::Vector3(Vec.x/lhs.X, Vec.y/lhs.Y, Vec.z/lhs.Z); }

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
std::ostream& operator << (std::ostream& stream, const Mezzanine::Vector3& x)
{
    #ifdef MEZZXML
        //stream << "<Vector3 Version=\"1\" X=\"" << x.X << "\" Y=\"" << x.Y << "\" Z=\"" << x.Z << "\"/>";
        Serialize(stream,x);
    #else
        stream << "[" << x.X << "," << x.Y << "," << x.Z << "]";
    #endif // \MEZZXML
    return stream;
}

#ifdef MEZZXML
std::istream& operator >> (std::istream& stream, Mezzanine::Vector3& Vec)
    { return DeSerialize(stream, Vec); }

void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::Vector3& Vec)
    { Vec.ProtoDeSerialize(OneNode); }
#endif // \MEZZXML

Ogre::Vector3& operator << (Ogre::Vector3& VecTo, const Mezzanine::Vector3& VecFrom)
{
    VecTo = VecFrom.GetOgreVector3();
    return VecTo;
}

Ogre::Vector3& operator << (Ogre::Vector3& VecTo, const btVector3& VecFrom)
{
    VecTo.x=VecFrom.getX();
    VecTo.y=VecFrom.getY();
    VecTo.z=VecFrom.getZ();
    return VecTo;
}

Ogre::Vector3&  operator << (Ogre::Vector3& VecTo, const cAudio::cVector3& VecFrom)
{
    VecTo.x=VecFrom.x;
    VecTo.y=VecFrom.y;
    VecTo.z=VecFrom.z;
    return VecTo;
}


btVector3& operator << (btVector3& VecTo, const Ogre::Vector3& VecFrom)
{
    VecTo.setX(VecFrom.x);
    VecTo.setY(VecFrom.y);
    VecTo.setZ(VecFrom.z);
    VecTo.setW(0);
    return VecTo;
}

btVector3& operator << (btVector3& VecTo, const Mezzanine::Vector3& VecFrom)
{
    VecTo=VecFrom.GetBulletVector3();
    return VecTo;
}

btVector3&  operator << (btVector3& VecTo, const cAudio::cVector3& VecFrom)
{
    VecTo.setX(VecFrom.x);
    VecTo.setY(VecFrom.y);
    VecTo.setZ(VecFrom.z);
    return VecTo;
}

Mezzanine::Vector3& operator << (Mezzanine::Vector3& VecTo, const Ogre::Vector3& VecFrom)
{
    VecTo=VecFrom;
    return VecTo;
}

Mezzanine::Vector3& operator << (Mezzanine::Vector3& VecTo, const btVector3& VecFrom)
{
    VecTo=VecFrom;
    return VecTo;
}

Mezzanine::Vector3&  operator << (Mezzanine::Vector3& VecTo, const cAudio::cVector3& VecFrom)
{
    VecTo=VecFrom;
    return VecTo;
}

cAudio::cVector3&  operator << (cAudio::cVector3& VecTo, const Ogre::Vector3& VecFrom)
{
    VecTo.x=VecFrom.x;
    VecTo.y=VecFrom.y;
    VecTo.z=VecFrom.z;
    return VecTo;
}

cAudio::cVector3&  operator << (cAudio::cVector3& VecTo, const btVector3& VecFrom)
{
    VecTo.x=VecFrom.getX();
    VecTo.y=VecFrom.getY();
    VecTo.z=VecFrom.getZ();
    return VecTo;
}

cAudio::cVector3&  operator << (cAudio::cVector3& VecTo, const Mezzanine::Vector3& VecFrom)
{
    VecTo=VecFrom.GetcAudioVector3();
    return VecTo;
}

#endif
