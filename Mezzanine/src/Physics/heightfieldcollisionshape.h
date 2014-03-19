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
#ifndef _physicsheightfieldcollisionshape_h
#define _physicsheightfieldcollisionshape_h

#include "Physics/fieldcollisionshape.h"

class btHeightfieldTerrainShape;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class HeightfieldCollisionShape
        /// @headerfile collisionshape.h
        /// @brief A series of values that store hieght in a grid like fashion
        /// @details A fast terrain implementation that comes at the cost of
        /// overhangs and fine detail. This can be worked around by adding actors for
        /// additional terrain.
        ///////////////////////////////////////
        class MEZZ_LIB HeightfieldCollisionShape : public FieldCollisionShape
        {
        protected:
            /// @internal
            /// @brief The internal implementation.
            btHeightfieldTerrainShape* HeightfieldShape;
        public:
            /// @brief Class Constructor.
            /// @param Name The name of this Shape.
            HeightfieldCollisionShape(const String& Name);
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            HeightfieldCollisionShape(const String& Name, btHeightfieldTerrainShape* BulletShape);
            /// @brief Class Destructor.
            virtual ~HeightfieldCollisionShape();

            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
        };//HeightfieldCollisionShape
    }//Physics
}//Mezzanine

#ifndef SWIG
/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::CollisionShape& ShapeToSerialize)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::HeightfieldCollisionShape& ShapeToSerialize);
/// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::CollisionShape& x)
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::HeightfieldCollisionShape& x);
/// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CollisionShape& x)
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::HeightfieldCollisionShape& x);
#endif

#endif
