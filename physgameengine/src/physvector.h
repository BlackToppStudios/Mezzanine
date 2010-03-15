#ifndef _physvector_h
#define _physvector_h

#include "physdatatypes.h"

class PhysVector3
{
	public:
		PhysReal X;
		PhysReal Y;
		PhysReal Z;

        PhysVector3();
		PhysVector3(PhysReal X, PhysReal Y, PhysReal Z);
};

#endif
