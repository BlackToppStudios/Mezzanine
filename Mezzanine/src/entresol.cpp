// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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

#include "entresol.h"
#include "entresolmanager.h"
#include "world.h"

// Manager Includes
#include "entitymanager.h"
#include "Graphics/graphicsmanager.h"
#include "Graphics/meshmanager.h"
#include "Graphics/scenemanager.h"
#include "Graphics/texturemanager.h"
#include "Input/inputmanager.h"
#include "Network/networkmanager.h"
#include "Physics/collisionshapemanager.h"
#include "Physics/physicsmanager.h"
#include "Resource/resourcemanager.h"
#include "UI/uimanager.h"

#include "filestream.h"

// Enabled implementation includes
#ifdef ENABLE_OALS_AUDIO_IMPLEMENTATION
    #include "Audio/OALS/oalsaudiomanagerfactory.h"
#endif // ENABLE_OALS_AUDIO_IMPLEMENTATION

#ifdef MEZZLUA51
    #include "Scripting/Lua51/lua51scriptingengine.h"
#endif // MEZZLUA51

#include <OgreParticleFXPlugin.h>

#include <SDL.h>

#include <OgreRoot.h>
#include <OgreLogManager.h>
#include <OgreParticleFXPlugin.h>

#include <sstream>
#include <string>
#include <cassert>

namespace Mezzanine
{
    template<> Entresol* Singleton<Entresol>::SingletonPtr = NULL;
    Entresol::ManagerFactoryMap Entresol::ManagerFactories;

    ///////////////////////////////////////////////////////////////////////////////
    // Entresol Constructors and Destructors

    /// @todo In the Entrosol, remove all references to a plugins file
    Entresol::Entresol()
    {
        ManagerVec temp;
        this->Construct(".","Mezzanine.log",temp);
    }

    Entresol::Entresol(const String& EngineDataPath, const Resource::ArchiveType ArchType, const String& InitializerFile)
    {
        if( String::npos != InitializerFile.find(".mxi") ) {
            this->ConstructFromXML(EngineDataPath, ArchType, InitializerFile);
        }else{
            MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Attempting to initialze Mezzanine from an unsupported file type.");
        }
    }

    Entresol::Entresol(ManagerFactoryVec& CustomFactories, const String& EngineDataPath, const Resource::ArchiveType ArchType, const String& InitializerFile)
    {
        for(ManagerFactoryVec::iterator it = CustomFactories.begin(); it != CustomFactories.end(); ++it)
        {
            this->AddManagerFactory( (*it) );
        }

        if( String::npos != InitializerFile.find(".mxi") ) {
            this->ConstructFromXML(EngineDataPath, ArchType, InitializerFile);
        }else{
            MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Attempting to initialze Mezzanine from an unsupported file type.");
        }
    }

    Entresol::Entresol(const String& EngineDataPath, const String& LogFileName)
    {
        ManagerVec temp;
        this->Construct(EngineDataPath, LogFileName, temp);
    }

    Entresol::Entresol(const String& EngineDataPath, const String& LogFileName, const ManagerVec& ManagersToBeAdded)
    {
        this->Construct(EngineDataPath, LogFileName, ManagersToBeAdded);
    }

