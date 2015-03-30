// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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

#include "Physics/collisionshapemanager.h"
#include "Graphics/mesh.h"
#include "Graphics/meshmanager.h"
#include "Resource/resourcemanager.h"
#include "Resource/filestream.h"

#include "Physics/collisionshape.h"
#include "Physics/boxcollisionshape.h"
#include "Physics/capsulecollisionshape.h"
#include "Physics/conecollisionshape.h"
#include "Physics/convexhullcollisionshape.h"
#include "Physics/cylindercollisionshape.h"
#include "Physics/multispherecollisionshape.h"
#include "Physics/spherecollisionshape.h"
#include "Physics/dynamicmeshcollisionshape.h"
#include "Physics/heightfieldcollisionshape.h"
#include "Physics/planecollisionshape.h"
#include "Physics/softcollisionshape.h"
#include "Physics/staticmeshcollisionshape.h"
#include "Physics/compoundcollisionshape.h"

// For logging
#include "stringtool.h"
#include "entresol.h"

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
    template<> Physics::CollisionShapeManager* Singleton<Physics::CollisionShapeManager>::SingletonPtr = NULL;

    namespace Physics
    {
        //template<> CollisionShapeManager* Singleton<CollisionShapeManager>::SingletonPtr = NULL;
        const String CollisionShapeManager::ImplementationName = "DefaultCollisionShapeManager";
        const ManagerBase::ManagerType CollisionShapeManager::InterfaceType = ManagerBase::MT_CollisionShapeManager;

        CollisionShapeManager::CollisionShapeManager()
            {  }

        CollisionShapeManager::CollisionShapeManager(XML::Node& XMLNode)
        {
            /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.
        }

        CollisionShapeManager::~CollisionShapeManager()
        {
            this->Deinitialize();
            this->DestroyAllShapes();
        }

        btTriangleMesh* CollisionShapeManager::CreateBulletTrimesh(Graphics::Mesh* ObjectMesh, Boole UseAllSubmeshes)
        {
            Ogre::MeshPtr myMesh = ObjectMesh->_GetInternalMesh();
            Ogre::SubMesh* subMesh = NULL;
            Ogre::IndexData*  IndexData = NULL;
            Ogre::VertexData* VertexData = NULL;
            Boole use32bitindexes = false;
            unsigned int triCount = 0;
            unsigned int vCount = 0;
            unsigned int iCount = 0;
            Whole VertPrevSize = 0;
            Whole IndiPrevSize = 0;
            Ogre::Vector3* vertices = NULL;
            unsigned long* indices = NULL;
            Boole SharedVerts = myMesh->getSubMesh(0)->useSharedVertices;

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
                if( !UseAllSubmeshes && ( SubMeshIndex > 0 ) )
                    break;
                if( SharedVerts && (SubMeshIndex > 0) )
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
                triCount += ( IndexData->indexCount / 3 );

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

                if( use32bitindexes )
                {
                    for (size_t k = 0; k < triCount*3; ++k)
                    {
                        indices[index_offset+IndiPrevSize] = pLong[k];
                        index_offset++;
                    }
                }else{
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
                    SoftCollisionShape* SoftBodyShape = new SoftCollisionShape(Name,(btSoftBodyCollisionShape*)InternalShape);
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

        ///////////////////////////////////////////////////////////////////////////////
        // Generic Shape Management

        void CollisionShapeManager::StoreShape(CollisionShape* Shape)
        {
            ShapeMapIterator CS = this->CollisionShapes.find(Shape->GetName());
            if(CS != this->CollisionShapes.end())
            {
                if((*CS).second != Shape)
                {
                    MEZZ_EXCEPTION(Exception::II_DUPLICATE_IDENTITY_EXCEPTION,"Name of Collision Shape already exists on another object.  Names should be Unique.");
                }
            }else{
                this->CollisionShapes[Shape->GetName()] = Shape;
            }
        }

        CollisionShape* CollisionShapeManager::GetShape(const String& Name)
        {
            ShapeMapIterator CS = this->CollisionShapes.find(Name);
            if(CS == this->CollisionShapes.end()) return NULL;
            else return (*CS).second;
        }

        Whole CollisionShapeManager::GetNumStoredShapes()
        {
            return this->CollisionShapes.size();
        }

        void CollisionShapeManager::RemoveShape(CollisionShape* Shape)
        {
            this->RemoveShape( Shape->GetName() );
        }

        void CollisionShapeManager::RemoveShape(const String& Name)
        {
            ShapeMapIterator CS = this->CollisionShapes.find(Name);
            if(CS == this->CollisionShapes.end())
                return;
            this->CollisionShapes.erase(CS);
        }

        void CollisionShapeManager::RemoveAllShapes()
        {
            this->CollisionShapes.clear();
        }

        void CollisionShapeManager::DestroyShape(CollisionShape* Shape)
        {
            this->DestroyShape(Shape->GetName());
        }

        void CollisionShapeManager::DestroyShape(const String& Name)
        {
            ShapeMapIterator CS = this->CollisionShapes.find(Name);
            if(CS == this->CollisionShapes.end())
                return;
            delete (*CS).second;
            this->CollisionShapes.erase(CS);
        }

        void CollisionShapeManager::DestroyAllShapes()
        {
            for( ShapeMapIterator CS = this->CollisionShapes.begin() ; CS != this->CollisionShapes.end() ; CS++ )
            {
                CollisionShape* CurrShape = (*CS).second;
                delete CurrShape;
            }
            this->CollisionShapes.clear();
        }

        CollisionShapeManager::ShapeMapIterator CollisionShapeManager::BeginCollisionShape()
            { return this->CollisionShapes.begin(); }

        CollisionShapeManager::ShapeMapIterator CollisionShapeManager::EndCollisionShape()
            { return this->CollisionShapes.end(); }

        CollisionShapeManager::ConstShapeMapIterator CollisionShapeManager::BeginCollisionShape() const
            { return this->CollisionShapes.begin(); }

        CollisionShapeManager::ConstShapeMapIterator CollisionShapeManager::EndCollisionShape() const
            { return this->CollisionShapes.end(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Shape Creation Utilities

        ConvexHullCollisionShape* CollisionShapeManager::GenerateConvexHull(const String& Name, Graphics::Mesh* ObjectMesh, Boole UseAllSubmeshes)
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

        ConvexHullCollisionShape* CollisionShapeManager::GenerateConvexHull(const String& Name, const String& MeshName, const String& Group, Boole UseAllSubmeshes)
        {
            Graphics::Mesh* ObjectMesh = Graphics::MeshManager::GetSingletonPtr()->LoadMesh(MeshName,Group);
            return this->GenerateConvexHull(Name,ObjectMesh,UseAllSubmeshes);
        }

        DynamicMeshCollisionShape* CollisionShapeManager::GenerateDynamicTriMesh(const String& Name, Graphics::Mesh* ObjectMesh, Boole UseAllSubmeshes)
        {
            btGImpactMeshShape* gimpact = new btGImpactMeshShape(this->CreateBulletTrimesh(ObjectMesh,UseAllSubmeshes));
            return new DynamicMeshCollisionShape(Name,gimpact);
        }

        DynamicMeshCollisionShape* CollisionShapeManager::GenerateDynamicTriMesh(const String& Name, const String& MeshName, const String& Group, Boole UseAllSubmeshes)
        {
            Graphics::Mesh* ObjectMesh = Graphics::MeshManager::GetSingletonPtr()->LoadMesh(MeshName,Group);
            return this->GenerateDynamicTriMesh(Name,ObjectMesh,UseAllSubmeshes);
        }

        StaticMeshCollisionShape* CollisionShapeManager::GenerateStaticTriMesh(const String& Name, Graphics::Mesh* ObjectMesh, Boole UseAllSubmeshes)
        {
            btBvhTriangleMeshShape* tmpshape = new btBvhTriangleMeshShape(this->CreateBulletTrimesh(ObjectMesh,UseAllSubmeshes),true);
            return new StaticMeshCollisionShape(Name,tmpshape);
        }

        StaticMeshCollisionShape* CollisionShapeManager::GenerateStaticTriMesh(const String& Name, const String& MeshName, const String& Group, Boole UseAllSubmeshes)
        {
            Graphics::Mesh* ObjectMesh = Graphics::MeshManager::GetSingletonPtr()->LoadMesh(MeshName,Group);
            return this->GenerateStaticTriMesh(Name,ObjectMesh,UseAllSubmeshes);
        }

        CompoundCollisionShape* CollisionShapeManager::PerformConvexDecomposition(const String& Name, Graphics::Mesh* ObjectMesh, Whole Depth, Real CPercent, Real PPercent, Boole UseAllSubmeshes)
        {
            //new submesh friendly code
            Ogre::MeshPtr myMesh = ObjectMesh->_GetInternalMesh();
            Ogre::SubMesh* subMesh = NULL;
            Ogre::IndexData*  indexData = NULL;
            Ogre::VertexData* vertexData = NULL;
            Boole use32bitindexes = false;
            unsigned int currtriCount = 0;
            unsigned int triCount = 0;
            unsigned int vCount = 0;
            unsigned int iCount = 0;
            Whole VertPrevSize = 0;
            Whole IndiPrevSize = 0;
            Boole SharedVerts = myMesh->getSubMesh(0)->useSharedVertices;

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
                if( !UseAllSubmeshes && (SubMeshIndex > 0) )
                    break;
                if( SharedVerts && (SubMeshIndex > 0) )
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
                for( size_t j = 0 ; j < vertexData->vertexCount ; j++, vertex += vBuffer->getVertexSize() )
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

                if( use32bitindexes )
                {
                    for (size_t k = 0; k < currtriCount*3; ++k)
                    {
                        if(SubMeshIndex > 0 && VertPerSubMesh) {
                            indices[index_offset+IndiPrevSize] = pLong[k] + VertPerSubMesh[SubMeshIndex];
                        }else{
                            indices[index_offset+IndiPrevSize] = pLong[k];
                        }
                        index_offset++;
                    }
                }else{
                    for( size_t k = 0 ; k < currtriCount * 3 ; ++k )
                    {
                        if(SubMeshIndex > 0 && VertPerSubMesh) {
                            indices[index_offset+IndiPrevSize] = (static_cast<unsigned long>(pShort[k])) + VertPerSubMesh[SubMeshIndex];
                        }else{
                            indices[index_offset+IndiPrevSize] = static_cast<unsigned long>(pShort[k]);
                        }
                        index_offset++;
                    }
                }
                iBuffer->unlock();

                VertPrevSize += vertexData->vertexCount;
                IndiPrevSize += indexData->indexCount;
            }

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
            delete[] vertices;
            delete[] indices;
            delete[] VertPerSubMesh;
            return compound;
        }

        CompoundCollisionShape* CollisionShapeManager::PerformConvexDecomposition(const String& Name, const String& MeshName, const String& Group, Whole Depth, Real CPercent, Real PPercent, Boole UseAllSubmeshes)
        {
            Graphics::Mesh* ObjectMesh = Graphics::MeshManager::GetSingletonPtr()->LoadMesh(MeshName,Group);
            return this->PerformConvexDecomposition(Name,ObjectMesh,Depth,CPercent,PPercent,UseAllSubmeshes);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Shape Saving/Loading Utilities

        void CollisionShapeManager::LoadAllShapesFromXMLFile(const String& FileName, const String& Group)
        {
            /// @todo Replace this stack allocated stream for one initialized from the Resource Manager, after the system is ready.
            Resource::FileStream ShapesStream( FileName, Resource::ResourceManager::GetSingletonPtr()->GetAssetPath(FileName,Group) );
            XML::Document ShapesDoc;
            XML::ParseResult DocResult = ShapesDoc.Load(ShapesStream);
            if( DocResult.Status != XML::StatusOk ) {
                MEZZ_EXCEPTION(Exception::SYNTAX_ERROR_EXCEPTION_XML,"Failed to parse XML file \"" + FileName + "\".");
            }
            XML::Node ShapesRoot = ShapesDoc.GetChild("InitializerRoot");
            if( ShapesRoot.Empty() ) {
                MEZZ_EXCEPTION(Exception::SYNTAX_ERROR_EXCEPTION_XML,"Failed to find expected Root node in \"" + FileName + "\".");
            }

            for( XML::NodeIterator ShapeIt = ShapesRoot.begin() ; ShapeIt != ShapesRoot.end() ; ++ShapeIt )
            {
                CollisionShape* DeSerializedShape = Physics::CreateShape( (*ShapeIt) );
                this->CollisionShapes.insert( std::pair<String,CollisionShape*>(DeSerializedShape->GetName(),DeSerializedShape) );
            }
        }

        void CollisionShapeManager::SaveAllStoredShapesToXMLFile(const String& FileName)
        {
            XML::Document ShapesDoc;
            XML::Node DeclNode = ShapesDoc.AppendChild(XML::NodeDeclaration);
            XML::Attribute VerAttrib = DeclNode.AppendAttribute("version");

            if( DeclNode.SetName("xml") && VerAttrib.SetValue("1.0") ) {
                XML::Node ShapesRoot = ShapesDoc.AppendChild( "ShapesRoot" );
                for( ShapeMapIterator ShapeIt = this->CollisionShapes.begin() ; ShapeIt != this->CollisionShapes.end() ; ++ShapeIt )
                {
                    (*ShapeIt).second->ProtoSerialize( ShapesRoot );
                }

                /// @todo Replace this stack allocated stream for one initialized from the Resource Manager, after the system is ready.
                Resource::FileStream SettingsStream(FileName,".",Resource::SF_Truncate | Resource::SF_Write);
                ShapesDoc.Save(SettingsStream,"\t",XML::FormatIndent);
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Failed to create XML document declaration for file \"" + FileName + "\".");
            }
        }

        void CollisionShapeManager::SaveShapesToXMLFile(const String& FileName, ShapeVector& ShapesToSave)
        {
            XML::Document ShapesDoc;
            XML::Node DeclNode = ShapesDoc.AppendChild(XML::NodeDeclaration);
            XML::Attribute VerAttrib = DeclNode.AppendAttribute("version");

            if( DeclNode.SetName("xml") && VerAttrib.SetValue("1.0") ) {
                XML::Node ShapesRoot = ShapesDoc.AppendChild( "ShapesRoot" );
                for( ShapeVectorIterator ShapeIt = ShapesToSave.begin() ; ShapeIt != ShapesToSave.end() ; ++ShapeIt )
                {
                    (*ShapeIt)->ProtoSerialize( ShapesRoot );
                }

                /// @todo Replace this stack allocated stream for one initialized from the Resource Manager, after the system is ready.
                Resource::FileStream SettingsStream(FileName,".",Resource::SF_Truncate | Resource::SF_Write);
                ShapesDoc.Save(SettingsStream,"\t",XML::FormatIndent);
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Failed to create XML document declaration for file \"" + FileName + "\".");
            }
        }

        void CollisionShapeManager::LoadAllShapesFromBinaryFile(const String& FileName, const String& Group)
        {
            btBulletWorldImporter Importer;
            Ogre::DataStreamPtr Stream = Ogre::ResourceGroupManager::getSingleton().openResource(FileName,Group);
            char* buffer = new char[Stream->size()];
            Stream->read((void*)buffer, Stream->size());
            if(!Importer.loadFileFromMemory(buffer, Stream->size()))
            {
                MEZZ_EXCEPTION(Exception::IO_FILE_EXCEPTION,"Failed to load file: " + FileName + ".")
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
                    ShapeMapIterator it = this->CollisionShapes.find(Name);
                    if(it == this->CollisionShapes.end())
                    {
                        CollisionShape* NewShape = this->WrapShape(Name,Shape);
                        this->CollisionShapes.insert( std::pair<String,CollisionShape*>(Name,NewShape) );
                    }
                }else{
                    static Whole NameCount = 0;
                    Name = String("Unnamed") += StringTools::ConvertToString(NameCount++);
                    CollisionShape* NewShape = this->WrapShape(Name,Shape);
                    this->UnnamedShapes.push_back(NewShape);
                }
            }
        }

        void CollisionShapeManager::SaveAllStoredShapesToBinaryFile(const String& FileName)
        {
            btDefaultSerializer* BulletSerializer = new btDefaultSerializer(1024*1024*5);
            BulletSerializer->startSerialization();
            for( ShapeMapIterator it = this->CollisionShapes.begin() ; it != this->CollisionShapes.end() ; it++ )
            {
                CollisionShape* Shape = (*it).second;
                BulletSerializer->registerNameForPointer((void*)Shape->_GetInternalShape(),(*it).first.c_str());
                int len = Shape->_GetInternalShape()->calculateSerializeBufferSize();
                btChunk* chunk = BulletSerializer->allocate(len,1);
                const char* structType = Shape->_GetInternalShape()->serialize(chunk->m_oldPtr, BulletSerializer);
                BulletSerializer->finalizeChunk(chunk,structType,BT_SHAPE_CODE,Shape->_GetInternalShape());
            }
            BulletSerializer->finishSerialization();
            FILE* f2 = fopen(FileName.c_str(),"wb");
            fwrite(BulletSerializer->getBufferPointer(),BulletSerializer->getCurrentBufferSize(),1,f2);
            fclose(f2);
        }

        void CollisionShapeManager::SaveShapesToBinaryFile(const String& FileName, ShapeVector& ShapesToSave)
        {
            btDefaultSerializer* BulletSerializer = new btDefaultSerializer(1024*1024*5);
            BulletSerializer->startSerialization();
            for( ShapeVectorIterator it = ShapesToSave.begin() ; it != ShapesToSave.end() ; it++ )
            {
                Physics::CollisionShape* Shape = (*it);
                BulletSerializer->registerNameForPointer((void*)Shape->_GetInternalShape(),(*it)->GetName().c_str());
                int len = Shape->_GetInternalShape()->calculateSerializeBufferSize();
                btChunk* chunk = BulletSerializer->allocate(len,1);
                const char* structType = Shape->_GetInternalShape()->serialize(chunk->m_oldPtr, BulletSerializer);
                BulletSerializer->finalizeChunk(chunk,structType,BT_SHAPE_CODE,Shape->_GetInternalShape());
            }
            BulletSerializer->finishSerialization();
            FILE* f2 = fopen(FileName.c_str(),"wb");
            fwrite(BulletSerializer->getBufferPointer(),BulletSerializer->getCurrentBufferSize(),1,f2);
            fclose(f2);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Unnamed Shape Management

        CollisionShapeManager::ShapeVector& CollisionShapeManager::GetUnnamedShapes()
        {
            return this->UnnamedShapes;
        }

        void CollisionShapeManager::SetNameForUnnamedShape(const String& NewName, CollisionShape* Shape)
        {
            for( ShapeVectorIterator ShapeIt = this->UnnamedShapes.begin() ; ShapeIt != this->UnnamedShapes.end() ; ++ShapeIt )
            {
                if( (*ShapeIt) == Shape ) {
                    this->UnnamedShapes.erase( ShapeIt );
                    break;
                }
            }

            ShapeMapIterator NaIt = this->CollisionShapes.find(NewName);
            if(NaIt != this->CollisionShapes.end()) {
                MEZZ_EXCEPTION(Exception::II_DUPLICATE_IDENTITY_EXCEPTION,"Attempting to assign non-unique name to an unnamed Collision Shape.");
            }

            Shape->_SetShapeName(NewName);
            this->CollisionShapes.insert( std::pair<String,CollisionShape*>(NewName,Shape) );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void CollisionShapeManager::Initialize()
            { Initialized = true; }

        void CollisionShapeManager::Deinitialize()
            { Initialized = false; }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        ManagerBase::ManagerType CollisionShapeManager::GetInterfaceType() const
            { return CollisionShapeManager::InterfaceType; }

        String CollisionShapeManager::GetImplementationTypeName() const
            { return CollisionShapeManager::ImplementationName; }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultCollisionShapeManagerFactory Methods

        DefaultCollisionShapeManagerFactory::DefaultCollisionShapeManagerFactory()
            {  }

        DefaultCollisionShapeManagerFactory::~DefaultCollisionShapeManagerFactory()
            {  }

        String DefaultCollisionShapeManagerFactory::GetManagerImplName() const
            { return CollisionShapeManager::ImplementationName; }

        ManagerBase::ManagerType DefaultCollisionShapeManagerFactory::GetManagerType() const
            { return CollisionShapeManager::InterfaceType; }

        EntresolManager* DefaultCollisionShapeManagerFactory::CreateManager(NameValuePairList& Params)
        {
            if( CollisionShapeManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return CollisionShapeManager::GetSingletonPtr();
            }else return new CollisionShapeManager();
        }

        EntresolManager* DefaultCollisionShapeManagerFactory::CreateManager(XML::Node& XMLNode)
        {
            if( CollisionShapeManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return CollisionShapeManager::GetSingletonPtr();
            }else return new CollisionShapeManager(XMLNode);
        }

        void DefaultCollisionShapeManagerFactory::DestroyManager(EntresolManager* ToBeDestroyed)
            { delete ToBeDestroyed; }
    }//Physics
}//Mezzanine

#endif
