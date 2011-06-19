#ifndef _catchapp_cpp
#define _catchapp_cpp

#include "catchapp.h"
#include "callbacks.h"
#include "buttoncallbacks.h"
#include <cassert>

using namespace phys;

CatchApp* CatchApp::TheRealCatchApp = 0;

CatchApp::CatchApp(const Vector3 &WorldLowerBounds, const Vector3 &WorldUpperBounds, SceneManager::SceneManagerType SceneType, const unsigned short int &MaxPhysicsProxies)
    : CurrScore(0),
      LastActorThrown(NULL),
      EndTimer(NULL),
      CurrentState(CatchApp::Catch_Init),
      PlaneOfPlay(Plane(Vector3(2.0,1.0,0.0), Vector3(1.0,2.0,0.0), Vector3(1.0,1.0,0.0)))
{
    assert(0==CatchApp::TheRealCatchApp);
    CatchApp::TheRealCatchApp = this;

    try
    {
        TheWorld = new World( WorldLowerBounds, WorldUpperBounds, SceneType, MaxPhysicsProxies);
    }catch( exception x){
        //could not create world
    }
    Loader = new LevelLoader();
}

CatchApp::~CatchApp()
{
    CatchApp::TheRealCatchApp = NULL;
    //delete TheWorld;
}

