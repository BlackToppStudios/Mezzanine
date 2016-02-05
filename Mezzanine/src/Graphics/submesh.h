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

#ifndef _graphicssubmesh_h
#define _graphicssubmesh_h

#include "Graphics/meshinfo.h"

namespace Ogre
{
    class SubMesh;
}//Ogre

namespace Mezzanine
{
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class represents a sub-section of an overall mesh.
        /// @details Every mesh has at least one SubMesh.  Most meshes have only 1 or 2.  SubMeshes are often
        /// implemented in cases where one portion of the mesh needs to be animated or given a different
        /// material then another portion.
        ///////////////////////////////////////
        class MEZZ_LIB SubMesh
        {
        protected:
            /// @internal
            /// @brief A pointer to the internal SubMesh this is based on.
            Ogre::SubMesh* InternalSubMesh;
        public:
            /// @internal
            /// @brief Internal Constructor.
            /// @param Internal The internal SubMesh this SubMesh class is based on.
            SubMesh(Ogre::SubMesh* Internal);
            /// @brief Class Destructor.
            ~SubMesh();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Sets the name of the Material to be used by this SubMesh.
            /// @param MatName The name of the Material to be applied to this SubMesh.
            void SetMaterialName(const String& MatName);
            /// @brief Gets the name of the Material being used by this SubMesh.
            /// @return Returns a const reference to a String containing the name of the Material to be used with this SubMesh.
            const String& GetMaterialName() const;

            /// @brief Gets whether or not this SubMesh shares a Vertex Buffer with other SubMeshes.
            /// @remarks A Mesh can only have a single shared buffer across all SubMeshes.  So if any SubMesh does use a shared buffer,
            /// all of the SubMeshes that do share one, share the same buffer.
            /// @return Returns true if this SubMesh uses a shared Vertex Buffer with other SubMeshes.
            Boole UsesSharedVertices() const;
            /// @brief Gets the number of Vertices in this SubMesh.
            /// @return Returns the number of Vertices that make up this SubMesh.
            Whole GetVertexCount() const;
            /// @brief Gets the number of Indices in this SubMesh.
            /// @return Returns the number of Indices used to assemble the vertices in this SubMesh.
            Whole GetIndexCount() const;

            ///////////////////////////////////////////////////////////////////////////////
            // SubMesh Information Methods

            /// @brief Gets the information used to render this SubMesh.
            /// @remarks This method makes no attempt to clear any pre-existing data on the MeshInfo struct before use.  Relevant data will be overwritten.
            /// @param ToFill The MeshInfo struct to be populated with data from this SubMesh.
            void GetInfo(MeshInfo& ToFill) const;
            /// @brief Gets the vertex information of this SubMesh and appends it to the VertexInfo provided.
            /// @param ToFill The VertexInfo struct to have this SubMesh vertex information added to.
            void AppendVertexInfo(VertexInfo& ToFill) const;

            /// @brief Gets the vertex position information of this SubMesh and appends it to the container provided.
            /// @param ToFill The container to append this SubMeshes vertex position info to.
            void AppendVertexPositionInfo(Vector3Vec& ToFill) const;
            /// @brief Gets the vertex texture coordinate information of this SubMesh and appends it to the container provided.
            /// @param ToFill The container to append this SubMeshes vertex texture coordinate info to.
            void AppendVertexTexCoordInfo(Vector2Vec& ToFill) const;
            /// @brief Gets the vertex normal information of this SubMesh and appends it to the container provided.
            /// @param ToFill The container to append this SubMeshes vertex normal info to.
            void AppendVertexNormalInfo(Vector3Vec& ToFill) const;
            /// @brief Gets the vertex tangent information of this SubMesh and appends it to the container provided.
            /// @param ToFill The container to append this SubMeshes vertex tangent info to.
            void AppendVertexTangentInfo(Vector3Vec& ToFill) const;
            /// @brief Gets the Index information of this SubMesh and appends it to the container provided.
            /// @param ToFill The container to append this SubMeshes Index info to.
            void AppendIndexInfo(IntVec& ToFill) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Gets the internal SubMesh pointer.
            /// @return Returns a pointer pointing to the internal SubMesh.
            Ogre::SubMesh* _GetInternalSubMesh() const;
        };//SubMesh
    }//Graphics
}//Mezzanine

#endif
