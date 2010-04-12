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
#ifndef _physvector_cpp
#define _physvector_cpp

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"

#include "physvector.h"

PhysVector3::PhysVector3()
{
    X=0;
    Y=0;
    Z=0;
}

PhysVector3::PhysVector3(PhysReal x, PhysReal y, PhysReal z)
{
	X=x;
	Y=y;
	Z=z;
}

std::ostream& operator << (std::ostream& stream, const PhysVector3& x)
{
    stream << "[" << x.X << "," << x.Y << "," << x.Z << "]";
    return stream;
}

btVector3 PhysVector3::GetBulletVector3()
{
    btVector3 Theirs;
    Theirs.setX(this->X);
    Theirs.setY(this->Y);
    Theirs.setZ(this->Z);
    Theirs.setW(0);
    return Theirs;
}

void PhysVector3::ExtractBulletVector3(btVector3 Ours)
{
    this->X=Ours.getX();
    this->Y=Ours.getY();
    this->Z=Ours.getZ();
}

Ogre::Vector3 PhysVector3::GetOgreVector3()
{
    Ogre::Vector3 Theirs;
    Theirs.x=this->X;
    Theirs.y=this->Y;
    Theirs.z=this->Z;
    return Theirs;
}

void PhysVector3::ExtractOgreVector3(Ogre::Vector3 Ours)
{
    this->X=Ours.x;
    this->Y=Ours.y;
    this->Z=Ours.z;
}

#endif