void CatchApp::MakeGUI()
{
    UIManager* GUI = TheWorld->GetUIManager();
    Viewport* UIViewport = TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->GetViewport(0);

    ColourValue Transparent(0.0,0.0,0.0,0.0);
    ColourValue Black(0.0,0.0,0.0,1.0);
    ColourValue TransBlack(0.0,0.0,0.0,0.85);
    ColourValue Gray(0.2,0.2,0.2,1.0);

    //Make the Main Menu screen and associated layers.
    GUI->LoadGorilla("Catch_Menu");
    UI::Screen* MainMenuScreen = GUI->CreateScreen("MainMenuScreen", "Catch_Menu", UIViewport);
    UI::Layer* MainMenuLayer = MainMenuScreen->CreateLayer("MainMenuLayer",0);

    //Build the Main Menu Screen
    UI::Rectangle* Background = MainMenuLayer->CreateRectangle( Vector2(0,0), Vector2(1,1));
    Background->SetBackgroundSprite("MainMenuBackground");

    UI::Menu* MainMenuMenu = MainMenuLayer->CreateMenu( "MS_Menu", Vector2(0.0,0.915), Vector2(1.0,0.086));
    MainMenuMenu->GetRootWindow()->GetWindowBack()->SetBackgroundSprite("MMBrickBackground");
    MainMenuMenu->GetRootWindow()->SetAutoHide(false);

    UI::TextButton* LevelSelectAccess = MainMenuMenu->GetRootWindow()->CreateAccessorButton( "MS_LevelSelect", Vector2(0.05, 0.93), Vector2(0.22, 0.06), 16, "Level Select" );
    LevelSelectAccess->SetBackgroundSprite("MMButton");
    LevelSelectAccess->SetHoveredSprite("MMHoveredButton");
    UI::MenuWindow* LevelSelectWin = MainMenuMenu->GetRootWindow()->CreateChildMenuWindow("MS_LevelSelect", Vector2(0.02,0.02), Vector2(0.96,0.80), LevelSelectAccess);
    LevelSelectWin->GetWindowBack()->SetBackgroundSprite("MMLSBackground");

    UI::TextButton* OptionsAccess = MainMenuMenu->GetRootWindow()->CreateAccessorButton( "MS_Options", Vector2(0.28, 0.93), Vector2(0.22, 0.06), 16, "Options" );
    OptionsAccess->SetBackgroundSprite("MMButton");
    OptionsAccess->SetHoveredSprite("MMHoveredButton");
    UI::MenuWindow* OptionsWin = MainMenuMenu->GetRootWindow()->CreateChildMenuWindow("MS_Options", Vector2(0.01,0.01), Vector2(0.01,0.01), OptionsAccess);

    UI::TextButton* CreditsAccess = MainMenuMenu->GetRootWindow()->CreateAccessorButton( "MS_Credits", Vector2(0.51, 0.93), Vector2(0.22, 0.06), 16, "Credits" );
    CreditsAccess->SetBackgroundSprite("MMButton");
    CreditsAccess->SetHoveredSprite("MMHoveredButton");
    UI::MenuWindow* CreditsWin = MainMenuMenu->GetRootWindow()->CreateChildMenuWindow("MS_Credits", Vector2(0.01,0.01), Vector2(0.01,0.01), CreditsAccess);

    UI::TextButton* AppExitAccess = MainMenuMenu->GetRootWindow()->CreateAccessorButton( "MS_AppExit", Vector2(0.74, 0.93), Vector2(0.22, 0.06), 16, "Exit Game" );
    AppExitAccess->SetBackgroundSprite("MMButton");
    AppExitAccess->SetHoveredSprite("MMHoveredButton");
    UI::MenuWindow* AppExitWin = MainMenuMenu->GetRootWindow()->CreateChildMenuWindow("MS_AppExit", Vector2(0.25, 0.3), Vector2(0.5, 0.3), AppExitAccess);
    AppExitWin->GetWindowBack()->SetBackgroundColour(TransBlack);
    UI::Caption* AppExitWarn = AppExitWin->CreateCaption("MS_AppExitWarn", Vector2(0.25, 0.35), Vector2(0.5, 0.08), 16, "Are you sure you want to exit?");
    AppExitWarn->SetBackgroundColour(Transparent);
    UI::TextButton* AppExitConf = AppExitWin->CreateTextButton("MS_AppExitConf", Vector2(0.27, 0.45), Vector2(0.22, 0.08), 16, "Yes");
    AppExitConf->SetBackgroundColour(Gray);
    UI::TextButton* AppExitDeny = AppExitWin->CreateBackButton(/*"MS_AppExitDeny", */Vector2(0.51, 0.45), Vector2(0.22, 0.08), 16, "No");
    AppExitDeny->SetBackgroundColour(Gray);
    //End of Main Menu Screen

    //Make the Game screen and associated layers.
    GUI->LoadGorilla("Catch_Game");
    UI::Screen* GameScreen = GUI->CreateScreen("GameScreen", "Catch_Game", UIViewport);
    UI::Layer* Menu = GameScreen->CreateLayer("MenuLayer", 10);
    UI::Layer* Report = GameScreen->CreateLayer("ReportLayer", 8);
    UI::Layer* ItemShop = GameScreen->CreateLayer("ItemShopLayer", 4);
    UI::Layer* Stats = GameScreen->CreateLayer("StatsLayer", 1);
    UI::Layer* HUD = GameScreen->CreateLayer("HUDLayer", 0);

    //Build the Game Screen
    //Build the HUD layer
    UI::Caption* Timer = HUD->CreateCaption( "GS_Timer", Vector2(0.8995, 0.006), Vector2(0.0965, 0.06), 20, "0:00");
    Timer->SetBackgroundSprite("TimerArea");

    UI::Rectangle* TIcon = HUD->CreateRectangle( Vector2(0.8515, 0.006), Vector2(0.0482, 0.06));
    TIcon->SetBackgroundSprite("TimerLogo");

    UI::Button* MenuButton = HUD->CreateButton( "GS_Menu", Vector2(0.008, 0.922), Vector2(0.16, 0.065));
    MenuButton->SetButtonCallback(new GSMenu(MenuButton));
    MenuButton->SetBackgroundSprite("MenuButtonHiRes");

    UI::Button* StoreButton = HUD->CreateButton( "GS_Store", Vector2(0.922, 0.922), Vector2(0.06, 0.065));
    StoreButton->SetButtonCallback(new GSStore(StoreButton));
    StoreButton->SetBackgroundSprite("StoreButtonHiRes");

    UI::Rectangle* StoreText = HUD->CreateRectangle( Vector2(0.767, 0.922), Vector2(0.14, 0.065));
    StoreText->SetBackgroundSprite("StoreText");

    UI::Caption* ScoreAmount = HUD->CreateCaption( "GS_ScoreArea", Vector2(0.135, 0.006), Vector2(0.15, 0.065), 20, "0");
    ScoreAmount->SetBackgroundColour(Transparent);

    UI::Rectangle* ScoreText = HUD->CreateRectangle( Vector2(0.008, 0.006), Vector2(0.12, 0.06));
    ScoreText->SetBackgroundSprite("ScoreText");
    //End of HUD Layer

    //Build the ItemShop Layer
    UI::Window* ItemShopWindow = ItemShop->CreateWidgetWindow("ItemShop", Vector2(0.25, 0.11), Vector2(0.5, 0.78125));
    ItemShopWindow->GetWindowBack()->SetBackgroundSprite("WindowVertBack");

    UI::ButtonListBox* ItemShopList = ItemShopWindow->CreateButtonListBox("StoreItemList",Vector2(0.28,0.54),Vector2(0.44,0.32),0.02,UI::SB_Separate);
    ItemShopList->SetAutoHideScroll(false);
    ItemShopList->GetBoxBack()->SetBackgroundColour(TransBlack);
    ItemShopList->GetVertScroll()->GetScrollBack()->SetBackgroundColour(TransBlack);
    ItemShop->Hide();
    //End of ItemShop Layer

    //Build the Menu Layer
    UI::Menu* GameMenu = Menu->CreateMenu( "GameMenu", Vector2(0.35, 0.27), Vector2(0.3, 0.45));
    GameMenu->GetRootWindow()->GetWindowBack()->SetBackgroundSprite("MenuBack");
    UI::Button* ReturnButton = GameMenu->GetRootWindow()->CreateButton( "GS_Return", Vector2(0.37, 0.56), Vector2(0.26, 0.05));
    ReturnButton->SetButtonCallback(new GSReturn(ReturnButton));
    ReturnButton->SetBackgroundSprite("ReturnButton");
    UI::Button* GameExitButton = GameMenu->GetRootWindow()->CreateButton( "GS_GameExit", Vector2(0.37, 0.64), Vector2(0.26, 0.05));
    GameExitButton->SetBackgroundSprite("ExitButton");

    UI::Button* VideoAccess = GameMenu->GetRootWindow()->CreateAccessorButton("GS_VideoSettingsButton", Vector2(0.37, 0.32), Vector2(0.26, 0.05));
    UI::MenuWindow* VideoSettings = GameMenu->GetRootWindow()->CreateChildMenuWindow("VideoSettings", Vector2(0.18, 0.22), Vector2(0.64, 0.55), VideoAccess);
    VideoSettings->GetWindowBack()->SetBackgroundSprite("WindowHoriBack");
    VideoAccess->SetBackgroundSprite("VideoSetButton");

    UI::Button* VideoBack = VideoSettings->CreateBackButton(Vector2(0.72, 0.705), Vector2(0.09, 0.05));
    VideoBack->SetBackgroundColour(Black);

    UI::Button* SoundAccess = GameMenu->GetRootWindow()->CreateAccessorButton("GS_SoundSettingsButton", Vector2(0.37, 0.40), Vector2(0.26, 0.05));
    UI::MenuWindow* SoundSettings = GameMenu->GetRootWindow()->CreateChildMenuWindow("SoundSettings", Vector2(0.18, 0.22), Vector2(0.64, 0.55), SoundAccess);
    SoundSettings->GetWindowBack()->SetBackgroundSprite("WindowHoriBack");
    SoundAccess->SetBackgroundSprite("SoundSetButton");

    UI::Button* SoundBack = SoundSettings->CreateBackButton(Vector2(0.72, 0.705), Vector2(0.09, 0.05));
    SoundBack->SetBackgroundColour(Black);
    Menu->Hide();
    //End of Menu Layer

    //Build the Stats Layer
    UI::Caption* CurFPS = Stats->CreateCaption( "CurFPS", Vector2(0.16, 0.06), Vector2(0.06, 0.065), 14, "0.0");
    CurFPS->SetBackgroundColour(Transparent);
    CurFPS->HorizontallyAlign(UI::Txt_Left);

    UI::Caption* CurFPSText = Stats->CreateCaption( "CurFPSText", Vector2(0.008, 0.06), Vector2(0.15, 0.065), 14, "Current FPS: ");
    CurFPSText->SetBackgroundColour(Transparent);
    CurFPSText->HorizontallyAlign(UI::Txt_Left);

    UI::Caption* AvFPS = Stats->CreateCaption( "AvFPS", Vector2(0.16, 0.105), Vector2(0.06, 0.065), 14, "0.0");
    AvFPS->SetBackgroundColour(Transparent);
    AvFPS->HorizontallyAlign(UI::Txt_Left);

    UI::Caption* AvFPSText = Stats->CreateCaption( "AvFPSText", Vector2(0.008, 0.105), Vector2(0.15, 0.065), 14, "Average FPS: ");
    AvFPSText->SetBackgroundColour(Transparent);
    AvFPSText->HorizontallyAlign(UI::Txt_Left);
    //End of Stats Layer

    //Build the Report Layer
    UI::Window* LevelReport = Report->CreateWidgetWindow("LevelReport", Vector2(0.2, 0.2), Vector2(0.6, 0.6));
    LevelReport->GetWindowBack()->SetBackgroundColour(Gray);
    //TempCaption
    UI::Caption* TempCapt = LevelReport->CreateCaption("GS_TempWarning", Vector2(0.25, 0.3), Vector2(0.5, 0.3), 18, "Future spot of level reports.");
    TempCapt->SetBackgroundColour(Transparent);
    UI::TextButton* FinishButton = LevelReport->CreateTextButton("GS_Finish", Vector2(0.42, 0.66), Vector2(0.16, 0.08), 14, "Finish");
    FinishButton->SetBackgroundColour(TransBlack);
    Report->Hide();
    //End of Report Layer
    //End of Game Screen
}

