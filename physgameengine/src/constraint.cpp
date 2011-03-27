//© Copyright 2010 BlackTopp Studios Inc.
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

    TypedConstraint::TypedConstraint ()
    {
    }

    TypedConstraint::TypedConstraint (ActorRigid* bodya, ActorRigid* bodyb)
    {
        ActorA=bodya;
        ActorB=bodyb;
        BodyA=bodya->physrigidbody;
        BodyB=bodyb->physrigidbody;
        ConstraintBase=NULL;
    }

    TypedConstraint::TypedConstraint (ActorRigid* bodya)
    {
        ActorA=bodya;
        ActorB=NULL;
        BodyA=bodya->physrigidbody;
        BodyB=NULL;
        ConstraintBase=NULL;
    }

    TypedConstraint::~TypedConstraint ()
    {
    }

    ActorRigid* TypedConstraint::GetActorA()
    {
        return ActorA;
    }

    ActorRigid* TypedConstraint::GetActorB()
    {
        return ActorB;
    }

    /////////////////////////////////////////
    // ConeTwist Constraint Functions

    ConeTwistConstraint::ConeTwistConstraint (ActorRigid* ActorA, ActorRigid* ActorB, Vector3 VectorA,
                                              Vector3 VectorB, Quaternion QuaternionA, Quaternion QuaternionB) : TypedConstraint (ActorA, ActorB)
    {
        btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
        btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        ConeTwist = new btConeTwistConstraint (*BodyA, *BodyB, transa, transb);
        ConstraintBase = ConeTwist;
    }

    ConeTwistConstraint::ConeTwistConstraint (ActorRigid* ActorA, Vector3 VectorA, Quaternion QuaternionA) : TypedConstraint (ActorA)
    {
        btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
        ConeTwist = new btConeTwistConstraint (*BodyA, transa);
        ConstraintBase = ConeTwist;
    }

    ConeTwistConstraint::ConeTwistConstraint (btConeTwistConstraint* Constraint)
    {
        ConeTwist = Constraint;
        ConstraintBase = Constraint;
    }

    ConeTwistConstraint::~ConeTwistConstraint ()
    {
        delete ConeTwist;
    }

    void ConeTwistConstraint::SetAngularOnly (bool AngularOnly)
    {
        this->ConeTwist->setAngularOnly(AngularOnly);
    }

    void ConeTwistConstraint::SetLimit (int LimitIndex, Real LimitValue)
    {
        this->ConeTwist->setLimit(LimitIndex, LimitValue);
    }

    void ConeTwistConstraint::SetLimit (Real SwingSpan1, Real SwingSpan2, Real Twistspan,
                                        Real Softness, Real BiasFactor, Real RelaxationFactor)
    {
        this->ConeTwist->setLimit(SwingSpan1, SwingSpan2, Twistspan, Softness, BiasFactor, RelaxationFactor);
    }

    void ConeTwistConstraint::SetDamping (Real Damping)
    {
        this->ConeTwist->setDamping(Damping);
    }

    void ConeTwistConstraint::SetMaxMotorImpulse (Real MaxMotorImpulse)
    {
        this->ConeTwist->setMaxMotorImpulse(MaxMotorImpulse);
    }

    void ConeTwistConstraint::SetMaxMotorImpulseNormalized (Real MaxMotorImpulse)
    {
        this->ConeTwist->setMaxMotorImpulseNormalized(MaxMotorImpulse);
    }

    void ConeTwistConstraint::SetFixThresh (Real FixThresh)
    {
        this->ConeTwist->setFixThresh(FixThresh);
    }

    void ConeTwistConstraint::SetMotorTarget (Quaternion Quat)
    {
        this->ConeTwist->setMotorTarget(Quat.GetBulletQuaternion());
    }

    void ConeTwistConstraint::SetMotorTargetInConstraintSpace (Quaternion Quat)
    {
        this->ConeTwist->setMotorTargetInConstraintSpace(Quat.GetBulletQuaternion());
    }

    void ConeTwistConstraint::EnableMotor (bool Enable)
    {
        this->ConeTwist->enableMotor(Enable);
    }

    bool ConeTwistConstraint::IsPassedSwingLimit ()
    {
        return this->ConeTwist->isPastSwingLimit();
    }

    void ConeTwistConstraint::SetParam (int num, Real value, int axis)
    {
        this->ConeTwist->setParam(num, value, axis);
    }

    Real ConeTwistConstraint::GetParam (int num, int axis)
    {
        return this->ConeTwist->getParam(num, axis);
    }

    /////////////////////////////////////////
    // Generic6Dof Constraint Functions

    Generic6DofConstraint::Generic6DofConstraint ()
    {
    }

    Generic6DofConstraint::Generic6DofConstraint (ActorRigid* ActorA, ActorRigid* ActorB) : TypedConstraint (ActorA, ActorB)
    {
    }

    Generic6DofConstraint::Generic6DofConstraint (ActorRigid* ActorA, ActorRigid* ActorB, Vector3 VectorA,
                                                  Vector3 VectorB, Quaternion QuaternionA, Quaternion QuaternionB, bool UseLinearReferenceA) : TypedConstraint (ActorA, ActorB)
    {
        btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
        btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        Generic6dof = new btGeneric6DofConstraint(*BodyA, *BodyB, transa, transb, UseLinearReferenceA);
        ConstraintBase = Generic6dof;
    }

    Generic6DofConstraint::Generic6DofConstraint (ActorRigid* ActorB, Vector3 VectorB, Quaternion QuaternionB, bool UseLinearReferenceB) : TypedConstraint (ActorB)
    {
        btTransform transa(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        Generic6dof = new btGeneric6DofConstraint(*BodyA, transa, UseLinearReferenceB);
        ConstraintBase = Generic6dof;
    }

    Generic6DofConstraint::Generic6DofConstraint (btGeneric6DofConstraint* Constraint)
    {
        Generic6dof = Constraint;
        ConstraintBase = Constraint;
    }

    Generic6DofConstraint::~Generic6DofConstraint ()
    {
        delete Generic6dof;
    }

    void Generic6DofConstraint::SetOffsetALocation (Vector3 Location)
    {
        this->Generic6dof->getFrameOffsetA().setOrigin(Location.GetBulletVector3());
    }

    void Generic6DofConstraint::SetOffsetBLocation (Vector3 Location)
    {
        this->Generic6dof->getFrameOffsetB().setOrigin(Location.GetBulletVector3());
    }

    void Generic6DofConstraint::SetLinearUpperLimit (Vector3 Limit)
    {
        this->Generic6dof->setLinearUpperLimit(Limit.GetBulletVector3());
    }

    void Generic6DofConstraint::SetLinearLowerLimit (Vector3 Limit)
    {
        this->Generic6dof->setLinearLowerLimit(Limit.GetBulletVector3());
    }

    void Generic6DofConstraint::SetAngularUpperLimit (Vector3 Limit)
    {
        this->Generic6dof->setAngularUpperLimit(Limit.GetBulletVector3());
    }

    void Generic6DofConstraint::SetAngularLowerLimit (Vector3 Limit)
    {
        this->Generic6dof->setAngularLowerLimit(Limit.GetBulletVector3());
    }

    void Generic6DofConstraint::SetUseFrameOffset (bool UseOffset)
    {
        this->Generic6dof->setUseFrameOffset(UseOffset);
    }

    void Generic6DofConstraint::SetLimit (int Axis, Real Low, Real High)
    {
        this->Generic6dof->setLimit(Axis, Low, High);
    }

    void Generic6DofConstraint::CalculateTransforms ()
    {
        this->Generic6dof->calculateTransforms();
    }

    void Generic6DofConstraint::SetParam (int num, Real value, int axis)
    {
        this->Generic6dof->setParam(num, value, axis);
    }

    Real Generic6DofConstraint::GetParam (int num, int axis)
    {
        return this->Generic6dof->getParam (num, axis);
    }

    /////////////////////////////////////////
    // Generic6Dof Spring Constraint Functions

    Generic6DofSpringConstraint::Generic6DofSpringConstraint ()
    {
    }

    Generic6DofSpringConstraint::Generic6DofSpringConstraint (ActorRigid* ActorA, ActorRigid* ActorB) : Generic6DofConstraint (ActorA, ActorB)
    {
    }

    Generic6DofSpringConstraint::Generic6DofSpringConstraint (ActorRigid* ActorA, ActorRigid* ActorB, Vector3 VectorA,
                                                              Vector3 VectorB, Quaternion QuaternionA, Quaternion QuaternionB, bool UseLinearReferenceA) : Generic6DofConstraint (ActorA, ActorB)
    {
        btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
        btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        Generic6dofSpring = new btGeneric6DofSpringConstraint(*BodyA, *BodyB, transa, transb, UseLinearReferenceA);
        Generic6dof = Generic6dofSpring;
        ConstraintBase = Generic6dofSpring;
    }

    Generic6DofSpringConstraint::Generic6DofSpringConstraint (btGeneric6DofSpringConstraint* Constraint)
    {
        Generic6dofSpring = Constraint;
        Generic6dof = Constraint;
        ConstraintBase = Constraint;
    }

    Generic6DofSpringConstraint::~Generic6DofSpringConstraint ()
    {
        delete Generic6dofSpring;
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

    HingeConstraint::HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 PivotInA, Vector3 PivotInB, Vector3 AxisInA, Vector3 AxisInB, bool UseReferenceA) : TypedConstraint (ActorA, ActorB)
    {
        btVector3 tempA(AxisInA.GetBulletVector3());
        btVector3 tempB(AxisInB.GetBulletVector3());
        Hinge = new btHingeConstraint(*BodyA, *BodyB, PivotInA.GetBulletVector3(), PivotInB.GetBulletVector3(), tempA, tempB, bool(UseReferenceA));
        ConstraintBase = Hinge;
    }

    HingeConstraint::HingeConstraint(ActorRigid* ActorA, Vector3 PivotInA, Vector3 AxisInA, bool UseReferenceA) : TypedConstraint (ActorA)
    {
        btVector3 tempA(AxisInA.GetBulletVector3());
        Hinge = new btHingeConstraint(*BodyA, PivotInA.GetBulletVector3(), tempA, bool(UseReferenceA));
        ConstraintBase = Hinge;
    }

    HingeConstraint::HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 VectorA, Vector3 VectorB, Quaternion QuaternionA, Quaternion QuaternionB, bool UseReferenceA) : TypedConstraint (ActorA, ActorB)
    {
        btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
        btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        Hinge = new btHingeConstraint(*BodyA, *BodyB, transa, transb, UseReferenceA);
        ConstraintBase = Hinge;
    }

    HingeConstraint::HingeConstraint(btHingeConstraint* Constraint)
    {
        Hinge = Constraint;
        ConstraintBase = Constraint;
    }

    HingeConstraint::~HingeConstraint()
    {
        delete Hinge;
    }

    void HingeConstraint::SetAPivotLocation(Vector3 Location)
    {
        this->Hinge->getAFrame().setOrigin(Location.GetBulletVector3());
    }

    void HingeConstraint::SetBPivotLocation(Vector3 Location)
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

    void HingeConstraint::SetMotorTarget(Quaternion QuatAInB, Real Dt)
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

    void HingeConstraint::SetAxis(Vector3 AxisInA)
    {
        btVector3 temp(AxisInA.GetBulletVector3());
        this->Hinge->setAxis(temp);
    }

    void HingeConstraint::SetUseFrameOffset(bool FrameOffset)
    {
        this->Hinge->setUseFrameOffset(FrameOffset);
    }

    void HingeConstraint::SetParam(int num, Real value, int axis)
    {
        this->Hinge->setParam(num, value, axis);
    }

    Real HingeConstraint::GetParam(int num, int axis)
    {
        return this->Hinge->getParam(num, axis);
    }

    /////////////////////////////////////////
    // Hinge2 Constraint Functions

    Hinge2Constraint::Hinge2Constraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 Anchor, Vector3 Axis1, Vector3 Axis2) : Generic6DofSpringConstraint(ActorA, ActorB)
    {
        btVector3 temp1(Anchor.GetBulletVector3());
        btVector3 temp2(Axis1.GetBulletVector3());
        btVector3 temp3(Axis2.GetBulletVector3());
        Hinge2 = new btHinge2Constraint(*BodyA, *BodyB, temp1, temp2, temp3);
        Generic6dofSpring = Hinge2;
        Generic6dof = Hinge2;
        ConstraintBase = Hinge2;
    }

    Hinge2Constraint::Hinge2Constraint(btHinge2Constraint* Constraint)
    {
        Hinge2 = Constraint;
        Generic6dofSpring = Constraint;
        Generic6dof = Constraint;
        ConstraintBase = Constraint;
    }

    Hinge2Constraint::~Hinge2Constraint()
    {
        delete Hinge2;
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

    Point2PointConstraint::Point2PointConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 PivotA, Vector3 PivotB) : TypedConstraint(ActorA, ActorB)
    {
        Point2Point = new btPoint2PointConstraint(*BodyA, *BodyB, PivotA.GetBulletVector3(), PivotB.GetBulletVector3());
        ConstraintBase = Point2Point;
    }

    Point2PointConstraint::Point2PointConstraint(ActorRigid* ActorA, Vector3 PivotA) : TypedConstraint(ActorA)
    {
        Point2Point = new btPoint2PointConstraint(*BodyA, PivotA.GetBulletVector3());
        ConstraintBase = Point2Point;
    }

    Point2PointConstraint::Point2PointConstraint(btPoint2PointConstraint* Constraint)
    {
        Point2Point = Constraint;
        ConstraintBase = Constraint;
    }

    Point2PointConstraint::~Point2PointConstraint()
    {
        delete Point2Point;
    }

    void Point2PointConstraint::SetPivotA(Vector3 PivotA)
    {
        this->Point2Point->setPivotA(PivotA.GetBulletVector3());
    }

    void Point2PointConstraint::SetPivotB(Vector3 PivotB)
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

    void Point2PointConstraint::SetParam(int num, Real value, int axis)
    {
        this->Point2Point->setParam(num, value, axis);
    }

    Real Point2PointConstraint::GetParam(int num, int axis)
    {
        return this->Point2Point->getParam(num, axis);
    }

    /////////////////////////////////////////
    // Slider Constraint Functions

    SliderConstraint::SliderConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 VectorA, Vector3 VectorB, Quaternion QuaternionA, Quaternion QuaternionB, bool UseLinearReferenceA) : TypedConstraint(ActorA, ActorB)
    {
        btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
        btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        Slider = new btSliderConstraint(*BodyA, *BodyB, transa, transb, UseLinearReferenceA);
        ConstraintBase = Slider;
    }

    SliderConstraint::SliderConstraint(ActorRigid* ActorB, Vector3 VectorB, Quaternion QuaternionB, bool UseLinearReferenceA) : TypedConstraint(ActorB)
    {
        btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
        Slider = new btSliderConstraint(*BodyA, transb, UseLinearReferenceA);
        ConstraintBase = Slider;
    }

    SliderConstraint::SliderConstraint(btSliderConstraint* Constraint)
    {
        Slider = Constraint;
        ConstraintBase = Slider;
    }

    SliderConstraint::~SliderConstraint()
    {
        delete Slider;
    }

    void SliderConstraint::SetFrameOffsetALocation(Vector3 Location)
    {
        this->Slider->getFrameOffsetA().setOrigin(Location.GetBulletVector3());
    }

    void SliderConstraint::SetFrameOffsetBLocation(Vector3 Location)
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

    void SliderConstraint::SetParam(int num, Real value, int axis)
    {
        this->Slider->setParam(num, value, axis);
    }

    Real SliderConstraint::GetParam(int num, int axis)
    {
        return this->Slider->getParam(num, axis);
    }

    /////////////////////////////////////////
    // Universal Constraint Functions

    UniversalConstraint::UniversalConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 Anchor, Vector3 Axis1, Vector3 Axis2) : Generic6DofConstraint(ActorA, ActorB)
    {
        btVector3 temp1(Anchor.GetBulletVector3());
        btVector3 temp2(Axis1.GetBulletVector3());
        btVector3 temp3(Axis2.GetBulletVector3());
        Universal = new btUniversalConstraint(*BodyA, *BodyB, temp1, temp2, temp3);
        Generic6dof = Universal;
        ConstraintBase = Universal;
    }

    UniversalConstraint::UniversalConstraint(btUniversalConstraint* Constraint)
    {
        Universal = Constraint;
        Generic6dof = Constraint;
        ConstraintBase = Constraint;
    }

    UniversalConstraint::~UniversalConstraint()
    {
        delete Universal;
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
