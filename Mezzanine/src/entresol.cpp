// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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

// Enabled implementation includes
#ifdef ENABLE_OALS_AUDIO_IMPLEMENTATION
    #include "Audio/OALS/oalsaudiomanagerfactory.h"
#endif // ENABLE_OALS_AUDIO_IMPLEMENTATION

#ifdef MEZZLUA51
    #include "Scripting/Lua51/lua51scriptingengine.h"
#endif // MEZZLUA51


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
#include <cassert>

#include "world.h"
#include "worldmanager.h"

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

    void Entresol::SetupLogging(const String& OgreLogFileName)
    {
        SetupOgreLogging(OgreLogFileName);
        SetupInternalLogging();
    }

    void Entresol::SetupOgreLogging(const String& OgreLogFileName)
    {
        /// @todo Allow the FrameScheduler Log target to be inspected and changed here
        Ogre::LogManager* OgreLogs = Ogre::LogManager::getSingletonPtr();
        if( NULL == OgreLogs )
            { OgreLogs = new Ogre::LogManager(); }

        if(!OgreLogFileName.empty())
        {
            OgreLogs->createLog(String("Graphics")+OgreLogFileName,true,true);
        }
        else
        {
            OgreLogs->createLog("GraphicsMezzanine.log",true,true);
        }
    }

    void Entresol::SetupInternalLogging()
    {
        this->Aggregator = new Threading::LogAggregator();
        Aggregator->SetAggregationTarget(&WorkScheduler);
        this->WorkScheduler.AddWorkUnitMain(Aggregator, "LogAggregator");
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
        #ifdef LINUX
        //assert( !( SDL_WasInit(0) | SDL_INIT_VIDEO ) && "SDL already shut down.  SDL Shutdown forces x11 unload, which Ogre needs for it's shutdown." );
        #endif

        delete Ogre::Root::getSingletonPtr();
        OgreCore = 0;
        delete SubSystemParticleFXPlugin;
    }

    void Entresol::DestroySDL()
    {
        #ifdef LINUX
        // Fail is SDL is de-intialized before Ogre, but only if Ogre has been initialized.
        assert( NULL==OgreCore // ( (SDL_WasInit(0) | SDL_INIT_VIDEO)
                && "SDL already shut down.  SDL Shutdown forces x11 unload, which Ogre needs for it's shutdown." );
        #endif
        SDL_Quit();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Mezzanine constructors

    /// @TODO In the Entrosol, reomves all references to a plugins file
    Entresol::Entresol()
    {
        Physics::ManagerConstructionInfo PhysicsInfo;
        std::vector <EntresolManager*> temp;

        this->Construct(PhysicsInfo,"DefaultSceneManager",".","Mezzanine.log",temp);
    }


    Entresol::Entresol(const String& EngineDataPath, const Mezzanine::ArchiveType ArchType, const String& InitializerFile)
    {
        if(String::npos != InitializerFile.find(".mxi"))
            { ConstructFromXML(EngineDataPath, ArchType, InitializerFile); }
        else
            { MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Attempting to initialze Mezzanine from an unsupported file type."); }
    }

    Entresol::Entresol(ManagerFactoryVec& CustomFactories, const String& EngineDataPath, const Mezzanine::ArchiveType ArchType, const String& InitializerFile)
    {
        for(ManagerFactoryVec::iterator it = CustomFactories.begin(); it != CustomFactories.end(); ++it)
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
        std::vector<EntresolManager*> temp;
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
                        const std::vector<EntresolManager*>& ManagersToBeAdded)
    {
        this->Construct(PhysicsInfo,
                        SceneType,
                        EngineDataPath,
                        LogFileName,
                        ManagersToBeAdded );

    }

    void Entresol::Construct(const Physics::ManagerConstructionInfo& PhysicsInfo,
                                const String& SceneType,
                                const String& EngineDataPath,
                                const String& GraphicsLogFileName,
                                const std::vector<EntresolManager*>& ManagersToBeAdded )

    {
        //Add default manager factories
        this->AddAllEngineDefaultManagerFactories();
        World::AddAllEngineDefaultManagerFactories();
        //Set some sane Defaults for some values
        this->ManualLoopBreak = 0;

        SetupOgre();
        SetupLogging(GraphicsLogFileName);

        // Load the necessary plugins.
        SubSystemParticleFXPlugin = new Ogre::ParticleFXPlugin();
        Ogre::Root::getSingleton().installPlugin(SubSystemParticleFXPlugin);

        //add each manager that was passed in to the manager list
        for(std::vector<EntresolManager*>::const_iterator iter = ManagersToBeAdded.begin(); iter!= ManagersToBeAdded.end(); iter++)
            { this->AddManager(*iter); }

        //Dummy param list so we can use the auto-added manager types if needed
        NameValuePairList Params;
        // Create and add any managers that have not been taken care of yet.
        if(this->GetResourceManager()==0)
            { this->AddManager(new Resource::ResourceManager(EngineDataPath)); }
        if(this->GetGraphicsManager()==0)
            { this->AddManager(new Graphics::GraphicsManager()); }
        if(this->GetEventManager()==0)
            { this->AddManager(new EventManager()); }
        if(this->GetInputManager()==0)
            { this->AddManager(new Input::InputManager()); }
        if(this->GetUIManager()==0)
            { this->AddManager(new UI::UIManager()); }
        if(this->GetMeshManager()==0)
            { this->AddManager(new Graphics::MeshManager()); }
        if(this->GetTextureManager()==0)
            { this->AddManager(new Graphics::TextureManager()); }
        if(this->GetCollisionShapeManager()==0)
            { this->AddManager(new Physics::CollisionShapeManager()); }

        #ifdef ENABLE_OALS_AUDIO_IMPLEMENTATION
        if(this->GetAudioManager()==0)
            { this->AddManager( this->CreateManager("OALSAudioManager",Params,false) ); }
        #endif //ENABLE_OALS_AUDIO_IMPLEMENTATION

        // This Tests various assumptions about the way the platform works, and will not act
        SanityChecks();
    }

    void Entresol::ConstructFromXML(const String& EngineDataPath, const Mezzanine::ArchiveType ArchType, const String& InitializerFile)
    {
        //Add default manager factories
        this->AddAllEngineDefaultManagerFactories();
        World::AddAllEngineDefaultManagerFactories();
        //Set some sane Defaults for some values.
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
        if(Resource::ResourceManager::SingletonValid())
            { AddManager(Resource::ResourceManager::GetSingletonPtr()); }
        else
            { AddManager(new Resource::ResourceManager(EngineDataPath, ArchType)); }

        // Open and load the initializer doc.
        Resource::ResourceManager* ResourceMan = GetResourceManager();
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
                Boole GroupRecursive = false;
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
        if( !ResourceInit.empty() ) {
            /// @todo Replace this stack allocated stream for one initialized from the Resource Manager, after the system is ready.
            Resource::FileStream ResourceStream(ResourceInit,EngineDataPath);
            XML::Document ResourceDoc;
            ResourceDoc.Load(ResourceStream);
            // Get an iterator to the first resource group node, and declare them all.
            XML::Node ResourceLocations = ResourceDoc.GetChild("ResourceLocations");
            for( XML::NodeIterator GroupIt = ResourceLocations.begin() ; GroupIt != ResourceLocations.end() ; ++GroupIt )
            {
                String GroupName, GroupPath;
                ArchiveType GroupType = AT_Invalid;
                Boole GroupRecursive = false;
                // Get the group path
                CurrAttrib = (*GroupIt).GetAttribute("GroupPath");
                if(!CurrAttrib.Empty())
                    GroupPath = CurrAttrib.AsString();
                // Get the group type
                CurrAttrib = (*GroupIt).GetAttribute("GroupType");
                if(!CurrAttrib.Empty())
                    GroupType = Resource::ResourceManager::GetArchiveTypeFromString(CurrAttrib.AsString());
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
        if(!GUIInit.empty()) {
            /// @todo This is currently not implemented.
        }

        SanityChecks();
    }


    void Entresol::SanityChecks()
    {
        // Crossplatform::WaitMilliseconds(1500);
        //Perform a Test that only needs to be done once for the SDL/userinputevent system.`
        /*Log("Verifying size of userinput events:");
        Log(sizeof(Input::InputCode));
        Log(sizeof(SDL_Scancode));
        Log(sizeof(int));//*/
        if(sizeof(Input::InputCode) != sizeof(SDL_Scancode)) {
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"User input subsystem Event Sizes Don't match, userinput subsystem will go be buggier than a highschool fortran class.");
        }

        if(sizeof(Input::InputCode) != sizeof(int)) {
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Internal User input subsystem Event Sizes Don't match, userinput subsystem cannot function.");
        }
    }

    Boole Entresol::VerifyManagerInitializations()
    {
        std::vector<String> ManagerNames;
        for (std::list< EntresolManager* >::iterator Iter=this->ManagerList.begin(); Iter!=this->ManagerList.end(); ++Iter )
        {
            if(!(*Iter)->IsInitialized()) {
                ManagerNames.push_back( (*Iter)->GetInterfaceTypeAsString() );
            }
        }

        if( ManagerNames.empty() ) {
            return true;
        }else{
            StringStream ExceptionStream;
            if( 1 == ManagerNames.size() ) {
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
        this->DestroyAllWorlds();
        World::DestroyAllManagerFactories();
        this->DestroyAllManagers();
        this->DestroyAllManagerFactories();
        this->DestroyLogging();

        this->DestroyOgre();
        this->DestroySDL();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void Entresol::PauseAllWorlds(Boole Pause)
    {
        for( WorldContainerIterator it = this->Worlds.begin(); it != this->Worlds.end(); it++ )
        {
            (*it)->GetPhysicsManager()->PauseSimulation(Pause);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Initialization
    ///////////////////////////////////////

    void Entresol::EngineInit( const Boole &CallMainLoop )
    {
        Graphics::GraphicsManager::GetSingletonPtr()->Initialize();
        for (std::list< EntresolManager* >::iterator Iter=this->ManagerList.begin(); Iter!=this->ManagerList.end(); ++Iter )
        {
            StringStream InitStream;
            InitStream << "Initializing " << (*Iter)->GetInterfaceTypeAsString() << "." << endl;
            this->_Log( InitStream.str() );
            if( (*Iter)->GetInterfaceType() != ManagerBase::MT_GraphicsManager ) {
                (*Iter)->Initialize();
            }
        }

        if(CallMainLoop)
            { this->MainLoop(); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // MainLoop
    ///////////////////////////////////////

    Threading::FrameScheduler& Entresol::GetScheduler()
        { return this->WorkScheduler; }

    void Entresol::MainLoop()
    {
        this->PreMainLoopInit();

        while(!ManualLoopBreak)
            { DoOneFrame(); } //Main loop

        ManualLoopBreak = 0;
    }

    void Entresol::PreMainLoopInit()
    {
        VerifyManagerInitializations();

        for( WorldContainerIterator it = this->Worlds.begin(); it != this->Worlds.end(); it++ )
        {
            (*it)->GetPhysicsManager()->MainLoopInitialize();
            (*it)->GetAreaEffectManager()->MainLoopInitialize();
        }
    }

    void Entresol::DoOneFrame()
    {
        #ifdef MEZZDEBUG
        WorkScheduler.GetLog() << "<FrameCounterStart Frame=\"" << WorkScheduler.GetFrameCount() << "\" Time=\"" << GetTimeStamp()<< "\" />" << endl;
        #endif
        WorkScheduler.RunAllMonopolies(); //1
        WorkScheduler.CreateThreads();    //2
        WorkScheduler.RunMainThreadWork();//3
        WorkScheduler.JoinAllThreads();   //4
        WorkScheduler.ResetAllWorkUnits();//5
        #ifdef MEZZDEBUG
        WorkScheduler.GetLog() << "<FrameCounterPrePause Frame=\"" << WorkScheduler.GetFrameCount() << "\" Time=\"" << GetTimeStamp()<< "\" />" << endl;
        #endif
        WorkScheduler.WaitUntilNextFrame(); //6
        #ifdef MEZZDEBUG
        WorkScheduler.GetLog() << "<FrameCounterEnd Frame=\"" << WorkScheduler.GetFrameCount() << "\" Time=\"" << GetTimeStamp()<< "\" />" << endl;
        #endif
    }

    void Entresol::BreakMainLoop(Boole Break)
    {
        if(Break) {
            while(!ManualLoopBreak)
                { Threading::AtomicCompareAndSwap32(&ManualLoopBreak,0,1); }
        } else {
            while(ManualLoopBreak)
                { Threading::AtomicCompareAndSwap32(&ManualLoopBreak,1,0); }
        }
    }

    Whole Entresol::GetFrameCount() const
        { return WorkScheduler.GetFrameCount(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Simple get and Set functions
    ///////////////////////////////////////

    void Entresol::SetTargetFrameRate(const Whole NewFrameRate)
        { this->SetTargetFrameTimeMicroseconds( Real(1000000) / Real(NewFrameRate) ); }

    void Entresol::SetTargetFrameTimeMilliseconds(const Whole NewTargetTime)
        { this->SetTargetFrameTimeMicroseconds( NewTargetTime * 1000 ); }

    void Entresol::SetTargetFrameTimeMicroseconds(const Whole NewTargetTime)
        { WorkScheduler.SetFrameLength(NewTargetTime); }

    Whole Entresol::GetTargetFrameTimeMilliseconds() const
        { return WorkScheduler.GetFrameLength()*0.001; }

    Whole Entresol::GetTargetFrameTimeMicroseconds() const
        { return WorkScheduler.GetFrameLength(); }

    Whole Entresol::GetLastFrameTimeMilliseconds() const
        { return WorkScheduler.GetLastFrameTime()*0.001; }

    Whole Entresol::GetLastFrameTimeMicroseconds() const
        { return WorkScheduler.GetLastFrameTime(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Factory Management

    void Entresol::AddManagerFactory(EntresolManagerFactory* ToBeAdded)
    {
        this->ManagerFactories.insert(std::pair<String,EntresolManagerFactory*>(ToBeAdded->GetManagerImplName(),ToBeAdded));
    }

    void Entresol::RemoveManagerFactory(EntresolManagerFactory* ToBeRemoved)
    {
        this->RemoveManagerFactory(ToBeRemoved->GetManagerImplName());
    }

    void Entresol::RemoveManagerFactory(const String& ImplName)
    {
        ManagerFactoryIterator ManIt = this->ManagerFactories.find(ImplName);
        if( ManIt != this->ManagerFactories.end() )
            { this->ManagerFactories.erase(ManIt); }
    }

    void Entresol::DestroyManagerFactory(EntresolManagerFactory* ToBeRemoved)
    {
        this->DestroyManagerFactory(ToBeRemoved->GetManagerImplName());
    }

    void Entresol::DestroyManagerFactory(const String& ImplName)
    {
        ManagerFactoryIterator ManIt = this->ManagerFactories.find(ImplName);
        if( ManIt != this->ManagerFactories.end() ) {
            delete ManIt->second;
            this->ManagerFactories.erase(ManIt);
        }
    }

    void Entresol::DestroyAllManagerFactories()
    {
        for( ManagerFactoryIterator ManIt = this->ManagerFactories.begin() ; ManIt != this->ManagerFactories.end() ; ++ManIt )
            { delete (*ManIt).second; }
        this->ManagerFactories.clear();
    }

    void Entresol::AddAllEngineDefaultManagerFactories()
    {
        ManagerFactoryIterator ManIt;
        //DefaultCollisionShapeManager
        ManIt = this->ManagerFactories.find("DefaultCollisionShapeManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new Physics::DefaultCollisionShapeManagerFactory());
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
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new Graphics::DefaultMeshManagerFactory());
        //DefaultNetworkManager
        ManIt = this->ManagerFactories.find("DefaultNetworkManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new DefaultNetworkManagerFactory());
        //DefaultResourceManager
        ManIt = this->ManagerFactories.find("DefaultResourceManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new Resource::DefaultResourceManagerFactory());
        //DefaultTextureManager
        ManIt = this->ManagerFactories.find("DefaultTextureManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new Graphics::DefaultTextureManagerFactory());
        //DefaultUIManager
        ManIt = this->ManagerFactories.find("DefaultUIManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new UI::DefaultUIManagerFactory());

        #ifdef ENABLE_OALS_AUDIO_IMPLEMENTATION
        //OALSAudioManager
        ManIt = this->ManagerFactories.find("OALSAudioManager");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new Audio::OALS::OALSAudioManagerFactory());
        #endif //ENABLE_OALS_AUDIO_IMPLEMENTATION

        #ifdef MEZZLUA51
        ManIt = this->ManagerFactories.find("Lua51ScriptingEngine");
        if( ManIt == this->ManagerFactories.end() ) this->AddManagerFactory(new Scripting::Lua::Lua51ScriptingEngineFactory());
        #endif

    }

    ///////////////////////////////////////////////////////////////////////////////
    // Upper Management

    EntresolManager* Entresol::CreateManager(const String& ManagerImplName, NameValuePairList& Params, Boole AddToWorld)
    {
        ManagerFactoryIterator ManIt = this->ManagerFactories.find(ManagerImplName);
        if( ManIt == this->ManagerFactories.end() )
        {
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to create manager of type \"" + ManagerImplName + "\", which has no factory registered.");
        }
        EntresolManager* NewMan = (*ManIt).second->CreateManager(Params);
        if(AddToWorld)
            this->AddManager(NewMan);
        return NewMan;
    }

    EntresolManager* Entresol::CreateManager(const String& ManagerImplName, XML::Node& XMLNode, Boole AddToWorld)
    {
        ManagerFactoryIterator ManIt = this->ManagerFactories.find(ManagerImplName);
        if( ManIt == this->ManagerFactories.end() )
        {
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to create manager of type \"" + ManagerImplName + "\", which has no factory registered.");
        }
        EntresolManager* NewMan = (*ManIt).second->CreateManager(XMLNode);
        if(AddToWorld)
            this->AddManager(NewMan);
        return NewMan;
    }

    void Entresol::DestroyManager(EntresolManager* ToBeDestroyed)
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
            EntresolManager* Current = this->ManagerList.front();
            #ifdef MEZZDEBUG
            this->_Log("Deleting " + Current->GetInterfaceTypeAsString() + ".");
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

    void Entresol::AddManager(EntresolManager* ManagerToAdd)
    {
        #ifdef MEZZDEBUG
        this->_Log("Adding " + ManagerToAdd->GetInterfaceTypeAsString() + ".\n");
        #endif
        // We have to verify the manager is unique.  A number of issues can arrise if a manager is double inserted.
        for( std::list< EntresolManager* >::iterator ManIter = this->ManagerList.begin() ; ManIter != this->ManagerList.end() ; ++ManIter )
        {
            if( (*ManIter) == ManagerToAdd )
                return;
        }
        this->ManagerList.push_back(ManagerToAdd);
    }

    EntresolManager* Entresol::GetManager(const ManagerBase::ManagerType RetrieveType, UInt16 WhichOne)
    {
        for(std::list< EntresolManager* >::iterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
        {
            if( (*ManIter)->GetInterfaceType() == RetrieveType )
            {
                if( 0 == WhichOne ) return *ManIter; // we use our copy of WhichOne as a countdown to 0
                else --WhichOne;
            }
        }
        return NULL;
    }

    void Entresol::RemoveManager(EntresolManager* ManagerToRemove)
    {
        for( std::list< EntresolManager* >::iterator ManIter = this->ManagerList.begin() ; ManIter != this->ManagerList.end() ; ++ManIter )
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
        for(std::list< EntresolManager* >::iterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
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

    Audio::AudioManager* Entresol::GetAudioManager(const UInt16 WhichOne)
    {
        return dynamic_cast<Audio::AudioManager*>( this->GetManager(ManagerBase::MT_AudioManager, WhichOne) );
    }

    Physics::CollisionShapeManager* Entresol::GetCollisionShapeManager(const UInt16 WhichOne)
    {
        return dynamic_cast<Physics::CollisionShapeManager*>( this->GetManager(ManagerBase::MT_CollisionShapeManager, WhichOne) );
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

    Graphics::MeshManager* Entresol::GetMeshManager(const UInt16 WhichOne)
    {
        return dynamic_cast<Graphics::MeshManager*>( this->GetManager(ManagerBase::MT_MeshManager, WhichOne) );
    }
    #ifdef MEZZNETWORK
    NetworkManager* Entresol::GetNetworkManager(const UInt16 WhichOne)
    {
        return dynamic_cast<NetworkManager*>( this->GetManager(ManagerBase::MT_NetworkManager, WhichOne) );
    }
    #endif

    Resource::ResourceManager* Entresol::GetResourceManager(const UInt16 WhichOne)
    {
        return dynamic_cast<Resource::ResourceManager*>( this->GetManager(ManagerBase::MT_ResourceManager, WhichOne) );
    }

    Graphics::TextureManager* Entresol::GetTextureManager(const UInt16 WhichOne)
    {
        return dynamic_cast<Graphics::TextureManager*>( this->GetManager(ManagerBase::MT_TextureManager, WhichOne) );
    }

    UI::UIManager* Entresol::GetUIManager(const UInt16 WhichOne)
    {
        return dynamic_cast<UI::UIManager*>( this->GetManager(ManagerBase::MT_UIManager, WhichOne) );
    }

    Scripting::iScriptingManager* Entresol::GetScriptingManager(const UInt16 WhichOne)
        { return dynamic_cast<Scripting::iScriptingManager*>( this->GetManager(ManagerBase::MT_ScriptingManager, WhichOne) ); }

    ///////////////////////////////////////////////////////////////////////////////
    // World Management

    World* Entresol::CreateWorld(const String& WorldName)
    {
        World* NewWorld = new World(WorldName);
        this->AddWorld(NewWorld);
        return NewWorld;
    }

    World* Entresol::CreateWorld(const String& WorldName, const std::vector <WorldManager*>& ManagerToBeAdded)
    {
        World* NewWorld = new World(WorldName, ManagerToBeAdded);
        this->AddWorld(NewWorld);
        return NewWorld;
    }

    World* Entresol::CreateWorld(const String& WorldName, const Physics::ManagerConstructionInfo& PhysicsInfo,const String& SceneType)
    {
        World* NewWorld = new World(WorldName, PhysicsInfo, SceneType );
        this->AddWorld(NewWorld);
        return NewWorld;
    }

    World* Entresol::CreateWorld(const String& WorldName, const std::vector <WorldManager*>& ManagerToBeAdded,
        const Physics::ManagerConstructionInfo& PhysicsInfo,const String& SceneType)
    {
        World* NewWorld = new World(WorldName, ManagerToBeAdded, PhysicsInfo, SceneType );
        this->AddWorld(NewWorld);
        return NewWorld;
    }

    void Entresol::AddWorld(World* WorldToBeAdded)
    {
        Worlds.push_back(WorldToBeAdded);
    }

    World* Entresol::GetWorld(const String& WorldName)
    {
        for( ConstWorldContainerIterator it = this->Worlds.begin() ; it != this->Worlds.end() ; it++ )
        {
            World* w = (*it);
            if ( WorldName == w->GetName() ) {
                return w;
            }
        }
        return NULL;
    }

    World* Entresol::GetWorld(const Whole Index)
    {
        return this->Worlds.at(Index);
    }

    UInt16 Entresol::GetNumWorlds()
    {
        return this->Worlds.size();
    }

    World* Entresol::RemoveWorld(World* WorldToBeRemoved)
    {
        for( WorldContainerIterator it = this->Worlds.begin(); it != this->Worlds.end(); it++ )
        {
            World* w = (*it);
            if ( WorldToBeRemoved == w ) {
                this->Worlds.erase(it);
                return w;
            }
        }
        return NULL;
    }

    World* Entresol::RemoveWorldByName(const String& WorldName)
    {
        for( WorldContainerIterator it = this->Worlds.begin(); it != this->Worlds.end(); it++ )
        {
            World* w = (*it);
            if ( WorldName == w->GetName() ) {
                this->Worlds.erase(it);
                return w;
            }
        }
        return NULL;
    }

    void Entresol::RemoveAllWorlds()
    {
        this->Worlds.clear();
    }

    void Entresol::DestroyWorld(World* WorldToBeDestroyed)
    {
        for( WorldContainerIterator it = this->Worlds.begin(); it != this->Worlds.end(); it++ )
        {
            World* w = (*it);
            if ( WorldToBeDestroyed == w ) {
                delete w;
                this->Worlds.erase(it);
                return;
            }
        }
    }

    void Entresol::DestroyWorldByName(const String& WorldName)
    {
        for( WorldContainerIterator it = this->Worlds.begin(); it != this->Worlds.end(); it++ )
        {
            World* w = (*it);
            if ( WorldName == w->GetName() ) {
                delete w;
                this->Worlds.erase(it);
                return;
            }
        }
    }

    void Entresol::DestroyAllWorlds()
    {
        for( WorldContainerIterator it = this->Worlds.begin(); it != this->Worlds.end(); it++ )
        {
            World* w = (*it);
            delete w;
        }
        this->Worlds.clear();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Logging

    void Entresol::_LogString(const String& Message)
    {
        // if it is in the Audiologs then it has already happened so it needs to be logged first
        if( Message.size() > 0 )
            { this->_GetLogStream() << Message; }
    }

    Logger& Entresol::_GetLogStream(Threading::ThreadId ID)
    {
        Threading::FrameScheduler::Resource* AlmostResults = this->WorkScheduler.GetThreadResource(ID);
        if(AlmostResults)
            { return AlmostResults->GetUsableLogger(); }
        else
            { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Could not access thread Specific Logger from invalid thread."); }
    }

    Threading::LogAggregator* Entresol::_GetLogAggregator()
        { return this->Aggregator; }
}
#endif
