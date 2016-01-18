// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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

#ifndef _graphicssubmesh_cpp
#define _graphicssubmesh_cpp

#include "Graphics/submesh.h"

#include <Ogre.h>

namespace Mezzanine
{
    namespace Graphics
    {
        SubMesh::SubMesh(Ogre::SubMesh* Internal) :
            InternalSubMesh(Internal)
            {  }

        SubMesh::~SubMesh()
        {
            // Let Ogre Manage submeshes on it's own.
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        void SubMesh::SetMaterialName(const String& MatName)
            { this->InternalSubMesh->setMaterialName(MatName); }

        const String& SubMesh::GetMaterialName() const
            { return this->InternalSubMesh->getMaterialName(); }

        Boole SubMesh::UsesSharedVertices() const
            { return this->InternalSubMesh->useSharedVertices; }

        Whole SubMesh::GetVertexCount() const
        {
            Ogre::VertexData* VertData = ( this->InternalSubMesh->useSharedVertices ? this->InternalSubMesh->parent->sharedVertexData : this->InternalSubMesh->vertexData );
            return VertData->vertexCount;
        }

        Whole SubMesh::GetIndexCount() const
        {
            return this->InternalSubMesh->indexData->indexCount;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // SubMesh Information Methods

        void SubMesh::GetInfo(MeshInfo& ToFill) const
        {
            // Get the simple stuff first
            Ogre::RenderOperation Render;
            this->InternalSubMesh->_getRenderOperation(Render);
            ToFill.OperationType = Render.operationType;

            ToFill.MaterialName = this->InternalSubMesh->getMaterialName();

            /// @todo Should probably be replaced with something that calls our material manager.
            Ogre::MaterialManager* MatMan = Ogre::MaterialManager::getSingletonPtr();
            if( MatMan != NULL ) {
                Ogre::ResourcePtr Mat = MatMan->getByName( this->InternalSubMesh->getMaterialName() );
                ToFill.MaterialGroup = Mat->getOrigin();
            }

            this->AppendVertexInfo(ToFill.Vertices);
        }

        void SubMesh::AppendVertexInfo(VertexInfo& ToFill) const
        {
            Ogre::VertexData* VertData = ( this->InternalSubMesh->useSharedVertices ? this->InternalSubMesh->parent->sharedVertexData : this->InternalSubMesh->vertexData );
            ToFill.VertexCount += VertData->vertexCount;
            ToFill.IndexCount += this->InternalSubMesh->indexData->indexCount;

            this->AppendVertexPositionInfo(ToFill.Positions);
            this->AppendVertexNormalInfo(ToFill.Normals);
            this->AppendVertexTexCoordInfo(ToFill.UVs);
            this->AppendVertexTangentInfo(ToFill.Tangents);
            this->AppendIndexInfo(ToFill.Indices);
        }

        void SubMesh::AppendVertexPositionInfo(Vector3Vec& ToFill) const
        {
            Whole VertexOffset = ToFill.size();
            Whole NewSize = VertexOffset + this->GetVertexCount();

            Ogre::VertexData* VertData = ( this->InternalSubMesh->useSharedVertices ? this->InternalSubMesh->parent->sharedVertexData : this->InternalSubMesh->vertexData );
            const Ogre::VertexElement* PositionElem = VertData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
            if( PositionElem != NULL ) {
                ToFill.resize(NewSize);
                Ogre::HardwareVertexBufferSharedPtr vBuffer = VertData->vertexBufferBinding->getBuffer(PositionElem->getSource());

                float* pReal = NULL;
                unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                for( size_t j = VertexOffset ; j < NewSize ; j++, vertex += vBuffer->getVertexSize() )
                {
                    PositionElem->baseVertexPointerToElement(vertex, &pReal);
                    ToFill[j].X = *pReal++;
                    ToFill[j].Y = *pReal++;
                    ToFill[j].Z = *pReal++;
                }
                vBuffer->unlock();
            }
        }

        void SubMesh::AppendVertexTexCoordInfo(Vector2Vec& ToFill) const
        {
            Whole VertexOffset = ToFill.size();
            Whole NewSize = VertexOffset + this->GetVertexCount();

            Ogre::VertexData* VertData = ( this->InternalSubMesh->useSharedVertices ? this->InternalSubMesh->parent->sharedVertexData : this->InternalSubMesh->vertexData );
            const Ogre::VertexElement* TexCoordElem = VertData->vertexDeclaration->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES);
            if( TexCoordElem != NULL ) {
                ToFill.resize(NewSize);
                Ogre::HardwareVertexBufferSharedPtr vBuffer = VertData->vertexBufferBinding->getBuffer(TexCoordElem->getSource());

                float* pReal = NULL;
                unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                for( size_t j = VertexOffset ; j < NewSize ; j++, vertex += vBuffer->getVertexSize() )
                {
                    TexCoordElem->baseVertexPointerToElement(vertex, &pReal);
                    ToFill[j].X = *pReal++;
                    ToFill[j].Y = *pReal++;
                }
                vBuffer->unlock();
            }
        }

        void SubMesh::AppendVertexNormalInfo(Vector3Vec& ToFill) const
        {
            Whole VertexOffset = ToFill.size();
            Whole NewSize = VertexOffset + this->GetVertexCount();

            Ogre::VertexData* VertData = ( this->InternalSubMesh->useSharedVertices ? this->InternalSubMesh->parent->sharedVertexData : this->InternalSubMesh->vertexData );
            const Ogre::VertexElement* NormalElem = VertData->vertexDeclaration->findElementBySemantic(Ogre::VES_NORMAL);
            if( NormalElem != NULL ) {
                ToFill.resize(NewSize);
                Ogre::HardwareVertexBufferSharedPtr vBuffer = VertData->vertexBufferBinding->getBuffer(NormalElem->getSource());

                float* pReal = NULL;
                unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                for( size_t j = VertexOffset ; j < NewSize ; j++, vertex += vBuffer->getVertexSize() )
                {
                    NormalElem->baseVertexPointerToElement(vertex, &pReal);
                    ToFill[j].X = *pReal++;
                    ToFill[j].Y = *pReal++;
                    ToFill[j].Z = *pReal++;
                }
                vBuffer->unlock();
            }
        }

        void SubMesh::AppendVertexTangentInfo(Vector3Vec& ToFill) const
        {
            Whole VertexOffset = ToFill.size();
            Whole NewSize = VertexOffset + this->GetVertexCount();

            Ogre::VertexData* VertData = ( this->InternalSubMesh->useSharedVertices ? this->InternalSubMesh->parent->sharedVertexData : this->InternalSubMesh->vertexData );
            const Ogre::VertexElement* TangentElem = VertData->vertexDeclaration->findElementBySemantic(Ogre::VES_TANGENT);
            if( TangentElem != NULL ) {
                ToFill.resize(NewSize);
                Ogre::HardwareVertexBufferSharedPtr vBuffer = VertData->vertexBufferBinding->getBuffer(TangentElem->getSource());

                float* pReal = NULL;
                unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                for( size_t j = VertexOffset ; j < NewSize ; j++, vertex += vBuffer->getVertexSize() )
                {
                    TangentElem->baseVertexPointerToElement(vertex, &pReal);
                    ToFill[j].X = *pReal++;
                    ToFill[j].Y = *pReal++;
                    ToFill[j].Z = *pReal++;
                }
                vBuffer->unlock();
            }
        }

        void SubMesh::AppendIndexInfo(IntVec& ToFill) const
        {
            Whole IndexOffset = ToFill.size();
            Whole NewSize = IndexOffset + this->GetVertexCount();

            Ogre::HardwareIndexBufferSharedPtr iBuffer = this->InternalSubMesh->indexData->indexBuffer;
            Boole use32bitindexes = ( iBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT );
            ToFill.resize(NewSize);

            long* pLong = static_cast<long*>( iBuffer->lock( Ogre::HardwareBuffer::HBL_READ_ONLY ) );
            if( use32bitindexes ) {
                for( size_t CurrIndex = IndexOffset ; CurrIndex < NewSize ; ++CurrIndex )
                    { ToFill[CurrIndex] = pLong[CurrIndex]; }
            }else{
                short* pShort = reinterpret_cast<short*>(pLong);
                for( size_t CurrIndex = IndexOffset ; CurrIndex < NewSize ; ++CurrIndex )
                    { ToFill[CurrIndex] = static_cast<unsigned long>( pShort[CurrIndex] ); }
            }
            iBuffer->unlock();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::SubMesh* SubMesh::_GetInternalSubMesh() const
            { return this->InternalSubMesh; }
    }//Graphics
}//Mezzanine

#endif
