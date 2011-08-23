//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _constraint_cpp
#define _constraint_cpp

#include "constraint.h"
#include "actorrigid.h"
#include "serialization.h"
#include "world.h"


#include <btBulletDynamicsCommon.h>


/// @cond 0

namespace phys
{
    /////////////////////////////////////////
    // Functions

    String ConstraintParamAsString(ConstraintParam Param)
    {
        switch (Param)
        {
            case Con_ERP:           return String("Con_ERP");
            case Con_Stop_ERP:      return String("Con_Stop_ERP");
            case Con_CFM:           return String("Con_CFM");
            case Con_Stop_CFM:      return String("Con_Stop_CFM");
            default: throw(phys::Exception("Attempted to convert invalid Constraint Paramater to a String."));
        }
    }

    /////////////////////////////////////////
    // TypedConstraint Functions

    TypedConstraint::TypedConstraint()
        : ConstraintBase(NULL)
    {
    }

    TypedConstraint::~TypedConstraint()
    {
    }

    void TypedConstraint::SetBodies(ActorRigid* Act1, ActorRigid* Act2)
    {
        ActA = Act1;
        ActB = Act2;
        BodyA = Act1->physrigidbody;
        BodyB = Act2->physrigidbody;
    }

    void TypedConstraint::SetBodies(ActorRigid* Act1)
    {
        ActA = Act1;
        ActB = NULL;
        BodyA = Act1->physrigidbody;
        BodyB = NULL;
    }

    ActorRigid* TypedConstraint::GetActorA() const
        { return ActA; }

    ActorRigid* TypedConstraint::GetActorB() const
        { return ActB; }

    void TypedConstraint::SetParam(ConstraintParam Param, Real Value, int Axis)
        { this->ConstraintBase->setParam(Param, Value, Axis); }

    Real TypedConstraint::GetParam(ConstraintParam Param, int Axis) const
        { return this->ConstraintBase->getParam(Param, Axis); }

    // Serializable
    void TypedConstraint::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node TypedConstraintNode = CurrentRoot.AppendChild(SerializableName());                     // The base node all the base constraint stuff will go in
        if (!TypedConstraintNode)
            { SerializeError("Create TypedConstraintNode", SerializableName()); }

        xml::Attribute Version = TypedConstraintNode.AppendAttribute("Version");
        xml::Attribute ActorNameA = TypedConstraintNode.AppendAttribute("ActorNameA");
        xml::Attribute ActorNameB = TypedConstraintNode.AppendAttribute("ActorNameB");

        if (Version && ActorNameA && ActorNameB)
        {
            Version.SetValue(1);
            ActorNameA.SetValue( this->GetActorA()->GetName() );
            ActorNameB.SetValue( this->GetActorB()->GetName() );
        }else{
            SerializeError("Create Attributes on TypedConstraintNode", SerializableName());
        }

