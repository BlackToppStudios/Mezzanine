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
        public:
            /// @brief Standard Constructor.
            /// @param Actor The actor this settings class configures.
            /// @param PhysicsObject The physics object belonging to the actor this class configures.
            ActorBasePhysicsSettings(ActorBase* Actor, btCollisionObject* PhysicsObject);

            /// @brief Class destructor.
            virtual ~ActorBasePhysicsSettings();

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

            /// @brief Sets the state of the object to Kinematic.
            /// @details This function will set the object to a Kinematic Object. @n
            /// Kinematic Objects are like Static Objects but are also able to be moved directly by character controllers.
            virtual void SetKinematic();

            /// @brief Is the actir kinematic
            /// @return True if the actor is kinematic false if it is not.
            virtual bool GetKinematic() const;

            /// @brief Sets the state of the object to Static.
            /// @details This function will set the object to a Static Object. @n
            /// Static Objects don't move or have any force applied to them, but are cabable of exerting force on other objects.
            virtual void SetStatic();

            /// @brief Is the actor static
            /// @return True if the actor is Static false if it is not.
            virtual bool GetStatic() const;

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
            virtual void SetActivationState(phys::ActorActivationState State, bool Force = false);

            /// @brief How activated or deactivated is the object.
            /// @return A phys::ActorActivationState that describes whether the actor is part of the calculated simulation or not.
            virtual phys::ActorActivationState GetActivationState() const;

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

            /// @brief Sets the Damping for this object.
            /// @details Both of Linear Damping and Angular Damping default to zero.  This is usful if you wish to simulate
            /// something like air resistance.  Values can range from 0.0 to 1.0.
            /// @param LinDamping Real representing the amount of Linear Damping to be applied.
            /// @param AngDamping Real representing the amount of Angular Damping to be applied.
            virtual void SetDamping(const Real& LinDamping, const Real& AngDamping);

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
            ~ActorSoftPhysicsSettings();
    };//actorsoftphysicssettings
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
/// @return This returns thexml::Node that was passed in.
phys::xml::Node& PHYS_LIB operator >> (const phys::xml::Node& OneNode, phys::ActorBasePhysicsSettings& Ev);

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
/// @return This returns thexml::Node that was passed in.
phys::xml::Node& PHYS_LIB operator >> (const phys::xml::Node& OneNode, phys::ActorRigidPhysicsSettings& Ev);

#endif // \PHYSXML


#endif
