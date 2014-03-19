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
#ifndef _physicsfieldcollisionshape_h
#define _physicsfieldcollisionshape_h

#include "Physics/collisionshape.h"

class btConcaveShape;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class FieldCollisionShape
        /// @headerfile collisionshape.h
        /// @brief This is the base class for all Field shapes.
        /// @details A Mesh shape is any shape that is intended to be used as surface
        /// that extends for a a significant or even unending length. These can be used
        /// as the basis for ground and hills or as walls at the end of gameplay area.
        /// Performance is dependant on the complexity of the field, and are dependent
        /// on complexity.
        ///////////////////////////////////////
        class MEZZ_LIB FieldCollisionShape : public CollisionShape
        {
        protected:
            /// @brief Sets the internal pointers on the base classes.
            void SetPointers(btConcaveShape* Shape);
        public:
            /// @brief Class Constructor.
            FieldCollisionShape();
            /// @brief Class Destructor.
            virtual ~FieldCollisionShape();

            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const = 0;
            /// @copydoc CollisionShape::GetBulletShape
            virtual btConcaveShape* GetBulletConcaveShape() const;

            /// @copydoc CollisionShape::GetBulletShape
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;
            /// @copydoc CollisionShape::GetBulletShape
            virtual void ProtoDeSerialize(const XML::Node& OneNode);
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "FieldCollisionShape"
            static String SerializableName();
        };//FieldCollisionShape
    }//Physics
}//Mezzanine

#ifndef SWIG
/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::CollisionShape& ShapeToSerialize)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::FieldCollisionShape& ShapeToSerialize);
/// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::CollisionShape& x)
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::FieldCollisionShape& x);
/// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CollisionShape& x)
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::FieldCollisionShape& x);
#endif

#endif
