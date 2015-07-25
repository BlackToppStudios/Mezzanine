// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#ifndef _physicsconetwistconstraint_h
#define _physicsconetwistconstraint_h

#include "Physics/dualtransformconstraint.h"

class btConeTwistConstraint;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class ConeTwistConstraint
        /// @brief This is a constraint that will limit the movement allowed from one body to within a cone area around another object.
        /// @details This constraint is commonly used to simulate limbs for a ragdoll and moves similarly to a ball in a socket. @n @n
        /// In a kinematic or character chain of bones, the child limbs should be defined as Body/Proxy A, and the parents as B.
        ///////////////////////////////////////
        class MEZZ_LIB ConeTwistConstraint : public DualTransformConstraint
        {
        protected:
            /// @internal
            /// @brief The internal constraint that this class encapsulates.
            btConeTwistConstraint* ConeTwist;

            /// @copydoc DualTransformConstraint::CreateConstraint(RigidProxy*, RigidProxy*, const Transform&, const Transform&)
            virtual void CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB);
            /// @copydoc DualTransformConstraint::DestroyConstraint()
            virtual void DestroyConstraint();
        public:
            /// @brief Class constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA The first proxy to apply this constraint to.
            /// @param ProxyB The second proxy to apply this constraint to.
            /// @param TransA The transform in ProxyA's local space to attach this constraint to.
            /// @param TransB The transform in ProxyB's local space to attach this constraint to.
            /// @param Creator A pointer to the manager that created this constraint.
            ConeTwistConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB, PhysicsManager* Creator);
            /// @brief Single body constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA The first proxy to apply this constraint to.
            /// @param TransA The transform in ProxyA's local space to attach this constraint to.
            /// @param Creator A pointer to the manager that created this constraint.
            ConeTwistConstraint(const UInt32 ID, RigidProxy* ProxyA, const Transform& TransA, PhysicsManager* Creator);
            /// @brief XML constructor.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            /// @param Creator A pointer to the manager that created this constraint.
            ConeTwistConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator);
            /// @brief Class destructor.
            virtual ~ConeTwistConstraint();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods


            /// @brief Gets whether or not the child object is outside the set limits for the constraint.
            /// @return Returns true if the child body is out of bounds and needs force applied to correct it's transform.
            Boole IsPassedSwingLimit() const;
            /// @brief Gets the current angle of thw twist applied to the constraint.
            /// @return Returns a Real representing the amount of rotation currently applied on the twisting axis in radians.
            Real GetTwistAngle() const;

            /// @copydoc DualTransformConstraint::SetPivotATransform(const Transform&)
            virtual void SetPivotATransform(const Transform& TransA);
            /// @copydoc DualTransformConstraint::SetPivotBTransform(const Transform&)
            virtual void SetPivotBTransform(const Transform& TransB);
            /// @copydoc DualTransformConstraint::GetPivotATransform()
            virtual Transform GetPivotATransform() const;
            /// @copydoc DualTransformConstraint::GetPivotBTransform()
            virtual Transform GetPivotBTransform() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration Methods

            /// @brief Enables (or disables) the motor of this constraint.
            /// @param Enable If true, the constraint will push the child body towards the orientation set as the motor target.
            virtual void EnableMotor(const Boole Enable);
            /// @brief Gets whether or not the motor is enabled.
            /// @return Returns true if the motor is enabled, false otherwise.
            virtual Boole IsMotorEnabled() const;

            virtual void SetAngularOnly(const Boole AngularOnly);
            virtual Boole GetAngularOnly() const;

            /// @brief Sets a constraint limit based on Index.
            /// @remarks The valid indexes that can be specified are 3, 4, and 5.  Any other index being specified will perform no action. @n
            /// The indexes are mapped thusly: @n
            /// Index 3 is the TwistSpan. @n
            /// Index 4 is SwingSpan 2. @n
            /// Index 5 is SwingSpan 1.
            /// @param LimitIndex The index of the constraint limit to alter.
            /// @param LimitValue The value to set for the specified limit.
            virtual void SetLimit(const Integer LimitIndex, const Real LimitValue);
            /// @brief Sets all constraint limits.
            /// @param SwingSpan1 The first parameter that determines the size/shape of the cone in which the body will be able to move (in radians).  If different from SwingSpan2 this will create a non-circular area of movement.
            /// @param SwingSpan2 The second parameter that determines the size/shape of the cone in which the body will be able to move (in radians).  If different from SwingSpan1 this will create a non-circular area of movement.
            /// @param TwistSpan The amount of rotation that is allowed on the axis the constraint is connected, aka how much the body is allowed to twist in radians.
            /// @param Softness A scalar value representing the % of the limits which are free to move.  Above this value limits will start to be enforced with a force related to how
            /// close to the limit the constraint is.  Lower values mean smoother forces being applied to keep an object within the swing limits.  A value of 1.0 means the object is
            /// free to swing however within the strictly defined limits. Generally this shouldn't be below 0.8. @n Range: 0.0-1.0. @n Initial value: 1.0.
            /// @param BiasFactor A scalar value representing how strong the the forces used to enforce the "twist" limit set on this constraint.  In most cases this should remain in the range of 0.0-0.6.  @n
            /// Allowed range: 0.0-1.0.
            /// @param RelaxationFactor A scalar value representing how strong the forces used to enforce the "swing" limits set on this constraint.  In most cases this should remain near
            /// 1.0. @n Range: 0.0-1.0.
            virtual void SetLimits(const Real SwingSpan1, const Real SwingSpan2, const Real TwistSpan, const Real Softness = 1.0, const Real BiasFactor = 0.3, const Real RelaxationFactor = 1.0);
            /// @brief Gets a constraint limit based on Index.
            /// @remarks The valid indexes to that be retrieved by this method are 3, 4, and 5.  Any other index being specified will assert. @n
            /// The indexes are mapped thusly: @n
            /// Index 3 is the TwistSpan. @n
            /// Index 4 is SwingSpan 2. @n
            /// Index 5 is SwingSpan 1.
            /// @param LimitIndex The index of the constraint limit to retrieve.
            virtual Real GetLimit(const Integer LimitIndex) const;
            /// @brief Gets the currently set value for the constraint Swing span on the Y axis.
            /// @return Returns a Real representing the allowed range of "swing" on the Y axis in radians.
            virtual Real GetSwingSpan1() const;
            /// @brief Gets the currently set value for the constraint Swing span on the Z axis.
            /// @return Returns a Real representing the allowed range of "swing" on the Z axis in radians.
            virtual Real GetSwingSpan2() const;
            /// @brief Gets the currently set value for the constraint Twist span on the X axis.
            /// @return Returns a Real representing the allowed range of "twist" on the X axis in radians.
            virtual Real GetTwistSpan() const;
            /// @brief Gets the % portion of the swing limit in which to start enforcing the limit.
            /// @return Returns a Real representing how close to the limit to start enforcing the swing limits.
            virtual Real GetLimitSoftness() const;
            /// @brief Gets the strength of the force enforcing the twist limits.
            /// @return Returns a Real representing the strength of the force to be used when enforcing the twist limits.
            virtual Real GetBiasFactor() const;
            /// @brief Gets the strength of the force enforcing the swing limits.
            /// @return Returns a Real representing the strength of the force to be used when enforcing the swing limits.
            virtual Real GetRelaxationFactor() const;

            virtual void SetDamping(const Real Damping);
            virtual Real GetDamping() const;
            virtual void SetMaxMotorImpulse(const Real MaxMotorImpulse);
            virtual void SetMaxMotorImpulseNormalized(const Real MaxMotorImpulse);
            virtual Boole IsMaxMotorImpulseNormalized() const;
            virtual Real GetMaxMotorImpulse() const;
            virtual void SetFixThresh(const Real FixThresh);
            virtual Real GetFixThresh() const;
            virtual void SetMotorTarget(const Quaternion& Quat);
            virtual void SetMotorTargetInConstraintSpace(const Quaternion& Quat);
            /// @note The returned target is always in constraint space and clamped to be within the allowed limits for the constraint.
            virtual Quaternion GetMotorTarget() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Constraint Parameters

            /// @copydoc Constraint::GetValidParamsOnAxis(int) const
            virtual ParamList GetValidParamsOnAxis(int Axis) const;
            /// @copydoc Constraint::GetValidLinearAxes() const
            virtual AxisList GetValidLinearAxes() const;
            /// @copydoc Constraint::GetValidAngularAxes() const
            virtual AxisList GetValidAngularAxes() const;
            /// @copydoc Constraint::HasParamBeenSet(ConstraintParam,int) const
            virtual Boole HasParamBeenSet(ConstraintParam Param, int Axis) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc Constraint::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
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
        };//ConeTwistConstraint
    }//Physics
}//Mezzanine

#endif
