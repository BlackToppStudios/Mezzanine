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
#ifndef _physicsstaticmeshcollisionshape_h
#define _physicsstaticmeshcollisionshape_h

#include "Physics/meshcollisionshape.h"

class btBvhTriangleMeshShape;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class StaticMeshCollisionShape
        /// @brief A triangle mesh collsion shape based on a graphics mesh.
        /// @details As the name suggests, this is a TriMesh shape intended for static objects.  If this
        /// shape is used in a dynamic object, it will simply not provide any collisions with other objects. @n @n
        /// This shape will provide "pixel-perfect" collisions like the DynamicMeshCollisionShape class, but
        /// is much more performant at the cost of only being usable on static objects.
        ///////////////////////////////////////
        class MEZZ_LIB StaticMeshCollisionShape : public MeshCollisionShape
        {
        protected:
            btBvhTriangleMeshShape* BvhShape;
        public:
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            StaticMeshCollisionShape(const String& Name, btBvhTriangleMeshShape* BulletShape);
            /// @brief Class Destructor.
            virtual ~StaticMeshCollisionShape();

            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
        };//StaticMeshCollisionShape
    }//Physics
}//Mezzanine

#ifndef SWIG
/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::CollisionShape& ShapeToSerialize)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::StaticMeshCollisionShape& ShapeToSerialize);
/// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::CollisionShape& x)
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::StaticMeshCollisionShape& x);
/// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CollisionShape& x)
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::StaticMeshCollisionShape& x);
#endif

#endif
