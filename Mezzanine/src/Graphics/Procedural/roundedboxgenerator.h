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
#ifndef _graphicsproceduralroundedboxgenerator_h
#define _graphicsproceduralroundedboxgenerator_h

#include "Graphics/Procedural/meshgenerator.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A generator class for a rounded box mesh.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB RoundedBoxGenerator : public MeshGenerator<RoundedBoxGenerator>
            {
            protected:
                /// @internal
                /// @brief The size of the box to generate.
                Vector3 BoxSize;
                /// @internal
                /// @brief The radius of the rounded portion of the box corners.
                Real BoxChamferSize;
                /// @internal
                /// @brief The number of segments to generate along the length of the rounded edges.
                Whole NumChamferSeg;
                /// @internal
                /// @brief The number of segments to generate along the X axis for each side.
                Whole NumSegX;
                /// @internal
                /// @brief The number of segments to generate along the Y axis for each side.
                Whole NumSegY;
                /// @internal
                /// @brief The number of segments to generate along the Z axis for each side.
                Whole NumSegZ;

                /// @internal
                /// @brief Builds an "edge" of the rounded box, ie a quarter cylinder.
                /// @param Buffer The buffer to append the edge data to.
                /// @param XPos Expects either "1" or "-1" based on the side of the X axis to generate the edge on.
                /// @param YPos Expects either "1" or "-1" based on the side of the Y axis to generate the edge on.
                /// @param ZPos Expects either "1" or "-1" based on the side of the Z axis to generate the edge on.
                void _AddEdge(TriangleBuffer& Buffer, Int16 XPos, Int16 YPos, Int16 ZPos) const;
                /// @internal
                /// @brief Builds a "corner" of the rounded box, ie a 1/8th of a sphere.
                /// @param Buffer The buffer to append the corner data to.
                /// @param IsXPositive Whether or not the corner needing to be generated is on the positive side of the X axis.
                /// @param IsYPositive Whether or not the corner needing to be generated is on the positive side of the Y axis.
                /// @param IsZPositive Whether or not the corner needing to be generated is on the positive side of the Z axis.
                void _AddCorner(TriangleBuffer& Buffer, Boole IsXPositive, Boole IsYPositive, Boole IsZPositive) const;
            public:
                /// @brief Vector constructor.
                /// @param Size The size of the box to generate.
                /// @param ChamferSize The radius of the rounded portion of the box corners.
                /// @param SegX The number of segments to generate along the X axis for each face.
                /// @param SegY The number of segments to generate along the Y axis for each face.
                /// @param SegZ The number of segments to generate along the Z axis for each face.
                /// @param ChamferSeg The number of segments to generate along the length of the rounded edges.
                RoundedBoxGenerator(const Vector3& Size, const Real ChamferSize, const Whole SegX = 1, const Whole SegY = 1, const Whole SegZ = 1, const Whole ChamferSeg = 1);
                /// @brief Real constructor.
                /// @param SizeX The size of the box to generate on the X axis.
                /// @param SizeY The size of the box to generate on the Y axis.
                /// @param SizeZ The size of the box to generate on the Z axis.
                /// @param ChamferSize The radius of the rounded portion of the box corners.
                /// @param SegX The number of segments to generate along the X axis for each face.
                /// @param SegY The number of segments to generate along the Y axis for each face.
                /// @param SegZ The number of segments to generate along the Z axis for each face.
                /// @param ChamferSeg The number of segments to generate along the length of the rounded edges.
                RoundedBoxGenerator(const Real SizeX, const Real SizeY, const Real SizeZ, const Real ChamferSize, const Whole SegX = 1, const Whole SegY = 1, const Whole SegZ = 1, const Whole ChamferSeg = 1);
                /// @brief Class destructor.
                virtual ~RoundedBoxGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc MeshGenerator::AddToTriangleBuffer(TriangleBuffer&) const
                virtual void AddToTriangleBuffer(TriangleBuffer& Buffer) const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the size of the box along X axis.
                /// @exception If the size is set to 0 or less, a PARAMETERS_EXCEPTION will be thrown.
                /// @param SizeX The size to set for the generated box on the X axis.
                /// @return Returns a reference to this.
                RoundedBoxGenerator& SetSizeX(const Real SizeX);
                /// @brief Sets the size of the box along Y axis.
                /// @exception If the size is set to 0 or less, a PARAMETERS_EXCEPTION will be thrown.
                /// @param SizeY The size to set for the generated box on the Y axis.
                /// @return Returns a reference to this.
                RoundedBoxGenerator& SetSizeY(const Real SizeY);
                /// @brief Sets the size of the box along Z axis.
                /// @exception If the size is set to 0 or less, a PARAMETERS_EXCEPTION will be thrown.
                /// @param SizeZ The size to set for the generated box on the Z axis.
                /// @return Returns a reference to this.
                RoundedBoxGenerator& SetSizeZ(const Real SizeZ);
                /// @brief Sets the size of the box.
                /// @exception If the size is set to 0 or less, a PARAMETERS_EXCEPTION will be thrown.
                /// @param Size The size of the box that will be generated.
                /// @return Returns a reference to this.
                RoundedBoxGenerator& SetSize(const Vector3& Size);

                /// @brief Sets the size of the chamfer, ie the radius of the rounded part
                /// @exception If the size is set to 0 or less, a PARAMETERS_EXCEPTION will be thrown.
                /// @param ChamferSize The radius of the rounded portion of the box corners.
                /// @return Returns a reference to this.
                RoundedBoxGenerator& SetChamferSize(const Real ChamferSize);

                /// @brief Sets the number of segments along X axis
                /// @exception If the number of segments is set to 0, a PARAMETERS_EXCEPTION will be thrown.
                /// @param SegX The number of segments to generate along the X axis for each side.
                /// @return Returns a reference to this.
                RoundedBoxGenerator& SetNumSegX(const Whole SegX);
                /// @brief Sets the number of segments along Y axis
                /// @exception If the number of segments is set to 0, a PARAMETERS_EXCEPTION will be thrown.
                /// @param SegY The number of segments to generate along the Y axis for each side.
                /// @return Returns a reference to this.
                RoundedBoxGenerator& SetNumSegY(const Whole SegY);
                /// @brief Sets the number of segments along Z axis
                /// @exception If the number of segments is set to 0, a PARAMETERS_EXCEPTION will be thrown.
                /// @param SegZ The number of segments to generate along the Z axis for each side.
                /// @return Returns a reference to this.
                RoundedBoxGenerator& SetNumSegZ(const Whole SegZ);

                /// @brief Sets the number of segments along the rounded edge of the box.
                /// @exception If the number of segments is set to 0, a PARAMETERS_EXCEPTION will be thrown.
                /// @param ChamferSeg The number of segments to generate along the length of the rounded edges.
                /// @return Returns a reference to this.
                RoundedBoxGenerator& SetNumChamferSeg(const Whole ChamferSeg);
            };//RoundedBoxGenerator
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
