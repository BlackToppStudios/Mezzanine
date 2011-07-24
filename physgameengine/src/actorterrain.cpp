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
#ifndef _actorterrain_cpp
#define _actorterrain_cpp

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

#include "actorterrain.h"
#include "world.h"
#include "physicsmanager.h"
#include "objectreference.h"
#include "actorgraphicssettings.h"
#include "internalmotionstate.h.cpp"
#include "internalmeshtools.h.cpp"

namespace phys
{
    ActorTerrain::ActorTerrain(Vector3 InitPosition, String name, String file, String group)
        : ActorBase()
    {
        this->GraphicsObject = World::GetWorldPointer()->GetSceneManager()->GetGraphicsWorldPointer()->createEntity(name, file, group);
        this->GraphicsSettings = new ActorGraphicsSettings(this,GraphicsObject);
        this->MotionState = new internal::PhysMotionState(GraphicsNode);
        this->MotionState->SetPosition(InitPosition);
        CreateCollisionTerrain();
        ActorType = ActorBase::Actorterrain;
    }

    ActorTerrain::~ActorTerrain()
    {
    }

    void ActorTerrain::CreateCollisionTerrain()
    {
        //CollisionObject = new btCollisionObject();
        btScalar mass = 0.f;
        RigidBody = new btRigidBody(mass, this->MotionState, this->Shape);
        CollisionObject = RigidBody;
        ObjectReference* ActorRef = new ObjectReference(phys::WOT_ActorTerrain,this);
        Ogre::Any OgreRef(ActorRef);
        GraphicsObject->setUserAny(OgreRef);
        CollisionObject->setUserPointer(ActorRef);
    }

    void ActorTerrain::CreateShapeFromMeshStatic(bool UseAllSubmeshes)
    {
        if(!ShapeIsSaved)
        {
            delete Shape;
        }
        /// @todo - Check for thread safety
        btBvhTriangleMeshShape *tmpshape = new btBvhTriangleMeshShape(internal::MeshTools::CreateBulletTrimesh(GraphicsObject,UseAllSubmeshes),true);
        this->Shape=tmpshape;
        ShapeIsSaved = false;
        this->Shape->setLocalScaling(btVector3(1.0,1.0,1.0));
        this->CollisionObject->setCollisionShape(this->Shape);
        this->RigidBody->updateInertiaTensor();
    }

    void ActorTerrain::CreateShapeFromMeshDynamic(short unsigned int Accuracy, bool UseAllSubmeshes)
    {
    }

    bool ActorTerrain::IsStaticOrKinematic()
    {
        return RigidBody->isStaticOrKinematicObject();
    }

    std::string ActorTerrain::GetName () const
    {
        return this->GraphicsObject->getName();
    }

    ActorRigidPhysicsSettings* ActorTerrain::GetPhysicsSettings()
    {
        return 0;
    }

    void ActorTerrain::AddObjectToWorld (World *TargetWorld)
    {
        TargetWorld->GetPhysicsManager()->GetPhysicsWorldPointer()->addCollisionObject(this->CollisionObject);
        //TargetWorld->Physics->BulletDynamicsWorld->addRigidBody(this->CollisionObject);
        this->AttachToGraphics();
    }

    void ActorTerrain::RemoveObjectFromWorld(World* TargetWorld)
    {
        TargetWorld->GetPhysicsManager()->GetPhysicsWorldPointer()->removeCollisionObject(this->CollisionObject);
        //TargetWorld->Physics->BulletDynamicsWorld->removeRigidBody(this->CollisionObject);
        this->DetachFromGraphics();
    }

    btRigidBody* ActorTerrain::GetBulletObject()
    {
        return RigidBody;
    }
}

#endif
