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
#ifndef _entitymanager_h
#define _entitymanager_h

#include "worldmanager.h"
#include "worldmanagerfactory.h"
#include "iteratorrange.h"
#ifndef SWIG
    #include "eventpublisher.h"
    #include "Threading/workunit.h"
#endif

namespace Mezzanine
{
    namespace Graphics
    {
        class Mesh;
    }
    namespace Physics
    {
        class CollisionShape;
    }
    class Entity;
    class EntityFactory;
    class EntityManager;
    class FieldOfForce;
    class GravityField;
    class GravityWell;
    class RigidDebris;
    class SoftDebris;
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a Mezzanine::Threading::iWorkUnit for the updating of Actors.
    ///////////////////////////////////////
    class MEZZ_LIB ActorUpdateWorkUnit : public Threading::DefaultWorkUnit
    {
    protected:
        /// @brief A pointer to the manager this work unit is processing.
        EntityManager* TargetManager;
    public:
        /// @brief Class constructor.
        /// @param Target The EntityManager this work unit will process during the frame.
        ActorUpdateWorkUnit(EntityManager* Target);
        /// @brief Copy constructor.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        ActorUpdateWorkUnit(const ActorUpdateWorkUnit& Other) = delete;
        /// @brief Move constructor.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being moved.  WHICH WILL NEVER HAPPEN.
        ActorUpdateWorkUnit(ActorUpdateWorkUnit&& Other) = delete;
        /// @brief Class destructor.
        virtual ~ActorUpdateWorkUnit() = default;

        /// @brief Assignment Operator.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        ActorUpdateWorkUnit& operator=(const ActorUpdateWorkUnit& Other) = delete;
        /// @brief Move Assignment Operator.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being moved.  WHICH WILL NEVER HAPPEN.
        ActorUpdateWorkUnit& operator=(ActorUpdateWorkUnit&& Other) = delete;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief This does any required update of the Actors stored by it's manager.
        /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
        virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
    };//ActorUpdateWorkUnit

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a Mezzanine::Threading::iWorkUnit for the updating of AreaEffects.
    ///////////////////////////////////////
    class MEZZ_LIB AreaEffectUpdateWorkUnit : public Threading::DefaultWorkUnit
    {
    protected:
        /// @brief A pointer to the manager this work unit is processing.
        EntityManager* TargetManager;
    public:
        /// @brief Class constructor.
        /// @param Target The EntityManager this work unit will process during the frame.
        AreaEffectUpdateWorkUnit(EntityManager* Target);
        /// @brief Copy constructor.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        AreaEffectUpdateWorkUnit(const AreaEffectUpdateWorkUnit& Other) = delete;
        /// @brief Move constructor.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being moved.  WHICH WILL NEVER HAPPEN.
        AreaEffectUpdateWorkUnit(AreaEffectUpdateWorkUnit&& Other) = delete;
        /// @brief Class destructor.
        virtual ~AreaEffectUpdateWorkUnit() = default;

        /// @brief Assignment Operator.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        AreaEffectUpdateWorkUnit& operator=(const AreaEffectUpdateWorkUnit& Other) = delete;
        /// @brief Move Assignment Operator.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being moved.  WHICH WILL NEVER HAPPEN.
        AreaEffectUpdateWorkUnit& operator=(AreaEffectUpdateWorkUnit&& Other) = delete;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief This does any required update of the AreaEffects stored by it's manager.
        /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
        virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
    };//AreaEffectUpdateWorkUnit

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a Mezzanine::Threading::iWorkUnit for the updating of Debris.
    ///////////////////////////////////////
    class MEZZ_LIB DebrisUpdateWorkUnit : public Threading::DefaultWorkUnit
    {
    protected:
        /// @brief A pointer to the manager this work unit is processing.
        EntityManager* TargetManager;
    public:
        /// @brief Class constructor.
        /// @param Target The EntityManager this work unit will process during the frame.
        DebrisUpdateWorkUnit(EntityManager* Target);
        /// @brief Copy constructor.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        DebrisUpdateWorkUnit(const DebrisUpdateWorkUnit& Other) = delete;
        /// @brief Move constructor.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being moved.  WHICH WILL NEVER HAPPEN.
        DebrisUpdateWorkUnit(DebrisUpdateWorkUnit&& Other) = delete;
        /// @brief Class destructor.
        virtual ~DebrisUpdateWorkUnit() = default;

