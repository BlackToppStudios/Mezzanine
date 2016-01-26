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
#ifndef _graphicsproceduralboxgenerator_h
#define _graphicsproceduralboxgenerator_h

#include "Graphics/Procedural/Mesh/meshgenerator.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A generator class for a box mesh.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB BoxGenerator : public MeshGenerator<BoxGenerator>
            {
            public:
                /// @brief A String containing the name of the Negative X face of the box.
                static const String TAG_NEGX;
                /// @brief A String containing the name of the Negative Y face of the box.
                static const String TAG_NEGY;
                /// @brief A String containing the name of the Negative Z face of the box.
                static const String TAG_NEGZ;
                /// @brief A String containing the name of the X face of the box.
                static const String TAG_X;
                /// @brief A String containing the name of the Y face of the box.
                static const String TAG_Y;
                /// @brief A String containing the name of the Z face of the box.
                static const String TAG_Z;
            protected:
                /// @internal
                /// @brief The size of the box to generate.
                Vector3 BoxHalf;
                /// @internal
                /// @brief The number of segments to generate along the X axis for each face.
                Whole NumSegX;
                /// @internal
                /// @brief The number of segments to generate along the Y axis for each face.
                Whole NumSegY;
                /// @internal
                /// @brief The number of segments to generate along the Z axis for each face.
                Whole NumSegZ;
            public:
                /// @brief Vector constructor.
                /// @param HalfExtents The half size of the box to generate.
                /// @param SegX The number of segments to generate along the X axis for each face.
                /// @param SegY The number of segments to generate along the Y axis for each face.
                /// @param SegZ The number of segments to generate along the Z axis for each face.
                BoxGenerator(const Vector3& HalfExtents, const Whole SegX = 1, const Whole SegY = 1, const Whole SegZ = 1);
                /// @brief Real constructor.
                /// @param HalfSizeX The half size of the box to generate on the X axis.
                /// @param HalfSizeY The half size of the box to generate on the Y axis.
                /// @param HalfSizeZ The half size of the box to generate on the Z axis.
                /// @param SegX The number of segments to generate along the X axis for each face.
                /// @param SegY The number of segments to generate along the Y axis for each face.
                /// @param SegZ The number of segments to generate along the Z axis for each face.
                BoxGenerator(const Real HalfSizeX, const Real HalfSizeY, const Real HalfSizeZ, const Whole SegX = 1, const Whole SegY = 1, const Whole SegZ = 1);
                /// @brief Class destructor.
                virtual ~BoxGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc MeshGenerator::AddToTriangleBuffer(TriangleBuffer&) const
                virtual void AddToTriangleBuffer(TriangleBuffer& Buffer) const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the half size along the X axis.
                /// @exception If the size is set to 0 or less, a PARAMETERS_EXCEPTION will be thrown.
                /// @param HalfSizeX The half size to set for the generated box on the X axis.
                /// @return Returns a reference to this.
                BoxGenerator& SetHalfSizeX(const Real HalfSizeX);
                /// @brief Sets the half size along the Y axis.
                /// @exception If the size is set to 0 or less, a PARAMETERS_EXCEPTION will be thrown.
                /// @param HalfSizeY The half size to set for the generated box on the Y axis.
                /// @return Returns a reference to this.
                BoxGenerator& SetHalfSizeY(const Real HalfSizeY);
                /// @brief Sets the half size along the Z axis.
                /// @exception If the size is set to 0 or less, a PARAMETERS_EXCEPTION will be thrown.
                /// @param HalfSizeZ The half size to set for the generated box on the Z axis.
                /// @return Returns a reference to this.
                BoxGenerator& SetHalfSizeZ(const Real HalfSizeZ);
                /// @brief Sets the half size of the box.
                /// @param HalfExtents The half size of the box that will be generated.
                /// @return Returns a reference to this.
                BoxGenerator& SetHalfExtents(const Vector3& HalfExtents);

                /// @brief Sets the number of segments along X axis.
                /// @remarks The default number of segments on the X axis is 1.
                /// @exception If the number of segments is set to 0, a PARAMETERS_EXCEPTION will be thrown.
                /// @param SegX The number of segments to set along the X axis.
                /// @return Returns a reference to this.
                BoxGenerator& SetNumSegX(const Whole SegX);
                /// @brief Sets the number of segments along Y axis.
                /// @remarks The default number of segments on the Y axis is 1.
                /// @exception If the number of segments is set to 0, a PARAMETERS_EXCEPTION will be thrown.
                /// @param SegX The number of segments to set along the Y axis.
                /// @return Returns a reference to this.
                BoxGenerator& SetNumSegY(const Whole SegY);
                /// @brief Sets the number of segments along Z axis.
                /// @remarks The default number of segments on the Z axis is 1.
                /// @exception If the number of segments is set to 0, a PARAMETERS_EXCEPTION will be thrown.
                /// @param SegX The number of segments to set along the Z axis.
                /// @return Returns a reference to this.
                BoxGenerator& SetNumSegZ(const Whole SegZ);
            };//BoxGenerator
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
