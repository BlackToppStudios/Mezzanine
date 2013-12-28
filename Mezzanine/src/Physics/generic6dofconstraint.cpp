// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
        ////////////////////////////////////////////////////////////////////////////////
        // Generic6Dof Constraint Functions

        Generic6DofConstraint::Generic6DofConstraint()
            { }

        btTypedConstraint* Generic6DofConstraint::GetConstraintBase() const
            { return this->Generic6dof; }

        ////////////////////////////////////////////////////////////////////////////////
        // Generic6DofConstraint Construction and Destruction

        Generic6DofConstraint::Generic6DofConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& VectorA,
                                                      const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseLinearReferenceA)
        {
            this->SetBodies(ProxyA,ProxyB);
            Transform TransformA(VectorA, QuaternionA);
            Transform TransformB(VectorB, QuaternionB);
            this->Generic6dof = new btGeneric6DofConstraint(*(ProxA->_GetPhysicsObject()), *(ProxB->_GetPhysicsObject()), TransformA.GetBulletTransform(), TransformB.GetBulletTransform(), UseLinearReferenceA);
        }

        Generic6DofConstraint::Generic6DofConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransformA, const Transform& TransformB, bool UseLinearReferenceA)
        {
            this->SetBodies(ProxyA,ProxyB);
            this->Generic6dof = new btGeneric6DofConstraint(*(ProxA->_GetPhysicsObject()), *(ProxB->_GetPhysicsObject()), TransformA.GetBulletTransform(), TransformB.GetBulletTransform(), UseLinearReferenceA);
        }

        Generic6DofConstraint::Generic6DofConstraint(RigidProxy* ProxyB, const Vector3& VectorB, const Quaternion& QuaternionB, bool UseLinearReferenceB)
        {
            this->SetBodies(ProxyB);

            btTransform transa(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
            this->Generic6dof = new btGeneric6DofConstraint(*(ProxA->_GetPhysicsObject()), transa, UseLinearReferenceB);
        }

        Generic6DofConstraint::Generic6DofConstraint(RigidProxy* ProxyB, const Transform& TransformB, bool UseLinearReferenceB)
        {
            this->SetBodies(ProxyB);

            this->Generic6dof = new btGeneric6DofConstraint(*(ProxA->_GetPhysicsObject()), TransformB.GetBulletTransform(), UseLinearReferenceB);
        }

        Generic6DofConstraint::~Generic6DofConstraint()
        {
            if(this->Generic6dof)
                delete this->Generic6dof;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Generic6DofConstraint Location and Rotation

        void Generic6DofConstraint::SetPivotATransform(const Transform& TranA)
            { this->Generic6dof->getFrameOffsetA() = TranA.GetBulletTransform(); }

        void Generic6DofConstraint::SetPivotBTransform(const Transform& TranB)
            { this->Generic6dof->getFrameOffsetB() = TranB.GetBulletTransform(); }

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
        // Generic6DofConstraint Basic Limit Accessors

        void Generic6DofConstraint::SetLimit(int Axis, Real Lower, Real Upper)
            { this->Generic6dof->setLimit(Axis, Lower, Upper); }

        void Generic6DofConstraint::SetLinearLimitUpper(const Vector3& Limit)
            { this->Generic6dof->setLinearUpperLimit(Limit.GetBulletVector3()); }

        void Generic6DofConstraint::SetLinearLimitLower(const Vector3& Limit)
            { this->Generic6dof->setLinearLowerLimit(Limit.GetBulletVector3()); }

        Vector3 Generic6DofConstraint::GetLinearLimitUpper() const
            { return Vector3(this->Generic6dof->getTranslationalLimitMotor()->m_upperLimit); }

        Vector3 Generic6DofConstraint::GetLinearLimitLower() const
            { return Vector3(this->Generic6dof->getTranslationalLimitMotor()->m_lowerLimit); }

        void Generic6DofConstraint::SetAngularLimitUpper(const Vector3& Limit)
            { this->Generic6dof->setAngularUpperLimit(Limit.GetBulletVector3()); }

        void Generic6DofConstraint::SetAngularLimitLower(const Vector3& Limit)
            { this->Generic6dof->setAngularLowerLimit(Limit.GetBulletVector3()); }

        Vector3 Generic6DofConstraint::GetAngularLimitUpper() const
            { return Vector3(this->GetAngularLimitUpperOnAxis(0),this->GetAngularLimitUpperOnAxis(1),this->GetAngularLimitUpperOnAxis(2)); }

        Vector3 Generic6DofConstraint::GetAngularLimitLower() const
            { return Vector3(this->GetAngularLimitLowerOnAxis(0),this->GetAngularLimitLowerOnAxis(1),this->GetAngularLimitLowerOnAxis(2)); }

        Real Generic6DofConstraint::GetAngularLimitLowerOnAxis(int RotationalAxis) const
            { return this->Generic6dof->getRotationalLimitMotor(AxisToAngularAxis(RotationalAxis))->m_loLimit; }

        Real Generic6DofConstraint::GetAngularLimitUpperOnAxis(int RotationalAxis) const
            { return this->Generic6dof->getRotationalLimitMotor(AxisToAngularAxis(RotationalAxis))->m_hiLimit; }

        ////////////////////////////////////////////////////////////////////////////////
        // Generic6DofConstraint Angular Limit and Motor Details

        void Generic6DofConstraint::SetAngularLimitMaxForce(const Vector3& MaxLimitForces)
            { this->SetAngularLimitMaxForceOnAxis(MaxLimitForces.X,AngularXAsRotationalAxis);  this->SetAngularLimitMaxForceOnAxis(MaxLimitForces.Y,AngularYAsRotationalAxis);  this->SetAngularLimitMaxForceOnAxis(MaxLimitForces.Z,AngularZAsRotationalAxis); }

        void Generic6DofConstraint::SetAngularLimitMaxForceOnAxis(Real MaxLimitForce, int Axis)
            { this->Generic6dof->getRotationalLimitMotor(AxisToAngularAxis(Axis))->m_maxLimitForce = MaxLimitForce; }

        Vector3 Generic6DofConstraint::GetAngularLimitMaxForce() const
            { return Vector3(this->GetAngularLimitMaxForceOnAxis(AngularXAsRotationalAxis), this->GetAngularLimitMaxForceOnAxis(AngularYAsRotationalAxis), this->GetAngularLimitMaxForceOnAxis(AngularZAsRotationalAxis)); }

        Real Generic6DofConstraint::GetAngularLimitMaxForceOnAxis(int Axis) const
            { return this->Generic6dof->getRotationalLimitMotor(AxisToAngularAxis(Axis))->m_maxLimitForce; }


        void Generic6DofConstraint::SetAngularMotorTargetVelocity(const Vector3& Velocities)
            { this->SetAngularMotorTargetVelocityOnAxis(Velocities.X,AngularXAsRotationalAxis);  this->SetAngularMotorTargetVelocityOnAxis(Velocities.Y,AngularYAsRotationalAxis);  this->SetAngularMotorTargetVelocityOnAxis(Velocities.Z,AngularZAsRotationalAxis); }

        void Generic6DofConstraint::SetAngularMotorTargetVelocityOnAxis(Real Velocity, int Axis)
            { this->Generic6dof->getRotationalLimitMotor(AxisToAngularAxis(Axis))->m_targetVelocity = Velocity; }

        Vector3 Generic6DofConstraint::GetAngularMotorTargetVelocity() const
            { return Vector3(this->GetAngularMotorTargetVelocityOnAxis(AngularXAsRotationalAxis), this->GetAngularMotorTargetVelocityOnAxis(AngularYAsRotationalAxis), this->GetAngularMotorTargetVelocityOnAxis(AngularZAsRotationalAxis)); }

        Real Generic6DofConstraint::GetAngularMotorTargetVelocityOnAxis(int Axis) const
            { return this->Generic6dof->getRotationalLimitMotor(AxisToAngularAxis(Axis))->m_targetVelocity; }


        void Generic6DofConstraint::SetAngularMotorMaxForce(const Vector3& Forces)
            { this->SetAngularMotorMaxForceOnAxis(Forces.X,AngularXAsRotationalAxis);  this->SetAngularMotorMaxForceOnAxis(Forces.Y,AngularYAsRotationalAxis);  this->SetAngularMotorMaxForceOnAxis(Forces.Z,AngularZAsRotationalAxis); }

        void Generic6DofConstraint::SetAngularMotorMaxForceOnAxis(Real Force, int Axis)
            { this->Generic6dof->getRotationalLimitMotor(AxisToAngularAxis(Axis))->m_maxMotorForce = Force; }

        Vector3 Generic6DofConstraint::GetAngularMotorMaxForce() const
            { return Vector3(this->GetAngularMotorMaxForceOnAxis(AngularXAsRotationalAxis), this->GetAngularMotorMaxForceOnAxis(AngularYAsRotationalAxis), this->GetAngularMotorMaxForceOnAxis(AngularZAsRotationalAxis)); }

        Real Generic6DofConstraint::GetAngularMotorMaxForceOnAxis(int Axis) const
            { return this->Generic6dof->getRotationalLimitMotor(AxisToAngularAxis(Axis))->m_maxMotorForce; }


        void Generic6DofConstraint::SetAngularMotorDamping(const Vector3& Dampings)
            { this->SetAngularMotorDampingOnAxis(Dampings.X,AngularXAsRotationalAxis);  this->SetAngularMotorDampingOnAxis(Dampings.Y,AngularYAsRotationalAxis);  this->SetAngularMotorDampingOnAxis(Dampings.Z,AngularZAsRotationalAxis); }

        void Generic6DofConstraint::SetAngularMotorDampingOnAxis(Real Damping, int Axis)
            { this->Generic6dof->getRotationalLimitMotor(AxisToAngularAxis(Axis))->m_damping = Damping; }

        Vector3 Generic6DofConstraint::GetAngularMotorDamping() const
            { return Vector3(this->GetAngularMotorDampingOnAxis(AngularXAsRotationalAxis), this->GetAngularMotorDampingOnAxis(AngularYAsRotationalAxis), this->GetAngularMotorDampingOnAxis(AngularZAsRotationalAxis)); }

        Real Generic6DofConstraint::GetAngularMotorDampingOnAxis(int Axis) const
            { return this->Generic6dof->getRotationalLimitMotor(AxisToAngularAxis(Axis))->m_damping; }


        void Generic6DofConstraint::SetAngularMotorRestitution(const Vector3& Restistutions)
            { SetAngularMotorRestitutionOnAxis(Restistutions.X,AngularXAsRotationalAxis);  SetAngularMotorRestitutionOnAxis(Restistutions.Y,AngularYAsRotationalAxis);  SetAngularMotorRestitutionOnAxis(Restistutions.Z,AngularZAsRotationalAxis); }

        void Generic6DofConstraint::SetAngularMotorRestitutionOnAxis(Real Restistution, int Axis)
            { this->Generic6dof->getRotationalLimitMotor(AxisToAngularAxis(Axis))->m_bounce = Restistution; }

        Vector3 Generic6DofConstraint::GetAngularMotorRestitution() const
            { return Vector3(GetAngularMotorRestitutionOnAxis(AngularXAsRotationalAxis), GetAngularMotorRestitutionOnAxis(AngularYAsRotationalAxis), GetAngularMotorRestitutionOnAxis(AngularZAsRotationalAxis)); }

        Real Generic6DofConstraint::GetAngularMotorRestitutionOnAxis(int Axis) const
            { return this->Generic6dof->getRotationalLimitMotor(AxisToAngularAxis(Axis))->m_bounce; }


        void Generic6DofConstraint::SetAngularMotorEnabled(const Vector3& Enableds)
            { SetAngularMotorEnabledOnAxis(Enableds.X,AngularXAsRotationalAxis);  SetAngularMotorEnabledOnAxis(Enableds.Y,AngularYAsRotationalAxis);  SetAngularMotorEnabledOnAxis(Enableds.Z,AngularZAsRotationalAxis); }

        void Generic6DofConstraint::SetAngularMotorEnabledOnAxis(bool Enabled, int Axis)
            { this->Generic6dof->getRotationalLimitMotor(AxisToAngularAxis(Axis))->m_enableMotor = Enabled; }

        Vector3 Generic6DofConstraint::GetAngularMotorEnabled() const
            { return Vector3(GetAngularMotorEnabledOnAxis(AngularXAsRotationalAxis), GetAngularMotorEnabledOnAxis(AngularYAsRotationalAxis), GetAngularMotorEnabledOnAxis(AngularZAsRotationalAxis)); }

        bool Generic6DofConstraint::GetAngularMotorEnabledOnAxis(int Axis) const
            { return this->Generic6dof->getRotationalLimitMotor(AxisToAngularAxis(Axis))->m_enableMotor; }

        ////////////////////////////////////////////////////////////////////////////////
        // Generic6DofConstraint Linear Limit and Motor Details

        void Generic6DofConstraint::SetLinearLimitSoftness(Real Softness)
            { this->Generic6dof->getTranslationalLimitMotor()->m_limitSoftness = Softness; }

        Real Generic6DofConstraint::GetLinearLimitSoftness() const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_limitSoftness; }

        void Generic6DofConstraint::SetLinearLimitDamping(Real Damping)
            { this->Generic6dof->getTranslationalLimitMotor()->m_damping = Damping; }

        Real Generic6DofConstraint::GetLinearLimitDamping() const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_damping; }

        void Generic6DofConstraint::SetLinearLimitRestitution(Real Restitution)
            { this->Generic6dof->getTranslationalLimitMotor()->m_restitution = Restitution; }

        Real Generic6DofConstraint::GetLinearLimitRestitution() const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_restitution; }


        void Generic6DofConstraint::SetLinearMotorMaxForce(const Vector3& Forces)
            { this->SetLinearMotorMaxForceOnAxis(Forces.X,LinearX);  this->SetLinearMotorMaxForceOnAxis(Forces.Y,LinearY);  this->SetLinearMotorMaxForceOnAxis(Forces.Z,LinearZ); }

        void Generic6DofConstraint::SetLinearMotorMaxForceOnAxis(Real Force, int Axis)
            { this->Generic6dof->getTranslationalLimitMotor()->m_maxMotorForce[Axis] = Force; }

        Vector3 Generic6DofConstraint::GetLinearMotorMaxForce() const
            { return Vector3(this->GetLinearMotorMaxForceOnAxis(LinearX), this->GetLinearMotorMaxForceOnAxis(LinearY), this->GetLinearMotorMaxForceOnAxis(LinearZ)); }

        Real Generic6DofConstraint::GetLinearMotorMaxForceOnAxis(int Axis) const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_maxMotorForce[Axis]; }

        void Generic6DofConstraint::SetLinearMotorTargetVelocity(const Vector3& Velocities)
            { this->SetLinearMotorTargetVelocityOnAxis(Velocities.X,LinearX);  this->SetLinearMotorTargetVelocityOnAxis(Velocities.Y,LinearY);  this->SetLinearMotorTargetVelocityOnAxis(Velocities.Z,LinearZ); }

        void Generic6DofConstraint::SetLinearMotorTargetVelocityOnAxis(Real Velocity, int Axis)
            { this->Generic6dof->getTranslationalLimitMotor()->m_targetVelocity[Axis] = Velocity; }

        Vector3 Generic6DofConstraint::GetLinearMotorTargetVelocity() const
            { return Vector3(this->GetLinearMotorTargetVelocityOnAxis(LinearX), this->GetLinearMotorTargetVelocityOnAxis(LinearY), this->GetLinearMotorTargetVelocityOnAxis(LinearZ)); }

        Real Generic6DofConstraint::GetLinearMotorTargetVelocityOnAxis(int Axis) const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_targetVelocity[Axis]; }


        void Generic6DofConstraint::SetLinearMotorEnabled(const Vector3& Enableds)
            { this->SetLinearMotorEnabledOnAxis(Enableds.X,LinearX);  this->SetLinearMotorEnabledOnAxis(Enableds.Y,LinearY);  this->SetLinearMotorEnabledOnAxis(Enableds.Z,LinearZ); }

        void Generic6DofConstraint::SetLinearMotorEnabledOnAxis(bool Enabled, int Axis)
            { this->Generic6dof->getTranslationalLimitMotor()->m_enableMotor[Axis] = Enabled; }

        Vector3 Generic6DofConstraint::GetLinearMotorEnabled() const
            { return Vector3(this->GetLinearMotorEnabledOnAxis(LinearX), this->GetLinearMotorEnabledOnAxis(LinearY), this->GetLinearMotorEnabledOnAxis(LinearZ)); }

        bool Generic6DofConstraint::GetLinearMotorEnabledOnAxis(int Axis) const
            { return this->Generic6dof->getTranslationalLimitMotor()->m_enableMotor[Axis]; }



        ////////////////////////////////////////////////////////////////////////////////
        // Generic6DofConstraint Axis, Params and other Details

        Constraint::ParamList Generic6DofConstraint::ValidParamOnAxis(int Axis) const
        {
            Constraint::ParamList Results;
            if(0<=Axis && 5>=Axis)
            {
                Results.push_back(Con_Stop_ERP);
                Results.push_back(Con_CFM);
                Results.push_back(Con_Stop_CFM);
            }
            return Results;
        }

        Constraint::AxisList Generic6DofConstraint::ValidLinearAxis() const
        {
            Constraint::AxisList Results;
            Results.push_back(0);
            Results.push_back(1);
            Results.push_back(2);
            return Results;
        }

        Constraint::AxisList Generic6DofConstraint::ValidAngularAxis() const
        {
            Constraint::AxisList Results;
            Results.push_back(3);
            Results.push_back(4);
            Results.push_back(5);
            return Results;
        }

        bool Generic6DofConstraint::HasParamBeenSet(ConstraintParam Param, int Axis) const
        {
            // the logic here should match the logic in the source at http://bulletphysics.com/Bullet/BulletFull/btGeneric6DofConstraint_8cpp_source.html#l00964
            if(0>Axis || 5<Axis)
                { return false; }
            return  ( Con_Stop_ERP==Param && this->Generic6dof->getFlags() & (BT_6DOF_FLAGS_ERP_STOP << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) )   ||  //if we are checking the stop_erp AND the stop_erp bit is set for the correct axis
                    ( Con_Stop_CFM==Param && this->Generic6dof->getFlags() & (BT_6DOF_FLAGS_CFM_STOP << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) )   ||  //if we are checking the stop_cfm AND the stop_cfm bit is set
                    ( Con_CFM==Param      && this->Generic6dof->getFlags() & (BT_6DOF_FLAGS_CFM_NORM << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) )   ;   //if we are checking the cfm AND the cfm bit is set
        }

        bool Generic6DofConstraint::GetUseFrameOffset() const
            { return this->Generic6dof->getUseFrameOffset(); }

        void Generic6DofConstraint::SetUseFrameOffset(bool FrameOffset)
            { this->Generic6dof->setUseFrameOffset(FrameOffset); }

        ///////////////////////////////////////////////////////////////////////////////
        // Generic6DofConstraint Serialization

        void Generic6DofConstraint::ProtoSerialize(XML::Node& CurrentRoot) const
        {

            XML::Node G6dofNode = CurrentRoot.AppendChild(this->Generic6DofConstraint::SerializableName());                     // The base node all the base constraint stuff will go in
            if (!G6dofNode)
                { SerializeError("Create G6dofNode", SerializableName()); }


            XML::Node LinLimUpp = G6dofNode.AppendChild("LinearLimitUpper");                    // Basic Limit Stuff
            if (!LinLimUpp)
                { SerializeError("Create LinLimUpp", SerializableName()); }
            this->GetLinearLimitUpper().ProtoSerialize(LinLimUpp);

            XML::Node LinLimLow = G6dofNode.AppendChild("LinearLimitLower");
            if (!LinLimLow)
                { SerializeError("Create LinLimLow", SerializableName()); }
            this->GetLinearLimitLower().ProtoSerialize(LinLimLow);

            XML::Node AngLimUpp = G6dofNode.AppendChild("AngularLimitUpper");
            if (!AngLimUpp)
                { SerializeError("Create AngLimUpp", SerializableName()); }
            this->GetAngularLimitUpper().ProtoSerialize(AngLimUpp);

            XML::Node AngLimLow = G6dofNode.AppendChild("AngularLimitLower");
            if (!AngLimLow)
                { SerializeError("Create AngLimLow", SerializableName()); }
            this->GetAngularLimitLower().ProtoSerialize(AngLimLow);


            XML::Node AngularLimitMaxForce = G6dofNode.AppendChild("AngularLimitMaxForce");                   // Angular Limit and Motor Details
            if (!AngularLimitMaxForce)
                { SerializeError("Create AngularLimitMaxForce", SerializableName()); }
            this->GetAngularLimitMaxForce().ProtoSerialize(AngularLimitMaxForce);

            XML::Node AngularMotorTargetVelocity = G6dofNode.AppendChild("AngularMotorTargetVelocity");
            if (!AngularMotorTargetVelocity)
                { SerializeError("Create AngularMotorTargetVelocity", SerializableName()); }
            this->GetAngularMotorTargetVelocity().ProtoSerialize(AngularMotorTargetVelocity);

            XML::Node AngularMotorMaxForce = G6dofNode.AppendChild("AngularMotorMaxForce");
            if (!AngularMotorMaxForce)
                { SerializeError("Create AngularMotorMaxForce", SerializableName()); }
            this->GetAngularMotorMaxForce().ProtoSerialize(AngularMotorMaxForce);

            XML::Node AngularMotorDamping = G6dofNode.AppendChild("AngularMotorDamping");
            if (!AngularMotorDamping)
                { SerializeError("Create AngularMotorDamping", SerializableName()); }
            this->GetAngularMotorDamping().ProtoSerialize(AngularMotorDamping);

            XML::Node AngularMotorRestitution = G6dofNode.AppendChild("AngularMotorRestitution");
            if (!AngularMotorRestitution)
                { SerializeError("Create AngularMotorRestitution", SerializableName()); }
            this->GetAngularMotorRestitution().ProtoSerialize(AngularMotorRestitution);

            XML::Node AngularMotorEnabled = G6dofNode.AppendChild("AngularMotorEnabled");
            if (!AngularMotorEnabled)
                { SerializeError("Create AngularMotorEnabled", SerializableName()); }
            this->GetAngularMotorEnabled().ProtoSerialize(AngularMotorEnabled);


            XML::Node LinearMotorMaxForce = G6dofNode.AppendChild("LinearMotorMaxForce");                   // Linear limit and motor details
            if (!LinearMotorMaxForce)
                { SerializeError("Create LinearMotorMaxForce", SerializableName()); }
            this->GetLinearMotorMaxForce().ProtoSerialize(LinearMotorMaxForce);

            XML::Node LinearMotorTargetVelocity = G6dofNode.AppendChild("LinearMotorTargetVelocity");
            if (!LinearMotorTargetVelocity)
                { SerializeError("Create LinearMotorTargetVelocity", SerializableName()); }
            this->GetLinearMotorTargetVelocity().ProtoSerialize(LinearMotorTargetVelocity);

            XML::Node LinearMotorEnabled = G6dofNode.AppendChild("LinearMotorEnabled");
            if (!LinearMotorEnabled)
                { SerializeError("Create LinearMotorEnabled", SerializableName()); }
            this->GetLinearMotorEnabled().ProtoSerialize(LinearMotorEnabled);

            Mezzanine::XML::Attribute Version = G6dofNode.AppendAttribute("Version");                            // Version
            if (!Version)
                { SerializeError("Create Version", SerializableName()); }
            Version.SetValue(1);

            Mezzanine::XML::Attribute LinearLimitSoftness = G6dofNode.AppendAttribute("LinearLimitSoftness");    // Linear Attributes.
            if (!LinearLimitSoftness)
                { SerializeError("Create LinearLimitSoftness", SerializableName()); }
            LinearLimitSoftness.SetValue(this->GetLinearLimitSoftness());

            Mezzanine::XML::Attribute LinearLimitDamping = G6dofNode.AppendAttribute("LinearLimitDamping");
            if (!LinearLimitDamping)
                { SerializeError("Create LinearLimitDamping", SerializableName()); }
            LinearLimitDamping.SetValue(this->GetLinearLimitDamping());

            Mezzanine::XML::Attribute LinearLimitRestitution = G6dofNode.AppendAttribute("LinearLimitRestitution");
            if (!LinearLimitRestitution)
                { SerializeError("Create LinearLimitRestitution", SerializableName()); }
            LinearLimitRestitution.SetValue(this->GetLinearLimitRestitution());

            this->DualTransformConstraint::ProtoSerialize(G6dofNode);
        }

        void Generic6DofConstraint::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->Generic6DofConstraint::SerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    XML::Node DualTranny = OneNode.GetChild("DualTransformConstraint");
                    if(!DualTranny)
                        { DeSerializeError("locate DualTransforn node",SerializableName()); }
                    this->DualTransformConstraint::ProtoDeSerialize(DualTranny);

                    this->SetLinearLimitSoftness(OneNode.GetAttribute("LinearLimitSoftness").AsReal());
                    this->SetLinearLimitDamping(OneNode.GetAttribute("LinearLimitDamping").AsReal());
                    this->SetLinearLimitRestitution(OneNode.GetAttribute("LinearLimitRestitution").AsReal());

                    Vector3 vec;

                    XML::Node LinearLimitUpper = OneNode.GetChild("LinearLimitUpper");
                    if(!LinearLimitUpper || !LinearLimitUpper.GetFirstChild())
                        { DeSerializeError("locate LinearLimitUpper node",SerializableName()); }
                    vec.ProtoDeSerialize(LinearLimitUpper.GetFirstChild());
                    this->SetLinearLimitUpper(vec);

                    XML::Node LinearLimitLower = OneNode.GetChild("LinearLimitLower");
                    if(!LinearLimitLower || !LinearLimitLower.GetFirstChild())
                        { DeSerializeError("locate LinearLimitLower node",SerializableName()); }
                    vec.ProtoDeSerialize(LinearLimitLower.GetFirstChild());
                    this->SetLinearLimitLower(vec);

                    XML::Node AngularLimitUpper = OneNode.GetChild("AngularLimitUpper");
                    if(!AngularLimitUpper || !AngularLimitUpper.GetFirstChild())
                        { DeSerializeError("locate AngularLimitUpper node",SerializableName()); }
                    vec.ProtoDeSerialize(AngularLimitUpper.GetFirstChild());
                    this->SetAngularLimitUpper(vec);

                    XML::Node AngularLimitLower = OneNode.GetChild("AngularLimitLower");
                    if(!AngularLimitLower || !AngularLimitLower.GetFirstChild())
                        { DeSerializeError("locate AngularLimitLower node",SerializableName()); }
                    vec.ProtoDeSerialize(AngularLimitLower.GetFirstChild());
                    this->SetAngularLimitLower(vec);

                    XML::Node AngularLimitMaxForce = OneNode.GetChild("AngularLimitMaxForce");
                    if(!AngularLimitMaxForce || !AngularLimitMaxForce.GetFirstChild())
                        { DeSerializeError("locate AngularLimitMaxForce node",SerializableName()); }
                    vec.ProtoDeSerialize(AngularLimitMaxForce.GetFirstChild());
                    this->SetAngularLimitMaxForce(vec);

                    XML::Node AngularMotorTargetVelocity = OneNode.GetChild("AngularMotorTargetVelocity");
                    if(!AngularMotorTargetVelocity || !AngularMotorTargetVelocity.GetFirstChild())
                        { DeSerializeError("locate AngularMotorTargetVelocity node",SerializableName()); }
                    vec.ProtoDeSerialize(AngularMotorTargetVelocity.GetFirstChild());
                    this->SetAngularMotorTargetVelocity(vec);

                    XML::Node AngularMotorMaxForce = OneNode.GetChild("AngularMotorMaxForce");
                    if(!AngularMotorMaxForce || !AngularMotorMaxForce.GetFirstChild())
                        { DeSerializeError("locate AngularMotorMaxForce node",SerializableName()); }
                    vec.ProtoDeSerialize(AngularMotorMaxForce.GetFirstChild());
                    this->SetAngularMotorMaxForce(vec);

                    XML::Node AngularMotorDamping = OneNode.GetChild("AngularMotorDamping");
                    if(!AngularMotorDamping || !AngularMotorDamping.GetFirstChild())
                        { DeSerializeError("locate AngularMotorDamping node",SerializableName()); }
                    vec.ProtoDeSerialize(AngularMotorDamping.GetFirstChild());
                    this->SetAngularMotorDamping(vec);

                    XML::Node AngularMotorRestitution = OneNode.GetChild("AngularMotorRestitution");
                    if(!AngularMotorRestitution || !AngularMotorRestitution.GetFirstChild())
                        { DeSerializeError("locate AngularMotorRestitution node",SerializableName()); }
                    vec.ProtoDeSerialize(AngularMotorRestitution.GetFirstChild());
                    this->SetAngularMotorRestitution(vec);

                    XML::Node AngularMotorEnabled = OneNode.GetChild("AngularMotorEnabled");
                    if(!AngularMotorEnabled || !AngularMotorEnabled.GetFirstChild())
                        { DeSerializeError("locate AngularMotorEnabled node",SerializableName()); }
                    vec.ProtoDeSerialize(AngularMotorEnabled.GetFirstChild());
                    this->SetAngularMotorEnabled(vec);

                    XML::Node LinearMotorMaxForce = OneNode.GetChild("LinearMotorMaxForce");
                    if(!LinearMotorMaxForce || !LinearMotorMaxForce.GetFirstChild())
                        { DeSerializeError("locate LinearMotorMaxForce node",SerializableName()); }
                    vec.ProtoDeSerialize(LinearMotorMaxForce.GetFirstChild());
                    this->SetLinearMotorMaxForce(vec);

                    XML::Node LinearMotorTargetVelocity = OneNode.GetChild("LinearMotorTargetVelocity");
                    if(!LinearMotorTargetVelocity || !LinearMotorTargetVelocity.GetFirstChild())
                        { DeSerializeError("locate LinearMotorTargetVelocity node",SerializableName()); }
                    vec.ProtoDeSerialize(LinearMotorTargetVelocity.GetFirstChild());
                    this->SetLinearMotorTargetVelocity(vec);

                    XML::Node LinearMotorEnabled = OneNode.GetChild("LinearMotorEnabled");
                    if(!LinearMotorEnabled || !LinearMotorEnabled.GetFirstChild())
                        { DeSerializeError("locate LinearMotorEnabled node",SerializableName()); }
                    vec.ProtoDeSerialize(LinearMotorEnabled.GetFirstChild());
                    this->SetLinearMotorEnabled(vec);
                }else{
                    DeSerializeError("find usable serialization version",SerializableName());
                }
            }else{
                DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
            }
        }

        String Generic6DofConstraint::SerializableName()
            { return String("Generic6DofConstraint"); }
    }//Physics
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::Generic6DofConstraint& x)
{
    Mezzanine::Serialize(stream,x);
    return stream;
}

std::istream& operator >> (std::istream& stream, Mezzanine::Physics::Generic6DofConstraint& x)
    { return Mezzanine::DeSerialize(stream, x); }

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Generic6DofConstraint& x)
    { x.ProtoDeSerialize(OneNode); }

#endif
