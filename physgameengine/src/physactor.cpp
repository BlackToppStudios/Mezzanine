#ifndef _physactor_cpp
#define _physactor_cpp

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "physactor.h"

///////////////////////////////////
// ActorBase class fuctions

ActorBase::ActorBase ()
{
}

ActorBase::~ActorBase ()
{
}

void ActorBase::CreateEntity (PhysString name, PhysString file, PhysString group)
{
    entity = this->physscenemanager->createEntity(name, file, group);
}

void ActorBase::CreateSceneNode ()
{
    node = this->physscenemanager->createSceneNode();
}

void ActorBase::SetOgreLocation (PhysReal x, PhysReal y, PhysReal z)
{
    this->node->setPosition(x, y, z);
}

void ActorBase::SetOgreOrientation (PhysReal x, PhysReal y, PhysReal z, PhysReal w)
{
    this->node->setOrientation(x, y, z, w);
}

///////////////////////////////////
// ActorDynRigid class functions

ActorDynRigid::ActorDynRigid ()
{
}

ActorDynRigid::~ActorDynRigid ()
{
    delete physorientation;
}

void ActorDynRigid::CreateRigidObject ()
{
    PhysReal X=0;
    PhysReal Y=0;
    PhysReal Z=0;
    PhysReal W=0;
    const btScalar& x=X;
    const btScalar& y=Y;
    const btScalar& z=Z;
    const btScalar& w=W;
    physorientation = new btQuaternion(x, y, z, w);
}

void ActorDynRigid::AddObjectToWorld ()
{
    //TODO: add code for adding object to the physics world
}

///////////////////////////////////
// ActordynSoft class functions

ActorDynSoft::ActorDynSoft ()
{
}

ActorDynSoft::~ActorDynSoft ()
{
}

void ActorDynSoft::CreateSoftObject ()
{
}

void ActorDynSoft::AddObjectToWorld ()
{
    //TODO: add code for adding object to the physics world
}

///////////////////////////////////
// ActorSta class functions

ActorSta::ActorSta ()
{
}

ActorSta::~ActorSta ()
{
}

void ActorSta::CreateRigidObject ()
{
}

void ActorSta::AddObjectToWorld ()
{
    //TODO: add code for adding object to the physics world
}

#endif
