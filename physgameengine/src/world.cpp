//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _world_cpp
#define _world_cpp
///////////////////////////////////////////////////////////////////////////////
//The world that integrates everything
//
//This is where all the calls to the the underlying libraries should be from.
//World is an abstraction layer to all of the non-ui libraries. The ui
//layers wx and ogre both have their own main loops, so it is almost mandatory
//for other code to interact with those libraries directly.
///////////////////////////////////////////////////////////////////////////////
//Includes
#include "datatypes.h"
#include "eventbase.h"
#include "world.h"
#include "vector3.h"
#include "crossplatform.h"
#include "graphicsmanager.h"
#include "actorbase.h"
#include "eventuserinput.h"
#include "managerbase.h"
#include "eventmanager.h"
#include "cameramanager.h"
#include "physicsmanager.h"
#include "soundmanager.h"
#include "resourcemanager.h"
#include "scenemanager.h"
#include "uimanager.h"

#include "actorcontainervector.h"
#include "ray.h"
#include "actorrigid.h"
#include "vector3wactor.h"
#include "plane.h"
#include "camera.h"

#include <SDL.h>
#include <Ogre.h>
#include <btBulletDynamicsCommon.h>

#include <sstream>
#include <string>

using namespace std;

namespace phys
{
    World* World::TheRealWorld = 0;

    ///////////////////////////////////////////////////////////////////////////////
    // Physworld constructors
    World::World()
    {
        Vector3 Lbounds(-1000.0,-1000.0,-1000.0);
        Vector3 Ubounds(1000.0,1000.0,1000.0);
        std::vector <ManagerBase*> temp;

        this->Construct(Lbounds, Ubounds, 10, "SceneManager", SceneManager::Generic, "Physgame.log", temp);
    }

    World::World(   const Vector3 &GeographyLowerBounds_,
                    const Vector3 &GeographyUpperbounds_,
                    std::string SceneManagerName,
                    SceneManager::SceneManagerType SceneType,
                    const unsigned short int  &MaxPhysicsProxies_,
                    std::string LogFileName)
    {
        std::vector <ManagerBase*> temp;
        this->Construct(GeographyLowerBounds_,
                        GeographyUpperbounds_,
                        MaxPhysicsProxies_,
                        SceneManagerName,
                        SceneType,
                        LogFileName,
                        temp );
    }

    World::World(  const Vector3 &GeographyLowerBounds_,
            const Vector3 &GeographyUpperbounds_,
            const unsigned short int &MaxPhysicsProxies_,
            std::string SceneManagerName,
            SceneManager::SceneManagerType SceneType,
            const std::string &LogFileName,
            const std::vector <ManagerBase*> &ManagerToBeAdded)
    {
        this->Construct(GeographyLowerBounds_,
                        GeographyUpperbounds_,
                        MaxPhysicsProxies_,
                        SceneManagerName,
                        SceneType,
                        LogFileName,
                        ManagerToBeAdded );

    }

    World* World::GetWorldPointer()
    {
        assert(0!=World::TheRealWorld);
        return TheRealWorld;
    }

    void World::Construct(  const Vector3 &GeographyLowerBounds_,
                                const Vector3 &GeographyUpperbounds_,
                                const unsigned short int &MaxPhysicsProxies_,
                                std::string SceneManagerName,
                                SceneManager::SceneManagerType SceneType,
                                std::string LogFileName,
                                std::vector <ManagerBase*> ManagerToBeAdded)
    {
        //Set some sane Defaults for some values
        this->SetWindowName("AppName");
        this->TargetFrameLength=16;
        this->HasSDLBeenInitialized=false;
        this->FrameTime = 0;
        this->OgreRoot = new Ogre::Root(crossplatform::GetPluginsDotCFG(),crossplatform::GetSettingsDotCFG(),"Physgame.log");

        assert(0==World::TheRealWorld);
        World::TheRealWorld = this;

        //add each manager that was passed in to the manager list
        for(std::vector<ManagerBase*>::iterator iter = ManagerToBeAdded.begin(); iter!= ManagerToBeAdded.end(); iter++)
            { this->AddManager(*iter); }

        /// @todo With the removal of the public ogre pointers, and increased dependancy of managers on other managers as a result, a re-examining of the order managers
        /// and components are initialized is in order.
        //Create and add any managers that have not been taken care of yet.
        if(this->GetActorManager()==0)
            { this->AddManager(new ActorContainerVector()); }
        if(this->GetGraphicsManager()==0)
            { this->AddManager(new GraphicsManager()); }
        if(this->GetSoundManager()==0)
            { this->AddManager(new SoundManager()); }
        if(this->GetResourceManager()==0)
            { this->AddManager(new ResourceManager()); }
        if(this->GetEventManager()==0)
            { this->AddManager(new EventManager()); }
        if(this->GetPhysicsManager()==0)
            { this->AddManager(new PhysicsManager(GeographyLowerBounds_,GeographyUpperbounds_,MaxPhysicsProxies_)); }
        if(this->GetSceneManager()==0)
            { this->AddManager(new SceneManager(SceneManagerName, SceneType)); }
        if(this->GetUIManager()==0)
            { this->AddManager(new UIManager()); }

        // This Tests various assumptions about the way the platform works, and will not act
        SanityChecks();

    }

