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
        this->PhysicsSettings = new ActorRigidPhysicsSettings(this,physrigidbody);
        BasePhysicsSettings = PhysicsSettings;
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

    std::string ActorRigid::GetName() const
    {
        return this->GraphicsObject->getName();
    }

    ActorRigidPhysicsSettings* ActorRigid::GetPhysicsSettings()
    {
        return PhysicsSettings;
    }

    void ActorRigid::SetLinearMovementFactor(const Vector3& Factor)
    {
        this->physrigidbody->setLinearFactor(Factor.GetBulletVector3());
    }

    Vector3 ActorRigid::GetLinearMovementFactor()
    {
        Vector3 LinFact(this->physrigidbody->getLinearFactor());
        return LinFact;
    }

    void ActorRigid::SetAngularMovementFactor(const Vector3& Factor)
    {
        this->physrigidbody->setAngularFactor(Factor.GetBulletVector3());
    }

    Vector3 ActorRigid::GetAngularMovementFactor()
    {
        Vector3 AngFact(this->physrigidbody->getAngularFactor());
        return AngFact;
    }

    void ActorRigid::AddObjectToWorld(World *TargetWorld)
    {
        TargetWorld->GetPhysicsManager()->GetPhysicsWorldPointer()->addRigidBody(this->physrigidbody,PhysicsSettings->GetCollisionGroup(),PhysicsSettings->GetCollisionMask());
        this->AttachToGraphics();
    }

    void ActorRigid::RemoveObjectFromWorld(World* TargetWorld)
    {
        btSoftRigidDynamicsWorld* BWorld = TargetWorld->GetPhysicsManager()->GetPhysicsWorldPointer();
        BWorld->removeRigidBody(this->physrigidbody);
        this->DetachFromGraphics();
    }

    btRigidBody* ActorRigid::GetBulletObject()
    {
        return physrigidbody;
    }

///////////////////////////////////////////////////////////////////////////////
// Serialization
///////////////////////////////////////
#ifdef PHYSXML
    void ActorRigid::ThrowSerialError(const String& Fail) const
        { SerializeError(Fail, "ActorRigid"); }

    void ActorRigid::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node ActorNode = CurrentRoot.AppendChild("ActorRigid");
        if (!ActorNode) { ThrowSerialError("create ActorRigidNode");}

        xml::Attribute ActorName = ActorNode.AppendAttribute("Name");
            ActorName.SetValue(this->GetName());
        xml::Attribute ActorFile = ActorNode.AppendAttribute("File");
            ActorFile.SetValue(this->GraphicsObject->getMesh()->getName());
        xml::Attribute ActorGroup = ActorNode.AppendAttribute("Group");
            ActorGroup.SetValue(this->GraphicsObject->getMesh()->getGroup());
        if( !(ActorName && ActorFile && ActorGroup) )
            { ThrowSerialError("creating ActorRigid Attributes");}

        ActorBase::ProtoSerialize(ActorNode);
    }

#endif  // \physxml

}

#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::ActorRigid& ActorToSerialize)
{
    Serialize(stream, ActorToSerialize);
    return stream;
}
#endif  // \physxml

#endif
