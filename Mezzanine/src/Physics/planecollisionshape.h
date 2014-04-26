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
#ifndef _physicsplanecollisionshape_h
#define _physicsplanecollisionshape_h

#include "plane.h"
#include "Physics/fieldcollisionshape.h"

class btStaticPlaneShape;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class PlaneCollisionShape
        /// @brief A Flat wall/floor of limitless size
        ///////////////////////////////////////
        class MEZZ_LIB PlaneCollisionShape : public FieldCollisionShape
        {
        protected:
            /// @internal
            /// @brief A pointer to the internal shape.
            btStaticPlaneShape* PlaneShape;
        public:
            /// @brief Math Constructor.
            /// @param Name The name of this Shape.
            /// @param Other A plane math class to copy the plane dimensions from.
            PlaneCollisionShape(const String& Name, const Plane& Other);
            /// @brief Descriptive constructor.
            /// @param Norm The positive direction of the plane.
            /// @param Constant The Constant with which to project the plane.
            PlaneCollisionShape(const String& Name, const Vector3& Norm, const Real Constant);
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            PlaneCollisionShape(const String& Name, btStaticPlaneShape* BulletShape);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            PlaneCollisionShape(const XML::Node& XMLNode);
            /// @brief Class Destructor.
            virtual ~PlaneCollisionShape();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the positive direction of the plane.
            /// @return Returns a Vector3 expressing the positive direction of this plane.
            Vector3 GetNormal() const;
            /// @brief Gets the constant with which to project the plane.
            /// @return Returns a Real expressing the constant used to project the plane.
            Real GetConstant() const;

            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc CollisionShape::ProtoSerialize(XML::Node&) const
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;
            /// @copydoc CollisionShape::ProtoDeSerialize(const XML::Node&)
            virtual void ProtoDeSerialize(const XML::Node& OneNode);
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "PlaneCollisionShape".
            static String SerializableName();
        };//PlaneCollisionShape
    }//Physics
}//Mezzanine

#ifndef SWIG
/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::CollisionShape& ShapeToSerialize)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::PlaneCollisionShape& ShapeToSerialize);
/// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::CollisionShape& x)
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::PlaneCollisionShape& x);
/// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CollisionShape& x)
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::PlaneCollisionShape& x);
#endif

#endif
