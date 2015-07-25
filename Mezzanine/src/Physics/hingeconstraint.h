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
#ifndef _physicshingeconstraint_h
#define _physicshingeconstraint_h

#include "Physics/dualtransformconstraint.h"

class btHingeConstraint;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class HingeConstraint
        /// @brief This is a constraint to be used to restrict the movement between two objects to angular rotation on a single axis.
        /// @details As the name suggests, this constraint essentially works like a door Hinge.
        ///////////////////////////////////////
        class MEZZ_LIB HingeConstraint : public DualTransformConstraint
        {
        protected:
            /// @internal
            /// @brief Bullet constraint that this class encapsulates.
            btHingeConstraint* Hinge;

            /// @copydoc DualTransformConstraint::CreateConstraint(RigidProxy*, RigidProxy*, const Transform&, const Transform&)
            virtual void CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB);
            /// @internal
            /// @brief Creates the internal constraint.
            /// @param RigidA The first proxy to apply this constraint to.
            /// @param RigidB The second proxy to apply this constraint to.
            /// @param PivotA The location in ProxyA's local space to apply the constraint to.
            /// @param PivotB The location in ProxyB's local space to apply the constraint to.
            /// @param AxisInA The axis(for ProxyA) on which the hinge is to act.  For example, a door hinge would be (0.0,1.0,0.0), aka the positive Y axis.
            /// @param AxisInB The axis(for ProxyB) on which the hinge is to act.  For example, a door hinge would be (0.0,1.0,0.0), aka the positive Y axis.
            virtual void CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Vector3& PivotInA, const Vector3& PivotInB, const Vector3& AxisInA, const Vector3& AxisInB);
            /// @copydoc DualTransformConstraint::DestroyConstraint()
            virtual void DestroyConstraint();
        public:
            /// @brief Convenience axis constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA The first proxy to apply this constraint to.
            /// @param ProxyB The second proxy to apply this constraint to.
            /// @param PivotA The location in ProxyA's local space to apply the constraint to.
            /// @param PivotB The location in ProxyB's local space to apply the constraint to.
            /// @param AxisInA The axis(for ProxyA) on which the hinge is to act.  For example, a door hinge would be (0.0,1.0,0.0), aka the positive Y axis.
            /// @param AxisInB The axis(for ProxyB) on which the hinge is to act.  For example, a door hinge would be (0.0,1.0,0.0), aka the positive Y axis.
            /// @param Creator A pointer to the manager that created this constraint.
            HingeConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& PivotInA, const Vector3& PivotInB, const Vector3& AxisInA, const Vector3& AxisInB, PhysicsManager* Creator);
            /// @brief Transform constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA The first proxy to apply this constraint to.
            /// @param ProxyB The second proxy to apply this constraint to.
            /// @param TransA The offset and rotation from ProxyAs center of gravity to get to match an offset from ProxyB.
            /// @param TransB The offset and rotation from ProxyBs center of gravity.
            /// @param Creator A pointer to the manager that created this constraint.
            HingeConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB, PhysicsManager* Creator);
            /// @brief Single body axis constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA The proxy to apply this constraint to.
            /// @param PivotInA The point in the objects(ProxyA) local space where the constraint is to be attached to world space.
            /// @param AxisInA The axis(for ProxyA) on which the hinge is to act.  For example, a door hinge would be (0.0,1.0,0.0), aka the positive Y axis.
            /// @param Creator A pointer to the manager that created this constraint.
            HingeConstraint(const UInt32 ID, RigidProxy* ProxyA, const Vector3& PivotInA, const Vector3& AxisInA, PhysicsManager* Creator);
            /// @brief Single body transform constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA The first proxy to apply this constraint to.
            /// @param TransB The offset and rotation from ProxyAs center of gravity.
            /// @param Creator A pointer to the manager that created this constraint.
            HingeConstraint(const UInt32 ID, RigidProxy* ProxyA, const Transform& TransA, PhysicsManager* Creator);
            /// @brief XML constructor.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            /// @param Creator A pointer to the manager that created this constraint.
            HingeConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator);
            /// @brief Class destructor.
            virtual ~HingeConstraint();

            ////////////////////////////////////////////////////////////////////////////////
            // Position and Orientation

            /// @copydoc DualTransformConstraint::SetPivotTransforms(const Transform&, const Transform&)
            /// @remarks This implements a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
            virtual void SetPivotTransforms(const Transform& TransA, const Transform& TransB);
            /// @copydoc DualTransformConstraint::SetTransformA
            virtual void SetPivotATransform(const Transform& TransA);
            /// @copydoc DualTransformConstraint::SetTransformB
            virtual void SetPivotBTransform(const Transform& TransB);
            /// @copydoc DualTransformConstraint::GetTransformA
            virtual Transform GetPivotATransform() const;
            /// @copydoc DualTransformConstraint::GetTransformB
            virtual Transform GetPivotBTransform() const;

            /// @copydoc DualTransformConstraint::SetPivotALocation(const Vector3&)
            /// @remarks Ultimately this information winds up being stored in the TransformA. This implements
            /// a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
            virtual void SetPivotALocation(const Vector3& Location);
            /// @copydoc DualTransformConstraint::SetPivotBLocation(const Vector3&)
            /// @remarks Ultimately this information winds up being stored in the TransformB. This implements
            /// a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
            virtual void SetPivotBLocation(const Vector3& Location);
            /// @copydoc DualTransformConstraint::GetPivotALocation() const
            /// @remarks This implements a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
            virtual Vector3 GetPivotALocation() const;
            /// @copydoc DualTransformConstraint::GetPivotBLocation() const
            /// @remarks This implements a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
            virtual Vector3 GetPivotBLocation() const;

            /// @copydoc DualTransformConstraint::SetAPivotRotation(const Quaternion&)
            /// @remarks Ultimately this information winds up being stored in the TransformA. This implements
            /// a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
            virtual void SetAPivotRotation(const Quaternion& Rotation);
            /// @copydoc DualTransformConstraint::SetBPivotRotation(const Quaternion&)
            /// @remarks Ultimately this information winds up being stored in the TransformB. This implements
            /// a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
            virtual void SetBPivotRotation(const Quaternion& Rotation);
            /// @copydoc DualTransformConstraint::GetAPivotRotation() const
            /// @remarks This implements a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
            virtual Quaternion GetAPivotRotation() const;
            /// @copydoc DualTransformConstraint::GetBPivotRotation() const
            /// @remarks This implements a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
            virtual Quaternion GetBPivotRotation() const;

            ////////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Sets the axis on which this constraint acts.
            /// @param AxisInA A vector3 representing the axis to be used with this constraint.
            virtual void SetAxis(const Vector3& AxisInA);
            /// @brief Gets the current angle of the hinge.
            /// @return Gets the amount of rotation this hinge is off from it's origin in radians.
            virtual Real GetHingeAngle();

            /// @brief Sets whether or not an offset of the constraint frame should be used to calculate internal data.
            /// @param FrameOffset The full effect of this being true or false is uknown, but internal documentation suggests "true" provides more stable results.
            virtual void SetUseFrameOffset(const Boole FrameOffset);
            /// @brief Gets whether or not an offset of the constraint frame should be used to calculate internal data.
            /// @return Returns whether or not an offset in the constraint frame is being used by the internal constraint.
            virtual Boole GetUseFrameOffset() const;

            /// @brief Sets whether or not to perform linear math from ProxyA's perspective.
            /// @param UseRefFrameA True to perform math from ProxyA's perspective, false to perform math from ProxyB's perspective.  Initial Value: false.
            virtual void SetUseReferenceFrameA(const Boole UseRefFrameA);
            /// @brief Gets whether or not to perform linear math from ProxyA's perspective.
            /// @return Returns true if math is being done from ProxyA's perspective, false if math is being done from ProxyB's perspective.
            virtual Boole GetUseReferenceFrameA() const;

            ////////////////////////////////////////////////////////////////////////////////
            // Limits

            /// @brief Sets the angle limits of the constraint in radians.
            /// @param Low The minimum angle limit for the constraint in radians.
            /// @param High The maximum angle limit for the constraint in radians.
            /// @param Softness Not currently used internally.
            /// @param BiasFactor Multiplier for the constraint error, constraint appears more "soft" when closer to zero.
            /// @param RelaxationFactor The amount of bounce to apply when the constraint reaches it's limit.  Range: 0.0-1.0.
            virtual void SetLimits(const Real Low, const Real High, const Real Softness = 0.9, const Real BiasFactor = 0.3, const Real RelaxationFactor = 1.0);
            /// @brief Return the Lower Limit of the hinge
            /// @return A real containing the Lower Limit
            virtual Real GetLimitLow() const;
            /// @brief Return the Upper Limit of the hinge
            /// @return A real containing the Higher Limit
            virtual Real GetLimitHigh() const;
            /// @brief Return the Softness of the hinge
            /// @return A real containing the Softness
            virtual Real GetLimitSoftness() const;
            /// @brief Return the bias factor of the hinge (Not entirely certain hat this on is)
            /// @return A real containing the bias Factor
            virtual Real GetLimitBiasFactor() const;
            /// @brief Return the Relaxation Factor of the hinge
            /// @return A real containing the Relaxation Factor
            virtual Real GetLimitRelaxationFactor() const;

            ////////////////////////////////////////////////////////////////////////////////
            // Angular Motor

            /// @brief Enables(or Disables) the motor on the hinge and sets it's parameters.
            /// @param EnableMotor Sets whether or not the motor on this constraint is enabled.
            /// @param TargetVelocity The desired velocity of rotation the motor will have.  This may or may not be achieved based on obstructions in the simulation.
            /// @param MaxMotorImpulse The maximum amount of force the motor is to apply to try and reach it's target velocity.
            virtual void EnableMotor(const Boole EnableMotor, const Real TargetVelocity, const Real MaxMotorImpulse);
            /// @brief Enables(or Disables) the motor on the hinge.
            /// @warning Be sure to set values for the Motor max impulse and/or velocity before enabling the motor, or else you may get a crash.
            /// @param EnableMotor Sets whether or not the motor on this constraint is enabled.
            virtual void SetMotorEnabled(const Boole EnableMotor);
            /// @brief Is this motor on this hinge enabled?
            /// @return True if it is, false otherwise.
            virtual Boole GetMotorEnabled() const;

            /// @brief Sets the maximum amount of force the motor is to apply.
            /// @param MaxMotorImpulse The maximum amount of force the motor is to apply to try and reach it's target velocity.
            virtual void SetMaxMotorImpulse(const Real MaxMotorImpulse);
            /// @brief Retrieve the maximimum value that the acceleration of the motor can be increased.
            /// @return A real containing the maximum impulse.
            virtual Real GetMaxMotorImpulse() const;

            /// @brief Desired angular velocity of the motor
            /// @param TargetVelocity The Desired velocity
            /// @warning Causes segfaults in some tests.
            virtual void SetMotorTargetVelocity(const Real TargetVelocity);
            /// @brief Get the Target Velocity.
            /// @return the target valocity as a real.
            virtual Real GetMotorTargetVelocity() const;

            /// @brief Sets a Target Velocity, indirectly using the angle stored in a quaternion.
            /// @param QuatAInB The angle a quaternion relative to the two objects in the constraint.
            /// @param Delta The Desired Time steps that the target rotational velocity should be reached in.
            virtual void SetMotorTarget(const Quaternion& QuatAInB, const Real Delta);
            /// @brief Set the Rotational velocity in a more direct fashion
            /// @param TargetAngle The desired angle in radians.
            /// @param Delta The Desired Time steps that the target rotational velocity should be reached in.
            virtual void SetMotorTarget(const Real TargetAngle, const Real Delta);

            ////////////////////////////////////////////////////////////////////////////////
            // Parameter Configuration

            /// @copydoc Constraint::GetValidParamsOnAxis(int) const
            virtual Constraint::ParamList GetValidParamsOnAxis(int Axis) const;
            /// @copydoc Constraint::GetValidLinearAxes() const
            virtual Constraint::AxisList GetValidLinearAxes() const;
            /// @copydoc Constraint::GetValidAngularAxes() const
            virtual Constraint::AxisList GetValidAngularAxes() const;
            /// @copydoc Constraint::ValidAngularAxis(ConstraintParam,int) const
            virtual Boole HasParamBeenSet(ConstraintParam Param, int Axis) const;

            ////////////////////////////////////////////////////////////////////////////////
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

            ////////////////////////////////////////////////////////////////////////////////
            // Internal

            /// @copydoc Constraint::_GetConstraintBase() const
            virtual btTypedConstraint* _GetConstraintBase() const;
        };//HingeConstraint
    }//Physics
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

#ifndef SWIG
/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::Constraint& x)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::HingeConstraint& x);
/// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::Constraint& x)
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::HingeConstraint& x);
/// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Constraint& x)
void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::HingeConstraint& x);
#endif


#endif
