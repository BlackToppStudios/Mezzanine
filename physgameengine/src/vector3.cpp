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
#ifndef _vector3_cpp
#define _vector3_cpp

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include <cAudio.h>

#include "vector3.h"
#include "exception.h"
#include "world.h"          // Needed for Error logging in streaming
#include "xml.h"            // Needed for streaming to xml

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    // Constructors
    Vector3::Vector3()
    {
        X=0;
        Y=0;
        Z=0;
    }

    Vector3::Vector3(Real x, Real y, Real z)
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

    Vector3::Vector3(const phys::Vector3& Vec)
        { *this = Vec; }

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

    bool Vector3::operator!= (const Vector3 &Vec) const
        { return ( Vec.X != this->X || Vec.Y != this->Y || Vec.Z != this->Z ); }

    bool Vector3::operator!= (const btVector3 &Vec) const
        { return ( Vec.getX() != this->X || Vec.getY() != this->Y || Vec.getZ() != this->Z ); }

    bool Vector3::operator!= (const Ogre::Vector3 &Vec) const
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
        return Vector3(
                this->Y * Vec.Z - this->Z * Vec.Y,
                this->Z * Vec.X - this->X * Vec.Z,
                this->X * Vec.Y - this->Y * Vec.X
            );
    }

    Real Vector3::dotProduct(const Vector3& Vec) const
    {
        return this->X * Vec.X + this->Y * Vec.Y + this->Z * Vec.Z;
    }

    void Vector3::Normalize()
    {
        Real TempLength = this->Distance(Vector3(0.0f,0.0f,0.0f));
        if (0!=TempLength)
        {
             (*this) /= TempLength;
        }else{
            /// @todo discuss the merits throwing an error here.
        }
    }

    Vector3 Vector3::GetNormal() const
    {
        Real TempLength = this->Distance(Vector3(0.0f,0.0f,0.0f));
        if (0!=TempLength)
        {
            return (*this) / TempLength;
        }else{
            /// @todo discuss the merits throwing an error here.
            return (*this);
        }
    }

    Vector3 Vector3::GetDirection(const Vector3& Destination) const
    {
        Vector3 Dir = Destination - *this;
        Dir.Normalize();
        return Dir;
    }

    void Vector3::Inverse()
    {
        /// @todo fix division by zero in Vector3::Inverse()
        X=1/X;
        Y=1/Y;
        Z=1/Z;
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

    void Vector3::ExtractBulletVector3(btVector3 Ours)
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

    void Vector3::ExtractOgreVector3(Ogre::Vector3 Ours)
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

    void Vector3::ExtractcAudioVector3(cAudio::cVector3 Ours)
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

}

phys::Vector3 operator+ (const btVector3  &Vec, const phys::Vector3& lhs)
    { return lhs + Vec; }
phys::Vector3 operator- (const btVector3  &Vec, const phys::Vector3& lhs)
    { return phys::Vector3(Vec.getX()-lhs.X, Vec.getY()-lhs.Y, Vec.getZ()-lhs.Z); }
phys::Vector3 operator* (const btVector3  &Vec, const phys::Vector3& lhs)
    { return lhs * Vec; }
phys::Vector3 operator/ (const btVector3  &Vec, const phys::Vector3& lhs)
    { return phys::Vector3(Vec.getX()/lhs.X, Vec.getY()/lhs.Y, Vec.getZ()/lhs.Z); }

phys::Vector3 operator+ (const Ogre::Vector3 &Vec, const phys::Vector3& lhs)
    { return lhs + Vec; }
phys::Vector3 operator- (const Ogre::Vector3 &Vec, const phys::Vector3& lhs)
    { return phys::Vector3(Vec.x-lhs.X, Vec.y-lhs.Y, Vec.z-lhs.Z); }
phys::Vector3 operator* (const Ogre::Vector3 &Vec, const phys::Vector3& lhs)
    { return lhs * Vec; }
phys::Vector3 operator/ (const Ogre::Vector3 &Vec, const phys::Vector3& lhs)
    { return phys::Vector3(Vec.x/lhs.X, Vec.y/lhs.Y, Vec.z/lhs.Z); }

phys::Vector3 operator+ (const cAudio::cVector3 &Vec, const phys::Vector3& lhs)
    { return lhs + Vec; }
