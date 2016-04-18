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
#ifndef terrainmanager_cpp
#define terrainmanager_cpp

#include "terrainmanager.h"
#include "terrain.h"
#include "Physics/physicsmanager.h"
#include "entresol.h"
#include "world.h"

#include <sstream>
#include <algorithm>

namespace Mezzanine
{
    TerrainManager::FactoryContainer TerrainManager::TerrainFactories;

    ///////////////////////////////////////////////////////////////////////////////
    // TerrainUpdateWorkUnit Methods

    TerrainUpdateWorkUnit::TerrainUpdateWorkUnit(const TerrainUpdateWorkUnit& Other)
        {  }

    TerrainUpdateWorkUnit& TerrainUpdateWorkUnit::operator=(const TerrainUpdateWorkUnit& Other)
        { return *this; }

    TerrainUpdateWorkUnit::TerrainUpdateWorkUnit(TerrainManager* Target) :
        TargetManager(Target) {  }

    TerrainUpdateWorkUnit::~TerrainUpdateWorkUnit()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void TerrainUpdateWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
    {
        for( TerrainManager::TerrainIterator TerrIt = this->TargetManager->Terrains.begin() ; TerrIt != this->TargetManager->Terrains.end() ; ++TerrIt )
        {
            (*TerrIt)->_Update();
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // TerrainManager Methods

    const String TerrainManager::ImplementationName = "DefaultTerrainManager";
    const ManagerBase::ManagerType TerrainManager::InterfaceType = ManagerBase::MT_TerrainManager;

    TerrainManager::TerrainManager(World* Creator) :
        WorldManager(Creator),
        TerrainUpdateWork(NULL),
        ThreadResources(NULL)
    {
        this->TerrainUpdateWork = new TerrainUpdateWorkUnit(this);
    }

    TerrainManager::TerrainManager(World* Creator, const XML::Node& XMLNode) :
        WorldManager(Creator),
        TerrainUpdateWork(NULL),
        ThreadResources(NULL)
    {
        /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.

        this->TerrainUpdateWork = new TerrainUpdateWorkUnit(this);
    }

    TerrainManager::~TerrainManager()
    {
        this->Deinitialize();
        this->DestroyAllTerrains();

        delete this->TerrainUpdateWork;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Prefab Terrain Type Creation

    ///////////////////////////////////////////////////////////////////////////////
    // Terrain Management

    Terrain* TerrainManager::CreateTerrain(const String& TypeName, const String& InstanceName, const NameValuePairMap& Params)
    {
        FactoryIterator TerrFactIt = this->TerrainFactories.find( TypeName );
        if( TerrFactIt != this->TerrainFactories.end() ) {
            Terrain* Ret = (*TerrFactIt).second->CreateTerrain( InstanceName, this->ParentWorld, Params );
            this->Terrains.push_back( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create an Terrain of unknown type.");
        }
    }

    Terrain* TerrainManager::CreateTerrain(const XML::Node& SelfRoot)
    {
        FactoryIterator TerrFactIt = this->TerrainFactories.find( SelfRoot.Name() );
        if( TerrFactIt != this->TerrainFactories.end() ) {
            Terrain* Ret = (*TerrFactIt).second->CreateTerrain( SelfRoot, this->ParentWorld );
            this->Terrains.push_back( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a Terrain of unknown type.");
        }
    }

    Terrain* TerrainManager::GetTerrain(const Whole Index) const
    {
        return this->Terrains.at(Index);
    }

    Terrain* TerrainManager::GetTerrain(const String& Name) const
    {
        for( ConstTerrainIterator TerrIt = this->Terrains.begin() ; TerrIt != this->Terrains.end() ; ++TerrIt )
        {
            if( (*TerrIt)->GetName() == Name )
                return (*TerrIt);
        }
        return NULL;
    }

    Whole TerrainManager::GetNumTerrains() const
    {
        return this->Terrains.size();
    }

    void TerrainManager::DestroyTerrain(const Whole Index)
    {
        TerrainIterator TerrIt = ( Index < this->GetNumTerrains() ? this->Terrains.begin() + Index : this->Terrains.end() );
        if( TerrIt != this->Terrains.end() )
        {
            FactoryIterator TerrFactIt = this->TerrainFactories.find( (*TerrIt)->GetDerivedSerializableName() );
            if( TerrFactIt != this->TerrainFactories.end() ) {
                (*TerrFactIt).second->DestroyTerrain( (*TerrIt) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to destroy a Terrain of unknown type.");
            }

            this->Terrains.erase(TerrIt);
        }
    }

    void TerrainManager::DestroyTerrain(Terrain* ToBeDestroyed)
    {
        TerrainIterator TerrIt = std::find( this->Terrains.begin(), this->Terrains.end(), ToBeDestroyed );
        if( TerrIt != this->Terrains.end() )
        {
            FactoryIterator TerrFactIt = this->TerrainFactories.find( (*TerrIt)->GetDerivedSerializableName() );
            if( TerrFactIt != this->TerrainFactories.end() ) {
                (*TerrFactIt).second->DestroyTerrain( (*TerrIt) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to destroy a Terrain of unknown type.");
            }

            this->Terrains.erase(TerrIt);
        }
    }

    void TerrainManager::DestroyAllTerrains()
    {
        for( TerrainIterator TerrIt = this->Terrains.begin() ; TerrIt != this->Terrains.end() ; ++TerrIt )
        {
            FactoryIterator TerrFactIt = this->TerrainFactories.find( (*TerrIt)->GetDerivedSerializableName() );
            if( TerrFactIt != this->TerrainFactories.end() ) {
                (*TerrFactIt).second->DestroyTerrain( (*TerrIt) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to destroy a Terrain of unknown type.");
            }
        }
        this->Terrains.clear();
    }

    TerrainManager::TerrainIterator TerrainManager::BeginTerrain()
        { return this->Terrains.begin(); }

    TerrainManager::TerrainIterator TerrainManager::EndTerrain()
        { return this->Terrains.end(); }

    TerrainManager::ConstTerrainIterator TerrainManager::BeginTerrain() const
        { return this->Terrains.begin(); }

    TerrainManager::ConstTerrainIterator TerrainManager::EndTerrain() const
        { return this->Terrains.end(); }

    ///////////////////////////////////////////////////////////////////////////////
    // TerrainFactory Management

    void TerrainManager::AddTerrainFactory(TerrainFactory* ToBeAdded)
    {
        TerrainManager::TerrainFactories.insert(std::pair<String,TerrainFactory*>(ToBeAdded->GetTypeName(),ToBeAdded));
    }

    void TerrainManager::RemoveTerrainFactory(TerrainFactory* ToBeRemoved)
    {
        TerrainManager::RemoveTerrainFactory(ToBeRemoved->GetTypeName());
    }

    void TerrainManager::RemoveTerrainFactory(const String& ImplName)
    {
        FactoryIterator TerrFactIt = TerrainManager::TerrainFactories.find(ImplName);
        if( TerrFactIt != TerrainManager::TerrainFactories.end() )
            { TerrainManager::TerrainFactories.erase(TerrFactIt); }
    }

    void TerrainManager::DestroyTerrainFactory(TerrainFactory* ToBeDestroyed)
    {
        TerrainManager::DestroyTerrainFactory(ToBeDestroyed->GetTypeName());
    }

    void TerrainManager::DestroyTerrainFactory(const String& ImplName)
    {
        FactoryIterator TerrFactIt = TerrainManager::TerrainFactories.find(ImplName);
        if( TerrFactIt != TerrainManager::TerrainFactories.end() ) {
            delete TerrFactIt->second;
            TerrainManager::TerrainFactories.erase(TerrFactIt);
        }
    }

    void TerrainManager::DestroyAllTerrainFactories()
    {
        for( FactoryIterator TerrFactIt = TerrainManager::TerrainFactories.begin() ; TerrFactIt != TerrainManager::TerrainFactories.end() ; ++TerrFactIt )
            { delete (*TerrFactIt).second; }
        TerrainManager::TerrainFactories.clear();
    }

    void TerrainManager::AddAllDefaultTerrainFactories()
    {

    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void TerrainManager::Pause(const UInt32 PL)
    {
        // Do nothing currently
    }

    void TerrainManager::Initialize()
    {
        if( !this->Initialized )
        {
            WorldManager::Initialize();

            this->TheEntresol->GetScheduler().AddWorkUnitMain( this->TerrainUpdateWork, "TerrainUpdateWork" );
            Physics::PhysicsManager* PhysicsMan = static_cast<Physics::PhysicsManager*>( this->ParentWorld->GetManager(ManagerBase::MT_PhysicsManager) );
            if( PhysicsMan ) {
                this->TerrainUpdateWork->AddDependency( PhysicsMan->GetSimulationWork() );
            }

            this->Initialized = true;
        }
    }

    void TerrainManager::Deinitialize()
    {
        if( this->Initialized )
        {
            this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->TerrainUpdateWork );
            this->TerrainUpdateWork->ClearDependencies();

            this->Initialized = false;
        }
    }

    TerrainUpdateWorkUnit* TerrainManager::GetTerrainUpdateWork()
        { return this->TerrainUpdateWork; }

    ///////////////////////////////////////////////////////////////////////////////
    // Type Identifier Methods

    ManagerBase::ManagerType TerrainManager::GetInterfaceType() const
        { return TerrainManager::InterfaceType; }

    String TerrainManager::GetImplementationTypeName() const
        { return TerrainManager::ImplementationName; }

    ///////////////////////////////////////////////////////////////////////////////
    // DefaultTerrainManagerFactory Methods

    DefaultTerrainManagerFactory::DefaultTerrainManagerFactory()
        {  }

    DefaultTerrainManagerFactory::~DefaultTerrainManagerFactory()
        {  }

    String DefaultTerrainManagerFactory::GetManagerImplName() const
        { return TerrainManager::ImplementationName; }

    ManagerBase::ManagerType DefaultTerrainManagerFactory::GetManagerType() const
        { return TerrainManager::InterfaceType; }

    WorldManager* DefaultTerrainManagerFactory::CreateManager(World* Creator, const NameValuePairList& Params)
        { return new TerrainManager(Creator); }

    WorldManager* DefaultTerrainManagerFactory::CreateManager(World* Creator, const XML::Node& XMLNode)
        { return new TerrainManager(Creator,XMLNode); }

    void DefaultTerrainManagerFactory::DestroyManager(WorldManager* ToBeDestroyed)
        { delete ToBeDestroyed; }
}//Mezzanine

#endif
