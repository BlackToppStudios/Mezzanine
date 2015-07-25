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
#ifndef _physicsconetwistconstraint_cpp
#define _physicsconetwistconstraint_cpp

#include "Physics/conetwistconstraint.h"
#include "Physics/rigidproxy.h"

#include "stringtool.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Physics
    {
        /////////////////////////////////////////
        // ConeTwist Constraint Functions

        ConeTwistConstraint::ConeTwistConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB, PhysicsManager* Creator) :
            DualTransformConstraint(ID,ProxyA,ProxyB,Creator),
            ConeTwist(NULL)
            { this->CreateConstraint(ProxyA,ProxyB,TransA,TransB); }

        ConeTwistConstraint::ConeTwistConstraint(const UInt32 ID, RigidProxy* ProxyA, const Transform& TransA, PhysicsManager* Creator) :
            DualTransformConstraint(ID,ProxyA,Creator),
            ConeTwist(NULL)
            { this->CreateConstraint(ProxyA,NULL,TransA,Transform()); }

        ConeTwistConstraint::ConeTwistConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator) :
            DualTransformConstraint(0,NULL,Creator),
            ConeTwist(NULL)
            { this->ProtoDeSerialize(SelfRoot); }

        ConeTwistConstraint::~ConeTwistConstraint()
            { this->DestroyConstraint(); }

        void ConeTwistConstraint::CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB)
        {
            if( this->ConeTwist == NULL ) {
                if( RigidA && RigidB ) {
                    this->ConeTwist = new btConeTwistConstraint(*(RigidA->_GetPhysicsObject()),*(RigidB->_GetPhysicsObject()),TransA.GetBulletTransform(),TransB.GetBulletTransform());
                }else if( RigidA ) {
                    this->ConeTwist = new btConeTwistConstraint(*(RigidA->_GetPhysicsObject()),TransA.GetBulletTransform());
                }
            }
        }

        void ConeTwistConstraint::DestroyConstraint()
        {
            this->EnableConstraint(false);
            if( this->ConeTwist != NULL ) {
                delete this->ConeTwist;
                this->ConeTwist = NULL;
            }
            this->ProxA = NULL;
            this->ProxB = NULL;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        Boole ConeTwistConstraint::IsPassedSwingLimit() const
            { return this->ConeTwist->isPastSwingLimit(); }

        Real ConeTwistConstraint::GetTwistAngle() const
            { return this->ConeTwist->getTwistAngle(); }

        void ConeTwistConstraint::SetPivotATransform(const Transform& TransA)
        {
            btTransform BTrans = this->ConeTwist->getBFrame();
            this->ConeTwist->setFrames(TransA.GetBulletTransform(),BTrans);
        }

        void ConeTwistConstraint::SetPivotBTransform(const Transform& TransB)
        {
            btTransform ATrans = this->ConeTwist->getAFrame();
            this->ConeTwist->setFrames(ATrans,TransB.GetBulletTransform());
        }

        Transform ConeTwistConstraint::GetPivotATransform() const
            { return Transform( this->ConeTwist->getAFrame() ); }

        Transform ConeTwistConstraint::GetPivotBTransform() const
            { return Transform( this->ConeTwist->getBFrame() ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration Methods

        void ConeTwistConstraint::EnableMotor(const Boole Enable)
            { this->ConeTwist->enableMotor(Enable); }

        Boole ConeTwistConstraint::IsMotorEnabled() const
            { return this->ConeTwist->isMotorEnabled(); }

        void ConeTwistConstraint::SetAngularOnly(const Boole AngularOnly)
            { this->ConeTwist->setAngularOnly(AngularOnly); }

        Boole ConeTwistConstraint::GetAngularOnly() const
            { return this->ConeTwist->getAngularOnly(); }

        void ConeTwistConstraint::SetLimit(const Integer LimitIndex, const Real LimitValue)
            { this->ConeTwist->setLimit(LimitIndex, LimitValue); }

        void ConeTwistConstraint::SetLimits(const Real SwingSpan1, const Real SwingSpan2, const Real TwistSpan, const Real Softness, const Real BiasFactor, const Real RelaxationFactor)
            { this->ConeTwist->setLimit(SwingSpan1, SwingSpan2, TwistSpan, Softness, BiasFactor, RelaxationFactor); }

        Real ConeTwistConstraint::GetLimit(const Integer LimitIndex) const
            { return this->ConeTwist->getLimit(LimitIndex); }

        Real ConeTwistConstraint::GetSwingSpan1() const
            { return this->ConeTwist->getSwingSpan1(); }

        Real ConeTwistConstraint::GetSwingSpan2() const
            { return this->ConeTwist->getSwingSpan2(); }

        Real ConeTwistConstraint::GetTwistSpan() const
            { return this->ConeTwist->getTwistAngle(); }

        Real ConeTwistConstraint::GetLimitSoftness() const
            { return this->ConeTwist->getLimitSoftness(); }

        Real ConeTwistConstraint::GetBiasFactor() const
            { return this->ConeTwist->getBiasFactor(); }

        Real ConeTwistConstraint::GetRelaxationFactor() const
            { return this->ConeTwist->getRelaxationFactor(); }

        void ConeTwistConstraint::SetDamping(const Real Damping)
            { this->ConeTwist->setDamping(Damping); }

        Real ConeTwistConstraint::GetDamping() const
            { return this->ConeTwist->getDamping(); }

        void ConeTwistConstraint::SetMaxMotorImpulse(const Real MaxMotorImpulse)
            { this->ConeTwist->setMaxMotorImpulse(MaxMotorImpulse); }

        void ConeTwistConstraint::SetMaxMotorImpulseNormalized(const Real MaxMotorImpulse)
            { this->ConeTwist->setMaxMotorImpulseNormalized(MaxMotorImpulse); }

        Boole ConeTwistConstraint::IsMaxMotorImpulseNormalized() const
            { return this->ConeTwist->isMaxMotorImpulseNormalized(); }

        Real ConeTwistConstraint::GetMaxMotorImpulse() const
            { return this->ConeTwist->getMaxMotorImpulse(); }

        void ConeTwistConstraint::SetFixThresh(const Real FixThresh)
            { this->ConeTwist->setFixThresh(FixThresh); }

        Real ConeTwistConstraint::GetFixThresh() const
            { return this->ConeTwist->getFixThresh(); }

        void ConeTwistConstraint::SetMotorTarget(const Quaternion& Quat)
            { this->ConeTwist->setMotorTarget( Quat.GetBulletQuaternion() ); }

        void ConeTwistConstraint::SetMotorTargetInConstraintSpace(const Quaternion& Quat)
            { this->ConeTwist->setMotorTargetInConstraintSpace( Quat.GetBulletQuaternion() ); }

        Quaternion ConeTwistConstraint::GetMotorTarget() const
            { return Quaternion( this->ConeTwist->getMotorTarget() ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Constraint Parameters

        Constraint::ParamList ConeTwistConstraint::GetValidParamsOnAxis(int Axis) const
        {
            Constraint::ParamList Results;
            // All params are valid on each of the 6 axes
            if( Axis >= 0 && Axis < 6 ) {
                Results.push_back(Con_ERP);
                Results.push_back(Con_Stop_ERP);
                Results.push_back(Con_CFM);
                Results.push_back(Con_Stop_CFM);
            }
            return Results;
        }

        Constraint::AxisList ConeTwistConstraint::GetValidLinearAxes() const
        {
            Constraint::AxisList Results;
            Results.push_back(0);
            Results.push_back(1);
            Results.push_back(2);
            return Results;
        }

        Constraint::AxisList ConeTwistConstraint::GetValidAngularAxes() const
        {
            Constraint::AxisList Results;
            Results.push_back(3);
            Results.push_back(4);
            Results.push_back(5);
            return Results;
        }

        Boole ConeTwistConstraint::HasParamBeenSet(Physics::ConstraintParam Param, int Axis) const
        {
            if( Param == Physics::Con_ERP || Param == Physics::Con_Stop_ERP ) {
                if( Axis >= 0 && Axis < 3 ) {
                    return ( this->ConeTwist->getFlags() & BT_CONETWIST_FLAGS_LIN_ERP );
                }else if( Axis >= 3 && Axis < 6 ) {
                    // This internally correlates to the biasfactor
                    return true;
                }
            }else if( Param == Physics::Con_CFM || Param == Physics::Con_Stop_CFM ) {
                if( Axis >= 0 && Axis < 3 ) {
                    return ( this->ConeTwist->getFlags() & BT_CONETWIST_FLAGS_LIN_CFM );
                }else if( Axis >= 3 && Axis < 6 ) {
                    return ( this->ConeTwist->getFlags() & BT_CONETWIST_FLAGS_ANG_CFM );
                }
            }
            return false;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void ConeTwistConstraint::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Constraint::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( ConeTwistConstraint::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("MotorEnabled").SetValue( this->IsMotorEnabled() ) &&
                PropertiesNode.AppendAttribute("AngularOnly").SetValue( this->GetAngularOnly() ) &&
                PropertiesNode.AppendAttribute("Damping").SetValue( this->GetDamping() ) &&
                PropertiesNode.AppendAttribute("MaxMotorImpulse").SetValue( this->GetMaxMotorImpulse() ) &&
                PropertiesNode.AppendAttribute("FixThreshold").SetValue( this->GetFixThresh() ) &&
                PropertiesNode.AppendAttribute("SwingSpan1").SetValue( this->GetSwingSpan1() ) &&
                PropertiesNode.AppendAttribute("SwingSpan2").SetValue( this->GetSwingSpan2() ) &&
                PropertiesNode.AppendAttribute("TwistSpan").SetValue( this->GetTwistSpan() ) &&
                PropertiesNode.AppendAttribute("LimitSoftness").SetValue( this->GetLimitSoftness() ) &&
                PropertiesNode.AppendAttribute("BiasFactor").SetValue( this->GetBiasFactor() ) &&
                PropertiesNode.AppendAttribute("RelaxationFactor").SetValue( this->GetRelaxationFactor() ) )
            {
                XML::Node MotorTargetNode = PropertiesNode.AppendChild("MotorTarget");
                this->GetMotorTarget().ProtoSerialize( MotorTargetNode );

                return;
            }else{
                SerializeError("Create XML Attribute Values",ConeTwistConstraint::GetSerializableName() + "Properties",true);
            }
        }

        void ConeTwistConstraint::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Constraint::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( ConeTwistConstraint::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    Real ConeSwingSpan1 = 0.0;
                    Real ConeSwingSpan2 = 0.0;
                    Real ConeTwistSpan = 0.0;
                    Real ConeLimitSoftness = 1.0;
                    Real ConeBiasFactor = 0.3;
                    Real ConeRelaxationFactor = 1.0;

                    CurrAttrib = PropertiesNode.GetAttribute("MotorEnabled");
                    if( !CurrAttrib.Empty() )
                        this->EnableMotor( CurrAttrib.AsBool() );

                    CurrAttrib = PropertiesNode.GetAttribute("AngularOnly");
                    if( !CurrAttrib.Empty() )
                        this->SetAngularOnly( CurrAttrib.AsBool() );

                    CurrAttrib = PropertiesNode.GetAttribute("Damping");
                    if( !CurrAttrib.Empty() )
                        this->SetDamping( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("MaxMotorImpulse");
                    if( !CurrAttrib.Empty() )
                        this->SetMaxMotorImpulse( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("FixThreshold");
                    if( !CurrAttrib.Empty() )
                        this->SetFixThresh( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("SwingSpan1");
                    if( !CurrAttrib.Empty() )
                        ConeSwingSpan1 = CurrAttrib.AsReal();

                    CurrAttrib = PropertiesNode.GetAttribute("SwingSpan2");
                    if( !CurrAttrib.Empty() )
                        ConeSwingSpan2 = CurrAttrib.AsReal();

                    CurrAttrib = PropertiesNode.GetAttribute("TwistSpan");
                    if( !CurrAttrib.Empty() )
                        ConeTwistSpan = CurrAttrib.AsReal();

                    CurrAttrib = PropertiesNode.GetAttribute("LimitSoftness");
                    if( !CurrAttrib.Empty() )
                        ConeLimitSoftness = CurrAttrib.AsReal();

                    CurrAttrib = PropertiesNode.GetAttribute("BiasFactor");
                    if( !CurrAttrib.Empty() )
                        ConeBiasFactor = CurrAttrib.AsReal();

                    CurrAttrib = PropertiesNode.GetAttribute("RelaxationFactor");
                    if( !CurrAttrib.Empty() )
                        ConeRelaxationFactor = CurrAttrib.AsReal();

                    this->SetLimits(ConeSwingSpan1,ConeSwingSpan2,ConeTwistSpan,ConeLimitSoftness,ConeBiasFactor,ConeRelaxationFactor);

                    XML::Node MotorTargetNode = PropertiesNode.GetChild("MotorTarget").GetFirstChild();
                    if( !MotorTargetNode.Empty() ) {
                        Quaternion MotorTarget(MotorTargetNode);
                        this->SetMotorTarget(MotorTarget);
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( ConeTwistConstraint::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,ConeTwistConstraint::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String ConeTwistConstraint::GetDerivedSerializableName() const
            { return ConeTwistConstraint::GetSerializableName(); }

        String ConeTwistConstraint::GetSerializableName()
            { return "ConeTwistConstraint"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal

        btTypedConstraint* ConeTwistConstraint::_GetConstraintBase() const
            { return this->ConeTwist; }
    }//Physics
}//Mezzanine

#endif
