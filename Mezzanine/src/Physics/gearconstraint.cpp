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
#ifndef _physicsgearconstraint_cpp
#define _physicsgearconstraint_cpp

#include "Physics/gearconstraint.h"
#include "Physics/physicsmanager.h"
#include "Physics/rigidproxy.h"

#include "stringtool.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Physics
    {
        GearConstraint::GearConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& AxisA, const Vector3& AxisB, PhysicsManager* Creator) :
            Constraint(ID,ProxyA,ProxyB,Creator)
            { this->CreateConstraint(ProxyA,ProxyB,AxisA,AxisB); }

        GearConstraint::GearConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& AxisA, const Vector3& AxisB, const Real Ratio, PhysicsManager* Creator) :
            Constraint(ID,ProxyA,ProxyB,Creator)
            { this->Gear = new btGearConstraint(*(this->ProxA->_GetPhysicsObject()), *(this->ProxB->_GetPhysicsObject()), AxisA.GetBulletVector3(), AxisB.GetBulletVector3(), Ratio); }

        GearConstraint::GearConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator) :
            Constraint(0,NULL,Creator)
            { this->ProtoDeSerialize(SelfRoot); }

        GearConstraint::~GearConstraint()
            { this->DestroyConstraint(); }

        void GearConstraint::CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Vector3& AxisA, const Vector3& AxisB)
        {
            if( this->Gear == NULL ) {
                this->Gear = new btGearConstraint(*(RigidA->_GetPhysicsObject()), *(RigidB->_GetPhysicsObject()), AxisA.GetBulletVector3(), AxisB.GetBulletVector3(), 0);
            }
        }

        void GearConstraint::DestroyConstraint()
        {
            this->EnableConstraint(false);
            if( this->Gear != NULL ) {
                delete this->Gear;
                this->Gear = NULL;
            }
            this->ProxA = NULL;
            this->ProxB = NULL;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Axis configuration

        void GearConstraint::SetAxisA(const Vector3& Axis)
            { this->Gear->setAxisA(Axis.GetBulletVector3()); }

        void GearConstraint::SetAxisB(const Vector3& Axis)
            { this->Gear->setAxisB(Axis.GetBulletVector3()); }

        Vector3 GearConstraint::GetAxisA() const
            { return Vector3( this->Gear->getAxisA() ); }

        Vector3 GearConstraint::GetAxisB() const
            { return Vector3( this->Gear->getAxisB() ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Ratio Configuration

        void GearConstraint::SetRotationRatio(const Real Ratio)
            { this->Gear->setRatio(Ratio); }

        Real GearConstraint::GetRotationRatio() const
            { return this->Gear->getRatio(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Parameter Configuration

        Constraint::ParamList GearConstraint::GetValidParamsOnAxis(int Axis) const
        {
            Constraint::ParamList Results;
            return Results;
        }

        Constraint::AxisList GearConstraint::GetValidLinearAxes() const
        {
            Constraint::AxisList Results;
            return Results;
        }

        Constraint::AxisList GearConstraint::GetValidAngularAxes() const
        {
            Constraint::AxisList Results;
            return Results;
        }

        Boole GearConstraint::HasParamBeenSet(ConstraintParam Param, int Axis) const
        {
            return false;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void GearConstraint::ProtoSerializeInitData(XML::Node& SelfRoot) const
        {
            XML::Node InitDataNode = SelfRoot.AppendChild( GearConstraint::GetSerializableName() + "InitData" );

            if( InitDataNode.AppendAttribute("Version").SetValue("1") &&
                ( this->ProxA != NULL ? InitDataNode.AppendAttribute("ProxyA-ID").SetValue( this->ProxA->GetProxyID() ) : false ) &&
                ( this->ProxB != NULL ? InitDataNode.AppendAttribute("ProxyB-ID").SetValue( this->ProxB->GetProxyID() ) : false ) )
            {
                if( this->ProxA != NULL ) {
                    XML::Node AxisANode = InitDataNode.AppendChild("AxisA");
                    this->GetAxisA().ProtoSerialize( AxisANode );
                }
                if( this->ProxB != NULL ) {
                    XML::Node AxisBNode = InitDataNode.AppendChild("AxisB");
                    this->GetAxisB().ProtoSerialize( AxisBNode );
                }

                return;
            }else{
                SerializeError("Create XML Attribute Values",GearConstraint::GetSerializableName() + "InitData",true);
            }
        }

        void GearConstraint::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Constraint::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( GearConstraint::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("RotationRatio").SetValue( this->GetRotationRatio() ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",GearConstraint::GetSerializableName() + "Properties",true);
            }
        }

        void GearConstraint::ProtoDeSerializeInitData(const XML::Node& SelfRoot)
        {
            this->DestroyConstraint();

            XML::Attribute CurrAttrib;
            XML::Node InitDataNode = SelfRoot.GetChild( GearConstraint::GetSerializableName() + "InitData" );

            if( !InitDataNode.Empty() ) {
                if(InitDataNode.GetAttribute("Version").AsInt() == 1) {
                    Vector3 AxisA;
                    Vector3 AxisB;

                    CurrAttrib = InitDataNode.GetAttribute("ProxyA-ID");
                    if( !CurrAttrib.Empty() )
                        this->ProxA = static_cast<RigidProxy*>( this->Manager->GetProxyByID( CurrAttrib.AsUint() ) );

                    CurrAttrib = InitDataNode.GetAttribute("ProxyB-ID");
                    if( !CurrAttrib.Empty() )
                        this->ProxB = static_cast<RigidProxy*>( this->Manager->GetProxyByID( CurrAttrib.AsUint() ) );

                    XML::Node AxisANode = InitDataNode.GetChild("AxisA").GetFirstChild();
                    if( !AxisANode.Empty() ) {
                        AxisA.ProtoDeSerialize(AxisANode);
                    }

                    XML::Node AxisBNode = InitDataNode.GetChild("AxisB").GetFirstChild();
                    if( !AxisBNode.Empty() ) {
                        AxisB.ProtoDeSerialize(AxisBNode);
                    }

                    this->CreateConstraint(this->ProxA,this->ProxB,AxisA,AxisB);
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( GearConstraint::GetSerializableName() + "InitData" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,GearConstraint::GetSerializableName() + "InitData" + " was not found in the provided XML node, which was expected.");
            }
        }

        void GearConstraint::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Constraint::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( GearConstraint::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("RotationRatio");
                    if( !CurrAttrib.Empty() )
                        this->SetRotationRatio( CurrAttrib.AsReal() );
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( GearConstraint::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,GearConstraint::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String GearConstraint::GetDerivedSerializableName() const
            { return GearConstraint::GetSerializableName(); }

        String GearConstraint::GetSerializableName()
            { return "GearConstraint"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        btTypedConstraint* GearConstraint::_GetConstraintBase() const
            { return this->Gear; }
    }//Physics
}//Mezzanine

#endif