        String CurrentAxisName;
        AxisList AllAxis = this->ValidAxis();
        for(AxisList::iterator AxisIter=AllAxis.begin(); AllAxis.end()!=AxisIter; ++AxisIter)
        {
            xml::Node OneAxisNode;
            CurrentAxisName = String(StringCat("Axis",ToString(*AxisIter)));                        // Should result in "Axis-1", "Axis0", "Axis1" ...
            ParamList AxisParams = ValidParamOnAxis(*AxisIter);
            for(ParamList::iterator ParamIter=AxisParams.begin(); AxisParams.end()!=ParamIter; ++ParamIter)
            {
                if(HasParamBeenSet(*ParamIter,*AxisIter))                                           // No need to create a node if no attributes exist for it, so we will create one for the first attribute that does exist and
                {                                                                                   // reuse it until we move onto the next Axis
                    if (!OneAxisNode)
                    {
                        OneAxisNode = TypedConstraintNode.AppendChild(CurrentAxisName);
                        if (!OneAxisNode)
                            { SerializeError( StringCat("Create ", CurrentAxisName ," Node"), SerializableName()); }
                    }

                    xml::Attribute CurrenParamAttribute = OneAxisNode.AppendAttribute( ConstraintParamAsString(*ParamIter) );
                    if (!CurrenParamAttribute)
                        { SerializeError( StringCat("Create ", ConstraintParamAsString(*ParamIter), " Attribute in ", CurrentAxisName ," Node"), SerializableName()); }
                    CurrenParamAttribute.SetValue( this->GetParam(*ParamIter,*AxisIter));
                }
            }
        }

    }

    // DeSerializable
    void TypedConstraint::ProtoDeSerialize(const xml::Node& OneNode)
    {
    }

    String TypedConstraint::SerializableName() const
        { return String("TypedConstraint"); }

    /////////////////////////////////////////
    // ConeTwist Constraint Functions

    ConeTwistConstraint::ConeTwistConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA,
                                              const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB)
    {
        SetBodies(ActorA,ActorB);

        btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
        btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        ConeTwist = new btConeTwistConstraint (*BodyA, *BodyB, transa, transb);
        ConstraintBase = ConeTwist;
    }

    ConeTwistConstraint::ConeTwistConstraint(ActorRigid* ActorA, const Vector3& VectorA, const Quaternion& QuaternionA)
    {
        SetBodies(ActorA);

        btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
        ConeTwist = new btConeTwistConstraint (*BodyA, transa);
        ConstraintBase = ConeTwist;
    }

    ConeTwistConstraint::~ConeTwistConstraint()
    {
        delete ConeTwist;
    }

    void ConeTwistConstraint::SetAngularOnly(bool AngularOnly)
    {
        this->ConeTwist->setAngularOnly(AngularOnly);
    }

    void ConeTwistConstraint::SetLimit(int LimitIndex, Real LimitValue)
    {
        this->ConeTwist->setLimit(LimitIndex, LimitValue);
    }

    void ConeTwistConstraint::SetLimit(Real SwingSpan1, Real SwingSpan2, Real Twistspan,
                                       Real Softness, Real BiasFactor, Real RelaxationFactor)
    {
        this->ConeTwist->setLimit(SwingSpan1, SwingSpan2, Twistspan, Softness, BiasFactor, RelaxationFactor);
    }

    void ConeTwistConstraint::SetDamping(Real Damping)
    {
        this->ConeTwist->setDamping(Damping);
    }

    void ConeTwistConstraint::SetMaxMotorImpulse(Real MaxMotorImpulse)
    {
        this->ConeTwist->setMaxMotorImpulse(MaxMotorImpulse);
    }

    void ConeTwistConstraint::SetMaxMotorImpulseNormalized(Real MaxMotorImpulse)
    {
        this->ConeTwist->setMaxMotorImpulseNormalized(MaxMotorImpulse);
    }

    void ConeTwistConstraint::SetFixThresh(Real FixThresh)
    {
        this->ConeTwist->setFixThresh(FixThresh);
    }

    void ConeTwistConstraint::SetMotorTarget(const Quaternion& Quat)
    {
        this->ConeTwist->setMotorTarget(Quat.GetBulletQuaternion());
    }

    void ConeTwistConstraint::SetMotorTargetInConstraintSpace(const Quaternion& Quat)
    {
        this->ConeTwist->setMotorTargetInConstraintSpace(Quat.GetBulletQuaternion());
    }

    void ConeTwistConstraint::EnableMotor(bool Enable)
    {
        this->ConeTwist->enableMotor(Enable);
    }

    bool ConeTwistConstraint::IsPassedSwingLimit()
    {
        return this->ConeTwist->isPastSwingLimit();
    }

    /////////////////////////////////////////
    // Generic6Dof Constraint Functions

    Generic6DofConstraint::Generic6DofConstraint()
    {
    }

    Generic6DofConstraint::Generic6DofConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA,
                                                  const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseLinearReferenceA)
    {
        SetBodies(ActorA,ActorB);

        btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
        btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        Generic6dof = new btGeneric6DofConstraint(*BodyA, *BodyB, transa, transb, UseLinearReferenceA);
        ConstraintBase = Generic6dof;
    }

    Generic6DofConstraint::Generic6DofConstraint(ActorRigid* ActorB, const Vector3& VectorB, const Quaternion& QuaternionB, bool UseLinearReferenceB)
    {
        SetBodies(ActorB);

        btTransform transa(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        Generic6dof = new btGeneric6DofConstraint(*BodyA, transa, UseLinearReferenceB);
        ConstraintBase = Generic6dof;
    }

    Generic6DofConstraint::~Generic6DofConstraint()
    {
        if(Generic6dof)
            delete Generic6dof;
    }

    void Generic6DofConstraint::SetOffsetALocation(const Vector3& Location)
    {
        this->Generic6dof->getFrameOffsetA().setOrigin(Location.GetBulletVector3());
    }

    void Generic6DofConstraint::SetOffsetBLocation(const Vector3& Location)
    {
        this->Generic6dof->getFrameOffsetB().setOrigin(Location.GetBulletVector3());
    }

    void Generic6DofConstraint::SetLinearUpperLimit(const Vector3& Limit)
    {
        this->Generic6dof->setLinearUpperLimit(Limit.GetBulletVector3());
    }

    void Generic6DofConstraint::SetLinearLowerLimit(const Vector3& Limit)
    {
        this->Generic6dof->setLinearLowerLimit(Limit.GetBulletVector3());
    }

    void Generic6DofConstraint::SetAngularUpperLimit(const Vector3& Limit)
    {
        this->Generic6dof->setAngularUpperLimit(Limit.GetBulletVector3());
    }

    void Generic6DofConstraint::SetAngularLowerLimit(const Vector3& Limit)
    {
        this->Generic6dof->setAngularLowerLimit(Limit.GetBulletVector3());
    }

    void Generic6DofConstraint::SetUseFrameOffset(bool UseOffset)
    {
        this->Generic6dof->setUseFrameOffset(UseOffset);
    }

    void Generic6DofConstraint::SetLimit(int Axis, Real Low, Real High)
    {
        this->Generic6dof->setLimit(Axis, Low, High);
    }

    void Generic6DofConstraint::CalculateTransforms()
    {
        this->Generic6dof->calculateTransforms();
    }

    TypedConstraint::ParamList Generic6DofConstraint::ValidParamOnAxis(int Axis) const
    {
        TypedConstraint::ParamList Results;
        if(0<=Axis && 5>=Axis)
        {
            Results.push_back(Con_Stop_ERP);
            Results.push_back(Con_CFM);
            Results.push_back(Con_Stop_CFM);
        }
        return Results;
    }

    TypedConstraint::AxisList Generic6DofConstraint::ValidLinearAxis() const
    {
        TypedConstraint::AxisList Results;
        Results.push_back(0);
        Results.push_back(1);
        Results.push_back(2);
        return Results;
    }

    TypedConstraint::AxisList Generic6DofConstraint::ValidAngularAxis() const
    {
        TypedConstraint::AxisList Results;
        Results.push_back(3);
        Results.push_back(4);
        Results.push_back(5);
        return Results;
    }

    bool Generic6DofConstraint::HasParamBeenSet(ConstraintParam Param, int Axis) const
    {
        // the logic here should match the logic in the source at http://bulletphysics.com/Bullet/BulletFull/btGeneric6DofConstraint_8cpp_source.html#l00964
        if(0<=Axis && 5>=Axis)
            { return false; }
        return  ( Con_Stop_ERP==Param && this->Generic6dof->m_flags & (BT_6DOF_FLAGS_ERP_STOP << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) )   ||  //if we are checking the stop_erp AND the stop_erp bit is set for the correct axis
                ( Con_Stop_CFM==Param && this->Generic6dof->m_flags & (BT_6DOF_FLAGS_CFM_STOP << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) )   ||  //if we are checking the stop_cfm AND the stop_cfm bit is set
                ( Con_CFM==Param      && this->Generic6dof->m_flags & (BT_6DOF_FLAGS_CFM_NORM << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) )   ;   //if we are checking the cfm AND the cfm bit is set
    }

#ifdef PHYSXML
    void Generic6DofConstraint::ProtoSerialize(xml::Node& CurrentRoot) const
    {

        xml::Node G6dofNode = CurrentRoot.AppendChild(SerializableName());                     // The base node all the base constraint stuff will go in
        if (!G6dofNode)
            { SerializeError("Create G6dofNode", SerializableName()); }

        // ActorA/B
            // Transform

        // Linear
            // Upper limit
            // Lower Limit

        // Angular
            // Upper limit
            // Lower Limit

        // Linear and angular Motors
            // The whole Motor Class for each

        //Typedconstraint





        // Spring x6
            // Enabled
            // Stiffness
            // Damping

/*
        xml::Attribute VerAttr = P2PNode.AppendAttribute("Version");
        xml::Attribute TauAttr = P2PNode.AppendAttribute("Tau");
        xml::Attribute ClaAttr = P2PNode.AppendAttribute("ImpulseClamping");
        xml::Attribute DamAttr = P2PNode.AppendAttribute("Damping");

        if( VerAttr && TauAttr && ClaAttr && DamAttr )
        {
            VerAttr.SetValue(1);
            TauAttr.SetValue(this->GetTAU());
            ClaAttr.SetValue(this->GetImpulseClamping());
            DamAttr.SetValue(this->GetDamping());
        }else{
            SerializeError("Create P2PNode Attributes", SerializableName());
        }

        xml::Node ActorANode = P2PNode.AppendChild("ActorA");
        if (!ActorANode)
            { SerializeError("Create ActorANode", SerializableName()); }
        this->GetPivotA().ProtoSerialize(ActorANode);

        xml::Node ActorBNode = P2PNode.AppendChild("ActorB");
        if (!ActorBNode)
            { SerializeError("Create ActorBNode", SerializableName()); }
        this->GetPivotB().ProtoSerialize(ActorBNode);

        this->TypedConstraint::ProtoSerialize(P2PNode);*/
    }

    void Generic6DofConstraint::ProtoDeSerialize(const xml::Node& OneNode)
    {

    }

    String Generic6DofConstraint::SerializableName() const
        { return String("Generic6DofConstraint"); }
#endif // /PHYSXML

    /////////////////////////////////////////
    // Generic6Dof Spring Constraint Functions

    Generic6DofSpringConstraint::Generic6DofSpringConstraint()
    {
    }

    Generic6DofSpringConstraint::Generic6DofSpringConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA,
                                                              const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseLinearReferenceA)
    {
        SetBodies(ActorA,ActorB);

        btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
        btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        Generic6dofSpring = new btGeneric6DofSpringConstraint(*BodyA, *BodyB, transa, transb, UseLinearReferenceA);
        Generic6dof = Generic6dofSpring;
        ConstraintBase = Generic6dofSpring;
    }

    Generic6DofSpringConstraint::~Generic6DofSpringConstraint()
    {
        if(Generic6dofSpring)
        {
            delete Generic6dofSpring;
            Generic6dofSpring = NULL;
            Generic6dof = NULL;
        }
    }

    void Generic6DofSpringConstraint::SetStiffness(int Index, Real Stiffness)
    {
        this->Generic6dofSpring->setStiffness(Index, Stiffness);
    }

    void Generic6DofSpringConstraint::SetDamping(int Index, Real Damping)
    {
        this->Generic6dofSpring->setDamping(Index, Damping);
    }

    void Generic6DofSpringConstraint::SetEquilibriumPoint(int Index)
    {
        this->Generic6dofSpring->setEquilibriumPoint(Index);
    }

    void Generic6DofSpringConstraint::EnableSpring(int Index, bool Enable)
    {
        this->Generic6dofSpring->enableSpring(Index, Enable);
    }

    /////////////////////////////////////////
    // Hinge Constraint Functions

    HingeConstraint::HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& PivotInA, const Vector3& PivotInB, const Vector3& AxisInA, const Vector3& AxisInB, bool UseReferenceFrameA)
    {
        SetBodies(ActorA,ActorB);

        btVector3 tempA(AxisInA.GetBulletVector3());
        btVector3 tempB(AxisInB.GetBulletVector3());
        Hinge = new btHingeConstraint(*BodyA, *BodyB, PivotInA.GetBulletVector3(), PivotInB.GetBulletVector3(), tempA, tempB, bool(UseReferenceFrameA));
        ConstraintBase = Hinge;
    }

    HingeConstraint::HingeConstraint(ActorRigid* ActorA, const Vector3& PivotInA, const Vector3& AxisInA, bool UseReferenceFrameA)
    {
        SetBodies(ActorA);

        btVector3 tempA(AxisInA.GetBulletVector3());
        Hinge = new btHingeConstraint(*BodyA, PivotInA.GetBulletVector3(), tempA, bool(UseReferenceFrameA));
        ConstraintBase = Hinge;
    }

    HingeConstraint::HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA, const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseReferenceFrameA)
    {
        SetBodies(ActorA,ActorB);

        btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
        btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        Hinge = new btHingeConstraint(*BodyA, *BodyB, transa, transb, UseReferenceFrameA);
        ConstraintBase = Hinge;
    }

    HingeConstraint::HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Transform& TransformA, const Transform& TransformB, bool UseReferenceFrameA)
    {
        SetBodies(ActorA,ActorB);
        Hinge = new btHingeConstraint(*BodyA, *BodyB, TransformA.GetBulletTransform(), TransformB.GetBulletTransform(), UseReferenceFrameA);
        ConstraintBase = Hinge;
    }

    HingeConstraint::~HingeConstraint()
    {
        if(Hinge)
            delete Hinge;
    }

    bool HingeConstraint::GetUseReferenceFrameA() const
        { return this->Hinge->m_useReferenceFrameA; }

    void HingeConstraint::SetUseReferenceFrameA(bool UseReferenceFrameA)
        { this->Hinge->m_useReferenceFrameA = UseReferenceFrameA; }

    void HingeConstraint::SetAPivotLocation(const Vector3& Location)
        { this->Hinge->getAFrame().setOrigin(Location.GetBulletVector3()); }

    void HingeConstraint::SetBPivotLocation(const Vector3& Location)
        { this->Hinge->getBFrame().setOrigin(Location.GetBulletVector3()); }

    void HingeConstraint::EnableAngularMotor(bool EnableMotor, Real TargetVelocity, Real MaxMotorImpulse)
        { this->Hinge->enableAngularMotor(EnableMotor, TargetVelocity, MaxMotorImpulse); }

    void HingeConstraint::EnableMotor(bool EnableMotor)
        { this->Hinge->enableMotor(EnableMotor); }

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

    bool HingeConstraint::GetMotorEnabled() const
        { return this->Hinge->getEnableAngularMotor(); }

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

    void HingeConstraint::SetAxis(const Vector3& AxisInA)
    {
        btVector3 temp(AxisInA.GetBulletVector3());
        this->Hinge->setAxis(temp);
    }

    bool HingeConstraint::getUseFrameOffset() const
        { return this->Hinge->getUseFrameOffset(); }

    void HingeConstraint::SetUseFrameOffset(bool FrameOffset)
        { this->Hinge->setUseFrameOffset(FrameOffset); }

    void HingeConstraint::SetTransformA(const Transform& TranA)
        { this->Hinge->getAFrame() << TranA; }

    void HingeConstraint::SetTransformB(const Transform& TranB)
        { this->Hinge->getBFrame() << TranB; }

    Transform HingeConstraint::GetTransformA() const
        { return Transform(this->Hinge->getAFrame()); }

    Transform HingeConstraint::GetTransformB() const
        { return Transform(this->Hinge->getBFrame()); }

    TypedConstraint::ParamList HingeConstraint::ValidParamOnAxis(int Axis) const
    {
        TypedConstraint::ParamList Results;
        if(-1==Axis||5==Axis)
        {
            Results.push_back(Con_Stop_ERP);
            Results.push_back(Con_CFM);
            Results.push_back(Con_Stop_CFM);
        }
        return Results;
    }

    TypedConstraint::AxisList HingeConstraint::ValidLinearAxis() const
    {
        TypedConstraint::AxisList Results;
        Results.push_back(-1);
        return Results;
    }

    TypedConstraint::AxisList HingeConstraint::ValidAngularAxis() const
    {
        TypedConstraint::AxisList Results;
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

#ifdef PHYSXML
    void HingeConstraint::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node HingeNode = CurrentRoot.AppendChild(SerializableName());          // The base node all the base constraint stuff will go in
        if (!HingeNode)
            { SerializeError("Create HingeNode", SerializableName()); }

        xml::Attribute VerAttr = HingeNode.AppendAttribute("Version");              // Base Attributes
        xml::Attribute RefA = HingeNode.AppendAttribute("ReferenceInA");
        xml::Attribute FrameOff = HingeNode.AppendAttribute("UseFrameOffset");
        if( VerAttr && RefA && FrameOff )
        {
            VerAttr.SetValue(1);
            RefA.SetValue(this->GetUseReferenceFrameA());
            FrameOff.SetValue(this->getUseFrameOffset());
        }else{
            SerializeError("Create HingeNode Attributes", SerializableName());
        }

        xml::Node ActorANode = HingeNode.AppendChild("ActorA");                     // Get everything we need about ActorA
        if (!ActorANode)
            { SerializeError("Create ActorANode", SerializableName()); }
        this->GetTransformA().ProtoSerialize(ActorANode);

        xml::Node ActorBNode = HingeNode.AppendChild("ActorB");                     // Get everything we need about ActorB
        if (!ActorBNode)
            { SerializeError("Create ActorBNode", SerializableName()); }
        this->GetTransformB().ProtoSerialize(ActorBNode);

        xml::Node MotorNode = HingeNode.AppendChild("Motor");                       // Motor Node
        if (!MotorNode)
            { SerializeError("Create MotorNode", SerializableName()); }

        xml::Attribute MotEnabled = MotorNode.AppendAttribute("Enabled");           // Motor Attributes
        xml::Attribute MotImpulse = MotorNode.AppendAttribute("MaxImpulse");
        xml::Attribute MotTarget = MotorNode.AppendAttribute("TargetVelocity");
        if( MotEnabled && MotImpulse && MotTarget )
        {
            MotEnabled.SetValue(this->GetMotorEnabled());
            MotImpulse.SetValue(this->GetMaxMotorImpulse());
            MotTarget.SetValue(this->GetMotorTargetVelocity());
        }else{
            SerializeError("Create MotorNode Attributes", SerializableName());
        }

        xml::Node LimitNode = HingeNode.AppendChild("Limits");                      // Limits Node
        if (!LimitNode)
            { SerializeError("Create LimitNode", SerializableName()); }

        xml::Attribute LimLow = LimitNode.AppendAttribute("Low");                   // Limits Attributes
        xml::Attribute LimHigh = LimitNode.AppendAttribute("High");
        xml::Attribute LimSoft = LimitNode.AppendAttribute("Softness");
        xml::Attribute LimBias = LimitNode.AppendAttribute("BiasFactor");
        xml::Attribute LimRelax = LimitNode.AppendAttribute("RelaxationFactor");
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

        this->TypedConstraint::ProtoSerialize(HingeNode);
    }

    void HingeConstraint::ProtoDeSerialize(const xml::Node& OneNode)
    {

    }

    String HingeConstraint::SerializableName() const
        { return String("HingeConstraint"); }
