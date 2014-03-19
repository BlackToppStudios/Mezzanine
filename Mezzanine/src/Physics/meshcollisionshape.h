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
#ifndef _physicsmeshcollisionshape_h
#define _physicsmeshcollisionshape_h

#include "Physics/collisionshape.h"

class btConcaveShape;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class MeshCollisionShape
        /// @headerfile collisionshape.h
        /// @brief This is the base class for all Mesh shapes.
        /// @details A Mesh shape is any shape that uses a series of triangles to determine it's
        /// bounds.  As a rule all Mesh Shapes are considered Concave(even though the triangles don't
        /// need to form a concave shape, there is no way to assure the physics engine of that).  Mesh
        /// shapes are not very performant, and should only be used with static objects, such as terrain.
        ///////////////////////////////////////
        class MEZZ_LIB MeshCollisionShape : public CollisionShape
        {
        protected:
            /// @brief Sets the internal pointers on the base classes.
            void SetPointers(btConcaveShape* Shape);
        public:
            /// @brief Class Constructor.
            MeshCollisionShape();
            /// @brief Class Destructor.
            virtual ~MeshCollisionShape();
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const = 0;
            /// @copydoc CollisionShape::GetBulletShape
            virtual btConcaveShape* GetBulletConcaveShape() const;

            /// @copydoc CollisionShape::GetBulletShape
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;
            /// @copydoc CollisionShape::GetBulletShape
            virtual void ProtoDeSerialize(const XML::Node& OneNode);
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "MeshCollisionShape"
            static String SerializableName();
        };//MeshCollisionShape
    }//Physics
}//Mezzanine

#ifndef SWIG
/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::CollisionShape& ShapeToSerialize)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::MeshCollisionShape& ShapeToSerialize);
/// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::CollisionShape& x)
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::MeshCollisionShape& x);
/// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CollisionShape& x)
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::MeshCollisionShape& x);
#endif

#endif
