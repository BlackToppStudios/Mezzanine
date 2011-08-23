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
#include "transform.h"
#include "vector3.h"

#include <algorithm>
#include <vector>

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
    /// @enum ConstraintParam
    /// @brief Used by constraints for setting some parameters.
    /// @details See the constraint class documentation for more details.
    enum ConstraintParam
    {
        Con_ERP         = 1,    ///< ERP values adjust how fast the errors in the constraints are reduced.
        Con_Stop_ERP    = 2,
        Con_CFM         = 3,    ///< CFM values adds some small value to the main diagonal on the constraint matrix to prevent degenerate matrices.
        Con_Stop_CFM    = 4
    };

    /// @brief Get a Constraint Parameter as a String
    /// @param Param The Parameter to get as a String
    /// @return A String That contains the name of a ConstraintParam
    String ConstraintParamAsString(ConstraintParam Param);

    /// @brief How many ConstraintParam Exist.
    /// @details Used in some algorithms and we didn't want it to look like a magic so we defined it here.
    const int ConstraintParamCount = 4;

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
            virtual ActorRigid* GetActorA() const;
            /// @brief Gets the second actor this constraint applies to.
            /// @return Returns a pointer to the second actor this constraint applies to.
            virtual ActorRigid* GetActorB() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Constraint

            /// @brief Used to Store lists of param for return types
            typedef std::vector<ConstraintParam> ParamList;

            /// @brief Used to Store lists of Int Axis for return types
            /// @details In general Axis will come in groups of three, such as 0,1,2, or 0,1,2,3,4,5 which could represent X,Y, and Z or multiple grousp of X,Y, and Z. These Axis
            /// can represent Linear/Translation or Angular/Rotation information. Some Constraints support values that affect all constraints, this is usually represented a -1.
            typedef std::vector<int> AxisList;

            /// @brief Get a sorted (low to high) list of Parameters that are valid on this Axis
            /// @details Parameters returned from this will work on the given axis even if they are not valid on any other axis. There is no guarantee that the Parameters
            /// will be uniquely stored per an axis. There is no guarantee that changing one parameter will not change another.
            /// @param Axis the Axis to check.
            /// @return A Paramlist with all the valid parameters for this axis.
            virtual ParamList ValidParamOnAxis(int Axis) const = 0;

            /// @brief Get a sorted (low to high) list of all axis that operate linearly (that lock sliding/translation)
            /// @warning Odd behaviors, maybe even undefined behavior can happen if This returns a matching Axis to a Linear Axis. Any given axis should only be one or the other
            /// @return An Axislist with the Axis this constraint linearly supports.
            virtual AxisList ValidLinearAxis() const = 0;

            /// @brief Get A list sorted (low to high) of all axis that operate Angularly (that lock sliding/translation)
            /// @warning Odd behaviors, maybe even undefined behavior can happen if This returns a matching Axis to a Linear Axis. Any given axis should only be one or the other
            /// @return An Axislist with the Axis this constraint Angularly supports.
            virtual AxisList ValidAngularAxis() const = 0;

            /// @brief Has the given Param on the Given Axis been set yet.
            /// @param Param The parameter to Check
            /// @param Axis The Axis on which to check the param
            /// @details This will probably have to implement the same logic aas what is in the respective get/setparam function of each constraint http://bulletphysics.com/Bullet/BulletFull/classbtTypedConstraint.html#a11f44dd743b59bc05e25d500456e2559
            /// @return True if it has been set, false if it has.
            virtual bool HasParamBeenSet(ConstraintParam Param, int Axis) const = 0;

            /// @brief Get a sorted (low to high) list of All Valid Axis
            /// @details This is implemented using ValidLinearAxis and ValidAngularAxis, Derived versions of this class may wish to make a more specialized
            /// implementation of this method that doesn't have the overhead of passing around 3 vectors by value.
            /// @return An Axislist with all the Axis this constraint supports.
            virtual AxisList ValidAxis() const
            {
                AxisList Ang = this->ValidAngularAxis();
                AxisList Lin = this->ValidLinearAxis();
                Lin.insert(Lin.end()-1,Ang.begin(),Ang.end());              // just kinda stick the two together and return that, no checking is performed for uniqueness.
                sort(Lin.begin(),Lin.end());                                // Usually the Linear axis are 0,1,2 and the angular are 3,4,5 so hopefully, this will hardly ever need to do work.
                return Lin;
            }

            /// @brief Is a certain Parameter valid on a certain axis
            /// @param Param The Parameter to Check
            /// @param Axis The Axis to Check
            /// @details This is implemented using ValidParamOnAxis, Derived versions of this class may wish to make a more specialized
            /// implementation of this method.
            /// @return True if Param is valid on Axis, and Axis is valid. If anything is invalid this returns false.
            virtual bool IsParamValidOnAxis(ConstraintParam Param, int Axis) const
            {
                ParamList Supported = ValidParamOnAxis(Axis);
                if(ConstraintParamCount == Supported.size())                           // no need to check deeper, because everything is supported
                {
                    return true;
                }else{
                    if(Supported.size())
                    {
                        return ( std::find(Supported.begin(),Supported.end(),Param) != Supported.end() );     // should return true if found
                    }else{
                        return false;                                                   // size is 0 of course it is not supported
                    }
                }
            }

            /// @brief Get A sorted (low to high) list of Parameters that are valid on all Axis
            /// @details This is implemented using ValidAxis and ValidParamOnAxis, Derived versions of this class may wish to make a more specialized
            /// implementation of this method that doesn't have the overhead of passing around many vectors by value and executing slow loops. Most
            /// likely all of these constraint Parameter and axis functions could be replaced with some fairly simple switch statements and statically
            /// coded values that are specific to the kind of constraint.
            /// @return A Paramlist with the parameters valid on all axis.
            virtual ParamList ValidParams() const
            {
                AxisList AllAxis = this->ValidAxis();
                ParamList Results;
                if(AllAxis.size())                                              //If we have no Axis, the we have no valid Parameters.
                {
                    Results = this->ValidParamOnAxis(*(AllAxis.begin()));       // Let's start off with whatever the first Axis Support, then we will trim from here.

                    if(AllAxis.size()>1)                                        //if the constraint only support one axis then we already have our answer, and we don't want to run of the end of the Paramlist
                    {
                        for(AxisList::iterator AxisIter = AllAxis.begin()+1; AllAxis.end()!=AxisIter; ++AxisIter)       //For each axis after the first
                        {
                            for(int ParamID = Results.size(); ParamID<=0; --ParamID)        // We start at the back and work by index because reverse erase does no accept rever iterators, and
                            {
                                if (!IsParamValidOnAxis(Results.at(ParamID),*AxisIter))     // if an Item is erase near the beginning, it changes all the items after, making forward iteration logic that
                                    { Results.erase( Results.begin()+ParamID ); }           // erases needlessly complicated, and potentially expensive depending on the Vector implementation
                            }
                        }
                    }
                }
                sort(Results.begin(),Results.end());
                return Results;
            }

            /// @brief Provides override of constraint parameters.
            /// @details Parameters such as ERP(Error Reduction Parameter) and CFM(Constraint Force Mixing) can be altered with this function.  Optionally provide axis.
            /// @param Param The parameter to override.
            /// @param Value The new value for the parameter.
            /// @param Axis Optional axis.
            virtual void SetParam(ConstraintParam Param, Real Value, int Axis=-1);

            /// @brief Gets value of constraint parameters.
            /// @details See SetParam() for clarification.  Gets information on constraint parameters.
            /// @param Para, The parameter to get information for.
            /// @param Axis Optional axis.
            /// @return Returns the value for the requested parameter.
            virtual Real GetParam(ConstraintParam Param, int Axis=-1) const;

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

            /// @brief Inheritance Constructor.
            /// @details This is only called by derived classes, and shouldn't be called manually.
            Generic6DofConstraint();
        public:
            /// @brief Two body Verbose constructor
            /// @param ActorA The First body to be bound
            /// @param ActorB  The Second body to be bound
            /// @param VectorA The offset from ActorA's center of gravity to get to match an offset from ActorB
            /// @param VectorB The offset from ActorB's center of gravity.
            /// @param QuaternionA Relative rotation from ActorA
            /// @param QuaternionB Relative rotation from ActorB
            /// @param UseLinearReferenceA Perform Linear math from ActorA's perspective, default to false.
            Generic6DofConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA, const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseLinearReferenceA = false);
            /// @brief Two body Terse constructor
            /// @param ActorA The First body to be bound
            /// @param ActorB  The Second body to be bound
            /// @param TransformA The offset and rotation from ActorA's center of gravity to get to match an offset from ActorB
            /// @param TransformB The offset and rotation from ActorB's center of gravity.
            /// @param UseLinearReferenceA Perform Linear math from ActorA's perspective, default to false.
            Generic6DofConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Transform& TransformA, const Transform& TransformB, bool UseLinearReferenceA = false);


            Generic6DofConstraint(ActorRigid* ActorB, const Vector3& Vectorb, const Quaternion& QuaternionB, bool UseLinearReferenceB);
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~Generic6DofConstraint();
            virtual void SetOffsetALocation(const Vector3& Location);
            virtual void SetOffsetBLocation(const Vector3& Location);
            //should get rotation functions
            // should get transform functions
            virtual void SetLinearUpperLimit(const Vector3& Limit);
            virtual void SetLinearLowerLimit(const Vector3& Limit);
            virtual void SetAngularUpperLimit(const Vector3& Limit);
            virtual void SetAngularLowerLimit(const Vector3& Limit);
            virtual void SetUseFrameOffset(bool UseOffset);
            virtual void SetLimit(int Axis, Real Low, Real High);
            virtual void CalculateTransforms();

            /// @copydoc TypedConstraint::ValidParamOnAxis(int) const
            virtual TypedConstraint::ParamList ValidParamOnAxis(int Axis) const;
            /// @copydoc TypedConstraint::ValidLinearAxis() const
            virtual TypedConstraint::AxisList ValidLinearAxis() const;
            /// @copydoc TypedConstraint::ValidAngularAxis() const
            virtual TypedConstraint::AxisList ValidAngularAxis() const;
            /// @copydoc TypedConstraint::ValidAngularAxis(ConstraintParam,int) const
            virtual bool HasParamBeenSet(ConstraintParam Param, int Axis) const;

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
            /// @warning A precondition of using this is that all of the actors intended for use must already be Deserialized.
            virtual void ProtoDeSerialize(const xml::Node& OneNode);

            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "Generic6DofConstraint"
            String SerializableName() const;
