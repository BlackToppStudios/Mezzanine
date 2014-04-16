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
#ifndef _physicsphysicsmanager_h
#define _physicsphysicsmanager_h

//forward Declarations so that we do not need #include "btBulletDynamicsCommon.h"
class btAxisSweep3;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btSoftRigidDynamicsWorld;
class btDiscreteDynamicsWorld;
class btDynamicsWorld;
class btCollisionShape;
class btSoftBodyRigidBodyCollisionConfiguration;
class btGhostPairCallback;
class btBroadphaseInterface;
class btCollisionConfiguration;
class btThreadSupportInterface;

typedef float btScalar;

#include "datatypes.h"
#ifndef SWIG
    #include "worldmanager.h"
    #include "managerfactory.h"
    #include "singleton.h"
#endif
#ifndef SWIG
    #include "Physics/collidablepair.h"
    #include "Physics/constraint.h"
    #include "Physics/managerconstructioninfo.h"
#endif
#ifndef SWIG
    #include "Threading/workunit.h"
    #include "Threading/monopoly.h"
#endif
namespace Mezzanine
{
    // internal forward declarations
    class Entresol;
    class WorldTrigger;
    namespace debug {
        class InternalDebugDrawer;
    }
    namespace Physics
    {
        class Collision;
        class CollisionDispatcher;
        class ParallelCollisionDispatcher;
        class PhysicsManager;
        class CollidableProxy;
        class CollisionShape;
        class GhostProxy;
        class RigidProxy;
        class SoftProxy;

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a Mezzanine::Threading::iWorkUnit for the single threaded processing of physics simulations.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB SimulationWorkUnit : public Threading::DefaultWorkUnit
        {
        protected:
            /// @internal
            /// @brief A pointer to the manager this work unit is processing.
            PhysicsManager* TargetManager;
        private:
            /// @internal
            /// @brief Protected copy constructor.  THIS IS NOT ALLOWED.
            /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
            SimulationWorkUnit(const SimulationWorkUnit& Other);
            /// @internal
            /// @brief Protected assignment operator.  THIS IS NOT ALLOWED.
            /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
            SimulationWorkUnit& operator=(const SimulationWorkUnit& Other);
        public:
            /// @brief Class constructor.
            /// @param Target The PhysicsManager this work unit will process during the frame.
            SimulationWorkUnit(PhysicsManager* Target);
            /// @brief Class destructor.
            virtual ~SimulationWorkUnit();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief This does any required update of the Graphical Scene graph and REnders one frame
            /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
            virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
        };//SimulationWorkUnit

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a Mezzanine::Threading::iWorkUnit for the multi-threaded processing of physics simulations.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB SimulationMonopolyWorkUnit : public Threading::MonopolyWorkUnit
        {
        protected:
            /// @internal
            /// @brief A pointer to the manager this work unit is processing.
            PhysicsManager* TargetManager;
        private:
            /// @internal
            /// @brief Private copy constructor.  THIS IS NOT ALLOWED.
            /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
            SimulationMonopolyWorkUnit(const SimulationMonopolyWorkUnit& Other);
            /// @internal
            /// @brief Protected assignment operator.  THIS IS NOT ALLOWED.
            /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
            SimulationMonopolyWorkUnit& operator=(const SimulationMonopolyWorkUnit& Other);
        public:
            /// @brief Class constructor.
            /// @param Target The PhysicsManager this work unit will process during the frame.
            SimulationMonopolyWorkUnit(PhysicsManager* Target);
            /// @brief Class destructor.
            virtual ~SimulationMonopolyWorkUnit();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief A no-op but declared for compatibility with Monopoly work unit
            /// @param AmountToUse The number of threads permitted for use, if this were implemented
            virtual void UseThreads(const Whole& AmountToUse);
            /// @brief Gets the number of threads this work unit will attempt to use during it's monopoly.
            /// @return Returns a Whole representing the number of threads that will be attempted to be created during running of this monopoly.
            virtual Whole UsingThreadCount();
            /// @brief This does any required update of the Graphical Scene graph and REnders one frame
            /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
            virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
        };//SimulationMonopolyWorkUnit

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a Mezzanine::Threading::iWorkUnit for the updating of WorldTriggers.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB WorldTriggerUpdateWorkUnit : public Threading::DefaultWorkUnit
        {
        protected:
            /// @internal
            /// @brief A pointer to the manager this work unit is processing.
            PhysicsManager* TargetManager;
            /// @internal
            /// @brief Protected copy constructor.  THIS IS NOT ALLOWED.
            /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
            WorldTriggerUpdateWorkUnit(const WorldTriggerUpdateWorkUnit& Other);
            /// @internal
            /// @brief Protected assignment operator.  THIS IS NOT ALLOWED.
            /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
            WorldTriggerUpdateWorkUnit& operator=(const WorldTriggerUpdateWorkUnit& Other);
        public:
            /// @brief Class constructor.
            /// @param Target The PhysicsManager this work unit will process during the frame.
            WorldTriggerUpdateWorkUnit(PhysicsManager* Target);
            /// @brief Class destructor.
            virtual ~WorldTriggerUpdateWorkUnit();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief This does any required update of the Graphical Scene graph and REnders one frame
            /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
            virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
        };//WorldTriggerUpdateWorkUnit

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a Mezzanine::Threading::iWorkUnit for the updating of the physics debug drawer.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB DebugDrawWorkUnit : public Threading::DefaultWorkUnit
        {
        protected:
            /// @internal
            /// @brief A pointer to the manager this work unit is processing.
            PhysicsManager* TargetManager;
            /// @internal
            /// @brief Protected copy constructor.  THIS IS NOT ALLOWED.
            /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
            DebugDrawWorkUnit(const DebugDrawWorkUnit& Other);
            /// @internal
            /// @brief Protected assignment operator.  THIS IS NOT ALLOWED.
            /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
            DebugDrawWorkUnit& operator=(const DebugDrawWorkUnit& Other);
        public:
            /// @brief Class constructor.
            /// @param Target The PhysicsManager this work unit will process during the frame.
            DebugDrawWorkUnit(PhysicsManager* Target);
            /// @brief Class destructor.
            virtual ~DebugDrawWorkUnit();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief This does any required update of the Graphical Scene graph and REnders one frame
            /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
            virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
        };//DebugDrawWorkUnit

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is simply a place for storing all the Physics Related functions
        /// @details This is a place for storing items related to Debug physics
        /// drawing, Adding constraints, screwing with gravity and doing other physics
        /// Related features.
        ///////////////////////////////////////
        class MEZZ_LIB PhysicsManager : public WorldManager
        {
        public:
            /// @brief Basic container type for @ref CollidableProxy storage by this class.
            typedef std::vector< CollidableProxy* >             ProxyContainer;
            /// @brief Iterator type for @ref CollidableProxy instances stored by this class.
            typedef ProxyContainer::iterator                    ProxyIterator;
            /// @brief Const Iterator type for @ref CollidableProxy instances stored by this class.
            typedef ProxyContainer::const_iterator              ConstProxyIterator;
            /// @brief Basic container type for @ref Constraint storage by this class.
            typedef std::vector< Constraint* >                  ConstraintContainer;
            /// @brief Iterator type for @ref Constraint instances stored by this class.
            typedef ConstraintContainer::iterator               ConstraintIterator;
            /// @brief Const Iterator type for @ref Constraint instances stored by this class.
            typedef ConstraintContainer::const_iterator         ConstConstraintIterator;
            /// @brief Basic container type for @ref WorldTrigger storage by this class.
            typedef std::vector< WorldTrigger* >                WorldTriggerContainer;
            /// @brief Iterator type for @ref WorldTrigger instances stored by this class.
            typedef WorldTriggerContainer::iterator             WorldTriggerIterator;
            /// @brief Const Iterator type for @ref WorldTrigger instances stored by this class.
            typedef WorldTriggerContainer::const_iterator       ConstWorldTriggerIterator;
            /// @brief Basic container type for @ref Collision storage by this class.
            typedef std::map< CollidablePair, Collision* >      CollisionContainer;
            /// @brief Iterator type for @ref Collision instances stored by this class.
            typedef CollisionContainer::iterator                CollisionIterator;
            /// @brief Const Iterator type for @ref Collision instances stored by this class.
            typedef CollisionContainer::const_iterator          ConstCollisionIterator;
            /// @brief A std::pair to assist with collision sorting operations.
            typedef std::pair< CollidablePair, Collision* >     CollisionSortPair;
        protected:
            friend class CollisionDispatcher;
            friend class ParallelCollisionDispatcher;
            friend class SimulationWorkUnit;
            friend class SimulationMonopolyWorkUnit;
            friend class WorldTriggerUpdateWorkUnit;
            friend class DebugDrawWorkUnit;

