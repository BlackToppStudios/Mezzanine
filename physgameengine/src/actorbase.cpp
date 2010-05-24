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
#ifndef _physactorbase_cpp
#define _physactorbase_cpp

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
//#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
//#include "BulletCollision/CollisionShapes/btShapeHull.h"
//#include "BulletCollision/Gimpact/btGImpactShape.h"

#include "actorbase.h"
#include "internalmotionstate.h.cpp"

namespace phys{
    ///////////////////////////////////
    // ActorBase class fuctions
    ActorBase::ActorBase (String name, String file, String group, World* _World)
    {
        this->GameWorld = _World;
        this->node = this->GameWorld->OgreSceneManager->createSceneNode();
        this->GameWorld->OgreSceneManager->getRootSceneNode()->addChild(this->node);
        this->MotionState = new internal::PhysMotionState(this->node);
        this->Shape = new btEmptyShape();
        this->CreateEntity(name, file, group);
    }

    ActorBase::~ActorBase ()
    {
        delete MotionState;
        delete Shape;
    }

    ///////////////////////////////////
    // ActorBase Private misc functions
    btTriangleMesh* ActorBase::CreateTrimesh()
    {
        // Get the mesh from the entity
        Ogre::MeshPtr myMesh = entity->getMesh();

        // Get the submesh and associated data
        Ogre::SubMesh* subMesh = myMesh->getSubMesh(0);
        Ogre::IndexData*  indexData = subMesh->indexData;
        Ogre::VertexData* vertexData = subMesh->vertexData;

        // Get the position element
        const Ogre::VertexElement* posElem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

        // Get a pointer to the vertex buffer
        Ogre::HardwareVertexBufferSharedPtr vBuffer = vertexData->vertexBufferBinding->getBuffer(posElem->getSource());

        // Get a pointer to the index buffer
        Ogre::HardwareIndexBufferSharedPtr iBuffer = indexData->indexBuffer;

        // Get the number of triangles
        unsigned int triCount = indexData->indexCount/3;

        // Allocate space for the vertices and indices
        Ogre::Vector3* vertices = new Ogre::Vector3[vertexData->vertexCount];
        unsigned long* indices  = new unsigned long[indexData->indexCount];

        // Lock the vertex buffer (READ ONLY)
        unsigned char* vertex =   static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        float* pReal = NULL;
        for (size_t j = 0; j < vertexData->vertexCount; ++j, vertex += vBuffer->getVertexSize() )
        {
            posElem->baseVertexPointerToElement(vertex, &pReal);
            Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
            vertices[j] = pt;
        }
        vBuffer->unlock();
        size_t index_offset = 0;
        bool use32bitindexes = (iBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

        // Lock the index buffer (READ ONLY)
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

        // We now have vertices and indices ready to go

        // The Bullet triangle mesh
        btTriangleMesh* trimesh = new btTriangleMesh(use32bitindexes);

        // Setup the tri mesh
        btVector3 vert0, vert1, vert2;
        int i=0;

        // For every triangle
        for (unsigned int y=0; y<triCount; y++)
        {
            // Set each vertex
            vert0.setValue(vertices[indices[i]].x, vertices[indices[i]].y, vertices[indices[i]].z);
            vert1.setValue(vertices[indices[i+1]].x, vertices[indices[i+1]].y, vertices[indices[i+1]].z);
            vert2.setValue(vertices[indices[i+2]].x, vertices[indices[i+2]].y, vertices[indices[i+2]].z);

            // Add it into the trimesh
            trimesh->addTriangle(vert0, vert1, vert2);

            // Increase index count
            i+=3;
        }
        delete[] vertices;
        delete[] indices;

        return trimesh;
    }

    ///////////////////////////////////
    // ActorBase Constructor functions

    void ActorBase::CreateEntity (String name, String file, String group)
    {
        this->entity = this->GameWorld->OgreSceneManager->createEntity(name, file, group);
    }

    void ActorBase::CreateSceneNode ()
    {
        this->node = this->GameWorld->OgreSceneManager->createSceneNode();
    }

    ///////////////////////////////////
    // ActorBase Private Location Functions

    void ActorBase::SetOgreLocation (PhysVector3 Location)
    {
        this->node->setPosition(Location.GetOgreVector3());
    }

    PhysVector3 ActorBase::GetOgreLocation()
    {
        PhysVector3 temp;
        temp.ExtractOgreVector3(this->node->getPosition());
        return temp;
    }

    void ActorBase::SetBulletLocation (PhysVector3 Location)
    {
        //btTransform* temp = this->CollisionObject->getWorldTransform();
        this->CollisionObject->getWorldTransform().setOrigin(Location.GetBulletVector3());
    }

    PhysVector3 ActorBase::GetBulletLocation()
    {
        PhysVector3 temp;
        btTransform trans = this->CollisionObject->getWorldTransform();
        temp.ExtractBulletVector3(trans.getOrigin());
        return temp;
    }

    ///////////////////////////////////
    // ActorBase Private Orientation functions

    void ActorBase::SetOgreOrientation (Quaternion Rotation)
    {
        this->node->setOrientation(Rotation.GetOgreQuaternion());
    }

    void ActorBase::SetBulletOrientation (Quaternion Rotation)
    {
        btTransform temp = this->CollisionObject->getWorldTransform();
        temp.setRotation(Rotation.GetBulletQuaternion());
    }

    ///////////////////////////////////
    // ActorBase Public Location functions

    void ActorBase::SetLocation (Real x, Real y, Real z)
    {
        PhysVector3 temp(x,y,z);
        this->SetLocation(temp);
    }

    void ActorBase::SetLocation (PhysVector3 Place)
    {
        this->SetBulletLocation(Place);
        this->SetOgreLocation(Place);
    }

    PhysVector3 ActorBase::GetLocation()
    {
        return this->GetBulletLocation();
    }

    void ActorBase::SetInitLocation(PhysVector3 Location)
    {
        this->SetBulletLocation(Location);
    }

    ///////////////////////////////////
    // ActorBase Public Orientation functions

    void ActorBase::SetInitOrientation(Quaternion Orientation)
    {
        this->SetBulletOrientation(Orientation);
    }

    void ActorBase::SetOrientation (Real x, Real y, Real z, Real w)
    {
        Quaternion temp(x,y,z,w);
        this->SetOrientation(temp);
    }

    void ActorBase::SetOrientation (Quaternion Rotation)
    {
        this->SetBulletOrientation(Rotation);
        this->SetOgreOrientation(Rotation);
    }

    ///////////////////////////////////
    // ActorBase Public Misc Functions

    void ActorBase::AttachToGraphics ()
    {
        PhysVector3 temp;
        //temp.ExtractBulletVector3(this->MotionState->initposition.getOrigin());
        temp = GetBulletLocation();
        this->node->setPosition(temp.GetOgreVector3());
        this->node->attachObject(this->entity);
    }

    ///////////////////////////////////
    // ActorBase Public Collision flag functions
    void ActorBase::SetKinematic()
    {
        int x=2;
        this->CollisionObject->setCollisionFlags(x);
    }

    void ActorBase::SetStatic()
    {
        int x=1;
        this->CollisionObject->setCollisionFlags(x);
    }
}// /phys
#endif
