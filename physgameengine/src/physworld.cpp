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
#include "physdatatypes.h"
#include "physevent.h"
#include "physworld.h"
#include "physvector.h"
#include "physcrossplatform.h"
#include "physworldcallbackmanager.h"
#include "physgamesettings.h"

#include "SDL.h"
#include "btBulletDynamicsCommon.h"
#include <Ogre.h>

#include <sstream>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Physworld constructor
//this should create the basic objects for stroing and tracking the roots of
//objects in the game
physworld::physworld()
{
	this->Construct(
		new PhysVector3(-100.0,-100.0,-100.0),
		new PhysVector3(100.0, 100.0, 100.0),
		10
		);
}

physworld::physworld(PhysVector3* GeographyLowerBounds_, PhysVector3* GeographyUpperbounds_, unsigned short int  MaxPhysicsProxies_)
{
	this->Construct(GeographyLowerBounds_, GeographyUpperbounds_, MaxPhysicsProxies_);
}

void physworld::Construct(PhysVector3* GeographyLowerBounds_, PhysVector3* GeographyUpperbounds_, unsigned short int  MaxPhysicsProxies_)
{
	PlayerSettings = new Settings();

	//We create our Ogre environment
	this->OgreRoot = new Ogre::Root(GetPluginsDotCFG(),GetSettingsDotCFG(),"Physgame.log");
	//Ogre resource related code
	//this->OgreRoot->addResourceLocation(GetDataDirectory(), "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false);
    this->OgreRoot->addResourceLocation(GetDataDirectory(), "FileSystem", "Robot", false);
    this->OgreResource = Ogre::ResourceGroupManager::getSingletonPtr();
    this->OgreResource->declareResource("robot.mesh", "Mesh", "Robot");
    //this->OgreResource->declareResource("robot.skeleton", "Skeleton", "Robot");
    this->OgreResource->declareResource("Examples.material", "Material", "Robot");
    //this->OgreResource->declareResource("r2skin.jpg", "Texture", "Robot");
    //this->OgreResource->initialiseResourceGroup("Robot");

    //Callbacks are the main way that a game using the physworld will be able to have their code run at custom times
	this->CallBacks = new physworldCallBackManager(this);

    //Events are the main way for the game using the physworld to  get information about the various subsystems
    this->Events = new PhysEventManager;

	//instantiate the Physics engine and related items
	GeographyLowerBounds = GeographyLowerBounds_;
	GeographyUpperbounds = GeographyUpperbounds_;
	MaxPhysicsProxies = MaxPhysicsProxies_;

	btVector3 worldAabbMin(GeographyLowerBounds->X, GeographyLowerBounds->Y, GeographyLowerBounds->Z);
	btVector3 worldAabbMax(GeographyUpperbounds->X, GeographyUpperbounds->Y, GeographyUpperbounds->Z);

	this->BulletBroadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, MaxPhysicsProxies);
	this->BulletSolver = new btSequentialImpulseConstraintSolver;
	this->BulletCollisionConfiguration = new btDefaultCollisionConfiguration();
	this->BulletDispatcher = new btCollisionDispatcher(BulletCollisionConfiguration);

	this->BulletDynamicsWorld = new btDiscreteDynamicsWorld(
												BulletDispatcher,
												BulletBroadphase,
												BulletSolver,
												BulletCollisionConfiguration);

}
///////////////////////////////////////////////////////////////////////////////
//tears the world down
physworld::~physworld()
{
	//Destroy the physical world that we loved and cherished
	//dWorldDestroy(this->OdeWorld);
	delete GeographyLowerBounds;
	delete GeographyUpperbounds;
	delete BulletBroadphase;
	delete BulletCollisionConfiguration;
	delete BulletDispatcher;
	delete BulletSolver;
	delete BulletDynamicsWorld;

	//All the pointers Ogre made should get taken care of by OGRE
	delete OgreRoot;

	//clear up our objects
	delete CallBacks;
	delete Events;


	delete PlayerSettings;

	SDL_FreeSurface(SDLscreen);

	//remove sdl stuff
	void SDL_Quit(void);
}

///////////////////////////////////////////////////////////////////////////////
//appends to the gamelog which is managed by Ogre
void physworld::Log(string Message)
{
	Ogre::LogManager::getSingleton().logMessage(Message);
}

void physworld::Log(PhysWhole Message)
{
	stringstream temp;
	temp << Message;
	Ogre::LogManager::getSingleton().logMessage(temp.str());
}

void physworld::Log(PhysReal Message)
{
	stringstream temp;
	temp << Message;
	Ogre::LogManager::getSingleton().logMessage(temp.str());
}

void physworld::Log(size_t Message)
{
	stringstream temp;
	temp << Message;
	Ogre::LogManager::getSingleton().logMessage(temp.str());
}

void physworld::LogAndThrow(string Message)
{
	this->Log(Message);
	throw(Message);
}

void physworld::LogAndThrow(PhysWhole Message)
{
	this->Log(Message);
	throw(Message);
}

