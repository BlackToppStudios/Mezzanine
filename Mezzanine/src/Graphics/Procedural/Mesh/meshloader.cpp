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
#ifndef _graphicsproceduralmeshloader_cpp
#define _graphicsproceduralmeshloader_cpp

#include "Graphics/Procedural/Mesh/meshloader.h"

#include "Graphics/mesh.h"
#include "Graphics/submesh.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            MeshLoader::MeshLoader() :
                GenMesh(NULL)
                {  }

            MeshLoader::~MeshLoader()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void MeshLoader::AddToTriangleBuffer(TriangleBuffer& Buffer) const
            {
                Buffer.RebaseOffset();
                Buffer.EstimateVertexCount( this->GenMesh->GetVertexCount() );
                Buffer.EstimateIndexCount( this->GenMesh->GetIndexCount() );

                for( Whole SubMeshIndex = 0 ; SubMeshIndex < this->GenMesh->GetNumSubMeshes() ; ++SubMeshIndex )
                {
                    Graphics::MeshInfo SubMeshInfo;
                    SubMesh* CurrSubMesh = this->GenMesh->GetSubMesh(SubMeshIndex);

                    CurrSubMesh->GetInfo(SubMeshInfo);
                    /// @todo This doesn't account for the submesh name.  Should research/consider adding this.
                    Buffer.BeginSection(SubMeshInfo.MaterialName,SubMeshInfo.MaterialGroup,SubMeshInfo.OperationType);
                    for( Whole CurrVert = 0 ; CurrVert < SubMeshInfo.Vertices.VertexCount ; ++CurrVert )
                        { Buffer.AddVertex( SubMeshInfo.Vertices.Positions[CurrVert], SubMeshInfo.Vertices.Normals[CurrVert], SubMeshInfo.Vertices.UVs[CurrVert] ); }

                    for( Whole CurrIndex = 0 ; CurrIndex < SubMeshInfo.Vertices.IndexCount ; ++CurrIndex )
                        { Buffer.AddIndex( SubMeshInfo.Vertices.Indices[CurrIndex] ); }
                    Buffer.EndSection();
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            MeshLoader& MeshLoader::SetMesh(Mesh* ToGenerate)
            {
                this->GenMesh = ToGenerate;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
