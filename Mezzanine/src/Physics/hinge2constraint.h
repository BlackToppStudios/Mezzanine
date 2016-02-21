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
#ifndef _physicshinge2constraint_h
#define _physicshinge2constraint_h

#include "Physics/generic6dofspringconstraint.h"

#ifndef SWIG
class btHinge2Constraint;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Hinge2Constraint
        /// @brief This is a convience class that will configure a 6DoF spring constraint with 2 angular and 1 linear (with a spring) axes of freedom such that it can behave like a wheel and axel.
        /// @details Simulating a Wheel and Axel is the primary use for this constraint.  The 2 angular axes that it gives you are the axis for rotating the child object around the parents axis
        /// (aka turning the wheel), and the child rotating around it's own local axis (aka spinning the wheel for motion).  The linear axis acts as suspension (which is where the spring comes in).
        /// @n @n
        /// In a normal vehicle use case, Axis1 would be the vertical axis of the main body.  Axis2 would be the X axis of the main body, assuming it is pointed along the Z axis.
        ///////////////////////////////////////
        class MEZZ_LIB Hinge2Constraint : public Generic6DofSpringConstraint
        {
        protected:
            /// @copydoc TypedConstraint::_GetConstraintBase() const
            virtual btHinge2Constraint* Hinge2() const;

            /// @copydoc DualTransformConstraint::CreateConstraint(RigidProxy*, RigidProxy*, const Transform&, const Transform&)
            virtual void CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB);
            /// @internal
            /// @brief Conveniece method used primarily for serialization to create an internal constraint.
            /// @param RigidA A pointer to the first proxy that will be constrained.
            /// @param RigidB A pointer to the second proxy that will be constrained.
            /// @param Anchor The point in world cocrdinates where the "axel" and "suspension" meet.
            /// @param Axis1 The axis on which the child object should rotate about the parent object(aka turning).  Must be orthogonal to Axis2.
            /// @param Axis2 The axis on which the child object will rotate on it's own(aka spinning).
            virtual void CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Vector3& Anchor, const Vector3& Axis1, const Vector3& Axis2);
        public:
            /// @brief Convenience constructor.
            /// @remarks All axes passed in should be in world coordinates.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA A pointer to the first proxy that will be constrained.
            /// @param ProxyB A pointer to the second proxy that will be constrained.
            /// @param Anchor The point in world cocrdinates where the "axel" and "suspension" meet.
            /// @param Axis1 The axis on which the child object should rotate about the parent object(aka turning).  Must be orthogonal to Axis2.
            /// @param Axis2 The axis on which the child object will rotate on it's own(aka spinning).
            /// @param Creator A pointer to the manager that created this constraint.
            Hinge2Constraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& Anchor, const Vector3& Axis1, const Vector3& Axis2, PhysicsManager* Creator);
            /// @brief Dual-Transform constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA A pointer to the first proxy that will be constrained.
            /// @param ProxyB A pointer to the second proxy that will be constrained.
            /// @param TransA The offset and rotation from ProxyAs center of gravity to get to match an offset from ProxyB.
            /// @param TransB The offset and rotation from ProxyBs center of gravity.
            /// @param Creator A pointer to the manager that created this constraint.
            Hinge2Constraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB, PhysicsManager* Creator);
            /// @brief XML constructor.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            /// @param Creator A pointer to the manager that created this constraint.
            Hinge2Constraint(const XML::Node& SelfRoot, PhysicsManager* Creator);
            /// @brief Class destructor.
            virtual ~Hinge2Constraint();

            ///////////////////////////////////////////////////////////////////////////////
            // Limits

            /// @brief Convenience method for setting the maximum allowed turning angle.
            /// @param AngelMax The maximum angle the child object is allowed to rotated around the parent (max turning angle).
            virtual void SetUpperLimit(const Real AngelMax);
            /// @brief Convenience method for setting the minimum allowed turning angle.
            /// @param AngelMin The minimum angle the child object is allowed to rotated around the parent (min turning angle).
            virtual void SetLowerLimit(const Real AngelMin);

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc Constraint::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @brief Get the name of the the XML tag the class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();
        };//Hinge2Constraint
    }//Physics
}//Mezzanine
#endif


#endif
