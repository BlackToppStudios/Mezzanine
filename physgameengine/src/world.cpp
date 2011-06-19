//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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

#include "physgame.h"

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

        this->Construct(Lbounds, Ubounds, 10, SceneManager::Generic, "Physgame.log", temp);
    }

    World::World(   const Vector3 &GeographyLowerBounds_,
                    const Vector3 &GeographyUpperbounds_,
                    SceneManager::SceneManagerType SceneType,
                    const unsigned short int  &MaxPhysicsProxies_,
                    std::string LogFileName)
    {
        std::vector <ManagerBase*> temp;
        this->Construct(GeographyLowerBounds_,
                        GeographyUpperbounds_,
                        MaxPhysicsProxies_,
                        SceneType,
                        LogFileName,
                        temp );
    }

    World::World(  const Vector3 &GeographyLowerBounds_,
            const Vector3 &GeographyUpperbounds_,
            const unsigned short int &MaxPhysicsProxies_,
            SceneManager::SceneManagerType SceneType,
            const std::string &LogFileName,
            const std::vector <ManagerBase*> &ManagerToBeAdded)
    {
        this->Construct(GeographyLowerBounds_,
                        GeographyUpperbounds_,
                        MaxPhysicsProxies_,
                        SceneType,
                        LogFileName,
                        ManagerToBeAdded );

    }

    World* World::GetWorldPointer()
    {
        assert(0!=World::TheRealWorld);
        return TheRealWorld;
    }

    namespace
    {
        // Since there doesn't seem to be anyway to check if the ogre root exists yet.
        // if a you haven't made an ogre root, then Ogre::Root::getSingletonPtr(); appears to return gibberish rather that a zero.
        Ogre::Root* OgreCore = 0;
    }

    void World::Construct(  const Vector3 &GeographyLowerBounds_,
                                const Vector3 &GeographyUpperbounds_,
                                const unsigned short int &MaxPhysicsProxies_,
                                SceneManager::SceneManagerType SceneType,
                                std::string LogFileName,
                                std::vector <ManagerBase*> ManagerToBeAdded)
    {
        //Set some sane Defaults for some values
        this->TargetFrameLength=16;
        this->HasSDLBeenInitialized=false;
        this->FrameTime = 0;

        this->SetLoggingFrequency(LogOncePerFrame);


        if ( 0 == OgreCore )
            { OgreCore = new Ogre::Root(crossplatform::GetPluginsDotCFG(),crossplatform::GetSettingsDotCFG(),LogFileName); }
        else
            { OgreCore = Ogre::Root::getSingletonPtr(); }

        World::TheRealWorld = this;

        //add each manager that was passed in to the manager list
        for(std::vector<ManagerBase*>::iterator iter = ManagerToBeAdded.begin(); iter!= ManagerToBeAdded.end(); iter++)
            { this->AddManager(*iter); }

        //Create and add any managers that have not been taken care of yet.
        if(this->GetActorManager()==0)
            { this->AddManager(new ActorManager()); }
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
            { this->AddManager(new SceneManager(SceneType)); }
        if(this->GetUIManager()==0)
            { this->AddManager(new UIManager()); }
        if(this->GetTimerManager()==0)
            { this->AddManager(new TimerManager()); }

        // This Tests various assumptions about the way the platform works, and will not act
        SanityChecks();
    }

    void World::SanityChecks()
    {
        crossplatform::WaitMilliseconds(1500);
        //Perform a Test that only needs to be done once for the SDL/Physuserinputevent system.`
        Log("Verifying size of userinput events:");
        Log(sizeof(MetaCode::InputCode));
        Log(sizeof(SDL_Scancode));
        Log(sizeof(int));
        if(sizeof(MetaCode::InputCode) != sizeof(SDL_Scancode))
        {
            LogAndThrow("User input subsystem Event Sizes Don't match, userinput subsystem will go down faster than 'that' girl on prom night.");
        }else{
            Log("External User input subsystem Event Sizes match, the User Input subsystem won't crash instantly");
        }

        if(sizeof(MetaCode::InputCode) != sizeof(int))
        {
            LogAndThrow("Internal User input subsystem Event Sizes Don't match, userinput subsystem cannot function.");
        }else{
            Log("Internal User input subsystem Event Sizes match, the User Input subsystem won't crash instantly");
        }
    }

    void World::OneTimeMainLoopInit()
    {
        this->GetGraphicsManager()->ResetRenderTimer();
    }

    ///////////////////////////////////////////////////////////////////////////////
    //tears the world down
    World::~World()
    {
        for( std::list<ManagerBase*>::iterator iter = this->ManagerList.begin() ; iter!= ManagerList.end() ; /*iter++*/ )
        {
            delete (*iter);
            iter = ManagerList.erase(iter);
        }
        ManagerList.clear();

        //All the pointers Ogre made should get taken care of by OGRE
        Ogre::Root::getSingleton().shutdown();
        delete Ogre::Root::getSingletonPtr();
        OgreCore=0;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Logging

    // anonymous namespace for function pointer for logging commit mess
    namespace
    {
        Whole FrequencyCounter__=0;     // Used by some loggin functions, and interpretted differently by each
        SimpleTimer *LogTimer=0;

        void SetupXSecondTimer();

        void Never()
            {}

        void EachFrame()
        {
            #ifdef PHYSDEBUG
            static Whole LogIteration = 0;
            World::GetWorldPointer()->LogStream << "Logging - every frame, iteration: " << LogIteration;
            ++LogIteration;
            #endif
            World::GetWorldPointer()->DoMainLoopLogging();
        }

        void EachXFrame()
        {
            #ifdef PHYSDEBUG
            static Whole LogIteration = 0;
            World::GetWorldPointer()->LogStream << "Logging - per X frames, iteration: " << LogIteration;
            ++LogIteration;
            #endif
            static Whole X=0;
            if (X>FrequencyCounter__)
            {
                World::GetWorldPointer()->DoMainLoopLogging();
                X=0;
            }else{
                ++X;
            }
        }

        void EachXSeconds() //function still exists because we need it to identify what kind of logging frequency we are using.
            { }

        //helper functions and classes
        class TimerLogInX : public TimerCallback
        {
            virtual void DoCallbackItems()
            {
                #ifdef PHYSDEBUG
                static Whole LogIteration = 0;
                World::GetWorldPointer()->LogStream << "Logging - per X seconds, iteration: " << LogIteration;
                ++LogIteration;
                #endif
                World::GetWorldPointer()->DoMainLoopLogging();
                LogTimer->Reset();
                LogTimer->Start();
            }
        };

        void SetupXSecondTimer()
        {
            if (LogTimer)
            {
                World::GetWorldPointer()->GetTimerManager()->DestroyTimer(LogTimer);
                LogTimer=0;
            }
            LogTimer = World::GetWorldPointer()->GetTimerManager()->CreateSimpleTimer(Timer::StopWatch);
            LogTimer->SetInitialTime(FrequencyCounter__ * 1000000);
            LogTimer->SetGoalTime(0);
            LogTimer->Reset();
            //LogTimer->SetAutoReset(true);
            LogTimer->Start();
            LogTimer->SetCallback(new TimerLogInX);
        }

    }

    void World::SetLoggingFrequency(World::LoggingFrequency HowOften, Whole FrequencyCounter)
    {
        if (LogTimer)
        {
            World::GetWorldPointer()->GetTimerManager()->DestroyTimer(LogTimer);
            LogTimer=0;
        }
        FrequencyCounter__=FrequencyCounter;

        switch (HowOften)
        {
            case LogNever:
                LogCommitFunc=&Never;
                break;
            case LogOncePerFrame:
                LogCommitFunc=&EachFrame;
                break;
            case LogOncePerXFrames:
                LogCommitFunc=&EachXFrame;
                break;
            case LogOncePerXSeconds:
                SetupXSecondTimer();
                LogCommitFunc=&EachXSeconds;
                break;
        }
    }

    World::LoggingFrequency World::GetLoggingFrequency()
    {
        if (&Never==LogCommitFunc)
            return LogNever;

        if (&EachFrame==LogCommitFunc)
            return LogOncePerFrame;

        if (&EachXFrame==LogCommitFunc)
            return LogOncePerXFrames;

        if (&EachXSeconds==LogCommitFunc)
            return LogOncePerXSeconds;
    }

    void World::CommitLog()
        { this->DoMainLoopLogging(); }

    void World::LogString(const String& Message)
    {
        // if it is in the Audiologs then it has already happened so it needs to be logged first
        if (this->GetSoundManager()->GetLogs())
            { this->LogStream << this->GetSoundManager()->GetLogs()->str(); }
        if(Message.size()>0)
            { this->LogStream << endl << Message; }
    }

    void World::Log()
    {
        this->LogString("");
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

        //this->CreateRenderWindow();
        #ifdef PHYSDEBUG
        this->Log("Created the Render Work");
        #endif

        for (std::list< ManagerBase* >::iterator Iter=this->ManagerList.begin(); Iter!=this->ManagerList.end(); ++Iter )
        {
            this->LogStream << "Initializing " << (*Iter)->GetTypeName() << " Manager" << endl;
            if((*Iter)->GetType() != ManagerBase::GraphicsManager)
            {
                (*Iter)->Initialize();
            }
        }
        GetGraphicsManager()->Initialize();
        HasSDLBeenInitialized = GetGraphicsManager()->HasSDLBeenInitialized();

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

        Ogre::Timer* LoopTimer = new Ogre::Timer();
        this->OneTimeMainLoopInit();
        //As long as all the CallBacks return true the game continues
        bool DoNotBreak=true;
        while (DoNotBreak)
        {
            for (std::list< ManagerBase* >::iterator Iter=this->ManagerList.begin(); Iter!=this->ManagerList.end(); ++Iter )
            {
                #ifdef PHYSDEBUG
                this->LogStream << "Current Manager: " << (*Iter)->GetTypeName() << " - Priority: " << (*Iter)->GetPriority();
                #endif

                #ifdef PHYSPROFILE
                LoopTimer->reset();
                #endif

                //Actual main loop work
                if( !(*Iter)->PreMainLoopItems() )
                    { DoNotBreak=false; }

                (*Iter)->DoMainLoopItems();

                if( !(*Iter)->PostMainLoopItems() )
                    { DoNotBreak=false; }

                #ifdef PHYSPROFILE
                this->LogStream << (*Iter)->GetTypeName() << " took " << LoopTimer->getMicroseconds() << " microseconds.";
                this->Log();
                #endif

                LogCommitFunc();
            }

        }//End of main loop

        //Some after loop cleanup
        //this->DestroyRenderWindow();
        delete LoopTimer;
    }

    void World::DoMainLoopLogging()
    {
        if( this->LogStream.str().size() > 0 )
        {
            Ogre::LogManager::getSingleton().logMessage(this->LogStream.str());
            this->LogStream.str("");
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Private Functions
    ///////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////
    //Loads the Ogre Configuration
    void World::LoadOgreSettings()
    {
        //Try loading from the default location
        if (!Ogre::Root::getSingleton().restoreConfig())
        {
            //if we can't do that then lets make new settings
            if (!this->GetGraphicsManager()->ShowGraphicsSettingDialog())
            {
                this->LogAndThrow("Error: Could not setup Ogre.");
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Simple get and Set functions
    ///////////////////////////////////////

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
        //this->LogStream << "Calling World::GetManager(Type:"<<ManagersToGet<<") searching through "<<this->ManagerList.size()<<" Items.";
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
                        //this->LogStream << " - Got:" << *ManIter;
                        //this->Log();
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

    ActorManager* World::GetActorManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<ActorManager*> (this->GetManager(ManagerBase::ActorManager, WhichOne));
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

    TimerManager* World::GetTimerManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<TimerManager*> (this->GetManager(ManagerBase::TimerManager, WhichOne));
    }

    UIManager* World::GetUIManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<UIManager*> (this->GetManager(ManagerBase::UIManager, WhichOne));
    }

}
#endif
