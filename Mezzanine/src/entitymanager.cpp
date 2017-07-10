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
#ifndef _entitymanager_cpp
#define _entitymanager_cpp

#include "entitymanager.h"
#include "entresol.h"
#include "world.h"

#include "Physics/physicsmanager.h"

#include "entity.h"
#include "actor.h"
#include "areaeffect.h"
#include "debris.h"
#include "terrain.h"
#include "vehicle.h"

#include "fieldofforce.h"
#include "gravityfield.h"
#include "gravitywell.h"
#include "rigiddebris.h"
#include "softdebris.h"

namespace
{
    using namespace Mezzanine;

    /// @brief Compares two Entities to determine the order in which they should be stored.
    /// @param First The first Entity to compare.
    /// @param Second The second Entity to compare.
    /// @return Returns true if the First Entity should occur before the second Entity.
    Boole EntityInserter(Entity* First, Entity* Second)
        { return First->GetEntityType() < Second->GetEntityType(); }
    /// @brief Convenience comparison method for std::lower_bound.
    /// @param First The Entity to be compared.
    /// @param Second The EntityType to compare with the Entity.
    /// @return Returns true if the Entity provided should occur before an Entity of the specified type.
    Boole LowerEntityCompare(Entity* First, const EntityType Second)
        { return First->GetEntityType() < Second; }
    /// @brief Convenience comparison method for std::upper_bound.
    /// @param First The EntityType to compare with the Entity.
    /// @param Second The Entity to be compared.
    /// @return Returns true if an Entity of the specified type should occur before the Entity provided.
    Boole UpperEntityCompare(const EntityType First, Entity* Second)
        { return First < Second->GetEntityType(); }
}

namespace Mezzanine
{
    EntityManager::FactoryContainer EntityManager::EntityFactories;

    ///////////////////////////////////////////////////////////////////////////////
    // ActorUpdateWorkUnit Methods

    ActorUpdateWorkUnit::ActorUpdateWorkUnit(EntityManager* Target) :
        TargetManager(Target)
        {  }

