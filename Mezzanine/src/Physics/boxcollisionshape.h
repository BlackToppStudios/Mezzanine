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
#ifndef _physicsboxcollisionshape_h
#define _physicsboxcollisionshape_h

#include <datatypes.h>

#include "Physics/primitivecollisionshape.h"

class btBoxShape;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class BoxCollisionShape
        /// @headerfile collisionshape.h
        /// @brief A box physics shape.
        /// @details This is exactly what it sounds like.  A primitive shape of a box that is constructed
        /// by it's half extents.  Half extents can't be negative, but otherwise can be any value. The margin
        /// value is subtracted from the total size rather than added for this shape.
        /// @warning This does not fully implement ProtoDeSerialize, this must be deserialized with the Appropriate Deserializer.
        ///////////////////////////////////////
        class MEZZ_LIB BoxCollisionShape : public PrimitiveCollisionShape
        {
        public:
            /// @brief Class Constructor.
            /// @param Name The name of this shape.
            /// @param HalfExtents A vector3 representing half the shapes size on each axis.
            BoxCollisionShape(const String& Name, const Vector3& HalfExtents);
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this shape.
            /// @param BulletShape The internal shape this shape is based on.
            BoxCollisionShape(const String& Name, btBoxShape* BulletShape);

            /// @brief DeSerializing Constructor
            /// @param OneNode The node to use for constructing this shape
            BoxCollisionShape(XML::Node OneNode);

            /// @brief Class Destructor.
            virtual ~BoxCollisionShape();
            /// @brief Gets the half extents used to construct this box.
            /// @return Returns a vector3 containing the half extents of this box, with no scaling or margin (Original value).
            virtual Vector3 GetCleanHalfExtents() const;
            /// @brief Gets the current half extents used in collision checking.
            /// @return Returns a vector3 containing the half extents of this box, with scaling applied, minus the size of the margin(Original value * Scaling - margin).
            virtual Vector3 GetHalfExtents() const;
            /// @brief Gets the half extents with padding, this is the size the shape will appear to be.
            /// @return Returns a vector3 containing the half extents, with scaling applied, with the margin added to each axis (Original value * Scaling).
            virtual Vector3 GetHalfExtentsWithMargin() const;
            /// @brief Checks to see if a point in local space is inside this shape.
            /// @return Returns a Boole indicating whether or not the specified point is inside the shape.
            /// @param Location The specified point to perform the check.
            /// @param Tolerance The amount of leeway to give in the check.  If the distance from the
            /// specified point is equal or less then the Tolerance provided then this will return true.
            virtual Boole IsInside(const Vector3& Location, const Real& Tolerance) const;
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
            /// @copydoc CollisionShape::GetBulletShape
            virtual btBoxShape* GetBulletBoxShape() const;

            // Serializable
            /// @copydoc CollisionShape::GetBulletShape
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;
            /// @copydoc CollisionShape::GetBulletShape
            virtual void ProtoDeSerialize(const XML::Node& OneNode);
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "BoxCollisionShape"
            static String GetSerializableName();
        };//BoxCollisionShape
    }//Physics
}//Mezzanine

#ifndef SWIG
/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::CollisionShape& ShapeToSerialize)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::BoxCollisionShape& ShapeToSerialize);
#endif


#endif
