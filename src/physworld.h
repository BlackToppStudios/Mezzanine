//The game will need a containter for all the playing pieces, and something to talk to
//the screen and the physics engine. It makes sense to call this the world object.
#ifndef _physworld_h
#define _physworld_h

#include <sstream>
#include <string>

#include <ode/ode.h>
#include <Ogre.h>

using namespace std;

class physworld
{
	private:
        stringstream* ConsoleLog;
		dWorldID OdeWorld;
        Ogre::Root* OgreRoot;

	public:
		physworld();
		~physworld();
        string GetLog();
        void GameInit();
};

#endif
