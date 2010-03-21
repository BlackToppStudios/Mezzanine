#ifndef _physvector_cpp
#define _physvector_cpp

#include <Ogre.h>
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

PhysQuaternion::PhysQuaternion()
{
    X=0;
    Y=0;
    Z=0;
    W=0;
}

PhysQuaternion::PhysQuaternion(PhysReal X, PhysReal Y, PhysReal Z, PhysReal W)
{
    X=x;
    Y=y;
    Z=z;
    W=w;
}

#endif
