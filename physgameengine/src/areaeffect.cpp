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
#ifndef _areaeffect_cpp
#define _areaeffect_cpp

#include "areaeffect.h"
#include "world.h"
#include "actorbase.h"
#include "physicsmanager.h"

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>

namespace phys{

    AreaEffect::AreaEffect(const String &name, Vector3 Location)
        : Name (name)
    {
        TheWorld = World::GetWorldPointer();
        CreateGhostObject(Location);
    }

    AreaEffect::~AreaEffect()
    {
        delete Ghost;
        delete Shape;
    }

    void AreaEffect::CreateGhostObject(Vector3 Location)
    {
        Ghost = new btPairCachingGhostObject();
        Ghost->setCollisionFlags(Ghost->getCollisionFlags() + btCollisionObject::CF_NO_CONTACT_RESPONSE);
        Ghost->getWorldTransform().setOrigin(Location.GetBulletVector3());
        Ghost->setUserPointer(this);
    }

    void AreaEffect::AddActorToList(ActorBase* Actor)
    {
        AddedActors.push_back(Actor);
        OverlappingActors.push_back(Actor);
    }

    void AreaEffect::RemoveActorFromList(ActorBase* Actor)
    {
        RemovedActors.push_back(Actor);
        for( std::list<ActorBase*>::iterator c=OverlappingActors.begin(); c!=OverlappingActors.end(); c++)
        {
            if ( Actor == (*c) )
            {
                OverlappingActors.erase(c);
            }
        }
    }

    void AreaEffect::UpdateActorList()
    {
        if ( !(AddedActors.empty()) )
            AddedActors.clear();
        if ( !(RemovedActors.empty()) )
            RemovedActors.clear();
        btSoftRigidDynamicsWorld* PhysWorld = TheWorld->GetPhysicsManager()->GetPhysicsWorldPointer();
        PhysWorld->getDispatcher()->dispatchAllCollisionPairs(Ghost->getOverlappingPairCache(), PhysWorld->getDispatchInfo(), PhysWorld->getDispatcher());

        int OverlappingPairs = Ghost->getOverlappingPairCache()->getNumOverlappingPairs();
        btBroadphasePair* Pair = Ghost->getOverlappingPairCache()->getOverlappingPairArrayPtr();
        std::list<ActorBase*>::iterator it = OverlappingActors.begin();
        // Make a bool vector to keep track of which actors to keep when updating.
        std::vector<bool> Tracker;
        Tracker.resize(OverlappingActors.size());
        std::vector<bool>::iterator bit;
        for ( bit = Tracker.begin() ; bit != Tracker.end() ; bit++ )
        {
            (*bit) = false;
        }
        // Add objects to the necessary lists as needed.  Also track what needs to be removed for later.
        for ( int x=0 ; x<OverlappingPairs ; x++ )
        {
            // Get the non-ghost object from a given pair and cast it to a usable pointer.
            btCollisionObject* ColObj = Pair[x].m_pProxy0->m_clientObject != Ghost ? (btCollisionObject*)(Pair[x].m_pProxy0->m_clientObject) : (btCollisionObject*)(Pair[x].m_pProxy1->m_clientObject);
            ActorBase* Actor = (ActorBase*)(ColObj->getUserPointer());
            // Check list for the actor in the pair.
            for( it = OverlappingActors.begin(), bit = Tracker.begin() ; it != OverlappingActors.end() ; it++, bit++ )
            {
                if ( Actor == (*it) )
                {
                    (*bit) = true;
                    break;
                }
            }
            if ( it == OverlappingActors.end() )
            {
                AddActorToList(Actor);
                Tracker.push_back(true);
            }
        }
        // Verify they are the same size.  Then remove items from the list as necessary.
        if ( OverlappingActors.size() == Tracker.size() )
        {
            std::list<ActorBase*>::iterator sit = OverlappingActors.begin();
            for ( bit = Tracker.begin() ; bit != Tracker.end() ; )
            {
                if ( (*bit) == false )
                {
                    bit = Tracker.erase(bit);
                    ActorBase* Act = (*sit);
                    RemovedActors.push_back(Act);
                    sit = OverlappingActors.erase(sit);
                }else{
                    sit++;
                    bit++;
                }
            }
        }
    }

    void AreaEffect::CreateSphereShape(Real Radius)
    {
        Shape = new btSphereShape(Radius);
        Ghost->setCollisionShape(Shape);
    }

    void AreaEffect::CreateCylinderShape(Vector3 HalfExtents)
    {
        Shape = new btCylinderShape(HalfExtents.GetBulletVector3());
        Ghost->setCollisionShape(Shape);
    }

    void AreaEffect::CreateBoxShape(Vector3 HalfExtents)
    {
        Shape = new btBoxShape(HalfExtents.GetBulletVector3());
        Ghost->setCollisionShape(Shape);
    }

