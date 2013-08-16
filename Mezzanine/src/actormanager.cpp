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
#ifndef actormanager_cpp
#define actormanager_cpp

#include "actormanager.h"
#include "actorbase.h"
#include "Physics/physicsmanager.h"
#include "entresol.h"

#include <sstream>

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

    ActorManager::ActorManager() :
        ActorUpdateWork(NULL),
        ThreadResources(NULL)
    {
        this->ActorUpdateWork = new ActorUpdateWorkUnit(this);
    }

    ActorManager::ActorManager(XML::Node& XMLNode) :
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

        delete ActorUpdateWork;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Managing all actors

    void ActorManager::AddActor(ActorBase* Actor)
    {
        this->Actors.push_back(Actor);
        Actor->AddToWorld();
    }

    ActorBase* ActorManager::GetActor(const Whole& Index) const
    {
        return this->Actors.at(Index);
    }

    ActorBase* ActorManager::GetActor(const String& Name) const
    {
        for( ConstActorIterator it = this->Actors.begin() ; it != this->Actors.end() ; ++it )
        {
            if(Name == (*it)->GetName())
                return (*it);
        }
        return NULL;
    }

    Whole ActorManager::GetNumActors() const
    {
        return this->Actors.size();
    }

    void ActorManager::RemoveActor(const Whole& Index)
    {
        ActorIterator it = this->Actors.begin() + Index;
        (*it)->RemoveFromWorld();
        this->Actors.erase(it);
    }

    void ActorManager::RemoveActor(ActorBase* ToBeRemoved)
    {
        for( ActorIterator it = this->Actors.begin() ; it != this->Actors.end() ; ++it )
        {
            if(ToBeRemoved == (*it))
            {
                (*it)->RemoveFromWorld();
                this->Actors.erase(it);
                return;
            }
        }
    }

    void ActorManager::RemoveAllActors()
    {
        if( this->Actors.empty() )
            return;
        for( ActorIterator it = this->Actors.begin() ; it != this->Actors.end() ; ++it )
            (*it)->RemoveFromWorld();
        this->Actors.clear();
    }

    void ActorManager::DestroyActor(const Whole& Index)
    {
        ActorIterator it = this->Actors.begin() + Index;
        (*it)->RemoveFromWorld();
        delete (*it);
        this->Actors.erase(it);
    }

    void ActorManager::DestroyActor(ActorBase* ToBeDestroyed)
    {
        for( ActorIterator it = this->Actors.begin() ; it != this->Actors.end() ; ++it )
        {
            if(ToBeDestroyed == (*it))
            {
                (*it)->RemoveFromWorld();
                delete (*it);
                this->Actors.erase(it);
                return;
            }
        }
    }

    void ActorManager::DestroyAllActors()
    {
        if( this->Actors.empty() )
            return;
        for( ActorIterator it = this->Actors.begin() ; it != this->Actors.end() ; ++it )
        {
            (*it)->RemoveFromWorld();
            delete (*it);
        }
        this->Actors.clear();
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
            //WorldManager::Initialize();

            this->TheEntresol->GetScheduler().AddWorkUnitMain( this->ActorUpdateWork, "ActorUpdateWork" );
            Physics::PhysicsManager* PhysicsMan = Entresol::GetSingletonPtr()->GetPhysicsManager();
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
    {
        return this->ActorUpdateWork;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Type Identifier Methods

    ManagerBase::ManagerType ActorManager::GetInterfaceType() const
        { return ManagerBase::MT_ActorManager; }

    String ActorManager::GetImplementationTypeName() const
        { return "DefaultActorManager"; }

    ///////////////////////////////////////////////////////////////////////////////
    // DefaultActorManagerFactory Methods

    DefaultActorManagerFactory::DefaultActorManagerFactory()
    {
    }

    DefaultActorManagerFactory::~DefaultActorManagerFactory()
    {
    }

    String DefaultActorManagerFactory::GetManagerTypeName() const
    {
        return "DefaultActorManager";
    }

    ManagerBase* DefaultActorManagerFactory::CreateManager(NameValuePairList& Params)
    {
        return new ActorManager();
    }

    ManagerBase* DefaultActorManagerFactory::CreateManager(XML::Node& XMLNode)
    {
        return new ActorManager(XMLNode);
    }

    void DefaultActorManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
    {
        delete ToBeDestroyed;
    }
}//Mezzanine

#endif
