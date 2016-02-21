// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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

#ifndef _world_cpp
#define _world_cpp

#include "entresol.h"
#include "world.h"
#include "worldmanager.h"
#include "worldmanagerfactory.h"
#include "stringtool.h"

//#include "pagingmanager.h"
#include "actormanager.h"
#include "areaeffectmanager.h"
#include "debrismanager.h"
#include "terrainmanager.h"

#include "Audio/soundscapemanager.h"
#include "Graphics/scenemanager.h"
#include "Physics/physicsmanager.h"

// Enabled implementation includes
#ifdef ENABLE_OALS_AUDIO_IMPLEMENTATION
    #include "Audio/OALS/oalssoundscapemanagerfactory.h"
#endif // ENABLE_OALS_AUDIO_IMPLEMENTATION

namespace Mezzanine
{
    World::ManagerFactoryMap World::ManagerFactories;

    World::World(const String& WorldName) :
        Name(WorldName)
    {
        Physics::ManagerConstructionInfo PhysicsInfo;
        std::vector <WorldManager*> temp;

        this->Construct(PhysicsInfo,"DefaultSceneManager",temp);
    }

    World::World(const String& WorldName, const WorldManagerContainer& Managers) :
        Name(WorldName)
    {
        Physics::ManagerConstructionInfo PhysicsInfo;

        this->Construct(PhysicsInfo,"DefaultSceneManager",Managers);
    }

    World::World(const String& WorldName, const Physics::ManagerConstructionInfo& PhysicsInfo, const String& SceneType) :
        Name(WorldName)
    {
        std::vector <WorldManager*> temp;

        this->Construct(PhysicsInfo, SceneType, temp);
    }

    World::World(const String& WorldName, const WorldManagerContainer& Managers, const Physics::ManagerConstructionInfo& PhysicsInfo, const String& SceneType) :
        Name(WorldName)
    {
        this->Construct(PhysicsInfo, SceneType, Managers);
    }

    World::World(const XML::Node& SelfNode)
    {

    }

    World::~World()
    {
        this->DestroyAllManagers();
    }

    void World::Construct(const Physics::ManagerConstructionInfo& PhysicsInfo, const String& SceneType, const WorldManagerContainer& ManagerToBeAdded  )
    {
        //add each manager that was passed in to the manager list
        for( ConstWorldManagerIterator iter = ManagerToBeAdded.begin() ; iter!= ManagerToBeAdded.end() ; ++iter )
            { this->AddManager(*iter); }

        //Dummy param list so we can use the auto-added manager types if needed
        NameValuePairList Params;
        if( this->GetManager(ManagerBase::MT_ActorManager) == 0 ) {
            this->CreateManager("DefaultActorManager",Params,true);
        }
        if( this->GetManager(ManagerBase::MT_AreaEffectManager) == 0 ) {
            this->CreateManager("DefaultAreaEffectManager",Params,true);
        }
        if( this->GetManager(ManagerBase::MT_DebrisManager) == 0 ) {
            this->CreateManager("DefaultDebrisManager",Params,true);
        }
        if( this->GetManager(ManagerBase::MT_SceneManager) == 0 ) {
            Params.push_back( std::make_pair( String("InternalManagerTypeName"),SceneType ) );
            this->CreateManager("DefaultSceneManager",Params,true);
            Params.clear();
        }
        if( this->GetManager(ManagerBase::MT_PhysicsManager) == 0 ) {
            Params.push_back( std::make_pair( String("GeographyUpperBounds"),StringTools::ConvertToString( PhysicsInfo.GeographyUpperBounds ) ) );
            Params.push_back( std::make_pair( String("GeogrpahyLowerBounds"),StringTools::ConvertToString( PhysicsInfo.GeographyLowerBounds ) ) );
            Params.push_back( std::make_pair( String("MaxProxies"),StringTools::ConvertToString( PhysicsInfo.MaxProxies ) ) );
            Params.push_back( std::make_pair( String("Gravity"),StringTools::ConvertToString( PhysicsInfo.Gravity ) ) );
            Params.push_back( std::make_pair( String("SoftRigidWorld"),StringTools::ConvertToString( Boole(PhysicsInfo.PhysicsFlags & Physics::ManagerConstructionInfo::PCF_SoftRigidWorld) ) ) );
            Params.push_back( std::make_pair( String("LimitlessWorld"),StringTools::ConvertToString( Boole(PhysicsInfo.PhysicsFlags & Physics::ManagerConstructionInfo::PCF_LimitlessWorld) ) ) );
            Params.push_back( std::make_pair( String("MultiThreaded"),StringTools::ConvertToString( Boole(PhysicsInfo.PhysicsFlags & Physics::ManagerConstructionInfo::PCF_Multithreaded) ) ) );
            this->CreateManager("DefaultPhysicsManager",Params,true);
            Params.clear();
        }

        #ifdef ENABLE_OALS_AUDIO_IMPLEMENTATION
        if( this->GetManager(ManagerBase::MT_SoundScapeManager) == 0 ) {
            this->CreateManager("OALSSoundScapeManager",Params,true);
        }
        #endif //ENABLE_OALS_AUDIO_IMPLEMENTATION
    }

