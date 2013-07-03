//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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

namespace Mezzanine
{
    class ActorRigid;
}

class btRigidBody;
class btTypedConstraint;

namespace Mezzanine
{
    namespace Physics
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

        /// @internal
        /// @brief used to help convert string to the axis they indicate
        /// @param it it is the character that is passed in to indicate what the axis is. This should be the [4] character or the 5 character of the string.
        /// @return this returns an int that indicates the Axis for the string.
        int char4ToAxis(char it);

        ///////////////////////////////////////////////////////////////////////////////
        /// @class Constraint
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
        class MEZZ_LIB Constraint
        {
        protected:
            friend class PhysicsManager;
            ////////////////////////////////////////////////////////////////////////////////
            // Constraint Data Members
            /// @brief First rigid body the constraint applies to.
            btRigidBody* BodyA;
            /// @brief Second rigid body the constraint applies to(if applicable).
            btRigidBody* BodyB;
            /// @brief First Actor the constraint applies to.
            ActorRigid* ActA;
            /// @brief Second Actor the constraint applies to.
            ActorRigid* ActB;

            ////////////////////////////////////////////////////////////////////////////////
            // Constraint Protected Methods
            /// @brief Sets the Internal actor pointers.
            void SetBodies(ActorRigid* Act1, ActorRigid* Act2);
            /// @brief Sets the Internal actor pointers.
            void SetBodies(ActorRigid* Act1);
            /// @brief No initialization constructor.
            /// @details This is protected to prevent creating an instance of this directly.
            Constraint();
        public:
            ////////////////////////////////////////////////////////////////////////////////
            // Constraint Core Functionality
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~Constraint();

            /// @brief Gets the first actor this constraint applies to.
            /// @return Returns a pointer to the first actor this constraint applies to.
            virtual ActorRigid* GetActorA() const;
            /// @brief Gets the second actor this constraint applies to.
            /// @return Returns a pointer to the second actor this constraint applies to.
            virtual ActorRigid* GetActorB() const;

            ////////////////////////////////////////////////////////////////////////////////
            // Constraint Position and Orientation
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
            // Constraint Parameters
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
                if(size_t(ConstraintParamCount) == Supported.size())                           // no need to check deeper, because everything is supported
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

            /// @brief Get the Bullet constraint that this class encapsulates.
            /// @return A pointer to the btTypedConstraint that stores the underlying constraint.
            virtual btTypedConstraint* GetConstraintBase() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Constraint Serialization

            /// @brief Convert this class to an XML::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this vectorw should be appended to.
            /// @details This stores each Actor's name and every constraint parameter on each if it has been set.
            /// This should allow for looking up the required actors in the Actor Manager. This should also prevent
            /// any interference with different values and minimize the size of the serialized version
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and XML::Node containing the data.
            /// @details This should only be run once all the actors have finishes being deserialized. This will fail
            /// if the actors it expects are not present.
            virtual void ProtoDeSerialize(const XML::Node& OneNode);
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "Constraint"
            static String SerializableName();
        };//Constraint
    }//Physics
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

/// @brief Convert a constraint to XML and send it down a stream.
/// @param stream The stream to send it down.
/// @param x The constraint to send down.
/// @return This returns the output stream to allow operator chaining.
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::Constraint& x);
/// @brief Get a constraint from an XML stream.
/// @param stream The stream to get it out of.
/// @param x The it you will get out of the stream.
/// @return This returns the input stream to allow operator chaining.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::Constraint& x);
/// @brief Converts an XML Node into a functional in memory construct.
/// @param OneNode The xml node that contains the deserialize class instance.
/// @param x The class instance to overwrite witht the proto serialized version in the node.
void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Constraint& x);

#endif
