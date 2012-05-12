//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _internalmeshtools_cpp
#define _internalmeshtools_cpp

// Keeps this file form being documented by doxygen
/// @cond 0

#include "Internal/meshtools.h.cpp"
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "ConvexBuilder.h"
#include "Internal/decompinterface.h.cpp"

namespace Mezzanine
{
    namespace Internal
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
            Ogre::VertexData* vertexData = subMesh->useSharedVertices ? myMesh->sharedVertexData : subMesh->vertexData;

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
            Ogre::VertexData* vertexData = subMesh->useSharedVertices ? myMesh->sharedVertexData : subMesh->vertexData;

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
            Ogre::VertexData* vertexData = subMesh->useSharedVertices ? myMesh->sharedVertexData : subMesh->vertexData;

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
    }//internal
}//Mezzanine

/// @endcond

#endif
