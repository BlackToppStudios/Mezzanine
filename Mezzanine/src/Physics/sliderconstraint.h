// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _physicssliderconstraint_h
#define _physicssliderconstraint_h

#include "Physics/dualtransformconstraint.h"

class btSliderConstraint;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class SliderConstraint
        /// @brief This is a constraint for permitting movement on only a singular linear or angular axis.
        /// @details This class comes with an unusually high amount of parameters that can be set.  A lot of the parameters are obsolete but remain
        /// in the class in case they become used again or their boilerplate is proven to be useful.  In other words, they exist because they are
        /// still there in the internal implementation so we reflect that.
        /// @n @n
        /// Like other constraints with limits, such as the ConeTwistConstraint, there are a small series of parameters that control how the constraint
        /// is to behave when near, at, or beyond it's set limits.  In the case of the Slider they are; Softness, Damping, and Restituion.  @n
        /// Constraint Softness is the act of applying a pre-emptive force to smooth out the force to be applied.  Lower values means a smoother
        /// transition from further away from the limit whereas higher values mean a more rapid transition closer to  the limit.  Values should be in
        /// a range of 0.0-1.0.  Values in the higher end of the range (0.9-1.0) are recommended.  @n
        /// Constraint Damping is similar to damping in other parts of the physics engine.  It is more-or-less something that inhibits movement along
        /// appropriate axes.  Damping is useful to simulate some forms of friction, or even air resistance.  Values should be in a range of 0.0-1.0.
        /// Values in the higher end of the range (0.7-1.0) are recommended.  @n
        /// Constraint Restitution is also similar to other parts of the physics engine.  It is the force applied back after a limit is reached, based
        /// on the size of the force that hit the limit.  Larger values mean it'll bounce back more, whereas lower values will give little to no bounce
        /// back.  Values should in a range of 0.0-1.0.  Values in the lower end of the range (0.0-0.3) are recommended.
        /// @n @n
        /// To further complicate the SliderConstraint compared to other systems in the physics subsystem, there are multiple types of limits defined.
        /// The different properties mentioned above can be used at different times depending on the position of the child object and how it is moving.  @n
        /// The definition of the "Dir" prefix used in the SliderConstraint methods is unknown, but it applies to whenever the child object is within
        /// the set limits of the constraint and moving along the configured axis of the constraint.  @n
        /// The definition of the "Lim" prefix is "Limit", and it applies to whenever the child object hits the configured limit of the constraint along
        /// the configured axis of the constraint.  @n
        /// The definition of the "Ortho" prefix is "Orthogonal", and it applies to any forces that are enacted on the child object that force it to
        /// move against the configured axis on the constraint.
        ///////////////////////////////////////
        class MEZZ_LIB SliderConstraint : public DualTransformConstraint
        {
        protected:
            /// @internal
            /// @brief Bullet constraint that this class encapsulates.
            btSliderConstraint* Slider;

            /// @copydoc DualTransformConstraint::CreateConstraint(RigidProxy*, RigidProxy*, const Transform&, const Transform&)
            virtual void CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB);
            /// @copydoc DualTransformConstraint::DestroyConstraint()
            virtual void DestroyConstraint();
        public:
            /// @brief Two body Terse constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA The First proxy to be bound.
            /// @param ProxyB  The Second proxy to be bound.
            /// @param TransA The offset and rotation from ProxyA's center of gravity to get to match an offset from ProxyB.
            /// @param TransB The offset and rotation from ProxyB's center of gravity.
            /// @param Creator A pointer to the manager that created this constraint.
            SliderConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB, PhysicsManager* Creator);
            /// @brief Standard one body constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA The First proxy to be bound.
            /// @param TransA The offset and rotation from ProxyA's center of gravity.
            /// @param Creator A pointer to the manager that created this constraint.
            SliderConstraint(const UInt32 ID, RigidProxy* ProxyA, const Transform& TransA, PhysicsManager* Creator);
            /// @brief XML constructor.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @param Creator A pointer to the manager that created this constraint.
            SliderConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator);
            /// @brief Class destructor.
            virtual ~SliderConstraint();

            ////////////////////////////////////////////////////////////////////////////////
            // Location and Rotation

            /// @copydoc DualTransformConstraint::SetPivotTransforms(const Transform&, const Transform&)
            virtual void SetPivotTransforms(const Transform& TransA, const Transform& TransB);
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
            // Utility

            /// @brief Sets whether or not an offset of the constraint frame should be used to calculate internal data.
            /// @param FrameOffset The full effect of this being true or false is uknown, but internal documentation suggests "true" provides more stable results.
            virtual void SetUseFrameOffset(const Boole FrameOffset);
            /// @brief Gets whether or not an offset of the constraint frame should be used to calculate internal data.
            /// @return Returns whether or not an offset in the constraint frame is being used by the internal constraint.
            virtual Boole GetUseFrameOffset() const;

            /// @brief Sets the upper linear limit of this slider.
            /// @param UpperLimit The upper limit for sliding on the linear axis.
            virtual void SetUpperLinLimit(const Real UpperLimit);
            /// @brief Gets the upper linear limit of this slider.
            /// @return Returns a Real representing the current upper limit for sliding on the linear axis.
            virtual Real GetUpperLinLimit() const;
            /// @brief Sets the upper angular limit of this slider.
            /// @param UpperLimit The upper limit for sliding on the angular axis.
            virtual void SetUpperAngLimit(const Real UpperLimit);
            /// @brief Gets the upper angular limit of this slider.
            /// @return Returns a Real representing the current upper limit for sliding on the angular axis.
            virtual Real GetUpperAngLimit() const;
            /// @brief Sets the lower linear limit of this slider.
            /// @param LowerLimit The lower limit for sliding on the linear axis.
            virtual void SetLowerLinLimit(const Real LowerLimit);
            /// @brief Gets the lower linear limit of this slider.
            /// @return Returns a Real representing the current lower limit for sliding on the linear axis.
            virtual Real GetLowerLinLimit() const;
            /// @brief Sets the lower angular limit of this slider.
            /// @param LowerLimit The lower limit for sliding on the angular axis.
            virtual void SetLowerAngLimit(const Real LowerLimit);
            /// @brief Gets the lower angular limit of this slider.
            /// @return Returns a Real representing the current lower limit for sliding on the angular axis.
            virtual Real GetLowerAngLimit() const;

            /// @brief Sets whether or not the motor for the linear axis of this constraint is enabled.
            /// @param OnOff True to enabled the motor, false to disable it.
            virtual void SetPoweredLinMotor(const Boole OnOff);
            /// @brief Gets whether or not the motor for the linear axis of this constraint is enabled.
            /// @return Returns true if the motor is currently enabled, false otherwise.
            virtual Boole GetPoweredLinMotor() const;
            /// @brief Sets whether or not the motor for the angular axis of this constraint is enabled.
            /// @param OnOffTrue to enabled the motor, false to disable it.
            virtual void SetPoweredAngMotor(const Boole OnOff);
            /// @brief Gets whether or not the motor for the angular axis of this constraint is enabled.
            /// @return Returns true if the motor is currently enabled, false otherwise.
            virtual Boole GetPoweredAngMotor() const;
            /// @brief Sets the target velocity of the motor on the linear axis of ths constraint.
            /// @param TargetLinMotorVelocity The target velocity to acheive, which determines the amount of force to apply on the axis.
            virtual void SetTargetLinMotorVelocity(const Real TargetLinMotorVelocity);
            /// @brief Gets the target velocity of the motor on the linear axis of ths constraint.
            /// @return Returns a Real representing how fast the motor should drive the constrained object on the linear axis of the constraint.
            virtual Real GetTargetLinMotorVelocity() const;
            /// @brief Sets the target velocity of the motor on the angular axis of ths constraint.
            /// @param TargetAngMotorVelocity The target velocity to acheive, which determines the amount of force to apply on the axis.
            virtual void SetTargetAngMotorVelocity(const Real TargetAngMotorVelocity);
            /// @brief Gets the target velocity of the motor on the angular axis of ths constraint.
            /// @return Returns a Real representing how fast the motor should drive the constrained object on the angular axis of the constraint.
            virtual Real GetTargetAngMotorVelocity() const;
            /// @brief Sets the maximum force allowed to be applied on the linear axis of this constraint.
            /// @param MaxLinMotorForce The cap for the force that can be appled to the child object on the linear axis of this constraint.
            virtual void SetMaxLinMotorForce(const Real MaxLinMotorForce);
            /// @brief Gets the maximum force allowed to be applied on the linear axis of this constraint.
            /// @return Returns a Real representing the maximum allowed force on the linear axis of this constraint.
            virtual Real GetMaxLinMotorForce() const;
            /// @brief Sets the maximum force allowed to be applied on the angular axis of this constraint.
            /// @param MaxAngMotorForce The cap for the force that can be appled to the child object on the angular axis of this constraint.
            virtual void SetMaxAngMotorForce(const Real MaxAngMotorForce);
            /// @brief Gets the maximum force allowed to be applied on the angular axis of this constraint.
            /// @return Returns a Real representing the maximum allowed force on the angular axis of this constraint.
            virtual Real GetMaxAngMotorForce() const;

            /// @brief Sets the softness value to be used when hitting the linear limit of this constraint.
            /// @param SoftnessLimLin The softness value to be set for the linear axis.
            virtual void SetSoftnessLimLin(const Real SoftnessLimLin);
            /// @brief Gets the softness value to be used when hitting the linear limit of this constraint.
            /// @return Returns a Real representing the current softness on the linear axis.
            virtual Real GetSoftnessLimLin() const;
            /// @brief Sets the softness value to be used when hitting the angular limit of this constraint.
            /// @param SoftnessLimAng The softness value to be set for the angular axis.
            virtual void SetSoftnessLimAng(const Real SoftnessLimAng);
            /// @brief Gets the softness value to be used when hitting the angular limit of this constraint.
            /// @return Returns a Real representing the current softness on the angular axis.
            virtual Real GetSoftnessLimAng() const;
            /// @brief Sets the softness value to be used when force is applied against the configured linear axis of this constraint.
            /// @param SoftnessOrthoLin The softness value to be set for the linear axis.
            virtual void SetSoftnessOrthoLin(const Real SoftnessOrthoLin);
            /// @brief Gets the softness value to be used when force is applied against the configured linear axis of this constraint.
            /// @return Returns a Real representing the current softness on the linear axis.
            virtual Real GetSoftnessOrthoLin() const;
            /// @brief Sets the softness value to be used when force is applied against the configured angular axis of this constraint.
            /// @param SoftnessOrthoAng The softness value to be set for the angular axis.
            virtual void SetSoftnessOrthoAng(const Real SoftnessOrthoAng);
            /// @brief Gets the softness value to be used when force is applied against the configured angular axis of this constraint.
            /// @return Returns a Real representing the current softness on the angular axis.
            virtual Real GetSoftnessOrthoAng() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Obsolete Methods - All methods listed in this subsection appears to alter deprecated/unused data

            /// @brief Sets the softness value to be used when within the linear limits of this constraint.
            /// @param SoftnessDirLin The softness value to be set for the linear axis.
            virtual void SetSoftnessDirLin(const Real SoftnessDirLin);
            /// @brief Gets the softness value to be used when within the linear limits of this constraint.
            /// @return Returns a Real representing the current softness on the linear axis.
            virtual Real GetSoftnessDirLin() const;
            /// @brief Sets the softness value to be used when within the angular limits of this constraint.
            /// @param SoftnessDirAng The softness value to be set for the angular axis.
            virtual void SetSoftnessDirAng(const Real SoftnessDirAng);
            /// @brief Gets the softness value to be used when within the angular limits of this constraint.
            /// @return Returns a Real representing the current softness on the angular axis.
            virtual Real GetSoftnessDirAng() const;

            /// @brief Sets the damping value to be used when within the linear limits of this constraint.
            /// @param DampingDirLin The damping value to be set for the linear axis.
            virtual void SetDampingDirLin(const Real DampingDirLin);
            /// @brief Gets the damping value to be used when within the linear limits of this constraint.
            /// @return Returns a Real representing the current damping on the linear axis.
            virtual Real GetDampingDirLin() const;
            /// @brief Sets the damping value to be used when within the angular limits of this constraint.
            /// @param DampingDirAng The damping value to be set for the angular axis.
            virtual void SetDampingDirAng(const Real DampingDirAng);
            /// @brief Gets the damping value to be used when within the angular limits of this constraint.
            /// @return Returns a Real representing the current damping on the angular axis.
            virtual Real GetDampingDirAng() const;
            /// @brief Sets the damping value to be used when hitting the linear limit of this constraint.
            /// @param DampingLimLin The damping value to be set for the linear axis.
            virtual void SetDampingLimLin(const Real DampingLimLin);
            /// @brief Gets the damping value to be used when hitting the linear limit of this constraint.
            /// @return Returns a Real representing the current damping on the linear axis.
            virtual Real GetDampingLimLin() const;
            /// @brief Sets the damping value to be used when hitting the angular limit of this constraint.
            /// @param DampingLimAng The damping value to be set for the angular axis.
            virtual void SetDampingLimAng(const Real DampingLimAng);
            /// @brief Gets the damping value to be used when hitting the angular limit of this constraint.
            /// @return Returns a Real representing the current damping on the angular axis.
            virtual Real GetDampingLimAng() const;
            /// @brief Sets the damping value to be used when force is applied against the configured linear axis of this constraint.
            /// @param DampingOrthoLin The damping value to be set for the linear axis.
            virtual void SetDampingOrthoLin(const Real DampingOrthoLin);
            /// @brief Gets the damping value to be used when force is applied against the configured linear axis of this constraint.
            /// @return Returns a Real representing the current damping on the linear axis.
            virtual Real GetDampingOrthoLin() const;
            /// @brief Sets the damping value to be used when force is applied against the configured angular axis of this constraint.
            /// @param DampingOrthoAng The damping value to be set for the angular axis.
            virtual void SetDampingOrthoAng(const Real DampingOrthoAng);
            /// @brief Gets the damping value to be used when force is applied against the configured angular axis of this constraint.
            /// @return Returns a Real representing the current damping on the angular axis.
            virtual Real GetDampingOrthoAng() const;

            /// @brief Sets the restitution value to be used when within the linear limits of this constraint.
            /// @param RestitutionDirLin The restitution value to be set for the linear axis.
            virtual void SetRestitutionDirLin(const Real RestitutionDirLin);
            /// @brief Gets the restitution value to be used when within the linear limits of this constraint.
            /// @return Returns a Real representing the current restitution on the linear axis.
            virtual Real GetRestitutionDirLin() const;
            /// @brief Sets the restitution value to be used when within the angular limits of this constraint.
            /// @param RestitutionDirAng The restitution value to be set for the angular axis.
            virtual void SetRestitutionDirAng(const Real RestitutionDirAng);
            /// @brief Gets the restitution value to be used when within the angular limits of this constraint.
            /// @return Returns a Real representing the current restitution on the angular axis.
            virtual Real GetRestitutionDirAng() const;
            /// @brief Sets the restitution value to be used when hitting the linear limit of this constraint.
            /// @param RestitutionLimLin The restitution value to be set for the linear axis.
            virtual void SetRestitutionLimLin(const Real RestitutionLimLin);
            /// @brief Gets the restitution value to be used when hitting the linear limit of this constraint.
            /// @return Returns a Real representing the current restitution on the linear axis.
            virtual Real GetRestitutionLimLin() const;
            /// @brief Sets the restitution value to be used when hitting the angular limit of this constraint.
            /// @param RestitutionLimAng The restitution value to be set for the angular axis.
            virtual void SetRestitutionLimAng(const Real RestitutionLimAng);
            /// @brief Gets the restitution value to be used when hitting the angular limit of this constraint.
            /// @return Returns a Real representing the current restitution on the angular axis.
            virtual Real GetRestitutionLimAng() const;
            /// @brief Sets the restitution value to be used when force is applied against the configured linear axis of this constraint.
            /// @param RestitutionOrthoLin The restitution value to be set for the linear axis.
            virtual void SetRestitutionOrthoLin(const Real RestitutionOrthoLin);
            /// @brief Gets the restitution value to be used when force is applied against the configured linear axis of this constraint.
            /// @return Returns a Real representing the current restitution on the linear axis.
            virtual Real GetRestitutionOrthoLin() const;
            /// @brief Sets the restitution value to be used when force is applied against the configured angular axis of this constraint.
            /// @param RestitutionOrthoAng The restitution value to be set for the angular axis.
            virtual void SetRestitutionOrthoAng(const Real RestitutionOrthoAng);
            /// @brief Gets the restitution value to be used when force is applied against the configured angular axis of this constraint.
            /// @return Returns a Real representing the current restitution on the angular axis.
            virtual Real GetRestitutionOrthoAng() const;

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
        };//SliderConstraint
    }//Physics
}//Mezzanine

#endif
