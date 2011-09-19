//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _actorphysicssettings_h
#define _actorphysicssettings_h

#include "datatypes.h"
#include "vector3.h"
#include "enumerations.h"

#ifdef PHYSXML
#include "xml.h"
#include <iostream>
#endif

class btCollisionObject;
class btRigidBody;
class btSoftBody;

namespace phys
{
    class ActorBase;
    class ActorRigid;
    class ActorSoft;
    class ActorTerrain;
    class CollisionShape;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorBasePhysicsSettings
    /// @headerfile actorphysicssettings.h
    /// @brief This is a base helper class for configuring physics settings of an actor.
    /// @details This class contains functions for the configuring of physics specific settings of an actor.
    /// This class can only configure the actors physics.  For configuring actor graphics, see ActorGraphicsSettings.
    ///////////////////////////////////////
    class PHYS_LIB ActorBasePhysicsSettings
    {
        protected:
            /// @internal
            /// @brief Physics Object of the actor.
            btCollisionObject* ActorCO;
            /// @brief The Actor this belongs to.
            ActorBase* Parent;
            /// @brief The physics shape of the actor.
            CollisionShape* ActorShape;

            /// @internal
            /// @brief A number of actors will use this in conjuction with the physics system
            Whole CollisionGroup;

            /// @internal
            /// @brief Stores the kind of actors that can collide with each other.
            Whole CollisionMask;

            void AssignShape(CollisionShape* Shape);
        public:
            /// @brief Standard Constructor.
            /// @param Actor The actor this settings class configures.
            /// @param PhysicsObject The physics object belonging to the actor this class configures.
            ActorBasePhysicsSettings(ActorBase* Actor, btCollisionObject* PhysicsObject);

            /// @brief Class destructor.
            virtual ~ActorBasePhysicsSettings();

            /// @brief Sets the collision shape to be used.
            /// @param Shape The shape to be applied.
            virtual void SetCollisionShape(CollisionShape* Shape);

            /// @brief Gets the collision shape currently in use by this actor.
            /// @return Returns a pointer to the collision shape being used.
            virtual CollisionShape* GetCollisionShape() const;

            /// @brief Set the collision group and mask for the actor to determine what it should collide with.
            /// @details These values are automatically calculated for you with some sane default values.  Only edit these if you know what you are doing.
            /// @param Group The group to which this actor belongs.
            /// @param Mask The other groups to which this actor should collide with.
            virtual void SetCollisionGroupAndMask(const Whole& Group, const Whole& Mask);

            /// @brief Gets the actors collision group.
            /// @return Returns a Whole representing the collision group this actor is set to.
            virtual Whole GetCollisionGroup() const;

            /// @brief Gets the actors collision mask.
            /// @return Returns a Whole representing what other collision groups this actor should collide with.
            virtual Whole GetCollisionMask() const;

            /// @brief Sets the Actors' friction coefficient.
            /// @details Higher values will resist sliding across surfaces.  This number is the
            /// coefficient of friction.  Range is from 0.0 to 2.0.  Behavior in this regard is determined by the friction of both
            /// colliding bodies.  @n Default: 0.5
            /// @param Friction A Real that is the friction coefficient desired.
            virtual void SetFriction(const Real& Friction);

            /// @brief Gets the Actors' friction coefficient.
            /// @return Returns a Real representing the actors' friction coefficient.
            virtual Real GetFriction() const;

            /// @brief Sets the Actors' restitution coefficient.
            /// @details Restitution determines how much energy is left after a collision with an object.
            /// Range is from 0.0 to 1.0.  Behavior in this regard is determined by the restitution of both colliding bodies.
            /// @n Default: 0.0
            /// @param Restitution A Real that is the restitution coefficient desired.
            virtual void SetRestitution(const Real& Restitution);

            /// @brief Gets the Actors' restitution coefficient.
            /// @return Returns a Real representing the actors' restitution coefficient.
            virtual Real GetRestitution() const;

            /// @brief Sets the parameters used for Continuous Collision Detection.
            /// @details If a Swept Sphere Radius is not provided, this function will attempt to find one for you using the currently set Collision Shape.
            /// If this fails for any reason or a shape is not set, it will set the radius to 1.
            /// @param MotionThreshold The speed at which the object has to be moving in order to enable CCD for the object.  If set to zero CCD will be disabled.
            /// @param SweptSphereRadius The radius of the sphere to be used for CCD sweep tests.  Essentially this should be the largest radius in which a sphere can fully fit inside your object.
            virtual void SetCCDParams(const Real& MotionThreshold, const Real& SweptSphereRadius = 0);

