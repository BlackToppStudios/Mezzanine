#ifndef _physquaternion_h
#define _physquaternion_h

#include "physdatatypes.h"

class PhysQuaternion
{
    public:
        PhysReal X;
        PhysReal Y;
        PhysReal Z;
        PhysReal W;

        PhysQuaternion();
        PhysQuaternion(PhysReal X, PhysReal Y, PhysReal Z, PhysReal W);

        btVector3 GetBulletQuaternion();
        Ogre::Quaternion GetOgreQuaternion();
};

#endif
