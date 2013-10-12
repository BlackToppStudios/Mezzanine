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
#ifndef _physicsgeneric6dofspringconstraint_h
#define _physicsgeneric6dofspringconstraint_h

#include "Physics/generic6dofconstraint.h"

class btGeneric6DofSpringConstraint;

namespace Mezzanine
{
    namespace Physics
    {
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
        class MEZZ_LIB Generic6DofSpringConstraint : public Generic6DofConstraint
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
                AngularZ        = 5     ///< Rotation on the Z axis
            };

            ////////////////////////////////////////////////////////////////////////////////
            // Generic6DofSpringConstraint Construction and Destruction

            /// @brief Two proxy Verbose constructor.
            /// @param ProxyA The First proxy to be bound.
            /// @param ProxyB  The Second proxy to be bound.
            /// @param VectorA The offset from ProxyAs center of gravity to get to match an offset from ProxyB.
            /// @param VectorB The offset from ProxyBs center of gravity.
            /// @param QuaternionA Relative rotation from ProxyA.
            /// @param QuaternionB Relative rotation from ProxyB.
            /// @param UseLinearReferenceA Perform Linear math from ProxyA's perspective, default to false.
            Generic6DofSpringConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& VectorA, const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseLinearReferenceA = false);
            /// @brief Two proxy Terse constructor.
            /// @param ProxyA The First proxy to be bound.
            /// @param ProxyB  The Second proxy to be bound.
            /// @param TransformA The offset and rotation from ProxyAs center of gravity to get to match an offset from ProxyB.
            /// @param TransformB The offset and rotation from ProxyBs center of gravity.
            /// @param UseLinearReferenceA Perform Linear math from ProxyA's perspective, default to false.
            Generic6DofSpringConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransformA, const Transform& TransformB, bool UseLinearReferenceA = false);
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

            /// @brief Convert this class to an XML::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this vectorw should be appended to.
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and XML::Node containing the data.
            /// @warning A precondition of using this is that all of the actors intended for use must already be Deserialized.
            virtual void ProtoDeSerialize(const XML::Node& OneNode);
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "Generic6DofConstraint"
            static String SerializableName();
        };//Generic6DofSpringConstraint
    }//Physics
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

/// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::Constraint& x)
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::Generic6DofSpringConstraint& x);
/// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::Constraint& x)
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::Generic6DofSpringConstraint& x);
/// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Constraint& x)
void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Generic6DofSpringConstraint& x);

#endif
