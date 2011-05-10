#ifndef _catchapp_cpp
#define _catchapp_cpp

#include "catchapp.h"
#include "callbacks.h"
#include <cassert>

using namespace phys;

CatchApp* CatchApp::TheRealCatchApp = 0;

CatchApp::CatchApp(const Vector3 &WorldLowerBounds, const Vector3 &WorldUpperBounds, SceneManager::SceneManagerType SceneType, const unsigned short int &MaxPhysicsProxies)
    : CurrScore(0),
      LastActorThrown(NULL),
      CurrentState(CatchApp::Catch_Loading),
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
}

CatchApp::~CatchApp()
{
    CatchApp::TheRealCatchApp = NULL;
    //delete TheWorld;
}

void CatchApp::LoadContent()
{
    // Get managers
    PhysicsManager* PhysMan = TheWorld->GetPhysicsManager();
    SceneManager* SceneMan = TheWorld->GetSceneManager();
    ResourceManager* ResourceMan = TheWorld->GetResourceManager();

    // Camera Setup
	Camera* DefCamera = TheWorld->GetCameraManager()->GetDefaultCamera();
	DefCamera->SetLocation(Vector3(0,0,425));
	DefCamera->LookAt(Vector3(0,0,0));

	// Lights Setup
    SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);

    // Physics Setup
    PhysMan->SetGravity(Vector3(0,-1000,0));

    //Configure the wireframe Drawer
    //PhysMan->SetDebugPhysicsWireCount(2);
    //PhysMan->SetDebugPhysicsRendering(1);

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.
    String groupname("Group1");
    String datadir = crossplatform::GetDataDirectory();
    ResourceMan->AddResourceLocation(datadir, "FileSystem", groupname, false);
    ResourceMan->AddResourceLocation(datadir+"Common.zip", "Zip", groupname, false);
    ResourceMan->AddResourceLocation(datadir+"Ferris.zip", "Zip", groupname, false);
    ResourceMan->AddResourceLocation(datadir+"AdvThrowables.zip", "Zip", groupname, false);
    ResourceMan->InitResourceGroup(groupname);

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,120),Vector3(15,-10,120),Vector3(0,10,120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", groupname, 3, 3);

    // Create the Wheel
    ActorRigid* FerrisWheel = new ActorRigid (100.0,"FerrisWheel","wheel.mesh",groupname);
    TheWorld->GetResourceManager()->ImportShapeData(FerrisWheel,"ferris_wheel.bullet");
    FerrisWheel->SetLocation(100,0,0);
    TheWorld->GetActorManager()->AddActor(FerrisWheel);

    // Create the trayz
    Real TrayMass = 50.0;
    ActorRigid* Tray1 = new ActorRigid(TrayMass,"Tray1","tray.mesh",groupname);
    TheWorld->GetResourceManager()->ImportShapeData(Tray1,"ferris_tray.bullet");
    PhysMan->StorePhysicsShape(Tray1,"TrayShape");
    Tray1->SetLocation(30.4,14.3,0);
    Tray1->SetDamping(0,0.1);
    TheWorld->GetActorManager()->AddActor(Tray1);
    ActorRigid* Tray2 = new ActorRigid(TrayMass,"Tray2","tray.mesh",groupname);
    PhysMan->ApplyPhysicsShape(Tray2,"TrayShape");
    Tray2->SetLocation(169.6,14.3,0);
    Tray2->SetDamping(0,0.1);
    TheWorld->GetActorManager()->AddActor(Tray2);
    ActorRigid* Tray3 = new ActorRigid(TrayMass,"Tray3","tray.mesh",groupname);
    PhysMan->ApplyPhysicsShape(Tray3,"TrayShape");
    Tray3->SetLocation(30.4,-42.7,0);
    Tray3->SetDamping(0,0.1);
    TheWorld->GetActorManager()->AddActor(Tray3);
    ActorRigid* Tray4 = new ActorRigid(TrayMass,"Tray4","tray.mesh",groupname);
    PhysMan->ApplyPhysicsShape(Tray4,"TrayShape");
    Tray4->SetLocation(169.6,-42.7,0);
    Tray4->SetDamping(0,0.1);
    TheWorld->GetActorManager()->AddActor(Tray4);
    ActorRigid* Tray5 = new ActorRigid(TrayMass,"Tray5","tray.mesh",groupname);
    PhysMan->ApplyPhysicsShape(Tray5,"TrayShape");
    Tray5->SetLocation(71.5,55.4,0);
    Tray5->SetDamping(0,0.1);
    TheWorld->GetActorManager()->AddActor(Tray5);
    ActorRigid* Tray6 = new ActorRigid(TrayMass,"Tray6","tray.mesh",groupname);
    PhysMan->ApplyPhysicsShape(Tray6,"TrayShape");
    Tray6->SetLocation(128.5,55.4,0);
    Tray6->SetDamping(0,0.1);
    TheWorld->GetActorManager()->AddActor(Tray6);
    ActorRigid* Tray7 = new ActorRigid(TrayMass,"Tray7","tray.mesh",groupname);
    PhysMan->ApplyPhysicsShape(Tray7,"TrayShape");
    Tray7->SetLocation(71.5,-83.8,0);
    Tray7->SetDamping(0,0.1);
    TheWorld->GetActorManager()->AddActor(Tray7);
    ActorRigid* Tray8 = new ActorRigid(TrayMass,"Tray8","tray.mesh",groupname);
    PhysMan->ApplyPhysicsShape(Tray8,"TrayShape");
    Tray8->SetLocation(128.5,-83.8,0);
    Tray8->SetDamping(0,0.1);
    TheWorld->GetActorManager()->AddActor(Tray8);// */

    // Create world anchor for the wheel, which will allow it to spin.
    HingeConstraint* WheelAnchor = new HingeConstraint(FerrisWheel,Vector3(0,0,0),Vector3(0,0,1),true);
    PhysMan->AddConstraint(WheelAnchor,false);// */

    // Create the series of hinges for connecting the 8 trays to the wheel
    HingeConstraint* Tray1Anchor = new HingeConstraint(FerrisWheel,Tray1,Vector3(-69.6,28.5,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(Tray1Anchor,true);
    HingeConstraint* Tray2Anchor = new HingeConstraint(FerrisWheel,Tray2,Vector3(69.6,28.5,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(Tray2Anchor,true);
    HingeConstraint* Tray3Anchor = new HingeConstraint(FerrisWheel,Tray3,Vector3(-69.6,-28.5,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(Tray3Anchor,true);
    HingeConstraint* Tray4Anchor = new HingeConstraint(FerrisWheel,Tray4,Vector3(69.6,-28.5,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(Tray4Anchor,true);
    HingeConstraint* Tray5Anchor = new HingeConstraint(FerrisWheel,Tray5,Vector3(-28.5,69.6,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(Tray5Anchor,true);
    HingeConstraint* Tray6Anchor = new HingeConstraint(FerrisWheel,Tray6,Vector3(28.5,69.6,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(Tray6Anchor,true);
    HingeConstraint* Tray7Anchor = new HingeConstraint(FerrisWheel,Tray7,Vector3(-28.5,-69.6,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(Tray7Anchor,true);
    HingeConstraint* Tray8Anchor = new HingeConstraint(FerrisWheel,Tray8,Vector3(28.5,-69.6,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(Tray8Anchor,true);// */

    // Create some throwable objects
    Real UraniumMass = 20.0;
    Real LeadMass = 30.0;
    Real ClayMass = 12.0;
/*    ActorRigid* Uranium1 = new ActorRigid(UraniumMass,"Uranium1","uranium.mesh",groupname);
    Uranium1->CreateShapeFromMeshDynamic(1);
    Uranium1->SetLocation(25,40,0);
    TheWorld->GetActorManager()->AddActor(Uranium1);
    ActorRigid* Uranium2 = new ActorRigid(UraniumMass,"Uranium2","uranium.mesh",groupname);
    Uranium2->CreateShapeFromMeshDynamic(1);
    Uranium2->SetLocation(-25,40,0);
    TheWorld->GetActorManager()->AddActor(Uranium2);// */
 /*   ActorRigid* Lead1 = new ActorRigid(LeadMass,"Lead1","lead.mesh",groupname);
    Lead1->CreateShapeFromMeshDynamic(1);
    Lead1->SetLocation(25,-0,0);
    TheWorld->GetActorManager()->AddActor(Lead1);
    ActorRigid* Lead2 = new ActorRigid(LeadMass,"Lead2","lead.mesh",groupname);
    Lead2->CreateShapeFromMeshDynamic(1);
    Lead2->SetLocation(-25,-0,0);
    TheWorld->GetActorManager()->AddActor(Lead2);// */
 /*   ActorRigid* Clay1 = new ActorRigid(ClayMass,"Clay1","clay_pot.mesh",groupname);
    Clay1->CreateShapeFromMeshDynamic(1);
    Clay1->SetLocation(25,-40,0);
    TheWorld->GetActorManager()->AddActor(Clay1);
    ActorRigid* Clay2 = new ActorRigid(ClayMass,"Clay2","clay_pot.mesh",groupname);
    Clay2->CreateShapeFromMeshDynamic(1);
    Clay2->SetLocation(-25,-40,0);
    TheWorld->GetActorManager()->AddActor(Clay2);// */

    // Create the zones
    PlayZone = new AreaOfPlay("PlayArea",Vector3(0,0,0));
    PlayZone->CreateBoxShape(Vector3(280,280,35));
    PhysMan->AddAreaEffect(PlayZone);// */

    StartZone = new StartingArea("StartArea",Vector3(-170,0,0));
    StartZone->CreateBoxShape(Vector3(50,130,15));
    StartZone->CreateGraphicsBox(ColourValue(0.1,0.8,0.1,0.2));
    PhysMan->AddAreaEffect(StartZone);// */

    ScoreZone = new ScoreArea("ScoreArea",Vector3(100,0,0));
    ScoreZone->CreateBoxShape(Vector3(100,100,30));
    ScoreZone->CreateGraphicsBox(ColourValue(0.2,0.2,0.8,0.2));
    PhysMan->AddAreaEffect(ScoreZone);// */
}

void CatchApp::MakeGUI()
{
    String MainMenuScreen = "MainMenuScreen";
    String GameScreen = "GameScreen";
    String MenuLayer = "MenuLayer";
    String HUDLayer = "HUDLayer";
    String ItemShopLayer = "ItemShopLayer";
    String StatsLayer = "StatsLayer";
    UIManager* GUI = TheWorld->GetUIManager();
    Viewport* UIViewport = TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->GetViewport(0);
    GUI->LoadGorilla("Catch!");

    UI::Screen* screen = GUI->CreateScreen(GameScreen, "Catch!", UIViewport);
    UI::Layer* Menu = screen->CreateLayer(MenuLayer, 10);
    UI::Layer* ItemShop = screen->CreateLayer(ItemShopLayer, 4);
    UI::Layer* Stats = screen->CreateLayer(StatsLayer, 1);
    UI::Layer* HUD = screen->CreateLayer(HUDLayer, 0);

    ColourValue Transparent(0.0,0.0,0.0,0.0);
    ColourValue Black(0.0,0.0,0.0,1.0);
    ColourValue TransBlack(0.0,0.0,0.0,0.3);

    //Build the Game Screen
    //Build the HUD layer
    UI::Caption* Timer = HUD->CreateCaption( "Timer", Vector2(0.8995, 0.006), Vector2(0.0965, 0.06), 20, "0:00");
    Timer->SetBackgroundSprite("TimerArea");

    UI::Rectangle* TIcon = HUD->CreateRectangle( Vector2(0.8515, 0.006), Vector2(0.0482, 0.06));
    TIcon->SetBackgroundSprite("TimerLogo");

    UI::Button* MenuButton = HUD->CreateButton( "Menu", Vector2(0.008, 0.922), Vector2(0.16, 0.065));
    MenuButton->SetBackgroundSprite("MenuButtonHiRes");

    UI::Button* StoreButton = HUD->CreateButton( "Store", Vector2(0.922, 0.922), Vector2(0.06, 0.065));
    StoreButton->SetBackgroundSprite("StoreButtonHiRes");

    UI::Rectangle* StoreText = HUD->CreateRectangle( Vector2(0.767, 0.922), Vector2(0.14, 0.065));
    StoreText->SetBackgroundSprite("StoreText");

    UI::Caption* ScoreAmount = HUD->CreateCaption( "ScoreArea", Vector2(0.135, 0.006), Vector2(0.15, 0.065), 20, "0");
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
    UI::Button* ReturnButton = GameMenu->GetRootWindow()->CreateButton( "Return", Vector2(0.37, 0.56), Vector2(0.26, 0.05));
    ReturnButton->SetBackgroundSprite("ReturnButton");
    UI::Button* ExitButton = GameMenu->GetRootWindow()->CreateButton( "Exit", Vector2(0.37, 0.64), Vector2(0.26, 0.05));
    ExitButton->SetBackgroundSprite("ExitButton");

    UI::Button* VideoAccess = GameMenu->GetRootWindow()->CreateAccessorButton("VideoSettingsButton", Vector2(0.37, 0.32), Vector2(0.26, 0.05));
    UI::MenuWindow* VideoSettings = GameMenu->GetRootWindow()->CreateChildMenuWindow("VideoSettings", Vector2(0.18, 0.22), Vector2(0.64, 0.55), VideoAccess);
    VideoSettings->GetWindowBack()->SetBackgroundSprite("WindowHoriBack");
    VideoAccess->SetBackgroundSprite("VideoSetButton");

    UI::Button* VideoBack = VideoSettings->CreateBackButton(Vector2(0.72, 0.705), Vector2(0.09, 0.05));
    VideoBack->SetBackgroundColour(Black);

    UI::Button* SoundAccess = GameMenu->GetRootWindow()->CreateAccessorButton("SoundSettingsButton", Vector2(0.37, 0.40), Vector2(0.26, 0.05));
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
    //End of Game Screen
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

    //Set logging frequency
    TheWorld->SetLoggingFrequency(World::LogNever);

    //Set the Make the RenderWindow and load system stuff
	TheWorld->GameInit(false);

	// Set the Title
    TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->SetWindowCaption("Catch!");
    TheWorld->SetTargetFrameRate(60);

    //Actually Load the game stuff
    LoadContent();

    //Generate the UI
    MakeGUI();

	//Start the Main Loop
	TheWorld->MainLoop();

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
    static bool videobuttonpushed = false;
    if ( InputQueryer->IsKeyboardButtonPushed(MetaCode::KEY_F) && !videobuttonpushed )
    {
        videobuttonpushed = true;
        TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->setFullscreen(true);
    }
    else if ( InputQueryer->IsKeyboardButtonPushed(MetaCode::KEY_G)  && !videobuttonpushed )
    {
        videobuttonpushed = true;
        TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->setFullscreen(false);
    }
    else if ( InputQueryer->IsKeyboardButtonPushed(MetaCode::KEY_R)  && !videobuttonpushed )
    {
        videobuttonpushed = true;
        TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->setRenderResolution(1024,768);
    }
    else if ( InputQueryer->IsKeyboardButtonPushed(MetaCode::KEY_T)  && !videobuttonpushed )
    {
        videobuttonpushed = true;
        TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->setRenderResolution(800,600);
    }
    else { videobuttonpushed = false; }

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
                if("Store" == MouseButton->GetName())
                {
                    String ItemShopL = "ItemShopLayer";
                    UI::Layer* layer = UIMan->GetLayer(ItemShopL);
                    if(MetaCode::BUTTON_PRESSING == State)
                        layer->SetVisible(!layer->IsVisible());
                }
                else if("Menu" == MouseButton->GetName())
                {
                    String MenuL = "MenuLayer";
                    UI::Layer* layer = UIMan->GetLayer(MenuL);
                    if(MetaCode::BUTTON_PRESSING == State)
                        layer->SetVisible(!layer->IsVisible());
                }
                else if("Return" == MouseButton->GetName())
                {
                    String MenuL = "MenuLayer";
                    UI::Layer* layer = UIMan->GetLayer(MenuL);
                    layer->Hide();
                }
                else if("Exit" == MouseButton->GetName())
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
            if (0 == ClickOnActor || 0 == ClickOnActor->Actor)
            {
                #ifdef PHYSDEBUG
                TheWorld->Log("No Actor Clicked on");
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
    UI::Caption* Timer = TheWorld->GetUIManager()->GetScreen("GameScreen")->GetLayer("HUDLayer")->GetCaption("Timer");
    std::stringstream time;
    Whole TotalSeconds = gametime/1000;
    Whole Minutes = TotalSeconds/60;
    Whole Seconds;
    if(60>TotalSeconds)
    {
        Seconds = TotalSeconds;
    }else{
        Seconds = TotalSeconds%60;
    }
    if(10>Seconds)
    {
        time << Minutes << ":" << 0 << Seconds;
    }else{
        time << Minutes << ":" << Seconds;
    }
    Timer->SetText(time.str());

    // Update the score
    UI::Caption* ScoreAmount = TheWorld->GetUIManager()->GetScreen("GameScreen")->GetLayer("HUDLayer")->GetCaption("ScoreArea");
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

    /*if(//Perform check to see if the last actor thrown is within the limits of the level)
    {
        //If it's not within the limits
        LastActorThrown = NULL;
    }

    if(StartZone->IsEmpty())
    {
        if(NULL==LastActorThrown)
        {
            if(ScoreZone->AllObjectsAtRest())
            {
                //end the game somehow, show the final score screen.
            }
        }
    }
    */
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

#endif