void physworld::LogAndThrow(PhysReal Message)
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

	bool Callbackbools[] = {true, true, true, true};

	//Used for tracking times to prevent Infinite render loops in graphically simple games
	//PhysWhole Times[] = {0,0,0,0};

	//TODO finish test code
	this->OgreResource->initialiseResourceGroup("Robot");
	this->OgreSceneManager->setAmbientLight( Ogre::ColourValue( 1, 1, 1 ) );
	Ogre::Entity *ent1 = this->OgreSceneManager->createEntity( "Robot", "robot.mesh" );
	Ogre::SceneNode *node1 = this->OgreSceneManager->getRootSceneNode()->createChildSceneNode( "RobotNode" );
	node1->attachObject( ent1 );

    PhysWhole FrameDelay = 0;
    PhysWhole FrameTime = 0;

	//This is the beginning of the mainloop
	//As long as all the CallBacks return true the game continues
	while (Callbackbools[0] && Callbackbools[1] && Callbackbools[2] && Callbackbools[3])
	{
        //To prepare each callback we add an event to the event manager which includes the time since th last frame render ended
        //However we will only do this if a callback is set
        if(this->CallBacks->IsPreInputCallbackSet())
        {
			this->Events->AddEvent(new PhysEventRenderTime(FrameTime));
			Callbackbools[0] = this->CallBacks->PreInput();
			this->DoMainLoopInputBuffering();
        }

		if(this->CallBacks->IsPrePhysicsCallbackSet())
        {
            this->Events->AddEvent(new PhysEventRenderTime(FrameTime));
			Callbackbools[1] = this->CallBacks->PrePhysics();
			this->DoMainLoopPhysics();
        }

		if(this->CallBacks->IsPreRenderCallbackSet())
        {
            this->Events->AddEvent(new PhysEventRenderTime(FrameTime));
			Callbackbools[2] = this->CallBacks->PreRender();
        }

		//Render the frame and figure the amount of time it took
		this->DoMainLoopRender();
		FrameTime = RenderTimer.getMilliseconds(); //Limit frame rate to 62.5
		RenderTimer.reset();
		if(16>FrameTime){               		//use 16666 for microseconds
		    FrameDelay++;
			WaitMilliseconds( FrameDelay );
		}else if(16==FrameTime){
        }else{
            if (0<FrameDelay){
                FrameDelay--;
            }else{
                FrameDelay=0;
            }
        }

		if(this->CallBacks->IsPostRenderCallbackSet())
        {
			this->Events->AddEvent(new PhysEventRenderTime(FrameTime));
			Callbackbools[3] = this->CallBacks->PostRender();
        }

	}//End of main loop

	//Some after loop cleanup
	this->DestroyRenderWindow();
}

void physworld::MoveCamera(PhysVector3 Position, PhysVector3 LookAt)
{
	this->OgreCamera->setPosition(Ogre::Vector3(Position.X,Position.Y,Position.Z));
    this->OgreCamera->lookAt(Ogre::Vector3(LookAt.X,LookAt.Y,LookAt.Z));
}

void physworld::DoMainLoopAllItems()
{
	this->DoMainLoopPhysics();
	this->DoMainLoopWindowManagerBuffering();
	this->DoMainLoopInputBuffering();
	this->DoMainLoopRender();
}

void physworld::DoMainLoopPhysics()
{
	//TODO: Step the physics world here per main loop items
	//this->Events->AddsomeEvents
}

void physworld::DoMainLoopWindowManagerBuffering()
{
    this->PreProcessSDLEvents();
    Log("WM EventCount Pending:");
    Log(SDL_WmEvents.size());
    //TODO: make Physevents for each of the events in SDL_WmEvents(and delete the SDL events)
}

void physworld::DoMainLoopInputBuffering()
{
    this->PreProcessSDLEvents();
    Log("User Input EventCount Pending:");
    Log(SDL_WmEvents.size());
	//TODO: make Physevents for each of the events in SDL_WmEvents(and delete the SDL events)
}

void physworld::DoMainLoopRender()
{
	RenderPhysWorld(this);
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
    SDL_WM_SetCaption("Catch!", NULL);

	//Start Ogre Without a native render window
	this->OgreGameWindow = this->OgreRoot->initialise(false, "physgame");

	//Configure Ogre to render to the SDL window
	Ogre::NameValuePairList *misc;
	misc=(Ogre::NameValuePairList*) GetSDLOgreBinder();
	(*misc)["title"] = Ogre::String("Catch!");
	this->OgreGameWindow = this->OgreRoot->createRenderWindow("physgame", PlayerSettings->getRenderHeight(), PlayerSettings->getRenderWidth(), PlayerSettings->getFullscreen(), misc);
    //Added following lines to attempt to make the render window visable
    //this->OgreGameWindow->setVisible(true);
	//this->OgreGameWindow->setActive(true);
    //this->OgreGameWindow->setAutoUpdated(true);

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

void physworld::DestroyRenderWindow()
{
    this->OgreGameWindow->destroy();
}

///////////////////////////////////////////////////////////////////////////////
// Deals with SDL


//This function will get all the events from SDL and Sort them into one of two Queues
void physworld::PreProcessSDLEvents()
{

    SDL_Event FromSDL;
	while(SDL_PollEvent(&FromSDL))
	{
        switch(FromSDL.type)
        {
            case SDL_ACTIVEEVENT:   //when the window gains focus
            case SDL_VIDEORESIZE:   //when the screen is resized
            case SDL_VIDEOEXPOSE:   //when the windows goes from being hidden to being shown
            case SDL_QUIT:          //when SDL closes
            case SDL_SYSWMEVENT:
                SDL_WmEvents.push(FromSDL);
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_JOYAXISMOTION:
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
            case SDL_JOYBALLMOTION:
            case SDL_JOYHATMOTION:
                SDL_UserInputEvents.push(FromSDL);
                break;
            case SDL_USEREVENT://Never thrown by SDL, but could be added by a user
            default:
                throw ("Unknown SDL Event Inserted");
                break;
        }
	}
}


#endif
