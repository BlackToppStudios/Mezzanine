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
#ifndef _graphicsproceduralicospheregenerator_h
#define _graphicsproceduralicospheregenerator_h

#include "Graphics/Procedural/Mesh/meshgenerator.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A generator class for a sphere mesh where all triangles are the same size.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB IcoSphereGenerator : public MeshGenerator<IcoSphereGenerator>
            {
            protected:
                /// @internal
                /// @brief The radius of the sphere.
                Real SphereRadius;
                /// @internal
                /// @brief The number of iterations needed to build the sphere mesh.
                Whole NumIterations;
            public:
                /// @brief Class constructor.
                /// @param Radius The radius of the sphere.
                /// @param Iterations The number of iterations needed to build the sphere mesh.
                IcoSphereGenerator(const Real Radius, const Whole Iterations = 2);
                /// @brief Class destructor.
                virtual ~IcoSphereGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc MeshGenerator::AddToTriangleBuffer(TriangleBuffer&) const
                virtual void AddToTriangleBuffer(TriangleBuffer& Buffer) const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the radius of the sphere.
                /// @exception If the radius is set to 0 or less, a PARAMETERS_EXCEPTION will be thrown.
                /// @param Radius The radius of the sphere.
                /// @return Returns a reference to this.
                IcoSphereGenerator& SetRadius(const Real Radius);
                /// @brief Sets the number of iterations needed to build the sphere mesh.
                /// @remarks At only a single iteration, this generator will create an icosahedron.  Each iteration quadrupels the number of faces than the previous.
                /// This controls the resolution of the sphere, and ramps up quickly.
                /// @exception If the number of iterations is set to 0, a PARAMETERS_EXCEPTION will be thrown.
                /// @param Iterations The number of iterations needed to build the sphere mesh.
                /// @return Returns a reference to this.
                IcoSphereGenerator& SetNumIterations(const Whole Iterations);
            };//IcoSphereGenerator
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
