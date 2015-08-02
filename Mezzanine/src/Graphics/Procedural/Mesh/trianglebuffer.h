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
/*
 -----------------------------------------------------------------------------
 This source file is part of ogre-procedural

 For the latest info, see http://code.google.com/p/ogre-procedural/

 Copyright (c) 2010-2013 Michael Broutin

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */
#ifndef _graphicsproceduraltrianglebuffer_h
#define _graphicsproceduraltrianglebuffer_h

#include "Graphics/Procedural/proceduraldatatypes.h"
#include "matrix4x4.h"

namespace Mezzanine
{
    namespace Graphics
    {
        class Mesh;
        namespace Procedural
        {
            class TriangleBuffer;
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A simple definition for a Vertex to be used when procedurally generating meshes.
            /// @details
            ///////////////////////////////////////
            struct MEZZ_LIB Vertex
            {
                /// @brief The position of the vertex in local space.
                Vector3 Position;
                /// @brief The facing direction of the vertex in local space.
                Vector3 Normal;
                /// @brief The UV coordinate for applying textures.
                Vector2 UV;

                /// @brief Blank constructor.
                Vertex()
                    {  }
                /// @brief Detailed constructor.
                /// @param Pos The position of the vertex in local space.
                /// @param Norm The facing direction of the vertex in local space.
                /// @param TexCoord The UV coordinate for applying textures.
                Vertex(const Vector3& Pos, const Vector3& Norm, const Vector2& TexCoord) :
                    Position(Pos),
                    Normal(Norm),
                    UV(TexCoord)
                    {  }
                /// @brief Class destructor.
                ~Vertex()
                    {  }
            };//Vertex

            /// @brief Basic container type for Vertex storage.
            typedef std::vector<Vertex>                     VertexContainer;
            /// @brief Iterator type for Vertex instances in a VertexContainer.
            typedef VertexContainer::iterator               VertexIterator;
            /// @brief Const Iterator type for Vertex instances in a VertexContainer.
            typedef VertexContainer::const_iterator         ConstVertexIterator;

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A convenience class used to describe a small portion of the mesh/buffer.
            /// @details
            ///////////////////////////////////////
            struct MEZZ_LIB BufferSection
            {
                /// @brief The name of this section.
                String SectionName;
                /// @brief The start indicy of this section in the parent buffer.
                Whole FirstIndex;
                /// @brief The end indicy of this section in the parent buffer.
                Whole LastIndex;
                /// @brief The start vertex of this section in the parent buffer.
                Whole FirstVertex;
                /// @brief The end vertex of this section in the parent buffer.
                Whole LastVertex;
                /// @brief A pointer to the parent buffer this section is inside.
                TriangleBuffer* Buffer;
            };//BufferSection

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A convenience buffer that stores vertices and indices of a mesh to be generated.
            /// @details This buffer is different from native Graphics buffers in that it both has a combined API for the
            /// manipulation of vertices and indices and it doesn't create a cooresponding buffer in video card memory.
            /// This buffer exists solely in system memory and nothing is allocated on the video card until a mesh is
            /// generated.
            ///////////////////////////////////////
            class MEZZ_LIB TriangleBuffer
            {
            public:
                /// @brief Basic container type for Section storage in this class.
                typedef std::map<String,BufferSection>          SectionContainer;
                /// @brief Iterator type for Section instances stored in this class.
                typedef SectionContainer::iterator              SectionIterator;
                /// @brief Const Iterator type for Section instances stored in this class.
                typedef SectionContainer::const_iterator        ConstSectionIterator;
            protected:
                /// @internal
                /// @brief Container storing all of the sections of this buffer.
                SectionContainer Sections;
                /// @internal
                /// @brief Container storing all of the Vertices in this buffer.
                VertexContainer Vertices;
                /// @internal
                /// @brief Container storing all of the Indices in this buffer.
                IndexContainer Indices;
                /// @internal
                /// @brief Convenience pointer to the Vertex currently being manipulated.
                Vertex* CurrentVertex;
                /// @internal
                /// @brief The user estimated Vertex count for this buffer.
                Integer EstimatedVertexCount;
                /// @internal
                /// @brief The user estimated Index count for this buffer.
                Integer EstimatedIndexCount;
                /// @internal
                /// @brief Offset used to set where buffer/mesh appends begin.
                Integer GlobalOffset;
            public:
                /// @brief Class constructor.
                TriangleBuffer();
                /// @brief Class destructor.
                ~TriangleBuffer();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Appends the contents of another buffer to this buffer.
                /// @param Other The other TriangleBuffer to append to this.
                void AppendBuffer(const TriangleBuffer& Other);
                /// @brief Builds a Mesh from this buffer.
                /// @note The returned mesh will need a material applied to it for proper rendering.
                /// @param MeshName The name to give to the generated mesh.
                /// @param MeshGroup The asset group to place the mesh in.
                /// @param MatName The name of the material to apply to the mesh.
                /// @param MatGroup The asset group where the material can be found.
                /// @return Returns a pointer to the created Mesh.
                Mesh* GenerateMesh(const String& MeshName, const String& MeshGroup, const String& MatName, const String& MatGroup) const;