#endif // /PHYSXML


    /////////////////////////////////////////
    // Hinge2 Constraint Functions

    Hinge2Constraint::Hinge2Constraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& Anchor, const Vector3& Axis1, const Vector3& Axis2)
    {
        SetBodies(ActorA,ActorB);

        btVector3 temp1(Anchor.GetBulletVector3());
        btVector3 temp2(Axis1.GetBulletVector3());
        btVector3 temp3(Axis2.GetBulletVector3());
        Hinge2 = new btHinge2Constraint(*BodyA, *BodyB, temp1, temp2, temp3);
        Generic6dofSpring = Hinge2;
        Generic6dof = Hinge2;
        ConstraintBase = Hinge2;
    }

    Hinge2Constraint::~Hinge2Constraint()
    {
        delete Hinge2;
        Hinge2 = NULL;
        Generic6dofSpring = NULL;
        Generic6dof = NULL;
    }

    void Hinge2Constraint::SetUpperLimit(Real Ang1Max)
    {
        this->Hinge2->setUpperLimit(Ang1Max);
    }

    void Hinge2Constraint::SetLowerLimit(Real Ang1Min)
    {
        this->Hinge2->setLowerLimit(Ang1Min);
    }

    /////////////////////////////////////////
    // Point2Point Constraint Functions

    Point2PointConstraint::Point2PointConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& PivotA, const Vector3& PivotB)
    {
        SetBodies(ActorA,ActorB);

        Point2Point = new btPoint2PointConstraint(*BodyA, *BodyB, PivotA.GetBulletVector3(), PivotB.GetBulletVector3());
        ConstraintBase = Point2Point;
    }

    Point2PointConstraint::Point2PointConstraint(ActorRigid* ActorA, const Vector3& PivotA)
    {
        SetBodies(ActorA);

        Point2Point = new btPoint2PointConstraint(*BodyA, PivotA.GetBulletVector3());
        ConstraintBase = Point2Point;
    }

    Point2PointConstraint::~Point2PointConstraint()
    {
        delete Point2Point;
    }

    void Point2PointConstraint::SetPivotA(const Vector3& PivotA)
    {
        this->Point2Point->setPivotA(PivotA.GetBulletVector3());
    }

    void Point2PointConstraint::SetPivotB(const Vector3& PivotB)
    {
        this->Point2Point->setPivotB(PivotB.GetBulletVector3());
    }

    Vector3 Point2PointConstraint::GetPivotA() const
    {
        return Vector3(this->Point2Point->getPivotInA());
    }

    Vector3 Point2PointConstraint::GetPivotB() const
        { return Vector3(this->Point2Point->getPivotInB()); }

    void Point2PointConstraint::SetImpulseClamping(Real Clamping)
        { this->Point2Point->m_setting.m_impulseClamp = Clamping; }

    Real Point2PointConstraint::GetImpulseClamping() const
        { return this->Point2Point->m_setting.m_impulseClamp; }

    void Point2PointConstraint::SetDamping(Real Damping)
        { this->Point2Point->m_setting.m_damping = Damping; }

    Real Point2PointConstraint::GetDamping() const
        { return this->Point2Point->m_setting.m_damping; }


    void Point2PointConstraint::SetTAU(Real TAU)
    {
        this->Point2Point->m_setting.m_tau = TAU;
    }

    Real Point2PointConstraint::GetTAU() const
    {
        return this->Point2Point->m_setting.m_tau;
    }

    TypedConstraint::ParamList Point2PointConstraint::ValidParamOnAxis(int Axis) const
    {
        TypedConstraint::ParamList Results;
        if(-1==Axis)
        {
            Results.push_back(Con_ERP);
            Results.push_back(Con_Stop_ERP);
            Results.push_back(Con_CFM);
            Results.push_back(Con_Stop_CFM);
        }
        return Results;
    }

    TypedConstraint::AxisList Point2PointConstraint::ValidLinearAxis() const
    {
        TypedConstraint::AxisList Results;
        Results.push_back(-1);
        return Results;
    }

    TypedConstraint::AxisList Point2PointConstraint::ValidAngularAxis() const
    {
        TypedConstraint::AxisList Results;
        return Results;
    }

    bool Point2PointConstraint::HasParamBeenSet(ConstraintParam Param, int Axis) const
    {
        // the logic here should match the logic in the source at http://bulletphysics.com/Bullet/BulletFull/btPoint2PointConstraint_8cpp_source.html#l00202
        if (-1!=Axis)
            { return false; }
        return  ( (Con_ERP==Param||Con_Stop_ERP==Param) && this->Point2Point->m_flags & BT_P2P_FLAGS_ERP )     ||   //If we are checking erp OR we are checking stoperp AND the erp Flag is set OR
                ( (Con_CFM==Param||Con_Stop_CFM==Param) && this->Point2Point->m_flags & BT_P2P_FLAGS_CFM )      ;   //   we are checking cfm OR we are checking stopcfm AND the cfm Flag is set
    }

