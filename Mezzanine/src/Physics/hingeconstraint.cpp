// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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

#include "Physics/hingeconstraint.h"
#include "Physics/rigidproxy.h"

#include "stringtool.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Physics
    {
        ////////////////////////////////////////////////////////////////////////////////
        // Hinge Constraint Functions

        HingeConstraint::HingeConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& PivotInA, const Vector3& PivotInB, const Vector3& AxisInA, const Vector3& AxisInB, PhysicsManager* Creator) :
            DualTransformConstraint(ID,ProxyA,ProxyB,Creator),
            Hinge(NULL)
            { this->CreateConstraint(ProxyA,ProxyB,PivotInA,PivotInB,AxisInA,AxisInB); }

        HingeConstraint::HingeConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB, PhysicsManager* Creator) :
            DualTransformConstraint(ID,ProxyA,ProxyB,Creator),
            Hinge(NULL)
            { this->CreateConstraint(ProxyA,ProxyB,TransA.GetBulletTransform(),TransB.GetBulletTransform()); }

        HingeConstraint::HingeConstraint(const UInt32 ID, RigidProxy* ProxyA, const Vector3& PivotInA, const Vector3& AxisInA, PhysicsManager* Creator) :
            DualTransformConstraint(ID,ProxyA,Creator),
            Hinge(NULL)
            { this->CreateConstraint(ProxyA,NULL,PivotInA,Vector3(),AxisInA,Vector3()); }

        HingeConstraint::HingeConstraint(const UInt32 ID, RigidProxy* ProxyA, const Transform& TransA, PhysicsManager* Creator) :
            DualTransformConstraint(ID,ProxyA,Creator),
            Hinge(NULL)
            { this->CreateConstraint(ProxyA,NULL,TransA.GetBulletTransform(),Transform()); }

        HingeConstraint::HingeConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator) :
            DualTransformConstraint(0,NULL,Creator),
            Hinge(NULL)
            { this->ProtoDeSerialize(SelfRoot); }

        HingeConstraint::~HingeConstraint()
            { this->DestroyConstraint(); }

        void HingeConstraint::CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB)
        {
            if( this->Hinge == NULL ) {
                if( RigidA && RigidB ) {
                    this->Hinge = new btHingeConstraint(*(RigidA->_GetPhysicsObject()),*(RigidB->_GetPhysicsObject()),TransA.GetBulletTransform(),TransB.GetBulletTransform(),false);
                }else if( RigidA ) {
                    this->Hinge = new btHingeConstraint(*(RigidA->_GetPhysicsObject()),TransA.GetBulletTransform(),false);
                }
            }
        }

        void HingeConstraint::CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Vector3& PivotInA, const Vector3& PivotInB, const Vector3& AxisInA, const Vector3& AxisInB)
        {
            if( this->Hinge == NULL ) {
                if( RigidA && RigidB ) {
                    this->Hinge = new btHingeConstraint(*(RigidA->_GetPhysicsObject()),*(RigidB->_GetPhysicsObject()),PivotInA.GetBulletVector3(),PivotInB.GetBulletVector3(),AxisInA.GetBulletVector3(),AxisInB.GetBulletVector3(),false);
                }else if( RigidA ) {
                    this->Hinge = new btHingeConstraint(*(RigidA->_GetPhysicsObject()),PivotInA.GetBulletVector3(),AxisInA.GetBulletVector3(),false);
                }
            }
        }

        void HingeConstraint::DestroyConstraint()
        {
            this->EnableConstraint(false);
            if( this->Hinge != NULL ) {
                delete this->Hinge;
                this->Hinge = NULL;
            }
            this->ProxA = NULL;
            this->ProxB = NULL;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Position and Orientation

        void HingeConstraint::SetPivotTransforms(const Transform& TransA, const Transform& TransB)
            { this->Hinge->setFrames(TransA.GetBulletTransform(),TransB.GetBulletTransform()); }

        void HingeConstraint::SetPivotATransform(const Transform& TransA)
            { this->Hinge->setFrames(TransA.GetBulletTransform(),this->Hinge->getBFrame()); }

        void HingeConstraint::SetPivotBTransform(const Transform& TransB)
            { this->Hinge->setFrames(this->Hinge->getAFrame(),TransB.GetBulletTransform()); }

        Transform HingeConstraint::GetPivotATransform() const
            { return Transform(this->Hinge->getAFrame()); }

        Transform HingeConstraint::GetPivotBTransform() const
            { return Transform(this->Hinge->getBFrame()); }

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

        ////////////////////////////////////////////////////////////////////////////////
        // Utility

        void HingeConstraint::SetAxis(const Vector3& AxisInA)
            { btVector3 Temp = AxisInA.GetBulletVector3();  this->Hinge->setAxis(Temp); }

        Real HingeConstraint::GetHingeAngle()
            { return this->Hinge->getHingeAngle(); }

        void HingeConstraint::SetUseFrameOffset(const Boole FrameOffset)
            { this->Hinge->setUseFrameOffset(FrameOffset); }

        Boole HingeConstraint::GetUseFrameOffset() const
            { return this->Hinge->getUseFrameOffset(); }

        void HingeConstraint::SetUseReferenceFrameA(const Boole UseReferenceFrameA)
            { this->Hinge->setUseReferenceFrameA(UseReferenceFrameA); }

        Boole HingeConstraint::GetUseReferenceFrameA() const
            { return this->Hinge->getUseReferenceFrameA(); }

        ////////////////////////////////////////////////////////////////////////////////
        // Limits

        void HingeConstraint::SetLimits(Real Low, Real High, Real Softness, Real BiasFactor, Real RelaxationFactor)
            { this->Hinge->setLimit(Low, High, Softness, BiasFactor, RelaxationFactor); }

        Real HingeConstraint::GetLimitLow() const
            { return this->Hinge->getLowerLimit(); }

        Real HingeConstraint::GetLimitHigh() const
            { return this->Hinge->getUpperLimit(); }

        Real HingeConstraint::GetLimitSoftness() const
            { return this->Hinge->getLimitSoftness(); }

        Real HingeConstraint::GetLimitBiasFactor() const
            { return this->Hinge->getLimitBiasFactor(); }

        Real HingeConstraint::GetLimitRelaxationFactor() const
            { return this->Hinge->getLimitRelaxationFactor(); }

        ////////////////////////////////////////////////////////////////////////////////
        // Angular Motor

        void HingeConstraint::EnableMotor(const Boole EnableMotor, const Real TargetVelocity, const Real MaxMotorImpulse)
            { this->Hinge->enableAngularMotor(EnableMotor, TargetVelocity, MaxMotorImpulse); }

        void HingeConstraint::SetMotorEnabled(const Boole EnableMotor)
            { this->Hinge->enableMotor(EnableMotor); }

        Boole HingeConstraint::GetMotorEnabled() const
            { return this->Hinge->getEnableAngularMotor(); }

        void HingeConstraint::SetMaxMotorImpulse(const Real MaxMotorImpulse)
            { this->Hinge->setMaxMotorImpulse(MaxMotorImpulse); }

        Real HingeConstraint::GetMaxMotorImpulse() const
            { return this->Hinge->getMaxMotorImpulse(); }

        void HingeConstraint::SetMotorTargetVelocity(const Real TargetVelocity)
            { this->Hinge->setMotorTargetVelocity(TargetVelocity); }

        Real HingeConstraint::GetMotorTargetVelocity() const
            { return this->Hinge->getMotorTargetVelosity(); }

        void HingeConstraint::SetMotorTarget(const Quaternion& QuatAInB, const Real Delta)
            { this->Hinge->setMotorTarget(QuatAInB.GetBulletQuaternion(),Delta); }

        void HingeConstraint::SetMotorTarget(const Real TargetAngle, const Real Delta)
            { this->Hinge->setMotorTarget(TargetAngle,Delta); }

        ////////////////////////////////////////////////////////////////////////////////
        // Parameter Configuration

        Constraint::ParamList HingeConstraint::GetValidParamsOnAxis(int Axis) const
        {
            Constraint::ParamList Results;
            if( -1 == Axis || 5 == Axis ) {
                Results.push_back(Con_Stop_ERP);
                Results.push_back(Con_CFM);
                Results.push_back(Con_Stop_CFM);
            }
            return Results;
        }

        Constraint::AxisList HingeConstraint::GetValidLinearAxes() const
        {
            Constraint::AxisList Results;
            Results.push_back(-1);
            return Results;
        }

        Constraint::AxisList HingeConstraint::GetValidAngularAxes() const
        {
            Constraint::AxisList Results;
            Results.push_back(5);
            return Results;
        }

        Boole HingeConstraint::HasParamBeenSet(ConstraintParam Param, int Axis) const
        {
            // the logic here should match the logic in the source at http://bulletphysics.com/Bullet/BulletFull/btHingeConstraint_8cpp_source.html#l00962
            if ( -1!=Axis && 5!=Axis )
                { return false; }
            return  ( Con_Stop_ERP==Param && this->Hinge->getFlags() & BT_HINGE_FLAGS_ERP_STOP )   ||  //if we are checking the stop_erp AND the stop_erp bit is set
                    ( Con_Stop_CFM==Param && this->Hinge->getFlags() & BT_HINGE_FLAGS_CFM_STOP )   ||  //if we are checking the stop_cfm AND the stop_cfm bit is set
                    ( Con_CFM==Param      && this->Hinge->getFlags() & BT_HINGE_FLAGS_CFM_NORM )   ;   //if we are checking the cfm AND the cfm bit is set
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Serialization

        void HingeConstraint::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Constraint::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( HingeConstraint::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("UseFrameOffset").SetValue( this->GetUseFrameOffset() ) &&
                PropertiesNode.AppendAttribute("UseReferenceFrameA").SetValue( this->GetUseReferenceFrameA() ) &&
                PropertiesNode.AppendAttribute("LimitLow").SetValue( this->GetLimitLow() ) &&
                PropertiesNode.AppendAttribute("LimitHigh").SetValue( this->GetLimitHigh() ) &&
                PropertiesNode.AppendAttribute("LimitSoftness").SetValue( this->GetLimitSoftness() ) &&
                PropertiesNode.AppendAttribute("LimitBiasFactor").SetValue( this->GetLimitBiasFactor() ) &&
                PropertiesNode.AppendAttribute("LimitRelaxationFactor").SetValue( this->GetLimitRelaxationFactor() ) &&
                PropertiesNode.AppendAttribute("MotorEnabled").SetValue( this->GetMotorEnabled() ) &&
                PropertiesNode.AppendAttribute("MaxMotorImpulse").SetValue( this->GetMaxMotorImpulse() ) &&
                PropertiesNode.AppendAttribute("MotorTargetVelocity").SetValue( this->GetMotorTargetVelocity() ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",HingeConstraint::GetSerializableName() + "Properties",true);
            }
        }

        void HingeConstraint::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Constraint::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( HingeConstraint::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if( PropertiesNode.GetAttribute("Version").AsInt() == 1 ) {
                    Real LimitLow = 0.0;
                    Real LimitHigh = 0.0;
                    Real LimitSoftness = 0.9;
                    Real LimitBiasFactor = 0.3;
                    Real LimitRelaxationFactor = 1.0;

                    CurrAttrib = PropertiesNode.GetAttribute("UseFrameOffset");
                    if( !CurrAttrib.Empty() )
                        this->SetUseFrameOffset( CurrAttrib.AsBool() );

                    CurrAttrib = PropertiesNode.GetAttribute("UseReferenceFrameA");
                    if( !CurrAttrib.Empty() )
                        this->SetUseReferenceFrameA( CurrAttrib.AsBool() );

                    CurrAttrib = PropertiesNode.GetAttribute("LimitLow");
                    if( !CurrAttrib.Empty() )
                        LimitLow = CurrAttrib.AsReal();

                    CurrAttrib = PropertiesNode.GetAttribute("LimitHigh");
                    if( !CurrAttrib.Empty() )
                        LimitHigh = CurrAttrib.AsReal();

                    CurrAttrib = PropertiesNode.GetAttribute("LimitSoftness");
                    if( !CurrAttrib.Empty() )
                        LimitSoftness = CurrAttrib.AsReal();

                    CurrAttrib = PropertiesNode.GetAttribute("LimitBiasFactor");
                    if( !CurrAttrib.Empty() )
                        LimitBiasFactor = CurrAttrib.AsReal();

                    CurrAttrib = PropertiesNode.GetAttribute("LimitRelaxationFactor");
                    if( !CurrAttrib.Empty() )
                        LimitRelaxationFactor = CurrAttrib.AsReal();

                    this->SetLimits(LimitLow,LimitHigh,LimitSoftness,LimitBiasFactor,LimitRelaxationFactor);

                    CurrAttrib = PropertiesNode.GetAttribute("MotorEnabled");
                    if( !CurrAttrib.Empty() )
                        this->SetMotorEnabled( CurrAttrib.AsBool() );

                    CurrAttrib = PropertiesNode.GetAttribute("MaxMotorImpulse");
                    if( !CurrAttrib.Empty() )
                        this->SetMaxMotorImpulse( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("MotorTargetVelocity");
                    if( !CurrAttrib.Empty() )
                        this->SetMotorTargetVelocity( CurrAttrib.AsReal() );
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( HingeConstraint::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,HingeConstraint::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String HingeConstraint::GetDerivedSerializableName() const
            { return HingeConstraint::GetSerializableName(); }

        String HingeConstraint::GetSerializableName()
            { return "HingeConstraint"; }

        ////////////////////////////////////////////////////////////////////////////////
        // Internal

        btTypedConstraint* HingeConstraint::_GetConstraintBase() const
            { return this->Hinge; }
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
