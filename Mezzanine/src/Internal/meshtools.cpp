// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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

/// @cond DontDocumentInternal

#include "Internal/meshtools.h.cpp"
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "ConvexBuilder.h"
#include "Internal/decompinterface.h.cpp"

namespace Mezzanine
{
    namespace Internal
    {
        namespace MeshTools
        {
            ///////////////////////////////////////////////////////////////////////////////
            // Simple Utility

            Whole GetVertexCount(Ogre::MeshPtr TheMesh)
            {
                //if( TheMesh->sharedVertexData ) {
                if( TheMesh->getSubMesh(0)->useSharedVertices ) {
                    return TheMesh->sharedVertexData->vertexCount;
                }

                Whole Ret = 0;
                Whole SubMeshCount = TheMesh->getNumSubMeshes();
                for( Whole SubMeshIndex = 0 ; SubMeshIndex < SubMeshCount ; ++SubMeshIndex )
                {
                    Ret += TheMesh->getSubMesh(SubMeshIndex)->vertexData->vertexCount;
                }
                return Ret;
            }

            Whole GetIndexCount(Ogre::MeshPtr TheMesh)
            {
                Whole Ret = 0;
                Whole SubMeshCount = TheMesh->getNumSubMeshes();
                for( Whole SubMeshIndex = 0 ; SubMeshIndex < SubMeshCount ; ++SubMeshIndex )
                {
                    Ret += TheMesh->getSubMesh(SubMeshIndex)->indexData->indexCount;
                }
                return Ret;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Multi SubMesh Query

            Vector3Vec GetVertexPositions(Ogre::MeshPtr TheMesh)
            {
                Whole SubMeshCount = TheMesh->getNumSubMeshes();
                if( SubMeshCount == 1 ) {
                    return GetSubMeshVertexPositions(TheMesh,0);
                }else{
                    Vector3Vec Ret;
                    for( Whole SubMeshIndex = 0 ; SubMeshIndex < SubMeshCount ; ++SubMeshIndex )
                    {
                        Vector3Vec SubMeshResult = GetSubMeshVertexPositions(TheMesh,SubMeshIndex);
                        Ret.insert(Ret.end(),SubMeshResult.begin(),SubMeshResult.end());
                    }
                    return Ret;
                }
            }

            Vector3Vec GetVertexNormals(Ogre::MeshPtr TheMesh)
            {
                Whole SubMeshCount = TheMesh->getNumSubMeshes();
                if( SubMeshCount == 1 ) {
                    return GetSubMeshVertexNormals(TheMesh,0);
                }else{
                    Vector3Vec Ret;
                    for( Whole SubMeshIndex = 0 ; SubMeshIndex < SubMeshCount ; ++SubMeshIndex )
                    {
                        Vector3Vec SubMeshResult = GetSubMeshVertexNormals(TheMesh,SubMeshIndex);
                        Ret.insert(Ret.end(),SubMeshResult.begin(),SubMeshResult.end());
                    }
                    return Ret;
                }
            }

            Vector2Vec GetVertexUVs(Ogre::MeshPtr TheMesh)
            {
                Whole SubMeshCount = TheMesh->getNumSubMeshes();
                if( SubMeshCount == 1 ) {
                    return GetSubMeshVertexUVs(TheMesh,0);
                }else{
                    Vector2Vec Ret;
                    for( Whole SubMeshIndex = 0 ; SubMeshIndex < SubMeshCount ; ++SubMeshIndex )
                    {
                        Vector2Vec SubMeshResult = GetSubMeshVertexUVs(TheMesh,SubMeshIndex);
                        Ret.insert(Ret.end(),SubMeshResult.begin(),SubMeshResult.end());
                    }
                    return Ret;
                }
            }

            IntVec GetIndexes(Ogre::MeshPtr TheMesh)
            {
                Whole SubMeshCount = TheMesh->getNumSubMeshes();
                if( SubMeshCount == 1 ) {
                    return GetSubMeshIndexes(TheMesh,0);
                }else{
                    IntVec Ret;
                    for( Whole SubMeshIndex = 0 ; SubMeshIndex < SubMeshCount ; ++SubMeshIndex )
                    {
                        IntVec SubMeshResult = GetSubMeshIndexes(TheMesh,SubMeshIndex);
                        Ret.insert(Ret.end(),SubMeshResult.begin(),SubMeshResult.end());
                    }
                    return Ret;
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Single SubMesh Query

            Vector3Vec GetSubMeshVertexPositions(Ogre::MeshPtr TheMesh, const Whole SubMeshIndex)
            {
                Ogre::SubMesh* subMesh = TheMesh->getSubMesh(SubMeshIndex);
                Ogre::VertexData* vertexData = subMesh->useSharedVertices ? TheMesh->sharedVertexData : subMesh->vertexData;

                const Ogre::VertexElement* posElem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
                Ogre::HardwareVertexBufferSharedPtr vBuffer = vertexData->vertexBufferBinding->getBuffer(posElem->getSource());

                Vector3Vec Ret;
                Ret.resize(vertexData->vertexCount);

                float* pReal = NULL;
                unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                for( size_t j = 0; j < vertexData->vertexCount ; j++, vertex += vBuffer->getVertexSize() )
                {
                    posElem->baseVertexPointerToElement(vertex, &pReal);
                    Ret[j].x = *pReal++;
                    Ret[j].y = *pReal++;
                    Ret[j].z = *pReal++;
                }
                vBuffer->unlock();
                return Ret;
            }

            Vector3Vec GetSubMeshVertexNormals(Ogre::MeshPtr TheMesh, const Whole SubMeshIndex)
            {
                Ogre::SubMesh* subMesh = TheMesh->getSubMesh(SubMeshIndex);
                Ogre::VertexData* vertexData = subMesh->useSharedVertices ? TheMesh->sharedVertexData : subMesh->vertexData;

                const Ogre::VertexElement* normElem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_NORMAL);
                Ogre::HardwareVertexBufferSharedPtr vBuffer = vertexData->vertexBufferBinding->getBuffer(normElem->getSource());

                Vector3Vec Ret;
                Ret.resize(vertexData->vertexCount);

                float* pReal = NULL;
                unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                for( size_t j = 0; j < vertexData->vertexCount ; j++, vertex += vBuffer->getVertexSize() )
                {
                    normElem->baseVertexPointerToElement(vertex, &pReal);
                    Ret[j].x = *pReal++;
                    Ret[j].y = *pReal++;
                    Ret[j].z = *pReal++;
                }
                vBuffer->unlock();
                return Ret;
            }

            Vector2Vec GetSubMeshVertexUVs(Ogre::MeshPtr TheMesh, const Whole SubMeshIndex)
            {
                Ogre::SubMesh* subMesh = TheMesh->getSubMesh(SubMeshIndex);
                Ogre::VertexData* vertexData = subMesh->useSharedVertices ? TheMesh->sharedVertexData : subMesh->vertexData;

                const Ogre::VertexElement* uvElem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES);
                Ogre::HardwareVertexBufferSharedPtr vBuffer = vertexData->vertexBufferBinding->getBuffer(uvElem->getSource());

                Vector2Vec Ret;
                Ret.resize(vertexData->vertexCount);

                float* pReal = NULL;
                unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                for( size_t j = 0; j < vertexData->vertexCount ; j++, vertex += vBuffer->getVertexSize() )
                {
                    uvElem->baseVertexPointerToElement(vertex, &pReal);
                    Ret[j].x = *pReal++;
                    Ret[j].y = *pReal++;
                }
                vBuffer->unlock();
                return Ret;
            }

            IntVec GetSubMeshIndexes(Ogre::MeshPtr TheMesh, const Whole SubMeshIndex)
            {
                Ogre::SubMesh* subMesh = TheMesh->getSubMesh(SubMeshIndex);
                Ogre::IndexData* indexData = subMesh->indexData;

                Ogre::HardwareIndexBufferSharedPtr iBuffer = indexData->indexBuffer;

                IntVec Ret;
                Ret.resize(indexData->indexCount);

                Boole use32bitindexes = ( iBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT );

                long* pLong = static_cast<long*>( iBuffer->lock( Ogre::HardwareBuffer::HBL_READ_ONLY ) );
                short* pShort = reinterpret_cast<short*>(pLong);

                if( use32bitindexes ) {
                    for( size_t CurrIndex = 0 ; CurrIndex < indexData->indexCount ; ++CurrIndex )
                        { Ret[CurrIndex] = pLong[CurrIndex]; }
                }else{
                    for( size_t CurrIndex = 0 ; CurrIndex < indexData->indexCount ; ++CurrIndex )
                        { Ret[CurrIndex] = static_cast<unsigned long>( pShort[CurrIndex] ); }
                }
                iBuffer->unlock();
                return Ret;
            }

            String GetSubMeshMaterialName(Ogre::MeshPtr TheMesh, const Whole SubMeshIndex)
            {
                return TheMesh->getSubMesh(SubMeshIndex)->getMaterialName();
            }

            String GetSubMeshMaterialOrigin(Ogre::MeshPtr TheMesh, const Whole SubMeshIndex)
            {
                Ogre::ResourcePtr Mat = Ogre::MaterialManager::getSingleton().getByName( GetSubMeshMaterialName(TheMesh,SubMeshIndex) );
                return Mat->getOrigin();
            }

            Ogre::RenderOperation::OperationType GetSubMeshRenderType(Ogre::MeshPtr TheMesh, const Whole SubMeshIndex)
            {
                Ogre::RenderOperation Render;
                TheMesh->getSubMesh(SubMeshIndex)->_getRenderOperation(Render);
                return Render.operationType;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Interweaved Element Fetch

            ///////////////////////////////////////////////////////////////////////////////
            // Miscellaneous Information

            void GetTransformedMeshData(Ogre::Entity* TheEntity, Vector3Vec& VertexArray, IntVec& IndexArray)
            {
                Ogre::MeshPtr TheMesh = TheEntity->getMesh();

                Boole added_shared = false;
                size_t current_offset = 0;
                size_t shared_offset = 0;
                size_t next_offset = 0;
                size_t index_offset = 0;
                size_t vertex_count = 0;
                size_t index_count = 0;

                Ogre::Vector3 Offset(0,0,0);
                Ogre::Quaternion Orientation(0,0,0,1);
                Ogre::Vector3 Scale(1,1,1);

                Ogre::SceneNode* ParentNode = TheEntity->getParentSceneNode();
                if( ParentNode != NULL ) {
                    Offset = ParentNode->_getDerivedPosition();
                    Orientation = ParentNode->_getDerivedOrientation();
                    Scale = ParentNode->_getDerivedScale();
                }

                Boole useSoftwareBlendingVertices = TheEntity->hasSkeleton();
                if( useSoftwareBlendingVertices ) {
                    TheEntity->_updateAnimation();
                }

                // Calculate how many vertices and indices we're going to need
                for( unsigned short i = 0 ; i < TheMesh->getNumSubMeshes() ; ++i )
                {
                    Ogre::SubMesh* submesh = TheMesh->getSubMesh( i );

                    // We only need to add the shared vertices once
                    if( submesh->useSharedVertices ) {
                        if( !added_shared ) {
                            vertex_count += TheMesh->sharedVertexData->vertexCount;
                            added_shared = true;
                        }
                    }else{
                        vertex_count += submesh->vertexData->vertexCount;
                    }

                    // Add the indices
                    index_count += submesh->indexData->indexCount;
                }

                // Prepare our vectors
                VertexArray.resize(vertex_count);
                IndexArray.resize(index_count);

                added_shared = false;

                // Run through the submeshes again, adding the data into the arrays
                for( unsigned short i = 0 ; i < TheMesh->getNumSubMeshes() ; ++i )
                {
                    Ogre::SubMesh* submesh = TheMesh->getSubMesh(i);
                    Ogre::VertexData* vertex_data;
                    if( useSoftwareBlendingVertices ) //When there is animation:
                        { vertex_data = submesh->useSharedVertices ? TheEntity->_getSkelAnimVertexData() : TheEntity->getSubEntity(i)->_getSkelAnimVertexData(); }
                    else
                        { vertex_data = submesh->useSharedVertices ? TheMesh->sharedVertexData : submesh->vertexData; }

                    if( (!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared) ) {
                        if( submesh->useSharedVertices ) {
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
                            VertexArray[current_offset + j] = (Orientation * (pt * Scale)) + Offset;
                        }

                        vbuf->unlock();
                        next_offset += vertex_data->vertexCount;
                    }


                    Ogre::IndexData* index_data = submesh->indexData;
                    size_t numTris = index_data->indexCount / 3;
                    Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

                    Boole use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

                    unsigned long* pLong = static_cast<unsigned long*>( ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY) );
                    unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

                    size_t offset = (submesh->useSharedVertices) ? shared_offset : current_offset;
                    size_t index_start = index_data->indexStart;
                    size_t last_index = numTris * 3 + index_start;

                    if( use32bitindexes ) {
                        for( size_t k = index_start ; k < last_index ; ++k )
                            { IndexArray[ index_offset++ ] = pLong[k] + static_cast<unsigned long>( offset ); }
                    }else{
                        for( size_t k = index_start ; k < last_index ; ++k )
                            { IndexArray[ index_offset++ ] = static_cast<unsigned long>( pShort[k] ) + static_cast<unsigned long>( offset ); }
                    }

                    ibuf->unlock();
                    current_offset = next_offset;
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Physics Utilities

            btTriangleMesh* CreateBulletTrimesh(Ogre::MeshPtr TheMesh, const Boole UseAllSubmeshes)
            {
                Ogre::SubMesh* subMesh = NULL;
                Ogre::IndexData* indexData = NULL;
                Ogre::VertexData* vertexData = NULL;
                Boole use32bitindexes = false;
                unsigned int triCount = 0;
                unsigned int vCount = 0;
                unsigned int iCount = 0;
                Whole VertPrevSize = 0;
                Whole IndiPrevSize = 0;
                Ogre::Vector3* vertices = NULL;
                unsigned long* indices = NULL;
                Boole SharedVerts = TheMesh->getSubMesh(0)->useSharedVertices;

                if( UseAllSubmeshes ) {
                    for( Whole X = 0 ; X < TheMesh->getNumSubMeshes() ; X++ )
                    {
                        vCount += SharedVerts ? TheMesh->sharedVertexData->vertexCount : TheMesh->getSubMesh(X)->vertexData->vertexCount;
                        iCount += TheMesh->getSubMesh(X)->indexData->indexCount;
                    }
                }else{
                    vCount += SharedVerts ? TheMesh->sharedVertexData->vertexCount : TheMesh->getSubMesh(0)->vertexData->vertexCount;
                    iCount += TheMesh->getSubMesh(0)->indexData->indexCount;
                }

                vertices = new Ogre::Vector3[vCount];
                indices  = new unsigned long[iCount];

                // Get the submesh and associated data
                for( unsigned short int SubMeshIndex = 0 ; SubMeshIndex < TheMesh->getNumSubMeshes() ; SubMeshIndex++ )
                {
                    if( !UseAllSubmeshes && ( SubMeshIndex > 0 ) )
                        break;
                    if( SharedVerts && ( SubMeshIndex > 0 ) )
                        break;

                    subMesh = TheMesh->getSubMesh(SubMeshIndex);
                    indexData = subMesh->indexData;
                    vertexData = SharedVerts ? TheMesh->sharedVertexData : TheMesh->getSubMesh(SubMeshIndex)->vertexData;

                    // Get the position element
                    const Ogre::VertexElement* posElem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
                    // Get a pointer to the vertex buffer
                    Ogre::HardwareVertexBufferSharedPtr vBuffer = vertexData->vertexBufferBinding->getBuffer(posElem->getSource());
                    // Get a pointer to the index buffer
                    Ogre::HardwareIndexBufferSharedPtr iBuffer = indexData->indexBuffer;
                    // Get the number of triangles
                    triCount += (indexData->indexCount/3);

                    // Lock the vertex buffer (READ ONLY)
                    unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                    float* pReal = NULL;
                    for( size_t j = 0 ; j < vertexData->vertexCount ; ++j, vertex += vBuffer->getVertexSize() )
                    {
                        posElem->baseVertexPointerToElement(vertex, &pReal);
                        Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
                        vertices[j + VertPrevSize] = pt;
                    }
                    vBuffer->unlock();
                    size_t index_offset = 0;
                    use32bitindexes = ( iBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT );

                    // Lock the index buffer (READ ONLY)
                    unsigned long* pLong = static_cast<unsigned long*>(iBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                    unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

                    if( use32bitindexes ) {
                        for( size_t k = 0 ; k < triCount * 3 ; ++k )
                        {
                            indices[index_offset+IndiPrevSize] = pLong[k];
                            index_offset++;
                        }
                    }else{
                        for( size_t k = 0 ; k < triCount * 3 ; ++k )
                        {
                            indices[index_offset+IndiPrevSize] = static_cast<unsigned long>(pShort[k]);
                            index_offset++;
                        }
                    }
                    iBuffer->unlock();

                    VertPrevSize += vertexData->vertexCount;
                    IndiPrevSize += indexData->indexCount;
                }

                // We now have vertices and indices ready to go

                // The Bullet triangle mesh
                btTriangleMesh* trimesh = new btTriangleMesh(use32bitindexes);

                // Setup the tri mesh
                btVector3 vert0, vert1, vert2;
                int i = 0;

                // For every triangle
                for( unsigned int y = 0 ; y < triCount ; y++ )
                {
                    // Set each vertex
                    vert0.setValue(vertices[indices[i]].x, vertices[indices[i]].y, vertices[indices[i]].z);
                    vert1.setValue(vertices[indices[i+1]].x, vertices[indices[i+1]].y, vertices[indices[i+1]].z);
                    vert2.setValue(vertices[indices[i+2]].x, vertices[indices[i+2]].y, vertices[indices[i+2]].z);

                    // Add it into the trimesh
                    trimesh->addTriangle(vert0, vert1, vert2);

                    // Increase index count
                    i += 3;
                }
                delete[] vertices;
                delete[] indices;

                return trimesh;
            }
        }//MeshTools
    }//Internal
}//Mezzanine

/// @endcond

#endif
