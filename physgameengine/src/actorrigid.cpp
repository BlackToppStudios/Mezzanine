//© Copyright 2010 BlackTopp Studios Inc.
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
#include "ConvexBuilder.h"

#include "actorrigid.h"
#include "internalmotionstate.h.cpp" // This is required for the internal physmotionstate :(
#include "internaldecompinterface.h.cpp"

namespace phys{
    ///////////////////////////////////
    // ActorRigid class functions

    ActorRigid::ActorRigid (Real mass, String name, String file, String group, World* _World) : ActorBase (name, file, group, _World)
    {
        this->CreateMotionState(this->node);
        this->CreateRigidObject(mass);
        ActorType=ActorBase::Actorrigid;
    }

    ActorRigid::~ActorRigid ()
    {
        delete physrigidbody;
        CollisionObject = NULL;
    }

    void ActorRigid::CreateRigidObject (Real pmass)
    {
        btScalar bmass=pmass;
        this->physrigidbody = new btRigidBody (bmass, this->MotionState, this->Shape);
        CollisionObject=physrigidbody;
        if(0.0 == bmass)
        {
            CollisionObject->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
        }
    }

    void ActorRigid::PerformConvexDecomposition(unsigned int depth, float cpercent, float ppercent)
    {
        Ogre::MeshPtr myMesh = entity->getMesh();
        Ogre::SubMesh* subMesh = myMesh->getSubMesh(0);
        Ogre::IndexData*  indexData = subMesh->indexData;
        Ogre::VertexData* vertexData = subMesh->vertexData;

        const Ogre::VertexElement* posElem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
        Ogre::HardwareVertexBufferSharedPtr vBuffer = vertexData->vertexBufferBinding->getBuffer(posElem->getSource());
        Ogre::HardwareIndexBufferSharedPtr iBuffer = indexData->indexBuffer;

        unsigned int triCount = indexData->indexCount/3;
        Ogre::Vector3* vertices = new Ogre::Vector3[vertexData->vertexCount];
        unsigned int* indices = new unsigned int[indexData->indexCount];
        unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        float* pReal = NULL;
        for (size_t j = 0; j < vertexData->vertexCount; j++, vertex += vBuffer->getVertexSize() )
        {
            posElem->baseVertexPointerToElement(vertex, &pReal);
            vertices[j].x = *pReal++;
            vertices[j].y = *pReal++;
            vertices[j].z = *pReal++;
        }
        vBuffer->unlock();
        size_t index_offset = 0;
        bool use32bitindexes = (iBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

        unsigned long* pLong = static_cast<unsigned long*>(iBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

        if (use32bitindexes)
        {
            for (size_t k = 0; k < triCount*3; ++k)
            {
                indices[index_offset++] = pLong[k];
            }
        }
        else
        {
            for (size_t k = 0; k < triCount*3; ++k)
            {
                indices[index_offset++] = static_cast<unsigned long>(pShort[k]);
            }
        }
        iBuffer->unlock();

        ConvexDecomposition::DecompDesc desc;
        desc.mVcount = vertexData->vertexCount;
        desc.mTcount = triCount;
        desc.mVertices = &vertices[0].x;
        desc.mIndices = &indices[0];
        unsigned int maxv  = 16;
        float skinWidth    = 0.0;
        desc.mDepth        = depth;
		desc.mCpercent     = cpercent;
		desc.mPpercent     = ppercent;
		desc.mMaxVertices  = maxv;
		desc.mSkinWidth    = skinWidth;

        internal::PhysConvexDecomposition decomp;
        desc.mCallback = &decomp;

        ConvexBuilder cb(desc.mCallback);
        cb.process(desc);

        btCompoundShape* compound = new btCompoundShape(false);
        btTransform trans;
        trans.setIdentity();
        for (int i=0;i<decomp.m_convexShapes.size();i++)
        {
            btVector3 centroid = decomp.m_convexCentroids[i];
            trans.setOrigin(centroid);
            btConvexHullShape* convexShape = decomp.m_convexShapes[i];
            compound->addChildShape(trans,convexShape);
        }
        Shape=compound;
        ShapeIsSaved = false;
        this->physrigidbody->setCollisionShape(this->Shape);

        delete[] vertices;
        delete[] indices;

        return;
    }

    void ActorRigid::AddObjectToWorld (World *TargetWorld)
    {
        TargetWorld->GetPhysicsManager()->GetPhysicsWorldPointer()->addRigidBody(this->physrigidbody);
        //TargetWorld->Physics->BulletDynamicsWorld->addRigidBody(this->physrigidbody);
        this->AttachToGraphics();
    }

    void ActorRigid::RemoveObjectFromWorld(World* TargetWorld)
    {
        TargetWorld->GetPhysicsManager()->GetPhysicsWorldPointer()->removeRigidBody(this->physrigidbody);
        //TargetWorld->Physics->BulletDynamicsWorld->removeRigidBody(this->physrigidbody);
        this->DetachFromGraphics();
    }

    void ActorRigid::CreateShapeFromMeshDynamic(short unsigned int accuracy)
    {
        if(accuracy==1)
        {
            if(!ShapeIsSaved)
            {
                delete Shape;
            }
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
            ShapeIsSaved = false;
            this->Shape->setLocalScaling(btVector3(1.0,1.0,1.0));
            this->physrigidbody->setCollisionShape(this->Shape);
            this->physrigidbody->setMassProps(mass,inertia);
            this->physrigidbody->updateInertiaTensor();
            return;
        }
        if(accuracy==2)
        {
            if(!ShapeIsSaved)
            {
                delete Shape;
            }
            int depth=5;
            float cpercent=5;
            float ppercent=15;
            this->PerformConvexDecomposition(depth,cpercent,ppercent);

            btScalar mass=this->physrigidbody->getInvMass();
            mass=1/mass;
            btVector3 inertia(0,0,0);
            Shape->calculateLocalInertia(mass, inertia);
            this->physrigidbody->setMassProps(mass,inertia);
            this->physrigidbody->updateInertiaTensor();
            return;
        }
        if(accuracy==3)
        {
            if(!ShapeIsSaved)
            {
                delete Shape;
            }
            int depth=7;
            float cpercent=5;
            float ppercent=10;
            this->PerformConvexDecomposition(depth,cpercent,ppercent);

            btScalar mass=this->physrigidbody->getInvMass();
            mass=1/mass;
            btVector3 inertia(0,0,0);
            Shape->calculateLocalInertia(mass, inertia);
            this->physrigidbody->setMassProps(mass,inertia);
            this->physrigidbody->updateInertiaTensor();
            return;
        }
        if(accuracy==4)
        {
            if(!ShapeIsSaved)
            {
                delete Shape;
            }
            btGImpactMeshShape* gimpact = new btGImpactMeshShape(this->CreateTrimesh());
            btScalar mass=this->physrigidbody->getInvMass();
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
        Vector3 test;
        test.ExtractOgreVector3(this->entity->getMesh()->getBounds().getSize());
        if(test.X==test.Y && test.Y==test.Z)
        {
            if(!ShapeIsSaved)
            {
                delete Shape;
            }
            Real radius=test.X*0.5;
            btSphereShape* sphereshape = new btSphereShape(radius);
            btScalar mass=this->physrigidbody->getInvMass();
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

    void ActorRigid::CreateShapeFromMeshStatic()
    {
        if(!ShapeIsSaved)
        {
            delete Shape;
        }
        /// @todo - Check for thread safety
        btBvhTriangleMeshShape *tmpshape = new btBvhTriangleMeshShape(this->CreateTrimesh(),true);
        this->Shape=tmpshape;
        ShapeIsSaved = false;
        this->Shape->setLocalScaling(btVector3(1.0,1.0,1.0));
        this->physrigidbody->setCollisionShape(this->Shape);
        this->physrigidbody->updateInertiaTensor();
    }

    std::string ActorRigid::GetName () const
    {
        return this->entity->getName();
    }

    void ActorRigid::LimitMovementOnAxis(bool x, bool y, bool z)
    {
        btVector3 LinFact(x,y,z);
        this->physrigidbody->setLinearFactor(LinFact);
        return;
    }

    void ActorRigid::DisableDeactivation()
    {
        this->physrigidbody->setActivationState(DISABLE_DEACTIVATION);
    }
}
#endif
