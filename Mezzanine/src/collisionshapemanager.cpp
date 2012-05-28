//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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
#ifndef _collisionshapemanager_cpp
#define _collisionshapemanager_cpp

#include "collisionshapemanager.h"
#include "collisionshape.h"
#include "mesh.h"
#include "meshmanager.h"

// For logging
#include "world.h"

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "ConvexBuilder.h"
#include "Internal/decompinterface.h.cpp"
#include <btBulletWorldImporter.h>

#include <fstream>

namespace Mezzanine
{
    template<> CollisionShapeManager* Singleton<CollisionShapeManager>::SingletonPtr = 0;

    CollisionShapeManager::CollisionShapeManager()
    {
        this->Priority = 40;
    }

#ifdef MEZZXML
    CollisionShapeManager::CollisionShapeManager(xml::Node& XMLNode)
    {
        this->Priority = 40;
        /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.
    }
#endif

    CollisionShapeManager::~CollisionShapeManager()
    {
        DestroyAllShapes();
    }

    CollisionShapeManager::iterator CollisionShapeManager::begin()
        { return this->CollisionShapes.begin(); }

    CollisionShapeManager::const_iterator CollisionShapeManager::begin() const
        { return this->CollisionShapes.begin(); }

    CollisionShapeManager::iterator CollisionShapeManager::end()
        { return this->CollisionShapes.end(); }

    CollisionShapeManager::const_iterator CollisionShapeManager::end() const
        { return this->CollisionShapes.end(); }