        /// @brief Assignment Operator.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        DebrisUpdateWorkUnit& operator=(const DebrisUpdateWorkUnit& Other) = delete;
        /// @brief Move Assignment Operator.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being moved.  WHICH WILL NEVER HAPPEN.
        DebrisUpdateWorkUnit& operator=(DebrisUpdateWorkUnit&& Other) = delete;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief This does any required update of the Debriss stored by it's manager.
        /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
        virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
    };//DebrisUpdateWorkUnit

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a Mezzanine::Threading::iWorkUnit for the updating of Terrains.
    ///////////////////////////////////////
    class MEZZ_LIB TerrainUpdateWorkUnit : public Threading::DefaultWorkUnit
    {
    protected:
        /// @brief A pointer to the manager this work unit is processing.
        EntityManager* TargetManager;
    public:
        /// @brief Class constructor.
        /// @param Target The EntityManager this work unit will process during the frame.
        TerrainUpdateWorkUnit(EntityManager* Target);
        /// @brief Copy constructor.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        TerrainUpdateWorkUnit(const TerrainUpdateWorkUnit& Other) = delete;
        /// @brief Move constructor.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being moved.  WHICH WILL NEVER HAPPEN.
        TerrainUpdateWorkUnit(TerrainUpdateWorkUnit&& Other) = delete;
        /// @brief Class destructor.
        virtual ~TerrainUpdateWorkUnit() = default;

        /// @brief Assignment Operator.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        TerrainUpdateWorkUnit& operator=(const TerrainUpdateWorkUnit& Other) = delete;
        /// @brief Move Assignment Operator.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being moved.  WHICH WILL NEVER HAPPEN.
        TerrainUpdateWorkUnit& operator=(TerrainUpdateWorkUnit&& Other) = delete;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief This does any required update of the Terrains stored by it's manager.
        /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
        virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
    };//TerrainUpdateWorkUnit

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a Mezzanine::Threading::iWorkUnit for the updating of Vehicles.
    ///////////////////////////////////////
    class MEZZ_LIB VehicleUpdateWorkUnit : public Threading::DefaultWorkUnit
    {
    protected:
        /// @brief A pointer to the manager this work unit is processing.
        EntityManager* TargetManager;
    public:
        /// @brief Class constructor.
        /// @param Target The EntityManager this work unit will process during the frame.
        VehicleUpdateWorkUnit(EntityManager* Target);
        /// @brief Copy constructor.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        VehicleUpdateWorkUnit(const VehicleUpdateWorkUnit& Other) = delete;
        /// @brief Move constructor.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being moved.  WHICH WILL NEVER HAPPEN.
        VehicleUpdateWorkUnit(VehicleUpdateWorkUnit&& Other) = delete;
        /// @brief Class destructor.
        virtual ~VehicleUpdateWorkUnit() = default;

        /// @brief Assignment Operator.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        VehicleUpdateWorkUnit& operator=(const VehicleUpdateWorkUnit& Other) = delete;
        /// @brief Move Assignment Operator.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being moved.  WHICH WILL NEVER HAPPEN.
        VehicleUpdateWorkUnit& operator=(VehicleUpdateWorkUnit&& Other) = delete;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief This does any required update of the Vehicles stored by it's manager.
        /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
        virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
    };//VehicleUpdateWorkUnit

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is the event class for high level management events relating to Entities.
    ///////////////////////////////////////
    class MEZZ_LIB EntityManagementEvent : public Event
    {
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief The name of the Entity being managed.
        const String ManagedEntityName;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Text-less constructor.
        /// @param Name The name of the event being fired.
        /// @param Managed The name of the Entity being managed.
        EntityManagementEvent(const EventNameType& Name, const String& ManagedName) :
            Event(Name), ManagedEntityName(ManagedName) {  }
        /// @brief Class destructor.
        virtual ~EntityManagementEvent() = default;
    };//EntityManagementEvent

