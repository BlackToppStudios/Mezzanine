//The game will need a containter for all the playing pieces, and something to talk to
//the screen and the physics engine. It makes sense to call this the world object.
#ifndef _physworld_h
#define _physworld_h

#include <ode/ode.h>

class physworld
{
	private:
		dWorldID OdeWorld;
	public:
		physworld();
		~physworld();
	
};

#endif
