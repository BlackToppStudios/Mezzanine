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
#ifndef _physicsgenericsixdofspringconstraint_h
#define _physicsgenericsixdofspringconstraint_h

#include "Physics/sixdofconstraint.h"

class btGeneric6DofSpring2Constraint;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief Creates a constraint as configurable as the 6Dof constraint, but has added support for spring motion.
        ///////////////////////////////////////
        class MEZZ_LIB SixDofSpringConstraint : public SixDofConstraint
        {
        protected:
            /// @brief The internal constraint that this class encapsulates.
            btGeneric6DofSpring2Constraint* Spring6Dof;

            /// @copydoc DualTransformConstraint::CreateConstraint(RigidProxy*, RigidProxy*, const Transform&, const Transform&)
            virtual void CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB);
            /// @copydoc DualTransformConstraint::DestroyConstraint()
            virtual void DestroyConstraint();

            /// @brief Single body inheritance constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param Prox1 A pointer to the first/only proxy that will be constrained.
            /// @param Creator A pointer to the manager that created this constraint.
            SixDofSpringConstraint(const UInt32 ID, RigidProxy* Prox1, PhysicsManager* Creator);
            /// @brief Dual body inheritance constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param Prox1 A pointer to the first proxy that will be constrained.
            /// @param Prox2 A pointer to the second proxy that will be constrained.
            /// @param Creator A pointer to the manager that created this constraint.
            SixDofSpringConstraint(const UInt32 ID, RigidProxy* Prox1, RigidProxy* Prox2, PhysicsManager* Creator);
        public:
            /// @brief Two proxy Terse constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA The First proxy to be bound.
            /// @param ProxyB  The Second proxy to be bound.
            /// @param TransA The offset and rotation from ProxyAs center of gravity to get to match an offset from ProxyB.
            /// @param TransB The offset and rotation from ProxyBs center of gravity.
            /// @param Creator A pointer to the manager that created this constraint.
            SixDofSpringConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB, PhysicsManager* Creator);
            /// @brief XML constructor.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @param Creator A pointer to the manager that created this constraint.
            SixDofSpringConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator);
            /// @brief Class destructor.
            virtual ~SixDofSpringConstraint();

            ////////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Auto-generates the equilibrium points on each axis of this constraint.
            /// @remarks The current offsets of both bodies in constraint space is what is used as
            /// the equilibrium points for each axis.
            virtual void CalculateSpringEquilibriumPoints();
            /// @copydoc SixDofConstraint::GetLinearDistance() const
            virtual Real GetLinearDistance() const;

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
            // Linear Spring Settings

            /// @brief Set the Stiffness of the springs on each Linear Axis.
            /// @param Stiffness A Vector3 containing the X, Y and Z stiffnesses.
            virtual void SetLinearSpringStiffness(const Vector3& Stiffness);
            /// @brief Get the Stiffness for all Linear Axis
            /// @return A Vector3 with the Stiffness on the X, Y and Z Linear Axis.
            virtual Vector3 GetLinearSpringStiffness() const;
            /// @brief Sets the stiffness on a specific linear axis on this constraint.
            /// @param Stiffness The amount of resistance to compressing force the spring should have.
            /// @param TranslationAxis The Axis to work with.
            virtual void SetLinearSpringStiffnessOnAxis(const Real Stiffness, Whole TranslationAxis);
            /// @brief Gets the stiffness on a specific linear axis on this constraint.
            /// @param TranslationAxis The Axis to work with.
            /// @return Returns the currently set stiffness on the specified axis.
            virtual Real GetLinearSpringStiffnessOnAxis(Whole TranslationAxis) const;

            /// @brief Set the Damping of the springs on each Linear Axis.
            /// @param Damps A Vector3 containing the X, Y and Z desired damping.
            virtual void SetLinearSpringDamping(const Vector3& Damps);
            /// @brief Get the Damping for all Linear Axis
            /// @return A Vector3 with the Damping on the X, Y and Z Linear Axis.
            virtual Vector3 GetLinearSpringDamping() const;
            /// @brief Sets the damping on a specific linear axis on this constraint.
            /// @param Damping The amount of damping to apply to all movement on the spring.
            /// @param TranslationAxis The Axis to work with.
            virtual void SetLinearSpringDampingOnAxis(const Real Damping, Whole TranslationAxis);
            /// @brief Gets the damping on a specific linear axis on this constraint.
            /// @param TranslationAxis The Axis to work with.
            /// @return Returns the currently set damping on the specified axis.
            virtual Real GetLinearSpringDampingOnAxis(Whole TranslationAxis) const;

            /// @brief Set whether or not a spring is enabled on a specific linear axis on this constraint.
            /// @param Stiffies A Vector3 containing the X, Y and Z enabled statuses. This is interpreted as 0 for false and any other value for true.
            virtual void SetLinearSpringEnabled(const Vector3& Enabled);
            /// @brief Get the Enabled Status for all Linear Axis
            /// @return A Vector3 with the Enabled Status on the X, Y and Z Linear Axis.
            virtual Vector3 GetLinearSpringEnabled() const;
            /// @brief Set whether or not a spring is enabled on a specific linear axis on this constraint.
            /// @param Enabled The enabled status of the spring on the specified axis.
            /// @param TranslationAxis The Axis to work with.
            virtual void SetLinearSpringEnabledOnAxis(const Boole Enabled, Whole TranslationAxis);
            /// @brief Get whether or not a spring is enabled on a specific linear axis on this constraint.
            /// @param TranslationAxis The Axis to work with.
            /// @return Returns true if the spring on the specified axis is enabled, false otherwise.
            virtual Boole GetLinearSpringEnabledOnAxis(Whole TranslationAxis) const;

            /// @brief Gets the rest point of the spring on each Linear axis.
            /// @return Returns a Vector3 containing the values of the rest position on the respective axis in constraint space.
            virtual Vector3 GetLinearSpringEquilibriumPoints() const;

            ////////////////////////////////////////////////////////////////////////////////
            // Angular Spring Settings

            /// @brief Set the Stiffness of the springs on each Angular Axis.
            /// @param Stiffness A Vector3 containing the X, Y and Z stiffnesses.
            virtual void SetAngularSpringStiffness(const Vector3& Stiffness);
            /// @brief Get the Stiffness for all Angular Axis
            /// @return A Vector3 with the Stiffness on the X, Y and Z Angular Axis.
            virtual Vector3 GetAngularSpringStiffness() const;
            /// @brief Sets the stiffness on a specific angular axis on this constraint.
            /// @param Stiffness The amount of resistance to compressing force the spring should have.
            /// @param RotationAxis The Axis to work with.
            virtual void SetAngularSpringStiffnessOnAxis(const Real Stiffness, Whole RotationAxis);
            /// @brief Gets the stiffness on a specific angular axis on this constraint.
            /// @param RotationAxis The Axis to work with.
            /// @return Returns the currently set stiffness on the specified axis.
            virtual Real GetAngularSpringStiffnessOnAxis(Whole RotationAxis) const;

            /// @brief Set the Damping of the springs on each Angular Axis.
            /// @param Damps A Vector3 containing the X, Y and Z desired damping.
            virtual void SetAngularSpringDamping(const Vector3& Damps);
            /// @brief Get the Damping for all Angular Axis
            /// @return A Vector3 with the Damping on the X, Y and Z Angular Axis.
            virtual Vector3 GetAngularSpringDamping() const;
            /// @brief Sets the damping on a specific angular axis on this constraint.
            /// @param Damping The amount of damping to apply to all movement on the spring.
            /// @param RotationAxis The Axis to work with.
            virtual void SetAngularSpringDampingOnAxis(const Real Damping, Whole RotationAxis);
            /// @brief Gets the damping on a specific angular axis on this constraint.
            /// @param RotationAxis The Axis to work with.
            /// @return Returns the currently set damping on the specified axis.
            virtual Real GetAngularSpringDampingOnAxis(Whole RotationAxis) const;

            /// @brief Set the Stiffness of the springs on each Angular Axis.
            /// @param Enableness A Vector3 containing the X, Y and Z enabled statuses. This is interpreted as 0 for false and any other value for true.
            virtual void SetAngularSpringEnabled(const Vector3& Enableness);
            /// @brief Get the Enabled Status for all Angular Axis
            /// @return A Vector3 with the Enabled Status on the X, Y and Z Angular Axis.
            virtual Vector3 GetAngularSpringEnabled() const;
            /// @brief Set whether or not a spring is enabled on a specific angular axis on this constraint.
            /// @param Enabled The enabled status of the spring on the specified axis.
            /// @param RotationAxis The Axis to work with.
            virtual void SetAngularSpringEnabledOnAxis(const Boole Enabled, Whole RotationAxis);
            /// @brief Get whether or not a spring is enabled on a specific angular axis on this constraint.
            /// @param RotationAxis The Axis to work with.
            /// @return Returns true if the spring on the specified axis is enabled, false otherwise.
            virtual Boole GetAngularSpringEnabledOnAxis(Whole RotationAxis) const;

            /// @brief Gets the rest point of the spring on each Angular axis.
            /// @return Returns a Vector3 containing the values of the rest position on the respective axis in constraint space.
            virtual Vector3 GetAngularSpringEquilibriumPoints() const;

            ////////////////////////////////////////////////////////////////////////////////
            // Combined Linear and Angular Axis Spring Settings

            /// @brief Set the spring stiffness on a given axis
            /// @param Index The Desired axis. This accepts 0,1,2 for Linear X,Y, and Z or 3,4,5 for Angular X,Y, and Z. This can also accept Item from this classes Usable Axis enum;
            /// @param Stiffness A real with the new desired stiffness.
            virtual void SetSpringStiffness(int Index, Real Stiffness);
            /// @brief Retrieve the Stiffness of the spring on the given axis
            /// @param Index The Desired axis. This accepts 0,1,2 for Linear X,Y, and Z or 3,4,5 for Angular X,Y, and Z. This can also accept Item from this classes Usable Axis enum;
            /// @return A real with the requested value;
            virtual Real GetSpringStiffness(int Index) const;

            /// @brief Set the spring Damping on a given axis.
            /// @param Index The Desired axis. This accepts 0,1,2 for Linear X,Y, and Z or 3,4,5 for Angular X,Y, and Z. This can also accept Item from this classes Usable Axis enum;
            /// @param Damping A real with the new desired Damping.
            virtual void SetSpringDamping(int Index, Real Damping);
            /// @brief Retrieve the Damping of the spring on the given axis
            /// @param Index The Desired axis. This accepts 0,1,2 for Linear X,Y, and Z or 3,4,5 for Angular X,Y, and Z. This can also accept Item from this classes Usable Axis enum;
            /// @return A real with the requested value.
            virtual Real GetSpringDamping(int Index) const;

            /// @brief Set the spring's enabled status on a given axis.
            /// @param Index The Desired axis. This accepts 0,1,2 for Linear X,Y, and Z or 3,4,5 for Angular X,Y, and Z. This can also accept Item from this classes Usable Axis enum;
            /// @param Enable A Boole with the spring's enabled status.
            virtual void SetSpringEnabled(int Index, Boole Enable);
            /// @brief Retrieve the EnabledStatus of the spring on the given axis
            /// @param Index The Desired axis. This accepts 0,1,2 for Linear X,Y, and Z or 3,4,5 for Angular X,Y, and Z. This can also accept Item from this classes Usable Axis enum;
            /// @return A Boole with the requested value.
            virtual Boole GetSpringEnabled(int Index) const;

            /// @brief Automatically assigns the rest point of the spring on the specified axis based on the current offset of both constrained bodies.
            /// @param Index The Axis to work with.
            virtual void CalculateSpringEquilibriumPoint(int Index);
            /// @brief Sets the rest point of the spring on the specified axis.
            /// @param Index The Axis to work with.
            /// @param Point The position in constraint space on the specified Axis to set as the rest spot.
            virtual void SetSpringEquilibriumPoint(int Index, const Real Point);
            /// @brief Gets the rest point of the spring on the specified axis.
            /// @param Index The Axis to work with.
            virtual Real GetSpringEquilibriumPoint(int Index) const;

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

            ///////////////////////////////////////////////////////////////////////////////
            // Internal

            /// @copydoc Constraint::_GetConstraintBase() const
            virtual btTypedConstraint* _GetConstraintBase() const;
        };//SixDofSpringConstraint
    }//Physics
}//Mezzanine

#endif
