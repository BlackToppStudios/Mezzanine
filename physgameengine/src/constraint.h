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
    /// @enum ConstraintParams
    /// @brief Used by constraints for setting some parameters.
    /// @details See the constraint class documentation for more details.
    enum ConstraintParams
    {
        Con_ERP         = 1,    ///< ERP values adjust how fast the errors in the constraints are reduced. @n
        Con_Stop_ERP    = 2,
        Con_CFM         = 3,    ///< CFM values adds some small value to the main diagonal on the constraint matrix to prevent degenerate matrices.
        Con_Stop_CFM    = 4
    };

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
            ActorRigid* ActA;
            /// @brief Second Actor the constraint applies to.
            ActorRigid* ActB;
            /// @brief Bullet constraint that this class encapsulates.
            btTypedConstraint* ConstraintBase;
            /// @brief Sets the Internal actor pointers.
            void SetBodies(ActorRigid* Act1, ActorRigid* Act2);
            /// @brief Sets the Internal actor pointers.
            void SetBodies(ActorRigid* Act1);
            /// @brief No initialization constructor.
            /// @details This is protected to prevent creating an instance of this directly.
            TypedConstraint();
        public:
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~TypedConstraint();
            /// @brief Gets the first actor this constraint applies to.
            /// @return Returns a pointer to the first actor this constraint applies to.
            virtual ActorRigid* GetActorA();
            /// @brief Gets the second actor this constraint applies to.
            /// @return Returns a pointer to the second actor this constraint applies to.
            virtual ActorRigid* GetActorB();
            /// @brief Provides override of constraint parameters.
            /// @details Parameters such as ERP(Error Reduction Parameter) and CFM(Constraint Force Mixing) can be altered with this function.  Optionally provide axis.
            /// @param Param The parameter to override.
            /// @param Value The new value for the parameter.
            /// @param Axis Optional axis.


            ///////////////////////////////////////////////////////////////////////////////
            // Constraint

            typedef std::vector<ConstraintParams> ParamList;


            virtual void SetParam(ConstraintParams Param, Real Value, int Axis=-1);
            /// @brief Gets value of constraint parameters.
            /// @details See SetParam() for clarification.  Gets information on constraint parameters.
            /// @param Para, The parameter to get information for.
            /// @param Axis Optional axis.
            /// @return Returns the value for the requested parameter.
            virtual Real GetParam(ConstraintParams Param, int Axis=-1);

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization
#ifdef PHYSXML
            // Serializable
            /// @brief Convert this class to an xml::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this vectorw should be appended to.
            virtual void ProtoSerialize(xml::Node& CurrentRoot) const;

            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and xml::Node containing the data.
            virtual void ProtoDeSerialize(const xml::Node& OneNode);

            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "TypedConstraint"
            String SerializableName() const;
