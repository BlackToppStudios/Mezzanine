#ifndef _physactor_cpp
#define _physactor_cpp

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

#include "physactor.h"

///////////////////////////////////
// ActorBase class fuctions

ActorBase::ActorBase (PhysString name, PhysString file)
{
    this->MotionState = new btDefaultMotionState();
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
    entity = this->physscenemanager->createEntity(name, file, group);
}

void ActorBase::CreateSceneNode ()
{
    node = this->physscenemanager->createSceneNode();
}

void ActorBase::SetOgreLocation (PhysVector3 Place)
{
    this->node->setPosition(Place.GetOgreVector3());
}

void ActorBase::SetBulletLocation (PhysVector3 Location)
{
    this->MotionState->m_graphicsWorldTrans.setOrigin(Location.GetBulletVector3());
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

/*ActorDynRigid::ActorDynRigid ()
{
}*/

ActorDynRigid::~ActorDynRigid ()
{
    //delete physorientation;
}

void ActorDynRigid::CreateRigidObject ()
{
/*    btScalar& x=orientation.X;
    btScalar& z=orientation.Z;
    btScalar& y=orientation.Y;
    btScalar& w=orientation.W;
    physorientation = new btQuaternion(x, y, z, w);*/
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

/*ActorSta::ActorSta ()
{
}*/

ActorSta::~ActorSta ()
{
}

void ActorSta::CreateRigidObject ()
{
}

void ActorSta::AddObjectToWorld (PhysWorld *TargetWorld, btDiscreteDynamicsWorld* TargetPhysicsWorld)
{
    //TargetPhysicsWorld->addRigidBody(

}

#endif