#ifdef PHYSXML
    void Point2PointConstraint::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node P2PNode = CurrentRoot.AppendChild(SerializableName());                     // The base node all the base constraint stuff will go in
        if (!P2PNode)
            { SerializeError("Create P2PNode", SerializableName()); }

        xml::Attribute VerAttr = P2PNode.AppendAttribute("Version");
        xml::Attribute TauAttr = P2PNode.AppendAttribute("Tau");
        xml::Attribute ClaAttr = P2PNode.AppendAttribute("ImpulseClamping");
        xml::Attribute DamAttr = P2PNode.AppendAttribute("Damping");

        //xml::Attribute FltAttr = P2PNode.AppendAttribute("FLT_MAX"); FltAttr.SetValue(FLT_MAX);
        //xml::Attribute InfAttr = P2PNode.AppendAttribute("SIMD_INFINITY"); InfAttr.SetValue(SIMD_INFINITY);
        //xml::Attribute InpAttr = P2PNode.AppendAttribute("IN_PARALLELL_SOLVER"); InpAttr.SetValue(IN_PARALLELL_SOLVER);

        if( VerAttr && TauAttr && ClaAttr && DamAttr )
        {
            VerAttr.SetValue(1);
            TauAttr.SetValue(this->GetTAU());
            ClaAttr.SetValue(this->GetImpulseClamping());
            DamAttr.SetValue(this->GetDamping());
        }else{
            SerializeError("Create P2PNode Attributes", SerializableName());
        }

        xml::Node ActorANode = P2PNode.AppendChild("ActorA");
        if (!ActorANode)
            { SerializeError("Create ActorANode", SerializableName()); }
        this->GetPivotA().ProtoSerialize(ActorANode);

        xml::Node ActorBNode = P2PNode.AppendChild("ActorB");
        if (!ActorBNode)
            { SerializeError("Create ActorBNode", SerializableName()); }
        this->GetPivotB().ProtoSerialize(ActorBNode);

        this->TypedConstraint::ProtoSerialize(P2PNode);
    }

    void Point2PointConstraint::ProtoDeSerialize(const xml::Node& OneNode)
    {

    }

    String Point2PointConstraint::SerializableName() const
        { return String("Point2PointConstraint"); }
