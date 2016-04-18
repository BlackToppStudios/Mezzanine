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
#ifndef vehiclemanager_h
#define vehiclemanager_h

#include "worldmanager.h"
#include "worldmanagerfactory.h"
#ifndef SWIG
    #include "Threading/workunit.h"
#endif

namespace Mezzanine
{
    class Vehicle;
    class VehicleFactory;
    class VehicleManager;

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a Mezzanine::Threading::iWorkUnit for the updating of vehicles.
    ///////////////////////////////////////
    class MEZZ_LIB VehicleUpdateWorkUnit : public Threading::DefaultWorkUnit
    {
    protected:
        /// @internal
        /// @brief A pointer to the manager this work unit is processing.
        VehicleManager* TargetManager;
        /// @internal
        /// @brief Protected copy constructor.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        VehicleUpdateWorkUnit(const VehicleUpdateWorkUnit& Other);
        /// @internal
        /// @brief Protected assignment operator.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        VehicleUpdateWorkUnit& operator=(const VehicleUpdateWorkUnit& Other);
    public:
        /// @brief Class constructor.
        /// @param Target The VehicleManager this work unit will process during the frame.
        VehicleUpdateWorkUnit(VehicleManager* Target);
        /// @brief Class destructor.
        virtual ~VehicleUpdateWorkUnit();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief This does any required update of the Vehicles stored by it's manager.
        /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
        virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
    };//VehicleUpdateWorkUnit

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A manager responsible for the storage and management of all actors that exist in a world.
    /// @details More or less Management point for a container of actors to help keep them sorted.
    ///////////////////////////////////////
    class MEZZ_LIB VehicleManager : public WorldManager
    {
    public:
        /// @brief Basic container type for VehicleFactory storage by this class.
        typedef std::map<String,VehicleFactory*>       FactoryContainer;
        /// @brief Iterator type for VehicleFactory instances stored by this class.
        typedef FactoryContainer::iterator             FactoryIterator;
        /// @brief Const Iterator type for VehicleFactory instances stored by this class.
        typedef FactoryContainer::const_iterator       ConstFactoryIterator;
        /// @brief Basic container type for @ref Vehicle storage by this class.
        typedef std::vector<Vehicle*>                  VehicleContainer;
        /// @brief Iterator type for @ref Vehicle instances stored by this class.
        typedef VehicleContainer::iterator             VehicleIterator;
        /// @brief Const Iterator type for @ref Vehicle instances stored by this class.
        typedef VehicleContainer::const_iterator       ConstVehicleIterator;

        /// @brief A String containing the name of this manager implementation.
        static const String ImplementationName;
        /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
        static const ManagerBase::ManagerType InterfaceType;
    protected:
        friend class VehicleUpdateWorkUnit;

        /// @internal
        /// @brief A map containing all registered Vehicle type factories.
        static FactoryContainer VehicleFactories;
        /// @internal
        /// @brief Container storing all Vehicles belonging to this manager.
        VehicleContainer Vehicles;

        /// @internal
        /// @brief The work unit that updates all the vehicles stored by this manager.
        VehicleUpdateWorkUnit* VehicleUpdateWork;
        /// @internal
        /// @brief Can be used for thread safe logging and other thread specific resources.
        Threading::DefaultThreadSpecificStorage::Type* ThreadResources;
    public:
        /// @brief Class constructor.
        /// @param Creator The parent world that is creating the manager.
        VehicleManager(World* Creator);
        /// @brief XML constructor.
        /// @param Creator The parent world that is creating the manager.
        /// @param XMLNode The node of the xml document to construct from.
        VehicleManager(World* Creator, const XML::Node& XMLNode);
        /// @brief Class destructor.
        virtual ~VehicleManager();

        ///////////////////////////////////////////////////////////////////////////////
        // Prefab Vehicle Type Creation

        ///////////////////////////////////////////////////////////////////////////////
        // Vehicle Management

        /// @brief Creates a new Vehicle.
        /// @param TypeName A string containing the name of the type of Vehicle to be constructed.
        /// @param InstanceName A string containing the name to be given to the created Vehicle.
        /// @param Params A container of additional parameters to be used for the construction of the new Vehicle.
        /// @return Returns a pointer to the created Vehicle.
        Vehicle* CreateVehicle(const String& TypeName, const String& InstanceName, const NameValuePairMap& Params);
        /// @brief Creates a new Vehicle class from an XML node.
        /// @remarks This is mostly useful for deserialization.
        /// @return Returns a pointer to the created Vehicle.
        Vehicle* CreateVehicle(const XML::Node& SelfRoot);

