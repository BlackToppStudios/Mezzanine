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
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"

#include "world.h"
#include "physicsmanager.h"
#include "actorrigid.h"
#include "objectreference.h"
#include "actorgraphicssettings.h"
#include "internalmotionstate.h.cpp" // This is required for the internal physmotionstate :(
#include "internalmeshtools.h.cpp"

namespace phys{
    ///////////////////////////////////
    // ActorRigid class functions

    ActorRigid::ActorRigid(const Real& mass, const String& name, const String& file, const String& group)
        : ActorBase (name, file, group)
    {
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

    void ActorRigid::CreateShapeFromMeshDynamic(short unsigned int Accuracy, bool UseAllSubmeshes)
    {
        if(Accuracy==1)
        {
            if(!ShapeIsSaved)
            {
                delete Shape;
            }
            /// @todo - Check for thread safety
            btConvexShape *tmpshape = new btConvexTriangleMeshShape(internal::MeshTools::CreateBulletTrimesh(GraphicsObject,UseAllSubmeshes));
            btShapeHull *hull = new btShapeHull(tmpshape);
            btScalar margin = tmpshape->getMargin();
            hull->buildHull(margin);
            tmpshape->setUserPointer(hull);
            btConvexHullShape* convexShape = new btConvexHullShape();
            for (int b=0;b<hull->numVertices();b++)
            {
                convexShape->addPoint(hull->getVertexPointer()[b]);
            }
            delete tmpshape;
            delete hull;
            btScalar mass=this->physrigidbody->getInvMass();
            if(0 != mass)
                mass=1/mass;
            btVector3 inertia(0,0,0);
            convexShape->calculateLocalInertia(mass, inertia);
            Shape = convexShape;
            ShapeIsSaved = false;
            this->Shape->setLocalScaling(btVector3(1.0,1.0,1.0));
            this->physrigidbody->setCollisionShape(this->Shape);
            this->physrigidbody->setMassProps(mass,inertia);
            this->physrigidbody->updateInertiaTensor();
            return;
        }
        if(Accuracy==2)
        {
            if(!ShapeIsSaved)
            {
                delete Shape;
            }
            int depth=5;
            float cpercent=5;
            float ppercent=15;
            Shape = internal::MeshTools::PerformConvexDecomposition(GraphicsObject,depth,cpercent,ppercent,UseAllSubmeshes);
            ShapeIsSaved = false;
            this->physrigidbody->setCollisionShape(this->Shape);

            btScalar mass=this->physrigidbody->getInvMass();
            if(0 != mass)
                mass=1/mass;
            btVector3 inertia(0,0,0);
            Shape->calculateLocalInertia(mass, inertia);
            this->physrigidbody->setMassProps(mass,inertia);
            this->physrigidbody->updateInertiaTensor();
            return;
        }
        if(Accuracy==3)
        {
            if(!ShapeIsSaved)
            {
                delete Shape;
            }
            int depth=7;
            float cpercent=5;
            float ppercent=10;
            Shape = internal::MeshTools::PerformConvexDecomposition(GraphicsObject,depth,cpercent,ppercent,UseAllSubmeshes);
            ShapeIsSaved = false;
            this->physrigidbody->setCollisionShape(this->Shape);

            btScalar mass=this->physrigidbody->getInvMass();
            if(0 != mass)
                mass=1/mass;
            btVector3 inertia(0,0,0);
            Shape->calculateLocalInertia(mass, inertia);
            this->physrigidbody->setMassProps(mass,inertia);
            this->physrigidbody->updateInertiaTensor();
            return;
        }
        if(Accuracy==4)
        {
            if(!ShapeIsSaved)
            {
                delete Shape;
            }
            btGImpactMeshShape* gimpact = new btGImpactMeshShape(internal::MeshTools::CreateBulletTrimesh(GraphicsObject,UseAllSubmeshes));
            btScalar mass=this->physrigidbody->getInvMass();
            if(0 != mass)
                mass=1/mass;
            btVector3 inertia(0,0,0);
            gimpact->calculateLocalInertia(mass, inertia);
            gimpact->setLocalScaling(btVector3(1.f,1.f,1.f));
            gimpact->setMargin(0.04);
            gimpact->updateBound();
            Shape=gimpact;
            ShapeIsSaved = false;
            this->physrigidbody->setCollisionShape(this->Shape);
            this->physrigidbody->setMassProps(mass,inertia);
            this->physrigidbody->updateInertiaTensor();
            return;
        }
        return;
    }

    void ActorRigid::CreateSphereShapeFromMesh()
    {
        Vector3 test(this->GraphicsObject->getMesh()->getBounds().getSize());
        if(test.X==test.Y && test.Y==test.Z)
        {
            if(!ShapeIsSaved)
            {
                delete Shape;
            }
            Real radius=test.X*0.5;
            btSphereShape* sphereshape = new btSphereShape(radius);
            btScalar mass=this->physrigidbody->getInvMass();
            if(0 != mass)
                mass=1/mass;
            btVector3 inertia(0,0,0);
            sphereshape->calculateLocalInertia(mass, inertia);
            Shape = sphereshape;
            ShapeIsSaved = false;
            this->Shape->setLocalScaling(btVector3(1.f,1.f,1.f));
            this->physrigidbody->setCollisionShape(this->Shape);
            this->physrigidbody->setMassProps(mass,inertia);
            this->physrigidbody->updateInertiaTensor();
            return;
        }
        else
        {
            return;
        }
    }

    void ActorRigid::CreateShapeFromMeshStatic(bool UseAllSubmeshes)
    {
        if(!ShapeIsSaved)
        {
            delete Shape;
        }
        /// @todo - Check for thread safety
        btBvhTriangleMeshShape *tmpshape = new btBvhTriangleMeshShape(internal::MeshTools::CreateBulletTrimesh(GraphicsObject,UseAllSubmeshes),true);
        this->Shape=tmpshape;
        ShapeIsSaved = false;
        //this->Shape->setLocalScaling(btVector3(1.0,1.0,1.0));
        this->physrigidbody->setCollisionShape(this->Shape);
        this->physrigidbody->updateInertiaTensor();
    }

    ActorRigidPhysicsSettings* ActorRigid::GetPhysicsSettings()
    {
        return PhysicsSettings;
    }

    void ActorRigid::LimitMovementOnAxis(bool x, bool y, bool z)
    {
        btVector3 LinFact(x,y,z);
        this->physrigidbody->setLinearFactor(LinFact);
        return;
    }

    void ActorRigid::AddObjectToWorld(World *TargetWorld)
    {
        TargetWorld->GetPhysicsManager()->GetPhysicsWorldPointer()->addRigidBody(this->physrigidbody);
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
}
#endif
