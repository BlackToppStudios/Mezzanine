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
#ifndef _physicspoint2pointconstraint_h
#define _physicspoint2pointconstraint_h

#include "Physics/constraint.h"

class btPoint2PointConstraint;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Point2PointConstraint
        /// @brief Tries to make a point relative to each of two actors match in 3d space, without regard to rotation.
        /// @details This will translate (push around) the actors to attempt to get the offsets to match up in 3d space.
        /// The actors can freely rotate about their relative offsets.
        ///////////////////////////////////////
        class MEZZ_LIB Point2PointConstraint : public Constraint
        {
        protected:
            /// @internal
            /// @brief The internal constraint that this class encapsulates.
            btPoint2PointConstraint* Point2Point;

            /// @internal
            /// @brief Creates the internal constraint.
            /// @remarks This methods exists primarily as a convenience for serialization, and shouldn't be called unless it is known to be safe.
            /// @param RigidA A pointer to the first Proxy to be constrained.
            /// @param RigidB A pointer to the second Proxy to be constrained.
            /// @param PivotA The location in ProxyA's local space to apply the constraint to.
            /// @param PivotB The location in ProxyB's local space to apply the constraint to.
            virtual void CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Vector3& PivotA, const Vector3& PivotB);
            /// @internal
            /// @brief Destroys the internal constraint.
            /// @remarks This methods exists primarily as a convenience for serialization, and shouldn't be called unless it is known to be safe.
            virtual void DestroyConstraint();
        public:
            /// @brief Double body constructor.  Binds the two bodies.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA The first proxy to apply this constraint to.
            /// @param ProxyB The second proxy to apply this constraint to.
            /// @param PivotA The location in ProxyA's local space to apply the constraint to.
            /// @param PivotB The location in ProxyB's local space to apply the constraint to.
            /// @param Creator A pointer to the manager that created this constraint.
            Point2PointConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& PivotA, const Vector3& PivotB, PhysicsManager* Creator);
            /// @brief Single body constructor.  Binds the body to world space.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA The proxy to apply this constraint to.
            /// @param PivotA The position relative to ProxyA's center of gravity to "Pin" to the world.
            /// @param Creator A pointer to the manager that created this constraint.
            Point2PointConstraint(const UInt32 ID, RigidProxy* ProxyA, const Vector3& PivotA, PhysicsManager* Creator);
            /// @brief XML constructor.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            /// @param Creator A pointer to the manager that created this constraint.
            Point2PointConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator);
            /// @brief Class destructor.
            virtual ~Point2PointConstraint();

            ////////////////////////////////////////////////////////////////////////////////
            // Position and Orientation

            /// @brief Set offset of the first proxy.
            /// @param PivotA The offset as a Vector3 relative to the center of mass of ProxyA.
            virtual void SetPivotA(const Vector3& PivotA);
            /// @brief Set offset of the second proxy.
            /// @param PivotB The offset as a Vector3 relative to the center of mass of ProxyB.
            virtual void SetPivotB(const Vector3& PivotB);
            /// @brief Get offset of the first proxy.
            /// @return The offset as a Vector3 relative to the center of mass of ProxyA.
            virtual Vector3 GetPivotA() const;
            /// @brief Get offset of the second proxy.
            /// @return The offset as a Vector3 relative to the center of mass of ProxyB.
            virtual Vector3 GetPivotB() const;

            ////////////////////////////////////////////////////////////////////////////////
            // Specific Physics Settings

            /// @brief Set the current impulse clamping on the constraint.
            /// @param Clamping This is a value that the constraint solver can use to adjust accumlated values when solving the constraint.
            virtual void SetImpulseClamping(const Real Clamping);
            /// @brief get the current impulse clamping value.
            /// @return A real with the Clamping.
            virtual Real GetImpulseClamping() const;
            /// @brief Set a resistive force against the constraint, not too dissimilar to from hinge friction or Air resistance.
            /// @param Damping A real with the desired values.
            virtual void SetDamping(const Real Damping);
            /// @brief Get the current Damping.
            /// @return A Real with the Damping value.
            virtual Real GetDamping() const;
            /// @brief This may be a scalar for how strongly Angular momentum affects linear momemtum.
            /// @todo Research this more carefully.
            /// @details This function is a tightly wrapped bullet 3d function. No real documentation for it exists, from its responsibility/location in Bullet3d and
            /// a basic understanding of torque ( see http://en.wikipedia.org/wiki/Torque ) It is highly likely that it is a value to adjust how torque affects momentum.
            virtual void SetTAU(const Real TAU);
            /// @brief Retrieve the Tau Setting.
            /// @return The Tau value as a Real.
            virtual Real GetTAU() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Parameter Configuration

            /// @copydoc Constraint::GetValidParamsOnAxis(int) const
            virtual Constraint::ParamList GetValidParamsOnAxis(int Axis) const;
            /// @copydoc Constraint::GetValidLinearAxes() const
            virtual Constraint::AxisList GetValidLinearAxes() const;
            /// @copydoc Constraint::GetValidAngularAxes() const
            virtual Constraint::AxisList GetValidAngularAxes() const;
            /// @copydoc Constraint::ValidAngularAxis(ConstraintParam,int) const
            virtual Boole HasParamBeenSet(ConstraintParam Param, int Axis) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc Constraint::ProtoSerializeInitData(XML::Node&) const
            virtual void ProtoSerializeInitData(XML::Node& SelfRoot) const;
            /// @copydoc Constraint::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;

            /// @copydoc Constraint::ProtoDeSerializeInitData(const XML::Node&)
            virtual void ProtoDeSerializeInitData(const XML::Node& SelfRoot);
            /// @copydoc Constraint::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc Constraint::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @brief Get the name of the the XML tag the class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal

            /// @copydoc Constraint::_GetConstraintBase() const
            virtual btTypedConstraint* _GetConstraintBase() const;
        };//Point2PointConstraint
    }//Physics
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

#ifndef SWIG
/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::Constraint& x)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::Point2PointConstraint& x);
/// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::Constraint& x)
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::Point2PointConstraint& x);
/// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Constraint& x)
void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Point2PointConstraint& x);
#endif

#endif