                /// @brief Gives an estimation of the number of vertices need for this triangle buffer.
                /// @remarks If this function is called several times, it means an extra vertices count, not an absolute measure.
                /// @param VertexCount The amount of additional Vertices to allocate for.
                void EstimateVertexCount(const Whole VertexCount);
                /// @brief Gives an estimation of the number of indices needed for this triangle buffer.
                /// @remarks If this function is called several times, it means an extra indices count, not an absolute measure.
                /// @param IndexCount The amount of additional Indexes to allocate for.
                void EstimateIndexCount(const Whole IndexCount);

                /// @brief Rebase index offset.
                /// @remarks Call this function before you add a new mesh to the triangle buffer.
                void RebaseOffset();

                /// @brief Gets a modifiable reference to vertices.
                /// @return Returns a reference to the container storing the Vertices of this buffer.
                VertexContainer& GetVertices();
                /// @brief Gets a non-modifiable reference to vertices.
                /// @return Returns a const reference to the container storing the Vertices of this buffer.
                const VertexContainer& GetVertices() const;
                /// @brief Gets a modifiable reference to vertices.
                /// @return Returns a reference to the container storing the Indices of this buffer.
                IndexContainer& GetIndices();
                /// @brief Gets a non-modifiable reference to indices.
                /// @return Returns a const reference to the container storing the Indices of this buffer.
                const IndexContainer& GetIndices() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Section Utility

                /// @brief Gets a BufferSection that can be appended to the end of this buffer.
                /// @param SectName The name of the new BufferSection to create.
                /// @return Returns a new BufferSection that is appended to the end.
                BufferSection BeginSection(const String& SectName = "");
                /// @brief Sets a BufferSection as the end section of this buffer.
                /// @param Sect The BufferSection to set as the end of the buffer.
                void EndSection(BufferSection& Sect);
                /// @brief Gets a BufferSection that is the entire contents of this buffer.
                /// @param SectName The name of the new BufferSection to create.
                /// @return Returns a new BufferSection that expresses the entire contents of this buffer.
                BufferSection GetFullSection(const String& SectName = "");

                ///////////////////////////////////////////////////////////////////////////////
                // Vertex Management

