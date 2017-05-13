#ifndef _catchplayer_cpp
#define _catchplayer_cpp

#include "catchplayer.h"

CatchPlayer::CatchPlayer(ProfileManager* Manager) :
    Profile(NULL),
    ProMan(Manager)
    {  }

CatchPlayer::~CatchPlayer()
{
    if( this->Profile != NULL ) {
        this->ProMan->SaveProfile(this->Profile);
        //this->Profile->Unload();
    }
}

void CatchPlayer::SetIdentity(GameProfile* CatchPlayerProfile)
{
    if( this->Profile != NULL ) {
        this->ProMan->SaveProfile(this->Profile);
        //this->Profile->Unload();
    }
    this->Profile = CatchPlayerProfile;
    this->ProMan->LoadProfile(this->Profile);
}

void CatchPlayer::InitWorldObjects(World* GameWorld)
{
    Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( GameWorld->GetManager( ManagerBase::MT_SceneManager ) );
    if( SceneMan != NULL && SceneMan->GetNumProxies(Mezzanine::PT_Graphics_CameraProxy) == 0 ) {
        Graphics::CameraProxy* PlayCam = SceneMan->CreateCamera();
        PlayCam->SetNearClipDistance(0.5);
        PlayCam->LookAt(Vector3(0,0,0));
        this->Control.SetCamera(PlayCam);
    }
}

CatchPlayerControl& CatchPlayer::GetControl()
    { return this->Control; }

GameProfile* CatchPlayer::GetProfile() const
    { return this->Profile; }

const String& CatchPlayer::GetName() const
    { return this->Profile->GetName(); }

void CatchPlayer::Update(Input::InputManager* InputMan, const Whole DeltaTime)
{
#ifdef MEZZDEBUG
    Input::Mouse* SysMouse = InputMan->GetSystemMouse();
    Input::Keyboard* SysKeyboard = InputMan->GetSystemKeyboard();
    // Determine our camera linear movement
    if( SysKeyboard->IsButtonPressed(Input::KEY_LEFT) || SysKeyboard->IsButtonPressed(Input::KEY_A) )
        this->Control.StrafeLeft( 300 * (DeltaTime * 0.001) );
    if( SysKeyboard->IsButtonPressed(Input::KEY_RIGHT) || SysKeyboard->IsButtonPressed(Input::KEY_D) )
        this->Control.StrafeRight( 300 * (DeltaTime * 0.001) );
    if( SysKeyboard->IsButtonPressed(Input::KEY_UP) || SysKeyboard->IsButtonPressed(Input::KEY_W) )
        this->Control.MoveForward( 300 * (DeltaTime * 0.001) );
    if( SysKeyboard->IsButtonPressed(Input::KEY_DOWN) || SysKeyboard->IsButtonPressed(Input::KEY_S) )
        this->Control.MoveBackward( 300 * (DeltaTime * 0.001) );
    // Determine our camera angular movement
    static Boole EnableRotation = false; // Temporary measure
    if( SysKeyboard->IsButtonPressing(Input::KEY_HOME) )
        EnableRotation = !EnableRotation;

    if( EnableRotation ) {
        Vector2 Offset = SysMouse->GetMouseDelta();
        if( SysMouse->IsButtonPressed(Input::MOUSEBUTTON_2) && !Offset.IsZero() ) {
            this->Control.Rotate(Offset.X * 0.01,Offset.Y * 0.01,0);
        }
    }
#endif
}


/*
    if( this->ActiveProfile != NULL ) {
        UI::Screen* MainMenuScreen = static_cast<UI::UIManager*>( this->TheEntresol->GetManager(ManagerBase::MT_UIManager) )->GetScreen("MainMenuScreen");
        if( MainMenuScreen != NULL ) {
            UI::Widget* ProfileDisplay = MainMenuScreen->GetWidget("MS_ProfilesAccess");
            UI::SingleLineTextLayer* ProfileDisplayText = static_cast<UI::SingleLineTextLayer*>( ProfileDisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
            ProfileDisplayText->SetText( this->ActiveProfile->GetName() );
        }
    }
*/
#endif