#endif // /PHYSXML
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
            ////////////////////////////////////////////////////////////////////////////////
            // HingeConstraint Construction and Destruction
            /// @brief Creates a Hinge constraint that will connect two actors together by their offsets.
            /// @param ActorA The first actor to apply this constraint to.
            /// @param ActorB The second actor to apply this constraint to.
            /// @param PivotA The location in ActorA's local space to apply the constraint to.
            /// @param PivotB The location in ActorB's local space to apply the constraint to.
            /// @param AxisInA The axis(for ActorA) on which the hinge is to act.  For example, a door hinge would be (0.0,1.0,0.0), aka the positive Y axis.
            /// @param AxisInB The axis(for ActorB) on which the hinge is to act.  For example, a door hinge would be (0.0,1.0,0.0), aka the positive Y axis.
            /// @param UseReferenceFrameA By default, this constraint uses ActorB's local space as the reference for certain values, such as the rotational limits. This simply controls whether or not it should use ActorA's local space instead.
            HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& PivotInA, const Vector3& PivotInB, const Vector3& AxisInA, const Vector3& AxisInB, bool UseReferenceFrameA=false);
            /// @brief Creates a Hinge constraint that will attach an actor to a point in world space.
            /// @param ActorA The actor to apply this constraint to.
            /// @param PivotInA The point in the object's(ActorA) local space where the constraint is to be attached to world space.
            /// @param AxisInA The axis(for ActorA) on which the hinge is to act.  For example, a door hinge would be (0.0,1.0,0.0), aka the positive Y axis.
            /// @param UseReferenceFrameA By default, this constraint uses ActorB's local space as the reference for certain values, such as the rotational limits. This simply controls whether or not it should use ActorA's local space instead.
            HingeConstraint(ActorRigid* ActorA, const Vector3& PivotInA, const Vector3& AxisInA, bool UseReferenceFrameA=false);
            /// @brief Create a Hinge with components of a tranform
            /// @param ActorA The first actor to apply this constraint to.
            /// @param ActorB The second actor to apply this constraint to.
            /// @param VectorA The location component of Transform A
            /// @param VectorB The location component of Transform B
            /// @param QuaternionA The rotation component of Transform A
            /// @param QuaternionB The rotation component of Transform B
            /// @param UseReferenceFrameA By default, this constraint uses ActorB's local space as the reference for certain values, such as the rotational limits. This simply controls whether or not it should use ActorA's local space instead.
            HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA, const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseReferenceFrameA=false);
            /// @brief Create a Hinge with components of a tranform
            /// @param ActorA The first actor to apply this constraint to.
            /// @param ActorB The second actor to apply this constraint to.
            /// @param TransformA The location component of Transform A
            /// @param TransformB The location component of Transform B
            /// @param UseReferenceFrameA By default, this constraint uses ActorB's local space as the reference for certain values, such as the rotational limits. This simply controls whether or not it should use ActorA's local space instead.
            HingeConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Transform& TransformA, const Transform& TransformB, bool UseReferenceFrameA=false);
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~HingeConstraint();

            ////////////////////////////////////////////////////////////////////////////////
            // HingeConstraint Position and Orientation
            /// @brief Set The relative location of the hinge pivot from ActorA's Center of gravity.
            /// @param Location The New value for PivotA
            /// @details Ultimately this information winds up being stored in the TransformA
            virtual void SetAPivotLocation(const Vector3& Location);
            /// @brief Set The relative location of the hinge pivot from ActorB's Center of gravity.
            /// @param Location The New value for PivotB
            /// @details Ultimately this information winds up being stored in the TransformB
            virtual void SetBPivotLocation(const Vector3& Location);
            /// @brief Get the location of the hinge pivot relative to ActorA's Center of gravity
            /// @return A Vector3 with the pivot location.
            virtual Vector3 GetAPivotLocation() const;
            /// @brief Get the location of the hinge pivot relative to ActorB's Center of gravity
            /// @return A Vector3 with the pivot location.
            virtual Vector3 GetBPivotLocation() const;

            /// @brief Set The relative rotation of ActorA
            /// @param Rotation The new rotation amount for A
            /// @details Ultimately this information winds up being stored in the TransformA
            virtual void SetARotation(const Quaternion& Rotation);
            /// @brief Set The relative rotation of ActorB
            /// @param otation The new rotation amount for B
            /// @details Ultimately this information winds up being stored in the TransformB
            virtual void SetBRotation(const Quaternion& Rotation);
            /// @brief Get the relative rotation for ActorA
            /// @return A Quaternion that has the rotation
            virtual Quaternion GetARotation() const;
            /// @brief Get the relative rotation for ActorB
            /// @return A Quaternion that has the rotation
            virtual Quaternion GetBRotation() const;

            /// @brief Set the Position and Rotation using a Transform
            /// @param TranA The new Position and rotation
            virtual void SetTransformA(const Transform& TranA);
            /// @brief Set the Position and Rotation using a Transform
            /// @param TranB The new Position and rotation
            virtual void SetTransformB(const Transform& TranB);
            /// @brief Get the current Rotation and Location of Actor A
            /// @return This returns a phys::Transform
            virtual Transform GetTransformA() const;
            /// @brief Get the current Rotation and Location of Actor B
            /// @return This returns a phys::Transform
            virtual Transform GetTransformB() const;

            ////////////////////////////////////////////////////////////////////////////////
            // HingeConstraint Angular Motor
            /// @brief Enables(or Disables) the motor on the hinge and sets it's parameters.
            /// @param EnableMotor Sets whether or not the motor on this constraint is enabled.
            /// @param TargetVelocity The desired velocity of rotation the motor will have.  This may or may not be achieved based on obstructions in the simulation.
            /// @param MaxMotorImpulse The maximum amount of force the motor is to apply to try and reach it's target velocity.
            virtual void EnableMotor(bool EnableMotor, Real TargetVelocity, Real MaxMotorImpulse);
            /// @brief Enables(or Disables) the motor on the hinge.
            /// @param EnableMotor Sets whether or not the motor on this constraint is enabled.
            virtual void EnableMotor(bool EnableMotor);
            /// @brief Is this motor on this hinge enabled?
            /// @return True if it is, false otherwise.
            virtual bool GetMotorEnabled() const;
            /// @brief Sets the maximum amount of force the motor is to apply.
            /// @param MaxMotorImpulse The maximum amount of force the motor is to apply to try and reach it's target velocity.
            virtual void SetMaxMotorImpulse(Real MaxMotorImpulse);
            /// @brief Retrieve the maximimum value that the acceleration of the motor can be increased.
            /// @return A real containing the maximum impulse.
            virtual Real GetMaxMotorImpulse() const;
            /// @brief Sets a Target Velocity, indirectly using the angle stored in a quaternion.
            /// @details Is implemented in terms of SetMotorTarget(Real, Real);
            /// @param QuatAInB The angle a quaternion relative to the two objects in the constraint.
            /// @param Dt The Desired Time steps that the target rotational velocity should be reached in.
            virtual void SetMotorTarget(const Quaternion& QuatAInB, Real Dt);
            /// @brief Set the Rotational velocity in a more direct fashion
            /// @param TargetAngle The desired angle in radians.
            /// @param Dt The Desired Time steps that the target rotational velocity should be reached in.
            virtual void SetMotorTarget(Real TargetAngle, Real Dt);
            /// @brief Desired angular velocity of the motor
            /// @param TargetVelocity The Desired velocity
            virtual void SetMotorTargetVelocity(Real TargetVelocity);
            /// @brief Get the Target Velocity.
            /// @return the target valocity as a real.
            virtual Real GetMotorTargetVelocity() const;

            ////////////////////////////////////////////////////////////////////////////////
            // HingeConstraint Limits
            /// @brief Sets the angle limits of the constraint in radians.
            /// @param Low The minimum angle limit for the constraint in radians.
            /// @param High The maximum angle limit for the constraint in radians.
            /// @param Softness Not currently used internally.
            /// @param BiasFactor Multiplier for the constraint error, constraint appears more "soft" when closer to zero.
            /// @param RelaxationFactor The amount of bounce to apply when the constraint reaches it's limit.  Range: 0.0-1.0.
            virtual void SetLimit(Real Low, Real High, Real Softness=0.9, Real BiasFactor=0.3, Real RelaxationFactor=1.0);
            /// @brief Return the Lower Limit of the hinge
            /// @return A real containing the Lower Limit
            virtual Real GetLimitLow() const;
            /// @brief Return the Upper Limit of the hinge
            /// @return A real containing the Higher Limit
            virtual Real GetLimitHigh() const;
            /// @brief Return the Softness of the hinge
            /// @return A real containing the Softness
            virtual Real GetLimitSoftness() const;
            /// @brief Return the bias factor of the hinge (Not entirely certain hat this on is)
            /// @return A real containing the bias Factor
            virtual Real GetLimitBiasFactor() const;
            /// @brief Return the Relaxation Factor of the hinge
            /// @return A real containing the Relaxation Factor
            virtual Real GetLimitRelaxationFactor() const;

            ////////////////////////////////////////////////////////////////////////////////
            // HingeConstraint Details
            /// @brief Sets the axis on which this constraint acts.
            /// @param AxisInA A vector3 representing the axis to be used with this constraint.
            virtual void SetAxis(const Vector3& AxisInA);
            /// @copydoc TypedConstraint::ValidParamOnAxis(int) const
            virtual TypedConstraint::ParamList ValidParamOnAxis(int Axis) const;
            /// @copydoc TypedConstraint::ValidLinearAxis() const
            virtual TypedConstraint::AxisList ValidLinearAxis() const;
            /// @copydoc TypedConstraint::ValidAngularAxis() const
            virtual TypedConstraint::AxisList ValidAngularAxis() const;
            /// @copydoc TypedConstraint::ValidAngularAxis(ConstraintParam,int) const
            virtual bool HasParamBeenSet(ConstraintParam Param, int Axis) const;
            /// @brief Retrieve the stored value from the physics subsystem(bullet)
            /// @return a True or false.
            virtual bool GetUseFrameOffset() const;
            /// @brief Set the stored value for UseFrameOffset on this hinge in the physics subsystem(bullet)
            /// @param FrameOffset The new desired value.
            virtual void SetUseFrameOffset(bool FrameOffset);
            /// @brief Is this Using Reference Frame A
            /// @return A the value UseReferenceFrameA is set to internally.
            virtual bool GetUseReferenceFrameA() const;
            /// @brief Change whether this is Using Reference Frame A or not
            /// @param UseReferenceFrameA Whether certain math be performed from the perspective of Actor A or Actor B (we think this is the case, but we have not test thoroughly)
            virtual void SetUseReferenceFrameA(bool UseReferenceFrameA=false);

