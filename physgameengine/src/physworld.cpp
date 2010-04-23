//© Copyright 2010 Joseph Toppi and John Blackwood
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
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
#include "physactor.h"
#include "physeventuserinput.h"

#include "SDL.h"
#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include <Ogre.h>

#include <sstream>
#include <string>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Physworld constructor
//this should create the basic objects for stroing and tracking the roots of
//objects in the game
/// @todo TODO Fix the `const`ness of all methods to be as const as allowable
PhysWorld::PhysWorld()
{
	this->Construct(
		new PhysVector3(-100.0,-100.0,-100.0),
		new PhysVector3(100.0, 100.0, 100.0),
		10
		);
}

/// @todo TODO: adjust the constructors to allow for pointers to a callback manager and event manager

PhysWorld::PhysWorld(PhysVector3* GeographyLowerBounds_, PhysVector3* GeographyUpperbounds_, unsigned short int  MaxPhysicsProxies_)
{
	this->Construct(GeographyLowerBounds_, GeographyUpperbounds_, MaxPhysicsProxies_);
}

void PhysWorld::Construct(PhysVector3* GeographyLowerBounds_, PhysVector3* GeographyUpperbounds_, unsigned short int  MaxPhysicsProxies_)
{
    //Set some sane Defaults for some values
    this->SetWindowName("AppName");
    this->TargetFrameLength=16;

    this->PhysicsStepsize = btScalar(1.)/btScalar(60.);

	PlayerSettings = new Settings();

	//We create our Ogre environment
	this->OgreRoot = new Ogre::Root(GetPluginsDotCFG(),GetSettingsDotCFG(),"Physgame.log");

	//Ogre resource related code
    this->OgreResource = Ogre::ResourceGroupManager::getSingletonPtr();

    // This Tests various assumptions about the wa ythe platform works, and will not akk
    SanityChecks();

    //Callbacks are the main way that a game using the PhysWorld will be able to have their code run at custom times
	this->CallBacks = new PhysWorldCallBackManager(this);

    //Events are the main way for the game using the physworld to  get information about the various subsystems
    this->Events = new PhysEventManager(this);

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

	this->BulletDynamicsWorld = new btSoftRigidDynamicsWorld(
												BulletDispatcher,
												BulletBroadphase,
												BulletSolver,
												BulletCollisionConfiguration);
}

void PhysWorld::SanityChecks()
{
    //Perform a Test that only needs to be done once for the SDL/Physuserinputevent system.
    Log("Verifying size of userinput events:");
    Log(sizeof(MetaCode::InputCode));
    Log(sizeof(SDLKey));
    Log(sizeof(int));
    if(sizeof(MetaCode::InputCode) != sizeof(SDLKey))
    {
        LogAndThrow("User input subsystem Event Sizes  Don't match, userinput subsystem will go down faster than 'that' girl on prom night.");
    }else{
        Log("External User input subsystem Event Sizes match, the User Input subsystem won't crash instantly");
    }

    if(sizeof(MetaCode::InputCode) != sizeof(int))
    {
        LogAndThrow("Internal User input subsystem Event Sizes Don't match, userinput subsystem cannot function.");
    }else{
        Log("Internal User input subsystem Event Sizes match, the User Input subsystem won't crash instantly");
    }

    TestLogger();
}

