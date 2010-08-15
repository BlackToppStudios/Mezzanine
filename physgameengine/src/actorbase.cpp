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
#ifndef _physactorbase_cpp
#define _physactorbase_cpp

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"

#include "actorbase.h"
#include "internalmotionstate.h.cpp"
#include "internalmeshinfo.h.cpp"

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
        ActorSounds = NULL;
        Animation = NULL;
        ShapeIsSaved = false;
        ActorType = ActorBase::Actorbase;
    }

    ActorBase::~ActorBase ()
    {
        delete MotionState;
        if(!ShapeIsSaved)
        {
            delete Shape;
        };
        delete node;
        delete entity;
        if(CollisionObject)
        {
            delete CollisionObject;
        }
    }

    ///////////////////////////////////
    // ActorBase Private misc functions
    btTriangleMesh* ActorBase::CreateTrimesh() const
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

    void ActorBase::GetMeshVerticies (internal::MeshInfo &TheMesh)
    {
        Ogre::MeshPtr myMesh = entity->getMesh();
        Ogre::SubMesh* subMesh = myMesh->getSubMesh(0);
        Ogre::VertexData* vertexData = subMesh->vertexData;

        const Ogre::VertexElement* posElem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
        Ogre::HardwareVertexBufferSharedPtr vBuffer = vertexData->vertexBufferBinding->getBuffer(posElem->getSource());

        TheMesh.Verticies = new Ogre::Vector3[vertexData->vertexCount];
        TheMesh.VCount = vertexData->vertexCount;

        unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        float* pReal = NULL;
        for (size_t j = 0; j < vertexData->vertexCount; j++, vertex += vBuffer->getVertexSize() )
        {
            posElem->baseVertexPointerToElement(vertex, &pReal);
            TheMesh.Verticies[j].x = *pReal++;
            TheMesh.Verticies[j].y = *pReal++;
            TheMesh.Verticies[j].z = *pReal++;
        }
        vBuffer->unlock();
    }

    void ActorBase::GetMeshIndicies (internal::MeshInfo &TheMesh)
    {
        Ogre::MeshPtr myMesh = entity->getMesh();
        Ogre::SubMesh* subMesh = myMesh->getSubMesh(0);
        Ogre::IndexData*  indexData = subMesh->indexData;

        Ogre::HardwareIndexBufferSharedPtr iBuffer = indexData->indexBuffer;

        TheMesh.Indicies = new int[indexData->indexCount];
        TheMesh.ICount = indexData->indexCount;

        size_t index_offset = 0;
        bool use32bitindexes = (iBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

        long* pLong = static_cast<long*>(iBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        short* pShort = reinterpret_cast<short*>(pLong);

        if (use32bitindexes)
        {
            for (size_t k = 0; k < indexData->indexCount; ++k)
            {
                TheMesh.Indicies[index_offset++] = pLong[k];
            }
        }
        else
        {
            for (size_t k = 0; k < indexData->indexCount; ++k)
            {
                TheMesh.Indicies[index_offset++] = static_cast<unsigned long>(pShort[k]);
            }
        }
        iBuffer->unlock();
    }

    void ActorBase::GetMeshNormals (internal::MeshInfo &TheMesh)
    {
        Ogre::MeshPtr myMesh = entity->getMesh();
        Ogre::SubMesh* subMesh = myMesh->getSubMesh(0);
        Ogre::VertexData* vertexData = subMesh->vertexData;

        const Ogre::VertexElement* posElem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_NORMAL);
        Ogre::HardwareVertexBufferSharedPtr vBuffer = vertexData->vertexBufferBinding->getBuffer(posElem->getSource());

        TheMesh.Normals = new Ogre::Vector3[vertexData->vertexCount];

        unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        float* pReal = NULL;
        for (size_t j = 0; j < vertexData->vertexCount; j++, vertex += vBuffer->getVertexSize() )
        {
            posElem->baseVertexPointerToElement(vertex, &pReal);
            TheMesh.Normals[j].x = *pReal++;
            TheMesh.Normals[j].y = *pReal++;
            TheMesh.Normals[j].z = *pReal++;
        }
        vBuffer->unlock();
    }

    void ActorBase::GetMeshTextures (internal::MeshInfo &TheMesh)
    {
        Ogre::MeshPtr myMesh = entity->getMesh();
        Ogre::SubMesh* subMesh = myMesh->getSubMesh(0);
        Ogre::VertexData* vertexData = subMesh->vertexData;

        const Ogre::VertexElement* posElem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES);
        Ogre::HardwareVertexBufferSharedPtr vBuffer = vertexData->vertexBufferBinding->getBuffer(posElem->getSource());

        TheMesh.Textures = new Ogre::Vector2[vertexData->vertexCount];

        unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        float* pReal = NULL;
        for (size_t j = 0; j < vertexData->vertexCount; j++, vertex += vBuffer->getVertexSize() )
        {
            posElem->baseVertexPointerToElement(vertex, &pReal);
            TheMesh.Textures[j].x = *pReal++;
            TheMesh.Textures[j].y = *pReal++;
        }
        vBuffer->unlock();
    }

    void ActorBase::GetOtherMeshInfo(internal::MeshInfo &TheMesh)
    {
        // Entity Name
        TheMesh.Name = entity->getName();
        // Material Name
        Ogre::MeshPtr myMesh = entity->getMesh();
        Ogre::SubMesh* subMesh = myMesh->getSubMesh(0);
        TheMesh.Material = subMesh->getMaterialName();
        // Material File Name
        Ogre::ResourcePtr Mat = ActorBase::GetOgreResourceManager()->_getResourceManager("Material")->getByName(TheMesh.Material);
        TheMesh.MaterialFile = Mat.getPointer()->getOrigin();
        // Group Name
        TheMesh.Group = ActorBase::GetOgreResourceManager()->findGroupContainingResource(TheMesh.MaterialFile);
        // Render Operation
        Ogre::RenderOperation Render;
        subMesh->_getRenderOperation(Render);
        TheMesh.RendOp = Render.operationType;
    }

    Ogre::ResourceGroupManager* ActorBase::GetOgreResourceManager()
    {
        return this->GameWorld->OgreResource;
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

    void ActorBase::SetOgreLocation (Vector3 Location)
    {
        this->node->setPosition(Location.GetOgreVector3());
    }

    Vector3 ActorBase::GetOgreLocation() const
    {
        Vector3 temp;
        temp.ExtractOgreVector3(this->node->getPosition());
        return temp;
    }

    void ActorBase::SetBulletLocation (Vector3 Location)
    {
        //btTransform* temp = this->CollisionObject->getWorldTransform();
        this->CollisionObject->getWorldTransform().setOrigin(Location.GetBulletVector3());
    }

    Vector3 ActorBase::GetBulletLocation() const
    {
        Vector3 temp;
        //btTransform trans = this->CollisionObject->getWorldTransform();
        temp.ExtractBulletVector3(this->CollisionObject->getWorldTransform().getOrigin());
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
        this->CollisionObject->getWorldTransform().setRotation(Rotation.GetBulletQuaternion(true));
    }

    ///////////////////////////////////
    // ActorBase Public Location functions

    void ActorBase::SetLocation (Real x, Real y, Real z)
    {
        Vector3 temp(x,y,z);
        this->SetLocation(temp);
    }

    void ActorBase::SetLocation (Vector3 Place)
    {
        this->SetBulletLocation(Place);
        this->SetOgreLocation(Place);
    }

    Vector3 ActorBase::GetLocation() const
    {
        return this->GetBulletLocation();
    }

    void ActorBase::SetInitLocation(Vector3 Location)
    {
        this->SetBulletLocation(Location);
        if ( btCollisionObject::CO_RIGID_BODY == this->CollisionObject->getInternalType() )
        {
            btRigidBody* Rigid = static_cast< btRigidBody* > (this->CollisionObject);
            Rigid->updateInertiaTensor();
        }
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
        Vector3 tempv;
        Quaternion tempq;
        tempv = GetBulletLocation();
        tempq.ExtractBulletQuaternion(CollisionObject->getWorldTransform().getRotation());
        this->node->setPosition(tempv.GetOgreVector3());
        this->node->setOrientation(tempq.GetOgreQuaternion());
        this->node->attachObject(this->entity);
    }

    void ActorBase::DetachFromGraphics ()
    {
        this->node->detachObject(this->entity);
    }

    void ActorBase::SetActorScaling(Vector3 scaling)
    {
        this->node->setScale(scaling.GetOgreVector3());
        this->Shape->setLocalScaling(scaling.GetBulletVector3());
    }

    int ActorBase::GetType()
    {
        return this->ActorType;
    }

    const bool ActorBase::GetShapeIsSaved()
    {
        return ShapeIsSaved;
    }

    void ActorBase::SetBasicCollisionParams(Real Friction, Real Restitution)
    {
        this->CollisionObject->setFriction(Friction);
        this->CollisionObject->setRestitution(Restitution);
    }

    void ActorBase::SetAnimation(String &AnimationName, bool Loop)
    {
        if(this->IsAnimated())
        {
            Animation->setEnabled(false);
        }
        Animation = entity->getAnimationState(AnimationName);
        Animation->setLoop(Loop);
    }

    void ActorBase::EnableAnimation(bool Enable)
    {
        if(Animation)
        {
            Animation->setEnabled(Enable);
        }
    }

    bool ActorBase::IsAnimated()
    {
        if(Animation)
        {
            return Animation->getEnabled();
        }else{
            return false;
        }
    }

    void ActorBase::AdvanceAnimation(Real Time)
    {
        if(Animation)
        {
            Animation->addTime(Time);
        }
    }

    void ActorBase::RemoveSetAnimation()
    {
        if(Animation)
        {
            Animation->setEnabled(false);
            Animation = NULL;
        }
    }

    ///////////////////////////////////
    // ActorBase Public Collision flag functions
    void ActorBase::SetKinematic()
    {
        //int x=2;
        this->CollisionObject->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
    }

    void ActorBase::SetStatic()
    {
        //int x=1;
        this->CollisionObject->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    }

    bool ActorBase::IsStaticOrKinematic()
    {
        //bool result = this->CollisionObject->isStaticOrKinematicObject();
        return this->CollisionObject->isStaticOrKinematicObject();
    }
}// /phys

std::ostream& operator << (std::ostream& stream, const phys::ActorBase& x)
{
    stream << "[" << x.GetName() << " at:" << x.GetLocation() << "]";
    return stream;
}

#endif
