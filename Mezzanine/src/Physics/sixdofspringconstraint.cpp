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
#ifndef _physicsgenericsixdofspringconstraint_cpp
#define _physicsgenericsixdofspringconstraint_cpp

#include "Physics/sixdofspringconstraint.h"
#include "Physics/rigidproxy.h"

#include "stringtool.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Physics
    {
        ////////////////////////////////////////////////////////////////////////////////
        // Generic6Dof Spring Constraint Functions

        SixDofSpringConstraint::SixDofSpringConstraint(const UInt32 ID, RigidProxy* Prox1, PhysicsManager* Creator) :
            SixDofConstraint(ID,Prox1,Creator),
            Spring6Dof(nullptr)
            {  }

        SixDofSpringConstraint::SixDofSpringConstraint(const UInt32 ID, RigidProxy* Prox1, RigidProxy* Prox2, PhysicsManager* Creator) :
            SixDofConstraint(ID,Prox1,Prox2,Creator),
            Spring6Dof(nullptr)
            {  }

        SixDofSpringConstraint::SixDofSpringConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB, PhysicsManager* Creator) :
            SixDofConstraint(ID,ProxyA,ProxyB,Creator),
            Spring6Dof(nullptr)
            { this->CreateConstraint(ProxyA,ProxyB,TransA,TransB); }

        SixDofSpringConstraint::SixDofSpringConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator) :
            SixDofConstraint(0,nullptr,Creator),
            Spring6Dof(nullptr)
            { this->ProtoDeSerialize(SelfRoot); }

        SixDofSpringConstraint::~SixDofSpringConstraint()
            { this->DestroyConstraint(); }

        void SixDofSpringConstraint::CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB)
        {
            if( this->Spring6Dof == nullptr ) {
                if( RigidA && RigidB ) {
                    this->Spring6Dof = new btGeneric6DofSpring2Constraint(*(RigidA->_GetPhysicsObject()),*(RigidB->_GetPhysicsObject()),TransA.GetBulletTransform(),TransB.GetBulletTransform());
                }else if( RigidA ) {
                    this->Spring6Dof = new btGeneric6DofSpring2Constraint(*(RigidA->_GetPhysicsObject()),TransA.GetBulletTransform());
                }
            }
        }

        void SixDofSpringConstraint::DestroyConstraint()
        {
            this->EnableConstraint(false);
            if( this->Spring6Dof != nullptr ) {
                delete this->Spring6Dof;
                this->Spring6Dof = nullptr;
            }
            this->ProxA = nullptr;
            this->ProxB = nullptr;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Utility

        void SixDofSpringConstraint::CalculateSpringEquilibriumPoints()
            { this->Spring6Dof->setEquilibriumPoint(); }

        Real SixDofSpringConstraint::GetLinearDistance() const
            { return this->Spring6Dof->getTranslationalLimitMotor()->m_currentLinearDiff.length(); }

        ////////////////////////////////////////////////////////////////////////////////
        // Location and Rotation

        void SixDofSpringConstraint::SetPivotTransforms(const Transform& TransA, const Transform& TransB)
            { this->Spring6Dof->setFrames(TransA.GetBulletTransform(),TransB.GetBulletTransform()); }

        void SixDofSpringConstraint::SetPivotATransform(const Transform& TransA)
            { this->Spring6Dof->setFrames(TransA.GetBulletTransform(),this->Spring6Dof->getFrameOffsetB()); }

        void SixDofSpringConstraint::SetPivotBTransform(const Transform& TransB)
            { this->Spring6Dof->setFrames(this->Spring6Dof->getFrameOffsetA(),TransB.GetBulletTransform()); }

        Transform SixDofSpringConstraint::GetPivotATransform() const
            { return this->Spring6Dof->getFrameOffsetA(); }

        Transform SixDofSpringConstraint::GetPivotBTransform() const
            { return this->Spring6Dof->getFrameOffsetB(); }

        void SixDofSpringConstraint::SetPivotALocation(const Vector3& Location)
            { this->Spring6Dof->getFrameOffsetA().setOrigin(Location.GetBulletVector3()); }

        void SixDofSpringConstraint::SetPivotBLocation(const Vector3& Location)
            { this->Spring6Dof->getFrameOffsetB().setOrigin(Location.GetBulletVector3()); }

        Vector3 SixDofSpringConstraint::GetPivotALocation() const
            { return Vector3(this->Spring6Dof->getFrameOffsetA().getOrigin()); }

        Vector3 SixDofSpringConstraint::GetPivotBLocation() const
            { return Vector3(this->Spring6Dof->getFrameOffsetB().getOrigin()); }

        void SixDofSpringConstraint::SetPivotARotation(const Quaternion& Rotation)
            { this->Spring6Dof->getFrameOffsetA().setRotation(Rotation.GetBulletQuaternion()); }

        void SixDofSpringConstraint::SetPivotBRotation(const Quaternion& Rotation)
            { this->Spring6Dof->getFrameOffsetA().setRotation(Rotation.GetBulletQuaternion()); }

        Quaternion SixDofSpringConstraint::GetPivotARotation() const
            { return Quaternion(this->Spring6Dof->getFrameOffsetA().getRotation()); }

        Quaternion SixDofSpringConstraint::GetPivotBRotation() const
            { return Quaternion(this->Spring6Dof->getFrameOffsetB().getRotation()); }

        ////////////////////////////////////////////////////////////////////////////////
        // Basic Limit Accessors

        void SixDofSpringConstraint::SetLimit(Whole Axis, Real Lower, Real Upper)
            { this->Spring6Dof->setLimit(Axis, Lower, Upper); }

        void SixDofSpringConstraint::SetLinearLimitLower(const Vector3& Limit)
            { this->Spring6Dof->setLinearLowerLimit(Limit.GetBulletVector3()); }

        Vector3 SixDofSpringConstraint::GetLinearLimitLower() const
            { return Vector3(this->Spring6Dof->getTranslationalLimitMotor()->m_lowerLimit); }

        void SixDofSpringConstraint::SetLinearLimitUpper(const Vector3& Limit)
            { this->Spring6Dof->setLinearUpperLimit(Limit.GetBulletVector3()); }

        Vector3 SixDofSpringConstraint::GetLinearLimitUpper() const
            { return Vector3(this->Spring6Dof->getTranslationalLimitMotor()->m_upperLimit); }

        void SixDofSpringConstraint::SetLinearLimitLowerOnAxis(const Real Limit, Whole TranslationAxis)
            { this->Spring6Dof->getTranslationalLimitMotor()->m_lowerLimit[TranslationAxis] = Limit; }

        Real SixDofSpringConstraint::GetLinearLimitLowerOnAxis(Whole TranslationAxis) const
            { return this->Spring6Dof->getTranslationalLimitMotor()->m_lowerLimit[TranslationAxis]; }

        void SixDofSpringConstraint::SetLinearLimitUpperOnAxis(const Real Limit, Whole TranslationAxis)
            { this->Spring6Dof->getTranslationalLimitMotor()->m_upperLimit[TranslationAxis] = Limit; }

        Real SixDofSpringConstraint::GetLinearLimitUpperOnAxis(Whole TranslationAxis) const
            { return this->Spring6Dof->getTranslationalLimitMotor()->m_upperLimit[TranslationAxis]; }

        void SixDofSpringConstraint::SetAngularLimitUpper(const Vector3& Limit)
            { this->Spring6Dof->setAngularUpperLimit(Limit.GetBulletVector3()); }

        Vector3 SixDofSpringConstraint::GetAngularLimitUpper() const
            { return Vector3(this->GetAngularLimitUpperOnAxis(0),this->GetAngularLimitUpperOnAxis(1),this->GetAngularLimitUpperOnAxis(2)); }

        void SixDofSpringConstraint::SetAngularLimitLower(const Vector3& Limit)
            { this->Spring6Dof->setAngularLowerLimit(Limit.GetBulletVector3()); }

        Vector3 SixDofSpringConstraint::GetAngularLimitLower() const
            { return Vector3(this->GetAngularLimitLowerOnAxis(0),this->GetAngularLimitLowerOnAxis(1),this->GetAngularLimitLowerOnAxis(2)); }

        void SixDofSpringConstraint::SetAngularLimitLowerOnAxis(const Real Limit, Whole RotationAxis)
            { this->Spring6Dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(RotationAxis))->m_loLimit = Limit; }

        Real SixDofSpringConstraint::GetAngularLimitLowerOnAxis(Whole RotationAxis) const
            { return this->Spring6Dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(RotationAxis))->m_loLimit; }

        void SixDofSpringConstraint::SetAngularLimitUpperOnAxis(const Real Limit, Whole RotationAxis)
            { this->Spring6Dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(RotationAxis))->m_hiLimit = Limit; }

        Real SixDofSpringConstraint::GetAngularLimitUpperOnAxis(Whole RotationAxis) const
            { return this->Spring6Dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(RotationAxis))->m_hiLimit; }

        ////////////////////////////////////////////////////////////////////////////////
        // Angular Limit and Motor Details

        void SixDofSpringConstraint::SetAngularMotorTargetVelocity(const Vector3& Velocities)
            { this->SetAngularMotorTargetVelocityOnAxis(Velocities.X,AngularXAsRotationAxis);  this->SetAngularMotorTargetVelocityOnAxis(Velocities.Y,AngularYAsRotationAxis);  this->SetAngularMotorTargetVelocityOnAxis(Velocities.Z,AngularZAsRotationAxis); }

        Vector3 SixDofSpringConstraint::GetAngularMotorTargetVelocity() const
            { return Vector3(this->GetAngularMotorTargetVelocityOnAxis(AngularXAsRotationAxis), this->GetAngularMotorTargetVelocityOnAxis(AngularYAsRotationAxis), this->GetAngularMotorTargetVelocityOnAxis(AngularZAsRotationAxis)); }

        void SixDofSpringConstraint::SetAngularMotorTargetVelocityOnAxis(const Real Velocity, Whole Axis)
            { this->Spring6Dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_targetVelocity = Velocity; }

        Real SixDofSpringConstraint::GetAngularMotorTargetVelocityOnAxis(Whole Axis) const
            { return this->Spring6Dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_targetVelocity; }

        Vector3 SixDofSpringConstraint::GetAngularMotorMaxForce() const
            { return Vector3(this->GetAngularMotorMaxForceOnAxis(AngularXAsRotationAxis), this->GetAngularMotorMaxForceOnAxis(AngularYAsRotationAxis), this->GetAngularMotorMaxForceOnAxis(AngularZAsRotationAxis)); }

        void SixDofSpringConstraint::SetAngularMotorMaxForce(const Vector3& Forces)
            { this->SetAngularMotorMaxForceOnAxis(Forces.X,AngularXAsRotationAxis);  this->SetAngularMotorMaxForceOnAxis(Forces.Y,AngularYAsRotationAxis);  this->SetAngularMotorMaxForceOnAxis(Forces.Z,AngularZAsRotationAxis); }

        void SixDofSpringConstraint::SetAngularMotorMaxForceOnAxis(const Real Force, Whole Axis)
            { this->Spring6Dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_maxMotorForce = Force; }

        Real SixDofSpringConstraint::GetAngularMotorMaxForceOnAxis(Whole Axis) const
            { return this->Spring6Dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_maxMotorForce; }

        void SixDofSpringConstraint::SetAngularMotorDamping(const Vector3& Dampings)
            { this->SetAngularMotorDampingOnAxis(Dampings.X,AngularXAsRotationAxis);  this->SetAngularMotorDampingOnAxis(Dampings.Y,AngularYAsRotationAxis);  this->SetAngularMotorDampingOnAxis(Dampings.Z,AngularZAsRotationAxis); }

        Vector3 SixDofSpringConstraint::GetAngularMotorDamping() const
            { return Vector3(this->GetAngularMotorDampingOnAxis(AngularXAsRotationAxis), this->GetAngularMotorDampingOnAxis(AngularYAsRotationAxis), this->GetAngularMotorDampingOnAxis(AngularZAsRotationAxis)); }

        void SixDofSpringConstraint::SetAngularMotorDampingOnAxis(const Real Damping, Whole Axis)
            { this->Spring6Dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_springDamping = Damping; }

        Real SixDofSpringConstraint::GetAngularMotorDampingOnAxis(Whole Axis) const
            { return this->Spring6Dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_springDamping; }

        void SixDofSpringConstraint::SetAngularMotorRestitution(const Vector3& Restitutions)
            { this->SetAngularMotorRestitutionOnAxis(Restitutions.X,AngularXAsRotationAxis);  this->SetAngularMotorRestitutionOnAxis(Restitutions.Y,AngularYAsRotationAxis);  this->SetAngularMotorRestitutionOnAxis(Restitutions.Z,AngularZAsRotationAxis); }

        Vector3 SixDofSpringConstraint::GetAngularMotorRestitution() const
            { return Vector3(this->GetAngularMotorRestitutionOnAxis(AngularXAsRotationAxis), this->GetAngularMotorRestitutionOnAxis(AngularYAsRotationAxis), this->GetAngularMotorRestitutionOnAxis(AngularZAsRotationAxis)); }

        void SixDofSpringConstraint::SetAngularMotorRestitutionOnAxis(const Real Restitution, Whole Axis)
            { this->Spring6Dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_bounce = Restitution; }

        Real SixDofSpringConstraint::GetAngularMotorRestitutionOnAxis(Whole Axis) const
            { return this->Spring6Dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_bounce; }

        void SixDofSpringConstraint::SetAngularMotorEnabled(const Vector3& Enableds)
            { SetAngularMotorEnabledOnAxis(Enableds.X,AngularXAsRotationAxis);  SetAngularMotorEnabledOnAxis(Enableds.Y,AngularYAsRotationAxis);  SetAngularMotorEnabledOnAxis(Enableds.Z,AngularZAsRotationAxis); }

        Vector3 SixDofSpringConstraint::GetAngularMotorEnabled() const
            { return Vector3(GetAngularMotorEnabledOnAxis(AngularXAsRotationAxis), GetAngularMotorEnabledOnAxis(AngularYAsRotationAxis), GetAngularMotorEnabledOnAxis(AngularZAsRotationAxis)); }

        void SixDofSpringConstraint::SetAngularMotorEnabledOnAxis(const Boole Enabled, Whole Axis)
            { this->Spring6Dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_enableMotor = Enabled; }

        Boole SixDofSpringConstraint::GetAngularMotorEnabledOnAxis(Whole Axis) const
            { return this->Spring6Dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_enableMotor; }

        ////////////////////////////////////////////////////////////////////////////////
        // Linear Limit and Motor Details

        void SixDofSpringConstraint::SetLinearMotorMaxForce(const Vector3& Forces)
            { this->SetLinearMotorMaxForceOnAxis(Forces.X,LinearX);  this->SetLinearMotorMaxForceOnAxis(Forces.Y,LinearY);  this->SetLinearMotorMaxForceOnAxis(Forces.Z,LinearZ); }

        void SixDofSpringConstraint::SetLinearMotorMaxForceOnAxis(const Real Force, Whole Axis)
            { this->Spring6Dof->getTranslationalLimitMotor()->m_maxMotorForce[Axis] = Force; }

        Vector3 SixDofSpringConstraint::GetLinearMotorMaxForce() const
            { return Vector3(this->GetLinearMotorMaxForceOnAxis(LinearX), this->GetLinearMotorMaxForceOnAxis(LinearY), this->GetLinearMotorMaxForceOnAxis(LinearZ)); }

        Real SixDofSpringConstraint::GetLinearMotorMaxForceOnAxis(Whole Axis) const
            { return this->Spring6Dof->getTranslationalLimitMotor()->m_maxMotorForce[Axis]; }

        void SixDofSpringConstraint::SetLinearMotorTargetVelocity(const Vector3& Velocities)
            { this->SetLinearMotorTargetVelocityOnAxis(Velocities.X,LinearX);  this->SetLinearMotorTargetVelocityOnAxis(Velocities.Y,LinearY);  this->SetLinearMotorTargetVelocityOnAxis(Velocities.Z,LinearZ); }

        void SixDofSpringConstraint::SetLinearMotorTargetVelocityOnAxis(const Real Velocity, Whole Axis)
            { this->Spring6Dof->getTranslationalLimitMotor()->m_targetVelocity[Axis] = Velocity; }

        Vector3 SixDofSpringConstraint::GetLinearMotorTargetVelocity() const
            { return Vector3(this->GetLinearMotorTargetVelocityOnAxis(LinearX), this->GetLinearMotorTargetVelocityOnAxis(LinearY), this->GetLinearMotorTargetVelocityOnAxis(LinearZ)); }

        Real SixDofSpringConstraint::GetLinearMotorTargetVelocityOnAxis(Whole Axis) const
            { return this->Spring6Dof->getTranslationalLimitMotor()->m_targetVelocity[Axis]; }

        void SixDofSpringConstraint::SetLinearMotorEnabled(const Vector3& Enableds)
            { this->SetLinearMotorEnabledOnAxis(Enableds.X,LinearX);  this->SetLinearMotorEnabledOnAxis(Enableds.Y,LinearY);  this->SetLinearMotorEnabledOnAxis(Enableds.Z,LinearZ); }

        void SixDofSpringConstraint::SetLinearMotorEnabledOnAxis(const Boole Enabled, Whole Axis)
            { this->Spring6Dof->getTranslationalLimitMotor()->m_enableMotor[Axis] = Enabled; }

        Vector3 SixDofSpringConstraint::GetLinearMotorEnabled() const
            { return Vector3(this->GetLinearMotorEnabledOnAxis(LinearX), this->GetLinearMotorEnabledOnAxis(LinearY), this->GetLinearMotorEnabledOnAxis(LinearZ)); }

        Boole SixDofSpringConstraint::GetLinearMotorEnabledOnAxis(Whole Axis) const
            { return this->Spring6Dof->getTranslationalLimitMotor()->m_enableMotor[Axis]; }

        ////////////////////////////////////////////////////////////////////////////////
        // Linear Spring Settings

        void SixDofSpringConstraint::SetLinearSpringStiffness(const Vector3& Stiffness)
            { this->SetSpringStiffness(LinearX, Stiffness.X); this->SetSpringStiffness(LinearY, Stiffness.Y); this->SetSpringStiffness(LinearZ, Stiffness.Z); }

        Vector3 SixDofSpringConstraint::GetLinearSpringStiffness() const
            { return Vector3(this->GetSpringStiffness(LinearX),this->GetSpringStiffness(LinearY),this->GetSpringStiffness(LinearZ)); }

        void SixDofSpringConstraint::SetLinearSpringStiffnessOnAxis(const Real Stiffness, Whole TranslationAxis)
            { this->Spring6Dof->setStiffness( TranslationAxis, Stiffness ); }

        Real SixDofSpringConstraint::GetLinearSpringStiffnessOnAxis(Whole TranslationAxis) const
            { return this->Spring6Dof->getStiffness( TranslationAxis ); }

        void SixDofSpringConstraint::SetLinearSpringDamping(const Vector3& Damps)
            { this->SetSpringDamping(LinearX, Damps.X); this->SetSpringDamping(LinearY, Damps.Y); this->SetSpringDamping(LinearZ, Damps.Z); }

        Vector3 SixDofSpringConstraint::GetLinearSpringDamping() const
            { return Vector3(this->GetSpringDamping(LinearX),this->GetSpringDamping(LinearY),this->GetSpringDamping(LinearZ)); }

        void SixDofSpringConstraint::SetLinearSpringDampingOnAxis(const Real Damping, Whole TranslationAxis)
            { this->Spring6Dof->setDamping( TranslationAxis, Damping ); }

        Real SixDofSpringConstraint::GetLinearSpringDampingOnAxis(Whole TranslationAxis) const
            { return this->Spring6Dof->getDamping( TranslationAxis ); }

        void SixDofSpringConstraint::SetLinearSpringEnabled(const Vector3& Enableness)
            { this->SetSpringEnabled(LinearX, Enableness.X); this->SetSpringEnabled(LinearY, Enableness.Y); this->SetSpringEnabled(LinearZ, Enableness.Z); }

        Vector3 SixDofSpringConstraint::GetLinearSpringEnabled() const
            { return Vector3(this->GetSpringEnabled(LinearX),this->GetSpringEnabled(LinearY),this->GetSpringEnabled(LinearZ)); }

        void SixDofSpringConstraint::SetLinearSpringEnabledOnAxis(const Boole Enabled, Whole TranslationAxis)
            { this->Spring6Dof->enableSpring( TranslationAxis, Enabled ); }

        Boole SixDofSpringConstraint::GetLinearSpringEnabledOnAxis(Whole TranslationAxis) const
            { return this->Spring6Dof->isSpringEnabled( TranslationAxis ); }

        Vector3 SixDofSpringConstraint::GetLinearSpringEquilibriumPoints() const
            { return Vector3(this->GetSpringEquilibriumPoint(LinearX),this->GetSpringEquilibriumPoint(LinearY),this->GetSpringEquilibriumPoint(LinearZ)); }

        ////////////////////////////////////////////////////////////////////////////////
        // Angular Spring Settings

        void SixDofSpringConstraint::SetAngularSpringStiffness(const Vector3& Stiffness)
            { this->SetSpringStiffness(AngularX, Stiffness.X); this->SetSpringStiffness(AngularY, Stiffness.Y); this->SetSpringStiffness(AngularZ, Stiffness.Z); }

        Vector3 SixDofSpringConstraint::GetAngularSpringStiffness() const
            { return Vector3(this->GetSpringStiffness(AngularX),this->GetSpringStiffness(AngularY),this->GetSpringStiffness(AngularZ)); }

        void SixDofSpringConstraint::SetAngularSpringStiffnessOnAxis(const Real Stiffness, Whole RotationAxis)
            { this->Spring6Dof->setStiffness( ConvertAngularTo6Axis(RotationAxis), Stiffness ); }

        Real SixDofSpringConstraint::GetAngularSpringStiffnessOnAxis(Whole RotationAxis) const
            { return this->Spring6Dof->getRotationalLimitMotor( ConvertAngularTo6Axis(RotationAxis) )->m_springStiffness; }

        void SixDofSpringConstraint::SetAngularSpringDamping(const Vector3& Damps)
            { this->SetSpringDamping(AngularX, Damps.X); this->SetSpringDamping(AngularY, Damps.Y); this->SetSpringDamping(AngularZ, Damps.Z); }

        Vector3 SixDofSpringConstraint::GetAngularSpringDamping() const
            { return Vector3(this->GetSpringDamping(AngularX),this->GetSpringDamping(AngularY),this->GetSpringDamping(AngularZ)); }

        void SixDofSpringConstraint::SetAngularSpringDampingOnAxis(const Real Damping, Whole RotationAxis)
            { this->Spring6Dof->setDamping( ConvertAngularTo6Axis(RotationAxis), Damping ); }

        Real SixDofSpringConstraint::GetAngularSpringDampingOnAxis(Whole RotationAxis) const
            { return this->Spring6Dof->getRotationalLimitMotor( ConvertAngularTo6Axis(RotationAxis) )->m_springDamping; }

        void SixDofSpringConstraint::SetAngularSpringEnabled(const Vector3& Enableness)
            { this->SetSpringEnabled(AngularX, Enableness.X); this->SetSpringEnabled(AngularY, Enableness.Y); this->SetSpringEnabled(AngularZ, Enableness.Z); }

        Vector3 SixDofSpringConstraint::GetAngularSpringEnabled() const
            { return Vector3(this->GetSpringEnabled(AngularX),this->GetSpringEnabled(AngularY),this->GetSpringEnabled(AngularZ)); }

        void SixDofSpringConstraint::SetAngularSpringEnabledOnAxis(const Boole Enabled, Whole RotationAxis)
            { this->Spring6Dof->enableSpring( ConvertAngularTo6Axis(RotationAxis), Enabled ); }

        Boole SixDofSpringConstraint::GetAngularSpringEnabledOnAxis(Whole RotationAxis) const
            { return this->Spring6Dof->getRotationalLimitMotor( ConvertAngularTo6Axis(RotationAxis) )->m_enableMotor; }

        Vector3 SixDofSpringConstraint::GetAngularSpringEquilibriumPoints() const
            { return Vector3(this->GetSpringEquilibriumPoint(AngularX),this->GetSpringEquilibriumPoint(AngularY),this->GetSpringEquilibriumPoint(AngularZ)); }

        ////////////////////////////////////////////////////////////////////////////////
        // Combined Linear and Angular Axis Spring Settings

        void SixDofSpringConstraint::SetSpringStiffness(int Index, Real Stiffness)
            { this->Spring6Dof->setStiffness(Index, Stiffness); }

        Real SixDofSpringConstraint::GetSpringStiffness(int Index) const
            { return this->Spring6Dof->getStiffness(Index); }

        void SixDofSpringConstraint::SetSpringDamping(int Index, Real Damping)
            { this->Spring6Dof->setDamping(Index, Damping); }

        Real SixDofSpringConstraint::GetSpringDamping(int Index) const
            { return this->Spring6Dof->getDamping(Index); }

        void SixDofSpringConstraint::SetSpringEnabled(int Index, Boole Enable)
            { this->Spring6Dof->enableSpring(Index, Enable); }

        Boole SixDofSpringConstraint::GetSpringEnabled(int Index) const
            { return this->Spring6Dof->isSpringEnabled(Index); }

        void SixDofSpringConstraint::CalculateSpringEquilibriumPoint(int Index)
            { this->Spring6Dof->setEquilibriumPoint(Index); }

        void SixDofSpringConstraint::SetSpringEquilibriumPoint(int Index, const Real Point)
            { this->Spring6Dof->setEquilibriumPoint(Index,Point); }

        Real SixDofSpringConstraint::GetSpringEquilibriumPoint(int Index) const
            { return this->Spring6Dof->getEquilibriumPoint(Index); }

        ////////////////////////////////////////////////////////////////////////////////
        // Axis Params

        Constraint::ParamList SixDofSpringConstraint::GetValidParamsOnAxis(int Axis) const
        {
            Constraint::ParamList Results;
            if( 0 <= Axis && 5 >= Axis ) {
                Results.push_back(Con_Stop_ERP);
                Results.push_back(Con_CFM);
                Results.push_back(Con_Stop_CFM);
            }
            return Results;
        }

        Constraint::AxisList SixDofSpringConstraint::GetValidLinearAxes() const
        {
            Constraint::AxisList Results;
            Results.push_back(0);
            Results.push_back(1);
            Results.push_back(2);
            return Results;
        }

        Constraint::AxisList SixDofSpringConstraint::GetValidAngularAxes() const
        {
            Constraint::AxisList Results;
            Results.push_back(3);
            Results.push_back(4);
            Results.push_back(5);
            return Results;
        }

        Boole SixDofSpringConstraint::HasParamBeenSet(ConstraintParam Param, int Axis) const
        {
            // http://bulletphysics.com/Bullet/BulletFull/btSixDofSpringConstraint_8cpp_source.html#l00964
            if( 0 > Axis || 5 < Axis ) {
                return false;
            }
            return ( Con_Stop_ERP==Param && this->Spring6Dof->getFlags() & (BT_6DOF_FLAGS_ERP_STOP << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) ) || //if we are checking the stop_erp AND the stop_erp bit is set for the correct axis
                   ( Con_Stop_CFM==Param && this->Spring6Dof->getFlags() & (BT_6DOF_FLAGS_CFM_STOP << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) ) || //if we are checking the stop_cfm AND the stop_cfm bit is set
                   ( Con_CFM==Param      && this->Spring6Dof->getFlags() & (BT_6DOF_FLAGS_CFM_NORM << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) );   //if we are checking the cfm AND the cfm bit is set
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Serialization

        void SixDofSpringConstraint::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->SixDofSpringConstraint::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( SixDofSpringConstraint::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") ) {
                XML::Node LinearAxesNode = PropertiesNode.AppendChild("LinearAxes");
                for( Whole LinAxis = Physics::LinearStart ; LinAxis < Physics::LinearEnd ; ++LinAxis )
                {
                    XML::Node LinearAxisNode = LinearAxesNode.AppendChild("LinearAxis");
                    if( LinearAxisNode.AppendAttribute("Version").SetValue("1") &&
                        LinearAxisNode.AppendAttribute("Axis").SetValue( LinAxis ) &&
                        LinearAxisNode.AppendAttribute("SpringStiffness").SetValue( this->GetLinearSpringStiffnessOnAxis( LinAxis ) ) &&
                        LinearAxisNode.AppendAttribute("SpringDamping").SetValue( this->GetLinearSpringDampingOnAxis( LinAxis ) ) &&
                        LinearAxisNode.AppendAttribute("SpringEnabled").SetValue( this->GetLinearSpringEnabledOnAxis( LinAxis ) ) )
                    {
                        continue;
                    }else{
                        SerializeError("Create XML Attribute Values",SixDofSpringConstraint::GetSerializableName() + " Linear Axes",true);
                    }
                }

                XML::Node AngularAxesNode = PropertiesNode.AppendChild("AngularAxes");
                for( Whole AngAxis = Physics::AngularStart ; AngAxis < Physics::AngularEnd ; ++AngAxis )
                {
                    XML::Node AngularAxisNode = AngularAxesNode.AppendChild("AngularAxis");
                    if( AngularAxisNode.AppendAttribute("Version").SetValue("1") &&
                        AngularAxisNode.AppendAttribute("Axis").SetValue( AngAxis ) &&
                        AngularAxisNode.AppendAttribute("SpringStiffness").SetValue( this->GetAngularSpringStiffnessOnAxis( AngAxis ) ) &&
                        AngularAxisNode.AppendAttribute("SpringDamping").SetValue( this->GetAngularSpringDampingOnAxis( AngAxis ) ) &&
                        AngularAxisNode.AppendAttribute("SpringEnabled").SetValue( this->GetAngularSpringEnabledOnAxis( AngAxis ) ) )
                    {
                        continue;
                    }else{
                        SerializeError("Create XML Attribute Values",SixDofSpringConstraint::GetSerializableName() + " Angular Axes",true);
                    }
                }

                return;
            }else{
                SerializeError("Create XML Attribute Values",SixDofSpringConstraint::GetSerializableName() + "Properties",true);
            }
        }

        void SixDofSpringConstraint::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->SixDofSpringConstraint::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( SixDofSpringConstraint::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if( PropertiesNode.GetAttribute("Version").AsInt() == 1 ) {
                    XML::Node LinearAxesNode = PropertiesNode.GetChild("LinearAxes");
                    for( XML::NodeIterator LinearAxesNodeIt = LinearAxesNode.begin() ; LinearAxesNodeIt != LinearAxesNode.end() ; ++LinearAxesNodeIt )
                    {
                        if( !(*LinearAxesNodeIt).Empty() ) {
                            if( (*LinearAxesNodeIt).GetAttribute("Version").AsInt() == 1 ) {
                                Integer Axis = -1;

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("Axis");
                                if( !CurrAttrib.Empty() )
                                    Axis = CurrAttrib.AsInt();

                                if( Axis < 0 || Axis > 2 )
                                    continue;

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("SpringStiffness");
                                if( !CurrAttrib.Empty() )
                                    this->SetLinearSpringStiffnessOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("SpringDamping");
                                if( !CurrAttrib.Empty() )
                                    this->SetLinearSpringDampingOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("SpringEnabled");
                                if( !CurrAttrib.Empty() )
                                    this->SetLinearSpringEnabledOnAxis( CurrAttrib.AsBool(), Axis );
                            }
                        }
                    }

                    XML::Node AngularAxesNode = PropertiesNode.GetChild("AngularAxes");
                    for( XML::NodeIterator AngularAxesNodeIt = AngularAxesNode.begin() ; AngularAxesNodeIt != AngularAxesNode.end() ; ++AngularAxesNodeIt )
                    {
                        if( !(*AngularAxesNodeIt).Empty() ) {
                            if( (*AngularAxesNodeIt).GetAttribute("Version").AsInt() == 1 ) {
                                Integer Axis = -1;

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("Axis");
                                if( !CurrAttrib.Empty() )
                                    Axis = CurrAttrib.AsInt();

                                if( Axis < 0 || Axis > 5 )
                                    continue;

                                Axis = this->ConvertFrom6AxisTo3Axis(Axis);

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("SpringStiffness");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularSpringStiffnessOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("SpringDamping");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularSpringDampingOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("SpringEnabled");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularSpringEnabledOnAxis( CurrAttrib.AsBool(), Axis );
                            }
                        }
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( SixDofSpringConstraint::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,SixDofSpringConstraint::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String SixDofSpringConstraint::GetDerivedSerializableName() const
            { return SixDofSpringConstraint::GetSerializableName(); }

        String SixDofSpringConstraint::GetSerializableName()
            { return "SixDofSpringConstraint"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal

        btTypedConstraint* SixDofSpringConstraint::_GetConstraintBase() const
            { return this->Spring6Dof; }
    }//Physics
}//Mezzanine

#endif
