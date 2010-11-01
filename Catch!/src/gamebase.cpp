#ifndef _gamebase_cpp
#define _gamebase_cpp
///////////////////////////////////////////////////////////////////////////////
// Gamewide Logic misc Features go here
///////////////////////////////////////////////////////////////////////////////

#include "gamebase.h"       //Game Include
#include "startingarea.h"
#include "scorearea.h"
#include "itemshoplist.h"
#include <physgame.h>       //Physgame include
#include <sstream>          //STL includes

using namespace phys;

//Create the World Globally! and set it to hold some actors
World *TheWorld;
ItemShopList* Items;
ActorBase* LastActorThrown;
StartingArea* StartZone;
ScoreArea* ScoreZone;
Whole CurrScore = 0;

const Plane PlaneOfPlay( Vector3(2.0,1.0,-5.0), Vector3(1.0,2.0,-5.0), Vector3(1.0,1.0,-5.0));

int main(int argc, char **argv)
{
    try
    {
        TheWorld = new World( Vector3(-1000.0,-1000.0,-1000.0), Vector3(1000.0,1000.0,1000.0), "SceneManager", SceneManager::Generic, 50);
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

    //Set the Make the RenderWindow and load system stuff
	TheWorld->GameInit(false);

	TheWorld->SetWindowName( "Catch!" );

    //Set up polling for the letter Q and middle mouse button, and the mouse X and Y locations
    TheWorld->GetEventManager()->AddPollingCheck( MetaCode(0, 1, MetaCode::KEY_q) );
    TheWorld->GetEventManager()->AddPollingCheck( MetaCode(0, 3, MetaCode::MOUSEBUTTON) );
    TheWorld->GetEventManager()->AddPollingCheck( MetaCode(0, 0, MetaCode::MOUSEABSOLUTEHORIZONTAL) );

    //Actually Load the game stuff
    LoadContent();

    //Generate the UI
    MakeGUI();

    //Configure the wireframe Drawer
    TheWorld->GetPhysicsManager()->SetDebugPhysicsWireCount(2);
    TheWorld->GetPhysicsManager()->SetDebugPhysicsRendering(0);

    //Setup some light and configure the camera.
    TheWorld->GetCameraManager()->GetDefaultCamera()->SetCameraType(Camera::Orthographic);
    TheWorld->GetSceneManager()->SetAmbientLight(1.0,1.0,1.0,1.0);

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
    UIButton* Timer = TheWorld->GetUIManager()->GetScreen("DefaultScreen")->GetLayer("HUDLayer")->GetButton("Timer");
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
    UIButton* ScoreAmount = TheWorld->GetUIManager()->GetScreen("DefaultScreen")->GetLayer("HUDLayer")->GetButton("ScoreArea");
    std::stringstream Score;
    string ScoreOut;
    Score << CurrScore;
    Score >> ScoreOut;
    ScoreAmount->SetText(ScoreOut);

    // Update Stat information
    UIButton* CurFPS = TheWorld->GetUIManager()->GetScreen("DefaultScreen")->GetLayer("StatsLayer")->GetButton("CurFPS");
    UIButton* AvFPS = TheWorld->GetUIManager()->GetScreen("DefaultScreen")->GetLayer("StatsLayer")->GetButton("AvFPS");
    std::stringstream CFPSstream;
    std::stringstream AFPSstream;
    CFPSstream << TheWorld->GetGraphicsManager()->GetLastFPS();
    AFPSstream << TheWorld->GetGraphicsManager()->GetAverageFPS();
    string CFPS;
    string AFPS;
    CFPSstream >> CFPS;
    AFPSstream >> AFPS;
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

bool PreInput()
{
    return true;
}

bool PostInput()
{
    //User Input through a WorldQueryTool
    static WorldQueryTool Queryer(TheWorld);

    Queryer.GatherEvents();
    TheWorld->Log("Mouse location From WorldQueryTool X/Y");
    TheWorld->Log(Queryer.GetMouseX());
    TheWorld->Log(Queryer.GetMouseY());

    static Point2PointConstraint* Dragger=NULL;

    if( Queryer.IsMouseButtonPushed(1) )
    {
        UIButton* MouseButton = NULL;
        Whole MouseX = Queryer.GetMouseX();
        Whole MouseY = Queryer.GetMouseY();
        UIScreen* Screen = TheWorld->GetUIManager()->GetScreen("DefaultScreen");
        for(Whole x=0 ; x != Screen->GetNumLayers() ; x++ )
        {
            UILayer* Layer = Screen->GetLayer(x);
            MouseButton = Layer->GetButtonMouseIsOver(MouseX, MouseY);
            if(MouseButton)
            {
                break;
            }
        }
        if(MouseButton)
        {
            if("Store" == MouseButton->GetName())
            {
                UILayer* Layer = Screen->GetLayer("ItemShopLayer");
                Layer->Show();
            }
            if("Menu" == MouseButton->GetName())
            {
                UILayer* Layer = Screen->GetLayer("MenuLayer");
                Layer->Show();
            }
            if("Return" == MouseButton->GetName())
            {
                UILayer* Layer = Screen->GetLayer("MenuLayer");
                Layer->Hide();
            }
            if("Exit" == MouseButton->GetName())
            {
                return false;
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

    /*ActorRigid *object1 = new ActorRigid (10,"Robot","robot.mesh","Group1",TheWorld);
    object1->CreateShapeFromMeshDynamic(1);
    object1->SetInitLocation(Vector3(0,0,0));
    object1->SetInitOrientation(Quaternion(0.5, 0.5, 0.0, 0.9));
    TheWorld->GetActorManager()->AddActor(object1);
    TheWorld->GetPhysicsManager()->SetGravity(Vector3(0,0,0));

    TheWorld->GetCameraManager()->GetDefaultCamera()->SetLocation(Vector3(0,0,-500));
    TheWorld->GetCameraManager()->GetDefaultCamera()->LookAt(Vector3(0,0,0));*/
}

void MakeGUI()
{
    String DefaultScreen = "DefaultScreen";
    String MenuLayer = "MenuLayer";
    String HUDLayer = "HUDLayer";
    String ItemShopLayer = "ItemShopLayer";
    String StatsLayer = "StatsLayer";
    UIManager* GUI = TheWorld->GetUIManager();
    Real WHeight = (Real)(TheWorld->GetGraphicsManager()->getRenderHeight());
    Real WWidth = (Real)(TheWorld->GetGraphicsManager()->getRenderWidth());
    GUI->LoadGorilla("Catch!");

    UIScreen* Screen = GUI->CreateScreen(DefaultScreen, "Catch!");
    UILayer* Menu = Screen->CreateLayer(MenuLayer, 10);
    UILayer* ItemShop = Screen->CreateLayer(ItemShopLayer, 4);
    UILayer* Stats = Screen->CreateLayer(StatsLayer, 1);
    UILayer* HUD = Screen->CreateLayer(HUDLayer, 0);

    //Build the HUD layer
    UIButton* Timer = HUD->CreateButton( "Timer", WWidth * 0.8995, WHeight * 0.006, WWidth * 0.0965, WHeight * 0.06, 20, "0:00");
    Timer->SetAsCaption(true);
    Timer->HorizontallyAlign(UIButton::Middle);
    Timer->VerticallyAlign(UIButton::Center);
    Timer->SetBackgroundSprite("TimerArea");

    UIRectangle* TIcon = HUD->CreateRectangle( WWidth * 0.8515, WHeight * 0.006, WWidth * 0.0482, WHeight * 0.06);
    TIcon->SetBackgroundSprite("TimerLogo");

    UIButton* MenuButton = HUD->CreateButton( "Menu", WWidth * 0.008, WHeight * 0.922, WWidth * 0.16, WHeight * 0.06, 20, " ");
    MenuButton->SetBackgroundSprite("MenuButton");

    UIButton* StoreButton = HUD->CreateButton( "Store", WWidth * 0.922, WHeight * 0.922, WWidth * 0.065, WHeight * 0.065, 10, " ");
    StoreButton->SetBackgroundSprite("StoreButton");

    UIRectangle* StoreText = HUD->CreateRectangle( WWidth * 0.767, WHeight * 0.922, WWidth * 0.14, WHeight * 0.065);
    StoreText->SetBackgroundSprite("StoreText");

    UIButton* ScoreAmount = HUD->CreateButton( "ScoreArea", WWidth * 0.135, WHeight * 0.006, WWidth * 0.15, WHeight * 0.065, 20, "0");
    //ScoreAmount->SetBackgroundSprite("ScoreCashArea");
    ColourValue Transparent(0.0,0.0,0.0,0.0);
    ScoreAmount->SetBackgroundColour(Transparent);
    ScoreAmount->SetAsCaption(true);
    ScoreAmount->HorizontallyAlign(UIButton::Middle);
    ScoreAmount->VerticallyAlign(UIButton::Center);

    UIRectangle* ScoreText = HUD->CreateRectangle( WWidth * 0.008, WHeight * 0.006, WWidth * 0.12, WHeight * 0.06);
    ScoreText->SetBackgroundSprite("ScoreText");

    //Build the ItemShop Layer
    Items = new ItemShopList(WWidth * 0.1, WHeight * 0.075, WWidth * 0.4, WHeight * 0.6, ItemShop, TheWorld);
    ColourValue Color(0.8,0.8,0.95,0.9);
    Items->GetBackdrop()->SetBackgroundColour(Color);
    ItemShop->Hide();

    //Build the Menu Layer
    UIRectangle* MenuBackground = Menu->CreateRectangle( WWidth * 0.25, WHeight * 0.15,
                                                         WWidth * 0.5, WHeight * 0.7 );
    ColourValue Colours(0.4,0.8,0.3,1.0);
    MenuBackground->SetBackgroundColour(Colours);
    UIButton* ReturnButton = Menu->CreateButton( "Return", WWidth * 0.30, WHeight * 0.61,
                                            WWidth * 0.4, WHeight * 0.08,
                                            1, "Return to Game");
    ReturnButton->HorizontallyAlign(UIButton::Middle);
    ReturnButton->VerticallyAlign(UIButton::Center);
    Colours = ColourValue(0.6,0.2,0.2,1.0);
    ReturnButton->SetBackgroundColour(Colours);
    UIButton* ExitButton = Menu->CreateButton( "Exit", WWidth * 0.30, WHeight * 0.73,
                                            WWidth * 0.4, WHeight * 0.08,
                                            1, "Exit Game");
    ExitButton->HorizontallyAlign(UIButton::Middle);
    ExitButton->VerticallyAlign(UIButton::Center);
    Colours = ColourValue(0.6,0.2,0.2,1.0);
    ExitButton->SetBackgroundColour(Colours);
    Menu->Hide();

    //Misc Extra's
    UIButton* CurFPS = Stats->CreateButton( "CurFPS", WWidth * 0.16, WHeight * 0.06, WWidth * 0.06, WHeight * 0.065, 14, "0.0");
    CurFPS->SetBackgroundColour(Transparent);
    CurFPS->SetAsCaption(true);
    CurFPS->HorizontallyAlign(UIButton::Left);
    CurFPS->VerticallyAlign(UIButton::Center);

    UIButton* CurFPSText = Stats->CreateButton( "CurFPSText", WWidth * 0.008, WHeight * 0.06, WWidth * 0.15, WHeight * 0.065, 14, "Current FPS: ");
    CurFPSText->SetBackgroundColour(Transparent);
    CurFPSText->SetAsCaption(true);
    CurFPSText->HorizontallyAlign(UIButton::Left);
    CurFPSText->VerticallyAlign(UIButton::Center);

    UIButton* AvFPS = Stats->CreateButton( "AvFPS", WWidth * 0.16, WHeight * 0.105, WWidth * 0.06, WHeight * 0.065, 14, "0.0");
    AvFPS->SetBackgroundColour(Transparent);
    AvFPS->SetAsCaption(true);
    AvFPS->HorizontallyAlign(UIButton::Left);
    AvFPS->VerticallyAlign(UIButton::Center);

    UIButton* AvFPSText = Stats->CreateButton( "AvFPSText", WWidth * 0.008, WHeight * 0.105, WWidth * 0.15, WHeight * 0.065, 14, "Average FPS: ");
    AvFPSText->SetBackgroundColour(Transparent);
    AvFPSText->SetAsCaption(true);
    AvFPSText->HorizontallyAlign(UIButton::Left);
    AvFPSText->VerticallyAlign(UIButton::Center);
    //Stats->Hide();
}
#endif
