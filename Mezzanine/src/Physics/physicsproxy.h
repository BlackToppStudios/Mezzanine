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
#include "worldproxy.h"

class btCollisionObject;
class btCollisionShape;

namespace Mezzanine
{
    class WorldObject;
    namespace Physics
    {
        class CollisionShape;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class PhysicsProxy
        /// @headerfile physicsproxy.h
        /// @brief This is a proxy from which physics objects are handled.
        /// @details This class is used to hold and configure the physics information for a world object.
        /// This class holds physics information from the physics sub-library and serves as a means to interact with it.
        /// Direct interaction with the internal physics object is discouraged.
        ///////////////////////////////////////
        class MEZZ_LIB PhysicsProxy : public WorldProxy
        {
        protected:
            /// @internal
            /// @brief The physics shape of the World Object.
            CollisionShape* WorldObjectShape;
            /// @internal
            /// @brief This is an internal only shape that provides the ability for a collision shape to be scaled locally (to this object alone).
            btCollisionShape* ScalerShape;
            /// @internal
            /// @brief The classifications pertaining to this object in regards to collisions.
            Whole CollisionGroup;
            /// @internal
            /// @brief  Stores the kind of World Objects that can collide with each other.
            Whole CollisionMask;
        public:
            /// @brief Standard Constructor.
            PhysicsProxy();
            /// @brief Class Destructor.
            virtual ~PhysicsProxy();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Accessor for the type of physics object.
            /// @return Returns enum value for the type of proxy this object is.
            virtual ProxyType GetPhysicsProxyType() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Collision Settings

            /// @brief Set the collision group and mask for the world object to determine what it should collide with.
            /// @details These values are automatically calculated for you with some sane default values.  Only edit these if you know what you are doing.
            /// @param Group The collision group to which this world object belongs.
            /// @param Mask The other groups to which this world object should collide with.
            virtual void SetCollisionGroupAndMask(const Whole& Group, const Whole& Mask);
            /// @brief Gets the objects collision group.
            /// @return Returns a Whole representing the collision group this object is set to.
            virtual Whole GetCollisionGroup() const;
            /// @brief Gets the object's collision mask.
            /// @return Returns a Whole representing what collision groups this object should collide with.
            virtual Whole GetCollisionMask() const;
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

            /// @brief Sets the friction coefficient.
            /// @details Higher values will resist sliding across surfaces.  This number is the
            /// coefficient of friction.  Range is from 0.0 to 2.0.  Behavior in this regard is determined by the friction of both
            /// colliding bodies.  @n Default: 0.5
            /// @param Friction A Real that is the friction coefficient desired.
            virtual void SetFriction(const Real& Friction);
            /// @brief Gets the friction coefficient.
            /// @return Returns a Real representing the World Object friction coefficient.
            virtual Real GetFriction() const;
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

            /// @brief Checks if the object is active in the simulation.
            /// @return Returns true if the object is active, false if it's deactivated(at rest).
            virtual bool IsActive() const;
            /// @brief Sets the activation state of the world object.
            /// @param State The activation state to set for the world object.  See the world objectActivationState enum for more info.
            /// @param Force Whether or not you want to force the state.  Some states may not apply based on the condition of the world object if this is set to false.
            virtual void SetActivationState(const Physics::WorldObjectActivationState State, bool Force = false);

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
            virtual void SetScale(const Vector3& Sc);
            /// @copydoc WorldProxy::SetScale(const Real, const Real, const Real)
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
            virtual void Scale(const Vector3& Scale);
            /// @copydoc WorldProxy::Scale(const Real, const Real, const Real)
            virtual void Scale(const Real X, const Real Y, const Real Z);

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            ///@internal
            ///@brief Accessor for the internal physics object.
            virtual btCollisionObject* _GetBasePhysicsObject() const = 0;
        };// PhysicsProxy
    }// Physics
}// Mezzanine

#endif