void CatchApp::CreateLoadingScreen()
{
    UIManager* GUI = TheWorld->GetUIManager();
    GUI->LoadGorilla("Catch_Loading");
    Viewport* UIViewport = TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->GetViewport(0);
    UI::Screen* LoadScreen = GUI->CreateScreen("LoadingScreen", "Catch_Loading", UIViewport);
    UI::Layer* LoadLayer = LoadScreen->CreateLayer("LoadingLayer", 0);
    UI::Rectangle* Load = LoadLayer->CreateRectangle( Vector2(0, 0), Vector2(1, 1));
    Load->SetBackgroundSprite("LoadingBackground");
    //LoadScreen->RenderOnce();
    crossplatform::RenderPhysWorld();
}

void CatchApp::ConfigResources()
{
    ResourceManager* ResourceMan = TheWorld->GetResourceManager();
    String CommonGroup("Common");
    String datadir = crossplatform::GetDataDirectory();
    ResourceMan->AddResourceLocation(datadir, "FileSystem", CommonGroup, false);
    ResourceMan->AddResourceLocation(datadir+"Common.zip", "Zip", CommonGroup, false);
    ResourceMan->AddResourceLocation(datadir+"AdvThrowables.zip", "Zip", CommonGroup, false);
    ResourceMan->InitResourceGroup(CommonGroup);
}

