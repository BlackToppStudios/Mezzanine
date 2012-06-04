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
#ifndef _physicsdualtransformconstraint_cpp
#define _physicsdualtransformconstraint_cpp

#include "dualtransformconstraint.h"
#include "stringtool.h"
#include "serialization.h"

namespace Mezzanine
{
    namespace Physics
    {
#ifdef MEZZXML
        ///////////////////////////////////////////////////////////////////////////////
        // DualTransformConstraint Serialization

        // Serializable
        void DualTransformConstraint::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node DualTransformConstraintNode = CurrentRoot.AppendChild(SerializableName());                     // The base node all the base constraint stuff will go in
            if (!DualTransformConstraintNode)
                { SerializeError("Create DualTransformConstraintNode", SerializableName()); }

            XML::Attribute Version = DualTransformConstraintNode.AppendAttribute("Version");
            if (Version)
            {
                Version.SetValue(1);
            }else{
                SerializeError("Create Attributes on DualTransformConstraintNode", SerializableName());
            }

            XML::Node ActorANode = DualTransformConstraintNode.AppendChild("ActorA");                     // Get everything we need about ActorA
            if (!ActorANode)
                { SerializeError("Create ActorANode", SerializableName()); }
            this->GetPivotATransform().ProtoSerialize(ActorANode);

            XML::Node ActorBNode = DualTransformConstraintNode.AppendChild("ActorB");                     // Get everything we need about ActorB
            if (!ActorBNode)
                { SerializeError("Create ActorBNode", SerializableName()); }
            this->GetPivotBTransform().ProtoSerialize(ActorBNode);

            this->Constraint::ProtoSerialize(DualTransformConstraintNode);
        }

        // DeSerializable
        void DualTransformConstraint::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->DualTransformConstraint::SerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    XML::Node ConstraintNode = OneNode.GetChild("Constraint");
                    if(!ConstraintNode)
                        { DeSerializeError("locate Constraint node",SerializableName()); }
                    this->Constraint::ProtoDeSerialize(ConstraintNode);

                    XML::Node TransformA = OneNode.GetChild("ActorA").GetFirstChild();
                    if (!TransformA)
                        { DeSerializeError("locate transform for ActorA",SerializableName()); }
                    Transform temp;
                    temp.ProtoDeSerialize(TransformA);
                    this->SetPivotATransform(temp);

                    XML::Node TransformB = OneNode.GetChild("ActorB").GetFirstChild();
                    if (!TransformB)
                        { DeSerializeError("locate transform for ActorB",SerializableName()); }
                    temp.ProtoDeSerialize(TransformB);
                    this->SetPivotBTransform(temp);
                }else{
                    DeSerializeError("find usable serialization version",SerializableName());
                }
            }else{
                DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
            }
        }

        String DualTransformConstraint::SerializableName()
            { return String("DualTransformConstraint"); }
#endif // /MEZZXML
    }//Physics
}//Mezzanine

#ifdef MEZZXML
    ///////////////////////////////////////////////////////////////////////////////
    // Class External << Operators for streaming or assignment

    std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::DualTransformConstraint& x)
    {
        Mezzanine::Serialize(stream,x);
        return stream;
    }

    std::istream& operator >> (std::istream& stream, Mezzanine::Physics::DualTransformConstraint& x)
        { return Mezzanine::DeSerialize(stream, x); }

    void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::DualTransformConstraint& x)
        { x.ProtoDeSerialize(OneNode); }
#endif // /MEZZXML

#endif
