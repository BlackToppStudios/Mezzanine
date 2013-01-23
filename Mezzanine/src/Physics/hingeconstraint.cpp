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
#ifndef _physicshingeconstraint_cpp
#define _physicshingeconstraint_cpp

#include "hingeconstraint.h"
#include "stringtool.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Physics
    {
        ////////////////////////////////////////////////////////////////////////////////
        // Hinge Constraint Functions
        /////////////////////////////////////////
        btTypedConstraint* HingeConstraint::GetConstraintBase() const
            { return this->Hinge; }

        ////////////////////////////////////////////////////////////////////////////////
        // HingeConstraint Construction and Destruction
        HingeConstraint::HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& PivotInA, const Vector3& PivotInB, const Vector3& AxisInA, const Vector3& AxisInB, bool UseReferenceFrameA)
        {
            SetBodies(ActorA,ActorB);

            btVector3 tempA(AxisInA.GetBulletVector3());
            btVector3 tempB(AxisInB.GetBulletVector3());
            Hinge = new btHingeConstraint(*BodyA, *BodyB, PivotInA.GetBulletVector3(), PivotInB.GetBulletVector3(), tempA, tempB, bool(UseReferenceFrameA));
        }

        HingeConstraint::HingeConstraint(ActorRigid* ActorA, const Vector3& PivotInA, const Vector3& AxisInA, bool UseReferenceFrameA)
        {
            SetBodies(ActorA);

            btVector3 tempA(AxisInA.GetBulletVector3());
            Hinge = new btHingeConstraint(*BodyA, PivotInA.GetBulletVector3(), tempA, bool(UseReferenceFrameA));
        }

        HingeConstraint::HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA, const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseReferenceFrameA)
        {
            SetBodies(ActorA,ActorB);

            btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
            btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
            Hinge = new btHingeConstraint(*BodyA, *BodyB, transa, transb, UseReferenceFrameA);
        }

        HingeConstraint::HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Transform& TransformA, const Transform& TransformB, bool UseReferenceFrameA)
        {
            SetBodies(ActorA,ActorB);
            Hinge = new btHingeConstraint(*BodyA, *BodyB, TransformA.GetBulletTransform(), TransformB.GetBulletTransform(), UseReferenceFrameA);
        }


        HingeConstraint::~HingeConstraint()
        {
            if(Hinge)
                delete Hinge;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // HingeConstraint Position and Orientation
        void HingeConstraint::SetPivotALocation(const Vector3& Location)
            { this->Hinge->getAFrame().setOrigin(Location.GetBulletVector3()); }

        void HingeConstraint::SetPivotBLocation(const Vector3& Location)
            { this->Hinge->getBFrame().setOrigin(Location.GetBulletVector3()); }

        Vector3 HingeConstraint::GetPivotALocation() const
            { return Vector3(this->Hinge->getAFrame().getOrigin()); }

        Vector3 HingeConstraint::GetPivotBLocation() const
            { return Vector3(this->Hinge->getBFrame().getOrigin()); }

        void HingeConstraint::SetAPivotRotation(const Quaternion& Rotation)
            { this->Hinge->getBFrame().setRotation(Rotation.GetBulletQuaternion()); }

        void HingeConstraint::SetBPivotRotation(const Quaternion& Rotation)
            { this->Hinge->getBFrame().setRotation(Rotation.GetBulletQuaternion()); }

        Quaternion HingeConstraint::GetAPivotRotation() const
            { return Quaternion(this->Hinge->getAFrame().getRotation()); }

        Quaternion HingeConstraint::GetBPivotRotation() const
            { return Quaternion(this->Hinge->getBFrame().getRotation()); }

        void HingeConstraint::SetPivotATransform(const Transform& TranA)
            { this->Hinge->getAFrame() << TranA; }

        void HingeConstraint::SetPivotBTransform(const Transform& TranB)
            { this->Hinge->getBFrame() << TranB; }

        Transform HingeConstraint::GetPivotATransform() const
            { return Transform(this->Hinge->getAFrame()); }

        Transform HingeConstraint::GetPivotBTransform() const
            { return Transform(this->Hinge->getBFrame()); }

        ////////////////////////////////////////////////////////////////////////////////
        // HingeConstraint Angular Motor
        void HingeConstraint::EnableMotor(bool EnableMotor, Real TargetVelocity, Real MaxMotorImpulse)
            { this->Hinge->enableAngularMotor(EnableMotor, TargetVelocity, MaxMotorImpulse); }

        void HingeConstraint::EnableMotor(bool EnableMotor)
            { this->Hinge->enableMotor(EnableMotor); }

        bool HingeConstraint::GetMotorEnabled() const
            { return this->Hinge->getEnableAngularMotor(); }

        void HingeConstraint::SetMaxMotorImpulse(Real MaxMotorImpulse)
            { this->Hinge->setMaxMotorImpulse(MaxMotorImpulse); }

        Real HingeConstraint::GetMaxMotorImpulse() const
            { return this->Hinge->getMaxMotorImpulse(); }

        void HingeConstraint::SetMotorTarget(const Quaternion& QuatAInB, Real Dt)
            { this->Hinge->setMotorTarget(QuatAInB.GetBulletQuaternion(), Dt); }

        void HingeConstraint::SetMotorTarget(Real TargetAngle, Real Dt)
            { this->Hinge->setMotorTarget(TargetAngle, Dt); }

        void HingeConstraint::SetMotorTargetVelocity(Real TargetVelocity)
            { return this->SetMotorTargetVelocity(TargetVelocity); }

        Real HingeConstraint::GetMotorTargetVelocity() const
            { return this->Hinge->getMotorTargetVelosity(); }

        ////////////////////////////////////////////////////////////////////////////////
        // HingeConstraint Limits
        void HingeConstraint::SetLimit(Real Low, Real High, Real Softness, Real BiasFactor, Real RelaxationFactor)
            { this->Hinge->setLimit(Low, High, Softness, BiasFactor, RelaxationFactor); }

        Real HingeConstraint::GetLimitLow() const
            { return this->Hinge->getLowerLimit(); }

        Real HingeConstraint::GetLimitHigh() const
            { return this->Hinge->getUpperLimit(); }

        Real HingeConstraint::GetLimitSoftness() const
            { return this->Hinge->m_limitSoftness; }

        Real HingeConstraint::GetLimitBiasFactor() const
            { return this->Hinge->m_biasFactor; }

        Real HingeConstraint::GetLimitRelaxationFactor() const
            { return this->Hinge->m_relaxationFactor; }

        ////////////////////////////////////////////////////////////////////////////////
        // HingeConstraint Details
        void HingeConstraint::SetAxis(const Vector3& AxisInA)
        {
            btVector3 temp(AxisInA.GetBulletVector3());
            this->Hinge->setAxis(temp);
        }

        Real HingeConstraint::GetHingeAngle()
        {
            return this->Hinge->getHingeAngle();
        }

        Constraint::ParamList HingeConstraint::ValidParamOnAxis(int Axis) const
        {
            Constraint::ParamList Results;
            if(-1==Axis||5==Axis)
            {
                Results.push_back(Con_Stop_ERP);
                Results.push_back(Con_CFM);
                Results.push_back(Con_Stop_CFM);
            }
            return Results;
        }

        Constraint::AxisList HingeConstraint::ValidLinearAxis() const
        {
            Constraint::AxisList Results;
            Results.push_back(-1);
            return Results;
        }

        Constraint::AxisList HingeConstraint::ValidAngularAxis() const
        {
            Constraint::AxisList Results;
            Results.push_back(5);
            return Results;
        }

        bool HingeConstraint::HasParamBeenSet(ConstraintParam Param, int Axis) const
        {
            // the logic here should match the logic in the source at http://bulletphysics.com/Bullet/BulletFull/btHingeConstraint_8cpp_source.html#l00962
            if ( -1!=Axis && 5!=Axis )
                { return false; }
            return  ( Con_Stop_ERP==Param && this->Hinge->m_flags & BT_HINGE_FLAGS_ERP_STOP )   ||  //if we are checking the stop_erp AND the stop_erp bit is set
                    ( Con_Stop_CFM==Param && this->Hinge->m_flags & BT_HINGE_FLAGS_CFM_STOP )   ||  //if we are checking the stop_cfm AND the stop_cfm bit is set
                    ( Con_CFM==Param      && this->Hinge->m_flags & BT_HINGE_FLAGS_CFM_NORM )   ;   //if we are checking the cfm AND the cfm bit is set
        }

        bool HingeConstraint::GetUseReferenceFrameA() const
            { return this->Hinge->m_useReferenceFrameA; }

        void HingeConstraint::SetUseReferenceFrameA(bool UseReferenceFrameA)
            { this->Hinge->m_useReferenceFrameA = UseReferenceFrameA; }

        bool HingeConstraint::GetUseFrameOffset() const
            { return this->Hinge->getUseFrameOffset(); }

        void HingeConstraint::SetUseFrameOffset(bool FrameOffset)
            { this->Hinge->setUseFrameOffset(FrameOffset); }

        ////////////////////////////////////////////////////////////////////////////////
        // HingeConstraint Serialization
        void HingeConstraint::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node HingeNode = CurrentRoot.AppendChild(SerializableName());          // The base node all the base constraint stuff will go in
            if (!HingeNode)
                { SerializeError("Create HingeNode", SerializableName()); }

            XML::Attribute VerAttr = HingeNode.AppendAttribute("Version");              // Base Attributes
            XML::Attribute RefA = HingeNode.AppendAttribute("ReferenceInA");
            XML::Attribute FrameOff = HingeNode.AppendAttribute("UseFrameOffset");
            if( VerAttr && RefA && FrameOff )
            {
                VerAttr.SetValue(1);
                RefA.SetValue(this->GetUseReferenceFrameA());
                FrameOff.SetValue(this->GetUseFrameOffset());
            }else{
                SerializeError("Create HingeNode Attributes", SerializableName());
            }

            XML::Node MotorNode = HingeNode.AppendChild("Motor");                       // Motor Node
            if (!MotorNode)
                { SerializeError("Create MotorNode", SerializableName()); }

            XML::Attribute MotEnabled = MotorNode.AppendAttribute("Enabled");           // Motor Attributes
            XML::Attribute MotImpulse = MotorNode.AppendAttribute("MaxImpulse");
            XML::Attribute MotTarget = MotorNode.AppendAttribute("TargetVelocity");
            if( MotEnabled && MotImpulse && MotTarget )
            {
                MotEnabled.SetValue(this->GetMotorEnabled());
                MotImpulse.SetValue(this->GetMaxMotorImpulse());
                MotTarget.SetValue(this->GetMotorTargetVelocity());
            }else{
                SerializeError("Create MotorNode Attributes", SerializableName());
            }

            XML::Node LimitNode =  HingeNode.AppendChild("Limits");                      // Limits Node
            if (!LimitNode)
                { SerializeError("Create LimitNode", SerializableName()); }

            XML::Attribute LimLow = LimitNode.AppendAttribute("Low");                   // Limits Attributes
            XML::Attribute LimHigh = LimitNode.AppendAttribute("High");
            XML::Attribute LimSoft = LimitNode.AppendAttribute("Softness");
            XML::Attribute LimBias = LimitNode.AppendAttribute("BiasFactor");
            XML::Attribute LimRelax = LimitNode.AppendAttribute("RelaxationFactor");
            if( LimLow && LimHigh && LimSoft && LimBias && LimRelax )
            {
                LimLow.SetValue(this->GetLimitLow());
                LimHigh.SetValue(this->GetLimitHigh());
                LimSoft.SetValue(this->GetLimitSoftness());
                LimBias.SetValue(this->GetLimitBiasFactor());
                LimRelax.SetValue(this->GetLimitRelaxationFactor());
            }else{
                SerializeError("Create MotorNode Attributes", SerializableName());
            }

            DualTransformConstraint::ProtoSerialize(HingeNode);
        }

        void HingeConstraint::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->HingeConstraint::SerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    XML::Node DualTranny = OneNode.GetChild("DualTransformConstraint");
                    if(!DualTranny)
                        { DeSerializeError("locate DualTransforn node",SerializableName()); }
                    this->DualTransformConstraint::ProtoDeSerialize(DualTranny);

                    XML::Node MotorNode = OneNode.GetChild("Motor");
                    if(!MotorNode)
                        { DeSerializeError("locate Motor node",SerializableName()); }

                    XML::Node LimitNode = OneNode.GetChild("Limits");
                    if(!LimitNode)
                        { DeSerializeError("locate Limits node",SerializableName()); }

                    this->SetUseReferenceFrameA(OneNode.GetAttribute("ReferenceInA").AsBool());
                    this->SetUseFrameOffset(OneNode.GetAttribute("UseFrameOffset").AsBool());

                    this->EnableMotor(
                        MotorNode.GetAttribute("Enabled").AsBool(),
                        MotorNode.GetAttribute("TargetVelocity").AsReal(),
                        MotorNode.GetAttribute("MaxImpulse").AsReal() );

                    this->SetLimit(
                        LimitNode.GetAttribute("Low").AsReal(),
                        LimitNode.GetAttribute("High").AsReal(),
                        LimitNode.GetAttribute("Softness").AsReal(),
                        LimitNode.GetAttribute("BiasFactor").AsReal(),
                        LimitNode.GetAttribute("RelaxationFactor").AsReal() );
                }else{
                    DeSerializeError("find usable serialization version",SerializableName());
                }
            }else{
                DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
            }
        }

        String HingeConstraint::SerializableName()
            { return String("HingeConstraint"); }

    }//Physics
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::HingeConstraint& x)
{
    Mezzanine::Serialize(stream,x);
    return stream;
}

std::istream& operator >> (std::istream& stream, Mezzanine::Physics::HingeConstraint& x)
    { return Mezzanine::DeSerialize(stream, x); }

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::HingeConstraint& x)
    { x.ProtoDeSerialize(OneNode); }

#endif
