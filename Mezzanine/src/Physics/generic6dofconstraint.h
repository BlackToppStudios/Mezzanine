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
#ifndef _physicsgeneric6dofconstraint_h
#define _physicsgeneric6dofconstraint_h

#include "Physics/dualtransformconstraint.h"

class btGeneric6DofConstraint;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief Create simple but specific limits on any axis of movement or rotation
        /// @details Provides optional limits on the x, y, and z for rotation and
        /// translation. You can get the ID of the axis with GetValidLinearAxes() or
        /// GetValidAngularAxes()  for this particular constraint at the time of this
        /// writing the constraint axis looked liked this:
        ///     - 0: Translation X
        ///     - 1: Translation Y
        ///     - 2: Translation Z
        ///     - 3: Rotation X
        ///     - 4: Rotation Y
        ///     - 5: Rotation Z
        ///
        /// This has to perform a larger amount of calculations and should be used only
        /// when other simpler constraints have been ruled out.
        /// @n @n
        /// As with all limits when the upper and lower limits match the two actors will
        /// be locked on that axis and relative translation or rotation will not be
        /// permitted (within the bounds or error correction). If the Lower limit is
        /// lower than the Upper limit a range of translation or rotation will be
        /// allowed. If the Lower limit is higher than the Upper limit this will cause
        /// the system to realized that no position can satisfy the constraint and
        /// no restriction will be enforced.
        /// @n @n
        /// Each of the Axis also has a motor that can be enabled to cause a specified
        /// amount of translation or rotation. To aid in selection of specific Axis,
        /// you should use the UsableAxis enum
        ///////////////////////////////////////
        class MEZZ_LIB Generic6DofConstraint : public DualTransformConstraint
        {
        public:
            /// @brief Identify the Axis a bit easier when iterating over them is less convienent than typing an Identifier.
            enum UsableAxis
            {
                LinearStart     = 0,    ///< Convenience type for loops.
                LinearX         = 0,    ///< Translation on the X axis.
                LinearY         = 1,    ///< Translation on the Y axis.
                LinearZ         = 2,    ///< Translation on the Z axis.
                LinearEnd       = 3,    ///< Convenience type for loops.
                AngularStart    = 3,    ///< Convenience type for loops.
                AngularX        = 3,    ///< Rotation on the X axis.
                AngularY        = 4,    ///< Rotation on the Y axis.
                AngularZ        = 5,    ///< Rotation on the Z axis.
                AngularEnd      = 6,    ///< Convenience type for loops.

                AngularXAsRotationAxis        = 0,    ///< Rotation on the X axis, when working with only rotational Axis.
                AngularYAsRotationAxis        = 1,    ///< Rotation on the Y axis, when working with only rotational Axis.
                AngularZAsRotationAxis        = 2     ///< Rotation on the Z axis, when working with only rotational Axis.
            };
        protected:
            /// @internal
            /// @brief The internal constraint that this class encapsulates.
            btGeneric6DofConstraint* Generic6dof;

            /// @copydoc DualTransformConstraint::CreateConstraint(RigidProxy*, RigidProxy*, const Transform&, const Transform&)
            virtual void CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB);
            /// @copydoc DualTransformConstraint::DestroyConstraint()
            virtual void DestroyConstraint();

            /// @internal
            /// @brief Single body inheritance constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param Prox1 A pointer to the first/only proxy that will be constrained.
            /// @param Creator A pointer to the manager that created this constraint.
            Generic6DofConstraint(const UInt32 ID, RigidProxy* Prox1, PhysicsManager* Creator);
            /// @internal
            /// @brief Dual body inheritance constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param Prox1 A pointer to the first proxy that will be constrained.
            /// @param Prox2 A pointer to the second proxy that will be constrained.
            /// @param Creator A pointer to the manager that created this constraint.
            Generic6DofConstraint(const UInt32 ID, RigidProxy* Prox1, RigidProxy* Prox2, PhysicsManager* Creator);
        public:
            /// @brief Dual body constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA The First proxy to be bound.
            /// @param ProxyB  The Second proxy to be bound.
            /// @param TransA The offset and rotation from ProxyAs center of gravity to get to match an offset from ProxyB.
            /// @param TransB The offset and rotation from ProxyBs center of gravity.
            /// @param Creator A pointer to the manager that created this constraint.
            Generic6DofConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB, PhysicsManager* Creator);
            /// @brief Single body constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyB The proxy to be bound to the world.
            /// @param TransB The offset and rotation for the ProxyB pivot/hinge/joint.
            /// @param Creator A pointer to the manager that created this constraint.
            Generic6DofConstraint(const UInt32 ID, RigidProxy* ProxyB, const Transform& TransB, PhysicsManager* Creator);
            /// @brief XML constructor.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @param Creator A pointer to the manager that created this constraint.
            Generic6DofConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator);
            /// @brief Class destructor.
            virtual ~Generic6DofConstraint();

            ////////////////////////////////////////////////////////////////////////////////
            // Location and Rotation

            /// @copydoc DualTransformConstraint::SetPivotTransforms(const Transform&, const Transform&)
            virtual void SetPivotTransforms(const Transform& TransA, const Transform& TransB);
            /// @copydoc DualTransformConstraint::SetPivotATransform(const Transform&)
            virtual void SetPivotATransform(const Transform& TransA);
            /// @copydoc DualTransformConstraint::SetPivotBTransform(const Transform&)
            virtual void SetPivotBTransform(const Transform& TransB);
            /// @copydoc DualTransformConstraint::GetPivotATransform()
            virtual Transform GetPivotATransform() const;
            /// @copydoc DualTransformConstraint::GetPivotBTransform()
            virtual Transform GetPivotBTransform() const;

            /// @copydoc DualTransformConstraint::SetPivotALocation(const Vector3&)
            virtual void SetPivotALocation(const Vector3& Location);
            /// @copydoc DualTransformConstraint::SetPivotBLocation(const Vector3&)
            virtual void SetPivotBLocation(const Vector3& Location);
            /// @copydoc DualTransformConstraint::GetPivotALocation()
            virtual Vector3 GetPivotALocation() const;
            /// @copydoc DualTransformConstraint::GetPivotBLocation()
            virtual Vector3 GetPivotBLocation() const;

            /// @copydoc DualTransformConstraint::SetPivotARotation(const Quaternion&)
            virtual void SetPivotARotation(const Quaternion& Rotation);
            /// @copydoc DualTransformConstraint::SetPivotBRotation(const Quaternion&)
            virtual void SetPivotBRotation(const Quaternion& Rotation);
            /// @copydoc DualTransformConstraint::GetPivotARotation()
            virtual Quaternion GetPivotARotation() const;
            /// @copydoc DualTransformConstraint::GetPivotBRotation()
            virtual Quaternion GetPivotBRotation() const;

            ////////////////////////////////////////////////////////////////////////////////
            // Query

            /// @brief Gets the linear distance between both bodies of this constraint.
            /// @remarks If this is anchored to the world it should return 0.
            /// @return Returns the distance between the two bodies in this constraint.
            virtual Real GetLinearDistance() const;

            ////////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Sets whether or not an offset of the constraint frame should be used to calculate internal data.
            /// @param FrameOffset The full effect of this being true or false is unknown, but internal documentation suggests "true" provides more stable results.
            virtual void SetUseFrameOffset(const Boole FrameOffset);
            /// @brief Gets whether or not an offset of the constraint frame should be used to calculate internal data.
            /// @return Returns whether or not an offset in the constraint frame is being used by the internal constraint.
            virtual Boole GetUseFrameOffset() const;
            /// @brief Sets whether or not to perform linear math from ProxyA's perspective.
            /// @param UseRefFrameA True to perform math from ProxyA's perspective, false to perform math from ProxyB's perspective.  Initial Value: false.
            void SetUseLinearReferenceFrameA(const Boole UseRefFrameA);
            /// @brief Gets whether or not to perform linear math from ProxyA's perspective.
            /// @return Returns true if math is being done from ProxyA's perspective, false if math is being done from ProxyB's perspective.
            Boole GetUseLinearReferenceFrameA() const;

            /// @brief Convert an Axis from a full 6 digit range to a 3 digit range.
            /// @remarks Due to the way this is stored internally, All the axis are listed from from
            /// 0 to 2. So rather than throw an exception(or have undefined behavior) if the Axis
            /// selected one of those, this select the axis like this:
            ///     - 0, 3: Rotation X
            ///     - 1, 4: Rotation Y
            ///     - 2, 5: Rotation Z
            /// @param Axis the Axis ID to be converted.
            /// @return A number in the range of 0-2.  Useful for error checking or converting an Axis value from all axes to Linear or Angular only axis range.
            virtual Whole ConvertFrom6AxisTo3Axis(const Whole Axis) const;
            /// @brief Convert an angular Axis from a 3 digit range to a full 6 digit range.
            /// @remarks This method assumes the Axis being passed in represents an Angular axis, and if it's value is less than three, three will be
            /// added to it.  If this is used with a linear Axis, you'll likely have a bad time.
            /// @param Axis the Axis ID to be converted.
            /// @return Returns a number in the range of 0-5.
            virtual Whole ConvertAngularTo6Axis(const Whole Axis) const;

            ////////////////////////////////////////////////////////////////////////////////
            // Basic Limit Accessors

            /// @brief Change the upper and lower limit for one axis of translation or rotation limit
            /// @param Axis The axis to change
            /// @param Lower The new lower limit
            /// @param UpperThe new Higher limit
            virtual void SetLimit(Whole Axis, Real Lower, Real Upper);

            /// @brief Set the Upper limits on translation
            /// @return A Vector3 that stores the upper limit x, y and z that cannot be exceeded
            virtual void SetLinearLimitLower(const Vector3& Limit);
            /// @brief Get the Upper limits on translation
            /// @return A Vector3 that stores the upper limit x, y and z that cannot be exceeded
            virtual Vector3 GetLinearLimitLower() const;
            /// @brief Set the lower limits on translation
            /// @param Limit A Vector3 that stores the lower limit x, y and z that cannot be exceeded
            virtual void SetLinearLimitUpper(const Vector3& Limit);
            /// @brief Get the lower limits on translation
            /// @param Limit A Vector3 that stores the lower limit x, y and z that cannot be exceeded
            virtual Vector3 GetLinearLimitUpper() const;

            /// @brief Sets the lower linear limit on a specific axis of the constraint.
            /// @param Limit The limit to apply to the specified axis.
            /// @param TranslationAxis The Axis to work with.
            void SetLinearLimitLowerOnAxis(const Real Limit, Whole TranslationAxis);
            /// @brief Get a specific lower translation limit.
            /// @param TranslationAxis The Axis to work with.
            /// @return A real containing the specified lower limit.
            Real GetLinearLimitLowerOnAxis(Whole TranslationAxis) const;
            /// @brief Sets the upper linear limit on a specific axis of the constraint.
            /// @param Limit The limit to apply to the specified axis.
            /// @param TranslationAxis The Axis to work with.
            void SetLinearLimitUpperOnAxis(const Real Limit, Whole TranslationAxis);
            /// @brief Get a specific upper translation limit.
            /// @param TranslationAxis The Axis to work with.
            /// @return A real containing the specified upper limit.
            Real GetLinearLimitUpperOnAxis(Whole TranslationAxis) const;

            /// @brief Set the Upper limits on rotation
            /// @param Limit A Vector3 that store the lower limit x, y and z rotation in radians
            virtual void SetAngularLimitUpper(const Vector3& Limit);
            /// @brief Get the Power limits on rotation
            /// @return Limit A Vector3 that stores the lower limit x, y and z rotation in radians
            virtual Vector3 GetAngularLimitUpper() const;
            /// @brief Set the Lower limits on rotation
            /// @param Limit A Vector3 that store the upper limit x, y and z rotation in radians
            virtual void SetAngularLimitLower(const Vector3& Limit);
            /// @brief Get the Upper limits on rotation
            /// @return Limit A Vector3 that stores the upper limit x, y and z rotation in radians
            virtual Vector3 GetAngularLimitLower() const;

            /// @brief Sets the lower rotation limit on a specific axis of the constraint.
            /// @param Limit The limit to apply to the specified axis.
            /// @param RotationAxis The Axis to work with.
            void SetAngularLimitLowerOnAxis(const Real Limit, Whole RotationAxis);
            /// @brief Get a specific lower rotation limit.
            /// @param RotationAxis The Axis to work with.
            /// @return A real containing the specified lower limit.
            Real GetAngularLimitLowerOnAxis(Whole RotationAxis) const;
            /// @brief Sets the upper rotation limit on a specific axis of the constraint.
            /// @param Limit The limit to apply to the specified axis.
            /// @param RotationAxis The Axis to work with.
            void SetAngularLimitUpperOnAxis(const Real Limit, Whole RotationAxis);
            /// @brief Get a specific upper rotation limit.
            /// @param RotationAxis The Axis to work with.
            /// @return A real containing the specified upper limit.
            Real GetAngularLimitUpperOnAxis(Whole RotationAxis) const;

            ////////////////////////////////////////////////////////////////////////////////
            // Angular Limit and Motor Details

            /// @brief Set the Maximimum amount of force applied to ensure limits are not surpassed.
            /// @param MaxLimitForces A Vector3 containing the X, Y and Z Maximium forces.
            virtual void SetAngularLimitMaxForce(const Vector3& MaxLimitForces);
            /// @brief Get the Maximimum amount of force applied to ensure limits are not surpassed.
            /// @return A Vector3 with the forces on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularLimitMaxForce() const;
            /// @brief Set the Maximimum amount of force applied to ensure a limit on one axis is not surpassed.
            /// @param MaxLimitForce The new maximum force.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(Whole).
            virtual void SetAngularLimitMaxForceOnAxis(const Real MaxLimitForce, Whole Axis);
            /// @brief Get the Maximimum amount of force applied to ensure a limit one axis is not surpassed.
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(Whole).
            /// @return A Real with the force for the Given Axis.
            virtual Real GetAngularLimitMaxForceOnAxis(Whole Axis) const;

            /// @brief Set the Target velocity of the motor on each anuglar axis.
            /// @param Velocities A Vector3 containing the X, Y and Z Target Velocites.
            virtual void SetAngularMotorTargetVelocity(const Vector3& Velocities);
            /// @brief Get the target velocity for all angular Axis
            /// @return A Vector3 with the TAger Velocities on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularMotorTargetVelocity() const;
            /// @brief For one Axis, set the target velocity of the angular motor.
            /// @param Velocity The new Target Velovity.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(Whole).
            virtual void SetAngularMotorTargetVelocityOnAxis(const Real Velocity, Whole Axis);
            /// @brief Get the Target Velocity for one axis.
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(Whole).
            /// @return A Real with the force for the Given Axis.
            virtual Real GetAngularMotorTargetVelocityOnAxis(Whole Axis) const;

            /// @brief Set the Angular Motor Maximum force on all 3 rotational axis
            /// @param Forces A Vector3 with the Max Motor Force for each axis.
            virtual void SetAngularMotorMaxForce(const Vector3& Forces);
            /// @brief Get the Max Motor Force for each Axis
            /// @return A Vector3 with the max force on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularMotorMaxForce() const;
            /// @brief For one Axis, set the Maximimum Motor Force.
            /// @param Force The new Max motor force.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(Whole).
            virtual void SetAngularMotorMaxForceOnAxis(const Real Force, Whole Axis);
            /// @brief Get the Max motor Force on a certain Axis.
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(Whole).
            /// @return A Real with the Max Motor Force for the Given Axis.
            virtual Real GetAngularMotorMaxForceOnAxis(Whole Axis) const;

            /// @brief Set the Angular Motor Damping for each Angular Axis.
            /// @param Dampings A Vector3 with Damping value for the X, Y and Z axis.
            virtual void SetAngularMotorDamping(const Vector3& Dampings);
            /// @brief Get the Damping for all Angular Axis.
            /// @return A Vector3 with the forces on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularMotorDamping() const;
            /// @brief For one Axis, set the Damping.
            /// @param Damping The new amount to Damp rotation on the given Axis.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(Whole).
            virtual void SetAngularMotorDampingOnAxis(const Real Damping, Whole Axis);
            /// @brief Get the Damping for one given Axis.
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(Whole).
            /// @return A Real with the XXX for the Given Axis.
            virtual Real GetAngularMotorDampingOnAxis(Whole Axis) const;

            /// @brief Set the Bounciness/Restition for rotation on all three Axis
            /// @param Restitutions A Vector3 containing all the New Bounciness values
            virtual void SetAngularMotorRestitution(const Vector3& Restitutions);
            /// @brief Get the Restitution values for all three axis
            /// @return A Vector3 with the forces on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularMotorRestitution() const;
            /// @brief For one Axis, set the Restitution/Bounciness/
            /// @param Restitution The new value for the given Axis.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(Whole).
            virtual void SetAngularMotorRestitutionOnAxis(const Real Restitution, Whole Axis);
            /// @brief Get the Restitution/Bounciness for a single Axis
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(Whole).
            /// @return A Real with the Restitution for the Given Axis.
            virtual Real GetAngularMotorRestitutionOnAxis(Whole Axis) const;

            /// @brief Set whether or not the motor is enabled for all Axis Simultaneously.
            /// @param Enableds A Vector3 that will be interpretted as 3 true/false values where 0 is false and any other value it true.
            virtual void SetAngularMotorEnabled(const Vector3& Enableds);
            /// @brief Get a Vector3 with 3 zero or nonzero values that store whether or not a given rotational motor is enable.
            /// @return A Vector3 with the forces on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularMotorEnabled() const;
            /// @brief For one Axis, set whether or not the motor is enabled
            /// @param Enabled Is the motor enabled? TRue for yes, false for no.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(Whole).
            virtual void SetAngularMotorEnabledOnAxis(const Boole Enabled, Whole Axis);
            /// @brief Is a specific rotational motor enabled.
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(Whole).
            /// @return A Boole that is true if the given Axis is enabled.
            virtual Boole GetAngularMotorEnabledOnAxis(Whole Axis) const;

            ////////////////////////////////////////////////////////////////////////////////
            // Linear Limit and Motor Details

            /// @brief Set the Softness of the linear Limits.
            /// @param Softness How spongy, how much give does the constraint have.
            virtual void SetLinearLimitSoftness(const Real Softness);
            /// @brief Get the Softness of the linear Limits.
            /// @return The Softness as a real.
            virtual Real GetLinearLimitSoftness() const;

            /// @brief Set the Damping of the linear Limits.
            /// @param Damping The new damping value placed on forces the limits impose.
            virtual void SetLinearLimitDamping(const Real Damping);
            /// @brief Get the Damping of the linear Limits.
            /// @return The Damping as a real.
            virtual Real GetLinearLimitDamping() const;

            /// @brief Set the Restitution of the linear Limits.
            /// @param Restitution How bouncy are the limits.
            virtual void SetLinearLimitRestitution(const Real Restitution);
            /// @brief Get the Restitution of the linear Limits.
            /// @return The Restitution as a real.
            virtual Real GetLinearLimitRestitution() const;

            /// @brief Set the Linear Motor Maximum force on all 3 translation axis.
            /// @param Forces A Vector3 with the Max Motor Force for each axis.
            virtual void SetLinearMotorMaxForce(const Vector3& Forces);
            /// @brief For one Axis, set the Maximimum Motor Force.
            /// @param Force The new Max motor force.
            /// @param Axis The Linear Axis to be set.
            virtual void SetLinearMotorMaxForceOnAxis(const Real Force, Whole Axis);
            /// @brief Get the Max Motor Force for each Axis.
            /// @return A Vector3 with the max force on the X, Y and Z Linear Axis.
            virtual Vector3 GetLinearMotorMaxForce() const;
            /// @brief Get the Max motor Force on a certain Axis.
            /// @param Axis The Linear Axis to get.
            /// @return A Real with the Max Motor Force for the Given Axis.
            virtual Real GetLinearMotorMaxForceOnAxis(Whole Axis) const;

            /// @brief Set the Target velocity of the motor on each anuglar axis.
            /// @param Velocities A Vector3 containing the X, Y and Z Target Velocites.
            virtual void SetLinearMotorTargetVelocity(const Vector3& Velocities);
            /// @brief For one Axis, set the target velocity of the Linear motor.
            /// @param Velocity The new Target Velovity.
            /// @param Axis The Linear Axis to be set.
            virtual void SetLinearMotorTargetVelocityOnAxis(const Real Velocity, Whole Axis);
            /// @brief Get the target velocity for all Linear Axis
            /// @return A Vector3 with the Target Velocities on the X, Y and Z Linear Axis.
            virtual Vector3 GetLinearMotorTargetVelocity() const;
            /// @brief Get the Target Velocity for one axis.
            /// @param Axis The Linear Axis to get.
            /// @return A Real with the force for the Given Axis.
            virtual Real GetLinearMotorTargetVelocityOnAxis(Whole Axis) const;

            /// @brief Set whether or not the motor is enabled for all Linear Axis Simultaneously.
            /// @param Enableds A Vector3 that will be interpretted as 3 true/false values where 0 is false and any other value it true.
            virtual void SetLinearMotorEnabled(const Vector3& Enableds);
            /// @brief For one Axis, set whether or not the motor is enabled
            /// @param Enabled Is the motor enabled? True for yes, false for no.
            /// @param Axis The Linear Axis to be set.
            virtual void SetLinearMotorEnabledOnAxis(const Boole Enabled, Whole Axis);
            /// @brief Get a Vector3 with 3 zero or nonzero values that store whether or not a given rotational motor is enable.
            /// @return A Vector3 with the forces on the X, Y and Z Linear Axis.
            virtual Vector3 GetLinearMotorEnabled() const;
            /// @brief Is a specific Linear motor enabled.
            /// @param Axis The Linear Axis to get.
            /// @return A Boole that is true if the given Axis is enabled.
            virtual Boole GetLinearMotorEnabledOnAxis(Whole Axis) const;

            ////////////////////////////////////////////////////////////////////////////////
            // Axis Params

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
        };//Generic6DofConstraint
    }//Physics
}//Mezzanine

#endif