    /// @brief Convenience type for passing around EntityManagementEvent.
    using EntityManagementEventPtr = std::shared_ptr<EntityManagementEvent>;

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is an interface class for a world manager responsible for the management of Entity instances.
    ///////////////////////////////////////
    class MEZZ_LIB EntityManager : public WorldManager
    {
    public:
        /// @brief Basic container type for EntityFactory storage by this class.
        using FactoryContainer = std::map<String,EntityFactory*>;
        /// @brief Iterator type for EntityFactory instances stored by this class.
        using FactoryIterator = FactoryContainer::iterator;
        /// @brief Const Iterator type for EntityFactory instances stored by this class.
        using ConstFactoryIterator = FactoryContainer::const_iterator;
        /// @brief Basic container type for @ref Entity storage by this class.
        using EntityContainer = std::vector<Entity*>;
        /// @brief Iterator type for @ref Entity instances stored by this class.
        using EntityIterator = EntityContainer::iterator;
        /// @brief Const Iterator type for @ref Entity instances stored by this class.
        using ConstEntityIterator = EntityContainer::const_iterator;
        /// @brief Convenience type for passing around a range of entities.
        using EntityRange = IteratorRange<EntityIterator>;
        /// @brief Convenience type for passing around a const range of entities.
        using ConstEntityRange = IteratorRange<ConstEntityIterator>;

        /// @brief A String containing the name of this manager implementation.
        static const String ImplementationName;
        /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
        static const ManagerBase::ManagerType InterfaceType;

        /// @brief Event name for when an Entity is created.
        /// @remarks Events with this name are of type: EntityManagementEvent.  Event dispatch occurs after the Entity is created.
        static const EventNameType EventEntityCreated;
        /// @brief Event name for when an Entity is destroyed.
        /// @remarks Events with this name are of type: EntityManagementEvent.  Event dispatch occurs before the Entity is destroyed.
        static const EventNameType EventEntityDestroyed;
    protected:
        friend class ActorUpdateWorkUnit;
        friend class AreaEffectUpdateWorkUnit;
        friend class DebrisUpdateWorkUnit;
        friend class TerrainUpdateWorkUnit;
        friend class VehicleUpdateWorkUnit;

        /// @brief A map containing all registered Entity type factories.
        static FactoryContainer EntityFactories;
        /// @brief An event publisher for global entity related events.
        EventPublisher Publisher;
        /// @brief Container storing all Entities belonging to this manager.
        EntityContainer Entities;

        /// @brief The work unit that updates all the actors stored by this manager.
        ActorUpdateWorkUnit* ActorUpdateWork;
        /// @brief The work unit that updates all the actors stored by this manager.
        AreaEffectUpdateWorkUnit* AreaEffectUpdateWork;
        /// @brief The work unit that updates all the Debris stored by this manager.
        DebrisUpdateWorkUnit* DebrisUpdateWork;
        /// @brief The work unit that updates all the terrains stored by this manager.
        TerrainUpdateWorkUnit* TerrainUpdateWork;
        /// @brief The work unit that updates all the vehicles stored by this manager.
        VehicleUpdateWorkUnit* VehicleUpdateWork;
        /// @brief Can be used for thread safe logging and other thread specific resources.
        Threading::DefaultThreadSpecificStorage::Type* ThreadResources;

