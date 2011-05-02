//� Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _physactorrigid_h
#define _physactorrigid_h

#include "actorbase.h"
#include "actorphysicssettings.h"
#include "crossplatformexport.h"

///////////////////////////////////
// Forward Declares
class btRigidBody;

///////////////////////////////////
// Actual code
namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorRigid
    /// @headerfile actorbase.h
    /// @brief This is the actor class for Rigid Objects.
    /// @details This class should be used to make any rigid object that can be moved as a
    /// result of force.  Most objects will fall into this catagory.  A few examples of a
    /// Rigid Object: Boxes, Car Frames, Chairs, etc.  For Semi Rigid bodies that are
    /// deformable, like jello, it is better to use ActorSoft.
    ///////////////////////////////////////
    class PHYS_LIB ActorRigid : public ActorBase {
        protected:
            friend class TypedConstraint;
            /// @brief Used to simulate the behavior of a btRigidBody
            btRigidBody* physrigidbody;

            /// @brief This class encapsulates physics specific configuration for this actor.
            ActorRigidPhysicsSettings* PhysicsSettings;

            /// @brief Creates a rigid object for the actor.
            /// @details Creates a rigid object to be placed in the physics world later. @n
            /// This is automaticly called by the Constructor and shouldn't be called manually.
            /// @param pmass "Real Mass" The mass of the object.
            virtual void CreateRigidObject (Real pmass);

        public:
            /// @brief Descriptive constructor.
            /// @details This constructor contains the basic information needed to make a Rigid Object. @n
            /// This class inherits from ActorBase.
            /// @param mass The mass the object will have in the World.
            /// @param name The name of the actor.
            /// @param file The 3d mesh file that contains the 3d model the actor will use.
            /// @param group The resource group where the 3d mesh and other related files can be found.
            ActorRigid(Real mass, String name, String file, String group);

            /// @brief Destructor.
            /// @details The class destructor.
            virtual ~ActorRigid();

            /// @brief Retrieves the name of the object.
            /// @details This function will retrieve the name of the object,
            virtual std::string GetName() const;

            /// @brief Creates a collision shape from mesh file.
            /// @details This function will read the location of every verticy in the mesh file and use that to
            /// construct a triangle mesh shape and attach it to this objects collision shape.  This shoiuld
            /// be used with only with Dynamic objects.
            /// @param Accuracy A value from 1 to 4. The higher the more accurate, but the more resource intensive
            /// @param UseAllSubmeshes Should Child meshes, if any, be used when approximating physics shapes
            virtual void CreateShapeFromMeshDynamic(short unsigned int Accuracy = 1, bool UseAllSubmeshes = false);

            /// @brief Creates a sphere collision shape.
            /// @details Will dynamically create a sphere collision shape from the loaded mesh file.
            virtual void CreateSphereShapeFromMesh();

            /// @brief Creates a collision shape from mesh file.
            /// @details This function will read the location of every verticy in the mesh file and use that to
            /// construct a triangle mesh shape and attach it to this objects collision shape.  This shoiuld
            /// be used with only with Static objects.
            /// @param UseAllSubmeshes If true, this will use the geometry of all submeshes of the model to make the shape.  Otherwise it'll only use the first submesh.
            virtual void CreateShapeFromMeshStatic(bool UseAllSubmeshes = false);

            /// @brief Gets the physics settings class associated with this actor.
            /// @return Returns a pointer to the physics settings class in use by this actor.
            virtual ActorRigidPhysicsSettings* GetPhysicsSettings();

            /// @brief Sets the Damping for this object.
            /// @details Both of Linear Damping and Angular Damping default to zero.  This is usful if you wish to simulate
            /// something like air resistance.  Values can range from 0.0 to 1.0.
            /// @param LinDamping Real representing the amount of Linear Damping to be applied.
            /// @param AngDamping Real representing the amount of Angular Damping to be applied.
            virtual void SetDamping(Real LinDamping, Real AngDamping);

            /// @brief Sets the Linear Velocity of this object.
            /// @param LinVel Vector3 representing the Linear Velocity to be set.
            virtual void SetLinearVelocity(Vector3 LinVel);

            /// @brief Gets the Linear Velocity of this object.
            /// @return Returns the currently set Linear Velocity of this object.
            virtual Vector3 GetLinearVelocity();

            /// @brief Sets the Angular Velocity of this object.
            /// @param AngVel Vector3 representing the Angular Velocity to be set.
            virtual void SetAngularVelocity(Vector3 AngVel);

            /// @brief Gets the Angular Velocity of this object.
            /// @return Returns the currently set Angular Velocity of this object.
            virtual Vector3 GetAngularVelocity();

            /// @brief Sets the gravity for only this object.
            /// @details This value will override the world gravity.  Should be called after adding to the world.
            /// When the object is added to the world the world gravity is applied to it.
            /// @param Gravity Vector3 representing the direction and strength of gravity to be applied.
            virtual void SetIndividualGravity(const Vector3& Gravity);

            /// @brief Gets the gravity being applied to this object.
            /// @details This is the gravity applied to this object, which may or may not be the same as the world gravity.
            /// @return Returns a Vector3 representing the gravity currently being applied to this object.
            virtual Vector3 GetIndividualGravity();

            /// @brief Restricts movement on the axis or axies of your choice.
            /// @details This function will lock any and all axies you define you want to be locked.
            /// Simply pass true to allow movement on that axis, false if you don't.  This function
            /// is primarily useful for 2D games, in which if you are viewing the playing area from
            /// the side you can pass in LimitMovementOnAxis(true,true,false) and the object will
            /// only be able to move up, down, or side to side, but not in or out.
            /// @param x Allow or Disallow use of the X axis for movement.
            /// @param y Allow or Disallow use of the Y axis for movement.
            /// @param z Allow or Disallow use of the Z axis for movement.
            virtual void LimitMovementOnAxis(bool x, bool y, bool z);

            // Inherited from ActorBase
            virtual void AddObjectToWorld (World *TargetWorld);
            virtual void RemoveObjectFromWorld(World* TargetWorld);

            /// @internal
            /// @brief Get the Physics data raw from the physic subsystem
            /// @return Currently this returns a pointer to a btRigidBody.
            virtual btRigidBody* GetBulletObject();
    };
}
#endif
