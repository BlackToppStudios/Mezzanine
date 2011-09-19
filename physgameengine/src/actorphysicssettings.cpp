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
#ifndef _actorphysicssettings_cpp
#define _actorphysicssettings_cpp

#include "actorphysicssettings.h"
#include "actorbase.h"
#include "actorrigid.h"
#include "actorsoft.h"
#include "actorterrain.h"
#include "collisionshape.h"
#include "collisionshapemanager.h"
#include "serialization.h"
#include "world.h"

#ifdef PHYSXML
#include <memory>
#endif

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

namespace phys
{
    ActorBasePhysicsSettings::ActorBasePhysicsSettings(ActorBase* Actor, btCollisionObject* PhysicsObject)
        : Parent(Actor),
          ActorShape(NULL),
          ActorCO(PhysicsObject),
          CollisionGroup(0),
          CollisionMask(0)
    {
    }

    ActorBasePhysicsSettings::~ActorBasePhysicsSettings()
    {
    }

    void ActorBasePhysicsSettings::AssignShape(CollisionShape* Shape)
    {
        this->ActorShape = Shape;
        Parent->Shape = Shape->GetBulletShape();
    }

    void ActorBasePhysicsSettings::SetCollisionShape(CollisionShape* Shape)
    {
    }

    CollisionShape* ActorBasePhysicsSettings::GetCollisionShape() const
    {
        return ActorShape;
    }

    void ActorBasePhysicsSettings::SetCollisionGroupAndMask(const Whole& Group, const Whole& Mask)
    {
        CollisionGroup = Group;
        CollisionMask = Mask;
        if(Parent->IsInWorld())
        {
            Parent->RemoveObjectFromWorld();
            Parent->AddObjectToWorld();
        }
    }

    Whole ActorBasePhysicsSettings::GetCollisionGroup() const
    {
        return CollisionGroup;
    }

    Whole ActorBasePhysicsSettings::GetCollisionMask() const
    {
        return CollisionMask;
    }

    void ActorBasePhysicsSettings::SetFriction(const Real& Friction)
    {
        ActorCO->setFriction(Friction);
    }

    Real ActorBasePhysicsSettings::GetFriction() const
    {
        return ActorCO->getFriction();
    }

    void ActorBasePhysicsSettings::SetRestitution(const Real& Restitution)
        { ActorCO->setRestitution(Restitution); }

    Real ActorBasePhysicsSettings::GetRestitution() const
        { return ActorCO->getRestitution(); }

    void ActorBasePhysicsSettings::SetCCDParams(const Real& MotionThreshold, const Real& SweptSphereRadius)
    {
        if(0==MotionThreshold)
        {
            ActorCO->setCcdMotionThreshold(MotionThreshold);
            ActorCO->setCcdSweptSphereRadius(MotionThreshold);
        }else{
            ActorCO->setCcdMotionThreshold(MotionThreshold);
            if(0==SweptSphereRadius)
            {
                if(ActorShape)
                {
                    btTransform Trans;
                    btVector3 AABBmin, AABBmax, AABBsize;
                    ActorShape->GetBulletShape()->getAabb(Trans,AABBmin,AABBmax);
                    AABBsize = AABBmax - AABBmin;
                    Real ResultRadius = AABBsize.getX() < AABBsize.getY() ? (AABBsize.getX() < AABBsize.getZ() ? AABBsize.getX() : AABBsize.getZ()) : (AABBsize.getY() < AABBsize.getZ() ? AABBsize.getY() : AABBsize.getZ());
                    ActorCO->setCcdSweptSphereRadius(ResultRadius*0.2);
                }else{
                    ActorCO->setCcdSweptSphereRadius(1);
                }
            }else{
                ActorCO->setCcdSweptSphereRadius(SweptSphereRadius);
            }
        }
    }

    Real ActorBasePhysicsSettings::GetCCDMotionThreshold() const
    {
        return ActorCO->getCcdMotionThreshold();
    }

    Real ActorBasePhysicsSettings::GetCCDSphereRadius() const
    {
        return ActorCO->getCcdSweptSphereRadius();
    }

    void ActorBasePhysicsSettings::SetKinematic()
    {
        ActorCO->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
    }

    bool ActorBasePhysicsSettings::IsKinematic() const
    {
        return ActorCO->isKinematicObject();
    }

    void ActorBasePhysicsSettings::SetStatic()
    {
        ActorCO->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    }

