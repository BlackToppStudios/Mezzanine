//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _physicsdualtransformconstraint_h
#define _physicsdualtransformconstraint_h

#include "Physics/constraint.h"

namespace Mezzanine
{
    namespace Physics
    {
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
        class MEZZ_LIB DualTransformConstraint : public Constraint
        {
            public:
            /// @brief Set the Position and Rotation using a Transform
            /// @param TranA The new Position and rotation
            virtual void SetPivotATransform(const Transform& TranA) = 0;
            /// @brief Set the Position and Rotation using a Transform
            /// @param TranB The new Position and rotation
            virtual void SetPivotBTransform(const Transform& TranB) = 0;
            /// @brief Get the current Rotation and Location of Actor A
            /// @return This returns a Mezzanine::Transform
            virtual Transform GetPivotATransform() const = 0;
            /// @brief Get the current Rotation and Location of Actor B
            /// @return This returns a Mezzanine::Transform
            virtual Transform GetPivotBTransform() const = 0;

            /// @brief Set The relative location of the  pivot from ActorA's Center of gravity.
            /// @param Location The New value for PivotA
            /// @details Ultimately this information winds up being stored in the TransformA.
            virtual void SetPivotALocation(const Vector3& Location)
                { SetPivotATransform( Transform(Location, GetPivotARotation()) ); }
            /// @brief Set The relative location of the  pivot from ActorB's Center of gravity.
            /// @param Location The New value for PivotB
            /// @details Ultimately this information winds up being stored in the TransformB
            virtual void SetPivotBLocation(const Vector3& Location)
                { SetPivotBTransform( Transform(Location, GetPivotBRotation()) ); }
            /// @brief Get the location of the pivot relative to ActorA's Center of gravity
            /// @return A Vector3 with the pivot location.
            virtual Vector3 GetPivotALocation() const
                { return GetPivotATransform().Location; }
            /// @brief Get the location of the pivot relative to ActorB's Center of gravity
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
#ifdef MEZZXML
            // Serializable
            /// @brief Convert this class to an XML::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this vectorw should be appended to.
            /// @details This stores each Actor's Transform
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;

            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and XML::Node containing the data.
            virtual void ProtoDeSerialize(const XML::Node& OneNode);

            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "DualTransformConstraint"
            static String SerializableName();
#endif
        };//DualTransformConstraint
    }//Physics
}//Mezzanine

#ifdef MEZZXML
    ///////////////////////////////////////////////////////////////////////////////
    // Class External << Operators for streaming or assignment

    /// @copydoc operator << (std::ostream& stream, const Mezzanine::Physics::Constraint& x)
    std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Physics::DualTransformConstraint& x);
    /// @copydoc operator >> (std::istream& stream, Mezzanine::Physics::Constraint& x)
    std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::DualTransformConstraint& x);
    /// @copydoc operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Constraint& x)
    void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::DualTransformConstraint& x);
#endif // \MEZZXML

#endif
