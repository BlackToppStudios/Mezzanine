//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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

/// @todo Remove #include "mezzanine.h" and just include what is required. Waiting for multithreaded refactor, because we will have to do this again after that.
#include "mezzanine.h"

//Enabled implementation includes
#ifdef ENABLE_OALS_AUDIO_IMPLEMENTATION
    // Permit the factories to be visible so they can be auto-added.
    #include "Audio/OALS/oalsaudiomanagerfactory.h"
    #include "Audio/OALS/oalssoundscapemanagerfactory.h"
#endif //ENABLE_OALS_AUDIO_IMPLEMENTATION

//#include "OgreBspSceneManagerPlugin.h"
//#include "OgreCgPlugin.h"
//#include "OgreOctreePlugin.h"
//#include "OgreOctreeZonePlugin.h"
#include "OgreParticleFXPlugin.h"
//#include "OgrePCZPlugin.h"

#include <SDL.h>
#include <Ogre.h>
#include <btBulletDynamicsCommon.h>

#include <sstream>
#include <string>

using namespace std;

namespace Mezzanine
{
    template<> Entresol* Singleton<Entresol>::SingletonPtr = NULL;

    namespace
    {
        // Since there doesn't seem to be anyway to check if the ogre root exists yet.
        // if a you haven't made an ogre root, then Ogre::Root::getSingletonPtr(); appears to return gibberish rather that a zero.
        Ogre::Root* OgreCore = NULL;
    }


    ///////////////////////////////////////////////////////////////////////////////
    // Construction and Destruction Helpers

    void Entresol::SetupLogging(const String& LogFileName)
    {
        /// @todo Allow the FrameScheduler Log target to be inspected and changed here
        Ogre::LogManager* OgreLogs = Ogre::LogManager::getSingletonPtr();
        if( NULL == OgreLogs )
            { OgreLogs = new Ogre::LogManager(); }

        if(!LogFileName.empty())
        {
            OgreLogs->createLog(String("Graphics")+LogFileName,true,true);
        }
        else
        {
            OgreLogs->createLog("GraphicsMezzanine.log",true,true);
        }
        this->Aggregator = new Threading::LogAggregator();
        Aggregator->SetAggregationTarget(&WorkScheduler);
        this->WorkScheduler.AddWorkUnitMain(Aggregator);
    }

    void Entresol::DestroyLogging()
    {
        this->WorkScheduler.RemoveWorkUnitMain(Aggregator);
        delete Aggregator;
    }

    void Entresol::SetupOgre()
    {
        if ( NULL == OgreCore )
            { OgreCore = new Ogre::Root("","",""); }
        else
            { OgreCore = Ogre::Root::getSingletonPtr(); }
    }

