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
#ifndef _vector3_cpp
#define _vector3_cpp

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include <cAudio.h>

#include "vector3.h"

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

    Vector3::Vector3(Ogre::Vector3 Vec)
    {
        this->ExtractOgreVector3(Vec);
    }

    Vector3::Vector3(btVector3 Vec)
    {
        this->ExtractBulletVector3(Vec);
    }

    Vector3::Vector3(cAudio::cVector3 Vec)
    {
        this->ExtractcAudioVector3(Vec);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Assignment Operators
    Vector3& Vector3::operator= (const btVector3 &bt3)
    {
        (*this)<<bt3;
        return *this;
    }

    Vector3& Vector3::operator=(const Ogre::Vector3 &Vec3)
    {
        (*this)<<Vec3;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Unary Operators
    Vector3 Vector3::operator- ()
    {
        Vector3 Temp(-X,-Y,-Z);
        return Temp;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Vector3 Arithmetic with Real
    Vector3 Vector3::operator* (const Real &scalar) const
    {
        return Vector3(
            this->X * scalar,
            this->Y * scalar,
            this->Z * scalar
        );
    }

    Vector3 Vector3::operator/ (const Real &scalar) const
    {
        return Vector3(
            this->X / scalar,
            this->Y / scalar,
            this->Z / scalar
        );
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Vector3 Arithmetic and assignment with Real
    void Vector3::operator*= (const Real &scalar)
    {
        this->X *= scalar;
        this->Y *= scalar;
        this->Z *= scalar;
    }

    void Vector3::operator/= (const Real &scalar)
    {
        this->X /= scalar;
        this->Y /= scalar;
        this->Z /= scalar;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Equality Comparison operators
    bool Vector3::operator== (const Vector3 &Vec2)
    {
        if ( Vec2.X == this->X && Vec2.Y == this->Y && Vec2.Z == this->Z )
            { return true; }
        return false;
    }

    bool Vector3::operator== (const btVector3 &Vec2)
    {
        if ( Vec2.getX() == this->X && Vec2.getY() == this->Y && Vec2.getZ() == this->Z )
            { return true; }
        return false;
    }

    bool Vector3::operator== (const Ogre::Vector3 &Vec2)
    {
        if ( Vec2.x == this->X && Vec2.y == this->Y && Vec2.z == this->Z )
            { return true; }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Arithmetic Operators

    Vector3 Vector3::operator+ (const Vector3 &Vec2) const
    {
        Vector3 Temp(X,Y,Z);
        Temp.X+=Vec2.X;
        Temp.Y+=Vec2.Y;
        Temp.Z+=Vec2.Z;
        return Temp;
    }

    Vector3 Vector3::operator- (const Vector3 &Vec2) const
    {
        Vector3 Temp(X,Y,Z);
        Temp.X-=Vec2.X;
        Temp.Y-=Vec2.Y;
        Temp.Z-=Vec2.Z;
        return Temp;
    }

    Vector3 Vector3::operator* (const Vector3 &Vec2) const
    {
        Vector3 Temp(X,Y,Z);
        Temp.X*=Vec2.X;
        Temp.Y*=Vec2.Y;
        Temp.Z*=Vec2.Z;
        return Temp;
    }

    Vector3 Vector3::operator/ (const Vector3 &Vec2) const
    {
        Vector3 Temp(X,Y,Z);
        Temp.X/=Vec2.X;
        Temp.Y/=Vec2.Y;
        Temp.Z/=Vec2.Z;
        return Temp;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Arithmetic Operators with btVector3

    Vector3 Vector3::operator+ (const btVector3  &Vec2)
    {
        Vector3 Temp(X,Y,Z);
        Temp.X+=Vec2.getX();
        Temp.Y+=Vec2.getY();
        Temp.Z+=Vec2.getZ();
        return Temp;
    }

    Vector3 Vector3::operator- (const btVector3  &Vec2)
    {
        Vector3 Temp(X,Y,Z);
        Temp.X-=Vec2.getX();
        Temp.Y-=Vec2.getY();
        Temp.Z-=Vec2.getZ();
        return Temp;
    }

    Vector3 Vector3::operator* (const btVector3  &Vec2)
    {
        Vector3 Temp(X,Y,Z);
        Temp.X*=Vec2.getX();
        Temp.Y*=Vec2.getY();
        Temp.Z*=Vec2.getZ();
        return Temp;
    }

    Vector3 Vector3::operator/ (const btVector3  &Vec2)
    {
        Vector3 Temp(X,Y,Z);
        Temp.X/=Vec2.getX();
        Temp.Y/=Vec2.getY();
        Temp.Z/=Vec2.getZ();
        return Temp;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Arithmetic Operators with Ogre::Vector3

    Vector3 Vector3::operator+ (const Ogre::Vector3 &Vec2)
    {
        Vector3 Temp(X,Y,Z);
        Temp.X+=Vec2.x;
        Temp.Y+=Vec2.y;
        Temp.Z+=Vec2.z;
        return Temp;
    }

    Vector3 Vector3::operator- (const Ogre::Vector3 &Vec2)
    {
        Vector3 Temp(X,Y,Z);
        Temp.X-=Vec2.x;
        Temp.Y-=Vec2.y;
        Temp.Z-=Vec2.z;
        return Temp;
    }

    Vector3 Vector3::operator* (const Ogre::Vector3 &Vec2)
    {
        Vector3 Temp(X,Y,Z);
        Temp.X*=Vec2.x;
        Temp.Y*=Vec2.y;
        Temp.Z*=Vec2.z;
        return Temp;
    }

    Vector3 Vector3::operator/ (const Ogre::Vector3 &Vec2)
    {
        Vector3 Temp(X,Y,Z);
        Temp.X/=Vec2.x;
        Temp.Y/=Vec2.y;
        Temp.Z/=Vec2.z;
        return Temp;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Fancy Math

    Vector3 Vector3::CrossProduct( const Vector3& rkVector ) const
    {
        return Vector3(
                this->Y * rkVector.Z - this->Z * rkVector.Y,
                this->Z * rkVector.X - this->X * rkVector.Z,
                this->X * rkVector.Y - this->Y * rkVector.X
            );
    }

    Real Vector3::dotProduct(const Vector3& vec) const
    {
        return this->X * vec.X + this->Y * vec.Y + this->Z * vec.Z;
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

    void Vector3::Inverse()
    {
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

    Real Vector3::Distance(const Vector3 &Vec2) const
    {
        float Xdelta = this->X - Vec2.X;
        float Ydelta = this->Y - Vec2.Y;
        float Zdelta = this->Z - Vec2.Z;
        return Ogre::Math::Sqrt( Xdelta*Xdelta + Ydelta*Ydelta + Zdelta*Zdelta );
    }

}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
std::ostream& operator << (std::ostream& stream, const phys::Vector3& x)
{
    stream << "[" << x.X << "," << x.Y << "," << x.Z << "]";
    return stream;
}


Ogre::Vector3& operator << (Ogre::Vector3& VecTo, const phys::Vector3& VecFrom)
{
    VecTo.x=VecFrom.X;
    VecTo.y=VecFrom.Y;
    VecTo.z=VecFrom.Z;
    return VecTo;
}

Ogre::Vector3& operator << (Ogre::Vector3& VecTo, const btVector3& VecFrom)
{
    VecTo.x=VecFrom.getX();
    VecTo.y=VecFrom.getY();
    VecTo.z=VecFrom.getZ();
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
    VecTo.setX(VecFrom.X);
    VecTo.setY(VecFrom.Y);
    VecTo.setZ(VecFrom.Z);
    VecTo.setW(0);
    return VecTo;
}

phys::Vector3& operator << (phys::Vector3& VecTo, const Ogre::Vector3& VecFrom)
{
    VecTo.X=VecFrom.x;
    VecTo.Y=VecFrom.y;
    VecTo.Z=VecFrom.z;
    return VecTo;
}

phys::Vector3& operator << (phys::Vector3& VecTo, const btVector3& VecFrom)
{
    VecTo.X=VecFrom.getX();
    VecTo.Y=VecFrom.getY();
    VecTo.Z=VecFrom.getZ();
    return VecTo;
}

#endif