phys::Vector3 operator- (const cAudio::cVector3 &Vec, const phys::Vector3& lhs)
    { return phys::Vector3(Vec.x-lhs.X, Vec.y-lhs.Y, Vec.z-lhs.Z); }
phys::Vector3 operator* (const cAudio::cVector3 &Vec, const phys::Vector3& lhs)
    { return lhs * Vec; }
phys::Vector3 operator/ (const cAudio::cVector3 &Vec, const phys::Vector3& lhs)
    { return phys::Vector3(Vec.x/lhs.X, Vec.y/lhs.Y, Vec.z/lhs.Z); }

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
std::ostream& operator << (std::ostream& stream, const phys::Vector3& x)
{
    #ifdef PHYSXML
        /*phys::xml::Document Doc;
        Doc.Load("");           // This sets the encoding to UTF8 ?!
        phys::xml::Node VecNode = Doc.AppendChild("Vector3");

        if (VecNode)
        {
            phys::xml::Attribute VersionAttr = VecNode.AppendAttribute("Version");
            phys::xml::Attribute XAttr = VecNode.AppendAttribute("X");
            phys::xml::Attribute YAttr = VecNode.AppendAttribute("Y");
            phys::xml::Attribute ZAttr = VecNode.AppendAttribute("Z");
            if( VersionAttr && XAttr && YAttr && ZAttr)
            {
                if( VersionAttr.SetValue("1") && XAttr.SetValue(x.X) && YAttr.SetValue(x.Y) && ZAttr.SetValue(x.Z))
                {
                    // It worked we don't need to do anything
                }else{
                    phys::World::GetWorldPointer()->LogAndThrow("Could not Stream Vector3 XML Attribute Values.");
                }
            }else{
                phys::World::GetWorldPointer()->LogAndThrow("Could not Stream Vector3 XML Attribute Names.");
            }
        }else{
            phys::World::GetWorldPointer()->LogAndThrow("Could not Stream Vector3 XML Anything.");
        }

        Doc.Save(stream,"\t",phys::xml::FormatNoDeclaration | phys::xml::FormatRaw);
        */
        stream << "<Vector3 Version=\"1\" X=\"" << x.X << "\" Y=\"" << x.Y << "\" Z=\"" << x.Z << "\" />";
    #else
        stream << "[" << x.X << "," << x.Y << "," << x.Z << "]";
    #endif // \PHYSXML
    return stream;
}

#ifdef PHYSXML
std::istream& operator >> (std::istream& stream, phys::Vector3& Vec)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "Vector3", OneTag) );

    Doc->GetFirstChild() >> Vec;

    return stream;
}

phys::xml::Node& operator >> (const phys::xml::Node& OneNode, phys::Vector3& Vec)
{
    if ( phys::String(OneNode.Name())==phys::String("Vector3") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            Vec.X=OneNode.GetAttribute("X").AsReal();
            Vec.Y=OneNode.GetAttribute("Y").AsReal();
            Vec.Z=OneNode.GetAttribute("Z").AsReal();
        }else{
            throw( phys::Exception("Incompatible XML Version for Vector3: Not Version 1"));
        }
    }else{
        throw( phys::Exception(phys::StringCat("Attempting to deserialize a Vector3, found a ", OneNode.Name())));
    }
}
#endif // \PHYSXML

Ogre::Vector3& operator << (Ogre::Vector3& VecTo, const phys::Vector3& VecFrom)
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

btVector3& operator << (btVector3& VecTo, const phys::Vector3& VecFrom)
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

phys::Vector3& operator << (phys::Vector3& VecTo, const Ogre::Vector3& VecFrom)
{
    VecTo=VecFrom;
    return VecTo;
}

phys::Vector3& operator << (phys::Vector3& VecTo, const btVector3& VecFrom)
{
    VecTo=VecFrom;
    return VecTo;
}

phys::Vector3&  operator << (phys::Vector3& VecTo, const cAudio::cVector3& VecFrom)
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

cAudio::cVector3&  operator << (cAudio::cVector3& VecTo, const phys::Vector3& VecFrom)
{
    VecTo=VecFrom.GetcAudioVector3();
    return VecTo;
}

#endif
