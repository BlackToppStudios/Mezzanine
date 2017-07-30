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
#ifndef _physicsuniversalconstraint_h
#define _physicsuniversalconstraint_h

#include "Physics/genericsixdofconstraint.h"

class btUniversalConstraint;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a constraint for sharing the rotation of one object along an angular axis with another.
        /// @details The UniversalConstriant shares some similarities with the Gear and Hinge2 constraints, but it is distinct in the nuance of
        /// it's behaviors.  @n
        /// With the Gear constraint you can set a ratio on which the amount of rotation that is transferred from the parent
        /// object to it's child.  With the UniversalConstraint the amount of rotation is evenly matched.  @n
        /// With the Hinge2 constraint the axes provided must always be orthogonal and rotations along those axis aren't really related.  The
        /// child can spin about it's own axis without really disrupting the parent whatsoever.  With the Universal constraint you must define
        /// two axes, one on the parent and one on the child, however those axes must be perpendicular to the axis you wish to rotate the object
        /// around.  @n
        /// After the constraint is constructed the constraint will attempt to keep the axes provided at 90 degrees apart, forcing the parent
        /// or child to rotate as needed to maintain that angular distance.
        class MEZZ_LIB UniversalConstraint : public GenericSixDofConstraint
        {
        protected:
            /// @copydoc TypedConstraint::_GetConstraintBase() const
            virtual btUniversalConstraint* Universal() const;

            /// @copydoc DualTransformConstraint::CreateConstraint(RigidProxy*, RigidProxy*, const Transform&, const Transform&)
            virtual void CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB);
            /// @internal
            /// @brief Conveniece method used primarily for serialization to create an internal constraint.
            /// @param RigidA A pointer to the first proxy that will be constrained.
            /// @param RigidB A pointer to the second proxy that will be constrained.
            /// @param Anchor The central point around both Axis1 and Axis 2 will connect and spin.
            /// @param Axis1 An axis perpendicular to the axis you wish to have the ProxyA spin.
            /// @param Axis2 An axis perpendicular to the axis you wish to have the ProxyB spin.
            virtual void CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Vector3& Anchor, const Vector3& Axis1, const Vector3& Axis2);
        public:
            /// @brief Convenience axis constructor.
            /// @remarks All axes passed in should be in world coordinates.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA A pointer to the first proxy that will be constrained.
            /// @param ProxyB A pointer to the second proxy that will be constrained.
            /// @param Anchor The central point around both Axis1 and Axis 2 will connect and spin.
            /// @param Axis1 An axis perpendicular to the axis you wish to have the ProxyA spin.
            /// @param Axis2 An axis perpendicular to the axis you wish to have the ProxyB spin.
            /// @param Creator A pointer to the manager that created this constraint.
            UniversalConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& Anchor, const Vector3& Axis1, const Vector3& Axis2, PhysicsManager* Creator);
            /// @brief Class constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA A pointer to the first proxy that will be constrained.
            /// @param ProxyB A pointer to the second proxy that will be constrained.
            /// @param TransA The offset and rotation from ProxyAs center of gravity to get to match an offset from ProxyB.
            /// @param TransB The offset and rotation from ProxyBs center of gravity.
            /// @param Creator A pointer to the manager that created this constraint.
            UniversalConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB, PhysicsManager* Creator);
            /// @brief XML constructor.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @param Creator A pointer to the manager that created this constraint.
            UniversalConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator);
            /// @brief Class destructor.
            virtual ~UniversalConstraint();

            ///////////////////////////////////////////////////////////////////////////////
            // Limits

            /// @brief Sets the maximum angular limits of this constraint.
            /// @param Ang1Max The maximum angle allowed on Axis1.
            /// @param Ang2Max The maximum angle allowed on Axis2.
            virtual void SetUpperLimit(const Real Ang1Max, const Real Ang2Max);
            /// @brief Sets the minimum angular limits of this constraint.
            /// @param Ang1Min The minimum angle allowed on Axis1.
            /// @param Ang2Min The minimum angle allowed on Axis2.
            virtual void SetLowerLimit(const Real Ang1Min, const Real Ang2Min);

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc Constraint::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @brief Get the name of the the XML tag the class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();
        };//UniversalConstraint
    }//Physics
}//Mezzanine

#endif