    void ActorUpdateWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
    {
        Whole FrameTime = CurrentThreadStorage.GetLastFrameTime();
        EntityManager::EntityRange Range = this->TargetManager->GetActorRange();
        for( Entity* CurrActor : Range )
            { CurrActor->_Update(FrameTime); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // AreaEffectUpdateWorkUnit Methods

    AreaEffectUpdateWorkUnit::AreaEffectUpdateWorkUnit(EntityManager* Target) :
        TargetManager(Target)
        {  }

    void AreaEffectUpdateWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
    {
        Whole FrameTime = CurrentThreadStorage.GetLastFrameTime();
        EntityManager::EntityRange Range = this->TargetManager->GetAreaEffectRange();
        for( Entity* CurrEntity : Range )
        {
            AreaEffect* CurrAreaEffect = static_cast<AreaEffect*>( CurrEntity );
            CurrAreaEffect->_Update(FrameTime);
            CurrAreaEffect->ApplyEffect();
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // DebrisUpdateWorkUnit Methods

    DebrisUpdateWorkUnit::DebrisUpdateWorkUnit(EntityManager* Target) :
        TargetManager(Target)
        {  }

    void DebrisUpdateWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
    {
        Whole FrameTime = CurrentThreadStorage.GetLastFrameTime();
        EntityManager::EntityRange Range = this->TargetManager->GetDebrisRange();
        for( Entity* CurrDebris : Range )
            { CurrDebris->_Update(FrameTime); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // TerrainUpdateWorkUnit Methods

    TerrainUpdateWorkUnit::TerrainUpdateWorkUnit(EntityManager* Target) :
        TargetManager(Target)
        {  }

    void TerrainUpdateWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
    {
        Whole FrameTime = CurrentThreadStorage.GetLastFrameTime();
        EntityManager::EntityRange Range = this->TargetManager->GetTerrainRange();
        for( Entity* CurrTerrain : Range )
            { CurrTerrain->_Update(FrameTime); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // VehicleUpdateWorkUnit Methods

    VehicleUpdateWorkUnit::VehicleUpdateWorkUnit(EntityManager* Target) :
        TargetManager(Target)
        {  }

    void VehicleUpdateWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
    {
        Whole FrameTime = CurrentThreadStorage.GetLastFrameTime();
        EntityManager::EntityRange Range = this->TargetManager->GetVehicleRange();
        for( Entity* CurrVehicle : Range )
            { CurrVehicle->_Update(FrameTime); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // EntityManager Methods

    const String EntityManager::ImplementationName = "DefaultEntityManager";
    const ManagerBase::ManagerType EntityManager::InterfaceType = ManagerBase::MT_EntityManager;

    const EventNameType EntityManager::EventEntityCreated = "EntityCreated";
    const EventNameType EntityManager::EventEntityDestroyed = "EntityDestroyed";

    EntityManager::EntityManager(World* Creator) :
        WorldManager(Creator),
        ActorUpdateWork(nullptr),
        AreaEffectUpdateWork(nullptr),
        DebrisUpdateWork(nullptr),
        TerrainUpdateWork(nullptr),
        VehicleUpdateWork(nullptr),
        ThreadResources(nullptr)
    {
        this->ActorUpdateWork = new ActorUpdateWorkUnit(this);
        this->AreaEffectUpdateWork = new AreaEffectUpdateWorkUnit(this);
        this->DebrisUpdateWork = new DebrisUpdateWorkUnit(this);
        this->TerrainUpdateWork = new TerrainUpdateWorkUnit(this);
        this->VehicleUpdateWork = new VehicleUpdateWorkUnit(this);

        this->Publisher.AddSubscriptionTable(EventEntityCreated);
        this->Publisher.AddSubscriptionTable(EventEntityDestroyed);
    }

    EntityManager::EntityManager(World* Creator, const XML::Node& XMLNode) :
        WorldManager(Creator),
        ActorUpdateWork(nullptr),
        AreaEffectUpdateWork(nullptr),
        DebrisUpdateWork(nullptr),
        TerrainUpdateWork(nullptr),
        VehicleUpdateWork(nullptr),
        ThreadResources(nullptr)
    {
        /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.

        this->ActorUpdateWork = new ActorUpdateWorkUnit(this);
        this->AreaEffectUpdateWork = new AreaEffectUpdateWorkUnit(this);
        this->DebrisUpdateWork = new DebrisUpdateWorkUnit(this);
        this->TerrainUpdateWork = new TerrainUpdateWorkUnit(this);
        this->VehicleUpdateWork = new VehicleUpdateWorkUnit(this);

        this->Publisher.AddSubscriptionTable(EventEntityCreated);
        this->Publisher.AddSubscriptionTable(EventEntityDestroyed);
    }

    EntityManager::~EntityManager()
    {
        this->Deinitialize();
        this->DestroyAllEntities();

        delete this->ActorUpdateWork;
        delete this->AreaEffectUpdateWork;
        delete this->DebrisUpdateWork;
        delete this->TerrainUpdateWork;
        delete this->VehicleUpdateWork;
    }

    void EntityManager::AddEntity(Entity* ToAdd)
    {
        EntityIterator InsertPos = std::lower_bound(this->Entities.begin(),this->Entities.end(),ToAdd,EntityInserter);
        this->Entities.insert(InsertPos,ToAdd);

        EntityManagementEventPtr EntEv = std::make_shared<EntityManagementEvent>(EventEntityCreated,ToAdd->GetName());
        this->Publisher.DispatchEvent(EntEv);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Prefab Entity Type Creation

    FieldOfForce* EntityManager::CreateFieldOfForce(const String& Name)
    {
        FactoryIterator AEFactIt = this->EntityFactories.find( FieldOfForce::GetSerializableName() );
        if( AEFactIt != this->EntityFactories.end() ) {
            FieldOfForce* Ret = static_cast<FieldOfForceFactory*>( (*AEFactIt).second )->CreateFieldOfForce( Name, this->ParentWorld );
            this->AddEntity( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a FieldOfForce AreaEffect without it's factory registered.");
        }
    }

    FieldOfForce* EntityManager::CreateFieldOfForce(const String& Name, Physics::CollisionShape* AEShape)
    {
        FactoryIterator AEFactIt = this->EntityFactories.find( FieldOfForce::GetSerializableName() );
        if( AEFactIt != this->EntityFactories.end() ) {
            FieldOfForce* Ret = static_cast<FieldOfForceFactory*>( (*AEFactIt).second )->CreateFieldOfForce( Name, AEShape, this->ParentWorld );
            this->AddEntity( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a FieldOfForce AreaEffect without it's factory registered.");
        }
    }

    FieldOfForce* EntityManager::CreateFieldOfForce(const XML::Node& SelfRoot)
    {
        FactoryIterator AEFactIt = this->EntityFactories.find( FieldOfForce::GetSerializableName() );
        if( AEFactIt != this->EntityFactories.end() ) {
            FieldOfForce* Ret = static_cast<FieldOfForceFactory*>( (*AEFactIt).second )->CreateFieldOfForce( SelfRoot, this->ParentWorld );
            this->AddEntity( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a FieldOfForce AreaEffect without it's factory registered.");
        }
    }

    GravityField* EntityManager::CreateGravityField(const String& Name)
    {
        FactoryIterator AEFactIt = this->EntityFactories.find( GravityField::GetSerializableName() );
        if( AEFactIt != this->EntityFactories.end() ) {
            GravityField* Ret = static_cast<GravityFieldFactory*>( (*AEFactIt).second )->CreateGravityField( Name, this->ParentWorld );
            this->AddEntity( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a GravityField AreaEffect without it's factory registered.");
        }
    }

    GravityField* EntityManager::CreateGravityField(const String& Name, Physics::CollisionShape* AEShape)
    {
        FactoryIterator AEFactIt = this->EntityFactories.find( GravityField::GetSerializableName() );
        if( AEFactIt != this->EntityFactories.end() ) {
            GravityField* Ret = static_cast<GravityFieldFactory*>( (*AEFactIt).second )->CreateGravityField( Name, AEShape, this->ParentWorld );
            this->AddEntity( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a GravityField AreaEffect without it's factory registered.");
        }
    }

    GravityField* EntityManager::CreateGravityField(const XML::Node& SelfRoot)
    {
        FactoryIterator AEFactIt = this->EntityFactories.find( GravityField::GetSerializableName() );
        if( AEFactIt != this->EntityFactories.end() ) {
            GravityField* Ret = static_cast<GravityFieldFactory*>( (*AEFactIt).second )->CreateGravityField( SelfRoot, this->ParentWorld );
            this->AddEntity( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a GravityField AreaEffect without it's factory registered.");
        }
    }

    GravityWell* EntityManager::CreateGravityWell(const String& Name)
    {
        FactoryIterator AEFactIt = this->EntityFactories.find( GravityWell::GetSerializableName() );
        if( AEFactIt != this->EntityFactories.end() ) {
            GravityWell* Ret = static_cast<GravityWellFactory*>( (*AEFactIt).second )->CreateGravityWell( Name, this->ParentWorld );
            this->AddEntity( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a GravityWell AreaEffect without it's factory registered.");
        }
    }

    GravityWell* EntityManager::CreateGravityWell(const String& Name, Physics::CollisionShape* AEShape)
    {
        FactoryIterator AEFactIt = this->EntityFactories.find( GravityWell::GetSerializableName() );
        if( AEFactIt != this->EntityFactories.end() ) {
            GravityWell* Ret = static_cast<GravityWellFactory*>( (*AEFactIt).second )->CreateGravityWell( Name, AEShape, this->ParentWorld );
            this->AddEntity( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a GravityWell AreaEffect without it's factory registered.");
        }
    }

    GravityWell* EntityManager::CreateGravityWell(const XML::Node& SelfRoot)
    {
        FactoryIterator AEFactIt = this->EntityFactories.find( GravityWell::GetSerializableName() );
        if( AEFactIt != this->EntityFactories.end() ) {
            GravityWell* Ret = static_cast<GravityWellFactory*>( (*AEFactIt).second )->CreateGravityWell( SelfRoot, this->ParentWorld );
            this->AddEntity( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a GravityWell AreaEffect without it's factory registered.");
        }
    }

    RigidDebris* EntityManager::CreateRigidDebris(const String& Name, const Real Mass)
    {
        FactoryIterator DebFactIt = this->EntityFactories.find( RigidDebris::GetSerializableName() );
        if( DebFactIt != this->EntityFactories.end() ) {
            RigidDebris* Ret = static_cast<RigidDebrisFactory*>( (*DebFactIt).second )->CreateRigidDebris( Name, Mass, this->ParentWorld );
            this->AddEntity( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a RigidDebris without it's factory registered.");
        }
    }

    RigidDebris* EntityManager::CreateRigidDebris(const String& Name, const Real Mass, Graphics::Mesh* DebMesh, Physics::CollisionShape* DebShape)
    {
        FactoryIterator DebFactIt = this->EntityFactories.find( RigidDebris::GetSerializableName() );
        if( DebFactIt != this->EntityFactories.end() ) {
            RigidDebris* Ret = static_cast<RigidDebrisFactory*>( (*DebFactIt).second )->CreateRigidDebris( Name, Mass, DebMesh, DebShape, this->ParentWorld );
            this->AddEntity( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a RigidDebris without it's factory registered.");
        }
    }

    RigidDebris* EntityManager::CreateRigidDebris(const XML::Node& SelfRoot)
    {
        FactoryIterator DebFactIt = this->EntityFactories.find( RigidDebris::GetSerializableName() );
        if( DebFactIt != this->EntityFactories.end() ) {
            RigidDebris* Ret = static_cast<RigidDebrisFactory*>( (*DebFactIt).second )->CreateRigidDebris( SelfRoot, this->ParentWorld );
            this->AddEntity( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a RigidDebris without it's factory registered.");
        }
    }

    SoftDebris* EntityManager::CreateSoftDebris(const String& Name, const Real Mass)
    {
        FactoryIterator DebFactIt = this->EntityFactories.find( SoftDebris::GetSerializableName() );
        if( DebFactIt != this->EntityFactories.end() ) {
            SoftDebris* Ret = static_cast<SoftDebrisFactory*>( (*DebFactIt).second )->CreateSoftDebris( Name, Mass, this->ParentWorld );
            this->AddEntity( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a SoftDebris without it's factory registered.");
        }
    }

    SoftDebris* EntityManager::CreateSoftDebris(const XML::Node& SelfRoot)
    {
        FactoryIterator DebFactIt = this->EntityFactories.find( SoftDebris::GetSerializableName() );
        if( DebFactIt != this->EntityFactories.end() ) {
            SoftDebris* Ret = static_cast<SoftDebrisFactory*>( (*DebFactIt).second )->CreateSoftDebris( SelfRoot, this->ParentWorld );
            this->AddEntity( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a SoftDebris without it's factory registered.");
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Entity Management

    Entity* EntityManager::CreateEntity(const String& TypeName, const String& InstanceName, const NameValuePairMap& Params)
    {
        FactoryIterator EntFactIt = this->EntityFactories.find( TypeName );
        if( EntFactIt != this->EntityFactories.end() ) {
            Entity* Ret = (*EntFactIt).second->CreateEntity( InstanceName, this->ParentWorld, Params );
            this->AddEntity( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create an Entity of unknown type.");
        }
    }

    Entity* EntityManager::CreateEntity(const XML::Node& SelfRoot)
    {
        FactoryIterator EntFactIt = this->EntityFactories.find( SelfRoot.Name() );
        if( EntFactIt != this->EntityFactories.end() ) {
            Entity* Ret = (*EntFactIt).second->CreateEntity( SelfRoot, this->ParentWorld );
            this->AddEntity( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a Entity of unknown type.");
        }
    }

    Entity* EntityManager::GetEntity(const Whole Index) const
    {
        return this->Entities.at(Index);
    }

    Entity* EntityManager::GetEntity(const String& Name) const
    {
        for( ConstEntityIterator EntIt = this->Entities.begin() ; EntIt != this->Entities.end() ; ++EntIt )
        {
            if( (*EntIt)->GetName() == Name ) {
                return (*EntIt);
            }
        }
        return nullptr;
    }

    Whole EntityManager::GetNumEntities() const
    {
        return this->Entities.size();
    }

    void EntityManager::DestroyEntity(const Whole Index)
    {
        EntityIterator EntIt = ( Index < this->GetNumEntities() ? this->Entities.begin() + Index : this->Entities.end() );
        if( EntIt != this->Entities.end() ) {
            FactoryIterator EntFactIt = this->EntityFactories.find( (*EntIt)->GetDerivedSerializableName() );
            if( EntFactIt != this->EntityFactories.end() ) {
                EntityManagementEventPtr EntEv = std::make_shared<EntityManagementEvent>(EventEntityDestroyed,(*EntIt)->GetName());
                this->Publisher.DispatchEvent(EntEv);

                (*EntFactIt).second->DestroyEntity( (*EntIt) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to destroy a Entity of unknown type.");
            }

            this->Entities.erase(EntIt);
        }
    }

    void EntityManager::DestroyEntity(Entity* ToBeDestroyed)
    {
        EntityIterator EntIt = std::find( this->Entities.begin(), this->Entities.end(), ToBeDestroyed );
        if( EntIt != this->Entities.end() ) {
            FactoryIterator EntFactIt = this->EntityFactories.find( (*EntIt)->GetDerivedSerializableName() );
            if( EntFactIt != this->EntityFactories.end() ) {
                EntityManagementEventPtr EntEv = std::make_shared<EntityManagementEvent>(EventEntityDestroyed,(*EntIt)->GetName());
                this->Publisher.DispatchEvent(EntEv);

                (*EntFactIt).second->DestroyEntity( (*EntIt) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to destroy a Entity of unknown type.");
            }

            this->Entities.erase(EntIt);
        }
    }

    void EntityManager::DestroyAllEntities()
    {
        for( EntityIterator EntIt = this->Entities.begin() ; EntIt != this->Entities.end() ; ++EntIt )
        {
            FactoryIterator EntFactIt = this->EntityFactories.find( (*EntIt)->GetDerivedSerializableName() );
            if( EntFactIt != this->EntityFactories.end() ) {
                EntityManagementEventPtr EntEv = std::make_shared<EntityManagementEvent>(EventEntityDestroyed,(*EntIt)->GetName());
                this->Publisher.DispatchEvent(EntEv);

                (*EntFactIt).second->DestroyEntity( (*EntIt) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to destroy a Entity of unknown type.");
            }
        }
        this->Entities.clear();
    }

    EntityManager::EntityIterator EntityManager::BeginEntity()
        { return this->Entities.begin(); }

    EntityManager::EntityIterator EntityManager::EndEntity()
        { return this->Entities.end(); }

    EntityManager::ConstEntityIterator EntityManager::BeginEntity() const
        { return this->Entities.begin(); }

    EntityManager::ConstEntityIterator EntityManager::EndEntity() const
        { return this->Entities.end(); }

    EntityManager::EntityRange EntityManager::GetActorRange()
    {
        EntityIterator ActBegin = std::lower_bound(this->Entities.begin(),this->Entities.end(),Mezzanine::ET_ActorFirst,LowerEntityCompare);
        EntityIterator ActEnd = std::upper_bound(this->Entities.begin(),this->Entities.end(),Mezzanine::ET_ActorLast,UpperEntityCompare);
        return EntityRange(ActBegin,ActEnd);
    }

    EntityManager::EntityRange EntityManager::GetAreaEffectRange()
    {
        EntityIterator AEBegin = std::lower_bound(this->Entities.begin(),this->Entities.end(),Mezzanine::ET_AreaEffectFirst,LowerEntityCompare);
        EntityIterator AEEnd = std::upper_bound(this->Entities.begin(),this->Entities.end(),Mezzanine::ET_AreaEffectLast,UpperEntityCompare);
        return EntityRange(AEBegin,AEEnd);
    }

    EntityManager::EntityRange EntityManager::GetDebrisRange()
    {
        EntityIterator DebBegin = std::lower_bound(this->Entities.begin(),this->Entities.end(),Mezzanine::ET_DebrisFirst,LowerEntityCompare);
        EntityIterator DebEnd = std::upper_bound(this->Entities.begin(),this->Entities.end(),Mezzanine::ET_DebrisLast,UpperEntityCompare);
        return EntityRange(DebBegin,DebEnd);
    }

    EntityManager::EntityRange EntityManager::GetTerrainRange()
    {
        EntityIterator TerBegin = std::lower_bound(this->Entities.begin(),this->Entities.end(),Mezzanine::ET_TerrainFirst,LowerEntityCompare);
        EntityIterator TerEnd = std::upper_bound(this->Entities.begin(),this->Entities.end(),Mezzanine::ET_TerrainLast,UpperEntityCompare);
        return EntityRange(TerBegin,TerEnd);
    }

    EntityManager::EntityRange EntityManager::GetVehicleRange()
    {
        EntityIterator VehBegin = std::lower_bound(this->Entities.begin(),this->Entities.end(),Mezzanine::ET_VehicleFirst,LowerEntityCompare);
        EntityIterator VehEnd = std::upper_bound(this->Entities.begin(),this->Entities.end(),Mezzanine::ET_VehicleLast,UpperEntityCompare);
        return EntityRange(VehBegin,VehEnd);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // EntityFactory Management

    void EntityManager::AddEntityFactory(EntityFactory* ToBeAdded)
    {
        EntityManager::EntityFactories.insert(std::pair<String,EntityFactory*>(ToBeAdded->GetTypeName(),ToBeAdded));
    }

    void EntityManager::RemoveEntityFactory(EntityFactory* ToBeRemoved)
    {
        EntityManager::RemoveEntityFactory(ToBeRemoved->GetTypeName());
    }

    void EntityManager::RemoveEntityFactory(const String& ImplName)
    {
        FactoryIterator EntFactIt = EntityManager::EntityFactories.find(ImplName);
        if( EntFactIt != EntityManager::EntityFactories.end() )
            { EntityManager::EntityFactories.erase(EntFactIt); }
    }

    void EntityManager::DestroyEntityFactory(EntityFactory* ToBeDestroyed)
    {
        EntityManager::DestroyEntityFactory(ToBeDestroyed->GetTypeName());
    }

    void EntityManager::DestroyEntityFactory(const String& ImplName)
    {
        FactoryIterator EntFactIt = EntityManager::EntityFactories.find(ImplName);
        if( EntFactIt != EntityManager::EntityFactories.end() ) {
            delete EntFactIt->second;
            EntityManager::EntityFactories.erase(EntFactIt);
        }
    }

    void EntityManager::DestroyAllEntityFactories()
    {
        for( FactoryIterator EntFactIt = EntityManager::EntityFactories.begin() ; EntFactIt != EntityManager::EntityFactories.end() ; ++EntFactIt )
            { delete (*EntFactIt).second; }
        EntityManager::EntityFactories.clear();
    }

    void EntityManager::AddAllDefaultEntityFactories()
    {
        EntityManager::AddEntityFactory(new GenericEntityFactory());
        EntityManager::AddEntityFactory(new FieldOfForceFactory());
        EntityManager::AddEntityFactory(new GravityFieldFactory());
        EntityManager::AddEntityFactory(new GravityWellFactory());
        EntityManager::AddEntityFactory(new RigidDebrisFactory());
        EntityManager::AddEntityFactory(new SoftDebrisFactory());
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void EntityManager::Pause(const UInt32 PL)
    {
        // Do nothing currently
    }

    void EntityManager::MainLoopInitialize()
    {
        StringStream LogStream;
        LogStream << "Printing Entities in container order:" << std::endl;
        for( Entity* CurrEntity : this->Entities )
            { LogStream << CurrEntity->GetName() << " : " << CurrEntity->GetEntityType() << std::endl; }
        Entresol::GetSingletonPtr()->_Log( LogStream.str() );

        EntityRange Range = this->GetAreaEffectRange();
        for( Entity* CurrAreaEffect : Range )
        {
            CurrAreaEffect->_Update(0);
            static_cast<AreaEffect*>( CurrAreaEffect )->ApplyEffect();
        }
    }

    void EntityManager::Initialize()
    {
        if( !this->Initialized ) {
            WorldManager::Initialize();

            Threading::FrameScheduler& Scheduler = this->TheEntresol->GetScheduler();
            Scheduler.AddWorkUnitMain( this->ActorUpdateWork, "ActorUpdateWork" );
            Scheduler.AddWorkUnitMain( this->AreaEffectUpdateWork, "AreaEffectUpdateWork" );
            Scheduler.AddWorkUnitMain( this->DebrisUpdateWork, "DebrisUpdateWork" );
            Scheduler.AddWorkUnitMain( this->TerrainUpdateWork, "TerrainUpdateWork" );
            Scheduler.AddWorkUnitMain( this->VehicleUpdateWork, "VehicleUpdateWork" );

            Physics::PhysicsManager* PhysicsMan = static_cast<Physics::PhysicsManager*>( this->ParentWorld->GetManager(ManagerBase::MT_PhysicsManager) );
            if( PhysicsMan ) {
                this->ActorUpdateWork->AddDependency( PhysicsMan->GetSimulationWork() );
                this->AreaEffectUpdateWork->AddDependency( PhysicsMan->GetSimulationWork() );
                this->AreaEffectUpdateWork->AddDependency( PhysicsMan->GetWorldTriggerUpdateWork() );
                this->DebrisUpdateWork->AddDependency( PhysicsMan->GetSimulationWork() );
                this->TerrainUpdateWork->AddDependency( PhysicsMan->GetSimulationWork() );
                this->VehicleUpdateWork->AddDependency( PhysicsMan->GetSimulationWork() );
            }
            this->AreaEffectUpdateWork->AddDependency( this->GetActorUpdateWork() );

            this->Initialized = true;
        }
    }

    void EntityManager::Deinitialize()
    {
        if( this->Initialized ) {
            Threading::FrameScheduler& Scheduler = this->TheEntresol->GetScheduler();
            Scheduler.RemoveWorkUnitMain( this->ActorUpdateWork );
            this->ActorUpdateWork->ClearDependencies();
            Scheduler.RemoveWorkUnitMain( this->AreaEffectUpdateWork );
            this->AreaEffectUpdateWork->ClearDependencies();
            Scheduler.RemoveWorkUnitMain( this->DebrisUpdateWork );
            this->DebrisUpdateWork->ClearDependencies();
            Scheduler.RemoveWorkUnitMain( this->TerrainUpdateWork );
            this->TerrainUpdateWork->ClearDependencies();
            Scheduler.RemoveWorkUnitMain( this->VehicleUpdateWork );
            this->VehicleUpdateWork->ClearDependencies();

            this->Initialized = false;
        }
    }

    EventPublisher& EntityManager::GetPublisher()
        { return this->Publisher; }

    const EventPublisher& EntityManager::GetPublisher() const
        { return this->Publisher; }

    ActorUpdateWorkUnit* EntityManager::GetActorUpdateWork()
        { return this->ActorUpdateWork; }

    AreaEffectUpdateWorkUnit* EntityManager::GetAreaEffectUpdateWork()
        { return this->AreaEffectUpdateWork; }

    DebrisUpdateWorkUnit* EntityManager::GetDebrisUpdateWork()
        { return this->DebrisUpdateWork; }

    TerrainUpdateWorkUnit* EntityManager::GetTerrainUpdateWork()
        { return this->TerrainUpdateWork; }

    VehicleUpdateWorkUnit* EntityManager::GetVehicleUpdateWork()
        { return this->VehicleUpdateWork; }

    ///////////////////////////////////////////////////////////////////////////////
    // Type Identifier Methods

    ManagerBase::ManagerType EntityManager::GetInterfaceType() const
        { return EntityManager::InterfaceType; }

    String EntityManager::GetImplementationTypeName() const
        { return EntityManager::ImplementationName; }

    ///////////////////////////////////////////////////////////////////////////////
    // DefaultEntityManagerFactory Methods

    String DefaultEntityManagerFactory::GetManagerImplName() const
        { return EntityManager::ImplementationName; }

    ManagerBase::ManagerType DefaultEntityManagerFactory::GetManagerType() const
        { return EntityManager::InterfaceType; }

    WorldManager* DefaultEntityManagerFactory::CreateManager(World* Creator, const NameValuePairList& Params)
        { return new EntityManager(Creator); }

    WorldManager* DefaultEntityManagerFactory::CreateManager(World* Creator, const XML::Node& XMLNode)
        { return new EntityManager(Creator,XMLNode); }

    void DefaultEntityManagerFactory::DestroyManager(WorldManager* ToBeDestroyed)
        { delete ToBeDestroyed; }
}//Mezzanine

#endif
