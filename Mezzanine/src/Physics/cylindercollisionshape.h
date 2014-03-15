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
#ifndef _physicscylindercollisionshape_h
#define _physicscylindercollisionshape_h

#include "Physics/primitivecollisionshape.h"

class btCylinderShape;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class CylinderCollisionShape
        /// @headerfile collisionshape.h
        /// @brief A cylinder physics shape.
        /// @details A primitive cylinder shape.  Like Capsules and Cones Cylinders can be
        /// aligned to any of the 3 linear axes(X, Y, or Z).
        ///////////////////////////////////////
        class MEZZ_LIB CylinderCollisionShape : public PrimitiveCollisionShape
        {
        protected:
            /// @internal
            /// @brief Does the initialization for a Cylinder
            /// @param Name The name of the shape to initialize
            /// @param HalfExtents This of the shape to initialize
            /// @param UpAxis which direction gets the flat 'top' of the cylinder perpendicular ot it.
            void Construct(const String& Name, const Vector3& HalfExtents, StandardAxis UpAxis);

            /// @internal
            /// @brief Does the initialization for a Cylinder
            /// @param Name The name of the shape to initialize
            /// @param BulletShape A pointer to a valid fully constructed internal Collision shape.
            void Construct(const String& Name, btCylinderShape* BulletShape);

        public:
            /// @brief Verbose Vector Constructor.
            /// @param Name The name of this Shape.
            /// @param Radius The radius of the Cylinder.
            /// @param Height The height of the Cylinder, Height is indicated in the direction of the upAxis.
            /// @param UpAxis Which axis the cylinder is to be oriented along. Typical usage is for
            /// a capsule to be oriented on the Y axis(0,1,0), which would make it stand upright.
            CylinderCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis);
            /// @brief Verbose Constructor.
            /// @param Name The name of this Shape.
            /// @param Radius The radius of the Cylinder.
            /// @param Height The height of the Cylinder, Height is indicated in the direction of the upAxis.
            /// @param UpAxis Which StandardAxis the cylinder is to be oriented along.
            CylinderCollisionShape(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis);
            /// @brief Terse Vector Constructor.
            /// @param Name The name of this Shape.
            /// @param HalfExtents A Vector3 with the Height of the cylinder on the axis corresponding to the UpAxis and the Radius store in the other two axis.
            /// @param UpAxis Which axis the cylinder is to be oriented along. Typical usage is for
            /// a capsule to be oriented on the Y axis(0,1,0), which would make it stand upright.
            CylinderCollisionShape(const String& Name, const Vector3& HalfExtents, const Vector3& UpAxis);
            /// @brief Terse Constructor.
            /// @param Name The name of this Shape.
            /// @param HalfExtents A Vector3 with the Height of the cylinder on the axis corresponding to the UpAxis and the Radius store in the other two axis.
            /// @param UpAxis Which StandardAxis the cylinder is to be oriented along.
            CylinderCollisionShape(const String& Name, const Vector3& HalfExtents, StandardAxis UpAxis);
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            CylinderCollisionShape(const String& Name, btCylinderShape* BulletShape);
            /// @copydoc BoxCollisionShape::BoxCollisionShape(XML::Node OneNode)
            CylinderCollisionShape(XML::Node OneNode);
            /// @brief Class Destructor.
            virtual ~CylinderCollisionShape();

            /// @brief A (very) simple way to create a Vector3 containing the Half Extents of a Cylinder with given dimenions/
            /// @param Radius The radius of the Cylinder.
            /// @param Height The height of the Cylinder, Height is indicated in the direction of the upAxis.
            /// @param UpAxis Which StandardAxis the cylinder is to be oriented along.
            static Vector3 CreateHalfExtents(const Real& Radius, const Real& Height, StandardAxis UpAxis);
            /// @brief Gets the half extents used to construct this cylinder.
            /// @return Returns a vector3 containing the half extents of this cylinder.
            virtual Vector3 GetHalfExtents() const;
            /// @brief Gets the half extents with padding.
            /// @return Returns a vector3 containing the half extents with the margin added to each axis.
            virtual Vector3 GetHalfExtentsWithMargin() const;
            /// @brief Gets the radius of the cylinder.
            /// @return Returns a real representing the radius at the base of the cylinder.
            virtual Real GetRadius() const;
            /// @brief Gets which axis this cylinder is oriented along.
            /// @return Returns a Vector3 representing which local axis is oriented along the world up axis.
            virtual Vector3 GetUpAxis() const;
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
            /// @copydoc CollisionShape::GetBulletShape
            virtual btCylinderShape* GetBulletCylinderShape() const;

            /// @copydoc CollisionShape::GetBulletShape
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;
            /// @copydoc CollisionShape::GetBulletShape
            virtual void ProtoDeSerialize(const XML::Node& OneNode);
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "CylinderCollisionShape"
            static String SerializableName();
        };// CylinderCollisionShape
    }//Physics
}//Mezzanine

#ifndef SWIG
/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::CollisionShape& ShapeToSerialize)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::CylinderCollisionShape& ShapeToSerialize);
/// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::CollisionShape& x)
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::CylinderCollisionShape& x);
/// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CollisionShape& x)
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CylinderCollisionShape& x);
#endif

#endif
