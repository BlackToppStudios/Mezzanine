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
#ifndef _physicsphysicsmanager_h
#define _physicsphysicsmanager_h

//forward Declarations so that we do not need #include "btBulletDynamicsCommon.h"
class btAxisSweep3;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btSoftRigidDynamicsWorld;
class btDynamicsWorld;
class btCollisionShape;
class btSoftBodyRigidBodyCollisionConfiguration;
class btGhostPairCallback;
class btBroadphaseInterface;
class btCollisionConfiguration;

typedef float btScalar;

#include <map>
#include <vector>

#include "Physics/constraint.h"
#include "datatypes.h"
#include "managerbase.h"
#include "managerfactory.h"
#include "singleton.h"
#include "objectpair.h"

namespace Mezzanine
{
    // internal forward declarations
    class Entresol;
    class ActorBase;
    class Vector3WActor;
    class AreaEffect;
    class WorldTrigger;
    namespace debug {
        class InternalDebugDrawer;
    }
    namespace Physics
    {
        class Collision;
        class CollisionDispatcher;

        ///////////////////////////////////////////////////////////////////////////////
        /// @class PhysicsConstructionInfo
        /// @headerfile physicsmanager.h
        /// @brief
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB PhysicsConstructionInfo
        {
            public:
                /// @enum PhysicsConstructionFlags
                /// @brief This is an enum used by the physics manager to determine what internal
                /// classes should be used when creating the world.
                enum PhysicsConstructionFlags
                {
                    //World type flags
                    PCF_SoftRigidWorld = 1,

                    //narrowphase flags
                    PCF_LimitlessWorld = 2,

                    //threading flags
                    PCF_Multithreaded = 4
                };

                /// @brief Class constructor.
                PhysicsConstructionInfo();
                /// @brief Class destructor.
                ~PhysicsConstructionInfo();
                /// @brief Assignment Operator.
                /// @param Other The other PhysicsConstructionInfo to be copied.
                /// @return Returns a reference to this.
                PhysicsConstructionInfo& operator=(const PhysicsConstructionInfo& Other);

                /// @brief The flags to initialize the physics system with.
                Whole PhysicsFlags;
                /// @brief The lower limits of the worlds AABB.
                /// @note This member is ignored if the "PCF_LimitlessWorld" flag is passed in.
                Vector3 GeographyLowerBounds;
                /// @brief The upper limits of the worlds AABB.
                /// @note This member is ignored if the "PCF_LimitlessWorld" flag is passed in.
                Vector3 GeographyUpperBounds;
                /// @brief The maximum number of Actors and Area Effects you expect to have in the world at one time.
                /// @note This member is ignored if the "PCF_LimitlessWorld" flag is passed in.
                Whole MaxProxies;
                /// @brief The gravity to set for the world.
                Vector3 Gravity;
        };

        // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
        #ifdef SWIG
        %template(SingletonPhysicsManager) Singleton<PhysicsManager>;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        /// @class PhysicsManager
        /// @headerfile physicsmanager.h
        /// @brief This is simply a place for storing all the Physics Related functions
        /// @details This is a place for storing items related to Debug physics
        /// drawing, Adding constraints, screwing with gravity and doing other physics
        /// Related features.
        ///////////////////////////////////////
        class MEZZ_LIB PhysicsManager : public ManagerBase, public Singleton<PhysicsManager>
        {
            private:
                // needed for collision processing
                friend class CollisionDispatcher;
                //Some Data Items
                PhysicsConstructionInfo WorldConstructionInfo;
                bool SimulationPaused;
                Whole SubstepModifier;
                Real StepSize;
                std::vector< Physics::Constraint* > Constraints;
                std::vector< AreaEffect* > AreaEffects;
                std::vector< WorldTrigger* > Triggers;
                std::map< ObjectPair,Physics::Collision* > Collisions;

                // Some Items bullet requires
                btGhostPairCallback* GhostCallback;
                btBroadphaseInterface* BulletBroadphase;
                btCollisionConfiguration* BulletCollisionConfiguration;
                btCollisionDispatcher* BulletDispatcher;
                btSequentialImpulseConstraintSolver* BulletSolver;
                btSoftRigidDynamicsWorld* BulletDynamicsWorld;
                debug::InternalDebugDrawer* BulletDrawer;

                /// @brief This takes care of all the real work in contructing this
                /// @details This method is called by all the constructors to insure consistent behavior.
                /// @param Info The construction info class with all the settings you wish the world to have.
                void Construct(const PhysicsConstructionInfo& Info);