    Boole World::VerifyManagerInitializations()
    {

    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    const String& World::GetName() const
        { return this->Name; }

    void World::PauseWorld(const Boole Pause)
    {
        static_cast<Physics::PhysicsManager*>( this->GetManager(ManagerBase::MT_PhysicsManager) )->PauseSimulation(Pause);
    }

    void World::Clear()
    {
        Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( this->GetManager(ManagerBase::MT_PhysicsManager) );
        // Start with constraints and anything else that is linking the objects.
        // Nuke the metadata while we're at it.
        if( PhysMan != NULL ) {
            PhysMan->DestroyAllConstraints();
            PhysMan->DestroyAllWorldTriggers();
            PhysMan->ClearPhysicsMetaData();
        }

        ActorManager* ActorMan = static_cast<ActorManager*>( this->GetManager(ManagerBase::MT_ActorManager) );
        AreaEffectManager* AreaEffectMan = static_cast<AreaEffectManager*>( this->GetManager(ManagerBase::MT_AreaEffectManager) );
        DebrisManager* DebrisMan = static_cast<DebrisManager*>( this->GetManager(ManagerBase::MT_DebrisManager) );
        // Now get the higher level world objects.  They'll take out their bound proxies as they go.
        if( ActorMan != NULL ) {
            ActorMan->DestroyAllActors();
        }
        if( AreaEffectMan != NULL ) {
            AreaEffectMan->DestroyAllAreaEffects();
        }
        if( DebrisMan != NULL ) {
            DebrisMan->DestroyAllDebris();
        }

        Audio::SoundScapeManager* SoundScapeMan = static_cast<Audio::SoundScapeManager*>( this->GetManager(ManagerBase::MT_SoundScapeManager) );
        Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( this->GetManager(ManagerBase::MT_SceneManager) );
        // Now deal with any dangling proxies.
        // Any additional minor cleanup related to these managers can be done after proxy cleanup as well.
        if( PhysMan != NULL ) {
            PhysMan->DestroyAllProxies();
        }
        if( SceneMan != NULL ) {
            SceneMan->DestroyAllProxies();
            SceneMan->DisableSky();
        }
        if( SoundScapeMan != NULL ) {
            SoundScapeMan->DestroyAllProxies();
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Initialization

    void World::Initialize()
    {
        for( WorldManagerIterator ManIter = this->WorldManagers.begin() ; ManIter != this->WorldManagers.end() ; ++ManIter )
        {
            #ifdef MEZZDEBUG
            StringStream InitStream;
            InitStream << "Initializing " << (*ManIter)->GetImplementationTypeName() << " as " << (*ManIter)->GetInterfaceTypeAsString() << "." << std::endl;
            Entresol::GetSingletonPtr()->_Log( InitStream.str() );
            #endif
            (*ManIter)->Initialize();
        }
    }

    void World::Deinitialize()
    {
        for( WorldManagerIterator ManIter = this->WorldManagers.begin() ; ManIter != this->WorldManagers.end() ; ++ManIter )
        {
            #ifdef MEZZDEBUG
            StringStream DeinitStream;
            DeinitStream << "Deinitializing " << (*ManIter)->GetImplementationTypeName() << " as " << (*ManIter)->GetInterfaceTypeAsString() << "." << std::endl;
            Entresol::GetSingletonPtr()->_Log( DeinitStream.str() );
            #endif
            (*ManIter)->Deinitialize();
        }
    }

    void World::PreMainLoopInit()
    {
        static_cast<Physics::PhysicsManager*>( this->GetManager(ManagerBase::MT_PhysicsManager) )->MainLoopInitialize();
        static_cast<AreaEffectManager*>( this->GetManager(ManagerBase::MT_AreaEffectManager) )->MainLoopInitialize();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Upper Management

    WorldManager* World::CreateManager(const String& ManagerImplName, const NameValuePairList& Params, Boole AddToWorld)
    {
        ManagerFactoryIterator FactoryIt = World::ManagerFactories.find(ManagerImplName);
        if( FactoryIt == World::ManagerFactories.end() ) {
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to create manager of type \"" + ManagerImplName + "\", which has no factory registered.");
        }
        WorldManager* NewMan = (*FactoryIt).second->CreateManager(this,Params);
        if(AddToWorld) {
            this->AddManager(NewMan);
        }
        return NewMan;
    }

    WorldManager* World::CreateManager(const String& ManagerImplName, const XML::Node& XMLNode, Boole AddToWorld)
    {
        ManagerFactoryIterator FactoryIt = World::ManagerFactories.find(ManagerImplName);
        if( FactoryIt == World::ManagerFactories.end() ) {
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to create manager of type \"" + ManagerImplName + "\", which has no factory registered.");
        }
        WorldManager* NewMan = (*FactoryIt).second->CreateManager(this,XMLNode);
        if(AddToWorld) {
            this->AddManager(NewMan);
        }
        return NewMan;
    }

    void World::DestroyManager(WorldManager* ToBeDestroyed)
    {
        String ImplName = ToBeDestroyed->GetImplementationTypeName();
        ManagerFactoryIterator FactoryIt = World::ManagerFactories.find(ImplName);
        if( FactoryIt == World::ManagerFactories.end() ) {
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to destroy manager of type \"" + ImplName + "\", which has no factory registered.");
        }
        this->RemoveManager(ToBeDestroyed);
        (*FactoryIt).second->DestroyManager(ToBeDestroyed);
    }

    void World::DestroyAllManagers()
    {
        this->Clear();
        this->Deinitialize();
        /*std::list<WorldManager*> WorldManagerList(this->WorldManagers.begin(),this->WorldManagers.end());
        while( !(WorldManagerList.empty()) )
        {
            WorldManager* Current = WorldManagerList.front();
            ManagerFactoryIterator FactoryIt = this->ManagerFactories.find(Current->GetImplementationTypeName());
            if( FactoryIt == this->ManagerFactories.end() ) {
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to destroy manager of type \"" + Current->GetImplementationTypeName() + "\", which has no factory registered.");
            }else{
                (*FactoryIt).second->DestroyManager(Current);
            }
            WorldManagerList.pop_front();
        }// */
        for( WorldManagerIterator WorldManIt = this->WorldManagers.begin() ; WorldManIt != this->WorldManagers.end() ; ++WorldManIt )
        {
            String ImplName = (*WorldManIt)->GetImplementationTypeName();
            ManagerFactoryIterator FactoryIt = World::ManagerFactories.find(ImplName);
            if( FactoryIt == World::ManagerFactories.end() ) {
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to destroy manager of type \"" + ImplName + "\", which has no factory registered.");
            }else{
                (*FactoryIt).second->DestroyManager( (*WorldManIt) );
            }
        }// */
        this->WorldManagers.clear();
    }

