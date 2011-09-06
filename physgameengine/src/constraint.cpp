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

    ////////////////////////////////////////////////////////////////////////////////
    // TypedConstraint Protected Methods
    TypedConstraint::TypedConstraint()
        { }

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

    ////////////////////////////////////////////////////////////////////////////////
    // TypedConstraint Core Functionality
    TypedConstraint::~TypedConstraint()
        { }

    ActorRigid* TypedConstraint::GetActorA() const
        { return ActA; }

    ActorRigid* TypedConstraint::GetActorB() const
        { return ActB; }

    ///////////////////////////////////////////////////////////////////////////////
    // TypedConstraint Parameters
    void TypedConstraint::SetParam(ConstraintParam Param, Real Value, int Axis)
        { this->GetConstraintBase()->setParam(Param, Value, Axis); }

    Real TypedConstraint::GetParam(ConstraintParam Param, int Axis) const
        { return this->GetConstraintBase()->getParam(Param, Axis); }

#ifdef PHYSXML
    ///////////////////////////////////////////////////////////////////////////////
    // TypedConstraint Serialization
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
#endif // /PHYSXML

    ///////////////////////////////////////////////////////////////////////////////
    // DualTransformConstraint Serialization

    // Serializable
    void DualTransformConstraint::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node DualTransformConstraintNode = CurrentRoot.AppendChild(SerializableName());                     // The base node all the base constraint stuff will go in
        if (!DualTransformConstraintNode)
            { SerializeError("Create DualTransformConstraintNode", SerializableName()); }

        xml::Attribute Version = DualTransformConstraintNode.AppendAttribute("Version");
        if (Version)
        {
            Version.SetValue(1);
        }else{
            SerializeError("Create Attributes on DualTransformConstraintNode", SerializableName());
        }

        xml::Node ActorANode = DualTransformConstraintNode.AppendChild("ActorA");                     // Get everything we need about ActorA
        if (!ActorANode)
            { SerializeError("Create ActorANode", SerializableName()); }
        this->GetPivotATransform().ProtoSerialize(ActorANode);

        xml::Node ActorBNode = DualTransformConstraintNode.AppendChild("ActorB");                     // Get everything we need about ActorB
        if (!ActorBNode)
            { SerializeError("Create ActorBNode", SerializableName()); }
        this->GetPivotBTransform().ProtoSerialize(ActorBNode);

        this->TypedConstraint::ProtoSerialize(DualTransformConstraintNode);
    }

    // DeSerializable
    void DualTransformConstraint::ProtoDeSerialize(const xml::Node& OneNode)
    {
    }

    String DualTransformConstraint::SerializableName() const
        { return String("DualTransformConstraint"); }

    /////////////////////////////////////////
    // ConeTwist Constraint Functions

    ConeTwistConstraint::ConeTwistConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA,
                                              const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB)
    {
        SetBodies(ActorA,ActorB);

        btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
        btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        ConeTwist = new btConeTwistConstraint (*BodyA, *BodyB, transa, transb);
    }

    ConeTwistConstraint::ConeTwistConstraint(ActorRigid* ActorA, const Vector3& VectorA, const Quaternion& QuaternionA)
    {
        SetBodies(ActorA);

        btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
        ConeTwist = new btConeTwistConstraint (*BodyA, transa);
    }

    ConeTwistConstraint::~ConeTwistConstraint()
    {
        delete ConeTwist;
    }

    btTypedConstraint* ConeTwistConstraint::GetConstraintBase() const
                { return this->ConeTwist; }

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

    ////////////////////////////////////////////////////////////////////////////////
    // Generic6Dof Constraint Functions
    Generic6DofConstraint::Generic6DofConstraint()
        { }

    btTypedConstraint* Generic6DofConstraint::GetConstraintBase() const
        { return this->Generic6dof; }

    ////////////////////////////////////////////////////////////////////////////////
    // Generic6DofConstraint Construction and Destruction
    Generic6DofConstraint::Generic6DofConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA,
                                                  const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseLinearReferenceA)
    {
        SetBodies(ActorA,ActorB);
        Transform TransformA(VectorA, QuaternionA);
        Transform TransformB(VectorB, QuaternionB);
        Generic6dof = new btGeneric6DofConstraint(*BodyA, *BodyB, TransformA.GetBulletTransform(), TransformB.GetBulletTransform(), UseLinearReferenceA);
    }

    Generic6DofConstraint::Generic6DofConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Transform& TransformA, const Transform& TransformB, bool UseLinearReferenceA)
    {
        SetBodies(ActorA,ActorB);
        Generic6dof = new btGeneric6DofConstraint(*BodyA, *BodyB, TransformA.GetBulletTransform(), TransformB.GetBulletTransform(), UseLinearReferenceA);
    }

    Generic6DofConstraint::Generic6DofConstraint(ActorRigid* ActorB, const Vector3& VectorB, const Quaternion& QuaternionB, bool UseLinearReferenceB)
    {
        SetBodies(ActorB);

        btTransform transa(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        Generic6dof = new btGeneric6DofConstraint(*BodyA, transa, UseLinearReferenceB);
    }

    Generic6DofConstraint::Generic6DofConstraint(ActorRigid* ActorB, const Transform& TransformB, bool UseLinearReferenceB)
    {
        SetBodies(ActorB);

        Generic6dof = new btGeneric6DofConstraint(*BodyA, TransformB.GetBulletTransform(), UseLinearReferenceB);
    }

    Generic6DofConstraint::~Generic6DofConstraint()
    {
        if(Generic6dof)
            delete Generic6dof;
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
    void Generic6DofConstraint::SetLinearUpperLimit(const Vector3& Limit)
        { this->Generic6dof->setLinearUpperLimit(Limit.GetBulletVector3()); }

    void Generic6DofConstraint::SetLinearLowerLimit(const Vector3& Limit)
        { this->Generic6dof->setLinearLowerLimit(Limit.GetBulletVector3()); }

    Vector3 Generic6DofConstraint::GetLinearUpperLimit() const
        { return Vector3(this->Generic6dof->getTranslationalLimitMotor()->m_upperLimit); }

    Vector3 Generic6DofConstraint::GetLinearLowerLimit() const
        { return Vector3(this->Generic6dof->getTranslationalLimitMotor()->m_lowerLimit); }

    void Generic6DofConstraint::SetAngularUpperLimit(const Vector3& Limit)
        { this->Generic6dof->setAngularUpperLimit(Limit.GetBulletVector3()); }

    void Generic6DofConstraint::SetAngularLowerLimit(const Vector3& Limit)
        { this->Generic6dof->setAngularLowerLimit(Limit.GetBulletVector3()); }

    Vector3 Generic6DofConstraint::GetAngularUpperLimit() const
        { return Vector3(GetAngularUpperLimitOnAxis(0),GetAngularUpperLimitOnAxis(1),GetAngularUpperLimitOnAxis(2)); }

    Vector3 Generic6DofConstraint::GetAngularLowerLimit() const
        { return Vector3(GetAngularLowerLimitOnAxis(0),GetAngularLowerLimitOnAxis(1),GetAngularLowerLimitOnAxis(2)); }

    Real Generic6DofConstraint::GetAngularLowerLimitOnAxis(int RotationalAxis) const
        { return this->Generic6dof->getRotationalLimitMotor(AxisToRotAxis(RotationalAxis))->m_loLimit; }

    Real Generic6DofConstraint::GetAngularUpperLimitOnAxis(int RotationalAxis) const
        { return this->Generic6dof->getRotationalLimitMotor(AxisToRotAxis(RotationalAxis))->m_hiLimit; }

    ////////////////////////////////////////////////////////////////////////////////
    // Generic6DofConstraint Detailed Limits and Motors
    void Generic6DofConstraint::SetLimit(int Axis, Real Lower, Real Upper)
        { this->Generic6dof->setLimit(Axis, Lower, Upper); }

    void Generic6DofConstraint::SetAngularMotorTargetVelocity(const Vector3& Velocities)
        { SetAngularMotorTargetVelocityOnAxis(Velocities.X,0);  SetAngularMotorTargetVelocityOnAxis(Velocities.Y,1);  SetAngularMotorTargetVelocityOnAxis(Velocities.Z,2);}

    void Generic6DofConstraint::SetAngularMotorTargetVelocityOnAxis(Real Velocity, int Axis)
        { this->Generic6dof->getRotationalLimitMotor(AxisToRotAxis(Axis))->m_targetVelocity = Velocity; }

    Vector3 Generic6DofConstraint::GetAngularMotorTargetVelocity() const
        { return Vector3(GetAngularMotorTargetVelocityOnAxis(0), GetAngularMotorTargetVelocityOnAxis(1), GetAngularMotorTargetVelocityOnAxis(2));}

    Real Generic6DofConstraint::GetAngularMotorTargetVelocityOnAxis(int Axis) const
        { return this->Generic6dof->getRotationalLimitMotor(AxisToRotAxis(Axis))->m_targetVelocity; }


    void Generic6DofConstraint::SetAngularMotorMaxMotorForce(const Vector3& Forces)
        { SetAngularMotorMaxMotorForceOnAxis(Forces.X,0);  SetAngularMotorMaxMotorForceOnAxis(Forces.Y,1);  SetAngularMotorMaxMotorForceOnAxis(Forces.Z,2);}

    void Generic6DofConstraint::SetAngularMotorMaxMotorForceOnAxis(Real Force, int Axis)
        { this->Generic6dof->getRotationalLimitMotor(AxisToRotAxis(Axis))->m_maxMotorForce = Force; }

    Vector3 Generic6DofConstraint::GetAngularMotorMaxMotorForce() const
        { return Vector3(GetAngularMotorMaxMotorForceOnAxis(0), GetAngularMotorMaxMotorForceOnAxis(1), GetAngularMotorMaxMotorForceOnAxis(2));}

    Real Generic6DofConstraint::GetAngularMotorMaxMotorForceOnAxis(int Axis) const
        { return this->Generic6dof->getRotationalLimitMotor(AxisToRotAxis(Axis))->m_maxMotorForce; }


    void Generic6DofConstraint::SetAngularMotorDamping(const Vector3& Dampings)
        { SetAngularMotorDampingOnAxis(Dampings.X,0);  SetAngularMotorDampingOnAxis(Dampings.Y,1);  SetAngularMotorDampingOnAxis(Dampings.Z,2);}

    void Generic6DofConstraint::SetAngularMotorDampingOnAxis(Real Damping, int Axis)
        { this->Generic6dof->getRotationalLimitMotor(AxisToRotAxis(Axis))->m_damping = Damping; }

    Vector3 Generic6DofConstraint::GetAngularMotorDamping() const
        { return Vector3(GetAngularMotorDampingOnAxis(0), GetAngularMotorDampingOnAxis(1), GetAngularMotorDampingOnAxis(2));}

    Real Generic6DofConstraint::GetAngularMotorDampingOnAxis(int Axis) const
        { return this->Generic6dof->getRotationalLimitMotor(AxisToRotAxis(Axis))->m_damping; }


    void Generic6DofConstraint::SetAngularMotorRestitution(const Vector3& Restistutions)
        { SetAngularMotorRestitutionOnAxis(Restistutions.X,0);  SetAngularMotorRestitutionOnAxis(Restistutions.Y,1);  SetAngularMotorRestitutionOnAxis(Restistutions.Z,2);}

    void Generic6DofConstraint::SetAngularMotorRestitutionOnAxis(Real Restistution, int Axis)
        { this->Generic6dof->getRotationalLimitMotor(AxisToRotAxis(Axis))->m_bounce = Restistution; }

    Vector3 Generic6DofConstraint::GetAngularMotorRestitution() const
        { return Vector3(GetAngularMotorRestitutionOnAxis(0), GetAngularMotorRestitutionOnAxis(1), GetAngularMotorRestitutionOnAxis(2));}

    Real Generic6DofConstraint::GetAngularMotorRestitutionOnAxis(int Axis) const
        { return this->Generic6dof->getRotationalLimitMotor(AxisToRotAxis(Axis))->m_bounce; }


    void Generic6DofConstraint::SetAngularMotorEnabled(const Vector3& Enableds)
        { SetAngularMotorEnabledOnAxis(Enableds.X,0);  SetAngularMotorEnabledOnAxis(Enableds.Y,1);  SetAngularMotorEnabledOnAxis(Enableds.Z,2);}

    void Generic6DofConstraint::SetAngularMotorEnabledOnAxis(bool Enabled, int Axis)
        { this->Generic6dof->getRotationalLimitMotor(AxisToRotAxis(Axis))->m_enableMotor = (Enabled!=0); }

    Vector3 Generic6DofConstraint::GetAngularMotorEnabled() const
        { return Vector3(GetAngularMotorEnabledOnAxis(0), GetAngularMotorEnabledOnAxis(1), GetAngularMotorEnabledOnAxis(2));}

    bool Generic6DofConstraint::GetAngularMotorEnabledOnAxis(int Axis) const
        { return this->Generic6dof->getRotationalLimitMotor(AxisToRotAxis(Axis))->m_enableMotor; }




    ////////////////////////////////////////////////////////////////////////////////
    // Generic6DofConstraint Axis, Params and other Details
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

    bool Generic6DofConstraint::GetUseFrameOffset() const
        { this->Generic6dof->getUseFrameOffset(); }
    void Generic6DofConstraint::SetUseFrameOffset(bool FrameOffset)
        { this->Generic6dof->setUseFrameOffset(FrameOffset); }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization
#ifdef PHYSXML
    void Generic6DofConstraint::ProtoSerialize(xml::Node& CurrentRoot) const
    {

        xml::Node G6dofNode = CurrentRoot.AppendChild(SerializableName());                     // The base node all the base constraint stuff will go in
        if (!G6dofNode)
            { SerializeError("Create G6dofNode", SerializableName()); }

        // Linear
            // Upper limit
            // Lower Limit

        // Angular
            // Upper limit
            // Lower Limit

        // Linear and angular Motors
            // The whole Motor Class for each




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
        this->GetPivotB().ProtoSerialize(ActorBNode);*/

        this->DualTransformConstraint::ProtoSerialize(G6dofNode);
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

    bool HingeConstraint::GetUseReferenceFrameA() const
        { return this->Hinge->m_useReferenceFrameA; }

    void HingeConstraint::SetUseReferenceFrameA(bool UseReferenceFrameA)
        { this->Hinge->m_useReferenceFrameA = UseReferenceFrameA; }

    bool HingeConstraint::GetUseFrameOffset() const
        { return this->Hinge->getUseFrameOffset(); }

    void HingeConstraint::SetUseFrameOffset(bool FrameOffset)
        { this->Hinge->setUseFrameOffset(FrameOffset); }


#ifdef PHYSXML
    ////////////////////////////////////////////////////////////////////////////////
    // HingeConstraint Serialization
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
            FrameOff.SetValue(this->GetUseFrameOffset());
        }else{
            SerializeError("Create HingeNode Attributes", SerializableName());
        }

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

        DualTransformConstraint::ProtoSerialize(HingeNode);
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

    ////////////////////////////////////////////////////////////////////////////////
    // Point2PointConstraint Position and Orientation
    Point2PointConstraint::Point2PointConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& PivotA, const Vector3& PivotB)
    {
        SetBodies(ActorA,ActorB);

        Point2Point = new btPoint2PointConstraint(*BodyA, *BodyB, PivotA.GetBulletVector3(), PivotB.GetBulletVector3());
    }

    Point2PointConstraint::Point2PointConstraint(ActorRigid* ActorA, const Vector3& PivotA)
    {
        SetBodies(ActorA);

        Point2Point = new btPoint2PointConstraint(*BodyA, PivotA.GetBulletVector3());
    }

    Point2PointConstraint::~Point2PointConstraint()
        { delete Point2Point; }

    btTypedConstraint* Point2PointConstraint::GetConstraintBase() const
        { return this->Point2Point; }
    ////////////////////////////////////////////////////////////////////////////////
    // Point2PointConstraint Position and Orientation
    void Point2PointConstraint::SetPivotALocation(const Vector3& PivotA)
        { this->Point2Point->setPivotA(PivotA.GetBulletVector3()); }

    void Point2PointConstraint::SetPivotBLocation(const Vector3& PivotB)
        { this->Point2Point->setPivotB(PivotB.GetBulletVector3()); }

    Vector3 Point2PointConstraint::GetPivotALocation() const
        { return Vector3(this->Point2Point->getPivotInA()); }

    Vector3 Point2PointConstraint::GetPivotBLocation() const
        { return Vector3(this->Point2Point->getPivotInB()); }

    ////////////////////////////////////////////////////////////////////////////////
    // Point2PointConstraint Specific Physics Settings
    void Point2PointConstraint::SetImpulseClamping(Real Clamping)
        { this->Point2Point->m_setting.m_impulseClamp = Clamping; }

    Real Point2PointConstraint::GetImpulseClamping() const
        { return this->Point2Point->m_setting.m_impulseClamp; }

    void Point2PointConstraint::SetDamping(Real Damping)
        { this->Point2Point->m_setting.m_damping = Damping; }

    Real Point2PointConstraint::GetDamping() const
        { return this->Point2Point->m_setting.m_damping; }

    void Point2PointConstraint::SetTAU(Real TAU)
        { this->Point2Point->m_setting.m_tau = TAU; }

    Real Point2PointConstraint::GetTAU() const
        { return this->Point2Point->m_setting.m_tau; }

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
        //return this->Point2Point->hasParamBeenSet(Param,Axis);
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
        this->GetPivotALocation().ProtoSerialize(ActorANode);

        xml::Node ActorBNode = P2PNode.AppendChild("ActorB");
        if (!ActorBNode)
            { SerializeError("Create ActorBNode", SerializableName()); }
        this->GetPivotBLocation().ProtoSerialize(ActorBNode);

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
    }

    SliderConstraint::SliderConstraint(ActorRigid* ActorB, const Vector3& VectorB, const Quaternion& QuaternionB, bool UseLinearReferenceA)
    {
        SetBodies(ActorB);

        btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        Slider = new btSliderConstraint(*BodyA, transb, UseLinearReferenceA);
    }

    SliderConstraint::~SliderConstraint()
    {
        delete Slider;
    }

    btTypedConstraint* SliderConstraint::GetConstraintBase() const
        { return this->Slider; }

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
