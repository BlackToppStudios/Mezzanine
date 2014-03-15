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
#ifndef _physicssoftcollisionshape_h
#define _physicssoftcollisionshape_h

#include "Physics/meshcollisionshape.h"

class btSoftBodyCollisionShape;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class SoftCollisionShape
        /// @headerfile softcollisionshape.h
        /// @brief A collision shape for soft proxies.
        /// @details This collision shape is different from all the other collision shapes because it's
        /// meant to be used for only one type of object: SoftProxys.  This shape doesn't use triangles
        /// in the traditional manner other mesh shapes do, but rather it uses the individual internal
        /// nodes that make the SoftProxy as points for building triangles to process collisions with.  As
        /// a result a SoftBodyCollisionShape should never be reused and should be kept unique to the Actor
        /// it applies to. @n @n
        /// Normally the SoftProxy will be responsible for the creation of it's own collision shape, so
        /// the user should never have to.  So there are no non-internal constructors provided for this class.
        ///////////////////////////////////////
        class MEZZ_LIB SoftCollisionShape : public MeshCollisionShape
        {
        protected:
            btSoftBodyCollisionShape* SoftShape;
        public:
            /// @internal
            /// @brief Internal Constructor.
            /// @param Name The name of this Shape.
            /// @param BulletShape The internal shape this shape is based on.
            SoftCollisionShape(const String& Name, btSoftBodyCollisionShape* BulletShape);
            /// @brief Class Destructor.
            virtual ~SoftCollisionShape();
            /// @copydoc CollisionShape::GetType()
            virtual CollisionShape::ShapeType GetType() const;
        };//SoftCollisionShape
    }//Physics
}//Mezzanine

#ifndef SWIG
/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::CollisionShape& ShapeToSerialize)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::SoftCollisionShape& ShapeToSerialize);
/// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::CollisionShape& x)
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::SoftCollisionShape& x);
/// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CollisionShape& x)
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::SoftCollisionShape& x);
#endif

#endif
