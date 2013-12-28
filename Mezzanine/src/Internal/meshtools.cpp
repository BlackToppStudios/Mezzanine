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

        void MeshTools::GetMeshInformation( Ogre::Entity *entity,
                                    size_t &vertex_count,
                                    Ogre::Vector3* &vertices,
                                    size_t &index_count,
                                    unsigned long* &indices,
                                    const Ogre::Vector3 &position,
                                    const Ogre::Quaternion &orient,
                                    const Ogre::Vector3 &scale)
        {
            bool added_shared = false;
            size_t current_offset = 0;
            size_t shared_offset = 0;
            size_t next_offset = 0;
            size_t index_offset = 0;
            vertex_count = index_count = 0;

            Ogre::MeshPtr mesh = entity->getMesh();

            bool useSoftwareBlendingVertices = entity->hasSkeleton();

            if (useSoftwareBlendingVertices)
            {                                   //10,000th line of code
                entity->_updateAnimation();
            }

            // Calculate how many vertices and indices we're going to need
            for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
            {
                Ogre::SubMesh* submesh = mesh->getSubMesh( i );

                // We only need to add the shared vertices once
                if(submesh->useSharedVertices)
                {
                    if( !added_shared )
                    {
                        vertex_count += mesh->sharedVertexData->vertexCount;
                        added_shared = true;
                    }
                }
                else
                    { vertex_count += submesh->vertexData->vertexCount; }

                // Add the indices
                index_count += submesh->indexData->indexCount;
            }


            // Allocate space for the vertices and indices
            vertices = new Ogre::Vector3[vertex_count];
            indices = new unsigned long[index_count];

            added_shared = false;

            // Run through the submeshes again, adding the data into the arrays
            for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
            {
                Ogre::SubMesh* submesh = mesh->getSubMesh(i);
                Ogre::VertexData* vertex_data;
                if(useSoftwareBlendingVertices) //When there is animation:
                    { vertex_data = submesh->useSharedVertices ? entity->_getSkelAnimVertexData() : entity->getSubEntity(i)->_getSkelAnimVertexData(); }
                else
                    { vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData; }

                if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
                {
                    if(submesh->useSharedVertices)
                    {
                        added_shared = true;
                        shared_offset = current_offset;
                    }

                    const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
                    Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
                    unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

                    // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
                    //  as second argument. So make it float, to avoid trouble when Ogre::Real will
                    //  be comiled/typedefed as double:
                    //      Ogre::Real* pReal;
                    float* pReal;

                    for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
                    {
                        posElem->baseVertexPointerToElement(vertex, &pReal);
                        Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
                        vertices[current_offset + j] = (orient * (pt * scale)) + position;
                    }

                    vbuf->unlock();
                    next_offset += vertex_data->vertexCount;
                }


                Ogre::IndexData* index_data = submesh->indexData;
                size_t numTris = index_data->indexCount / 3;
                Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

                bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

                unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

                size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
                size_t index_start = index_data->indexStart;
                size_t last_index = numTris*3 + index_start;

                if (use32bitindexes)
                {
                    for (size_t k = index_start; k < last_index; ++k)
                        { indices[index_offset++] = pLong[k] + static_cast<unsigned long>( offset ); }
                }else{
                    for (size_t k = index_start; k < last_index; ++k)
                        { indices[ index_offset++ ] = static_cast<unsigned long>( pShort[k] ) + static_cast<unsigned long>( offset ); }
                }

                ibuf->unlock();
                current_offset = next_offset;
            }
        }
    }//internal
}//Mezzanine

/// @endcond

#endif
