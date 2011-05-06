//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _internalmeshtools_cpp
#define _internalmeshtools_cpp

// Keeps this file form being documented by doxygen
/// @cond 0

#include "internalmeshtools.h.cpp"
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "ConvexBuilder.h"
#include "internaldecompinterface.h.cpp"

namespace phys{
    namespace internal
    {
        MeshInfo::MeshInfo()
        {
            Verticies = NULL;
            Indicies = NULL;
            Normals = NULL;
            Textures = NULL;
            VCount = 0;
            ICount = 0;
        }

        MeshInfo::~MeshInfo()
        {
            delete[] Verticies;
            delete[] Indicies;
            delete[] Normals;
            delete[] Textures;
        }

        MeshTools::MeshTools()
        {
        }

        MeshTools::~MeshTools()
        {
        }

        void MeshTools::GetMeshVerticies(Ogre::Entity* TheEntity, MeshInfo &TheMesh)
        {
            Ogre::MeshPtr myMesh = TheEntity->getMesh();
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

        void MeshTools::GetMeshIndicies(Ogre::Entity* TheEntity, MeshInfo &TheMesh)
        {
            Ogre::MeshPtr myMesh = TheEntity->getMesh();
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

        void MeshTools::GetMeshNormals(Ogre::Entity* TheEntity, MeshInfo &TheMesh)
        {
            Ogre::MeshPtr myMesh = TheEntity->getMesh();
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

        void MeshTools::GetMeshTextures(Ogre::Entity* TheEntity, MeshInfo &TheMesh)
        {
            Ogre::MeshPtr myMesh = TheEntity->getMesh();
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

        void MeshTools::GetOtherMeshInfo(Ogre::Entity* TheEntity, MeshInfo &TheMesh)
        {
            // Entity Name
            TheMesh.Name = TheEntity->getName();
            // Material Name
            Ogre::MeshPtr myMesh = TheEntity->getMesh();
            Ogre::SubMesh* subMesh = myMesh->getSubMesh(0);
            TheMesh.Material = subMesh->getMaterialName();
            // Material File Name
            Ogre::ResourcePtr Mat = Ogre::MaterialManager::getSingleton().getByName(TheMesh.Material);
            TheMesh.MaterialFile = Mat.getPointer()->getOrigin();
            // Group Name
            TheMesh.Group = myMesh.getPointer()->getGroup();
            // Render Operation
            Ogre::RenderOperation Render;
            subMesh->_getRenderOperation(Render);
            TheMesh.RendOp = Render.operationType;
        }

        btTriangleMesh* MeshTools::CreateBulletTrimesh(Ogre::Entity* TheEntity, bool UseAllSubmeshes)
        {
            Ogre::MeshPtr myMesh = TheEntity->getMesh();
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

            if(UseAllSubmeshes)
            {
                for( Whole X = 0 ; X < myMesh->getNumSubMeshes() ; X++ )
                {
                    vCount+=myMesh->getSubMesh(X)->vertexData->vertexCount;
                    iCount+=myMesh->getSubMesh(X)->indexData->indexCount;
                }
            }else{
                vCount+=myMesh->getSubMesh(0)->vertexData->vertexCount;
                iCount+=myMesh->getSubMesh(0)->indexData->indexCount;
            }

            vertices = new Ogre::Vector3[vCount];
            indices  = new unsigned long[iCount];

            // Get the submesh and associated data
            for( unsigned short int SubMeshIndex = 0 ; SubMeshIndex < myMesh->getNumSubMeshes() ; SubMeshIndex++ )
            {
                if(!UseAllSubmeshes && (SubMeshIndex > 0))
                    break;

                subMesh = myMesh->getSubMesh(SubMeshIndex);
                IndexData = subMesh->indexData;
                VertexData = subMesh->vertexData;

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

        btCompoundShape* MeshTools::PerformConvexDecomposition(Ogre::Entity* TheEntity, unsigned int depth, float cpercent, float ppercent, bool UseAllSubmeshes)
        {
            //new submesh friendly code
            Ogre::MeshPtr myMesh = TheEntity->getMesh();
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

            Whole* VertPerSubMesh = NULL;

            if(UseAllSubmeshes)
            {
                VertPerSubMesh = new Whole[myMesh->getNumSubMeshes()];
                for( Whole X = 0 ; X < myMesh->getNumSubMeshes() ; X++ )
                {
                    vCount+=myMesh->getSubMesh(X)->vertexData->vertexCount;
                    iCount+=myMesh->getSubMesh(X)->indexData->indexCount;
                    VertPerSubMesh[X] = myMesh->getSubMesh(X)->vertexData->vertexCount;
                }
            }else{
                vCount+=myMesh->getSubMesh(0)->vertexData->vertexCount;
                iCount+=myMesh->getSubMesh(0)->indexData->indexCount;
            }

            Ogre::Vector3* vertices = new Ogre::Vector3[vCount];
            unsigned int* indices  = new unsigned int[iCount];

            for( unsigned short int SubMeshIndex = 0 ; SubMeshIndex < myMesh->getNumSubMeshes() ; SubMeshIndex++ )
            {
                if(!UseAllSubmeshes && (SubMeshIndex > 0))
                    break;

                subMesh = myMesh->getSubMesh(SubMeshIndex);
                indexData = subMesh->indexData;
                vertexData = subMesh->vertexData;

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
            return compound;
        }
    }//internal
}//phys

/// @endcond

#endif
