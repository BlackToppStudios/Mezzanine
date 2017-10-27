// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _physicsfixedconstraint_cpp
#define _physicsfixedconstraint_cpp

#include "Physics/fixedconstraint.h"
#include "Physics/rigidproxy.h"

#include "stringtool.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Physics
    {
        FixedConstraint::FixedConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB, PhysicsManager* Creator) :
            DualTransformConstraint(ID,ProxyA,ProxyB,Creator)
            { this->CreateConstraint(ProxyA,ProxyB,TransA,TransB); }

        FixedConstraint::FixedConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator) :
            DualTransformConstraint(0,nullptr,Creator)
            { this->ProtoDeSerialize(SelfRoot); }

        FixedConstraint::~FixedConstraint()
            { this->DestroyConstraint(); }

        void FixedConstraint::CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB)
        {
            this->Fixed = new btFixedConstraint(*(RigidA->_GetPhysicsObject()),*(RigidB->_GetPhysicsObject()),TransA.GetBulletTransform(),TransB.GetBulletTransform());
        }

        void FixedConstraint::DestroyConstraint()
        {
            if( this->Fixed != nullptr ) {
                this->EnableConstraint(false);
                delete this->Fixed;
                this->Fixed = nullptr;
            }
            this->ProxA = nullptr;
            this->ProxB = nullptr;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Location and Rotation

        void FixedConstraint::SetPivotTransforms(const Transform& TransA, const Transform& TransB)
            { this->Fixed->setFrames(TransA.GetBulletTransform(),TransB.GetBulletTransform()); }

        void FixedConstraint::SetPivotATransform(const Transform& TransA)
            { this->Fixed->setFrames(TransA.GetBulletTransform(),this->Fixed->getFrameOffsetB()); }

        void FixedConstraint::SetPivotBTransform(const Transform& TransB)
            { this->Fixed->setFrames(this->Fixed->getFrameOffsetA(),TransB.GetBulletTransform()); }

        Transform FixedConstraint::GetPivotATransform() const
            { return this->Fixed->getFrameOffsetA(); }

        Transform FixedConstraint::GetPivotBTransform() const
            { return this->Fixed->getFrameOffsetB(); }

        void FixedConstraint::SetPivotALocation(const Vector3& Location)
            { this->Fixed->getFrameOffsetA().setOrigin(Location.GetBulletVector3()); }

        void FixedConstraint::SetPivotBLocation(const Vector3& Location)
            { this->Fixed->getFrameOffsetB().setOrigin(Location.GetBulletVector3()); }

        Vector3 FixedConstraint::GetPivotALocation() const
            { return Vector3(this->Fixed->getFrameOffsetA().getOrigin()); }

        Vector3 FixedConstraint::GetPivotBLocation() const
            { return Vector3(this->Fixed->getFrameOffsetB().getOrigin()); }

        void FixedConstraint::SetPivotARotation(const Quaternion& Rotation)
            { this->Fixed->getFrameOffsetA().setRotation(Rotation.GetBulletQuaternion()); }

        void FixedConstraint::SetPivotBRotation(const Quaternion& Rotation)
            { this->Fixed->getFrameOffsetA().setRotation(Rotation.GetBulletQuaternion()); }

        Quaternion FixedConstraint::GetPivotARotation() const
            { return Quaternion(this->Fixed->getFrameOffsetA().getRotation()); }

        Quaternion FixedConstraint::GetPivotBRotation() const
            { return Quaternion(this->Fixed->getFrameOffsetB().getRotation()); }

        ////////////////////////////////////////////////////////////////////////////////
        // Axis Params

        Constraint::ParamList FixedConstraint::GetValidParamsOnAxis(int Axis) const
        {
            Constraint::ParamList Results;
            if( 0 <= Axis && 5 >= Axis ) {
                Results.push_back(Con_Stop_ERP);
                Results.push_back(Con_CFM);
                Results.push_back(Con_Stop_CFM);
            }
            return Results;
        }

        Constraint::AxisList FixedConstraint::GetValidLinearAxes() const
        {
            Constraint::AxisList Results;
            Results.push_back(0);
            Results.push_back(1);
            Results.push_back(2);
            return Results;
        }

        Constraint::AxisList FixedConstraint::GetValidAngularAxes() const
        {
            Constraint::AxisList Results;
            Results.push_back(3);
            Results.push_back(4);
            Results.push_back(5);
            return Results;
        }

        Boole FixedConstraint::HasParamBeenSet(ConstraintParam Param, int Axis) const
        {
            // http://bulletphysics.com/Bullet/BulletFull/btGeneric6DofConstraint_8cpp_source.html#l00964
            if( 0 > Axis || 5 < Axis )
                { return false; }
            return ( Con_Stop_ERP==Param && this->Fixed->getFlags() & (BT_6DOF_FLAGS_ERP_STOP << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) ) || //if we are checking the stop_erp AND the stop_erp bit is set for the correct axis
                   ( Con_Stop_CFM==Param && this->Fixed->getFlags() & (BT_6DOF_FLAGS_CFM_STOP << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) ) || //if we are checking the stop_cfm AND the stop_cfm bit is set
                   ( Con_CFM==Param      && this->Fixed->getFlags() & (BT_6DOF_FLAGS_CFM_NORM << (Axis * BT_6DOF_FLAGS_AXIS_SHIFT)) );   //if we are checking the cfm AND the cfm bit is set
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        String FixedConstraint::GetDerivedSerializableName() const
            { return FixedConstraint::GetSerializableName(); }

        String FixedConstraint::GetSerializableName()
            { return "FixedConstraint"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal

        btTypedConstraint* FixedConstraint::_GetConstraintBase() const
            { return this->Fixed; }
    }//Physics
}//Mezzanine

#endif
