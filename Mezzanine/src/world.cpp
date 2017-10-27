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

#ifndef _world_cpp
#define _world_cpp

#include "entresol.h"
#include "world.h"
#include "worldmanager.h"
#include "worldmanagerfactory.h"
#include "stringtool.h"

//#include "pagingmanager.h"

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
        for( WorldManager* ToAdd : ManagerToBeAdded )
            { this->AddManager(ToAdd); }

        //Dummy param list so we can use the auto-added manager types if needed
        NameValuePairList Params;
        if( this->GetManager(ManagerBase::MT_EntityManager) == 0 ) {
            this->CreateManager("DefaultEntityManager",Params,true);
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
        return true;
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
        #ifdef MEZZDEBUG
        Entresol::GetSingletonPtr()->_Log( "Starting to clear the contents of World: " + this->Name + ".\n" );
        #endif
        Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( this->GetManager(ManagerBase::MT_PhysicsManager) );
        // Start with constraints and anything else that is linking the objects.
        // Nuke the metadata while we're at it.
        if( PhysMan != nullptr ) {
            PhysMan->DestroyAllConstraints();
            PhysMan->DestroyAllWorldTriggers();
            PhysMan->ClearPhysicsMetaData();
        }

        EntityManager* EntityMan = static_cast<EntityManager*>( this->GetManager(ManagerBase::MT_EntityManager) );
        // Now get the higher level world objects.  They'll take out their bound proxies as they go.
        if( EntityMan != nullptr ) {
            EntityMan->DestroyAllEntities();
        }

        Audio::SoundScapeManager* SoundScapeMan = static_cast<Audio::SoundScapeManager*>( this->GetManager(ManagerBase::MT_SoundScapeManager) );
        Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( this->GetManager(ManagerBase::MT_SceneManager) );
        // Now deal with any dangling proxies.
        // Any additional minor cleanup related to these managers can be done after proxy cleanup as well.
        if( PhysMan != nullptr ) {
            PhysMan->DestroyAllComponents();
        }
        if( SceneMan != nullptr ) {
            SceneMan->DestroyAllComponents();
            SceneMan->DisableSky();
        }
        if( SoundScapeMan != nullptr ) {
            SoundScapeMan->DestroyAllComponents();
        }
        #ifdef MEZZDEBUG
        Entresol::GetSingletonPtr()->_Log( "Finished clearing the contents of World: " + this->Name + ".\n" );
        #endif
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Initialization

    void World::Initialize()
    {
        for( WorldManager* CurrManager : this->WorldManagers )
        {
        #ifdef MEZZDEBUG
            StringStream InitStream;
            InitStream << "Initializing " << CurrManager->GetImplementationTypeName() << " as " << CurrManager->GetInterfaceTypeAsString() << "." << std::endl;
            Entresol::GetSingletonPtr()->_Log( InitStream.str() );
        #endif
            CurrManager->Initialize();
        }
    }

    void World::Deinitialize()
    {
        for( WorldManager* CurrManager : this->WorldManagers )
        {
        #ifdef MEZZDEBUG
            StringStream DeinitStream;
            DeinitStream << "Deinitializing " << CurrManager->GetImplementationTypeName() << " as " << CurrManager->GetInterfaceTypeAsString() << "." << std::endl;
            Entresol::GetSingletonPtr()->_Log( DeinitStream.str() );
        #endif
            CurrManager->Deinitialize();
        }
    }

    void World::PreMainLoopInit()
    {
        static_cast<Physics::PhysicsManager*>( this->GetManager(ManagerBase::MT_PhysicsManager) )->MainLoopInitialize();
        static_cast<EntityManager*>( this->GetManager(ManagerBase::MT_EntityManager) )->MainLoopInitialize();
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
        for( WorldManager* CurrManager : this->WorldManagers )
        {
            String ImplName = CurrManager->GetImplementationTypeName();
            ManagerFactoryIterator FactoryIt = World::ManagerFactories.find(ImplName);
            if( FactoryIt == World::ManagerFactories.end() ) {
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to destroy manager of type \"" + ImplName + "\", which has no factory registered.");
            }else{
                (*FactoryIt).second->DestroyManager( CurrManager );
            }
        }// */
        this->WorldManagers.clear();
    }

    Boole World::AddManager(WorldManager* ManagerToAdd)
    {
        ManagerBase::ManagerType IType = ManagerToAdd->GetInterfaceType();
        for( WorldManager* CurrManager : this->WorldManagers )
        {
            if( CurrManager->GetInterfaceType() == IType )
                return false;
        }
        this->WorldManagers.push_back(ManagerToAdd);
        return true;
    }

    WorldManager* World::GetManager(const Whole ManagerToGet) const
    {
        for( WorldManager* CurrManager : this->WorldManagers )
        {
            if( CurrManager->GetInterfaceType() == ManagerToGet )
                return CurrManager;
        }
        return nullptr;
    }

    EntityManager* World::GetEntityManager(const Whole ManagerToGet) const
    {
        return static_cast<EntityManager*>( this->GetManager(ManagerBase::MT_EntityManager) );
    }

    EntityComponentManager* World::GetComponentManager(const Whole ManagerToGet) const
    {
        if( ManagerToGet >= ManagerBase::MT_EntityComponentManagerFirst &&
            ManagerToGet <= ManagerBase::MT_EntityComponentManagerLast )
        {
            return static_cast<EntityComponentManager*>( this->GetManager(ManagerToGet) );
        }
        return nullptr;
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
        World::ManagerFactories.emplace(ToBeAdded->GetManagerImplName(),ToBeAdded);
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
        for( World::ManagerFactoryMap::value_type& CurrFactory : World::ManagerFactories )
            { delete CurrFactory.second; }
        World::ManagerFactories.clear();
    }

    void World::AddAllEngineDefaultManagerFactories()
    {
        ManagerFactoryIterator ManIt;
        //DefaultEntityManager
        ManIt = World::ManagerFactories.find("DefaultEntityManager");
        if( ManIt == World::ManagerFactories.end() ) World::AddManagerFactory(new DefaultEntityManagerFactory());
        //DefaultPhysicsManager
        ManIt = World::ManagerFactories.find("DefaultPhysicsManager");
        if( ManIt == World::ManagerFactories.end() ) World::AddManagerFactory(new Physics::DefaultPhysicsManagerFactory());
        //DefaultSceneManager
        ManIt = World::ManagerFactories.find("DefaultSceneManager");
        if( ManIt == World::ManagerFactories.end() ) World::AddManagerFactory(new Graphics::DefaultSceneManagerFactory());

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