            //Some Data Items
            Boole SimulationPaused;
            Integer DebugRenderMode;
            Whole SubstepModifier;
            Whole ThreadCount;
            Real StepSize;
            Real TimeMultiplier; ///< A Multiplier that adjusts how fast physics runs relative to clock time.

            ManagerConstructionInfo WorldConstructionInfo;

            ProxyContainer Proxies;
            ConstraintContainer Constraints;
            WorldTriggerContainer Triggers;
            CollisionContainer Collisions;

            // Some Items bullet requires
            btGhostPairCallback* GhostCallback;
            btThreadSupportInterface* BulletSolverThreads;
            btThreadSupportInterface* BulletDispatcherThreads;
            btBroadphaseInterface* BulletBroadphase;
            btCollisionConfiguration* BulletCollisionConfiguration;
            btCollisionDispatcher* BulletDispatcher;
            btSequentialImpulseConstraintSolver* BulletSolver;
            btSoftRigidDynamicsWorld* BulletDynamicsWorld;
            debug::InternalDebugDrawer* BulletDrawer;

            /// @internal
            /// @brief The work unit that does the stepping of the simulation.
            Threading::DefaultWorkUnit* SimulationWork;
            /// @internal
            /// @brief The work unit that processes all world triggers.
            WorldTriggerUpdateWorkUnit* WorldTriggerUpdateWork;
            /// @internal
            /// @brief The work unit that updates the debug drawer with the latest physics rendering.
            DebugDrawWorkUnit* DebugDrawWork;

