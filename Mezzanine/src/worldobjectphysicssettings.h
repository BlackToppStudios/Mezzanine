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
#ifndef _worldobjectphysicssettings_h
#define _worldobjectphysicssettings_h

#include "datatypes.h"
#include "enumerations.h"
#include "Physics/physicsenumerations.h"
#include "XML/xml.h"

class btCollisionObject;
class btRigidBody;
class btSoftBody;

namespace Mezzanine
{
    class WorldObject;
    namespace Physics
    {
        class CollisionShape;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /// @class WorldObjectPhysicsSettings
    /// @headerfile worldobjectphysicssettings.h
    /// @brief This is a base helper class for configuring physics settings of a world object.
    /// @details This class contains functions for the configuring of physics specific settings of a world object.
    /// This class can only configure the world objects physics.  For configuring world object graphics, see
    /// WorldObjectGraphicsSettings.
    ///////////////////////////////////////
    class MEZZ_LIB WorldObjectPhysicsSettings
    {
        protected:
            /// @internal
            /// @brief Physics Object of the World Object.
            btCollisionObject* WorldObjectCO;
            /// @internal
            /// @brief The World Object this belongs to.
            WorldObject* Parent;
            /// @internal
            /// @brief The physics shape of the World Object.
            Physics::CollisionShape* WorldObjectShape;
            /// @internal
            /// @brief A number of World Objects will use this in conjuction with the physics system
            Whole CollisionGroup;
            /// @internal
            /// @brief Stores the kind of World Objects that can collide with each other.
            Whole CollisionMask;
            /// @internal
            /// @brief Assigns a shape to the appropriate objects.
            void AssignShape(Physics::CollisionShape* Shape);
        public:
            /// @brief Standard Constructor.
            /// @param WO The World Object this settings class configures.
            /// @param PhysicsObject The physics object belonging to the World Object this class configures.
            WorldObjectPhysicsSettings(WorldObject* WO, btCollisionObject* PhysicsObject);

            /// @brief Class destructor.
            virtual ~WorldObjectPhysicsSettings();

            /// @brief Sets the collision shape to be used.
            /// @param Shape The shape to be applied.
            virtual void SetCollisionShape(Physics::CollisionShape* Shape);

            /// @brief Gets the collision shape currently in use by this actor.
            /// @return Returns a pointer to the collision shape being used.
            virtual Physics::CollisionShape* GetCollisionShape() const;

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

            /// @brief Is the actir kinematic
            /// @return True if the actor is kinematic false if it is not.
            virtual bool IsKinematic() const;

            /// @brief Is the actor static
            /// @return True if the actor is Static false if it is not.
            virtual bool IsStatic() const;

            /// @brief Checks of the actor is static or kinematic.
            /// @details Checks of the actor is static or kinematic, returns true if it is either.
            /// @return Returns true if the actor is static or kinematic.
            virtual bool IsStaticOrKinematic() const;

            /// @brief Will this respond to 3d collisions.
            /// @return False is it does not respond to collisions, True if it will
            virtual bool GetCollisionResponse() const;

            /// @brief Checks if the object is active in the simulation.
            /// @return Returns true if the object is active, false if it's deactivated(at rest).
            virtual bool IsActive() const;

            /// @brief Sets the activation state of the actor.
            /// @param State The activation state to set for the actor.  See the ActivationState enum for more info.
            /// @param Force Whether or not you want to force the state.  Some states may not apply based on the condition of the actor if this is set to false.
            virtual void SetActivationState(const Physics::ActivationState& State, bool Force = false);

            /// @brief How activated or deactivated is the object.
            /// @return A Physics::ActivationState that describes whether the actor is part of the calculated simulation or not.
            virtual Physics::ActivationState GetActivationState() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this quaternion should be appended to.
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;

            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and XML::Node containing the data.
            virtual void ProtoDeSerialize(const XML::Node& OneNode);

            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "ActorBasePhysicsSettings"
            static String SerializableName();
    };//WorldObjectPhysicsSettings

    ///////////////////////////////////////////////////////////////////////////////
    /// @class NonTriggerPhysicsSettings
    /// @headerfile worldobjectphysicssettings.h
    /// @brief This is an extension of the basic WorldObjectPhysicsSettings class for non-trigger objects(I.E not AreaEffects).
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB NonTriggerPhysicsSettings : public WorldObjectPhysicsSettings
    {
        protected:
        public:
            /// @brief Class constructor.
            /// @param WO The World Object this settings class configures.
            /// @param PhysicsObject The physics object belonging to the World Object this class configures.
            NonTriggerPhysicsSettings(WorldObject* WO, btCollisionObject* PhysicsObject);
            /// @brief Class destructor.
            virtual ~NonTriggerPhysicsSettings();

            /// @brief Sets the World Object to be able to collide with other objects in the world.
            /// @details By default collision response is enabled.  Only call this function if you have disabled collision response.
            /// @param Enable Whether or not to enable collision response.
            virtual void SetCollisionResponse(bool Enable);

            /// @brief Sets the World Object friction coefficient.
            /// @details Higher values will resist sliding across surfaces.  This number is the
            /// coefficient of friction.  Range is from 0.0 to 2.0.  Behavior in this regard is determined by the friction of both
            /// colliding bodies.  @n Default: 0.5
            /// @param Friction A Real that is the friction coefficient desired.
            virtual void SetFriction(const Real& Friction);

            /// @brief Gets the World Object friction coefficient.
            /// @return Returns a Real representing the World Object friction coefficient.
            virtual Real GetFriction() const;

            /// @brief Sets the World Object restitution coefficient.
            /// @details Restitution determines how much energy is left after a collision with an object.
            /// Range is from 0.0 to 1.0.  Behavior in this regard is determined by the restitution of both colliding bodies.
            /// @n Default: 0.0
            /// @param Restitution A Real that is the restitution coefficient desired.
            virtual void SetRestitution(const Real& Restitution);

            /// @brief Gets the World Object restitution coefficient.
            /// @return Returns a Real representing the World Object restitution coefficient.
            virtual Real GetRestitution() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            // Serializable
            /// @brief Convert this class to an XML::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this quaternion should be appended to.
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;

            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and XML::Node containing the data.
            virtual void ProtoDeSerialize(const XML::Node& OneNode);

            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "ActorBasePhysicsSettings"
            static String SerializableName();

    };//NonTriggerPhysicsSettings
}//Mezzanine

#endif
