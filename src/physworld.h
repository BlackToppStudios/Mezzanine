#ifndef _physworld_h
#define _physworld_h
///////////////////////////////////////////////////////////////////////////////
// The game will need a container for all the playing pieces, and something to
//talk tothe screen and the physics engine. It makes sense to call this the
//world object. This is the

///////////////////////////////////////////////////////////////////////////////
//Includes

#include <sstream>
#include <string>

#include <ode/ode.h>
#include <Ogre.h>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Physworld Class Declaration
class physworld
{
	private:
		//a place to store logs
        stringstream* ConsoleLog;

		//the ODE Physics world
		dWorldID OdeWorld;

		//Ogre objects
        Ogre::Root* OgreRoot;
        Ogre::RenderSystem* OgreRenderSystem;
        Ogre::RenderWindow* GameWindow;

	public:
		physworld();
		~physworld();

		//I plan on implementing a logging system beyond what ogre3d provides
        string GetLog();
        void GameInit();
};

#endif
