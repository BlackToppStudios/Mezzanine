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

///////////////////////////////////////////////////////////////////////////////
/// @class PhysMotionState
/// @headerfile physactor.h
/// @brief This class is used by the actor class to sync between the physics world and the graphical world.
/// @details This class provides the link for position and orientation between the two worlds in the engine.
/// This is called on every step(frame) of the world to sync the actor if it has moved.
///////////////////////////////////////

class PhysMotionState : public btMotionState {
    private:
        friend class ActorBase;
        Ogre::SceneNode* snode;
        btTransform initposition;

    public:
        /// @brief Blank Constructor.
        /// @details Basic no-initialization constructor.
        PhysMotionState();
        /// @brief Constructor.
        /// @details The class constructor.
        /// @param Scenenode The scenenode belonging to the actor.
        PhysMotionState(Ogre::SceneNode* scenenode);
        /// @brief Destructor.
        /// @details The class destructor.
        virtual ~PhysMotionState();
        /// @brief Sets the scenenode.
        /// @details Sets the scenenode to be sync'd every step.
        /// @param Scenenode The scenenode belonging to the actor.
        void SetNode(Ogre::SceneNode* scenenode);
        /// @brief Sets the initial position.
        /// @details Sets the position the actor will be placed in when it is added to the world.
        /// This function is called on by the ActorBase function SetInitPosition().
        /// @param Position The vector3 representing the location to be used.
        void SetPosition(PhysVector3 position);
        /// @brief Sets the initial orientation.
        /// @details Sets the orientation the actor will have when it is added to the world.
        /// This function is called on by the ActorBase function SetInitOrientation().
        /// @param Orientation The vector3 representing the orientation to be used.
        void SetOrientation(PhysQuaternion orientation);

        /// @brief Sets the initial position.
        /// @details This function is called on by the physics world upon adding the actor to the world.
        /// This function uses the previous set vector3 that was set with SetInitPosition(). @n
        /// Default position is (0,0,0).
        /// @param WorldTrans The location and orientation data.
        virtual void getWorldTransform(btTransform &worldTrans) const;
        /// @brief Updates the position and orientation.
        /// @details This function is called each step(frame) by the physics world to sync the physics and graphical worlds.
        /// @param WorldTrans The location and orientation data.
        virtual void setWorldTransform(const btTransform &worldTrans);
};

///////////////////////////////////
// ActorBase class fuctions

ActorBase::ActorBase (PhysString name, PhysString file, PhysString group, PhysWorld* World)
{
    this->GameWorld = World;
    this->node = this->GameWorld->OgreSceneManager->createSceneNode();
    this->GameWorld->OgreSceneManager->getRootSceneNode()->addChild(this->node);
    this->MotionState = new PhysMotionState(this->node);
    this->Shape = new btEmptyShape();
    this->CreateEntity(name, file, group);
}

ActorBase::~ActorBase ()
{
    delete MotionState;
    delete Shape;
}

void ActorBase::CreateTrimesh()
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

    // --------------- Cleanup

    delete[] vertices;
    delete[] indices;

    ///TODO - Check for thread safety
    Shape = new btBvhTriangleMeshShape(trimesh, true);
}

void ActorBase::CreateEntity (PhysString name, PhysString file, PhysString group)
{
    this->entity = this->GameWorld->OgreSceneManager->createEntity(name, file, group);
}

void ActorBase::CreateSceneNode ()
{
    this->node = this->GameWorld->OgreSceneManager->createSceneNode();
}

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
    btTransform temp = this->CollisionObject->getWorldTransform();
    temp.setOrigin(Location.GetBulletVector3());
}

PhysVector3 ActorBase::GetBulletLocation()
{
    PhysVector3 temp;
    btTransform trans = this->CollisionObject->getWorldTransform();
    temp.ExtractBulletVector3(trans.getOrigin());
    return temp;
}

void ActorBase::SetBulletInitLocation (PhysVector3 Location)
{
    this->MotionState->initposition.setOrigin(Location.GetBulletVector3());
}

void ActorBase::SetOgreOrientation (PhysQuaternion Rotation)
{
    this->node->setOrientation(Rotation.GetOgreQuaternion());
}

