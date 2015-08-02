// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#ifndef actormanager_cpp
#define actormanager_cpp

#include "actormanager.h"
#include "actor.h"
#include "Physics/physicsmanager.h"
#include "entresol.h"
#include "world.h"

#include <sstream>
#include <algorithm>

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // ActorUpdateWorkUnit Methods

    ActorUpdateWorkUnit::ActorUpdateWorkUnit(const ActorUpdateWorkUnit& Other)
        {  }

    ActorUpdateWorkUnit& ActorUpdateWorkUnit::operator=(const ActorUpdateWorkUnit& Other)
        { return *this; }

    ActorUpdateWorkUnit::ActorUpdateWorkUnit(ActorManager* Target) :
        TargetManager(Target) {  }

    ActorUpdateWorkUnit::~ActorUpdateWorkUnit()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void ActorUpdateWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
    {
        for( ActorManager::ActorIterator ActIt = this->TargetManager->Actors.begin() ; ActIt != this->TargetManager->Actors.end() ; ++ActIt )
        {
            (*ActIt)->_Update();
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // ActorManager Methods

    const String ActorManager::ImplementationName = "DefaultActorManager";
    const ManagerBase::ManagerType ActorManager::InterfaceType = ManagerBase::MT_ActorManager;

    ActorManager::ActorManager(World* Creator) :
        WorldManager(Creator),
        ActorUpdateWork(NULL),
        ThreadResources(NULL)
    {
        this->ActorUpdateWork = new ActorUpdateWorkUnit(this);
    }

    ActorManager::ActorManager(World* Creator, XML::Node& XMLNode) :
        WorldManager(Creator),
        ActorUpdateWork(NULL),
        ThreadResources(NULL)
    {
        /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.

        this->ActorUpdateWork = new ActorUpdateWorkUnit(this);
    }

    ActorManager::~ActorManager()
    {
        this->Deinitialize();
        this->DestroyAllActors();

        delete this->ActorUpdateWork;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Prefab Actor Type Creation

    ///////////////////////////////////////////////////////////////////////////////
    // Actor Management

    Actor* ActorManager::CreateActor(const String& TypeName, const String& InstanceName, const NameValuePairMap& Params)
    {
        FactoryIterator ActFactIt = this->ActorFactories.find( TypeName );
        if( ActFactIt != this->ActorFactories.end() ) {
            Actor* Ret = (*ActFactIt).second->CreateActor( InstanceName, this->ParentWorld, Params );
            this->Actors.push_back( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create an Actor of unknown type.");
        }
    }

    Actor* ActorManager::CreateActor(const XML::Node& SelfRoot)
    {
        FactoryIterator ActFactIt = this->ActorFactories.find( SelfRoot.Name() );
        if( ActFactIt != this->ActorFactories.end() ) {
            Actor* Ret = (*ActFactIt).second->CreateActor( SelfRoot, this->ParentWorld );
            this->Actors.push_back( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a Actor of unknown type.");
        }
    }

    Actor* ActorManager::GetActor(const Whole Index) const
    {
        return this->Actors.at(Index);
    }

    Actor* ActorManager::GetActor(const String& Name) const
    {
        for( ConstActorIterator ActIt = this->Actors.begin() ; ActIt != this->Actors.end() ; ++ActIt )
        {
            if( (*ActIt)->GetName() == Name )
                return (*ActIt);
        }
        return NULL;
    }

    Whole ActorManager::GetNumActors() const
    {
        return this->Actors.size();
    }

    void ActorManager::DestroyActor(const Whole Index)
    {
        ActorIterator ActIt = ( Index < this->GetNumActors() ? this->Actors.begin() + Index : this->Actors.end() );
        if( ActIt != this->Actors.end() )
        {
            FactoryIterator ActFactIt = this->ActorFactories.find( (*ActIt)->GetDerivedSerializableName() );
            if( ActFactIt != this->ActorFactories.end() ) {
                (*ActFactIt).second->DestroyActor( (*ActIt) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to destroy a Actor of unknown type.");
            }

            this->Actors.erase(ActIt);
        }
    }

    void ActorManager::DestroyActor(Actor* ToBeDestroyed)
    {
        ActorIterator ActIt = std::find( this->Actors.begin(), this->Actors.end(), ToBeDestroyed );
        if( ActIt != this->Actors.end() )
        {
            FactoryIterator ActFactIt = this->ActorFactories.find( (*ActIt)->GetDerivedSerializableName() );
            if( ActFactIt != this->ActorFactories.end() ) {
                (*ActFactIt).second->DestroyActor( (*ActIt) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to destroy a Actor of unknown type.");
            }

            this->Actors.erase(ActIt);
        }
    }

    void ActorManager::DestroyAllActors()
    {
        for( ActorIterator ActIt = this->Actors.begin() ; ActIt != this->Actors.end() ; ++ActIt )
        {
            FactoryIterator ActFactIt = this->ActorFactories.find( (*ActIt)->GetDerivedSerializableName() );
            if( ActFactIt != this->ActorFactories.end() ) {
                (*ActFactIt).second->DestroyActor( (*ActIt) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to destroy a Actor of unknown type.");
            }
        }
        this->Actors.clear();
    }

    ActorManager::ActorIterator ActorManager::BeginActor()
        { return this->Actors.begin(); }

    ActorManager::ActorIterator ActorManager::EndActor()
        { return this->Actors.end(); }

    ActorManager::ConstActorIterator ActorManager::BeginActor() const
        { return this->Actors.begin(); }

    ActorManager::ConstActorIterator ActorManager::EndActor() const
        { return this->Actors.end(); }

    ///////////////////////////////////////////////////////////////////////////////
    // ActorFactory Management

    void ActorManager::AddActorFactory(ActorFactory* ToBeAdded)
    {
        this->ActorFactories.insert(std::pair<String,ActorFactory*>(ToBeAdded->GetTypeName(),ToBeAdded));
    }

    void ActorManager::RemoveActorFactory(ActorFactory* ToBeRemoved)
    {
        this->RemoveActorFactory(ToBeRemoved->GetTypeName());
    }

    void ActorManager::RemoveActorFactory(const String& ImplName)
    {
        FactoryIterator ActFactIt = this->ActorFactories.find(ImplName);
        if( ActFactIt != this->ActorFactories.end() )
            { this->ActorFactories.erase(ActFactIt); }
    }

    void ActorManager::DestroyActorFactory(ActorFactory* ToBeDestroyed)
    {
        this->DestroyActorFactory(ToBeDestroyed->GetTypeName());
    }

    void ActorManager::DestroyActorFactory(const String& ImplName)
    {
        FactoryIterator ActFactIt = this->ActorFactories.find(ImplName);
        if( ActFactIt != this->ActorFactories.end() ) {
            delete ActFactIt->second;
            this->ActorFactories.erase(ActFactIt);
        }
    }

    void ActorManager::DestroyAllActorFactories()
    {
        for( FactoryIterator ActFactIt = this->ActorFactories.begin() ; ActFactIt != this->ActorFactories.end() ; ++ActFactIt )
            { delete (*ActFactIt).second; }
        this->ActorFactories.clear();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void ActorManager::Pause(const UInt32 PL)
    {
        // Do nothing currently
    }

    void ActorManager::Initialize()
    {
        if( !this->Initialized )
        {
            WorldManager::Initialize();

            this->TheEntresol->GetScheduler().AddWorkUnitMain( this->ActorUpdateWork, "ActorUpdateWork" );
            Physics::PhysicsManager* PhysicsMan = this->ParentWorld->GetPhysicsManager();
            if( PhysicsMan ) {
                this->ActorUpdateWork->AddDependency( PhysicsMan->GetSimulationWork() );
            }

            this->Initialized = true;
        }
    }

    void ActorManager::Deinitialize()
    {
        if( this->Initialized )
        {
            this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->ActorUpdateWork );
            this->ActorUpdateWork->ClearDependencies();

            this->Initialized = false;
        }
    }

    ActorUpdateWorkUnit* ActorManager::GetActorUpdateWork()
        { return this->ActorUpdateWork; }

    ///////////////////////////////////////////////////////////////////////////////
    // Type Identifier Methods

    ManagerBase::ManagerType ActorManager::GetInterfaceType() const
        { return ActorManager::InterfaceType; }

    String ActorManager::GetImplementationTypeName() const
        { return ActorManager::ImplementationName; }

    ///////////////////////////////////////////////////////////////////////////////
    // DefaultActorManagerFactory Methods

    DefaultActorManagerFactory::DefaultActorManagerFactory()
        {  }

    DefaultActorManagerFactory::~DefaultActorManagerFactory()
        {  }

    String DefaultActorManagerFactory::GetManagerImplName() const
        { return ActorManager::ImplementationName; }

    ManagerBase::ManagerType DefaultActorManagerFactory::GetManagerType() const
        { return ActorManager::InterfaceType; }

    WorldManager* DefaultActorManagerFactory::CreateManager(World* Creator, NameValuePairList& Params)
        { return new ActorManager(Creator); }

    WorldManager* DefaultActorManagerFactory::CreateManager(World* Creator, XML::Node& XMLNode)
        { return new ActorManager(Creator,XMLNode); }

    void DefaultActorManagerFactory::DestroyManager(WorldManager* ToBeDestroyed)
        { delete ToBeDestroyed; }
}//Mezzanine

#endif
