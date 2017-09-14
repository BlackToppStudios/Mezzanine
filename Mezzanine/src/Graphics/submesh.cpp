// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#include "Graphics/vertextools.h"

#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreVertexIndexData.h>
#include <OgreMaterialManager.h>
#include <OgreMaterial.h>

namespace Mezzanine
{
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        // SubMesh Methods

        SubMesh::SubMesh(Ogre::SubMesh* Internal) :
            InternalSubMesh(Internal)
            {  }

        SubMesh::~SubMesh()
            {  }  // Let Ogre Manage submeshes on it's own.

        Ogre::VertexData* SubMesh::GetVertexData() const
        {
            Ogre::VertexData* VertData = NULL;
            if( this->InternalSubMesh->useSharedVertices ) {
                VertData = this->InternalSubMesh->parent->sharedVertexData;
            }else{
                VertData = this->InternalSubMesh->vertexData;
            }
            return VertData;
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

        MeshInfo SubMesh::GetMeshInfo() const
        {
            MeshInfo ToFill;
            // Get the simple stuff first
            Ogre::RenderOperation Render;
            this->InternalSubMesh->_getRenderOperation(Render);
            ToFill.OperationType = Render.operationType;

            ToFill.MaterialName = this->InternalSubMesh->getMaterialName();

            /// @todo Should probably be replaced with something that calls our material manager.
            Ogre::MaterialManager* MatMan = Ogre::MaterialManager::getSingletonPtr();
            if( !ToFill.MaterialName.empty() && MatMan != NULL ) {
                Ogre::MaterialPtr Mat = MatMan->getByName( this->InternalSubMesh->getMaterialName() );
                ToFill.MaterialGroup = Mat->getOrigin();
            }

            Ogre::VertexData* VertData = ( this->InternalSubMesh->useSharedVertices ? this->InternalSubMesh->parent->sharedVertexData : this->InternalSubMesh->vertexData );
            ToFill.Vertices.VertexCount = VertData->vertexCount;
            ToFill.Vertices.IndexCount = this->InternalSubMesh->indexData->indexCount;

            ToFill.Vertices.Positions = this->GetVertexPositions();
            ToFill.Vertices.Normals = this->GetVertexNormals();
            ToFill.Vertices.UVs = this->GetVertexTexCoords();
            ToFill.Vertices.Tangents = this->GetVertexTangents();
            ToFill.Vertices.Indices = this->GetIndices();

            return ToFill;
        }

        VertexInfo SubMesh::GetVertexInfo() const
        {
            VertexInfo ToFill;
            Ogre::VertexData* VertData = ( this->InternalSubMesh->useSharedVertices ? this->InternalSubMesh->parent->sharedVertexData : this->InternalSubMesh->vertexData );
            ToFill.VertexCount = VertData->vertexCount;
            ToFill.IndexCount = this->InternalSubMesh->indexData->indexCount;

            ToFill.Positions = this->GetVertexPositions();
            ToFill.Normals = this->GetVertexNormals();
            ToFill.UVs = this->GetVertexTexCoords();
            ToFill.Tangents = this->GetVertexTangents();
            ToFill.Indices = this->GetIndices();
            return ToFill;
        }

        Vector3Vec SubMesh::GetVertexPositions() const
        {
            Ogre::VertexData* VertData = this->GetVertexData();
            return VertexTools::GetVertexPositions(VertData);
        }

        Vector2Vec SubMesh::GetVertexTexCoords() const
        {
            Ogre::VertexData* VertData = this->GetVertexData();
            return VertexTools::GetVertexTexCoords(VertData);
        }

        Vector3Vec SubMesh::GetVertexNormals() const
        {
            Ogre::VertexData* VertData = this->GetVertexData();
            return VertexTools::GetVertexNormals(VertData);
        }

        Vector3Vec SubMesh::GetVertexTangents() const
        {
            Ogre::VertexData* VertData = this->GetVertexData();
            return VertexTools::GetVertexTangents(VertData);
        }

        Vector3Vec SubMesh::GetVertexBinormals() const
        {
            Ogre::VertexData* VertData = this->GetVertexData();
            return VertexTools::GetVertexBinormals(VertData);
        }

        IntVec SubMesh::GetIndices() const
        {
            Ogre::IndexData* IndexConfig = this->InternalSubMesh->indexData;
            return VertexTools::GetIndices(IndexConfig);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::SubMesh* SubMesh::_GetInternalSubMesh() const
            { return this->InternalSubMesh; }
    }//Graphics
}//Mezzanine

#endif
