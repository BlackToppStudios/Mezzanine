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
#ifndef _physicsconecollisionshape_h
#define _physicsconecollisionshape_h

#include "Physics/primitivecollisionshape.h"

class btConeShape;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class ConeCollisionShape
        /// @headerfile collisionshape.h
        /// @brief A cone physics shape.
        /// @details Keep in mind when building a cone shape, the provided radius is for the base of the cone,
        /// but the pivot point of the shape is still at it's center and not it's base.  Like Capsules and Cylinders,
        /// Cones can be aligned to any of the 3 linear axes(X, Y, or Z).
        ///////////////////////////////////////
        class MEZZ_LIB ConeCollisionShape : public PrimitiveCollisionShape
        {
        protected:
            /// @internal
            /// @brief Performe shared contructor work.
            /// @param Name The name of this Shape.
            /// @param Radius The radius of the base of the Cone.
            /// @param Height The height of the Cone.
            /// @param UpAxis Which axis the Cone is to be oriented along. Uses StandardAxis enum, Axis_Y to make a
            /// vertical capsule
            void Construct(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis);
            /// @internal
            /// @brief Simulate some messed up the physics library does
            /// @return A Vector3 containing some StandarAxis based on what needs to go where.
            Vector3 GetAxisMathBS() const;
        public:
            /// @brief Class Constructor.
            /// @param Name The name of this Shape.
            /// @param Radius The radius of the base of the Cone.
            /// @param Height The height of the Cone.
            /// @param UpAxis Which axis the Cone is to be oriented along.  Typical usage is for
            /// a capsule to be oriented on the Y axis(0,1,0), which would make it stand upright.
            ConeCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis);
            /// @brief Class Constructor.
            /// @param Name The name of this Shape.
            /// @param Radius The radius of the base of the Cone.
            /// @param Height The height of the Cone.
            /// @param UpAxis Which axis the Cone is to be oriented along. Uses StandardAxis enum, Axis_Y to make a
            /// vertical capsule
            ConeCollisionShape(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis);
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            ConeCollisionShape(const String& Name, btConeShape* BulletShape);
            /// @copydoc BoxCollisionShape::BoxCollisionShape(XML::Node OneNode)
            ConeCollisionShape(XML::Node OneNode);
            /// @brief Class Destructor.
            virtual ~ConeCollisionShape();

            /// @brief Gets the radius of the cone, as used for collision checking.
            /// @return Returns a real representing the radius at the base of the cone.
            virtual Real GetRadius() const;
            /// @brief Gets the height of the cone, as used for collision checking.
            /// @return Returns a real representing height of the cone.
            virtual Real GetHeight() const;
            /// @brief Gets the radius of the cone, as originally passed in.
            /// @return Returns a real representing the radius at the base of the cone.
            virtual Real GetCleanRadius() const;
            /// @brief Gets the height of the cone, as originally passed in.
            /// @return Returns a real representing height of the cone.
            virtual Real GetCleanHeight() const;
            /// @brief Which axis is up defines which 2 axis are used to scale the radius.
            /// @return A value that represent the scaling that only the radius undergoes.
            Real GetRadiusScaling() const;
            /// @brief Which axis is up defines which axis is used to scale height.
            /// @return A value that represent the scaling that only the height undergoes.
            Real GetHeightScaling() const;

            /// @brief Gets which axis this cone is oriented along.
            /// @return Returns a Vector3 representing which local axis is oriented along the world up axis.
            virtual Vector3 GetUpAxis() const;
            /// @brief Gets which axis this cone is oriented along.
            /// @return Returns a StandardAxis representing which local axis is oriented along the world up axis.
            virtual StandardAxis GetUpStandardAxis() const;
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
            /// @brief Gets a pointer to the upcasted internal shape.
            /// @return Returns a pointer to the internal Cone shape this class gets it's functionality from.
            virtual btConeShape* GetBulletConeShape() const;

            /// @copydoc CollisionShape::ProtoSerialize(XML::Node& CurrentRoot) const
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;
            /// @copydoc CollisionShape::ProtoDeSerialize(const XML::Node& OneNode)
            virtual void ProtoDeSerialize(const XML::Node& OneNode);

            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "ConeCollisionShape"
            static String GetSerializableName();
        };//ConeCollisionShape
    }//Physics
}//Mezzanine

#ifndef SWIG
/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::CollisionShape& ShapeToSerialize)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::ConeCollisionShape& ShapeToSerialize);
/// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::CollisionShape& x)
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::ConeCollisionShape& x);
/// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CollisionShape& x)
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::ConeCollisionShape& x);
#endif

#endif
