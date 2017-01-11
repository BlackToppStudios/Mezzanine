// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _physicsgearconstraint_h
#define _physicsgearconstraint_h

#include "Physics/constraint.h"

class btGearConstraint;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class GearConstraint
        /// @brief This is a constraint that duplicate the angular motion of one object to another, adjusted by the provided ratio.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB GearConstraint : public Constraint
        {
        protected:
            /// @internal
            /// @brief Bullet constraint that this class encapsulates.
            btGearConstraint* Gear;

            /// @internal
            /// @brief Creates the internal constraint.
            /// @remarks This methods exists primarily as a convenience for serialization, and shouldn't be called unless it is known to be safe.
            /// @param RigidA A pointer to the first Proxy to be constrained.
            /// @param RigidB A pointer to the second Proxy to be constrained.
            /// @param AxisA The offset to place the constraint in the first proxys local space.
            /// @param AxisB The offset to place the constraint in the second proxys local space.
            virtual void CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Vector3& AxisA, const Vector3& AxisB);
            /// @internal
            /// @brief Destroys the internal constraint.
            /// @remarks This methods exists primarily as a convenience for serialization, and shouldn't be called unless it is known to be safe.
            virtual void DestroyConstraint();
        public:
            /// @brief Double body constructor.  Binds the two bodies.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA The first proxy to apply this constraint to.
            /// @param ProxyB The second proxy to apply this constraint to.
            /// @param PivotA The axis in ProxyA's local space to apply the constraint to.
            /// @param PivotB The axis in ProxyB's local space to apply the constraint to.
            /// @param Creator A pointer to the manager that created this constraint.
            GearConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& AxisA, const Vector3& AxisB, PhysicsManager* Creator);
            /// @brief Double body constructor.  Binds the two bodies.
            /// @param ID The unique identifier assigned to this constraint.
            /// @param ProxyA The first proxy to apply this constraint to.
            /// @param ProxyB The second proxy to apply this constraint to.
            /// @param PivotA The axis in ProxyA's local space to apply the constraint to.
            /// @param PivotB The axis in ProxyB's local space to apply the constraint to.
            /// @param Ratio The amount the rotation from ProxyA that shall be used to be applied to ProxyB.
            /// @param Creator A pointer to the manager that created this constraint.
            GearConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& AxisA, const Vector3& AxisB, const Real Ratio, PhysicsManager* Creator);
            /// @brief XML constructor.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @param Creator A pointer to the manager that created this constraint.
            GearConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator);
            /// @brief Class destructor.
            virtual ~GearConstraint();

            ///////////////////////////////////////////////////////////////////////////////
            // Axis configuration

            /// @brief Sets the axis in ActorA's local space which will translate to ActorB.
            /// @param Axis A vector3 expressing the axis on ActorA this constraint will be applied to.
            virtual void SetAxisA(const Vector3& Axis);
            /// @brief Sets the axis in ActorB's local space which will be manipulated.
            /// @param Axis A vector3 expressing the axis on ActorB this constraint will be applied to.
            virtual void SetAxisB(const Vector3& Axis);
            /// @brief Gets the axis in ActorA's local space which will translate to ActorB.
            /// @return Returns a vector3 expressing the axis on ActorA this constraint is being applied to.
            virtual Vector3 GetAxisA() const;
            /// @brief Gets the axis in ActorB's local space which will be manipulated.
            /// @return Returns a vector3 expressing the axis on ActorB this constraint is being applied to.
            virtual Vector3 GetAxisB() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Ratio Configuration

            /// @brief Sets the ratio at which ActorA's rotation will be applied to ActorB.
            /// @param Ratio The ratio at which rotations on ActorA's specified axis will be transfered to ActorB's specified axis.
            virtual void SetRotationRatio(const Real Ratio);
            /// @brief Gets the ratio at which ActorA's rotation will be applied to ActorB.
            /// @return Returns a Real representing the ratio at which rotations on ActorA's specified axis is being transfered to ActorB's specified axis.
            virtual Real GetRotationRatio() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Parameter Configuration

            /// @copydoc Constraint::GetValidParamsOnAxis(int) const
            virtual Constraint::ParamList GetValidParamsOnAxis(int Axis) const;
            /// @copydoc Constraint::GetValidLinearAxes() const
            virtual Constraint::AxisList GetValidLinearAxes() const;
            /// @copydoc Constraint::GetValidAngularAxes() const
            virtual Constraint::AxisList GetValidAngularAxes() const;
            /// @copydoc Constraint::ValidAngularAxis(ConstraintParam,int) const
            virtual Boole HasParamBeenSet(ConstraintParam Param, int Axis) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc Constraint::ProtoSerializeInitData(XML::Node&) const
            virtual void ProtoSerializeInitData(XML::Node& SelfRoot) const;
            /// @copydoc Constraint::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;

            /// @copydoc Constraint::ProtoDeSerializeInitData(const XML::Node&)
            virtual void ProtoDeSerializeInitData(const XML::Node& SelfRoot);
            /// @copydoc Constraint::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc Constraint::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @brief Get the name of the the XML tag the class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @copydoc Constraint::_GetConstraintBase() const
            virtual btTypedConstraint* _GetConstraintBase() const;
        };//GearConstraint
    }//Physics
}//Mezzanine

#endif
