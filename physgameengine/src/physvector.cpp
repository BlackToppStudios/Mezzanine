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

Ogre::Vector3 PhysVector3::GetOgreVector3()
{
    Ogre::Vector3 Theirs;
    Theirs.x=this->X;
    Theirs.y=this->Y;
    Theirs.z=this->Z;
    return Theirs;
}

#endif