        /// @brief Adds an Entity to this manager.
        /// @param ToAdd The Entity to be added to this managers storage.
        void AddEntity(Entity* ToAdd);
    public:
        /// @brief Class constructor.
        /// @param Creator The parent world that is creating the manager.
        EntityManager(World* Creator);
        /// @brief XML constructor.
        /// @param Creator The parent world that is creating the manager.
        /// @param XMLNode The node of the xml document to construct from.
        EntityManager(World* Creator, const XML::Node& XMLNode);
        /// @brief Class destructor.
        virtual ~EntityManager();

        ///////////////////////////////////////////////////////////////////////////////
        // Prefab Entity Type Creation

        /// @brief Creates a new FieldOfForce.
        /// @param Name The name to be given to the new FieldOfForce.
        /// @return Returns a pointer to the created AreaEffect.
        FieldOfForce* CreateFieldOfForce(const String& Name);
        /// @brief Creates a new FieldOfForce.
        /// @param Name The name to be given to the new FieldOfForce.
        /// @param AEShape A pointer to the Collision shape that will be applied to the new FieldOfForce.
        /// @return Returns a pointer to the created AreaEffect.
        FieldOfForce* CreateFieldOfForce(const String& Name, Physics::CollisionShape* AEShape);
        /// @brief Creates a new FieldOfForce.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        /// @return Returns a pointer to the created AreaEffect.
        FieldOfForce* CreateFieldOfForce(const XML::Node& SelfRoot);
        /// @brief Creates a new GravityField.
        /// @param Name The name to be given to the new GravityField.
        /// @return Returns a pointer to the created AreaEffect.
        GravityField* CreateGravityField(const String& Name);
        /// @brief Creates a new GravityField.
        /// @param Name The name to be given to the new GravityField.
        /// @param AEShape A pointer to the Collision shape that will be applied to the new GravityField.
        /// @return Returns a pointer to the created AreaEffect.
        GravityField* CreateGravityField(const String& Name, Physics::CollisionShape* AEShape);
        /// @brief Creates a new GravityField.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        /// @return Returns a pointer to the created AreaEffect.
        GravityField* CreateGravityField(const XML::Node& SelfRoot);
        /// @brief Creates a new GravityWell.
        /// @param Name The name to be given to the new GravityWell.
        /// @return Returns a pointer to the created AreaEffect.
        GravityWell* CreateGravityWell(const String& Name);
        /// @brief Creates a new GravityWell.
        /// @param Name The name to be given to the new GravityWell.
        /// @param AEShape A pointer to the Collision shape that will be applied to the new GravityWell.
        /// @return Returns a pointer to the created AreaEffect.
        GravityWell* CreateGravityWell(const String& Name, Physics::CollisionShape* AEShape);
        /// @brief Creates a new GravityWell.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        /// @return Returns a pointer to the created AreaEffect.
        GravityWell* CreateGravityWell(const XML::Node& SelfRoot);

        /// @brief Creates a new RigidDebris.
        /// @param Name The name to be given to the new RigidDebris.
        /// @param Mass The mass of the debris object.
        /// @return Returns a pointer to the created Debris.
        RigidDebris* CreateRigidDebris(const String& Name, const Real Mass);
        /// @brief Creates a new RigidDebris.
        /// @param Name The name to be given to the new RigidDebris.
        /// @param Mass The mass of the debris object.
        /// @param DebMesh A pointer to the Graphics mesh that will be applied to the new RigidDebris.
        /// @param DebShape A pointer to the Collision shape that will be applied to the new RigidDebris.
        /// @return Returns a pointer to the created Debris.
        RigidDebris* CreateRigidDebris(const String& Name, const Real Mass, Graphics::Mesh* DebMesh, Physics::CollisionShape* DebShape);
        /// @brief Creates a new RigidDebris.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        /// @return Returns a pointer to the created Debris.
        RigidDebris* CreateRigidDebris(const XML::Node& SelfRoot);
        /// @brief Creates a new SoftDebris.
        /// @param Name The name to be given to the new SoftDebris.
        /// @param Mass The mass of the debris object.
        /// @return Returns a pointer to the created Debris.
        SoftDebris* CreateSoftDebris(const String& Name, const Real Mass);
        /// @brief Creates a new SoftDebris.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        /// @return Returns a pointer to the created Debris.
        SoftDebris* CreateSoftDebris(const XML::Node& SelfRoot);

