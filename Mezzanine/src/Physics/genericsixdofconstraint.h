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
#ifndef _physicsgenericsixdofconstraint_h
#define _physicsgenericsixdofconstraint_h

#include "Physics/sixdofconstraint.h"

class btGeneric6DofConstraint;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a generic implementation of the 6Dof constraint.
        ///////////////////////////////////////
        class MEZZ_LIB GenericSixDofConstraint : public SixDofConstraint
        {
        protected:
            /// @brief The internal constraint that this class encapsulates.
            btGeneric6DofConstraint* Generic6dof;

            /// @copydoc DualTransformConstraint::CreateConstraint(RigidProxy*, RigidProxy*, const Transform&, const Transform&)
            virtual void CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB);
            /// @copydoc DualTransformConstraint::DestroyConstraint()
            virtual void DestroyConstraint();

            /// @brief Single body inheritance constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param Prox1 A pointer to the first/only proxy that will be constrained.
            /// @param Creator A pointer to the manager that created this constraint.
            GenericSixDofConstraint(const UInt32 ID, RigidProxy* Prox1, PhysicsManager* Creator);
            /// @brief Dual body inheritance constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param Prox1 A pointer to the first proxy that will be constrained.
            /// @param Prox2 A pointer to the second proxy that will be constrained.
            /// @param Creator A pointer to the manager that created this constraint.
            GenericSixDofConstraint(const UInt32 ID, RigidProxy* Prox1, RigidProxy* Prox2, PhysicsManager* Creator);
        public:
            /// @brief Dual body constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA The First proxy to be bound.
            /// @param ProxyB The Second proxy to be bound.
            /// @param TransA The offset and rotation from ProxyAs center of gravity to get to match an offset from ProxyB.
            /// @param TransB The offset and rotation from ProxyBs center of gravity.
            /// @param Creator A pointer to the manager that created this constraint.
            GenericSixDofConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB, PhysicsManager* Creator);
            /// @brief Single body constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyB The proxy to be bound to the world.
            /// @param TransB The offset and rotation for the ProxyB pivot/hinge/joint.
            /// @param Creator A pointer to the manager that created this constraint.
            GenericSixDofConstraint(const UInt32 ID, RigidProxy* ProxyB, const Transform& TransB, PhysicsManager* Creator);
            /// @brief XML constructor.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @param Creator A pointer to the manager that created this constraint.
            GenericSixDofConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator);
            /// @brief Class destructor.
            virtual ~GenericSixDofConstraint();

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

            /// @copydoc SixDofConstraint::GetLinearDistance() const
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

            ////////////////////////////////////////////////////////////////////////////////
            // Basic Limit Accessors

            /// @copydoc SixDofConstraint::SetLimit(Whole, Real, Real)
            virtual void SetLimit(Whole Axis, Real Lower, Real Upper);

            /// @copydoc SixDofConstraint::SetLinearLimitLower(const Vector3&)
            virtual void SetLinearLimitLower(const Vector3& Limit);
            /// @copydoc SixDofConstraint::GetLinearLimitLower() const
            virtual Vector3 GetLinearLimitLower() const;
            /// @copydoc SixDofConstraint::SetLinearLimitUpper(const Vector3&)
            virtual void SetLinearLimitUpper(const Vector3& Limit);
            /// @copydoc SixDofConstraint::GetLinearLimitUpper() const
            virtual Vector3 GetLinearLimitUpper() const;

            /// @copydoc SixDofConstraint::SetLinearLimitLowerOnAxis(const Real, Whole)
            void SetLinearLimitLowerOnAxis(const Real Limit, Whole TranslationAxis);
            /// @copydoc SixDofConstraint::GetLinearLimitLowerOnAxis(Whole) const
            Real GetLinearLimitLowerOnAxis(Whole TranslationAxis) const;
            /// @copydoc SixDofConstraint::SetLinearLimitUpperOnAxis(const Real, Whole)
            void SetLinearLimitUpperOnAxis(const Real Limit, Whole TranslationAxis);
            /// @copydoc SixDofConstraint::GetLinearLimitUpperOnAxis(Whole) const
            Real GetLinearLimitUpperOnAxis(Whole TranslationAxis) const;

            /// @copydoc SixDofConstraint::SetAngularLimitUpper(const Vector3&)
            virtual void SetAngularLimitUpper(const Vector3& Limit);
            /// @copydoc SixDofConstraint::GetAngularLimitUpper() const
            virtual Vector3 GetAngularLimitUpper() const;
            /// @copydoc SixDofConstraint::SetAngularLimitLower(const Vector3&)
            virtual void SetAngularLimitLower(const Vector3& Limit);
            /// @copydoc SixDofConstraint::GetAngularLimitLower() const
            virtual Vector3 GetAngularLimitLower() const;

            /// @copydoc SixDofConstraint::SetAngularLimitLowerOnAxis(const Real, Whole)
            void SetAngularLimitLowerOnAxis(const Real Limit, Whole RotationAxis);
            /// @copydoc SixDofConstraint::GetAngularLimitLowerOnAxis(Whole) const
            Real GetAngularLimitLowerOnAxis(Whole RotationAxis) const;
            /// @copydoc SixDofConstraint::SetAngularLimitUpperOnAxis(const Real, Whole)
            void SetAngularLimitUpperOnAxis(const Real Limit, Whole RotationAxis);
            /// @copydoc SixDofConstraint::GetAngularLimitUpperOnAxis(Whole) const
            Real GetAngularLimitUpperOnAxis(Whole RotationAxis) const;

            ////////////////////////////////////////////////////////////////////////////////
            // Angular Limit and Motor Details

            /// @copydoc SixDofConstraint::SetAngularMotorTargetVelocity(const Vector3&)
            virtual void SetAngularMotorTargetVelocity(const Vector3& Velocities);
            /// @copydoc SixDofConstraint::GetAngularMotorTargetVelocity() const
            virtual Vector3 GetAngularMotorTargetVelocity() const;
            /// @copydoc SixDofConstraint::SetAngularMotorTargetVelocityOnAxis(const Real, Whole)
            virtual void SetAngularMotorTargetVelocityOnAxis(const Real Velocity, Whole Axis);
            /// @copydoc SixDofConstraint::GetAngularMotorTargetVelocityOnAxis(Whole) const
            virtual Real GetAngularMotorTargetVelocityOnAxis(Whole Axis) const;

            /// @copydoc SixDofConstraint::SetAngularMotorMaxForce(const Vector3&)
            virtual void SetAngularMotorMaxForce(const Vector3& Forces);
            /// @copydoc SixDofConstraint::GetAngularMotorMaxForce() const
            virtual Vector3 GetAngularMotorMaxForce() const;
            /// @copydoc SixDofConstraint::SetAngularMotorMaxForceOnAxis(const Real, Whole)
            virtual void SetAngularMotorMaxForceOnAxis(const Real Force, Whole Axis);
            /// @copydoc SixDofConstraint::GetAngularMotorMaxForceOnAxis(Whole) const
            virtual Real GetAngularMotorMaxForceOnAxis(Whole Axis) const;

            /// @copydoc SixDofConstraint::SetAngularMotorDamping(const Vector3&)
            virtual void SetAngularMotorDamping(const Vector3& Dampings);
            /// @copydoc SixDofConstraint::GetAngularMotorDamping() const
            virtual Vector3 GetAngularMotorDamping() const;
            /// @copydoc SixDofConstraint::SetAngularMotorDampingOnAxis(const Real, Whole)
            virtual void SetAngularMotorDampingOnAxis(const Real Damping, Whole Axis);
            /// @copydoc SixDofConstraint::GetAngularMotorDampingOnAxis(Whole) const
            virtual Real GetAngularMotorDampingOnAxis(Whole Axis) const;

            /// @copydoc SixDofConstraint::SetAngularMotorRestitution(const Vector3&)
            virtual void SetAngularMotorRestitution(const Vector3& Restitutions);
            /// @copydoc SixDofConstraint::GetAngularMotorRestitution() const
            virtual Vector3 GetAngularMotorRestitution() const;
            /// @copydoc SixDofConstraint::SetAngularMotorRestitutionOnAxis(const Real, Whole)
            virtual void SetAngularMotorRestitutionOnAxis(const Real Restitution, Whole Axis);
            /// @copydoc SixDofConstraint::GetAngularMotorRestitutionOnAxis(Whole) const
            virtual Real GetAngularMotorRestitutionOnAxis(Whole Axis) const;

            /// @copydoc SixDofConstraint::SetAngularMotorEnabled(const Vector3&)
            virtual void SetAngularMotorEnabled(const Vector3& Enableds);
            /// @copydoc SixDofConstraint::GetAngularMotorEnabled() const
            virtual Vector3 GetAngularMotorEnabled() const;
            /// @copydoc SixDofConstraint::SetAngularMotorEnabledOnAxis(const Boole, Whole)
            virtual void SetAngularMotorEnabledOnAxis(const Boole Enabled, Whole Axis);
            /// @copydoc SixDofConstraint::GetAngularMotorEnabledOnAxis(Whole) const
            virtual Boole GetAngularMotorEnabledOnAxis(Whole Axis) const;

            ////////////////////////////////////////////////////////////////////////////////
            // Linear Limit and Motor Details

            /// @copydoc SixDofConstraint::SetLinearMotorMaxForce(const Vector3&)
            virtual void SetLinearMotorMaxForce(const Vector3& Forces);
            /// @copydoc SixDofConstraint::SetLinearMotorMaxForceOnAxis(const Real, Whole)
            virtual void SetLinearMotorMaxForceOnAxis(const Real Force, Whole Axis);
            /// @copydoc SixDofConstraint::GetLinearMotorMaxForce() const
            virtual Vector3 GetLinearMotorMaxForce() const;
            /// @copydoc SixDofConstraint::GetLinearMotorMaxForceOnAxis(Whole) const
            virtual Real GetLinearMotorMaxForceOnAxis(Whole Axis) const;

            /// @copydoc SixDofConstraint::SetLinearMotorTargetVelocity(const Vector3&)
            virtual void SetLinearMotorTargetVelocity(const Vector3& Velocities);
            /// @copydoc SixDofConstraint::SetLinearMotorTargetVelocityOnAxis(const Real, Whole)
            virtual void SetLinearMotorTargetVelocityOnAxis(const Real Velocity, Whole Axis);
            /// @copydoc SixDofConstraint::GetLinearMotorTargetVelocity() const
            virtual Vector3 GetLinearMotorTargetVelocity() const;
            /// @copydoc SixDofConstraint::GetLinearMotorTargetVelocityOnAxis(Whole) const
            virtual Real GetLinearMotorTargetVelocityOnAxis(Whole Axis) const;

            /// @copydoc SixDofConstraint::SetLinearMotorEnabled(const Vector3&)
            virtual void SetLinearMotorEnabled(const Vector3& Enableds);
            /// @copydoc SixDofConstraint::SetLinearMotorEnabledOnAxis(const Boole, Whole)
            virtual void SetLinearMotorEnabledOnAxis(const Boole Enabled, Whole Axis);
            /// @copydoc SixDofConstraint::GetLinearMotorEnabled() const
            virtual Vector3 GetLinearMotorEnabled() const;
            /// @copydoc SixDofConstraint::GetLinearMotorEnabledOnAxis(Whole) const
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
        };//GenericSixDofConstraint
    }//Physics
}//Mezzanine

#endif