            /// @brief This takes care of all the real work in contructing this
            /// @details This method is called by all the constructors to insure consistent behavior.
            /// @param Info The construction info class with all the settings you wish the world to have.
            virtual void Construct(const ManagerConstructionInfo& Info);
            /// @brief Tear down this physics world
            virtual void Destroy();

            /// @brief Calls the ConditionsAreMet() and ApplyTrigger() functions of every stored trigger.
            /// @details This function is automatically called every step.
            virtual void ProcessAllTriggers();
            /// @brief Checks the internal collision data and generates/updates collisions as necessary.
            /// @details This function is automatically called every step.
            virtual void ProcessAllCollisions();

            /// @brief The World that will be used for the InternalTickCallback
            /// @warning The prevents two PhysicsManagers from running in two differen
            static PhysicsManager* CallBackWorld;
            /// @brief Internal Callback that is called each substep of the simulation.
            static void InternalTickCallback(btDynamicsWorld* world, btScalar timeStep);
        public:
            /// @brief Simple Constructor
            /// @details This constructor will assign some sane default values and will create a physics
            /// world that can be used immediately
            PhysicsManager();
            /// @brief Simple Constructor
            /// @details This constructor will assign some sane default values and will create a physics
            /// world that can be used immediately
            /// @param Info The construction info class with all the settings you wish the world to have.
            PhysicsManager(const ManagerConstructionInfo& Info);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            PhysicsManager(XML::Node& XMLNode);
            /// @brief Deconstructor
            /// @details This deletes all those crazy pointers that Bullet, the physics subsystem need.
            virtual ~PhysicsManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Simulation Management

            /// @brief Pauses the simulation, preventing the physics world from taking action.
            /// @param Pause Wether or not to pause the simulation.
            void PauseSimulation(Boole Pause);
            /// @brief Gets Whether or not the simulation is currently paused.
            /// @return Returns whether or not the simulation is paused.
            Boole SimulationIsPaused();

            /// @brief How much faster or slower that reality is the physic ssystem
            /// @return 1 indicates normal time, less indicates slower and higher indicates faster times
            Real GetTimeMultiplier() const;
            /// @brief Change how fast the physicsworks relatve to well time
            /// @param value 2.0 to double simulation, .5 to half it.
            void SetTimeMultiplier(const Real &value);

            ///////////////////////////////////////////////////////////////////////////////
            // Gravity Management

            /// @brief Sets the gravity.
            /// @details Sets the strength and direction of gravity within the world.
            /// @param pgrav Vector3 representing the strength and direction of gravity.
            void SetWorldGravity(const Vector3& pgrav);
            /// @brief Gets the gravity.
            /// @details Gets the currently set world gravity.
            /// @return Returns the currently set world gravity.
            Vector3 GetWorldGravity();
            /// @brief Sets the gravity for soft bodies.
            /// @details Gravity for soft bodies is stored separately from rigid bodies.  So if you plan to use soft bodies in your game/simulation
            /// you need to call this function otherwise they won't fall.
            /// @param sgrav Vector3 representing the strength and direction of gravity.
            void SetWorldSoftGravity(const Vector3& sgrav);
            /// @brief Gets the soft body gravity.
            /// @details Gets the currently set soft body world gravity.
            /// @return Returns the currently set soft body world gravity.
            Vector3 GetWorldSoftGravity();

