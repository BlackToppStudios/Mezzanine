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
#include "inputmanager.h"



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

	//All the pointers Ogre made should get taken care of by OGRE
	delete OgreRoot;
	delete OgreFrameListener;

	//remove sdl stuff
	void SDL_Quit(void);
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
	this->LoadOgreSettings();

	this->CreateRenderWindow();

	//bind our callbacks to Ogre and the render window
	OgreRoot->addFrameListener(OgreFrameListener);

	//Start the game rendering
	this->OgreRoot->startRendering();
}

void physworld::MoveCamera(Ogre::Vector3 Position, Ogre::Vector3 LookAt)
{
	this->OgreCamera->setPosition(Position);
    this->OgreCamera->lookAt(LookAt);
}

///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//Lods the Ogre Configuration
void physworld::LoadOgreSettings()
{
	//Try loading from the default location
	if (!this->OgreRoot->restoreConfig())
    {
    	//if we can't do that then lets make new settings
    	if (!this->ShowSystemSettingDialog())
    	{
    		this->LogAndThrow("Error 1: Could not setup Ogre.");
		}
    }

}

//Seriously read the Function Name
void physworld::CreateRenderWindow()
{
	//Get what is needed for SDL started
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		this->Log("Error 2: Unable to init SDL, SDL Error Follows:");
		this->LogAndThrow(SDL_GetError());
	}

	//Setup the SDL render window
	SDLscreen = SDL_SetVideoMode(PlayerSettings->getRenderHeight(), PlayerSettings->getRenderWidth(), 0, SDL_OPENGL);

	//Start Ogre Without a native render window
	this->OgreGameWindow = this->OgreRoot->initialise(false, "physgame");

	//Configure Ogre to render to the SDL window
	Ogre::NameValuePairList misc;
	misc=GetSDLOgreBinder();
	this->OgreGameWindow = this->OgreRoot->createRenderWindow("physgame", PlayerSettings->getRenderHeight(), PlayerSettings->getRenderWidth(), PlayerSettings->getFullscreen(), &misc);

	//prepare a scenemanager
	this->OgreSceneManager = this->OgreRoot->createSceneManager(Ogre::ST_GENERIC,"SceneManager");

	//setup a default camera
	this->OgreCamera = this->OgreSceneManager->createCamera("Camera");
	this->OgreCamera->setNearClipDistance(5.0f);
    this->OgreCamera->setFarClipDistance(5000.0f);

	this->MoveCamera(Ogre::Vector3(0.0f,0.0f,500.0f), Ogre::Vector3(0.0f,0.0f,0.0f));

	//viewport connects camera and render window
	this->OgreViewport = this->OgreGameWindow->addViewport(OgreCamera);

	//setting the aspect ratio must be done after we setup the viewport
	this->OgreCamera->setAspectRatio( Ogre::Real(OgreViewport->getActualWidth()) / Ogre::Real(OgreViewport->getActualHeight()) );
}



#endif
