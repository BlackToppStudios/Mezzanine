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
#include "Physics/collisionshape.h"

#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Physics
    {
        PhysicsProxy::PhysicsProxy() :
            WorldObjectShape(NULL),
            ScalerShape(NULL),
            CollisionGroup(0),
            CollisionMask(0)
        {
        }

        PhysicsProxy::~PhysicsProxy()
        {
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        ///////////////////////////////////////////////////////////////////////////////
        // Collision Settings

        void PhysicsProxy::SetCollisionGroupAndMask(const Whole& Group, const Whole& Mask)
        {
            this->CollisionGroup = Group;
            this->CollisionMask = Mask;
        }

        Whole PhysicsProxy::GetCollisionGroup() const
        {
            return this->CollisionGroup;
        }

        Whole PhysicsProxy::GetCollisionMask() const
        {
            return this->CollisionMask;
        }

        void PhysicsProxy::SetCollisionShape(CollisionShape* Shape)
        {
            /*if( this->WorldObjectShape != Shape )
            {
                if( this->ScalerShape != NULL ) {
                    delete this->ScalerShape;
                    this->ScalerShape = NULL;
                }

                switch( Shape->GetType() )
                {
                    // All the basic convex shapes
                    case CollisionShape::ST_Box:
                    case CollisionShape::ST_Capsule:
                    case CollisionShape::ST_Compound:
                    case CollisionShape::ST_Cone:
                    case CollisionShape::ST_ConvexHull:
                    case CollisionShape::ST_Cylinder:
                    case CollisionShape::ST_MultiSphere:
                    case CollisionShape::ST_Sphere:
                    {

                    }

                    case CollisionShape::ST_Compound:

                    case CollisionShape::ST_DynamicTriMesh:
                    case CollisionShape::ST_Heightfield:
                    case CollisionShape::ST_Plane:
                    case CollisionShape::ST_ActorSoft:
                    case CollisionShape::ST_StaticTriMesh:
                }
            }//*/


            this->WorldObjectShape = Shape;
            this->_GetBasePhysicsObject()->setCollisionShape( Shape->_GetInternalShape() );
        }

        CollisionShape* PhysicsProxy::GetCollisionShape() const
        {
            return this->WorldObjectShape;
        }

        void PhysicsProxy::SetCollisionResponse(bool Enable)
        {
            if( Enable == this->GetCollisionResponse() )
                return;

            btCollisionObject* Base = this->_GetBasePhysicsObject();
            if(Enable) {
                Base->setCollisionFlags( Base->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE );
            }else{
                Base->setCollisionFlags( Base->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE );
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
            btCollisionObject* Base = this->_GetBasePhysicsObject();
            Base->setCollisionFlags( Base->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );
        }

        void PhysicsProxy::SetStatic()
        {
            btCollisionObject* Base = this->_GetBasePhysicsObject();
            Base->setCollisionFlags( Base->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT );
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
        // Physics Properties

        void PhysicsProxy::SetFriction(const Real& Friction)
        {
            this->_GetBasePhysicsObject()->setFriction(Friction);
        }

        Real PhysicsProxy::GetFriction() const
        {
            return this->_GetBasePhysicsObject()->getFriction();
        }

        void PhysicsProxy::SetRestitution(const Real& Restitution)
        {
            this->_GetBasePhysicsObject()->setRestitution(Restitution);
        }

        Real PhysicsProxy::GetRestitution() const
        {
            return this->_GetBasePhysicsObject()->getRestitution();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Activation State

        bool PhysicsProxy::IsActive() const
        {
            return ( ACTIVE_TAG == this->_GetBasePhysicsObject()->getActivationState() );
        }

        void PhysicsProxy::SetActivationState(const Physics::WorldObjectActivationState State, bool Force)
        {
            if(Force) this->_GetBasePhysicsObject()->forceActivationState(State);
            else this->_GetBasePhysicsObject()->setActivationState(State);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Transform Methods

        void PhysicsProxy::SetLocation(const Vector3& Loc)
        {
            this->SetLocation(Loc.X,Loc.Y,Loc.Z);
        }

        void PhysicsProxy::SetLocation(const Real X, const Real Y, const Real Z)
        {
            btVector3 NewLoc(X,Y,Z);
            this->_GetBasePhysicsObject()->getWorldTransform().setOrigin(NewLoc);
            this->_GetBasePhysicsObject()->getInterpolationWorldTransform().setOrigin(NewLoc);
        }

        Vector3 PhysicsProxy::GetLocation() const
        {
            return Vector3( this->_GetBasePhysicsObject()->getWorldTransform().getOrigin() );
        }

        void PhysicsProxy::SetOrientation(const Quaternion& Ori)
        {
            this->SetOrientation(Ori.X,Ori.Y,Ori.Z,Ori.W);
        }

        void PhysicsProxy::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
        {
            btQuaternion NewRot(X,Y,Z,W);
            this->_GetBasePhysicsObject()->getWorldTransform().setRotation(NewRot);
            this->_GetBasePhysicsObject()->getInterpolationWorldTransform().setRotation(NewRot);
        }

        Quaternion PhysicsProxy::GetOrientation() const
        {
            return Quaternion( this->_GetBasePhysicsObject()->getWorldTransform().getRotation() );
        }

        void PhysicsProxy::SetScale(const Vector3& Sc)
        {

        }

        void PhysicsProxy::SetScale(const Real X, const Real Y, const Real Z)
        {

        }

        Vector3 PhysicsProxy::GetScale() const
        {

        }

        void PhysicsProxy::Translate(const Vector3& Trans)
        {
            this->SetLocation( this->GetLocation() + Trans );
        }

        void PhysicsProxy::Translate(const Real X, const Real Y, const Real Z)
        {
            Vector3 Trans(X,Y,Z);
            this->SetLocation( this->GetLocation() + Trans );
        }

        void PhysicsProxy::Yaw(const Real Angle)
        {
            Quaternion NewRot = this->GetOrientation() * Quaternion(Angle,Vector3::Unit_Y());
            this->SetOrientation(NewRot);
        }

        void PhysicsProxy::Pitch(const Real Angle)
        {
            Quaternion NewRot = this->GetOrientation() * Quaternion(Angle,Vector3::Unit_X());
            this->SetOrientation(NewRot);
        }

        void PhysicsProxy::Roll(const Real Angle)
        {
            Quaternion NewRot = this->GetOrientation() * Quaternion(Angle,Vector3::Unit_Z());
            this->SetOrientation(NewRot);
        }

        void PhysicsProxy::Rotate(const Vector3& Axis, const Real Angle)
        {
            Quaternion NewRot = this->GetOrientation() * Quaternion(Angle,Axis);
            this->SetOrientation(NewRot);
        }

        void PhysicsProxy::Rotate(const Quaternion& Rotation)
        {
            Quaternion NewRot = this->GetOrientation() * Rotation;
            this->SetOrientation(NewRot);
        }

        void PhysicsProxy::Scale(const Vector3& Scale)
        {

        }

        void PhysicsProxy::Scale(const Real X, const Real Y, const Real Z)
        {

        }
    }// Physics
}// Mezzanine

#endif
