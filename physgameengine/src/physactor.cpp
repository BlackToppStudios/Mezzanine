#ifndef _physactor_cpp
#define _physactor_cpp

#include <Ogre.h>
#include "physactor.h"

///////////////////////////////////
// ActorBase class fuctions

ActorBase::~ActorBase ()
{
}

ActorBase::ActorBase ()
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

//void actorbase::SetOgreOrientation ()
//{
//    this->node->setOrientation();
//}

///////////////////////////////////
// ActorDynRigid class functions

ActorDynRigid::ActorDynRigid () : ActorBase ()
{
}

ActorDynRigid::~ActorDynRigid ()
{
    delete physorientation;
}

void ActorDynRigid::CreateRigidObject ();
{
    PhysReal x=y=z=w=0;
    physorientation = new btQuaternion (&x,&y,&z,&w);
}

void ActorDynRigid::AddObjectToWorld ();
{
    //TODO: add code for adding object to the physics world
}

///////////////////////////////////
// ActordynSoft class functions

ActorDynSoft::ActorDynSoft () : ActorBase ()
{
}

ActorDynSoft::~ActorDynSoft ()
{
}

void ActorDynSoft::CreateSoftObject ();
{
}

void ActorDynSoft::AddObjectToWorld ();
{
    //TODO: add code for adding object to the physics world
}

///////////////////////////////////
// ActorSta class functions

ActorSta::ActorSta () : ActorBase ()
{
}

ActorSta::~ActorSta ()
{
}

void ActorSta::CreateRigidObject ();
{
}

void ActorSta::AddObjectToWorld ();
{
    //TODO: add code for adding object to the physics world
}

#endif