#endif
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
            ConeTwistConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA, const Vector3& Vectorb, const Quaternion& QuaternionA, const Quaternion& QuaternionB);
            ConeTwistConstraint(ActorRigid* ActorA, const Vector3& VectorA, const Quaternion& QuaternionA);
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~ConeTwistConstraint();
            virtual void SetAngularOnly(bool AngularOnly);
            virtual void SetLimit(int LimitIndex, Real LimitValue);
            virtual void SetLimit(Real SwingSpan1, Real SwingSpan2, Real Twistspan, Real Softness=1.0, Real BiasFactor=0.3, Real RelaxationFactor=1.0);
            virtual void SetDamping(Real Damping);
            virtual void SetMaxMotorImpulse(Real MaxMotorImpulse);
            virtual void SetMaxMotorImpulseNormalized(Real MaxMotorImpulse);
            virtual void SetFixThresh(Real FixThresh);
            virtual void SetMotorTarget(const Quaternion& Quat);
            virtual void SetMotorTargetInConstraintSpace(const Quaternion& Quat);
            virtual void EnableMotor(bool Enable);
            virtual bool IsPassedSwingLimit();
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
            /// @brief Inheritance Constructor.
            /// @details This is only called by derived classes, and shouldn't be called manually.
            Generic6DofConstraint();
            Generic6DofConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA, const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseLinearReferenceA);
            Generic6DofConstraint(ActorRigid* ActorB, const Vector3& Vectorb, const Quaternion& QuaternionB, bool UseLinearReferenceB);
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~Generic6DofConstraint();
            virtual void SetOffsetALocation(const Vector3& Location);
            virtual void SetOffsetBLocation(const Vector3& Location);
            virtual void SetLinearUpperLimit(const Vector3& Limit);
            virtual void SetLinearLowerLimit(const Vector3& Limit);
            virtual void SetAngularUpperLimit(const Vector3& Limit);
            virtual void SetAngularLowerLimit(const Vector3& Limit);
            virtual void SetUseFrameOffset(bool UseOffset);
            virtual void SetLimit(int Axis, Real Low, Real High);
            virtual void CalculateTransforms();
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class Generic6DofSpringConstraint
    /// @headerfile constraint.h
    /// @brief Creates a constraint as configurable as the 6Dof constraint, but has added support for spring motion.
    /// @details When using functions of this class that require you to specify the index, the springs are arranged like so: @n
    /// 0: Translation X @n
    /// 1: Translation Y @n
    /// 2: Translation Z @n
    /// 3: Rotation X @n
    /// 4: Rotation Y @n
    /// 5: Rotation Z
    ///////////////////////////////////////
    class PHYS_LIB Generic6DofSpringConstraint : public Generic6DofConstraint
    {
        protected:
            /// @brief Bullet constraint that this class encapsulates.
            btGeneric6DofSpringConstraint* Generic6dofSpring;
        public:
            /// @brief Inheritance Constructor.
            /// @details This is only called by derived classes, and shouldn't be called manually.
            Generic6DofSpringConstraint();
            Generic6DofSpringConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA, const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseLinearReferenceA);
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~Generic6DofSpringConstraint();
            virtual void SetStiffness(int Index, Real Stiffness);
            virtual void SetDamping(int Index, Real Damping);
            virtual void SetEquilibriumPoint(int Index);
            virtual void EnableSpring(int Index, bool Enable);
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class HingeConstraint
    /// @headerfile constraint.h
    /// @brief This is a constraint to be used to restrict the movement between two objects to angular rotation on a single axis.
    /// @details As the name suggests, this constraint essentially works like a door Hinge.
    ///////////////////////////////////////
    class PHYS_LIB HingeConstraint : public TypedConstraint
    {
        protected:
            /// @brief Bullet constraint that this class encapsulates.
            btHingeConstraint* Hinge;
        public:
            /// @brief Creates a Hinge constraint that will connect two actors together by their offsets.
            /// @param ActorA The first actor to apply this constraint to.
            /// @param ActorB The second actor to apply this constraint to.
            /// @param PivotA The location in ActorA's local space to apply the constraint to.
            /// @param PivotB The location in ActorB's local space to apply the constraint to.
            /// @param AxisInA The axis(for ActorA) on which the hinge is to act.  For example, a door hinge would be (0.0,1.0,0.0), aka the positive Y axis.
            /// @param AxisInB The axis(for ActorB) on which the hinge is to act.  For example, a door hinge would be (0.0,1.0,0.0), aka the positive Y axis.
            /// @param UseReferenceA By default, this constraint uses ActorB's local space as the reference for certain values, such as the rotational limits.
            /// This simply controls whether or not it should use ActorA's local space instead.
            HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& PivotInA, const Vector3& PivotInB, const Vector3& AxisInA, const Vector3& AxisInB, bool UseReferenceA=false);
            /// @brief Creates a Hinge constraint that will attach an actor to a point in world space.
            /// @param ActorA The actor to apply this constraint to.
            /// @param PivotInA The point in the object's(ActorA) local space where the constraint is to be attached to world space.
            /// @param AxisInA The axis(for ActorA) on which the hinge is to act.  For example, a door hinge would be (0.0,1.0,0.0), aka the positive Y axis.
            /// @param UseReferenceA By default, this constraint uses ActorB's local space as the reference for certain values, such as the rotational limits.
            /// This simply controls whether or not it should use ActorA's local space instead.
            HingeConstraint(ActorRigid* ActorA, const Vector3& PivotInA, const Vector3& AxisInA, bool UseReferenceA=false);
            /// @brief
            /// @param ActorA The first actor to apply this constraint to.
            /// @param ActorB The second actor to apply this constraint to.
            /// @param VectorA
            /// @param VectorB
            /// @param QuaternionA
            /// @param QuaternionB
            /// @param UseReferenceA By default, this constraint uses ActorB's local space as the reference for certain values, such as the rotational limits.
            /// This simply controls whether or not it should use ActorA's local space instead.
            HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA, const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseReferenceA=false);
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~HingeConstraint();
            virtual void SetAPivotLocation(const Vector3& Location);
            virtual void SetBPivotLocation(const Vector3& Location);
            virtual void SetAngularOnly(bool AngularOnly);
            /// @brief Enables(or Disables) the motor on the hinge and sets it's parameters.
            /// @param EnableMotor Sets whether or not the motor on this constraint is enabled.
            /// @param TargetVelocity The desired velocity of rotation the motor will have.  This may or may not be achieved based on obstructions in the simulation.
            /// @param MaxMotorImpulse The maximum amount of force the motor is to apply to try and reach it's target velocity.
            virtual void EnableAngularMotor(bool EnableMotor, Real TargetVelocity, Real MaxMotorImpulse);
            /// @brief Enables(or Disables) the motor on the hinge.
            /// @param EnableMotor Sets whether or not the motor on this constraint is enabled.
            virtual void EnableMotor(bool EnableMotor);
            /// @brief Sets the maximum amount of force the motor is to apply.
            /// @param MaxMotorImpulse The maximum amount of force the motor is to apply to try and reach it's target velocity.
            virtual void SetMaxMotorImpulse(Real MaxMotorImpulse);
            virtual void SetMotorTarget(const Quaternion& QuatAInB, Real Dt);
            virtual void SetMotorTarget(Real TargetAngle, Real Dt);
            /// @brief Sets the angle limits of the constraint in radians.
            /// @param Low The minimum angle limit for the constraint in radians.
            /// @param High The maximum angle limit for the constraint in radians.
            /// @param Softness Not currently used internally.
            /// @param BiasFactor Multiplier for the constraint error, constraint appears more "soft" when closer to zero.
            /// @param RelaxationFactor The amount of bounce to apply when the constraint reaches it's limit.  Range: 0.0-1.0.
            virtual void SetLimit(Real Low, Real High, Real Softness=0.9, Real BiasFactor=0.3, Real RelaxationFactor=1.0);
            /// @brief Sets the axis on which this constraint acts.
            /// @param AxisInA A vector3 representing the axis to be used with this constraint.
            virtual void SetAxis(const Vector3& AxisInA);
            virtual void SetUseFrameOffset(bool FrameOffset);
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
            Hinge2Constraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& Anchor, const Vector3& Axis1, const Vector3& Axis2);
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~Hinge2Constraint();
            virtual void SetUpperLimit(Real Ang1Max);
            virtual void SetLowerLimit(Real Ang1Min);
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
            /// @brief Double body constructor.  Binds the two bodies.
            /// @param ActorA The first actor to apply this constraint to.
            /// @param ActorB The second actor to apply this constraint to.
            /// @param PivotA The location in ActorA's local space to apply the constraint to.
            /// @param PivotB The location in ActorB's local space to apply the constraint to.
            Point2PointConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& PivotA, const Vector3& PivotB);
            /// @brief Single body constructor.  Binds the body to world space.
            /// @param ActorA The actor to apply this constraint to.
            /// @param PivotA
            Point2PointConstraint(ActorRigid* ActorA, const Vector3& PivotA);
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~Point2PointConstraint();
            virtual void SetPivotA(const Vector3& PivotA);
            virtual void SetPivotB(const Vector3& PivotB);
            virtual Vector3 GetPivotInA();
            virtual Vector3 GetPivotInB();
            virtual void SetImpulseClamping(Real Clamping);
            virtual void SetTAU(Real TAU);
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
            SliderConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA, const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseLinearReferenceA);
            SliderConstraint(ActorRigid* ActorB, const Vector3& VectorB, const Quaternion& QuaternionB, bool UseLinearReferenceA);
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~SliderConstraint();
            virtual void SetFrameOffsetALocation(const Vector3& Location);
            virtual void SetFrameOffsetBLocation(const Vector3& Location);
            virtual void SetUpperLinLimit(Real UpperLimit);
            virtual void SetUpperAngLimit(Real UpperLimit);
            virtual void SetLowerLinLimit(Real LowerLimit);
            virtual void SetLowerAngLimit(Real LowerLimit);
            virtual void SetPoweredLinMotor(bool OnOff);
            virtual void SetTargetLinMotorVelocity(Real TargetLinMotorVelocity);
            virtual void SetMaxLinMotorForce(Real MaxLinMotorForce);
            virtual void SetPoweredAngMotor(bool OnOff);
            virtual void SetTargetAngMotorVelocity(Real TargetAngMotorVelocity);
            virtual void SetMaxAngMotorForce(Real MaxAngMotorForce);
            virtual void SetUseFrameOffset(bool FrameOffset);
            virtual void SetSoftnessDirLin(Real SoftnessDirLin);
            virtual void SetRestitutionDirLin(Real RestitutionDirLin);
            virtual void SetDampingDirLin(Real DampingDirLin);
            virtual void SetSoftnessDirAng(Real SoftnessDirAng);
            virtual void SetRestitutionDirAng(Real RestitutionDirAng);
            virtual void SetDampingDirAng(Real DampingDirAng);
            virtual void SetSoftnessLimLin(Real SoftnessLimLin);
            virtual void SetRestitutionLimLin(Real RestitutionLimLin);
            virtual void SetDampingLimLin(Real DampingLimLin);
            virtual void SetSoftnessLimAng(Real SoftnessLimAng);
            virtual void SetRestitutionLimAng(Real RestitutionLimAng);
            virtual void SetDampingLimAng(Real DampingLimAng);
            virtual void SetSoftnessOrthoLin(Real SoftnessOrthoLin);
            virtual void SetRestitutionOrthoLin(Real RestitutionOrthoLin);
            virtual void SetDampingOrthoLin(Real DampingOrthoLin);
            virtual void SetSoftnessOrthoAng(Real SoftnessOrthoAng);
            virtual void SetRestitutionOrthoAng(Real RestitutionOrthoAng);
            virtual void SetDampingOrthoAng(Real DampingOrthoAng);
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
            UniversalConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& Anchor, const Vector3& Axis1, const Vector3& Axis2);
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~UniversalConstraint();
            virtual void SetUpperLimit(Real Ang1Max, Real Ang2Max);
            virtual void SetLowerLimit(Real Ang1Min, Real Ang2Min);
    };
}//phys


/// @endcond
#endif

