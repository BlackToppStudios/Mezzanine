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
#ifndef _physicssliderconstraint_cpp
#define _physicssliderconstraint_cpp

#include "sliderconstraint.h"
#include "stringtool.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Physics
    {
        /////////////////////////////////////////
        // Slider Constraint Functions

        SliderConstraint::SliderConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA, const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseLinearReferenceA)
        {
            SetBodies(ActorA,ActorB);

            btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
            btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
            Slider = new btSliderConstraint(*BodyA, *BodyB, transa, transb, UseLinearReferenceA);
        }

        SliderConstraint::SliderConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Transform& TransformA, const Transform& TransformB, bool UseLinearReferenceA)
        {
            SetBodies(ActorA,ActorB);
            Slider = new btSliderConstraint(*BodyA, *BodyB, TransformA.GetBulletTransform(), TransformB.GetBulletTransform(), UseLinearReferenceA);
        }

        SliderConstraint::SliderConstraint(ActorRigid* ActorB, const Vector3& VectorB, const Quaternion& QuaternionB, bool UseLinearReferenceA)
        {
            SetBodies(ActorB);

            btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
            Slider = new btSliderConstraint(*BodyA, transb, UseLinearReferenceA);
        }

        SliderConstraint::~SliderConstraint()
        {
            delete Slider;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Generic6DofConstraint Location and Rotation
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

        void SliderConstraint::SetUpperLinLimit(const Real& UpperLimit)
        {
            this->Slider->setUpperLinLimit(UpperLimit);
        }

        void SliderConstraint::SetUpperAngLimit(const Real& UpperLimit)
        {
            this->Slider->setUpperAngLimit(UpperLimit);
        }

        void SliderConstraint::SetLowerLinLimit(const Real& LowerLimit)
        {
            this->Slider->setLowerLinLimit(LowerLimit);
        }

        void SliderConstraint::SetLowerAngLimit(const Real& LowerLimit)
        {
            this->Slider->setLowerAngLimit(LowerLimit);
        }

        void SliderConstraint::SetPoweredLinMotor(bool OnOff)
        {
            this->Slider->setPoweredLinMotor(OnOff);
        }

        void SliderConstraint::SetTargetLinMotorVelocity(const Real& TargetLinMotorVelocity)
        {
            this->Slider->setTargetLinMotorVelocity(TargetLinMotorVelocity);
        }

        void SliderConstraint::SetMaxLinMotorForce(const Real& MaxLinMotorForce)
        {
            this->Slider->setMaxLinMotorForce(MaxLinMotorForce);
        }

        void SliderConstraint::SetPoweredAngMotor(bool OnOff)
        {
            this->Slider->setPoweredAngMotor(OnOff);
        }

        void SliderConstraint::SetTargetAngMotorVelocity(const Real& TargetAngMotorVelocity)
        {
            this->Slider->setTargetAngMotorVelocity(TargetAngMotorVelocity);
        }

        void SliderConstraint::SetMaxAngMotorForce(const Real& MaxAngMotorForce)
        {
            this->Slider->setMaxAngMotorForce(MaxAngMotorForce);
        }

        void SliderConstraint::SetUseFrameOffset(bool FrameOffset)
        {
            this->Slider->setUseFrameOffset(FrameOffset);
        }

        void SliderConstraint::SetSoftnessDirLin(const Real& SoftnessDirLin)
        {
            this->Slider->setSoftnessDirLin(SoftnessDirLin);
        }

        void SliderConstraint::SetRestitutionDirLin(const Real& RestitutionDirLin)
        {
            this->Slider->setRestitutionDirLin(RestitutionDirLin);
        }

        void SliderConstraint::SetDampingDirLin(const Real& DampingDirLin)
        {
            this->Slider->setDampingDirLin(DampingDirLin);
        }

        void SliderConstraint::SetSoftnessDirAng(const Real& SoftnessDirAng)
        {
            this->Slider->setSoftnessDirAng(SoftnessDirAng);
        }

        void SliderConstraint::SetRestitutionDirAng(const Real& RestitutionDirAng)
        {
            this->Slider->setRestitutionDirAng(RestitutionDirAng);
        }

        void SliderConstraint::SetDampingDirAng(const Real& DampingDirAng)
        {
            this->Slider->setDampingDirAng(DampingDirAng);
        }

        void SliderConstraint::SetSoftnessLimLin(const Real& SoftnessLimLin)
        {
            this->Slider->setSoftnessLimLin(SoftnessLimLin);
        }

        void SliderConstraint::SetRestitutionLimLin(const Real& RestitutionLimLin)
        {
            this->Slider->setRestitutionLimLin(RestitutionLimLin);
        }

        void SliderConstraint::SetDampingLimLin(const Real& DampingLimLin)
        {
            this->Slider->setDampingLimLin(DampingLimLin);
        }

        void SliderConstraint::SetSoftnessLimAng(const Real& SoftnessLimAng)
        {
            this->Slider->setSoftnessLimAng(SoftnessLimAng);
        }

        void SliderConstraint::SetRestitutionLimAng(const Real& RestitutionLimAng)
        {
            this->Slider->setRestitutionLimAng(RestitutionLimAng);
        }

        void SliderConstraint::SetDampingLimAng(const Real& DampingLimAng)
        {
            this->Slider->setDampingLimAng(DampingLimAng);
        }

        void SliderConstraint::SetSoftnessOrthoLin(const Real& SoftnessOrthoLin)
        {
            this->Slider->setSoftnessOrthoLin(SoftnessOrthoLin);
        }

        void SliderConstraint::SetRestitutionOrthoLin(const Real& RestitutionOrthoLin)
        {
            this->Slider->setRestitutionOrthoLin(RestitutionOrthoLin);
        }

        void SliderConstraint::SetDampingOrthoLin(const Real& DampingOrthoLin)
        {
            this->Slider->setDampingOrthoLin(DampingOrthoLin);
        }

        void SliderConstraint::SetSoftnessOrthoAng(const Real& SoftnessOrthoAng)
        {
            this->Slider->setSoftnessOrthoAng(SoftnessOrthoAng);
        }

        void SliderConstraint::SetRestitutionOrthoAng(const Real& RestitutionOrthoAng)
        {
            this->Slider->setRestitutionOrthoAng(RestitutionOrthoAng);
        }

        void SliderConstraint::SetDampingOrthoAng(const Real& DampingOrthoAng)
        {
            this->Slider->setDampingOrthoAng(DampingOrthoAng);
        }

        ////////////////////////////////////////////////////////////////////////////////
        // SliderConstraint Axis, Params and other Details
        Constraint::ParamList SliderConstraint::ValidParamOnAxis(int Axis) const
        {
            Constraint::ParamList Results;
            if(0<=Axis && 5>=Axis)
            {
                Results.push_back(Con_Stop_ERP);
                Results.push_back(Con_CFM);
                Results.push_back(Con_Stop_CFM);
            }
            return Results;
        }

        Constraint::AxisList SliderConstraint::ValidLinearAxis() const
        {
            Constraint::AxisList Results;
            Results.push_back(0);
            Results.push_back(1);
            Results.push_back(2);
            return Results;
        }

        Constraint::AxisList SliderConstraint::ValidAngularAxis() const
        {
            Constraint::AxisList Results;
            Results.push_back(3);
            Results.push_back(4);
            Results.push_back(5);
            return Results;
        }

        bool SliderConstraint::HasParamBeenSet(ConstraintParam Param, int Axis) const
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

        btTypedConstraint* SliderConstraint::GetConstraintBase() const
            { return this->Slider; }
    }//Physics
}//Mezzanine

#endif
