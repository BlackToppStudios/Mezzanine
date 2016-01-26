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
#ifndef _physicsgeneric6dofspringconstraint_cpp
#define _physicsgeneric6dofspringconstraint_cpp

#include "Physics/generic6dofspringconstraint.h"
#include "Physics/rigidproxy.h"

#include "stringtool.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Physics
    {
        ////////////////////////////////////////////////////////////////////////////////
        // Generic6Dof Spring Constraint Functions

        Generic6DofSpringConstraint::Generic6DofSpringConstraint(const UInt32 ID, RigidProxy* Prox1, PhysicsManager* Creator) :
            Generic6DofConstraint(ID,Prox1,Creator)
            {  }

        Generic6DofSpringConstraint::Generic6DofSpringConstraint(const UInt32 ID, RigidProxy* Prox1, RigidProxy* Prox2, PhysicsManager* Creator) :
            Generic6DofConstraint(ID,Prox1,Prox2,Creator)
            {  }

        Generic6DofSpringConstraint::Generic6DofSpringConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB, PhysicsManager* Creator) :
            Generic6DofConstraint(ID,ProxyA,ProxyB,Creator)
            { this->CreateConstraint(ProxyA,ProxyB,TransA,TransB); }

        Generic6DofSpringConstraint::Generic6DofSpringConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator) :
            Generic6DofConstraint(0,NULL,Creator)
            { this->ProtoDeSerialize(SelfRoot); }

        Generic6DofSpringConstraint::~Generic6DofSpringConstraint()
            { /* Generic6Dof class will handle the cleanup */ }

        btGeneric6DofSpringConstraint* Generic6DofSpringConstraint::Generic6dofSpring() const
            { return static_cast<btGeneric6DofSpringConstraint*>( this->Generic6dof ); }

        void Generic6DofSpringConstraint::CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB)
        {
            if( this->Generic6dof ) {
                this->Generic6dof = new btGeneric6DofSpringConstraint(*(RigidA->_GetPhysicsObject()),*(RigidB->_GetPhysicsObject()),TransA.GetBulletTransform(),TransB.GetBulletTransform(),false);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Utility

        void Generic6DofSpringConstraint::CalculateSpringEquilibriumPoints()
            { this->Generic6dofSpring()->setEquilibriumPoint(); }

        ////////////////////////////////////////////////////////////////////////////////
        // Linear Spring Settings

        void Generic6DofSpringConstraint::SetLinearSpringStiffness(const Vector3& Stiffness)
            { this->SetSpringStiffness(LinearX, Stiffness.X); this->SetSpringStiffness(LinearY, Stiffness.Y); this->SetSpringStiffness(LinearZ, Stiffness.Z); }

        Vector3 Generic6DofSpringConstraint::GetLinearSpringStiffness() const
            { return Vector3(this->GetSpringStiffness(LinearX),this->GetSpringStiffness(LinearY),this->GetSpringStiffness(LinearZ)); }

        void Generic6DofSpringConstraint::SetLinearSpringStiffnessOnAxis(const Real Stiffness, Whole TranslationAxis)
            { this->Generic6dofSpring()->setStiffness( TranslationAxis, Stiffness ); }

        Real Generic6DofSpringConstraint::GetLinearSpringStiffnessOnAxis(Whole TranslationAxis) const
            { return this->Generic6dofSpring()->getStiffness( TranslationAxis ); }

        void Generic6DofSpringConstraint::SetLinearSpringDamping(const Vector3& Damps)
            { this->SetSpringDamping(LinearX, Damps.X); this->SetSpringDamping(LinearY, Damps.Y); this->SetSpringDamping(LinearZ, Damps.Z); }

        Vector3 Generic6DofSpringConstraint::GetLinearSpringDamping() const
            { return Vector3(this->GetSpringDamping(LinearX),this->GetSpringDamping(LinearY),this->GetSpringDamping(LinearZ)); }

        void Generic6DofSpringConstraint::SetLinearSpringDampingOnAxis(const Real Damping, Whole TranslationAxis)
            { this->Generic6dofSpring()->setDamping( TranslationAxis, Damping ); }

        Real Generic6DofSpringConstraint::GetLinearSpringDampingOnAxis(Whole TranslationAxis) const
            { return this->Generic6dofSpring()->getDamping( TranslationAxis ); }

        void Generic6DofSpringConstraint::SetLinearSpringEnabled(const Vector3& Enableness)
            { this->SetSpringEnabled(LinearX, Enableness.X); this->SetSpringEnabled(LinearY, Enableness.Y); this->SetSpringEnabled(LinearZ, Enableness.Z); }

        Vector3 Generic6DofSpringConstraint::GetLinearSpringEnabled() const
            { return Vector3(this->GetSpringEnabled(LinearX),this->GetSpringEnabled(LinearY),this->GetSpringEnabled(LinearZ)); }

        void Generic6DofSpringConstraint::SetLinearSpringEnabledOnAxis(const Boole Enabled, Whole TranslationAxis)
            { this->Generic6dofSpring()->enableSpring( TranslationAxis, Enabled ); }

        Boole Generic6DofSpringConstraint::GetLinearSpringEnabledOnAxis(Whole TranslationAxis) const
            { return this->Generic6dofSpring()->isSpringEnabled( TranslationAxis ); }

        Vector3 Generic6DofSpringConstraint::GetLinearSpringEquilibriumPoints() const
            { return Vector3(this->GetSpringEquilibriumPoint(LinearX),this->GetSpringEquilibriumPoint(LinearY),this->GetSpringEquilibriumPoint(LinearZ)); }

        ////////////////////////////////////////////////////////////////////////////////
        // Angular Spring Settings

        void Generic6DofSpringConstraint::SetAngularSpringStiffness(const Vector3& Stiffness)
            { this->SetSpringStiffness(AngularX, Stiffness.X); this->SetSpringStiffness(AngularY, Stiffness.Y); this->SetSpringStiffness(AngularZ, Stiffness.Z); }

        Vector3 Generic6DofSpringConstraint::GetAngularSpringStiffness() const
            { return Vector3(this->GetSpringStiffness(AngularX),this->GetSpringStiffness(AngularY),this->GetSpringStiffness(AngularZ)); }

        void Generic6DofSpringConstraint::SetAngularSpringStiffnessOnAxis(const Real Stiffness, Whole RotationAxis)
            { this->Generic6dofSpring()->setStiffness( ConvertAngularTo6Axis(RotationAxis), Stiffness ); }

        Real Generic6DofSpringConstraint::GetAngularSpringStiffnessOnAxis(Whole RotationAxis) const
            { return this->Generic6dofSpring()->getStiffness( ConvertAngularTo6Axis(RotationAxis) ); }

        void Generic6DofSpringConstraint::SetAngularSpringDamping(const Vector3& Damps)
            { this->SetSpringDamping(AngularX, Damps.X); this->SetSpringDamping(AngularY, Damps.Y); this->SetSpringDamping(AngularZ, Damps.Z); }

        Vector3 Generic6DofSpringConstraint::GetAngularSpringDamping() const
            { return Vector3(this->GetSpringDamping(AngularX),this->GetSpringDamping(AngularY),this->GetSpringDamping(AngularZ)); }

        void Generic6DofSpringConstraint::SetAngularSpringDampingOnAxis(const Real Damping, Whole RotationAxis)
            { this->Generic6dofSpring()->setDamping( ConvertAngularTo6Axis(RotationAxis), Damping ); }

        Real Generic6DofSpringConstraint::GetAngularSpringDampingOnAxis(Whole RotationAxis) const
            { return this->Generic6dofSpring()->getDamping( ConvertAngularTo6Axis(RotationAxis) ); }

        void Generic6DofSpringConstraint::SetAngularSpringEnabled(const Vector3& Enableness)
            { this->SetSpringEnabled(AngularX, Enableness.X); this->SetSpringEnabled(AngularY, Enableness.Y); this->SetSpringEnabled(AngularZ, Enableness.Z); }

        Vector3 Generic6DofSpringConstraint::GetAngularSpringEnabled() const
            { return Vector3(this->GetSpringEnabled(AngularX),this->GetSpringEnabled(AngularY),this->GetSpringEnabled(AngularZ)); }

        void Generic6DofSpringConstraint::SetAngularSpringEnabledOnAxis(const Boole Enabled, Whole RotationAxis)
            { this->Generic6dofSpring()->enableSpring( ConvertAngularTo6Axis(RotationAxis), Enabled ); }

        Boole Generic6DofSpringConstraint::GetAngularSpringEnabledOnAxis(Whole RotationAxis) const
            { return this->Generic6dofSpring()->isSpringEnabled( ConvertAngularTo6Axis(RotationAxis) ); }

        Vector3 Generic6DofSpringConstraint::GetAngularSpringEquilibriumPoints() const
            { return Vector3(this->GetSpringEquilibriumPoint(AngularX),this->GetSpringEquilibriumPoint(AngularY),this->GetSpringEquilibriumPoint(AngularZ)); }

        ////////////////////////////////////////////////////////////////////////////////
        // Combined Linear and Angular Axis Spring Settings

        void Generic6DofSpringConstraint::SetSpringStiffness(int Index, Real Stiffness)
            { this->Generic6dofSpring()->setStiffness(Index, Stiffness); }

        Real Generic6DofSpringConstraint::GetSpringStiffness(int Index) const
            { return this->Generic6dofSpring()->getStiffness(Index); }

        void Generic6DofSpringConstraint::SetSpringDamping(int Index, Real Damping)
            { this->Generic6dofSpring()->setDamping(Index, Damping); }

        Real Generic6DofSpringConstraint::GetSpringDamping(int Index) const
            { return this->Generic6dofSpring()->getDamping(Index); }

        void Generic6DofSpringConstraint::SetSpringEnabled(int Index, Boole Enable)
            { this->Generic6dofSpring()->enableSpring(Index, Enable); }

        Boole Generic6DofSpringConstraint::GetSpringEnabled(int Index) const
            { return this->Generic6dofSpring()->isSpringEnabled(Index); }

        void Generic6DofSpringConstraint::CalculateSpringEquilibriumPoint(int Index)
            { this->Generic6dofSpring()->setEquilibriumPoint(Index); }

        void Generic6DofSpringConstraint::SetSpringEquilibriumPoint(int Index, const Real Point)
            { this->Generic6dofSpring()->setEquilibriumPoint(Index,Point); }

        Real Generic6DofSpringConstraint::GetSpringEquilibriumPoint(int Index) const
            { return this->Generic6dofSpring()->getEquilibriumPoint(Index); }

        ////////////////////////////////////////////////////////////////////////////////
        // Serialization

        void Generic6DofSpringConstraint::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Generic6DofConstraint::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( Generic6DofSpringConstraint::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") ) {
                XML::Node LinearAxesNode = PropertiesNode.AppendChild("LinearAxes");
                for( Whole LinAxis = Generic6DofConstraint::LinearStart ; LinAxis < Generic6DofConstraint::LinearEnd ; ++LinAxis )
                {
                    XML::Node LinearAxisNode = LinearAxesNode.AppendChild("LinearAxis");
                    if( LinearAxisNode.AppendAttribute("Version").SetValue("1") &&
                        LinearAxisNode.AppendAttribute("Axis").SetValue( LinAxis ) &&
                        LinearAxisNode.AppendAttribute("SpringStiffness").SetValue( this->GetLinearSpringStiffnessOnAxis( LinAxis ) ) &&
                        LinearAxisNode.AppendAttribute("SpringDamping").SetValue( this->GetLinearSpringDampingOnAxis( LinAxis ) ) &&
                        LinearAxisNode.AppendAttribute("SpringEnabled").SetValue( this->GetLinearSpringEnabledOnAxis( LinAxis ) ) )
                    {
                        continue;
                    }else{
                        SerializeError("Create XML Attribute Values",Generic6DofSpringConstraint::GetSerializableName() + " Linear Axes",true);
                    }
                }

                XML::Node AngularAxesNode = PropertiesNode.AppendChild("AngularAxes");
                for( Whole AngAxis = Generic6DofConstraint::AngularStart ; AngAxis < Generic6DofConstraint::AngularEnd ; ++AngAxis )
                {
                    XML::Node AngularAxisNode = AngularAxesNode.AppendChild("AngularAxis");
                    if( AngularAxisNode.AppendAttribute("Version").SetValue("1") &&
                        AngularAxisNode.AppendAttribute("Axis").SetValue( AngAxis ) &&
                        AngularAxisNode.AppendAttribute("SpringStiffness").SetValue( this->GetAngularSpringStiffnessOnAxis( AngAxis ) ) &&
                        AngularAxisNode.AppendAttribute("SpringDamping").SetValue( this->GetAngularSpringDampingOnAxis( AngAxis ) ) &&
                        AngularAxisNode.AppendAttribute("SpringEnabled").SetValue( this->GetAngularSpringEnabledOnAxis( AngAxis ) ) )
                    {
                        continue;
                    }else{
                        SerializeError("Create XML Attribute Values",Generic6DofSpringConstraint::GetSerializableName() + " Angular Axes",true);
                    }
                }

                return;
            }else{
                SerializeError("Create XML Attribute Values",Generic6DofSpringConstraint::GetSerializableName() + "Properties",true);
            }
        }

        void Generic6DofSpringConstraint::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Generic6DofConstraint::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( Generic6DofSpringConstraint::GetSerializableName() + "Properties" );

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

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("SpringStiffness");
                                if( !CurrAttrib.Empty() )
                                    this->SetLinearSpringStiffnessOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("SpringDamping");
                                if( !CurrAttrib.Empty() )
                                    this->SetLinearSpringDampingOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*LinearAxesNodeIt).GetAttribute("SpringEnabled");
                                if( !CurrAttrib.Empty() )
                                    this->SetLinearSpringEnabledOnAxis( CurrAttrib.AsBool(), Axis );
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

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("SpringStiffness");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularSpringStiffnessOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("SpringDamping");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularSpringDampingOnAxis( CurrAttrib.AsReal(), Axis );

                                CurrAttrib = (*AngularAxesNodeIt).GetAttribute("SpringEnabled");
                                if( !CurrAttrib.Empty() )
                                    this->SetAngularSpringEnabledOnAxis( CurrAttrib.AsBool(), Axis );
                            }
                        }
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( Generic6DofSpringConstraint::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,Generic6DofSpringConstraint::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String Generic6DofSpringConstraint::GetDerivedSerializableName() const
            { return Generic6DofSpringConstraint::GetSerializableName(); }

        String Generic6DofSpringConstraint::GetSerializableName()
            { return "Generic6DofSpringConstraint"; }
    }//Physics
}//Mezzanine

#endif