void CatchApp::PopulateScoreValues()
{
    ItemScoreValues["Gold"] = 100;
    ItemScoreValues["Iron"] = 50;
    ItemScoreValues["Clay"] = 10;
    ItemScoreValues["Uranium"] = 100;
    ItemScoreValues["Rubber"] = 30;
    ItemScoreValues["Lead"] = 30;
    ItemScoreValues["Styrofoam"] = 10;
    ItemScoreValues["Wood"] = 20;
}

void CatchApp::PopulateShopValues()
{
    ShopCostValues["Wooden Plank"] = 50;
}

void CatchApp::ChangeState(const CatchApp::GameState &StateToSet)
{
    if(CurrentState == StateToSet)
        return;
    UIManager* UIMan = TheWorld->GetUIManager();
    switch (StateToSet)
    {
        case CatchApp::Catch_GameScreen:
        {
            for( Whole Screen = 0 ; Screen < UIMan->GetNumScreens() ; Screen++ )
            {
                UI::Screen* CurrScreen = UIMan->GetScreen(Screen);
                if("GameScreen" == CurrScreen->GetName())
                    CurrScreen->Show();
                else
                    CurrScreen->Hide();
            }
            break;
        }
        case CatchApp::Catch_Loading:
        {
            for( Whole Screen = 0 ; Screen < UIMan->GetNumScreens() ; Screen++ )
            {
                UI::Screen* CurrScreen = UIMan->GetScreen(Screen);
                if("LoadingScreen" == CurrScreen->GetName())
                    CurrScreen->Show();
                else
                    CurrScreen->Hide();
            }
            break;
        }
        case CatchApp::Catch_MenuScreen:
        {
            for( Whole Screen = 0 ; Screen < UIMan->GetNumScreens() ; Screen++ )
            {
                UI::Screen* CurrScreen = UIMan->GetScreen(Screen);
                if("MainMenuScreen" == CurrScreen->GetName())
                    CurrScreen->Show();
                else
                    CurrScreen->Hide();
            }
            break;
        }
    }
    CurrentState = StateToSet;
}

bool CatchApp::CheckEndOfLevel()
{
    if(!EndTimer)
    {
        EndTimer = TheWorld->GetTimerManager()->CreateSimpleTimer(Timer::StopWatch);
        EndTimer->SetInitialTime(5 * 1000000);
        EndTimer->SetCurrentTime(5 * 1000000);
        EndTimer->SetGoalTime(0);
        EndTimer->Start();
    }
    for( Whole SA = 0 ; SA < ScoreAreas.size() ; SA++ )
    {
        std::vector<ActorBase*>& FinalTest = ScoreAreas[SA]->GetAddedActors();
        for( Whole F = 0 ; F < FinalTest.size() ; F++ )
        {
            if(LastActorThrown == FinalTest[F])
                EndTimer->Reset();
        }
    }
    if(EndTimer->IsStopped())
        return true;
    else
        return false;
}

bool CatchApp::AllStartZonesEmpty()
{
    for( Whole X = 0 ; X < StartAreas.size() ; X++ )
    {
        if(!StartAreas[X]->IsEmpty())
            return false;
    }
    return true;
}

bool CatchApp::IsInsideAnyStartZone(ActorBase* Actor)
{
    for( Whole X = 0 ; X < StartAreas.size() ; X++ )
    {
        if(StartAreas[X]->IsInside(Actor))
            return true;
    }
    return false;
}

void CatchApp::UnloadLevel()
{
    PhysicsManager* PhysMan = TheWorld->GetPhysicsManager();
    SceneManager* SceneMan = TheWorld->GetSceneManager();
    ActorManager* ActorMan = TheWorld->GetActorManager();

    PhysMan->DestroyAllConstraints();
    PhysMan->DestroyAllAreaEffects();
    PhysMan->DestroyAllPhysicsShapes();
    SceneMan->DestroyAllLights();
    SceneMan->DestroyAllParticleEffects();
    SceneMan->DestroyAllWorldNodes();
    ActorMan->DestroyAllActors();
}

