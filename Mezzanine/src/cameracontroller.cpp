//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#include "camera.h"
#include "mathtool.h"
#include "rayquerytool.h"
#include "ray.h"
#include "vector3wactor.h"
#include "enumerations.h"

namespace Mezzanine
{
    CameraController::CameraController(Camera* ToBeControlled)
        : Controlled(ToBeControlled),
          CurrentMMode(CCM_Fly),
          HoverHeight(2),
          YawRad(0),
          PitchRad(0),
          RollRad(0),
          YawLimits(NULL),
          PitchLimits(NULL),
          RollLimits(NULL)
    {
    }

    CameraController::~CameraController()
    {
        RemoveYawLimits();
        RemovePitchLimits();
        RemoveRollLimits();
    }

    void CameraController::CheckAngleRollover(Real Angle)
    {
        Real Pi = MathTool::GetPi();
        if(Angle > Pi)
        {
            Angle = -Pi + (Angle - Pi);
        }
        else if(Angle < -Pi)
        {
            Angle = Pi + (Angle + Pi);
        }
    }

    void CameraController::CheckAngleLimits()
    {
        if(YawLimits)
        {
            if(YawRad > YawLimits->Upper) YawRad = YawLimits->Upper;
            if(YawRad < YawLimits->Lower) YawRad = YawLimits->Lower;
        }
        if(PitchLimits)
        {
            if(PitchRad > PitchLimits->Upper) PitchRad = PitchLimits->Upper;
            if(PitchRad < PitchLimits->Lower) PitchRad = PitchLimits->Lower;
        }
        if(YawLimits)
        {
            if(RollRad > RollLimits->Upper) RollRad = RollLimits->Upper;
            if(RollRad < RollLimits->Lower) RollRad = RollLimits->Lower;
        }
    }

    void CameraController::CheckAllAngles()
    {
        CheckAngleLimits();
        CheckAngleRollover(YawRad);
        CheckAngleRollover(PitchRad);
        CheckAngleRollover(RollRad);
    }

    void CameraController::CheckHeight()
    {
        Vector3 Loc(Controlled->GetLocation());
        Real Dist = FindDistanceToGround();
        if(0==Dist)
            return;
        Real DeltaDist = Dist - HoverHeight;
        Loc.Y-=DeltaDist;
        Controlled->SetLocation(Loc);
    }

    Real CameraController::FindDistanceToGround()
    {
        Vector3 Loc(Controlled->GetLocation());
        Vector3 Dest(Loc + Vector3(0,-2000,0));
        Ray* GroundRay = new Ray(Loc,Dest);
        Vector3WActor* Result = RayQueryTool::GetFirstActorOnRayByPolygon(*GroundRay,Mezzanine::WOT_ActorTerrain);
        if(NULL == Result)
        {
            delete Result;
            delete GroundRay;
            return 0;
        }
        if(NULL == Result->Actor)
        {
            delete Result;
            delete GroundRay;
            return 0;
        }
        Real Distance = Loc.Y - (Result->Actor->GetLocation() + Result->Vector).Y;
        delete Result;
        delete GroundRay;
        return Distance;
    }

    Camera* CameraController::GetControlledCamera() const
    {
        return Controlled;
    }

    void CameraController::SetMovementMode(const CameraController::MovementMode& MoveMode)
    {
        CurrentMMode = MoveMode;
    }

    CameraController::MovementMode CameraController::GetMovementMode() const
    {
        return CurrentMMode;
    }

    void CameraController::SetHoverHeight(const Real& Hover)
    {
        HoverHeight = Hover;
    }

    Real CameraController::GetHoverHeight() const
    {
        return HoverHeight;
    }

    void CameraController::SetYawLimits(const Real& UpperLimit, const Real& LowerLimit)
    {
        if(!YawLimits)
            YawLimits = new AngleLimits();
        YawLimits->Upper = UpperLimit;
        YawLimits->Lower = LowerLimit;
    }

    void CameraController::RemoveYawLimits()
    {
        if(YawLimits)
        {
            delete YawLimits;
            YawLimits = NULL;
        }
    }

    void CameraController::SetPitchLimits(const Real& UpperLimit, const Real& LowerLimit)
    {
        if(!PitchLimits)
            PitchLimits = new AngleLimits();
        PitchLimits->Upper = UpperLimit;
        PitchLimits->Lower = LowerLimit;
    }

    void CameraController::RemovePitchLimits()
    {
        if(PitchLimits)
        {
            delete PitchLimits;
            PitchLimits = NULL;
        }
    }

    void CameraController::SetRollLimits(const Real& UpperLimit, const Real& LowerLimit)
    {
        if(!RollLimits)
            RollLimits = new AngleLimits();
        RollLimits->Upper = UpperLimit;
        RollLimits->Lower = LowerLimit;
    }

    void CameraController::RemoveRollLimits()
    {
        if(RollLimits)
        {
            delete RollLimits;
            RollLimits = NULL;
        }
    }

    void CameraController::MoveForward(Real Units)
    {
        Vector3 Move(0,0,-Units);
        Controlled->MoveRelative(Move);
        if(CCM_Walk == CurrentMMode)
            CheckHeight();
    }

    void CameraController::MoveBackward(Real Units)
    {
        Vector3 Move(0,0,Units);
        Controlled->MoveRelative(Move);
        if(CCM_Walk == CurrentMMode)
            CheckHeight();
    }

    void CameraController::StrafeLeft(Real Units)
    {
        Vector3 Move(-Units,0,0);
        Controlled->MoveRelative(Move);
        if(CCM_Walk == CurrentMMode)
            CheckHeight();
    }

    void CameraController::StrafeRight(Real Units)
    {
        Vector3 Move(Units,0,0);
        Controlled->MoveRelative(Move);
        if(CCM_Walk == CurrentMMode)
            CheckHeight();
    }

    void CameraController::Rotate(Real Yaw, Real Pitch, Real Roll)
    {
        YawRad+=Yaw;
        PitchRad+=Pitch;
        RollRad+=Roll;
        CheckAllAngles();
        Quaternion YawQuat, PitchQuat, RollQuat;

        if(0==YawRad)
            YawQuat = Quaternion(0,0,0,1);
        else
            YawQuat = Quaternion(-YawRad,Vector3::Unit_Y());

        if(0==PitchRad)
            PitchQuat = Quaternion(0,0,0,1);
        else
            PitchQuat = Quaternion(-PitchRad,Vector3::Unit_X());

        if(0==RollRad)
            RollQuat = Quaternion(0,0,0,1);
        else
            RollQuat = Quaternion(-RollRad,Vector3::Unit_Z());

        Quaternion CamRot = YawQuat * PitchQuat * RollQuat;
        Controlled->SetOrientation(CamRot);
    }

    void CameraController::Rotate6DOF(Real Yaw, Real Pitch, Real Roll)
    {
        Quaternion CamRot(Controlled->GetOrientation());
        CamRot = CamRot *
            Quaternion(-Yaw,Vector3::Unit_Y()) *
            Quaternion(-Pitch,Vector3::Unit_X()) *
            Quaternion(-Roll,Vector3::Unit_Z());
        Controlled->SetOrientation(CamRot);// */
    }
}

#endif
