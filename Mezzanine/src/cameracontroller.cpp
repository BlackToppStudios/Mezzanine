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
#ifndef _cameracontroller_cpp
#define _cameracontroller_cpp

#include "cameracontroller.h"
#include "Graphics/cameraproxy.h"
#include "mathtool.h"
#include "rayquerytool.h"
#include "ray.h"
#include "enumerations.h"
#include "actor.h"

namespace Mezzanine
{
    CameraController::CameraController(Graphics::CameraProxy* ToBeControlled) :
        Controlled(ToBeControlled),
        CurrentMMode(CCM_Fly),
        HoverHeight(2),
        YawRad(0),
        PitchRad(0),
        RollRad(0),
        YawLimits(NULL),
        PitchLimits(NULL),
        RollLimits(NULL)
        {  }

    CameraController::~CameraController()
    {
        this->RemoveYawLimits();
        this->RemovePitchLimits();
        this->RemoveRollLimits();
    }

    void CameraController::CheckAngleRollover(Real Angle)
    {
        Real Pi = MathTools::GetPi();
        if(Angle > Pi) {
            Angle = -Pi + (Angle - Pi);
        }else if(Angle < -Pi) {
            Angle = Pi + (Angle + Pi);
        }
    }

    void CameraController::CheckAngleLimits()
    {
        if(this->YawLimits) {
            if(YawRad > this->YawLimits->Upper)
                YawRad = this->YawLimits->Upper;
            if(YawRad < this->YawLimits->Lower)
                YawRad = this->YawLimits->Lower;
        }
        if(this->PitchLimits) {
            if(PitchRad > this->PitchLimits->Upper)
                PitchRad = this->PitchLimits->Upper;
            if(PitchRad < this->PitchLimits->Lower)
                PitchRad = this->PitchLimits->Lower;
        }
        if(this->YawLimits) {
            if(RollRad > this->RollLimits->Upper)
                RollRad = this->RollLimits->Upper;
            if(RollRad < this->RollLimits->Lower)
                RollRad = this->RollLimits->Lower;
        }
    }

    void CameraController::CheckAllAngles()
    {
        this->CheckAngleLimits();
        this->CheckAngleRollover(this->YawRad);
        this->CheckAngleRollover(this->PitchRad);
        this->CheckAngleRollover(this->RollRad);
    }

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
        Vector3 Loc(this->Controlled->GetLocation());
        Vector3 Dest(Loc + Vector3(0,-2000,0));
        Ray GroundRay(Loc,Dest);
        UInt32 flags = Mezzanine::WO_MeshTerrain | Mezzanine::WO_HeightfieldTerrain | Mezzanine::WO_VectorFieldTerrain | Mezzanine::WO_VoxelTerrain;
        if( !RayCaster.GetFirstObjectOnRayByPolygon(GroundRay,flags) )
            { return 0; }
        Real Distance = Loc.Y - (RayCaster.LastQueryResultsObjectPtr()->GetLocation() + RayCaster.LastQueryResultsOffset()).Y;
        return Distance;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

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
    {
        if(!this->YawLimits)
            this->YawLimits = new AngleLimits();
        this->YawLimits->Upper = UpperLimit;
        this->YawLimits->Lower = LowerLimit;
    }

    void CameraController::RemoveYawLimits()
    {
        if(this->YawLimits) {
            delete this->YawLimits;
            this->YawLimits = NULL;
        }
    }

    void CameraController::SetPitchLimits(const Real& UpperLimit, const Real& LowerLimit)
    {
        if(!this->PitchLimits)
            this->PitchLimits = new AngleLimits();
        this->PitchLimits->Upper = UpperLimit;
        this->PitchLimits->Lower = LowerLimit;
    }

    void CameraController::RemovePitchLimits()
    {
        if(this->PitchLimits) {
            delete this->PitchLimits;
            this->PitchLimits = NULL;
        }
    }

    void CameraController::SetRollLimits(const Real& UpperLimit, const Real& LowerLimit)
    {
        if(!this->RollLimits)
            this->RollLimits = new AngleLimits();
        this->RollLimits->Upper = UpperLimit;
        this->RollLimits->Lower = LowerLimit;
    }

    void CameraController::RemoveRollLimits()
    {
        if(this->RollLimits) {
            delete this->RollLimits;
            this->RollLimits = NULL;
        }
    }

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
        this->YawRad += Yaw;
        this->PitchRad += Pitch;
        this->RollRad += Roll;
        this->CheckAllAngles();
        Quaternion YawQuat, PitchQuat, RollQuat;

        if(0 == this->YawRad)
            YawQuat = Quaternion(0,0,0,1);
        else
            YawQuat = Quaternion(-YawRad,Vector3::Unit_Y());

        if(0 == this->PitchRad)
            PitchQuat = Quaternion(0,0,0,1);
        else
            PitchQuat = Quaternion(-PitchRad,Vector3::Unit_X());

        if(0 == this->RollRad)
            RollQuat = Quaternion(0,0,0,1);
        else
            RollQuat = Quaternion(-RollRad,Vector3::Unit_Z());

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