    Boole World::AddManager(WorldManager* ManagerToAdd)
    {
        ManagerBase::ManagerType IType = ManagerToAdd->GetInterfaceType();
        for( WorldManagerIterator ManIter = this->WorldManagers.begin() ; ManIter != this->WorldManagers.end() ; ++ManIter )
        {
            if( (*ManIter)->GetInterfaceType() == IType )
                return false;
        }
        this->WorldManagers.push_back(ManagerToAdd);
        return true;
    }

    WorldManager* World::GetManager(const Whole ManagerToGet)
    {
        for( WorldManagerIterator ManIter = this->WorldManagers.begin() ; ManIter != this->WorldManagers.end() ; ++ManIter )
        {
            if( (*ManIter)->GetInterfaceType() == ManagerToGet )
                return *ManIter;
        }
        return NULL;
    }

    void World::RemoveManager(WorldManager* ToBeRemoved)
    {
        for( WorldManagerIterator ManIter = this->WorldManagers.begin() ; ManIter != this->WorldManagers.end() ; ++ManIter )
        {
            if( (*ManIter) == ToBeRemoved ) {
                this->WorldManagers.erase(ManIter);
                return;
            }
        }
    }

    void World::RemoveManager(const Whole ToBeRemoved)
    {
        for( WorldManagerIterator ManIter = this->WorldManagers.begin() ; ManIter != this->WorldManagers.end() ; ++ManIter )
        {
            if( (*ManIter)->GetInterfaceType() == ToBeRemoved ) {
                this->WorldManagers.erase(ManIter);
                return;
            }
        }
    }

