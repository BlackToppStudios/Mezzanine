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
#ifndef _physactorrigid_cpp
#define _physactorrigid_cpp

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"

#include "actorrigid.h"
#include "internalmotionstate.h.cpp" // This is required for the internal physmotionstate :(

namespace phys{
    ///////////////////////////////////
    // ActorRigid class functions

    ActorRigid::ActorRigid (Real mass, String name, String file, String group, World* _World) : ActorBase (name, file, group, _World)
    {
        this->CreateRigidObject(mass);
    }

    ActorRigid::~ActorRigid ()
    {
        delete physrigidbody;
    }

    void ActorRigid::CreateRigidObject (Real pmass)
    {
        btScalar bmass=pmass;
        this->physrigidbody = new btRigidBody (bmass, this->MotionState, this->Shape);
        CollisionObject=physrigidbody;
    }

    void ActorRigid::AddObjectToWorld (World *TargetWorld, btSoftRigidDynamicsWorld* btWorld)
    {
        btWorld->addRigidBody(this->physrigidbody);
        AttachToGraphics();
    }

    void ActorRigid::CreateShapeFromMeshDynamic(short unsigned int accuracy)
    {
        if(accuracy==1)
        {
            delete Shape;
            /// @todo - Check for thread safety
            btConvexShape *tmpshape = new btConvexTriangleMeshShape(this->CreateTrimesh());
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
            mass=1/mass;
            btVector3 inertia(0,0,0);
            convexShape->calculateLocalInertia(mass, inertia);
            Shape = convexShape;
            this->Shape->setLocalScaling(btVector3(0.95,0.95,0.95));
            this->physrigidbody->setCollisionShape(this->Shape);
            this->physrigidbody->setMassProps(mass,inertia);
            return;
        }
        if(accuracy==2)
        {
            delete Shape;
            /// @todo add code here to increase the verticies that is used to create the hull from the first accuracy setting.
            btConvexShape *tmpshape = new btConvexTriangleMeshShape(this->CreateTrimesh());
            Shape=tmpshape;
            this->physrigidbody->setCollisionShape(this->Shape);
            return;
        }
        if(accuracy==3)
        {
            delete Shape;
            /// @todo add code here for compound shapes of convex hulls
            this->physrigidbody->setCollisionShape(this->Shape);
            return;
        }
        if(accuracy==4)
        {
            delete Shape;
            btGImpactMeshShape* gimpact = new btGImpactMeshShape(this->CreateTrimesh());
            btScalar mass=this->physrigidbody->getInvMass();
            mass=1/mass;
            btVector3 inertia(0,0,0);
            gimpact->calculateLocalInertia(mass, inertia);
            gimpact->setLocalScaling(btVector3(1.f,1.f,1.f));
            gimpact->setMargin(0.04);
            gimpact->updateBound();
            Shape=gimpact;
            this->physrigidbody->setCollisionShape(this->Shape);
            this->physrigidbody->setMassProps(mass,inertia);
            return;
        }
        return;
    }

    void ActorRigid::CreateShapeFromMeshStatic()
    {
        delete Shape;

        /// @todo - Check for thread safety
        btBvhTriangleMeshShape *tmpshape = new btBvhTriangleMeshShape(this->CreateTrimesh(),true);
        this->Shape=tmpshape;
        this->Shape->setLocalScaling(btVector3(0.95,0.95,0.95));
        this->physrigidbody->setCollisionShape(this->Shape);
    }

    void ActorRigid::LimitMovementOnAxis(bool x, bool y, bool z)
    {
        btVector3 LinFact(x,y,z);
        this->physrigidbody->setLinearFactor(LinFact);
        return;
    }
}
#endif