    bool ActorBasePhysicsSettings::IsStatic() const
    {
        return ActorCO->isStaticObject();
    }

    bool ActorBasePhysicsSettings::IsStaticOrKinematic() const
    {
        return ActorCO->isStaticOrKinematicObject();
    }

    void ActorBasePhysicsSettings::EnableCollisionResponse()
    {
        World* GameWorld = World::GetWorldPointer();
        switch (Parent->GetType())
        {
            case ActorBase::Actorrigid:
            case ActorBase::Actorsoft:
            case ActorBase::Actorterrain:
            {
                /*if(Parent->IsInWorld())
                {
                    Parent->RemoveObjectFromWorld(GameWorld);
                    ActorCO->setCollisionFlags(ActorCO->getCollisionFlags() - btCollisionObject::CF_NO_CONTACT_RESPONSE);
                    Parent->AddObjectToWorld(GameWorld);
                }else{*/
                    ActorCO->setCollisionFlags(ActorCO->getCollisionFlags() - btCollisionObject::CF_NO_CONTACT_RESPONSE);
                //}
                break;
            }
            case ActorBase::Actorcharacter:
            {
                return;
                break;
            }
            default:
                return;
        }
    }

    void ActorBasePhysicsSettings::DisableCollisionResponse()
    {
        World* GameWorld = World::GetWorldPointer();
        switch (Parent->GetType())
        {
            case ActorBase::Actorrigid:
            case ActorBase::Actorsoft:
            case ActorBase::Actorterrain:
            {
                /*if(Parent->IsInWorld())
                {
                    Parent->RemoveObjectFromWorld(GameWorld);
                    ActorCO->setCollisionFlags(ActorCO->getCollisionFlags() + btCollisionObject::CF_NO_CONTACT_RESPONSE);
                    Parent->AddObjectToWorld(GameWorld);
                }else{*/
                    ActorCO->setCollisionFlags(ActorCO->getCollisionFlags() + btCollisionObject::CF_NO_CONTACT_RESPONSE);
                //}
                break;
            }
            case ActorBase::Actorcharacter:
            {
                return;
                break;
            }
            default:
                return;
        }
    }

    bool ActorBasePhysicsSettings::GetCollisionResponse() const
    {
        return !(ActorCO->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE);
    }

    bool ActorBasePhysicsSettings::IsActive() const
    {
        int Activation = ActorCO->getActivationState();
        if( ACTIVE_TAG == Activation )
        {
            return true;
        }else{
            return false;
        }
    }

    void ActorBasePhysicsSettings::SetActivationState(phys::ActorActivationState State, bool Force)
    {
        if(Force)
            ActorCO->forceActivationState(State);
        else
            ActorCO->setActivationState(State);
    }

    ActorBasePhysicsSettings* ActorBasePhysicsSettings::GetBasePointer()
        { return dynamic_cast<ActorBasePhysicsSettings*>(this); }


