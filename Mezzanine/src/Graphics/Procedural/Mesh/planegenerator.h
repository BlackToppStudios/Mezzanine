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
#ifndef _graphicsproceduralplanegenerator_h
#define _graphicsproceduralplanegenerator_h

#include "Graphics/Procedural/Mesh/meshgenerator.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A generator class for a plane mesh.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB PlaneGenerator : public MeshGenerator<PlaneGenerator>
            {
            protected:
                /// @internal
                /// @brief The facing direction of the plane.
                Vector3 PlaneNormal;
                /// @internal
                /// @brief The size of the plane.
                Vector2 PlaneSize;
                /// @internal
                /// @brief The number of segments to generate on the X axis.
                Whole NumSegX;
                /// @internal
                /// @brief The number of segments to generate on the Y axis.
                Whole NumSegY;
            public:
                /// @brief Blank constructor.
                PlaneGenerator();
                /// @brief Descriptive constructor.
                /// @param Norm The facing direction of the plane.
                /// @param Size The size of the plane.
                PlaneGenerator(const Vector3& Norm, const Vector2& Size);
                /// @brief Class destructor.
                virtual ~PlaneGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc MeshGenerator::AddToTriangleBuffer(TriangleBuffer&) const
                virtual void AddToTriangleBuffer(TriangleBuffer& Buffer) const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the normal of the plane.
                /// @exception If the normal is zero length a PARAMETERS_EXCEPTION will be thrown.
                /// @param Norm The facing direction to apply to the plane.
                /// @return Returns a reference to this.
                PlaneGenerator& SetNormal(const Vector3& Norm);

                /// @brief Sets the size of this plane on the X axis.
                /// @exception If the size passed in is zero or less a PARAMETERS_EXCEPTION will be thrown.
                /// @param SizeX The size to set for the generated plane on the X axis.
                /// @return Returns a reference to this.
                PlaneGenerator& SetSizeX(const Real SizeX);
                /// @brief Sets the size of this plane on the Y axis.
                /// @exception If the size passed in is zero or less a PARAMETERS_EXCEPTION will be thrown.
                /// @param SizeX The size to set for the generated plane on the Y axis.
                /// @return Returns a reference to this.
                PlaneGenerator& SetSizeY(const Real SizeY);
                /// @brief Sets the size of the plane.
                /// @exception If the size passed in on either axis is zero or less a PARAMETERS_EXCEPTION will be thrown.
                /// @param Size The size to set for the generated plane.
                /// @return Returns a reference to this.
                PlaneGenerator& SetSize(const Vector2 Size);

                /// @brief Sets the number of segements along local X axis.
                /// @remarks The default number of segments on the X axis is 1.
                /// @exception If the number of segments is set to 0, a PARAMETERS_EXCEPTION will be thrown.
                /// @param SegX The number of segments to set along the X axis.
                /// @return Returns a reference to this.
                PlaneGenerator& SetNumSegX(const Whole SegX);
                /// @brief Sets the number of segments along local Y axis.
                /// @remarks The default number of segments on the Y axis is 1.
                /// @exception If the number of segments is set to 0, a PARAMETERS_EXCEPTION will be thrown.
                /// @param SegY The number of segments to set along the Y axis.
                /// @return Returns a reference to this.
                PlaneGenerator& SetNumSegY(const Whole SegY);
            };//PlaneGenerator
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
