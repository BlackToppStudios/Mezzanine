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
#ifndef _graphicsproceduralplaneuvmodifier_h
#define _graphicsproceduralplaneuvmodifier_h

#include "Graphics/Procedural/Mesh/meshmodifier.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A modifier that will generate UV coordinates as they would need to be on a plane model.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB PlaneUVModifier : public MeshModifier
            {
            protected:
                /// @internal
                /// @brief The location of the center of the Plane.
                Vector3 PlaneCenter;
                /// @internal
                /// @brief The facing direction of the Plane.
                Vector3 PlaneNormal;
                /// @internal
                /// @brief The size of the bounded area of the plane.
                Vector2 PlaneSize;
            public:
                /// @brief Blank constructor.
                PlaneUVModifier();
                /// @brief Class destructor.
                virtual ~PlaneUVModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc MeshModifier::Modify(TriangleBuffer&)
                virtual void Modify(TriangleBuffer& Buffer);
                /// @copydoc MeshModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the central location of the Plane to project UV's onto.
                /// @param Center The location of the center of the Plane.
                /// @return Returns a reference to this.
                PlaneUVModifier& SetPlaneCenter(const Vector3& Center);
                /// @brief Sets the central location of the Plane to project UV's onto.
                /// @param X The location of the center of the Plane on the X axis.
                /// @param Y The location of the center of the Plane on the Y axis.
                /// @param Z The location of the center of the Plane on the Z axis.
                /// @return Returns a reference to this.
                PlaneUVModifier& SetPlaneCenter(const Real X, const Real Y, const Real Z);
                /// @brief Sets the facing direction of the Plane to project UV's onto.
                /// @param Normal The facing direction of the Plane.
                /// @return Returns a reference to this.
                PlaneUVModifier& SetPlaneNormal(const Vector3& Normal);
                /// @brief Sets the facing direction of the Plane to project UV's onto.
                /// @param X The X component of the directional vector the Plane is facing.
                /// @param Y The Y component of the directional vector the Plane is facing.
                /// @param Z The Z component of the directional vector the Plane is facing.
                /// @return Returns a reference to this.
                PlaneUVModifier& SetPlaneNormal(const Real X, const Real Y, const Real Z);
                /// @brief Sets the size of the Plane to project UV's onto.
                /// @param Size The size of the bounded area of the Plane.
                /// @return Returns a reference to this.
                PlaneUVModifier& SetPlaneSize(const Vector2& Size);
                /// @brief Sets the size of the Plane to project UV's onto.
                /// @param X The size of the bounded area of the Plane on the X axis.
                /// @param Y The size of the bounded area of the Plane on the Y axis.
                /// @return Returns a reference to this.
                PlaneUVModifier& SetPlaneSize(const Real X, const Real Y);
            };//PlaneUVModifier
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
