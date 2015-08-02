// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#include "Physics/collisionshapemanager.h"

#include "enumerations.h"
#include "stringtool.h"

#include "Internal/motionstate.h.cpp"

#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>

namespace Mezzanine
{
    namespace Physics
    {
        RigidProxy::RigidProxy(const UInt32 ID, const Real Mass, PhysicsManager* Creator) :
            CollidableProxy(ID,Creator),
            PhysicsRigidBody(NULL)
        {
            this->CreateRigidObject(Mass);
        }

        RigidProxy::RigidProxy(const UInt32 ID, const Real Mass, CollisionShape* Shape, PhysicsManager* Creator) :
            CollidableProxy(ID,Creator),
            PhysicsRigidBody(NULL)
        {
            this->CreateRigidObject(Mass);
            this->SetCollisionShape(Shape);
        }

        RigidProxy::RigidProxy(const XML::Node& SelfRoot, PhysicsManager* Creator) :
            CollidableProxy(Creator),
            PhysicsRigidBody(NULL)
        {
            this->CreateRigidObject(1);
            this->ProtoDeSerialize(SelfRoot);
        }

        RigidProxy::~RigidProxy()
        {
            if( this->IsInWorld() )
                this->RemoveFromWorld();

            delete this->PhysicsRigidBody->getMotionState();
            delete this->PhysicsRigidBody;
        }

