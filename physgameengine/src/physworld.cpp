#ifndef _physworld_cpp
#define _physworld_cpp
///////////////////////////////////////////////////////////////////////////////
//The world that integrates everything
//
//This is where all the calls to the the underlying libraries should be from.
//PhysWorld is an abstraction layer to all of the non-ui libraries. The ui
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
#include "physmisc.h"

#include "SDL.h"
#include "btBulletDynamicsCommon.h"
#include <Ogre.h>

#include <sstream>
#include <string>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Physworld constructor
//this should create the basic objects for stroing and tracking the roots of
//objects in the game
PhysWorld::PhysWorld()
{
	this->Construct(
		new PhysVector3(-100.0,-100.0,-100.0),
		new PhysVector3(100.0, 100.0, 100.0),
		10
		);
}

PhysWorld::PhysWorld(PhysVector3* GeographyLowerBounds_, PhysVector3* GeographyUpperbounds_, unsigned short int  MaxPhysicsProxies_)
{
	this->Construct(GeographyLowerBounds_, GeographyUpperbounds_, MaxPhysicsProxies_);
}

void PhysWorld::Construct(PhysVector3* GeographyLowerBounds_, PhysVector3* GeographyUpperbounds_, unsigned short int  MaxPhysicsProxies_)
{
	PlayerSettings = new Settings();

	//We create our Ogre environment
	this->OgreRoot = new Ogre::Root(GetPluginsDotCFG(),GetSettingsDotCFG(),"Physgame.log");
	//Ogre resource related code
	//TODO: From here to the callbacks should probably be moved to another file
	//this->OgreRoot->addResourceLocation(GetDataDirectory(), "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false);
    this->OgreRoot->addResourceLocation(GetDataDirectory(), "FileSystem", "Robot", false);
    this->OgreResource = Ogre::ResourceGroupManager::getSingletonPtr();
    this->OgreResource->declareResource("robot.mesh", "Mesh", "Robot");
    //this->OgreResource->declareResource("robot.skeleton", "Skeleton", "Robot");
    this->OgreResource->declareResource("Examples.material", "Material", "Robot");
    //this->OgreResource->declareResource("r2skin.jpg", "Texture", "Robot");
    //this->OgreResource->initialiseResourceGroup("Robot");

    TestLogger();

    //Callbacks are the main way that a game using the PhysWorld will be able to have their code run at custom times
	this->CallBacks = new PhysWorldCallBackManager(this);

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

void PhysWorld::TestLogger()
{
    string temp0("0");
    char temp1 = 'a';
    short int temp2 = 2;
    int temp3 = 3;
    long int temp4 = 4;
    unsigned short int temp5 = 5;
    unsigned int temp6 = 6;
    unsigned long int temp7 = 7;
    bool temp8 = true;
    float temp9 = 0.9;
    double temp10 = 9.9999;
    long double temp11 = 11.00011;
    wchar_t temp12 = 'L';
    size_t temp13 = 13;
    PhysReal temp14 = 1.4;
    PhysWhole temp15 = 15;
    PhysString temp16("16 or so");
    PhysVector3 temp17(0,1,7);
    //dynamic_cast<PhysEvent*>// Add physevent as something that can be logged.
    //TODO add each type of event here to make it really wasy to log events

    OneLogTest(temp0, "string");
    OneLogTest(temp1, "char");
    OneLogTest(temp2, "short int");
    OneLogTest(temp3, "int");
    OneLogTest(temp4, "long int");
    OneLogTest(temp5, "unsigned short int");
    OneLogTest(temp6, "unsigned int");
    OneLogTest(temp7, "unsigned long int");
    OneLogTest(temp8, "bool");
    OneLogTest(temp9, "float");
    OneLogTest(temp10, "double");
    OneLogTest(temp11, "long double");
    OneLogTest(temp12, "wchar_t");
    OneLogTest(temp13, "size_t");
    OneLogTest(temp14, "PhysReal");
    OneLogTest(temp15, "PhysWhole");
    OneLogTest(temp16, "PhysString");
    OneLogTest(temp17, "PhysVector3");
}

template <class T> void PhysWorld::OneLogTest(T Data, string DataType, string Message1, string Message2)
{
    try
    {
        Log(Message1+DataType);
        LogAndThrow(Data);
    } catch (T excepted) {
        Log(Message2+DataType);
        Log(excepted);
    }
}
///////////////////////////////////////////////////////////////////////////////
//tears the world down
PhysWorld::~PhysWorld()
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
template <class T> void PhysWorld::Log(T Message)
{
	stringstream temp;
	temp << Message;
	Ogre::LogManager::getSingleton().logMessage(temp.str());
}

template <class T> void PhysWorld::LogAndThrow(T Message)
{
	this->Log(Message);
	throw(Message);
}

///////////////////////////////////////////////////////////////////////////////
//Shows the ogre settings Dialog, and allows it to save settings to ogres
//preset save location
bool PhysWorld::ShowSystemSettingDialog()
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
void PhysWorld::GameInit()
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

		//Render the frame and figure the amount of time it took //Limit frame rate to 62.5
		this->DoMainLoopRender();
		FrameTime = RenderTimer.getMilliseconds();
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

void PhysWorld::MoveCamera(PhysVector3 Position, PhysVector3 LookAt)
{
	this->OgreCamera->setPosition(Ogre::Vector3(Position.X,Position.Y,Position.Z));
    this->OgreCamera->lookAt(Ogre::Vector3(LookAt.X,LookAt.Y,LookAt.Z));
}

void PhysWorld::DoMainLoopAllItems()
{
	this->DoMainLoopPhysics();
	this->DoMainLoopWindowManagerBuffering();
	this->DoMainLoopInputBuffering();
	this->DoMainLoopRender();
}

void PhysWorld::DoMainLoopPhysics()
{
	//TODO: Step the physics world here per main loop items
	//this->Events->AddsomeEvents
}

void PhysWorld::DoMainLoopWindowManagerBuffering()
{
    this->PreProcessSDLEvents();
    Log("WM EventCount Pending:");
    Log(SDL_WmEvents.size());
    //TODO: make Physevents for each of the events in SDL_W1111mEvents(and delete the SDL events)
}

void PhysWorld::DoMainLoopInputBuffering()
{
    this->PreProcessSDLEvents();
    Log("User Input EventCount Pending:");
    Log(SDL_UserInputEvents.size());

    PhysEventUserInput FromSDLEvents;

    //TODO: make Physevents for each of the events in SDL_WmEvents(and delete the SDL events)
    //RawEvent QueueEvent;
    while( !SDL_UserInputEvents.empty() )
    {
        SDL_Event * CurrentEvent = SDL_UserInputEvents.front();
        this->Log(GetNameOfEventFrom(*CurrentEvent));

        SDL_UserInputEvents.pop();
    }//*/
}

void PhysWorld::DoMainLoopRender()
{
	RenderPhysWorld(this);
}
///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////
//Loads the Ogre Configuration
void PhysWorld::LoadOgreSettings()
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
void PhysWorld::CreateRenderWindow()
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

void PhysWorld::DestroyRenderWindow()
{
    this->OgreGameWindow->destroy();
}

///////////////////////////////////////////////////////////////////////////////
// Deals with SDL


//This function will get all the events from SDL and Sort them into one of two Queues
void PhysWorld::PreProcessSDLEvents()
{

    RawEvent FromSDL;
	while(SDL_PollEvent(&FromSDL))
	{
        switch(FromSDL.type)
        {
            case SDL_ACTIVEEVENT:   //when the window gains focus
            case SDL_VIDEORESIZE:   //when the screen is resized
            case SDL_VIDEOEXPOSE:   //when the windows goes from being hidden to being shown
            case SDL_QUIT:          //when SDL closes
            case SDL_SYSWMEVENT:
                SDL_WmEvents.push(&FromSDL);
                break;
            case SDL_KEYDOWN:
                exit(EXIT_SUCCESS); //remove this as soon as we get the X working
            case SDL_KEYUP:
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_JOYAXISMOTION:
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
            case SDL_JOYBALLMOTION:
            case SDL_JOYHATMOTION:
                SDL_UserInputEvents.push(&FromSDL);
                break;
            case SDL_USEREVENT://Never thrown by SDL, but could be added by a user
            default:
                throw ("Unknown SDL Event Inserted");
                break;
        }
	}
}


#endif