    phys::ActorActivationState ActorBasePhysicsSettings::GetActivationState() const
        { return (phys::ActorActivationState)ActorCO->getActivationState(); }

#ifdef PHYSXML
        // Serializable
        void ActorBasePhysicsSettings::ProtoSerialize(xml::Node& CurrentRoot) const
        {
            xml::Node BaseNode = CurrentRoot.AppendChild(this->ActorBasePhysicsSettings::SerializableName());
            if (!BaseNode)
                { SerializeError("Create BaseNode", SerializableName()); }

            phys::xml::Attribute Version = BaseNode.AppendAttribute("Version");                            // Version
            if (!Version)
                { SerializeError("Create Version Attribute", SerializableName()); }
            Version.SetValue(1);

            phys::xml::Attribute CollisionShape = BaseNode.AppendAttribute("CollisionShape");
            if (!CollisionShape)
                { SerializeError("Create CollisionShape Attribute", SerializableName()); }
            CollisionShape.SetValue(this->GetCollisionShape()->GetName());

            phys::xml::Attribute Friction = BaseNode.AppendAttribute("Friction");
            if (!Friction)
                { SerializeError("Create Friction Attribute", SerializableName()); }
            Friction.SetValue(this->GetFriction());

            phys::xml::Attribute Restitution = BaseNode.AppendAttribute("Restitution");
            if (!Restitution)
                { SerializeError("Create Restitution Attribute", SerializableName()); }
            Restitution.SetValue(this->GetRestitution());

            phys::xml::Attribute Kinematic = BaseNode.AppendAttribute("Kinematic");
            if (!Kinematic)
                { SerializeError("Create Kinematic Attribute", SerializableName()); }
            Kinematic.SetValue(this->IsKinematic());

            phys::xml::Attribute Static = BaseNode.AppendAttribute("Static");
            if (!Static)
                { SerializeError("Create Static Attribute", SerializableName()); }
            Static.SetValue(this->IsStatic());

            phys::xml::Attribute CollisionResponse = BaseNode.AppendAttribute("CollisionResponse");
            if (!CollisionResponse)
                { SerializeError("Create CollisionResponse Attribute", SerializableName()); }
            CollisionResponse.SetValue(this->GetCollisionResponse());

            phys::xml::Attribute CollisionGroup = BaseNode.AppendAttribute("CollisionGroup");
            if (!CollisionGroup)
                { SerializeError("Create CollisionGroup Attribute", SerializableName()); }
            CollisionGroup.SetValue(this->GetCollisionGroup());

            phys::xml::Attribute CollisionMask = BaseNode.AppendAttribute("CollisionMask");
            if (!CollisionMask)
                { SerializeError("Create CollisionMask Attribute", SerializableName()); }
            CollisionMask.SetValue(this->GetCollisionMask());

            phys::xml::Attribute ActivationState = BaseNode.AppendAttribute("ActivationState");
            if (!ActivationState)
                { SerializeError("Create ActivationState Attribute", SerializableName()); }
            ActivationState.SetValue(this->GetActivationState());

            phys::xml::Attribute CCDMotionThreshold = BaseNode.AppendAttribute("CCDMotionThreshold");
            if (!CCDMotionThreshold)
                { SerializeError("Create CCDMotionThreshold Attribute", SerializableName()); }
            CCDMotionThreshold.SetValue(this->GetCCDMotionThreshold());

            phys::xml::Attribute CCDSphereRadius = BaseNode.AppendAttribute("CCDSphereRadius");
            if (!CCDSphereRadius)
                { SerializeError("Create CCDSphereRadius Attribute", SerializableName()); }
            CCDSphereRadius.SetValue(this->GetCCDSphereRadius());
        }

