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
        /// translation. You can get the ID of the axis with ValidLinearAxis() or
        /// ValidAngularAxis()  for this particular constraint at the time of this
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
        protected:
            /// @brief Bullet constraint that this class encapsulates.
            btGeneric6DofConstraint* Generic6dof;
            /// @brief Inheritance Constructor.
            /// @details This is only called by derived classes, and shouldn't be called manually.
            Generic6DofConstraint();
        public:
            /// @brief Identify the Axis a bit easier when iterating over them is less convienent than typing an Identifier
            enum UsableAxis{
                LinearX         = 0,    ///< Translation on the X axis
                LinearY         = 1,    ///< Translation on the Y axis
                LinearZ         = 2,    ///< Translation on the Z axis
                AngularX        = 3,    ///< Rotation on the X axis
                AngularY        = 4,    ///< Rotation on the Y axis
                AngularZ        = 5,    ///< Rotation on the Z axis

                AngularXAsRotationalAxis        = 0,    ///< Rotation on the X axis, when working with only rotational Axis
                AngularYAsRotationalAxis        = 1,    ///< Rotation on the Y axis, when working with only rotational Axis
                AngularZAsRotationalAxis        = 2     ///< Rotation on the Z axis, when working with only rotational Axis
            };

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofConstraint Construction and Destruction

            /// @brief Two proxy Verbose constructor.
            /// @param ProxyA The First proxy to be bound.
            /// @param ProxyB  The Second proxy to be bound.
            /// @param VectorA The offset from ProxyAs center of gravity to get to match an offset from ProxyB.
            /// @param VectorB The offset from ProxyBs center of gravity.
            /// @param QuaternionA Relative rotation from ProxyA.
            /// @param QuaternionB Relative rotation from ProxyB.
            /// @param UseLinearReferenceA Perform Linear math from ProxyA's perspective, default to false.
            Generic6DofConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& VectorA, const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, Boole UseLinearReferenceA = false);
            /// @brief Two proxy Terse constructor.
            /// @param ProxyA The First proxy to be bound.
            /// @param ProxyB  The Second proxy to be bound.
            /// @param TransformA The offset and rotation from ProxyAs center of gravity to get to match an offset from ProxyB.
            /// @param TransformB The offset and rotation from ProxyBs center of gravity.
            /// @param UseLinearReferenceA Perform Linear math from ProxyA's perspective, default to false.
            Generic6DofConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransformA, const Transform& TransformB, Boole UseLinearReferenceA = false);
            /// @brief Body and World Verbose constructor.
            /// @param ProxyB The proxy to be bound to the world.
            /// @param VectorB The offset for the ProxyB pivot/hinge/joint.
            /// @param QuaternionB The rotation.
            /// @param UseLinearReferenceB Perform Linear math from ProxyBs perspective, default to true.
            /// @details This Joins an object to the world.
            Generic6DofConstraint(RigidProxy* ProxyB, const Vector3& VectorB, const Quaternion& QuaternionB, Boole UseLinearReferenceB = true);
            /// @brief Body and World Terse constructor.
            /// @param ProxyB The proxy to be bound to the world.
            /// @param TransformB The offset and rotation for the ProxyB pivot/hinge/joint.
            /// @param UseLinearReferenceB Perform Linear math from ProxyB's perspective, default to true.
            /// @details This Joins an object to the world.
            Generic6DofConstraint(RigidProxy* ProxyB, const Transform& TransformB, Boole UseLinearReferenceB = true);
            /// @brief Class destructor.
            virtual ~Generic6DofConstraint();

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofConstraint Location and Rotation

            /// @copydoc DualTransformConstraint::SetPivotATransform(const Transform&)
            virtual void SetPivotATransform(const Transform& TranA);
            /// @copydoc DualTransformConstraint::SetPivotBTransform(const Transform&)
            virtual void SetPivotBTransform(const Transform& TranB);
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
            // Generic6DofConstraint Basic Limit Accessors

            /// @brief Change the upper and lower limit for one axis of translation or rotation limit
            /// @param Axis The axis to change
            /// @param Lower The new lower limit
            /// @param UpperThe new Higher limit
            virtual void SetLimit(int Axis, Real Lower, Real Upper);

            /// @brief Set the lower limits on translation
            /// @param Limit A Vector3 that stores the lower limit x, y and z that cannot be exceeded
            virtual void SetLinearLimitUpper(const Vector3& Limit);
            /// @brief Set the Upper limits on translation
            /// @return A Vector3 that stores the upper limit x, y and z that cannot be exceeded
            virtual void SetLinearLimitLower(const Vector3& Limit);
            /// @brief Get the lower limits on translation
            /// @param Limit A Vector3 that stores the lower limit x, y and z that cannot be exceeded
            virtual Vector3 GetLinearLimitUpper() const;
            /// @brief Get the Upper limits on translation
            /// @return A Vector3 that stores the upper limit x, y and z that cannot be exceeded
            virtual Vector3 GetLinearLimitLower() const;

            /// @brief Set the Upper limits on rotation
            /// @param Limit A Vector3 that store the lower limit x, y and z rotation in radians
            virtual void SetAngularLimitUpper(const Vector3& Limit);
            /// @brief Set the Lower limits on rotation
            /// @param Limit A Vector3 that store the upper limit x, y and z rotation in radians
            virtual void SetAngularLimitLower(const Vector3& Limit);
            /// @brief Get the Power limits on rotation
            /// @return Limit A Vector3 that stores the lower limit x, y and z rotation in radians
            virtual Vector3 GetAngularLimitUpper() const;
            /// @brief Get the Upper limits on rotation
            /// @return Limit A Vector3 that stores the upper limit x, y and z rotation in radians
            virtual Vector3 GetAngularLimitLower() const;

            /// @brief Get a specific lower rotational limit
            /// @param RotationalAxis The Axis to work with.
            Real GetAngularLimitLowerOnAxis(int RotationalAxis) const;
            /// @brief Get a specific upper rotational limit
            /// @param RotationalAxis The Axis to work with.
            /// @details This selects axis with
            /// @return A real containing the specified upper limit
            Real GetAngularLimitUpperOnAxis(int RotationalAxis) const;

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofConstraint Angular Limit and Motor Details

            /// @brief Convert to internal Axis IDs from external or internal axis IDs
            /// @param Axis the Axis ID to be converted
            /// @return A number that correlates toe corresponding internal Axis. For example Axis 4 is the Rotation Y. This would return the corresponding ID for the internal Rotation Y axis
            /// @details Due to the way this is stored internally, All the axis are listed from from
            /// 0 to 2. So rather than throw an exception(or have undefined behavior) if the Axis
            /// selected one of those, this select the axis like this:
            ///     - 0, 3: Rotation X
            ///     - 1, 4: Rotation Y
            ///     - 2, 5: Rotation Z
            /// @return A real containing the specified lower limit
            inline virtual int AxisToAngularAxis(int Axis) const
                { return (Axis%3); }

            /// @brief Set the Maximimum amount of force applied to ensure limits are not surpassed.
            /// @param MaxLimitForces A Vector3 containing the X, Y and Z Maximium forces.
            virtual void SetAngularLimitMaxForce(const Vector3& MaxLimitForces);
            /// @brief Set the Maximimum amount of force applied to ensure a limit on one axis is not surpassed.
            /// @param MaxLimitForce The new maximum force.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(int).
            virtual void SetAngularLimitMaxForceOnAxis(Real MaxLimitForce, int Axis);
            /// @brief Get the Maximimum amount of force applied to ensure limits are not surpassed.
            /// @return A Vector3 with the forces on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularLimitMaxForce() const;
            /// @brief Get the Maximimum amount of force applied to ensure a limit one axis is not surpassed.
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(int).
            /// @return A Real with the force for the Given Axis.
            virtual Real GetAngularLimitMaxForceOnAxis(int Axis) const;

            /// @brief Set the Target velocity of the motor on each anuglar axis.
            /// @param Velocities A Vector3 containing the X, Y and Z Target Velocites.
            virtual void SetAngularMotorTargetVelocity(const Vector3& Velocities);
            /// @brief For one Axis, set the target velocity of the angular motor.
            /// @param Velocity The new Target Velovity.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(int).
            virtual void SetAngularMotorTargetVelocityOnAxis(Real Velocity, int Axis);
            /// @brief Get the target velocity for all angular Axis
            /// @return A Vector3 with the TAger Velocities on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularMotorTargetVelocity() const;
            /// @brief Get the Target Velocity for one axis.
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(int).
            /// @return A Real with the force for the Given Axis.
            virtual Real GetAngularMotorTargetVelocityOnAxis(int Axis) const;

            /// @brief Set the Angular Motor Maximum force on all 3 rotational axis
            /// @param Forces A Vector3 with the Max Motor Force for each axis.
            virtual void SetAngularMotorMaxForce(const Vector3& Forces);
            /// @brief For one Axis, set the Maximimum Motor Force.
            /// @param Force The new Max motor force.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(int).
            virtual void SetAngularMotorMaxForceOnAxis(Real Force, int Axis);
            /// @brief Get the Max Motor Force for each Axis
            /// @return A Vector3 with the max force on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularMotorMaxForce() const;
            /// @brief Get the Max motor Force on a certain Axis.
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(int).
            /// @return A Real with the Max Motor Force for the Given Axis.
            virtual Real GetAngularMotorMaxForceOnAxis(int Axis) const;

            /// @brief Set the Angular Motor Damping for each Angular Axis.
            /// @param Dampings A Vector3 with Damping value for the X, Y and Z axis.
            virtual void SetAngularMotorDamping(const Vector3& Dampings);
            /// @brief For one Axis, set the Damping.
            /// @param Damping The new amount to Damp rotation on the given Axis.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(int).
            virtual void SetAngularMotorDampingOnAxis(Real Damping, int Axis);
            /// @brief Get the Damping for all Angular Axis.
            /// @return A Vector3 with the forces on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularMotorDamping() const;
            /// @brief Get the Damping for one given Axis.
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(int).
            /// @return A Real with the XXX for the Given Axis.
            virtual Real GetAngularMotorDampingOnAxis(int Axis) const;

            /// @brief Set the Bounciness/Restition for rotation on all three Axis
            /// @param Restistutions A Vector3 containing all the New Bounciness values
            virtual void SetAngularMotorRestitution(const Vector3& Restistutions);
            /// @brief For one Axis, set the Restistution/Bounciness/
            /// @param Restistution The new value for the given Axis.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(int).
            virtual void SetAngularMotorRestitutionOnAxis(Real Restistution, int Axis);
            /// @brief Get the Restistution values for all three axis
            /// @return A Vector3 with the forces on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularMotorRestitution() const;
            /// @brief Get the Restistution/Bounciness for a single Axis
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(int).
            /// @return A Real with the Restistution for the Given Axis.
            virtual Real GetAngularMotorRestitutionOnAxis(int Axis) const;

            /// @brief Set whether or not the motor is enabled for all Axis Simultaneously.
            /// @param Enableds A Vector3 that will be interpretted as 3 true/false values where 0 is false and any other value it true.
            virtual void SetAngularMotorEnabled(const Vector3& Enableds);
            /// @brief For one Axis, set whether or not the motor is enabled
            /// @param Enabled Is the motor enabled? TRue for yes, false for no.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(int).
            virtual void SetAngularMotorEnabledOnAxis(Boole Enabled, int Axis);
            /// @brief Get a Vector3 with 3 zero or nonzero values that store whether or not a given rotational motor is enable.
            /// @return A Vector3 with the forces on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularMotorEnabled() const;
            /// @brief Is a specific rotational motor enabled.
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(int).
            /// @return A Boole that is true if the given Axis is enabled.
            virtual Boole GetAngularMotorEnabledOnAxis(int Axis) const;

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofConstraint Linear Limit and Motor Details

            /// @brief Set the Softness of the linear Limits
            /// @param Softness How spongy, how much give does the constraint have.
            virtual void SetLinearLimitSoftness(Real Softness);
            /// @brief Get the Softness of the linear Limits
            /// @return The Softness as a real.
            virtual Real GetLinearLimitSoftness() const;

            /// @brief Set the Damping of the linear Limits
            /// @param Damping The new damping value placed on forces the limits impose
            virtual void SetLinearLimitDamping(Real Damping);
            /// @brief Get the Damping of the linear Limits
            /// @return The Damping as a real.
            virtual Real GetLinearLimitDamping() const;

            /// @brief Set the Restitution of the linear Limits.
            /// @param Restitution How bouncy are the limits.
            virtual void SetLinearLimitRestitution(Real Restitution);
            /// @brief Get the Restitution of the linear Limits.
            /// @return The Restitution as a real.
            virtual Real GetLinearLimitRestitution() const;

            /// @brief Set the Linear Motor Maximum force on all 3 translation axis
            /// @param Forces A Vector3 with the Max Motor Force for each axis.
            virtual void SetLinearMotorMaxForce(const Vector3& Forces);
            /// @brief For one Axis, set the Maximimum Motor Force.
            /// @param Force The new Max motor force.
            /// @param Axis The Linear Axis to be set.
            virtual void SetLinearMotorMaxForceOnAxis(Real Force, int Axis);
            /// @brief Get the Max Motor Force for each Axis
            /// @return A Vector3 with the max force on the X, Y and Z Linear Axis.
            virtual Vector3 GetLinearMotorMaxForce() const;
            /// @brief Get the Max motor Force on a certain Axis.
            /// @param Axis The Linear Axis to get.
            /// @return A Real with the Max Motor Force for the Given Axis.
            virtual Real GetLinearMotorMaxForceOnAxis(int Axis) const;

            /// @brief Set the Target velocity of the motor on each anuglar axis.
            /// @param Velocities A Vector3 containing the X, Y and Z Target Velocites.
            virtual void SetLinearMotorTargetVelocity(const Vector3& Velocities);
            /// @brief For one Axis, set the target velocity of the Linear motor.
            /// @param Velocity The new Target Velovity.
            /// @param Axis The Linear Axis to be set.
            virtual void SetLinearMotorTargetVelocityOnAxis(Real Velocity, int Axis);
            /// @brief Get the target velocity for all Linear Axis
            /// @return A Vector3 with the Target Velocities on the X, Y and Z Linear Axis.
            virtual Vector3 GetLinearMotorTargetVelocity() const;
            /// @brief Get the Target Velocity for one axis.
            /// @param Axis The Linear Axis to get.
            /// @return A Real with the force for the Given Axis.
            virtual Real GetLinearMotorTargetVelocityOnAxis(int Axis) const;

            /// @brief Set whether or not the motor is enabled for all Linear Axis Simultaneously.
            /// @param Enableds A Vector3 that will be interpretted as 3 true/false values where 0 is false and any other value it true.
            virtual void SetLinearMotorEnabled(const Vector3& Enableds);
            /// @brief For one Axis, set whether or not the motor is enabled
            /// @param Enabled Is the motor enabled? True for yes, false for no.
            /// @param Axis The Linear Axis to be set.
            virtual void SetLinearMotorEnabledOnAxis(Boole Enabled, int Axis);
            /// @brief Get a Vector3 with 3 zero or nonzero values that store whether or not a given rotational motor is enable.
            /// @return A Vector3 with the forces on the X, Y and Z Linear Axis.
            virtual Vector3 GetLinearMotorEnabled() const;
            /// @brief Is a specific Linear motor enabled.
            /// @param Axis The Linear Axis to get.
            /// @return A Boole that is true if the given Axis is enabled.
            virtual Boole GetLinearMotorEnabledOnAxis(int Axis) const;

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofConstraint Axis, Params and other Details

            /// @copydoc Constraint::ValidParamOnAxis(int) const
            virtual Constraint::ParamList ValidParamOnAxis(int Axis) const;
            /// @copydoc Constraint::ValidLinearAxis() const
            virtual Constraint::AxisList ValidLinearAxis() const;
            /// @copydoc Constraint::ValidAngularAxis() const
            virtual Constraint::AxisList ValidAngularAxis() const;
            /// @copydoc Constraint::ValidAngularAxis(ConstraintParam,int) const
            virtual Boole HasParamBeenSet(ConstraintParam Param, int Axis) const;

            /// @brief Retrieve the stored value from the physics subsystem(bullet)
            /// @return a True or false.
            virtual Boole GetUseFrameOffset() const;
            /// @brief Set the stored value for UseFrameOffset on this hinge in the physics subsystem(bullet)
            /// @param FrameOffset The new desired value.
            virtual void SetUseFrameOffset(Boole FrameOffset);

            /// @copydoc Constraint::GetConstraintBase() const
            virtual btTypedConstraint* GetConstraintBase() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Generic6DofConstraint Serialization

            /// @brief Convert this class to an XML::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this vectorw should be appended to.
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and XML::Node containing the data.
            /// @warning A precondition of using this is that all of the actors intended for use must already be Deserialized.
            virtual void ProtoDeSerialize(const XML::Node& OneNode);
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "Generic6DofConstraint"
            static String SerializableName();
        };//Generic6DofConstraint
    }//Physics
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

#ifndef SWIG
/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::Constraint& x)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::Generic6DofConstraint& x);
/// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::Constraint& x)
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::Generic6DofConstraint& x);
/// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Constraint& x)
void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Generic6DofConstraint& x);
#endif

#endif
