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
#ifndef _physicspoint2pointconstraint_cpp
#define _physicspoint2pointconstraint_cpp

#include "point2pointconstraint.h"
#include "stringtool.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Physics
    {
        ////////////////////////////////////////////////////////////////////////////////
        // Point2PointConstraint Position and Orientation
        Point2PointConstraint::Point2PointConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& PivotA, const Vector3& PivotB)
        {
            SetBodies(ActorA,ActorB);

            Point2Point = new btPoint2PointConstraint(*BodyA, *BodyB, PivotA.GetBulletVector3(), PivotB.GetBulletVector3());
        }

        Point2PointConstraint::Point2PointConstraint(ActorRigid* ActorA, const Vector3& PivotA)
        {
            SetBodies(ActorA);

            Point2Point = new btPoint2PointConstraint(*BodyA, PivotA.GetBulletVector3());
        }

        Point2PointConstraint::~Point2PointConstraint()
            { delete Point2Point; }

        btTypedConstraint* Point2PointConstraint::GetConstraintBase() const
            { return this->Point2Point; }
        ////////////////////////////////////////////////////////////////////////////////
        // Point2PointConstraint Position and Orientation
        void Point2PointConstraint::SetPivotALocation(const Vector3& PivotA)
            { this->Point2Point->setPivotA(PivotA.GetBulletVector3()); }

        void Point2PointConstraint::SetPivotBLocation(const Vector3& PivotB)
            { this->Point2Point->setPivotB(PivotB.GetBulletVector3()); }

        Vector3 Point2PointConstraint::GetPivotALocation() const
            { return Vector3(this->Point2Point->getPivotInA()); }

        Vector3 Point2PointConstraint::GetPivotBLocation() const
            { return Vector3(this->Point2Point->getPivotInB()); }

        ////////////////////////////////////////////////////////////////////////////////
        // Point2PointConstraint Specific Physics Settings
        void Point2PointConstraint::SetImpulseClamping(Real Clamping)
            { this->Point2Point->m_setting.m_impulseClamp = Clamping; }

        Real Point2PointConstraint::GetImpulseClamping() const
            { return this->Point2Point->m_setting.m_impulseClamp; }

        void Point2PointConstraint::SetDamping(Real Damping)
            { this->Point2Point->m_setting.m_damping = Damping; }

        Real Point2PointConstraint::GetDamping() const
            { return this->Point2Point->m_setting.m_damping; }

        void Point2PointConstraint::SetTAU(Real TAU)
            { this->Point2Point->m_setting.m_tau = TAU; }

        Real Point2PointConstraint::GetTAU() const
            { return this->Point2Point->m_setting.m_tau; }

        Constraint::ParamList Point2PointConstraint::ValidParamOnAxis(int Axis) const
        {
            Constraint::ParamList Results;
            if(-1==Axis)
            {
                Results.push_back(Con_ERP);
                Results.push_back(Con_Stop_ERP);
                Results.push_back(Con_CFM);
                Results.push_back(Con_Stop_CFM);
            }
            return Results;
        }

        Constraint::AxisList Point2PointConstraint::ValidLinearAxis() const
        {
            Constraint::AxisList Results;
            Results.push_back(-1);
            return Results;
        }

        Constraint::AxisList Point2PointConstraint::ValidAngularAxis() const
        {
            Constraint::AxisList Results;
            return Results;
        }

        bool Point2PointConstraint::HasParamBeenSet(ConstraintParam Param, int Axis) const
        {
            //return this->Point2Point->hasParamBeenSet(Param,Axis);
            // the logic here should match the logic in the source at http://bulletphysics.com/Bullet/BulletFull/btPoint2PointConstraint_8cpp_source.html#l00202
            if (-1!=Axis)
                { return false; }
            return  ( (Con_ERP==Param||Con_Stop_ERP==Param) && this->Point2Point->getFlags() & BT_P2P_FLAGS_ERP )     ||   //If we are checking erp OR we are checking stoperp AND the erp Flag is set OR
                    ( (Con_CFM==Param||Con_Stop_CFM==Param) && this->Point2Point->getFlags() & BT_P2P_FLAGS_CFM )      ;   //   we are checking cfm OR we are checking stopcfm AND the cfm Flag is set
        }

        void Point2PointConstraint::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node P2PNode = CurrentRoot.AppendChild(SerializableName());                     // The base node all the base constraint stuff will go in
            if (!P2PNode)
                { SerializeError("Create P2PNode", SerializableName()); }

            XML::Attribute VerAttr = P2PNode.AppendAttribute("Version");
            XML::Attribute TauAttr = P2PNode.AppendAttribute("Tau");
            XML::Attribute ClaAttr = P2PNode.AppendAttribute("ImpulseClamping");
            XML::Attribute DamAttr = P2PNode.AppendAttribute("Damping");

            if( VerAttr && TauAttr && ClaAttr && DamAttr )
            {
                VerAttr.SetValue(1);
                TauAttr.SetValue(this->GetTAU());
                ClaAttr.SetValue(this->GetImpulseClamping());
                DamAttr.SetValue(this->GetDamping());
            }else{
                SerializeError("Create P2PNode Attributes", SerializableName());
            }

            XML::Node ActorANode = P2PNode.AppendChild("ActorA");
            if (!ActorANode)
                { SerializeError("Create ActorANode", SerializableName()); }
            this->GetPivotALocation().ProtoSerialize(ActorANode);

            XML::Node ActorBNode = P2PNode.AppendChild("ActorB");
            if (!ActorBNode)
                { SerializeError("Create ActorBNode", SerializableName()); }
            this->GetPivotBLocation().ProtoSerialize(ActorBNode);

            this->Constraint::ProtoSerialize(P2PNode);
        }

        void Point2PointConstraint::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->Point2PointConstraint::SerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    this->Constraint::ProtoDeSerialize(OneNode.GetChild("Constraint"));

                    this->SetTAU(OneNode.GetAttribute("Tau").AsReal());
                    this->SetImpulseClamping(OneNode.GetAttribute("ImpulseClamping").AsReal());
                    this->SetDamping(OneNode.GetAttribute("Damping").AsReal());

                    XML::Node ActorANode = OneNode.GetChild("ActorA");
                    if(!ActorANode)
                        { DeSerializeError("Could not find ActorA position",SerializableName()); }

                    XML::Node ActorBNode = OneNode.GetChild("ActorB");
                    if(!ActorBNode)
                        { DeSerializeError("Could not find ActorB position",SerializableName()); }

                    Vector3 temp;
                    temp.ProtoDeSerialize(ActorANode.GetFirstChild());
                    SetPivotALocation(temp);
                    temp.ProtoDeSerialize(ActorBNode.GetFirstChild());
                    SetPivotBLocation(temp);

                }else{
                    DeSerializeError("find usable serialization version",SerializableName());
                }
            }else{
                DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
            }
        }

        String Point2PointConstraint::SerializableName()
            { return String("Point2PointConstraint"); }
    }//Physics
}//Mezzanine


///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::Point2PointConstraint& x)
{
    Mezzanine::Serialize(stream,x);
    return stream;
}

std::istream& operator >> (std::istream& stream, Mezzanine::Physics::Point2PointConstraint& x)
    { return Mezzanine::DeSerialize(stream, x); }

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Point2PointConstraint& x)
    { x.ProtoDeSerialize(OneNode); }

#endif