            ///////////////////////////////////////////////////////////////////////////////
            // Creating Proxies

            /// @brief Creates a new GhostProxy.
            /// @return Returns a pointer to the created proxy.
            GhostProxy* CreateGhostProxy();
            /// @brief Creates a new GhostProxy.
            /// @param Shape A pointer to the collision shape that will be applied to the new proxy.
            /// @param AddToWorld Wether or not the new proxy should be added to the world after it has been created.
            /// @return Returns a pointer to the created proxy.
            GhostProxy* CreateGhostProxy(CollisionShape* Shape, const Boole AddToWorld = true);
            /// @brief Creates a new GhostProxy.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            /// @return Returns a pointer to the created proxy.
            GhostProxy* CreateGhostProxy(const XML::Node& SelfRoot);
            /// @brief Creates a new RigidProxy.
            /// @param Mass The mass of the new proxy.
            /// @return Returns a pointer to the created proxy.
            RigidProxy* CreateRigidProxy(const Real Mass);
            /// @brief Creates a new RigidProxy.
            /// @param Mass The mass of the new proxy.
            /// @param AddToWorld Wether or not the new proxy should be added to the world after it has been created.
            /// @param Shape A pointer to the collision shape that will be applied to the new proxy.
            /// @return Returns a pointer to the created proxy.
            RigidProxy* CreateRigidProxy(const Real Mass, CollisionShape* Shape, const Boole AddToWorld = true);
            /// @brief Creates a new RigidProxy.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            /// @return Returns a pointer to the created proxy.
            RigidProxy* CreateRigidProxy(const XML::Node& SelfRoot);
            /// @brief Creates a new SoftProxy.
            /// @param Mass The total mass of the new proxy.
            /// @return Returns a pointer to the created proxy.
            SoftProxy* CreateSoftProxy(const Real Mass);
            /// @brief Creates a new SoftProxy.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            /// @return Returns a pointer to the created proxy.
            SoftProxy* CreateSoftProxy(const XML::Node& SelfRoot);

            ///////////////////////////////////////////////////////////////////////////////
            // Proxy Management

            /// @brief Gets a CollidableProxy instance by index.
            /// @param Index The index of the CollidableProxy to be retrieved.
            /// @return Returns a pointer to the CollidableProxy at the specified index.
            CollidableProxy* GetProxy(const UInt32 Index) const;
            /// @brief Gets the number of CollidableProxy instances in this manager.
            /// @return Returns a UInt32 representing the number of CollidableProxy instances contained in this manager.
            UInt32 GetNumProxies() const;
            /// @brief Deletes a CollidableProxy.
            /// @param ToBeDestroyed A pointer to the CollidableProxy you want deleted.
            void DestroyProxy(CollidableProxy* ToBeDestroyed);
            /// @brief Deletes all stored CollidableProxy instances.
            void DestroyAllProxies();

            #ifndef SWIG
            /// @brief Gets an iterator to the first Collidable Proxy in this manager.
            /// @return Returns an iterator to the first Collidable Proxy being stored by this manager.
            ProxyIterator BeginCollidableProxy();
            /// @brief Gets an iterator to one passed the last Collidable Proxy in this manager.
            /// @return Returns an iterator to one passed the last Collidable Proxy being stored by this manager.
            ProxyIterator EndCollidableProxy();
            /// @brief Gets a const iterator to the first Collidable Proxy in this manager.
            /// @return Returns a const iterator to the first Collidable Proxy being stored by this manager.
            ConstProxyIterator BeginCollidableProxy() const;
            /// @brief Gets a const iterator to one passed the last Collidable Proxy in this manager.
            /// @return Returns a const iterator to one passed the last Collidable Proxy being stored by this manager.
            ConstProxyIterator EndCollidableProxy() const;
            #endif

            ///////////////////////////////////////////////////////////////////////////////
            // Constraint Management

