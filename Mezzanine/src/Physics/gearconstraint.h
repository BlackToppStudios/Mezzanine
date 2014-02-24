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
#ifndef _physicsgearconstraint_h
#define _physicsgearconstraint_h

#include "Physics/constraint.h"

class btGearConstraint;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class GearConstraint
        /// @headerfile gearconstraint.h
        /// @brief This is a constraint that duplicate the angular motion of one object to another, adjusted by the provided ratio.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB GearConstraint : public Constraint
        {
        protected:
            /// @brief Bullet constraint that this class encapsulates.
            btGearConstraint* Gear;
        public:
            /// @brief Double body constructor.  Binds the two bodies.
            /// @param ProxyA The first proxy to apply this constraint to.
            /// @param ProxyB The second proxy to apply this constraint to.
            /// @param PivotA The axis in ProxyA's local space to apply the constraint to.
            /// @param PivotB The axis in ProxyB's local space to apply the constraint to.
            /// @param Ratio The amount the rotation from ProxyA that shall be used to be applied to ProxyB.
            GearConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& AxisA, const Vector3& AxisB, const Real Ratio);
            /// @brief Class destructor.
            virtual ~GearConstraint();

            ///////////////////////////////////////////////////////////////////////////////
            // Axis configuration

            /// @brief Sets the axis in ActorA's local space which will translate to ActorB.
            /// @param Axis A vector3 expressing the axis on ActorA this constraint will be applied to.
            virtual void SetAxisA(const Vector3& Axis);
            /// @brief Sets the axis in ActorB's local space which will be manipulated.
            /// @param Axis A vector3 expressing the axis on ActorB this constraint will be applied to.
            virtual void SetAxisB(const Vector3& Axis);
            /// @brief Gets the axis in ActorA's local space which will translate to ActorB.
            /// @return Returns a vector3 expressing the axis on ActorA this constraint is being applied to.
            virtual Vector3 GetAxisA() const;
            /// @brief Gets the axis in ActorB's local space which will be manipulated.
            /// @return Returns a vector3 expressing the axis on ActorB this constraint is being applied to.
            virtual Vector3 GetAxisB() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Ratio Configuration

            /// @brief Sets the ratio at which ActorA's rotation will be applied to ActorB.
            /// @param Ratio The ratio at which rotations on ActorA's specified axis will be transfered to ActorB's specified axis.
            virtual void SetRotationRatio(const Real Ratio);
            /// @brief Gets the ratio at which ActorA's rotation will be applied to ActorB.
            /// @return Returns a Real representing the ratio at which rotations on ActorA's specified axis is being transfered to ActorB's specified axis.
            virtual Real GetRotationRatio() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Parameter Configuration

            /// @copydoc Constraint::ValidParamOnAxis(int) const
            virtual Constraint::ParamList ValidParamOnAxis(int Axis) const;
            /// @copydoc Constraint::ValidLinearAxis() const
            virtual Constraint::AxisList ValidLinearAxis() const;
            /// @copydoc Constraint::ValidAngularAxis() const
            virtual Constraint::AxisList ValidAngularAxis() const;
            /// @copydoc Constraint::ValidAngularAxis(ConstraintParam,int) const
            virtual Boole HasParamBeenSet(ConstraintParam Param, int Axis) const;

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

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @copydoc Constraint::GetConstraintBase() const
            virtual btTypedConstraint* GetConstraintBase() const;
        };//GearConstraint
    }//Physics
}//Mezzanine

#endif