        // DeSerializable
        void ActorBasePhysicsSettings::ProtoDeSerialize(const xml::Node& OneNode)
        {
            if ( phys::String(OneNode.Name())==this->ActorBasePhysicsSettings::SerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    CollisionShape* Shapeptr = phys::World::GetWorldPointer()->GetCollisionShapeManager()->GetShape(  OneNode.GetAttribute("CollisionShape").AsString());
                    if(!Shapeptr)
                        { DeSerializeError("Find the correct Collision Shape",this->ActorBasePhysicsSettings::SerializableName()); }
                    this->SetCollisionShape( Shapeptr );
                    this->SetFriction(OneNode.GetAttribute("Friction").AsReal());
                    this->SetRestitution(OneNode.GetAttribute("Restitution").AsReal());
                    if (OneNode.GetAttribute("Kinematic").AsBool())
                        { this->SetKinematic(); }
                    if (OneNode.GetAttribute("Static").AsBool())
                        { this->SetStatic(); }
                    this->SetCollisionGroupAndMask(OneNode.GetAttribute("CollisionGroup").AsWhole(),OneNode.GetAttribute("CollisionMask").AsWhole());
                    this->SetActivationState((phys::ActorActivationState)OneNode.GetAttribute("ActivationState").AsInt());
                    this->SetCCDParams(OneNode.GetAttribute("CCDMotionThreshold").AsReal(),OneNode.GetAttribute("CCDSphereRadius").AsReal());
                }else{
                    throw( phys::Exception(String("Incompatible XML Version for")+ this->ActorBasePhysicsSettings::SerializableName() + ": Not Version 1"));
                }
            }else{
                throw( phys::Exception(phys::StringCat("Attempting to deserialize a ", this->ActorBasePhysicsSettings::SerializableName(),", found a ", OneNode.Name())));
            }
        }

        String ActorBasePhysicsSettings::SerializableName()
            { return String("ActorBasePhysicsSettings"); }
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
// ActorRigidPhysicsSettings

    ActorRigidPhysicsSettings::ActorRigidPhysicsSettings(ActorRigid* Actor, btRigidBody* PhysicsObject)
        : ActorBasePhysicsSettings(Actor,PhysicsObject),
          RigidParent(Actor),
          ActorRB(PhysicsObject)
    {
        bool Dynamic = !IsStaticOrKinematic();
        CollisionGroup = Dynamic ? phys::CF_GenericFilter : phys::CF_StaticFilter;
        CollisionMask = Dynamic ? phys::CF_AllFilter : phys::CF_AllFilter ^ phys::CF_StaticFilter;
    }

    ActorRigidPhysicsSettings::~ActorRigidPhysicsSettings()
    {
    }

    void ActorRigidPhysicsSettings::SetCollisionShape(CollisionShape* Shape)
    {
        AssignShape(Shape);
        if(CollisionShape::ST_StaticTriMesh != Shape->GetType())
        {
            btScalar mass = this->ActorRB->getInvMass();
            if(0 != mass)
                mass=1/mass;
            btVector3 inertia(0,0,0);
            Shape->GetBulletShape()->calculateLocalInertia(mass, inertia);
            this->ActorRB->setMassProps(mass,inertia);
            this->ActorRB->setCollisionShape(Shape->GetBulletShape());
            this->ActorRB->updateInertiaTensor();
        }else{
            this->ActorRB->setCollisionShape(Shape->GetBulletShape());
        }
        World::GetWorldPointer()->GetCollisionShapeManager()->StoreShape(Shape);
    }

    void ActorRigidPhysicsSettings::SetDamping(const Real& LinDamping, const Real& AngDamping)
        { ActorRB->setDamping(LinDamping, AngDamping); }

    Real ActorRigidPhysicsSettings::GetLinearDamping() const
        { return ActorRB->getLinearDamping(); }

    Real ActorRigidPhysicsSettings::GetAngularDamping() const
        { return ActorRB->getAngularDamping(); }

    void ActorRigidPhysicsSettings::SetLinearVelocity(const Vector3& LinVel)
    {
        ActorRB->setLinearVelocity(LinVel.GetBulletVector3()); }

    Vector3 ActorRigidPhysicsSettings::GetLinearVelocity() const
        { return Vector3(ActorRB->getLinearVelocity()); }

    void ActorRigidPhysicsSettings::SetAngularVelocity(const Vector3& AngVel)
        { ActorRB->setAngularVelocity(AngVel.GetBulletVector3()); }

    Vector3 ActorRigidPhysicsSettings::GetAngularVelocity() const
        { return Vector3(ActorRB->getAngularVelocity()); }

    void ActorRigidPhysicsSettings::SetIndividualGravity(const Vector3& Gravity)
        { ActorRB->setGravity(Gravity.GetBulletVector3()); }

    Vector3 ActorRigidPhysicsSettings::GetIndividualGravity() const
        { return Vector3(ActorRB->getGravity()); }

    Vector3 ActorRigidPhysicsSettings::GetForce() const
        { return Vector3(ActorRB->getTotalForce()); }

    Vector3 ActorRigidPhysicsSettings::GetTorque() const
        { return Vector3(ActorRB->getTotalTorque()); }

    void ActorRigidPhysicsSettings::ApplyForce(const Vector3& Force)
        { ActorRB->applyCentralForce(Force.GetBulletVector3()); }

    void ActorRigidPhysicsSettings::ApplyTorque(const Vector3& Torque)
        { ActorRB->applyTorque(Torque.GetBulletVector3()); }

    Real ActorRigidPhysicsSettings::GetMass() const
        { return  ActorRB->getInvMass() ? 1/ActorRB->getInvMass() : 0; }

    Vector3 ActorRigidPhysicsSettings::GetLocalInertia() const
        { return  Vector3(ActorRB->getInvInertiaDiagLocal()).Inverse() ; }

    void ActorRigidPhysicsSettings::SetMass(Real NewMass)
        { ActorRB->setMassProps(NewMass, GetLocalInertia().GetBulletVector3()); }

    void ActorRigidPhysicsSettings::SetMass(Real NewMass,const Vector3& NewInertia)
        { ActorRB->setMassProps(NewMass, NewInertia.GetBulletVector3()); }