            /// @brief Adds a constraint to the world.
            /// @details Adds the constraint to the world so that it can/will take effect.
            /// @param Con The constraint to be added.
            /// @param DisableCollisions Sets whether or not the linked bodies collide with each other.
            void AddConstraint(Physics::Constraint* Con, Boole DisableCollisions = false);
            /// @brief Gets a constraint by index.
            /// @param Index The index of the constraint you want.
            /// @return Returns a pointer to the specified constraint.
            Physics::Constraint* GetConstraint(const Whole& Index);
            /// @brief Gets the number of constraints currently in the world.
            /// @return Returns a whole representing the number of constraints in the world.
            Whole GetNumConstraints();
            /// @brief Removes a constraint from the world.
            /// @details Removes a constraint from the world so that it will have no effect.
            /// @param Con The constraint to be removed.
            void RemoveConstraint(Physics::Constraint* Con);
            /// @brief Destroys all constraints currently in the manager.
            /// @details In phashedstringractice it is cleaner to remove constraints from the world before removing any constrained actors.
            void DestroyAllConstraints();

            ///////////////////////////////////////////////////////////////////////////////
            // Trigger Management

            /// @brief Adds a trigger to the world.
            /// @details Adds a trigger to the world so that it can/will take effect.
            /// @param Trig The trigger to be added.
            void AddWorldTrigger(WorldTrigger* Trig);
            /// @brief Gets a trigger by name.
            /// @param Name The name of the trigger to find.
            /// @return Returns a pointer to the named trigger, or NULL if it doesn't exist.
            WorldTrigger* GetWorldTrigger(const String& Name);
            /// @brief Gets a trigger by index.
            /// @param Index The index of the trigger you want.
            /// @return Returns a pointer to the trigger at the specified index.
            WorldTrigger* GetWorldTrigger(const Whole& Index);
            /// @brief Gets the number of triggers currently in the world.
            /// @return Returns a whole representing the number of triggers in the world.
            Whole GetNumWorldTriggers();
            /// @brief Removes a trigger from the world.
            /// @details Removes a trigger from the world so that it will have no effect.
            /// @param Trig The trigger to be removed.
            void RemoveWorldTrigger(WorldTrigger* Trig);
            /// @brief Destroys all triggers currently in the manager.
            void DestroyAllWorldTriggers();

            ///////////////////////////////////////////////////////////////////////////////
            // Collision Management

            /// @brief Gets a Collision by collidable pair.
            /// @param Pair A pair of CollidableProxies.
            /// @return Returns a pointer to the Collision if a collision for the provided pair exists, NULL otherwise.
            Physics::Collision* GetCollision(CollidablePair* Pair);
            /// @brief Gets the number of Collisions currently in the world.
            /// @return Returns a whole representing the number of Collisions in the world.
            Whole GetNumCollisions();
            /// @brief Removes an existing collision from the world.
            /// @remarks In general it's not a great idea to call on this manually, but there are some situations where you would.
            /// Mostly this function exists to facilitate removal of objects from the world before the simulation ends.
            /// In such cases you have to clean up traces of the collision.
            /// @param Col The collision to be removed.
            void RemoveCollision(Physics::Collision* Col);
            /// @brief Removes all stored collisions that involve the specified CollidableProxy.
            /// @param Proxy The Proxy which will have all of it's collisions removed.
            void RemoveCollisionsContainingProxy(CollidableProxy* Proxy);
            /// @brief Destroys all collisions currently being stored and processed in the manager.
            void DestroyAllCollisions();

            /// @brief Get an CollisionIterator to the first Collision.
            /// @return An CollisionIterator to the first Collision.
            CollisionIterator BeginCollision();
            /// @brief Get a CollisionIterator to one past the last Collision.
            /// @return A CollisionIterator to one past the last Collision.
            CollisionIterator EndCollision();
            #if !(defined(SWIG) && defined(MEZZLUA51)) // Stop Swig from making lua bindings but allow other languages
            /// @brief Get a ConstCollisionIterator to the first Collision.
            /// @return A ConstCollisionIterator to the first Collision.
            ConstCollisionIterator BeginCollision() const;
            /// @brief Get a ConstCollisionIterator to one past the last Collision.
            /// @return A ConstCollisionIterator to one past the last Collision.
            ConstCollisionIterator EndCollision() const;
            #endif

            ///////////////////////////////////////////////////////////////////////////////
            // Debug Management