        ///////////////////////////////////////////////////////////////////////////////
        // Entity Management

        /// @brief Creates a new Entity.
        /// @param TypeName A string containing the name of the type of Entity to be constructed.
        /// @param InstanceName A string containing the name to be given to the created Entity.
        /// @param Params A container of additional parameters to be used for the construction of the new Entity.
        /// @return Returns a pointer to the created Entity.
        Entity* CreateEntity(const String& TypeName, const String& InstanceName, const NameValuePairMap& Params);
        /// @brief Creates a new Entity class from an XML node.
        /// @remarks This is mostly useful for deserialization.
        /// @return Returns a pointer to the created Entity.
        Entity* CreateEntity(const XML::Node& SelfRoot);

        /// @brief Gets an Entity by Index.
        /// @param Index The index of the Entity you wish to retrieve.
        /// @return Returns a pointer to the Entity at the specified index.
        virtual Entity* GetEntity(const Whole Index) const;
        /// @brief Gets an Entity by Name.
        /// @param Name The name of the Entity you wish to retrieve.
        /// @return Returns a pointer to the Entity of the specified name.
        virtual Entity* GetEntity(const String& Name) const;
        /// @brief Gets the number of Entities stored in this manager.
        /// @return Returns a whole representing the current Entity count.
        virtual Whole GetNumEntities() const;
        /// @brief Destroys an Entity at the specified index.
        /// @param Index The index at which to destroy the Entity.
        virtual void DestroyEntity(const Whole Index);
        /// @brief Destroys an Entity.
        /// @param ToBeDestroyed The Entity to be destroyed.
        virtual void DestroyEntity(Entity* ToBeDestroyed);
        /// @brief Destroys all Entities currently within this manager.
        virtual void DestroyAllEntities();

        #ifndef SWIG
        /// @brief Gets an iterator to the first Entity in this manager.
        /// @return Returns an iterator to the first Entity being stored by this manager.
        EntityIterator BeginEntity();
        /// @brief Gets an iterator to one passed the last Entity in this manager.
        /// @return Returns an iterator to one passed the last Entity being stored by this manager.
        EntityIterator EndEntity();
        /// @brief Gets a const iterator to the first Entity in this manager.
        /// @return Returns a const iterator to the first Entity being stored by this manager.
        ConstEntityIterator BeginEntity() const;
        /// @brief Gets a const iterator to one passed the last Entity in this manager.
        /// @return Returns a const iterator to one passed the last Entity being stored by this manager.
        ConstEntityIterator EndEntity() const;

        /// @brief Gets an EntityRange of all the Actors in this manager.
        /// @return Returns an iterator range containing all the Actors being stored in this manager.
        EntityRange GetActorRange();
        /// @brief Gets an EntityRange of all the AreaEffects in this manager.
        /// @return Returns an iterator range containing all the AreaEffects being stored in this manager.
        EntityRange GetAreaEffectRange();
        /// @brief Gets an EntityRange of all the Debris in this manager.
        /// @return Returns an iterator range containing all the Debris being stored in this manager.
        EntityRange GetDebrisRange();
        /// @brief Gets an EntityRange of all the Terrains in this manager.
        /// @return Returns an iterator range containing all the Terrains being stored in this manager.
        EntityRange GetTerrainRange();
        /// @brief Gets an EntityRange of all the Vehicles in this manager.
        /// @return Returns an iterator range containing all the Vehicles being stored in this manager.
        EntityRange GetVehicleRange();
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        // EntityFactory Management

