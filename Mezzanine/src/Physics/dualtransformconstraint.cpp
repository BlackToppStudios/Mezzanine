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
#ifndef _physicsdualtransformconstraint_cpp
#define _physicsdualtransformconstraint_cpp

#include "Physics/dualtransformconstraint.h"
#include "Physics/physicsmanager.h"
#include "Physics/rigidproxy.h"

#include "stringtool.h"
#include "serialization.h"

namespace Mezzanine
{
    namespace Physics
    {
        DualTransformConstraint::DualTransformConstraint(const UInt32 ID, RigidProxy* Prox1, PhysicsManager* Creator) :
            Constraint(ID,Prox1,Creator)
            {  }

        DualTransformConstraint::DualTransformConstraint(const UInt32 ID, RigidProxy* Prox1, RigidProxy* Prox2, PhysicsManager* Creator) :
            Constraint(ID,Prox1,Prox2,Creator)
            {  }

        DualTransformConstraint::~DualTransformConstraint()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Transform Methods

        void DualTransformConstraint::SetPivotTransforms(const Transform& TransA, const Transform& TransB)
            { this->SetPivotATransform( TransA );  this->SetPivotBTransform( TransB ); }

        void DualTransformConstraint::SetPivotALocation(const Vector3& Location)
            { this->SetPivotATransform( Transform(Location,this->GetPivotARotation()) ); }

        void DualTransformConstraint::SetPivotBLocation(const Vector3& Location)
            { this->SetPivotBTransform( Transform(Location,this->GetPivotBRotation()) ); }

        Vector3 DualTransformConstraint::GetPivotALocation() const
            { return this->GetPivotATransform().Location; }

        Vector3 DualTransformConstraint::GetPivotBLocation() const
            { return this->GetPivotBTransform().Location; }

        void DualTransformConstraint::SetPivotARotation(const Quaternion& Rotation)
            { this->SetPivotATransform( Transform(this->GetPivotALocation(),Rotation) ); }

        void DualTransformConstraint::SetPivotBRotation(const Quaternion& Rotation)
            { this->SetPivotBTransform( Transform(this->GetPivotBLocation(),Rotation) ); }

        Quaternion DualTransformConstraint::GetPivotARotation() const
            { return this->GetPivotATransform().Rotation; }

        Quaternion DualTransformConstraint::GetPivotBRotation() const
            { return this->GetPivotBTransform().Rotation; }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void DualTransformConstraint::ProtoSerializeInitData(XML::Node& SelfRoot) const
        {
            XML::Node InitDataNode = SelfRoot.AppendChild( DualTransformConstraint::GetSerializableName() + "InitData" );

            if( InitDataNode.AppendAttribute("Version").SetValue("1") &&
                ( this->ProxA != NULL ? InitDataNode.AppendAttribute("ProxyA-ID").SetValue( this->ProxA->GetComponentID() ) : false ) &&
                ( this->ProxB != NULL ? InitDataNode.AppendAttribute("ProxyB-ID").SetValue( this->ProxB->GetComponentID() ) : false ) )
            {
                if( this->ProxA != NULL ) {
                    XML::Node PivotATransNode = InitDataNode.AppendChild("PivotATransform");
                    this->GetPivotATransform().ProtoSerialize( PivotATransNode );
                }
                if( this->ProxB != NULL ) {
                    XML::Node PivotBTransNode = InitDataNode.AppendChild("PivotBTransform");
                    this->GetPivotBTransform().ProtoSerialize( PivotBTransNode );
                }

                return;
            }else{
                SerializeError("Create XML Attribute Values",DualTransformConstraint::GetSerializableName() + "InitData",true);
            }
        }

        void DualTransformConstraint::ProtoDeSerializeInitData(const XML::Node& SelfRoot)
        {
            this->DestroyConstraint();

            XML::Attribute CurrAttrib;
            XML::Node InitDataNode = SelfRoot.GetChild( DualTransformConstraint::GetSerializableName() + "InitData" );

            if( !InitDataNode.Empty() ) {
                if(InitDataNode.GetAttribute("Version").AsInt() == 1) {
                    Transform PivotA;
                    Transform PivotB;

                    CurrAttrib = InitDataNode.GetAttribute("ProxyA-ID");
                    if( !CurrAttrib.Empty() )
                        this->ProxA = static_cast<RigidProxy*>( this->Manager->GetProxyByID( CurrAttrib.AsUint() ) );

                    CurrAttrib = InitDataNode.GetAttribute("ProxyB-ID");
                    if( !CurrAttrib.Empty() )
                        this->ProxB = static_cast<RigidProxy*>( this->Manager->GetProxyByID( CurrAttrib.AsUint() ) );

                    XML::Node PivotANode = InitDataNode.GetChild("PivotATransform").GetFirstChild();
                    if( !PivotANode.Empty() ) {
                        PivotA.ProtoDeSerialize(PivotANode);
                    }

                    XML::Node PivotBNode = InitDataNode.GetChild("PivotBTransform").GetFirstChild();
                    if( !PivotBNode.Empty() ) {
                        PivotB.ProtoDeSerialize(PivotBNode);
                    }

                    this->CreateConstraint(this->ProxA,this->ProxB,PivotA,PivotB);
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( DualTransformConstraint::GetSerializableName() + "InitData" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,DualTransformConstraint::GetSerializableName() + "InitData" + " was not found in the provided XML node, which was expected.");
            }
        }

        String DualTransformConstraint::GetSerializableName()
            { return "DualTransformConstraint"; }
    }//Physics
}//Mezzanine

#endif
