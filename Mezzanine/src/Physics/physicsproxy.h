//c Copyright 2010 - 2012 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

The Mezzanine Engine is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The Mezzanine Engine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with The Mezzanine Engine. If not, see <http://www.gnu.org/licenses/>.
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
#ifndef _physicsproxy_h
#define _physicsproxy_h

#include "Physics/physicsenumerations.h"
#include "axisalignedbox.h"
#include "worldproxy.h"

class btCollisionObject;
class btCollisionShape;

namespace Mezzanine
{
    class WorldObject;
    namespace Physics
    {
        class PhysicsManager;
        class CollisionShape;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a proxy from which physics objects are handled.
        /// @details This class is used to hold and configure the physics information for a world object.
        /// This class holds physics information from the physics sub-library and serves as a means to interact with it.
        /// Direct interaction with the internal physics object is discouraged.
        ///////////////////////////////////////
        class MEZZ_LIB PhysicsProxy : public WorldProxy
        {
        protected:
            /// @internal
            /// @brief A vector3 storing the scaling applied to this body.
            /// @note This exists because in bullet scaling is a property of shapes instead of bodies.
            Vector3 BodyScale;
            /// @internal
            /// @brief The physics shape of this proxy.
            CollisionShape* ProxyShape;
            /// @internal
            /// @brief This is an internal only shape that provides the ability for a collision shape to be scaled locally (to this object alone).
            btCollisionShape* ScalerShape;
            /// @internal
            /// @brief This is a pointer to the physics manager that created and owns this proxy.
            PhysicsManager* Manager;
            /// @internal
            /// @brief The classifications pertaining to this object in regards to collisions.
            Int16 CollisionGroup;
            /// @internal
            /// @brief  Stores the kind of World Objects that can collide with each other.
            Int16 CollisionMask;
        public:
            /// @brief Standard Constructor.
            /// @param Creator A pointer to the manager that created this proxy.
            PhysicsProxy(PhysicsManager* Creator);
            /// @brief Class Destructor.
            virtual ~PhysicsProxy();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets whether or not this proxy can have scaling applied to itself (instead of it's shape).
            /// @remarks When using some collision shapes (such as compound shapes or dynamic trimesh shapes) you cannot
            /// scale the body using that shape appropriately.  Any scaling that would be applied to the body has to be
            /// applied to the shape instead, which will impact it's dimensions globally, across all proxies using that
            /// shape.  In cases where the shape is used by only one proxy, this isn't an issue.  Otherwise scaling should
            /// be done with care.  This method will tell you if you should use caution when scaling this proxy.
            /// @return Returns true if this body can scale independantly from it's shape, false otherwise.
            virtual Bool CanLocallyScale() const;

            /// @brief Gets this proxies AABB.
            /// @note This will only return valid values if this proxy is in the world.  A proxy outside of the world has no AABB.
            /// @return Returns an AxisAlignedBox containing the AABB of this physics proxy.
            virtual AxisAlignedBox GetAABB() const;

            /// @copydoc WorldProxy::AddToWorld()
            virtual void AddToWorld() = 0;
            /// @copydoc WorldProxy::RemoveFromWorld()
            virtual void RemoveFromWorld() = 0;
            /// @copydoc WorldProxy::IsInWorld() const
            virtual Bool IsInWorld() const;

            /// @copydoc PhysicsProxy::GetCreator() const
            virtual WorldManager* GetCreator() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Collision Settings

            /// @brief Set the collision group and mask for the proxy to determine what it should collide with.
            /// @details These values are automatically calculated for you with some sane default values.  Only edit these if you know what you are doing.
            /// @param Group The collision group to which this proxy belongs.
            /// @param Mask The other groups to which this proxy should collide with.
            virtual void SetCollisionGroupAndMask(const Int16 Group, const Int16 Mask);
            /// @brief Sets which collision group this proxy belongs to, which determines it's collision behavior.
            /// @param Group The collision group to which this proxy belongs.
            virtual void SetCollisionGroup(const Int16 Group);
            /// @brief Sets the collision mask of this proxy, which determines which groups it will collide with.
            /// @param Mask The other groups to which this proxy should collide with.
            virtual void SetCollisionMask(const Int16 Mask);
            /// @brief Gets the objects collision group.
            /// @return Returns a Int16 representing the collision group this object is set to.
            virtual Int16 GetCollisionGroup() const;
            /// @brief Gets the object's collision mask.
            /// @return Returns a Int16 representing what collision groups this object should collide with.
            virtual Int16 GetCollisionMask() const;

            /// @brief Sets the collision shape to be used.
            /// @param Shape The shape to be applied.
            virtual void SetCollisionShape(CollisionShape* Shape);
            /// @brief Gets the collision shape currently in use by this object.
            /// @return Returns a pointer to the collision shape being used.
            virtual CollisionShape* GetCollisionShape() const;

            /// @brief Sets the World Object to be able to collide with other objects in the world.
            /// @details By default collision response is enabled.  Only call this function if you have disabled collision response.
            /// @param Enable Whether or not to enable collision response.
            virtual void SetCollisionResponse(bool Enable);
            /// @brief Will this respond to 3d collisions.
            /// @return False is it does not respond to collisions, True if it will
            virtual bool GetCollisionResponse() const;

            /// @brief Sets the collection of flags that help determine collision response for this object.
            /// @details See Physics::CollisionFlags enum for more details on the available flags.
            /// @param Flags A bitmask containing all the flags to be applied to this object.
            virtual void SetCollisionFlags(const Whole Flags);
            /// @brief Gets the collection of flags that help determine collision response for this object.
            /// @return Returns a bitmask containing all the flags used to determine collision reponse for this object.
            virtual Whole GetCollisionFlags() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Static or Kinematic Properties

            /// @brief Sets the state of the object to Kinematic.
            /// @details This function will set the object to a Kinematic Object. @n
            /// Kinematic Objects are like Static Objects but are also able to be moved directly by character controllers.
            virtual void SetKinematic();
            /// @brief Sets the state of the object to Static.
            /// @details This function will set the object to a Static Object. @n
            /// Static Objects don't move or have any force applied to them, but are cabable of exerting force on other objects.
            virtual void SetStatic();
            /// @brief Is the object kinematic.
            /// @return True if the object is kinematic false if it is not.
            virtual bool IsKinematic() const;
            /// @brief Is the object static.
            /// @return True if the object is Static false if it is not.
            virtual bool IsStatic() const;
            /// @brief Checks if the object is either static or kinematic.
            /// @details Checks if the object is either static or kinematic, returns true if it is either.
            /// @return Returns true if the object is static or kinematic.
            virtual bool IsStaticOrKinematic() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Physics Properties

            /// @brief Sets the sliding friction coefficient.
            /// @details Higher values will resist sliding across surfaces.  This number is the
            /// coefficient of friction.  Range is from 0.0 to 2.0.  Behavior in this regard is determined by the friction of both
            /// colliding bodies.  @n Default: 0.5
            /// @param Friction A Real that is the sliding friction coefficient desired.
            virtual void SetFriction(const Real Friction);
            /// @brief Gets the sliding friction coefficient.
            /// @return Returns a Real representing the sliding friction coefficient.
            virtual Real GetFriction() const;

            /// @brief Sets the rolling friction coefficient.
            /// @details This works on the same basic idea as "normal" friction but this is the coefficient to be used when the object
            /// rolls across the surface instead of slides across.
            /// @param Friction A Real that is the rolling friction coefficient desired.
            virtual void SetRollingFriction(const Real Friction);
            /// @brief Gets the rolling friction coefficient.
            /// @return Returns a Real representing the rolling friction coefficient.
            virtual Real GetRollingFriction() const;

            /// @brief Sets the anisotropic friction factor.
            /// @details Anisotropic friction is when an object has different behaviors for sliding or rolling on different axes.  An
            /// example would be a skateboard that rolls easily back and forth, but resists side to side. @n @n
            /// Note that only one Anisotropic Friction Flag can be set at a time.
            /// @param Friction A Vector3 expressing the coefficients on each of this objects local axes that will be applied to the global friction value.
            /// @param Mode The type of friction the passed in value should be set as.  See Physics::AnisotropicFrictionFlags enum for more details.
            virtual void SetAnisotropicFriction(const Vector3& Friction, const Whole Mode);
            /// @brief Gets the current Anisotropic friction mode being applied to this proxy.
            /// @return Returns a AnisotropicFrictionFlags enum value representing the anisotropic friction mode being used by this proxy.
            virtual Physics::AnisotropicFrictionFlags GetAnisotropicFrictionMode() const;
            /// @brief Gets whether or not anisotropic friction is being used in a specified mode.
            /// @param Mode The Physics::AnisotropicFrictionFlags value to check for.
            /// @return Returns true if the specified mode is the current mode of Anisotropic Friction being used.
            virtual Bool IsAnisotropicFrictionModeSet(const Whole Mode) const;
            /// @brief Gets the anisotropic friction factor.
            /// @return Returns a Vector3 representing the anisotropic friction factors on each of the linear axes.
            virtual Vector3 GetAnisotropicFriction() const;

            /// @brief Sets the restitution coefficient.
            /// @details Restitution determines how much energy is left after a collision with an object.
            /// Range is from 0.0 to 1.0.  Behavior in this regard is determined by the restitution of both colliding bodies.
            /// @n Default: 0.0
            /// @param Restitution A Real that is the restitution coefficient desired.
            virtual void SetRestitution(const Real& Restitution);
            /// @brief Gets the World Object restitution coefficient.
            /// @return Returns a Real representing the World Object restitution coefficient.
            virtual Real GetRestitution() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Activation State

            /// @brief Sets the activation state of the world object.
            /// @param State The activation state to set for this proxy.  See the ActivationState enum for more info.
            /// @param Force Whether or not you want to force the state.  Some states may not apply based on the condition of the proxy if this is set to false.
            virtual void SetActivationState(const Physics::ActivationState State, bool Force = false);
            /// @brief Gets the current activation state of this proxy.
            /// @return Returns a Physics::ActivationState value representing the current state of this proxy.
            virtual Physics::ActivationState GetActivationState() const;
            /// @brief Checks if the object is active in the simulation.
            /// @return Returns true if the object is active, false if it's deactivated(at rest).
            virtual bool IsActive() const;

            /// @brief Sets the amount of time this object needs to have no forces enacted upon it to be deactivated.
            /// @details This deactivate simply means it is skipped on more robust checks and force checking.  It does not mean
            /// that it has to be manually reactivated by the user.  A force being applied to it will re-activate it automatically.
            /// This setting is a simple optimization.
            /// @param Time The amount of time in seconds this object needs to deactivate.
            virtual void SetDeactivationTime(const Real Time);
            /// @brief Gets the current deactivation time for this object.
            /// @return Returns a Real representing the current amount of time in seconds this object needs to be stationary to deactivate.
            virtual Real GetDeactivationTime() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Transform Methods

            /// @copydoc WorldProxy::SetLocation(const Vector3&)
            virtual void SetLocation(const Vector3& Loc);
            /// @copydoc WorldProxy::SetLocation(const Real, const Real, const Real)
            virtual void SetLocation(const Real X, const Real Y, const Real Z);
            /// @copydoc WorldProxy::GetLocation() const
            virtual Vector3 GetLocation() const;
            /// @copydoc WorldProxy::SetOrientation(const Quaternion&)
            virtual void SetOrientation(const Quaternion& Ori);
            /// @copydoc WorldProxy::SetOrientation(const Real, const Real, const Real, const Real)
            virtual void SetOrientation(const Real X, const Real Y, const Real Z, const Real W);
            /// @copydoc WorldProxy::GetOrientation() const
            virtual Quaternion GetOrientation() const;
            /// @copydoc WorldProxy::SetScale(const Vector3&)
            /// @note In order to preserve consistent functionality between physics proxies and proxies of other subsystems, in the
            /// event this proxy can't be locally scaled, it will globally scale the collision shape it is currently using.
            virtual void SetScale(const Vector3& Sc);
            /// @copydoc WorldProxy::SetScale(const Real, const Real, const Real)
            /// @note In order to preserve consistent functionality between physics proxies and proxies of other subsystems, in the
            /// event this proxy can't be locally scaled, it will globally scale the collision shape it is currently using.
            virtual void SetScale(const Real X, const Real Y, const Real Z);
            /// @copydoc WorldProxy::GetScale() const
            virtual Vector3 GetScale() const;

            /// @copydoc WorldProxy::Translate(const Vector3&)
            virtual void Translate(const Vector3& Trans);
            /// @copydoc WorldProxy::Translate(const Real, const Real, const Real)
            virtual void Translate(const Real X, const Real Y, const Real Z);
            /// @copydoc WorldProxy::Yaw(const Real)
            virtual void Yaw(const Real Angle);
            /// @copydoc WorldProxy::Pitch(const Real)
            virtual void Pitch(const Real Angle);
            /// @copydoc WorldProxy::Roll(const Real)
            virtual void Roll(const Real Angle);
            /// @copydoc WorldProxy::Rotate(const Vector3&, const Real)
            virtual void Rotate(const Vector3& Axis, const Real Angle);
            /// @copydoc WorldProxy::Rotate(const Quaternion&)
            virtual void Rotate(const Quaternion& Rotation);
            /// @copydoc WorldProxy::Scale(const Vector3&)
            /// @note In order to preserve consistent functionality between physics proxies and proxies of other subsystems, in the
            /// event this proxy can't be locally scaled, it will globally scale the collision shape it is currently using.
            virtual void Scale(const Vector3& Scale);
            /// @copydoc WorldProxy::Scale(const Real, const Real, const Real)
            /// @note In order to preserve consistent functionality between physics proxies and proxies of other subsystems, in the
            /// event this proxy can't be locally scaled, it will globally scale the collision shape it is currently using.
            virtual void Scale(const Real X, const Real Y, const Real Z);

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc WorldProxy::ProtoSerialize(XML::Node&) const
            virtual void ProtoSerialize(XML::Node& ParentNode) const;
            /// @brief Convert the properties of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @brief Convert the shape of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeShape(XML::Node& SelfRoot) const;

            /// @copydoc WorldProxy::ProtoDeSerialize(const XML::Node&)
            virtual void ProtoDeSerialize(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the properties of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the shape of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeShape(const XML::Node& SelfRoot);

            /// @copydoc WorldProxy::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc WorldProxy::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Sets the maximum distance to be considered for processing collisions with this object.
            /// @param Threshold The maximum distance in world units that will be considered a collision with this object.
            virtual void _SetContactProcessingThreshold(const Real Threshold);
            /// @internal
            /// @brief Gets the maximum distance to be considered for processing collisions with this object.
            /// @return Returns a Real represnting the maximum distance in world units that is considered a collision with this object.
            virtual Real _GetContactProcessingThreshold() const;
            /// @internal
            /// @brief Accessor for the internal physics object.
            /// @return Returns a pointer to the internal object of this proxy.
            virtual btCollisionObject* _GetBasePhysicsObject() const = 0;
        };// PhysicsProxy
    }// Physics
}// Mezzanine

#endif
