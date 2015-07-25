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
#ifndef _physicsconstraint_h
#define _physicsconstraint_h

#include "crossplatformexport.h"
#include "datatypes.h"
#include "quaternion.h"
#include "transform.h"
#include "vector3.h"

class btRigidBody;
class btTypedConstraint;

namespace Mezzanine
{
    namespace Physics
    {
        class RigidProxy;
        class PhysicsManager;

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

        /// @internal
        /// @brief used to help convert string to the axis they indicate
        /// @param it it is the character that is passed in to indicate what the axis is. This should be the [4] character or the 5 character of the string.
        /// @return this returns an int that indicates the Axis for the string.
        int char4ToAxis(char it);

        ///////////////////////////////////////////////////////////////////////////////
        /// @class Constraint
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
        ///     - GetValidParamsOnAxis(int Axis) const - What parameters can be changed on the given Axis
        ///     - GetValidLinearAxes() const - Which Axis Suppport Translation
        ///     - GetValidAngularAxes() const - Which axis support Rotation
        ///     - HasParamBeenSet(ConstraintParam Param, int Axis) const - Has a the given param on the given axis been set
        ///
        /// It is advisable to re-implement a few more functions as well, these all have implementation writtens in terms of
        /// of the function that must be implemented, but the genericity of them may impede performance.
        ///     - GetValidAxes() const - Full List of all valid Axis. Combines the lists from GetValidLinearAxes() const and GetValidAngularAxes() const.
        ///     - IsParamValidOnAxis(ConstraintParam, int) const - Uses the other function to check Axis and then check if the param is valid there
        ///     - GetValidParams() const - A list of Parameters every Axis on this constraint implements
        ///
        ///
        ///////////////////////////////////////////////////////////////////////////////
        class MEZZ_LIB Constraint
        {
        public:
            /// @brief Used to Store lists of param for return types
            typedef std::vector<ConstraintParam> ParamList;
            /// @brief Used to Store lists of Int Axis for return types
            /// @details In general Axis will come in groups of three, such as 0,1,2, or 0,1,2,3,4,5 which could represent X,Y, and Z or multiple grousp of X,Y, and Z. These Axis
            /// can represent Linear/Translation or Angular/Rotation information. Some Constraints support values that affect all constraints, this is usually represented a -1.
            typedef std::vector<int> AxisList;
        protected:
            friend class PhysicsManager;

            /// @internal
            /// @brief The first Collidable this constraint applies to.
            RigidProxy* ProxA;
            /// @internal
            /// @brief The second Collidable this constraint applies to.
            RigidProxy* ProxB;
            /// @internal
            /// @brief This is a pointer to the physics manager that created and owns this constraint.
            PhysicsManager* Manager;
            /// @internal
            /// @brief The unique ID used to identify this constraint.
            UInt32 ConstraintID;
            /// @internal
            /// @brief Whether or not collisions will be allowed to occure between the constrained bodies.
            Boole AllowCollisions;
            /// @internal
            /// @brief Whether or not the constraint is currently taking effect.
            Boole Enabled;

            /// @brief Zero body constructor.
            /// @remarks This should only ever be used for XML deserialization.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param Creator A pointer to the manager that created this constraint.
            Constraint(const UInt32 ID, PhysicsManager* Creator);
            /// @brief Single body constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param Prox1 A pointer to the first/only proxy that will be constrained.
            /// @param Creator A pointer to the manager that created this constraint.
            Constraint(const UInt32 ID, RigidProxy* Prox1, PhysicsManager* Creator);
            /// @brief Dual body constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param Prox1 A pointer to the first proxy that will be constrained.
            /// @param Prox2 A pointer to the second proxy that will be constrained.
            /// @param Creator A pointer to the manager that created this constraint.
            Constraint(const UInt32 ID, RigidProxy* Prox1, RigidProxy* Prox2, PhysicsManager* Creator);
        public:
            /// @brief Class destructor.
            virtual ~Constraint();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the unique ID of this constraint.
            /// @return Returns a UInt32 representing the unique ID of this constraint among all other constraints in it's parent manager.
            virtual UInt32 GetConstraintID() const;

            /// @brief Enables or disables this constraint.
            /// @param Enable Whether or not to make this constraint take effect.
            virtual void EnableConstraint(const Boole Enable);
            /// @brief Gets whether or not this constraint is enabled.
            /// @return Returns true if this constraint is currently enabled, false otherwise.
            virtual Boole IsConstraintEnabled() const;

            /// @brief Sets whether collisions between the two constrained bodies should occur.
            /// @note If this is set while the constraint is enabled, it will have to be disabled and then re-enabled to take effect.
            /// @param Allow Whether or not collisions will be allowed to occure between the constrained bodies.
            virtual void SetAllowCollisions(const Boole Allow);
            /// @brief Gets whether or not collisions can/will occur between the two constrained bodies.
            /// @return Returns true if collisions are enabled between both bodies of this constraint, false otherwise.
            virtual Boole GetAllowCollisions() const;

            /// @brief Gets the first Proxy this constraint applies to.
            /// @return Returns a pointer to the first Proxy this constraint applies to.
            virtual RigidProxy* GetProxyA() const;
            /// @brief Gets the second Proxy this constraint applies to.
            /// @return Returns a pointer to the second Proxy this constraint applies to.
            virtual RigidProxy* GetProxyB() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Constraint Parameters

