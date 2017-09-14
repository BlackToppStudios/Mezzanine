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
#ifndef _physicshinge2constraint_cpp
#define _physicshinge2constraint_cpp

#include "Physics/hinge2constraint.h"
#include "Physics/rigidproxy.h"

#include "stringtool.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Physics
    {
        ////////////////////////////////////////////////////////////////////////////////
        // Hinge2 Constraint Functions

        Hinge2Constraint::Hinge2Constraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& Anchor, const Vector3& Axis1, const Vector3& Axis2, PhysicsManager* Creator) :
            SixDofSpringConstraint(ID,ProxyA,ProxyB,Creator)
            { this->CreateConstraint(ProxyA,ProxyB,Anchor,Axis1,Axis2); }

        Hinge2Constraint::Hinge2Constraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB, PhysicsManager* Creator) :
            SixDofSpringConstraint(ID,ProxyA,ProxyB,Creator)
            { this->CreateConstraint(ProxyA,ProxyB,TransA,TransB); }

        Hinge2Constraint::Hinge2Constraint(const XML::Node& SelfRoot, PhysicsManager* Creator) :
            SixDofSpringConstraint(0,nullptr,Creator)
            { this->ProtoDeSerialize(SelfRoot); }

        Hinge2Constraint::~Hinge2Constraint()
            { /* Generic6Dof class will handle the cleanup */ }

        btHinge2Constraint* Hinge2Constraint::Hinge2() const
            { return static_cast<btHinge2Constraint*>( this->Spring6Dof ); }

        void Hinge2Constraint::CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB)
        {
            if( this->Spring6Dof == nullptr ) {
                btVector3 TempOrigin(0,0,0);
                btVector3 TempAxis1(0,1,0);
                btVector3 TempAxis2(1,0,0);
                this->Spring6Dof = new btHinge2Constraint(*(RigidA->_GetPhysicsObject()),*(RigidB->_GetPhysicsObject()),TempOrigin,TempAxis1,TempAxis2);
                this->Spring6Dof->setFrames(TransA.GetBulletTransform(),TransB.GetBulletTransform());
            }
        }

        void Hinge2Constraint::CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Vector3& Anchor, const Vector3& Axis1, const Vector3& Axis2)
        {
            if( this->Spring6Dof == nullptr ) {
                btVector3 TempOrigin = Anchor.GetBulletVector3();
                btVector3 TempAxis1 = Axis1.GetBulletVector3();
                btVector3 TempAxis2 = Axis2.GetBulletVector3();
                this->Spring6Dof = new btHinge2Constraint(*(RigidA->_GetPhysicsObject()),*(RigidB->_GetPhysicsObject()),TempOrigin,TempAxis1,TempAxis2);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Limits

        void Hinge2Constraint::SetUpperLimit(const Real Ang1Max)
            { this->Hinge2()->setUpperLimit(Ang1Max); }

        void Hinge2Constraint::SetLowerLimit(const Real Ang1Min)
            { this->Hinge2()->setLowerLimit(Ang1Min); }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        String Hinge2Constraint::GetDerivedSerializableName() const
            { return Hinge2Constraint::GetSerializableName(); }

        String Hinge2Constraint::GetSerializableName()
            { return "Hinge2Constraint"; }
    }//Physics
}//Mezzanine

#endif
