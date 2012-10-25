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
#ifndef _physicsproxy_cpp
#define _physicsproxy_cpp

#include "physicsproxy.h"
#include "btBulletDynamicsCommon.h"

namespace Mezzanine
{
	namespace Physics
	{
		PhysicsProxy::PhysicsProxy(WorldObject* WO)
			:	WorldObjectShape(NULL),
				CollisionGroup(NULL),
				CollisionMask(NULL),
				Parent(WO)
		{
		}
		
		PhysicsProxy::~PhysicsProxy()
		{
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// Collision Settings
		
		void PhysicsProxy::SetCollisionGroupAndMask(const Whole& Group, const Whole& Mask)
		{
			CollisionGroup = Group;
			CollisionMask = Mask;
			if(Parent->IsInWorld())
			{
				Parent->RemoveFromWorld();
				Parent->AddToWorld();
			}
		}

		
		Whole PhysicsProxy::GetCollisionGroup()
		{
			return CollisionGroup;
		}
		
		Whole PhysicsProxy::GetCollisionMask()
		{
			return CollisionMask;
		}
		
		void PhysicsProxy::SetCollisionShape(CollisionShape* Shape)
		{
			this->WorldObjectShape = Shape;
			this->_GetBasePhysicsObject()->setCollisionShape(Shape->GetBulletShape);
		}
		
		CollisionShape* PhysicsProxy::GetCollisionShape()
		{
			return WorldObjectShape;
		}
		
		void PhysicsProxy::SetCollisionResponse(bool Enable)
		{
			if(Enable == this->GetCollisionResponse())
				return;
			switch (Parent->GetType())
			{
				case Mezzanine::WSO_ActorRigid:
				case Mezzanine::WSO_ActorSoft:
				{
					if(Enable) this->_GetBasePhysicsObject()->setCollisionFlags(_GetBasePhysicsObject()->getCollisionFlags() + btCollisionObject::CF_NO_CONTACT_RESPONSE);
					else GetBasePhysicsObject()->setCollisionFlags(_GetBasePhysicsObject()->getCollisionFlags() - btCollisionObject::CF_NO_CONTACT_RESPONSE);
					break;
				}
				case Mezzanine::WSO_ActorCharacter:
				{
					return;
					break;
				}
				default:
					return;
			}
		}

		bool PhysicsProxy::GetCollisionResponse() const
		{
			return !(this->_GetBasePhysicsObject()->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// Static or Kinematic Properties

		void PhysicsProxy::SetKinematic()
		{
			this->_GetBasePhysicsObject()->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
		}

		void PhysicsProxy::SetStatic()
		{
			this->_GetBasePhysicsObject()->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
		}
		
		bool PhysicsProxy::IsKinematic() const
		{
			return this->_GetBasePhysicsObject()->isKinematicObject();
		}

		bool PhysicsProxy::IsStatic() const
		{
			return this->_GetBasePhysicsObject()->isStaticObject();
		}

		bool PhysicsProxy::IsStaticOrKinematic() const
		{
			return this->_GetBasePhysicsObject()->isStaticOrKinematicObject();
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// Utility
		
		void PhysicsProxy::GetLocation() const
		{
			Vector3 Location(this->_GetBasePhysicsObject()->getWorldTransform().getOrigin());
			return Location;
		}
	
		Quaternion* PhysicsProxy::GetOrientation() const
		{
			Quaternion Rotation(this->_GetBasePhysicsObject()->getWorldTransform().getRotation());
			return Rotation;
		}
		
		Vector3 PhysicsProxy::GetScaling() const
		{
			Vector3 Scale(this->_GetBasePhysicsObject()->getCollisionShape()->getLocalScaling());
			return Scale;
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// Physics Properties
		
		void PhysicsProxy::SetFriction(const Real& Friction)
		{
			this->_GetBasePhysicsObject()->SetFriction(Friction);
		}
		
		Real PhysicsProxy::GetFriction() const
		{
			return this->_GetBasePhysicsObject()->GetFriction();
		}
		
		void PhysicsProxy::SetRestitution(const Real& Restitution)
		{
			this->_GetBasePhysicsObject()->SetRestitution(Restitution);
		}
		
		Real PhysicsProxy::GetRestitution() const
		{
			this->_GetBasePhysicsObject()->GetRestitution();
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// Activation State
		
		bool PhysicsProxy::isActive() const
		{
			int Activation = this->_GetBasePhysicsObject()->getActivationState();
			if( ACTIVE_TAG == Activation ) return true;
			else return false;
		}
		
		void PhysicsProxy::SetActivationState(const WorldObjectActivationState& State, bool Force = false)
		{
			if(Force) this->_GetBasePhysicsObject()-->forceActivationState(State);
			else this->_GetBasePhysicsObject()-->setActivationState(State);
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// Internal Methods
		
		void PhysicsProxy::_SetLocation(const Real &x, const Real &y, const Real &z)
		{
			Vecter3 Location(x, y, z);
			this->SetLocation(Location);
		}
		
		void PhysicsProxy::_SetLocation(const Vector3 &Location)
		{
			this->_GetBasePhysicsObject()->getWorldTransform().setOrigin(Location.GetBulletVector3());
			this->_GetBasePhysicsObject()->getInterpolationWorldTransform().setOrigin(Location.GetBulletVector3());
		}
		
		void PhysicsProxy::_SetOrientation(const Real& x, const Real& y, const Real& z, const Real& w)
		{
			Quaternion Rotation(x, y, z, w);
			this->_SetOrientation(Rotation);
		}
	
		void PhysicsProxy::_SetOrientation(const Quaternion &Rotation)
		{
			this->_GetBasePhysicsObject()->getWorldTransform().setRotation(Rotation.GetBulletQuaternion(true));
			this->_GetBasePhysicsObject()->getInterpolationWorldTransform().setRotation(Rotation.GetBulletQuaternion(true));
		}		
		
		void PhysicsProxy::_SetScaling(const Vector3 &Scale)
		{
			this->_GetBasePhysicsObject()->getCollisionShape()->setLocalScaling(Scale.GetBulletVector3());
		}
	}// Physics
}// Mezzanine

#endif