    void World::SanityChecks()
    {
        crossplatform::WaitMilliseconds(1500);
        //Perform a Test that only needs to be done once for the SDL/Physuserinputevent system.`
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

    void World::TestLogger()
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
        Real temp14 = 1.4;
        Whole temp15 = 15;
        String temp16("16 or so");
        const Vector3 temp17(0,1,7);
        RawEvent temp18;
            temp18.type = SDL_KEYDOWN;
            temp18.key.keysym.sym = SDLK_BACKSPACE;
        MetaCode temp19(temp18);
        btVector3 temp20(0,2,0);
        Ogre::Vector3 temp21(0,2,1);
        Ray temp22(Vector3(0,0,2),Vector3(2,0,0));
        //"temp23"
            //Just for the ActorBaseLogTests
        /*    this->AddResourceLocation(crossplatform::GetDataDirectory(), "FileSystem", "RobotTest", false);
            this->DeclareResource("robot.mesh", "Mesh", "RobotTest");
        ActorRigid temp24(1.0,"Robot24","robot.mesh","RobotTest",this); */ /// @todo TODO: remove robot and come up with another solution
        ActorRigid* temp24=0;
        Vector3WActor temp25( temp24, Vector3(0,2,5));
        Plane temp26(Vector3(2.0, 6.0, 2.0), 6.0);
        ActorBase* temp27=0;

        //dynamic_cast<PhysEvent*>// Add physevent as something that can be logged.
        /// @todo TODO add each type of event here (logtest) to make it really easy to log events
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
        OneLogTest(temp14, "Real");
        OneLogTest(temp15, "Whole");
        OneLogTest(temp16, "String");
        OneLogTest(temp17, "phys::Vector3");
        OneLogTest(temp18, "RawEvent"); /// @todo TODO Figure out How does this called the same streaming function as MetaCode ?!?!?
        OneLogTest(temp19, "MetaCode");
        //OneLogTest(temp20, "btVector3");
        OneLogTest(temp21, "Ogre::Vector3");
        OneLogTest(temp22,"Ray");
        OneLogTest("temp23","<char const*>");
        OneLogTest(temp24,"ActorRigid");
        OneLogTest(temp25,"Vector3WActor");
        OneLogTest(temp26,"Plane");
        OneLogTest(temp27,"ActorBase");
        OneLogTest('7',"const char");
    }

    template <class T> void World::OneLogTest(T Data, string DataType, string Message1, string Message2)
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
    World::~World()
    {
        //All the pointers Ogre made should get taken care of by OGRE
        delete OgreRoot;

        for(std::list<ManagerBase*>::iterator iter = this->ManagerList.begin(); iter!= ManagerList.end(); iter++)
        {
            this->ManagerList.erase(iter);
        }

        //remove sdl stuff
        SDL_FreeSurface(SDLscreen);
        SDL_Quit();
    }

    ///////////////////////////////////////////////////////////////////////////////
    //appends to the gamelog which is managed by Ogre
    template <class T> void World::Log(T Message)
    {
        static stringstream Converter;

        static std::stringstream* Audiolog = 0;
        if (0 == Audiolog)
        {
            Audiolog = this->GetSoundManager()->GetLogs();
        }else{
            Converter << Audiolog->str();
            Audiolog->str("");
        }

        Converter << this->LogStream.str() << Message;
        this->LogStream.str("");
        Ogre::LogManager::getSingleton().logMessage(Converter.str());
        Converter.str("");
    }

