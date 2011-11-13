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
#ifndef _physactorrigid_cpp
#define _physactorrigid_cpp

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

#include "world.h"
#include "meshmanager.h"
#include "physicsmanager.h"
#include "actorrigid.h"
#include "objectreference.h"
#include "eventcollision.h"
#include "constraint.h"
#include "actorgraphicssettings.h"
#include "internalmotionstate.h.cpp" // This is required for the internal physmotionstate :(
#include "internalmeshtools.h.cpp"
#include "serialization.h"

namespace phys{
    ///////////////////////////////////
    // ActorRigid class functions

    ActorRigid::ActorRigid(const Real& mass, const String& name, const String& file, const String& group)
        : ActorBase ()
    {
        // this isn't required to operate, but it does allow the mesh manager to know what is loaded.
        World::GetWorldPointer()->GetMeshManager()->LoadMesh(file,group);

        this->GraphicsObject = World::GetWorldPointer()->GetSceneManager()->GetGraphicsWorldPointer()->createEntity(name, file, group);
        this->MotionState = new internal::PhysMotionState(GraphicsNode);
        this->CreateRigidObject(mass);
        this->GraphicsSettings = new ActorGraphicsSettings(this,GraphicsObject);
        BasePhysicsSettings = new ActorRigidPhysicsSettings(this,physrigidbody);
        ActorType=ActorBase::Actorrigid;
    }

    ActorRigid::~ActorRigid()
    {
        delete physrigidbody;
        CollisionObject = NULL;
    }

    void ActorRigid::CreateRigidObject(const Real& pmass)
    {
        btScalar bmass=pmass;
        this->physrigidbody = new btRigidBody(bmass, this->MotionState, this->Shape);
        CollisionObject=physrigidbody;
        ObjectReference* ActorRef = new ObjectReference(phys::WOT_ActorRigid,this);
        Ogre::Any OgreRef(ActorRef);
        GraphicsObject->setUserAny(OgreRef);
        CollisionObject->setUserPointer(ActorRef);
        if(0.0 == bmass)
        {
            CollisionObject->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
        }else{
            CollisionObject->setCollisionFlags(CollisionObject->getCollisionFlags() & (~btCollisionObject::CF_STATIC_OBJECT));
        }
    }

    String ActorRigid::GetName() const
        { return String(this->GraphicsObject->getName()); }

    ActorRigidPhysicsSettings* ActorRigid::GetPhysicsSettings() const
        { return static_cast<ActorRigidPhysicsSettings*>(this->BasePhysicsSettings); }

    void ActorRigid::SetLinearMovementFactor(const Vector3& Factor)
        { this->physrigidbody->setLinearFactor(Factor.GetBulletVector3()); }

    Vector3 ActorRigid::GetLinearMovementFactor() const
        { return Vector3(this->physrigidbody->getLinearFactor()); }

    void ActorRigid::SetAngularMovementFactor(const Vector3& Factor)
        { this->physrigidbody->setAngularFactor(Factor.GetBulletVector3()); }

    Vector3 ActorRigid::GetAngularMovementFactor() const
        { return Vector3(this->physrigidbody->getAngularFactor()); }

    void ActorRigid::AddObjectToWorld()
    {
        World::GetWorldPointer()->GetPhysicsManager()->GetPhysicsWorldPointer()->addRigidBody(this->physrigidbody,GetPhysicsSettings()->GetCollisionGroup(),GetPhysicsSettings()->GetCollisionMask());
        this->AttachToGraphics();
    }

    void ActorRigid::RemoveObjectFromWorld()
    {
        btSoftRigidDynamicsWorld* BWorld = World::GetWorldPointer()->GetPhysicsManager()->GetPhysicsWorldPointer();
        BWorld->removeRigidBody(this->physrigidbody);
        this->DetachFromGraphics();
    }

    void ActorRigid::_Update()
        {  }