    void AreaEffect::CreateShapeFromMesh(String Filename, String Group)
    {
        Ogre::Entity* entity = Ogre::Root::getSingleton()._getCurrentSceneManager()->createEntity("AETest1", Filename, Group);
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

        btTriangleMesh* trimesh = new btTriangleMesh(use32bitindexes);
        btVector3 vert0, vert1, vert2;
        int i=0;
        for (unsigned int y=0; y<triCount; y++)
        {
            vert0.setValue(vertices[indices[i]].x, vertices[indices[i]].y, vertices[indices[i]].z);
            vert1.setValue(vertices[indices[i+1]].x, vertices[indices[i+1]].y, vertices[indices[i+1]].z);
            vert2.setValue(vertices[indices[i+2]].x, vertices[indices[i+2]].y, vertices[indices[i+2]].z);
            trimesh->addTriangle(vert0, vert1, vert2);
            i+=3;
        }
        delete[] vertices;
        delete[] indices;
        Ogre::Root::getSingleton()._getCurrentSceneManager()->destroyEntity(entity);

        Shape = new btGImpactMeshShape(trimesh);
        Ghost->setCollisionShape(Shape);
    }

    void AreaEffect::ScaleFieldShape(Vector3 Scale)
    {
        Ghost->getCollisionShape()->setLocalScaling(Scale.GetBulletVector3());
    }

    Vector3 AreaEffect::GetFieldShapeScale()
    {
        Vector3 Scale(Ghost->getCollisionShape()->getLocalScaling());
        return Scale;
    }

    void AreaEffect::SetLocation(Vector3 Location)
    {
        Ghost->getWorldTransform().setOrigin(Location.GetBulletVector3());
    }

    Vector3 AreaEffect::GetLocation()
    {
        Vector3 Loc(Ghost->getWorldTransform().getOrigin());
        return Loc;
    }

    ConstString& AreaEffect::GetName()
    {
        return Name;
    }

    std::list<ActorBase*>& AreaEffect::GetOverlappingActors()
    {
        return OverlappingActors;
    }

    std::vector<ActorBase*>& AreaEffect::GetAddedActors()
    {
        return AddedActors;
    }

    std::vector<ActorBase*>& AreaEffect::GetRemovedActors()
    {
        return RemovedActors;
    }

    ///////////////////////////////////
    // TestAE functions

    TestAE::TestAE(const String &name, Vector3 Location) : AreaEffect(name, Location)
    {
    }

    TestAE::~TestAE()
    {
    }

    void TestAE::ApplyEffect()
    {
        std::vector<ActorBase*>* Added = &(GetAddedActors());
        std::vector<ActorBase*>* Removed = &(GetRemovedActors());
        std::list<ActorBase*>* Current = &(GetOverlappingActors());

        std::vector<ActorBase*>::iterator AaRIt;
        std::list<ActorBase*>::iterator CurrIt;

        ActorBase* Act = NULL;

        if ( !(Added->empty()) )
        {
            TheWorld->Log("Actors Added to field this frame:");
            for ( AaRIt = Added->begin() ; AaRIt != Added->end() ; AaRIt++ )
            {
                Act = (*AaRIt);
                TheWorld->Log(Act);
            }
        }
        if ( !(Removed->empty()) )
        {
            TheWorld->Log("Actors Removed from field this frame:");
            for ( AaRIt = Removed->begin() ; AaRIt != Removed->end() ; AaRIt++ )
            {
                Act = (*AaRIt);
                TheWorld->Log(Act);
            }
        }
        if ( !(Current->empty()) )
        {
            TheWorld->Log("Actors Currently in field this frame:");
            for ( CurrIt = Current->begin() ; CurrIt != Current->end() ; CurrIt++ )
            {
                Act = (*CurrIt);
                TheWorld->Log(Act);
            }
        }
    }

    ///////////////////////////////////
    // GravityField functions

    GravityField::GravityField(const String &name, Vector3 Location) : AreaEffect(name, Location)
    {
    }

    GravityField::~GravityField()
    {
    }

    void GravityField::ApplyEffect()
    {
        std::vector<ActorBase*>* Added = &(GetAddedActors());
        std::vector<ActorBase*>* Removed = &(GetRemovedActors());
        std::vector<ActorBase*>::iterator It;
        PhysicsManager* Physics = TheWorld->GetPhysicsManager();
        ActorBase* Act = NULL;

        if ( !(Added->empty()) )
        {
            for ( It = Added->begin() ; It != Added->end() ; It++ )
            {
                Act = (*It);
                Physics->SetIndividualGravity(Act, Grav);
            }
        }
        if ( !(Removed->empty()) )
        {
            for ( It = Removed->begin() ; It != Removed->end() ; It++ )
            {
                Act = (*It);
                Physics->SetIndividualGravity(Act, Physics->GetGravity());
            }
        }
    }

    void GravityField::SetFieldGravity(Vector3 Gravity)
    {
        Grav = Gravity;
    }

    Vector3 GravityField::GetFieldGravity()
    {
        return Grav;
    }
}

#endif
