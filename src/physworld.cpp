//this is the implemenation of the physworld
#ifndef _physworld_cpp
#define _physworld_cpp

#include "physworld.h"

physworld::physworld()
{
	//We create our Ogre environment and ODE enviroment
	this->OgreRoot = new Ogre::Root("/etc/OGRE/plugins.cfg");
	//this->GameWindow = this->OgreRoot->initialise(true, "Physgame");

	this->OdeWorld = dWorldCreate();
}

physworld::~physworld()
{
	//Destroy the physical world that we loved and cherished, until we SIGTERMED
	dWorldDestroy(this->OdeWorld);
}

string physworld::GetLog()
{
    return this->ConsoleLog->str();
}

void physworld::GameInit()
{
	this->OgreRoot->showConfigDialog();
}
#endif
