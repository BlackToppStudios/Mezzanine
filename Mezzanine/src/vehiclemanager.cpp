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
#ifndef vehiclemanager_cpp
#define vehiclemanager_cpp

#include "vehiclemanager.h"
#include "vehicle.h"
#include "Physics/physicsmanager.h"
#include "entresol.h"
#include "world.h"

#include <sstream>
#include <algorithm>

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // VehicleUpdateWorkUnit Methods

    VehicleUpdateWorkUnit::VehicleUpdateWorkUnit(const VehicleUpdateWorkUnit& Other)
        {  }

    VehicleUpdateWorkUnit& VehicleUpdateWorkUnit::operator=(const VehicleUpdateWorkUnit& Other)
        { return *this; }

    VehicleUpdateWorkUnit::VehicleUpdateWorkUnit(VehicleManager* Target) :
        TargetManager(Target) {  }

    VehicleUpdateWorkUnit::~VehicleUpdateWorkUnit()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void VehicleUpdateWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
    {
        for( VehicleManager::VehicleIterator VehIt = this->TargetManager->Vehicles.begin() ; VehIt != this->TargetManager->Vehicles.end() ; ++VehIt )
        {
            (*VehIt)->_Update();
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // VehicleManager Methods

    const String VehicleManager::ImplementationName = "DefaultVehicleManager";
    const ManagerBase::ManagerType VehicleManager::InterfaceType = ManagerBase::MT_VehicleManager;

    VehicleManager::VehicleManager(World* Creator) :
        WorldManager(Creator),
        VehicleUpdateWork(NULL),
        ThreadResources(NULL)
    {
        this->VehicleUpdateWork = new VehicleUpdateWorkUnit(this);
    }

    VehicleManager::VehicleManager(World* Creator, const XML::Node& XMLNode) :
        WorldManager(Creator),
        VehicleUpdateWork(NULL),
        ThreadResources(NULL)
    {
        /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.

        this->VehicleUpdateWork = new VehicleUpdateWorkUnit(this);
    }

    VehicleManager::~VehicleManager()
    {
        this->Deinitialize();
        this->DestroyAllVehicles();

        delete this->VehicleUpdateWork;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Prefab Vehicle Type Creation

    ///////////////////////////////////////////////////////////////////////////////
    // Vehicle Management

    Vehicle* VehicleManager::CreateVehicle(const String& TypeName, const String& InstanceName, const NameValuePairMap& Params)
    {
        FactoryIterator VehFactIt = this->VehicleFactories.find( TypeName );
        if( VehFactIt != this->VehicleFactories.end() ) {
            Vehicle* Ret = (*VehFactIt).second->CreateVehicle( InstanceName, this->ParentWorld, Params );
            this->Vehicles.push_back( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create an Vehicle of unknown type.");
        }
    }

    Vehicle* VehicleManager::CreateVehicle(const XML::Node& SelfRoot)
    {
        FactoryIterator VehFactIt = this->VehicleFactories.find( SelfRoot.Name() );
        if( VehFactIt != this->VehicleFactories.end() ) {
            Vehicle* Ret = (*VehFactIt).second->CreateVehicle( SelfRoot, this->ParentWorld );
            this->Vehicles.push_back( Ret );
            return Ret;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a Vehicle of unknown type.");
        }
    }

    Vehicle* VehicleManager::GetVehicle(const Whole Index) const
    {
        return this->Vehicles.at(Index);
    }

    Vehicle* VehicleManager::GetVehicle(const String& Name) const
    {
        for( ConstVehicleIterator VehIt = this->Vehicles.begin() ; VehIt != this->Vehicles.end() ; ++VehIt )
        {
            if( (*VehIt)->GetName() == Name )
                return (*VehIt);
        }
        return NULL;
    }

    Whole VehicleManager::GetNumVehicles() const
    {
        return this->Vehicles.size();
    }

    void VehicleManager::DestroyVehicle(const Whole Index)
    {
        VehicleIterator VehIt = ( Index < this->GetNumVehicles() ? this->Vehicles.begin() + Index : this->Vehicles.end() );
        if( VehIt != this->Vehicles.end() )
        {
            FactoryIterator VehFactIt = this->VehicleFactories.find( (*VehIt)->GetDerivedSerializableName() );
            if( VehFactIt != this->VehicleFactories.end() ) {
                (*VehFactIt).second->DestroyVehicle( (*VehIt) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to destroy a Vehicle of unknown type.");
            }

            this->Vehicles.erase(VehIt);
        }
    }

    void VehicleManager::DestroyVehicle(Vehicle* ToBeDestroyed)
    {
        VehicleIterator VehIt = std::find( this->Vehicles.begin(), this->Vehicles.end(), ToBeDestroyed );
        if( VehIt != this->Vehicles.end() )
        {
            FactoryIterator VehFactIt = this->VehicleFactories.find( (*VehIt)->GetDerivedSerializableName() );
            if( VehFactIt != this->VehicleFactories.end() ) {
                (*VehFactIt).second->DestroyVehicle( (*VehIt) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to destroy a Vehicle of unknown type.");
            }

            this->Vehicles.erase(VehIt);
        }
    }

    void VehicleManager::DestroyAllVehicles()
    {
        for( VehicleIterator VehIt = this->Vehicles.begin() ; VehIt != this->Vehicles.end() ; ++VehIt )
        {
            FactoryIterator VehFactIt = this->VehicleFactories.find( (*VehIt)->GetDerivedSerializableName() );
            if( VehFactIt != this->VehicleFactories.end() ) {
                (*VehFactIt).second->DestroyVehicle( (*VehIt) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to destroy a Vehicle of unknown type.");
            }
        }
        this->Vehicles.clear();
    }

    VehicleManager::VehicleIterator VehicleManager::BeginVehicle()
        { return this->Vehicles.begin(); }

    VehicleManager::VehicleIterator VehicleManager::EndVehicle()
        { return this->Vehicles.end(); }

    VehicleManager::ConstVehicleIterator VehicleManager::BeginVehicle() const
        { return this->Vehicles.begin(); }

    VehicleManager::ConstVehicleIterator VehicleManager::EndVehicle() const
        { return this->Vehicles.end(); }

    ///////////////////////////////////////////////////////////////////////////////
    // VehicleFactory Management

    void VehicleManager::AddVehicleFactory(VehicleFactory* ToBeAdded)
    {
        this->VehicleFactories.insert(std::pair<String,VehicleFactory*>(ToBeAdded->GetTypeName(),ToBeAdded));
    }

    void VehicleManager::RemoveVehicleFactory(VehicleFactory* ToBeRemoved)
    {
        this->RemoveVehicleFactory(ToBeRemoved->GetTypeName());
    }

    void VehicleManager::RemoveVehicleFactory(const String& ImplName)
    {
        FactoryIterator VehFactIt = this->VehicleFactories.find(ImplName);
        if( VehFactIt != this->VehicleFactories.end() )
            { this->VehicleFactories.erase(VehFactIt); }
    }

    void VehicleManager::DestroyVehicleFactory(VehicleFactory* ToBeDestroyed)
    {
        this->DestroyVehicleFactory(ToBeDestroyed->GetTypeName());
    }

    void VehicleManager::DestroyVehicleFactory(const String& ImplName)
    {
        FactoryIterator VehFactIt = this->VehicleFactories.find(ImplName);
        if( VehFactIt != this->VehicleFactories.end() ) {
            delete VehFactIt->second;
            this->VehicleFactories.erase(VehFactIt);
        }
    }

    void VehicleManager::DestroyAllVehicleFactories()
    {
        for( FactoryIterator VehFactIt = this->VehicleFactories.begin() ; VehFactIt != this->VehicleFactories.end() ; ++VehFactIt )
            { delete (*VehFactIt).second; }
        this->VehicleFactories.clear();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void VehicleManager::Pause(const UInt32 PL)
    {
        // Do nothing currently
    }

    void VehicleManager::Initialize()
    {
        if( !this->Initialized )
        {
            WorldManager::Initialize();

            this->TheEntresol->GetScheduler().AddWorkUnitMain( this->VehicleUpdateWork, "VehicleUpdateWork" );
            Physics::PhysicsManager* PhysicsMan = static_cast<Physics::PhysicsManager*>( this->ParentWorld->GetManager(ManagerBase::MT_PhysicsManager) );
            if( PhysicsMan ) {
                this->VehicleUpdateWork->AddDependency( PhysicsMan->GetSimulationWork() );
            }

            this->Initialized = true;
        }
    }

    void VehicleManager::Deinitialize()
    {
        if( this->Initialized )
        {
            this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->VehicleUpdateWork );
            this->VehicleUpdateWork->ClearDependencies();

            this->Initialized = false;
        }
    }

    VehicleUpdateWorkUnit* VehicleManager::GetVehicleUpdateWork()
        { return this->VehicleUpdateWork; }

    ///////////////////////////////////////////////////////////////////////////////
    // Type Identifier Methods

    ManagerBase::ManagerType VehicleManager::GetInterfaceType() const
        { return VehicleManager::InterfaceType; }

    String VehicleManager::GetImplementationTypeName() const
        { return VehicleManager::ImplementationName; }

    ///////////////////////////////////////////////////////////////////////////////
    // DefaultVehicleManagerFactory Methods

    DefaultVehicleManagerFactory::DefaultVehicleManagerFactory()
        {  }

    DefaultVehicleManagerFactory::~DefaultVehicleManagerFactory()
        {  }

    String DefaultVehicleManagerFactory::GetManagerImplName() const
        { return VehicleManager::ImplementationName; }

    ManagerBase::ManagerType DefaultVehicleManagerFactory::GetManagerType() const
        { return VehicleManager::InterfaceType; }

    WorldManager* DefaultVehicleManagerFactory::CreateManager(World* Creator, const NameValuePairList& Params)
        { return new VehicleManager(Creator); }

    WorldManager* DefaultVehicleManagerFactory::CreateManager(World* Creator, const XML::Node& XMLNode)
        { return new VehicleManager(Creator,XMLNode); }

    void DefaultVehicleManagerFactory::DestroyManager(WorldManager* ToBeDestroyed)
        { delete ToBeDestroyed; }
}//Mezzanine

#endif
