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
#ifndef _physicspoint2pointconstraint_cpp
#define _physicspoint2pointconstraint_cpp

#include "Physics/point2pointconstraint.h"
#include "Physics/physicsmanager.h"
#include "Physics/rigidproxy.h"

#include "stringtool.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Physics
    {
        ////////////////////////////////////////////////////////////////////////////////
        // Point2PointConstraint Methods

        Point2PointConstraint::Point2PointConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& PivotA, const Vector3& PivotB, PhysicsManager* Creator) :
            Constraint(ID,ProxyA,ProxyB,Creator),
            Point2Point(NULL)
            { this->CreateConstraint(ProxyA,ProxyB,PivotA,PivotB); }

        Point2PointConstraint::Point2PointConstraint(const UInt32 ID, RigidProxy* ProxyA, const Vector3& PivotA, PhysicsManager* Creator) :
            Constraint(ID,ProxyA,Creator),
            Point2Point(NULL)
            { this->CreateConstraint(ProxyA,NULL,PivotA,Vector3()); }

        Point2PointConstraint::Point2PointConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator) :
            Constraint(0,NULL,Creator),
            Point2Point(NULL)
            { this->ProtoDeSerialize(SelfRoot); }

        Point2PointConstraint::~Point2PointConstraint()
            { this->DestroyConstraint(); }

        void Point2PointConstraint::CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Vector3& PivotA, const Vector3& PivotB)
        {
            if( this->Point2Point == NULL ) {
                if( RigidA && RigidB ) {
                    this->Point2Point = new btPoint2PointConstraint(*(RigidA->_GetPhysicsObject()),*(RigidB->_GetPhysicsObject()),PivotA.GetBulletVector3(),PivotB.GetBulletVector3());
                }else if( RigidA ) {
                    this->Point2Point = new btPoint2PointConstraint(*(RigidA->_GetPhysicsObject()),PivotA.GetBulletVector3());
                }
            }
        }

        void Point2PointConstraint::DestroyConstraint()
        {
            this->EnableConstraint(false);
            if( this->Point2Point != NULL ) {
                delete this->Point2Point;
                this->Point2Point = NULL;
            }
            this->ProxA = NULL;
            this->ProxB = NULL;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Position and Orientation

        void Point2PointConstraint::SetPivotA(const Vector3& PivotA)
            { this->Point2Point->setPivotA(PivotA.GetBulletVector3()); }

        void Point2PointConstraint::SetPivotB(const Vector3& PivotB)
            { this->Point2Point->setPivotB(PivotB.GetBulletVector3()); }

        Vector3 Point2PointConstraint::GetPivotA() const
            { return Vector3(this->Point2Point->getPivotInA()); }

        Vector3 Point2PointConstraint::GetPivotB() const
            { return Vector3(this->Point2Point->getPivotInB()); }

        ////////////////////////////////////////////////////////////////////////////////
        // Specific Physics Settings

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

        ///////////////////////////////////////////////////////////////////////////////
        // Parameter Configuration

        Constraint::ParamList Point2PointConstraint::GetValidParamsOnAxis(int Axis) const
        {
            Constraint::ParamList Results;
            if( -1 == Axis ) {
                Results.push_back(Con_ERP);
                Results.push_back(Con_Stop_ERP);
                Results.push_back(Con_CFM);
                Results.push_back(Con_Stop_CFM);
            }
            return Results;
        }

        Constraint::AxisList Point2PointConstraint::GetValidLinearAxes() const
        {
            Constraint::AxisList Results;
            Results.push_back(-1);
            return Results;
        }

        Constraint::AxisList Point2PointConstraint::GetValidAngularAxes() const
        {
            Constraint::AxisList Results;
            return Results;
        }

        Boole Point2PointConstraint::HasParamBeenSet(ConstraintParam Param, int Axis) const
        {
            //return this->Point2Point->hasParamBeenSet(Param,Axis);
            // the logic here should match the logic in the source at http://bulletphysics.com/Bullet/BulletFull/btPoint2PointConstraint_8cpp_source.html#l00202
            if( -1 != Axis ) {
                return false;
            }

            return ( ( Con_ERP == Param || Con_Stop_ERP == Param ) && this->Point2Point->getFlags() & BT_P2P_FLAGS_ERP ) ||   //If we are checking erp OR we are checking stoperp AND the erp Flag is set OR
                   ( ( Con_CFM == Param || Con_Stop_CFM == Param ) && this->Point2Point->getFlags() & BT_P2P_FLAGS_CFM );     //   we are checking cfm OR we are checking stopcfm AND the cfm Flag is set
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void Point2PointConstraint::ProtoSerializeInitData(XML::Node& SelfRoot) const
        {
            XML::Node InitDataNode = SelfRoot.AppendChild( Point2PointConstraint::GetSerializableName() + "InitData" );

            if( InitDataNode.AppendAttribute("Version").SetValue("1") &&
                ( this->ProxA != NULL ? InitDataNode.AppendAttribute("ProxyA-ID").SetValue( this->ProxA->GetProxyID() ) : false ) &&
                ( this->ProxB != NULL ? InitDataNode.AppendAttribute("ProxyB-ID").SetValue( this->ProxB->GetProxyID() ) : false ) )
            {
                if( this->ProxA != NULL ) {
                    XML::Node PivotANode = InitDataNode.AppendChild("PivotA");
                    this->GetPivotA().ProtoSerialize( PivotANode );
                }
                if( this->ProxB != NULL ) {
                    XML::Node PivotBNode = InitDataNode.AppendChild("PivotB");
                    this->GetPivotB().ProtoSerialize( PivotBNode );
                }

                return;
            }else{
                SerializeError("Create XML Attribute Values",Point2PointConstraint::GetSerializableName() + "InitData",true);
            }
        }

        void Point2PointConstraint::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Constraint::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( Point2PointConstraint::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("ImpulseClamping").SetValue( this->GetImpulseClamping() ) &&
                PropertiesNode.AppendAttribute("Damping").SetValue( this->GetDamping() ) &&
                PropertiesNode.AppendAttribute("TAU").SetValue( this->GetTAU() ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",Point2PointConstraint::GetSerializableName() + "Properties",true);
            }
        }

        void Point2PointConstraint::ProtoDeSerializeInitData(const XML::Node& SelfRoot)
        {
            this->DestroyConstraint();

            XML::Attribute CurrAttrib;
            XML::Node InitDataNode = SelfRoot.GetChild( Point2PointConstraint::GetSerializableName() + "InitData" );

            if( !InitDataNode.Empty() ) {
                if(InitDataNode.GetAttribute("Version").AsInt() == 1) {
                    Vector3 PivotA;
                    Vector3 PivotB;

                    CurrAttrib = InitDataNode.GetAttribute("ProxyA-ID");
                    if( !CurrAttrib.Empty() )
                        this->ProxA = static_cast<RigidProxy*>( this->Manager->GetProxyByID( CurrAttrib.AsUint() ) );

                    CurrAttrib = InitDataNode.GetAttribute("ProxyB-ID");
                    if( !CurrAttrib.Empty() )
                        this->ProxB = static_cast<RigidProxy*>( this->Manager->GetProxyByID( CurrAttrib.AsUint() ) );

                    XML::Node PivotANode = InitDataNode.GetChild("PivotA").GetFirstChild();
                    if( !PivotANode.Empty() ) {
                        PivotA.ProtoDeSerialize(PivotANode);
                    }

                    XML::Node PivotBNode = InitDataNode.GetChild("PivotB").GetFirstChild();
                    if( !PivotBNode.Empty() ) {
                        PivotB.ProtoDeSerialize(PivotBNode);
                    }

                    this->CreateConstraint(this->ProxA,this->ProxB,PivotA,PivotB);
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( Point2PointConstraint::GetSerializableName() + "InitData" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,Point2PointConstraint::GetSerializableName() + "InitData" + " was not found in the provided XML node, which was expected.");
            }
        }

        void Point2PointConstraint::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Constraint::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( Point2PointConstraint::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("ImpulseClamping");
                    if( !CurrAttrib.Empty() )
                        this->SetImpulseClamping( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("Damping");
                    if( !CurrAttrib.Empty() )
                        this->SetDamping( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("TAU");
                    if( !CurrAttrib.Empty() )
                        this->SetTAU( CurrAttrib.AsReal() );
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( Point2PointConstraint::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,Point2PointConstraint::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String Point2PointConstraint::GetDerivedSerializableName() const
            { return Point2PointConstraint::GetSerializableName(); }

        String Point2PointConstraint::GetSerializableName()
            { return "Point2PointConstraint"; }

        ////////////////////////////////////////////////////////////////////////////////
        // Internal

        btTypedConstraint* Point2PointConstraint::_GetConstraintBase() const
            { return this->Point2Point; }
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