CatchApp* CatchApp::GetCatchAppPointer()
{
    assert(0!=CatchApp::TheRealCatchApp);
    return TheRealCatchApp;
}

int CatchApp::GetCatchin()
{
    //Give the world functions to run before and after input and physics
    TheWorld->GetEventManager()->SetPreMainLoopItems(&CPreInput);
    TheWorld->GetEventManager()->SetPostMainLoopItems(&CPostInput);
    TheWorld->GetPhysicsManager()->SetPreMainLoopItems(&CPrePhysics);
    TheWorld->GetPhysicsManager()->SetPostMainLoopItems(&CPostPhysics);
    TheWorld->GetGraphicsManager()->SetPostMainLoopItems(&CPostRender);
    TheWorld->GetUIManager()->SetPreMainLoopItems(&CPreUI);
    TheWorld->GetUIManager()->SetPostMainLoopItems(&CPostUI);

    //Set the Make the RenderWindow and load system stuff
	TheWorld->GameInit(false);

	TheWorld->GetUIManager()->EnableButtonAutoRegister(true);
    TheWorld->GetUIManager()->AddAutoRegisterCode(MetaCode::MOUSEBUTTON_1);

    ConfigResources();
	CreateLoadingScreen();
	ChangeState(CatchApp::Catch_Loading);

	PopulateScoreValues();
    PopulateShopValues();

    //Set logging frequency
    TheWorld->SetLoggingFrequency(World::LogNever);

	// Set the Title
    TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->SetWindowCaption("Catch!");
    TheWorld->SetTargetFrameRate(60);

    // Create the Timer
    LevelTimer = TheWorld->GetTimerManager()->CreateSimpleTimer(Timer::Normal);

    //Generate the UI
    MakeGUI();

    Loader->SetNextLevel("MainMenu");
    do{
        ChangeState(CatchApp::Catch_Loading);
        //Actually Load the game stuff
        Loader->LoadLevel();

        if("MainMenu"==Loader->GetCurrentLevel())
            ChangeState(CatchApp::Catch_MenuScreen);
        else
            ChangeState(CatchApp::Catch_GameScreen);
        LevelTimer->Reset();
        LevelTimer->Start();

        //Start the Main Loop
        TheWorld->MainLoop();

        UnloadLevel();
        LevelTimer->Stop();
    } while(Loader->HasALevelToLoad());

	return 0;
}

bool CatchApp::PreInput()
{
    // using the Raw Event Manager, and deleting the events
    if( !CheckForStuff() )
        return false;
    return true;
}

bool CatchApp::PostInput()
{
    CameraController* DefaultControl = TheWorld->GetCameraManager()->GetOrCreateCameraController(TheWorld->GetCameraManager()->GetDefaultCamera());
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_LEFT) || InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_A))
        DefaultControl->StrafeLeft(300 * (TheWorld->GetFrameTime() * 0.001));
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_RIGHT) || InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_D))
        DefaultControl->StrafeRight(300 * (TheWorld->GetFrameTime() * 0.001));
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_UP) || InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_W))
        DefaultControl->MoveForward(300 * (TheWorld->GetFrameTime() * 0.001));
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_DOWN)  || InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_S))
        DefaultControl->MoveBackward(300 * (TheWorld->GetFrameTime() * 0.001));
    static bool MouseCam=false;
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_HOME) )
        MouseCam=true;
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_END))
        MouseCam=false;
    Vector2 Offset = InputQueryTool::GetMousePrevFrameOffset();
    if( MouseCam && Vector2(0,0) != Offset )
        DefaultControl->Rotate(Offset.X * 0.01,Offset.Y * 0.01,0);
    return true;
}

bool CatchApp::PreUI()
{
    return true;
}

