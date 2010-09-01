//© Copyright 2010 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _physicsmanager_h
#define _physicsmanager_h

//forward Declarations so that we do not need #include "btBulletDynamicsCommon.h"
class btAxisSweep3;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btSoftRigidDynamicsWorld;
class btDynamicsWorld;
class btCollisionShape;
class btSoftBodyRigidBodyCollisionConfiguration;

#include <map>

#include "managerbase.h"
#include "constraint.h"

namespace phys
{
    // internal forward declarations
    class World;
    class ActorBase;
    class Vector3WActor;
    namespace debug {
        class InternalDebugDrawer;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /// @class PhysicsManager
    /// @headerfile physicsmanager.h
    /// @brief This is simply a place for storing all the Physics Related functions
    /// @details This is a plage for storing items related to Debug physics
    /// drawing, Adding constraints, screwing with gravity and doing other physics
    /// Related features.
    class PhysicsManager : public ManagerBase
    {
        private:
            friend class World;
            //Some Data Items
            Vector3 GeographyLowerBounds;
            Vector3 GeographyUpperBounds;
            unsigned short int  MaxPhysicsProxies;
            //Real PhysicsStepsize; // use this->GameWorld->TargetFrameLength instead
            unsigned short int CollisionAge;
            Real Impulse;
            std::map< String, btCollisionShape* > PhysicsShapes;

            // Some Items bullet requires
            btAxisSweep3* BulletBroadphase;
            //btDefaultCollisionConfiguration* BulletCollisionConfiguration;
            btSoftBodyRigidBodyCollisionConfiguration* BulletCollisionConfiguration;
            btCollisionDispatcher* BulletDispatcher;
            btSequentialImpulseConstraintSolver* BulletSolver;
            btSoftRigidDynamicsWorld* BulletDynamicsWorld;
            debug::InternalDebugDrawer* BulletDrawer;

            /// @brief This takes care of all the real work in contructing this
            /// @details This method is called by all the constructors to insure consistent behavior.
            /// @param GameWorld_ This is a pointer to the phys::World that this manager will work with
            /// @param GeographyLowerBounds_ This Vector3 will loosely represent the lower right conrer of the world
            /// @param GeographyUpperbounds_ This Vector3 will loosely represent the upper left conrer of the world
            /// @param MaxPhysicsProxies_ This approximates the maximum amount of items allowed in the physics world
            void Construct(World* GameWorld_, const Vector3 &GeographyLowerBounds_, const Vector3 &GeographyUpperbounds_, const unsigned short int &MaxPhysicsProxies_);
        public:

            /// @brief Simple Constructor
            /// @details This constructor will assign some sane default values and will create a physics
            /// world that can be used immediately
            /// @param GameWorld_ This is a pointer to the game world this PhysicsManager will work with
            PhysicsManager(World* GameWorld_);

            /// @brief Simple Constructor
            /// @details This constructor will assign some sane default values and will create a physics
            /// world that can be used immediately
            /// @param GameWorld_ This is a pointer to the phys::World that this manager will work with
            /// @param GeographyLowerBounds_ This Vector3 will loosely represent the lower right conrer of the world
            /// @param GeographyUpperbounds_ This Vector3 will loosely represent the upper left conrer of the world
            /// @param MaxPhysicsProxies_ This approximates the maximum amount of items allowed in the physics world
            PhysicsManager(World* GameWorld_, const Vector3 &GeographyLowerBounds_, const Vector3 &GeographyUpperbounds_, const unsigned short int &MaxPhysicsProxies_);

            /// @brief This configures the Physics Manager to work with the Graphic settings
            /// @details This configures the Physics manager to work with the existing graphics settings. This must
            /// be called before the physics manager is used, but after the graphics have been initialized
            virtual void Initialize();

            /// @brief Physics stepping during the main loop
            /// @details This increments the the physics world the required amount to keep it in sync with the Graphics/Timing.
            virtual void DoMainLoopItems();

            /// @brief Deconstructor
            /// @details This deletes all those crazy pointers that Bullet, the physics subsystem need.
            virtual ~PhysicsManager();

            /// @brief Sets the gravity.
            /// @details Sets the strength and direction of gravity within the world.
            /// @param pgrav Vector3 representing the strength and direction of gravity.
            void SetGravity(Vector3 pgrav);