void PhysWorld::TestLogger()
{
    Log("Testing Logger with all data types");
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
    RawEvent temp18;
        temp18.type = SDL_KEYDOWN;
        temp18.key.keysym.sym = SDLK_BACKSPACE;
    MetaCode temp19(temp18);
    //dynamic_cast<PhysEvent*>// Add physevent as something that can be logged.
    /// @todo TODO add each type of event here (logtest) to make it really wasy to log events

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
    OneLogTest(temp18, "RawEvent"); /// @todo TODO Figure out How does this calle= the same streaming function as MetaCode ?!?!?
    OneLogTest(temp19, "MetaCode");
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
	delete this->BulletDynamicsWorld; /// @todo TODO Fix the error when deleting the bullets worlds

	//All the pointers Ogre made should get taken care of by OGRE
	delete OgreRoot;

	//clear up our objects
	delete CallBacks;
	delete Events;

	delete PlayerSettings;

	//remove sdl stuff

	SDL_FreeSurface(SDLscreen);
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
void PhysWorld::GameInit( bool CallMainLoop )
{
	//try to load the ogre config
	this->LoadOgreSettings();
	this->CreateRenderWindow();

    if(CallMainLoop)
    {
        MainLoop();
    }
}

void PhysWorld::MainLoop()
{
    //Create a the RenderTimer, which will be used to measure the time
    Ogre::Timer RenderTimer;

	bool Callbackbools[] = {true, true, true, true, true, true};

	//Used for tracking times to prevent Infinite render loops in graphically simple games
	//PhysWhole Times[] = {0,0,0,0};

	/// @todo TODO finish test code, there is a sloppy line of test code for the robot in the main loop
	this->OgreSceneManager->setAmbientLight( Ogre::ColourValue( 1, 1, 1 ) );

    PhysWhole FrameDelay = 0;
    PhysWhole FrameTime = 0;

    /*! @page mainloop1 Main Loop Structure and Flow
     The MainLoop is heart of most vidoe games and simulations.

     @section overview1 Main loop Overview
	 The Main loop runs in PhysWorld.MainLoop() which is called by default from @ref PhysWorld.GameInit(). By default this Method also starts the render, the physics andthe input systems. It does very little
	 on it's own. It uses the default callback manager (which you can swap out if you want), which is the callback manager point to by PhysWorld::CallBacks . Once started it runs the
	 callbacks present in the callback manager until one of them returns false. It has a pretty specific work flow. Starting with input, then physics, then rendering. There is a callback
	 before and after each of these tasks.
     \n \n The user input task is never started if there is no Pre or Post callback set. It won't crash or throw any weird error condtions, it was specifically designed this way. This
     allows the input buffering to be turned off by simply removing the callbacks. This might not sound like much, but if you simply don't need input this can be a little performance bump.
     It sounds even better when we say it works the same way for physics. If you do not have a pre or post physic callback set physics simply is not run. This allows for easy control over
     the state of motion  in a world. Currently Rendering is performed each iteration of the main loop, regardless of what Callbacks are set. This may be changed at some point in the future,
     but no immediate plans to change it are in the works.
     \n \n One iteration of the main loops is about 1/60 of a second and will render 1 frame and step physics about 1/60 (1/62.5 usually) of a second. Currently there is no way to uncap
     the framerate, we did not see a need for more (we may change this in the future). Situations with low performance should automatically be handled but increasing the size of physics
     steps and automatically not rendering frames when not appropriate. There will be fewer calls on callbacks, but the engine will not inhibit gameplay below the the target framerate.
    \n
     @section callbacks1 1) Input Callbacks and Buffering
     User input is the first task performed each main loop. This was done because in most games and simulations everything depends on the user input.
     @subsection input1 1.A) PreInputCallback
     This can be set using the function in PhysWorldCallBackManager that have "PreInput" in their names. This is a great place to to do begining of loop items, like gathering data for
     computer controlled characters, checking if game goals are met, and other items that needs to be done it iteration of the game loop and don't directly relate to physics or Rendering.
     @subsection input2 1.B) Input Buffering
     During this step Input events are gathered fromt the input Subsystem (which could be any of a number of user input libraries), which are ultimately made by the Operating System and
     processed into PhysEventUserInput objects, and placed in the Default event manager ( check the pointer PhysWorld::Events ).
     @subsection input3 1.C) PostInputCallback
     This callback is your first chance to access the userinput Events and try to use them. This callback can be managed by using methods in PhysWorldCallBackManager that have "PostInput"
     in their names. The functionality of this Callback slightly overlaps with the Prephysics Callbacks due to the timing of using them both. If both are used, one will be called
     immediately after the other.
    \n
     @section callbacks2 2) Physics callbacks and Event Buffering
     This step runs between user input and rendering to allow for changes caused by user input to be immediately visible to the user.
     @subsection physics1 2.A) PrePhysicsCallback
     The funtionality of this callback overlaps a great deal with the PostInputCallBack, but they both still exist to make organization easier when needed. You probably should have figured
     out by now how the Callbackmanager functions are named, the ones for this are ituitively named with "PrePhysics" in their name.
     @subsection physics2 2.B) Physics calculations and Event Buffering
     Physics will be run here, but currently is not fully implemented. But eventually physics event will be generated here. Including a system for checking for certain types of collisions.
     @subsection physics3 2.C) PostPhysicsCallback
     This Callback is you first chance to work with the physics events just generated. This is a good place to check if you want items to adjust hitpoints, explode, die or take some other
     action based on physics.
    \n
     @section callbacks3 3) Rendering
     Unlike Input and Physics this rendering will be performed each Frame regardless of what callbacks are set. Rendering is done
     last so that any changes to the game world physics makes are made visible in the same loop iteration.
     @subsection rendering1 3.A) PerRenderCallback
     Any tasks that could not be complete in the PostPhysics Callback can be performed here or you use this to make last minute
     preparations for rendering.
     @subsection rendering2 3.B) Rendering
     This is where the rendering actually occurs. The state of all the items in the world is grabbed from the physics subsystem
     and rendered. Additionally this is where the \ref FrameDelay is calculated and acted on, so the frame rate can stay consistent.
     @subsection rendering3 3.C) PostRenderCallback
     This is a great place to switch out the Callback Manager, for accomplishing tasks like switching between in game menus and
     actual gameplay. This is also a decent place to get a head start on anything that would be run in the PreInputCallback.
    \n
     @section endingmainloop1 Ending the Main Loop
     The main loop stores the return values of each of the 6 callbacks. If they are all true the mainloop continues to it's
     next iteration. If one or more of any of the callbacks returns false then the main loop ends, and the game engine exits
     gracefully, cleanly up the physics, graphics and any others subsystems it opened.
     No attempt will be made by the engine to clean up data in the callback manager. If the game code does not free these items
     the operating system had better do it automatically. On most PC platform this is not an issue, but it could be disastrous
     on a mobile, or underperforming gaming platform.
	*/
	//As long as all the CallBacks return true the game continues
	while (Callbackbools[0] && Callbackbools[1] && Callbackbools[2] && Callbackbools[3] && Callbackbools[4] && Callbackbools[5])
	{
        //Input buffering Callbacks
        if( this->CallBacks->IsPreInputCallbackSet() || this->CallBacks->IsPostInputCallbackSet() )
        {
     		if( this->CallBacks->IsPreInputCallbackSet() )
                { Callbackbools[0] = this->CallBacks->PreInput(); }
			this->DoMainLoopInputBuffering();
			if( this->CallBacks->IsPostInputCallbackSet() )
                { Callbackbools[1] = this->CallBacks->PostInput(); }
        }

        //Physics & Physics Callbacks
		if( this->CallBacks->IsPrePhysicsCallbackSet() || this->CallBacks->IsPostPhysicsCallbackSet() )
        {
            if( this->CallBacks->IsPrePhysicsCallbackSet() )
                { Callbackbools[2] = this->CallBacks->PrePhysics(); }
			this->DoMainLoopPhysics(FrameTime);
            if( this->CallBacks->IsPostPhysicsCallbackSet() )
                { Callbackbools[3] = this->CallBacks->PostPhysics(); }
        }

        //PreRender callback
		if(this->CallBacks->IsPreRenderCallbackSet())
        {
            Callbackbools[4] = this->CallBacks->PreRender();
			this->DoMainLoopWindowManagerBuffering();
        }

		//Render the frame and figure the amount of time it took //By default Limit frame rate to 62.5
		this->DoMainLoopRender();
		FrameTime = RenderTimer.getMilliseconds();
		RenderTimer.reset();
		if(this->TargetFrameLength>FrameTime){
		    FrameDelay++;
		}else if(this->TargetFrameLength==FrameTime){
        }else{
            if (0<FrameDelay){
                FrameDelay--;
            }else{
                FrameDelay=0;
            }
        }
        WaitMilliseconds( FrameDelay );

        //PostRender Callback
		if(this->CallBacks->IsPostRenderCallbackSet())
        {
			this->Events->AddEvent(new PhysEventRenderTime(FrameTime));
			Callbackbools[5] = this->CallBacks->PostRender();
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

void PhysWorld::DoMainLoopAllItems(PhysReal PreviousFrameTime)
{
	this->DoMainLoopPhysics(PreviousFrameTime);
	this->DoMainLoopWindowManagerBuffering();
	this->DoMainLoopInputBuffering();
	this->DoMainLoopRender();
}

void PhysWorld::DoMainLoopPhysics(PhysReal TimeElapsed)
{
    PhysReal FloatTime = TimeElapsed;
    FloatTime *= 0.0001;    //Convert from MilliSeconds to Seconds

    PhysReal IdealStep = this->TargetFrameLength;
    IdealStep *= 0.0001;

    //int MaxSteps = (FloatTime<IdealStep) ? 1 : int(FloatTime/IdealStep+1);
    int MaxSteps = (FloatTime<IdealStep) ? 1 : int(FloatTime/IdealStep+2);  //used 2 simply to be extra safe
    this->BulletDynamicsWorld->applyGravity();
    this->BulletDynamicsWorld->stepSimulation( FloatTime, MaxSteps, IdealStep);
    this->BulletDynamicsWorld->clearForces();
    //this->BulletDynamicsWorld->applyGravity();
    //this->BulletDynamicsWorld->synchronizeMotionStates();
    Log("Updated Physics");
}

void PhysWorld::DoMainLoopWindowManagerBuffering()
{
    this->Events->UpdateSystemEvents();
}

void PhysWorld::DoMainLoopInputBuffering()
{
    this->Events->UpdateUserInputEvents();
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
    /// @todo TODO multithreaded SDL willthe run event manager in another thread
	//Get what is needed for SDL started
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		this->Log("Error 2: Unable to init SDL, SDL Error Follows:");
		this->LogAndThrow(SDL_GetError());
	}

	//Setup the SDL render window
	this->SDLscreen = SDL_SetVideoMode(PlayerSettings->getRenderHeight(), PlayerSettings->getRenderWidth(), 0, SDL_OPENGL);
    SDL_WM_SetCaption(this->WindowName.c_str(), NULL);

	//Start Ogre Without a native render window
	this->OgreGameWindow = this->OgreRoot->initialise(false, "physgame");

	//Configure Ogre to render to the SDL window
	Ogre::NameValuePairList *misc;
	misc=(Ogre::NameValuePairList*) GetSDLOgreBinder();
	(*misc)["title"] = Ogre::String(this->WindowName);
	this->OgreGameWindow = this->OgreRoot->createRenderWindow("physgame", PlayerSettings->getRenderHeight(), PlayerSettings->getRenderWidth(), PlayerSettings->getFullscreen(), misc);
    //Added following lines to attempt to make the render window visible
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
// Bullet Related Public Members
///////////////////////////////////////

void PhysWorld::AddActor(ActorBase* ActorToAdd)
{
    ActorToAdd->AddObjectToWorld(this, this->BulletDynamicsWorld);
}


///////////////////////////////////////////////////////////////////////////////
// Simple get and Set functions
///////////////////////////////////////
std::string PhysWorld::GetWindowName()
{
    return this->WindowName;
}

void PhysWorld::SetWindowName(std::string NewName)
{
    /// @todo TODO Change the name of an application once it is running
    WindowName = NewName;
}

PhysWhole PhysWorld::GetTargetFrameTime()
{
    return this->TargetFrameLength;
}

void PhysWorld::SetTargetFrameTime(PhysWhole NewTargetTime)
{
    this->TargetFrameLength = NewTargetTime;
}

void PhysWorld::SetTargetFrameRate(PhysWhole NewFrameRate)
{
    this->SetTargetFrameTime( 1000/NewFrameRate );
}

///////////////////////////////////////////////////////////////////////////////
// Ogre Resource Related Public Members
///////////////////////////////////////

void PhysWorld::AddResourceLocation(PhysString Location, PhysString Type, PhysString Group, bool recursive=false)
{
    this->OgreRoot->addResourceLocation(Location, Type, Group, recursive);
}

void PhysWorld::DeclareResource(PhysString Name, PhysString Type, PhysString Group)
{
    this->OgreResource->declareResource(Name, Type, Group);
}

void PhysWorld::InitResourceGroup(PhysString Group)
{
    this->OgreResource->initialiseResourceGroup(Group);
}

///////////////////////////////////////////////////////////////////////////////
// Bullet World Management Functions
///////////////////////////////////////

void PhysWorld::SetGravity(PhysVector3 pgrav)
{
    this->BulletDynamicsWorld->setGravity(pgrav.GetBulletVector3());
}

void PhysWorld::ApplyGravity()
{
    this->BulletDynamicsWorld->applyGravity();
}

#endif