    btTriangleMesh* CollisionShapeManager::CreateBulletTrimesh(Mesh* ObjectMesh, bool UseAllSubmeshes)
    {
        Ogre::MeshPtr myMesh = ObjectMesh->GetOgreMesh();
        Ogre::SubMesh* subMesh = NULL;
        Ogre::IndexData*  IndexData = NULL;
        Ogre::VertexData* VertexData = NULL;
        bool use32bitindexes = false;
        unsigned int triCount = 0;
        unsigned int vCount = 0;
        unsigned int iCount = 0;
        Whole VertPrevSize = 0;
        Whole IndiPrevSize = 0;
        Ogre::Vector3* vertices = NULL;
        unsigned long* indices = NULL;
        bool SharedVerts = myMesh->getSubMesh(0)->useSharedVertices;

        if(UseAllSubmeshes)
        {
            for( Whole X = 0 ; X < myMesh->getNumSubMeshes() ; X++ )
            {
                vCount += SharedVerts ? myMesh->sharedVertexData->vertexCount : myMesh->getSubMesh(X)->vertexData->vertexCount;
                iCount += myMesh->getSubMesh(X)->indexData->indexCount;
            }
        }else{
            vCount += SharedVerts ? myMesh->sharedVertexData->vertexCount : myMesh->getSubMesh(0)->vertexData->vertexCount;
            iCount += myMesh->getSubMesh(0)->indexData->indexCount;
        }

        vertices = new Ogre::Vector3[vCount];
        indices  = new unsigned long[iCount];

        // Get the submesh and associated data
        for( unsigned short int SubMeshIndex = 0 ; SubMeshIndex < myMesh->getNumSubMeshes() ; SubMeshIndex++ )
        {
            if(!UseAllSubmeshes && (SubMeshIndex > 0))
                break;
            if(SharedVerts && (SubMeshIndex > 0))
                break;

            subMesh = myMesh->getSubMesh(SubMeshIndex);
            IndexData = subMesh->indexData;
            VertexData = SharedVerts ? myMesh->sharedVertexData : myMesh->getSubMesh(SubMeshIndex)->vertexData;

            // Get the position element
            const Ogre::VertexElement* posElem = VertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
            // Get a pointer to the vertex buffer
            Ogre::HardwareVertexBufferSharedPtr vBuffer = VertexData->vertexBufferBinding->getBuffer(posElem->getSource());
            // Get a pointer to the index buffer
            Ogre::HardwareIndexBufferSharedPtr iBuffer = IndexData->indexBuffer;
            // Get the number of triangles
            triCount+=(IndexData->indexCount/3);

            // Lock the vertex buffer (READ ONLY)
            unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            float* pReal = NULL;
            for (size_t j = 0; j < VertexData->vertexCount; ++j, vertex += vBuffer->getVertexSize() )
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);
                Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
                vertices[j + VertPrevSize] = pt;
            }
            vBuffer->unlock();
            size_t index_offset = 0;
            use32bitindexes = (iBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

            // Lock the index buffer (READ ONLY)
            unsigned long* pLong = static_cast<unsigned long*>(iBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

            if (use32bitindexes)
            {
                for (size_t k = 0; k < triCount*3; ++k)
                {
                    indices[index_offset+IndiPrevSize] = pLong[k];
                    index_offset++;
                }
            }
            else
            {
                for (size_t k = 0; k < triCount*3; ++k)
                {
                    indices[index_offset+IndiPrevSize] = static_cast<unsigned long>(pShort[k]);
                    index_offset++;
                }
            }
            iBuffer->unlock();

            VertPrevSize+=VertexData->vertexCount;
            IndiPrevSize+=IndexData->indexCount;
        }

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

    CollisionShape* CollisionShapeManager::WrapShape(const String& Name, btCollisionShape* InternalShape)
    {
        switch(InternalShape->getShapeType())
        {
            case BOX_SHAPE_PROXYTYPE:
            {
                BoxCollisionShape* BoxShape = new BoxCollisionShape(Name,(btBoxShape*)InternalShape);
                return BoxShape;
                break;
            }
            case CAPSULE_SHAPE_PROXYTYPE:
            {
                CapsuleCollisionShape* CapsuleShape = new CapsuleCollisionShape(Name,(btCapsuleShape*)InternalShape);
                return CapsuleShape;
                break;
            }
            case CONE_SHAPE_PROXYTYPE:
            {
                ConeCollisionShape* ConeShape = new ConeCollisionShape(Name,(btConeShape*)InternalShape);
                return ConeShape;
                break;
            }
            case CONVEX_HULL_SHAPE_PROXYTYPE:
            {
                ConvexHullCollisionShape* ConvexHullShape = new ConvexHullCollisionShape(Name,(btConvexHullShape*)InternalShape);
                return ConvexHullShape;
                break;
            }
            case CYLINDER_SHAPE_PROXYTYPE:
            {
                CylinderCollisionShape* CylinderShape = new CylinderCollisionShape(Name,(btCylinderShape*)InternalShape);
                return CylinderShape;
                break;
            }
            case MULTI_SPHERE_SHAPE_PROXYTYPE:
            {
                MultiSphereCollisionShape* MultiSphereShape = new MultiSphereCollisionShape(Name,(btMultiSphereShape*)InternalShape);
                return MultiSphereShape;
                break;
            }
            case SPHERE_SHAPE_PROXYTYPE:
            {
                SphereCollisionShape* SphereShape = new SphereCollisionShape(Name,(btSphereShape*)InternalShape);
                return SphereShape;
                break;
            }
            case GIMPACT_SHAPE_PROXYTYPE:
            {
                DynamicMeshCollisionShape* GImpactShape = new DynamicMeshCollisionShape(Name,(btGImpactMeshShape*)InternalShape);
                return GImpactShape;
                break;
            }
            case TERRAIN_SHAPE_PROXYTYPE:
            {
                HeightfieldCollisionShape* HeightFieldShape = new HeightfieldCollisionShape(Name,(btHeightfieldTerrainShape*)InternalShape);
                return HeightFieldShape;
                break;
            }
            case STATIC_PLANE_PROXYTYPE:
            {
                PlaneCollisionShape* PlaneShape = new PlaneCollisionShape(Name,(btStaticPlaneShape*)InternalShape);
                return PlaneShape;
                break;
            }
            case SOFTBODY_SHAPE_PROXYTYPE:
            {
                ActorSoftCollisionShape* SoftBodyShape = new ActorSoftCollisionShape(Name,(btSoftBodyCollisionShape*)InternalShape);
                return SoftBodyShape;
                break;
            }
            case TRIANGLE_MESH_SHAPE_PROXYTYPE:
            {
                StaticMeshCollisionShape* BvhShape = new StaticMeshCollisionShape(Name,(btBvhTriangleMeshShape*)InternalShape);
                return BvhShape;
                break;
            }
            case COMPOUND_SHAPE_PROXYTYPE:
            {
                CompoundCollisionShape* Compound = new CompoundCollisionShape(Name,(btCompoundShape*)InternalShape);
                return Compound;
                break;
            }
            default:
            {
                MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Attempting to load an unsupported/unwrapped Collision Shape in CompoundShapeManager::LoadAllShapesFromFile.");
            }//default
        }
    }

    void CollisionShapeManager::StoreShape(CollisionShape* Shape)
    {
        std::map<String,CollisionShape*>::iterator CS = CollisionShapes.find(Shape->GetName());
        if(CS != CollisionShapes.end())
        {
            if((*CS).second != Shape)
            {
                MEZZ_EXCEPTION(Exception::II_DUPLICATE_IDENTITY_EXCEPTION,"Name of Collision Shape already exists on another object.  Names should be Unique.");
            }
        }else{
            CollisionShapes[Shape->GetName()] = Shape;
        }
    }

    CollisionShape* CollisionShapeManager::GetShape(const String& Name)
    {
        std::map<String,CollisionShape*>::iterator CS = CollisionShapes.find(Name);
        if(CS == CollisionShapes.end()) return NULL;
        else return (*CS).second;
    }

    Whole CollisionShapeManager::GetNumStoredShapes()
    {
        return CollisionShapes.size();
    }

    void CollisionShapeManager::RemoveShape(CollisionShape* Shape)
    {
        this->RemoveShape(Shape->GetName());
    }

    void CollisionShapeManager::RemoveShape(const String& Name)
    {
        std::map<String,CollisionShape*>::iterator CS = CollisionShapes.find(Name);
        if(CS == CollisionShapes.end())
            return;
        CollisionShapes.erase(CS);
    }

    void CollisionShapeManager::RemoveAllShapes()
    {
        CollisionShapes.clear();
    }

    void CollisionShapeManager::DestroyShape(CollisionShape* Shape)
    {
        this->DestroyShape(Shape->GetName());
    }

    void CollisionShapeManager::DestroyShape(const String& Name)
    {
        std::map<String,CollisionShape*>::iterator CS = CollisionShapes.find(Name);
        if(CS == CollisionShapes.end())
            return;
        delete (*CS).second;
        CollisionShapes.erase(CS);
    }

    void CollisionShapeManager::DestroyAllShapes()
    {
        for( std::map<String,CollisionShape*>::iterator CS = CollisionShapes.begin() ; CS != CollisionShapes.end() ; CS++ )
        {
            CollisionShape* CurrShape = (*CS).second;
            delete CurrShape;
        }
        CollisionShapes.clear();
    }

    ConvexHullCollisionShape* CollisionShapeManager::GenerateConvexHull(const String& Name, Mesh* ObjectMesh, bool UseAllSubmeshes)
    {
        btConvexShape *tmpshape = new btConvexTriangleMeshShape(this->CreateBulletTrimesh(ObjectMesh,UseAllSubmeshes));
        btShapeHull *hull = new btShapeHull(tmpshape);
        btScalar margin = tmpshape->getMargin();
        hull->buildHull(margin);
        //tmpshape->setUserPointer(hull);
        btConvexHullShape* convexShape = new btConvexHullShape();
        for (int b=0;b<hull->numVertices();b++)
        {
            convexShape->addPoint(hull->getVertexPointer()[b]);
        }
        delete tmpshape;
        delete hull;
        return new ConvexHullCollisionShape(Name,convexShape);
    }

    ConvexHullCollisionShape* CollisionShapeManager::GenerateConvexHull(const String& Name, const String& MeshName, const String& Group, bool UseAllSubmeshes)
    {
        Mesh* ObjectMesh = MeshManager::GetSingletonPtr()->LoadMesh(MeshName,Group);
        return this->GenerateConvexHull(Name,ObjectMesh,UseAllSubmeshes);
    }

    DynamicMeshCollisionShape* CollisionShapeManager::GenerateDynamicTriMesh(const String& Name, Mesh* ObjectMesh, bool UseAllSubmeshes)
    {
        btGImpactMeshShape* gimpact = new btGImpactMeshShape(this->CreateBulletTrimesh(ObjectMesh,UseAllSubmeshes));
        return new DynamicMeshCollisionShape(Name,gimpact);
    }

    DynamicMeshCollisionShape* CollisionShapeManager::GenerateDynamicTriMesh(const String& Name, const String& MeshName, const String& Group, bool UseAllSubmeshes)
    {
        Mesh* ObjectMesh = MeshManager::GetSingletonPtr()->LoadMesh(MeshName,Group);
        return this->GenerateDynamicTriMesh(Name,ObjectMesh,UseAllSubmeshes);
    }

    StaticMeshCollisionShape* CollisionShapeManager::GenerateStaticTriMesh(const String& Name, Mesh* ObjectMesh, bool UseAllSubmeshes)
    {
        btBvhTriangleMeshShape* tmpshape = new btBvhTriangleMeshShape(this->CreateBulletTrimesh(ObjectMesh,UseAllSubmeshes),true);
        return new StaticMeshCollisionShape(Name,tmpshape);
    }

    StaticMeshCollisionShape* CollisionShapeManager::GenerateStaticTriMesh(const String& Name, const String& MeshName, const String& Group, bool UseAllSubmeshes)
    {
        Mesh* ObjectMesh = MeshManager::GetSingletonPtr()->LoadMesh(MeshName,Group);
        return this->GenerateStaticTriMesh(Name,ObjectMesh,UseAllSubmeshes);
    }

    CompoundCollisionShape* CollisionShapeManager::PerformConvexDecomposition(const String& Name, Mesh* ObjectMesh, Whole Depth, Real CPercent, Real PPercent, bool UseAllSubmeshes)
    {
        //new submesh friendly code
        Ogre::MeshPtr myMesh = ObjectMesh->GetOgreMesh();
        Ogre::SubMesh* subMesh = NULL;
        Ogre::IndexData*  indexData = NULL;
        Ogre::VertexData* vertexData = NULL;
        bool use32bitindexes = false;
        unsigned int currtriCount = 0;
        unsigned int triCount = 0;
        unsigned int vCount = 0;
        unsigned int iCount = 0;
        Whole VertPrevSize = 0;
        Whole IndiPrevSize = 0;
        bool SharedVerts = myMesh->getSubMesh(0)->useSharedVertices;

        Whole* VertPerSubMesh = NULL;

        if(UseAllSubmeshes)
        {
            VertPerSubMesh = new Whole[myMesh->getNumSubMeshes()];
            for( Whole X = 0 ; X < myMesh->getNumSubMeshes() ; X++ )
            {
                vCount += SharedVerts ? myMesh->sharedVertexData->vertexCount : myMesh->getSubMesh(X)->vertexData->vertexCount;
                iCount += myMesh->getSubMesh(X)->indexData->indexCount;
                VertPerSubMesh[X] = SharedVerts ? myMesh->sharedVertexData->vertexCount : myMesh->getSubMesh(X)->vertexData->vertexCount;
            }
        }else{
            vCount += SharedVerts ? myMesh->sharedVertexData->vertexCount : myMesh->getSubMesh(0)->vertexData->vertexCount;
            iCount += myMesh->getSubMesh(0)->indexData->indexCount;
        }

        Ogre::Vector3* vertices = new Ogre::Vector3[vCount];
        unsigned int* indices  = new unsigned int[iCount];

        for( unsigned short int SubMeshIndex = 0 ; SubMeshIndex < myMesh->getNumSubMeshes() ; SubMeshIndex++ )
        {
            if(!UseAllSubmeshes && (SubMeshIndex > 0))
                break;
            if(SharedVerts && (SubMeshIndex > 0))
                break;

            subMesh = myMesh->getSubMesh(SubMeshIndex);
            indexData = subMesh->indexData;
            vertexData = SharedVerts ? myMesh->sharedVertexData : myMesh->getSubMesh(SubMeshIndex)->vertexData;

            const Ogre::VertexElement* posElem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
            Ogre::HardwareVertexBufferSharedPtr vBuffer = vertexData->vertexBufferBinding->getBuffer(posElem->getSource());
            Ogre::HardwareIndexBufferSharedPtr iBuffer = indexData->indexBuffer;
            currtriCount=indexData->indexCount/3;
            triCount+=(indexData->indexCount/3);

            unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            float* pReal = NULL;
            for (size_t j = 0; j < vertexData->vertexCount; j++, vertex += vBuffer->getVertexSize() )
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);
                vertices[j + VertPrevSize].x = *pReal++;
                vertices[j + VertPrevSize].y = *pReal++;
                vertices[j + VertPrevSize].z = *pReal++;
            }
            vBuffer->unlock();
            size_t index_offset = 0;
            use32bitindexes = (iBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

            unsigned long* pLong = static_cast<unsigned long*>(iBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

            if (use32bitindexes)
            {
                for (size_t k = 0; k < currtriCount*3; ++k)
                {
                    if(SubMeshIndex > 0 && VertPerSubMesh)
                        indices[index_offset+IndiPrevSize] = pLong[k] + VertPerSubMesh[SubMeshIndex];
                    else
                        indices[index_offset+IndiPrevSize] = pLong[k];
                    index_offset++;
                }
            }
            else
            {
                for (size_t k = 0; k < currtriCount*3; ++k)
                {
                    if(SubMeshIndex > 0 && VertPerSubMesh)
                    {
                        indices[index_offset+IndiPrevSize] = (static_cast<unsigned long>(pShort[k])) + VertPerSubMesh[SubMeshIndex];
                    }
                    else
                    {
                        indices[index_offset+IndiPrevSize] = static_cast<unsigned long>(pShort[k]);
                    }
                    index_offset++;
                }
            }
            iBuffer->unlock();

            VertPrevSize+=vertexData->vertexCount;
            IndiPrevSize+=indexData->indexCount;
        }
        //old non-submesh friendly code
        /*Ogre::MeshPtr myMesh = entity->getMesh();
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
        iBuffer->unlock();*/

        ConvexDecomposition::DecompDesc desc;
        desc.mVcount = vertexData->vertexCount;
        desc.mTcount = triCount;
        desc.mVertices = &vertices[0].x;
        desc.mIndices = &indices[0];
        unsigned int maxv  = 16;
        float skinWidth    = 0.0;
        desc.mDepth        = Depth;
        desc.mCpercent     = CPercent;
        desc.mPpercent     = PPercent;
        desc.mMaxVertices  = maxv;
        desc.mSkinWidth    = skinWidth;

        Internal::MezzConvexDecomposition decomp;
        desc.mCallback = &decomp;

        ConvexBuilder cb(desc.mCallback);
        cb.process(desc);

        CompoundCollisionShape* compound = new CompoundCollisionShape(Name);
        btTransform trans;
        trans.setIdentity();
        for (int i=0;i<decomp.m_convexShapes.size();i++)
        {
            std::stringstream namestream;
            namestream << Name << "Child" << i;
            Vector3 centroid(decomp.m_convexCentroids[i]);
            ConvexHullCollisionShape* convexShape = new ConvexHullCollisionShape(namestream.str(),decomp.m_convexShapes[i]);
            compound->AddChildShape(convexShape,centroid);
        }
        return compound;
    }

    CompoundCollisionShape* CollisionShapeManager::PerformConvexDecomposition(const String& Name, const String& MeshName, const String& Group, Whole Depth, Real CPercent, Real PPercent, bool UseAllSubmeshes)
    {
        Mesh* ObjectMesh = MeshManager::GetSingletonPtr()->LoadMesh(MeshName,Group);
        return this->PerformConvexDecomposition(Name,ObjectMesh,Depth,CPercent,PPercent,UseAllSubmeshes);
    }

    void CollisionShapeManager::LoadAllShapesFromFile(const String& FileName, const String& Group)
    {
        btBulletWorldImporter Importer;
        Ogre::DataStreamPtr Stream = Ogre::ResourceGroupManager::getSingleton().openResource(FileName,Group);
        char* buffer = new char[Stream->size()];
        Stream->read((void*)buffer, Stream->size());
        if(!Importer.loadFileFromMemory(buffer, Stream->size()))
        {
            MEZZ_EXCEPTION(Exception::IO_UNKNOWN_EXCEPTION,"Failed to load file: " + FileName + ".")
        }
        delete[] buffer;
        for( Whole X = 0 ; X < Importer.getNumCollisionShapes() ; ++X )
        {
            btCollisionShape* Shape = Importer.getCollisionShapeByIndex(X);
            const char* MaybeAName = Importer.getNameForPointer((void*)Shape);
            String Name;
            if(MaybeAName)
            {
                Name = String(MaybeAName);
                CollisionShapeManager::iterator it = CollisionShapes.find(Name);
                if(it == CollisionShapes.end())
                {
                    CollisionShape* NewShape = WrapShape(Name,Shape);
                    CollisionShapes.insert(std::pair<String,CollisionShape*>(Name,NewShape));
                }
            }else{
                static Whole NameCount = 0;
                Name = String("Unnamed")+=ToString(NameCount++);
                CollisionShape* NewShape = WrapShape(Name,Shape);
                UnnamedShapes.insert(NewShape);
            }
        }
    }

    void CollisionShapeManager::SaveAllStoredShapesToFile(const String& FileName)
    {
        btDefaultSerializer* BulletSerializer = new btDefaultSerializer(1024*1024*5);
        BulletSerializer->startSerialization();
        for( std::map<String,CollisionShape*>::iterator it = CollisionShapes.begin() ; it != CollisionShapes.end() ; it++ )
        {
            CollisionShape* Shape = (*it).second;

            std::stringstream logstream;
            logstream << "Serializing Shape: " << Shape->GetName();
            World::GetWorldPointer()->Log(logstream.str());
            World::GetWorldPointer()->DoMainLoopLogging();

            BulletSerializer->registerNameForPointer((void*)Shape->GetBulletShape(),(*it).first.c_str());
            int len = Shape->GetBulletShape()->calculateSerializeBufferSize();
            btChunk* chunk = BulletSerializer->allocate(len,1);
            const char* structType = Shape->GetBulletShape()->serialize(chunk->m_oldPtr, BulletSerializer);
            BulletSerializer->finalizeChunk(chunk,structType,BT_SHAPE_CODE,Shape->GetBulletShape());
        }
        BulletSerializer->finishSerialization();
        FILE* f2 = fopen(FileName.c_str(),"wb");
        fwrite(BulletSerializer->getBufferPointer(),BulletSerializer->getCurrentBufferSize(),1,f2);
        fclose(f2);
    }

    void CollisionShapeManager::SaveShapesToFile(const String& FileName, std::vector<CollisionShape*>& ShapesToSave)
    {
        btDefaultSerializer* BulletSerializer = new btDefaultSerializer(1024*1024*5);
        BulletSerializer->startSerialization();
        for( std::vector<CollisionShape*>::iterator it = ShapesToSave.begin() ; it != ShapesToSave.end() ; it++ )
        {
            CollisionShape* Shape = (*it);
            BulletSerializer->registerNameForPointer((void*)Shape->GetBulletShape(),(*it)->GetName().c_str());
            int len = Shape->GetBulletShape()->calculateSerializeBufferSize();
            btChunk* chunk = BulletSerializer->allocate(len,1);
            const char* structType = Shape->GetBulletShape()->serialize(chunk->m_oldPtr, BulletSerializer);
            BulletSerializer->finalizeChunk(chunk,structType,BT_SHAPE_CODE,Shape->GetBulletShape());
        }
        BulletSerializer->finishSerialization();
        FILE* f2 = fopen(FileName.c_str(),"wb");
        fwrite(BulletSerializer->getBufferPointer(),BulletSerializer->getCurrentBufferSize(),1,f2);
        fclose(f2);
    }

    std::set<CollisionShape*>& CollisionShapeManager::GetUnnamedShapes()
    {
        return UnnamedShapes;
    }

    void CollisionShapeManager::SetNameForUnnamedShape(const String& NewName, CollisionShape* Shape)
    {
        std::set<CollisionShape*>::iterator UnIt = UnnamedShapes.find(Shape);
        if(UnIt == UnnamedShapes.end())
            return;
        CollisionShapeManager::iterator NaIt = CollisionShapes.find(NewName);
        if(NaIt != CollisionShapes.end())
            { MEZZ_EXCEPTION(Exception::II_DUPLICATE_IDENTITY_EXCEPTION,"Attempting to assign non-unique name to an unnamed Collision Shape."); }
        Shape->Name = NewName;
        CollisionShapes[NewName] = Shape;
    }

    void CollisionShapeManager::Initialize()
        { Initialized = true; }

    void CollisionShapeManager::DoMainLoopItems()
        {}

    ManagerBase::ManagerType CollisionShapeManager::GetInterfaceType() const
        { return ManagerBase::CollisionShapeManager; }

    String CollisionShapeManager::GetImplementationTypeName() const
        { return "DefaultCollisionShapeManager"; }

    ///////////////////////////////////////////////////////////////////////////////
    // DefaultCollisionShapeManagerFactory Methods

    DefaultCollisionShapeManagerFactory::DefaultCollisionShapeManagerFactory()
    {
    }

    DefaultCollisionShapeManagerFactory::~DefaultCollisionShapeManagerFactory()
    {
    }

    String DefaultCollisionShapeManagerFactory::GetManagerTypeName() const
    {
        return "DefaultCollisionShapeManager";
    }

    ManagerBase* DefaultCollisionShapeManagerFactory::CreateManager(NameValuePairList& Params)
    {
        if(CollisionShapeManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return CollisionShapeManager::GetSingletonPtr();
        }else return new CollisionShapeManager();
    }

    ManagerBase* DefaultCollisionShapeManagerFactory::CreateManager(xml::Node& XMLNode)
    {
        if(CollisionShapeManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return CollisionShapeManager::GetSingletonPtr();
        }else return new CollisionShapeManager(XMLNode);
    }

    void DefaultCollisionShapeManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
    {
        delete ToBeDestroyed;
    }
}//Mezzanine

#endif
