//this is the implemenation of the physworld
#ifndef _physworld_cpp
#define _physworld_cpp

#include "physworld.h"

physworld::physworld()
{
	//we create the world of physics and remember its locatation
	this->OdeWorld=dWorldCreate();
}

physworld::~physworld()
{
	//Destroy the physical world that we loved and cherished, until we SIGTERMED
	dWorldDestroy(this->OdeWorld);	
}

#endif
