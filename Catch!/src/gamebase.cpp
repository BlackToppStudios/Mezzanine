#ifndef _gamebase_cpp
#define _gamebase_cpp
///////////////////////////////////////////////////////////////////////////////
// Gamewide Logic misc Features go here
///////////////////////////////////////////////////////////////////////////////

#include "gamebase.h"       //Game Include
#include "startingarea.h"
#include "scorearea.h"
#include <physgame.h>       //Physgame include
#include <sstream>          //STL includes

using namespace phys;

//Create the World Globally! and set it to hold some actors
World *TheWorld;
ActorBase* LastActorThrown;
StartingArea* StartZone;
ScoreArea* ScoreZone;
Whole CurrScore = 0;

const Plane PlaneOfPlay( Vector3(2.0,1.0,-5.0), Vector3(1.0,2.0,-5.0), Vector3(1.0,1.0,-5.0));

int main(int argc, char **argv)
{
    try
    {
        TheWorld = new World( Vector3(-1000.0,-1000.0,-1000.0), Vector3(1000.0,1000.0,1000.0), SceneManager::Generic, 50);
    }catch( exception x){
        //could not create world
    }

    // Set the Title
    TheWorld->SetWindowName("Catch!... The Game!");
    TheWorld->SetTargetFrameRate(60);

    //Give the world functions to run before and after input and physics
    TheWorld->GetEventManager()->SetPreMainLoopItems(&PreInput);
    TheWorld->GetEventManager()->SetPostMainLoopItems(&PostInput);
    TheWorld->GetPhysicsManager()->SetPreMainLoopItems(&PrePhysics);
    TheWorld->GetPhysicsManager()->SetPostMainLoopItems(&PostPhysics);
    TheWorld->GetGraphicsManager()->SetPostMainLoopItems(&PostRender);
    TheWorld->GetUIManager()->SetPreMainLoopItems(&PreUI);
    TheWorld->GetUIManager()->SetPostMainLoopItems(&PostUI);

    //Set the Make the RenderWindow and load system stuff
	TheWorld->GameInit(false);

	TheWorld->SetWindowName( "Catch!" );

    //Set up polling for the letter Q and middle mouse button, and the mouse X and Y locations
    TheWorld->GetEventManager()->AddPollingCheck( MetaCode(0, MetaCode::KEY_q) );
    TheWorld->GetEventManager()->AddPollingCheck( MetaCode(0, MetaCode::MOUSEBUTTON_3) );
    TheWorld->GetEventManager()->AddPollingCheck( MetaCode(0, MetaCode::MOUSEABSOLUTEHORIZONTAL) );

    //Actually Load the game stuff
    LoadContent();

    //Generate the UI
    MakeGUI();

    //Configure the wireframe Drawer
    TheWorld->GetPhysicsManager()->SetDebugPhysicsWireCount(2);
    TheWorld->GetPhysicsManager()->SetDebugPhysicsRendering(0);

    //Setup some light and configure the camera.
    //TheWorld->GetCameraManager()->GetDefaultCamera()->SetCameraType(Camera::Orthographic);
    TheWorld->GetSceneManager()->SetAmbientLight(1.0,1.0,1.0,1.0);

    WorldNode* CameraNode = TheWorld->GetSceneManager()->CreateOrbitingNode( "Orbit1", Vector3(0,0,0), Vector3(0.0,0.0,-250.0), true );
    CameraNode->AttachElement(TheWorld->GetCameraManager()->GetDefaultCamera());
    CameraNode->LookAt(Vector3(0,0,0));

	//Start the Main Loop
	TheWorld->MainLoop();

	return 0;
}

