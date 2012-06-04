//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _hingeconstraint_h
#define _hingeconstraint_h

#include "Physics/dualtransformconstraint.h"

class btHingeConstraint;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class HingeConstraint
        /// @headerfile constraint.h
        /// @brief This is a constraint to be used to restrict the movement between two objects to angular rotation on a single axis.
        /// @details As the name suggests, this constraint essentially works like a door Hinge.
        ///////////////////////////////////////
        class MEZZ_LIB HingeConstraint : public DualTransformConstraint
        {
            protected:
                /// @brief Bullet constraint that this class encapsulates.
                btHingeConstraint* Hinge;
            public:
                ////////////////////////////////////////////////////////////////////////////////
                // HingeConstraint Construction and Destruction
                /// @brief Creates a Hinge constraint that will connect two actors together by their offsets.
                /// @param ActorA The first actor to apply this constraint to.
                /// @param ActorB The second actor to apply this constraint to.
                /// @param PivotA The location in ActorA's local space to apply the constraint to.
                /// @param PivotB The location in ActorB's local space to apply the constraint to.
                /// @param AxisInA The axis(for ActorA) on which the hinge is to act.  For example, a door hinge would be (0.0,1.0,0.0), aka the positive Y axis.
                /// @param AxisInB The axis(for ActorB) on which the hinge is to act.  For example, a door hinge would be (0.0,1.0,0.0), aka the positive Y axis.
                /// @param UseReferenceFrameA By default, this constraint uses ActorB's local space as the reference for certain values, such as the rotational limits. This simply controls whether or not it should use ActorA's local space instead.
                HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& PivotInA, const Vector3& PivotInB, const Vector3& AxisInA, const Vector3& AxisInB, bool UseReferenceFrameA=false);
                /// @brief Creates a Hinge constraint that will attach an actor to a point in world space.
                /// @param ActorA The actor to apply this constraint to.
                /// @param PivotInA The point in the object's(ActorA) local space where the constraint is to be attached to world space.
                /// @param AxisInA The axis(for ActorA) on which the hinge is to act.  For example, a door hinge would be (0.0,1.0,0.0), aka the positive Y axis.
                /// @param UseReferenceFrameA By default, this constraint uses ActorB's local space as the reference for certain values, such as the rotational limits. This simply controls whether or not it should use ActorA's local space instead.
                HingeConstraint(ActorRigid* ActorA, const Vector3& PivotInA, const Vector3& AxisInA, bool UseReferenceFrameA=false);
                /// @brief Create a Hinge with components of a tranform
                /// @param ActorA The first actor to apply this constraint to.
                /// @param ActorB The second actor to apply this constraint to.
                /// @param VectorA The location component of Transform A
                /// @param VectorB The location component of Transform B
                /// @param QuaternionA The rotation component of Transform A
                /// @param QuaternionB The rotation component of Transform B
                /// @param UseReferenceFrameA By default, this constraint uses ActorB's local space as the reference for certain values, such as the rotational limits. This simply controls whether or not it should use ActorA's local space instead.
                HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA, const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseReferenceFrameA=false);
                /// @brief Create a Hinge with components of a tranform
                /// @param ActorA The first actor to apply this constraint to.
                /// @param ActorB The second actor to apply this constraint to.
                /// @param TransformA The location component of Transform A
                /// @param TransformB The location component of Transform B
                /// @param UseReferenceFrameA By default, this constraint uses ActorB's local space as the reference for certain values, such as the rotational limits. This simply controls whether or not it should use ActorA's local space instead.
                HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Transform& TransformA, const Transform& TransformB, bool UseReferenceFrameA=false);
                /// @brief Class destructor.
                /// @details The class destructor.
                virtual ~HingeConstraint();

                ////////////////////////////////////////////////////////////////////////////////
                // HingeConstraint Position and Orientation
                /// @brief Set The relative location of the hinge pivot from ActorA's Center of gravity.
                /// @param Location The New value for PivotA
                /// @details Ultimately this information winds up being stored in the TransformA. This implements
                /// a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
                virtual void SetPivotALocation(const Vector3& Location);
                /// @brief Set The relative location of the hinge pivot from ActorB's Center of gravity.
                /// @param Location The New value for PivotB
                /// @details Ultimately this information winds up being stored in the TransformB. This implements
                /// a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
                virtual void SetPivotBLocation(const Vector3& Location);
                /// @brief Get the location of the hinge pivot relative to ActorA's Center of gravity
                /// @return A Vector3 with the pivot location.
                /// @details This implements a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
                virtual Vector3 GetPivotALocation() const;
                /// @brief Get the location of the hinge pivot relative to ActorB's Center of gravity
                /// @return A Vector3 with the pivot location.
                /// @details This implements a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
                virtual Vector3 GetPivotBLocation() const;

                /// @brief Set The relative rotation of ActorA
                /// @param Rotation The new rotation amount for A
                /// @details Ultimately this information winds up being stored in the TransformA. This implements
                /// a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
                virtual void SetAPivotRotation(const Quaternion& Rotation);
                /// @brief Set The relative rotation of ActorB
                /// @param otation The new rotation amount for B
                /// @details Ultimately this information winds up being stored in the TransformB. This implements
                /// a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
                virtual void SetBPivotRotation(const Quaternion& Rotation);
                /// @brief Get the relative rotation for ActorA
                /// @return A Quaternion that has the rotation
                /// @details This implements a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
                virtual Quaternion GetAPivotRotation() const;
                /// @brief Get the relative rotation for ActorB
                /// @return A Quaternion that has the rotation
                /// @details This implements a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
                virtual Quaternion GetBPivotRotation() const;

                /// @copydoc DualTransformConstraint::SetTransformA
                virtual void SetPivotATransform(const Transform& TranA);
                /// @copydoc DualTransformConstraint::SetTransformB
                virtual void SetPivotBTransform(const Transform& TranB);
                /// @copydoc DualTransformConstraint::GetTransformA
                virtual Transform GetPivotATransform() const;
                /// @copydoc DualTransformConstraint::GetTransformB
                virtual Transform GetPivotBTransform() const;

                ////////////////////////////////////////////////////////////////////////////////
                // HingeConstraint Angular Motor
                /// @brief Enables(or Disables) the motor on the hinge and sets it's parameters.
                /// @param EnableMotor Sets whether or not the motor on this constraint is enabled.
                /// @param TargetVelocity The desired velocity of rotation the motor will have.  This may or may not be achieved based on obstructions in the simulation.
                /// @param MaxMotorImpulse The maximum amount of force the motor is to apply to try and reach it's target velocity.
                virtual void EnableMotor(bool EnableMotor, Real TargetVelocity, Real MaxMotorImpulse);
                /// @brief Enables(or Disables) the motor on the hinge.
                /// @warning Be sure to set values for the Motor max impulse and/or velocity before enabling the motor, or else you may get a crash.
                /// @param EnableMotor Sets whether or not the motor on this constraint is enabled.
                virtual void EnableMotor(bool EnableMotor);
                /// @brief Is this motor on this hinge enabled?
                /// @return True if it is, false otherwise.
                virtual bool GetMotorEnabled() const;
                /// @brief Sets the maximum amount of force the motor is to apply.
                /// @param MaxMotorImpulse The maximum amount of force the motor is to apply to try and reach it's target velocity.
                virtual void SetMaxMotorImpulse(Real MaxMotorImpulse);
                /// @brief Retrieve the maximimum value that the acceleration of the motor can be increased.
                /// @return A real containing the maximum impulse.
                virtual Real GetMaxMotorImpulse() const;
                /// @brief Sets a Target Velocity, indirectly using the angle stored in a quaternion.
                /// @details Is implemented in terms of SetMotorTarget(Real, Real);
                /// @param QuatAInB The angle a quaternion relative to the two objects in the constraint.
                /// @param Dt The Desired Time steps that the target rotational velocity should be reached in.
                virtual void SetMotorTarget(const Quaternion& QuatAInB, Real Dt);
                /// @brief Set the Rotational velocity in a more direct fashion
                /// @param TargetAngle The desired angle in radians.
                /// @param Dt The Desired Time steps that the target rotational velocity should be reached in.
                virtual void SetMotorTarget(Real TargetAngle, Real Dt);
                /// @brief Desired angular velocity of the motor
                /// @param TargetVelocity The Desired velocity
                /// @warning Causes segfaults in some tests.
                virtual void SetMotorTargetVelocity(Real TargetVelocity);
                /// @brief Get the Target Velocity.
                /// @return the target valocity as a real.
                virtual Real GetMotorTargetVelocity() const;

                ////////////////////////////////////////////////////////////////////////////////
                // HingeConstraint Limits
                /// @brief Sets the angle limits of the constraint in radians.
                /// @param Low The minimum angle limit for the constraint in radians.
                /// @param High The maximum angle limit for the constraint in radians.
                /// @param Softness Not currently used internally.
                /// @param BiasFactor Multiplier for the constraint error, constraint appears more "soft" when closer to zero.
                /// @param RelaxationFactor The amount of bounce to apply when the constraint reaches it's limit.  Range: 0.0-1.0.
                virtual void SetLimit(Real Low, Real High, Real Softness=0.9, Real BiasFactor=0.3, Real RelaxationFactor=1.0);
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
                // HingeConstraint Details
                /// @brief Sets the axis on which this constraint acts.
                /// @param AxisInA A vector3 representing the axis to be used with this constraint.
                virtual void SetAxis(const Vector3& AxisInA);
                /// @brief Gets the current angle of the hinge.
                /// @return Gets the amount of rotation this hinge is off from it's origin in radians.
                virtual Real GetHingeAngle();
                /// @copydoc Constraint::ValidParamOnAxis(int) const
                virtual Constraint::ParamList ValidParamOnAxis(int Axis) const;
                /// @copydoc Constraint::ValidLinearAxis() const
                virtual Constraint::AxisList ValidLinearAxis() const;
                /// @copydoc Constraint::ValidAngularAxis() const
                virtual Constraint::AxisList ValidAngularAxis() const;
                /// @copydoc Constraint::ValidAngularAxis(ConstraintParam,int) const
                virtual bool HasParamBeenSet(ConstraintParam Param, int Axis) const;
                /// @brief Retrieve the stored value from the physics subsystem(bullet)
                /// @return a True or false.
                virtual bool GetUseFrameOffset() const;
                /// @brief Set the stored value for UseFrameOffset on this hinge in the physics subsystem(bullet)
                /// @param FrameOffset The new desired value.
                virtual void SetUseFrameOffset(bool FrameOffset);
                /// @brief Is this Using Reference Frame A
                /// @return A the value UseReferenceFrameA is set to internally.
                virtual bool GetUseReferenceFrameA() const;
                /// @brief Change whether this is Using Reference Frame A or not
                /// @param UseReferenceFrameA Whether certain math be performed from the perspective of Actor A or Actor B (we think this is the case, but we have not test thoroughly)
                virtual void SetUseReferenceFrameA(bool UseReferenceFrameA=false);

                /// @copydoc Constraint::GetConstraintBase() const
                virtual btTypedConstraint* GetConstraintBase() const;

#ifdef MEZZXML
                ////////////////////////////////////////////////////////////////////////////////
                // HingeConstraint Serialization
                // Serializable
                /// @brief Convert this class to an XML::Node ready for serialization
                /// @param CurrentRoot The point in the XML hierarchy that all this vectorw should be appended to.
                virtual void ProtoSerialize(XML::Node& CurrentRoot) const;
                // DeSerializable
                /// @brief Take the data stored in an XML and overwrite this instance of this object with it
                /// @param OneNode and XML::Node containing the data.
                /// @warning A precondition of using this is that all of the actors intended for use must already be Deserialized.
                virtual void ProtoDeSerialize(const XML::Node& OneNode);
                /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
                /// @return A string containing "HingeConstraint"
                static String SerializableName();
#endif // /MEZZXML
        };//HingeConstraint
    }//Physics
}//Mezzanine

#ifdef MEZZXML
    ///////////////////////////////////////////////////////////////////////////////
    // Class External << Operators for streaming or assignment

    /// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::Constraint& x)
    std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::HingeConstraint& x);
    /// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::Constraint& x)
    std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::HingeConstraint& x);
    /// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Constraint& x)
    void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::HingeConstraint& x);
#endif // \MEZZXML

#endif
