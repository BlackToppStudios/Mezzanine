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
        /// is expected that more derived classes will implement the Set/Get Pivot/Rotation Function in a more efficient
        /// Manner if a more efficient way exists. The non-transform get/set function are implmented in terms of the
        /// get/set transform function extra copies of values and extra reading/writing may occur if the compiler weakly
        /// optimizes. Of course, implementing more functions could slow down the software if it no longer fits in CPU
        /// caches. As always benchmark if this is something that may be critically slowing you down.
        ///////////////////////////////////////////////////////////////////////////////
        class MEZZ_LIB DualTransformConstraint : public Constraint
        {
        protected:
            /// @internal
            /// @brief Creates the internal constraint.
            /// @remarks This methods exists primarily as a convenience for serialization, and shouldn't be called unless it is known to be safe.
            /// @param RigidA A pointer to the first Proxy to be constrained.
            /// @param RigidB A pointer to the second Proxy to be constrained.
            /// @param TransA The offset to place the constraint in the first proxys local space.
            /// @param TransB The offset to place the constraint in the second proxys local space.
            virtual void CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB) = 0;
            /// @internal
            /// @brief Destroys the internal constraint.
            /// @remarks This methods exists primarily as a convenience for serialization, and shouldn't be called unless it is known to be safe.
            virtual void DestroyConstraint() = 0;

            /// @internal
            /// @brief Single body inheritance constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param Prox1 A pointer to the first/only proxy that will be constrained.
            /// @param Creator A pointer to the manager that created this constraint.
            DualTransformConstraint(const UInt32 ID, RigidProxy* Prox1, PhysicsManager* Creator);
            /// @internal
            /// @brief Dual body inheritance constructor.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param Prox1 A pointer to the first proxy that will be constrained.
            /// @param Prox2 A pointer to the second proxy that will be constrained.
            /// @param Creator A pointer to the manager that created this constraint.
            DualTransformConstraint(const UInt32 ID, RigidProxy* Prox1, RigidProxy* Prox2, PhysicsManager* Creator);
        public:
            /// @brief Class destructor.
            virtual ~DualTransformConstraint();

            ///////////////////////////////////////////////////////////////////////////////
            // Transform Methods

            /// @brief Sets the Position and Rotation for the first and second body using a Transform.
            /// @param TransA The new position and rotation for ProxyA.
            /// @param TransB The new position and rotation for ProxyB.
            virtual void SetPivotTransforms(const Transform& TransA, const Transform& TransB);
            /// @brief Sets the Position and Rotation for the first body using a Transform.
            /// @param TransA The new position and rotation.
            virtual void SetPivotATransform(const Transform& TransA) = 0;
            /// @brief Sets the Position and Rotation for the second body using a Transform.
            /// @param TransB The new position and rotation.
            virtual void SetPivotBTransform(const Transform& TransB) = 0;
            /// @brief Gets the current Rotation and Location of ProxyA.
            /// @return This returns a Mezzanine::Transform.
            virtual Transform GetPivotATransform() const = 0;
            /// @brief Gets the current Rotation and Location of ProxyB.
            /// @return This returns a Mezzanine::Transform.
            virtual Transform GetPivotBTransform() const = 0;

            /// @brief Sets The relative location of the  pivot from ProxA's Center of gravity.
            /// @param Location The New value for PivotA.
            virtual void SetPivotALocation(const Vector3& Location);
            /// @brief Sets The relative location of the  pivot from ProxB's Center of gravity.
            /// @param Location The New value for PivotB.
            virtual void SetPivotBLocation(const Vector3& Location);
            /// @brief Gets the location of the pivot relative to ProxA's Center of gravity.
            /// @return A Vector3 with the pivot location.
            virtual Vector3 GetPivotALocation() const;
            /// @brief Gets the location of the pivot relative to ProxB's Center of gravity.
            /// @return A Vector3 with the pivot location.
            virtual Vector3 GetPivotBLocation() const;

            /// @brief Sets The relative rotation of ProxA.
            /// @param Rotation The new rotation amount for ProxA.
            virtual void SetPivotARotation(const Quaternion& Rotation);
            /// @brief Sets The relative rotation of ProxB.
            /// @param otation The new rotation amount for ProxB.
            virtual void SetPivotBRotation(const Quaternion& Rotation);
            /// @brief Gets the relative rotation for ProxA.
            /// @return A Quaternion that has the rotation.
            virtual Quaternion GetPivotARotation() const;
            /// @brief Gets the relative rotation for ProxB.
            /// @return A Quaternion that has the rotation.
            virtual Quaternion GetPivotBRotation() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc Constraint::ProtoSerializeInitData(XML::Node&) const
            virtual void ProtoSerializeInitData(XML::Node& SelfRoot) const;
            /// @copydoc Constraint::ProtoDeSerializeInitData(const XML::Node&)
            virtual void ProtoDeSerializeInitData(const XML::Node& SelfRoot);

            /// @brief Get the name of the the XML tag the class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();
        };//DualTransformConstraint
    }//Physics
}//Mezzanine

#endif