bool PostRender()
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
    CFPSstream << TheWorld->GetGraphicsManager()->GetLastFPS();
    AFPSstream << TheWorld->GetGraphicsManager()->GetAverageFPS();
    String CFPS = CFPSstream.str();
    String AFPS = AFPSstream.str();
    CurFPS->SetText(CFPS);
    AvFPS->SetText(AFPS);

    // Turn on the Wireframe
    if (30000<gametime)
        { TheWorld->GetPhysicsManager()->SetDebugPhysicsRendering(0); }

    //IF the game has gone on for 150 or more seconds close it.
	if (150000<gametime || (TheWorld->GetEventManager()->GetNextQuitEvent()!=0) )
        { return false; }
    /*
    if(//Perform check to see if the last actor thrown is within the limits of the level)
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

bool PrePhysics()
{
    return true;
}

bool PostPhysics()
{
    //// Updating functions to be used when a suitable mesh is found/created.
    //ActorSoft* ActS = static_cast< ActorSoft* > (TheWorld->Actors->FindActor("Column1"));
    //ActS->UpdateSoftBody();
    return true;
}

bool PreUI()
{
    return true;
}

bool PostUI()
{
    //User Input through a WorldQueryTool
    static WorldQueryTool Queryer;

    Queryer.GatherEvents();
    TheWorld->Log("Mouse location From WorldQueryTool X/Y");
    TheWorld->Log(Queryer.GetMouseX());
    TheWorld->Log(Queryer.GetMouseY());

    if( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_LEFT) )
        { TheWorld->GetSceneManager()->GetNode("Orbit1")->IncrementOrbit(-0.01); }

    if( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_RIGHT) )
        { TheWorld->GetSceneManager()->GetNode("Orbit1")->IncrementOrbit(0.01); }

    //Resize the window
    static bool videobuttonpushed = false;
    if ( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_f) && !videobuttonpushed )
    {
        videobuttonpushed = true;
        TheWorld->GetGraphicsManager()->setFullscreen(true);
    }
    else if ( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_g)  && !videobuttonpushed )
    {
        videobuttonpushed = true;
        TheWorld->GetGraphicsManager()->setFullscreen(false);
    }
    else if ( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_r)  && !videobuttonpushed )
    {
        videobuttonpushed = true;
        TheWorld->GetGraphicsManager()->setRenderResolution(1024,768);
    }
    else if ( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_t)  && !videobuttonpushed )
    {
        videobuttonpushed = true;
        TheWorld->GetGraphicsManager()->setRenderResolution(800,600);
    }
    else { videobuttonpushed = false; }

    static Point2PointConstraint* Dragger=NULL;

    if( Queryer.IsMouseButtonPushed(1) )
    {
        if(TheWorld->GetUIManager()->MouseIsInUISystem())
        {
            UIManager* UIMan = TheWorld->GetUIManager();
            UI::Button* MouseButton = UIMan->GetHoveredButton();
            if(!MouseButton && UIMan->GetHoveredWidget())
                MouseButton = UIMan->GetHoveredWidget()->GetHoveredButton();
            if(MouseButton)
            {
                MetaCode::ButtonState State = Queryer.GetMouseButtonState(1);
                if("Store" == MouseButton->GetName())
                {
                    String ItemShopL = "ItemShopLayer";
                    UILayer* Layer = UIMan->GetLayer(ItemShopL);
                    if(MetaCode::BUTTON_PRESSING == State)
                        Layer->SetVisible(!Layer->GetVisible());
                }
                else if("Menu" == MouseButton->GetName())
                {
                    String MenuL = "MenuLayer";
                    UILayer* Layer = UIMan->GetLayer(MenuL);
                    if(MetaCode::BUTTON_PRESSING == State)
                        Layer->SetVisible(!Layer->GetVisible());
                }
                else if("Return" == MouseButton->GetName())
                {
                    String MenuL = "MenuLayer";
                    UILayer* Layer = UIMan->GetLayer(MenuL);
                    Layer->Hide();
                }
                else if("Exit" == MouseButton->GetName())
                {
                    return false;
                }
            }
        }else{
            #ifdef PHYSDEBUG
            TheWorld->Log("Gamebase CLICK:");
            TheWorld->LogStream << "Camera Location: " << TheWorld->GetCameraManager()->GetDefaultCamera()->GetCameraGlobalLocation() << endl;
            #endif

            Ray *MouseRay = Queryer.GetMouseRay(5000);
            //*MouseRay *= 1000;
            //Ray *MouseRay = new Ray(Vector3(500.0, 0.0, 0.0),Vector3(-500.0, 0.0, 0.0));

            Vector3WActor *ClickOnActor = Queryer.GetFirstActorOnRayByPolygon( *MouseRay );
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
                            rigid->DisableDeactivation();
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
            Vector3 *DragTo = Queryer.RayPlaneIntersection(*MouseRay, PlaneOfPlay);
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
            Act->RestoreActivation();
        }
    }

    // using the Raw Event Manager, and deleting the events
    if( !CheckForEsc() )
        return false;
    return true;
}

bool PreInput()
{
    return true;
}

bool PostInput()
{
    return true;
}

///////////////////
//Non-Callbacks
bool CheckForEsc()
{
    //this will either set the pointer to 0 or return a valid pointer to work with.
    EventUserInput* OneInput = TheWorld->GetEventManager()->PopNextUserInputEvent();

    //We check each Event
    while(0 != OneInput)
    {
        #ifdef PHYSDEBUG
        TheWorld->Log("Input Events Processed");
        #endif

        //we check each MetaCode in each Event
        for (int c=0; c<OneInput->GetMetaCodeCount(); c++ )
        {
            #ifdef PHYSDEBUG
            TheWorld->LogStream << Metacode << "(" << c << ")" = << OneInput->GetMetaCode(c));
            #endif
            //Is the key we just pushed ESCAPE
            if(MetaCode::KEY_ESCAPE == OneInput->GetMetaCode(c).GetCode())
            {
                return false;
            }
        }

        delete OneInput;
        OneInput = TheWorld->GetEventManager()->PopNextUserInputEvent();
    }

    return true;
}

void LoadContent()
{
    String groupname ("Group1");
    TheWorld->GetResourceManager()->AddResourceLocation(crossplatform::GetDataDirectory(), "FileSystem", groupname, false);
    TheWorld->GetResourceManager()->InitResourceGroup(groupname);
/*
    ActorRigid *object1 = new ActorRigid (0,"Ferris","ferrisWheel.mesh","Group1");
    object1->CreateShapeFromMeshDynamic(3);
    //object1->SetInitLocation(Vector3(0,0,0));
    object1->SetInitOrientation(Quaternion(1.0, 0.0, 0.0, 0.55));
    TheWorld->GetActorManager()->AddActor(object1);
    TheWorld->GetPhysicsManager()->SetGravity(Vector3(0,0,0));
*/
}

void MakeGUI()
{
    String MainMenuScreen = "MainMenuScreen";
    String GameScreen = "GameScreen";
    String MenuLayer = "MenuLayer";
    String HUDLayer = "HUDLayer";
    String ItemShopLayer = "ItemShopLayer";
    String StatsLayer = "StatsLayer";
    UIManager* GUI = TheWorld->GetUIManager();
    GUI->LoadGorilla("Catch!");

    UIScreen* Screen = GUI->CreateScreen(GameScreen, "Catch!");
    UILayer* Menu = Screen->CreateLayer(MenuLayer, 10);
    UILayer* ItemShop = Screen->CreateLayer(ItemShopLayer, 4);
    UILayer* Stats = Screen->CreateLayer(StatsLayer, 1);
    UILayer* HUD = Screen->CreateLayer(HUDLayer, 0);

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

    UI::MarkupText* TestMarkup = ItemShopWindow->CreateMarkupText("TestMarkup",Vector2(0.4,0.64),14,"Test1");

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
#endif
