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
#ifndef _rigidproxy_cpp
#define _rigidproxy_cpp

#include "Physics/rigidproxy.h"
#include "Physics/collisionshape.h"
#include "Physics/physicsmanager.h"
#include "collisionshapemanager.h"
#include "world.h"

#include "Internal/motionstate.h.cpp"

#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>

namespace Mezzanine
{
    namespace Physics
    {
        RigidProxy::RigidProxy(const Real& Mass)
        {
            this->CreateRigidObject(Mass);
        }

        RigidProxy::~RigidProxy()
        {
            delete PhysicsRigidBody;
        }

        void RigidProxy::CreateRigidObject(const Real& Mass)
        {
            this->PhysicsRigidBody = new btRigidBody(Mass, NULL/* MotionState */, NULL/* CollisionShape */);
            PhysicsRigidBody->setUserPointer(this);
            if(0.0 == Mass) {
                PhysicsRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
            }else{
                PhysicsRigidBody->setCollisionFlags(PhysicsRigidBody->getCollisionFlags() & (~btCollisionObject::CF_STATIC_OBJECT));
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Physics::ProxyType RigidProxy::GetPhysicsProxyType() const
        {
            return Physics::PT_Rigid;
        }

        void RigidProxy::AddToWorld()
        {
            if( !this->IsInWorld() && this->ParentObject ) {
                World* ParentWorld = this->ParentObject->GetWorld();
                if( ParentWorld ) {
                    ParentWorld->GetPhysicsManager()->_GetPhysicsWorldPointer()->addRigidBody( this->PhysicsRigidBody );
                }
            }
        }

        void RigidProxy::RemoveFromWorld()
        {
            if( this->IsInWorld() && this->ParentObject ) {
                World* ParentWorld = this->ParentObject->GetWorld();
                if( ParentWorld ) {
                    ParentWorld->GetPhysicsManager()->_GetPhysicsWorldPointer()->removeRigidBody( this->PhysicsRigidBody );
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Collision Settings

        void RigidProxy::SetCollisionShape(CollisionShape* Shape)
        {
            if(CollisionShape::ST_StaticTriMesh != Shape->GetType())
            {
                btScalar Mass = this->PhysicsRigidBody->getInvMass();
                if(0 != Mass)
                    Mass = 1 / Mass;
                btVector3 Inertia(0,0,0);
                Shape->_GetInternalShape()->calculateLocalInertia(Mass,Inertia);
                this->PhysicsRigidBody->setMassProps(Mass,Inertia);
                this->PhysicsProxy::SetCollisionShape(Shape);
                this->PhysicsRigidBody->updateInertiaTensor();
            }else{
                this->PhysicsProxy::SetCollisionShape(Shape);
            }
            CollisionShapeManager::GetSingletonPtr()->StoreShape(Shape);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Movement Factors

        void RigidProxy::SetAngularMovementFactor(const Vector3& Factor)
        {
            this->PhysicsRigidBody->setAngularFactor(Factor.GetBulletVector3());
        }

        Vector3 RigidProxy::GetAngularMovementFactor() const
        {
            Vector3 AngFact(this->PhysicsRigidBody->getAngularFactor());
            return AngFact;
        }

        void RigidProxy::SetLinearMovementFactor(const Vector3& Factor)
        {
            this->PhysicsRigidBody->setLinearFactor(Factor.GetBulletVector3());
        }

        Vector3 RigidProxy::GetLinearMovementFactor() const
        {
            Vector3 LinFact(this->PhysicsRigidBody->getLinearFactor());
            return LinFact;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Rigid Physics Properties

        void RigidProxy::SetDamping(const Real& LinDamping, const Real& AngDamping)
        {
            this->PhysicsRigidBody->setDamping(LinDamping, AngDamping);
        }

        Real RigidProxy::GetLinearDamping() const
        {
            return this->PhysicsRigidBody->getLinearDamping();
        }

        Real RigidProxy::GetAngularDamping() const
        {
            return this->PhysicsRigidBody->getAngularDamping();
        }

        void RigidProxy::SetLinearVelocity(const Vector3& LinVel)
        {
            this->PhysicsRigidBody->setLinearVelocity(LinVel.GetBulletVector3());
        }

        Vector3 RigidProxy::GetLinearVelocity() const
        {
            Vector3 LinVel(this->PhysicsRigidBody->getLinearVelocity());
            return LinVel;
        }

        void RigidProxy::SetAngularVelocity(const Vector3& AngVel)
        {
            this->PhysicsRigidBody->setAngularVelocity(AngVel.GetBulletVector3());
        }

        Vector3 RigidProxy::GetAngularVelocity() const
        {
            Vector3 AngVel(this->PhysicsRigidBody->getAngularVelocity());
            return AngVel;
        }

        void RigidProxy::SetIndividualGravity(const Vector3& Gravity)
        {
            this->PhysicsRigidBody->setGravity(Gravity.GetBulletVector3());
        }

        Vector3 RigidProxy::GetIndividualGravity() const
        {
            Vector3 Gravity(this->PhysicsRigidBody->getGravity());
            return Gravity;
        }

        Vector3 RigidProxy::GetForce() const
        {
            Vector3 Force(this->PhysicsRigidBody->getTotalForce());
            return Force;
        }

        Vector3 RigidProxy::GetTorque() const
        {
            Vector3 Torque(this->PhysicsRigidBody->getTotalTorque());
            return Torque;
        }

        void RigidProxy::ApplyForce(const Vector3& Force)
        {
            this->PhysicsRigidBody->applyCentralForce(Force.GetBulletVector3());
        }

        void RigidProxy::ApplyTorque(const Vector3& Torque)
        {
            this->PhysicsRigidBody->applyTorque(Torque.GetBulletVector3());
        }

        Real RigidProxy::GetMass() const
        {
            return  this->PhysicsRigidBody->getInvMass() != 0 ? 1/this->PhysicsRigidBody->getInvMass() : 0;
        }

        Vector3 RigidProxy::GetLocalInertia() const
        {
            return  Vector3(this->PhysicsRigidBody->getInvInertiaDiagLocal()).Inverse() ;
        }

        void RigidProxy::SetMass(Real NewMass)
        {
            this->PhysicsRigidBody->setMassProps(NewMass, GetLocalInertia().GetBulletVector3());
        }

        void RigidProxy::SetMass(Real NewMass,const Vector3& NewInertia)
        {
            this->PhysicsRigidBody->setMassProps(NewMass, NewInertia.GetBulletVector3());
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Sticky Data

        // virtual void SetStickyData(const Whole& MaxNumContacts);
        // virtual void ClearStickyContacts();
        // virtual StickyData* GetStickyData() const;

        /*
        void RigidProxy::SetStickyData(const Whole& MaxNumContacts)
        {
            if(MaxNumContacts > 0) StickyContacts->MaxNumContacts = MaxNumContacts;
            else ClearStickyContacts();
        }

        void RigidProxy::ClearStickyContacts()
        {
            if(0 == StickyContacts->StickyConstraints.size())
                return;
            btDiscreteDynamicsWorld* BulletWorld = Entresol::GetSingletonPtr()->GetPhysicsManager()->GetPhysicsWorldPointer();
            for( std::vector<StickyConstraint*>::iterator SCit = StickyContacts->StickyConstraints.begin() ; SCit != StickyContacts->StickyConstraints.end() ; ++SCit )
            {
                BulletWorld->removeConstraint((*SCit)->GetConstraintBase());

                ActorRigid* OtherActor = (*SCit)->GetActorA() != Parent ? (*SCit)->GetActorA() : (*SCit)->GetActorB();
                StickyData* OtherSettings = OtherActor->GetPhysicsSettings()->GetStickyData();
                for( std::vector<StickyConstraint*>::iterator SCit2 = OtherSettings->StickyConstraints.begin() ; SCit2 != OtherSettings->StickyConstraints.end() ; ++SCit2 )
                {
                    if( (*SCit) == (*SCit2) )
                    {
                        OtherSettings->StickyConstraints.erase(SCit2);
                        break;
                    }
                }

                delete (*SCit);
            }
            StickyContacts->StickyConstraints.clear();
            StickyContacts->CreationQueue.clear();
        }

        StickyData* RigidProxy::GetStickyData() const
            { return StickyContacts; }
		*/

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        btRigidBody* RigidProxy::_GetPhysicsObject() const
        {
            return this->PhysicsRigidBody;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Inherited from PhysicsProxy

        btCollisionObject* RigidProxy::_GetBasePhysicsObject() const
        {
            return this->PhysicsRigidBody;
        }
    }// Physics
}// Mezzanine

#endif