    void Entresol::DestroyOgre()
    {
        //Ogre::Root::getSingleton().shutdown();
        delete Ogre::Root::getSingletonPtr(); // This should be done by the shutdown method shouldn't it?
        OgreCore = 0;
        delete SubSystemParticleFXPlugin;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Mezzanine constructors

    /// @TODO In the Entrosol, reomves all references to a plugins file
    Entresol::Entresol()
    {
        Physics::ManagerConstructionInfo PhysicsInfo;
        std::vector <ManagerBase*> temp;

        this->Construct(PhysicsInfo,"DefaultSceneManager",".","Mezzanine.log",temp);
    }


    Entresol::Entresol(const String& EngineDataPath, const Mezzanine::ArchiveType ArchType, const String& InitializerFile)
    {
        if(String::npos != InitializerFile.find(".mxi"))
            { ConstructFromXML(EngineDataPath, ArchType, InitializerFile); }
        else
            { MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Attempting to initialze Mezzanine from an unsupported file type."); }
    }

    Entresol::Entresol(std::vector<ManagerFactory*>& CustomFactories, const String& EngineDataPath, const Mezzanine::ArchiveType ArchType, const String& InitializerFile)
    {
        for(std::vector<ManagerFactory*>::iterator it = CustomFactories.begin(); it != CustomFactories.end(); ++it)
        {
            AddManagerFactory( (*it) );
        }

        if(String::npos != InitializerFile.find(".mxi"))
            { ConstructFromXML(EngineDataPath, ArchType, InitializerFile); }
        else
            { MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Attempting to initialze Mezzanine from an unsupported file type."); }
    }


    Entresol::Entresol( const Physics::ManagerConstructionInfo& PhysicsInfo,
                        const String& SceneType,
                        const String& EngineDataPath,
                        const String& LogFileName)
    {
        std::vector <ManagerBase*> temp;
        this->Construct(PhysicsInfo,
                        SceneType,
                        EngineDataPath,
                        LogFileName,
                        temp );
    }

    Entresol::Entresol( const Physics::ManagerConstructionInfo& PhysicsInfo,
                        const String& SceneType,
                        const String& EngineDataPath,
                        const String& LogFileName,
                        const std::vector <ManagerBase*>& ManagerToBeAdded)
    {
        this->Construct(PhysicsInfo,
                        SceneType,
                        EngineDataPath,
                        LogFileName,
                        ManagerToBeAdded );

    }

    void Entresol::Construct(   const Physics::ManagerConstructionInfo& PhysicsInfo,
                                const String& SceneType,
                                const String& EngineDataPath,
                                const String& LogFileName,
                                const std::vector <ManagerBase*>& ManagerToBeAdded )

    {
        //Add default manager factories
        AddAllEngineDefaultManagerFactories();
        //Set some sane Defaults for some values
        this->TargetFrameLength = 16667;
        this->FrameTime = 0;
        this->ManualLoopBreak = 0;

        SetupOgre();
        SetupLogging(LogFileName);

        // Load the necessary plugins.
        SubSystemParticleFXPlugin = new Ogre::ParticleFXPlugin();
        Ogre::Root::getSingleton().installPlugin(SubSystemParticleFXPlugin);

        //add each manager that was passed in to the manager list
        for(std::vector<ManagerBase*>::const_iterator iter = ManagerToBeAdded.begin(); iter!= ManagerToBeAdded.end(); iter++)
            { this->AddManager(*iter); }

        //Dummy param list so we can use the auto-added manager types if needed
        NameValuePairList Params;
        //Create and add any managers that have not been taken care of yet.
        if(this->GetActorManager()==0)
            { this->AddManager(new ActorManager()); }
        if(this->GetAreaEffectManager()==0)
            { this->AddManager(new AreaEffectManager()); }
        if(this->GetResourceManager()==0)
            { this->AddManager(new ResourceManager(EngineDataPath)); }
        if(this->GetGraphicsManager()==0)
            { this->AddManager(new Graphics::GraphicsManager()); }
        if(this->GetEventManager()==0)
            { this->AddManager(new EventManager()); }
        if(this->GetInputManager()==0)
            { this->AddManager(new Input::InputManager()); }
        if(this->GetPhysicsManager()==0)
            { this->AddManager(new Physics::PhysicsManager(PhysicsInfo)); }
        if(this->GetSceneManager()==0)
            { this->AddManager(new SceneManager(SceneType)); }
        if(this->GetUIManager()==0)
            { this->AddManager(new UI::UIManager()); }
        if(this->GetMeshManager()==0)
            { this->AddManager(new MeshManager()); }
        if(this->GetCollisionShapeManager()==0)
            { this->AddManager(new CollisionShapeManager()); }
        if(this->GetCameraManager()==0)
            { this->AddManager(new CameraManager()); }
        #ifdef ENABLE_OALS_AUDIO_IMPLEMENTATION
        if(this->GetAudioManager()==0)
            { this->AddManager( this->CreateManager("OALSAudioManager",Params,false) ); }
        if(this->GetSoundScapeManager()==0)
            { this->AddManager( this->CreateManager("OALSSoundScapeManager",Params,false) ); }
        #endif //ENABLE_OALS_AUDIO_IMPLEMENTATION

        // This Tests various assumptions about the way the platform works, and will not act
        SanityChecks();
    }

    void Entresol::ConstructFromXML(const String& EngineDataPath, const Mezzanine::ArchiveType ArchType, const String& InitializerFile)
    {
        //Add default manager factories
        AddAllEngineDefaultManagerFactories();
        //Set some sane Defaults for some values.
        this->TargetFrameLength = 16667;
        this->FrameTime = 0;
        this->ManualLoopBreak = false;

        // Create Ogre.
        SetupOgre();

        // Load the necessary plugins.
        SubSystemParticleFXPlugin = new Ogre::ParticleFXPlugin();
        Ogre::Root::getSingleton().installPlugin(SubSystemParticleFXPlugin);

        // Set up the data we'll be populating.
        XML::Attribute CurrAttrib;
        String GUIInit, ResourceInit, PluginsInit, LogFileName;
        String PluginExtension, PluginPath;

        // Create or set the resource manager.
        /// @todo This currently forces our default resource manager to be constructed, which isn't in line with our factory/initiailzation design.
        /// This should be addressed somehow.
        if(ResourceManager::SingletonValid())
            { AddManager(ResourceManager::GetSingletonPtr()); }
        else
            { AddManager(new ResourceManager(EngineDataPath, ArchType)); }

        // Open and load the initializer doc.
        ResourceManager* ResourceMan = GetResourceManager();
        /// @todo Replace this stack allocated stream for one initialized from the Resource Manager, after the system is ready.
        Resource::FileStream InitStream(InitializerFile,EngineDataPath);
        XML::Document InitDoc;
        XML::ParseResult DocResult = InitDoc.Load(InitStream);
        if( DocResult.Status != XML::StatusOk )
        {
            StringStream ExceptionStream;
            ExceptionStream << "Failed to parse XML file \"" << InitializerFile << "\".";
            MEZZ_EXCEPTION(Exception::SYNTAX_ERROR_EXCEPTION_XML,ExceptionStream.str());
        }
        XML::Node InitRoot = InitDoc.GetChild("InitializerRoot");
        if( InitRoot.Empty() )
        {
            StringStream ExceptionStream;
            ExceptionStream << "Failed to find expected Root node in \"" << InitializerFile << "\".";
            MEZZ_EXCEPTION(Exception::SYNTAX_ERROR_EXCEPTION_XML,ExceptionStream.str());
        }

        // Get the world settings and set them.
        XML::Node WorldSettings = InitRoot.GetChild("WorldSettings");
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
                        SetTargetFrameTimeMicroseconds(CurrAttrib.AsWhole());
                }else{
                    this->SetTargetFrameRate(CurrAttrib.AsWhole());
                }
            }
            else
            {
                MEZZ_EXCEPTION(Exception::SYNTAX_ERROR_EXCEPTION_XML,String("Unknown WorldSetting ")+SecName);
            }

        }

        SetupLogging(LogFileName);

        // Get the other initializer files we'll be using, since we'll need the plugins initializer.
        XML::Node InitFiles = InitRoot.GetChild("OtherInitializers");
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

        // Load additional resource groups
        /*if(!ResourceInit.empty())
        {
            /// @todo Replace this stack allocated stream for one initialized from the Resource Manager, after the system is ready.
            Resource::FileStream ResourceStream(ResourceInit,EngineDataPath);
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
        XML::Node Managers = InitRoot.GetChild("Managers");
        for( XML::NodeIterator ManIt = Managers.begin() ; ManIt != Managers.end() ; ++ManIt )
        {
            CreateManager( (*ManIt).Name(), (*ManIt) );
        }

        // Load additional resource groups
        if(!ResourceInit.empty())
        {
            /// @todo Replace this stack allocated stream for one initialized from the Resource Manager, after the system is ready.
            Resource::FileStream ResourceStream(ResourceInit,EngineDataPath);
            XML::Document ResourceDoc;
            ResourceDoc.Load(ResourceStream);
            // Get an iterator to the first resource group node, and declare them all.
            XML::Node ResourceLocations = ResourceDoc.GetChild("ResourceLocations");
            for( XML::NodeIterator GroupIt = ResourceLocations.begin() ; GroupIt != ResourceLocations.end() ; ++GroupIt )
            {
                String GroupName, GroupPath;
                ArchiveType GroupType;
                bool GroupRecursive = false;
                // Get the group path
                CurrAttrib = (*GroupIt).GetAttribute("GroupPath");
                if(!CurrAttrib.Empty())
                    GroupPath = CurrAttrib.AsString();
                // Get the group type
                CurrAttrib = (*GroupIt).GetAttribute("GroupType");
                if(!CurrAttrib.Empty())
                    GroupType = ResourceManager::GetArchiveTypeFromString(CurrAttrib.AsString());
                // Get the group name
                CurrAttrib = (*GroupIt).GetAttribute("GroupName");
                if(!CurrAttrib.Empty())
                    GroupName = CurrAttrib.AsString();
                // Get whether this is recursive
                CurrAttrib = (*GroupIt).GetAttribute("Recursive");
                if(!CurrAttrib.Empty())
                    GroupRecursive = StringTools::ConvertToBool(CurrAttrib.AsString());
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


    void Entresol::SanityChecks()
    {
        //crossplatform::WaitMilliseconds(1500);
        //Perform a Test that only needs to be done once for the SDL/userinputevent system.`
        /*Log("Verifying size of userinput events:");
        Log(sizeof(Input::InputCode));
        Log(sizeof(SDL_Scancode));
        Log(sizeof(int));//*/
        if(sizeof(Input::InputCode) != sizeof(SDL_Scancode))
        {
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"User input subsystem Event Sizes Don't match, userinput subsystem will go be buggier than a highschool fortran class.");
        }

