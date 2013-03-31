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
#ifndef _physicsgeneric6dofspringconstraint_cpp
#define _physicsgeneric6dofspringconstraint_cpp

#include "generic6dofspringconstraint.h"
#include "stringtool.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Physics
    {
        /////////////////////////////////////////
        // Generic6Dof Spring Constraint Functions

        Generic6DofSpringConstraint::Generic6DofSpringConstraint()
        {
        }

        btGeneric6DofSpringConstraint* Generic6DofSpringConstraint::Generic6dofSpring() const
        {
            return static_cast<btGeneric6DofSpringConstraint*>(Generic6dof);
        }

        Generic6DofSpringConstraint::Generic6DofSpringConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA,
                                                                  const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseLinearReferenceA)
        {
            SetBodies(ActorA,ActorB);

            btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
            btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
            Generic6dof = new btGeneric6DofSpringConstraint(*BodyA, *BodyB, transa, transb, UseLinearReferenceA);
        }

        Generic6DofSpringConstraint::Generic6DofSpringConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Transform& TransformA, const Transform& TransformB, bool UseLinearReferenceA)
        {
            SetBodies(ActorA,ActorB);

            Generic6dof = new btGeneric6DofSpringConstraint(*BodyA, *BodyB, TransformA.GetBulletTransform(), TransformB.GetBulletTransform(), UseLinearReferenceA);
        }


        Generic6DofSpringConstraint::~Generic6DofSpringConstraint()
        {
            if(Generic6dof)
            {
                delete Generic6dof;
                Generic6dof = NULL;
            }
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Generic6DofSpringConstraint Linear Spring Settings
        void Generic6DofSpringConstraint::SetSpringLinearStiffness(const Vector3& Stiffies)
            { SetSpringStiffness(LinearX, Stiffies.X); SetSpringStiffness(LinearY, Stiffies.Y); SetSpringStiffness(LinearZ, Stiffies.Z); }

        void Generic6DofSpringConstraint::SetSpringLinearDamping(const Vector3& Damps)
            { SetSpringDamping(LinearX, Damps.X); SetSpringDamping(LinearY, Damps.Y); SetSpringDamping(LinearZ, Damps.Z); }

        void Generic6DofSpringConstraint::SetSpringLinearEnabled(const Vector3& Enableness)
            { SetSpringEnabled(LinearX, Enableness.X); SetSpringEnabled(LinearY, Enableness.Y); SetSpringEnabled(LinearZ, Enableness.Z); }

        Vector3 Generic6DofSpringConstraint::GetSpringLinearStiffness() const
            { return Vector3(GetSpringStiffness(LinearX),GetSpringStiffness(LinearY),GetSpringStiffness(LinearZ)); }

        Vector3 Generic6DofSpringConstraint::GetSpringLinearDamping() const
            { return Vector3(GetSpringDamping(LinearX),GetSpringDamping(LinearY),GetSpringDamping(LinearZ)); }

        Vector3 Generic6DofSpringConstraint::GetSpringLinearEnabled() const
            { return Vector3(GetSpringEnabled(LinearX),GetSpringEnabled(LinearY),GetSpringEnabled(LinearZ)); }

        ////////////////////////////////////////////////////////////////////////////////
        // Generic6DofSpringConstraint Angular Spring Settings
        void Generic6DofSpringConstraint::SetSpringAngularStiffness(const Vector3& Stiffies)
            { SetSpringStiffness(AngularX, Stiffies.X); SetSpringStiffness(AngularY, Stiffies.Y); SetSpringStiffness(AngularZ, Stiffies.Z); }

        void Generic6DofSpringConstraint::SetSpringAngularDamping(const Vector3& Damps)
            { SetSpringDamping(AngularX, Damps.X); SetSpringDamping(AngularY, Damps.Y); SetSpringDamping(AngularZ, Damps.Z); }

        void Generic6DofSpringConstraint::SetSpringAngularEnabled(const Vector3& Enableness)
            { SetSpringEnabled(AngularX, Enableness.X); SetSpringEnabled(AngularY, Enableness.Y); SetSpringEnabled(AngularZ, Enableness.Z); }

        Vector3 Generic6DofSpringConstraint::GetSpringAngularStiffness() const
            { return Vector3(GetSpringStiffness(AngularX),GetSpringStiffness(AngularY),GetSpringStiffness(AngularZ)); }

        Vector3 Generic6DofSpringConstraint::GetSpringAngularDamping() const
            { return Vector3(GetSpringDamping(AngularX),GetSpringDamping(AngularY),GetSpringDamping(AngularZ)); }

        Vector3 Generic6DofSpringConstraint::GetSpringAngularEnabled() const
            { return Vector3(GetSpringEnabled(AngularX),GetSpringEnabled(AngularY),GetSpringEnabled(AngularZ)); }

        ////////////////////////////////////////////////////////////////////////////////
        // Generic6DofSpringConstraint Per Axis Spring Settings
        void Generic6DofSpringConstraint::SetSpringStiffness(int Index, Real Stiffness)
            { this->Generic6dofSpring()->setStiffness(Index, Stiffness); }

        void Generic6DofSpringConstraint::SetSpringDamping(int Index, Real Damping)
            { this->Generic6dofSpring()->setDamping(Index, Damping); }

        void Generic6DofSpringConstraint::SetSpringEnabled(int Index, bool Enable)
            { this->Generic6dofSpring()->enableSpring(Index, Enable); }

        Real Generic6DofSpringConstraint::GetSpringStiffness(int Index) const
            { return this->Generic6dofSpring()->m_springStiffness[Index]; }

        Real Generic6DofSpringConstraint::GetSpringDamping(int Index) const
            { return this->Generic6dofSpring()->m_springDamping[Index]; }

        bool Generic6DofSpringConstraint::GetSpringEnabled(int Index) const
            { return this->Generic6dofSpring()->m_springEnabled[Index]; }

        ////////////////////////////////////////////////////////////////////////////////
        // Generic6DofSpringConstraint Calculated Items
        void Generic6DofSpringConstraint::CalculateSpringEquilibriumPoint()
            { this->Generic6dofSpring()->setEquilibriumPoint(); }

        void Generic6DofSpringConstraint::CalculateSpringEquilibriumPoint(int Index)
            { this->Generic6dofSpring()->setEquilibriumPoint(Index); }

        Vector3 Generic6DofSpringConstraint::GetCurrentSpringAngularEquilibriumPoints() const
            { return Vector3(GetCurrentSpringEquilibriumPoint(AngularX),GetCurrentSpringEquilibriumPoint(AngularY),GetCurrentSpringEquilibriumPoint(AngularZ)); }

        Vector3 Generic6DofSpringConstraint::GetCurrentSpringLinearEquilibriumPoints() const
            { return Vector3(GetCurrentSpringEquilibriumPoint(LinearX),GetCurrentSpringEquilibriumPoint(LinearY),GetCurrentSpringEquilibriumPoint(LinearZ)); }

        Real Generic6DofSpringConstraint::GetCurrentSpringEquilibriumPoint(int Index) const
            { return this->Generic6dofSpring()->m_equilibriumPoint[Index]; }

        ////////////////////////////////////////////////////////////////////////////////
        // Generic6DofSpringConstraint Serialization
        void Generic6DofSpringConstraint::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node G6dofSpringNode = CurrentRoot.AppendChild(SerializableName());                        // The base node all the base constraint stuff will go in
            if (!G6dofSpringNode)
                { SerializeError("Create G6dofSpringNode", SerializableName()); }

            Mezzanine::XML::Attribute Version = G6dofSpringNode.AppendAttribute("Version");                      // Version
            if (!Version)
                { SerializeError("Create Version", SerializableName()); }
            Version.SetValue(1);

            this->Generic6DofConstraint::ProtoSerialize(G6dofSpringNode);                                   // Serialize the 6dof, dualtransform and the constraint

            XML::Node SpringStiffness = G6dofSpringNode.AppendChild("SpringStiffness");
            if (!SpringStiffness)
                { SerializeError("Create SpringStiffness", SerializableName()); }
            XML::Node SpringDamping = G6dofSpringNode.AppendChild("SpringDamping");
            if (!SpringDamping)
                { SerializeError("Create SpringDamping", SerializableName()); }
            XML::Node SpringEnabled = G6dofSpringNode.AppendChild("SpringEnabled");
            if (!SpringEnabled)
                { SerializeError("Create SpringEnabled", SerializableName()); }


            for (int c=0; c<6; ++c)                                                                         // Each of the spring attributes
            {
                String AttrName("Axis"+StringTools::ConvertToString(c));

                Mezzanine::XML::Attribute AxisStiffness = SpringStiffness.AppendAttribute(AttrName);
                if (!AxisStiffness)
                    { SerializeError("Create AxisStiffness-"+AttrName, SerializableName()); }
                AxisStiffness.SetValue(this->GetSpringStiffness(c));

                Mezzanine::XML::Attribute AxisDamping = SpringDamping.AppendAttribute(AttrName);
                if (!AxisDamping)
                    { SerializeError("Create AxisDamping-"+AttrName, SerializableName()); }
                AxisDamping.SetValue(this->GetSpringDamping(c));

                Mezzanine::XML::Attribute AxisEnabled = SpringEnabled.AppendAttribute(AttrName);
                if (!AxisEnabled)
                    { SerializeError("Create AxisEnabled-"+AttrName, SerializableName()); }
                AxisEnabled.SetValue(this->GetSpringEnabled(c));
            }
        }

        void Generic6DofSpringConstraint::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->Generic6DofSpringConstraint::SerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    XML::Node g6dof = OneNode.GetChild("Generic6DofConstraint");
                    if(!g6dof)
                        { DeSerializeError("locate Generic6DofConstraint node",SerializableName()); }
                    this->Generic6DofConstraint::ProtoDeSerialize(g6dof);

                    XML::Node SpringStiffness = OneNode.GetChild("SpringStiffness");
                    if (!SpringStiffness)
                        { DeSerializeError("Find SpringStiffness Node", SerializableName()); }
                    XML::Node SpringDamping = OneNode.GetChild("SpringDamping");
                    if (!SpringDamping)
                        { DeSerializeError("Find SpringDamping Node", SerializableName()); }
                    XML::Node SpringEnabled = OneNode.GetChild("SpringEnabled");
                    if (!SpringEnabled)
                        { DeSerializeError("Find SpringEnabled Node", SerializableName()); }

                    XML::Attribute SpringStiffnessAttr = SpringStiffness.GetFirstAttribute();
                    while(SpringStiffnessAttr)
                    {
                        String CurrentName(SpringStiffnessAttr.Name());
                        if (4>CurrentName.size())
                            { DeSerializeError("Invalid axis attribute", SerializableName()); }
                        this->SetSpringStiffness(char4ToAxis(CurrentName[4]),SpringStiffnessAttr.AsReal());
                        SpringStiffnessAttr = SpringStiffnessAttr.GetNextAttribute();
                    }

                    XML::Attribute SpringDampingAttr = SpringDamping.GetFirstAttribute();
                    while(SpringDampingAttr)
                    {
                        String CurrentName(SpringDampingAttr.Name());
                        if (4>CurrentName.size())
                            { DeSerializeError("Invalid axis attribute", SerializableName()); }
                        this->SetSpringDamping(char4ToAxis(CurrentName[4]),SpringDampingAttr.AsReal());
                        SpringDampingAttr = SpringDampingAttr.GetNextAttribute();
                    }

                    XML::Attribute SpringEnabledAttr = SpringEnabled.GetFirstAttribute();
                    while(SpringEnabledAttr)
                    {
                        String CurrentName(SpringEnabledAttr.Name());
                        if (4>CurrentName.size())
                            { DeSerializeError("Invalid axis attribute", SerializableName()); }
                        this->SetSpringEnabled(char4ToAxis(CurrentName[4]),SpringEnabledAttr.AsBool());
                        SpringEnabledAttr = SpringEnabledAttr.GetNextAttribute();
                    }
                }else{
                    DeSerializeError("find usable serialization version",SerializableName());
                }
            }else{
                DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
            }
        }

        String Generic6DofSpringConstraint::SerializableName()
            { return String("Generic6DofSpringConstraint"); }
    }//Physics
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::Generic6DofSpringConstraint& x)
{
    Mezzanine::Serialize(stream,x);
    return stream;
}

std::istream& operator >> (std::istream& stream, Mezzanine::Physics::Generic6DofSpringConstraint& x)
    { return Mezzanine::DeSerialize(stream, x); }

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Generic6DofSpringConstraint& x)
    { x.ProtoDeSerialize(OneNode); }

#endif