#ifdef PHYSXML
            ////////////////////////////////////////////////////////////////////////////////
            // HingeConstraint Serialization
            // Serializable
            /// @brief Convert this class to an xml::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this vectorw should be appended to.
            virtual void ProtoSerialize(xml::Node& CurrentRoot) const;
            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and xml::Node containing the data.
            /// @warning A precondition of using this is that all of the actors intended for use must already be Deserialized.
            virtual void ProtoDeSerialize(const xml::Node& OneNode);
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "HingeConstraint"
            String SerializableName() const;
#endif // /PHYSXML

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
    /// @brief Tries to make a point relative to each of two actors match in 3d space, without regard to rotation.
    /// @details This will translate (push around) the actors to attempt to get the offsets to match up in 3d space.
    /// The actors can freely rotate about their relative offsets.
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
            /// @param PivotA The position relative to ActorA's center of gravity to "Pin" to the world.
            Point2PointConstraint(ActorRigid* ActorA, const Vector3& PivotA);
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~Point2PointConstraint();
            /// @brief Set offset of the first actor.
            /// @param PivotA The offset as a Vector3 relative to the center of mass of ActorA.
            virtual void SetPivotA(const Vector3& PivotA);
            /// @brief Set offset of the second actor.
            /// @param PivotB The offset as a Vector3 relative to the center of mass of ActorB.
            virtual void SetPivotB(const Vector3& PivotB);
            /// @brief Get offset of the first actor.
            /// @return The offset as a Vector3 relative to the center of mass of ActorA.
            virtual Vector3 GetPivotA() const;
            /// @brief Get offset of the second actor.
            /// @return The offset as a Vector3 relative to the center of mass of ActorB.
            virtual Vector3 GetPivotB() const;
            /// @brief Set the current impulse clamping on the constraint
            /// @param Clamping This is a value that the constraint solver can use to adjust accumlated values when solving the constraint.
            virtual void SetImpulseClamping(Real Clamping);
            /// @brief get the current impulse clamping value
            /// @return A real with the Clamping
            virtual Real GetImpulseClamping() const;
            /// @brief Set a resistive force against the constraint, not too dissimilar to from hinge friction or Air resistance
            /// @param Damping A real with the desired values
            virtual void SetDamping(Real Damping);
            /// @brief Get the current Damping
            /// @return A Real with the Damping value.
            virtual Real GetDamping() const;

            /// @brief This may be a scalar for how strongly Angular momentum affects linear momemtum
            /// @todo Research this more carefully
            /// @details This function is a tightly wrapped bullet 3d function. No real documentation for it exists, from its responsibility/location in Bullet3d and
            /// a basic understanding of torque ( see http://en.wikipedia.org/wiki/Torque ) It is highly likely that it is a value to adjust how torque affects momentum.
            virtual void SetTAU(Real TAU);
            /// @brief Retrieve the Tau Setting
            /// @return The Tau value as a Real
            virtual Real GetTAU() const;

            /// @copydoc TypedConstraint::ValidParamOnAxis(int) const
            virtual TypedConstraint::ParamList ValidParamOnAxis(int Axis) const;
            /// @copydoc TypedConstraint::ValidLinearAxis() const
            virtual TypedConstraint::AxisList ValidLinearAxis() const;
            /// @copydoc TypedConstraint::ValidAngularAxis() const
            virtual TypedConstraint::AxisList ValidAngularAxis() const;
            /// @copydoc TypedConstraint::ValidAngularAxis(ConstraintParam,int) const
            virtual bool HasParamBeenSet(ConstraintParam Param, int Axis) const;

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
            /// @warning A precondition of using this is that all of the actors intended for use must already be Deserialized.
            virtual void ProtoDeSerialize(const xml::Node& OneNode);

            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "Point2PointConstraint"
            String SerializableName() const;
#endif
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

#ifdef PHYSXML
    ///////////////////////////////////////////////////////////////////////////////
    // Class External << Operators for streaming or assignment

    /// @brief Convert a constraint to XML and send it down a stream
    /// @param stream The stream to send it down
    /// @param x The constraint to send down
    /// @return This returns the output stream to allow operator chaining.
    std::ostream& operator << (std::ostream& stream, const phys::TypedConstraint& x);

    /// @brief Get a constraint from an XML stream
    /// @param stream The stream to get it out of
    /// @param x The it you will get out of the stream
    /// @return This returns the input stream to allow operator chaining.
    std::istream& operator >> (std::istream& stream, phys::TypedConstraint& x);

    /// @brief Converts an XML Node into a functional in memory construct.
    /// @param OneNode The xml node that contains the deserialize class instance.
    /// @param x The class instance to overwrite witht the proto serialized version in the node.
    void operator >> (const phys::xml::Node& OneNode, phys::TypedConstraint& x);
#endif // \PHYSXML



#endif

