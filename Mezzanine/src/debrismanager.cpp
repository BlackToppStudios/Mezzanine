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
#ifndef _debrismanager_cpp
#define _debrismanager_cpp

/// @file
/// @brief This file contains the implementation for the manager that manages debris objects in a world.

#include "debrismanager.h"
#include "rigiddebris.h"
#include "softdebris.h"

#include "Physics/physicsmanager.h"
#include "entresol.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // DebrisUpdateWorkUnit Methods

    DebrisUpdateWorkUnit::DebrisUpdateWorkUnit(const DebrisUpdateWorkUnit& Other)
        {  }

    DebrisUpdateWorkUnit& DebrisUpdateWorkUnit::operator=(const DebrisUpdateWorkUnit& Other)
        { return *this; }

    DebrisUpdateWorkUnit::DebrisUpdateWorkUnit(DebrisManager* Target) :
        TargetManager(Target) {  }

    DebrisUpdateWorkUnit::~DebrisUpdateWorkUnit()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void DebrisUpdateWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
    {
        for( DebrisManager::DebrisIterator DebIt = this->TargetManager->Debriss.begin() ; DebIt != this->TargetManager->Debriss.end() ; ++DebIt )
        {
            (*DebIt)->_Update();
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // DebrisManager Methods

    DebrisManager::DebrisManager()
    {
        this->AddDebrisFactory( new RigidDebrisFactory() );
        this->AddDebrisFactory( new SoftDebrisFactory() );

        this->DebrisUpdateWork = new DebrisUpdateWorkUnit(this);
    }

    DebrisManager::DebrisManager(XML::Node& XMLNode)
    {
        /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.

        this->AddDebrisFactory( new RigidDebrisFactory() );
        this->AddDebrisFactory( new SoftDebrisFactory() );

        this->DebrisUpdateWork = new DebrisUpdateWorkUnit(this);
    }

    DebrisManager::~DebrisManager()
    {
        this->Deinitialize();
        this->DestroyAllDebris();
        this->DestroyAllDebrisFactories();

        delete this->DebrisUpdateWork;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Prefab Debris Type Creation

    RigidDebris* DebrisManager::CreateRigidDebris(const String& Name, const Real Mass, const Boole AddToWorld)
    {
        FactoryIterator DebFactIt = this->DebrisFactories.find( RigidDebris::GetSerializableName() );
        if( DebFactIt != this->DebrisFactories.end() ) {
            RigidDebris* Ret = static_cast<RigidDebrisFactory*>( (*DebFactIt).second )->CreateRigidDebris( Name, Mass, this->ParentWorld );
            this->Debriss.push_back( Ret );
            if( AddToWorld ) {
                Ret->AddToWorld();
            }
            return Ret;
        }else{
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to create a RigidDebris without it's factory registered.");
        }
    }

    RigidDebris* DebrisManager::CreateRigidDebris(const String& Name, const Real Mass, Graphics::Mesh* DebMesh, Physics::CollisionShape* DebShape, const Boole AddToWorld)
    {
        FactoryIterator DebFactIt = this->DebrisFactories.find( RigidDebris::GetSerializableName() );
        if( DebFactIt != this->DebrisFactories.end() ) {
            RigidDebris* Ret = static_cast<RigidDebrisFactory*>( (*DebFactIt).second )->CreateRigidDebris( Name, Mass, DebMesh, DebShape, this->ParentWorld );
            this->Debriss.push_back( Ret );
            if( AddToWorld ) {
                Ret->AddToWorld();
            }
            return Ret;
        }else{
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to create a RigidDebris without it's factory registered.");
        }
    }

    RigidDebris* DebrisManager::CreateRigidDebris(const XML::Node& SelfRoot)
    {
        FactoryIterator DebFactIt = this->DebrisFactories.find( RigidDebris::GetSerializableName() );
        if( DebFactIt != this->DebrisFactories.end() ) {
            RigidDebris* Ret = static_cast<RigidDebrisFactory*>( (*DebFactIt).second )->CreateRigidDebris( SelfRoot, this->ParentWorld );
            this->Debriss.push_back( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to create a RigidDebris without it's factory registered.");
        }
    }

    SoftDebris* DebrisManager::CreateSoftDebris(const String& Name, const Real Mass, const Boole AddToWorld)
    {
        FactoryIterator DebFactIt = this->DebrisFactories.find( SoftDebris::GetSerializableName() );
        if( DebFactIt != this->DebrisFactories.end() ) {
            SoftDebris* Ret = static_cast<SoftDebrisFactory*>( (*DebFactIt).second )->CreateSoftDebris( Name, Mass, this->ParentWorld );
            this->Debriss.push_back( Ret );
            if( AddToWorld ) {
                Ret->AddToWorld();
            }
            return Ret;
        }else{
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to create a SoftDebris without it's factory registered.");
        }
    }

    SoftDebris* DebrisManager::CreateSoftDebris(const XML::Node& SelfRoot)
    {
        FactoryIterator DebFactIt = this->DebrisFactories.find( SoftDebris::GetSerializableName() );
        if( DebFactIt != this->DebrisFactories.end() ) {
            SoftDebris* Ret = static_cast<SoftDebrisFactory*>( (*DebFactIt).second )->CreateSoftDebris( SelfRoot, this->ParentWorld );
            this->Debriss.push_back( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to create a SoftDebris without it's factory registered.");
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Debris Management

    Debris* DebrisManager::CreateDebris(const String& TypeName, const String& InstanceName, const NameValuePairMap& Params, const Boole AddToWorld)
    {
        FactoryIterator DebFactIt = this->DebrisFactories.find( TypeName );
        if( DebFactIt != this->DebrisFactories.end() ) {
            Debris* Ret = (*DebFactIt).second->CreateDebris( InstanceName, this->ParentWorld, Params );
            this->Debriss.push_back( Ret );
            if( AddToWorld ) {
                Ret->AddToWorld();
            }
            return Ret;
        }else{
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to create an Debris of unknown type.");
        }
    }

    Debris* DebrisManager::CreateDebris(const XML::Node& SelfRoot)
    {
        FactoryIterator DebFactIt = this->DebrisFactories.find( SelfRoot.Name() );
        if( DebFactIt != this->DebrisFactories.end() ) {
            Debris* Ret = (*DebFactIt).second->CreateDebris( SelfRoot, this->ParentWorld );
            this->Debriss.push_back( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to create a Debris of unknown type.");
        }
    }

    Debris* DebrisManager::GetDebris(const Whole Index) const
    {
        return this->Debriss.at(Index);
    }

    Debris* DebrisManager::GetDebris(const String& Name) const
    {
        for( ConstDebrisIterator DebIt = this->Debriss.begin() ; DebIt != this->Debriss.end() ; ++DebIt )
        {
            if( (*DebIt)->GetName() == Name )
                return (*DebIt);
        }
        return NULL;
    }

    Whole DebrisManager::GetNumDebris() const
    {
        return this->Debriss.size();
    }

    void DebrisManager::DestroyDebris(const Whole Index)
    {
        DebrisIterator DebIt = ( Index < this->GetNumDebris() ? this->Debriss.begin() + Index : this->Debriss.end() );
        if( DebIt != this->Debriss.end() )
        {
            FactoryIterator DebFactIt = this->DebrisFactories.find( (*DebIt)->GetDerivedSerializableName() );
            if( DebFactIt != this->DebrisFactories.end() ) {
                (*DebFactIt).second->DestroyDebris( (*DebIt) );
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to destroy a Debris of unknown type.");
            }

            this->Debriss.erase(DebIt);
        }
    }

    void DebrisManager::DestroyDebris(Debris* ToBeDestroyed)
    {
        DebrisIterator DebIt = std::find( this->Debriss.begin(), this->Debriss.end(), ToBeDestroyed );
        if( DebIt != this->Debriss.end() )
        {
            FactoryIterator DebFactIt = this->DebrisFactories.find( (*DebIt)->GetDerivedSerializableName() );
            if( DebFactIt != this->DebrisFactories.end() ) {
                (*DebFactIt).second->DestroyDebris( (*DebIt) );
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to destroy a Debris of unknown type.");
            }

            this->Debriss.erase(DebIt);
        }
    }

    void DebrisManager::DestroyAllDebris()
    {
        for( DebrisIterator DebIt = this->Debriss.begin() ; DebIt != this->Debriss.end() ; ++DebIt )
        {
            FactoryIterator DebFactIt = this->DebrisFactories.find( (*DebIt)->GetDerivedSerializableName() );
            if( DebFactIt != this->DebrisFactories.end() ) {
                (*DebFactIt).second->DestroyDebris( (*DebIt) );
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to destroy a Debris of unknown type.");
            }
        }
        this->Debriss.clear();
    }

    DebrisManager::DebrisIterator DebrisManager::BeginDebris()
        { return this->Debriss.begin(); }

    DebrisManager::DebrisIterator DebrisManager::EndDebris()
        { return this->Debriss.end(); }

    DebrisManager::ConstDebrisIterator DebrisManager::BeginDebris() const
        { return this->Debriss.begin(); }

    DebrisManager::ConstDebrisIterator DebrisManager::EndDebris() const
        { return this->Debriss.end(); }

    ///////////////////////////////////////////////////////////////////////////////
    // DebrisFactory Management

    void DebrisManager::AddDebrisFactory(DebrisFactory* ToBeAdded)
    {
        this->DebrisFactories.insert(std::pair<String,DebrisFactory*>(ToBeAdded->GetTypeName(),ToBeAdded));
    }

    void DebrisManager::RemoveDebrisFactory(DebrisFactory* ToBeRemoved)
    {
        this->RemoveDebrisFactory(ToBeRemoved->GetTypeName());
    }

    void DebrisManager::RemoveDebrisFactory(const String& ImplName)
    {
        FactoryIterator DebFactIt = this->DebrisFactories.find(ImplName);
        if( DebFactIt != this->DebrisFactories.end() )
            { this->DebrisFactories.erase(DebFactIt); }
    }

    void DebrisManager::DestroyDebrisFactory(DebrisFactory* ToBeDestroyed)
    {
        this->DestroyDebrisFactory(ToBeDestroyed->GetTypeName());
    }

    void DebrisManager::DestroyDebrisFactory(const String& ImplName)
    {
        FactoryIterator DebFactIt = this->DebrisFactories.find(ImplName);
        if( DebFactIt != this->DebrisFactories.end() ) {
            delete DebFactIt->second;
            this->DebrisFactories.erase(DebFactIt);
        }
    }

    void DebrisManager::DestroyAllDebrisFactories()
    {
        for( FactoryIterator DebFactIt = this->DebrisFactories.begin() ; DebFactIt != this->DebrisFactories.end() ; ++DebFactIt )
            { delete (*DebFactIt).second; }
        this->DebrisFactories.clear();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void DebrisManager::Pause(const UInt32 PL)
    {
        // Do nothing currently
    }

    void DebrisManager::Initialize()
    {
        if( !this->Initialized )
        {
            //WorldManager::Initialize();

            this->TheEntresol->GetScheduler().AddWorkUnitMain( this->DebrisUpdateWork, "DebrisUpdateWork" );
            Physics::PhysicsManager* PhysicsMan = Entresol::GetSingletonPtr()->GetPhysicsManager();
            if( PhysicsMan ) {
                this->DebrisUpdateWork->AddDependency( PhysicsMan->GetSimulationWork() );
            }

            this->Initialized = true;
        }
    }

    void DebrisManager::Deinitialize()
    {
        if( this->Initialized )
        {
            this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->DebrisUpdateWork );
            this->DebrisUpdateWork->ClearDependencies();

            this->Initialized = false;
        }
    }

    DebrisUpdateWorkUnit* DebrisManager::GetDebrisUpdateWork()
        { return this->DebrisUpdateWork; }

    ///////////////////////////////////////////////////////////////////////////////
    // Type Identifier Methods

    ManagerBase::ManagerType DebrisManager::GetInterfaceType() const
        { return ManagerBase::MT_DebrisManager; }

    String DebrisManager::GetImplementationTypeName() const
        { return "DefaultDebrisManager"; }

    ///////////////////////////////////////////////////////////////////////////////
    // DefaultDebrisManagerFactory Methods

    DefaultDebrisManagerFactory::DefaultDebrisManagerFactory()
        {  }

    DefaultDebrisManagerFactory::~DefaultDebrisManagerFactory()
        {  }

    String DefaultDebrisManagerFactory::GetManagerTypeName() const
        { return "DefaultDebrisManager"; }

    ManagerBase* DefaultDebrisManagerFactory::CreateManager(NameValuePairList& Params)
        { return new DebrisManager(); }

    ManagerBase* DefaultDebrisManagerFactory::CreateManager(XML::Node& XMLNode)
        { return new DebrisManager(XMLNode); }

    void DefaultDebrisManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
        { delete ToBeDestroyed; }
}//Mezzanine

#endif
