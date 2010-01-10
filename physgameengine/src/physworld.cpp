#ifndef _physworld_cpp
#define _physworld_cpp
///////////////////////////////////////////////////////////////////////////////
//The world that integrates everything
//
//This is where all the calls to the the underlying libraries should be from.
//physworld is an abstraction layer to all of the non-ui libraries. The ui
//layers wx and ogre both have their own main loops, so it is almost mandatory
//for other code to interact with those libraries directly.
///////////////////////////////////////////////////////////////////////////////
//Includes
#include "physevent.h"
#include "physworld.h"
#include "physvector.h"
#include "physcrossplatform.h"
#include "physworldcallbackmanager.h"
#include "physgamesettings.h"

#include "SDL.h"
#include "btBulletDynamicsCommon.h"

///////////////////////////////////////////////////////////////////////////////
// Physworld constructor
//this should create the basic objects for stroing and tracking the roots of
//objects in the game
physworld::physworld()
{
	//We create our Ogre environment and ODE enviroment
	this->OgreRoot = new Ogre::Root(GetPluginsDotCFG(),GetSettingsDotCFG(),"Physgame.log");

    //Callbacks are the main way that a game using the physworld will be able to have their code run at custom times
	this->CallBacks = new physworldCallBackManager(this);

    //Events are the main way for the game using the physworld to  get information about the various subsystems
    this->Events = new PhysEventManager;


	//instantiate the Physics engine
	//this->OdeWorld = dWorldCreate();

	//initilize the settings
	PlayerSettings = new Settings();
}

///////////////////////////////////////////////////////////////////////////////
//tears the world down
physworld::~physworld()
{
	//Destroy the physical world that we loved and cherished, until we SIGTERMED
	//dWorldDestroy(this->OdeWorld);

	//All the pointers Ogre made should get taken care of by OGRE
	delete OgreRoot;
	delete CallBacks;

	delete PlayerSettings;

	SDL_FreeSurface(SDLscreen);

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

    //Create a the RenderTimer, which will be used to measure the time
    Ogre::Timer RenderTimer;

	//Start the game rendering
	//this->OgreRoot->startRendering();
	bool Callback1 = true;
	bool Callback2 = true;
	bool Callback3 = true;
	bool Callback4 = true;

	//This is the beginning of the mainloop
	//As long as all the CallBacks return true the game continues
	while (Callback1 && Callback2 && Callback3 && Callback4)
	{
        //To prepare each callback we add an event to the event manager which includes the time sine th last frame render ended
        //However we will only do this if a callback is set
        //new PhysEventRenderTime(RenderTimer.getMilliseconds);
        if(this->CallBacks->IsPreInputCallbackSet())
        {
			this->Events->AddEvent(new PhysEventRenderTime(RenderTimer.getMilliseconds()));
			Callback1 = this->CallBacks->PreInput();
			this->DoMainLoopInputBuffering();
        }

		if(this->CallBacks->IsPrePhysicsCallbackSet())
        {
			this->Events->AddEvent(new PhysEventRenderTime(RenderTimer.getMilliseconds()));
			Callback2 = this->CallBacks->PrePhysics();
			this->DoMainLoopPhysics();
        }

		if(this->CallBacks->IsPreRenderCallbackSet())
        {
			this->Events->AddEvent(new PhysEventRenderTime(RenderTimer.getMilliseconds()));
			Callback3 = this->CallBacks->PreRender();
        }

		this->OgreRoot->renderOneFrame();
		RenderTimer.reset();

		if(this->CallBacks->IsPostRenderCallbackSet())
        {
			this->Events->AddEvent(new PhysEventRenderTime(RenderTimer.getMilliseconds()));
			Callback4 = this->CallBacks->PostRender();
        }
		//Callback4=false;//This is to force the mainloop to exit af one iteration
	}//End of main loop
}

void physworld::MoveCamera(PhysVector3 Position, PhysVector3 LookAt)
{
	this->OgreCamera->setPosition(Ogre::Vector3(Position.X,Position.Y,Position.Z));
    this->OgreCamera->lookAt(Ogre::Vector3(LookAt.X,LookAt.Y,LookAt.Z));
}

void physworld::DoMainLoopAllItems()
{
	this->DoMainLoopPhysics();
	this->DoMainLoopInputBuffering();
}

void physworld::DoMainLoopPhysics()
{
	//TODO: Step the physics world here per main loop items
	//this->Events->AddsomeEvents
}

void physworld::DoMainLoopInputBuffering()
{
	SDL_PumpEvents();
	//TODO: add the input gather to the event manager
}



///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////
//Loads the Ogre Configuration
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
	this->SDLscreen = SDL_SetVideoMode(PlayerSettings->getRenderHeight(), PlayerSettings->getRenderWidth(), 0, SDL_OPENGL);

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

	this->MoveCamera(PhysVector3(0.0f,0.0f,500.0f), PhysVector3(0.0f,0.0f,0.0f));

	//viewport connects camera and render window
	this->OgreViewport = this->OgreGameWindow->addViewport(OgreCamera);

	//setting the aspect ratio must be done after we setup the viewport
	this->OgreCamera->setAspectRatio( Ogre::Real(OgreViewport->getActualWidth()) / Ogre::Real(OgreViewport->getActualHeight()) );
}

#endif