bool CatchApp::PostUI()
{
    //User Input through a WorldQueryTool
    static RayQueryTool* RayQueryer = new RayQueryTool();
    static InputQueryTool* InputQueryer = new InputQueryTool();

    //Queryer.GatherEvents();
    TheWorld->Log("Mouse location From WorldQueryTool X/Y");
    TheWorld->Log(InputQueryer->GetMouseX());
    TheWorld->Log(InputQueryer->GetMouseY());

    //Resize the window
    if ( InputQueryer->GetKeyboardButtonState(MetaCode::KEY_F) == MetaCode::BUTTON_LIFTING )
        TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->setFullscreen(true);
    else if ( InputQueryer->GetKeyboardButtonState(MetaCode::KEY_G) == MetaCode::BUTTON_LIFTING )
        TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->setFullscreen(false);
    else if ( InputQueryer->GetKeyboardButtonState(MetaCode::KEY_R) == MetaCode::BUTTON_LIFTING )
        TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->setRenderResolution(1024,768);
    else if ( InputQueryer->GetKeyboardButtonState(MetaCode::KEY_T) == MetaCode::BUTTON_LIFTING )
        TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->setRenderResolution(800,600);

    static Point2PointConstraint* Dragger=NULL;

    if( InputQueryer->IsMouseButtonPushed(1) )
    {
        if(TheWorld->GetUIManager()->MouseIsInUISystem())
        {
            UIManager* UIMan = TheWorld->GetUIManager();
            UI::Button* MouseButton = UIMan->GetHoveredButton();
            if(!MouseButton && UIMan->GetHoveredWidget())
                MouseButton = UIMan->GetHoveredWidget()->GetHoveredButton();
            if(MouseButton)
            {
                MetaCode::ButtonState State = InputQueryer->GetMouseButtonState(1);
                if("GS_Finish" == MouseButton->GetName())
                {
                    return false;
                }
                else if("GS_GameExit" == MouseButton->GetName())
                {
                    return false;
                }
                else if("MS_AppExitConf" == MouseButton->GetName())
                {
                    return false;
                }
            }
        }else{
            #ifdef PHYSDEBUG
            TheWorld->Log("Gamebase CLICK:");
            TheWorld->LogStream << "Camera Location: " << TheWorld->GetCameraManager()->GetDefaultCamera()->GetLocation() << endl;
            #endif

            Ray *MouseRay = RayQueryer->GetMouseRay(5000);
            //*MouseRay *= 1000;
            //Ray *MouseRay = new Ray(Vector3(500.0, 0.0, 0.0),Vector3(-500.0, 0.0, 0.0));

            Vector3WActor *ClickOnActor = RayQueryer->GetFirstActorOnRayByPolygon(*MouseRay,phys::WOT_ActorRigid);
            #ifdef PHYSDEBUG
            TheWorld->LogStream << "MouseRay: " << *MouseRay << "| Length: " << MouseRay->Length() << endl;
            #endif

            //ActorBase *temp = ClickOnActor->Actor;

            bool firstframe=false;
            if(0 == ClickOnActor || 0 == ClickOnActor->Actor)
            {
                #ifdef PHYSDEBUG
                TheWorld->Log("No Actor Clicked on");
                #endif
            }else if(!IsInsideAnyStartZone(ClickOnActor->Actor)){
                #ifdef PHYSDEBUG
                TheWorld->Log("Actor is not in any starting zone");
                #endif
            }else{
                #ifdef PHYSDEBUG
                TheWorld->Log("Actor Clicked on"); TheWorld->Log(*ClickOnActor);
                TheWorld->Log("MouseRay"); TheWorld->Log(*MouseRay);
                TheWorld->Log("PlaneOfPlay"); TheWorld->Log(PlaneOfPlay);
                TheWorld->Log("ClickOnActor"); TheWorld->Log(*ClickOnActor);
                #endif
                if(!(ClickOnActor->Actor->IsStaticOrKinematic()))
                {
                    if(!Dragger) //If we have a dragger, then this is dragging, not clicking
                    {
                        if(ClickOnActor->Actor->GetType()==ActorBase::Actorrigid) //This is Dragging let's do some checks for sanity
                        {
                            Vector3 LocalPivot = ClickOnActor->Vector;
                            ActorRigid* rigid = static_cast<ActorRigid*>(ClickOnActor->Actor);
                            rigid->GetPhysicsSettings()->SetActivationState(phys::AAS_DisableDeactivation);
                            //Dragger = new Generic6DofConstraint(rigid, LocalPivot, Quaternion(0,0,0,1), false);
                            Dragger = new Point2PointConstraint(rigid, LocalPivot);
                            Dragger->SetTAU(0.001);
                            //Dragger->SetLinearLowerLimit(Vector3(0.f,0.f,0.f));
                            //Dragger->SetLinearUpperLimit(Vector3(0.f,0.f,0.f));
                            //Dragger->SetAngularLowerLimit(Vector3(0.f,0.f,0.f));
                            //Dragger->SetAngularUpperLimit(Vector3(0.f,0.f,0.f));
                            TheWorld->GetPhysicsManager()->AddConstraint(Dragger);
                            Dragger->SetParam(4,0.8,0); Dragger->SetParam(4,0.8,1); Dragger->SetParam(4,0.8,2); //Dragger->SetParam(4,0.8,3); Dragger->SetParam(4,0.8,4); Dragger->SetParam(4,0.8,5);
                            Dragger->SetParam(2,0.1,0); Dragger->SetParam(2,0.1,1); Dragger->SetParam(2,0.1,2); //Dragger->SetParam(2,0.1,3); Dragger->SetParam(2,0.1,4); Dragger->SetParam(2,0.1,5);
                            firstframe=true;
                            LastActorThrown = rigid;
                        }else{  // since we don't
                            #ifdef PHYSDEBUG
                            TheWorld->Log("Actor is not an ActorRigid.  Aborting.");
                            #endif
                        }
                    }
                }else{
                    #ifdef PHYSDEBUG
                    TheWorld->Log("Actor is Static/Kinematic.  Aborting.");
                    #endif
                }
            }

            // This chunk of code calculates the 3d point that the actor needs to be dragged to
            Vector3 *DragTo = RayQueryer->RayPlaneIntersection(*MouseRay, PlaneOfPlay);
            if (0 == DragTo)
            {
                #ifdef PHYSDEBUG
                TheWorld->Log("PlaneOfPlay Not Clicked on");
                #endif
            }else{
                if(Dragger && !firstframe)
                {
                    #ifdef PHYSDEBUG
                    TheWorld->Log("Dragged To");
                    TheWorld->Log(*DragTo);
                    #endif
                    Dragger->SetPivotB(*DragTo);
                }
            }

            if(Dragger && !IsInsideAnyStartZone(LastActorThrown))
            {
                ActorRigid* Act = Dragger->GetActorA();
                TheWorld->GetPhysicsManager()->RemoveConstraint(Dragger);
                delete Dragger;
                Dragger=NULL;
                Act->GetPhysicsSettings()->SetActivationState(phys::AAS_DisableDeactivation);
            }

            // Here we cleanup everything we needed for the clicking/dragging
            if ( DragTo )
                { delete DragTo; }
            if ( MouseRay )
                { delete MouseRay; }
        }

    }else{  //Since we are no longer clicking we need to setup for the next clicking
        if(Dragger)
        {
            ActorRigid* Act = Dragger->GetActorA();
            TheWorld->GetPhysicsManager()->RemoveConstraint(Dragger);
            delete Dragger;
            Dragger=NULL;
            Act->GetPhysicsSettings()->SetActivationState(phys::AAS_DisableDeactivation);
        }
    }
    return true;
}

