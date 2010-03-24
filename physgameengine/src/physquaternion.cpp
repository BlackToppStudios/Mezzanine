#ifndef _physquaternion_cpp
#define _physquaternion_cpp

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"

#include "physquaternion.h"

PhysQuaternion::PhysQuaternion()
{
    X=0;
    Y=0;
    Z=0;
    W=0;
}

PhysQuaternion::PhysQuaternion(PhysReal x, PhysReal y, PhysReal z, PhysReal w)
{
    X=x;
    Y=y;
    Z=z;
    W=w;
}

btVector3 PhysQuaternion::GetBulletQuaternion()
{
    btVector3 Theirs;
    Theirs.setX(this->X);
    Theirs.setY(this->Y);
    Theirs.setZ(this->Z);
    Theirs.setW(this->W);
    return Theirs;
}

Ogre::Quaternion PhysQuaternion::GetOgreQuaternion()
{
    Ogre::Quaternion Theirs;
    Theirs.x=this->X;
    Theirs.y=this->Y;
    Theirs.z=this->Z;
    Theirs.w=this->W;
    return Theirs;
}

#endif
