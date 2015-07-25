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
#ifndef _physicsconstraint_cpp
#define _physicsconstraint_cpp

#include "Physics/constraint.h"
#include "Physics/physicsenumerations.h"
#include "Physics/physicsmanager.h"
#include "Physics/rigidproxy.h"

#include "actormanager.h"
#include "entresol.h"

#include "stringtool.h"
#include "exception.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>

/// @cond DontDocumentInternal

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
                default: { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempted to convert invalid Constraint Paramater to a String."); }
            }
        }

        ConstraintParam StringAsConstraintParam(String Param)
        {
            if(5>Param.size())
                { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempted to convert invalid String to Constraint Paramater: Too Short."); }

            switch(Param.at(4))
            {
                case 'E':
                    if(ConstraintParamAsString(Con_ERP)==Param)
                        { return Con_ERP; }
                    else
                        { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempted to convert invalid String to Constraint Paramater: Appears to be Con_ERP but isn't."); }
                case 'C':
                    if(ConstraintParamAsString(Con_CFM)==Param)
                        { return Con_CFM; }
                    else
                        { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempted to convert invalid String to Constraint Paramater: Appears to be Con_CFM but isn't."); }
                case 'S':
                    switch(Param.at(9))
                    {
                        case 'E':
                            if(ConstraintParamAsString(Con_Stop_ERP)==Param)
                                { return Con_Stop_ERP; }
                            else
                                { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempted to convert invalid String to Constraint Paramater: Appears to be Con_Stop_ERP but isn't."); }
                        case 'C':
                            if(ConstraintParamAsString(Con_Stop_CFM)==Param)
                                { return Con_Stop_CFM; }
                            else
                                { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempted to convert invalid String to Constraint Paramater: Appears to be Con_Stop_CFM but isn't."); }
                        case 'S':

                        default:
                            { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempted to convert invalid String to Constraint Paramater: Appeared to be Con_Stop_Something, but wasn't."); }
                    }
                default:
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempted to convert invalid String to Constraint Paramater: Invalid Name."); }
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
                default: { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Cannot convert invalid axis name."); }
            }
        }

        /////////////////////////////////////////
        // Constraint Functions

        ////////////////////////////////////////////////////////////////////////////////
        // Constraint Protected Methods

        Constraint::Constraint(const UInt32 ID, PhysicsManager* Creator) :
            ProxA(NULL),
            ProxB(NULL),
            Manager(Creator),
            ConstraintID(ID),
            AllowCollisions(true),
            Enabled(false)
            {  }

        Constraint::Constraint(const UInt32 ID, RigidProxy* Prox1, PhysicsManager* Creator) :
            ProxA(Prox1),
            ProxB(NULL),
            Manager(Creator),
            ConstraintID(ID),
            AllowCollisions(true),
            Enabled(false)
        {
            if( this->ProxA ) {
                this->ProxA->SetActivationState(Physics::AS_DisableDeactivation);
            }
        }

        Constraint::Constraint(const UInt32 ID, RigidProxy* Prox1, RigidProxy* Prox2, PhysicsManager* Creator) :
            ProxA(Prox1),
            ProxB(Prox2),
            Manager(Creator),
            ConstraintID(ID),
            AllowCollisions(true),
            Enabled(false)
        {
            if( this->ProxA ) {
                this->ProxA->SetActivationState(Physics::AS_DisableDeactivation);
            }
            if( this->ProxB ) {
                this->ProxB->SetActivationState(Physics::AS_DisableDeactivation);
            }
        }

        Constraint::~Constraint()
            {  }

        ////////////////////////////////////////////////////////////////////////////////
        // Utility

        UInt32 Constraint::GetConstraintID() const
            { return this->ConstraintID; }

        void Constraint::EnableConstraint(const Boole Enable)
        {
            if( Enable != this->Enabled ) {
                if( Enable ) {
                    this->Manager->_GetPhysicsWorldPointer()->addConstraint(this->_GetConstraintBase(),!this->AllowCollisions);
                }else{
                    this->Manager->_GetPhysicsWorldPointer()->removeConstraint(this->_GetConstraintBase());
                }
                this->Enabled = Enable;
            }
        }

        Boole Constraint::IsConstraintEnabled() const
            { return this->Enabled; }

        void Constraint::SetAllowCollisions(const Boole Allow)
            { this->AllowCollisions = Allow; }

        Boole Constraint::GetAllowCollisions() const
            { return this->AllowCollisions; }

        RigidProxy* Constraint::GetProxyA() const
            { return this->ProxA; }

        RigidProxy* Constraint::GetProxyB() const
            { return this->ProxB; }

        ///////////////////////////////////////////////////////////////////////////////
        // Constraint Parameters

        Constraint::AxisList Constraint::GetValidAxes() const
        {
            AxisList Ang = this->GetValidAngularAxes();
            AxisList Lin = this->GetValidLinearAxes();
            Lin.insert(Lin.end(),Ang.begin(),Ang.end());              // just kinda stick the two together and return that, no checking is performed for uniqueness.
            std::sort(Lin.begin(),Lin.end());                         // Usually the Linear axis are 0,1,2 and the angular are 3,4,5 so hopefully, this will hardly ever need to do work.
            return Lin;
        }

        Boole Constraint::IsParamValidOnAxis(ConstraintParam Param, int Axis) const
        {
            ParamList Supported = GetValidParamsOnAxis(Axis);
            if( size_t(ConstraintParamCount) == Supported.size() ) {                         // no need to check deeper, because everything is supported
                return true;
            }else{
                if( Supported.size() ) {
                    return ( std::find(Supported.begin(),Supported.end(),Param) != Supported.end() );     // should return true if found
                }else{
                    return false;                                                   // size is 0 of course it is not supported
                }
            }
        }

        Constraint::ParamList Constraint::GetValidParams() const
        {
            AxisList AllAxis = this->GetValidAxes();
            ParamList Results;
            if( AllAxis.size() ) {                                            //If we have no Axis, the we have no valid Parameters.
                Results = this->GetValidParamsOnAxis( *(AllAxis.begin()) );       // Let's start off with whatever the first Axis Support, then we will trim from here.

                if( AllAxis.size() > 1 ) {                                      //if the constraint only support one axis then we already have our answer, and we don't want to run of the end of the Paramlist
                    for( AxisList::iterator AxisIter = AllAxis.begin() + 1 ; AllAxis.end() != AxisIter; ++AxisIter )       //For each axis after the first
                    {
                        for( int ParamID = Results.size() ; ParamID <= 0 ; --ParamID )        // We start at the back and work by index because reverse erase does no accept rever iterators, and
                        {
                            if( !this->IsParamValidOnAxis(Results.at(ParamID),*AxisIter) ) {   // if an Item is erase near the beginning, it changes all the items after, making forward iteration logic that
                                Results.erase( Results.begin()+ParamID );                      // erases needlessly complicated, and potentially expensive depending on the Vector implementation
                            }
                        }
                    }
                }
            }
            sort(Results.begin(),Results.end());
            return Results;
        }

        void Constraint::SetParam(ConstraintParam Param, Real Value, int Axis)
            { this->_GetConstraintBase()->setParam(Param, Value, Axis); }

        Real Constraint::GetParam(ConstraintParam Param, int Axis) const
            { return this->_GetConstraintBase()->getParam(Param, Axis); }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void Constraint::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild(this->GetDerivedSerializableName());

            this->ProtoSerializeInitData(SelfRoot);
            this->ProtoSerializeProperties(SelfRoot);
            this->ProtoSerializeGlobalOverrides(SelfRoot);
        }

        void Constraint::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            XML::Node PropertiesNode = SelfRoot.AppendChild( Constraint::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("ID").SetValue( this->ConstraintID ) &&
                PropertiesNode.AppendAttribute("Enabled").SetValue( this->IsConstraintEnabled() ) &&
                PropertiesNode.AppendAttribute("AllowCollisions").SetValue( this->GetAllowCollisions() ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",Constraint::GetSerializableName() + "Properties",true);
            }
        }

        void Constraint::ProtoSerializeGlobalOverrides(XML::Node& SelfRoot) const
        {
            XML::Node GlobalOverridesNode = SelfRoot.AppendChild( Constraint::GetSerializableName() + "GlobalOverrides" );

            String CurrentAxisName;
            AxisList AllAxis = this->GetValidAxes();
            for( AxisList::iterator AxisIter = AllAxis.begin() ; AllAxis.end() != AxisIter ; ++AxisIter )
            {
                XML::Node OverrideNode = GlobalOverridesNode.AppendChild("ParamOverride");

                ParamList AxisParams = this->GetValidParamsOnAxis(*AxisIter);
                for( ParamList::iterator ParamIter = AxisParams.begin() ; AxisParams.end() != ParamIter; ++ParamIter )
                {
                    if( this->HasParamBeenSet(*ParamIter,*AxisIter) ) {
                        if( OverrideNode.AppendAttribute("Version").SetValue("1") &&
                            OverrideNode.AppendAttribute("Axis").SetValue( *AxisIter ) &&
                            OverrideNode.AppendAttribute("Param").SetValue( ConstraintParamAsString(*ParamIter) ) &&
                            OverrideNode.AppendAttribute("Value").SetValue( this->GetParam(*ParamIter,*AxisIter) ) )
                        {
                            continue;
                        }else{
                            SerializeError(String("Create ") + ConstraintParamAsString(*ParamIter) + " Attribute in " + CurrentAxisName + " Node.",this->GetSerializableName());
                        }
                    }// If the parameter has already been set
                }// For each param
            }// For each axis
        }

        void Constraint::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            this->ProtoDeSerializeInitData(SelfRoot);
            this->ProtoDeSerializeProperties(SelfRoot);
            this->ProtoDeSerializeGlobalOverrides(SelfRoot);
        }

        void Constraint::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( Constraint::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("ID");
                    if( !CurrAttrib.Empty() )
                        this->ConstraintID = static_cast<UInt32>( CurrAttrib.AsUint() );

                    CurrAttrib = PropertiesNode.GetAttribute("AllowCollisions");
                    if( !CurrAttrib.Empty() )
                        this->SetAllowCollisions( CurrAttrib.AsBool(true) );

                    CurrAttrib = PropertiesNode.GetAttribute("Enabled");
                    if( !CurrAttrib.Empty() )
                        this->EnableConstraint( CurrAttrib.AsBool(false) );
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( Constraint::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,Constraint::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        void Constraint::ProtoDeSerializeGlobalOverrides(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;
            XML::Node OverridesNode = SelfRoot.GetChild( Constraint::GetSerializableName() + "GlobalOverrides" );

            if( !OverridesNode.Empty() ) {
                if( OverridesNode.GetAttribute("Version").AsInt() == 1 ) {
                    for( XML::NodeIterator OverrideNodeIt = OverridesNode.begin() ; OverrideNodeIt != OverridesNode.end() ; ++OverrideNodeIt )
                    {
                        if( (*OverrideNodeIt).GetAttribute("Version").AsInt() == 1 ) {
                            Integer Axis = -99;       // Known invalid axis just about everywhere
                            Integer Param = 0;        // Known invalid param
                            Real OverrideValue = 0.0;

                            CurrAttrib = (*OverrideNodeIt).GetAttribute("Axis");
                            if( !CurrAttrib.Empty() )
                                Axis = CurrAttrib.AsInt();

                            CurrAttrib = (*OverrideNodeIt).GetAttribute("Param");
                            if( !CurrAttrib.Empty() )
                                Param = CurrAttrib.AsUint();

                            CurrAttrib = (*OverrideNodeIt).GetAttribute("Value");
                            if( !CurrAttrib.Empty() )
                                OverrideValue = CurrAttrib.AsReal();

                            if( this->IsParamValidOnAxis(static_cast<ConstraintParam>(Param),Axis) ) {
                                this->SetParam(static_cast<ConstraintParam>(Param),OverrideValue,Axis);
                            }
                        }else{
                            MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + String("ParamOverride") + ": Not Version 1.");
                        }
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( Constraint::GetSerializableName() + "GlobalOverrides" ) + ": Not Version 1.");
                }
            }
        }

        String Constraint::GetSerializableName()
            { return "Constraint"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods
    }//Physics
}//Mezzanine

std::ostream& MEZZ_LIB operator<< (std::ostream& Stream, const Mezzanine::Physics::Constraint* Con)
{
    Mezzanine::Serialize(Stream,*Con);
    return Stream;
}

std::istream& MEZZ_LIB operator>> (std::istream& Stream, Mezzanine::Physics::Constraint* Con)
{
    return Mezzanine::DeSerialize(Stream,*Con);
}

void MEZZ_LIB operator<< (Mezzanine::XML::Node& OneNode, const Mezzanine::Physics::Constraint* Con)
{
    Con->ProtoSerialize(OneNode);
}

void MEZZ_LIB operator>> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Constraint* Con)
{
    Con->ProtoDeSerialize(OneNode);
}

/// @endcond

#endif