                /// @brief Calls the ApplyEffects() and UpdateActorList() function of every stored AreaEffect.
                /// @details This function is automatically called every step.
                void ProcessAllEffects();
                /// @brief Calls the ConditionsAreMet() and ApplyTrigger() functions of every stored trigger.
                /// @details This function is automatically called every step.
                void ProcessAllTriggers();
                /// @brief Checks the internal collision data and generates/updates collisions as necessary.
                /// @details This function is automatically called every step.
                void ProcessAllCollisions();

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
                PhysicsManager(const PhysicsConstructionInfo& Info);
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
                void PauseSimulation(bool Pause);
                /// @brief Gets Whether or not the simulation is currently paused.
                /// @return Returns whether or not the simulation is paused.
                bool SimulationIsPaused();

                ///////////////////////////////////////////////////////////////////////////////
                // Gravity Management

                /// @brief Sets the gravity.
                /// @details Sets the strength and direction of gravity within the world.
                /// @param pgrav Vector3 representing the strength and direction of gravity.
                void SetGravity(const Vector3& pgrav);
                /// @brief Gets the gravity.
                /// @details Gets the currently set world gravity.
                /// @return Returns the currently set world gravity.
                Vector3 GetGravity();
                /// @brief Sets the gravity for soft bodies.
                /// @details Gravity for soft bodies is stored separately from rigid bodies.  So if you plan to use soft bodies in your game/simulation
                /// you need to call this function otherwise they won't fall.
                /// @param sgrav Vector3 representing the strength and direction of gravity.
                void SetSoftGravity(const Vector3& sgrav);
                /// @brief Gets the soft body gravity.
                /// @details Gets the currently set soft body world gravity.
                /// @return Returns the currently set soft body world gravity.
                Vector3 GetSoftGravity();
                /// @brief Sets the gravity to be applied to a single object.
                /// @details This function does not change the global gravity, only how gravity behaves for this specific object.  Note: This function only works on ActorRigid's.
                /// @param Actor The actor whos gravity is to be changed.
                /// @param igrav The value of the gravity to be applied.
                void SetIndividualGravity(ActorBase* Actor, const Vector3& igrav);

                ///////////////////////////////////////////////////////////////////////////////
                // Constraint Management

                /// @brief Adds a constraint to the world.
                /// @details Adds the constraint to the world so that it can/will take effect.
                /// @param Con The constraint to be added.
                /// @param DisableCollisions Sets whether or not the linked bodies collide with each other.
                void AddConstraint(Physics::Constraint* Con, bool DisableCollisions = false);
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
                /// @details In practice it is cleaner to remove constraints from the world before removing any constrained actors.
                void DestroyAllConstraints();

                ///////////////////////////////////////////////////////////////////////////////
                // AreaEffect Management

                /// @brief Adds an area effect to the world.
                /// @details Adds an area effect to the world so that it can/will take effect.
                /// @param AE The area effect to be added.
                void AddAreaEffect(AreaEffect* AE);
                /// @brief Gets an Area Effect by name.
                /// @param Name The name of the area effect to find.
                /// @return Returns a pointer to the named area effect, or NULL if it doesn't exist.
                AreaEffect* GetAreaEffect(const String& Name);
                /// @brief Gets an Area Effect by index.
                /// @param Index The index of the area effect you want.
                /// @return Returns a pointer to the area effect at the specified index.
                AreaEffect* GetAreaEffect(const Whole& Index);
                /// @brief Gets the number of Area Effects currently in the world.
                /// @return Returns a whole representing the number of Area Effects in the world.
                Whole GetNumAreaEffects();
                /// @brief Removes an area effect from the world.
                /// @details Removes an area effect from the world so that it will have no effect.
                /// @param AE The area effect to be removed.
                void RemoveAreaEffect(AreaEffect* AE);
                /// @brief Destroys all area effects currently in the manager.
                void DestroyAllAreaEffects();

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

                /// @brief Gets a Collision by object pair.
                /// @param Pair A pair of objects.
                /// @return Returns a pointer to the Collision if a collision for the provided pair exists, NULL otherwise.
                Physics::Collision* GetCollision(ObjectPair* Pair);
                /// @brief Gets the number of Collisions currently in the world.
                /// @return Returns a whole representing the number of Collisions in the world.
                Whole GetNumCollisions();
                /// @brief Removes an existing collision from the world.
                /// @remarks In general it's not a great idea to call on this manually, but there are some situations where you would.
                /// Mostly this function exists to facilitate removal of objects from the world before the simulation ends.
                /// In such cases you have to clean up traces of the collision.
                /// @param Col The collision to be removed.
                void RemoveCollision(Physics::Collision* Col);
                /// @brief Removes all stored collisions that involve the specified Object.
                /// @param Object The Object which will have all of it's collisions removed.
                void RemoveCollisionsContainingObject(WorldObject* Object);
                /// @brief Destroys all collisions currently being stored and processed in the manager.
                void DestroyAllCollisions();

