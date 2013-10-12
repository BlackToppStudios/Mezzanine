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

#include "fieldofforce.h"
#include "gravityfield.h"
#include "gravitywell.h"

#include "Physics/physicsmanager.h"

#include "actormanager.h"
#include "entresol.h"

#include <sstream>
#include <algorithm>

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

    AreaEffectManager::AreaEffectManager() :
        AreaEffectUpdateWork(NULL),
        ThreadResources(NULL)
    {
        this->AddAreaEffectFactory( new FieldOfForceFactory() );
        this->AddAreaEffectFactory( new GravityFieldFactory() );
        this->AddAreaEffectFactory( new GravityWellFactory() );

        this->AreaEffectUpdateWork = new AreaEffectUpdateWorkUnit(this);
    }

    AreaEffectManager::AreaEffectManager(XML::Node& XMLNode) :
        AreaEffectUpdateWork(NULL),
        ThreadResources(NULL)
    {
        /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.

        this->AddAreaEffectFactory( new FieldOfForceFactory() );
        this->AddAreaEffectFactory( new GravityFieldFactory() );
        this->AddAreaEffectFactory( new GravityWellFactory() );

        this->AreaEffectUpdateWork = new AreaEffectUpdateWorkUnit(this);
    }

    AreaEffectManager::~AreaEffectManager()
    {
        this->Deinitialize();
        this->DestroyAllAreaEffects();
        this->DestroyAllAreaEffectFactories();

        delete this->AreaEffectUpdateWork;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Prefab AreaEffect Type Creation

    FieldOfForce* AreaEffectManager::CreateFieldOfForce(const String& Name)
    {
        FactoryIterator AEFactIt = this->AreaEffectFactories.find( FieldOfForce::GetSerializableName() );
        if( AEFactIt != this->AreaEffectFactories.end() ) {
            FieldOfForce* Ret = static_cast<FieldOfForceFactory*>( (*AEFactIt).second )->CreateFieldOfForce( Name, this->ParentWorld );
            this->AreaEffects.push_back( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to create a FieldOfForce AreaEffect without it's factory registered.");
        }
    }

    FieldOfForce* AreaEffectManager::CreateFieldOfForce(const XML::Node& SelfRoot)
    {
        FactoryIterator AEFactIt = this->AreaEffectFactories.find( FieldOfForce::GetSerializableName() );
        if( AEFactIt != this->AreaEffectFactories.end() ) {
            FieldOfForce* Ret = static_cast<FieldOfForceFactory*>( (*AEFactIt).second )->CreateFieldOfForce( SelfRoot, this->ParentWorld );
            this->AreaEffects.push_back( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to create a FieldOfForce AreaEffect without it's factory registered.");
        }
    }

    GravityField* AreaEffectManager::CreateGravityField(const String& Name)
    {
        FactoryIterator AEFactIt = this->AreaEffectFactories.find( GravityField::GetSerializableName() );
        if( AEFactIt != this->AreaEffectFactories.end() ) {
            GravityField* Ret = static_cast<GravityFieldFactory*>( (*AEFactIt).second )->CreateGravityField( Name, this->ParentWorld );
            this->AreaEffects.push_back( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to create a GravityField AreaEffect without it's factory registered.");
        }
    }

    GravityField* AreaEffectManager::CreateGravityField(const XML::Node& SelfRoot)
    {
        FactoryIterator AEFactIt = this->AreaEffectFactories.find( GravityField::GetSerializableName() );
        if( AEFactIt != this->AreaEffectFactories.end() ) {
            GravityField* Ret = static_cast<GravityFieldFactory*>( (*AEFactIt).second )->CreateGravityField( SelfRoot, this->ParentWorld );
            this->AreaEffects.push_back( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to create a GravityField AreaEffect without it's factory registered.");
        }
    }

    GravityWell* AreaEffectManager::CreateGravityWell(const String& Name)
    {
        FactoryIterator AEFactIt = this->AreaEffectFactories.find( GravityWell::GetSerializableName() );
        if( AEFactIt != this->AreaEffectFactories.end() ) {
            GravityWell* Ret = static_cast<GravityWellFactory*>( (*AEFactIt).second )->CreateGravityWell( Name, this->ParentWorld );
            this->AreaEffects.push_back( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to create a GravityWell AreaEffect without it's factory registered.");
        }
    }

    GravityWell* AreaEffectManager::CreateGravityWell(const XML::Node& SelfRoot)
    {
        FactoryIterator AEFactIt = this->AreaEffectFactories.find( GravityWell::GetSerializableName() );
        if( AEFactIt != this->AreaEffectFactories.end() ) {
            GravityWell* Ret = static_cast<GravityWellFactory*>( (*AEFactIt).second )->CreateGravityWell( SelfRoot, this->ParentWorld );
            this->AreaEffects.push_back( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to create a GravityWell AreaEffect without it's factory registered.");
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // AreaEffect Management

    AreaEffect* AreaEffectManager::CreateAreaEffect(const XML::Node& SelfRoot)
    {
        FactoryIterator AEFactIt = this->AreaEffectFactories.find( SelfRoot.Name() );
        if( AEFactIt != this->AreaEffectFactories.end() ) {
            AreaEffect* Ret = (*AEFactIt).second->CreateAreaEffect( SelfRoot, this->ParentWorld );
            this->AreaEffects.push_back( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to create an AreaEffect of unknown type.");
        }
    }

    AreaEffect* AreaEffectManager::GetAreaEffect(const Whole Index) const
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

    void AreaEffectManager::DestroyAreaEffect(const Whole Index)
    {
        AreaEffectIterator AEIt = ( Index < this->GetNumAreaEffects() ? this->AreaEffects.begin() + Index : this->AreaEffects.end() );
        if( AEIt != this->AreaEffects.end() )
        {
            FactoryIterator AEFactIt = this->AreaEffectFactories.find( (*AEIt)->GetDerivedSerializableName() );
            if( AEFactIt != this->AreaEffectFactories.end() ) {
                (*AEFactIt).second->DestroyAreaEffect( (*AEIt) );
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to destroy an AreaEffect of unknown type.");
            }

            this->AreaEffects.erase(AEIt);
        }
    }

    void AreaEffectManager::DestroyAreaEffect(AreaEffect* ToBeDestroyed)
    {
        AreaEffectIterator AEIt = std::find( this->AreaEffects.begin(), this->AreaEffects.end(), ToBeDestroyed );
        if( AEIt != this->AreaEffects.end() )
        {
            FactoryIterator AEFactIt = this->AreaEffectFactories.find( (*AEIt)->GetDerivedSerializableName() );
            if( AEFactIt != this->AreaEffectFactories.end() ) {
                (*AEFactIt).second->DestroyAreaEffect( (*AEIt) );
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to destroy an AreaEffect of unknown type.");
            }

            this->AreaEffects.erase(AEIt);
        }
    }

    void AreaEffectManager::DestroyAllAreaEffects()
    {
        for( AreaEffectIterator AEIt = this->AreaEffects.begin() ; AEIt != this->AreaEffects.end() ; ++AEIt )
        {
            FactoryIterator AEFactIt = this->AreaEffectFactories.find( (*AEIt)->GetDerivedSerializableName() );
            if( AEFactIt != this->AreaEffectFactories.end() ) {
                (*AEFactIt).second->DestroyAreaEffect( (*AEIt) );
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to destroy an AreaEffect of unknown type.");
            }
        }
        this->AreaEffects.clear();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // AreaEffectFactory Management

    void AreaEffectManager::AddAreaEffectFactory(AreaEffectFactory* ToBeAdded)
    {
        this->AreaEffectFactories.insert(std::pair<String,AreaEffectFactory*>(ToBeAdded->GetTypeName(),ToBeAdded));
    }

    void AreaEffectManager::RemoveAreaEffectFactory(AreaEffectFactory* ToBeRemoved)
    {
        this->RemoveAreaEffectFactory(ToBeRemoved->GetTypeName());
    }

    void AreaEffectManager::RemoveAreaEffectFactory(const String& ImplName)
    {
        FactoryIterator AEFactIt = this->AreaEffectFactories.find(ImplName);
        if( AEFactIt != this->AreaEffectFactories.end() )
            { this->AreaEffectFactories.erase(AEFactIt); }
    }

    void AreaEffectManager::DestroyAreaEffectFactory(AreaEffectFactory* ToBeDestroyed)
    {
        this->DestroyAreaEffectFactory(ToBeDestroyed->GetTypeName());
    }

    void AreaEffectManager::DestroyAreaEffectFactory(const String& ImplName)
    {
        FactoryIterator AEFactIt = this->AreaEffectFactories.find(ImplName);
        if( AEFactIt != this->AreaEffectFactories.end() ) {
            delete AEFactIt->second;
            this->AreaEffectFactories.erase(AEFactIt);
        }
    }

    void AreaEffectManager::DestroyAllAreaEffectFactories()
    {
        for( FactoryIterator AEFactIt = this->AreaEffectFactories.begin() ; AEFactIt != this->AreaEffectFactories.end() ; ++AEFactIt )
            { delete (*AEFactIt).second; }
        this->AreaEffectFactories.clear();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void AreaEffectManager::Pause(const UInt32 PL)
    {
        // Do nothing currently
    }

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
            //WorldManager::Initialize();

            this->TheEntresol->GetScheduler().AddWorkUnitMain( this->AreaEffectUpdateWork, "AreaEffectUpdateWork" );

            Physics::PhysicsManager* PhysicsMan = this->TheEntresol->GetPhysicsManager();
            if( PhysicsMan ) {
                this->AreaEffectUpdateWork->AddDependency( PhysicsMan->GetSimulationWork() );
                this->AreaEffectUpdateWork->AddDependency( PhysicsMan->GetWorldTriggerUpdateWork() );
                this->AreaEffectUpdateWork->AddDependency( PhysicsMan->GetDebugDrawWork() );
            }

            Mezzanine::ActorManager* ActorMan = this->TheEntresol->GetActorManager();
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
        { return this->AreaEffectUpdateWork; }

    ///////////////////////////////////////////////////////////////////////////////
    // Type Identifier Methods

    ManagerBase::ManagerType AreaEffectManager::GetInterfaceType() const
        { return ManagerBase::MT_AreaEffectManager; }

    String AreaEffectManager::GetImplementationTypeName() const
        { return "DefaultAreaEffectManager"; }

    ///////////////////////////////////////////////////////////////////////////////
    // DefaultAreaEffectManagerFactory Methods

    DefaultAreaEffectManagerFactory::DefaultAreaEffectManagerFactory()
        {  }

    DefaultAreaEffectManagerFactory::~DefaultAreaEffectManagerFactory()
        {  }

    String DefaultAreaEffectManagerFactory::GetManagerTypeName() const
        { return "DefaultAreaEffectManager"; }

    ManagerBase* DefaultAreaEffectManagerFactory::CreateManager(NameValuePairList& Params)
        { return new AreaEffectManager(); }

    ManagerBase* DefaultAreaEffectManagerFactory::CreateManager(XML::Node& XMLNode)
        { return new AreaEffectManager(XMLNode); }

    void DefaultAreaEffectManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
        { delete ToBeDestroyed; }
}//Mezzanine

#endif
