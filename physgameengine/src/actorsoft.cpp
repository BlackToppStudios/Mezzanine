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
#ifndef _physactorsoft_cpp
#define _physactorsoft_cpp

#include <Ogre.h>
//#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
//#include "BulletCollision/CollisionShapes/btShapeHull.h"
//#include "BulletCollision/Gimpact/btGImpactShape.h"

#include "internalmeshtools.h.cpp"
#include "objectreference.h"
#include "world.h"
#include "physicsmanager.h"
#include "actorsoft.h"
namespace phys{
    ///////////////////////////////////
    // ActorSoft class functions

    ActorSoft::ActorSoft (Real mass, String name, String file, String group)
        : ActorBase (name, file, group)
    {
        CreateSoftObject(mass);
        ActorType=ActorBase::Actorsoft;
    }

    ActorSoft::~ActorSoft ()
    {
        delete physsoftbody;
    }

    void ActorSoft::CreateSoftObject (Real mass)
    {
        //this->GraphicsObject->getMesh()->getSubMesh(0)->useSharedVertices = false;
        internal::MeshInfo CurMesh;
        internal::MeshTools::GetMeshVerticies(GraphicsObject,CurMesh);
        internal::MeshTools::GetMeshIndicies(GraphicsObject,CurMesh);
        internal::MeshTools::GetMeshNormals(GraphicsObject,CurMesh);
        internal::MeshTools::GetMeshTextures(GraphicsObject,CurMesh);
        internal::MeshTools::GetOtherMeshInfo(GraphicsObject,CurMesh);

        this->physsoftbody = btSoftBodyHelpers::CreateFromTriMesh(this->GameWorld->GetPhysicsManager()->GetPhysicsWorldPointer()->getWorldInfo(), &CurMesh.Verticies[0].x, &CurMesh.Indicies[0], CurMesh.ICount/3);
        CollisionObject=physsoftbody;
        ObjectReference* ActorRef = new ObjectReference(phys::WOT_ActorSoft,this);
        CollisionObject->setUserPointer(ActorRef);
        Shape = physsoftbody->getCollisionShape();
        physsoftbody->setTotalMass(mass, true);
        physsoftbody->m_cfg.collisions = /*btSoftBody::fCollision::CL_SS +*/ btSoftBody::fCollision::CL_RS;
        physsoftbody->m_cfg.piterations = 5;
        physsoftbody->randomizeConstraints();
        physsoftbody->generateBendingConstraints(2);
        physsoftbody->generateClusters(20);

        CreateManualMesh(CurMesh);

        this->GraphicsObject = this->GameWorld->GetSceneManager()->GetGraphicsWorldPointer()->createEntity(CurMesh.Name, CurMesh.Name + "M", CurMesh.Group);
        Ogre::Any OgreRef(ActorRef);
        GraphicsObject->setUserAny(OgreRef);

        this->physsoftbody->m_clusters[0]->m_collide = true;
    }

    void ActorSoft::CreateManualMesh (internal::MeshInfo &TheMesh)
    {
        Ogre::ManualObject* ManualEntity = new Ogre::ManualObject(TheMesh.Name);
        ManualEntity->setDynamic(true);
        ManualEntity->estimateVertexCount(TheMesh.VCount);
        ManualEntity->estimateIndexCount(TheMesh.ICount);

        ManualEntity->begin(TheMesh.Material, TheMesh.RendOp, TheMesh.Group);
        for( int x=0 ; x < TheMesh.VCount ; x++ )
        {
            ManualEntity->position(TheMesh.Verticies[x]);
            ManualEntity->normal(TheMesh.Normals[x]);
            ManualEntity->textureCoord(TheMesh.Textures[x]);
        }
        for( int y=0 ; y < TheMesh.ICount ; y++ )
        {
            ManualEntity->index(TheMesh.Indicies[y]);
        }
        ManualEntity->end();
        ManualEntity->convertToMesh(TheMesh.Name + "M", TheMesh.Group);
        delete ManualEntity;
    }

    void ActorSoft::AttachToGraphics ()
    {
        /*Vector3 tempv;
        Quaternion tempq;
        btTransform temp = this->physsoftbody->m_clusters[0]->m_framexform;
        tempv.ExtractBulletVector3(temp.getOrigin());
        tempq.ExtractBulletQuaternion(temp.getRotation());
        this->GraphicsNode->setPosition(tempv.GetOgreVector3());
        this->GraphicsNode->setOrientation(tempq.GetOgreQuaternion());
        this->GraphicsNode->attachObject(this->GraphicsObject);*/
        ActorBase::AttachToGraphics();
    }

    void ActorSoft::DetachFromGraphics ()
    {
        this->GraphicsNode->detachObject(this->GraphicsObject);
    }

