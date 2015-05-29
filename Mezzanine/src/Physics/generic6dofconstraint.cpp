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
#ifndef _physicsgeneric6dofconstraint_cpp
#define _physicsgeneric6dofconstraint_cpp

#include "Physics/generic6dofconstraint.h"
#include "Physics/rigidproxy.h"

#include "stringtool.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Physics
    {
        /// @todo Assess whether or not the array access methods here should be wrapped in bounds checks, as Bullet only does bounds checks for half of them, if that.

        ////////////////////////////////////////////////////////////////////////////////
        // Generic6Dof Constraint Functions

        Generic6DofConstraint::Generic6DofConstraint(const UInt32 ID, RigidProxy* Prox1, PhysicsManager* Creator) :
            DualTransformConstraint(ID,Prox1,Creator)
            {  }

        Generic6DofConstraint::Generic6DofConstraint(const UInt32 ID, RigidProxy* Prox1, RigidProxy* Prox2, PhysicsManager* Creator) :
            DualTransformConstraint(ID,Prox1,Prox2,Creator)
            {  }

        ////////////////////////////////////////////////////////////////////////////////
        // Generic6DofConstraint Construction and Destruction

        Generic6DofConstraint::Generic6DofConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB, PhysicsManager* Creator) :
            DualTransformConstraint(ID,ProxyA,ProxyB,Creator),
            Generic6dof(NULL)
            { this->CreateConstraint(ProxyA,ProxyB,TransA,TransB); }

        Generic6DofConstraint::Generic6DofConstraint(const UInt32 ID, RigidProxy* ProxyB, const Transform& TransB, PhysicsManager* Creator) :
            DualTransformConstraint(ID,ProxyB,Creator),
            Generic6dof(NULL)
            { this->CreateConstraint(ProxyB,NULL,TransB,Transform()); }

        Generic6DofConstraint::Generic6DofConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator) :
            DualTransformConstraint(0,NULL,Creator),
            Generic6dof(NULL)
            { this->ProtoDeSerialize(SelfRoot); }

        Generic6DofConstraint::~Generic6DofConstraint()
            { this->DestroyConstraint(); }

        void Generic6DofConstraint::CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB)
        {
            if( this->Generic6dof == NULL ) {
                if( RigidA && RigidB ) {
                    this->Generic6dof = new btGeneric6DofConstraint(*(RigidA->_GetPhysicsObject()),*(RigidB->_GetPhysicsObject()),TransA.GetBulletTransform(),TransB.GetBulletTransform(),false);
                }else if( RigidA ) {
                    this->Generic6dof = new btGeneric6DofConstraint(*(RigidA->_GetPhysicsObject()),TransA.GetBulletTransform(),true);
                }
            }
        }

        void Generic6DofConstraint::DestroyConstraint()
        {
            this->EnableConstraint(false);
            if( this->Generic6dof != NULL ) {
                delete this->Generic6dof;
                this->Generic6dof = NULL;
            }
            this->ProxA = NULL;
            this->ProxB = NULL;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Generic6DofConstraint Location and Rotation

        void Generic6DofConstraint::SetPivotTransforms(const Transform& TransA, const Transform& TransB)
            { this->Generic6dof->setFrames(TransA.GetBulletTransform(),TransB.GetBulletTransform()); }

        void Generic6DofConstraint::SetPivotATransform(const Transform& TransA)
            { this->Generic6dof->setFrames(TransA.GetBulletTransform(),this->Generic6dof->getFrameOffsetB()); }

        void Generic6DofConstraint::SetPivotBTransform(const Transform& TransB)
            { this->Generic6dof->setFrames(this->Generic6dof->getFrameOffsetA(),TransB.GetBulletTransform()); }

        Transform Generic6DofConstraint::GetPivotATransform() const
            { return this->Generic6dof->getFrameOffsetA(); }

        Transform Generic6DofConstraint::GetPivotBTransform() const
            { return this->Generic6dof->getFrameOffsetB(); }

        void Generic6DofConstraint::SetPivotALocation(const Vector3& Location)
            { this->Generic6dof->getFrameOffsetA().setOrigin(Location.GetBulletVector3()); }

        void Generic6DofConstraint::SetPivotBLocation(const Vector3& Location)
            { this->Generic6dof->getFrameOffsetB().setOrigin(Location.GetBulletVector3()); }

        Vector3 Generic6DofConstraint::GetPivotALocation() const
            { return Vector3(this->Generic6dof->getFrameOffsetA().getOrigin()); }

        Vector3 Generic6DofConstraint::GetPivotBLocation() const
            { return Vector3(this->Generic6dof->getFrameOffsetB().getOrigin()); }

        void Generic6DofConstraint::SetPivotARotation(const Quaternion& Rotation)
            { this->Generic6dof->getFrameOffsetA().setRotation(Rotation.GetBulletQuaternion()); }

        void Generic6DofConstraint::SetPivotBRotation(const Quaternion& Rotation)
            { this->Generic6dof->getFrameOffsetA().setRotation(Rotation.GetBulletQuaternion()); }

        Quaternion Generic6DofConstraint::GetPivotARotation() const
            { return Quaternion(this->Generic6dof->getFrameOffsetA().getRotation()); }

        Quaternion Generic6DofConstraint::GetPivotBRotation() const
            { return Quaternion(this->Generic6dof->getFrameOffsetB().getRotation()); }

        ////////////////////////////////////////////////////////////////////////////////
        // Utility

        void Generic6DofConstraint::SetUseFrameOffset(const Boole FrameOffset)
            { this->Generic6dof->setUseFrameOffset(FrameOffset); }

        Boole Generic6DofConstraint::GetUseFrameOffset() const
            { return this->Generic6dof->getUseFrameOffset(); }

        void Generic6DofConstraint::SetUseLinearReferenceFrameA(const Boole UseRefFrameA)
            { this->Generic6dof->setUseLinearReferenceFrameA(UseRefFrameA); }

        Boole Generic6DofConstraint::GetUseLinearReferenceFrameA() const
            { return this->Generic6dof->getUseLinearReferenceFrameA(); }

        Whole Generic6DofConstraint::ConvertFrom6AxisTo3Axis(const Whole Axis) const
            { return ( Axis % 3 ); }

        Whole Generic6DofConstraint::ConvertAngularTo6Axis(const Whole Axis) const
            { return ( Axis > 2 ? Axis : Axis + 3 ); }

        ////////////////////////////////////////////////////////////////////////////////
        // Basic Limit Accessors

        void Generic6DofConstraint::SetLimit(Whole Axis, Real Lower, Real Upper)
            { this->Generic6dof->setLimit(Axis, Lower, Upper); }

        void Generic6DofConstraint::SetLinearLimitLower(const Vector3& Limit)
            { this->Generic6dof->setLinearLowerLimit(Limit.GetBulletVector3()); }

        Vector3 Generic6DofConstraint::GetLinearLimitLower() const
            { return Vector3(this->Generic6dof->getTranslationalLimitMotor()->m_lowerLimit); }

        void Generic6DofConstraint::SetLinearLimitUpper(const Vector3& Limit)
            { this->Generic6dof->setLinearUpperLimit(Limit.GetBulletVector3()); }

        Vector3 Generic6DofConstraint::GetLinearLimitUpper() const
            { return Vector3(this->Generic6dof->getTranslationalLimitMotor()->m_upperLimit); }

        void Generic6DofConstraint::SetLinearLimitLowerOnAxis(const Real Limit, Whole TranslationAxis)
            { this->Generic6dof->getTranslationalLimitMotor()->m_lowerLimit[TranslationAxis] = Limit; }

        Real Generic6DofConstraint::GetLinearLimitLowerOnAxis(Whole TranslationAxis) const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_lowerLimit[TranslationAxis]; }

        void Generic6DofConstraint::SetLinearLimitUpperOnAxis(const Real Limit, Whole TranslationAxis)
            { this->Generic6dof->getTranslationalLimitMotor()->m_upperLimit[TranslationAxis] = Limit; }

        Real Generic6DofConstraint::GetLinearLimitUpperOnAxis(Whole TranslationAxis) const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_upperLimit[TranslationAxis]; }

        void Generic6DofConstraint::SetAngularLimitUpper(const Vector3& Limit)
            { this->Generic6dof->setAngularUpperLimit(Limit.GetBulletVector3()); }

        Vector3 Generic6DofConstraint::GetAngularLimitUpper() const
            { return Vector3(this->GetAngularLimitUpperOnAxis(0),this->GetAngularLimitUpperOnAxis(1),this->GetAngularLimitUpperOnAxis(2)); }

        void Generic6DofConstraint::SetAngularLimitLower(const Vector3& Limit)
            { this->Generic6dof->setAngularLowerLimit(Limit.GetBulletVector3()); }

        Vector3 Generic6DofConstraint::GetAngularLimitLower() const
            { return Vector3(this->GetAngularLimitLowerOnAxis(0),this->GetAngularLimitLowerOnAxis(1),this->GetAngularLimitLowerOnAxis(2)); }

        void Generic6DofConstraint::SetAngularLimitLowerOnAxis(const Real Limit, Whole RotationAxis)
            { this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(RotationAxis))->m_loLimit = Limit; }

        Real Generic6DofConstraint::GetAngularLimitLowerOnAxis(Whole RotationAxis) const
            { return this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(RotationAxis))->m_loLimit; }

        void Generic6DofConstraint::SetAngularLimitUpperOnAxis(const Real Limit, Whole RotationAxis)
            { this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(RotationAxis))->m_hiLimit = Limit; }

        Real Generic6DofConstraint::GetAngularLimitUpperOnAxis(Whole RotationAxis) const
            { return this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(RotationAxis))->m_hiLimit; }

        ////////////////////////////////////////////////////////////////////////////////
        // Angular Limit and Motor Details

        void Generic6DofConstraint::SetAngularLimitMaxForce(const Vector3& MaxLimitForces)
            { this->SetAngularLimitMaxForceOnAxis(MaxLimitForces.X,AngularXAsRotationAxis);  this->SetAngularLimitMaxForceOnAxis(MaxLimitForces.Y,AngularYAsRotationAxis);  this->SetAngularLimitMaxForceOnAxis(MaxLimitForces.Z,AngularZAsRotationAxis); }

        Vector3 Generic6DofConstraint::GetAngularLimitMaxForce() const
            { return Vector3(this->GetAngularLimitMaxForceOnAxis(AngularXAsRotationAxis), this->GetAngularLimitMaxForceOnAxis(AngularYAsRotationAxis), this->GetAngularLimitMaxForceOnAxis(AngularZAsRotationAxis)); }

        void Generic6DofConstraint::SetAngularLimitMaxForceOnAxis(const Real MaxLimitForce, Whole Axis)
            { this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_maxLimitForce = MaxLimitForce; }

        Real Generic6DofConstraint::GetAngularLimitMaxForceOnAxis(Whole Axis) const
            { return this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_maxLimitForce; }

        void Generic6DofConstraint::SetAngularMotorTargetVelocity(const Vector3& Velocities)
            { this->SetAngularMotorTargetVelocityOnAxis(Velocities.X,AngularXAsRotationAxis);  this->SetAngularMotorTargetVelocityOnAxis(Velocities.Y,AngularYAsRotationAxis);  this->SetAngularMotorTargetVelocityOnAxis(Velocities.Z,AngularZAsRotationAxis); }

        Vector3 Generic6DofConstraint::GetAngularMotorTargetVelocity() const
            { return Vector3(this->GetAngularMotorTargetVelocityOnAxis(AngularXAsRotationAxis), this->GetAngularMotorTargetVelocityOnAxis(AngularYAsRotationAxis), this->GetAngularMotorTargetVelocityOnAxis(AngularZAsRotationAxis)); }

        void Generic6DofConstraint::SetAngularMotorTargetVelocityOnAxis(const Real Velocity, Whole Axis)
            { this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_targetVelocity = Velocity; }

        Real Generic6DofConstraint::GetAngularMotorTargetVelocityOnAxis(Whole Axis) const
            { return this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_targetVelocity; }

        Vector3 Generic6DofConstraint::GetAngularMotorMaxForce() const
            { return Vector3(this->GetAngularMotorMaxForceOnAxis(AngularXAsRotationAxis), this->GetAngularMotorMaxForceOnAxis(AngularYAsRotationAxis), this->GetAngularMotorMaxForceOnAxis(AngularZAsRotationAxis)); }

        void Generic6DofConstraint::SetAngularMotorMaxForce(const Vector3& Forces)
            { this->SetAngularMotorMaxForceOnAxis(Forces.X,AngularXAsRotationAxis);  this->SetAngularMotorMaxForceOnAxis(Forces.Y,AngularYAsRotationAxis);  this->SetAngularMotorMaxForceOnAxis(Forces.Z,AngularZAsRotationAxis); }

        void Generic6DofConstraint::SetAngularMotorMaxForceOnAxis(const Real Force, Whole Axis)
            { this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_maxMotorForce = Force; }

        Real Generic6DofConstraint::GetAngularMotorMaxForceOnAxis(Whole Axis) const
            { return this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_maxMotorForce; }

        void Generic6DofConstraint::SetAngularMotorDamping(const Vector3& Dampings)
            { this->SetAngularMotorDampingOnAxis(Dampings.X,AngularXAsRotationAxis);  this->SetAngularMotorDampingOnAxis(Dampings.Y,AngularYAsRotationAxis);  this->SetAngularMotorDampingOnAxis(Dampings.Z,AngularZAsRotationAxis); }

        Vector3 Generic6DofConstraint::GetAngularMotorDamping() const
            { return Vector3(this->GetAngularMotorDampingOnAxis(AngularXAsRotationAxis), this->GetAngularMotorDampingOnAxis(AngularYAsRotationAxis), this->GetAngularMotorDampingOnAxis(AngularZAsRotationAxis)); }

        void Generic6DofConstraint::SetAngularMotorDampingOnAxis(const Real Damping, Whole Axis)
            { this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_damping = Damping; }

        Real Generic6DofConstraint::GetAngularMotorDampingOnAxis(Whole Axis) const
            { return this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_damping; }

        void Generic6DofConstraint::SetAngularMotorRestitution(const Vector3& Restitutions)
            { this->SetAngularMotorRestitutionOnAxis(Restitutions.X,AngularXAsRotationAxis);  this->SetAngularMotorRestitutionOnAxis(Restitutions.Y,AngularYAsRotationAxis);  this->SetAngularMotorRestitutionOnAxis(Restitutions.Z,AngularZAsRotationAxis); }

        Vector3 Generic6DofConstraint::GetAngularMotorRestitution() const
            { return Vector3(this->GetAngularMotorRestitutionOnAxis(AngularXAsRotationAxis), this->GetAngularMotorRestitutionOnAxis(AngularYAsRotationAxis), this->GetAngularMotorRestitutionOnAxis(AngularZAsRotationAxis)); }

        void Generic6DofConstraint::SetAngularMotorRestitutionOnAxis(const Real Restitution, Whole Axis)
            { this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_bounce = Restitution; }

        Real Generic6DofConstraint::GetAngularMotorRestitutionOnAxis(Whole Axis) const
            { return this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_bounce; }

        void Generic6DofConstraint::SetAngularMotorEnabled(const Vector3& Enableds)
            { SetAngularMotorEnabledOnAxis(Enableds.X,AngularXAsRotationAxis);  SetAngularMotorEnabledOnAxis(Enableds.Y,AngularYAsRotationAxis);  SetAngularMotorEnabledOnAxis(Enableds.Z,AngularZAsRotationAxis); }

        Vector3 Generic6DofConstraint::GetAngularMotorEnabled() const
            { return Vector3(GetAngularMotorEnabledOnAxis(AngularXAsRotationAxis), GetAngularMotorEnabledOnAxis(AngularYAsRotationAxis), GetAngularMotorEnabledOnAxis(AngularZAsRotationAxis)); }

        void Generic6DofConstraint::SetAngularMotorEnabledOnAxis(const Boole Enabled, Whole Axis)
            { this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_enableMotor = Enabled; }

        Boole Generic6DofConstraint::GetAngularMotorEnabledOnAxis(Whole Axis) const
            { return this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_enableMotor; }

        ////////////////////////////////////////////////////////////////////////////////
        // Linear Limit and Motor Details

        void Generic6DofConstraint::SetLinearLimitSoftness(const Real Softness)
            { this->Generic6dof->getTranslationalLimitMotor()->m_limitSoftness = Softness; }

        Real Generic6DofConstraint::GetLinearLimitSoftness() const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_limitSoftness; }

        void Generic6DofConstraint::SetLinearLimitDamping(const Real Damping)
            { this->Generic6dof->getTranslationalLimitMotor()->m_damping = Damping; }

        Real Generic6DofConstraint::GetLinearLimitDamping() const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_damping; }

        void Generic6DofConstraint::SetLinearLimitRestitution(const Real Restitution)
            { this->Generic6dof->getTranslationalLimitMotor()->m_restitution = Restitution; }

        Real Generic6DofConstraint::GetLinearLimitRestitution() const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_restitution; }

        void Generic6DofConstraint::SetLinearMotorMaxForce(const Vector3& Forces)
            { this->SetLinearMotorMaxForceOnAxis(Forces.X,LinearX);  this->SetLinearMotorMaxForceOnAxis(Forces.Y,LinearY);  this->SetLinearMotorMaxForceOnAxis(Forces.Z,LinearZ); }

        void Generic6DofConstraint::SetLinearMotorMaxForceOnAxis(const Real Force, Whole Axis)
            { this->Generic6dof->getTranslationalLimitMotor()->m_maxMotorForce[Axis] = Force; }

        Vector3 Generic6DofConstraint::GetLinearMotorMaxForce() const
            { return Vector3(this->GetLinearMotorMaxForceOnAxis(LinearX), this->GetLinearMotorMaxForceOnAxis(LinearY), this->GetLinearMotorMaxForceOnAxis(LinearZ)); }

        Real Generic6DofConstraint::GetLinearMotorMaxForceOnAxis(Whole Axis) const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_maxMotorForce[Axis]; }

        void Generic6DofConstraint::SetLinearMotorTargetVelocity(const Vector3& Velocities)
            { this->SetLinearMotorTargetVelocityOnAxis(Velocities.X,LinearX);  this->SetLinearMotorTargetVelocityOnAxis(Velocities.Y,LinearY);  this->SetLinearMotorTargetVelocityOnAxis(Velocities.Z,LinearZ); }

        void Generic6DofConstraint::SetLinearMotorTargetVelocityOnAxis(const Real Velocity, Whole Axis)
            { this->Generic6dof->getTranslationalLimitMotor()->m_targetVelocity[Axis] = Velocity; }

        Vector3 Generic6DofConstraint::GetLinearMotorTargetVelocity() const
            { return Vector3(this->GetLinearMotorTargetVelocityOnAxis(LinearX), this->GetLinearMotorTargetVelocityOnAxis(LinearY), this->GetLinearMotorTargetVelocityOnAxis(LinearZ)); }

        Real Generic6DofConstraint::GetLinearMotorTargetVelocityOnAxis(Whole Axis) const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_targetVelocity[Axis]; }

        void Generic6DofConstraint::SetLinearMotorEnabled(const Vector3& Enableds)
            { this->SetLinearMotorEnabledOnAxis(Enableds.X,LinearX);  this->SetLinearMotorEnabledOnAxis(Enableds.Y,LinearY);  this->SetLinearMotorEnabledOnAxis(Enableds.Z,LinearZ); }

        void Generic6DofConstraint::SetLinearMotorEnabledOnAxis(const Boole Enabled, Whole Axis)
            { this->Generic6dof->getTranslationalLimitMotor()->m_enableMotor[Axis] = Enabled; }

        Vector3 Generic6DofConstraint::GetLinearMotorEnabled() const
            { return Vector3(this->GetLinearMotorEnabledOnAxis(LinearX), this->GetLinearMotorEnabledOnAxis(LinearY), this->GetLinearMotorEnabledOnAxis(LinearZ)); }

        Boole Generic6DofConstraint::GetLinearMotorEnabledOnAxis(Whole Axis) const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_enableMotor[Axis]; }

        ////////////////////////////////////////////////////////////////////////////////
        // Axis and Params

        Constraint::ParamList Generic6DofConstraint::GetValidParamsOnAxis(int Axis) const
        {
            Constraint::ParamList Results;
            if( 0 <= Axis && 5 >= Axis ) {
                Results.push_back(Con_Stop_ERP);
                Results.push_back(Con_CFM);
                Results.push_back(Con_Stop_CFM);
            }
            return Results;
        }

        Constraint::AxisList Generic6DofConstraint::GetValidLinearAxes() const
        {
            Constraint::AxisList Results;
            Results.push_back(0);
            Results.push_back(1);
            Results.push_back(2);
            return Results;
        }

        Constraint::AxisList Generic6DofConstraint::GetValidAngularAxes() const
        {
            Constraint::AxisList Results;
            Results.push_back(3);
            Results.push_back(4);
            Results.push_back(5);
            return Results;
        }

        Boole Generic6DofConstraint::HasParamBeenSet(ConstraintParam Param, int Axis) const
        {
            // the logic here should match the logic in the source at http://bulletphysics.com/Bullet/BulletFull/btGeneric6DofConstraint_8cpp_source.html#l00964
            if( 0 > Axis || 5 < Axis )
                { return false; }
            return  ( Con_Stop_ERP==Param && this->Generic6dof->getFlags() & (BT_6DOF_FLAGS_ERP_STOP << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) )   ||  //if we are checking the stop_erp AND the stop_erp bit is set for the correct axis
                    ( Con_Stop_CFM==Param && this->Generic6dof->getFlags() & (BT_6DOF_FLAGS_CFM_STOP << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) )   ||  //if we are checking the stop_cfm AND the stop_cfm bit is set
                    ( Con_CFM==Param      && this->Generic6dof->getFlags() & (BT_6DOF_FLAGS_CFM_NORM << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) )   ;   //if we are checking the cfm AND the cfm bit is set
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void Generic6DofConstraint::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Constraint::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( Generic6DofConstraint::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("UseFrameOffset").SetValue( this->GetUseFrameOffset() ) &&
                PropertiesNode.AppendAttribute("UseLinearReferenceFrameA").SetValue( this->GetUseLinearReferenceFrameA() ) &&
                PropertiesNode.AppendAttribute("LinearLimitSoftness").SetValue( this->GetLinearLimitSoftness() ) &&
                PropertiesNode.AppendAttribute("LinearLimitDamping").SetValue( this->GetLinearLimitDamping() ) &&
                PropertiesNode.AppendAttribute("LinearLimitRestitution").SetValue( this->GetLinearLimitRestitution() ) )
            {
                XML::Node LinearAxesNode = PropertiesNode.AppendChild("LinearAxes");
                for( Integer LinAxis = Generic6DofConstraint::LinearStart ; LinAxis < Generic6DofConstraint::LinearEnd ; ++LinAxis )
                {
                    XML::Node LinearAxisNode = LinearAxesNode.AppendChild("LinearAxis");
                    if( LinearAxisNode.AppendAttribute("Version").SetValue("1") &&
                        LinearAxisNode.AppendAttribute("Axis").SetValue( LinAxis ) &&
                        LinearAxisNode.AppendAttribute("LimitLower").SetValue( this->GetLinearLimitLowerOnAxis(LinAxis) ) &&
                        LinearAxisNode.AppendAttribute("LimitUpper").SetValue( this->GetLinearLimitUpperOnAxis(LinAxis) ) &&
                        LinearAxisNode.AppendAttribute("MotorMaxForce").SetValue( this->GetLinearMotorMaxForceOnAxis(LinAxis) ) &&
                        LinearAxisNode.AppendAttribute("MotorTargetVelocity").SetValue( this->GetLinearMotorTargetVelocityOnAxis(LinAxis) ) &&
                        LinearAxisNode.AppendAttribute("MotorEnabled").SetValue( this->GetLinearMotorEnabledOnAxis(LinAxis) ) )
                    {
                        continue;
                    }else{
                        SerializeError("Create XML Attribute Values",Generic6DofConstraint::GetSerializableName() + " Linear Axes",true);
                    }
                }

                XML::Node AngularAxesNode = PropertiesNode.AppendChild("AngularAxes");
                for( Integer AngAxis = Generic6DofConstraint::AngularStart ; AngAxis < Generic6DofConstraint::AngularEnd ; ++AngAxis )
                {
                    XML::Node AngularAxisNode = AngularAxesNode.AppendChild("AngularAxis");
                    if( AngularAxisNode.AppendAttribute("Version").SetValue("1") &&
                        AngularAxisNode.AppendAttribute("Axis").SetValue( ConvertFrom6AxisTo3Axis(AngAxis) ) &&
                        AngularAxisNode.AppendAttribute("LimitLower").SetValue( this->GetAngularLimitLowerOnAxis( ConvertFrom6AxisTo3Axis(AngAxis) ) ) &&
                        AngularAxisNode.AppendAttribute("LimitUpper").SetValue( this->GetAngularLimitUpperOnAxis( ConvertFrom6AxisTo3Axis(AngAxis) ) ) &&
                        AngularAxisNode.AppendAttribute("LimitMaxForce").SetValue( this->GetAngularLimitMaxForceOnAxis( ConvertFrom6AxisTo3Axis(AngAxis) ) ) &&
                        AngularAxisNode.AppendAttribute("MotorTargetVelocity").SetValue( this->GetAngularMotorTargetVelocityOnAxis( ConvertFrom6AxisTo3Axis(AngAxis) ) ) &&
                        AngularAxisNode.AppendAttribute("MotorMaxForce").SetValue( this->GetAngularMotorMaxForceOnAxis( ConvertFrom6AxisTo3Axis(AngAxis) ) ) &&
                        AngularAxisNode.AppendAttribute("MotorDamping").SetValue( this->GetAngularMotorDampingOnAxis( ConvertFrom6AxisTo3Axis(AngAxis) ) ) &&
                        AngularAxisNode.AppendAttribute("MotorRestitution").SetValue( this->GetAngularMotorRestitutionOnAxis( ConvertFrom6AxisTo3Axis(AngAxis) ) ) &&
                        AngularAxisNode.AppendAttribute("MotorEnabled").SetValue( this->GetAngularMotorEnabledOnAxis( ConvertFrom6AxisTo3Axis(AngAxis) ) ) )
                    {
                        continue;
                    }else{
                        SerializeError("Create XML Attribute Values",Generic6DofConstraint::GetSerializableName() + " Angular Axes",true);
                    }
                }

                return;
            }else{
                SerializeError("Create XML Attribute Values",Generic6DofConstraint::GetSerializableName() + "Properties",true);
            }
        }

        void Generic6DofConstraint::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Constraint::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( Generic6DofConstraint::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if( PropertiesNode.GetAttribute("Version").AsInt() == 1 ) {
                    CurrAttrib = PropertiesNode.GetAttribute("UseFrameOffset");
                    if( !CurrAttrib.Empty() )
                        this->SetUseFrameOffset( CurrAttrib.AsBool() );

                    CurrAttrib = PropertiesNode.GetAttribute("UseLinearReferenceFrameA");
                    if( !CurrAttrib.Empty() )
                        this->SetUseLinearReferenceFrameA( CurrAttrib.AsBool() );

                    CurrAttrib = PropertiesNode.GetAttribute("LinearLimitSoftness");
                    if( !CurrAttrib.Empty() )
                        this->SetLinearLimitSoftness( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("LinearLimitDamping");
                    if( !CurrAttrib.Empty() )
                        this->SetLinearLimitDamping( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("LinearLimitRestitution");
                    if( !CurrAttrib.Empty() )
                        this->SetLinearLimitRestitution( CurrAttrib.AsReal() );

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

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("LimitLower");
                                if( !CurrAttrib.Empty() )
                                    this->SetLinearLimitLowerOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("LimitUpper");
                                if( !CurrAttrib.Empty() )
                                    this->SetLinearLimitUpperOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("MotorMaxForce");
                                if( !CurrAttrib.Empty() )
                                    this->SetLinearMotorMaxForceOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("MotorTargetVelocity");
                                if( !CurrAttrib.Empty() )
                                    this->SetLinearMotorTargetVelocityOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("MotorEnabled");
                                if( !CurrAttrib.Empty() )
                                    this->SetLinearMotorEnabledOnAxis( CurrAttrib.AsReal(), Axis );
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

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("LimitLower");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularLimitLowerOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("LimitUpper");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularLimitUpperOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("LimitMaxForce");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularLimitMaxForceOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("MotorTargetVelocity");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularMotorTargetVelocityOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("MotorMaxForce");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularMotorMaxForceOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("MotorDamping");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularMotorDampingOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("MotorRestitution");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularMotorRestitutionOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("MotorEnabled");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularMotorEnabledOnAxis( CurrAttrib.AsReal(), Axis );
                            }
                        }
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( Generic6DofConstraint::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,Generic6DofConstraint::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String Generic6DofConstraint::GetDerivedSerializableName() const
            { return Generic6DofConstraint::GetSerializableName(); }

        String Generic6DofConstraint::GetSerializableName()
            { return "Generic6DofConstraint"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal

        btTypedConstraint* Generic6DofConstraint::_GetConstraintBase() const
            { return this->Generic6dof; }
    }//Physics
}//Mezzanine

#endif