        if(sizeof(Input::InputCode) != sizeof(int))
        {
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Internal User input subsystem Event Sizes Don't match, userinput subsystem cannot function.");
        }
    }

    void Entresol::OneTimeMainLoopInit()
    {
        VerifyManagerInitializations();
        this->GetPhysicsManager()->MainLoopInitialize();
        this->GetAreaEffectManager()->MainLoopInitialize();
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
        DestroyLogging();

        SDL_Quit();

        DestroyOgre();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void Entresol::PauseWorld(bool Pause)
    {
        this->GetPhysicsManager()->PauseSimulation(Pause);
        this->GetSceneManager()->PauseAllParticles(Pause);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Logging

    void Entresol::LogString(const String& Message)
    {
        // if it is in the Audiologs then it has already happened so it needs to be logged first
        if(Message.size()>0)
            { this->GetLogStream() << Message; }
    }

    Logger& Entresol::GetLogStream(Threading::Thread::id ID)
    {
        Threading::FrameScheduler::Resource* AlmostResults = this->WorkScheduler.GetThreadResource(ID);
        if(AlmostResults)
            { return AlmostResults->GetUsableLogger(); }
        else
            { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Could not access thread Specific Logger from invalid thread."); }
    }

    Threading::LogAggregator* Entresol::GetLogAggregator()
        { return this->Aggregator; }

    Threading::LogBufferSwapper* Entresol::GetLogBufferSwapper()
        { return this->BufferSwapper; }

    ///////////////////////////////////////////////////////////////////////////////
    // Initialization
    ///////////////////////////////////////

    void Entresol::EngineInit( const bool &CallMainLoop )
    {
        for (std::list< ManagerBase* >::iterator Iter=this->ManagerList.begin(); Iter!=this->ManagerList.end(); ++Iter )
        {
            StringStream InitStream;
            InitStream << "Initializing " << (*Iter)->GetInterfaceTypeAsString() << ".";
            this->Log(InitStream.str());
            if((*Iter)->GetInterfaceType() != ManagerBase::MT_GraphicsManager)
            {
                (*Iter)->Initialize();
            }
        }
        Graphics::GraphicsManager::GetSingletonPtr()->Initialize();

        if(CallMainLoop)
        {
            this->MainLoop();
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // MainLoop
    ///////////////////////////////////////

    Threading::FrameScheduler& Entresol::GetScheduler()
    {
        return this->WorkScheduler;
    }

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

        Ogre::Timer* FrameTimer = new Ogre::Timer();
        this->OneTimeMainLoopInit();
        //As long as all the CallBacks return true the game continues
        bool DoNotBreak = true;
        while(DoNotBreak)
        {
            #ifdef MEZZDEBUG
            static UInt32 FrameCounter = 0;
            StringStream FrameStream;
            FrameStream << "<FrameCounterStart StartingFrame=\"" << FrameCounter << "\" />" << endl;
            this->Log(FrameStream.str());
            #endif
            FrameTimer->reset();

            WorkScheduler.RunAllMonopolies(); //1
            WorkScheduler.CreateThreads();    //2
            WorkScheduler.RunMainThreadWork();//3
            WorkScheduler.JoinAllThreads();   //4
            WorkScheduler.ResetAllWorkUnits();//5
            #ifdef MEZZDEBUG
            FrameStream.str("");
            FrameStream << "<FrameCounterPrePause Frame=\"" << FrameCounter << "\"" << "PrePauseLength=\"" << FrameTimer->getMicroseconds() << "\" />" << endl;
            this->Log(FrameStream.str());
            #endif
            WorkScheduler.WaitUntilNextFrame(); //6

            #ifdef MEZZDEBUG
            FrameStream.str("");
            FrameStream << "<FrameCounterEnd Frame=\"" << FrameCounter << "\"" << "Length=\"" << FrameTimer->getMicroseconds() << "\" />" << endl;
            this->Log(FrameStream.str());
            ++FrameCounter;
            #endif

            this->FrameTime = FrameTimer->getMicroseconds();

            if( ManualLoopBreak )
                break;
        }//End of main loop

        ManualLoopBreak = 0;
        delete FrameTimer;
    }

    void Entresol::BreakMainLoop()
    {
        while(!ManualLoopBreak)
            { Threading::AtomicCompareAndSwap32(&ManualLoopBreak,0,1); }
        //ManualLoopBreak = true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Simple get and Set functions
    ///////////////////////////////////////

    void Entresol::SetTargetFrameRate(const Whole NewFrameRate)
    {
        this->SetTargetFrameTimeMicroseconds( 1000000 / static_cast<Real>( NewFrameRate ) );
    }

    void Entresol::SetTargetFrameTimeMilliseconds(const Whole NewTargetTime)
    {
        this->SetTargetFrameTimeMicroseconds( NewTargetTime * 1000 );
    }

    void Entresol::SetTargetFrameTimeMicroseconds(const Whole NewTargetTime)
    {
        this->TargetFrameLength = NewTargetTime;
    }

    Whole Entresol::GetTargetFrameTimeMilliseconds() const
    {
        return this->TargetFrameLength * 0.001;
    }

    Whole Entresol::GetTargetFrameTimeMicroseconds() const
    {
        return this->TargetFrameLength;
    }

    Whole Entresol::GetFrameTimeMilliseconds() const
    {
        return this->FrameTime * 0.001;
    }

    Whole Entresol::GetFrameTimeMicroseconds() const
    {
        return this->FrameTime;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Factory Management

    void Entresol::AddManagerFactory(ManagerFactory* ToBeAdded)
    {
        this->ManagerFactories.insert(std::pair<String,ManagerFactory*>(ToBeAdded->GetManagerTypeName(),ToBeAdded));
    }

    void Entresol::RemoveManagerFactory(ManagerFactory* ToBeRemoved)
    {
        this->RemoveManagerFactory(ToBeRemoved->GetManagerTypeName());
    }

    void Entresol::RemoveManagerFactory(const String& ImplName)
    {
        ManagerFactoryIterator ManIt = this->ManagerFactories.find(ImplName);
        if( ManIt != this->ManagerFactories.end() )
        {
            this->ManagerFactories.erase(ManIt);
        }
    }

    void Entresol::DestroyManagerFactory(ManagerFactory* ToBeRemoved)
    {
        this->DestroyManagerFactory(ToBeRemoved->GetManagerTypeName());
    }

    void Entresol::DestroyManagerFactory(const String& ImplName)
    {
        ManagerFactoryIterator ManIt = this->ManagerFactories.find(ImplName);
        delete ManIt->second;
        this->ManagerFactories.erase(ManIt);
    }

    void Entresol::DestroyAllManagerFactories()
    {
        for( ManagerFactoryIterator ManIt = this->ManagerFactories.begin() ; ManIt != this->ManagerFactories.end() ; ++ManIt )
        {
            delete (*ManIt).second;
        }
        this->ManagerFactories.clear();
    }

    void Entresol::AddAllEngineDefaultManagerFactories()
    {
        ManagerFactoryIterator ManIt;
        //DefaultActorManager
        ManIt = this->ManagerFactories.find("DefaultActorManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new DefaultActorManagerFactory());
        //DefaultAreaEffectManager
        ManIt = this->ManagerFactories.find("DefaultAreaEffectManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new DefaultAreaEffectManagerFactory());
        //DefaultCameraManager
        ManIt = this->ManagerFactories.find("DefaultCameraManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new DefaultCameraManagerFactory());
        //DefaultCollisionShapeManager
        ManIt = this->ManagerFactories.find("DefaultCollisionShapeManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new DefaultCollisionShapeManagerFactory());
        //DefaultEventManager
        ManIt = this->ManagerFactories.find("DefaultEventManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new DefaultEventManagerFactory());
        //DefaultGraphicsManager
        ManIt = this->ManagerFactories.find("DefaultGraphicsManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new Graphics::DefaultGraphicsManagerFactory());
        //DefaultInputManager
        ManIt = this->ManagerFactories.find("DefaultInputManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new Input::DefaultInputManagerFactory());
        //DefaultMeshManager
        ManIt = this->ManagerFactories.find("DefaultMeshManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new DefaultMeshManagerFactory());
        //DefaultNetworkManager
        ManIt = this->ManagerFactories.find("DefaultNetworkManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new DefaultNetworkManagerFactory());
        //DefaultPhysicsManager
        ManIt = this->ManagerFactories.find("DefaultPhysicsManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new Physics::DefaultPhysicsManagerFactory());
        //DefaultResourceManager
        ManIt = this->ManagerFactories.find("DefaultResourceManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new DefaultResourceManagerFactory());
        //DefaultSceneManager
        ManIt = this->ManagerFactories.find("DefaultSceneManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new DefaultSceneManagerFactory());
        //DefaultTerrainManager
        ManIt = this->ManagerFactories.find("DefaultTerrainManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new DefaultTerrainManagerFactory());
        //DefaultUIManager
        ManIt = this->ManagerFactories.find("DefaultUIManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new UI::DefaultUIManagerFactory());

        #ifdef ENABLE_OALS_AUDIO_IMPLEMENTATION
        //OALSAudioManager
        ManIt = this->ManagerFactories.find("OALSAudioManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new Audio::OALS::OALSAudioManagerFactory());
        //OALSSoundScapeManager
        ManIt = this->ManagerFactories.find("OALSSoundScapeManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new Audio::OALS::OALSSoundScapeManagerFactory());
        #endif //ENABLE_OALS_AUDIO_IMPLEMENTATION
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Upper Management

    ManagerBase* Entresol::CreateManager(const String& ManagerImplName, NameValuePairList& Params, bool AddToWorld)
    {
        ManagerFactoryIterator ManIt = this->ManagerFactories.find(ManagerImplName);
        if( ManIt == this->ManagerFactories.end() )
        {
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to create manager of type \"" + ManagerImplName + "\", which has no factory registered.");
        }
        ManagerBase* NewMan = (*ManIt).second->CreateManager(Params);
        if(AddToWorld)
            this->AddManager(NewMan);
        return NewMan;
    }

    ManagerBase* Entresol::CreateManager(const String& ManagerImplName, XML::Node& XMLNode, bool AddToWorld)
    {
        ManagerFactoryIterator ManIt = this->ManagerFactories.find(ManagerImplName);
        if( ManIt == this->ManagerFactories.end() )
        {
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to create manager of type \"" + ManagerImplName + "\", which has no factory registered.");
        }
        ManagerBase* NewMan = (*ManIt).second->CreateManager(XMLNode);
        if(AddToWorld)
            this->AddManager(NewMan);
        return NewMan;
    }

    void Entresol::DestroyManager(ManagerBase* ToBeDestroyed)
    {
        ManagerFactoryIterator ManIt = this->ManagerFactories.find(ToBeDestroyed->GetImplementationTypeName());
        if( ManIt == this->ManagerFactories.end() )
        {
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to destroy manager of type \"" + ToBeDestroyed->GetImplementationTypeName() + "\", which has no factory registered.");
        }
        this->RemoveManager(ToBeDestroyed);
        (*ManIt).second->DestroyManager(ToBeDestroyed);
    }

    void Entresol::DestroyAllManagers()
    {
        //for( std::list<ManagerBase*>::iterator iter = --this->ManagerList.end(); !ManagerList.empty(); iter = --this->ManagerList.end() ) //Backward
        /*for( std::list<ManagerBase*>::iterator iter = this->ManagerList.begin(); !ManagerList.empty(); iter = this->ManagerList.begin() ) //forward
        {
            ManagerBase* Current = (*iter);
            #ifdef MEZZDEBUG
            this->Log("Deleting " + Current->GetInterfaceTypeAsString() + ".");
            this->DoMainLoopLogging();
            #endif
            ManagerFactoryIterator ManIt = this->ManagerFactories.find(Current->GetImplementationTypeName());
            if( ManIt == this->ManagerFactories.end() ) {
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to destroy manager of type \"" + Current->GetImplementationTypeName() + "\", which has no factory registered.");
            }else{
                (*ManIt).second->DestroyManager(Current);
            }
        }
        this->ManagerList.clear();//*/
        while( !(this->ManagerList.empty()) )
        {
            ManagerBase* Current = this->ManagerList.front();
            #ifdef MEZZDEBUG
            this->Log("Deleting " + Current->GetInterfaceTypeAsString() + ".");
            #endif

            ManagerFactoryIterator ManIt = this->ManagerFactories.find(Current->GetImplementationTypeName());
            if( ManIt == this->ManagerFactories.end() ) {
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to destroy manager of type \"" + Current->GetImplementationTypeName() + "\", which has no factory registered.");
            }else{
                (*ManIt).second->DestroyManager(Current);
            }

            this->ManagerList.pop_front();
        }//*/
    }

    void Entresol::AddManager(ManagerBase* ManagerToAdd)
    {
        #ifdef MEZZDEBUG
        this->Log("Adding " + ManagerToAdd->GetInterfaceTypeAsString() + ".");
        #endif
        // We have to verify the manager is unique.  A number of issues can arrise if a manager is double inserted.
        for( std::list< ManagerBase* >::iterator ManIter = this->ManagerList.begin() ; ManIter != this->ManagerList.end() ; ++ManIter )
        {
            if( (*ManIter) == ManagerToAdd )
                return;
        }
        this->ManagerList.push_back(ManagerToAdd);
    }

    void Entresol::RemoveManager(ManagerBase* ManagerToRemove)
    {
        for( std::list< ManagerBase* >::iterator ManIter = this->ManagerList.begin() ; ManIter != this->ManagerList.end() ; ++ManIter )
        {
            if( *ManIter == ManagerToRemove )
            {
                this->ManagerList.erase(ManIter);
                return;
            }
        }
    }

    void Entresol::RemoveManager(const ManagerBase::ManagerType ManagersToRemoveType, UInt16 WhichOne)
    {
        for(std::list< ManagerBase* >::iterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
        {
            if( (*ManIter)->GetInterfaceType() == ManagersToRemoveType )
            {
                if(0==WhichOne)     // we use our copy of WhichOne as a countdown to 0
                {
                    this->ManagerList.erase(ManIter);
                    return;
                }else{
                    --WhichOne;
                }
            }
        }
    }

    ManagerBase* Entresol::GetManager(const ManagerBase::ManagerType ManagersToGet, UInt16 WhichOne)
    {
        for(std::list< ManagerBase* >::iterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
        {
            if( (*ManIter)->GetInterfaceType() == ManagersToGet )
            {
                if( 0 == WhichOne ) return *ManIter; // we use our copy of WhichOne as a countdown to 0
                else --WhichOne;
            }
        }
        return NULL;
    }

    ActorManager* Entresol::GetActorManager(const UInt16 WhichOne)
    {
        return dynamic_cast<ActorManager*>( this->GetManager(ManagerBase::MT_ActorManager, WhichOne) );
    }

    AreaEffectManager* Entresol::GetAreaEffectManager(const UInt16 WhichOne)
    {
        return dynamic_cast<AreaEffectManager*>( this->GetManager(ManagerBase::MT_AreaEffectManager, WhichOne) );
    }

    Audio::AudioManager* Entresol::GetAudioManager(const UInt16 WhichOne)
    {
        return dynamic_cast<Audio::AudioManager*>( this->GetManager(ManagerBase::MT_AudioManager, WhichOne) );
    }

    CameraManager* Entresol::GetCameraManager(const UInt16 WhichOne)
    {
        return dynamic_cast<CameraManager*>( this->GetManager(ManagerBase::MT_CameraManager, WhichOne) );
    }

    CollisionShapeManager* Entresol::GetCollisionShapeManager(const UInt16 WhichOne)
    {
        return dynamic_cast<CollisionShapeManager*>( this->GetManager(ManagerBase::MT_CollisionShapeManager, WhichOne) );
    }

    EventManager* Entresol::GetEventManager(const UInt16 WhichOne)
    {
        return dynamic_cast<EventManager*>( this->GetManager(ManagerBase::MT_EventManager, WhichOne) );
    }

    Graphics::GraphicsManager* Entresol::GetGraphicsManager(const UInt16 WhichOne)
    {
        return dynamic_cast<Graphics::GraphicsManager*>( this->GetManager(ManagerBase::MT_GraphicsManager, WhichOne) );
    }

    Input::InputManager* Entresol::GetInputManager(const UInt16 WhichOne)
    {
        return dynamic_cast<Input::InputManager*>( this->GetManager(ManagerBase::MT_InputManager, WhichOne) );
    }

    MeshManager* Entresol::GetMeshManager(const UInt16 WhichOne)
    {
        return dynamic_cast<MeshManager*>( this->GetManager(ManagerBase::MT_MeshManager, WhichOne) );
    }
    #ifdef MEZZNETWORK
    NetworkManager* Entresol::GetNetworkManager(const UInt16 WhichOne)
    {
        return dynamic_cast<NetworkManager*>( this->GetManager(ManagerBase::MT_NetworkManager, WhichOne) );
    }
    #endif
    Physics::PhysicsManager* Entresol::GetPhysicsManager(const UInt16 WhichOne)
    {
        return dynamic_cast<Physics::PhysicsManager*>( this->GetManager(ManagerBase::MT_PhysicsManager, WhichOne) );
    }

    SceneManager* Entresol::GetSceneManager(const UInt16 WhichOne)
    {
        return dynamic_cast<SceneManager*>( this->GetManager(ManagerBase::MT_SceneManager, WhichOne) );
    }

    Audio::SoundScapeManager* Entresol::GetSoundScapeManager(const UInt16 WhichOne)
    {
        return dynamic_cast<Audio::SoundScapeManager*>( this->GetManager(ManagerBase::MT_SoundScapeManager, WhichOne) );
    }

    ResourceManager* Entresol::GetResourceManager(const UInt16 WhichOne)
    {
        return dynamic_cast<ResourceManager*>( this->GetManager(ManagerBase::MT_ResourceManager, WhichOne) );
    }

    UI::UIManager* Entresol::GetUIManager(const UInt16 WhichOne)
    {
        return dynamic_cast<UI::UIManager*>( this->GetManager(ManagerBase::MT_UIManager, WhichOne) );
    }

}
#endif
