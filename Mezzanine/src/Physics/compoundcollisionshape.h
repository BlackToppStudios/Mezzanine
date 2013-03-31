//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _physicscompoundcollisionshape_h
#define _physicscompoundcollisionshape_h

#include "Physics/collisionshape.h"

class btCompoundShape;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class CompoundCollisionShape
        /// @headerfile collisionshape.h
        /// @brief A collision shape composed of many other collision shapes placed and oriented in local space.
        /// @details This shape is the sum of all it's child shapes.  Unlike the MultiSphereCollisionShape and the
        /// ConvexHullCollisionShape, this shape does not form a convex hull of it's children.  When populating with
        /// primitives, Compound shapes offer the most flexibility with the best performace that you can get. @n @n
        /// When deleted a CompoundCollisionShape will clean up all of it's child shapes.
        ///////////////////////////////////////
        class MEZZ_LIB CompoundCollisionShape : public CollisionShape
        {
            protected:
                /// @brief Storage for Child shapes.
                std::vector<CollisionShape*> ChildShapes;
            public:
                /// @brief Class Constructor.
                /// @param Name The name of this Shape.
                CompoundCollisionShape(const String& Name);
                /// @internal
                /// @brief Internal Constructor.
                /// @param Name The name of this Shape.
                /// @param BulletShape The internal shape this shape is based on.
                CompoundCollisionShape(const String& Name, btCompoundShape* BulletShape);
                /// @brief Class Destructor.
                virtual ~CompoundCollisionShape();

                /// @brief Adds a shape to this compound shape.
                /// @param Child The shape to be added to this shape.
                /// @param ChildLocation The location this child is to have in local space.
                /// @param ChildRotation The rotation to apply to this child in local space.
                virtual void AddChildShape(CollisionShape* Child, const Vector3& ChildLocation, const Quaternion& ChildRotation);
                /// @brief Adds a shape to this compound shape.
                /// @param Child The shape to be added to this shape.
                /// @param ChildLocation The location this child is to have in local space.
                virtual void AddChildShape(CollisionShape* Child, const Vector3& ChildLocation);
                /// @brief Adds a shape to this compound shape.
                /// @param Child The shape to be added to this shape.
                /// @param ChildLocation The location and Rotation this child is to have in local space.
                virtual void AddChildShape(CollisionShape* Child, const Transform& ChildLocation);
                /// @brief Gets the number of children belonging to this compound shape.
                /// @return Returns the number of children belonging to this shape.
                virtual Whole GetNumChildren() const;
                /// @brief Get a specific child
                /// @param Index A number indicating which CollisionShape you want a pointer to.
                /// @return A pointer to the Specified CollisionShape
                virtual CollisionShape* GetChild(Whole Index) const;
                /// @brief Removes a Child shape from this compound shape.
                /// @param Child The child shape to be removed.
                virtual void RemoveChildShape(CollisionShape* Child);
                /// @brief Removed a Child shape from this compound shape by index.
                /// @param ChildIndex The index of the child shape to be removed.
                virtual void RemoveChildShape(const Whole& ChildIndex);

                /// @copydoc CollisionShape::GetType()
                virtual CollisionShape::ShapeType GetType() const;
                /// @copydoc CollisionShape::GetBulletShape
                virtual btCompoundShape* GetBulletCompoundShape() const;

                // Serializable
                /// @copydoc CollisionShape::GetBulletShape
                virtual void ProtoSerialize(XML::Node& CurrentRoot) const;
                // DeSerializable
                /// @copydoc CollisionShape::GetBulletShape
                virtual void ProtoDeSerialize(const XML::Node& OneNode);
                /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
                /// @return A string containing "CompoundCollisionShape"
                static String SerializableName();

        };//compoundcollisionshape
    }//Physics
}//Mezzanine

/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::CollisionShape& ShapeToSerialize)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::CompoundCollisionShape& ShapeToSerialize);
/// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::CollisionShape& x)
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::CompoundCollisionShape& x);
/// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CollisionShape& x)
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CompoundCollisionShape& x);


#endif
