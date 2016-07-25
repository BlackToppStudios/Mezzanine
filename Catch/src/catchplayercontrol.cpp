#ifndef _catchplayercontrol_cpp
#define _catchplayercontrol_cpp

#include "catchplayercontrol.h"

CatchPlayerControl::CatchPlayerControl() :
    CatchPlayerView(NULL)
{

}

CatchPlayerControl::~CatchPlayerControl()
{

}

void CatchPlayerControl::SetCamera(Graphics::CameraProxy* View)
{
    this->CatchPlayerView = View;
#ifdef MEZZDEBUG
    this->CamControl.SetControlledCamera(View);
#endif
}

Graphics::CameraProxy* CatchPlayerControl::GetCamera() const
{
    return this->CatchPlayerView;
}

void CatchPlayerControl::MoveToNextTrackNode(Real Amount)
{

}

void CatchPlayerControl::MoveToPrevTrackNode(Real Amount)
{

}

#ifdef MEZZDEBUG
    void CatchPlayerControl::MoveForward(Real Units)
        { this->CamControl.MoveForward(Units); }
    void CatchPlayerControl::MoveBackward(Real Units)
        { this->CamControl.MoveBackward(Units); }
    void CatchPlayerControl::StrafeLeft(Real Units)
        { this->CamControl.StrafeLeft(Units); }
    void CatchPlayerControl::StrafeRight(Real Units)
        { this->CamControl.StrafeRight(Units); }
    void CatchPlayerControl::Rotate(Real Yaw, Real Pitch, Real Roll)
        { this->CamControl.Rotate(Yaw,Pitch,Roll); }
#endif

#endif
