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
#ifndef _constraint_h
#define _constraint_h

#include "datatypes.h"
#include "quaternion.h"
#include "vector3.h"
namespace phys
{
    class ActorRigid;
}

class btRigidBody;
class btTypedConstraint;
class btConeTwistConstraint;
class btGeneric6DofConstraint;
class btGeneric6DofSpringConstraint;
class btHingeConstraint;
class btHinge2Constraint;
class btPoint2PointConstraint;
class btSliderConstraint;
class btUniversalConstraint;

namespace phys
{
    class TypedConstraint
    {
        protected:
            friend class World;
            friend class PhysicsManager;
            btRigidBody* BodyA;
            btRigidBody* BodyB;
            btTypedConstraint* ConstraintBase;
        public:
            TypedConstraint();
            virtual ~TypedConstraint();
            TypedConstraint(ActorRigid* bodya, ActorRigid* bodyb);
            TypedConstraint(ActorRigid* bodya);
            virtual void SetParam(int num, Real value, int axis=-1) = 0;
            virtual Real GetParam(int num, int axis=-1) = 0;
    };

    class ConeTwistConstraint: public TypedConstraint
    {
        protected:
            btConeTwistConstraint* ConeTwist;
        public:
            ConeTwistConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 VectorA, Vector3 Vectorb, Quaternion QuaternionA, Quaternion QuaternionB);
            ConeTwistConstraint(ActorRigid* ActorA, Vector3 VectorA, Quaternion QuaternionA);
            ConeTwistConstraint(btConeTwistConstraint* Constraint);
            virtual ~ConeTwistConstraint();
            void SetAngularOnly(bool AngularOnly);
            void SetLimit(int LimitIndex, Real LimitValue);
            void SetLimit(Real SwingSpan1, Real SwingSpan2, Real Twistspan, Real Softness=1.0, Real BiasFactor=0.3, Real RelaxationFactor=1.0);
            void SetDamping(Real Damping);
            void SetMaxMotorImpulse(Real MaxMotorImpulse);
            void SetMaxMotorImpulseNormalized(Real MaxMotorImpulse);
            void SetFixThresh(Real FixThresh);
            void SetMotorTarget(Quaternion Quat);
            void SetMotorTargetInConstraintSpace(Quaternion Quat);
            void EnableMotor(bool Enable);
            bool IsPassedSwingLimit();
            virtual void SetParam(int num, Real value, int axis=-1);
            virtual Real GetParam(int num, int axis=-1);
    };

    class Generic6DofConstraint: public TypedConstraint
    {
        protected:
            btGeneric6DofConstraint* Generic6dof;
        public:
            Generic6DofConstraint();
            Generic6DofConstraint(ActorRigid* ActorA, ActorRigid* ActorB);
            Generic6DofConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 VectorA, Vector3 VectorB, Quaternion QuaternionA, Quaternion QuaternionB, bool UseLinearReferenceA);
            Generic6DofConstraint(ActorRigid* ActorB, Vector3 Vectorb, Quaternion QuaternionB, bool UseLinearReferenceB);
            Generic6DofConstraint(btGeneric6DofConstraint* Constraint);
            virtual ~Generic6DofConstraint();
            void SetOffsetALocation(Vector3 Location);
            void SetOffsetBLocation(Vector3 Location);
            void SetLinearUpperLimit(Vector3 Limit);
            void SetLinearLowerLimit(Vector3 Limit);
            void SetAngularUpperLimit(Vector3 Limit);
            void SetAngularLowerLimit(Vector3 Limit);
            void SetUseFrameOffset(bool UseOffset);
            void SetLimit(int Axis, Real Low, Real High);
            void CalculateTransforms();
            virtual void SetParam(int num, Real value, int axis=-1);
            virtual Real GetParam(int num, int axis=-1);
    };

    class Generic6DofSpringConstraint: public Generic6DofConstraint
    {
        protected:
            btGeneric6DofSpringConstraint* Generic6dofSpring;
        public:
            Generic6DofSpringConstraint();
            Generic6DofSpringConstraint(ActorRigid* ActorA, ActorRigid* ActorB);
            Generic6DofSpringConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 VectorA, Vector3 VectorB, Quaternion QuaternionA, Quaternion QuaternionB, bool UseLinearReferenceA);
            Generic6DofSpringConstraint(btGeneric6DofSpringConstraint* Constraint);
            ~Generic6DofSpringConstraint();
            void SetStiffness(int Index, Real Stiffness);
            void SetDamping(int Index, Real Damping);
            void SetEquilibriumPoint(int Index);
            void EnableSpring(int Index, bool Enable);
    };

    class HingeConstraint: public TypedConstraint
    {
        protected:
            btHingeConstraint* Hinge;
        public:
            HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 PivotInA, Vector3 PivotInB, Vector3 AxisInA, Vector3 AxisInB, bool UseReferenceA);
            HingeConstraint(ActorRigid* ActorA, Vector3 PivotInA, Vector3 AxisInA, bool UseReferenceA);
            HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 VectorA, Vector3 VectorB, Quaternion QuaternionA, Quaternion QuaternionB, bool UseReferenceA=false);
            HingeConstraint(btHingeConstraint* Constraint);
            virtual ~HingeConstraint();
            void SetAPivotLocation(Vector3 Location);
            void SetBPivotLocation(Vector3 Location);
            void SetAngularOnly(bool AngularOnly);
            void EnableAngularMotor(bool EnableMotor, Real TargetVelocity, Real MaxMotorImpulse);
            void EnableMotor(bool EnableMotor);
            void SetMaxMotorImpulse(Real MaxMotorImpulse);
            void SetMotorTarget(Quaternion QuatAInB, Real Dt);
            void SetMotorTarget(Real TargetAngle, Real Dt);
            void SetLimit(Real Low, Real High, Real Softness=0.9, Real BiasFactor=0.3, Real RelaxationFactor=1.0);
            void SetAxis(Vector3 AxisInA);
            void SetUseFrameOffset(bool FrameOffset);
            virtual void SetParam(int num, Real value, int axis=-1);
            virtual Real GetParam(int num, int axis=-1);
    };

    class Hinge2Constraint: public Generic6DofSpringConstraint
    {
        protected:
            btHinge2Constraint* Hinge2;
        public:
            Hinge2Constraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 Anchor, Vector3 Axis1, Vector3 Axis2);
            Hinge2Constraint(btHinge2Constraint* Constraint);
            ~Hinge2Constraint();
            void SetUpperLimit(Real Ang1Max);
            void SetLowerLimit(Real Ang1Min);
    };

    class Point2PointConstraint: public TypedConstraint
    {
        protected:
            btPoint2PointConstraint* Point2Point;
        public:
            Point2PointConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 PivotA, Vector3 PivotB);
            Point2PointConstraint(ActorRigid* ActorA, Vector3 PivotA);
            Point2PointConstraint(btPoint2PointConstraint* Constraint);
            virtual ~Point2PointConstraint();
            void SetPivotA(Vector3 PivotA);
            void SetPivotB(Vector3 PivotB);
            virtual void SetParam(int num, Real value, int axis=-1);
            virtual Real GetParam(int num, int axis=-1);
    };

    class SliderConstraint: public TypedConstraint
    {
        protected:
            btSliderConstraint* Slider;
        public:
            SliderConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 VectorA, Vector3 VectorB, Quaternion QuaternionA, Quaternion QuaternionB, bool UseLinearReferenceA);
            SliderConstraint(ActorRigid* ActorB, Vector3 VectorB, Quaternion QuaternionB, bool UseLinearReferenceA);
            SliderConstraint(btSliderConstraint* Constraint);
            virtual ~SliderConstraint();
            void SetFrameOffsetALocation(Vector3 Location);
            void SetFrameOffsetBLocation(Vector3 Location);
            void SetUpperLinLimit(Real UpperLimit);
            void SetUpperAngLimit(Real UpperLimit);
            void SetLowerLinLimit(Real LowerLimit);
            void SetLowerAngLimit(Real LowerLimit);
            void SetPoweredLinMotor(bool OnOff);
            void SetTargetLinMotorVelocity(Real TargetLinMotorVelocity);
            void SetMaxLinMotorForce(Real MaxLinMotorForce);
            void SetPoweredAngMotor(bool OnOff);
            void SetTargetAngMotorVelocity(Real TargetAngMotorVelocity);
            void SetMaxAngMotorForce(Real MaxAngMotorForce);
            void SetUseFrameOffset(bool FrameOffset);
            void SetSoftnessDirLin(Real SoftnessDirLin);
            void SetRestitutionDirLin(Real RestitutionDirLin);
            void SetDampingDirLin(Real DampingDirLin);
            void SetSoftnessDirAng(Real SoftnessDirAng);
            void SetRestitutionDirAng(Real RestitutionDirAng);
            void SetDampingDirAng(Real DampingDirAng);
            void SetSoftnessLimLin(Real SoftnessLimLin);
            void SetRestitutionLimLin(Real RestitutionLimLin);
            void SetDampingLimLin(Real DampingLimLin);
            void SetSoftnessLimAng(Real SoftnessLimAng);
            void SetRestitutionLimAng(Real RestitutionLimAng);
            void SetDampingLimAng(Real DampingLimAng);
            void SetSoftnessOrthoLin(Real SoftnessOrthoLin);
            void SetRestitutionOrthoLin(Real RestitutionOrthoLin);
            void SetDampingOrthoLin(Real DampingOrthoLin);
            void SetSoftnessOrthoAng(Real SoftnessOrthoAng);
            void SetRestitutionOrthoAng(Real RestitutionOrthoAng);
            void SetDampingOrthoAng(Real DampingOrthoAng);
            virtual void SetParam(int num, Real value, int axis=-1);
            virtual Real GetParam(int num, int axis=-1);
    };

    class UniversalConstraint: public Generic6DofConstraint
    {
        protected:
            btUniversalConstraint* Universal;
        public:
            UniversalConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 Anchor, Vector3 Axis1, Vector3 Axis2);
            UniversalConstraint(btUniversalConstraint* Constraint);
            ~UniversalConstraint();
            void SetUpperLimit(Real Ang1Max, Real Ang2Max);
            void SetLowerLimit(Real Ang1Min, Real Ang2Min);
    };
}//phys

#endif
