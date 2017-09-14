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
#ifndef _graphicsproceduralboxuvmodifier_h
#define _graphicsproceduralboxuvmodifier_h

#include "Graphics/Procedural/Mesh/meshmodifier.h"

#include "vector3.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A modifier that will generate UV coordinates as they would need to be on a box model.
            /// @details The mapping type used here will not necessarily make or break the actual operation performed, it simple sets
            /// the data for a particular expectation. All of the axes specified here are in local model space. @n @n
            /// MT_Full is the simplest setup and is used for most if not all tutorial
            /// setups and it is where it uses the full texture, stretches it to match the aspect ratio of the face, and copies it for
            /// each face on the box.  The other setups are a little more complicated. @n @n
            /// MT_Cross is a more typical example of a professional cube map, where you place the quads for each face on a texture
            /// partitioned in a 4x3 grid with the middle row occupied by the X and Z axis faces in the following order: (-X,-Z,+X,+Z),
            /// and the top and bottom of the second column (from the left) occupied by the Y axis faces: +Y on top and -Y on bottom. @n @n
            /// MT_Packed is where a texture is partitioned into a 3x2 grid, and unlike MT_Cross, will take up the entire texture.  The
            /// top row are the positive facing directions in alphabetical order (+X,+Y,+Z) from left to right.  The bottom row are the
            /// negative facing directions, also in alphabetical order (-X,-Y,-Z) from left to right.
            ///////////////////////////////////////
            class MEZZ_LIB BoxUVModifier : public MeshModifier
            {
            public:
                /// @brief An enum describing how the quads for the cube are expected to be laid out on the texture.
                enum MappingType
                {
                    MT_Cross  = 1,   ///< Each face of the cube is packed into a 4x3 texture atlas and is arranged into a cross shape.
                    MT_Packed = 2,   ///< Each face of the cube is packed into a 3x2 texture atlas.
                    MT_Full   = 3    ///< There is only one texture to use for each face of the cube.
                };
            protected:
                /// @internal
                /// @brief The half-size of the box on each dimension.
                Vector3 BoxHalfExtents;
                /// @internal
                /// @brief The central position of the box.
                Vector3 BoxPosition;
                /// @internal
                /// @brief How the quads for the cube are expected to be laid out on the texture.
                MappingType Mapping;
            public:
                /// @brief Blank constructor.
                BoxUVModifier();
                /// @brief Class destructor.
                virtual ~BoxUVModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc MeshModifier::Modify(TriangleBuffer&)
                virtual void Modify(TriangleBuffer& Buffer);
                /// @copydoc MeshModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the UV mapping mode to be expected to be used with the mesh.
                /// @param Type The type of mapping to expect when setting up the UV's.
                /// @return Returns a reference to this.
                BoxUVModifier& SetMappingType(const MappingType Type);
                /// @brief Sets the center position of the Box to project UV's onto.
                /// @param Position The central position of the Box.
                /// @return Returns a reference to this.
                BoxUVModifier& SetBoxPosition(const Vector3& Position);
                /// @brief Sets the center position of the Box to project UV's onto.
                /// @param X The central position of the Box on the X axis.
                /// @param Y The central position of the Box on the Y axis.
                /// @param Z The central position of the Box on the Z axis.
                /// @return Returns a reference to this.
                BoxUVModifier& SetBoxPosition(const Real X, const Real Y, const Real Z);
                /// @brief Sets the half-size of the Box to project UV's onto.
                /// @param HalfSize Half the size of the Box to generate UV's with.
                /// @return Returns a reference to this.
                BoxUVModifier& SetBoxHalfSize(const Vector3& HalfSize);
                /// @brief Sets the half-size of the Box to project UV's onto.
                /// @param X Half the size of the Box to generate UV's with on the X axis.
                /// @param Y Half the size of the Box to generate UV's with on the Y axis.
                /// @param Z Half the size of the Box to generate UV's with on the Z axis.
                /// @return Returns a reference to this.
                BoxUVModifier& SetBoxHalfSize(const Real X, const Real Y, const Real Z);
            };//BoxUVModifier
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