            /// @brief Gets the amount of motion needed to enable CCD for this object.
            /// @return Returns a Real representing the required amount of motion to enable CCD, or zero if CCD is disabled.
            virtual Real GetCCDMotionThreshold() const;

            /// @brief Gets the radius of the embedded sphere used for CCD.
            /// @return Returns a Real representing the radius of the sphere embedded into the objects collision shape used for CCD.
            virtual Real GetCCDSphereRadius() const;

            /// @brief Sets the state of the object to Kinematic.
            /// @details This function will set the object to a Kinematic Object. @n
            /// Kinematic Objects are like Static Objects but are also able to be moved directly by character controllers.
            virtual void SetKinematic();

            /// @brief Is the actir kinematic
            /// @return True if the actor is kinematic false if it is not.
            virtual bool IsKinematic() const;

            /// @brief Sets the state of the object to Static.
            /// @details This function will set the object to a Static Object. @n
            /// Static Objects don't move or have any force applied to them, but are cabable of exerting force on other objects.
            virtual void SetStatic();

            /// @brief Is the actor static
            /// @return True if the actor is Static false if it is not.
            virtual bool IsStatic() const;

            /// @brief Checks of the actor is static or kinematic.
            /// @details Checks of the actor is static or kinematic, returns true if it is either.
            /// @return Returns true if the actor is static or kinematic.
            virtual bool IsStaticOrKinematic() const;

            /// @brief Sets the actor to be able to collide with other objects in the world.
            /// @details By default collision response is enabled.  Only call this function if you have disabled collision response.
            virtual void EnableCollisionResponse();

            /// @brief Sets the actor to be unable to collide with other objects in the world.
            /// @details By default collision response is enabled.  Be sure to reactivate collision response if you want your objects to collide again.
            virtual void DisableCollisionResponse();

            /// @brief Will this respond to 3d collisions.
            /// @return False is it does not respond to collisions, True if it will
            virtual bool GetCollisionResponse() const;

            /// @brief Checks if the object is active in the simulation.
            /// @return Returns true if the object is active, false if it's deactivated(at rest).
            virtual bool IsActive() const;

            /// @brief Sets the activation state of the actor.
            /// @param State The activation state to set for the actor.  See the ActorActivationState enum for more info.
            /// @param Force Whether or not you want to force the state.  Some states may not apply based on the condition of the actor if this is set to false.
            virtual void SetActivationState(ActorActivationState State, bool Force = false);

            /// @brief How activated or deactivated is the object.
            /// @return A phys::ActorActivationState that describes whether the actor is part of the calculated simulation or not.
            virtual ActorActivationState GetActivationState() const;

            /// @internal
            /// @brief Get a pointer to this class of type ActorBasePhysicsSettings
            /// @return A pointer ActorBasePhysicsSettings
            ActorBasePhysicsSettings* GetBasePointer();

#ifdef PHYSXML
            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            // Serializable
            /// @brief Convert this class to an xml::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this quaternion should be appended to.
            virtual void ProtoSerialize(xml::Node& CurrentRoot) const;

            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and xml::Node containing the data.
            virtual void ProtoDeSerialize(const xml::Node& OneNode);

            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "ActorBasePhysicsSettings"
            static String SerializableName();
#endif

    };//actorbasephysicssettings

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorRigidPhysicsSettings
    /// @headerfile actorphysicssettings.h
    /// @brief This is a helper class for configuring physics settings of an ActorRigid.
    /// @details This class contains functions for the configuring of physics specific settings of an ActorRigid.
    /// This class can only configure the ActorRigids physics.  For configuring actor graphics, see ActorGraphicsSettings.
    ///////////////////////////////////////
    class PHYS_LIB ActorRigidPhysicsSettings : public ActorBasePhysicsSettings
    {
        protected:
            /// @internal
            /// @brief Physics Object of the actor.
            btRigidBody* ActorRB;
            /// @brief The Actor this belongs to.
            ActorRigid* RigidParent;
        public:
            /// @brief Standard Constructor.
            /// @param Actor The actor this settings class configures.
            /// @param PhysicsObject The physics object belonging to the actor this class configures.
            ActorRigidPhysicsSettings(ActorRigid* Actor, btRigidBody* PhysicsObject);

