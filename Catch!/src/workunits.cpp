#ifndef _workunits_cpp
#define _workunits_cpp

#include "workunits.h"
#include "catchapp.h"

///////////////////////////////////////////////////////////////////////////////
// CatchPreInputWorkUnit Methods

CatchPreInputWorkUnit::CatchPreInputWorkUnit(CatchApp* Target) :
    CatchApplication(Target) {  }

CatchPreInputWorkUnit::~CatchPreInputWorkUnit()
    {  }

void CatchPreInputWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
{
    EventManager* EventMan = EventManager::GetSingletonPtr();
    //this will either set the pointer to 0 or return a valid pointer to work with.
    EventUserInput* OneInput = EventMan->PopNextUserInputEvent();

    //We check each Event
    while(0 != OneInput)
    {
        if(OneInput->GetType()!=EventBase::UserInput)
            { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Trying to process a non-EventUserInput as an EventUserInput."); }

        //we check each MetaCode in each Event
        /*for (unsigned int c=0; c<OneInput->GetMetaCodeCount(); c++ )
        {
            //Is the key we just pushed ESCAPE
            if(Input::KEY_ESCAPE == OneInput->GetMetaCode(c).GetCode() && Input::BUTTON_PRESSING == OneInput->GetMetaCode(c).GetMetaValue())
                { return false; }
        }// */

        delete OneInput;
        OneInput = EventMan->PopNextUserInputEvent();
    }

    EventGameWindow* OneWindowEvent = EventMan->PopNextGameWindowEvent();
    while(0 != OneWindowEvent)
    {
        if(OneWindowEvent->GetType()!=EventBase::GameWindow)
            { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Trying to process a non-EventGameWindow as an EventGameWindow."); }

        if(!OneWindowEvent->IsEventIDValid())
        {
            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Invalid EventID on GameWindow Event: " + OneWindowEvent->GetEventID());
        }

        delete OneWindowEvent;
        OneWindowEvent = EventMan->PopNextGameWindowEvent();
    }
}

///////////////////////////////////////////////////////////////////////////////
// CatchPostInputWorkUnit Methods

CatchPostInputWorkUnit::CatchPostInputWorkUnit(CatchApp* Target) :
    CatchApplication(Target) {  }

CatchPostInputWorkUnit::~CatchPostInputWorkUnit()
    {  }

void CatchPostInputWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
{
    Input::InputManager* InputMan = Input::InputManager::GetSingletonPtr();
    Input::Mouse* SysMouse = InputMan->GetSystemMouse();
    Input::Keyboard* SysKeyboard = InputMan->GetSystemKeyboard();
    CameraController* DefaultControl = CameraManager::GetSingletonPtr()->GetOrCreateCameraController(CameraManager::GetSingletonPtr()->GetCamera(0));
    if( SysKeyboard->IsButtonPressed(Input::KEY_LEFT) || SysKeyboard->IsButtonPressed(Input::KEY_A))
        DefaultControl->StrafeLeft(300 * (this->CatchApplication->TheEntresol->GetFrameTimeMilliseconds() * 0.001));
    if( SysKeyboard->IsButtonPressed(Input::KEY_RIGHT) || SysKeyboard->IsButtonPressed(Input::KEY_D))
        DefaultControl->StrafeRight(300 * (this->CatchApplication->TheEntresol->GetFrameTimeMilliseconds() * 0.001));
    if( SysKeyboard->IsButtonPressed(Input::KEY_UP) || SysKeyboard->IsButtonPressed(Input::KEY_W))
        DefaultControl->MoveForward(300 * (this->CatchApplication->TheEntresol->GetFrameTimeMilliseconds() * 0.001));
    if( SysKeyboard->IsButtonPressed(Input::KEY_DOWN)  || SysKeyboard->IsButtonPressed(Input::KEY_S))
        DefaultControl->MoveBackward(300 * (this->CatchApplication->TheEntresol->GetFrameTimeMilliseconds() * 0.001));
    static bool MouseCam=false;
    if( SysKeyboard->IsButtonPressed(Input::KEY_HOME) )
        MouseCam=true;
    if( SysKeyboard->IsButtonPressed(Input::KEY_END))
        MouseCam=false;
    Vector2 Offset = SysMouse->GetMouseDelta();
    if( MouseCam && Vector2(0,0) != Offset )
        DefaultControl->Rotate(Offset.X * 0.01,Offset.Y * 0.01,0);

    if( Input::BUTTON_PRESSING == SysKeyboard->GetButtonState(Input::KEY_C) )
    {
        Physics::PhysicsManager* PhysMan = Physics::PhysicsManager::GetSingletonPtr();
        if(PhysMan->GetDebugPhysicsRendering())
        {
            PhysMan->SetDebugPhysicsWireCount(0);
            PhysMan->SetDebugPhysicsRendering(0);
        }else{
            PhysMan->SetDebugPhysicsWireCount(2);
            PhysMan->SetDebugPhysicsRendering(1);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// CatchPostUIWorkUnit Methods

CatchPostUIWorkUnit::CatchPostUIWorkUnit(CatchApp* Target) :
    CatchApplication(Target) {  }

CatchPostUIWorkUnit::~CatchPostUIWorkUnit()
    {  }

void CatchPostUIWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
{
    Input::InputManager* InputMan = Input::InputManager::GetSingletonPtr();
    Input::Mouse* SysMouse = InputMan->GetSystemMouse();
    static RayQueryTool* RayQueryer = new RayQueryTool();
    static Physics::Point2PointConstraint* Dragger = NULL;

    if( SysMouse->IsButtonPressed(1) )
    {
        if( !UI::UIManager::GetSingletonPtr()->MouseIsInUISystem() )
        {
            Vector3WActor* ClickOnActor = 0;
            Ray* MouseRay = RayQueryer->GetMouseRay(5000);
            if(MouseRay)
            {
                ClickOnActor = RayQueryer->GetFirstActorOnRayByPolygon(*MouseRay,Mezzanine::WSO_ActorRigid);
            }
            //ActorBase *temp = ClickOnActor->Actor;

            bool firstframe=false;
            if( 0 != ClickOnActor &&
                0 != ClickOnActor->Actor &&
                this->CatchApplication->IsInsideAnyStartZone(ClickOnActor->Actor) )
            {
                if( !(ClickOnActor->Actor->IsStaticOrKinematic()) &&
                    ClickOnActor->Actor->GetType() == Mezzanine::WSO_ActorRigid &&
                    !Dragger )
                {
                    Vector3 LocalPivot = ClickOnActor->Vector;
                    ActorRigid* rigid = static_cast<ActorRigid*>(ClickOnActor->Actor);
                    rigid->GetPhysicsSettings()->SetActivationState(Mezzanine::Physics::WOAS_DisableDeactivation);
                    Dragger = new Physics::Point2PointConstraint(rigid, LocalPivot);
                    Dragger->SetTAU(0.001);
                    Physics::PhysicsManager::GetSingletonPtr()->AddConstraint(Dragger);
                    Dragger->SetParam(Physics::Con_Stop_CFM,0.8,-1);
                    Dragger->SetParam(Physics::Con_CFM,0.8,-1);
                    //Dragger->SetParam(Physics::Con_Stop_CFM,0.8,0); Dragger->SetParam(Physics::Con_Stop_CFM,0.8,1); Dragger->SetParam(Physics::Con_Stop_CFM,0.8,2); //Dragger->SetParam(4,0.8,3); Dragger->SetParam(4,0.8,4); Dragger->SetParam(4,0.8,5);
                    Dragger->SetParam(Physics::Con_Stop_ERP,0.1,-1);
                    Dragger->SetParam(Physics::Con_ERP,0.1,-1);
                    //Dragger->SetParam(Physics::Con_Stop_ERP,0.1,0); Dragger->SetParam(Physics::Con_Stop_ERP,0.1,1); Dragger->SetParam(Physics::Con_Stop_ERP,0.1,2); //Dragger->SetParam(2,0.1,3); Dragger->SetParam(2,0.1,4); Dragger->SetParam(2,0.1,5);
                    firstframe = true;
                    this->CatchApplication->LastActorThrown = rigid;
                }
            }

            Vector3* DragTo = 0;
            if( MouseRay )
            {
                // This chunk of code calculates the 3d point that the actor needs to be dragged to
                DragTo = RayQueryer->RayPlaneIntersection(*MouseRay, this->CatchApplication->PlaneOfPlay);
                if( DragTo != NULL )
                {
                    if( Dragger && !firstframe )
                    {
                        Dragger->SetPivotBLocation(*DragTo);
                    }
                }
            }

            if(Dragger && !this->CatchApplication->IsInsideAnyStartZone(this->CatchApplication->LastActorThrown))
            {
                ActorRigid* Act = Dragger->GetActorA();
                Physics::PhysicsManager::GetSingletonPtr()->RemoveConstraint(Dragger);
                delete Dragger;
                Dragger = NULL;
                Act->GetPhysicsSettings()->SetActivationState(Mezzanine::Physics::WOAS_DisableDeactivation);
            }

            // Here we cleanup everything we needed for the clicking/dragging
            if( DragTo )
                { delete DragTo; }
            if( MouseRay )
                { delete MouseRay; }
        }

    }else{  //Since we are no longer clicking we need to setup for the next clicking
        if(Dragger)
        {
            ActorRigid* Act = Dragger->GetActorA();
            Physics::PhysicsManager::GetSingletonPtr()->RemoveConstraint(Dragger);
            delete Dragger;
            Dragger = NULL;
            Act->GetPhysicsSettings()->SetActivationState(Mezzanine::Physics::WOAS_DisableDeactivation);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// CatchPostInputWorkUnit Methods

CatchPostGraphicsWorkUnit::CatchPostGraphicsWorkUnit(CatchApp* Target) :
    CatchApplication(Target) {  }

CatchPostGraphicsWorkUnit::~CatchPostGraphicsWorkUnit()
    {  }

void CatchPostGraphicsWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
{
    // Update the timer
    UI::Screen* GameScreen = UI::UIManager::GetSingletonPtr()->GetScreen("GameScreen");
    UI::OpenRenderableContainerWidget* HUDCont = static_cast<UI::OpenRenderableContainerWidget*>(GameScreen->GetWidget("GS_HUD"));
    UI::Caption* Timer = static_cast<UI::Caption*>(HUDCont->GetAreaRenderable("GS_Timer"));
    std::stringstream time;
    Whole TotalSeconds = this->CatchApplication->GetLevelTimer()->GetCurrentTimeInMilliseconds() / 1000;
    Whole Minutes = TotalSeconds / 60;
    Whole Seconds;

    if(60 > TotalSeconds) Seconds = TotalSeconds;
    else Seconds = TotalSeconds % 60;

    if(10 > Seconds) time << Minutes << ":" << 0 << Seconds;
    else time << Minutes << ":" << Seconds;

    Timer->SetText(time.str());

    // Update the score
    LevelScorer::ScorePair Scores = this->CatchApplication->GetLevelScorer()->CalculateThrowableScore();

    UI::Caption* ScoreAmount = static_cast<UI::Caption*>( HUDCont->GetAreaRenderable("GS_ScoreArea") );
    ScoreAmount->SetText( StringTools::ConvertToString( Scores.first + Scores.second ) );

    // Update Stat information
    Graphics::GraphicsManager* GraphicsMan = Graphics::GraphicsManager::GetSingletonPtr();
    UI::OpenRenderableContainerWidget* StatsCont = static_cast<UI::OpenRenderableContainerWidget*>(GameScreen->GetWidget("GS_Stats"));
    UI::Caption* CurFPS = static_cast<UI::Caption*>(StatsCont->GetAreaRenderable("CurFPS"));
    UI::Caption* AvFPS = static_cast<UI::Caption*>(StatsCont->GetAreaRenderable("AvFPS"));
    CurFPS->SetText(StringTools::ConvertToString(GraphicsMan->GetGameWindow(0)->GetLastFPS()));
    AvFPS->SetText(StringTools::ConvertToString(GraphicsMan->GetGameWindow(0)->GetAverageFPS()));

    StringStream FPSStream;
    FPSStream << "-------------------------- Current FPS: " << GraphicsMan->GetGameWindow(0)->GetLastFPS() << ", Average FPS: " << GraphicsMan->GetGameWindow(0)->GetAverageFPS() << " --------------------------";
    this->CatchApplication->TheEntresol->Log(FPSStream.str());
    this->CatchApplication->TheEntresol->DoMainLoopLogging();

    //See if the level is over
    if(this->CatchApplication->CurrentState != CatchApp::Catch_ScoreScreen)
    {
        if(this->CatchApplication->CheckEndOfLevel())
        {
            this->CatchApplication->ChangeState(CatchApp::Catch_ScoreScreen);
        }
    }
}

#endif
