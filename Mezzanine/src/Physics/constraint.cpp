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
#ifndef _physicsconstraint_cpp
#define _physicsconstraint_cpp

#include "constraint.h"
#include "actormanager.h"
#include "actorrigid.h"
#include "stringtool.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>

/// @cond 0

namespace Mezzanine
{
    namespace Physics
    {
        /////////////////////////////////////////
        // Functions

        String ConstraintParamAsString(ConstraintParam Param)
        {
            switch (Param)
            {
                case Con_ERP:           return String("Con_ERP");
                case Con_Stop_ERP:      return String("Con_Stop_ERP");
                case Con_CFM:           return String("Con_CFM");
                case Con_Stop_CFM:      return String("Con_Stop_CFM");
                default: { MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Attempted to convert invalid Constraint Paramater to a String."); }
            }
        }

        ConstraintParam StringAsConstraintParam(String Param)
        {
            if(5>Param.size())
                { MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Attempted to convert invalid String to Constraint Paramater: Too Short."); }

            switch(Param.at(4))
            {
                case 'E':
                    if(ConstraintParamAsString(Con_ERP)==Param)
                        { return Con_ERP; }
                    else
                        { MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Attempted to convert invalid String to Constraint Paramater: Appears to be Con_ERP but isn't."); }
                case 'C':
                    if(ConstraintParamAsString(Con_CFM)==Param)
                        { return Con_CFM; }
                    else
                        { MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Attempted to convert invalid String to Constraint Paramater: Appears to be Con_CFM but isn't."); }
                case 'S':
                    switch(Param.at(9))
                    {
                        case 'E':
                            if(ConstraintParamAsString(Con_Stop_ERP)==Param)
                                { return Con_Stop_ERP; }
                            else
                                { MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Attempted to convert invalid String to Constraint Paramater: Appears to be Con_Stop_ERP but isn't."); }
                        case 'C':
                            if(ConstraintParamAsString(Con_Stop_CFM)==Param)
                                { return Con_Stop_CFM; }
                            else
                                { MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Attempted to convert invalid String to Constraint Paramater: Appears to be Con_Stop_CFM but isn't."); }
                        case 'S':

                        default:
                            { MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Attempted to convert invalid String to Constraint Paramater: Appeared to be Con_Stop_Something, but wasn't."); }
                    }
                default:
                    { MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Attempted to convert invalid String to Constraint Paramater: Invalid Name."); }
            }
        }

        int char4ToAxis(char it)
        {
            switch(it)
            {
                case '-': return -1;         break;
                case '0': return 0;          break;
                case '1': return 1;          break;
                case '2': return 2;          break;
                case '3': return 3;          break;
                case '4': return 4;          break;
                case '5': return 5;          break;
                default: { MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Cannot convert invalid axis name."); }
            }
        }

        /////////////////////////////////////////
        // Constraint Functions

        ////////////////////////////////////////////////////////////////////////////////
        // Constraint Protected Methods
        Constraint::Constraint()
            { }

        void Constraint::SetBodies(ActorRigid* Act1, ActorRigid* Act2)
        {
            ActA = Act1;
            ActB = Act2;
            BodyA = Act1->PhysicsRigidBody;
            BodyB = Act2->PhysicsRigidBody;
            BodyA->setActivationState(Mezzanine::Physics::WOAS_DisableDeactivation);
            BodyB->setActivationState(Mezzanine::Physics::WOAS_DisableDeactivation);
        }

        void Constraint::SetBodies(ActorRigid* Act1)
        {
            ActA = Act1;
            ActB = NULL;
            BodyA = Act1->PhysicsRigidBody;
            BodyB = NULL;
            BodyA->setActivationState(Mezzanine::Physics::WOAS_DisableDeactivation);
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Constraint Core Functionality
        Constraint::~Constraint()
            { }

        ActorRigid* Constraint::GetActorA() const
            { return ActA; }

        ActorRigid* Constraint::GetActorB() const
            { return ActB; }

        ///////////////////////////////////////////////////////////////////////////////
        // Constraint Parameters
        void Constraint::SetParam(ConstraintParam Param, Real Value, int Axis)
            { this->GetConstraintBase()->setParam(Param, Value, Axis); }

        Real Constraint::GetParam(ConstraintParam Param, int Axis) const
            { return this->GetConstraintBase()->getParam(Param, Axis); }

        ///////////////////////////////////////////////////////////////////////////////
        // Constraint Serialization
        // Serializable
        void Constraint::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node ConstraintNode = CurrentRoot.AppendChild(SerializableName());                     // The base node all the base constraint stuff will go in
            if (!ConstraintNode)
                { SerializeError("Create ConstraintNode", SerializableName()); }

            XML::Attribute Version = ConstraintNode.AppendAttribute("Version");
            XML::Attribute ActorNameA = ConstraintNode.AppendAttribute("ActorNameA");
            XML::Attribute ActorNameB = ConstraintNode.AppendAttribute("ActorNameB");

            if (Version && ActorNameA && ActorNameB)
            {
                Version.SetValue(1);
                ActorNameA.SetValue( this->GetActorA()->GetName() );
                ActorNameB.SetValue( this->GetActorB()->GetName() );
            }else{
                SerializeError("Create Attributes on ConstraintNode", SerializableName());
            }

            String CurrentAxisName;
            AxisList AllAxis = this->ValidAxis();
            for(AxisList::iterator AxisIter=AllAxis.begin(); AllAxis.end()!=AxisIter; ++AxisIter)
            {
                XML::Node OneAxisNode;
                CurrentAxisName = String(StringTools::StringCat("Axis",StringTools::ConvertToString(*AxisIter)));                        // Should result in "Axis-1", "Axis0", "Axis1" ...
                ParamList AxisParams = ValidParamOnAxis(*AxisIter);
                for(ParamList::iterator ParamIter=AxisParams.begin(); AxisParams.end()!=ParamIter; ++ParamIter)
                {
                    if(HasParamBeenSet(*ParamIter,*AxisIter))                                           // No need to create a node if no attributes exist for it, so we will create one for the first attribute that does exist and
                    {                                                                                   // reuse it until we move onto the next Axis
                        if (!OneAxisNode)
                        {
                            OneAxisNode = ConstraintNode.AppendChild(CurrentAxisName);
                            if (!OneAxisNode)
                                { SerializeError( StringTools::StringCat("Create ", CurrentAxisName ," Node"), SerializableName()); }
                        }

                        XML::Attribute CurrenParamAttribute = OneAxisNode.AppendAttribute( ConstraintParamAsString(*ParamIter) );
                        if (!CurrenParamAttribute)
                            { SerializeError( StringTools::StringCat("Create ", ConstraintParamAsString(*ParamIter), " Attribute in ", CurrentAxisName ," Node"), SerializableName()); }
                        CurrenParamAttribute.SetValue( this->GetParam(*ParamIter,*AxisIter));
                    }
                }
            }
        }

        // DeSerializable
        void Constraint::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->Constraint::SerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    String ActorNameA(OneNode.GetAttribute("ActorNameA").AsString());                                                           // get Actors from the XML
                    String ActorNameB(OneNode.GetAttribute("ActorNameB").AsString());
                    if (""!=ActorNameA)                                                                                                         //Figure out if the actors are fine
                    {
                        ActorRigid* FutureA = dynamic_cast<ActorRigid*>(ActorManager::GetSingletonPtr()->GetActor(ActorNameA));     // get ActorA from the Actormanager
                        if (0==FutureA)
                            { DeSerializeError("find an ActorRigid named "+ActorNameA+" in the ActorManager", SerializableName()); }

                        if (""!=ActorNameB)
                        {
                            ActorRigid* FutureB = dynamic_cast<ActorRigid*>(ActorManager::GetSingletonPtr()->GetActor(ActorNameB)); // get ActorB from the Actormanager
                            if (0==FutureB)
                                { DeSerializeError("find an ActorRigid named "+ActorNameB+" in the ActorManager", SerializableName()); }
                            this->SetBodies(FutureA,FutureB);
                        }else{
                            this->SetBodies(FutureA);
                        }
                    }else{
                        DeSerializeError("retrieve ActorNameA",SerializableName());
                    }

                    XML::Node TheAxis = OneNode.GetFirstChild();
                    while(TheAxis)
                    {
                        String EnemyName(TheAxis.Name());                            //WWII country are we dealing with.
                        if(4>EnemyName.size())                                       //No country on the axis side WWII had fewer than 4 letters in its name. if USA somehow lands on this list it is an error
                            { DeSerializeError("find valid axis name, name is too short",SerializableName()); }
                        int AxisValue;

                        AxisValue=char4ToAxis(EnemyName[4]);

                        XML::Attribute AxisAttribute = TheAxis.GetFirstAttribute();
                        while(AxisAttribute)
                        {
                            this->SetParam(StringAsConstraintParam(AxisAttribute.Name()),AxisAttribute.AsReal(),AxisValue);
                            AxisAttribute = AxisAttribute.GetNextAttribute();
                        }

                        TheAxis = TheAxis.GetNextSibling();
                    }// /While(TheAxis)

                }else{
                    DeSerializeError("Incompatible XML Version for "+SerializableName(),SerializableName());
                }
            }else{
                DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name()+",",SerializableName());
            }
        }

        String Constraint::SerializableName()
            { return String("Constraint"); }

    }//Physics
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::Constraint& x)
{
    Mezzanine::Serialize(stream,x);
    return stream;
}

std::istream& operator >> (std::istream& stream, Mezzanine::Physics::Constraint& x)
    { return Mezzanine::DeSerialize(stream, x); }

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Constraint& x)
    { x.ProtoDeSerialize(OneNode); }

/// @endcond

#endif