    void World::RemoveAllManagers()
    {
        WorldManagers.clear();
    }

    World::WorldManagerContainer& World::GetWorldManagers()
    {
        return WorldManagers;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Factories Management

    void World::AddManagerFactory(WorldManagerFactory* ToBeAdded)
    {
        World::ManagerFactories.insert(std::pair<String,WorldManagerFactory*>(ToBeAdded->GetManagerImplName(),ToBeAdded));
    }

    void World::RemoveManagerFactory(WorldManagerFactory* ToBeRemoved)
    {
        World::RemoveManagerFactory(ToBeRemoved->GetManagerImplName());
    }

    void World::RemoveManagerFactory(const String& ImplName)
    {
        ManagerFactoryIterator ManIt = World::ManagerFactories.find(ImplName);
        if( ManIt != World::ManagerFactories.end() )
            { World::ManagerFactories.erase(ManIt); }
    }

    void World::DestroyManagerFactory(WorldManagerFactory* ToBeRemoved)
    {
        World::DestroyManagerFactory(ToBeRemoved->GetManagerImplName());
    }

    void World::DestroyManagerFactory(const String& ImplName)
    {
        ManagerFactoryIterator ManIt = World::ManagerFactories.find(ImplName);
        if( ManIt != World::ManagerFactories.end() ) {
            delete ManIt->second;
            World::ManagerFactories.erase(ManIt);
        }
    }

    void World::DestroyAllManagerFactories()
    {
        for( ManagerFactoryIterator ManIt = World::ManagerFactories.begin() ; ManIt != World::ManagerFactories.end() ; ++ManIt )
            { delete (*ManIt).second; }
        World::ManagerFactories.clear();
    }

    void World::AddAllEngineDefaultManagerFactories()
    {
        ManagerFactoryIterator ManIt;
        //DefaultActorManager
        ManIt = World::ManagerFactories.find("DefaultActorManager");
        if( ManIt == World::ManagerFactories.end() ) World::AddManagerFactory(new DefaultActorManagerFactory());
        //DefaultAreaEffectManager
        ManIt = World::ManagerFactories.find("DefaultAreaEffectManager");
        if( ManIt == World::ManagerFactories.end() ) World::AddManagerFactory(new DefaultAreaEffectManagerFactory());
        //DefaultDebrisManager
        ManIt = World::ManagerFactories.find("DefaultDebrisManager");
        if( ManIt == World::ManagerFactories.end() ) World::AddManagerFactory(new DefaultDebrisManagerFactory());
        //DefaultPhysicsManager
        ManIt = World::ManagerFactories.find("DefaultPhysicsManager");
        if( ManIt == World::ManagerFactories.end() ) World::AddManagerFactory(new Physics::DefaultPhysicsManagerFactory());
        //DefaultSceneManager
        ManIt = World::ManagerFactories.find("DefaultSceneManager");
        if( ManIt == World::ManagerFactories.end() ) World::AddManagerFactory(new Graphics::DefaultSceneManagerFactory());
        //DefaultTerrainManager
        ManIt = World::ManagerFactories.find("DefaultTerrainManager");
        if( ManIt == World::ManagerFactories.end() ) World::AddManagerFactory(new DefaultTerrainManagerFactory());

        #ifdef ENABLE_OALS_AUDIO_IMPLEMENTATION
        //OALSSoundScapeManager
        ManIt = World::ManagerFactories.find("OALSSoundScapeManager");
        if( ManIt == World::ManagerFactories.end() ) World::AddManagerFactory(new Audio::OALS::OALSSoundScapeManagerFactory());
        #endif //ENABLE_OALS_AUDIO_IMPLEMENTATION
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void World::ProtoSerialize(XML::Node& ParentNode) const
    {

    }

    void World::ProtoSerializeProperties(XML::Node& SelfRoot) const
    {

    }

    void World::ProtoDeSerialize(const XML::Node& SelfRoot)
    {

    }

    void World::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
    {

    }

    String World::GetDerivedSerializableName() const
        { return World::GetSerializableName(); }

    String World::GetSerializableName()
        { return "World"; }
}//Mezzanine

#endif