                /// @brief Used to make working with the Collisions easier.
                typedef std::map< ObjectPair,Physics::Collision* >::iterator CollisionIterator;
                /// @brief Used to make working with the Collisions easier, and avoid the risk of accidentally changing them.
                typedef std::map< ObjectPair,Physics::Collision* >::const_iterator ConstCollisionIterator;
                /// @brief Get an CollisionIterator to the first Collision.
                /// @return An CollisionIterator to the first Collision.
                CollisionIterator BeginCollision();
                /// @brief Get a CollisionIterator to one past the last Collision.
                /// @return A CollisionIterator to one past the last Collision.
                CollisionIterator EndCollision();
                /// @brief Get a ConstCollisionIterator to the first Collision.
                /// @return A ConstCollisionIterator to the first Collision.
                ConstCollisionIterator BeginCollision() const;
                /// @brief Get a ConstCollisionIterator to one past the last Collision.
                /// @return A ConstCollisionIterator to one past the last Collision.
                ConstCollisionIterator EndCollision() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Debug Management

                /// @brief Enables and Disables Physics Debug Drawing
                /// @details Enables and Disables Physics Debug Drawing using default wireframes. This will force renderings that match the physics
                /// subsytem pixel for pixel.
                /// @param ToBeEnabled 1 to turn it on, 0 to turn it off. There may be other options in the future, to enable fine tuned control
                void SetDebugPhysicsRendering(int ToBeEnabled);
                /// @brief Is Physics Debug Drawing currently enabled?
                /// @details lets you check if Physics Debug Drawing is enabled or not.
                /// @return 1 for it is on, and 0 for it is not. The may be other options later for selectively cnacking certain features
                int GetDebugPhysicsRendering();
                /// @brief How many Wireframes do you want drawn from previous events
                /// @details Each frame of the action gets its own wire frame, and how many of those back did you want to see? To see a minimal amount
                /// set this to 2, as the first wireframe is commonly entirely inside the  the rendered 3d mesh. You can use Entresol::GetTargetFrameTime()
                /// In conjunction with this to specify an amout of seconds worth of wireframes.
                /// @param WireFrameCount_ This is a whole number that is the amount of wire frames you wan to see. Don't forget to be mindful of the framerate,
                /// Any amount more than just a few seconds worth can be cumbersome.
                void SetDebugPhysicsWireCount(Whole WireFrameCount_);
                /// @brief This gets how many WireFrames are being drawn.
                /// @details This will tell you how many frames worth of previous in game events are being drawn.
                /// @return This returns either 2 or the last amount passed into Entresol::SetDebugPhysicsWireCount .
                Whole GetDebugPhysicsWireCount();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Resets all the internal physics structures in this manager.
                /// @warning This should only be called while the world is emtpy and objects have be unloaded from it.
                /// @param Info If you want to change the configuration of the world when restarting, you can optionally
                /// provide a new set of parameters to build the world with.
                void ResetPhysicsWorld(PhysicsConstructionInfo* Info = 0);
                /// @brief Clears all data relating to actors and other simulation objects from the physics world.
                /// @details This is best used with simulation cleanup.
                void ClearPhysicsMetaData();
                /// @brief Sets the modifier to be used when stepping the physics simulation.
                /// @remarks For the most part, the simulation tick is determined by your target framerate set on Mezzanine::World.  However
                /// there are some occasions when you will want to have it tick more often, in particular with sensative simulation setups
                /// involving many constraints, or small objects, or fast moving objects, or any combination of those.  In order to make your
                /// simulation more stable you have to tick in smaller intervals, making it less likely for the engine to miss something or
                /// become unstable.  When you pass in a modifier it'll ensure it ticks faster by that amount.  For example, if you pass in 2
                /// to this as the modifier, the physics simulation will take 2 smaller steps (the time being based on the target framerate)
                /// instead of one big one during the course of one frame.
                /// @param Modifier The amount of substeps per frame to perform.
                void SetSimulationSubstepModifier(const Whole& Modifier);
                /// @brief This does all the work reuired for the main loop to process physics
                /// @details
                /// @param TimeElapsed This is a real that represents the amount of time we need to simulate
                void DoMainLoopItems(const Real &TimeElapsed);
                /// @internal
                /// @brief This returns a pointer to the bullet physics world. This is for internal use only
                btSoftRigidDynamicsWorld* GetPhysicsWorldPointer();

                /// @brief Does all of the necessary configuration to prepare for a running simulation.
                void MainLoopInitialize();

                ///////////////////////////////////////////////////////////////////////////////
                // Inherited from Managerbase

                /// @copydoc Mezzanine::ManagerBase::Initialize()
                virtual void Initialize();
                /// @copydoc Mezzanine::ManagerBase::DoMainLoopItems()
                virtual void DoMainLoopItems();
                /// @copydoc ManagerBase::GetInterfaceType()
                virtual ManagerType GetInterfaceType() const;
                /// @copydoc ManagerBase::GetImplementationTypeName()
                virtual String GetImplementationTypeName() const;
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
    }
}//Mezzanine

#endif
