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
#ifndef _graphicsproceduralmeshgenerator_h
#define _graphicsproceduralmeshgenerator_h

#include "vector2.h"
#include "vector3.h"
#include "quaternion.h"

#include "Graphics/Procedural/Mesh/trianglebuffer.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A base class containing all of the utilities needed for a mesh generator.
            /// @details
            ///////////////////////////////////////
            template <typename T>
            class MeshGenerator
            {
            public:
                /// @enum GeneratorOptions
                /// @brief A convenience enum used to describe boolean options for a generator.
                enum GeneratorOptions
                {
                    GO_EnableNormals = 1,  ///< Whether or not to produce normals when generating a mesh.
                    GO_SwitchUV      = 2,  ///< Whether or not to swap the UV compontents when generating a mesh.
                    GO_Transform     = 4   ///< Whether or not a transform has been defined and is to be used.
                };
            protected:
                /// @internal
                /// @brief Orientation to apply the mesh.
                Quaternion Orientation;
                /// @internal
                /// @brief Position to apply to the mesh.
                Vector3 Position;
                /// @internal
                /// @brief Scale to apply to the mesh.
                Vector3 Scale;
                /// @internal
                /// @brief Rectangle in which the texture coordinates will be placed.
                Vector2 UVOrigin;
                /// @internal
                /// @brief U tile for texture coords generation.
                Real UTile;
                /// @internal
                /// @brief V tile for texture coords generation.
                Real VTile;
                /// @internal
                /// @brief Storage for the boolean options to be used by this generator.
                UInt8 GeneratorOpts;
                /// @internal
                /// @brief The number of texture coordinate sets to include.
                UInt8 NumTexCoordSet;

                /// @brief Adds a new point to a triangle buffer, using the format defined for that MeshGenerator.
                /// @param Buffer The triangle buffer to update.
                /// @param Loc The location of the new point.
                /// @param Norm the normal of the new point.
                /// @param UV the uv texcoord of the new point.
                inline void AddPoint(TriangleBuffer& Buffer, const Vector3& Loc, const Vector3& Norm, const Vector2& UV) const
                {
                    if( this->GeneratorOpts | GO_Transform ) {
                        Buffer.AddVertex(this->Position + this->Orientation * (this->Scale * Loc));
                    }else{
                        Buffer.AddVertex(Loc);
                    }

                    if( this->GeneratorOpts | GO_EnableNormals ) {
                        if( this->GeneratorOpts | GO_Transform ) {
                            Buffer.SetNormal(this->Orientation * Norm);
                        }else{
                            Buffer.SetNormal(Norm);
                        }
                    }

                    if( this->GeneratorOpts | GO_SwitchUV ) {
                        for( UInt8 Index = 0 ; Index < this->NumTexCoordSet ; Index++ )
                            { Buffer.SetTextureCoord( this->UVOrigin.X + UV.Y * this->UTile, this->UVOrigin.Y + UV.X * this->VTile ); }
                    }else{
                        for( UInt8 Index = 0 ; Index < this->NumTexCoordSet ; Index++ )
                            { Buffer.SetTextureCoord( this->UVOrigin.X + UV.X * this->UTile, this->UVOrigin.Y + UV.Y * this->VTile ); }
                    }
                }
            public:
                /// @brief Class constructor.
                MeshGenerator() :
                    Orientation(0,0,0,1),
                    Position(0,0,0),
                    Scale(1,1,1),
                    UVOrigin(0,0),
                    UTile(1.0),
                    VTile(1.0),
                    GeneratorOpts(GO_EnableNormals),
                    NumTexCoordSet(1)
                    {  }
                /// @brief Class destructor.
                virtual ~MeshGenerator()
                    {  }

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Generates the mesh.
                /// @param MeshName The name to give to the generated mesh.
                /// @param MeshGroup The asset group to place the mesh in.
                /// @param MatName The name of the material to apply to the mesh.
                /// @param MatGroup The asset group where the material can be found.
                /// @return Returns a pointer to the newly created Mesh.
                Mesh* GenerateMesh(const String& MeshName, const String& MeshGroup, const String& MatName, const String& MatGroup) const
                {
                    TriangleBuffer TriBuf;
                    this->AddToTriangleBuffer(TriBuf);
                    Mesh* NewMesh = TriBuf.GenerateMesh(MeshName,MeshGroup,MatName,MatGroup);
                    return NewMesh;
                }
                /// @brief Creates a TriangleBuffer with the the vertices and indices as configured in this generator.
                /// @return Returns a new TriangleBuffer with the generated vertices and indices of this generator.
                TriangleBuffer BuildTriangleBuffer() const
                {
                    TriangleBuffer TriBuf;
                    this->AddToTriangleBuffer(TriBuf);
                    return TriBuf;
                }
                /// @brief Adds the vertices and indices as configured in this generator to a triangle buffer.
                /// @param Buffer The buffer to append this generators vertices and indices to.
                virtual void AddToTriangleBuffer(TriangleBuffer& Buffer) const = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the U Tile.
                /// @remarks Default UTile value is "1".
                /// @param uTile The number by which U texture coordinates are multiplied.
                /// @return Returns a reference to this.
                inline T& SetUTile(const Real uTile)
                {
                    this->UTile = uTile;
                    return static_cast<T&>(*this);
                }
                /// @brief Sets the V Tile.
                /// @remarks Default VTile value is "1".
                /// @param vTile The number by which V texture coordinates are multiplied.
                /// @return Returns a reference to this.
                inline T& SetVTile(const Real vTile)
                {
                    this->VTile = vTile;
                    return static_cast<T&>(*this);
                }
                /// @brief Sets the texture rectangle.
                /// @param RectLeft The texture coordinate for the left edge.
                /// @param RectRight The texture coordinate for the right edge.
                /// @param RectTop The texture coordinate for the top edge.
                /// @param RectBottom The texture coordinate for the bottom.
                /// @return Returns a reference to this.
                inline T& SetTextureRectangle(const Real RectLeft, const Real RectRight, const Real RectTop, const Real RectBottom)
                {
                    this->UVOrigin.SetValues(RectTop,RectLeft);
                    this->UTile = RectRight - RectLeft;
                    this->VTile = RectBottom - RectTop;
                    return static_cast<T&>(*this);
                }
                /// @brief Sets the number of texture coordintate sets.
                /// @remarks Default NumTexCoordSet value is "1".
                /// @param NumSets The number of texture coordinate sets to use.
                /// @return Returns a reference to this.
                inline T& SetNumTexCoordSet(const UInt8 NumSets)
                {
                    this->NumTexCoordSet = NumSets;
                    return static_cast<T&>(*this);
                }

                /// @brief Sets whether normals are enabled or not.
                /// @remarks Default normals value is true.
                /// @param EnableNormals True to generate normals as points are added to the mesh, false otherwise.
                /// @return Returns a reference to this.
                inline T& SetEnableNormals(Boole EnableNormals)
                {
                    if( EnableNormals ) {
                        this->GeneratorOpts |= GO_EnableNormals;
                    }else{
                        this->GeneratorOpts &= ~GO_EnableNormals;
                    }
                    return static_cast<T&>(*this);
                }
                /// @brief Sets whether to switch U and V texture coordinates.
                /// @remarks Default uvs value is false.
                /// @param SwitchUV True to swap the UV texture coordinates as points are added to the mesh, false otherwise.
                /// @return Returns a reference to this.
                inline T& SetSwitchUV(Boole SwitchUV)
                {
                    if( SwitchUV ) {
                        this->GeneratorOpts |= GO_SwitchUV;
                    }else{
                        this->GeneratorOpts &= ~GO_SwitchUV;
                    }
                    return static_cast<T&>(*this);
                }

                ///////////////////////////////////////////////////////////////////////////////
                // Transform

                /// @brief Sets a translation baked into the resulting mesh.
                /// @param Loc The amount of translation to be applied on each axis.
                /// @return Returns a reference to this.
                inline T& SetPosition(const Vector3& Loc)
                {
                    this->Position = Loc;
                    this->GeneratorOpts |= GO_Transform;
                    return static_cast<T&>(*this);
                }
                /// @brief Sets a translation baked into the resulting mesh.
                /// @param X The amount of translation to be applied on the X axis.
                /// @param Y The amount of translation to be applied on the Y axis.
                /// @param Z The amount of translation to be applied on the Z axis.
                /// @return Returns a reference to this.
                inline T& SetPosition(const Real X, const Real Y, const Real Z)
                {
                    this->Position.SetValues(X,Y,Z);
                    this->GeneratorOpts |= GO_Transform;
                    return static_cast<T&>(*this);
                }
                /// @brief Sets an orientation baked into the resulting mesh.
                /// @param Ori The rotation to apply to a generated mesh.
                /// @return Returns a reference to this.
                inline T& SetOrientation(const Quaternion& Ori)
                {
                    this->Orientation = Ori;
                    this->GeneratorOpts |= GO_Transform;
                    return static_cast<T&>(*this);
                }
                /// @brief Sets an orientation baked into the resulting mesh.
                /// @param X The X component of the Axis.
                /// @param Y The Y component of the Axis.
                /// @param Z The Z component of the Axis.
                /// @param W Rotation on the Axis X, Y and Z defined.
                /// @return Returns a reference to this.
                inline T& SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
                {
                    this->Orientation.SetValues(X,Y,Z,W);
                    this->GeneratorOpts |= GO_Transform;
                    return static_cast<T&>(*this);
                }
                /// @brief Sets a scale baked into the resulting mesh.
                /// @param Scaling The amount of scaling to apply on each axis to all meshes generated by this generator.
                /// @return Returns a reference to this.
                inline T& SetScale(const Vector3& Scaling)
                {
                    this->Scale = Scaling;
                    this->GeneratorOpts |= GO_Transform;
                    return static_cast<T&>(*this);
                }
                /// @brief Sets a uniform scale baked into the resulting mesh.
                /// @param Scaling The amount of scaling to apply on each axis to all meshes generated by this generator.
                /// @return Returns a reference to this.
                inline T& SetScale(const Real Scaling)
                {
                    this->Scale.SetValues(Scaling,Scaling,Scaling);
                    this->GeneratorOpts |= GO_Transform;
                    return static_cast<T&>(*this);
                }
                /// @brief Sets a scale baked into the resulting mesh.
                /// @param X The amount of scaling to apply on the X axis to all meshes generated by this generator.
                /// @param Y The amount of scaling to apply on the Y axis to all meshes generated by this generator.
                /// @param Z The amount of scaling to apply on the Z axis to all meshes generated by this generator.
                /// @return Returns a reference to this.
                inline T& SetScale(const Real X, const Real Y, const Real Z)
                {
                    this->Scale.SetValues(X,Y,Z);
                    this->GeneratorOpts |= GO_Transform;
                    return static_cast<T&>(*this);
                }
                /// @brief Resets all transforms (orientation, position and scale) that would have been applied to the mesh to their default values.
                /// @return Returns a reference to this.
                inline T& ResetTransforms()
                {
                    this->GeneratorOpts &= ~GO_Transform;
                    this->Position.Zero();
                    this->Orientation.SetIdentity();
                    this->Scale.SetValues(1,1,1);
                    return static_cast<T&>(*this);
                }
            };//MeshGenerator
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
