#ifndef _physworld_cpp
#define _physworld_cpp
///////////////////////////////////////////////////////////////////////////////
//The world that integrates everything

///////////////////////////////////////////////////////////////////////////////
//additional Includes
#include "physworld.h"
#include "crossplatform.h"

///////////////////////////////////////////////////////////////////////////////
// Physworld constructor
//this should create the basic objects for stroing and tracking the roots of
//objects in the game
physworld::physworld()
{
	//We create our Ogre environment and ODE enviroment
	this->OgreRoot = new Ogre::Root(GetPluginsDotCFG());
	//this->GameWindow = this->OgreRoot->initialise(true, "Physgame");

	this->OdeWorld = dWorldCreate();
}

///////////////////////////////////////////////////////////////////////////////
//tears the world down
physworld::~physworld()
{
	//Destroy the physical world that we loved and cherished, until we SIGTERMED
	dWorldDestroy(this->OdeWorld);
}

///////////////////////////////////////////////////////////////////////////////
//The best access to what is in the gamelog
//returns a string copy of the game log.
string physworld::GetLog()
{
    return this->ConsoleLog->str();
}

///////////////////////////////////////////////////////////////////////////////
// Start the Game already
void physworld::GameInit()
{
	this->OgreRoot->showConfigDialog();
}
#endif
