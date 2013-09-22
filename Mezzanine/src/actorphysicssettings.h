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
#ifndef _actorphysicssettings_h
#define _actorphysicssettings_h

#include "transform.h"
#include "enumerations.h"
#include "worldobjectphysicssettings.h"

#ifndef SWIG
    #include "XML/xml.h"
#endif
#include <iostream>

class btCollisionObject;
class btRigidBody;

namespace Mezzanine
{
    class ActorBase;
    class ActorRigid;
    class Collision;
    class CollisionShape;
    namespace Physics
    {
        class Generic6DofConstraint;
        class SliderConstraint;
    }

    typedef Physics::Generic6DofConstraint StickyConstraint;
    /// @struct StickyConstraintConstructionInfo
    /// @headerfile actorphysicssettings.h
    /// @brief Simple struct for holding information on how sticky constraints should be constructed.
    struct MEZZ_LIB StickyConstraintConstructionInfo
    {
        Transform TransA;
        Transform TransB;
        ActorRigid* ActA;
        ActorRigid* ActB;

        StickyConstraintConstructionInfo()
        {
            this->ActA = NULL;
            this->ActB = NULL;
        }
        StickyConstraintConstructionInfo(const StickyConstraintConstructionInfo& Other)
        {
            this->TransA = Other.TransA;
            this->TransB = Other.TransB;
            this->ActA = Other.ActA;
            this->ActB = Other.ActB;
        }
    };//stickyconstraintconstructioninfo
    /// @struct StickyData
    /// @headerfile actorphysicssettings.h
    /// @brief This is a basic class for storing the data related to the sticky behavior available to actorrigid's.
    struct MEZZ_LIB StickyData
    {
        StickyData() : MaxNumContacts(0) {};
        std::vector<StickyConstraint*> StickyConstraints;
        std::vector<StickyConstraintConstructionInfo> CreationQueue;
        Whole MaxNumContacts;
    };//stickydata

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorBasePhysicsSettings
    /// @headerfile actorphysicssettings.h
    /// @brief This is a base helper class for configuring physics settings of an actor.
    /// @details This class contains functions for the configuring of physics specific settings of an actor.
    /// This class can only configure the actors physics.  For configuring actor graphics, see ActorGraphicsSettings.
    ///////////////////////////////////////
    class MEZZ_LIB ActorBasePhysicsSettings : public NonTriggerPhysicsSettings
    {
        protected:
        public:
            /// @brief Standard Constructor.
            /// @param Actor The actor this settings class configures.
            /// @param PhysicsObject The physics object belonging to the actor this class configures.
            ActorBasePhysicsSettings(ActorBase* Actor, btCollisionObject* PhysicsObject);

            /// @brief Class destructor.
            virtual ~ActorBasePhysicsSettings();

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

            /// @brief Sets the state of the object to Static.
            /// @details This function will set the object to a Static Object. @n
            /// Static Objects don't move or have any force applied to them, but are cabable of exerting force on other objects.
            virtual void SetStatic();

            /// @internal
            /// @brief Get a pointer to this class of type ActorBasePhysicsSettings
            /// @return A pointer ActorBasePhysicsSettings
            ActorBasePhysicsSettings* GetBasePointer();

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

    };//actorbasephysicssettings

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorRigidPhysicsSettings
    /// @headerfile actorphysicssettings.h
    /// @brief This is a helper class for configuring physics settings of an ActorRigid.
    /// @details This class contains functions for the configuring of physics specific settings of an ActorRigid.
    /// This class can only configure the ActorRigids physics.  For configuring actor graphics, see ActorGraphicsSettings.
    ///////////////////////////////////////
    class MEZZ_LIB ActorRigidPhysicsSettings : public ActorBasePhysicsSettings
    {
        protected:
            /// @internal
            /// @brief Physics Object of the actor.
            btRigidBody* ActorRB;
            /// @brief The Actor this belongs to.
            ActorRigid* RigidParent;
            /// @brief Data related to sticky behavior, if any is enabled.
            StickyData* StickyContacts;
        public:
            /// @brief Standard Constructor.
            /// @param Actor The actor this settings class configures.
            /// @param PhysicsObject The physics object belonging to the actor this class configures.
            ActorRigidPhysicsSettings(ActorRigid* Actor, btRigidBody* PhysicsObject);

            /// @brief Class destructor.
            virtual ~ActorRigidPhysicsSettings();

            /// @brief Sets the collision shape to be used.
            /// @param Shape The shape to be applied.
            virtual void SetCollisionShape(Physics::CollisionShape* Shape);

            /// @brief Sets the basic parameters for enabling sticky behavior with this actor.
            /// @param MaxNumContacts The maximum number of object this object can stick to or have stuck to it.
            virtual void SetStickyData(const Whole& MaxNumContacts);
            /// @brief Removes all the constraints currently active on this object
            virtual void ClearStickyContacts();
            /// @brief Gets the struct storing the data related to sticky behavior.
            /// @return Returns a pointer to the struct storing the sticky data for this actor.
            virtual StickyData* GetStickyData() const;

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
            /// @return A string containing "ActorRigidPhysicsSettings"
            static String SerializableName();

    };//actorrigidphysicssettings
}//Mezzanine

/// @brief Serializes the passed Mezzanine::ActorBasePhysicsSettings to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the Mezzanine::ActorBasePhysicsSettings to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::ActorBasePhysicsSettings& Ev);

/// @brief Deserialize a Mezzanine::ActorBasePhysicsSettings
/// @param stream The istream to get the xml from to (re)make the Mezzanine::ActorBasePhysicsSettings.
/// @param Ev the Mezzanine::ActorBasePhysicsSettings to be deserialized.
/// @return this returns the ostream, advanced past the Mezzanine::ActorBasePhysicsSettings that was recreated onto Ev.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::ActorBasePhysicsSettings& Ev);

/// @brief Set all values of a Mezzanine::ActorBasePhysicsSettings from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the Mezzanine::ActorBasePhysicsSettings.
/// @param Ev the Mezzanine::ActorBasePhysicsSettings to be reset.
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::ActorBasePhysicsSettings& Ev);

/// @brief Serializes the passed Mezzanine::ActorRigidPhysicsSettings to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the Mezzanine::ActorRigidPhysicsSettings to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::ActorRigidPhysicsSettings& Ev);

/// @brief Deserialize a Mezzanine::ActorRigidPhysicsSettings
/// @param stream The istream to get the xml from to (re)make the Mezzanine::ActorRigidPhysicsSettings.
/// @param Ev the Mezzanine::ActorRigidPhysicsSettings to be deserialized.
/// @return this returns the ostream, advanced past the Mezzanine::ActorRigidPhysicsSettings that was recreated onto Ev.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::ActorRigidPhysicsSettings& Ev);

/// @brief Set all values of a Mezzanine::ActorRigidPhysicsSettings from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the Mezzanine::ActorRigidPhysicsSettings.
/// @param Ev the Mezzanine::ActorRigidPhysicsSettings to be reset.
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::ActorRigidPhysicsSettings& Ev);



#endif