                /// @brief Adds a premade Vertex to the buffer.
                /// @param Vert The premade Vertex to be appended to this buffer.
                /// @return Returns a reference to this.
                TriangleBuffer& AddVertex(const Vertex& Vert);
                /// @brief Adds a new vertex to the buffer.
                /// @param Loc The location to set the new Vertex at, in local space.
                /// @param Norm The facing direction of the new Vertex, in local space.
                /// @param UV The texture coordinate to set for the new Vertex.
                /// @return Returns a reference to this.
                TriangleBuffer& AddVertex(const Vector3& Loc, const Vector3& Norm, const Vector2& UV);
                /// @brief Adds a new vertex to the buffer.
                /// @param Loc The location to set the new Vertex at, in local space.
                /// @return Returns a reference to this.
                TriangleBuffer& AddVertex(const Vector3& Loc);
                /// @brief Adds a new vertex to the buffer.
                /// @param X The location of the new Vertex on the X axis, in local space.
                /// @param Y The location of the new Vertex on the Y axis, in local space.
                /// @param Z The location of the new Vertex on the Z axis, in local space.
                /// @return Returns a reference to this.
                TriangleBuffer& AddVertex(const Real X, const Real Y, const Real Z);
                /// @brief Sets the normal of the current vertex.
                /// @param Norm The facing direction to set for the current Vertex.
                /// @return Returns a reference to this.
                TriangleBuffer& SetNormal(const Vector3& Norm);
                /// @brief Sets the texture coordinates of the current vertex.
                /// @param U The U component of texture UV component to set for the current Vertex.
                /// @param V The V component of texture UV component to set for the current Vertex.
                /// @return Returns a reference to this.
                TriangleBuffer& SetTextureCoord(const Real U, const Real V);
                /// @brief Sets the texture coordinates of the current vertex.
                /// @param UV The texture coordinate to set for the current Vertex.
                /// @return Returns a reference to this.
                TriangleBuffer& SetTextureCoord(const Vector2& UV);

                /// @brief Applies a matrix to transform all vertices inside this triangle buffer.
                /// @param Mat A full transform to be applied to each Vertex in this buffer.
                /// @return Returns a reference to this.
                TriangleBuffer& ApplyTransform(const Matrix4x4& Mat);
                /// @brief Applies the translation immediately to all the points contained in this triangle buffer.
                /// @param Trans The amount of movement to be applied on each axis.
                /// @return Returns a reference to this.
                TriangleBuffer& TranslateVertices(const Vector3& Trans);
                /// @brief Applies the translation immediately to all the points contained in this triangle buffer.
                /// @param X The amount of movement to be applied to each Vertex in this buffer on the X axis.
                /// @param Y The amount of movement to be applied to each Vertex in this buffer on the Y axis.
                /// @param Z The amount of movement to be applied to each Vertex in this buffer on the Z axis.
                /// @return Returns a reference to this.
                TriangleBuffer& TranslateVertices(const Real X, const Real Y, const Real Z);
                /// @brief Applies the rotation immediately to all the points contained in this triangle buffer.
                /// @param Rot The amount of rotation to be applied to each Vertex.
                /// @return Returns a reference to this.
                TriangleBuffer& RotateVertices(const Quaternion& Rot);
                /// @brief Applies an immediate scale operation to this triangle buffer.
                /// @param Scale The amount of scaling to be applied to each Vertex.
                /// @return Returns a reference to this.
                TriangleBuffer& ScaleVertices(const Vector3& Scale);
                /// @brief Applies an immediate scale operation to this triangle buffer.
                /// @param X The amount to scale the contents of this buffer on the X axis.
                /// @param Y The amount to scale the contents of this buffer on the Y axis.
                /// @param Z The amount to scale the contents of this buffer on the Z axis.
                /// @return Returns a reference to this.
                TriangleBuffer& ScaleVertices(const Real X, const Real Y, const Real Z);
                /// @brief Applies normal inversion on this triangle buffer.
                /// @return Returns a reference to this.
                TriangleBuffer& InvertNormals();

                ///////////////////////////////////////////////////////////////////////////////
                // Index Management

                /// @brief Adds an index to the index buffer.
                /// @note Index is relative to the latest call to RebaseOffset().
                /// @param Index The Index to be added.
                /// @return Returns a reference to this.
                TriangleBuffer& AddIndex(const Integer Index);
                /// @brief Adds a triangle to the index buffer.
                /// @note Index is relative to the latest call to RebaseOffset().
                /// @param Index1 The first point of the triangle to be added.
                /// @param Index2 The second point of the triangle to be added.
                /// @param Index3 The third point of the triangle to be added.
                /// @return Returns a reference to this.
                TriangleBuffer& AddTriangle(const Integer Index1, const Integer Index2, const Integer Index3);
            };//TriangleBuffer
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
