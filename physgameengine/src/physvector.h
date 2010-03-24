#ifndef _physvector_h
#define _physvector_h

#include "physdatatypes.h"

//Forward Declarations for wierd compatibility functions
class btVector3;
namespace Ogre
{
    class Vector3;
}

class PhysVector3
{
	public:
		PhysReal X;
		PhysReal Y;
		PhysReal Z;

        PhysVector3();
		PhysVector3(PhysReal X, PhysReal Y, PhysReal Z);

        //Operators
        btVector3 GetBulletVector3();
        Ogre::Vector3 GetOgreVector3();
};

std::ostream& operator << (std::ostream& stream, const PhysVector3& x);

#endif