        /// @brief Adds/registers a Entity factory with this manager, allowing it to be constructed through this API.
        /// @param ToBeAdded The Entity factory to be added.
        static void AddEntityFactory(EntityFactory* ToBeAdded);
        /// @brief Removes a Entity factory from this manager.
        /// @param ToBeRemoved A pointer to the Entity factory that is to be removed.
        static void RemoveEntityFactory(EntityFactory* ToBeRemoved);
        /// @brief Removes a Entity factory from this manager.
        /// @param ImplName The name of the Entity implementation created by the factory to be removed.
        static void RemoveEntityFactory(const String& ImplName);
        /// @brief Removes and destroys a Entity factory in this manager.
        /// @param ToBeDestroyed A pointer to the Entity factory that is to be removed and destroyed.
        static void DestroyEntityFactory(EntityFactory* ToBeDestroyed);
        /// @brief Removes and destroys a Entity factory in this manager.
        /// @param ImplName The name of the Entity implementation created by the factory to be removed and destroyed.
        static void DestroyEntityFactory(const String& ImplName);
        /// @brief Destroys all Entity factories in this manager.
        /// @warning The destruction of Entity factories should only be done after all the Entity have been destroyed, otherwise this will cause an exception.
        static void DestroyAllEntityFactories();
        /// @brief Adds all the default Entity factories provided by the engine.
        static void AddAllDefaultEntityFactories();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @copydoc WorldManager::Pause(const UInt32)
        virtual void Pause(const UInt32 PL);

        /// @brief Does all of the necessary configuration to prepare for the start of the main loop.
        virtual void MainLoopInitialize();
        /// @copydoc WorldManager::Initialize()
        virtual void Initialize();
        /// @copydoc ManagerBase::Deinitialize()
        virtual void Deinitialize();

        /// @brief Gets the EventPublisher responsible for dispatching global Entity events.
        /// @return Returns a reference to the EventPublisher that will dispatch Entity events performed by this manager.
        EventPublisher& GetPublisher();
        /// @brief Gets the EventPublisher responsible for dispatching global Entity events.
        /// @return Returns a const reference to the EventPublisher that will dispatch Entity events performed by this manager.
        const EventPublisher& GetPublisher() const;

        /// @brief Gets the work unit responsible for updating Actors stored by this manager.
        /// @return Returns a pointer to the ActorUpdateWorkUnit used by this manager.
        ActorUpdateWorkUnit* GetActorUpdateWork();
        /// @brief Gets the work unit responsible for updating AreaEffects stored by this manager.
        /// @return Returns a pointer to the AreaEffectUpdateWorkUnit used by this manager.
        AreaEffectUpdateWorkUnit* GetAreaEffectUpdateWork();
        /// @brief Gets the work unit responsible for updating Debris' stored by this manager.
        /// @return Returns a pointer to the ActorUpdateWorkUnit used by this manager.
        DebrisUpdateWorkUnit* GetDebrisUpdateWork();
        /// @brief Gets the work unit responsible for updating Terrains stored by this manager.
        /// @return Returns a pointer to the TerrainUpdateWorkUnit used by this manager.
        TerrainUpdateWorkUnit* GetTerrainUpdateWork();
        /// @brief Gets the work unit responsible for updating Vehicles stored by this manager.
        /// @return Returns a pointer to the VehicleUpdateWorkUnit used by this manager.
        VehicleUpdateWorkUnit* GetVehicleUpdateWork();

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        /// @copydoc ManagerBase::GetInterfaceType()
        virtual ManagerType GetInterfaceType() const;
        /// @copydoc ManagerBase::GetImplementationTypeName()
        virtual String GetImplementationTypeName() const;
    };//EntityManager

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A factory responsible for the creation and destruction of the default EntityManager.
    ///////////////////////////////////////
    class DefaultEntityManagerFactory : public WorldManagerFactory
    {
    public:
        /// @brief Class constructor.
        DefaultEntityManagerFactory() = default;
        /// @brief Class destructor.
        virtual ~DefaultEntityManagerFactory() = default;

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
    };//DefaultEntityManagerFactory
}//Mezzanine

#endif
