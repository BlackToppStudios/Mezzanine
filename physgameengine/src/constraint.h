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

    /// @brief Get a Constraint Parameter as a String.
    /// @param Param The Parameter to get as a String.
    /// @return A String That contains the name of a ConstraintParam.
    String ConstraintParamAsString(ConstraintParam Param);

    /// @brief Convert A string that might be ConstraintParam to one.
    /// @param Param A String that should be storing, a constraint param name.
    /// @return if the name matches on of the constraintParams exactly, return that one.
    /// @throw If the Param does not match a type exactly, then this will throw an exception.
    ConstraintParam StringAsConstraintParam(String Param);

    /// @brief How many ConstraintParam Exist.
    /// @details Used in some algorithms and we didn't want it to look like a magic so we defined it here.
    const int ConstraintParamCount = 4;

    ///////////////////////////////////////////////////////////////////////////////
    /// @class TypedConstraint
    /// @headerfile constraint.h
    /// @brief This is the base class for all constraints supported.
    /// @details This class provides the basis for all the other constraints.  This is
    /// also a virtual class and provides no constraint properties of it's own, thus
    /// shouldn't/can't be called on manually. \n \n
    /// Any class implementing a this must implement at least these functions to
    /// create a minimally functiontal constraint: \n
    ///     - GetConstraintBase() const - Return A pointer to the bullet constraint they use
    ///     - SetPivotALocation(const Vector3&) - Set Location of pivot Relative to A
    ///     - SetPivotALocation(const Vector3&) - Set Location of pivot Relative to B
    ///     - GetPivotALocation() const - Get Location of pivot Relative to A
    ///     - GetPivotBLocation() const - Get Location of pivot Relative to B
    ///     - ValidParamOnAxis(int Axis) const - What parameters can be changed on the given Axis
    ///     - ValidLinearAxis() const - Which Axis Suppport Translation
    ///     - ValidAngularAxis() const - Which axis support Rotation
    ///     - HasParamBeenSet(ConstraintParam Param, int Axis) const - Has a the given param on the given axis been set
    ///
    /// It is advisable to re-implement a few more functions as well, these all have implementation writtens in terms of
    /// of the function that must be implemented, but the genericity of them may impede performance.
    ///     - ValidAxis() const - Full List of all valid Axis. Combines the lists from ValidLinearAxis() const and ValidAngularAxis() const.
    ///     - IsParamValidOnAxis(ConstraintParam, int) const - Uses the other function to check Axis and then check if the param is valid there
    ///     - ValidParams() const - A list of Parameters every Axis on this constraint implements
    ///
    ///
    ///////////////////////////////////////////////////////////////////////////////
    class PHYS_LIB TypedConstraint
    {
        protected:
            friend class World;
            friend class PhysicsManager;

            ////////////////////////////////////////////////////////////////////////////////
            // TypedConstraint Data Members
            /// @brief First rigid body the constraint applies to.
            btRigidBody* BodyA;
            /// @brief Second rigid body the constraint applies to(if applicable).
            btRigidBody* BodyB;
            /// @brief First Actor the constraint applies to.
            ActorRigid* ActA;
            /// @brief Second Actor the constraint applies to.
            ActorRigid* ActB;

            ////////////////////////////////////////////////////////////////////////////////
            // TypedConstraint Protected Methods
            /// @brief Get the Bullet constraint that this class encapsulates.
            /// @return A pointer to the btTypedConstraint that stores the underlying constraint.
            virtual btTypedConstraint* GetConstraintBase() const = 0;
            /// @brief Sets the Internal actor pointers.
            void SetBodies(ActorRigid* Act1, ActorRigid* Act2);
            /// @brief Sets the Internal actor pointers.
            void SetBodies(ActorRigid* Act1);
            /// @brief No initialization constructor.
            /// @details This is protected to prevent creating an instance of this directly.
            TypedConstraint();
        public:
            ////////////////////////////////////////////////////////////////////////////////
            // TypedConstraint Core Functionality
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~TypedConstraint();

            /// @brief Gets the first actor this constraint applies to.
            /// @return Returns a pointer to the first actor this constraint applies to.
            virtual ActorRigid* GetActorA() const;
            /// @brief Gets the second actor this constraint applies to.
            /// @return Returns a pointer to the second actor this constraint applies to.
            virtual ActorRigid* GetActorB() const;

            ////////////////////////////////////////////////////////////////////////////////
            // TypedConstraint Position and Orientation
            /// @brief Set offset of the first actor.
            /// @param PivotA The offset as a Vector3 relative to the center of mass of ActorA.
            virtual void SetPivotALocation(const Vector3& PivotA) = 0;
            /// @brief Set offset of the second actor.
            /// @param PivotB The offset as a Vector3 relative to the center of mass of ActorB.
            virtual void SetPivotBLocation(const Vector3& PivotA) = 0;
            /// @brief Get offset of the first actor.
            /// @return The offset as a Vector3 relative to the center of mass of ActorA.
            virtual Vector3 GetPivotALocation() const = 0;
            /// @brief Get offset of the second actor.
            /// @return The offset as a Vector3 relative to the center of mass of ActorB.
            virtual Vector3 GetPivotBLocation() const = 0;
            /// @brief Set the current impulse clamping on the constraint
            /// @param Clamping This is a value that the constraint solver can use to adjust accumlated values when solving the constraint.

            ///////////////////////////////////////////////////////////////////////////////
            // TypedConstraint Parameters
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
            // TypedConstraint Serialization
#ifdef PHYSXML
            // Serializable
            /// @brief Convert this class to an xml::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this vectorw should be appended to.
            /// @details This stores each Actor's name and every constraint parameter on each if it has been set.
            /// This should allow for looking up the required actors in the Actor Manager. This should also prevent
            /// any interference with different values and minimize the size of the serialized version
            virtual void ProtoSerialize(xml::Node& CurrentRoot) const;

            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and xml::Node containing the data.
            /// @details This should only be run once all the actors have finishes being deserialized. This will fail
            /// if the actors it expects are not present.
            virtual void ProtoDeSerialize(const xml::Node& OneNode);

            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "TypedConstraint"
            String SerializableName() const;
#endif
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief All constraints that track rotation and location of the Pivot relative to each Actor inherit from this.
    /// @details Since not all contraints provide tracking for the Actor Transform (location/rotation of the pivot)
    /// we subdivided the constraints to unify the interface of those that could be unified. This stores nothing, but
    /// provides uniform access to the transform and rotation functions. \n \n
    /// Any deriving class must implement every thing from TypedConstraint and the four set/get Transform function. It
    /// is expected that more derived classes will implement the Set/Get Pivot/Rotation Functino in a more efficient
    /// Manner if a more efficient way exists. The non-transform get/set function are implmented in terms of the
    /// get/set transform function extra copies of values and extra reading/writing may occur if the compiler weakly
    /// optimizes. Of course, implementing more functions could slow down the software if it no longer fits in CPU
    /// caches. As always benchmark if this is something that may be critically slowing you down.
    ///////////////////////////////////////////////////////////////////////////////
    class PHYS_LIB DualTransformConstraint : public TypedConstraint
    {
        public:
        /// @brief Set the Position and Rotation using a Transform
        /// @param TranA The new Position and rotation
        virtual void SetPivotATransform(const Transform& TranA) = 0;
        /// @brief Set the Position and Rotation using a Transform
        /// @param TranB The new Position and rotation
        virtual void SetPivotBTransform(const Transform& TranB) = 0;
        /// @brief Get the current Rotation and Location of Actor A
        /// @return This returns a phys::Transform
        virtual Transform GetPivotATransform() const = 0;
        /// @brief Get the current Rotation and Location of Actor B
        /// @return This returns a phys::Transform
        virtual Transform GetPivotBTransform() const = 0;

        /// @brief Set The relative location of the hinge pivot from ActorA's Center of gravity.
        /// @param Location The New value for PivotA
        /// @details Ultimately this information winds up being stored in the TransformA.
        virtual void SetPivotALocation(const Vector3& Location)
            { SetPivotATransform( Transform(Location, GetPivotARotation()) ); }
        /// @brief Set The relative location of the hinge pivot from ActorB's Center of gravity.
        /// @param Location The New value for PivotB
        /// @details Ultimately this information winds up being stored in the TransformB
        virtual void SetPivotBLocation(const Vector3& Location)
            { SetPivotBTransform( Transform(Location, GetPivotBRotation()) ); }
        /// @brief Get the location of the hinge pivot relative to ActorA's Center of gravity
        /// @return A Vector3 with the pivot location.
        virtual Vector3 GetPivotALocation() const
            { return GetPivotATransform().Location; }
        /// @brief Get the location of the hinge pivot relative to ActorB's Center of gravity
        /// @return A Vector3 with the pivot location.
        virtual Vector3 GetPivotBLocation() const
            { return GetPivotBTransform().Location; }

        /// @brief Set The relative rotation of ActorA
        /// @param Rotation The new rotation amount for A
        /// @details Ultimately this information winds up being stored in the TransformA
        virtual void SetPivotARotation(const Quaternion& Rotation)
            { SetPivotATransform( Transform(GetPivotALocation(), Rotation) ); }
        /// @brief Set The relative rotation of ActorB
        /// @param otation The new rotation amount for B
        /// @details Ultimately this information winds up being stored in the TransformB
        virtual void SetPivotBRotation(const Quaternion& Rotation)
            { SetPivotBTransform( Transform(GetPivotBLocation(), Rotation) ); }
        /// @brief Get the relative rotation for ActorA
        /// @return A Quaternion that has the rotation
        virtual Quaternion GetPivotARotation() const
            { return GetPivotATransform().Rotation; }
        /// @brief Get the relative rotation for ActorB
        /// @return A Quaternion that has the rotation
        virtual Quaternion GetPivotBRotation() const
            { return GetPivotBTransform().Rotation; }

        ///////////////////////////////////////////////////////////////////////////////
        // DualTransformConstraint Serialization
#ifdef PHYSXML
        // Serializable
        /// @brief Convert this class to an xml::Node ready for serialization
        /// @param CurrentRoot The point in the XML hierarchy that all this vectorw should be appended to.
        /// @details This stores each Actor's Transform
        virtual void ProtoSerialize(xml::Node& CurrentRoot) const;

        // DeSerializable
        /// @brief Take the data stored in an XML and overwrite this instance of this object with it
        /// @param OneNode and xml::Node containing the data.
        virtual void ProtoDeSerialize(const xml::Node& OneNode);

        /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
        /// @return A string containing "DualTransformConstraint"
        String SerializableName() const;
#endif
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ConeTwistConstraint
    /// @headerfile constraint.h
    /// @brief
    /// @details This class is currently incomplete
    ///////////////////////////////////////
    class PHYS_LIB ConeTwistConstraint : public TypedConstraint
    {
        protected:
            /// @brief Bullet constraint that this class encapsulates.
            btConeTwistConstraint* ConeTwist;
            /// @copydoc TypedConstraint::GetConstraintBase() const
            virtual btTypedConstraint* GetConstraintBase() const;
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
    /// @brief Create simple but specific limits on any axis of movement or rotation
    /// @details Provides optional limits on the x, y, and z for rotation and
    /// translation. You can get the ID of the axis with ValidLinearAxis() or
    /// ValidAngularAxis()  for this particular constraint at the time of this
    /// writing the constraint axis looked liked this:
    ///     - 0: Translation X
    ///     - 1: Translation Y
    ///     - 2: Translation Z
    ///     - 3: Rotation X
    ///     - 4: Rotation Y
    ///     - 5: Rotation Z
    ///
    /// This has to perform a larger amount of calculations and should be used only
    /// when other simpler constraints have been ruled out.
    /// @n @n
    /// As with all limits when the upper and lower limits match the two actors will
    /// be locked on that axis and relative translation or rotation will not be
    /// permitted (within the bounds or error correction). If the Lower limit is
    /// lower than the Upper limit a range of translation or rotation will be
    /// allowed. If the Lower limit is higher than the Upper limit this will cause
    /// the system to realized that no position can satisfy the constraint and
    /// no restriction will be enforced.
    /// @n @n
    /// Each of the Axis also has a motor that can be enabled to cause a specified
    /// amount of translation or rotation. To aid in selection of specific Axis,
    /// you should use the UsableAxis enum
    ///////////////////////////////////////
    class PHYS_LIB Generic6DofConstraint : public DualTransformConstraint
    {
        protected:
            /// @brief Bullet constraint that this class encapsulates.
            btGeneric6DofConstraint* Generic6dof;
            /// @brief Inheritance Constructor.
            /// @details This is only called by derived classes, and shouldn't be called manually.
            Generic6DofConstraint();
            /// @copydoc TypedConstraint::GetConstraintBase() const
            virtual btTypedConstraint* GetConstraintBase() const;
        public:

            /// @brief Identify the Axis a bit easier when iterating over them is less convienent than typing an Identifier
            enum UsableAxis{
                LinearX         = 0,    ///< Translation on the X axis
                LinearY         = 1,    ///< Translation on the Y axis
                LinearZ         = 2,    ///< Translation on the Z axis
                AngularX        = 3,    ///< Rotation on the X axis
                AngularY        = 4,    ///< Rotation on the Y axis
                AngularZ        = 5,    ///< Rotation on the Z axis

                AngularXAsRotationalAxis        = 0,    ///< Rotation on the X axis, when working with only rotational Axis
                AngularYAsRotationalAxis        = 1,    ///< Rotation on the Y axis, when working with only rotational Axis
                AngularZAsRotationalAxis        = 2,    ///< Rotation on the Z axis, when working with only rotational Axis
            };

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofConstraint Construction and Destruction
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
            /// @brief Body and World Verbose constructor
            /// @param ActorA The body to be bound to the world
            /// @param VectorB The offset for the ActorB pivot/hinge/joint
            /// @param QuaternionB The rotation
            /// @param UseLinearReferenceB Perform Linear math from ActorB's perspective, default to true.
            /// @details This Joins an object to the world.
            Generic6DofConstraint(ActorRigid* ActorB, const Vector3& VectorB, const Quaternion& QuaternionB, bool UseLinearReferenceB = true);
            /// @brief Body and World Terse constructor
            /// @param ActorA The body to be bound to the world
            /// @param TransformB The offset and rotation for the ActorB pivot/hinge/joint
            /// @param UseLinearReferenceB Perform Linear math from ActorB's perspective, default to true.
            /// @details This Joins an object to the world.
            Generic6DofConstraint(ActorRigid* ActorB, const Transform& TransformB, bool UseLinearReferenceB = true);
            /// @brief Class destructor.
            virtual ~Generic6DofConstraint();

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofConstraint Location and Rotation
            /// @copydoc DualTransformConstraint::SetPivotATransform(const Transform&)
            virtual void SetPivotATransform(const Transform& TranA);
            /// @copydoc DualTransformConstraint::SetPivotBTransform(const Transform&)
            virtual void SetPivotBTransform(const Transform& TranB);
            /// @copydoc DualTransformConstraint::GetPivotATransform()
            virtual Transform GetPivotATransform() const;
            /// @copydoc DualTransformConstraint::GetPivotBTransform()
            virtual Transform GetPivotBTransform() const;

            /// @copydoc DualTransformConstraint::SetPivotALocation(const Vector3&)
            virtual void SetPivotALocation(const Vector3& Location);
            /// @copydoc DualTransformConstraint::SetPivotBLocation(const Vector3&)
            virtual void SetPivotBLocation(const Vector3& Location);
            /// @copydoc DualTransformConstraint::GetPivotALocation()
            virtual Vector3 GetPivotALocation() const;
            /// @copydoc DualTransformConstraint::GetPivotBLocation()
            virtual Vector3 GetPivotBLocation() const;

            /// @copydoc DualTransformConstraint::SetPivotARotation(const Quaternion&)
            virtual void SetPivotARotation(const Quaternion& Rotation);
            /// @copydoc DualTransformConstraint::SetPivotBRotation(const Quaternion&)
            virtual void SetPivotBRotation(const Quaternion& Rotation);
            /// @copydoc DualTransformConstraint::GetPivotARotation()
            virtual Quaternion GetPivotARotation() const;
            /// @copydoc DualTransformConstraint::GetPivotBRotation()
            virtual Quaternion GetPivotBRotation() const;

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofConstraint Basic Limit Accessors
            /// @brief Change the upper and lower limit for one axis of translation or rotation limit
            /// @param Axis The axis to change
            /// @param Lower The new lower limit
            /// @param UpperThe new Higher limit
            virtual void SetLimit(int Axis, Real Lower, Real Upper);

            /// @brief Set the lower limits on translation
            /// @param Limit A Vector3 that stores the lower limit x, y and z that cannot be exceeded
            virtual void SetLinearLimitUpper(const Vector3& Limit);
            /// @brief Set the Upper limits on translation
            /// @return A Vector3 that stores the upper limit x, y and z that cannot be exceeded
            virtual void SetLinearLimitLower(const Vector3& Limit);
            /// @brief Get the lower limits on translation
            /// @param Limit A Vector3 that stores the lower limit x, y and z that cannot be exceeded
            virtual Vector3 GetLinearLimitUpper() const;
            /// @brief Get the Upper limits on translation
            /// @return A Vector3 that stores the upper limit x, y and z that cannot be exceeded
            virtual Vector3 GetLinearLimitLower() const;

            /// @brief Set the Upper limits on rotation
            /// @param Limit A Vector3 that store the lower limit x, y and z rotation in radians
            virtual void SetAngularLimitUpper(const Vector3& Limit);
            /// @brief Set the Lower limits on rotation
            /// @param Limit A Vector3 that store the upper limit x, y and z rotation in radians
            virtual void SetAngularLimitLower(const Vector3& Limit);
            /// @brief Get the Power limits on rotation
            /// @return Limit A Vector3 that stores the lower limit x, y and z rotation in radians
            virtual Vector3 GetAngularLimitUpper() const;
            /// @brief Get the Upper limits on rotation
            /// @return Limit A Vector3 that stores the upper limit x, y and z rotation in radians
            virtual Vector3 GetAngularLimitLower() const;

            /// @brief Get a specific lower rotational limit
            /// @param RotationalAxis The Axis to work with.
            Real GetAngularLimitLowerOnAxis(int RotationalAxis) const;
            /// @brief Get a specific upper rotational limit
            /// @param RotationalAxis The Axis to work with.
            /// @details This  selects axis with
            /// @return A real containing the specified upper limit
            Real GetAngularLimitUpperOnAxis(int RotationalAxis) const;

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofConstraint Angular Limit and Motor Details
            /// @brief Convert to internal Axis IDs from external or internal axis IDs
            /// @param Axis the Axis ID to be converted
            /// @return A number that correlates toe corresponding internal Axis. For example Axis 4 is the Rotation Y. This would return the corresponding ID for the internal Rotation Y axis
            /// @details Due to the way this is stored internally, All the axis are listed from from
            /// 0 to 2. So rather than throw an exception(or have undefined behavior) if the Axis
            /// selected one of those, this select the axis like this:
            ///     - 0, 3: Rotation X
            ///     - 1, 4: Rotation Y
            ///     - 2, 5: Rotation Z
            /// @return A real containing the specified lower limit
            inline virtual int AxisToAngularAxis(int Axis) const
                { return (Axis%3); }

            /// @brief Set the Maximimum amount of force applied to ensure limits are not surpassed.
            /// @param MaxLimitForces A Vector3 containing the X, Y and Z Maximium forces.
            virtual void SetAngularLimitMaxForce(const Vector3& MaxLimitForces);
            /// @brief Set the Maximimum amount of force applied to ensure a limit on one axis is not surpassed.
            /// @param MaxLimitForce The new maximum force.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(int).
            virtual void SetAngularLimitMaxForceOnAxis(Real MaxLimitForce, int Axis);
            /// @brief Get the Maximimum amount of force applied to ensure limits are not surpassed.
            /// @return A Vector3 with the forces on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularLimitMaxForce() const;
            /// @brief Get the Maximimum amount of force applied to ensure a limit one axis is not surpassed.
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(int).
            /// @return A Real with the force for the Given Axis.
            virtual Real GetAngularLimitMaxForceOnAxis(int Axis) const;

            /// @brief Set the Target velocity of the motor on each anuglar axis.
            /// @param Velocities A Vector3 containing the X, Y and Z Target Velocites.
            virtual void SetAngularMotorTargetVelocity(const Vector3& Velocities);
            /// @brief For one Axis, set the target velocity of the angular motor.
            /// @param Velocity The new Target Velovity.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(int).
            virtual void SetAngularMotorTargetVelocityOnAxis(Real Velocity, int Axis);
            /// @brief Get the target velocity for all angular Axis
            /// @return A Vector3 with the TAger Velocities on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularMotorTargetVelocity() const;
            /// @brief Get the Target Velocity for one axis.
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(int).
            /// @return A Real with the force for the Given Axis.
            virtual Real GetAngularMotorTargetVelocityOnAxis(int Axis) const;

            /// @brief Set the Angular Motor Maximum force on all 3 rotational axis
            /// @param Forces A Vector3 with the Max Motor Force for each axis.
            virtual void SetAngularMotorMaxForce(const Vector3& Forces);
            /// @brief For one Axis, set the Maximimum Motor Force.
            /// @param Force The new Max motor force.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(int).
            virtual void SetAngularMotorMaxForceOnAxis(Real Force, int Axis);
            /// @brief Get the Max Motor Force for each Axis
            /// @return A Vector3 with the max force on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularMotorMaxForce() const;
            /// @brief Get the Max motor Force on a certain Axis.
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(int).
            /// @return A Real with the Max Motor Force for the Given Axis.
            virtual Real GetAngularMotorMaxForceOnAxis(int Axis) const;

            /// @brief Set the Angular Motor Damping for each Angular Axis.
            /// @param Dampings A Vector3 with Damping value for the X, Y and Z axis.
            virtual void SetAngularMotorDamping(const Vector3& Dampings);
            /// @brief For one Axis, set the Damping.
            /// @param Damping The new amount to Damp rotation on the given Axis.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(int).
            virtual void SetAngularMotorDampingOnAxis(Real Damping, int Axis);
            /// @brief Get the Damping for all Angular Axis.
            /// @return A Vector3 with the forces on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularMotorDamping() const;
            /// @brief Get the Damping for one given Axis.
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(int).
            /// @return A Real with the XXX for the Given Axis.
            virtual Real GetAngularMotorDampingOnAxis(int Axis) const;

            /// @brief Set the Bounciness/Restition for rotation on all three Axis
            /// @param Restistutions A Vector3 containing all the New Bounciness values
            virtual void SetAngularMotorRestitution(const Vector3& Restistutions);
            /// @brief For one Axis, set the Restistution/Bounciness/
            /// @param Restistution The new value for the given Axis.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(int).
            virtual void SetAngularMotorRestitutionOnAxis(Real Restistution, int Axis);
            /// @brief Get the Restistution values for all three axis
            /// @return A Vector3 with the forces on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularMotorRestitution() const;
            /// @brief Get the Restistution/Bounciness for a single Axis
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(int).
            /// @return A Real with the Restistution for the Given Axis.
            virtual Real GetAngularMotorRestitutionOnAxis(int Axis) const;

            /// @brief Set whether or not the motor is enabled for all Axis Simultaneously.
            /// @param Enableds A Vector3 that will be interpretted as 3 true/false values where 0 is false and any other value it true.
            virtual void SetAngularMotorEnabled(const Vector3& Enableds);
            /// @brief For one Axis, set whether or not the motor is enabled
            /// @param Enabled Is the motor enabled? TRue for yes, false for no.
            /// @param Axis The Angular Axis to be set, as per AxisToAngularAxis(int).
            virtual void SetAngularMotorEnabledOnAxis(bool Enabled, int Axis);
            /// @brief Get a Vector3 with 3 zero or nonzero values that store whether or not a given rotational motor is enable.
            /// @return A Vector3 with the forces on the X, Y and Z angular Axis.
            virtual Vector3 GetAngularMotorEnabled() const;
            /// @brief Is a specific rotational motor enabled.
            /// @param Axis The Angular Axis to get, as per AxisToAngularAxis(int).
            /// @return A bool that is true if the given Axis is enabled.
            virtual bool GetAngularMotorEnabledOnAxis(int Axis) const;

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofConstraint Linear Limit and Motor Details
            /// @brief Set the Softness of the linear Limits
            /// @param Softness How spongy, how much give does the constraint have.
            virtual void SetLinearLimitSoftness(Real Softness);
            /// @brief Get the Softness of the linear Limits
            /// @return The Softness as a real.
            virtual Real GetLinearLimitSoftness() const;

            /// @brief Set the Damping of the linear Limits
            /// @param Damping The new damping value placed on forces the limits impose
            virtual void SetLinearLimitDamping(Real Damping);
            /// @brief Get the Damping of the linear Limits
            /// @return The Damping as a real.
            virtual Real GetLinearLimitDamping() const;

            /// @brief Set the Restitution of the linear Limits.
            /// @param Restitution How bouncy are the limits.
            virtual void SetLinearLimitRestitution(Real Restitution);
            /// @brief Get the Restitution of the linear Limits.
            /// @return The Restitution as a real.
            virtual Real GetLinearLimitRestitution() const;

            /// @brief Set the Linear Motor Maximum force on all 3 translation axis
            /// @param Forces A Vector3 with the Max Motor Force for each axis.
            virtual void SetLinearMotorMaxForce(const Vector3& Forces);
            /// @brief For one Axis, set the Maximimum Motor Force.
            /// @param Force The new Max motor force.
            /// @param Axis The Linear Axis to be set.
            virtual void SetLinearMotorMaxForceOnAxis(Real Force, int Axis);
            /// @brief Get the Max Motor Force for each Axis
            /// @return A Vector3 with the max force on the X, Y and Z Linear Axis.
            virtual Vector3 GetLinearMotorMaxForce() const;
            /// @brief Get the Max motor Force on a certain Axis.
            /// @param Axis The Linear Axis to get.
            /// @return A Real with the Max Motor Force for the Given Axis.
            virtual Real GetLinearMotorMaxForceOnAxis(int Axis) const;

            /// @brief Set the Target velocity of the motor on each anuglar axis.
            /// @param Velocities A Vector3 containing the X, Y and Z Target Velocites.
            virtual void SetLinearMotorTargetVelocity(const Vector3& Velocities);
            /// @brief For one Axis, set the target velocity of the Linear motor.
            /// @param Velocity The new Target Velovity.
            /// @param Axis The Linear Axis to be set.
            virtual void SetLinearMotorTargetVelocityOnAxis(Real Velocity, int Axis);
            /// @brief Get the target velocity for all Linear Axis
            /// @return A Vector3 with the Target Velocities on the X, Y and Z Linear Axis.
            virtual Vector3 GetLinearMotorTargetVelocity() const;
            /// @brief Get the Target Velocity for one axis.
            /// @param Axis The Linear Axis to get.
            /// @return A Real with the force for the Given Axis.
            virtual Real GetLinearMotorTargetVelocityOnAxis(int Axis) const;

            /// @brief Set whether or not the motor is enabled for all Linear Axis Simultaneously.
            /// @param Enableds A Vector3 that will be interpretted as 3 true/false values where 0 is false and any other value it true.
            virtual void SetLinearMotorEnabled(const Vector3& Enableds);
            /// @brief For one Axis, set whether or not the motor is enabled
            /// @param Enabled Is the motor enabled? True for yes, false for no.
            /// @param Axis The Linear Axis to be set.
            virtual void SetLinearMotorEnabledOnAxis(bool Enabled, int Axis);
            /// @brief Get a Vector3 with 3 zero or nonzero values that store whether or not a given rotational motor is enable.
            /// @return A Vector3 with the forces on the X, Y and Z Linear Axis.
            virtual Vector3 GetLinearMotorEnabled() const;
            /// @brief Is a specific Linear motor enabled.
            /// @param Axis The Linear Axis to get.
            /// @return A bool that is true if the given Axis is enabled.
            virtual bool GetLinearMotorEnabledOnAxis(int Axis) const;

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofConstraint Axis, Params and other Details
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

            ///////////////////////////////////////////////////////////////////////////////
            // Generic6DofConstraint Serialization
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
    ///     - 0: Translation X
    ///     - 1: Translation Y
    ///     - 2: Translation Z
    ///     - 3: Rotation X
    ///     - 4: Rotation Y
    ///     - 5: Rotation Z
    ///////////////////////////////////////
    class PHYS_LIB Generic6DofSpringConstraint : public Generic6DofConstraint
    {
        protected:
            /// @brief Inheritance Constructor.
            /// @details This is only called by derived classes, and shouldn't be called manually.
            Generic6DofSpringConstraint();
            /// @copydoc TypedConstraint::GetConstraintBase() const
            virtual btGeneric6DofSpringConstraint* Generic6dofSpring() const;
        public:
            /// @brief Identify the Axis a bit easier when iterating over them is less convienent than typing an Identifier
            enum UsableAxis{
                LinearX         = 0,    ///< Translation on the X axis
                LinearY         = 1,    ///< Translation on the Y axis
                LinearZ         = 2,    ///< Translation on the Z axis
                AngularX        = 3,    ///< Rotation on the X axis
                AngularY        = 4,    ///< Rotation on the Y axis
                AngularZ        = 5,    ///< Rotation on the Z axis
            };

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofSpringConstraint Construction and Destruction
            /// @brief Two body Verbose constructor
            /// @param ActorA The First body to be bound
            /// @param ActorB  The Second body to be bound
            /// @param VectorA The offset from ActorA's center of gravity to get to match an offset from ActorB
            /// @param VectorB The offset from ActorB's center of gravity.
            /// @param QuaternionA Relative rotation from ActorA
            /// @param QuaternionB Relative rotation from ActorB
            /// @param UseLinearReferenceA Perform Linear math from ActorA's perspective, default to false.
            Generic6DofSpringConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA, const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseLinearReferenceA = false);
            /// @brief Two body Terse constructor
            /// @param ActorA The First body to be bound
            /// @param ActorB  The Second body to be bound
            /// @param TransformA The offset and rotation from ActorA's center of gravity to get to match an offset from ActorB
            /// @param TransformB The offset and rotation from ActorB's center of gravity.
            /// @param UseLinearReferenceA Perform Linear math from ActorA's perspective, default to false.
            Generic6DofSpringConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Transform& TransformA, const Transform& TransformB, bool UseLinearReferenceA = false);
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~Generic6DofSpringConstraint();

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofSpringConstraint Linear Spring Settings
            /// @brief Set the Stiffness of the springs on each Linear Axis.
            /// @param Stiffies A Vector3 containing the X, Y and Z stiffnesses.
            virtual void SetSpringLinearStiffness(const Vector3& Stiffies);
            /// @brief Set the Damping of the springs on each Linear Axis.
            /// @param Damps A Vector3 containing the X, Y and Z desired damping.
            virtual void SetSpringLinearDamping(const Vector3& Damps);
            /// @brief Set the Stiffness of the springs on each Linear Axis.
            /// @param Stiffies A Vector3 containing the X, Y and Z enabled statuses. This is interpretted as 0 for false and any other value for true.
            virtual void SetSpringLinearEnabled(const Vector3& Enableness);

            /// @brief Get the Stiffness for all Linear Axis
            /// @return A Vector3 with the Stiffness on the X, Y and Z Linear Axis.
            virtual Vector3 GetSpringLinearStiffness() const;
            /// @brief Get the Damping for all Linear Axis
            /// @return A Vector3 with the Damping on the X, Y and Z Linear Axis.
            virtual Vector3 GetSpringLinearDamping() const;
            /// @brief Get the Enabled Status for all Linear Axis
            /// @return A Vector3 with the Enabled Status on the X, Y and Z Linear Axis.
            virtual Vector3 GetSpringLinearEnabled() const;

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofSpringConstraint Angular Spring Settings
            /// @brief Set the Stiffness of the springs on each Angular Axis.
            /// @param Stiffies A Vector3 containing the X, Y and Z stiffnesses.
            virtual void SetSpringAngularStiffness(const Vector3& Stiffies);
            /// @brief Set the Damping of the springs on each Angular Axis.
            /// @param Damps A Vector3 containing the X, Y and Z desired damping.
            virtual void SetSpringAngularDamping(const Vector3& Damps);
            /// @brief Set the Stiffness of the springs on each Angular Axis.
            /// @param Stiffies A Vector3 containing the X, Y and Z enabled statuses. This is interpretted as 0 for false and any other value for true.
            virtual void SetSpringAngularEnabled(const Vector3& Enableness);

            /// @brief Get the Stiffness for all Angular Axis
            /// @return A Vector3 with the Stiffness on the X, Y and Z Angular Axis.
            virtual Vector3 GetSpringAngularStiffness() const;
            /// @brief Get the Damping for all Angular Axis
            /// @return A Vector3 with the Damping on the X, Y and Z Angular Axis.
            virtual Vector3 GetSpringAngularDamping() const;
            /// @brief Get the Enabled Status for all Angular Axis
            /// @return A Vector3 with the Enabled Status on the X, Y and Z Angular Axis.
            virtual Vector3 GetSpringAngularEnabled() const;

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofSpringConstraint Per Axis Spring Settings
            /// @brief Set the spring stiffness on a given axis
            /// @param Index The Desired axis. This accepts 0,1,2 for Linear X,Y, and Z or 3,4,5 for Angular X,Y, and Z. This can also accept Item from this classes Usable Axis enum;
            /// @param Stiffness A real with the new desired stiffness.
            virtual void SetSpringStiffness(int Index, Real Stiffness);
            /// @brief Set the spring Damping on a given axis.
            /// @param Index The Desired axis. This accepts 0,1,2 for Linear X,Y, and Z or 3,4,5 for Angular X,Y, and Z. This can also accept Item from this classes Usable Axis enum;
            /// @param Damping A real with the new desired Damping.
            virtual void SetSpringDamping(int Index, Real Damping);
            /// @brief Set the spring's enabled status on a given axis.
            /// @param Index The Desired axis. This accepts 0,1,2 for Linear X,Y, and Z or 3,4,5 for Angular X,Y, and Z. This can also accept Item from this classes Usable Axis enum;
            /// @param Enable A bool with the spring's enabled status.
            virtual void SetSpringEnabled(int Index, bool Enable);

            /// @brief Retrieve the Stiffness of the spring on the given axis
            /// @param Index The Desired axis. This accepts 0,1,2 for Linear X,Y, and Z or 3,4,5 for Angular X,Y, and Z. This can also accept Item from this classes Usable Axis enum;
            /// @return A real with the requested value;
            virtual Real GetSpringStiffness(int Index) const;
            /// @brief Retrieve the Damping of the spring on the given axis
            /// @param Index The Desired axis. This accepts 0,1,2 for Linear X,Y, and Z or 3,4,5 for Angular X,Y, and Z. This can also accept Item from this classes Usable Axis enum;
            /// @return A real with the requested value.
            virtual Real GetSpringDamping(int Index) const;
            /// @brief Retrieve the EnabledStatus of the spring on the given axis
            /// @param Index The Desired axis. This accepts 0,1,2 for Linear X,Y, and Z or 3,4,5 for Angular X,Y, and Z. This can also accept Item from this classes Usable Axis enum;
            /// @return A bool with the requested value.
            virtual bool GetSpringEnabled(int Index) const;

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofSpringConstraint Calculated Items
            /// @internal
            virtual void CalculateSpringEquilibriumPoint();
            /// @internal
            virtual void CalculateSpringEquilibriumPoint(int Index);

            /// @internal
            virtual Vector3 GetCurrentSpringAngularEquilibriumPoints() const;
            /// @internal
            virtual Vector3 GetCurrentSpringLinearEquilibriumPoints() const;
            /// @internal
            virtual Real GetCurrentSpringEquilibriumPoint(int Index) const;

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofSpringConstraint Serialization
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
    /// @class HingeConstraint
    /// @headerfile constraint.h
    /// @brief This is a constraint to be used to restrict the movement between two objects to angular rotation on a single axis.
    /// @details As the name suggests, this constraint essentially works like a door Hinge.
    ///////////////////////////////////////
    class PHYS_LIB HingeConstraint : public DualTransformConstraint
    {
        protected:
            /// @brief Bullet constraint that this class encapsulates.
            btHingeConstraint* Hinge;

            /// @copydoc TypedConstraint::GetConstraintBase() const
            virtual btTypedConstraint* GetConstraintBase() const;
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
            /// @details Ultimately this information winds up being stored in the TransformA. This implements
            /// a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
            virtual void SetPivotALocation(const Vector3& Location);
            /// @brief Set The relative location of the hinge pivot from ActorB's Center of gravity.
            /// @param Location The New value for PivotB
            /// @details Ultimately this information winds up being stored in the TransformB. This implements
            /// a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
            virtual void SetPivotBLocation(const Vector3& Location);
            /// @brief Get the location of the hinge pivot relative to ActorA's Center of gravity
            /// @return A Vector3 with the pivot location.
            /// @details This implements a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
            virtual Vector3 GetPivotALocation() const;
            /// @brief Get the location of the hinge pivot relative to ActorB's Center of gravity
            /// @return A Vector3 with the pivot location.
            /// @details This implements a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
            virtual Vector3 GetPivotBLocation() const;

            /// @brief Set The relative rotation of ActorA
            /// @param Rotation The new rotation amount for A
            /// @details Ultimately this information winds up being stored in the TransformA. This implements
            /// a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
            virtual void SetAPivotRotation(const Quaternion& Rotation);
            /// @brief Set The relative rotation of ActorB
            /// @param otation The new rotation amount for B
            /// @details Ultimately this information winds up being stored in the TransformB. This implements
            /// a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
            virtual void SetBPivotRotation(const Quaternion& Rotation);
            /// @brief Get the relative rotation for ActorA
            /// @return A Quaternion that has the rotation
            /// @details This implements a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
            virtual Quaternion GetAPivotRotation() const;
            /// @brief Get the relative rotation for ActorB
            /// @return A Quaternion that has the rotation
            /// @details This implements a more Hinge specific version of the logic than DualTransformConstraint for efficiency reasons.
            virtual Quaternion GetBPivotRotation() const;

            /// @copydoc DualTransformConstraint::SetTransformA
            virtual void SetPivotATransform(const Transform& TranA);
            /// @copydoc DualTransformConstraint::SetTransformB
            virtual void SetPivotBTransform(const Transform& TranB);
            /// @copydoc DualTransformConstraint::GetTransformA
            virtual Transform GetPivotATransform() const;
            /// @copydoc DualTransformConstraint::GetTransformB
            virtual Transform GetPivotBTransform() const;

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
    /// @details This class is incomplete
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
            ////////////////////////////////////////////////////////////////////////////////
            // Point2PointConstraint Construction and Destruction
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
            /// @copydoc TypedConstraint::GetConstraintBase() const
            virtual btTypedConstraint* GetConstraintBase() const;

            ////////////////////////////////////////////////////////////////////////////////
            // Point2PointConstraint Position and Orientation
            /// @brief Set offset of the first actor.
            /// @param PivotA The offset as a Vector3 relative to the center of mass of ActorA.
            virtual void SetPivotALocation(const Vector3& PivotA);
            /// @brief Set offset of the second actor.
            /// @param PivotB The offset as a Vector3 relative to the center of mass of ActorB.
            virtual void SetPivotBLocation(const Vector3& PivotB);
            /// @brief Get offset of the first actor.
            /// @return The offset as a Vector3 relative to the center of mass of ActorA.
            virtual Vector3 GetPivotALocation() const;
            /// @brief Get offset of the second actor.
            /// @return The offset as a Vector3 relative to the center of mass of ActorB.
            virtual Vector3 GetPivotBLocation() const;
            /// @brief Set the current impulse clamping on the constraint
            /// @param Clamping This is a value that the constraint solver can use to adjust accumlated values when solving the constraint.

            ////////////////////////////////////////////////////////////////////////////////
            // Point2PointConstraint Specific Physics Settings
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


#ifdef PHYSXML
            ///////////////////////////////////////////////////////////////////////////////
            // Serialization
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
    /// @details This class is incomplete
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
            /// @copydoc TypedConstraint::GetConstraintBase() const
            virtual btTypedConstraint* GetConstraintBase() const;
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

    /// @brief Convert a constraint to XML and send it down a stream.
    /// @param stream The stream to send it down.
    /// @param x The constraint to send down.
    /// @return This returns the output stream to allow operator chaining.
    std::ostream& operator << (std::ostream& stream, const phys::TypedConstraint& x);
    /// @brief Get a constraint from an XML stream.
    /// @param stream The stream to get it out of.
    /// @param x The it you will get out of the stream.
    /// @return This returns the input stream to allow operator chaining.
    std::istream& operator >> (std::istream& stream, phys::TypedConstraint& x);
    /// @brief Converts an XML Node into a functional in memory construct.
    /// @param OneNode The xml node that contains the deserialize class instance.
    /// @param x The class instance to overwrite witht the proto serialized version in the node.
    void operator >> (const phys::xml::Node& OneNode, phys::TypedConstraint& x);

    /// @copydoc operator << (std::ostream& stream, const phys::TypedConstraint& x)
    std::ostream& operator << (std::ostream& stream, const phys::DualTransformConstraint& x);
    /// @copydoc operator >> (std::istream& stream, phys::TypedConstraint& x)
    std::istream& operator >> (std::istream& stream, phys::DualTransformConstraint& x);
    /// @copydoc operator >> (const phys::xml::Node& OneNode, phys::TypedConstraint& x)
    void operator >> (const phys::xml::Node& OneNode, phys::DualTransformConstraint& x);

    /// @copydoc operator << (std::ostream& stream, const phys::TypedConstraint& x)
    std::ostream& operator << (std::ostream& stream, const phys::Generic6DofConstraint& x);
    /// @copydoc operator >> (std::istream& stream, phys::TypedConstraint& x)
    std::istream& operator >> (std::istream& stream, phys::Generic6DofConstraint& x);
    /// @copydoc operator >> (const phys::xml::Node& OneNode, phys::TypedConstraint& x)
    void operator >> (const phys::xml::Node& OneNode, phys::Generic6DofConstraint& x);

    /// @copydoc operator << (std::ostream& stream, const phys::TypedConstraint& x)
    std::ostream& operator << (std::ostream& stream, const phys::Generic6DofSpringConstraint& x);
    /// @copydoc operator >> (std::istream& stream, phys::TypedConstraint& x)
    std::istream& operator >> (std::istream& stream, phys::Generic6DofSpringConstraint& x);
    /// @copydoc operator >> (const phys::xml::Node& OneNode, phys::TypedConstraint& x)
    void operator >> (const phys::xml::Node& OneNode, phys::Generic6DofSpringConstraint& x);

    /// @copydoc operator << (std::ostream& stream, const phys::TypedConstraint& x)
    std::ostream& operator << (std::ostream& stream, const phys::HingeConstraint& x);
    /// @copydoc operator >> (std::istream& stream, phys::TypedConstraint& x)
    std::istream& operator >> (std::istream& stream, phys::HingeConstraint& x);
    /// @copydoc operator >> (const phys::xml::Node& OneNode, phys::TypedConstraint& x)
    void operator >> (const phys::xml::Node& OneNode, phys::HingeConstraint& x);

    /// @copydoc operator << (std::ostream& stream, const phys::TypedConstraint& x)
    std::ostream& operator << (std::ostream& stream, const phys::Point2PointConstraint& x);
    /// @copydoc operator >> (std::istream& stream, phys::TypedConstraint& x)
    std::istream& operator >> (std::istream& stream, phys::Point2PointConstraint& x);
    /// @copydoc operator >> (const phys::xml::Node& OneNode, phys::TypedConstraint& x)
    void operator >> (const phys::xml::Node& OneNode, phys::Point2PointConstraint& x);


#endif // \PHYSXML



#endif

