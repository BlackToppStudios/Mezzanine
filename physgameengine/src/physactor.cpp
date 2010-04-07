//© Copyright 2010 Joseph Toppi and John Blackwood
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
#ifndef _physactor_cpp
#define _physactor_cpp

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

#include "physactor.h"

class PhysMotionState : public btMotionState {
    private:
        friend class ActorBase;
        Ogre::SceneNode* snode;
        btTransform initposition;

    public:
        PhysMotionState();
        PhysMotionState(Ogre::SceneNode* scenenode);
        virtual ~PhysMotionState();
        void SetNode(Ogre::SceneNode* scenenode);

        virtual void getWorldTransform(btTransform &worldTrans) const;
        virtual void setWorldTransform(const btTransform &worldTrans);
};

///////////////////////////////////
// ActorBase class fuctions

ActorBase::ActorBase (PhysString name, PhysString file)
{
    this->MotionState = new PhysMotionState();
    this->Shape = new btEmptyShape();
    this->CreateEntity(name, file, "Default");
}

ActorBase::~ActorBase ()
{
    delete MotionState;
    delete Shape;
}

void ActorBase::CreateEntity (PhysString name, PhysString file, PhysString group)
{
    this->entity = this->physscenemanager->createEntity(name, file, group);
}

void ActorBase::CreateSceneNode ()
{
    this->node = this->physscenemanager->createSceneNode();
}

void ActorBase::SetOgreLocation (PhysVector3 Place)
{
    this->node->setPosition(Place.GetOgreVector3());
}

void ActorBase::SetBulletLocation (PhysVector3 Location)
{
    this->MotionState->initposition.setOrigin(Location.GetBulletVector3());
}

void ActorBase::SetOgreOrientation (PhysReal x, PhysReal y, PhysReal z, PhysReal w)
{
    this->node->setOrientation(x, y, z, w);
}

void ActorBase::SetLocation(PhysReal x, PhysReal y, PhysReal z)
{
    PhysVector3 temp(x,y,z);
    this->SetLocation(temp);
}

void ActorBase::SetLocation(PhysVector3 Place)
{
    this->SetBulletLocation(Place);
    this->SetOgreLocation(Place);
}

void ActorBase::SetOrientation(PhysReal x, PhysReal y, PhysReal z, PhysReal w)
{

}

void ActorBase::SetOrientation(PhysQuaternion Rotation)
{

}

void ActorBase::AttachToGraphics()
{
    CreateSceneNode ();
}



///////////////////////////////////
// ActorDynRigid class functions

ActorDynRigid::ActorDynRigid (PhysReal mass, PhysString name, PhysString file) : ActorBase (name, file)
{
    this->CreateRigidObject(mass);
}

ActorDynRigid::~ActorDynRigid ()
{
    //delete physorientation;
    delete physrigidbody;
}

void ActorDynRigid::CreateRigidObject (PhysReal pmass)
{
    btScalar bmass=pmass;
    this->physrigidbody = new btRigidBody (bmass, this->MotionState, this->Shape);
}

void ActorDynRigid::AddObjectToWorld (PhysWorld *TargetWorld, btDiscreteDynamicsWorld* TargetPhysicsWorld)
{
    //TODO: add code for adding object to the physics world
}

///////////////////////////////////
// ActordynSoft class functions

/*ActorDynSoft::ActorDynSoft ()
{
}*/

ActorDynSoft::~ActorDynSoft ()
{
}

void ActorDynSoft::CreateSoftObject ()
{
}

void ActorDynSoft::AddObjectToWorld (PhysWorld *TargetWorld, btDiscreteDynamicsWorld* TargetPhysicsWorld)
{
    //TODO: add code for adding object to the physics world
}

///////////////////////////////////
// ActorSta class functions

ActorSta::ActorSta (PhysString name, PhysString file) : ActorBase (name, file)
{
    CreateRigidObject();
}

ActorSta::~ActorSta ()
{
    delete physrigidbody;
}

void ActorSta::CreateRigidObject ()
{
    btScalar bmass=0;
    this->physrigidbody = new btRigidBody (bmass, this->MotionState, this->Shape);
}

void ActorSta::AddObjectToWorld (PhysWorld *TargetWorld, btDiscreteDynamicsWorld* TargetPhysicsWorld)
{
    //TargetPhysicsWorld->addRigidBody(

}

///////////////////////////////////
// PhysMotionState

PhysMotionState::PhysMotionState()
{
}

PhysMotionState::PhysMotionState(Ogre::SceneNode* scenenode)
{
    snode=scenenode;
    initposition.getIdentity();
}

PhysMotionState::~PhysMotionState()
{
    if (snode!=NULL)
    {
        delete snode;
    }
}

void PhysMotionState::SetNode(Ogre::SceneNode* scenenode)
{
    snode=scenenode;
}

void PhysMotionState::getWorldTransform(btTransform &worldTrans) const
{
    worldTrans=initposition;
}

void PhysMotionState::setWorldTransform(const btTransform &worldTrans)
{
    btQuaternion rotation = worldTrans.getRotation();
    snode->setOrientation(rotation.w(), rotation.x(), rotation.y(), rotation.z());
    btVector3 position = worldTrans.getOrigin();
    snode->setPosition(position.x(), position.y(), position.z());
}

#endif
