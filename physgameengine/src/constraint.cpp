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
#include <btBulletDynamicsCommon.h>

/// @cond 0

namespace phys
{
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

    ActorRigid* TypedConstraint::GetActorA()
    {
        return ActA;
    }

    ActorRigid* TypedConstraint::GetActorB()
    {
        return ActB;
    }

    void TypedConstraint::SetParam(ConstraintParams Param, Real Value, int Axis)
        { this->ConstraintBase->setParam(Param, Value, Axis); }

    Real TypedConstraint::GetParam(ConstraintParams Param, int Axis)
        { return this->ConstraintBase->getParam(Param, Axis); }

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

    HingeConstraint::HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& PivotInA, const Vector3& PivotInB, const Vector3& AxisInA, const Vector3& AxisInB, bool UseReferenceA)
    {
        SetBodies(ActorA,ActorB);

        btVector3 tempA(AxisInA.GetBulletVector3());
        btVector3 tempB(AxisInB.GetBulletVector3());
        Hinge = new btHingeConstraint(*BodyA, *BodyB, PivotInA.GetBulletVector3(), PivotInB.GetBulletVector3(), tempA, tempB, bool(UseReferenceA));
        ConstraintBase = Hinge;
    }

    HingeConstraint::HingeConstraint(ActorRigid* ActorA, const Vector3& PivotInA, const Vector3& AxisInA, bool UseReferenceA)
    {
        SetBodies(ActorA);

        btVector3 tempA(AxisInA.GetBulletVector3());
        Hinge = new btHingeConstraint(*BodyA, PivotInA.GetBulletVector3(), tempA, bool(UseReferenceA));
        ConstraintBase = Hinge;
    }

    HingeConstraint::HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA, const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseReferenceA)
    {
        SetBodies(ActorA,ActorB);

        btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
        btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        Hinge = new btHingeConstraint(*BodyA, *BodyB, transa, transb, UseReferenceA);
        ConstraintBase = Hinge;
    }

    HingeConstraint::~HingeConstraint()
    {
        if(Hinge)
            delete Hinge;
    }

    void HingeConstraint::SetAPivotLocation(const Vector3& Location)
    {
        this->Hinge->getAFrame().setOrigin(Location.GetBulletVector3());
    }

    void HingeConstraint::SetBPivotLocation(const Vector3& Location)
    {
        this->Hinge->getBFrame().setOrigin(Location.GetBulletVector3());
    }

    void HingeConstraint::SetAngularOnly(bool AngularOnly)
    {
        this->Hinge->setAngularOnly(AngularOnly);
    }

    void HingeConstraint::EnableAngularMotor(bool EnableMotor, Real TargetVelocity, Real MaxMotorImpulse)
    {
        this->Hinge->enableAngularMotor(EnableMotor, TargetVelocity, MaxMotorImpulse);
    }

    void HingeConstraint::EnableMotor(bool EnableMotor)
    {
        this->Hinge->enableMotor(EnableMotor);
    }

    void HingeConstraint::SetMaxMotorImpulse(Real MaxMotorImpulse)
    {
        this->Hinge->setMaxMotorImpulse(MaxMotorImpulse);
    }

    void HingeConstraint::SetMotorTarget(const Quaternion& QuatAInB, Real Dt)
    {
        this->Hinge->setMotorTarget(QuatAInB.GetBulletQuaternion(), Dt);
    }

    void HingeConstraint::SetMotorTarget(Real TargetAngle, Real Dt)
    {
        this->Hinge->setMotorTarget(TargetAngle, Dt);
    }

    void HingeConstraint::SetLimit(Real Low, Real High, Real Softness, Real BiasFactor, Real RelaxationFactor)
    {
        this->Hinge->setLimit(Low, High, Softness, BiasFactor, RelaxationFactor);
    }

    void HingeConstraint::SetAxis(const Vector3& AxisInA)
    {
        btVector3 temp(AxisInA.GetBulletVector3());
        this->Hinge->setAxis(temp);
    }

    void HingeConstraint::SetUseFrameOffset(bool FrameOffset)
    {
        this->Hinge->setUseFrameOffset(FrameOffset);
    }

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

    Vector3 Point2PointConstraint::GetPivotInA()
    {
        Vector3 Pivot(this->Point2Point->getPivotInA());
        return Pivot;
    }

    Vector3 Point2PointConstraint::GetPivotInB()
    {
        Vector3 Pivot(this->Point2Point->getPivotInB());
        return Pivot;
    }

    void Point2PointConstraint::SetImpulseClamping(Real Clamping)
    {
        this->Point2Point->m_setting.m_impulseClamp = Clamping;
    }

    void Point2PointConstraint::SetTAU(Real TAU)
    {
        this->Point2Point->m_setting.m_tau = TAU;
    }

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

/// @endcond

#endif
