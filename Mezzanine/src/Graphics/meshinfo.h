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
#ifndef _graphicsmeshinfo_h
#define _graphicsmeshinfo_h

#include "vector2.h"
#include "vector3.h"
#include "colourvalue.h"

namespace Mezzanine
{
    namespace Graphics
    {
        /// @brief Convenience type for a container of Vector3s.
        typedef std::vector<Vector3> Vector3Vec;
        /// @brief Convenience type for a container of Vector2s.
        typedef std::vector<Vector2> Vector2Vec;
        /// @brief Convenience type for a container of Integers.
        typedef std::vector<Integer> IntVec;
        /// @brief Convenience type for a container of Reals.
        typedef std::vector<Real> RealVec;

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A convenience class for the storage of Vertex data in a Mesh/SubMesh.
        ///////////////////////////////////////
        struct MEZZ_LIB VertexInfo
        {
            /// @brief The number of Vertices in the Mesh/SubMesh.
            Whole VertexCount;
            /// @brief The number of Indexes in the Mesh/SubMesh.
            Whole IndexCount;

            /// @brief A container storing the position of each vertex.
            Vector3Vec Positions;
            /// @brief A container storing the texture coordinates of each vertex.
            Vector2Vec UVs;
            /// @brief A container storing the normal of each vertex.
            Vector3Vec Normals;
            /// @brief A container storing the tangent of each vertex.
            Vector3Vec Tangents;
            /// @brief A container storing the binormal of each vertex.
            Vector3Vec Binormals;

            /// @brief A container of Indexes used to assemble the Vertices for rendering.
            IntVec Indices;

            /// @brief No init constructor.
            VertexInfo() :
                VertexCount(0),
                IndexCount(0)
                {  }
            /// @brief Class destructor.
            ~VertexInfo()
                {  }
        };//VertexInfo

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A convenience class for the storage of rendering data in a Mesh/SubMesh.
        /// @todo This is fairly simple struct that doesn't encompass all of the data a Vertex can have, just what they commonly have.
        /// As such, this should be extended to better account for the various vertex formats.
        ///////////////////////////////////////
        struct MEZZ_LIB MeshInfo
        {
            /// @brief Used Internally.  Describes how the Vertex data should be treated when rendering.
            Whole OperationType;

            /// @brief The name of the material used to render the Mesh/SubMesh.
            String MaterialName;
            /// @brief The resource group in which the named Material can be found.
            String MaterialGroup;

            /// @brief A collection of containers storing the various properties of a Vertex.
            VertexInfo Vertices;

            /// @brief No init constructor.
            MeshInfo() :
                OperationType(0)
                {  }
            /// @brief Class destructor.
            ~MeshInfo()
                {  }
        };//MeshInfo
    }//Graphics
}//Mezzanine

#endif