            /// @brief Sets the gravity for soft bodies.
            /// @details Gravity for soft bodies is stored separately from rigid bodies.  So if you plan to use soft bodies in your game/simulation
            /// you need to call this function otherwise they won't fall.
            /// @param sgrav Vector3 representing the strength and direction of gravity.
            void SetSoftGravity(Vector3 sgrav);

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
            /// set this to 2, as the first wireframe is commonly entirely inside the  the rendered 3d mesh. You can use World::GetTargetFrameTime()
            /// In conjunction with this to specify an amout of seconds worth of wireframes.
            /// @param WireFrameCount_ This is a whole number that is the amount of wire frames you wan to see. Don't forget to be mindful of the framerate,
            /// Any amount more than just a few seconds worth can be cumbersome.
            void SetDebugPhysicsWireCount(Whole WireFrameCount_);

            /// @brief This gets how many WireFrames are being drawn.
            /// @details This will tell you how many frames worth of previous in game events are being drawn.
            /// @return This returns either 2 or the last amount passed into World::SetDebugPhysicsWireCount .
            Whole GetDebugPhysicsWireCount();

            /// @brief This does all the work reuired for the main loop to process physics
            /// @details
            /// @param TimeElapsed This is a real that represents the amount of time we need to simulate
            void DoMainLoopItems(const Real &TimeElapsed);

            /// @brief Gets the offset between a point and an actor.
            /// @details This function will return the offset between the specified world point and the specified actors center of mass.
            /// @param OffsetInfo The vector and actor to compare for offset data.
            /// @return Returns the Vector3 representing the actors offset from the world point.
            Vector3 GetActorOffset(const Vector3WActor &OffsetInfo);

            /// @brief Adds a constraint to the world.
            /// @details Adds the constraint to the world so that it can/will take effect.
            /// @param Constraint The constraint to be added.
            void AddConstraint(TypedConstraint* Constraint);

            /// @brief Removes a constraint from the world.
            /// @details Removes a constraint from the world so that it will have no effect.
            /// @param Constraint The constraint to be removed.
            void RemoveConstraint(TypedConstraint* Constraint);

            /// @brief Stores an already generated physics shape.
            /// @details This function will take an actors physics shape and store it for re-use in other actors, in case a shape is able to be re-used by other actors.
            /// @param Actor The actor from which to store the shape.
            /// @param ShapeName The name you wish to assign to the shape being stored.
            void StorePhysicsShape(ActorBase* Actor, String &ShapeName);

            /// @brief Applies a previously stored shape.
            /// @details This function will take a stored shape and apply it to the actor provided.
            /// @param Actor The actor to which you want to apply the shape.
            /// @param ShapeName The name of the shape you wish to have applied to the actor.
            void ApplyPhysicsShape(ActorBase* Actor, String &ShapeName);

            /// @brief Sets the Collision Parameters.
            /// @details Sets the Collision Age and Force Filters used in filtering out collision contacts used to make events.  The lower these numbers, the more events will be generated.  @n
            /// These numbers both default to 1.
            /// @param Age The number of physics ticks the collision has to have existed to be used.  Usually you want 1 or 2.  Default: 1
            /// @param Force The amount of force applied in the collision to filter by.  This amount can vary more then the other param based on what you need.  Default: 1.0
            void SetCollisionParams(const unsigned short int Age, Real Force);

            /// @brief Gets the Collision Age limit.
            /// @details Gets the CollisionAge used in filtering out collision contacts used to make events.
            /// @return This function will return the number of physics ticks the collision has to have existed to be used.
            unsigned short int GetCollisionAge();

            /// @brief Gets the Collision Impulse limit.
            /// @details Gets the Collision Impulse used in filtering out collision contacts used to make events.
            /// @return This function will return the lower limit of the allowed force of the collision to generate an event.
            Real GetImpulse();

            /// @internal
            /// @brief This returns a pointer to the bullet physics world. This is for internal use only
            btSoftRigidDynamicsWorld* GetPhysicsWorldPointer();

        //Inherited from ManagerBase
            /// @brief This returns the type of this manager.
            /// @return This returns ManagerTypeName::PhysicsManager
            virtual ManagerTypeName GetType() const;
    };
}

#endif
