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
#ifndef _physicscapsulecollisionshape_h
#define _physicscapsulecollisionshape_h

#include "Physics/primitivecollisionshape.h"

class btCapsuleShape;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class CapsuleCollisionShape
        /// @headerfile collisionshape.h
        /// @brief A capsule physics shape.
        /// @details This shape is commonly used for characters.  Keep in mind the total height of any
        /// capsule is height+2*radius, since the height provided only applies to the cylinder portion
        /// of the capsule.  Like Cones and Cylinders, Capsules can be aligned to any of the 3 linear
        /// axes(X, Y, or Z).
        ///////////////////////////////////////
        class MEZZ_LIB CapsuleCollisionShape : public PrimitiveCollisionShape
        {
        protected:
            /// @internal
            /// @brief Performed share contructor work.
            /// @param Name The name of this Shape.
            /// @param Radius The radius of the capsule.
            /// @param Height The height of the cylindrical portion of the capsule.  Total height would be Height+2*radius.
            /// @param UpAxis Which axis the Capsule is to be oriented along.  Typical usage is for
            /// a capsule to be oriented on the Y axis(0,1,0), which would make it stand upright.
            void Construct(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis);
        public:
            /// @brief Class Constructor.
            /// @param Name The name of this Shape.
            /// @param Radius The radius of the capsule.
            /// @param Height The height of the cylindrical portion of the capsule.  Total height would be Height+2*radius.
            /// @param UpAxis Which axis the Capsule is to be oriented along.  Typical usage is for
            /// a capsule to be oriented on the Y axis(0,1,0), which would make it stand upright.
            CapsuleCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis);
            /// @brief Class Constructor.
            /// @param Name The name of this Shape.
            /// @param Radius The radius of the capsule.
            /// @param Height The height of the cylindrical portion of the capsule.  Total height would be Height+2*radius.
            /// @param UpAxis Which axis the Capsule is to be oriented along. Uses StandardAxis enum, Axis_Y to make a
            /// vertical capsule
            CapsuleCollisionShape(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis);
            /// @copydoc BoxCollisionShape::BoxCollisionShape(XML::Node OneNode)
            CapsuleCollisionShape(XML::Node OneNode);

            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            CapsuleCollisionShape(const String& Name, btCapsuleShape* BulletShape);
            /// @brief Class Destructor.
            virtual ~CapsuleCollisionShape();

            /// @brief Gets the radius of the capsule, as used in collision checking, with scaling and margin subtracted.
            /// @return Returns a real representing the radius of the capsule.
            virtual Real GetRadius() const;
            /// @brief Gets the height of the capsule, as used in collision checking, with scaling and margin subtracted.
            /// @return Returns a real representing the length of the cylindrical portion of the capsule.
            virtual Real GetHeight() const;
            /// @brief Gets the radius of the capsule, as the original value passed, without scaling and margin applied.
            /// @return Returns a real representing the radius of the capsule.
            virtual Real GetCleanRadius() const;
            /// @brief Gets the height of the capsule, as the original value passed, without scaling and margin applied.
            /// @return Returns a real representing the length of the cylindrical portion of the capsule.
            virtual Real GetCleanHeight() const;
            /* /// @brief Gets the radius of the capsule, with the region of padding used to aid collision Detection, with scaling applied and margin re-added.
            /// @return Returns a real representing the radius of the capsule.
            virtual Real GetRadiusWithMargin() const; */

            /// @brief Gets which axis this capsule is oriented along.
            /// @return Returns a Vector3 representing which local axis is oriented along the world up axis.
            virtual Vector3 GetUpAxis() const;
            /// @brief Gets which axis this capsule is oriented along.
            /// @return Returns a StandardAxis representing which local axis is oriented along the world up axis.
            virtual StandardAxis GetUpStandardAxis() const;
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
            /// @copydoc CollisionShape::GetBulletShape
            virtual btCapsuleShape* GetBulletCapsuleShape() const;

            // Serializable
            /// @copydoc CollisionShape::GetBulletShape
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;
            /// @copydoc CollisionShape::GetBulletShape
            virtual void ProtoDeSerialize(const XML::Node& OneNode);
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "CapsuleCollisionShape"
            static String GetSerializableName();
        };// CapsuleCollisionShape
    }//Physics
}//Mezzanine

#ifndef SWIG
/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::CollisionShape& ShapeToSerialize)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::CapsuleCollisionShape& ShapeToSerialize);
/// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::CollisionShape& x)
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::CapsuleCollisionShape& x);
/// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CollisionShape& x)
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CapsuleCollisionShape& x);
#endif


#endif