    template <class T> void World::LogAndThrow(T Message)
    {
        this->Log(Message);
        throw(Message);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Start the Game already
    void World::GameInit( const bool &CallMainLoop )
    {
        //#define PHYSDEBUG
        #ifdef PHYSDEBUG
        this->Log("Entering GameInit()");
        #endif

        this->LoadOgreSettings();
        #ifdef PHYSDEBUG
        this->Log("Loaded Graphics Settings");
        #endif

        this->CreateRenderWindow();
        #ifdef PHYSDEBUG
        this->Log("Created the Render Work");
        #endif

        for (std::list< ManagerBase* >::iterator Iter=this->ManagerList.begin(); Iter!=this->ManagerList.end(); ++Iter )
        {
            (*Iter)->Initialize();
            #ifdef PHYSDEBUG
            this->LogStream << "Initializing " << (*Iter)->GetTypeName() << " Manager";
            #endif
        }

        if(CallMainLoop)
        {
            #ifdef PHYSDEBUG
            this->Log("Starting the main loop");
            #endif
            this->MainLoop();
        }else{
            #ifdef PHYSDEBUG
            this->Log("Not calling the main loop");
            #endif
        }
    }

    void World::MainLoop()
    {
        /// @todo create a lighting manager and put this in there
        //this->OgreSceneManager->setAmbientLight( Ogre::ColourValue( 1, 1, 1 ) );

        /*! @page mainloop1 Main Loop Structure and Flow
         The MainLoop is heart of most video games and simulations.

         @section mainloopoverview1 Main loop Overview
         The Main loop runs in World.MainLoop() which is called by default from @ref World.GameInit(). By default this Method also starts the render, the physics andthe input systems. It does very
         little on it's own. The main loop then calls the PreMainLoopItems(), DoMainLoopItems and PreMainLoopItems(), for each manager in the order of their priority from Lowest to Highest.
         \n Here is a listing of  default priorities for each of the managers the a world intantiates by default:
            -50	User Input and events
            -40	Actors
            -30	Physics
            -20 Camera
            -10 Lighting (Not yet implemented)
              0 Graphics
             10	Sound
             20 Resources
        */

        //As long as all the CallBacks return true the game continues
        bool DoNotBreak=true;
        while (DoNotBreak)
        {
            for (std::list< ManagerBase* >::iterator Iter=this->ManagerList.begin(); Iter!=this->ManagerList.end(); ++Iter )
            {
                #ifdef PHYSDEBUG
                this->LogStream << "Current Manager: " << (*Iter)->GetTypeName() << " ";
                this->Log( (*Iter)->GetPriority() );
                #endif

                if( !(*Iter)->PreMainLoopItems() )
                    { DoNotBreak=false; }

                (*Iter)->DoMainLoopItems();

                if( !(*Iter)->PostMainLoopItems() )
                    { DoNotBreak=false; }

                this->DoMainLoopLogging();
            }

        }//End of main loop

        //Some after loop cleanup
        this->DestroyRenderWindow();
    }

    void  World::DoMainLoopLogging()
    {
        this->Log(this->LogStream.str());
        this->LogStream.str("");
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Private Functions
    ///////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////
    //Loads the Ogre Configuration
    void World::LoadOgreSettings()
    {
        //Try loading from the default location
        if (!this->OgreRoot->restoreConfig())
        {
            //if we can't do that then lets make new settings
            if (!this->GetGraphicsManager()->ShowGraphicsSettingDialog())
            {
                this->LogAndThrow("Error: Could not setup Ogre.");
            }
        }
    }

    void World::CreateRenderWindow()
    {
         //#define PHYSDEBUG

        #ifdef PHYSDEBUG
        this->Log("Entering CreateRenderWindow()");
        #endif
        /// @todo TODO set multithreaded SDL so it will the run event manager in another thread
        //Get what is needed for SDL started
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            this->Log("Error 2: Unable to init SDL, SDL Error Follows:");
            this->LogAndThrow(SDL_GetError());
        }
        this->HasSDLBeenInitialized=true;
        #ifdef PHYSDEBUG
        this->Log("Initialized SDL");
        #endif

		try
		{
			//Setup the SDL render window
			this->SDLscreen = SDL_SetVideoMode( this->GetGraphicsManager()->getRenderWidth(), this->GetGraphicsManager()->getRenderHeight(),0, SDL_OPENGL);
			SDL_WM_SetCaption(this->WindowName.c_str(), NULL);
			#ifdef PHYSDEBUG
            this->Log("Successfully Setup SDL");
            #endif
		}catch (exception& e) {
		    this->Log("Failed to Setup SDL");
			LogAndThrow(e.what());
		}

        //Start Ogre Without a native render window
        Ogre::RenderWindow* OgreGameWindow = NULL;
        try
        {
            //crossplatform::WaitMilliseconds(1000);
            OgreGameWindow = this->OgreRoot->initialise(false, this->WindowName);
            #ifdef PHYSDEBUG
            this->Log("Setup Ogre Window");
            #endif
        }catch (exception& e) {
		    this->Log("Failed to Setup Ogre Window");
			LogAndThrow(e.what());
		}

        //Configure Ogre to render to the SDL window
        Ogre::NameValuePairList *misc;
        misc=(Ogre::NameValuePairList*) crossplatform::GetSDLOgreBinder();
        (*misc)["title"] = Ogre::String(this->WindowName);
        OgreGameWindow = this->OgreRoot->createRenderWindow(WindowName, this->GetGraphicsManager()->getRenderHeight(), this->GetGraphicsManager()->getRenderWidth(), this->GetGraphicsManager()->getFullscreen(), misc);
        #ifdef PHYSDEBUG
        this->Log("Bound Ogre to an SDL window");
        #endif

        this->GetGraphicsManager()->SetOgreWindowPointer(OgreGameWindow);
        //prepare a scenemanager
        //this->OgreSceneManager = this->OgreRoot->createSceneManager(Ogre::ST_GENERIC,"SceneManager");
        #ifdef PHYSDEBUG
        this->Log("Created the Ogre Scenemanager");
        #endif

        //setup a default camera unless has been setup yet
        Camera* camera = NULL;
        if(this->GetCameraManager()==0)
        {
            this->AddManager(new CameraManager (0));
        }
        camera = this->GetCameraManager()->DefaultCamera;
        #ifdef PHYSDEBUG
        this->Log("Created Default Camera");
        #endif

        //viewport connects camera and render window
        Ogre::Viewport* OgreViewport = NULL;
        OgreViewport = this->GetGraphicsManager()->GetOgreWindowPointer()->addViewport(camera->Cam);
        this->GetCameraManager()->Viewports["DefaultViewport"] = OgreViewport;

        //setting the aspect ratio must be done after we setup the viewport
        camera->Cam->setAspectRatio( Ogre::Real(OgreViewport->getActualWidth()) / Ogre::Real(OgreViewport->getActualHeight()) );
        #ifdef PHYSDEBUG
        this->Log("Configured Viewport and Aspect Ratio");
        #endif
    }

    /// @todo Possibly move this function, along with the corresponding create function to the graphics manager as well.
    void World::DestroyRenderWindow()
    {
        this->GetGraphicsManager()->GetOgreWindowPointer()->destroy();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Simple get and Set functions
    ///////////////////////////////////////
    std::string World::GetWindowName()
    {
        return this->WindowName;
    }

    void World::SetWindowName(const String &NewName)
    {
        /// @todo TODO Change the name of an application once it is running
        WindowName = NewName;
        if(this->HasSDLBeenInitialized)
        {
            SDL_WM_SetCaption(WindowName.c_str(),WindowName.c_str());
        }
    }

    Whole World::GetTargetFrameTime()
    {
        return this->TargetFrameLength;
    }

    void World::SetTargetFrameTime(const Whole &NewTargetTime)
    {
        this->TargetFrameLength = NewTargetTime;
    }

    void World::SetTargetFrameRate(const Whole &NewFrameRate)
    {
        this->SetTargetFrameTime( 1000/NewFrameRate );
    }

    Whole World::GetFrameTime()
    {
        return this->FrameTime;
    }

    void World::SetFrameTime( const Whole &FrameTime_ )
    {
        this->FrameTime = FrameTime_;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Upper Management
    ///////////////////////////////////////
    void World::AddManager(ManagerBase* ManagerToAdd)
    {
        #ifdef PHYSDEBUG
        this->LogStream << "Calling World::AddManager("<<ManagerToAdd<<") size before:" <<this->ManagerList.size();
        #endif
        if(!this->ManagerList.empty())
        {
            for(std::list< ManagerBase* >::iterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
            {
                if( (*ManIter)->GetPriority() > ManagerToAdd->GetPriority())
                {
                    this->ManagerList.insert(ManIter, ManagerToAdd);
                    #ifdef PHYSDEBUG
                    this->LogStream << " - Added by sorted insertion:"<<ManagerToAdd<<" - size after:" <<this->ManagerList.size() ;
                    #endif
                    return;
                }
            }
        }
        ManagerList.push_back(ManagerToAdd);
        #ifdef PHYSDEBUG
        this->LogStream << " - Added by push_back:"<<ManagerToAdd<<" - size after:" <<this->ManagerList.size() << endl;
        #endif
    }

    void World::RemoveManager(ManagerBase* ManagerToRemove)
    {
        if(this->ManagerList.empty())
        {
            // do nothing
        }else{
            for(std::list< ManagerBase* >::iterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
            {
                if( *ManIter == ManagerToRemove )
                {
                    this->ManagerList.erase(ManIter);
                    break;
                }
            }
        }
    }

    void World::RemoveManager(const ManagerBase::ManagerTypeName &ManagersToRemoveType, short unsigned int WhichOne)
    {
        if(this->ManagerList.empty())
        {
            //do nothing
        }else{
            for(std::list< ManagerBase* >::iterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
            {
                if( (*ManIter)->GetType() == ManagersToRemoveType )
                {
                    if(0==WhichOne)     // we use our copy of WhichOne as a countdown to 0
                    {
                        this->ManagerList.erase(ManIter);
                    }else{
                        --WhichOne;
                    }
                }
            }
        }
    }

    ManagerBase* World::GetManager(const ManagerBase::ManagerTypeName &ManagersToGet, short unsigned int WhichOne)
    {
        #ifdef PHYSDEBUG
        this->LogStream << "Calling World::GetManager(Type:"<<ManagersToGet<<") searching through "<<this->ManagerList.size()<<" Items.";
        #endif
        if(this->ManagerList.empty())
        {
            return 0;
        }else{
            for(std::list< ManagerBase* >::iterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
            {
                if( (*ManIter)->GetType() == ManagersToGet )
                {
                    if(0==WhichOne)     // we use our copy of WhichOne as a countdown to 0
                    {
                        #ifdef PHYSDEBUG
                        this->LogStream << " - Got:" << *ManIter <<endl;
                        #endif
                        return *ManIter;
                    }else{
                        --WhichOne;
                    }
                }
            }
        }
        #ifdef PHYSDEBUG
        this->LogStream << " - Got:Nothing" << endl;
        #endif
        return 0;
    }

    void World::UpdateManagerOrder(ManagerBase* ManagerToChange, short int Priority_)
    {
        ManagerToChange->SetPriority(Priority_);
        if(this->ManagerList.empty())
        {
            ManagerList.push_back(ManagerToChange);
        }else{
            for(std::list< ManagerBase* >::iterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
            {
                if( (*ManIter)->GetPriority() > ManagerToChange->GetPriority() )
                {
                    this->ManagerList.insert(ManIter,ManagerToChange);
                }else if(*ManIter == ManagerToChange )
                {
                    this->ManagerList.erase(ManIter);
                }
            }
        }
    }

    void World::UpdateManagerOrder()
    {
        if(this->ManagerList.empty())
        {
            //do nothing
        }else{
            std::list< ManagerBase* > temp; //(this->ManagerList.begin(),this->ManagerList.end());
            temp.swap(this->ManagerList);
            for(std::list< ManagerBase* >::iterator TempIter = temp.begin(); TempIter!=temp.end(); ++TempIter )
            {
                this->AddManager(*TempIter);
            }
        }
    }

    ActorContainerBase* World::GetActorManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<ActorContainerBase*> (this->GetManager(ManagerBase::ActorContainerBase, WhichOne));
    }

    CameraManager* World::GetCameraManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<CameraManager*> (this->GetManager(ManagerBase::CameraManager, WhichOne));
    }

    EventManager* World::GetEventManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<EventManager*> (this->GetManager(ManagerBase::EventManager, WhichOne));
    }

    GraphicsManager* World::GetGraphicsManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<GraphicsManager*> (this->GetManager(ManagerBase::GraphicsManager, WhichOne));
    }

    PhysicsManager* World::GetPhysicsManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<PhysicsManager*> (this->GetManager(ManagerBase::PhysicsManager, WhichOne));
    }

    SceneManager* World::GetSceneManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<SceneManager*> (this->GetManager(ManagerBase::SceneManager, WhichOne));
    }

    SoundManager* World::GetSoundManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<SoundManager*> (this->GetManager(ManagerBase::SoundManager, WhichOne));
    }

    ResourceManager* World::GetResourceManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<ResourceManager*> (this->GetManager(ManagerBase::ResourceManager, WhichOne));
    }

    UIManager* World::GetUIManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<UIManager*> (this->GetManager(ManagerBase::UIManager, WhichOne));
    }

}
#endif