            /// @brief Class destructor.
            virtual ~ActorRigidPhysicsSettings();

            /// @brief Sets the collision shape to be used.
            /// @param Shape The shape to be applied.
            virtual void SetCollisionShape(CollisionShape* Shape);

            /// @brief Sets the Damping for this object.
            /// @details Both of Linear Damping and Angular Damping default to zero.  This is useful if you wish to simulate
            /// something like air resistance.  Values can range from 0.0 to 1.0.
            /// @param LinDamping Real representing the amount of Linear Damping(Movement) to be applied.
            /// @param AngDamping Real representing the amount of Angular Damping(Rotation) to be applied.
            virtual void SetDamping(const Real& LinDamping, const Real& AngDamping);
            /// @brief Get the linear damping
            /// @return A Real that has the Linear damping.
            virtual Real GetLinearDamping() const;
            /// @brief Get the Angular damping
            /// @return A Real that has the Angular damping.
            virtual Real GetAngularDamping() const;

            /// @brief Sets the Linear Velocity of this object.
            /// @param LinVel Vector3 representing the Linear Velocity to be set.
            virtual void SetLinearVelocity(const Vector3& LinVel);
            /// @brief Gets the Linear Velocity of this object.
            /// @return Returns the currently set Linear Velocity of this object.
            virtual Vector3 GetLinearVelocity() const;

            /// @brief Sets the Angular Velocity of this object.
            /// @param AngVel Vector3 representing the Angular Velocity to be set.
            virtual void SetAngularVelocity(const Vector3& AngVel);
            /// @brief Gets the Angular Velocity of this object.
            /// @return Returns the currently set Angular Velocity of this object.
            virtual Vector3 GetAngularVelocity() const;

            /// @brief Sets the gravity for only this object.
            /// @details This value will override the world gravity.  Should be called after adding to the world.
            /// When the object is added to the world the world gravity is applied to it.
            /// @param Gravity Vector3 representing the direction and strength of gravity to be applied.
            virtual void SetIndividualGravity(const Vector3& Gravity);
            /// @brief Gets the gravity being applied to this object.
            /// @details This is the gravity applied to this object, which may or may not be the same as the world gravity.
            /// @return Returns a Vector3 representing the gravity currently being applied to this object.
            virtual Vector3 GetIndividualGravity() const;

            /// @brief Get the total Force/Movement on the actor
            /// @return A Vector3 with the force of the entire Actor
            virtual Vector3 GetForce() const;
            /// @brief Get the Torque/Rotation
            /// @return A Vector3 with the Torque
            virtual Vector3 GetTorque() const;
            /// @brief Push/Apply force to an object.
            /// @param Force The amount and direction of the force in a Vector3
            virtual void ApplyForce(const Vector3& Force);
            /// @brief Spin/Apply Torque to an object.
            /// @param Torque The amount and direction of the torque in a Vector3
            virtual void ApplyTorque(const Vector3& Torque);

            /// @brief Get the total Mass of the actor
            /// @return A Real with the Mass of the Actor
            virtual Real GetMass() const;
            /// @brief Get the current intertia of the Actor
            /// @return A Vector3 with the Inertia
            virtual Vector3 GetLocalInertia() const;
            /// @brief Change the mass of the object.
            /// @param NewMass The amount of mass this should have.
            virtual void SetMass(Real NewMass);
            /// @brief Change the mass of the object.
            /// @param NewMass The amount of mass this should have.
            /// @param NewInertia The new inertia the object has.
            virtual void SetMass(Real NewMass, const Vector3& NewInertia);

#ifdef PHYSXML
            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            // Serializable
            /// @brief Convert this class to an xml::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this quaternion should be appended to.
            virtual void ProtoSerialize(xml::Node& CurrentRoot) const;

            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and xml::Node containing the data.
            virtual void ProtoDeSerialize(const xml::Node& OneNode);

            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "ActorRigidPhysicsSettings"
            static String SerializableName();
#endif
    };//actorrigidphysicssettings

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorSoftPhysicsSettings
    /// @headerfile actorphysicssettings.h
    /// @brief This is a helper class for configuring physics settings of an ActorSoft.
    /// @details This class contains functions for the configuring of physics specific settings of an ActorSoft.
    /// This class can only configure the ActorSofts physics.  For configuring actor graphics, see ActorGraphicsSettings.
    ///////////////////////////////////////
    class PHYS_LIB ActorSoftPhysicsSettings : public ActorBasePhysicsSettings
    {
        protected:
            /// @internal
            /// @brief Physics Object of the actor.
            btSoftBody* ActorSB;
            /// @brief The Actor this belongs to.
            ActorSoft* SoftParent;
        public:
            /// @brief Standard Constructor.
            /// @param Actor The actor this settings class configures.
            /// @param PhysicsObject The physics object belonging to the actor this class configures.
            ActorSoftPhysicsSettings(ActorSoft* Actor, btSoftBody* PhysicsObject);

