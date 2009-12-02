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

	//try to load the ogre config
    if (!this->OgreRoot->restoreConfig())
      	{throw "Could no setup Ogre.";}
    //RenderWindow* mWindow = mRoot->initialise(true,"Application");


	//instantiate the Physics engine
	this->OdeWorld = dWorldCreate();

	//initilize the settings
	PlayerSettings = new Settings();
}

///////////////////////////////////////////////////////////////////////////////
//tears the world down
physworld::~physworld()
{
	//Destroy the physical world that we loved and cherished, until we SIGTERMED
	dWorldDestroy(this->OdeWorld);

	//All the pointers Ogtr made should get taken care of by OGRE
	delete OgreRoot;
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
	//good for testing
	//this->OgreRoot->showConfigDialog();

	//Setup the render window
	this->OgreGameWindow = this->OgreRoot->initialise(true, "physgame");
	this->OgreGameWindow->setFullscreen(PlayerSettings->getFullscreen(),400,400);

	//creates a scenemanagernamed
	this->OgreSceneManager = this->OgreRoot->createSceneManager(Ogre::ST_GENERIC,"SceneManager");

	//get a camera and set the position
    this->OgreCamera = this->OgreSceneManager->createCamera("Camera");
    this->OgreCamera->setPosition(Ogre::Vector3(0.0f,0.0f,500.0f));
    this->OgreCamera->lookAt(Ogre::Vector3(0.0f,0.0f,0.0f));
    this->OgreCamera->setNearClipDistance(5.0f);
    this->OgreCamera->setFarClipDistance(5000.0f);

	//viewport connects camera and render window
	this->OgreViewport = this->OgreGameWindow->addViewport(OgreCamera);
    //this->OgreCamera->setBackgroundColour(Ogre::ColourValue(0.0f,0.0f,0.0f));

	//setting the aspect ratio must be done after we setup the viewport
	this->OgreCamera->setAspectRatio( Ogre::Real(OgreViewport->getActualWidth()) / Ogre::Real(OgreViewport->getActualHeight()) );

	//Start the game rendering
	this->OgreRoot->startRendering();
}
#endif
