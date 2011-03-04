//� Copyright 2010 BlackTopp Studios Inc.
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
#include "actorrigid.h"
#include "physicsmanager.h"

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>

namespace phys{

    AreaEffect::AreaEffect(const String &name, const Vector3 Location)
        : Name (name),
          Shape (NULL),
          GraphicsNode(NULL),
          GraphicsObject(NULL),
          ShapeType(AE_Unassigned)
    {
        TheWorld = World::GetWorldPointer();
        CreateGhostObject(Location);
    }

    AreaEffect::~AreaEffect()
    {
        delete Ghost;
        if(Shape)
            delete Shape;
    }

    void AreaEffect::CreateGhostObject(const Vector3 Location)
    {
        Ghost = new btPairCachingGhostObject();
        Ghost->setCollisionFlags(Ghost->getCollisionFlags() + btCollisionObject::CF_NO_CONTACT_RESPONSE);
        Ghost->getWorldTransform().setOrigin(Location.GetBulletVector3());
        Ghost->setUserPointer(this);
    }

    Ogre::MaterialPtr AreaEffect::CreateColouredMaterial(const ColourValue& Colour)
    {
        Ogre::MaterialPtr AEMaterial = Ogre::MaterialManager::getSingletonPtr()->create(Name+"Material", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        AEMaterial->setReceiveShadows(false);
        Ogre::Pass* pass = AEMaterial->getTechnique(0)->getPass(0);
        pass->setCullingMode(Ogre::CULL_NONE);
        pass->setDepthCheckEnabled(true);
        pass->setDepthWriteEnabled(false);
        pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        pass->setAmbient(Colour.GetOgreColourValue());
        pass->setDiffuse(Colour.GetOgreColourValue());
        AEMaterial->prepare();
        AEMaterial->load();
        return AEMaterial;
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
        //PhysWorld->getDispatcher()->dispatchAllCollisionPairs(Ghost->getOverlappingPairCache(), PhysWorld->getDispatchInfo(), PhysWorld->getDispatcher());

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

    void AreaEffect::CreateSphereShape(const Real Radius)
    {
        Shape = new btSphereShape(Radius);
        Ghost->setCollisionShape(Shape);
        ShapeType = AE_Sphere;
    }

    void AreaEffect::CreateCylinderShapeX(const Vector3 HalfExtents)
    {
        Shape = new btCylinderShapeX(HalfExtents.GetBulletVector3());
        Ghost->setCollisionShape(Shape);
        ShapeType = AE_CylinderX;
    }

    void AreaEffect::CreateCylinderShapeY(const Vector3 HalfExtents)
    {
        Shape = new btCylinderShape(HalfExtents.GetBulletVector3());
        Ghost->setCollisionShape(Shape);
        ShapeType = AE_CylinderY;
    }

    void AreaEffect::CreateCylinderShapeZ(const Vector3 HalfExtents)
    {
        Shape = new btCylinderShapeZ(HalfExtents.GetBulletVector3());
        Ghost->setCollisionShape(Shape);
        ShapeType = AE_CylinderZ;
    }

    void AreaEffect::CreateBoxShape(const Vector3 HalfExtents)
    {
        Shape = new btBoxShape(HalfExtents.GetBulletVector3());
        Ghost->setCollisionShape(Shape);
        ShapeType = AE_Box;
    }

    void AreaEffect::CreateShapeFromMesh(String Filename, String Group, bool MakeVisible)
    {
        Ogre::SceneManager* OgreManager = World::GetWorldPointer()->GetSceneManager()->GetGraphicsWorldPointer();
        if(GraphicsObject)
        {
            OgreManager->destroyEntity(GraphicsObject);
            GraphicsObject = NULL;
        }
        if(GraphicsNode)
        {
            OgreManager->destroySceneNode(GraphicsNode);
            GraphicsNode = NULL;
        }
        if(Shape)
        {
            delete Shape;
            Shape = NULL;
        }

        GraphicsObject = OgreManager->createEntity(Name, Filename, Group);
        Ogre::MeshPtr myMesh = GraphicsObject->getMesh();
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

        if(MakeVisible)
        {
            GraphicsNode = OgreManager->createSceneNode();
            OgreManager->getRootSceneNode()->addChild(GraphicsNode);
            GraphicsNode->attachObject(GraphicsObject);
        }else{
            OgreManager->destroyEntity(GraphicsObject);
            GraphicsObject = NULL;
        }

        Shape = new btGImpactMeshShape(trimesh);
        Ghost->setCollisionShape(Shape);
        ShapeType = AE_Custom;
    }

    void AreaEffect::ScaleFieldShape(const Vector3 Scale)
    {
        Ghost->getCollisionShape()->setLocalScaling(Scale.GetBulletVector3());
    }

    Vector3 AreaEffect::GetFieldShapeScale()
    {
        Vector3 Scale(Ghost->getCollisionShape()->getLocalScaling());
        return Scale;
    }

    void AreaEffect::SetLocation(const Vector3 Location)
    {
        Ghost->getWorldTransform().setOrigin(Location.GetBulletVector3());
        if(GraphicsNode)
            GraphicsNode->setPosition(Location.GetOgreVector3());
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

    AreaEffect::AEShapeType AreaEffect::GetShapeType()
    {
        return ShapeType;
    }

    void AreaEffect::CreateGraphicsSphere(const ColourValue& Colour, const Whole Rings, const Whole Segments)
    {
        if(AreaEffect::AE_Sphere != ShapeType)
        {
            TheWorld->Log("Attempting to create sphere graphics shape when AreaEffect isn't a sphere is not supported.  Exiting Function.");
            return;
        }
        String MatName = (CreateColouredMaterial(Colour))->getName();
        CreateGraphicsSphere(MatName,Rings,Segments);
    }

    void AreaEffect::CreateGraphicsSphere(String& MaterialName, const Whole Rings, const Whole Segments)
    {
        if(AreaEffect::AE_Sphere != ShapeType)
        {
            TheWorld->Log("Attempting to create sphere graphics shape when AreaEffect isn't a sphere is not supported.  Exiting Function.");
            return;
        }
        Ogre::SceneManager* OgreManager = World::GetWorldPointer()->GetSceneManager()->GetGraphicsWorldPointer();
        if(GraphicsObject)
        {
            OgreManager->destroyEntity(GraphicsObject);
            GraphicsObject = NULL;
        }
        if(GraphicsNode)
        {
            OgreManager->destroySceneNode(GraphicsNode);
            GraphicsNode = NULL;
        }
        Ogre::MaterialPtr TheMaterial = Ogre::MaterialManager::getSingleton().getByName(MaterialName);
        String GroupName = TheMaterial->getGroup();

        Real Radius = (static_cast<btSphereShape*>(Shape))->getRadius();
        Real RingAngle = (Ogre::Math::PI / Rings);
        Real SegAngle = (2 * Ogre::Math::PI / Segments);
        Whole VertIndex = 0;

        Ogre::ManualObject* sphere = new Ogre::ManualObject("TempMan");
        sphere->begin(MaterialName);

        for( Whole ring = 0 ; ring <= Rings ; ring++ ) {
        Real r0 = Radius * sinf (ring * RingAngle);
        Real y0 = Radius * cosf (ring * RingAngle);

            // Generate the group of segments for the current ring
            for( Whole seg = 0 ; seg <= Segments ; seg++ ) {
                Real x0 = r0 * sinf(seg * SegAngle);
                Real z0 = r0 * cosf(seg * SegAngle);

                // Add one vertex to the strip which makes up the sphere
                sphere->position( x0, y0, z0);
                sphere->normal(Ogre::Vector3(x0, y0, z0).normalisedCopy());
                sphere->textureCoord((Real) seg / (Real) Segments, (Real) ring / (Real) Rings);

                if (ring != Rings) {
                    // each vertex (except the last) has six indicies pointing to it
                    sphere->index(VertIndex + Segments + 1);
                    sphere->index(VertIndex);
                    sphere->index(VertIndex + Segments);
                    sphere->index(VertIndex + Segments + 1);
                    sphere->index(VertIndex + 1);
                    sphere->index(VertIndex);
                    VertIndex++;
                }
            }
        }

        sphere->end();
        sphere->convertToMesh(Name + "Mesh", GroupName);

        GraphicsObject = OgreManager->createEntity(Name,Name + "Mesh", GroupName);
        GraphicsNode = OgreManager->createSceneNode();
        OgreManager->getRootSceneNode()->addChild(GraphicsNode);
        GraphicsNode->setPosition((GetLocation()).GetOgreVector3());
        GraphicsNode->attachObject(GraphicsObject);
    }

    void AreaEffect::CreateGraphicsCylinder(const ColourValue& Colour)
    {
        if(AreaEffect::AE_CylinderX != ShapeType && AreaEffect::AE_CylinderY != ShapeType && AreaEffect::AE_CylinderZ != ShapeType)
        {
            TheWorld->Log("Attempting to create cylinder graphics shape when AreaEffect isn't a cylinder is not supported.  Exiting Function.");
            return;
        }
        String MatName = (CreateColouredMaterial(Colour))->getName();
        CreateGraphicsCylinder(MatName);
    }

    void AreaEffect::CreateGraphicsCylinder(String& MaterialName)
    {
        if(AreaEffect::AE_CylinderX != ShapeType && AreaEffect::AE_CylinderY != ShapeType && AreaEffect::AE_CylinderZ != ShapeType)
        {
            TheWorld->Log("Attempting to create cylinder graphics shape when AreaEffect isn't a cylinder is not supported.  Exiting Function.");
            return;
        }
        Ogre::SceneManager* OgreManager = World::GetWorldPointer()->GetSceneManager()->GetGraphicsWorldPointer();
        if(GraphicsObject)
        {
            OgreManager->destroyEntity(GraphicsObject);
            GraphicsObject = NULL;
        }
        if(GraphicsNode)
        {
            OgreManager->destroySceneNode(GraphicsNode);
            GraphicsNode = NULL;
        }
        Ogre::MaterialPtr TheMaterial = Ogre::MaterialManager::getSingleton().getByName(MaterialName);
        String GroupName = TheMaterial->getGroup();

        Ogre::ManualObject* cylinder = new Ogre::ManualObject("TempMan");
        cylinder->begin(MaterialName);

        cylinder->end();
        cylinder->convertToMesh(Name + "Mesh", GroupName);

        GraphicsObject = OgreManager->createEntity(Name,Name + "Mesh", GroupName);
        GraphicsNode = OgreManager->createSceneNode();
        OgreManager->getRootSceneNode()->addChild(GraphicsNode);
        GraphicsNode->setPosition((GetLocation()).GetOgreVector3());
        GraphicsNode->attachObject(GraphicsObject);
    }

    void AreaEffect::CreateGraphicsBox(const ColourValue& Colour)
    {
        if(AreaEffect::AE_Box != ShapeType)
        {
            TheWorld->Log("Attempting to create box graphics shape when AreaEffect isn't a box is not supported.  Exiting Function.");
            return;
        }
        String MatName = (CreateColouredMaterial(Colour))->getName();
        CreateGraphicsBox(MatName);
    }

    void AreaEffect::CreateGraphicsBox(String& MaterialName)
    {
        if(AreaEffect::AE_Box != ShapeType)
        {
            TheWorld->Log("Attempting to create box graphics shape when AreaEffect isn't a box is not supported.  Exiting Function.");
            return;
        }
        Ogre::SceneManager* OgreManager = TheWorld->GetSceneManager()->GetGraphicsWorldPointer();
        if(GraphicsObject)
        {
            OgreManager->destroyEntity(GraphicsObject);
            GraphicsObject = NULL;
        }
        if(GraphicsNode)
        {
            OgreManager->destroySceneNode(GraphicsNode);
            GraphicsNode = NULL;
        }
        Ogre::MaterialPtr TheMaterial = Ogre::MaterialManager::getSingleton().getByName(MaterialName);
        String GroupName = TheMaterial->getGroup();
        /// @todo The HalfExtents returned here includes scaling, should account for that so weird scaling sync issues don't occur.
        Vector3 Half((static_cast<btBoxShape*>(Shape))->getHalfExtentsWithoutMargin());

        Ogre::ManualObject* box = new Ogre::ManualObject("TempMan");
        box->begin(MaterialName);
        //Front
        box->position(-Half.X,-Half.Y,Half.Z);  box->normal(0,0,1);  box->textureCoord(0,1);
        box->position(Half.X,-Half.Y,Half.Z);   box->normal(0,0,1);  box->textureCoord(1,1);
        box->position(Half.X,Half.Y,Half.Z);    box->normal(0,0,1);  box->textureCoord(1,0);
        box->position(-Half.X,Half.Y,Half.Z);   box->normal(0,0,1);  box->textureCoord(0,0);
        //Back
        box->position(Half.X,-Half.Y,-Half.Z);  box->normal(0,0,-1); box->textureCoord(0,1);
        box->position(-Half.X,-Half.Y,-Half.Z); box->normal(0,0,-1); box->textureCoord(1,1);
        box->position(-Half.X,Half.Y,-Half.Z);  box->normal(0,0,-1); box->textureCoord(1,0);
        box->position(Half.X,Half.Y,-Half.Z);   box->normal(0,0,-1); box->textureCoord(0,0);
        //Left
        box->position(-Half.X,-Half.Y,-Half.Z); box->normal(-1,0,0); box->textureCoord(0,1);
        box->position(-Half.X,-Half.Y,Half.Z);  box->normal(-1,0,0); box->textureCoord(1,1);
        box->position(-Half.X,Half.Y,Half.Z);   box->normal(-1,0,0); box->textureCoord(1,0);
        box->position(-Half.X,Half.Y,-Half.Z);  box->normal(-1,0,0); box->textureCoord(0,0);
        //Right
        box->position(Half.X,-Half.Y,Half.Z);   box->normal(1,0,0);  box->textureCoord(0,1);
        box->position(Half.X,-Half.Y,-Half.Z);  box->normal(1,0,0);  box->textureCoord(1,1);
        box->position(Half.X,Half.Y,-Half.Z);   box->normal(1,0,0);  box->textureCoord(1,0);
        box->position(Half.X,Half.Y,Half.Z);    box->normal(1,0,0);  box->textureCoord(0,0);
        //Top
        box->position(-Half.X,Half.Y,Half.Z);   box->normal(0,1,0);  box->textureCoord(0,1);
        box->position(Half.X,Half.Y,Half.Z);    box->normal(0,1,0);  box->textureCoord(1,1);
        box->position(Half.X,Half.Y,-Half.Z);   box->normal(0,1,0);  box->textureCoord(1,0);
        box->position(-Half.X,Half.Y,-Half.Z);  box->normal(0,1,0);  box->textureCoord(0,0);
        //Bottom
        box->position(-Half.X,-Half.Y,-Half.Z); box->normal(0,-1,0); box->textureCoord(0,1);
        box->position(Half.X,-Half.Y,-Half.Z);  box->normal(0,-1,0); box->textureCoord(1,1);
        box->position(Half.X,-Half.Y,Half.Z);   box->normal(0,-1,0); box->textureCoord(1,0);
        box->position(-Half.X,-Half.Y,Half.Z);  box->normal(0,-1,0); box->textureCoord(0,0);

        box->triangle(0,1,2);    box->triangle(0,2,3);    //Front
        box->triangle(4,5,6);    box->triangle(4,6,7);    //Back
        box->triangle(8,9,10);   box->triangle(8,10,11);  //Left
        box->triangle(12,13,14); box->triangle(12,14,15); //Right
        box->triangle(16,17,18); box->triangle(16,18,19); //Top
        box->triangle(20,21,22); box->triangle(20,22,23); //Bottom

        box->end();
        Ogre::MeshPtr boxmesh = box->convertToMesh(Name + "Mesh", GroupName);
        boxmesh->_setBounds(Ogre::AxisAlignedBox(-Half.X,-Half.Y,-Half.Z,Half.X,Half.Y,Half.Z));
        Real RunnerUp = Half.X > Half.Y ? Half.X : Half.Y;
        Real Largest = RunnerUp > Half.Z ? RunnerUp : Half.Z;
        boxmesh->_setBoundingSphereRadius(Ogre::Math::Sqrt(3*Largest*Largest));

        GraphicsObject = OgreManager->createEntity(Name,Name + "Mesh", GroupName);
        GraphicsNode = OgreManager->createSceneNode();
        OgreManager->getRootSceneNode()->addChild(GraphicsNode);
        GraphicsNode->setPosition((GetLocation()).GetOgreVector3());
        GraphicsNode->attachObject(GraphicsObject);
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

    TestAE::TestAE(const String &name, const Vector3 Location) : AreaEffect(name, Location)
    {
    }

    TestAE::~TestAE()
    {
    }

    void TestAE::ApplyEffect()
    {
        std::vector<ActorBase*>::iterator AaRIt;
        std::list<ActorBase*>::iterator CurrIt;
        ActorBase* Act = NULL;

        if ( !AddedActors.empty() )
        {
            TheWorld->Log("Actors Added to field this frame:");
            for ( AaRIt = AddedActors.begin() ; AaRIt != AddedActors.end() ; AaRIt++ )
            {
                Act = (*AaRIt);
                TheWorld->Log(Act);
            }
        }
        if ( !RemovedActors.empty() )
        {
            TheWorld->Log("Actors Removed from field this frame:");
            for ( AaRIt = RemovedActors.begin() ; AaRIt != RemovedActors.end() ; AaRIt++ )
            {
                Act = (*AaRIt);
                TheWorld->Log(Act);
            }
        }
        if ( !OverlappingActors.empty() )
        {
            TheWorld->Log("Actors Currently in field this frame:");
            for ( CurrIt = OverlappingActors.begin() ; CurrIt != OverlappingActors.end() ; CurrIt++ )
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
        std::vector<ActorBase*>::iterator It;
        PhysicsManager* Physics = TheWorld->GetPhysicsManager();
        ActorBase* Act = NULL;

        if ( !AddedActors.empty() )
        {
            for ( It = AddedActors.begin() ; It != AddedActors.end() ; It++ )
            {
                Act = (*It);
                Physics->SetIndividualGravity(Act, Grav);
            }
        }
        if ( !RemovedActors.empty() )
        {
            for ( It = RemovedActors.begin() ; It != RemovedActors.end() ; It++ )
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

    ///////////////////////////////////
    // GravityWell functions

    GravityWell::GravityWell(const String &name, Vector3 Location)
        : AreaEffect(name, Location),
          AllowWorldGrav(true),
          Strength(0),
          AttenAmount(0),
          AttenStyle(GW_Att_None)
    {
    }

    GravityWell::~GravityWell()
    {
    }

    void GravityWell::ApplyEffect()
    {
        if(0 == Strength)
            return;
        ActorBase* Act = NULL;
        ActorRigid* ActRig = NULL;
        if(!AllowWorldGrav && !AddedActors.empty())
        {
            for ( std::vector<ActorBase*>::iterator AA = AddedActors.begin() ; AA != AddedActors.end() ; AA++ )
            {
                if(ActorBase::Actorrigid != (*AA)->GetType())
                    continue;
                ActRig = dynamic_cast<ActorRigid*>(*AA);
                ActRig->SetIndividualGravity(Vector3());
            }
        }
        if(!OverlappingActors.empty())
        {
            Vector3 ActorLoc, Direction;
            Real Distance, AppliedStrength, InvMass;
            Vector3 GhostLoc = this->GetLocation();
            for ( std::list<ActorBase*>::iterator OA = OverlappingActors.begin() ; OA != OverlappingActors.end() ; OA++ )
            {
                if(ActorBase::Actorrigid != (*OA)->GetType())
                    continue;
                //Collect necessary data
                ActorLoc = (*OA)->GetLocation();
                Distance = ActorLoc.Distance(GhostLoc);
                Direction = (GhostLoc - ActorLoc) / Distance;
                switch(AttenStyle)
                {
                    case GW_Att_Linear:
                        AppliedStrength = Strength - (AttenAmount * Distance);
                        break;
                    case GW_Att_Quadratic:
                        AppliedStrength = Strength - (AttenAmount * (Distance * Distance));
                        break;
                    default:
                        AppliedStrength = Strength;
                        break;
                }
                ActRig = static_cast<ActorRigid*>(*OA);
                InvMass = ActRig->GetBulletObject()->getInvMass();
                if(0 != InvMass)
                    AppliedStrength *= (1 / ActRig->GetBulletObject()->getInvMass());
                else
                    AppliedStrength = 0;
                if(0 > AppliedStrength)
                    AppliedStrength = 0;
                //Apply the Force
                ActRig->GetBulletObject()->applyCentralForce((Direction * AppliedStrength).GetBulletVector3());
            }
        }
        if(!AllowWorldGrav && !RemovedActors.empty())
        {
            Vector3 WorldGrav = TheWorld->GetPhysicsManager()->GetGravity();
            for ( std::vector<ActorBase*>::iterator RA = RemovedActors.begin() ; RA != RemovedActors.end() ; RA++ )
            {
                if(ActorBase::Actorrigid != (*RA)->GetType())
                    continue;
                ActRig = dynamic_cast<ActorRigid*>(*RA);
                ActRig->SetIndividualGravity(WorldGrav);
            }
        }
    }

    void GravityWell::SetFieldStrength(const Real FieldStrength)
    {
        Strength = FieldStrength;
    }

    Real GravityWell::GetFieldStrength()
    {
        return Strength;
    }

    void GravityWell::SetAllowWorldGravity(bool WorldGravity)
    {
        AllowWorldGrav = WorldGravity;
    }

    bool GravityWell::GetAllowWorldGravity()
    {
        return AllowWorldGrav;
    }

    void GravityWell::SetAttenuation(Real Amount, AttenuationStyle Style)
    {
        AttenAmount = Amount;
        AttenStyle = Style;
    }

    GravityWell::AttenuationStyle GravityWell::GetAttenuationStyle()
    {
        return AttenStyle;
    }

    Real GravityWell::GetAttenuationAmount()
    {
        return AttenAmount;
    }
}

#endif