bool CatchApp::PrePhysics()
{
    return true;
}

bool CatchApp::PostPhysics()
{
    //// Updating functions to be used when a suitable mesh is found/created.
    //ActorSoft* ActS = static_cast< ActorSoft* > (TheWorld->Actors->FindActor("Column1"));
    //ActS->UpdateSoftBody();

    for( Whole S = 0 ; S < ScoreAreas.size() ; S++ )
    {
        std::vector<ActorBase*>& Added = ScoreAreas[S]->GetAddedActors();
        std::vector<ActorBase*>& Removed = ScoreAreas[S]->GetRemovedActors();
        for( Whole A = 0 ; A < Added.size() ; A++ )
        {
            String ItemName = Added[A]->GetName();
            for( std::map<String,Whole>::iterator Ait = ItemScoreValues.begin() ; Ait != ItemScoreValues.end() ; Ait++ )
            {
                if(ItemName.find((*Ait).first) != String::npos)
                    CurrScore += (*Ait).second;
            }
        }
        for( Whole R = 0 ; R < Removed.size() ; R++ )
        {
            String ItemName = Removed[R]->GetName();
            for( std::map<String,Whole>::iterator Rit = ItemScoreValues.begin() ; Rit != ItemScoreValues.end() ; Rit++ )
            {
                if(ItemName.find((*Rit).first) != String::npos)
                    CurrScore -= (*Rit).second;
            }
        }
    }
    return true;
}

bool CatchApp::PostRender()
{
	//Lets set a variable for the time
	static Whole gametime = 0;

	TheWorld->Log(String("---------- Starting PosterRender CallBack -------------"));
    TheWorld->Log(String("Current Game Time "));

	//getting a message from the event manager)
	EventRenderTime* CurrentTime = TheWorld->GetEventManager()->PopNextRenderTimeEvent();
    Whole LastFrame = 0;

    // Is CurrentTime a valid event?
    while(0 != CurrentTime)
    {
        LastFrame = CurrentTime->getMilliSecondsSinceLastFrame();

        TheWorld->Log(gametime);
        gametime+=CurrentTime->getMilliSecondsSinceLastFrame();

        delete CurrentTime;
        CurrentTime = TheWorld->GetEventManager()->GetNextRenderTimeEvent();
    }

    // Update the timer
    UI::Caption* Timer = TheWorld->GetUIManager()->GetScreen("GameScreen")->GetLayer("HUDLayer")->GetCaption("GS_Timer");
    std::stringstream time;
    Whole TotalSeconds = LevelTimer->GetCurrentTimeInMilli() / 1000;
    Whole Minutes = TotalSeconds / 60;
    Whole Seconds;
    if(60 > TotalSeconds)
    {
        Seconds = TotalSeconds;
    }else{
        Seconds = TotalSeconds % 60;
    }
    if(10 > Seconds)
    {
        time << Minutes << ":" << 0 << Seconds;
    }else{
        time << Minutes << ":" << Seconds;
    }
    Timer->SetText(time.str());

    // Update the score
    UI::Caption* ScoreAmount = TheWorld->GetUIManager()->GetScreen("GameScreen")->GetLayer("HUDLayer")->GetCaption("GS_ScoreArea");
    std::stringstream Score;
    Score << CurrScore;
    String ScoreOut = Score.str();
    ScoreAmount->SetText(ScoreOut);

    // Update Stat information
    UI::Caption* CurFPS = TheWorld->GetUIManager()->GetScreen("GameScreen")->GetLayer("StatsLayer")->GetCaption("CurFPS");
    UI::Caption* AvFPS = TheWorld->GetUIManager()->GetScreen("GameScreen")->GetLayer("StatsLayer")->GetCaption("AvFPS");
    std::stringstream CFPSstream;
    std::stringstream AFPSstream;
    CFPSstream << TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->GetLastFPS();
    AFPSstream << TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->GetAverageFPS();
    String CFPS = CFPSstream.str();
    String AFPS = AFPSstream.str();
    CurFPS->SetText(CFPS);
    AvFPS->SetText(AFPS);

    //See if the level is over
    if(AllStartZonesEmpty())
    {
        if(CheckEndOfLevel())
        {
            TheWorld->GetPhysicsManager()->PauseSimulation(true);
            TheWorld->GetUIManager()->GetLayer("ReportLayer")->Show();
        }
    }

    return true;
}

