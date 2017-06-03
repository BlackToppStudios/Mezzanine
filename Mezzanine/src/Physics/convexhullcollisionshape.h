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
#ifndef _physicsconvexhullcollisionshape_h
#define _physicsconvexhullcollisionshape_h

#include "Physics/primitivecollisionshape.h"

class btConvexHullShape;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class ConvexHullCollisionShape
        /// @headerfile collisionshape.h
        /// @brief A simple convex shape built from a low number of points in local space.
        /// @details A convex hull is commonly used to generate a simple approximation of another, and
        /// more complicated, shape.  Usually the number of points in a convex hull doesn't exceed 32.  @n @n
        /// When a convex hull is generated automatically from an algorithm, the best way to think of
        /// the resulting shape in relation to the original shape is if you were to wrap a rubber band
        /// around it on one plane.  Then do this for all planes and all surfaces.  The resulting shape looks
        /// and behaves similar to that.
        ///////////////////////////////////////
        class MEZZ_LIB ConvexHullCollisionShape : public PrimitiveCollisionShape
        {
        public:
            /// @brief Class Constructor.
            /// @param Name The name of this Shape.
            /// @param Points A vector of vector3's containing all the points in local space to construct this shape from.
            ConvexHullCollisionShape(const String& Name, const std::vector<Vector3>& Points);
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            ConvexHullCollisionShape(const String& Name, btConvexHullShape* BulletShape);
            /// @copydoc BoxCollisionShape::BoxCollisionShape(XML::Node OneNode)
            ConvexHullCollisionShape(XML::Node OneNode);
            /// @brief Class Destructor.
            virtual ~ConvexHullCollisionShape();
            /// @brief Adds a point to this shape.
            /// @param Point The location in local space to be added.
            virtual void AddPoint(const Vector3& Point);

            /// @brief Gets an unscaled stored point in this ConvexHull.
            /// @return Returns a Vector3 representing the unscaled location in local space of the specified point.
            /// @param Index The index of the point desired.
            virtual Vector3 GetUnscaledPoint(const Whole& Index) const;
            /// @brief Gets a stored point as it is scaled in this ConvexHull.
            /// @return Returns a Vector3 representing the scaled location in local space of the specified point.
            /// @param Index The index of the point desired.
            virtual Vector3 GetPoint(const Whole& Index) const;
            /// @brief Gets the total number of points being stored in this shape.
            /// @return Returns a whole representing the current number of points in this shape.
            virtual Whole GetNumPoints() const;
            /// @brief Checks to see if a point in local space is inside this shape.
            /// @return Returns a Boole indicating whether or not the specified point is inside the shape.
            /// @param Location The specified point to perform the check.
            /// @param Tolerance The amount of leeway to give in the check.  If the distance from the
            /// specified point is equal or less then the Tolorance provided then this will return true.
            virtual Boole IsInside(const Vector3& Location, const Real& Tolerance) const;
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
            /// @copydoc CollisionShape::GetBulletShape
            virtual btConvexHullShape* GetBulletHullShape() const;

            /// @copydoc CollisionShape::GetBulletShape
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;
            /// @copydoc CollisionShape::GetBulletShape
            virtual void ProtoDeSerialize(const XML::Node& OneNode);
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "ConeCollisionShape"
            static String GetSerializableName();
        };// ConvexHullCollisionShape
    }//Physics
}//Mezzanine

#ifndef SWIG
/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::CollisionShape& ShapeToSerialize)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::ConvexHullCollisionShape& ShapeToSerialize);
/// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::CollisionShape& x)
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::ConvexHullCollisionShape& x);
/// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CollisionShape& x)
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::ConvexHullCollisionShape& x);
#endif

#endif