#endif // /PHYSXML

    /////////////////////////////////////////
    // Slider Constraint Functions

    SliderConstraint::SliderConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA, const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseLinearReferenceA)
    {
        SetBodies(ActorA,ActorB);

        btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
        btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        Slider = new btSliderConstraint(*BodyA, *BodyB, transa, transb, UseLinearReferenceA);
        ConstraintBase = Slider;
    }

    SliderConstraint::SliderConstraint(ActorRigid* ActorB, const Vector3& VectorB, const Quaternion& QuaternionB, bool UseLinearReferenceA)
    {
        SetBodies(ActorB);

        btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        Slider = new btSliderConstraint(*BodyA, transb, UseLinearReferenceA);
        ConstraintBase = Slider;
    }

    SliderConstraint::~SliderConstraint()
    {
        delete Slider;
    }

    void SliderConstraint::SetFrameOffsetALocation(const Vector3& Location)
    {
        this->Slider->getFrameOffsetA().setOrigin(Location.GetBulletVector3());
    }

    void SliderConstraint::SetFrameOffsetBLocation(const Vector3& Location)
    {
        this->Slider->getFrameOffsetB().setOrigin(Location.GetBulletVector3());
    }

    void SliderConstraint::SetUpperLinLimit(Real UpperLimit)
    {
        this->Slider->setUpperLinLimit(UpperLimit);
    }

    void SliderConstraint::SetUpperAngLimit(Real UpperLimit)
    {
        this->Slider->setUpperAngLimit(UpperLimit);
    }

    void SliderConstraint::SetLowerLinLimit(Real LowerLimit)
    {
        this->Slider->setLowerLinLimit(LowerLimit);
    }

    void SliderConstraint::SetLowerAngLimit(Real LowerLimit)
    {
        this->Slider->setLowerAngLimit(LowerLimit);
    }

    void SliderConstraint::SetPoweredLinMotor(bool OnOff)
    {
        this->Slider->setPoweredLinMotor(OnOff);
    }

    void SliderConstraint::SetTargetLinMotorVelocity(Real TargetLinMotorVelocity)
    {
        this->Slider->setTargetLinMotorVelocity(TargetLinMotorVelocity);
    }

    void SliderConstraint::SetMaxLinMotorForce(Real MaxLinMotorForce)
    {
        this->Slider->setMaxLinMotorForce(MaxLinMotorForce);
    }

    void SliderConstraint::SetPoweredAngMotor(bool OnOff)
    {
        this->Slider->setPoweredAngMotor(OnOff);
    }

    void SliderConstraint::SetTargetAngMotorVelocity(Real TargetAngMotorVelocity)
    {
        this->Slider->setTargetAngMotorVelocity(TargetAngMotorVelocity);
    }

    void SliderConstraint::SetMaxAngMotorForce(Real MaxAngMotorForce)
    {
        this->Slider->setMaxAngMotorForce(MaxAngMotorForce);
    }

    void SliderConstraint::SetUseFrameOffset(bool FrameOffset)
    {
        this->Slider->setUseFrameOffset(FrameOffset);
    }

    void SliderConstraint::SetSoftnessDirLin(Real SoftnessDirLin)
    {
        this->Slider->setSoftnessDirLin(SoftnessDirLin);
    }

    void SliderConstraint::SetRestitutionDirLin(Real RestitutionDirLin)
    {
        this->Slider->setRestitutionDirLin(RestitutionDirLin);
    }

    void SliderConstraint::SetDampingDirLin(Real DampingDirLin)
    {
        this->Slider->setDampingDirLin(DampingDirLin);
    }

    void SliderConstraint::SetSoftnessDirAng(Real SoftnessDirAng)
    {
        this->Slider->setSoftnessDirAng(SoftnessDirAng);
    }

    void SliderConstraint::SetRestitutionDirAng(Real RestitutionDirAng)
    {
        this->Slider->setRestitutionDirAng(RestitutionDirAng);
    }

    void SliderConstraint::SetDampingDirAng(Real DampingDirAng)
    {
        this->Slider->setDampingDirAng(DampingDirAng);
    }

    void SliderConstraint::SetSoftnessLimLin(Real SoftnessLimLin)
    {
        this->Slider->setSoftnessLimLin(SoftnessLimLin);
    }

    void SliderConstraint::SetRestitutionLimLin(Real RestitutionLimLin)
    {
        this->Slider->setRestitutionLimLin(RestitutionLimLin);
    }

    void SliderConstraint::SetDampingLimLin(Real DampingLimLin)
    {
        this->Slider->setDampingLimLin(DampingLimLin);
    }

    void SliderConstraint::SetSoftnessLimAng(Real SoftnessLimAng)
    {
        this->Slider->setSoftnessLimAng(SoftnessLimAng);
    }

    void SliderConstraint::SetRestitutionLimAng(Real RestitutionLimAng)
    {
        this->Slider->setRestitutionLimAng(RestitutionLimAng);
    }

    void SliderConstraint::SetDampingLimAng(Real DampingLimAng)
    {
        this->Slider->setDampingLimAng(DampingLimAng);
    }

    void SliderConstraint::SetSoftnessOrthoLin(Real SoftnessOrthoLin)
    {
        this->Slider->setSoftnessOrthoLin(SoftnessOrthoLin);
    }

    void SliderConstraint::SetRestitutionOrthoLin(Real RestitutionOrthoLin)
    {
        this->Slider->setRestitutionOrthoLin(RestitutionOrthoLin);
    }

    void SliderConstraint::SetDampingOrthoLin(Real DampingOrthoLin)
    {
        this->Slider->setDampingOrthoLin(DampingOrthoLin);
    }

    void SliderConstraint::SetSoftnessOrthoAng(Real SoftnessOrthoAng)
    {
        this->Slider->setSoftnessOrthoAng(SoftnessOrthoAng);
    }

    void SliderConstraint::SetRestitutionOrthoAng(Real RestitutionOrthoAng)
    {
        this->Slider->setRestitutionOrthoAng(RestitutionOrthoAng);
    }

    void SliderConstraint::SetDampingOrthoAng(Real DampingOrthoAng)
    {
        this->Slider->setDampingOrthoAng(DampingOrthoAng);
    }

    /////////////////////////////////////////
    // Universal Constraint Functions

    UniversalConstraint::UniversalConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& Anchor, const Vector3& Axis1, const Vector3& Axis2)
    {
        SetBodies(ActorA,ActorB);

        btVector3 temp1(Anchor.GetBulletVector3());
        btVector3 temp2(Axis1.GetBulletVector3());
        btVector3 temp3(Axis2.GetBulletVector3());
        Universal = new btUniversalConstraint(*BodyA, *BodyB, temp1, temp2, temp3);
        Generic6dof = Universal;
        ConstraintBase = Universal;
    }

    UniversalConstraint::~UniversalConstraint()
    {
        delete Universal;
        Universal = NULL;
        Generic6dof = NULL;
    }

    void UniversalConstraint::SetUpperLimit(Real Ang1Max, Real Ang2Max)
    {
        this->Universal->setUpperLimit(Ang1Max, Ang2Max);
    }

    void UniversalConstraint::SetLowerLimit(Real Ang1Min, Real Ang2Min)
    {
        this->Universal->setLowerLimit(Ang1Min, Ang2Min);
    }
}//phys


#ifdef PHYSXML
    ///////////////////////////////////////////////////////////////////////////////
    // Class External << Operators for streaming or assignment
    std::ostream& operator << (std::ostream& stream, const phys::TypedConstraint& x)
    {
        Serialize(stream,x);
        return stream;
    }

    std::istream& operator >> (std::istream& stream, phys::TypedConstraint& x)
        { return DeSerialize(stream, x); }

    void operator >> (const phys::xml::Node& OneNode, phys::TypedConstraint& x)
        { x.ProtoDeSerialize(OneNode); }
#endif // \PHYSXML



/// @endcond

#endif
