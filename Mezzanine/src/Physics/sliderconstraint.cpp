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
#ifndef _physicssliderconstraint_cpp
#define _physicssliderconstraint_cpp

#include "Physics/sliderconstraint.h"
#include "Physics/rigidproxy.h"

#include "stringtool.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Physics
    {
        /////////////////////////////////////////
        // Slider Constraint Functions

        SliderConstraint::SliderConstraint(const UInt32 ID, RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB, PhysicsManager* Creator) :
            DualTransformConstraint(ID,ProxyA,ProxyB,Creator),
            Slider(NULL)
            { this->CreateConstraint(ProxyA,ProxyB,TransA,TransB); }

        SliderConstraint::SliderConstraint(const UInt32 ID, RigidProxy* ProxyA, const Transform& TransA, PhysicsManager* Creator) :
            DualTransformConstraint(ID,ProxyA,Creator),
            Slider(NULL)
            { this->CreateConstraint(ProxyA,NULL,TransA,Transform()); }

        SliderConstraint::SliderConstraint(const XML::Node& SelfRoot, PhysicsManager* Creator) :
            DualTransformConstraint(0,NULL,Creator),
            Slider(NULL)
            { this->ProtoDeSerialize(SelfRoot); }

        SliderConstraint::~SliderConstraint()
            { this->DestroyConstraint(); }

        void SliderConstraint::CreateConstraint(RigidProxy* RigidA, RigidProxy* RigidB, const Transform& TransA, const Transform& TransB)
        {
            if( this->Slider == NULL ) {
                if( RigidA && RigidB ) {
                    this->Slider = new btSliderConstraint(*(RigidA->_GetPhysicsObject()),*(RigidB->_GetPhysicsObject()),TransA.GetBulletTransform(),TransB.GetBulletTransform(),false);
                }else if( RigidA ) {
                    this->Slider = new btSliderConstraint(*(RigidA->_GetPhysicsObject()),TransA.GetBulletTransform(),false);
                }
            }
        }

        void SliderConstraint::DestroyConstraint()
        {
            this->EnableConstraint(false);
            if( this->Slider != NULL ) {
                delete this->Slider;
                this->Slider = NULL;
            }
            this->ProxA = NULL;
            this->ProxB = NULL;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Location and Rotation

        void SliderConstraint::SetPivotTransforms(const Transform& TransA, const Transform& TransB)
            { this->Slider->setFrames(TransA.GetBulletTransform(),TransB.GetBulletTransform()); }

        void SliderConstraint::SetPivotATransform(const Transform& TranA)
            { this->Slider->getFrameOffsetA() = TranA.GetBulletTransform(); }

        void SliderConstraint::SetPivotBTransform(const Transform& TranB)
            { this->Slider->getFrameOffsetB() = TranB.GetBulletTransform(); }

        Transform SliderConstraint::GetPivotATransform() const
            { return this->Slider->getFrameOffsetA(); }

        Transform SliderConstraint::GetPivotBTransform() const
            { return this->Slider->getFrameOffsetB(); }

        void SliderConstraint::SetPivotALocation(const Vector3& Location)
            { this->Slider->getFrameOffsetA().setOrigin(Location.GetBulletVector3()); }

        void SliderConstraint::SetPivotBLocation(const Vector3& Location)
            { this->Slider->getFrameOffsetB().setOrigin(Location.GetBulletVector3()); }

        Vector3 SliderConstraint::GetPivotALocation() const
            { return Vector3(this->Slider->getFrameOffsetA().getOrigin()); }

        Vector3 SliderConstraint::GetPivotBLocation() const
            { return Vector3(this->Slider->getFrameOffsetB().getOrigin()); }

        void SliderConstraint::SetPivotARotation(const Quaternion& Rotation)
            { this->Slider->getFrameOffsetA().setRotation(Rotation.GetBulletQuaternion()); }

        void SliderConstraint::SetPivotBRotation(const Quaternion& Rotation)
            { this->Slider->getFrameOffsetA().setRotation(Rotation.GetBulletQuaternion()); }

        Quaternion SliderConstraint::GetPivotARotation() const
            { return Quaternion(this->Slider->getFrameOffsetA().getRotation()); }

        Quaternion SliderConstraint::GetPivotBRotation() const
            { return Quaternion(this->Slider->getFrameOffsetB().getRotation()); }

        ////////////////////////////////////////////////////////////////////////////////
        // Utility

        void SliderConstraint::SetUseFrameOffset(const Boole FrameOffset)
            { this->Slider->setUseFrameOffset(FrameOffset); }

        Boole SliderConstraint::GetUseFrameOffset() const
            { return this->Slider->getUseFrameOffset(); }

        void SliderConstraint::SetUpperLinLimit(const Real UpperLimit)
            { this->Slider->setUpperLinLimit(UpperLimit); }

        Real SliderConstraint::GetUpperLinLimit() const
            { return this->Slider->getUpperLinLimit(); }

        void SliderConstraint::SetUpperAngLimit(const Real UpperLimit)
            { this->Slider->setUpperAngLimit(UpperLimit); }

        Real SliderConstraint::GetUpperAngLimit() const
            { return this->Slider->getUpperAngLimit(); }

        void SliderConstraint::SetLowerLinLimit(const Real LowerLimit)
            { this->Slider->setLowerLinLimit(LowerLimit); }

        Real SliderConstraint::GetLowerLinLimit() const
            { return this->Slider->getLowerLinLimit(); }

        void SliderConstraint::SetLowerAngLimit(const Real LowerLimit)
            { this->Slider->setLowerAngLimit(LowerLimit); }

        Real SliderConstraint::GetLowerAngLimit() const
            { return this->Slider->getLowerAngLimit(); }

        void SliderConstraint::SetPoweredLinMotor(const Boole OnOff)
            { this->Slider->setPoweredLinMotor(OnOff); }

        Boole SliderConstraint::GetPoweredLinMotor() const
            { return this->Slider->getPoweredLinMotor(); }

        void SliderConstraint::SetPoweredAngMotor(const Boole OnOff)
            { this->Slider->setPoweredAngMotor(OnOff); }

        Boole SliderConstraint::GetPoweredAngMotor() const
            { return this->Slider->getPoweredAngMotor(); }

        void SliderConstraint::SetTargetLinMotorVelocity(const Real TargetLinMotorVelocity)
            { this->Slider->setTargetLinMotorVelocity(TargetLinMotorVelocity); }

        Real SliderConstraint::GetTargetLinMotorVelocity() const
            { return this->Slider->getTargetLinMotorVelocity(); }

        void SliderConstraint::SetTargetAngMotorVelocity(const Real TargetAngMotorVelocity)
            { this->Slider->setTargetAngMotorVelocity(TargetAngMotorVelocity); }

        Real SliderConstraint::GetTargetAngMotorVelocity() const
            { return this->Slider->getTargetAngMotorVelocity(); }

        void SliderConstraint::SetMaxLinMotorForce(const Real MaxLinMotorForce)
            { this->Slider->setMaxLinMotorForce(MaxLinMotorForce); }

        Real SliderConstraint::GetMaxLinMotorForce() const
            { return this->Slider->getMaxLinMotorForce(); }

        void SliderConstraint::SetMaxAngMotorForce(const Real MaxAngMotorForce)
            { this->Slider->setMaxAngMotorForce(MaxAngMotorForce); }

        Real SliderConstraint::GetMaxAngMotorForce() const
            { return this->Slider->getMaxAngMotorForce(); }

        void SliderConstraint::SetSoftnessLimLin(const Real SoftnessLimLin)
            { this->Slider->setSoftnessLimLin(SoftnessLimLin); }

        Real SliderConstraint::GetSoftnessLimLin() const
            { return this->Slider->getSoftnessLimLin(); }

        void SliderConstraint::SetSoftnessLimAng(const Real SoftnessLimAng)
            { this->Slider->setSoftnessLimAng(SoftnessLimAng); }

        Real SliderConstraint::GetSoftnessLimAng() const
            { return this->Slider->getSoftnessLimAng(); }

        void SliderConstraint::SetSoftnessOrthoLin(const Real SoftnessOrthoLin)
            { this->Slider->setSoftnessOrthoLin(SoftnessOrthoLin); }

        Real SliderConstraint::GetSoftnessOrthoLin() const
            { return this->Slider->getSoftnessOrthoLin(); }

        void SliderConstraint::SetSoftnessOrthoAng(const Real SoftnessOrthoAng)
            { this->Slider->setSoftnessOrthoAng(SoftnessOrthoAng); }

        Real SliderConstraint::GetSoftnessOrthoAng() const
            { return this->Slider->getSoftnessOrthoAng(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Obsolete Methods - All methods listed in this subsection appears to alter deprecated/unused data

        void SliderConstraint::SetSoftnessDirLin(const Real SoftnessDirLin)
            { this->Slider->setSoftnessDirLin(SoftnessDirLin); }

        Real SliderConstraint::GetSoftnessDirLin() const
            { return this->Slider->getSoftnessDirLin(); }

        void SliderConstraint::SetSoftnessDirAng(const Real SoftnessDirAng)
            { this->Slider->setSoftnessDirAng(SoftnessDirAng); }

        Real SliderConstraint::GetSoftnessDirAng() const
            { return this->Slider->getSoftnessDirAng(); }

        void SliderConstraint::SetDampingDirLin(const Real DampingDirLin)
            { this->Slider->setDampingDirLin(DampingDirLin); }

        Real SliderConstraint::GetDampingDirLin() const
            { return this->Slider->getDampingDirLin(); }

        void SliderConstraint::SetDampingDirAng(const Real DampingDirAng)
            { this->Slider->setDampingDirAng(DampingDirAng); }

        Real SliderConstraint::GetDampingDirAng() const
            { return this->Slider->getDampingDirAng(); }

        void SliderConstraint::SetDampingLimLin(const Real DampingLimLin)
            { this->Slider->setDampingLimLin(DampingLimLin); }

        Real SliderConstraint::GetDampingLimLin() const
            { return this->Slider->getDampingLimLin(); }

        void SliderConstraint::SetDampingLimAng(const Real DampingLimAng)
            { this->Slider->setDampingLimAng(DampingLimAng); }

        Real SliderConstraint::GetDampingLimAng() const
            { return this->Slider->getDampingLimAng(); }

        void SliderConstraint::SetDampingOrthoLin(const Real DampingOrthoLin)
            { this->Slider->setDampingOrthoLin(DampingOrthoLin); }

        Real SliderConstraint::GetDampingOrthoLin() const
            { return this->Slider->getDampingOrthoLin(); }

        void SliderConstraint::SetDampingOrthoAng(const Real DampingOrthoAng)
            { this->Slider->setDampingOrthoAng(DampingOrthoAng); }

        Real SliderConstraint::GetDampingOrthoAng() const
            { return this->Slider->getDampingOrthoAng(); }

        void SliderConstraint::SetRestitutionDirLin(const Real RestitutionDirLin)
            { this->Slider->setRestitutionDirLin(RestitutionDirLin); }

        Real SliderConstraint::GetRestitutionDirLin() const
            { return this->Slider->getRestitutionDirLin(); }

        void SliderConstraint::SetRestitutionDirAng(const Real RestitutionDirAng)
            { this->Slider->setRestitutionDirAng(RestitutionDirAng); }

        Real SliderConstraint::GetRestitutionDirAng() const
            { return this->Slider->getRestitutionDirAng(); }

        void SliderConstraint::SetRestitutionLimLin(const Real RestitutionLimLin)
            { this->Slider->setRestitutionLimLin(RestitutionLimLin); }

        Real SliderConstraint::GetRestitutionLimLin() const
            { return this->Slider->getRestitutionLimLin(); }

        void SliderConstraint::SetRestitutionLimAng(const Real RestitutionLimAng)
            { this->Slider->setRestitutionLimAng(RestitutionLimAng); }

        Real SliderConstraint::GetRestitutionLimAng() const
            { return this->Slider->getRestitutionLimAng(); }

        void SliderConstraint::SetRestitutionOrthoLin(const Real RestitutionOrthoLin)
            { this->Slider->setRestitutionOrthoLin(RestitutionOrthoLin); }

        Real SliderConstraint::GetRestitutionOrthoLin() const
            { return this->Slider->getRestitutionOrthoLin(); }

        void SliderConstraint::SetRestitutionOrthoAng(const Real RestitutionOrthoAng)
            { this->Slider->setRestitutionOrthoAng(RestitutionOrthoAng); }

        Real SliderConstraint::GetRestitutionOrthoAng() const
            { return this->Slider->getRestitutionOrthoAng(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Parameter Configuration

        Constraint::ParamList SliderConstraint::GetValidParamsOnAxis(int Axis) const
        {
            Constraint::ParamList Results;
            if( 0 <= Axis && 5 >= Axis ) {
                Results.push_back(Con_Stop_ERP);
                Results.push_back(Con_CFM);
                Results.push_back(Con_Stop_CFM);
            }
            return Results;
        }

        Constraint::AxisList SliderConstraint::GetValidLinearAxes() const
        {
            Constraint::AxisList Results;
            Results.push_back(0);
            Results.push_back(1);
            Results.push_back(2);
            return Results;
        }

        Constraint::AxisList SliderConstraint::GetValidAngularAxes() const
        {
            Constraint::AxisList Results;
            Results.push_back(3);
            Results.push_back(4);
            Results.push_back(5);
            return Results;
        }

        Boole SliderConstraint::HasParamBeenSet(ConstraintParam Param, int Axis) const
        {
            // the logic here should match the logic in the source at http://bulletphysics.com/Bullet/BulletFull/btGeneric6DofConstraint_8cpp_source.html#l00964
            if(0>Axis || 5<Axis)
                { return false; }

            if( Con_Stop_ERP == Param )
            {
                if( this->Slider->getFlags() & BT_SLIDER_FLAGS_ERP_LIMLIN ) return true;
                if( this->Slider->getFlags() & BT_SLIDER_FLAGS_ERP_ORTLIN ) return true;
                if( this->Slider->getFlags() & BT_SLIDER_FLAGS_ERP_LIMANG ) return true;
                if( this->Slider->getFlags() & BT_SLIDER_FLAGS_ERP_ORTANG ) return true;
            }
            else if( Con_Stop_CFM == Param )
            {
                if( this->Slider->getFlags() & BT_SLIDER_FLAGS_CFM_DIRLIN ) return true;
                if( this->Slider->getFlags() & BT_SLIDER_FLAGS_CFM_DIRANG ) return true;
            }
            else if( Con_CFM == Param )
            {
                if( this->Slider->getFlags() & BT_SLIDER_FLAGS_CFM_LIMLIN ) return true;
                if( this->Slider->getFlags() & BT_SLIDER_FLAGS_CFM_ORTLIN ) return true;
                if( this->Slider->getFlags() & BT_SLIDER_FLAGS_CFM_LIMANG ) return true;
                if( this->Slider->getFlags() & BT_SLIDER_FLAGS_CFM_ORTANG ) return true;
            }
            return false;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void SliderConstraint::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Constraint::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( SliderConstraint::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("UseFrameOffset").SetValue( this->GetUseFrameOffset() ) &&
                PropertiesNode.AppendAttribute("UpperLinLimit").SetValue( this->GetUpperLinLimit() ) &&
                PropertiesNode.AppendAttribute("UpperAngLimit").SetValue( this->GetUpperAngLimit() ) &&
                PropertiesNode.AppendAttribute("LowerLinLimit").SetValue( this->GetLowerLinLimit() ) &&
                PropertiesNode.AppendAttribute("LowerAngLimit").SetValue( this->GetLowerAngLimit() ) &&
                PropertiesNode.AppendAttribute("PoweredLinMotor").SetValue( this->GetPoweredLinMotor() ) &&
                PropertiesNode.AppendAttribute("PoweredAngMotor").SetValue( this->GetPoweredAngMotor() ) &&
                PropertiesNode.AppendAttribute("TargetLinMotorVelocity").SetValue( this->GetTargetLinMotorVelocity() ) &&
                PropertiesNode.AppendAttribute("TargetAngMotorVelocity").SetValue( this->GetTargetAngMotorVelocity() ) &&
                PropertiesNode.AppendAttribute("MaxLinMotorForce").SetValue( this->GetMaxLinMotorForce() ) &&
                PropertiesNode.AppendAttribute("MaxAngMotorForce").SetValue( this->GetMaxAngMotorForce() ) &&
                PropertiesNode.AppendAttribute("SoftnessLimLin").SetValue( this->GetSoftnessLimLin() ) &&
                PropertiesNode.AppendAttribute("SoftnessLimAng").SetValue( this->GetSoftnessLimAng() ) &&
                PropertiesNode.AppendAttribute("SoftnessOrthoLin").SetValue( this->GetSoftnessOrthoLin() ) &&
                PropertiesNode.AppendAttribute("SoftnessOrthoAng").SetValue( this->GetSoftnessOrthoAng() ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",SliderConstraint::GetSerializableName() + "Properties",true);
            }
        }

        void SliderConstraint::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Constraint::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( SliderConstraint::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if( PropertiesNode.GetAttribute("Version").AsInt() == 1 ) {
                    CurrAttrib = PropertiesNode.GetAttribute("UseFrameOffset");
                    if( !CurrAttrib.Empty() )
                        this->SetUseFrameOffset( CurrAttrib.AsBool() );

                    CurrAttrib = PropertiesNode.GetAttribute("UpperLinLimit");
                    if( !CurrAttrib.Empty() )
                        this->SetUpperLinLimit( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("UpperAngLimit");
                    if( !CurrAttrib.Empty() )
                        this->SetUpperAngLimit( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("LowerLinLimit");
                    if( !CurrAttrib.Empty() )
                        this->SetLowerLinLimit( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("LowerAngLimit");
                    if( !CurrAttrib.Empty() )
                        this->SetLowerAngLimit( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("PoweredLinMotor");
                    if( !CurrAttrib.Empty() )
                        this->SetPoweredLinMotor( CurrAttrib.AsBool() );

                    CurrAttrib = PropertiesNode.GetAttribute("PoweredAngMotor");
                    if( !CurrAttrib.Empty() )
                        this->SetPoweredAngMotor( CurrAttrib.AsBool() );

                    CurrAttrib = PropertiesNode.GetAttribute("TargetLinMotorVelocity");
                    if( !CurrAttrib.Empty() )
                        this->SetTargetLinMotorVelocity( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("TargetAngMotorVelocity");
                    if( !CurrAttrib.Empty() )
                        this->SetTargetAngMotorVelocity( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("MaxLinMotorForce");
                    if( !CurrAttrib.Empty() )
                        this->SetMaxLinMotorForce( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("MaxAngMotorForce");
                    if( !CurrAttrib.Empty() )
                        this->SetMaxAngMotorForce( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("SoftnessLimLin");
                    if( !CurrAttrib.Empty() )
                        this->SetSoftnessLimLin( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("SoftnessLimAng");
                    if( !CurrAttrib.Empty() )
                        this->SetSoftnessLimAng( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("SoftnessOrthoLin");
                    if( !CurrAttrib.Empty() )
                        this->SetSoftnessOrthoLin( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("SoftnessOrthoAng");
                    if( !CurrAttrib.Empty() )
                        this->SetSoftnessOrthoAng( CurrAttrib.AsReal() );
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( SliderConstraint::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,SliderConstraint::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String SliderConstraint::GetDerivedSerializableName() const
            { return SliderConstraint::GetSerializableName(); }

        String SliderConstraint::GetSerializableName()
            { return "SliderConstraint"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal

        btTypedConstraint* SliderConstraint::_GetConstraintBase() const
            { return this->Slider; }
    }//Physics
}//Mezzanine

#endif
