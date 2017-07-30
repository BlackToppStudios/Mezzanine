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
#ifndef _physicsgenericsixdofconstraint_cpp
#define _physicsgenericsixdofconstraint_cpp

#include "Physics/genericsixdofconstraint.h"
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
        // GenericSixDofConstraint Functions

        GenericSixDofConstraint::GenericSixDofConstraint(const UInt32 ID, RigidProxy* Prox1, PhysicsManager* Creator) :
            SixDofConstraint(ID,Prox1,Creator)
            {  }

        GenericSixDofConstraint::GenericSixDofConstraint(const UInt32 ID, RigidProxy* Prox1, RigidProxy* Prox2, PhysicsManager* Creator) :
            SixDofConstraint(ID,Prox1,Prox2,Creator)
            {  }

        ////////////////////////////////////////////////////////////////////////////////
        // GenericSixDofConstraint Construction and Destruction

        GenericSixDofConstraint::GenericSixDofConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB, PhysicsManager* Creator) :
            SixDofConstraint(ID,ProxyA,ProxyB,Creator),
            Generic6dof(nullptr)
            { this->CreateConstraint(ProxyA,ProxyB,TransA,TransB); }

        GenericSixDofConstraint::GenericSixDofConstraint(const UInt32 ID, RigidProxy* ProxyB, const Transform& TransB, PhysicsManager* Creator) :
            SixDofConstraint(ID,ProxyB,Creator),
            Generic6dof(nullptr)
            { this->CreateConstraint(ProxyB,nullptr,TransB,Transform()); }

        GenericSixDofConstraint::GenericSixDofConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator) :
            SixDofConstraint(0,nullptr,Creator),
            Generic6dof(nullptr)
            { this->ProtoDeSerialize(SelfRoot); }

        GenericSixDofConstraint::~GenericSixDofConstraint()
            { this->DestroyConstraint(); }

        void GenericSixDofConstraint::CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB)
        {
            if( this->Generic6dof == nullptr ) {
                if( RigidA && RigidB ) {
                    this->Generic6dof = new btGeneric6DofConstraint(*(RigidA->_GetPhysicsObject()),*(RigidB->_GetPhysicsObject()),TransA.GetBulletTransform(),TransB.GetBulletTransform(),false);
                }else if( RigidA ) {
                    this->Generic6dof = new btGeneric6DofConstraint(*(RigidA->_GetPhysicsObject()),TransA.GetBulletTransform(),true);
                }
            }
        }

        void GenericSixDofConstraint::DestroyConstraint()
        {
            if( this->Generic6dof != nullptr ) {
                this->EnableConstraint(false);
                delete this->Generic6dof;
                this->Generic6dof = nullptr;
            }
            this->ProxA = nullptr;
            this->ProxB = nullptr;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // GenericSixDofConstraint Location and Rotation

        void GenericSixDofConstraint::SetPivotTransforms(const Transform& TransA, const Transform& TransB)
            { this->Generic6dof->setFrames(TransA.GetBulletTransform(),TransB.GetBulletTransform()); }

        void GenericSixDofConstraint::SetPivotATransform(const Transform& TransA)
            { this->Generic6dof->setFrames(TransA.GetBulletTransform(),this->Generic6dof->getFrameOffsetB()); }

        void GenericSixDofConstraint::SetPivotBTransform(const Transform& TransB)
            { this->Generic6dof->setFrames(this->Generic6dof->getFrameOffsetA(),TransB.GetBulletTransform()); }

        Transform GenericSixDofConstraint::GetPivotATransform() const
            { return this->Generic6dof->getFrameOffsetA(); }

        Transform GenericSixDofConstraint::GetPivotBTransform() const
            { return this->Generic6dof->getFrameOffsetB(); }

        void GenericSixDofConstraint::SetPivotALocation(const Vector3& Location)
            { this->Generic6dof->getFrameOffsetA().setOrigin(Location.GetBulletVector3()); }

        void GenericSixDofConstraint::SetPivotBLocation(const Vector3& Location)
            { this->Generic6dof->getFrameOffsetB().setOrigin(Location.GetBulletVector3()); }

        Vector3 GenericSixDofConstraint::GetPivotALocation() const
            { return Vector3(this->Generic6dof->getFrameOffsetA().getOrigin()); }

        Vector3 GenericSixDofConstraint::GetPivotBLocation() const
            { return Vector3(this->Generic6dof->getFrameOffsetB().getOrigin()); }

        void GenericSixDofConstraint::SetPivotARotation(const Quaternion& Rotation)
            { this->Generic6dof->getFrameOffsetA().setRotation(Rotation.GetBulletQuaternion()); }

        void GenericSixDofConstraint::SetPivotBRotation(const Quaternion& Rotation)
            { this->Generic6dof->getFrameOffsetA().setRotation(Rotation.GetBulletQuaternion()); }

        Quaternion GenericSixDofConstraint::GetPivotARotation() const
            { return Quaternion(this->Generic6dof->getFrameOffsetA().getRotation()); }

        Quaternion GenericSixDofConstraint::GetPivotBRotation() const
            { return Quaternion(this->Generic6dof->getFrameOffsetB().getRotation()); }

        ////////////////////////////////////////////////////////////////////////////////
        // Query

        Real GenericSixDofConstraint::GetLinearDistance() const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_currentLinearDiff.length(); }

        ////////////////////////////////////////////////////////////////////////////////
        // Utility

        void GenericSixDofConstraint::SetUseFrameOffset(const Boole FrameOffset)
            { this->Generic6dof->setUseFrameOffset(FrameOffset); }

        Boole GenericSixDofConstraint::GetUseFrameOffset() const
            { return this->Generic6dof->getUseFrameOffset(); }

        void GenericSixDofConstraint::SetUseLinearReferenceFrameA(const Boole UseRefFrameA)
            { this->Generic6dof->setUseLinearReferenceFrameA(UseRefFrameA); }

        Boole GenericSixDofConstraint::GetUseLinearReferenceFrameA() const
            { return this->Generic6dof->getUseLinearReferenceFrameA(); }

        ////////////////////////////////////////////////////////////////////////////////
        // Basic Limit Accessors

        void GenericSixDofConstraint::SetLimit(Whole Axis, Real Lower, Real Upper)
            { this->Generic6dof->setLimit(Axis, Lower, Upper); }

        void GenericSixDofConstraint::SetLinearLimitLower(const Vector3& Limit)
            { this->Generic6dof->setLinearLowerLimit(Limit.GetBulletVector3()); }

        Vector3 GenericSixDofConstraint::GetLinearLimitLower() const
            { return Vector3(this->Generic6dof->getTranslationalLimitMotor()->m_lowerLimit); }

        void GenericSixDofConstraint::SetLinearLimitUpper(const Vector3& Limit)
            { this->Generic6dof->setLinearUpperLimit(Limit.GetBulletVector3()); }

        Vector3 GenericSixDofConstraint::GetLinearLimitUpper() const
            { return Vector3(this->Generic6dof->getTranslationalLimitMotor()->m_upperLimit); }

        void GenericSixDofConstraint::SetLinearLimitLowerOnAxis(const Real Limit, Whole TranslationAxis)
            { this->Generic6dof->getTranslationalLimitMotor()->m_lowerLimit[TranslationAxis] = Limit; }

        Real GenericSixDofConstraint::GetLinearLimitLowerOnAxis(Whole TranslationAxis) const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_lowerLimit[TranslationAxis]; }

        void GenericSixDofConstraint::SetLinearLimitUpperOnAxis(const Real Limit, Whole TranslationAxis)
            { this->Generic6dof->getTranslationalLimitMotor()->m_upperLimit[TranslationAxis] = Limit; }

        Real GenericSixDofConstraint::GetLinearLimitUpperOnAxis(Whole TranslationAxis) const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_upperLimit[TranslationAxis]; }

        void GenericSixDofConstraint::SetAngularLimitUpper(const Vector3& Limit)
            { this->Generic6dof->setAngularUpperLimit(Limit.GetBulletVector3()); }

        Vector3 GenericSixDofConstraint::GetAngularLimitUpper() const
            { return Vector3(this->GetAngularLimitUpperOnAxis(0),this->GetAngularLimitUpperOnAxis(1),this->GetAngularLimitUpperOnAxis(2)); }

        void GenericSixDofConstraint::SetAngularLimitLower(const Vector3& Limit)
            { this->Generic6dof->setAngularLowerLimit(Limit.GetBulletVector3()); }

        Vector3 GenericSixDofConstraint::GetAngularLimitLower() const
            { return Vector3(this->GetAngularLimitLowerOnAxis(0),this->GetAngularLimitLowerOnAxis(1),this->GetAngularLimitLowerOnAxis(2)); }

        void GenericSixDofConstraint::SetAngularLimitLowerOnAxis(const Real Limit, Whole RotationAxis)
            { this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(RotationAxis))->m_loLimit = Limit; }

        Real GenericSixDofConstraint::GetAngularLimitLowerOnAxis(Whole RotationAxis) const
            { return this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(RotationAxis))->m_loLimit; }

        void GenericSixDofConstraint::SetAngularLimitUpperOnAxis(const Real Limit, Whole RotationAxis)
            { this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(RotationAxis))->m_hiLimit = Limit; }

        Real GenericSixDofConstraint::GetAngularLimitUpperOnAxis(Whole RotationAxis) const
            { return this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(RotationAxis))->m_hiLimit; }

        ////////////////////////////////////////////////////////////////////////////////
        // Angular Limit and Motor Details

        void GenericSixDofConstraint::SetAngularMotorTargetVelocity(const Vector3& Velocities)
            { this->SetAngularMotorTargetVelocityOnAxis(Velocities.X,AngularXAsRotationAxis);  this->SetAngularMotorTargetVelocityOnAxis(Velocities.Y,AngularYAsRotationAxis);  this->SetAngularMotorTargetVelocityOnAxis(Velocities.Z,AngularZAsRotationAxis); }

        Vector3 GenericSixDofConstraint::GetAngularMotorTargetVelocity() const
            { return Vector3(this->GetAngularMotorTargetVelocityOnAxis(AngularXAsRotationAxis), this->GetAngularMotorTargetVelocityOnAxis(AngularYAsRotationAxis), this->GetAngularMotorTargetVelocityOnAxis(AngularZAsRotationAxis)); }

        void GenericSixDofConstraint::SetAngularMotorTargetVelocityOnAxis(const Real Velocity, Whole Axis)
            { this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_targetVelocity = Velocity; }

        Real GenericSixDofConstraint::GetAngularMotorTargetVelocityOnAxis(Whole Axis) const
            { return this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_targetVelocity; }

        Vector3 GenericSixDofConstraint::GetAngularMotorMaxForce() const
            { return Vector3(this->GetAngularMotorMaxForceOnAxis(AngularXAsRotationAxis), this->GetAngularMotorMaxForceOnAxis(AngularYAsRotationAxis), this->GetAngularMotorMaxForceOnAxis(AngularZAsRotationAxis)); }

        void GenericSixDofConstraint::SetAngularMotorMaxForce(const Vector3& Forces)
            { this->SetAngularMotorMaxForceOnAxis(Forces.X,AngularXAsRotationAxis);  this->SetAngularMotorMaxForceOnAxis(Forces.Y,AngularYAsRotationAxis);  this->SetAngularMotorMaxForceOnAxis(Forces.Z,AngularZAsRotationAxis); }

        void GenericSixDofConstraint::SetAngularMotorMaxForceOnAxis(const Real Force, Whole Axis)
            { this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_maxMotorForce = Force; }

        Real GenericSixDofConstraint::GetAngularMotorMaxForceOnAxis(Whole Axis) const
            { return this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_maxMotorForce; }

        void GenericSixDofConstraint::SetAngularMotorDamping(const Vector3& Dampings)
            { this->SetAngularMotorDampingOnAxis(Dampings.X,AngularXAsRotationAxis);  this->SetAngularMotorDampingOnAxis(Dampings.Y,AngularYAsRotationAxis);  this->SetAngularMotorDampingOnAxis(Dampings.Z,AngularZAsRotationAxis); }

        Vector3 GenericSixDofConstraint::GetAngularMotorDamping() const
            { return Vector3(this->GetAngularMotorDampingOnAxis(AngularXAsRotationAxis), this->GetAngularMotorDampingOnAxis(AngularYAsRotationAxis), this->GetAngularMotorDampingOnAxis(AngularZAsRotationAxis)); }

        void GenericSixDofConstraint::SetAngularMotorDampingOnAxis(const Real Damping, Whole Axis)
            { this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_damping = Damping; }

        Real GenericSixDofConstraint::GetAngularMotorDampingOnAxis(Whole Axis) const
            { return this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_damping; }

        void GenericSixDofConstraint::SetAngularMotorRestitution(const Vector3& Restitutions)
            { this->SetAngularMotorRestitutionOnAxis(Restitutions.X,AngularXAsRotationAxis);  this->SetAngularMotorRestitutionOnAxis(Restitutions.Y,AngularYAsRotationAxis);  this->SetAngularMotorRestitutionOnAxis(Restitutions.Z,AngularZAsRotationAxis); }

        Vector3 GenericSixDofConstraint::GetAngularMotorRestitution() const
            { return Vector3(this->GetAngularMotorRestitutionOnAxis(AngularXAsRotationAxis), this->GetAngularMotorRestitutionOnAxis(AngularYAsRotationAxis), this->GetAngularMotorRestitutionOnAxis(AngularZAsRotationAxis)); }

        void GenericSixDofConstraint::SetAngularMotorRestitutionOnAxis(const Real Restitution, Whole Axis)
            { this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_bounce = Restitution; }

        Real GenericSixDofConstraint::GetAngularMotorRestitutionOnAxis(Whole Axis) const
            { return this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_bounce; }

        void GenericSixDofConstraint::SetAngularMotorEnabled(const Vector3& Enableds)
            { SetAngularMotorEnabledOnAxis(Enableds.X,AngularXAsRotationAxis);  SetAngularMotorEnabledOnAxis(Enableds.Y,AngularYAsRotationAxis);  SetAngularMotorEnabledOnAxis(Enableds.Z,AngularZAsRotationAxis); }

        Vector3 GenericSixDofConstraint::GetAngularMotorEnabled() const
            { return Vector3(GetAngularMotorEnabledOnAxis(AngularXAsRotationAxis), GetAngularMotorEnabledOnAxis(AngularYAsRotationAxis), GetAngularMotorEnabledOnAxis(AngularZAsRotationAxis)); }

        void GenericSixDofConstraint::SetAngularMotorEnabledOnAxis(const Boole Enabled, Whole Axis)
            { this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_enableMotor = Enabled; }

        Boole GenericSixDofConstraint::GetAngularMotorEnabledOnAxis(Whole Axis) const
            { return this->Generic6dof->getRotationalLimitMotor(ConvertFrom6AxisTo3Axis(Axis))->m_enableMotor; }

        ////////////////////////////////////////////////////////////////////////////////
        // Linear Limit and Motor Details

        void GenericSixDofConstraint::SetLinearMotorMaxForce(const Vector3& Forces)
            { this->SetLinearMotorMaxForceOnAxis(Forces.X,LinearX);  this->SetLinearMotorMaxForceOnAxis(Forces.Y,LinearY);  this->SetLinearMotorMaxForceOnAxis(Forces.Z,LinearZ); }

        void GenericSixDofConstraint::SetLinearMotorMaxForceOnAxis(const Real Force, Whole Axis)
            { this->Generic6dof->getTranslationalLimitMotor()->m_maxMotorForce[Axis] = Force; }

        Vector3 GenericSixDofConstraint::GetLinearMotorMaxForce() const
            { return Vector3(this->GetLinearMotorMaxForceOnAxis(LinearX), this->GetLinearMotorMaxForceOnAxis(LinearY), this->GetLinearMotorMaxForceOnAxis(LinearZ)); }

        Real GenericSixDofConstraint::GetLinearMotorMaxForceOnAxis(Whole Axis) const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_maxMotorForce[Axis]; }

        void GenericSixDofConstraint::SetLinearMotorTargetVelocity(const Vector3& Velocities)
            { this->SetLinearMotorTargetVelocityOnAxis(Velocities.X,LinearX);  this->SetLinearMotorTargetVelocityOnAxis(Velocities.Y,LinearY);  this->SetLinearMotorTargetVelocityOnAxis(Velocities.Z,LinearZ); }

        void GenericSixDofConstraint::SetLinearMotorTargetVelocityOnAxis(const Real Velocity, Whole Axis)
            { this->Generic6dof->getTranslationalLimitMotor()->m_targetVelocity[Axis] = Velocity; }

        Vector3 GenericSixDofConstraint::GetLinearMotorTargetVelocity() const
            { return Vector3(this->GetLinearMotorTargetVelocityOnAxis(LinearX), this->GetLinearMotorTargetVelocityOnAxis(LinearY), this->GetLinearMotorTargetVelocityOnAxis(LinearZ)); }

        Real GenericSixDofConstraint::GetLinearMotorTargetVelocityOnAxis(Whole Axis) const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_targetVelocity[Axis]; }

        void GenericSixDofConstraint::SetLinearMotorEnabled(const Vector3& Enableds)
            { this->SetLinearMotorEnabledOnAxis(Enableds.X,LinearX);  this->SetLinearMotorEnabledOnAxis(Enableds.Y,LinearY);  this->SetLinearMotorEnabledOnAxis(Enableds.Z,LinearZ); }

        void GenericSixDofConstraint::SetLinearMotorEnabledOnAxis(const Boole Enabled, Whole Axis)
            { this->Generic6dof->getTranslationalLimitMotor()->m_enableMotor[Axis] = Enabled; }

        Vector3 GenericSixDofConstraint::GetLinearMotorEnabled() const
            { return Vector3(this->GetLinearMotorEnabledOnAxis(LinearX), this->GetLinearMotorEnabledOnAxis(LinearY), this->GetLinearMotorEnabledOnAxis(LinearZ)); }

        Boole GenericSixDofConstraint::GetLinearMotorEnabledOnAxis(Whole Axis) const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_enableMotor[Axis]; }

        ////////////////////////////////////////////////////////////////////////////////
        // Axis and Params

        Constraint::ParamList GenericSixDofConstraint::GetValidParamsOnAxis(int Axis) const
        {
            Constraint::ParamList Results;
            if( 0 <= Axis && 5 >= Axis ) {
                Results.push_back(Con_Stop_ERP);
                Results.push_back(Con_CFM);
                Results.push_back(Con_Stop_CFM);
            }
            return Results;
        }

        Constraint::AxisList GenericSixDofConstraint::GetValidLinearAxes() const
        {
            Constraint::AxisList Results;
            Results.push_back(0);
            Results.push_back(1);
            Results.push_back(2);
            return Results;
        }

        Constraint::AxisList GenericSixDofConstraint::GetValidAngularAxes() const
        {
            Constraint::AxisList Results;
            Results.push_back(3);
            Results.push_back(4);
            Results.push_back(5);
            return Results;
        }

        Boole GenericSixDofConstraint::HasParamBeenSet(ConstraintParam Param, int Axis) const
        {
            // http://bulletphysics.com/Bullet/BulletFull/btGeneric6DofConstraint_8cpp_source.html#l00964
            if( 0 > Axis || 5 < Axis )
                { return false; }
            return ( Con_Stop_ERP==Param && this->Generic6dof->getFlags() & (BT_6DOF_FLAGS_ERP_STOP << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) ) || //if we are checking the stop_erp AND the stop_erp bit is set for the correct axis
                   ( Con_Stop_CFM==Param && this->Generic6dof->getFlags() & (BT_6DOF_FLAGS_CFM_STOP << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) ) || //if we are checking the stop_cfm AND the stop_cfm bit is set
                   ( Con_CFM==Param      && this->Generic6dof->getFlags() & (BT_6DOF_FLAGS_CFM_NORM << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) );   //if we are checking the cfm AND the cfm bit is set
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void GenericSixDofConstraint::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->SixDofConstraint::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( GenericSixDofConstraint::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("UseFrameOffset").SetValue( this->GetUseFrameOffset() ) &&
                PropertiesNode.AppendAttribute("UseLinearReferenceFrameA").SetValue( this->GetUseLinearReferenceFrameA() ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",GenericSixDofConstraint::GetSerializableName() + "Properties",true);
            }
        }

        void GenericSixDofConstraint::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->SixDofConstraint::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( GenericSixDofConstraint::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if( PropertiesNode.GetAttribute("Version").AsInt() == 1 ) {
                    CurrAttrib = PropertiesNode.GetAttribute("UseFrameOffset");
                    if( !CurrAttrib.Empty() )
                        this->SetUseFrameOffset( CurrAttrib.AsBool() );

                    CurrAttrib = PropertiesNode.GetAttribute("UseLinearReferenceFrameA");
                    if( !CurrAttrib.Empty() )
                        this->SetUseLinearReferenceFrameA( CurrAttrib.AsBool() );
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( GenericSixDofConstraint::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,GenericSixDofConstraint::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String GenericSixDofConstraint::GetDerivedSerializableName() const
            { return GenericSixDofConstraint::GetSerializableName(); }

        String GenericSixDofConstraint::GetSerializableName()
            { return "GenericSixDofConstraint"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal

        btTypedConstraint* GenericSixDofConstraint::_GetConstraintBase() const
            { return this->Generic6dof; }
    }//Physics
}//Mezzanine

#endif
