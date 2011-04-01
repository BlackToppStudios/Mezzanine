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
#ifndef _constraint_h
#define _constraint_h

#include "crossplatformexport.h"
#include "datatypes.h"
#include "quaternion.h"
#include "vector3.h"

/// @cond 0

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
    ///////////////////////////////////////////////////////////////////////////////
    /// @class TypedConstraint
    /// @headerfile constraint.h
    /// @brief This is the base class for all constraints supported.
    /// @details This class provides the basis for all the other constraints.  This is
    /// also a virtual class and provides no constraint properties of it's own, thus
    /// shouldn't/can't be called on manually.
    ///////////////////////////////////////////////////////////////////////////////
    class PHYS_LIB TypedConstraint
    {
        protected:
            friend class World;
            friend class PhysicsManager;
            /// @brief First rigid body the constraint applies to.
            btRigidBody* BodyA;
            /// @brief Second rigid body the constraint applies to(if applicable).
            btRigidBody* BodyB;
            /// @brief First Actor the constraint applies to.
            ActorRigid* ActorA;
            /// @brief Second Actor the constraint applies to.
            ActorRigid* ActorB;
            /// @brief Bullet constraint that this class encapsulates.
            btTypedConstraint* ConstraintBase;
        public:
            /// @brief No initialization constructor.
            /// @details The no initialization class constructor.
            TypedConstraint();
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~TypedConstraint();
            /// @brief Two body constructor.
            /// @details This constructor is used when the applicable constraint is applied to two bodies, as opposed to a point a world space.
            /// @param bodya First actor to be applied to.
            /// @param bodyb Second actor to be applied to.
            TypedConstraint(ActorRigid* bodya, ActorRigid* bodyb);
            /// @brief One body constructor.
            /// @details This constructor is used when the applicable constraint is applied to a body and a point in world space.
            /// @param bodya The actor to be constraint to a point in world space.
            TypedConstraint(ActorRigid* bodya);
            /// @brief Gets the first actor this constraint applies to.
            /// @return Returns a pointer to the first actor this constraint applies to.
            ActorRigid* GetActorA();
            /// @brief Gets the second actor this constraint applies to.
            /// @return Returns a pointer to the second actor this constraint applies to.
            ActorRigid* GetActorB();
            /// @brief Provides override of constraint parameters.
            /// @details Parameters such as ERP and CFM can be altered with this function.  Optionally provide axis.
            /// @param num The parameter to override.
            /// @param value The new value for the parameter.
            /// @param axis Optional axis.
            virtual void SetParam(int num, Real value, int axis=-1) = 0;
            /// @brief Gets value of constraint parameters.
            /// @details See SetParam() for clarification.  Gets information on constraint parameters.
            /// @param num The parameter to get information for.
            /// @param axis Optional axis.
            /// @return Returns the value for the requested parameter.
            virtual Real GetParam(int num, int axis=-1) = 0;
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ConeTwistConstraint
    /// @headerfile constraint.h
    /// @brief
    /// @details
    ///////////////////////////////////////
    class PHYS_LIB ConeTwistConstraint : public TypedConstraint
    {
        protected:
            /// @brief Bullet constraint that this class encapsulates.
            btConeTwistConstraint* ConeTwist;
        public:
            ConeTwistConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 VectorA, Vector3 Vectorb, Quaternion QuaternionA, Quaternion QuaternionB);
            ConeTwistConstraint(ActorRigid* ActorA, Vector3 VectorA, Quaternion QuaternionA);
            /// @brief Internal constructor.
            /// @details Constructs this class around a pre-built bullet constraint.  This is an internal only constructor and shouldn't be called manually.
            /// @param Constraint The constraint to be constructed around.
            ConeTwistConstraint(btConeTwistConstraint* Constraint);
            /// @brief Class destructor.
            /// @details The class destructor.
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
            /// @brief Provides override of constraint parameters.
            /// @details Parameters such as ERP and CFM can be altered with this function.  Optionally provide axis.
            /// @param num The parameter to override.
            /// @param value The new value for the parameter.
            /// @param axis Optional axis.
            virtual void SetParam(int num, Real value, int axis=-1);
            /// @brief Gets value of constraint parameters.
            /// @details See SetParam() for clarification.  Gets information on constraint parameters.
            /// @param num The parameter to get information for.
            /// @param axis Optional axis.
            /// @return Returns the value for the requested parameter.
            virtual Real GetParam(int num, int axis=-1);
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class Generic6DofConstraint
    /// @headerfile constraint.h
    /// @brief
    /// @details
    ///////////////////////////////////////
    class PHYS_LIB Generic6DofConstraint : public TypedConstraint
    {
        protected:
            /// @brief Bullet constraint that this class encapsulates.
            btGeneric6DofConstraint* Generic6dof;
        public:
            /// @brief No initialization constructor.
            /// @details The no initialization class constructor.
            Generic6DofConstraint();
            /// @brief Inheritance constructor.
            /// @details This constructor exists for passing values down the inheritance tree from derived classes.  Should not be called on manually.
            /// @param ActorA First actor to have the constraint applied to.
            /// @param ActorB Second actor to have the constraint applied to.
            Generic6DofConstraint(ActorRigid* ActorA, ActorRigid* ActorB);
            Generic6DofConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 VectorA, Vector3 VectorB, Quaternion QuaternionA, Quaternion QuaternionB, bool UseLinearReferenceA);
            Generic6DofConstraint(ActorRigid* ActorB, Vector3 Vectorb, Quaternion QuaternionB, bool UseLinearReferenceB);
            /// @brief Internal constructor.
            /// @details Constructs this class around a pre-built bullet constraint.  This is an internal only constructor and shouldn't be called manually.
            /// @param Constraint The constraint to be constructed around.
            Generic6DofConstraint(btGeneric6DofConstraint* Constraint);
            /// @brief Class destructor.
            /// @details The class destructor.
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
            /// @brief Provides override of constraint parameters.
            /// @details Parameters such as ERP and CFM can be altered with this function.  Optionally provide axis.
            /// @param num The parameter to override.
            /// @param value The new value for the parameter.
            /// @param axis Optional axis.
            virtual void SetParam(int num, Real value, int axis=-1);
            /// @brief Gets value of constraint parameters.
            /// @details See SetParam() for clarification.  Gets information on constraint parameters.
            /// @param num The parameter to get information for.
            /// @param axis Optional axis.
            /// @return Returns the value for the requested parameter.
            virtual Real GetParam(int num, int axis=-1);
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class Generic6DofSpringConstraint
    /// @headerfile constraint.h
    /// @brief
    /// @details
    ///////////////////////////////////////
    class PHYS_LIB Generic6DofSpringConstraint : public Generic6DofConstraint
    {
        protected:
            /// @brief Bullet constraint that this class encapsulates.
            btGeneric6DofSpringConstraint* Generic6dofSpring;
        public:
            Generic6DofSpringConstraint();
            /// @brief Inheritance constructor.
            /// @details This constructor exists for passing values down the inheritance tree from derived classes.  Should not be called on manually.
            /// @param ActorA First actor to have the constraint applied to.
            /// @param ActorB Second actor to have the constraint applied to.
            Generic6DofSpringConstraint(ActorRigid* ActorA, ActorRigid* ActorB);
            Generic6DofSpringConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 VectorA, Vector3 VectorB, Quaternion QuaternionA, Quaternion QuaternionB, bool UseLinearReferenceA);
            /// @brief Internal constructor.
            /// @details Constructs this class around a pre-built bullet constraint.  This is an internal only constructor and shouldn't be called manually.
            /// @param Constraint The constraint to be constructed around.
            Generic6DofSpringConstraint(btGeneric6DofSpringConstraint* Constraint);
            /// @brief Class destructor.
            /// @details The class destructor.
            ~Generic6DofSpringConstraint();
            void SetStiffness(int Index, Real Stiffness);
            void SetDamping(int Index, Real Damping);
            void SetEquilibriumPoint(int Index);
            void EnableSpring(int Index, bool Enable);
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class HingeConstraint
    /// @headerfile constraint.h
    /// @brief
    /// @details
    ///////////////////////////////////////
    class PHYS_LIB HingeConstraint : public TypedConstraint
    {
        protected:
            /// @brief Bullet constraint that this class encapsulates.
            btHingeConstraint* Hinge;
        public:
            /// @brief
            /// @param AxisInA The orientation for the axis(for ActorA) on which the hinge is to act.  For example, a door hinge would be (0.0,1.0,0.0), aka the positive Y axis.
            /// @param AxisInB The orientation for the axis(for ActorB) on which the hinge is to act.  For example, a door hinge would be (0.0,1.0,0.0), aka the positive Y axis.
            /// @param UseReferenceA By default, this constraint uses ActorB's local space as the reference for certain values, such as the rotational limits.
            /// This simply controls whether or not it should use ActorA's local space instead.
            HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 PivotInA, Vector3 PivotInB, Vector3 AxisInA, Vector3 AxisInB, bool UseReferenceA=false);
            /// @brief
            /// @param AxisInA The axis(for ActorA) on which the hinge is to act.  For example, a door hinge would be (0.0,1.0,0.0), aka the positive Y axis.
            /// @param UseReferenceA By default, this constraint uses ActorB's local space as the reference for certain values, such as the rotational limits.
            /// This simply controls whether or not it should use ActorA's local space instead.
            HingeConstraint(ActorRigid* ActorA, Vector3 PivotInA, Vector3 AxisInA, bool UseReferenceA=false);
            /// @brief
            /// @param UseReferenceA By default, this constraint uses ActorB's local space as the reference for certain values, such as the rotational limits.
            /// This simply controls whether or not it should use ActorA's local space instead.
            HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 VectorA, Vector3 VectorB, Quaternion QuaternionA, Quaternion QuaternionB, bool UseReferenceA=false);
            /// @brief Internal constructor.
            /// @details Constructs this class around a pre-built bullet constraint.  This is an internal only constructor and shouldn't be called manually.
            /// @param Constraint The constraint to be constructed around.
            HingeConstraint(btHingeConstraint* Constraint);
            /// @brief Class destructor.
            /// @details The class destructor.
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
            /// @brief Provides override of constraint parameters.
            /// @details Parameters such as ERP and CFM can be altered with this function.  Optionally provide axis.
            /// @param num The parameter to override.
            /// @param value The new value for the parameter.
            /// @param axis Optional axis.
            virtual void SetParam(int num, Real value, int axis=-1);
            /// @brief Gets value of constraint parameters.
            /// @details See SetParam() for clarification.  Gets information on constraint parameters.
            /// @param num The parameter to get information for.
            /// @param axis Optional axis.
            /// @return Returns the value for the requested parameter.
            virtual Real GetParam(int num, int axis=-1);
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class Hinge2Constraint
    /// @headerfile constraint.h
    /// @brief
    /// @details
    ///////////////////////////////////////
    class PHYS_LIB Hinge2Constraint : public Generic6DofSpringConstraint
    {
        protected:
            /// @brief Bullet constraint that this class encapsulates.
            btHinge2Constraint* Hinge2;
        public:
            Hinge2Constraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 Anchor, Vector3 Axis1, Vector3 Axis2);
            /// @brief Internal constructor.
            /// @details Constructs this class around a pre-built bullet constraint.  This is an internal only constructor and shouldn't be called manually.
            /// @param Constraint The constraint to be constructed around.
            Hinge2Constraint(btHinge2Constraint* Constraint);
            /// @brief Class destructor.
            /// @details The class destructor.
            ~Hinge2Constraint();
            void SetUpperLimit(Real Ang1Max);
            void SetLowerLimit(Real Ang1Min);
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class Point2PointConstraint
    /// @headerfile constraint.h
    /// @brief
    /// @details
    ///////////////////////////////////////
    class PHYS_LIB Point2PointConstraint : public TypedConstraint
    {
        protected:
            /// @brief Bullet constraint that this class encapsulates.
            btPoint2PointConstraint* Point2Point;
        public:
            Point2PointConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 PivotA, Vector3 PivotB);
            Point2PointConstraint(ActorRigid* ActorA, Vector3 PivotA);
            /// @brief Internal constructor.
            /// @details Constructs this class around a pre-built bullet constraint.  This is an internal only constructor and shouldn't be called manually.
            /// @param Constraint The constraint to be constructed around.
            Point2PointConstraint(btPoint2PointConstraint* Constraint);
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~Point2PointConstraint();
            void SetPivotA(Vector3 PivotA);
            void SetPivotB(Vector3 PivotB);
            Vector3 GetPivotInA();
            Vector3 GetPivotInB();
            void SetImpulseClamping(Real Clamping);
            void SetTAU(Real TAU);
            /// @brief Provides override of constraint parameters.
            /// @details Parameters such as ERP and CFM can be altered with this function.  Optionally provide axis.
            /// @param num The parameter to override.
            /// @param value The new value for the parameter.
            /// @param axis Optional axis.
            virtual void SetParam(int num, Real value, int axis=-1);
            /// @brief Gets value of constraint parameters.
            /// @details See SetParam() for clarification.  Gets information on constraint parameters.
            /// @param num The parameter to get information for.
            /// @param axis Optional axis.
            /// @return Returns the value for the requested parameter.
            virtual Real GetParam(int num, int axis=-1);
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class SliderConstraint
    /// @headerfile constraint.h
    /// @brief
    /// @details
    ///////////////////////////////////////
    class PHYS_LIB SliderConstraint : public TypedConstraint
    {
        protected:
            /// @brief Bullet constraint that this class encapsulates.
            btSliderConstraint* Slider;
        public:
            SliderConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 VectorA, Vector3 VectorB, Quaternion QuaternionA, Quaternion QuaternionB, bool UseLinearReferenceA);
            SliderConstraint(ActorRigid* ActorB, Vector3 VectorB, Quaternion QuaternionB, bool UseLinearReferenceA);
            /// @brief Internal constructor.
            /// @details Constructs this class around a pre-built bullet constraint.  This is an internal only constructor and shouldn't be called manually.
            /// @param Constraint The constraint to be constructed around.
            SliderConstraint(btSliderConstraint* Constraint);
            /// @brief Class destructor.
            /// @details The class destructor.
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
            /// @brief Provides override of constraint parameters.
            /// @details Parameters such as ERP and CFM can be altered with this function.  Optionally provide axis.
            /// @param num The parameter to override.
            /// @param value The new value for the parameter.
            /// @param axis Optional axis.
            virtual void SetParam(int num, Real value, int axis=-1);
            /// @brief Gets value of constraint parameters.
            /// @details See SetParam() for clarification.  Gets information on constraint parameters.
            /// @param num The parameter to get information for.
            /// @param axis Optional axis.
            /// @return Returns the value for the requested parameter.
            virtual Real GetParam(int num, int axis=-1);
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class UniversalConstraint
    /// @headerfile constraint.h
    /// @brief
    /// @details
    ///////////////////////////////////////
    class PHYS_LIB UniversalConstraint : public Generic6DofConstraint
    {
        protected:
            /// @brief Bullet constraint that this class encapsulates.
            btUniversalConstraint* Universal;
        public:
            UniversalConstraint(ActorRigid* ActorA, ActorRigid* ActorB, Vector3 Anchor, Vector3 Axis1, Vector3 Axis2);
            /// @brief Internal constructor.
            /// @details Constructs this class around a pre-built bullet constraint.  This is an internal only constructor and shouldn't be called manually.
            /// @param Constraint The constraint to be constructed around.
            UniversalConstraint(btUniversalConstraint* Constraint);
            /// @brief Class destructor.
            /// @details The class destructor.
            ~UniversalConstraint();
            void SetUpperLimit(Real Ang1Max, Real Ang2Max);
            void SetLowerLimit(Real Ang1Min, Real Ang2Min);
    };
}//phys


/// @endcond
#endif

