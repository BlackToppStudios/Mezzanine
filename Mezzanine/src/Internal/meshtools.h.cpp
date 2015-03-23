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
#ifndef _internalmeshtools_h_cpp
#define _internalmeshtools_h_cpp

// Keeps this file form being documented by doxygen
/// @cond DontDocumentInternal

#include <btBulletDynamicsCommon.h>
#include <Ogre.h>
#include "datatypes.h"

namespace Mezzanine
{
    namespace Internal
    {
        namespace MeshTools
        {
            ///////////////////////////////////////////////////////////////////////////////
            // Convenience Types

            /// @brief Convenience type for a container of Vector3s.
            typedef std::vector<Ogre::Vector3> Vector3Vec;
            /// @brief Convenience type for a container of Vector2s.
            typedef std::vector<Ogre::Vector2> Vector2Vec;
            /// @brief Convenience type for a container of Integers.
            typedef std::vector<Integer> IntVec;

            ///////////////////////////////////////////////////////////////////////////////
            // Simple Utility

            /// @brief Gets the total number of vertices stored in a Mesh.
            /// @param TheMesh The internal Mesh being queried.
            /// @return Gets the number of vertices that exist across all SubMeshes in a Mesh.
            Whole MEZZ_LIB GetVertexCount(Ogre::MeshPtr TheMesh);
            /// @brief Gets the total number of indexes stored in a Mesh.
            /// @param TheMesh The internal Mesh being queried.
            /// @return Gets the number of indices that exist across all SubMeshes in a Mesh.
            Whole MEZZ_LIB GetIndexCount(Ogre::MeshPtr TheMesh);

            ///////////////////////////////////////////////////////////////////////////////
            // Multi SubMesh Query

            /// @brief Gets all of the vertex positions in a Mesh.
            /// @param TheMesh The internal Mesh being queried.
            /// @return Returns a vector of Vector3s containing the positions of each vertex across all SubMeshes in a Mesh.
            Vector3Vec MEZZ_LIB GetVertexPositions(Ogre::MeshPtr TheMesh);
            /// @brief Gets all of the vertex normals in a Mesh.
            /// @param TheMesh The internal Mesh being queried.
            /// @return Returns a vector of Vector3s containing the normals of each vertex across all SubMeshes in a Mesh.
            Vector3Vec MEZZ_LIB GetVertexNormals(Ogre::MeshPtr TheMesh);
            /// @brief Gets all of the vertex UVs in a Mesh.
            /// @param TheMesh The internal Mesh being queried.
            /// @return Returns a vector of Vector2s containing the UVs of each vertex across all SubMeshes in a Mesh.
            Vector2Vec MEZZ_LIB GetVertexUVs(Ogre::MeshPtr TheMesh);
            /// @brief Gets all of the indexes in a Mesh.
            /// @param TheMesh The internal Mesh being queried.
            /// @return Returns a vector of Integers containg the index data across all SubMeshes in a Mesh.
            IntVec MEZZ_LIB GetIndexes(Ogre::MeshPtr TheMesh);

            ///////////////////////////////////////////////////////////////////////////////
            // Single SubMesh Query

            /// @brief Gets all of the vertex positions in a specific SubMesh of a Mesh.
            /// @param TheMesh The internal Mesh being queried.
            /// @param SubMeshIndex The index of the specific SubMesh to get vertex data from.
            /// @return Returns a vector of Vector3s containing the positions of each vertex in the specified SubMesh.
            Vector3Vec MEZZ_LIB GetSubMeshVertexPositions(Ogre::MeshPtr TheMesh, const Whole SubMeshIndex);
            /// @brief Gets all of the vertex normals in a specific SubMesh of a Mesh.
            /// @param TheMesh The internal Mesh being queried.
            /// @param SubMeshIndex The index of the specific SubMesh to get vertex data from.
            /// @return Returns a vector of Vector3s containing the normals of each vertex in the specified SubMesh.
            Vector3Vec MEZZ_LIB GetSubMeshVertexNormals(Ogre::MeshPtr TheMesh, const Whole SubMeshIndex);
            /// @brief Gets all of the vertex UVs in a specific SubMesh of a Mesh.
            /// @param TheMesh The internal Mesh being queried.
            /// @param SubMeshIndex The index of the specific SubMesh to get vertex data from.
            /// @return Returns a vector of Vector2s containing the UVs of each vertex in the specified SubMesh.
            Vector2Vec MEZZ_LIB GetSubMeshVertexUVs(Ogre::MeshPtr TheMesh, const Whole SubMeshIndex);
            /// @brief Gets all of the indexes in a specific SubMesh of a Mesh.
            /// @param TheMesh The internal Mesh being queried.
            /// @param SubMeshIndex The index of the specific SubMesh to get index data from.
            /// @return Returns a vector of Integers containg the index data for the specified SubMesh.
            IntVec MEZZ_LIB GetSubMeshIndexes(Ogre::MeshPtr TheMesh, const Whole SubMeshIndex);

            /// @brief Gets the name of the material assigned to a specific SubMesh.
            /// @param TheMesh The internal Mesh being queried.
            /// @param SubMeshIndex The index of the specific SubMesh to get the assigned material from.
            /// @return Returns a String containing the name of the material assigned to the specified SubMesh.
            String MEZZ_LIB GetSubMeshMaterialName(Ogre::MeshPtr TheMesh, const Whole SubMeshIndex);
            /// @brief Gets the filename where the material assigned to a specific SubMesh came from.
            /// @param TheMesh The internal Mesh being queried.
            /// @param SubMeshIndex The index of the specific SubMesh to get the material file from.
            /// @return Returns the filename the material assigned to the specified SubMesh was parsed from.
            String MEZZ_LIB GetSubMeshMaterialOrigin(Ogre::MeshPtr TheMesh, const Whole SubMeshIndex);
            /// @brief Gets the type of Render Operation being used to render a specific SubMesh.
            /// @param TheMesh The internal Mesh being queried.
            /// @param SubMeshIndex The index of the specific SubMesh to get the RenderOperation of.
            /// @return Returns the type of RenderOperation being used to render the specified SubMesh.
            Ogre::RenderOperation::OperationType MEZZ_LIB GetSubMeshRenderType(Ogre::MeshPtr TheMesh, const Whole SubMeshIndex);

            ///////////////////////////////////////////////////////////////////////////////
            // Interweaved Element Fetch

            ///////////////////////////////////////////////////////////////////////////////
            // Miscellaneous Information

            /// @brief
            /// @param TheEntity The graphics object to collect Mesh data from.
            /// @param VertexArray The vector which will be populated with Vertex information.
            /// @param IndexArray The vector which will be populated with Index information.
            void MEZZ_LIB GetTransformedMeshData(Ogre::Entity* TheEntity, Vector3Vec& VertexArray, IntVec& IndexArray);

            ///////////////////////////////////////////////////////////////////////////////
            // Physics Utilities

            /// @brief Creates a trimesh shape from the mesh file.
            /// @param TheMesh The internal Mesh being queried.
            /// @param UseAllSubmeshes If true, this will use the geometry of all submeshes of the model to make the shape.  Otherwise it'll only use the first submesh.
            /// @return Returns a pointer to a btTriangleMesh copy of the provided mesh which can be used for physics collision shapes.
            btTriangleMesh* MEZZ_LIB CreateBulletTrimesh(Ogre::MeshPtr TheMesh, const Boole UseAllSubmeshes = false);
        }//MeshTools
    }//internal
}//Mezzanine

/// @endcond

#endif