            /// @brief Get a sorted (low to high) list of Parameters that are valid on this Axis
            /// @details Parameters returned from this will work on the given axis even if they are not valid on any other axis. There is no guarantee that the Parameters
            /// will be uniquely stored per an axis. There is no guarantee that changing one parameter will not change another.
            /// @param Axis the Axis to check.
            /// @return A Paramlist with all the valid parameters for this axis.
            virtual ParamList GetValidParamsOnAxis(int Axis) const = 0;
            /// @brief Get a sorted (low to high) list of all axis that operate linearly (that lock sliding/translation)
            /// @warning Odd behaviors, maybe even undefined behavior can happen if This returns a matching Axis to a Linear Axis. Any given axis should only be one or the other
            /// @return An Axislist with the Axis this constraint linearly supports.
            virtual AxisList GetValidLinearAxes() const = 0;
            /// @brief Get A list sorted (low to high) of all axis that operate Angularly (that lock sliding/translation)
            /// @warning Odd behaviors, maybe even undefined behavior can happen if This returns a matching Axis to a Linear Axis. Any given axis should only be one or the other
            /// @return An Axislist with the Axis this constraint Angularly supports.
            virtual AxisList GetValidAngularAxes() const = 0;
            /// @brief Has the given Param on the Given Axis been set yet.
            /// @param Param The parameter to Check
            /// @param Axis The Axis on which to check the param
            /// @details This will probably have to implement the same logic aas what is in the respective get/setparam function of each constraint http://bulletphysics.com/Bullet/BulletFull/classbtTypedConstraint.html#a11f44dd743b59bc05e25d500456e2559
            /// @return True if it has been set, false if it has.
            virtual Boole HasParamBeenSet(ConstraintParam Param, int Axis) const = 0;

            /// @brief Get a sorted (low to high) list of All Valid Axis
            /// @details This is implemented using ValidLinearAxis and ValidAngularAxis, Derived versions of this class may wish to make a more specialized
            /// implementation of this method that doesn't have the overhead of passing around 3 vectors by value.
            /// @return An Axislist with all the Axis this constraint supports.
            virtual AxisList GetValidAxes() const;
            /// @brief Is a certain Parameter valid on a certain axis
            /// @param Param The Parameter to Check
            /// @param Axis The Axis to Check
            /// @details This is implemented using ValidParamOnAxis, Derived versions of this class may wish to make a more specialized
            /// implementation of this method.
            /// @return True if Param is valid on Axis, and Axis is valid. If anything is invalid this returns false.
            virtual Boole IsParamValidOnAxis(ConstraintParam Param, int Axis) const;
            /// @brief Get A sorted (low to high) list of Parameters that are valid on all Axis
            /// @details This is implemented using ValidAxis and ValidParamOnAxis, Derived versions of this class may wish to make a more specialized
            /// implementation of this method that doesn't have the overhead of passing around many vectors by value and executing slow loops. Most
            /// likely all of these constraint Parameter and axis functions could be replaced with some fairly simple switch statements and statically
            /// coded values that are specific to the kind of constraint.
            /// @return A Paramlist with the parameters valid on all axis.
            virtual ParamList GetValidParams() const;

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

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that all this renderable should be appended to.
            virtual void ProtoSerialize(XML::Node& ParentNode) const;
            /// @brief Convert the data needed to initialize this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeInitData(XML::Node& SelfRoot) const = 0;
            /// @brief Convert the properties of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @brief Convert the global overrides of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeGlobalOverrides(XML::Node& SelfRoot) const;

            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerialize(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and initializes a new internal object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeInitData(const XML::Node& SelfRoot) = 0;
            /// @brief Take the data stored in an XML Node and overwrite the properties of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the global overrides of this object with it.
            /// @warning Internally there is no way to unset a global variable.  If you are calling this method without
            /// first called "ProtoDeSerializeInitData" then you will have the previous global overrides in effect as well.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeGlobalOverrides(const XML::Node& SelfRoot);

            /// @brief Gets the most derived serializable name of this Constraint.
            /// @note When creating a new Constraint class verify this method has a valid return for it in order for serialization to work properly.
            /// @return Returns the name of the XML tag from the most derived class of "this".
            virtual String GetDerivedSerializableName() const = 0;
            /// @brief Get the name of the the XML tag the class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @brief Get the Bullet constraint that this class encapsulates.
            /// @return A pointer to the btTypedConstraint that stores the underlying constraint.
            virtual btTypedConstraint* _GetConstraintBase() const = 0;
        };// Constraint
    }//Physics
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

#ifndef SWIG
/// @brief Convert a constraint to XML and send it out to a stream.
/// @param Stream The stream to send it out to.
/// @param Con The constraint to stream.
/// @return This returns the output stream to allow operator chaining.
std::ostream& MEZZ_LIB operator<< (std::ostream& Stream, const Mezzanine::Physics::Constraint* Con);
/// @brief Get a constraint from an XML stream.
/// @param Stream The stream to get it from.
/// @param Con The constraint to stream.
/// @return This returns the input stream to allow operator chaining.
std::istream& MEZZ_LIB operator>> (std::istream& Stream, Mezzanine::Physics::Constraint* Con);
/// @brief Serializes an XML node into a constraint.
/// @param OneNode The xml node that the constraint will be serialized to.
/// @param Con The constraint to serialize.
void MEZZ_LIB operator<< (Mezzanine::XML::Node& OneNode, const Mezzanine::Physics::Constraint* Con);
/// @brief Deserializes an XML node into a constraint.
/// @param OneNode The xml node that contains the class instance to be deserialized.
/// @param Con The constraint to deserialize to.
void MEZZ_LIB operator>> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Constraint* Con);
#endif

#endif
