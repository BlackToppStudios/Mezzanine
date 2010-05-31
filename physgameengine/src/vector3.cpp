//© Copyright 2010 Joseph Toppi and John Blackwood
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

    /////////////////////////////////////////////////////////////////////
    // Operators
    void Vector3::operator= (const btVector3 &bt3)
    {
        (*this)<<bt3;
    }

    void Vector3::operator=(const Ogre::Vector3 &OVec3)
    {
        (*this)<<OVec3;
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
}

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
