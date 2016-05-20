#ifndef _playercontrol_cpp
#define _playercontrol_cpp

#include "playercontrol.h"

PlayerControl::PlayerControl() :
    PlayerView(NULL)
{

}

PlayerControl::~PlayerControl()
{

}

void PlayerControl::SetCamera(Graphics::CameraProxy* View)
{
    this->PlayerView = View;
#ifdef MEZZDEBUG
    this->CamControl.SetControlledCamera(View);
#endif
}

Graphics::CameraProxy* PlayerControl::GetCamera() const
{
    return this->PlayerView;
}

void PlayerControl::MoveToNextTrackNode(Real Amount)
{

}

void PlayerControl::MoveToPrevTrackNode(Real Amount)
{

}

#ifdef MEZZDEBUG
    void PlayerControl::MoveForward(Real Units)
        { this->CamControl.MoveForward(Units); }
    void PlayerControl::MoveBackward(Real Units)
        { this->CamControl.MoveBackward(Units); }
    void PlayerControl::StrafeLeft(Real Units)
        { this->CamControl.StrafeLeft(Units); }
    void PlayerControl::StrafeRight(Real Units)
        { this->CamControl.StrafeRight(Units); }
    void PlayerControl::Rotate(Real Yaw, Real Pitch, Real Roll)
        { this->CamControl.Rotate(Yaw,Pitch,Roll); }
#endif

#endif