void ActorBase::SetBulletOrientation (PhysQuaternion Rotation)
{
    btTransform temp = this->CollisionObject->getWorldTransform();
    temp.setRotation(Rotation.GetBulletQuaternion());
}

void ActorBase::SetLocation (PhysReal x, PhysReal y, PhysReal z)
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
    this->SetBulletInitLocation(Location);
}

void ActorBase::SetInitOrientation(PhysQuaternion Orientation)
{
    this->MotionState->SetOrientation(Orientation);
}

void ActorBase::SetOrientation (PhysReal x, PhysReal y, PhysReal z, PhysReal w)
{
    PhysQuaternion temp(x,y,z,w);
    this->SetOrientation(temp);
}

void ActorBase::SetOrientation (PhysQuaternion Rotation)
{
    this->SetBulletOrientation(Rotation);
    this->SetOgreOrientation(Rotation);
}

void ActorBase::AttachToGraphics ()
{
    PhysVector3 temp;
    temp.ExtractBulletVector3(this->MotionState->initposition.getOrigin());
    this->node->setPosition(temp.GetOgreVector3());
    this->node->attachObject(this->entity);
}

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

///////////////////////////////////
// ActorRigid class functions

ActorRigid::ActorRigid (PhysReal mass, PhysString name, PhysString file, PhysString group, PhysWorld* World) : ActorBase (name, file, group, World)
{
    this->CreateRigidObject(mass);
}

ActorRigid::~ActorRigid ()
{
    delete physrigidbody;
}

void ActorRigid::CreateRigidObject (PhysReal pmass)
{
    btScalar bmass=pmass;
    this->physrigidbody = new btRigidBody (bmass, this->MotionState, this->Shape);
    CollisionObject=physrigidbody;
}

void ActorRigid::AddObjectToWorld (PhysWorld *TargetWorld, btSoftRigidDynamicsWorld* World)
{
    World->addRigidBody(this->physrigidbody);
    AttachToGraphics();
}

void ActorRigid::CreateShapeFromMesh()
{
    this->CreateTrimesh();
    this->physrigidbody->setCollisionShape(this->Shape);
}

///////////////////////////////////
// ActorSoft class functions

/*ActorSoft::ActorSoft ()
{
}*/

ActorSoft::~ActorSoft ()
{
    delete physsoftbody;
}

void ActorSoft::CreateSoftObject (btSoftBodyWorldInfo* softworldinfo, int nodecount, btVector3* nodearray, btScalar* massarray)
{
    this->physsoftbody = new btSoftBody (softworldinfo, nodecount, nodearray, massarray);
    CollisionObject=physsoftbody;
}

void ActorSoft::AddObjectToWorld (PhysWorld *TargetWorld, btSoftRigidDynamicsWorld* World)
{
    World->addSoftBody(this->physsoftbody);
}

void ActorSoft::CreateShapeFromMesh()
{
    this->CreateTrimesh();
    this->physsoftbody->setCollisionShape(this->Shape);
}

///////////////////////////////////
// PhysMotionState

PhysMotionState::PhysMotionState()
{
    this->initposition.setIdentity();
}

PhysMotionState::PhysMotionState(Ogre::SceneNode* scenenode)
{
    this->snode=scenenode;
    this->initposition.setIdentity();
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
    this->snode=scenenode;
}

void PhysMotionState::SetPosition(PhysVector3 position)
{
    this->initposition.setOrigin(position.GetBulletVector3());
}

void PhysMotionState::SetOrientation(PhysQuaternion orientation)
{
    this->initposition.setRotation(orientation.GetBulletQuaternion());
}

void PhysMotionState::getWorldTransform(btTransform &worldTrans) const
{
    worldTrans=initposition;
}

void PhysMotionState::setWorldTransform(const btTransform &worldTrans)
{
    btQuaternion rotation = worldTrans.getRotation();
    this->snode->setOrientation(rotation.w(), rotation.x(), rotation.y(), rotation.z());
    btVector3 position = worldTrans.getOrigin();
    this->snode->setPosition(position.x(), position.y(), position.z());
}

#endif
