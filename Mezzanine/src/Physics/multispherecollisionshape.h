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
#ifndef _physicsmultispherecollisionshape_h
#define _physicsmultispherecollisionshape_h

#include "Physics/primitivecollisionshape.h"

class btMultiSphereShape;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class MultiSphereCollisionShape
        /// @headerfile collisionshape.h
        /// @brief A physics shape comprised of multiple sphere's placed in local space.
        /// @details It's important to note that this shape isn't just a bunch of placed sphere's, but
        /// a convex hull comprised of all the provided sphere's.  You can make a capsule shape with
        /// two sphere's, one above the other(internally this is how it works).
        ///////////////////////////////////////
        class MEZZ_LIB MultiSphereCollisionShape : public PrimitiveCollisionShape
        {
        protected:
            /// @internal
            /// @brief Creates a btMultiSphereShape* from two vectors to help unify constructor logic
            /// @param Name The name of this name
            /// @param Locations An std::vector containing all the locations, this must have the same amount of locations as there are radii
            /// @param Radii An std::vector containing all the Radiuses, this must have the same amount of radii as there are locations
            /// @throw An out of bounds exception if the there are differing amounts of radii and locations.
            void Construct(const String& Name, const std::vector<Vector3>& Locations, const std::vector<Real>& Radii);
            /// @internal
            /// @brief Constructs the shape when the bullet is already prepared
            /// @param BulletShape The bullet shape to use
            /// @param Name The name of this name
            void Construct(const String& Name, btMultiSphereShape* BulletShape);
        public:
            /// @brief Class Constructor.
            /// @param Name The name of this Shape.
            MultiSphereCollisionShape(const String& Name, const std::vector<Vector3>& Locations, const std::vector<Real>& Radii);
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            MultiSphereCollisionShape(const String& Name, btMultiSphereShape* BulletShape);
            /// @copydoc BoxCollisionShape::BoxCollisionShape(XML::Node OneNode)
            MultiSphereCollisionShape(XML::Node OneNode);

            /// @brief Class Destructor.
            virtual ~MultiSphereCollisionShape();
            /// @brief Gets the location(in local space) of the sphere at the specified index.
            /// @return Returns a vector3 representing the location of the specified sphere.
            /// @param Index The index of the desired sphere.
            virtual Vector3 GetSphereLocation(const Whole& Index) const;
            /// @brief Gets the radius of the sphere at the specified index.
            /// @return Returns a Real representing the radius of the specified sphere.
            /// @param Index The index of the desired sphere.
            virtual Real GetSphereRadius(const Whole& Index) const;
            /// @brief Gets the number of spheres contained within this shape.
            /// @return Returns a Whole representing the number of spheres in this shape.
            virtual Whole GetNumSpheres() const;
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
            /// @copydoc CollisionShape::GetBulletShape
            virtual btMultiSphereShape* GetMultiSphereShape() const;

            /// @copydoc CollisionShape::GetBulletShape
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;
            /// @copydoc CollisionShape::GetBulletShape
            virtual void ProtoDeSerialize(const XML::Node& OneNode);
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "MultiSphereCollisionShape"
            static String SerializableName();
        };//MultiSphereCollisionShape
    }//Physics
}//Mezzanine

/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::CollisionShape& ShapeToSerialize)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::MultiSphereCollisionShape& ShapeToSerialize);
/// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::CollisionShape& x)
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::MultiSphereCollisionShape& x);
/// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CollisionShape& x)
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::MultiSphereCollisionShape& x);

#endif
