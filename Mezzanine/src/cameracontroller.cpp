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
#ifndef _cameracontroller_cpp
#define _cameracontroller_cpp

#include "cameracontroller.h"

#include "enumerations.h"
#include "ray.h"
#include "entity.h"

#include "Graphics/cameraproxy.h"
#include "Graphics/scenemanager.h"
#include "MathTools/mathtools.h"

#include <assert.h>

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // AngleData Methods

    AngleData::AngleData() :
        CurrentAngle(0),
        UpperLimit(-1),
        LowerLimit(1)
        {  }

    void AngleData::SetLimits(const Real Upper, const Real Lower)
    {
        this->UpperLimit = Upper;
        this->LowerLimit = Lower;
    }

    void AngleData::UnsetLimits()
    {
        this->UpperLimit = -1.0;
        this->LowerLimit = 1.0;
    }

    Boole AngleData::HasEnforcableLimit() const
        { return this->UpperLimit >= this->LowerLimit; }

    void AngleData::SetAngle(Real Angle)
    {
        // Enforce Limits
        if( this->HasEnforcableLimit() ) {
            if( Angle > UpperLimit )
                Angle = UpperLimit;
            if( Angle < LowerLimit )
                Angle = LowerLimit;
        }
        // Check Rollover
        Real Pi = MathTools::GetPi();
        if( Angle > Pi ) {
            Angle = -Pi + (Angle - Pi);
        }else if( Angle < -Pi ) {
            Angle = Pi + (Angle + Pi);
        }
        this->CurrentAngle = Angle;
    }

    void AngleData::Rotate(const Real Angle)
        { this->SetAngle( this->CurrentAngle + Angle ); }

    ///////////////////////////////////////////////////////////////////////////////
    // CameraController Methods

    CameraController::CameraController() :
        RayCaster(NULL),
        Controlled(NULL),
        HoverHeight(2),
        CurrentMMode(CCM_Fly)
        {  }

    CameraController::CameraController(Graphics::CameraProxy* ToBeControlled) :
        RayCaster(static_cast<Graphics::SceneManager*>(ToBeControlled->GetCreator())),
        Controlled(ToBeControlled),
        HoverHeight(2),
        CurrentMMode(CCM_Fly)
        {  }

    CameraController::~CameraController()
        {  }

    void CameraController::CheckHeight()
    {
        Vector3 Loc(this->Controlled->GetLocation());
        Real Dist = this->FindDistanceToGround();
        if( 0 == Dist )
            return;
        Real DeltaDist = Dist - this->HoverHeight;
        Loc.Y -= DeltaDist;
        this->Controlled->SetLocation(Loc);
    }

    Real CameraController::FindDistanceToGround()
    {
        assert(this->Controlled != NULL && "Must have a valid camera pointer to work with the controller.");

        Ray GroundRay(this->Controlled->GetLocation(),Vector3::Neg_Unit_Y());
        this->RayCaster.SetFilterFunction(
            [](EntityProxy* Prox) {
                Entity* Ent = Prox->GetParentEntity();
                return ( Ent && ( Ent->GetType() & Mezzanine::ET_AllTerrains ) );
            }
        );
        RayQueryHit Result = this->RayCaster.GetFirstShapeResult(GroundRay);
        if( Result.IsValid() ) {
            return Result.Distance;
        }
        return 0;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void CameraController::SetControlledCamera(Graphics::CameraProxy* ToBeControlled)
    {
        this->Controlled = ToBeControlled;
        this->RayCaster.SetWorld( this->Controlled->GetCreator()->GetWorld() );
    }

    Graphics::CameraProxy* CameraController::GetControlledCamera() const
        { return this->Controlled; }

    ///////////////////////////////////////////////////////////////////////////////
    // CameraController Properties

    void CameraController::SetMovementMode(const CameraController::MovementMode& MoveMode)
        { this->CurrentMMode = MoveMode; }

    CameraController::MovementMode CameraController::GetMovementMode() const
        { return this->CurrentMMode; }

    void CameraController::SetHoverHeight(const Real& Hover)
        { this->HoverHeight = Hover; }

    Real CameraController::GetHoverHeight() const
        { return this->HoverHeight; }

    void CameraController::SetYawLimits(const Real& UpperLimit, const Real& LowerLimit)
        { this->YawData.SetLimits(UpperLimit,LowerLimit); }

    void CameraController::RemoveYawLimits()
        { this->YawData.UnsetLimits(); }

    void CameraController::SetPitchLimits(const Real& UpperLimit, const Real& LowerLimit)
        { this->PitchData.SetLimits(UpperLimit,LowerLimit); }

    void CameraController::RemovePitchLimits()
        { this->PitchData.UnsetLimits(); }

    void CameraController::SetRollLimits(const Real& UpperLimit, const Real& LowerLimit)
        { this->RollData.SetLimits(UpperLimit,LowerLimit); }

    void CameraController::RemoveRollLimits()
        { this->RollData.UnsetLimits(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Transform Methods

    void CameraController::MoveForward(Real Units)
    {
        Vector3 Move(0,0,-Units);
        this->Controlled->MoveRelative(Move);
        if(CCM_Walk == this->CurrentMMode)
            this->CheckHeight();
    }

    void CameraController::MoveBackward(Real Units)
    {
        Vector3 Move(0,0,Units);
        this->Controlled->MoveRelative(Move);
        if(CCM_Walk == this->CurrentMMode)
            this->CheckHeight();
    }

    void CameraController::StrafeLeft(Real Units)
    {
        Vector3 Move(-Units,0,0);
        this->Controlled->MoveRelative(Move);
        if(CCM_Walk == CurrentMMode)
            CheckHeight();
    }

    void CameraController::StrafeRight(Real Units)
    {
        Vector3 Move(Units,0,0);
        this->Controlled->MoveRelative(Move);
        if(CCM_Walk == this->CurrentMMode)
            this->CheckHeight();
    }

    void CameraController::Rotate(Real Yaw, Real Pitch, Real Roll)
    {
        this->YawData.Rotate(Yaw);
        this->PitchData.Rotate(Pitch);
        this->RollData.Rotate(Roll);
        Quaternion YawQuat, PitchQuat, RollQuat;

        if(0 == this->YawData.CurrentAngle)
            YawQuat.SetIdentity();
        else
            YawQuat.SetFromAxisAngle(-this->YawData.CurrentAngle,Vector3::Unit_Y());

        if(0 == this->PitchData.CurrentAngle)
            PitchQuat.SetIdentity();
        else
            PitchQuat.SetFromAxisAngle(-this->PitchData.CurrentAngle,Vector3::Unit_X());

        if(0 == this->RollData.CurrentAngle)
            RollQuat.SetIdentity();
        else
            RollQuat.SetFromAxisAngle(-this->RollData.CurrentAngle,Vector3::Unit_Z());

        Quaternion CamRot = YawQuat * PitchQuat * RollQuat;
        this->Controlled->SetOrientation(CamRot);
    }

    void CameraController::Rotate6DOF(Real Yaw, Real Pitch, Real Roll)
    {
        Quaternion CamRot(this->Controlled->GetOrientation());
        CamRot = CamRot *
            Quaternion(-Yaw,Vector3::Unit_Y()) *
            Quaternion(-Pitch,Vector3::Unit_X()) *
            Quaternion(-Roll,Vector3::Unit_Z());
        this->Controlled->SetOrientation(CamRot);// */
    }
}

#endif