    Entresol::~Entresol()
    {
        // Gracefully shut down
        this->Deinitialize();

        // Destroy objects
        this->DestroyAllWorlds();
        this->DestroyAllManagers();
        this->DestroyLogging();

        // Unregister types
        World::DestroyAllManagerFactories();
        Entresol::DestroyAllManagerFactories();
        EntityManager::DestroyAllEntityFactories();

        // Shutdown internals
        this->DestroyOgre();
        this->DestroySDL();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Initialization and Deinitialization Methods

    void Entresol::Construct(const String& EngineDataPath, const String& GraphicsLogFileName, const ManagerVec& ManagersToBeAdded)
    {
        //Add default manager factories
        Entresol::AddAllEngineDefaultManagerFactories();
        World::AddAllEngineDefaultManagerFactories();
        EntityManager::AddAllDefaultEntityFactories();
        //Set some sane Defaults for some values
        this->ManualLoopBreak = 0;

        this->SetupOgre();
        this->SetupLogging(GraphicsLogFileName);

        // Load the necessary plugins.
        SubSystemParticleFXPlugin = new Ogre::ParticleFXPlugin();
        Ogre::Root::getSingleton().installPlugin(SubSystemParticleFXPlugin);

        //add each manager that was passed in to the manager list
        for( ConstManagerVecIterator iter = ManagersToBeAdded.begin() ; iter!= ManagersToBeAdded.end() ; iter++ )
            { this->AddManager(*iter); }

        //Dummy param list so we can use the auto-added manager types if needed
        NameValuePairList Params;
        // Create and add any managers that have not been taken care of yet.
        if( this->GetManager(ManagerBase::MT_ResourceManager) == 0 ) {
            Params.push_back( std::pair<String,String>("EngineDataPath",EngineDataPath) );
            this->CreateManager( "DefaultResourceManager", Params );
            Params.clear();
        }
        if( this->GetManager(ManagerBase::MT_GraphicsManager) == 0 ) {
            this->CreateManager( "DefaultGraphicsManager", Params );
        }
        if( this->GetManager(ManagerBase::MT_InputManager) == 0 ) {
            this->CreateManager( "DefaultInputManager", Params );
        }
        if( this->GetManager(ManagerBase::MT_UIManager) == 0 ) {
            this->CreateManager( "DefaultUIManager", Params );
        }
        if( this->GetManager(ManagerBase::MT_MeshManager) == 0 ) {
            this->CreateManager( "DefaultMeshManager", Params );
        }
        if( this->GetManager(ManagerBase::MT_TextureManager) == 0 ) {
            this->CreateManager( "DefaultTextureManager", Params );
        }
        if( this->GetManager(ManagerBase::MT_CollisionShapeManager) == 0 ) {
            this->CreateManager( "DefaultCollisionShapeManager", Params );
        }

        #ifdef ENABLE_OALS_AUDIO_IMPLEMENTATION
        if( this->GetManager(ManagerBase::MT_AudioManager) == 0 ) {
            this->CreateManager("OALSAudioManager",Params);
        }
        #endif //ENABLE_OALS_AUDIO_IMPLEMENTATION

        // This Tests various assumptions about the way the platform works, and will not act
        this->SanityChecks();
    }

    void Entresol::ConstructFromXML(const String& EngineDataPath, const Resource::ArchiveType ArchType, const String& InitializerFile)
    {
        //Add default manager factories
        Entresol::AddAllEngineDefaultManagerFactories();
        World::AddAllEngineDefaultManagerFactories();
        EntityManager::AddAllDefaultEntityFactories();
        //Set some sane Defaults for some values.
        this->ManualLoopBreak = false;

        // Create Ogre.
        this->SetupOgre();

        // Load the necessary plugins.
        this->SubSystemParticleFXPlugin = new Ogre::ParticleFXPlugin();
        Ogre::Root::getSingleton().installPlugin(this->SubSystemParticleFXPlugin);

        // Set up the data we'll be populating.
        XML::Attribute CurrAttrib;
        String GUIInit, ResourceInit, PluginsInit, LogFileName;
        String PluginExtension, PluginPath;

        // Create or set the resource manager.
        /// @todo This currently forces our default resource manager to be constructed, which isn't in line with our factory/initiailzation design.
        /// This should be addressed somehow.
        if( Resource::ResourceManager::SingletonValid() ) {
            this->AddManager( Resource::ResourceManager::GetSingletonPtr() );
        }else{
            this->AddManager( new Resource::ResourceManager(EngineDataPath, ArchType) );
        }

        // Open and load the initializer doc.
        Resource::ResourceManager* ResourceMan = static_cast<Resource::ResourceManager*>( this->GetManager(ManagerBase::MT_ResourceManager) );
        FileStream InitStream(InitializerFile,EngineDataPath);
        XML::Document InitDoc;
        XML::ParseResult DocResult = InitDoc.Load(InitStream);
        if( DocResult.Status != XML::StatusOk ) {
            StringStream ExceptionStream;
            ExceptionStream << "Failed to parse XML file \"" << InitializerFile << "\".";
            MEZZ_EXCEPTION(ExceptionBase::SYNTAX_ERROR_EXCEPTION_XML,ExceptionStream.str());
        }
        XML::Node InitRoot = InitDoc.GetChild("InitializerRoot");
        if( InitRoot.Empty() ) {
            StringStream ExceptionStream;
            ExceptionStream << "Failed to find expected Root node in \"" << InitializerFile << "\".";
            MEZZ_EXCEPTION(ExceptionBase::SYNTAX_ERROR_EXCEPTION_XML,ExceptionStream.str());
        }

        // Get the world settings and set them.
        XML::Node CoreConfig = InitRoot.GetChild("CoreConfig");
        for( XML::NodeIterator SetIt = CoreConfig.begin() ; SetIt != CoreConfig.end() ; ++SetIt )
        {
            String SecName = (*SetIt).Name();
            if( "FrameSettings" == SecName ) {
                CurrAttrib = (*SetIt).GetAttribute("TargetFrameRate");
                if(CurrAttrib.Empty()) {
                    CurrAttrib = (*SetIt).GetAttribute("TargetFrameTime");
                    if(!CurrAttrib.Empty())
                        SetTargetFrameTimeMicroseconds(CurrAttrib.AsWhole());
                }else{
                    this->SetTargetFrameRate(CurrAttrib.AsWhole());
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::SYNTAX_ERROR_EXCEPTION_XML,String("Unknown WorldSetting ")+SecName);
            }

        }

        this->SetupLogging(LogFileName);

        // Get the other initializer files we'll be using, since we'll need the plugins initializer.
        XML::Node InitFiles = InitRoot.GetChild("OtherInitializers");
        for( XML::NodeIterator InitIt = InitFiles.begin() ; InitIt != InitFiles.end() ; ++InitIt )
        {
            String InitFileName = (*InitIt).Name();
            if( "PluginInit" == InitFileName ) {
                CurrAttrib = (*InitIt).GetAttribute("FileName");
                if(!CurrAttrib.Empty())
                    PluginsInit = CurrAttrib.AsString();
            }else if( "ResourceInit" == InitFileName ) {
                CurrAttrib = (*InitIt).GetAttribute("FileName");
                if(!CurrAttrib.Empty())
                    ResourceInit = CurrAttrib.AsString();
            }else if( "GUIInit" == InitFileName ) {
                CurrAttrib = (*InitIt).GetAttribute("FileName");
                if(!CurrAttrib.Empty())
                    GUIInit = CurrAttrib.AsString();
            }
        }

        // Create the requested managers and set their necessary values.
        XML::Node Managers = InitRoot.GetChild("Managers");
        for( XML::NodeIterator ManIt = Managers.begin() ; ManIt != Managers.end() ; ++ManIt )
        {
            this->CreateManager( (*ManIt).Name(), (*ManIt) );
        }

        // Load additional resource groups
        if( !ResourceInit.empty() ) {
            /// @todo Replace this stack allocated stream for one initialized from the Resource Manager, after the system is ready.
            FileStream ResourceStream(ResourceInit,EngineDataPath);
            XML::Document ResourceDoc;
            ResourceDoc.Load(ResourceStream);
            // Get an iterator to the first resource group node, and declare them all.
            XML::Node ResourceLocations = ResourceDoc.GetChild("ResourceLocations");
            for( XML::NodeIterator GroupIt = ResourceLocations.begin() ; GroupIt != ResourceLocations.end() ; ++GroupIt )
            {
                String GroupName, GroupPath;
                Resource::ArchiveType GroupType = Resource::AT_Invalid;
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

        this->SanityChecks();
    }

    void Entresol::SetupOgre()
    {
        if( Ogre::Root::getSingletonPtr() == NULL ) {
            Ogre::Root* OgreCore = new Ogre::Root("","","");
            // Sanity checks for Ogre here?  It's a singleton so don't need to worry about storing the pointer.
        }
    }

    void Entresol::DestroyOgre()
    {
        #ifdef MEZZ_LINUX
        //assert( !( SDL_WasInit(0) | SDL_INIT_VIDEO ) && "SDL already shut down.  SDL Shutdown forces x11 unload, which Ogre needs for it's shutdown." );
        #endif
        if( Ogre::Root::getSingletonPtr() != NULL ) {
            delete Ogre::Root::getSingletonPtr();
        }
        if( this->SubSystemParticleFXPlugin != NULL ) {
            delete this->SubSystemParticleFXPlugin;
        }
    }

    void Entresol::DestroySDL()
    {
        #ifdef MEZZ_LINUX
        // Fail is SDL is de-intialized before Ogre, but only if Ogre has been initialized.
        assert( NULL == Ogre::Root::getSingletonPtr() // ( (SDL_WasInit(0) | SDL_INIT_VIDEO)
                && "SDL already shut down.  SDL Shutdown forces x11 unload, which Ogre needs for it's shutdown." );
        #endif
        SDL_Quit();
    }

    void Entresol::SetupLogging(const String& OgreLogFileName)
    {
        this->SetupOgreLogging(OgreLogFileName);
        this->SetupInternalLogging();
    }

    void Entresol::SetupOgreLogging(const String& OgreLogFileName)
    {
        /// @todo Allow the FrameScheduler Log target to be inspected and changed here
        Ogre::LogManager* OgreLogs = Ogre::LogManager::getSingletonPtr();
        if( NULL == OgreLogs ) {
            OgreLogs = new Ogre::LogManager();
        }

        if(!OgreLogFileName.empty()) {
            OgreLogs->createLog(String("Graphics")+OgreLogFileName,true,true,true);
        }else{
            OgreLogs->createLog("GraphicsMezzanine.log",true,true,true);
        }
    }

    void Entresol::SetupInternalLogging()
    {
        this->Aggregator = new Threading::LogAggregator();
        this->Aggregator->SetAggregationTarget(&WorkScheduler);
        this->WorkScheduler.AddWorkUnitMain(Aggregator, "LogAggregator");
    }

    void Entresol::DestroyLogging()
    {
        // One last aggregation
        /*Threading::DefaultThreadSpecificStorage::Type LogResource(&this->WorkScheduler);
        this->Aggregator->DoWork(LogResource);// */
        Threading::DefaultThreadSpecificStorage::Type* LogResource = this->WorkScheduler.GetThreadResource();
        this->WorkScheduler.GetLog() << LogResource->GetUsableLogger().str();
        LogResource->SwapAllBufferedResources();
        this->WorkScheduler.GetLog() << LogResource->GetUsableLogger().str();
        //this->WorkScheduler.ForceLogFlush();

        this->WorkScheduler.RemoveWorkUnitMain(Aggregator);
        delete this->Aggregator;
    }

    void Entresol::SanityChecks()
    {
        // Crossplatform::WaitMilliseconds(1500);
        //Perform a Test that only needs to be done once for the SDL/userinputevent system.`
        /*Log("Verifying size of userinput events:");
        Log(sizeof(Input::InputCode));
        Log(sizeof(SDL_Scancode));
        Log(sizeof(int));// */
        if(sizeof(Input::InputCode) != sizeof(SDL_Scancode)) {
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"User input subsystem Event Sizes Don't match, userinput subsystem will go be buggier than a highschool fortran class.");
        }

        if(sizeof(Input::InputCode) != sizeof(int)) {
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Internal User input subsystem Event Sizes Don't match, userinput subsystem cannot function.");
        }
    }

    Boole Entresol::VerifyManagerInitializations()
    {
        std::vector<String> ManagerNames;
        for( ManagerIterator Iter=this->ManagerList.begin(); Iter!=this->ManagerList.end(); ++Iter )
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
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,ExceptionStream.str());
            return false;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Threading::FrameScheduler& Entresol::GetScheduler()
        { return this->WorkScheduler; }

    ///////////////////////////////////////////////////////////////////////////////
    // Initialization and Deinitialization

    void Entresol::Initialize(const Boole InitWorlds)
    {
        Graphics::GraphicsManager::GetSingletonPtr()->Initialize();
        for( ManagerIterator Iter = this->ManagerList.begin() ; Iter != this->ManagerList.end() ; ++Iter )
        {
            StringStream InitStream;
            InitStream << "Initializing " << (*Iter)->GetImplementationTypeName() << " as " << (*Iter)->GetInterfaceTypeAsString() << "." << std::endl;
            this->_Log( InitStream.str() );
            if( (*Iter)->GetInterfaceType() != ManagerBase::MT_GraphicsManager ) {
                (*Iter)->Initialize();
            }
        }
        // Worlds in part depend on Entresol manager functionality, so they must be second.
        if( InitWorlds ) {
            for( WorldIterator WorldIt = this->Worlds.begin() ; WorldIt != this->Worlds.end() ; ++WorldIt )
            {
                (*WorldIt)->Initialize();
            }
        }
    }

    void Entresol::Deinitialize()
    {
        // Deinit the Worlds first due to dependencies.
        for( WorldIterator WorldIt = this->Worlds.begin() ; WorldIt != this->Worlds.end() ; ++WorldIt )
        {
            (*WorldIt)->Deinitialize();
        }
        // Now the Entresol.
        for( ManagerIterator Iter = this->ManagerList.begin() ; Iter != this->ManagerList.end() ; ++Iter )
        {
            StringStream DeinitStream;
            DeinitStream << "Deinitializing " << (*Iter)->GetImplementationTypeName() << " as " << (*Iter)->GetInterfaceTypeAsString() << "." << std::endl;
            this->_Log( DeinitStream.str() );
            (*Iter)->Deinitialize();
        }
    }

    void Entresol::PreMainLoopInit()
    {
        this->VerifyManagerInitializations();

        for( WorldIterator it = this->Worlds.begin(); it != this->Worlds.end(); it++ )
            { (*it)->PreMainLoopInit(); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // MainLoop

    void Entresol::MainLoop()
    {
        this->PreMainLoopInit();

        while( !this->ManualLoopBreak )
            { this->DoOneFrame(); } //Main loop

        this->ManualLoopBreak = 0;
    }

    void Entresol::DoOneFrame()
    {
        #ifdef MEZZDEBUG
        WorkScheduler.GetLog() << "<FrameCounterStart Frame=\"" << WorkScheduler.GetFrameCount() << "\" Time=\"" << GetTimeStamp()<< "\" />" << std::endl;
        #endif
        WorkScheduler.RunAllMonopolies(); //1
        WorkScheduler.CreateThreads();    //2
        WorkScheduler.RunMainThreadWork();//3
        WorkScheduler.JoinAllThreads();   //4
        WorkScheduler.ResetAllWorkUnits();//5
        #ifdef MEZZDEBUG
        WorkScheduler.GetLog() << "<FrameCounterPrePause Frame=\"" << WorkScheduler.GetFrameCount() << "\" Time=\"" << GetTimeStamp()<< "\" />" << std::endl;
        #endif
        WorkScheduler.WaitUntilNextFrame(); //6
        #ifdef MEZZDEBUG
        WorkScheduler.GetLog() << "<FrameCounterEnd Frame=\"" << WorkScheduler.GetFrameCount() << "\" Time=\"" << GetTimeStamp()<< "\" />" << std::endl;
        #endif
    }

    void Entresol::BreakMainLoop(Boole Break)
    {
        if(Break) {
            while( !this->ManualLoopBreak )
                { Threading::AtomicCompareAndSwap32(&this->ManualLoopBreak,0,1); }
        }else{
            while( this->ManualLoopBreak )
                { Threading::AtomicCompareAndSwap32(&this->ManualLoopBreak,1,0); }
        }
    }

    Whole Entresol::GetFrameCount() const
        { return this->WorkScheduler.GetFrameCount(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Simple get and Set functions
    ///////////////////////////////////////

    void Entresol::SetTargetFrameRate(const Whole NewFrameRate)
        { this->SetTargetFrameTimeMicroseconds( Real(1000000) / Real(NewFrameRate) ); }

    void Entresol::SetTargetFrameTimeMilliseconds(const Whole NewTargetTime)
        { this->SetTargetFrameTimeMicroseconds( NewTargetTime * 1000 ); }

    void Entresol::SetTargetFrameTimeMicroseconds(const Whole NewTargetTime)
        { this->WorkScheduler.SetFrameLength(NewTargetTime); }

    Whole Entresol::GetTargetFrameTimeMilliseconds() const
        { return this->WorkScheduler.GetFrameLength()*0.001; }

    Whole Entresol::GetTargetFrameTimeMicroseconds() const
        { return this->WorkScheduler.GetFrameLength(); }

    Whole Entresol::GetLastFrameTimeMilliseconds() const
        { return this->WorkScheduler.GetLastFrameTime()*0.001; }

    Whole Entresol::GetLastFrameTimeMicroseconds() const
        { return this->WorkScheduler.GetLastFrameTime(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Factory Management

    void Entresol::AddManagerFactory(EntresolManagerFactory* ToBeAdded)
    {
        Entresol::ManagerFactories.insert(std::pair<String,EntresolManagerFactory*>(ToBeAdded->GetManagerImplName(),ToBeAdded));
    }

    void Entresol::RemoveManagerFactory(EntresolManagerFactory* ToBeRemoved)
    {
        Entresol::RemoveManagerFactory(ToBeRemoved->GetManagerImplName());
    }

    void Entresol::RemoveManagerFactory(const String& ImplName)
    {
        ManagerFactoryIterator ManIt = Entresol::ManagerFactories.find(ImplName);
        if( ManIt != Entresol::ManagerFactories.end() )
            { Entresol::ManagerFactories.erase(ManIt); }
    }

    void Entresol::DestroyManagerFactory(EntresolManagerFactory* ToBeRemoved)
    {
        Entresol::DestroyManagerFactory(ToBeRemoved->GetManagerImplName());
    }

    void Entresol::DestroyManagerFactory(const String& ImplName)
    {
        ManagerFactoryIterator ManIt = Entresol::ManagerFactories.find(ImplName);
        if( ManIt != Entresol::ManagerFactories.end() ) {
            delete ManIt->second;
            Entresol::ManagerFactories.erase(ManIt);
        }
    }

    void Entresol::DestroyAllManagerFactories()
    {
        for( ManagerFactoryIterator ManIt = Entresol::ManagerFactories.begin() ; ManIt != Entresol::ManagerFactories.end() ; ++ManIt )
            { delete (*ManIt).second; }
        Entresol::ManagerFactories.clear();
    }

    void Entresol::AddAllEngineDefaultManagerFactories()
    {
        ManagerFactoryIterator ManIt;
        //DefaultCollisionShapeManager
        ManIt = Entresol::ManagerFactories.find("DefaultCollisionShapeManager");
        if( ManIt == Entresol::ManagerFactories.end() ) Entresol::AddManagerFactory(new Physics::DefaultCollisionShapeManagerFactory());
        //DefaultGraphicsManager
        ManIt = Entresol::ManagerFactories.find("DefaultGraphicsManager");
        if( ManIt == Entresol::ManagerFactories.end() ) Entresol::AddManagerFactory(new Graphics::DefaultGraphicsManagerFactory());
        //DefaultInputManager
        ManIt = Entresol::ManagerFactories.find("DefaultInputManager");
        if( ManIt == Entresol::ManagerFactories.end() ) Entresol::AddManagerFactory(new Input::DefaultInputManagerFactory());
        //DefaultMeshManager
        ManIt = Entresol::ManagerFactories.find("DefaultMeshManager");
        if( ManIt == Entresol::ManagerFactories.end() ) Entresol::AddManagerFactory(new Graphics::DefaultMeshManagerFactory());
        //DefaultResourceManager
        ManIt = Entresol::ManagerFactories.find("DefaultResourceManager");
        if( ManIt == Entresol::ManagerFactories.end() ) Entresol::AddManagerFactory(new Resource::DefaultResourceManagerFactory());
        //DefaultTextureManager
        ManIt = Entresol::ManagerFactories.find("DefaultTextureManager");
        if( ManIt == Entresol::ManagerFactories.end() ) Entresol::AddManagerFactory(new Graphics::DefaultTextureManagerFactory());
        //DefaultUIManager
        ManIt = Entresol::ManagerFactories.find("DefaultUIManager");
        if( ManIt == Entresol::ManagerFactories.end() ) Entresol::AddManagerFactory(new UI::DefaultUIManagerFactory());

        #ifdef ENABLE_OALS_AUDIO_IMPLEMENTATION
        //OALSAudioManager
        ManIt = Entresol::ManagerFactories.find("OALSAudioManager");
        if( ManIt == Entresol::ManagerFactories.end() ) Entresol::AddManagerFactory(new Audio::OALS::OALSAudioManagerFactory());
        #endif //ENABLE_OALS_AUDIO_IMPLEMENTATION

        #ifdef MEZZNETWORK
        //DefaultNetworkManager
        ManIt = Entresol::ManagerFactories.find("DefaultNetworkManager");
        if( ManIt == Entresol::ManagerFactories.end() ) Entresol::AddManagerFactory(new Network::DefaultNetworkManagerFactory());
        #endif //MEZZNETWORK

        #ifdef MEZZLUA51
        ManIt = Entresol::ManagerFactories.find("Lua51ScriptingEngine");
        if( ManIt == Entresol::ManagerFactories.end() ) Entresol::AddManagerFactory(new Scripting::Lua::Lua51ScriptingEngineFactory());
        #endif //MEZZLUA51
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Upper Management

    EntresolManager* Entresol::CreateManager(const String& ManagerImplName, const NameValuePairList& Params, const Boole AddToWorld)
    {
        ManagerFactoryIterator ManIt = Entresol::ManagerFactories.find(ManagerImplName);
        if( ManIt == Entresol::ManagerFactories.end() ) {
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to create manager of type \"" + ManagerImplName + "\", which has no factory registered.");
        }
        EntresolManager* NewMan = (*ManIt).second->CreateManager(Params);
        if(AddToWorld)
            this->AddManager(NewMan);
        return NewMan;
    }

    EntresolManager* Entresol::CreateManager(const String& ManagerImplName, const XML::Node& XMLNode, const Boole AddToWorld)
    {
        ManagerFactoryIterator ManIt = Entresol::ManagerFactories.find(ManagerImplName);
        if( ManIt == Entresol::ManagerFactories.end() ) {
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to create manager of type \"" + ManagerImplName + "\", which has no factory registered.");
        }
        EntresolManager* NewMan = (*ManIt).second->CreateManager(XMLNode);
        if(AddToWorld)
            this->AddManager(NewMan);
        return NewMan;
    }

    void Entresol::DestroyManager(EntresolManager* ToBeDestroyed)
    {
        ManagerFactoryIterator ManIt = Entresol::ManagerFactories.find(ToBeDestroyed->GetImplementationTypeName());
        if( ManIt == Entresol::ManagerFactories.end() ) {
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to destroy manager of type \"" + ToBeDestroyed->GetImplementationTypeName() + "\", which has no factory registered.");
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
            ManagerFactoryIterator ManIt = Entresol::ManagerFactories.find(Current->GetImplementationTypeName());
            if( ManIt == Entresol::ManagerFactories.end() ) {
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to destroy manager of type \"" + Current->GetImplementationTypeName() + "\", which has no factory registered.");
            }else{
                (*ManIt).second->DestroyManager(Current);
            }
        }
        this->ManagerList.clear();// */
        while( !(this->ManagerList.empty()) )
        {
            EntresolManager* Current = this->ManagerList.front();
            #ifdef MEZZDEBUG
            this->_Log("Deleting " + Current->GetInterfaceTypeAsString() + ".\n");
            #endif

            ManagerFactoryIterator ManIt = Entresol::ManagerFactories.find(Current->GetImplementationTypeName());
            if( ManIt == Entresol::ManagerFactories.end() ) {
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to destroy manager of type \"" + Current->GetImplementationTypeName() + "\", which has no factory registered.");
            }else{
                (*ManIt).second->DestroyManager(Current);
            }

            this->ManagerList.pop_front();
        }// */
    }

    void Entresol::AddManager(EntresolManager* ManagerToAdd)
    {
        #ifdef MEZZDEBUG
        this->_Log("Adding " + ManagerToAdd->GetInterfaceTypeAsString() + ".\n");
        #endif
        // We have to verify the manager is unique.  A number of issues can arise if a manager is double inserted.
        for( ManagerIterator ManIter = this->ManagerList.begin() ; ManIter != this->ManagerList.end() ; ++ManIter )
        {
            if( (*ManIter) == ManagerToAdd )
                return;
        }
        this->ManagerList.push_back(ManagerToAdd);
    }

    EntresolManager* Entresol::GetManager(const Whole RetrieveType, UInt16 WhichOne)
    {
        for(ManagerIterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
        {
            if( (*ManIter)->GetInterfaceType() == RetrieveType ) {
                if( 0 == WhichOne ) return *ManIter; // we use our copy of WhichOne as a countdown to 0
                else --WhichOne;
            }
        }
        return NULL;
    }

    void Entresol::RemoveManager(EntresolManager* ManagerToRemove)
    {
        for( ManagerIterator ManIter = this->ManagerList.begin() ; ManIter != this->ManagerList.end() ; ++ManIter )
        {
            if( *ManIter == ManagerToRemove ) {
                this->ManagerList.erase(ManIter);
                return;
            }
        }
    }

    void Entresol::RemoveManager(const Whole RemoveType, UInt16 WhichOne)
    {
        for( ManagerIterator ManIter = this->ManagerList.begin(); ManIter!=this->ManagerList.end(); ++ManIter )
        {
            if( (*ManIter)->GetInterfaceType() == RemoveType ) {
                if( 0 == WhichOne ) {   // we use our copy of WhichOne as a countdown to 0
                    this->ManagerList.erase(ManIter);
                    return;
                }else{
                    --WhichOne;
                }
            }
        }
    }

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
        for( ConstWorldIterator it = this->Worlds.begin() ; it != this->Worlds.end() ; it++ )
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
        for( WorldIterator it = this->Worlds.begin(); it != this->Worlds.end(); it++ )
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
        for( WorldIterator it = this->Worlds.begin(); it != this->Worlds.end(); it++ )
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
        for( WorldIterator it = this->Worlds.begin(); it != this->Worlds.end(); it++ )
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
        for( WorldIterator it = this->Worlds.begin(); it != this->Worlds.end(); it++ )
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
        for( WorldIterator it = this->Worlds.begin(); it != this->Worlds.end(); it++ )
        {
            World* w = (*it);
            delete w;
        }
        this->Worlds.clear();
    }

    void Entresol::PauseAllWorlds(Boole Pause)
    {
        for( WorldIterator it = this->Worlds.begin(); it != this->Worlds.end(); it++ )
        {
            (*it)->PauseWorld(Pause);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Logging

    void Entresol::_LogString(const String& Message)
    {
        if( Message.size() > 0 ) {
            this->_GetLogStream() << Message;
        }
    }

    void Entresol::_ForceLogFlush()
    {
        Threading::DefaultThreadSpecificStorage::Type ThreadStorage(&this->WorkScheduler);
        this->Aggregator->NextFlushForced();
        this->Aggregator->DoWork(ThreadStorage);
        this->WorkScheduler.GetLog().flush();
        Threading::FrameScheduler::Resource* ThreadResource = this->WorkScheduler.GetThreadResource();
        ThreadResource->SwapAllBufferedResources();
        this->Aggregator->NextFlushForced();
        this->Aggregator->DoWork(ThreadStorage);
        this->WorkScheduler.GetLog().flush();
    }

    Logger& Entresol::_GetLogStream(Threading::ThreadId ID)
    {
        Threading::FrameScheduler::Resource* AlmostResults = this->WorkScheduler.GetThreadResource(ID);
        if(AlmostResults)
            { return AlmostResults->GetUsableLogger(); }
        else
            { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_RANGE_EXCEPTION, "Could not access thread Specific Logger from invalid thread."); }
    }

    Threading::LogAggregator* Entresol::_GetLogAggregator()
        { return this->Aggregator; }
}
#endif
