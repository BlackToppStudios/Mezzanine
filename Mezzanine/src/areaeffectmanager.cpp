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
#ifndef _areaeffectmanager_cpp
#define _areaeffectmanager_cpp

#include "areaeffectmanager.h"
#include "areaeffect.h"
#include "entresol.h"

#include "Physics/physicsmanager.h"
#include "actormanager.h"

#include <sstream>

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // AreaEffectUpdateWorkUnit Methods

    AreaEffectUpdateWorkUnit::AreaEffectUpdateWorkUnit(const AreaEffectUpdateWorkUnit& Other)
        {  }

    AreaEffectUpdateWorkUnit& AreaEffectUpdateWorkUnit::operator=(const AreaEffectUpdateWorkUnit& Other)
        { return *this; }

    AreaEffectUpdateWorkUnit::AreaEffectUpdateWorkUnit(AreaEffectManager* Target) :
        TargetManager(Target) {  }

    AreaEffectUpdateWorkUnit::~AreaEffectUpdateWorkUnit()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void AreaEffectUpdateWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
    {
        for( AreaEffectManager::AreaEffectIterator AE = this->TargetManager->AreaEffects.begin() ; AE != this->TargetManager->AreaEffects.end() ; ++AE )
        {
            (*AE)->_Update();
            (*AE)->ApplyEffect();
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // AreaEffectManager Methods

    template<> AreaEffectManager* Singleton<AreaEffectManager>::SingletonPtr = NULL;

    AreaEffectManager::AreaEffectManager() :
        AreaEffectUpdateWork(NULL),
        ThreadResources(NULL)
    {
        this->AreaEffectUpdateWork = new AreaEffectUpdateWorkUnit(this);
    }

    AreaEffectManager::AreaEffectManager(XML::Node& XMLNode) :
        AreaEffectUpdateWork(NULL),
        ThreadResources(NULL)
    {
        /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.

        this->AreaEffectUpdateWork = new AreaEffectUpdateWorkUnit(this);
    }

    AreaEffectManager::~AreaEffectManager()
    {
        this->Deinitialize();
        this->DestroyAllAreaEffects();

        delete AreaEffectUpdateWork;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Managing all actors

    void AreaEffectManager::AddAreaEffect(AreaEffect* ToBeAdded)
    {
        this->AreaEffects.push_back(ToBeAdded);
        ToBeAdded->AddToWorld();
    }

    AreaEffect* AreaEffectManager::GetAreaEffect(const Whole& Index) const
    {
        return this->AreaEffects.at(Index);
    }

    AreaEffect* AreaEffectManager::GetAreaEffect(const String& Name) const
    {
        for( ConstAreaEffectIterator it = this->AreaEffects.begin() ; it != this->AreaEffects.end() ; ++it )
        {
            if(Name == (*it)->GetName())
                return (*it);
        }
        return NULL;
    }

    Whole AreaEffectManager::GetNumAreaEffects() const
    {
        return this->AreaEffects.size();
    }

    void AreaEffectManager::RemoveAreaEffect(const Whole& Index)
    {
        AreaEffectIterator it = this->AreaEffects.begin() + Index;
        (*it)->RemoveFromWorld();
        this->AreaEffects.erase(it);
    }

    void AreaEffectManager::RemoveAreaEffect(AreaEffect* ToBeRemoved)
    {
        for( AreaEffectIterator it = this->AreaEffects.begin() ; it != this->AreaEffects.end() ; ++it )
        {
            if(ToBeRemoved == (*it))
            {
                (*it)->RemoveFromWorld();
                this->AreaEffects.erase(it);
                return;
            }
        }
    }

    void AreaEffectManager::RemoveAllAreaEffects()
    {
        if( this->AreaEffects.empty() )
            return;
        for( AreaEffectIterator it = this->AreaEffects.begin() ; it != this->AreaEffects.end() ; ++it )
            (*it)->RemoveFromWorld();
        this->AreaEffects.clear();
    }

    void AreaEffectManager::DestroyAreaEffect(const Whole& Index)
    {
        AreaEffectIterator it = this->AreaEffects.begin() + Index;
        (*it)->RemoveFromWorld();
        delete (*it);
        this->AreaEffects.erase(it);
    }

    void AreaEffectManager::DestroyAreaEffect(AreaEffect* ToBeDestroyed)
    {
        for( AreaEffectIterator it = this->AreaEffects.begin() ; it != this->AreaEffects.end() ; ++it )
        {
            if(ToBeDestroyed == (*it))
            {
                (*it)->RemoveFromWorld();
                delete (*it);
                this->AreaEffects.erase(it);
                return;
            }
        }
    }

    void AreaEffectManager::DestroyAllAreaEffects()
    {
        if( this->AreaEffects.empty() )
            return;
        for( AreaEffectIterator it = this->AreaEffects.begin() ; it != this->AreaEffects.end() ; ++it )
        {
            (*it)->RemoveFromWorld();
            delete (*it);
        }
        this->AreaEffects.clear();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void AreaEffectManager::MainLoopInitialize()
    {
        for( AreaEffectManager::AreaEffectIterator AE = this->AreaEffects.begin() ; AE != this->AreaEffects.end() ; ++AE )
        {
            (*AE)->_Update();
            (*AE)->ApplyEffect();
        }
    }

    void AreaEffectManager::Initialize()
    {
        if( !this->Initialized )
        {
            this->TheEntresol->GetScheduler().AddWorkUnitMain( this->AreaEffectUpdateWork );

            Physics::PhysicsManager* PhysicsMan = Physics::PhysicsManager::GetSingletonPtr();
            if( PhysicsMan ) {
                this->AreaEffectUpdateWork->AddDependency( PhysicsMan->GetSimulationWork() );
                this->AreaEffectUpdateWork->AddDependency( PhysicsMan->GetWorldTriggerUpdateWork() );
                this->AreaEffectUpdateWork->AddDependency( PhysicsMan->GetDebugDrawWork() );
            }

            Mezzanine::ActorManager* ActorMan = ActorManager::GetSingletonPtr();
            if( ActorMan ) {
                this->AreaEffectUpdateWork->AddDependency( ActorMan->GetActorUpdateWork() );
            }

            this->Initialized = true;
        }
    }

    void AreaEffectManager::Deinitialize()
    {
        if( this->Initialized )
        {
            this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->AreaEffectUpdateWork );
            this->AreaEffectUpdateWork->ClearDependencies();

            this->Initialized = false;
        }
    }

    AreaEffectUpdateWorkUnit* AreaEffectManager::GetAreaEffectUpdateWork()
    {
        return this->AreaEffectUpdateWork;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Type Identifier Methods

    ManagerBase::ManagerType AreaEffectManager::GetInterfaceType() const
        { return ManagerBase::AreaEffectManager; }

    String AreaEffectManager::GetImplementationTypeName() const
        { return "DefaultAreaEffectManager"; }

    ///////////////////////////////////////////////////////////////////////////////
    // DefaultAreaEffectManagerFactory Methods

    DefaultAreaEffectManagerFactory::DefaultAreaEffectManagerFactory()
    {
    }

    DefaultAreaEffectManagerFactory::~DefaultAreaEffectManagerFactory()
    {
    }

    String DefaultAreaEffectManagerFactory::GetManagerTypeName() const
    {
        return "DefaultAreaEffectManager";
    }

    ManagerBase* DefaultAreaEffectManagerFactory::CreateManager(NameValuePairList& Params)
    {
        if(AreaEffectManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return AreaEffectManager::GetSingletonPtr();
        }else return new AreaEffectManager();
    }

    ManagerBase* DefaultAreaEffectManagerFactory::CreateManager(XML::Node& XMLNode)
    {
        if(AreaEffectManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return AreaEffectManager::GetSingletonPtr();
        }else return new AreaEffectManager(XMLNode);
    }

    void DefaultAreaEffectManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
    {
        delete ToBeDestroyed;
    }
}//Mezzanine

#endif