            /// @brief Class destructor.
            virtual ~ActorSoftPhysicsSettings();

            /// @brief Sets the collision shape to be used.
            /// @param Shape The shape to be applied.
            virtual void SetCollisionShape(CollisionShape* Shape);
    };//actorsoftphysicssettings

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorTerrainPhysicsSettings
    /// @headerfile actorphysicssettings.h
    /// @brief This is a helper class for configuring physics settings of an ActorRigid.
    /// @details This class contains functions for the configuring of physics specific settings of an ActorRigid.
    /// This class can only configure the ActorRigids physics.  For configuring actor graphics, see ActorGraphicsSettings.
    ///////////////////////////////////////
    class PHYS_LIB ActorTerrainPhysicsSettings : public ActorBasePhysicsSettings
    {
        protected:
            /// @internal
            /// @brief Physics Object of the actor.
            btRigidBody* ActorRB;
            /// @brief The Actor this belongs to.
            ActorTerrain* TerrainParent;
        public:
            /// @brief Standard Constructor.
            /// @param Actor The actor this settings class configures.
            /// @param PhysicsObject The physics object belonging to the actor this class configures.
            ActorTerrainPhysicsSettings(ActorTerrain* Actor, btRigidBody* PhysicsObject);

            /// @brief Class destructor.
            virtual ~ActorTerrainPhysicsSettings();

            /// @brief Sets the collision shape to be used.
            /// @param Shape The shape to be applied.
            virtual void SetCollisionShape(CollisionShape* Shape);
    };//actorterrainphysicssettings
}//phys

#ifdef PHYSXML

/// @brief Serializes the passed phys::ActorBasePhysicsSettings to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the phys::ActorBasePhysicsSettings to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& PHYS_LIB operator << (std::ostream& stream, const phys::ActorBasePhysicsSettings& Ev);

/// @brief Deserialize a phys::ActorBasePhysicsSettings
/// @param stream The istream to get the xml from to (re)make the phys::ActorBasePhysicsSettings.
/// @param Ev the phys::ActorBasePhysicsSettings to be deserialized.
/// @return this returns the ostream, advanced past the phys::ActorBasePhysicsSettings that was recreated onto Ev.
std::istream& PHYS_LIB operator >> (std::istream& stream, phys::ActorBasePhysicsSettings& Ev);

/// @brief Set all values of a phys::ActorBasePhysicsSettings from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the phys::ActorBasePhysicsSettings.
/// @param Ev the phys::ActorBasePhysicsSettings to be reset.
void PHYS_LIB operator >> (const phys::xml::Node& OneNode, phys::ActorBasePhysicsSettings& Ev);

/// @brief Serializes the passed phys::ActorRigidPhysicsSettings to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the phys::ActorRigidPhysicsSettings to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& PHYS_LIB operator << (std::ostream& stream, const phys::ActorRigidPhysicsSettings& Ev);

/// @brief Deserialize a phys::ActorRigidPhysicsSettings
/// @param stream The istream to get the xml from to (re)make the phys::ActorRigidPhysicsSettings.
/// @param Ev the phys::ActorRigidPhysicsSettings to be deserialized.
/// @return this returns the ostream, advanced past the phys::ActorRigidPhysicsSettings that was recreated onto Ev.
std::istream& PHYS_LIB operator >> (std::istream& stream, phys::ActorRigidPhysicsSettings& Ev);

/// @brief Set all values of a phys::ActorRigidPhysicsSettings from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the phys::ActorRigidPhysicsSettings.
/// @param Ev the phys::ActorRigidPhysicsSettings to be reset.
void PHYS_LIB operator >> (const phys::xml::Node& OneNode, phys::ActorRigidPhysicsSettings& Ev);

#endif // \PHYSXML


#endif
