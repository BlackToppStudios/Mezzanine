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
#ifndef _physicssixdofconstraint_cpp
#define _physicssixdofconstraint_cpp

#include "Physics/sixdofconstraint.h"

namespace Mezzanine
{
    namespace Physics
    {
        SixDofConstraint::SixDofConstraint(const UInt32 ID, RigidProxy* Prox1, PhysicsManager* Creator) :
            DualTransformConstraint(ID,Prox1,Creator)
            {  }

        SixDofConstraint::SixDofConstraint(const UInt32 ID, RigidProxy* Prox1, RigidProxy* Prox2, PhysicsManager* Creator) :
            DualTransformConstraint(ID,Prox1,Prox2,Creator)
            {  }

        SixDofConstraint::~SixDofConstraint()
            {  }

        ////////////////////////////////////////////////////////////////////////////////
        // Location and Rotation

        ////////////////////////////////////////////////////////////////////////////////
        // Query

        ////////////////////////////////////////////////////////////////////////////////
        // Utility

        Whole SixDofConstraint::ConvertFrom6AxisTo3Axis(const Whole Axis)
            { return ( Axis % 3 ); }

        Whole SixDofConstraint::ConvertAngularTo6Axis(const Whole Axis)
            { return ( Axis > 2 ? Axis : Axis + 3 ); }

        ////////////////////////////////////////////////////////////////////////////////
        // Basic Limit Accessors

        ////////////////////////////////////////////////////////////////////////////////
        // Angular Limit and Motor Details

        ////////////////////////////////////////////////////////////////////////////////
        // Linear Limit and Motor Details

        ////////////////////////////////////////////////////////////////////////////////
        // Axis Params

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void SixDofConstraint::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Constraint::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( SixDofConstraint::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") )
            {
                XML::Node LinearAxesNode = PropertiesNode.AppendChild("LinearAxes");
                for( Integer LinAxis = Physics::LinearStart ; LinAxis < Physics::LinearEnd ; ++LinAxis )
                {
                    XML::Node LinearAxisNode = LinearAxesNode.AppendChild("LinearAxis");
                    if( LinearAxisNode.AppendAttribute("Version").SetValue("1") &&
                        LinearAxisNode.AppendAttribute("Axis").SetValue( LinAxis ) &&
                        LinearAxisNode.AppendAttribute("LimitLower").SetValue( this->GetLinearLimitLowerOnAxis(LinAxis) ) &&
                        LinearAxisNode.AppendAttribute("LimitUpper").SetValue( this->GetLinearLimitUpperOnAxis(LinAxis) ) &&
                        LinearAxisNode.AppendAttribute("MotorMaxForce").SetValue( this->GetLinearMotorMaxForceOnAxis(LinAxis) ) &&
                        LinearAxisNode.AppendAttribute("MotorTargetVelocity").SetValue( this->GetLinearMotorTargetVelocityOnAxis(LinAxis) ) &&
                        LinearAxisNode.AppendAttribute("MotorEnabled").SetValue( this->GetLinearMotorEnabledOnAxis(LinAxis) ) )
                    {
                        continue;
                    }else{
                        SerializeError("Create XML Attribute Values",SixDofConstraint::GetSerializableName() + " Linear Axes",true);
                    }
                }

                XML::Node AngularAxesNode = PropertiesNode.AppendChild("AngularAxes");
                for( Integer AngAxis = Physics::AngularStart ; AngAxis < Physics::AngularEnd ; ++AngAxis )
                {
                    XML::Node AngularAxisNode = AngularAxesNode.AppendChild("AngularAxis");
                    if( AngularAxisNode.AppendAttribute("Version").SetValue("1") &&
                        AngularAxisNode.AppendAttribute("Axis").SetValue( ConvertFrom6AxisTo3Axis(AngAxis) ) &&
                        AngularAxisNode.AppendAttribute("LimitLower").SetValue( this->GetAngularLimitLowerOnAxis( ConvertFrom6AxisTo3Axis(AngAxis) ) ) &&
                        AngularAxisNode.AppendAttribute("LimitUpper").SetValue( this->GetAngularLimitUpperOnAxis( ConvertFrom6AxisTo3Axis(AngAxis) ) ) &&
                        AngularAxisNode.AppendAttribute("MotorTargetVelocity").SetValue( this->GetAngularMotorTargetVelocityOnAxis( ConvertFrom6AxisTo3Axis(AngAxis) ) ) &&
                        AngularAxisNode.AppendAttribute("MotorMaxForce").SetValue( this->GetAngularMotorMaxForceOnAxis( ConvertFrom6AxisTo3Axis(AngAxis) ) ) &&
                        AngularAxisNode.AppendAttribute("MotorDamping").SetValue( this->GetAngularMotorDampingOnAxis( ConvertFrom6AxisTo3Axis(AngAxis) ) ) &&
                        AngularAxisNode.AppendAttribute("MotorRestitution").SetValue( this->GetAngularMotorRestitutionOnAxis( ConvertFrom6AxisTo3Axis(AngAxis) ) ) &&
                        AngularAxisNode.AppendAttribute("MotorEnabled").SetValue( this->GetAngularMotorEnabledOnAxis( ConvertFrom6AxisTo3Axis(AngAxis) ) ) )
                    {
                        continue;
                    }else{
                        SerializeError("Create XML Attribute Values",SixDofConstraint::GetSerializableName() + " Angular Axes",true);
                    }
                }

                return;
            }else{
                SerializeError("Create XML Attribute Values",SixDofConstraint::GetSerializableName() + "Properties",true);
            }
        }