#ifdef PHYSXML
        // Serializable
    void ActorRigidPhysicsSettings::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node BaseNode = CurrentRoot.AppendChild(this->ActorRigidPhysicsSettings::SerializableName());
        if (!BaseNode)
            { SerializeError("Create BaseNode", SerializableName()); }

        phys::xml::Attribute Version = BaseNode.AppendAttribute("Version");                            // Version
        if (!Version)
            { SerializeError("Create Version Attribute", SerializableName()); }
        Version.SetValue(1);

        phys::xml::Attribute Mass = BaseNode.AppendAttribute("Mass");
        if (!Mass)
            { SerializeError("Create Mass Attribute", SerializableName()); }
        Mass.SetValue(this->GetMass());

        phys::xml::Attribute AngularDamping = BaseNode.AppendAttribute("AngularDamping");
        if (!AngularDamping)
            { SerializeError("Create AngularDamping Attribute", SerializableName()); }
        AngularDamping.SetValue(this->GetAngularDamping());

        phys::xml::Attribute LinearDamping = BaseNode.AppendAttribute("LinearDamping");
        if (!LinearDamping)
            { SerializeError("Create LinearDamping Attribute", SerializableName()); }
        LinearDamping.SetValue(this->GetLinearDamping());

        xml::Node AngularVelocity = BaseNode.AppendChild("AngularVelocity");
        if (!AngularVelocity)
            { SerializeError("Create AngularVelocity Node", SerializableName()); }
        this->GetAngularVelocity().ProtoSerialize(AngularVelocity);

        xml::Node LinearVelocity = BaseNode.AppendChild("LinearVelocity");
        if (!LinearVelocity)
            { SerializeError("Create LinearVelocity Node", SerializableName()); }
        this->GetLinearVelocity().ProtoSerialize(LinearVelocity);

        xml::Node IndividualGravity = BaseNode.AppendChild("IndividualGravity");
        if (!IndividualGravity)
            { SerializeError("Create IndividualGravity Node", SerializableName()); }
        this->GetIndividualGravity().ProtoSerialize(IndividualGravity);

        xml::Node TotalTorque = BaseNode.AppendChild("TotalTorque");
        if (!TotalTorque)
            { SerializeError("Create TotalTorque Node", SerializableName()); }
        this->GetTorque().ProtoSerialize(TotalTorque);

        xml::Node TotalForce = BaseNode.AppendChild("TotalForce");
        if (!TotalForce)
            { SerializeError("Create TotalForce Node", SerializableName()); }
        this->GetForce().ProtoSerialize(TotalForce);

        xml::Node Inertia = BaseNode.AppendChild("LocalInertia");
        if (!Inertia)
            { SerializeError("Create Inertia Node", SerializableName()); }
        this->GetLocalInertia().ProtoSerialize(Inertia);

        this->ActorBasePhysicsSettings::ProtoSerialize(BaseNode);
    }

    // DeSerializable
    void ActorRigidPhysicsSettings::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( phys::String(OneNode.Name())==this->ActorRigidPhysicsSettings::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                this->SetDamping(OneNode.GetAttribute("LinearDamping").AsReal(),OneNode.GetAttribute("AngularDamping").AsReal());

                phys::Vector3 TempVec;

                for(phys::xml::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
                {
                    phys::String Name(Child.Name());
                    switch(Name[5])
                    {
                        case 'B':   //ActorBasePhysicsSettings
                            if(Name==phys::String("ActorBasePhysicsSettings"))
                            {
                                this->ActorBasePhysicsSettings::ProtoDeSerialize(Child);
                            }else{
                                throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element B-\"",Name,"\"")) );
                            }
                            break;
                        case 'a':   //AngularVelocity
                            if(Name==phys::String("AngularVelocity"))
                            {
                                Child.GetFirstChild() >> TempVec;
                                this->SetAngularVelocity(TempVec);
                            }else{
                                throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element a-\"",Name,"\"")) );
                            }
                            break;
                        case 'r':   //LinearVelocity
                            if(Name==phys::String("LinearVelocity"))
                            {
                                Child.GetFirstChild() >> TempVec;
                                this->SetLinearVelocity(TempVec);
                            }else{
                                throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element r-\"",Name,"\"")) );
                            }
                            break;
                        case 'i':   //IndividualGravity
                            if(Name==phys::String("IndividualGravity"))
                            {
                                Child.GetFirstChild() >> TempVec;
                                this->SetIndividualGravity(TempVec);
                            }else{
                                throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element i-\"",Name,"\"")) );
                            }
                            break;
                        case 'T':   //TotalTorque
                            if(Name==phys::String("TotalTorque"))
                            {
                                Child.GetFirstChild() >> TempVec;
                                this->ApplyTorque(TempVec);
                            }else{
                                throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element T-\"",Name,"\"")) );
                            }
                            break;
                        case 'F':   //TotalForce
                            if(Name==phys::String("TotalForce"))
                            {
                                Child.GetFirstChild() >> TempVec;
                                this->ApplyForce(TempVec);
                            }else{
                                throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element F-\"",Name,"\"")) );
                            }
                            break;
                        case 'I':   //Inertia
                            if(Name==phys::String("LocalInertia"))
                            {
                                Child.GetFirstChild() >> TempVec;
                                this->SetMass(OneNode.GetAttribute("Mass").AsReal(), TempVec);
                            }else{
                                throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element I-\"",Name,"\"")) );
                            }
                            break;
                        default:
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for Light: Includes unknown Element default-\"",Name,"\"")) );
                            break;
                    }
                }

            }else{
                throw( phys::Exception(String("Incompatible XML Version for")+ this->ActorRigidPhysicsSettings::SerializableName() + ": Not Version 1"));
            }
        }else{
            throw( phys::Exception(String("Attempting to deserialize a ") + this->ActorRigidPhysicsSettings::SerializableName() + ", found a ", OneNode.Name()));
        }
    }

    String ActorRigidPhysicsSettings::SerializableName()
        { return String("ActorRigidPhysicsSettings"); }
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////
// ActorSoftPhysicsSettings

    ActorSoftPhysicsSettings::ActorSoftPhysicsSettings(ActorSoft* Actor, btSoftBody* PhysicsObject)
        : ActorBasePhysicsSettings(Actor,PhysicsObject),
          SoftParent(Actor),
          ActorSB(PhysicsObject)
    {
        CollisionGroup = phys::CF_GenericFilter;
        CollisionMask = phys::CF_AllFilter;
    }

    ActorSoftPhysicsSettings::~ActorSoftPhysicsSettings()
    {
    }

    void ActorSoftPhysicsSettings::SetCollisionShape(CollisionShape* Shape)
    {
        // do nothing, cause soft bodies get unique custom shapes.
    }

    ActorTerrainPhysicsSettings::ActorTerrainPhysicsSettings(ActorTerrain* Actor, btRigidBody* PhysicsObject)
        : ActorBasePhysicsSettings(Actor,PhysicsObject),
          TerrainParent(Actor),
          ActorRB(PhysicsObject)
    {
        CollisionGroup = phys::CF_StaticFilter;
        CollisionMask = phys::CF_AllFilter ^ phys::CF_StaticFilter;
    }

    ActorTerrainPhysicsSettings::~ActorTerrainPhysicsSettings()
    {
    }

    void ActorTerrainPhysicsSettings::SetCollisionShape(CollisionShape* Shape)
    {
        AssignShape(Shape);
        if(CollisionShape::ST_StaticTriMesh != Shape->GetType())
        {
            btScalar mass = this->ActorRB->getInvMass();
            if(0 != mass)
                mass=1/mass;
            btVector3 inertia(0,0,0);
            Shape->GetBulletShape()->calculateLocalInertia(mass, inertia);
            this->ActorRB->setMassProps(mass,inertia);
            this->ActorRB->updateInertiaTensor();
        }
        this->ActorRB->setCollisionShape(Shape->GetBulletShape());
        World::GetWorldPointer()->GetCollisionShapeManager()->StoreShape(Shape);
    }
}

#ifdef PHYSXML
std::ostream& operator<< (std::ostream& stream, const phys::ActorBasePhysicsSettings& Ev)
    { return Serialize(stream, Ev); }

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::ActorBasePhysicsSettings& Ev)
    { return DeSerialize(stream,Ev); }

void operator >> (const phys::xml::Node& OneNode, phys::ActorBasePhysicsSettings& Ev)
    { Ev.ProtoDeSerialize(OneNode); }


std::ostream& operator << (std::ostream& stream, const phys::ActorRigidPhysicsSettings& Ev)
    { return Serialize(stream, Ev); }

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::ActorRigidPhysicsSettings& Ev)
    { return DeSerialize(stream,Ev); }

void operator >> (const phys::xml::Node& OneNode, phys::ActorRigidPhysicsSettings& Ev)
    { Ev.ProtoDeSerialize(OneNode); }
#endif // \PHYSXML


#endif
