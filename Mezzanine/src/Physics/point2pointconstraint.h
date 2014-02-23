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
            /// @brief Bullet constraint that this class encapsulates.
            btPoint2PointConstraint* Point2Point;
        public:
            ////////////////////////////////////////////////////////////////////////////////
            // Point2PointConstraint Construction and Destruction

            /// @brief Double body constructor.  Binds the two bodies.
            /// @param ProxyA The first proxy to apply this constraint to.
            /// @param ProxyB The second proxy to apply this constraint to.
            /// @param PivotA The location in ProxyA's local space to apply the constraint to.
            /// @param PivotB The location in ProxyB's local space to apply the constraint to.
            Point2PointConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& PivotA, const Vector3& PivotB);
            /// @brief Single body constructor.  Binds the body to world space.
            /// @param ProxyA The proxy to apply this constraint to.
            /// @param PivotA The position relative to ProxyA's center of gravity to "Pin" to the world.
            Point2PointConstraint(RigidProxy* ProxyA, const Vector3& PivotA);
            /// @brief Class destructor.
            virtual ~Point2PointConstraint();

            ////////////////////////////////////////////////////////////////////////////////
            // Point2PointConstraint Position and Orientation

            /// @brief Set offset of the first proxy.
            /// @param PivotA The offset as a Vector3 relative to the center of mass of ProxyA.
            virtual void SetPivotALocation(const Vector3& PivotA);
            /// @brief Set offset of the second proxy.
            /// @param PivotB The offset as a Vector3 relative to the center of mass of ProxyB.
            virtual void SetPivotBLocation(const Vector3& PivotB);
            /// @brief Get offset of the first proxy.
            /// @return The offset as a Vector3 relative to the center of mass of ProxyA.
            virtual Vector3 GetPivotALocation() const;
            /// @brief Get offset of the second proxy.
            /// @return The offset as a Vector3 relative to the center of mass of ProxyB.
            virtual Vector3 GetPivotBLocation() const;

            ////////////////////////////////////////////////////////////////////////////////
            // Point2PointConstraint Specific Physics Settings

            /// @brief Set the current impulse clamping on the constraint
            /// @param Clamping This is a value that the constraint solver can use to adjust accumlated values when solving the constraint.
            virtual void SetImpulseClamping(Real Clamping);
            /// @brief get the current impulse clamping value
            /// @return A real with the Clamping
            virtual Real GetImpulseClamping() const;
            /// @brief Set a resistive force against the constraint, not too dissimilar to from hinge friction or Air resistance
            /// @param Damping A real with the desired values
            virtual void SetDamping(Real Damping);
            /// @brief Get the current Damping
            /// @return A Real with the Damping value.
            virtual Real GetDamping() const;
            /// @brief This may be a scalar for how strongly Angular momentum affects linear momemtum
            /// @todo Research this more carefully
            /// @details This function is a tightly wrapped bullet 3d function. No real documentation for it exists, from its responsibility/location in Bullet3d and
            /// a basic understanding of torque ( see http://en.wikipedia.org/wiki/Torque ) It is highly likely that it is a value to adjust how torque affects momentum.
            virtual void SetTAU(Real TAU);
            /// @brief Retrieve the Tau Setting
            /// @return The Tau value as a Real
            virtual Real GetTAU() const;

            /// @copydoc Constraint::ValidParamOnAxis(int) const
            virtual Constraint::ParamList ValidParamOnAxis(int Axis) const;
            /// @copydoc Constraint::ValidLinearAxis() const
            virtual Constraint::AxisList ValidLinearAxis() const;
            /// @copydoc Constraint::ValidAngularAxis() const
            virtual Constraint::AxisList ValidAngularAxis() const;
            /// @copydoc Constraint::ValidAngularAxis(ConstraintParam,int) const
            virtual Boolean HasParamBeenSet(ConstraintParam Param, int Axis) const;

            /// @copydoc Constraint::GetConstraintBase() const
            virtual btTypedConstraint* GetConstraintBase() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this vectorw should be appended to.
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and XML::Node containing the data.
            /// @warning A precondition of using this is that all of the actors intended for use must already be Deserialized.
            virtual void ProtoDeSerialize(const XML::Node& OneNode);
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "Point2PointConstraint"
            static String SerializableName();
        };//Point2PointConstraint
    }//Physics
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::Constraint& x)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::Point2PointConstraint& x);
/// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::Constraint& x)
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::Point2PointConstraint& x);
/// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Constraint& x)
void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Point2PointConstraint& x);

#endif
