//© Copyright 2010 - 2012 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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
#ifndef _entresol_cpp
#define _entresol_cpp
///////////////////////////////////////////////////////////////////////////////
//The Entresol that integrates everything
//
//This is where all the calls to the the underlying libraries should be from.
//Entresol is an abstraction layer to all of the non-ui libraries. The ui
//layers wx and ogre both have their own main loops, so it is almost mandatory
//for other code to interact with those libraries directly.
///////////////////////////////////////////////////////////////////////////////
//Includes

#include "mezzanine.h"

#include <SDL.h>
#include <Ogre.h>
#include <btBulletDynamicsCommon.h>

#include <sstream>
#include <string>

using namespace std;

namespace Mezzanine
{
    template<> Entresol* Singleton<Entresol>::SingletonPtr = 0;

    ///////////////////////////////////////////////////////////////////////////////
    // Mezzanine constructors
    Entresol::Entresol()
    {
        PhysicsConstructionInfo PhysicsInfo;
        std::vector <ManagerBase*> temp;

        this->Construct(PhysicsInfo,"DefaultSceneManager","plugins.cfg",".","Mezzanine.log",temp);
    }

#ifdef MEZZXML
    Entresol::Entresol(const String& EngineDataPath, const String& ArchiveType, const String& InitializerFile)
    {
        if(String::npos != InitializerFile.find(".mxi")) ConstructFromXML(EngineDataPath,ArchiveType,InitializerFile);
        //else if(String::npos == InitializerFile.find(".mxi")) ConstructFromText(EngineDataPath,InitializerFile);
        else { MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Attempting to initialze Mezzanine from an unsupported file type."); }
    }

    Entresol::Entresol(std::vector<ManagerFactory*>& CustomFactories, const String& EngineDataPath, const String& ArchiveType, const String& InitializerFile)
    {
        for( std::vector<ManagerFactory*>::iterator it = CustomFactories.begin() ; it != CustomFactories.end() ; ++it )
        {
            AddManagerFactory( (*it) );
        }

        if(String::npos != InitializerFile.find(".mxi")) ConstructFromXML(EngineDataPath,ArchiveType,InitializerFile);
        //else if(String::npos == InitializerFile.find(".mxi")) ConstructFromText(EngineDataPath,InitializerFile);
        else { MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Attempting to initialze Mezzanine from an unsupported file type."); }
    }
#endif

    Entresol::Entresol(   const PhysicsConstructionInfo& PhysicsInfo,
                    const String& SceneType,
                    const String& PluginsFileName,
                    const String& EngineDataPath,
                    const String& LogFileName)
    {
        std::vector <ManagerBase*> temp;
        this->Construct(PhysicsInfo,
                        SceneType,
                        PluginsFileName,
                        EngineDataPath,
                        LogFileName,
                        temp );
    }

    Entresol::Entresol(  const PhysicsConstructionInfo& PhysicsInfo,
                    const String& SceneType,
                    const String& PluginsFileName,
                    const String& EngineDataPath,
                    const String& LogFileName,
                    const std::vector <ManagerBase*> &ManagerToBeAdded)
    {
        this->Construct(PhysicsInfo,
                        SceneType,
                        PluginsFileName,
                        EngineDataPath,
                        LogFileName,
                        ManagerToBeAdded );

    }

    namespace
    {
        // Since there doesn't seem to be anyway to check if the ogre root exists yet.
        // if a you haven't made an ogre root, then Ogre::Root::getSingletonPtr(); appears to return gibberish rather that a zero.
        Ogre::Root* OgreCore = 0;
    }

    void Entresol::Construct(  const PhysicsConstructionInfo& PhysicsInfo,
                                const String& SceneType,
                                const String& PluginsFileName,
                                const String& EngineDataPath,
                                const String& LogFileName,
                                std::vector <ManagerBase*> ManagerToBeAdded)
    {
        //Add default manager factories
        AddAllEngineDefaultManagerFactories();
        //Set some sane Defaults for some values
        this->TargetFrameLength = 16;
        this->FrameTime = 0;
        this->ManualLoopBreak = false;

        this->SetLoggingFrequency(LogOncePerFrame);


        if ( 0 == OgreCore )
            { OgreCore = new Ogre::Root(EngineDataPath+PluginsFileName,"",LogFileName); }
        else
            { OgreCore = Ogre::Root::getSingletonPtr(); }

        //add each manager that was passed in to the manager list
        for(std::vector<ManagerBase*>::iterator iter = ManagerToBeAdded.begin(); iter!= ManagerToBeAdded.end(); iter++)
            { this->AddManager(*iter); }

        //Create and add any managers that have not been taken care of yet.
        if(this->GetActorManager()==0)
            { this->AddManager(new ActorManager()); }
        if(this->GetResourceManager()==0)
            { this->AddManager(new ResourceManager(EngineDataPath)); }
        if(this->GetGraphicsManager()==0)
            { this->AddManager(new GraphicsManager()); }
        if(this->GetAudioManager()==0)
            { this->AddManager(new AudioManager()); }
        if(this->GetEventManager()==0)
            { this->AddManager(new EventManager()); }
        if(this->GetInputManager()==0)
            { this->AddManager(new InputManager()); }
        if(this->GetPhysicsManager()==0)
            { this->AddManager(new PhysicsManager(PhysicsInfo)); }
        if(this->GetSceneManager()==0)
            { this->AddManager(new SceneManager(SceneType)); }
        if(this->GetUIManager()==0)
            { this->AddManager(new UIManager()); }
        if(this->GetTimerManager()==0)
            { this->AddManager(new TimerManager()); }
        if(this->GetMeshManager()==0)
            { this->AddManager(new MeshManager()); }
        if(this->GetCollisionShapeManager()==0)
            { this->AddManager(new CollisionShapeManager()); }
        if(this->GetCameraManager()==0)
            { this->AddManager(new CameraManager()); }

        // This Tests various assumptions about the way the platform works, and will not act
        SanityChecks();
    }

#ifdef MEZZXML
    void Entresol::ConstructFromXML(const String& EngineDataPath, const String& ArchiveType, const String& InitializerFile)
    {
        //Add default manager factories
        AddAllEngineDefaultManagerFactories();
        //Set some sane Defaults for some values.
        this->TargetFrameLength = 16;
        this->FrameTime = 0;
        this->ManualLoopBreak = false;
        this->SetLoggingFrequency(LogNever);

        // Create Ogre.
        Ogre::LogManager* OgreLogs = Ogre::LogManager::getSingletonPtr();
        if( NULL == OgreLogs ) OgreLogs = new Ogre::LogManager();
        if( NULL == OgreCore ) OgreCore = new Ogre::Root("","","");
        else OgreCore = Ogre::Root::getSingletonPtr();

        // Set up the data we'll be populating.
        XML::Attribute CurrAttrib;
        String GUIInit, ResourceInit, PluginsInit, LogFileName;
        String PluginExtension, PluginPath;

        // Create or set the resource manager.
        /// @todo This currently forces our default resource manager to be constructed, which isn't in line with our factory/initiailzation design.
        /// This should be addressed somehow.
        if(ResourceManager::SingletonValid()) AddManager(ResourceManager::GetSingletonPtr());
        else AddManager(new ResourceManager(EngineDataPath,ArchiveType));

        // Open and load the initializer doc.
        ResourceManager* ResourceMan = GetResourceManager();
        Resource::FileStreamDataStream InitStream(InitializerFile,EngineDataPath);
        XML::Document InitDoc;
        InitDoc.Load(InitStream);

        // Get the world settings and set them.
        XML::Node WorldSettings = InitDoc.GetChild("WorldSettings");
        for( XML::NodeIterator SetIt = WorldSettings.begin() ; SetIt != WorldSettings.end() ; ++SetIt )
        {
            String SecName = (*SetIt).Name();
            if( "FrameSettings" == SecName )
            {
                CurrAttrib = (*SetIt).GetAttribute("TargetFrameRate");
                if(CurrAttrib.Empty())
                {
                    CurrAttrib = (*SetIt).GetAttribute("TargetFrameTime");
                    if(!CurrAttrib.Empty())
                        SetTargetFrameTime(CurrAttrib.AsWhole());
                }else{
                    this->SetTargetFrameRate(CurrAttrib.AsWhole());
                }
            }
            else if( "LoggingSettings" == SecName )
            {
                XML::Attribute Frequency = (*SetIt).GetAttribute("Frequency");
                if(!Frequency.Empty())
                {
                    String FrequencyStr = Frequency.AsString();
                    StringTool::ToLowerCase(FrequencyStr);
                    if( "perframe" == FrequencyStr )
                    {
                        this->SetLoggingFrequency(LogOncePerFrame);
                    }
                    else if( "perxframes" == FrequencyStr )
                    {
                        CurrAttrib = (*SetIt).GetAttribute("Counter");
                        this->SetLoggingFrequency(LogOncePerXFrames,CurrAttrib.Empty() ? 5 : CurrAttrib.AsWhole() );
                    }
                    else if( "perxseconds" == FrequencyStr )
                    {
                        CurrAttrib = (*SetIt).GetAttribute("Counter");
                        this->SetLoggingFrequency(LogOncePerXSeconds,CurrAttrib.Empty() ? 5 : CurrAttrib.AsWhole() );
                    }
                }

                XML::Node LogFile = (*SetIt).GetChild("LogFile");
                if(!LogFile.Empty())
                {
                    CurrAttrib = LogFile.GetAttribute("FileName");
                    if(!CurrAttrib.Empty())
                        LogFileName = CurrAttrib.AsString();
                }
            }
        }

        // Setup the log.
        if(!LogFileName.empty()) OgreLogs->createLog(LogFileName,true,true);
        else OgreLogs->createLog("Mezzanine.log",true,true);

        // Get the other initializer files we'll be using, since we'll need the plugins initializer.
        XML::Node InitFiles = InitDoc.GetChild("OtherInitializers");
        for( XML::NodeIterator InitIt = InitFiles.begin() ; InitIt != InitFiles.end() ; ++InitIt )
        {
            String InitFileName = (*InitIt).Name();
            if( "PluginInit" == InitFileName )
            {
                CurrAttrib = (*InitIt).GetAttribute("FileName");
                if(!CurrAttrib.Empty())
                    PluginsInit = CurrAttrib.AsString();
            }
            else if( "ResourceInit" == InitFileName )
            {
                CurrAttrib = (*InitIt).GetAttribute("FileName");
                if(!CurrAttrib.Empty())
                    ResourceInit = CurrAttrib.AsString();
            }
            else if( "GUIInit" == InitFileName )
            {
                CurrAttrib = (*InitIt).GetAttribute("FileName");
                if(!CurrAttrib.Empty())
                    GUIInit = CurrAttrib.AsString();
            }
        }

        // Load the necessary plugins.
        if(!PluginsInit.empty())
        {
            PluginExtension = ResourceMan->GetPluginExtension();
            Resource::FileStreamDataStream PluginStream(PluginsInit,EngineDataPath);
            XML::Document PluginDoc;
            PluginDoc.Load(PluginStream);
            // Get the plugin path, if it's there.
            XML::Node PlgPath = PluginDoc.GetChild("PluginPath");
            if(!PlgPath.Empty())
            {
                CurrAttrib = PlgPath.GetAttribute("Path");
                if(!CurrAttrib.Empty())
                    PluginPath = CurrAttrib.AsString();
            }else PluginPath = ".";
            XML::Node OgrePlugins = PluginDoc.GetChild("OgrePlugins");
            for( XML::NodeIterator OPlgIt = OgrePlugins.begin() ; OPlgIt != OgrePlugins.end() ; ++OPlgIt )
            {
                CurrAttrib = (*OPlgIt).GetAttribute("FileName");
                if(!CurrAttrib.Empty())
                    OgreCore->loadPlugin(PluginPath + (CurrAttrib.AsString()) + PluginExtension);
            }
            XML::Node Plugins = PluginDoc.GetChild("Plugins");
            for( XML::NodeIterator PlgIt = Plugins.begin() ; PlgIt != Plugins.end() ; ++PlgIt )
            {
                /// @todo This is currently not supported as we have no plugins of our own.  This should be implemented if that changes.
            }
        }

        // Load additional resource groups
        /*if(!ResourceInit.empty())
        {
            Resource::FileStreamDataStream ResourceStream(ResourceInit,EngineDataPath);
            XML::Document ResourceDoc;
            ResourceDoc.Load(ResourceStream);
            // Get an iterator to the first resource group node, and declare them all.
            XML::Node ResourceLocations = ResourceDoc.GetChild("ResourceLocations");
            for( XML::NodeIterator GroupIt = ResourceLocations.begin() ; GroupIt != ResourceLocations.end() ; ++GroupIt )
            {
                String GroupName, GroupType, GroupPath;
                bool GroupRecursive = false;
                // Get the group path
                CurrAttrib = (*GroupIt).GetAttribute("GroupPath");
                if(!CurrAttrib.Empty())
                    GroupPath = CurrAttrib.AsString();
                // Get the group type
                CurrAttrib = (*GroupIt).GetAttribute("GroupType");
                if(!CurrAttrib.Empty())
                    GroupType = CurrAttrib.AsString();
                // Get the group name
                CurrAttrib = (*GroupIt).GetAttribute("GroupName");
                if(!CurrAttrib.Empty())
                    GroupName = CurrAttrib.AsString();
                // Get whether this is recursive
                CurrAttrib = (*GroupIt).GetAttribute("Recursive");
                if(!CurrAttrib.Empty())
                    GroupRecursive = StringTool::ConvertToBool(CurrAttrib.AsString());
                // Finally create the resource location.
                ResourceMan->AddAssetLocation(GroupPath,GroupType,GroupName,GroupRecursive);
            }
            // Get what resource groups should be initialized.
            XML::Node InitGroups = ResourceDoc.GetChild("InitGroups");
            for( XML::NodeIterator InitIt = InitGroups.begin() ; InitIt != InitGroups.end() ; ++InitIt )
            {
                String GroupName;
                CurrAttrib = (*InitIt).GetAttribute("GroupName");
                if(!CurrAttrib.Empty())
                    GroupName = CurrAttrib.AsString();
                ResourceMan->InitAssetGroup(GroupName);
            }
        }//*/

        // Create the requested managers and set their necessary values.
        XML::Node Managers = InitDoc.GetChild("Managers");
        for( XML::NodeIterator ManIt = Managers.begin() ; ManIt != Managers.end() ; ++ManIt )
        {
            CreateManager( (*ManIt).Name(), (*ManIt) );
        }

        // Load additional resource groups
        if(!ResourceInit.empty())
        {
            Resource::FileStreamDataStream ResourceStream(ResourceInit,EngineDataPath);
            XML::Document ResourceDoc;
            ResourceDoc.Load(ResourceStream);
            // Get an iterator to the first resource group node, and declare them all.
            XML::Node ResourceLocations = ResourceDoc.GetChild("ResourceLocations");
            for( XML::NodeIterator GroupIt = ResourceLocations.begin() ; GroupIt != ResourceLocations.end() ; ++GroupIt )
            {
                String GroupName, GroupType, GroupPath;
                bool GroupRecursive = false;
                // Get the group path
                CurrAttrib = (*GroupIt).GetAttribute("GroupPath");
                if(!CurrAttrib.Empty())
                    GroupPath = CurrAttrib.AsString();
                // Get the group type
                CurrAttrib = (*GroupIt).GetAttribute("GroupType");
                if(!CurrAttrib.Empty())
                    GroupType = CurrAttrib.AsString();
                // Get the group name
                CurrAttrib = (*GroupIt).GetAttribute("GroupName");
                if(!CurrAttrib.Empty())
                    GroupName = CurrAttrib.AsString();
                // Get whether this is recursive
                CurrAttrib = (*GroupIt).GetAttribute("Recursive");
                if(!CurrAttrib.Empty())
                    GroupRecursive = StringTool::ConvertToBool(CurrAttrib.AsString());
                // Finally create the resource location.
                ResourceMan->AddAssetLocation(GroupPath,GroupType,GroupName,GroupRecursive);
            }
            // Get what resource groups should be initialized.
            XML::Node InitGroups = ResourceDoc.GetChild("InitGroups");
            for( XML::NodeIterator InitIt = InitGroups.begin() ; InitIt != InitGroups.end() ; ++InitIt )
            {
                String GroupName;
                CurrAttrib = (*InitIt).GetAttribute("GroupName");
                if(!CurrAttrib.Empty())
                    GroupName = CurrAttrib.AsString();
                ResourceMan->InitAssetGroup(GroupName);
            }
        }

        // Configure the UI
        if(!GUIInit.empty())
        {
            /// @todo This is currently not implemented.
        }

        SanityChecks();
    }
#endif

    void Entresol::SanityChecks()
    {
        crossplatform::WaitMilliseconds(1500);
        //Perform a Test that only needs to be done once for the SDL/userinputevent system.`
        Log("Verifying size of userinput events:");
        Log(sizeof(Input::InputCode));
        Log(sizeof(SDL_Scancode));
        Log(sizeof(int));
        if(sizeof(Input::InputCode) != sizeof(SDL_Scancode))
        {
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"User input subsystem Event Sizes Don't match, userinput subsystem will go down faster than 'that' girl on prom night.");
        }else{
            Log("External User input subsystem Event Sizes match, the User Input subsystem won't crash instantly");
        }

        if(sizeof(Input::InputCode) != sizeof(int))
        {
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Internal User input subsystem Event Sizes Don't match, userinput subsystem cannot function.");
        }else{
            Log("Internal User input subsystem Event Sizes match, the User Input subsystem won't crash instantly");
        }
    }

    void Entresol::OneTimeMainLoopInit()
    {
        VerifyManagerInitializations();
        GraphicsManager::GetSingletonPtr()->ResetRenderTimer();
    }

    bool Entresol::VerifyManagerInitializations()
    {
        std::vector<String> ManagerNames;
        for (std::list< ManagerBase* >::iterator Iter=this->ManagerList.begin(); Iter!=this->ManagerList.end(); ++Iter )
        {
            if(!(*Iter)->IsInitialized())
            {
                ManagerNames.push_back( (*Iter)->GetInterfaceTypeAsString() );
            }
        }

        if(ManagerNames.empty())
        {
            return true;
        }else{
            StringStream ExceptionStream;
            if(1 == ManagerNames.size())
            {
                ExceptionStream << "Manager: ";
                ExceptionStream << ManagerNames.at(0);
                ExceptionStream << "is not initialized.  All managers need to be initiailzed when entering the main loop.";
            }else{
                ExceptionStream << "Managers: ";
                for( std::vector<String>::iterator Iter = ManagerNames.begin() ; Iter != ManagerNames.end() ; ++Iter )
                {
                    ExceptionStream << (*Iter) << ", ";
                }
                ExceptionStream << "are not initialized.  All managers need to be initiailzed when entering the main loop.";
            }
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,ExceptionStream.str());
            return false;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    //tears the world down
    Entresol::~Entresol()
    {
        DestroyAllManagers();
        DestroyAllManagerFactories();

        SDL_Quit();

        Ogre::Root::getSingleton().shutdown();
        delete Ogre::Root::getSingletonPtr(); // This should be done by the shutdown method shouldn't it?
        OgreCore=0;

    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void Entresol::PauseWorld(bool Pause)
    {
        PhysicsManager::GetSingletonPtr()->PauseSimulation(Pause);
        SceneManager::GetSingletonPtr()->PauseAllParticles(Pause);
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
        {
            Entresol::GetSingletonPtr()->LogStream.str("");
        }

        void EachFrame()
        {
            #ifdef MEZZDEBUG
            static Whole LogIteration = 0;
            Entresol::GetSingletonPtr()->LogStream << "Logging - every frame, iteration: " << LogIteration;
            ++LogIteration;
            #endif
            Entresol::GetSingletonPtr()->DoMainLoopLogging();
        }

        void EachXFrame()
        {
            #ifdef MEZZDEBUG
            static Whole LogIteration = 0;
            Entresol::GetSingletonPtr()->LogStream << "Logging - per X frames, iteration: " << LogIteration;
            ++LogIteration;
            #endif
            static Whole X=0;
            if (X>FrequencyCounter__)
            {
                Entresol::GetSingletonPtr()->DoMainLoopLogging();
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
                #ifdef MEZZDEBUG
                static Whole LogIteration = 0;
                Entresol::GetSingletonPtr()->LogStream << "Logging - per X seconds, iteration: " << LogIteration;
                ++LogIteration;
                #endif
                Entresol::GetSingletonPtr()->DoMainLoopLogging();
                LogTimer->Reset();
                LogTimer->Start();
            }
        };

        void SetupXSecondTimer()
        {
            if (LogTimer)
            {
                TimerManager::GetSingletonPtr()->DestroyTimer(LogTimer);
                LogTimer=0;
            }
            LogTimer = TimerManager::GetSingletonPtr()->CreateSimpleTimer(Timer::StopWatch);
            LogTimer->SetInitialTime(FrequencyCounter__ * 1000000);
            LogTimer->SetGoalTime(0);
            LogTimer->Reset();
            //LogTimer->SetAutoReset(true);
            LogTimer->Start();
            LogTimer->SetCallback(new TimerLogInX);
        }

    }

    void Entresol::SetLoggingFrequency(Entresol::LoggingFrequency HowOften, Whole FrequencyCounter)
    {
        if (LogTimer)
        {
            TimerManager::GetSingletonPtr()->DestroyTimer(LogTimer);
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

    Entresol::LoggingFrequency Entresol::GetLoggingFrequency()
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

    void Entresol::CommitLog()
        { this->DoMainLoopLogging(); }

    void Entresol::LogString(const String& Message)
    {
        // if it is in the Audiologs then it has already happened so it needs to be logged first
        AudioManager* AudioMan = this->GetAudioManager();
        if(AudioMan && AudioMan->GetLogs())
            { this->LogStream << AudioMan->GetLogs()->str(); }
        if(Message.size()>0)
            { this->LogStream << endl << Message; }
    }

    void Entresol::Log()
    {
        this->LogString("");
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Initialization
    ///////////////////////////////////////

    void Entresol::EngineInit( const bool &CallMainLoop )
    {
        #ifdef MEZZDEBUG
        this->Log("Entering EngineInit()");
        #endif

        for (std::list< ManagerBase* >::iterator Iter=this->ManagerList.begin(); Iter!=this->ManagerList.end(); ++Iter )
        {
            this->LogStream << "Initializing " << (*Iter)->GetInterfaceTypeAsString() << " Manager" << endl;
            if((*Iter)->GetInterfaceType() != ManagerBase::GraphicsManager)
            {
                (*Iter)->Initialize();
            }
        }
        GraphicsManager::GetSingletonPtr()->Initialize();

        if(CallMainLoop)
        {
            #ifdef MEZZDEBUG
            this->Log("Starting the main loop");
            #endif
            this->MainLoop();
        }else{
            #ifdef MEZZDEBUG
            this->Log("Not calling the main loop");
            #endif
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // MainLoop
    ///////////////////////////////////////

    void Entresol::MainLoop()
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
                #ifdef MEZZDEBUG
                this->LogStream << "Current Manager: " << (*Iter)->GetInterfaceTypeAsString() << " - Priority: " << (*Iter)->GetPriority();
                this->DoMainLoopLogging();
                #endif

                #ifdef MEZZPROFILE
                LoopTimer->reset();
                #endif

                //Actual main loop work
                if( !(*Iter)->PreMainLoopItems() )
                    { DoNotBreak=false; }

                (*Iter)->DoMainLoopItems();

                if( !(*Iter)->PostMainLoopItems() )
                    { DoNotBreak=false; }

                #ifdef MEZZPROFILE
                this->LogStream << (*Iter)->GetInterfaceTypeAsString() << " took " << LoopTimer->getMicroseconds() << " microseconds.";
                this->DoMainLoopLogging();
                #endif

                LogCommitFunc();
            }
            if(ManualLoopBreak)
                    break;
        }//End of main loop

        ManualLoopBreak = false;
        delete LoopTimer;
    }

    void Entresol::DoMainLoopLogging()
    {
        if( this->LogStream.str().size() > 0 )
        {
            Ogre::LogManager::getSingleton().logMessage(this->LogStream.str());
            this->LogStream.str("");
        }
    }

    void Entresol::BreakMainLoop()
    {
        ManualLoopBreak = true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Simple get and Set functions
    ///////////////////////////////////////

    Whole Entresol::GetTargetFrameTime()
    {
        return this->TargetFrameLength;
    }

    void Entresol::SetTargetFrameTime(const Whole& NewTargetTime)
    {
        this->TargetFrameLength = NewTargetTime;
    }

    void Entresol::SetTargetFrameRate(const Whole& NewFrameRate)
    {
        this->SetTargetFrameTime( 1000/NewFrameRate );
    }

    Whole Entresol::GetFrameTime()
    {
        return this->FrameTime;
    }

    void Entresol::SetFrameTime( const Whole& FrameTime_ )
    {
        this->FrameTime = FrameTime_;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Factory Management
    ///////////////////////////////////////
    void Entresol::AddManagerFactory(ManagerFactory* ToBeAdded)
    {
        ManagerFactories.insert(std::pair<String,ManagerFactory*>(ToBeAdded->GetManagerTypeName(),ToBeAdded));
    }

    void Entresol::RemoveManagerFactory(ManagerFactory* ToBeRemoved)
    {
        RemoveManagerFactory(ToBeRemoved->GetManagerTypeName());
    }

    void Entresol::RemoveManagerFactory(const String& ImplName)
    {
        ManagerFactoryIterator ManIt = ManagerFactories.find(ImplName);
        if( ManIt != ManagerFactories.end() )
        {
            ManagerFactories.erase(ManIt);
        }
    }

    void Entresol::DestroyManagerFactory(ManagerFactory* ToBeRemoved)
    {
        DestroyManagerFactory(ToBeRemoved->GetManagerTypeName());
    }

    void Entresol::DestroyManagerFactory(const String& ImplName)
    {
        ManagerFactoryIterator ManIt = ManagerFactories.find(ImplName);
    }

    void Entresol::DestroyAllManagerFactories()
    {
        for( ManagerFactoryIterator ManIt = ManagerFactories.begin() ; ManIt != ManagerFactories.end() ; ++ManIt )
        {
            delete (*ManIt).second;
        }
        ManagerFactories.clear();
    }

    void Entresol::AddAllEngineDefaultManagerFactories()
    {
        ManagerFactoryIterator ManIt;
        //DefaultActorManager
        ManIt = ManagerFactories.find("DefaultActorManager");
        if( ManIt == ManagerFactories.end() ) AddManagerFactory(new DefaultActorManagerFactory());
        //DefaultAudioManager
        ManIt = ManagerFactories.find("DefaultAudioManager");
        if( ManIt == ManagerFactories.end() ) AddManagerFactory(new DefaultAudioManagerFactory());
        //DefaultCameraManager
        ManIt = ManagerFactories.find("DefaultCameraManager");
        if( ManIt == ManagerFactories.end() ) AddManagerFactory(new DefaultCameraManagerFactory());
        //DefaultCollisionShapeManager
        ManIt = ManagerFactories.find("DefaultCollisionShapeManager");
        if( ManIt == ManagerFactories.end() ) AddManagerFactory(new DefaultCollisionShapeManagerFactory());
        //DefaultEventManager
        ManIt = ManagerFactories.find("DefaultEventManager");
        if( ManIt == ManagerFactories.end() ) AddManagerFactory(new DefaultEventManagerFactory());
        //DefaultGraphicsManager
        ManIt = ManagerFactories.find("DefaultGraphicsManager");
        if( ManIt == ManagerFactories.end() ) AddManagerFactory(new DefaultGraphicsManagerFactory());
        //DefaultInputManager
        ManIt = ManagerFactories.find("DefaultInputManager");
        if( ManIt == ManagerFactories.end() ) AddManagerFactory(new DefaultInputManagerFactory());
        //DefaultMeshManager
        ManIt = ManagerFactories.find("DefaultMeshManager");
        if( ManIt == ManagerFactories.end() ) AddManagerFactory(new DefaultMeshManagerFactory());
        //DefaultNetworkManager
        ManIt = ManagerFactories.find("DefaultNetworkManager");
        if( ManIt == ManagerFactories.end() ) AddManagerFactory(new DefaultNetworkManagerFactory());
        //DefaultPhysicsManager
        ManIt = ManagerFactories.find("DefaultPhysicsManager");
        if( ManIt == ManagerFactories.end() ) AddManagerFactory(new DefaultPhysicsManagerFactory());
        //DefaultResourceManager
        ManIt = ManagerFactories.find("DefaultResourceManager");
        if( ManIt == ManagerFactories.end() ) AddManagerFactory(new DefaultResourceManagerFactory());
        //DefaultSceneManager
        ManIt = ManagerFactories.find("DefaultSceneManager");
        if( ManIt == ManagerFactories.end() ) AddManagerFactory(new DefaultSceneManagerFactory());
        //DefaultTerrainManager
        ManIt = ManagerFactories.find("DefaultTerrainManager");
        if( ManIt == ManagerFactories.end() ) AddManagerFactory(new DefaultTerrainManagerFactory());
        //DefaultTimerManager
        ManIt = ManagerFactories.find("DefaultTimerManager");
        if( ManIt == ManagerFactories.end() ) AddManagerFactory(new DefaultTimerManagerFactory());
        //DefaultUIManager
        ManIt = ManagerFactories.find("DefaultUIManager");
        if( ManIt == ManagerFactories.end() ) AddManagerFactory(new DefaultUIManagerFactory());
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Upper Management
    ///////////////////////////////////////
    ManagerBase* Entresol::CreateManager(const String& ManagerImplName, NameValuePairList& Params, bool AddToWorld)
    {
        ManagerFactoryIterator ManIt = ManagerFactories.find(ManagerImplName);
        if( ManIt == ManagerFactories.end() )
        {
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to create manager of type \"" + ManagerImplName + "\", which has no factory registered.");
        }
        ManagerBase* NewMan = (*ManIt).second->CreateManager(Params);
        if(AddToWorld)
            AddManager(NewMan);
    }
#ifdef MEZZXML
    ManagerBase* Entresol::CreateManager(const String& ManagerImplName, XML::Node& XMLNode, bool AddToWorld)
    {
        ManagerFactoryIterator ManIt = ManagerFactories.find(ManagerImplName);
        if( ManIt == ManagerFactories.end() )
        {
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to create manager of type \"" + ManagerImplName + "\", which has no factory registered.");
        }
        ManagerBase* NewMan = (*ManIt).second->CreateManager(XMLNode);
        if(AddToWorld)
            AddManager(NewMan);
    }
#endif
    void Entresol::DestroyManager(ManagerBase* ToBeDestroyed)
    {
        ManagerFactoryIterator ManIt = ManagerFactories.find(ToBeDestroyed->GetImplementationTypeName());
        if( ManIt == ManagerFactories.end() )
        {
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to destroy manager of type \"" + ToBeDestroyed->GetImplementationTypeName() + "\", which has no factory registered.");
        }
        RemoveManager(ToBeDestroyed);
        (*ManIt).second->DestroyManager(ToBeDestroyed);
    }

    void Entresol::DestroyAllManagers()
    {
        ManagerBase* Current;
        //for( std::list<ManagerBase*>::iterator iter = --this->ManagerList.end(); !ManagerList.empty(); iter = --this->ManagerList.end() ) //Backward
        for( std::list<ManagerBase*>::iterator iter = this->ManagerList.begin(); !ManagerList.empty(); iter = this->ManagerList.begin() ) //forward
        {
            Current = (*iter);
            #ifdef MEZZDEBUG
            this->Log("Deleting " + Current->GetInterfaceTypeAsString());
            #endif
            DestroyManager(Current);
        }
        DoMainLoopLogging();
    }

    void Entresol::AddManager(ManagerBase* ManagerToAdd)
    {
        #ifdef MEZZDEBUG
        this->LogStream << "Calling Entresol::AddManager("<<ManagerToAdd<<") size before:" <<this->ManagerList.size();
        #endif
        if(!this->ManagerList.empty())
        {
            for(std::list< ManagerBase* >::iterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
            {
                // Early escape in case we try to double add a manager to the list.
                if( (*ManIter) == ManagerToAdd )
                    return;
                if( (*ManIter)->GetPriority() > ManagerToAdd->GetPriority() )
                {
                    this->ManagerList.insert(ManIter, ManagerToAdd);
                    #ifdef MEZZDEBUG
                    this->LogStream << " - Added by sorted insertion:"<<ManagerToAdd<<" - size after:" <<this->ManagerList.size() ;
                    #endif
                    return;
                }
            }
        }
        ManagerList.push_back(ManagerToAdd);
        #ifdef MEZZDEBUG
        this->LogStream << " - Added by push_back:"<<ManagerToAdd<<" - size after:" <<this->ManagerList.size() << endl;
        #endif
    }

    void Entresol::RemoveManager(ManagerBase* ManagerToRemove)
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

    void Entresol::RemoveManager(const ManagerBase::ManagerType& ManagersToRemoveType, short unsigned int WhichOne)
    {
        if(this->ManagerList.empty())
        {
            //do nothing
        }else{
            for(std::list< ManagerBase* >::iterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
            {
                if( (*ManIter)->GetInterfaceType() == ManagersToRemoveType )
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

    ManagerBase* Entresol::GetManager(const ManagerBase::ManagerType& ManagersToGet, short unsigned int WhichOne)
    {
        #ifdef MEZZDEBUG
        //this->LogStream << "Calling Entresol::GetManager(Type:"<<ManagersToGet<<") searching through "<<this->ManagerList.size()<<" Items.";
        #endif
        if(this->ManagerList.empty())
        {
            return 0;
        }else{
            for(std::list< ManagerBase* >::iterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
            {
                if( (*ManIter)->GetInterfaceType() == ManagersToGet )
                {
                    if(0==WhichOne)     // we use our copy of WhichOne as a countdown to 0
                    {
                        #ifdef MEZZDEBUG
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
        #ifdef MEZZDEBUG
        //this->LogStream << " - Got:Nothing" << endl;
        #endif
        return 0;
    }

    void Entresol::UpdateManagerOrder(ManagerBase* ManagerToChange, short int Priority_)
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

    void Entresol::UpdateManagerOrder()
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

    ActorManager* Entresol::GetActorManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<ActorManager*> (this->GetManager(ManagerBase::ActorManager, WhichOne));
    }

    AudioManager* Entresol::GetAudioManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<AudioManager*> (this->GetManager(ManagerBase::AudioManager, WhichOne));
    }

    CameraManager* Entresol::GetCameraManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<CameraManager*> (this->GetManager(ManagerBase::CameraManager, WhichOne));
    }

    CollisionShapeManager* Entresol::GetCollisionShapeManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<CollisionShapeManager*> (this->GetManager(ManagerBase::CollisionShapeManager, WhichOne));
    }

    EventManager* Entresol::GetEventManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<EventManager*> (this->GetManager(ManagerBase::EventManager, WhichOne));
    }

    GraphicsManager* Entresol::GetGraphicsManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<GraphicsManager*> (this->GetManager(ManagerBase::GraphicsManager, WhichOne));
    }

    InputManager* Entresol::GetInputManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<InputManager*> (this->GetManager(ManagerBase::InputManager, WhichOne));
    }

    MeshManager* Entresol::GetMeshManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<MeshManager*> (this->GetManager(ManagerBase::MeshManager, WhichOne));
    }
    #ifdef MEZZNETWORK
    NetworkManager* Entresol::GetNetworkManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<NetworkManager*> (this->GetManager(ManagerBase::NetworkManager, WhichOne));
    }
    #endif
    PhysicsManager* Entresol::GetPhysicsManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<PhysicsManager*> (this->GetManager(ManagerBase::PhysicsManager, WhichOne));
    }

    SceneManager* Entresol::GetSceneManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<SceneManager*> (this->GetManager(ManagerBase::SceneManager, WhichOne));
    }

    ResourceManager* Entresol::GetResourceManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<ResourceManager*> (this->GetManager(ManagerBase::ResourceManager, WhichOne));
    }

    TimerManager* Entresol::GetTimerManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<TimerManager*> (this->GetManager(ManagerBase::TimerManager, WhichOne));
    }

    UIManager* Entresol::GetUIManager(const short unsigned int &WhichOne)
    {
        return dynamic_cast<UIManager*> (this->GetManager(ManagerBase::UIManager, WhichOne));
    }

}
#endif