        void RigidProxy::CreateRigidObject(const Real Mass)
        {
            this->PhysicsRigidBody = new btRigidBody(Mass, NULL/* MotionState */, NULL/* CollisionShape */);
            this->PhysicsRigidBody->setMotionState( new Internal::MultiMotionState( this ) );
            this->PhysicsRigidBody->setUserPointer( static_cast<CollidableProxy*>( this ) );
            if(0.0 == Mass) {
                this->PhysicsRigidBody->setCollisionFlags( btCollisionObject::CF_STATIC_OBJECT );
                this->CollisionGroup = Physics::CF_StaticFilter;
                this->CollisionMask = Physics::CF_AllFilter & ~Physics::CF_StaticFilter;
            }else{
                this->PhysicsRigidBody->setCollisionFlags( this->PhysicsRigidBody->getCollisionFlags() & (~btCollisionObject::CF_STATIC_OBJECT) );
                // Use default group and mask
            }
            this->SetGravity( this->Manager->GetWorldGravity() );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Mezzanine::ProxyType RigidProxy::GetProxyType() const
        {
            return Mezzanine::PT_Physics_RigidProxy;
        }

        void RigidProxy::AddToWorld()
        {
            if( !this->IsInWorld() ) {
                // Preserve gravity when adding
                Vector3 Grav = this->GetGravity();
                this->Manager->_GetPhysicsWorldPointer()->addRigidBody( this->PhysicsRigidBody, this->CollisionGroup, this->CollisionMask );
                this->SetGravity(Grav);
            }
        }

        void RigidProxy::RemoveFromWorld()
        {
            if( this->IsInWorld() ) {
                this->Manager->_GetPhysicsWorldPointer()->removeRigidBody( this->PhysicsRigidBody );
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Collision Settings

        void RigidProxy::SetCollisionShape(CollisionShape* Shape)
        {
            if( Shape != NULL ) {
                if(CollisionShape::ST_StaticTriMesh != Shape->GetType()) {
                    Real Mass = this->PhysicsRigidBody->getInvMass();
                    if(0 != Mass) {
                        Mass = 1 / Mass;
                    }
                    btVector3 Inertia(0,0,0);
                    Shape->_GetInternalShape()->calculateLocalInertia(Mass,Inertia);
                    this->PhysicsRigidBody->setMassProps(Mass,Inertia);
                    this->CollidableProxy::SetCollisionShape(Shape);
                    this->PhysicsRigidBody->updateInertiaTensor();
                }else{
                    this->CollidableProxy::SetCollisionShape(Shape);
                }
                if (CollisionShapeManager::GetSingletonPtr())
                    { CollisionShapeManager::GetSingletonPtr()->StoreShape(Shape); }
            }else{
                this->CollidableProxy::SetCollisionShape(Shape);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Movement Factors

        void RigidProxy::SetLinearMovementFactor(const Vector3& Factor)
            { this->PhysicsRigidBody->setLinearFactor(Factor.GetBulletVector3()); }

        Vector3 RigidProxy::GetLinearMovementFactor() const
            { return Vector3(this->PhysicsRigidBody->getLinearFactor()); }

        void RigidProxy::SetAngularMovementFactor(const Vector3& Factor)
            { this->PhysicsRigidBody->setAngularFactor(Factor.GetBulletVector3()); }

        Vector3 RigidProxy::GetAngularMovementFactor() const
            { return Vector3(this->PhysicsRigidBody->getAngularFactor()); }

        ///////////////////////////////////////////////////////////////////////////////
        // Rigid Physics Properties

        void RigidProxy::SetMass(Real NewMass)
            { this->PhysicsRigidBody->setMassProps( NewMass, btVector3(1,1,1) / this->PhysicsRigidBody->getInvInertiaDiagLocal() ); }

        Real RigidProxy::GetMass() const
            { return ( this->PhysicsRigidBody->getInvMass() != 0 ? 1 / this->PhysicsRigidBody->getInvMass() : 0 ); }

        void RigidProxy::SetDamping(const Real LinDamping, const Real AngDamping)
            { this->PhysicsRigidBody->setDamping( LinDamping, AngDamping ); }

        Real RigidProxy::GetLinearDamping() const
            { return this->PhysicsRigidBody->getLinearDamping(); }

        Real RigidProxy::GetAngularDamping() const
            { return this->PhysicsRigidBody->getAngularDamping(); }

        void RigidProxy::SetLinearVelocity(const Vector3& LinVel)
            { this->PhysicsRigidBody->setLinearVelocity( LinVel.GetBulletVector3() ); }

        Vector3 RigidProxy::GetLinearVelocity() const
            { return Vector3(this->PhysicsRigidBody->getLinearVelocity()); }

        void RigidProxy::SetAngularVelocity(const Vector3& AngVel)
            { this->PhysicsRigidBody->setAngularVelocity( AngVel.GetBulletVector3() ); }

        Vector3 RigidProxy::GetAngularVelocity() const
            { return Vector3(this->PhysicsRigidBody->getAngularVelocity()); }

        void RigidProxy::SetGravity(const Vector3& Gravity)
            { this->PhysicsRigidBody->setGravity( Gravity.GetBulletVector3() ); }

        Vector3 RigidProxy::GetGravity() const
            { return Vector3(this->PhysicsRigidBody->getGravity()); }

        void RigidProxy::ApplyForce(const Vector3& Force)
            { this->PhysicsRigidBody->applyCentralForce( Force.GetBulletVector3() ); }

        Vector3 RigidProxy::GetAppliedForce() const
            { return Vector3(this->PhysicsRigidBody->getTotalForce()); }

        void RigidProxy::ApplyTorque(const Vector3& Torque)
            { this->PhysicsRigidBody->applyTorque( Torque.GetBulletVector3() ); }

        Vector3 RigidProxy::GetAppliedTorque() const
            { return Vector3(this->PhysicsRigidBody->getTotalTorque()); }

        ///////////////////////////////////////////////////////////////////////////////
        // Sticky Data

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
        // Transform Syncronization

        void RigidProxy::AddSyncObject(TransformableObject* ToBeAdded)
            { static_cast<Internal::MultiMotionState*>( this->PhysicsRigidBody->getMotionState() )->AddSyncObject(ToBeAdded); }

        TransformableObject* RigidProxy::GetSyncObject(const UInt32 Index) const
            { return static_cast<Internal::MultiMotionState*>( this->PhysicsRigidBody->getMotionState() )->GetSyncObject(Index); }

        UInt32 RigidProxy::GetNumSyncObjects() const
            { return static_cast<Internal::MultiMotionState*>( this->PhysicsRigidBody->getMotionState() )->GetNumSyncObjects(); }

        void RigidProxy::RemoveSyncObject(TransformableObject* ToBeRemoved)
            { static_cast<Internal::MultiMotionState*>( this->PhysicsRigidBody->getMotionState() )->RemoveSyncObject(ToBeRemoved); }

        void RigidProxy::RemoveAllSyncObjects()
            { static_cast<Internal::MultiMotionState*>( this->PhysicsRigidBody->getMotionState() )->RemoveAllSyncObjects(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void RigidProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->CollidableProxy::ProtoSerialize(SelfRoot);
            // We're at the base implementation, so no calling of child implementations
            XML::Node PropertiesNode = SelfRoot.AppendChild( RigidProxy::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("Mass").SetValue( this->GetMass() ) &&
                PropertiesNode.AppendAttribute("LinearDamping").SetValue( this->GetLinearDamping() ) &&
                PropertiesNode.AppendAttribute("AngularDamping").SetValue( this->GetAngularDamping() ) )
            {
                XML::Node LinVelNode = PropertiesNode.AppendChild("LinearVelocty");
                this->GetLinearVelocity().ProtoSerialize( LinVelNode );

                XML::Node AngVelNode = PropertiesNode.AppendChild("AngularVelocity");
                this->GetAngularVelocity().ProtoSerialize( AngVelNode );

                XML::Node LinFactNode = PropertiesNode.AppendChild("LinearFactor");
                this->GetLinearMovementFactor().ProtoSerialize( LinFactNode );

                XML::Node AngFactNode = PropertiesNode.AppendChild("AngularFactor");
                this->GetAngularMovementFactor().ProtoSerialize( AngFactNode );

                XML::Node ForceNode = PropertiesNode.AppendChild("Force");
                this->GetAppliedForce().ProtoSerialize( ForceNode );

                XML::Node TorqueNode = PropertiesNode.AppendChild("Torque");
                this->GetAppliedTorque().ProtoSerialize( TorqueNode );

                XML::Node GravityNode = PropertiesNode.AppendChild("Gravity");
                this->GetGravity().ProtoSerialize( GravityNode );

                return;
            }else{
                SerializeError("Create XML Attribute Values",RigidProxy::GetSerializableName() + "Properties",true);
            }
        }

        void RigidProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->PhysicsRigidBody->clearForces();
            this->CollidableProxy::ProtoDeSerialize(SelfRoot);
            // We're at the base implementation, so no calling of child implementations
            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( RigidProxy::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    Real LinDam = 0, AngDam = 0;

                    CurrAttrib = PropertiesNode.GetAttribute("Mass");
                    if( !CurrAttrib.Empty() )
                        this->SetMass( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("LinearDamping");
                    if( !CurrAttrib.Empty() )
                        LinDam = CurrAttrib.AsReal();

                    CurrAttrib = PropertiesNode.GetAttribute("AngularDamping");
                    if( !CurrAttrib.Empty() )
                        AngDam = CurrAttrib.AsReal();

                    this->SetDamping(LinDam,AngDam);

                    // Get the properties that need their own nodes
                    XML::Node LinVelNode = PropertiesNode.GetChild("LinearVelocty").GetFirstChild();
                    if( !LinVelNode.Empty() ) {
                        Vector3 LinVel(LinVelNode);
                        this->SetLinearVelocity(LinVel);
                    }

                    XML::Node AngVelNode = PropertiesNode.GetChild("AngularVelocity").GetFirstChild();
                    if( !AngVelNode.Empty() ) {
                        Vector3 AngVel(AngVelNode);
                        this->SetAngularVelocity(AngVel);
                    }

                    XML::Node LinFactNode = PropertiesNode.GetChild("LinearFactor").GetFirstChild();
                    if( !LinFactNode.Empty() ) {
                        Vector3 LinFact(LinFactNode);
                        this->SetLinearMovementFactor(LinFact);
                    }

                    XML::Node AngFactNode = PropertiesNode.GetChild("AngularFactor").GetFirstChild();
                    if( !AngFactNode.Empty() ) {
                        Vector3 AngFact(AngFactNode);
                        this->SetAngularMovementFactor(AngFact);
                    }

                    XML::Node ForceNode = PropertiesNode.GetChild("Force").GetFirstChild();
                    if( !ForceNode.Empty() ) {
                        Vector3 Force(ForceNode);
                        this->ApplyForce(Force);
                    }

                    XML::Node TorqueNode = PropertiesNode.GetChild("Torque").GetFirstChild();
                    if( !TorqueNode.Empty() ) {
                        Vector3 Torque(TorqueNode);
                        this->ApplyTorque(Torque);
                    }

                    XML::Node GravityNode = PropertiesNode.GetChild("Gravity").GetFirstChild();
                    if( !GravityNode.Empty() ) {
                        Vector3 Gravity(GravityNode);
                        this->SetGravity(Gravity);
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (RigidProxy::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,RigidProxy::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String RigidProxy::GetDerivedSerializableName() const
            { return RigidProxy::GetSerializableName(); }

        String RigidProxy::GetSerializableName()
            { return "RigidProxy"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        btRigidBody* RigidProxy::_GetPhysicsObject() const
            { return this->PhysicsRigidBody; }

        btCollisionObject* RigidProxy::_GetBasePhysicsObject() const
            { return this->PhysicsRigidBody; }
    }// Physics
}// Mezzanine

#endif
