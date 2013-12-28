// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _physicsgearconstraint_cpp
#define _physicsgearconstraint_cpp

#include "Physics/gearconstraint.h"
#include "Physics/rigidproxy.h"

#include "stringtool.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Physics
    {
        GearConstraint::GearConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& AxisA, const Vector3& AxisB, const Real Ratio)
        {
            this->SetBodies(ProxyA,ProxyB);

            this->Gear = new btGearConstraint(*(ProxA->_GetPhysicsObject()), *(ProxB->_GetPhysicsObject()), AxisA.GetBulletVector3(), AxisB.GetBulletVector3(), Ratio);
        }

        GearConstraint::~GearConstraint()
            { delete this->Gear; }

        ///////////////////////////////////////////////////////////////////////////////
        // Axis configuration

        void GearConstraint::SetAxisA(const Vector3& Axis)
            { this->Gear->setAxisInA(Axis.GetBulletVector3()); }

        void GearConstraint::SetAxisB(const Vector3& Axis)
            { this->Gear->setAxisInB(Axis.GetBulletVector3()); }

        Vector3 GearConstraint::GetAxisA() const
            { return Vector3( this->Gear->getAxisInA() ); }

        Vector3 GearConstraint::GetAxisB() const
            { return Vector3( this->Gear->getAxisInB() ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Ratio Configuration

        void GearConstraint::SetRotationRatio(const Real Ratio)
            { this->Gear->setRatio(Ratio); }

        Real GearConstraint::GetRotationRatio() const
            { return this->Gear->getRatio(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Parameter Configuration

        Constraint::ParamList GearConstraint::ValidParamOnAxis(int Axis) const
        {
            Constraint::ParamList Results;
            return Results;
        }

        Constraint::AxisList GearConstraint::ValidLinearAxis() const
        {
            Constraint::AxisList Results;
            return Results;
        }

        Constraint::AxisList GearConstraint::ValidAngularAxis() const
        {
            Constraint::AxisList Results;
            return Results;
        }

        bool GearConstraint::HasParamBeenSet(ConstraintParam Param, int Axis) const
        {
            return false;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void GearConstraint::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node P2PNode = CurrentRoot.AppendChild(this->GearConstraint::SerializableName());                     // The base node all the base constraint stuff will go in
            if (!P2PNode)
                { SerializeError("Create P2PNode", SerializableName()); }

            XML::Attribute VerAttr = P2PNode.AppendAttribute("Version");
            XML::Attribute RatioAttr = P2PNode.AppendAttribute("Ratio");

            if( VerAttr && RatioAttr )
            {
                VerAttr.SetValue(1);
                RatioAttr.SetValue(this->GetRotationRatio());
            }else{
                SerializeError("Create P2PNode Attributes", SerializableName());
            }

            XML::Node ActorANode = P2PNode.AppendChild("ActorA");
            if (!ActorANode)
                { SerializeError("Create ActorANode", SerializableName()); }
            this->GetAxisA().ProtoSerialize(ActorANode);

            XML::Node ActorBNode = P2PNode.AppendChild("ActorB");
            if (!ActorBNode)
                { SerializeError("Create ActorBNode", SerializableName()); }
            this->GetAxisB().ProtoSerialize(ActorBNode);

            this->Constraint::ProtoSerialize(P2PNode);
        }

        void GearConstraint::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->GearConstraint::SerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    this->Constraint::ProtoDeSerialize(OneNode.GetChild("Constraint"));

                    this->SetRotationRatio(OneNode.GetAttribute("Ratio").AsReal());

                    XML::Node ActorANode = OneNode.GetChild("ActorA");
                    if(!ActorANode)
                        { DeSerializeError("Could not find ActorA axis",SerializableName()); }

                    XML::Node ActorBNode = OneNode.GetChild("ActorB");
                    if(!ActorBNode)
                        { DeSerializeError("Could not find ActorB axis",SerializableName()); }

                    Vector3 temp;
                    temp.ProtoDeSerialize(ActorANode.GetFirstChild());
                    this->SetAxisA(temp);
                    temp.ProtoDeSerialize(ActorBNode.GetFirstChild());
                    this->SetAxisB(temp);
                }else{
                    DeSerializeError("find usable serialization version",SerializableName());
                }
            }else{
                DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
            }
        }

        String GearConstraint::SerializableName()
            { return "GearConstraint"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        btTypedConstraint* GearConstraint::GetConstraintBase() const
            { return this->Gear; }
    }//Physics
}//Mezzanine

#endif