    void ActorSoft::SetBulletLocation (Vector3 Location)
    {
        this->physsoftbody->m_clusters[0]->m_framexform.setOrigin(Location.GetBulletVector3());
        ActorBase::SetBulletLocation(Location);
    }

    Vector3 ActorSoft::GetBulletLocation() const
    {
        Vector3 temp(this->physsoftbody->m_clusters[0]->m_framexform.getOrigin());
        return temp;
        //return ActorBase::GetBulletLocation();
    }

    void ActorSoft::SetBulletOrientation (Quaternion Rotation)
    {
        this->physsoftbody->m_clusters[0]->m_framexform.setRotation(Rotation.GetBulletQuaternion(true));
        ActorBase::SetBulletOrientation(Rotation);
    }

    ///////////////////////////////////////
    // Public Functions

    void ActorSoft::UpdateSoftBody()
    {
        /*Ogre::VertexData* vertexData = entity->getMesh()->getSubMesh(0)->vertexData;

        const Ogre::VertexElement* posElem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
        Ogre::HardwareVertexBufferSharedPtr vBuffer = vertexData->vertexBufferBinding->getBuffer(posElem->getSource());

        unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));
        float* pReal = NULL;
        for (size_t j = 0; j < vertexData->vertexCount; j++ )
        {
            posElem->baseVertexPointerToElement(vertex, &pReal);
            *pReal++ = this->physsoftbody->m_nodes[j].m_x.x();
            *pReal++ = this->physsoftbody->m_nodes[j].m_x.y();
            *pReal++ = this->physsoftbody->m_nodes[j].m_x.z();
            vertex += vBuffer->getVertexSize();
        }
        vBuffer->unlock();// */

        //btVector3 position = this->physsoftbody->getWorldTransform().getOrigin();
        //btQuaternion rotation = this->physsoftbody->getWorldTransform().getRotation();
        btVector3 position = this->physsoftbody->m_clusters[0]->m_framexform.getOrigin();
        btQuaternion rotation = this->physsoftbody->m_clusters[0]->m_framexform.getRotation();
        this->GraphicsNode->setPosition(position.x(), position.y(), position.z());
        this->GraphicsNode->setOrientation(rotation.w(), rotation.x(), rotation.y(), rotation.z());
    }

    std::string ActorSoft::GetName () const
    {
        return this->GraphicsObject->getName();
    }

    void ActorSoft::SetActorScaling(Vector3 scaling)
    {
        this->GraphicsNode->setScale(scaling.GetOgreVector3());
        this->Shape->setLocalScaling(scaling.GetBulletVector3());
    }

    void ActorSoft::SetInitLocation(Vector3 Location)
    {
        this->SetBulletLocation(Location);
        //ActorBase::SetBulletLocation(Location);
        physsoftbody->translate(Location.GetBulletVector3());
        this->physsoftbody->m_initialWorldTransform.setOrigin(Location.GetBulletVector3());
        //this->GraphicsNode->setPosition(Location.GetOgreVector3());
    }

    void ActorSoft::SetInitOrientation(Quaternion Orientation)
    {
        this->SetBulletOrientation(Orientation);
        this->physsoftbody->m_initialWorldTransform.setRotation(Orientation.GetBulletQuaternion());
    }

    void ActorSoft::SetLocation (Real x, Real y, Real z)
    {
        Vector3 temp(x,y,z);
        this->SetLocation(temp);
    }

    void ActorSoft::SetLocation (Vector3 Place)
    {
        this->SetBulletLocation(Place);
        this->SetOgreLocation(Place);
    }

    Vector3 ActorSoft::GetLocation() const
    {
        return this->GetBulletLocation();
    }

    void ActorSoft::SetOrientation (Real x, Real y, Real z, Real w)
    {
        Quaternion temp(x,y,z,w);
        this->SetOrientation(temp);
    }

    void ActorSoft::SetOrientation (Quaternion Rotation)
    {
        this->SetBulletOrientation(Rotation);
        this->SetOgreOrientation(Rotation);
    }

    void ActorSoft::CreateShapeFromMeshDynamic(short unsigned int Accuracy, bool UseAllSubmeshes)
    {
    }

    void ActorSoft::AddObjectToWorld (World *TargetWorld)
    {
        TargetWorld->GetPhysicsManager()->GetPhysicsWorldPointer()->addSoftBody(this->physsoftbody);
        this->AttachToGraphics();
    }

    void ActorSoft::RemoveObjectFromWorld(World* TargetWorld)
    {
        TargetWorld->GetPhysicsManager()->GetPhysicsWorldPointer()->removeSoftBody(this->physsoftbody);
        this->DetachFromGraphics();
    }

    btSoftBody* ActorSoft::GetBulletObject()
    {
        return physsoftbody;
    }
}
#endif