        /// @brief Gets an Vehicle by Index.
        /// @param Index The index of the Vehicle you wish to retrieve.
        /// @return Returns a pointer to the Vehicle at the specified index.
        virtual Vehicle* GetVehicle(const Whole Index) const;
        /// @brief Gets an Vehicle by Name.
        /// @param Name The name of the Vehicle you wish to retrieve.
        /// @return Returns a pointer to the Vehicle of the specified name.
        virtual Vehicle* GetVehicle(const String& Name) const;
        /// @brief Gets the number of Vehicles stored in this manager.
        /// @return Returns a whole representing the current Vehicle count.
        virtual Whole GetNumVehicles() const;
        /// @brief Destroys an Vehicle at the specified index.
        /// @param Index The index at which to destroy the Vehicle.
        virtual void DestroyVehicle(const Whole Index);
        /// @brief Destroys an Vehicle.
        /// @param ToBeDestroyed The Vehicle to be destroyed.
        virtual void DestroyVehicle(Vehicle* ToBeDestroyed);
        /// @brief Destroys all Vehicles currently within this manager.
        virtual void DestroyAllVehicles();

        #ifndef SWIG
        /// @brief Gets an iterator to the first Vehicle in this manager.
        /// @return Returns an iterator to the first Vehicle being stored by this manager.
        VehicleIterator BeginVehicle();
        /// @brief Gets an iterator to one passed the last Vehicle in this manager.
        /// @return Returns an iterator to one passed the last Vehicle being stored by this manager.
        VehicleIterator EndVehicle();
        /// @brief Gets a const iterator to the first Vehicle in this manager.
        /// @return Returns a const iterator to the first Vehicle being stored by this manager.
        ConstVehicleIterator BeginVehicle() const;
        /// @brief Gets a const iterator to one passed the last Vehicle in this manager.
        /// @return Returns a const iterator to one passed the last Vehicle being stored by this manager.
        ConstVehicleIterator EndVehicle() const;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        // VehicleFactory Management

        /// @brief Adds/registers a Vehicle factory with this manager, allowing it to be constructed through this API.
        /// @param ToBeAdded The Vehicle factory to be added.
        static void AddVehicleFactory(VehicleFactory* ToBeAdded);
        /// @brief Removes a Vehicle factory from this manager.
        /// @param ToBeRemoved A pointer to the Vehicle factory that is to be removed.
        static void RemoveVehicleFactory(VehicleFactory* ToBeRemoved);
        /// @brief Removes a Vehicle factory from this manager.
        /// @param ImplName The name of the Vehicle implementation created by the factory to be removed.
        static void RemoveVehicleFactory(const String& ImplName);
        /// @brief Removes and destroys a Vehicle factory in this manager.
        /// @param ToBeDestroyed A pointer to the Vehicle factory that is to be removed and destroyed.
        static void DestroyVehicleFactory(VehicleFactory* ToBeDestroyed);
        /// @brief Removes and destroys a Vehicle factory in this manager.
        /// @param ImplName The name of the Vehicle implementation created by the factory to be removed and destroyed.
        static void DestroyVehicleFactory(const String& ImplName);
        /// @brief Destroys all Vehicle factories in this manager.
        /// @warning The destruction of Vehicle factories should only be done after all the Vehicle have been destroyed, otherwise this will cause an exception.
        static void DestroyAllVehicleFactories();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @copydoc WorldManager::Pause(const UInt32)
        virtual void Pause(const UInt32 PL);

        /// @copydoc WorldManager::Initialize()
        virtual void Initialize();
        /// @copydoc ManagerBase::Deinitialize()
        virtual void Deinitialize();

        /// @brief Gets the work unit responsible for updating vehicles stored by this manager.
        /// @return Returns a pointer to the VehicleUpdateWorkUnit used by this manager.
        VehicleUpdateWorkUnit* GetVehicleUpdateWork();

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        /// @copydoc ManagerBase::GetInterfaceType()
        virtual ManagerType GetInterfaceType() const;
        /// @copydoc ManagerBase::GetImplementationTypeName()
        virtual String GetImplementationTypeName() const;
    };//VehicleManager

    ///////////////////////////////////////////////////////////////////////////////
    /// @class DefaultVehicleManagerFactory
    /// @brief A factory responsible for the creation and destruction of the default VehicleManager.
    ///////////////////////////////////////
    class DefaultVehicleManagerFactory : public WorldManagerFactory
    {
    public:
        /// @brief Class constructor.
        DefaultVehicleManagerFactory();
        /// @brief Class destructor.
        virtual ~DefaultVehicleManagerFactory();

        /// @copydoc ManagerFactory::GetManagerImplName()
        String GetManagerImplName() const;
        /// @copydoc ManagerFactory::GetManagerType() const
        ManagerBase::ManagerType GetManagerType() const;

        /// @copydoc WorldManagerFactory::CreateManager(World*, const NameValuePairList&)
        WorldManager* CreateManager(World* Creator, const NameValuePairList& Params);
        /// @copydoc WorldManagerFactory::CreateManager(World*, const XML::Node&)
        WorldManager* CreateManager(World* Creator, const XML::Node& XMLNode);
        /// @copydoc WorldManagerFactory::DestroyManager(WorldManager*)
        void DestroyManager(WorldManager* ToBeDestroyed);
    };//DefaultVehicleManagerFactory
}//Mezzanine

#endif