bool CatchApp::CheckForStuff()
{
    //this will either set the pointer to 0 or return a valid pointer to work with.
    EventUserInput* OneInput = TheWorld->GetEventManager()->PopNextUserInputEvent();

    //We check each Event
    while(0 != OneInput)
    {
        if(OneInput->GetType()!=EventBase::UserInput)
            { TheWorld->LogAndThrow("Trying to process a non-EventUserInput as an EventUserInput."); }

        //we check each MetaCode in each Event
        for (unsigned int c=0; c<OneInput->GetMetaCodeCount(); c++ )
        {
            //Is the key we just pushed ESCAPE
            if(MetaCode::KEY_ESCAPE == OneInput->GetMetaCode(c).GetCode() && MetaCode::BUTTON_PRESSING == OneInput->GetMetaCode(c).GetMetaValue())
                { return false; }
        }

        delete OneInput;
        OneInput = TheWorld->GetEventManager()->PopNextUserInputEvent();
    }

    EventGameWindow* OneWindowEvent = TheWorld->GetEventManager()->PopNextGameWindowEvent();
    while(0 != OneWindowEvent)
    {
        if(OneWindowEvent->GetType()!=EventBase::GameWindow)
            { TheWorld->LogAndThrow("Trying to process a non-EventGameWindow as an EventGameWindow."); }

        if(!OneWindowEvent->IsEventIDValid())
        {
            TheWorld->Log("Invalid EventID on GameWindow Event");
            TheWorld->LogAndThrow(OneWindowEvent->GetEventID());
        }

        TheWorld->Log(*OneWindowEvent);
        TheWorld->Log(EventGameWindow::GameWindowEventIDToString(OneWindowEvent->GetEventID()));
        stringstream eventxml;
        eventxml << *OneWindowEvent;    // Test XML conversion and reconstruction
        EventGameWindow AnotherWindowEvent(EventGameWindow::GAME_WINDOW_NONE,0,0);
        eventxml >> AnotherWindowEvent;
        TheWorld->Log(AnotherWindowEvent);

        if (OneWindowEvent->GetEventID()==EventGameWindow::GAME_WINDOW_MINIMIZED)
        {
            Sound* Welcome = NULL;
            Welcome = TheWorld->GetSoundManager()->GetSoundByName("Welcome");
            if(Welcome)
            {
                Welcome->Play2d(false);
            }
        }

        delete OneWindowEvent;
        OneWindowEvent = TheWorld->GetEventManager()->PopNextGameWindowEvent();
    }

    EventCollision* OneCollision = TheWorld->GetEventManager()->PopNextCollisionEvent();
    EventCollision SecondCollision(0,0,Vector3(0.5,0.5,0.5),Vector3(1.5,1.5,1.5),Vector3(1,1,1),2.5);
    while(0 != OneCollision)
    {
        if(OneCollision->GetType() != EventBase::Collision)
            { TheWorld->LogAndThrow("Trying to process a non-EventCollision as an EventCollision."); }

        stringstream temp;
        temp << *OneCollision;
        temp >> SecondCollision;
        TheWorld->Log(SecondCollision);

        delete OneCollision;
        OneCollision = TheWorld->GetEventManager()->PopNextCollisionEvent();
    }

    return true;
}

void CatchApp::SetPlayArea(AreaOfPlay* PlayArea)
{
    PlayZone = PlayArea;
}

void CatchApp::RegisterScoreArea(ScoreArea* Score)
{
    ScoreAreas.push_back(Score);
}

void CatchApp::RegisterStartArea(StartingArea* Start)
{
    StartAreas.push_back(Start);
}

void CatchApp::AddThrowable(ActorBase* Throwable)
{
    ThrownItems.push_back(Throwable);
}

#endif
