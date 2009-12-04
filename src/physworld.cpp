#ifndef _physworld_cpp
#define _physworld_cpp
///////////////////////////////////////////////////////////////////////////////
//The world that integrates everything

///////////////////////////////////////////////////////////////////////////////
//additional Includes
#include "physworld.h"
#include "crossplatform.h"
#include "physworldframelistener.h"
#include "gamebase.h"

///////////////////////////////////////////////////////////////////////////////
// Physworld constructor
//this should create the basic objects for stroing and tracking the roots of
//objects in the game
physworld::physworld()
{
	//We create our Ogre environment and ODE enviroment
	this->OgreRoot = new Ogre::Root(GetPluginsDotCFG(),GetSettingsDotCFG(),"Physgame.log");
	this->OgreFrameListener = new physworldFrameListener(this);

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
	delete OgreFrameListener;
}

///////////////////////////////////////////////////////////////////////////////
//appends to the gamelog which is managed by Ogre
void physworld::Log(string Message)
{
	//Ogre::Log::Stream << Message;
	Ogre::LogManager::getSingleton().logMessage(Message);
	//Ogre::LogManager::getSingleton().flush();
}

void physworld::LogAndThrow(string Message)
{
	this->Log(Message);
	throw(Message);
}

///////////////////////////////////////////////////////////////////////////////
//Shows the ogre settins Dialog, and allows it to save settings to ogres
//preset save location
bool physworld::ShowSystemSettingDialog()
{
	try
	{
		return this->OgreRoot->showConfigDialog();
	} catch (exception& e) {
		this->Log("Ogre settings windows from main UI or mandatory setting failure");
		this->Log(e.what());
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Start the Game already
void physworld::GameInit()
{

	//try to load the ogre config
    if (!this->OgreRoot->restoreConfig())
    {
    	if (!this->ShowSystemSettingDialog())
    	{
    		this->LogAndThrow("Could not setup Ogre.");
		}
    }

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

	//bind our callbacks to Ogre and the render window
	OgreRoot->addFrameListener(OgreFrameListener);

	//Start the game rendering
	this->OgreRoot->startRendering();
}
#endif