            /// @brief Enables and Disables Physics Debug Drawing
            /// @details Enables and Disables Physics Debug Drawing using default wireframes. This will force renderings that match the physics
            /// subsytem pixel for pixel.
            /// @param DebugRenderingMode 1 to turn it on, 0 to turn it off. There may be other options in the future, to enable fine tuned control
            void SetDebugRenderingMode(const Integer DebugRenderingMode);
            /// @brief Is Physics Debug Drawing currently enabled?
            /// @details lets you check if Physics Debug Drawing is enabled or not.
            /// @return 1 for it is on, and 0 for it is not. The may be other options later for selectively cnacking certain features
            Integer GetDebugRenderingMode() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Resets all the internal physics structures in this manager.
            /// @warning This should only be called while the world is emtpy and objects have be unloaded from it.
            /// @param Info If you want to change the configuration of the world when restarting, you can optionally
            /// provide a new set of parameters to build the world with.
            void ResetPhysicsWorld(ManagerConstructionInfo* Info = 0);
            /// @brief Clears all data relating to actors and other simulation objects from the physics world.
            /// @details This is best used with simulation cleanup.
            void ClearPhysicsMetaData();
            /// @brief Sets the modifier to be used when stepping the physics simulation.
            /// @remarks For the most part, the simulation tick is determined by your target framerate set on Mezzanine::World.  However
            /// there are some occasions when you will want to have it tick more often, in particular with sensative simulation setups
            /// involving many constraints, or small objects, or fast moving objects, or any combination of those.  In order to make your
            /// simulation more stable you have to tick in smaller intervals, making it less likely for the engine to miss something or
            /// become unstable. When you pass in a modifier it'll ensure it ticks faster by that amount.  For example, if you pass in 2
            /// to this as the modifier, the physics simulation will take 2 smaller steps (the time being based on the target framerate)
            /// instead of one big one during the course of one frame.
            /// @param Modifier The amount of substeps per frame to perform.
            void SetSimulationSubstepModifier(const Whole& Modifier);

            /// @copydoc WorldManager::Pause(const UInt32)
            virtual void Pause(const UInt32 PL);

            /// @brief Does all of the necessary configuration to prepare for a running simulation.
            void MainLoopInitialize();
            /// @copydoc WorldManager::Initialize()
            virtual void Initialize();
            /// @copydoc ManagerBase::Deinitialize()
            virtual void Deinitialize();

            /// @brief The work that needs to be done each frame.
            /// @param CurrentThreadStorage The context of the current frame for the work to do.
            void DoPerFrameWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);

            /// @brief Gets a pointer to the work unit that steps the simulation.
            /// @return Returns a pointer to the DefaultWorkUnit that steps the simulation.
            Threading::DefaultWorkUnit* GetSimulationWork();
            /// @brief Gets a pointer to the work unit that updates all WorldTriggers.
            /// @return Returns a pointer to the WorldTriggerUpdateWorkUnit used by this manager.
            WorldTriggerUpdateWorkUnit* GetWorldTriggerUpdateWork();
            /// @brief Gets a pointer to the work unit that updates the debug drawer.
            /// @return Returns a pointer to the DebugDrawWorkUnit used by this manager.
            DebugDrawWorkUnit* GetDebugDrawWork();

            ///////////////////////////////////////////////////////////////////////////////
            // Type Identifier Methods

            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerBase::ManagerType GetInterfaceType() const;
            /// @copydoc ManagerBase::GetImplementationTypeName()
            virtual String GetImplementationTypeName() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            #if !(defined(SWIG) && defined(MEZZLUA51)) // Stop Swig from making lua bindings but allow other languages
            /// @internal
            /// @brief This returns a pointer to the bullet physics world. This is for internal use only
            btSoftRigidDynamicsWorld* _GetPhysicsWorldPointer();
            /// @internal
            /// @brief This returns a pointer to the bullet physics world. This is for internal use only
            const btSoftRigidDynamicsWorld* _GetPhysicsWorldPointer() const;
            #endif
        };//PhysicsManager

        ///////////////////////////////////////////////////////////////////////////////
        /// @class DefaultPhysicsManagerFactory
        /// @headerfile physicsmanager.h
        /// @brief A factory responsible for the creation and destruction of the default physicsmanager.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultPhysicsManagerFactory : public ManagerFactory
        {
        public:
            /// @brief Class constructor.
            DefaultPhysicsManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultPhysicsManagerFactory();

            /// @copydoc ManagerFactory::GetManagerTypeName()
            String GetManagerTypeName() const;
            /// @copydoc ManagerFactory::CreateManager(NameValuePairList&)
            ManagerBase* CreateManager(NameValuePairList& Params);

            /// @copydoc ManagerFactory::CreateManager(XML::Node&)
            ManagerBase* CreateManager(XML::Node& XMLNode);

            /// @copydoc ManagerFactory::DestroyManager(ManagerBase*)
            void DestroyManager(ManagerBase* ToBeDestroyed);
        };//DefaultPhysicsManagerFactory
    }//Physics
}//Mezzanine

#endif