    void ActorRigid::_NotifyCollision(EventCollision* Collision)
    {
        ActorBase::_NotifyCollision(Collision);
        ActorRigidPhysicsSettings::StickyData* StickyD = GetPhysicsSettings()->GetStickyData();
        if(!StickyD)
            return;
        if(Collision->GetNumContactPoints() == 0)
            return;

        if(StickyD->StickyConstraints.size() < StickyD->MaxNumContacts)
        {
            bool UseA = Collision->GetActorA() != this;
            ActorRigid* ActorA = dynamic_cast<ActorRigid*>(UseA ? Collision->GetActorA() : Collision->GetActorB());
            Transform TransA(UseA ? Collision->GetLocalALocation(1) : Collision->GetLocalBLocation(1));
            Transform TransB(UseA ? Collision->GetLocalBLocation(1) : Collision->GetLocalALocation(1));
            Generic6DofConstraint* NewSticky = new Generic6DofConstraint(ActorA,this,TransA,TransB);
            World::GetWorldPointer()->GetPhysicsManager()->GetPhysicsWorldPointer()->addConstraint(NewSticky->GetConstraintBase(),true);
            StickyD->StickyConstraints.push_back(NewSticky);
        }
    }

    btRigidBody* ActorRigid::GetBulletObject()
        { return physrigidbody; }

///////////////////////////////////////////////////////////////////////////////
// Serialization
///////////////////////////////////////
#ifdef PHYSXML
    void ActorRigid::ThrowSerialError(const String& Fail) const
        { SerializeError(Fail, SerializableName()); }

    String ActorRigid::GraphicsSettingsSerializableName() const
        { return "ActorGraphicsSettings"; }

    String ActorRigid::PhysicsSettingsSerializableName() const
        { return "ActorRigidPhysicsSettings"; }

    void ActorRigid::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node ActorNode = CurrentRoot.AppendChild("ActorRigid");
        if (!ActorNode) { ThrowSerialError("create ActorRigidNode");}

        xml::Attribute Version = ActorNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create set Version on ActorRigid node", SerializableName()); }

        xml::Attribute ActorName = ActorNode.AppendAttribute("Name");
            ActorName.SetValue(this->GetName());
        xml::Attribute ActorFile = ActorNode.AppendAttribute("File");
            ActorFile.SetValue(this->GraphicsObject->getMesh()->getName());
        xml::Attribute ActorGroup = ActorNode.AppendAttribute("Group");
            ActorGroup.SetValue(this->GraphicsObject->getMesh()->getGroup());
        if( !(ActorName && ActorFile && ActorGroup) )
            { ThrowSerialError("creating ActorRigid Attributes");}

        xml::Node LinearMovementFactor = ActorNode.AppendChild("LinearMovementFactor");
        if (!LinearMovementFactor) { ThrowSerialError("create LinearMovementFactor Node"); }
        this->GetLinearMovementFactor().ProtoSerialize(LinearMovementFactor);

        xml::Node AngularMovementFactor = ActorNode.AppendChild("AngularMovementFactor");
        if (!AngularMovementFactor) { ThrowSerialError("create AngularMovementFactor Node"); }
        this->GetAngularMovementFactor().ProtoSerialize(AngularMovementFactor);

        ActorBase::ProtoSerialize(ActorNode);
    }

    void ActorRigid::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( phys::String(OneNode.Name())==this->ActorRigid::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                this->ActorBase::ProtoDeSerialize(OneNode.GetChild(this->ActorBase::SerializableName()));

                Vector3 TempVec;
                xml::Node LinearMovementFactor = OneNode.GetChild("LinearMovementFactor").GetFirstChild();
                if(!LinearMovementFactor)
                    { DeSerializeError("locate LinearMovementFactor node",SerializableName()); }
                TempVec.ProtoDeSerialize(LinearMovementFactor);
                this->SetLinearMovementFactor(TempVec);

                xml::Node AngularMovementFactor = OneNode.GetChild("AngularMovementFactor").GetFirstChild();
                if(!AngularMovementFactor)
                    { DeSerializeError("locate AngularMovementFactor node",SerializableName()); }
                TempVec.ProtoDeSerialize(AngularMovementFactor);
                this->SetAngularMovementFactor(TempVec);
                // could not do Name, File, Group - done in ActorDeSerializer
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String ActorRigid::SerializableName()
        {   return String("ActorRigid"); }
#endif  // \physxml

}

#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::ActorRigid& ActorToSerialize)
{
    Serialize(stream, ActorToSerialize);
    return stream;
}

std::istream& operator >> (std::istream& stream, phys::ActorRigid& x)
    { return DeSerialize(stream, x); }

void operator >> (const phys::xml::Node& OneNode, phys::ActorRigid& x)
    { x.ProtoDeSerialize(OneNode); }


#endif  // \physxml

#endif