        void SixDofConstraint::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Constraint::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( SixDofConstraint::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if( PropertiesNode.GetAttribute("Version").AsInt() == 1 ) {
                    XML::Node LinearAxesNode = PropertiesNode.GetChild("LinearAxes");
                    for( XML::NodeIterator LinearAxesNodeIt = LinearAxesNode.begin() ; LinearAxesNodeIt != LinearAxesNode.end() ; ++LinearAxesNodeIt )
                    {
                        if( !(*LinearAxesNodeIt).Empty() ) {
                            if( (*LinearAxesNodeIt).GetAttribute("Version").AsInt() == 1 ) {
                                Integer Axis = -1;

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("Axis");
                                if( !CurrAttrib.Empty() )
                                    Axis = CurrAttrib.AsInt();

                                if( Axis < 0 || Axis > 2 )
                                    continue;

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("LimitLower");
                                if( !CurrAttrib.Empty() )
                                    this->SetLinearLimitLowerOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("LimitUpper");
                                if( !CurrAttrib.Empty() )
                                    this->SetLinearLimitUpperOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("MotorMaxForce");
                                if( !CurrAttrib.Empty() )
                                    this->SetLinearMotorMaxForceOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("MotorTargetVelocity");
                                if( !CurrAttrib.Empty() )
                                    this->SetLinearMotorTargetVelocityOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("MotorEnabled");
                                if( !CurrAttrib.Empty() )
                                    this->SetLinearMotorEnabledOnAxis( CurrAttrib.AsReal(), Axis );
                            }
                        }
                    }

                    XML::Node AngularAxesNode = PropertiesNode.GetChild("AngularAxes");
                    for( XML::NodeIterator AngularAxesNodeIt = AngularAxesNode.begin() ; AngularAxesNodeIt != AngularAxesNode.end() ; ++AngularAxesNodeIt )
                    {
                        if( !(*AngularAxesNodeIt).Empty() ) {
                            if( (*AngularAxesNodeIt).GetAttribute("Version").AsInt() == 1 ) {
                                Integer Axis = -1;

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("Axis");
                                if( !CurrAttrib.Empty() )
                                    Axis = CurrAttrib.AsInt();

                                if( Axis < 0 || Axis > 5 )
                                    continue;

                                Axis = this->ConvertFrom6AxisTo3Axis(Axis);

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("LimitLower");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularLimitLowerOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("LimitUpper");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularLimitUpperOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("MotorTargetVelocity");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularMotorTargetVelocityOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("MotorMaxForce");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularMotorMaxForceOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("MotorDamping");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularMotorDampingOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("MotorRestitution");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularMotorRestitutionOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("MotorEnabled");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularMotorEnabledOnAxis( CurrAttrib.AsReal(), Axis );
                            }
                        }
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( SixDofConstraint::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,SixDofConstraint::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String SixDofConstraint::GetSerializableName()
            { return "SixDofConstraint"; }
    }//Physics
}//Mezzanine

#endif
