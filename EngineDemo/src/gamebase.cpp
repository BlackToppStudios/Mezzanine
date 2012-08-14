#ifndef _gamebase_cpp
#define _gamebase_cpp
///////////////////////////////////////////////////////////////////////////////
// This whole of the Engine demo code is terrible and disgusting...
// This exists so that we can see the limits in worst case terms, intentionally
// using linear alogrithm, ridiculous precision on physics models, and in
// general sloppy code. The is sort of a testbed for any idea that comes about
// and should not be imitated, ever.
///////////////////////////////////////////////////////////////////////////////

#include "gamebase.h"       //Game Include
#include <mezzanine.h>      //Mezzanine include

#include <iostream>
#include <fstream>            //Only used for testing


using namespace Mezzanine;

//Create the World.... Globally! and set it to hold some actors
World *TheWorld;

const Plane PlaneOfPlay( Vector3(2.0,1.0,-5.0), Vector3(1.0,2.0,-5.0), Vector3(1.0,1.0,-5.0));

ActorBase *Robot7, *Robot8;

int main(int argc, char **argv)
{
    try
    {
        PhysicsConstructionInfo Info;
        Info.PhysicsFlags = PhysicsConstructionInfo::PCF_SoftRigidWorld;
        Info.GeographyLowerBounds = Vector3(-30000.0,-30000.0,-30000.0);
        Info.GeographyUpperBounds = Vector3(30000.0,30000.0,30000.0);
        Info.MaxProxies = 60;
        TheWorld = new World( Info, "DefaultSceneManager", "plugins.cfg", "data/common/");
    }catch(...){
        return 1;
        //could not create the perfect worldending program
    }
    TheWorld->SetTargetFrameRate(60);

    //TheWorld->SetLoggingFrequency(World::LogOncePerXFrames,250); //Every 250 frames should be once every 5 seconds or so.
    //TheWorld->SetLoggingFrequency(World::LogOncePerXSeconds,5);
    //TheWorld->SetLoggingFrequency(World::LogNever);

    GraphicsManager* GraphMan = TheWorld->GetGraphicsManager();

    //Give the world functions to run before and after input and physics
    TheWorld->GetEventManager()->SetPreMainLoopItems(&PreInput);
    TheWorld->GetEventManager()->SetPostMainLoopItems(&PostInput);
    TheWorld->GetPhysicsManager()->SetPreMainLoopItems(&PrePhysics);
    TheWorld->GetPhysicsManager()->SetPostMainLoopItems(&PostPhysics);
    TheWorld->GetGraphicsManager()->SetPostMainLoopItems(&PostRender);

    //Create the windows!
    GameWindow* FirstWindow = GraphMan->CreateGameWindow("First",800,600,0);
    Camera* FirstCam = CameraManager::GetSingletonPtr()->CreateCamera("FirstCam");
    Viewport* FirstViewport = FirstWindow->CreateViewport(FirstCam);
    FirstCam->SetLocation( Vector3(0,50,200) );
    FirstCam->LookAt( Vector3(0,0,0) );

    GameWindow* SecondWindow = GraphMan->CreateGameWindow("Second",640,480,0);
    Camera* SecondCam = CameraManager::GetSingletonPtr()->CreateCamera("SecondCam");
    Viewport* SecondViewport = SecondWindow->CreateViewport(SecondCam);
    SecondCam->SetLocation( Vector3(-300,50,-50) );
    SecondCam->LookAt( Vector3(0,0,0) );

    //Init
	TheWorld->EngineInit(false);

    // Configure Shadows
    TheWorld->GetSceneManager()->SetSceneShadowTechnique(SceneManager::SST_Stencil_Additive);
    TheWorld->GetSceneManager()->SetShadowFarDistance(3000);

    //Set up polling for the letter Q and middle mouse button, and the mouse X and Y locations
    TheWorld->GetEventManager()->AddPollingCheck( MetaCode(0, MetaCode::KEY_Q) );
    TheWorld->GetEventManager()->AddPollingCheck( MetaCode(0, MetaCode::MOUSEBUTTON_3) );
    TheWorld->GetEventManager()->AddPollingCheck( MetaCode(0, MetaCode::MOUSEBUTTON_1) );
    TheWorld->GetEventManager()->RemovePollingCheck( MetaCode(0, MetaCode::MOUSEBUTTON_3) );
    //TheWorld->GetEventManager()->AddPollingCheck( MetaCode(0, MetaCode::MOUSEABSOLUTEHORIZONTAL) );   //Not supported

    //Actually Load the game stuff
    LoadContent();

    //Generate the UI
    MakeGUI();

    //Configure the wireframe Drawer
    //TheWorld->GetPhysicsManager()->SetDebugPhysicsWireCount(2);
    //TheWorld->GetPhysicsManager()->SetDebugPhysicsRendering(0);

    //Setup some camera tricks
    //WorldNode* CameraNode = TheWorld->GetSceneManager()->CreateOrbitingNode( "Orbit1", Vector3(0,0,0), Vector3(0.0,200.0,750.0), true );
    //CameraNode->AttachElement(TheWorld->GetCameraManager()->GetDefaultCamera());
    TheWorld->GetCameraManager()->GetCamera(0)->SetLocation(Vector3(0.0,200.0,150.0));
    CameraController* DefaultControl = TheWorld->GetCameraManager()->GetOrCreateCameraController(TheWorld->GetCameraManager()->GetCamera(0));
    DefaultControl->SetMovementMode(CameraController::CCM_Walk);
    DefaultControl->SetHoverHeight(75);
    Light *Headlight = TheWorld->GetSceneManager()->CreateLight("Headlight");
    Headlight->SetLightType(Light::Directional);
    Vector3 LightLoc(200,300,0);
    Headlight->SetLocation(LightLoc);
    LightLoc.X = -LightLoc.X;
    LightLoc.Y = -LightLoc.Y;
    LightLoc.Normalize();
    Headlight->SetDirection(LightLoc);
    Headlight->SetDiffuseColour(ColourValue(0.7,0.7,0.7,1.0));
    Headlight->SetSpecularColour(ColourValue(0.7,0.7,0.7,1.0));
    Headlight->SetLocation(Vector3(0,150,0));
    //Headlight->SetAttenuation(1000.0, 0.0, 1.0, 0.0);         //I couldn't get these to work
    //CameraNode->AttachObject(Headlight);

	//Start the Main Loop
	TheWorld->MainLoop();

    //delete TheWorld;
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

    ActorBase* Act1 = TheWorld->GetActorManager()->GetActor("RobotWayUpFrontLeft");
    ActorBase* Act2 = TheWorld->GetActorManager()->GetActor("RobotWayUpFrontRight");
    if (Act1->IsAnimated())
    {
        Act1->AdvanceAnimation((Real)0.001 * LastFrame);
    }

    if (Act2->IsAnimated())
    {
        Act2->AdvanceAnimation((Real)0.0001 * LastFrame);
    }

    static bool notplayed=true;
    //if (1000<gametime && notplayed)
    if (notplayed)
    {
        notplayed=false;
        Audio::Sound* Welcome = NULL;
        Welcome = TheWorld->GetAudioManager()->GetSoundByName("Welcome");
        if(Welcome)
        {
            Welcome->Play2d(false);
        }
        #ifdef MEZZDEBUG
        TheWorld->Log("Played Welcome Fun:");
        #endif

    }

    // Update Stat information
    UI::OpenRenderableContainerWidget* HUD = static_cast<UI::OpenRenderableContainerWidget*>(TheWorld->GetUIManager()->GetScreen("DefaultScreen")->GetWidget("D_HUD"));
    UI::Caption* CurFPS = static_cast<UI::Caption*>(HUD->GetAreaRenderable("D_CurFPS"));
    UI::Caption* AvFPS = static_cast<UI::Caption*>(HUD->GetAreaRenderable("D_AvFPS"));
    std::stringstream CFPSstream;
    std::stringstream AFPSstream;
    CFPSstream << TheWorld->GetGraphicsManager()->GetGameWindow(0)->GetLastFPS();
    AFPSstream << TheWorld->GetGraphicsManager()->GetGameWindow(0)->GetAverageFPS();
    String CFPS = CFPSstream.str();
    String AFPS = AFPSstream.str();
    CurFPS->SetText(CFPS);
    AvFPS->SetText(AFPS);
    // Update mouse positions
    UI::Caption* IMPos = static_cast<UI::Caption*>(HUD->GetAreaRenderable("D_IMPos"));
    std::stringstream IMPosstream;
    IMPosstream << InputQueryTool::GetMouseCoordinates().X << "," << InputQueryTool::GetMouseCoordinates().Y;
    String IMPosTex = IMPosstream.str();
    IMPos->SetText(IMPosTex);

    // Turn on the Wireframe
    if (30000<gametime)
        { /*TheWorld->GetPhysicsManager()->SetDebugPhysicsRendering(1);*/ }

    //IF the game has gone on for 150 or more seconds close it.
	if (150000<gametime || (TheWorld->GetEventManager()->GetNextQuitEvent()!=0) )
        { return false; }

    return true;
}

bool PrePhysics()
{
    TheWorld->Log("Object Locations");
    //Replace this with something that uses the actor container and logs the location of everything
    TheWorld->Log(TheWorld->GetActorManager()->GetActor("MetalSphere2")->GetLocation());
    return true;
}

bool PostPhysics()
{
    //// Updating functions to be used when a suitable mesh is found/created.
    /*ActorSoft* ActS = static_cast< ActorSoft* > (TheWorld->GetActorManager()->FindActor("spheresoft"));
    ActS->UpdateSoftBody();
    TheWorld->Log("Soft Locations");
    TheWorld->Log(TheWorld->GetActorManager()->FindActor("spheresoft")->GetLocation());
    if(ActS->IsInWorld())
        TheWorld->Log("Softbody is in World");
    else
        TheWorld->Log("Softbody is not in World");// */
    return true;
}

bool PreUI()
{
    return true;
}

bool PostUI()
{
    return true;
}

bool PreInput()
{
    // using the Raw Event Manager, and deleting the events
    if( !CheckForStuff() )
        return false;
    return true;
}

bool PostInput()
{
    //User Input through a WorldQueryTool
    static RayQueryTool* RayQueryer = new RayQueryTool();
    static InputQueryTool* InputQueryer = new InputQueryTool();

    //Queryer.GatherEvents();
    #ifdef MEZZDEBUG
    TheWorld->Log("Mouse location From WorldQueryTool X/Y: ");
    TheWorld->LogStream << InputQueryer->GetMouseX() << ", " << InputQueryer->GetMouseY() << std::endl
                        << InputQueryer->GetRawMetaValue(MetaCode::MOUSEABSOLUTEHORIZONTAL) << ", " << InputQueryer->GetRawMetaValue(MetaCode::MOUSEABSOLUTEVERTICAL) << std::endl
                        << InputQueryer->GetMouseCoordinates() << std::endl;
    TheWorld->Log("Relative Mouse location From WorldQueryTool X/Y: ");
    TheWorld->LogStream << InputQueryer->GetRawMetaValue(MetaCode::MOUSEHORIZONTAL) << ", " << InputQueryer->GetRawMetaValue(MetaCode::MOUSEVERTICAL) << std::endl
                        << InputQueryer->GetMousePrevFrameOffset() << std::endl;

    TheWorld->LogStream << "Default Camera:" << std::endl << *(TheWorld->GetCameraManager()->GetCamera(0)) << std::endl;
    Camera TempCam("TempCam",TheWorld->GetCameraManager());
    std::stringstream CamXML;
    CamXML << *(TheWorld->GetCameraManager()->GetCamera(0));
    CamXML >> TempCam;
    TheWorld->LogStream << "Default serial-deserialized, the finally reserialized Camera" << std::endl << TempCam << std::endl; // */
    #endif

//    if(320>Queryer.GetMouseX() && Queryer.IsMouseButtonPushed(3))
//        {TheWorld->Cameras->IncrementYOrbit(-0.01, TheWorld->Cameras->GetNodeAttachedToCamera() );}

//    if(320<Queryer.GetMouseX() && Queryer.IsMouseButtonPushed(3))
//        {TheWorld->Cameras->IncrementYOrbit(0.01, TheWorld->Cameras->GetNodeAttachedToCamera() );}
    CameraController* DefaultControl = TheWorld->GetCameraManager()->GetOrCreateCameraController(TheWorld->GetCameraManager()->GetCamera(0));
    if( InputQueryer->IsKeyboardButtonPushed(MetaCode::KEY_LEFT) || InputQueryer->IsJoystickHatPushedInDirection(MetaCode::DIRECTIONALMOTION_UPLEFT, false))
    {
        //TheWorld->GetSceneManager()->GetNode("Orbit1")->IncrementOrbit(-0.01);
        DefaultControl->StrafeLeft(300 * (TheWorld->GetFrameTime() * 0.001));
    }

    if( InputQueryer->IsKeyboardButtonPushed(MetaCode::KEY_RIGHT) || InputQueryer->IsJoystickHatPushedInDirection(MetaCode::DIRECTIONALMOTION_DOWNRIGHT, false))
    {
        //TheWorld->GetSceneManager()->GetNode("Orbit1")->IncrementOrbit(0.01);
        DefaultControl->StrafeRight(300 * (TheWorld->GetFrameTime() * 0.001));
    }

    if( InputQueryer->IsKeyboardButtonPushed(MetaCode::KEY_UP) || InputQueryer->IsJoystickHatPushedInDirection(MetaCode::DIRECTIONALMOTION_UPLEFT, true))
    {
        //TheWorld->GetCameraManager()->GetDefaultCamera()->ZoomCamera( -12.0 );
        DefaultControl->MoveForward(300 * (TheWorld->GetFrameTime() * 0.001));
    }

    if( InputQueryer->IsKeyboardButtonPushed(MetaCode::KEY_DOWN)  || InputQueryer->IsJoystickHatPushedInDirection(MetaCode::DIRECTIONALMOTION_DOWNRIGHT, true))
    {
        //TheWorld->GetCameraManager()->GetDefaultCamera()->ZoomCamera( 12.0 );
        DefaultControl->MoveBackward(300 * (TheWorld->GetFrameTime() * 0.001));
    }

    static bool MouseCam=false;
    if( InputQueryer->IsKeyboardButtonPushed(MetaCode::KEY_HOME) )
    {
        MouseCam=true;
//        TheWorld->GetEventManager()->StartRelativeMouseMode();
    }
    if( InputQueryer->IsKeyboardButtonPushed(MetaCode::KEY_END))
    {
        MouseCam=false;
//        TheWorld->GetEventManager()->EndRelativeMouseMode();
    }

    Vector2 Offset = InputQueryer->GetMousePrevFrameOffset();
    if( MouseCam && Vector2(0,0) != Offset )
        DefaultControl->Rotate(Offset.X * 0.01,Offset.Y * 0.01,0);

    if( InputQueryer->IsKeyboardButtonPushed(MetaCode::KEY_M) || InputQueryer->IsJoystickButtonPushed(1) )
    {
        Audio::Sound* Theme = TheWorld->GetAudioManager()->GetSoundByName("Theme2");
        if(!Theme->IsPlaying())
        {
            Theme->Play2d(false);
        }
    }

    if( InputQueryer->IsKeyboardButtonPushed(MetaCode::KEY_N) || InputQueryer->IsJoystickButtonPushed(2) )
    {
        Audio::Sound* Theme = TheWorld->GetAudioManager()->GetSoundByName("Theme2");
        if(Theme->IsPlaying())
        {
            Theme->Stop();
        }
    }

    // Make a declaration for a static constrain so it survives the function lifetime
    static Physics::Point2PointConstraint* Dragger=NULL;

    if( InputQueryer->IsMouseButtonPushed(1) )
    {
        UIManager* UIMan = UIManager::GetSingletonPtr();
        if(UIMan->MouseIsInUISystem())
        {
            UI::Screen* DScreen = UIMan->GetScreen("DefaultScreen");
            UI::Widget* Hover = UIMan->GetHoveredWidget();
            if(Hover)
                Hover = Hover->GetBottomMostHoveredWidget();
            if("D_MenuAccess" == Hover->GetName())
            {
                DScreen->GetWidget("D_Menu")->Show();
            }
            if("D_Return" == Hover->GetName())
            {
                DScreen->GetWidget("D_Menu")->Hide();
            }
            if("D_Exit" == Hover->GetName())
            {
                return false;
            }
        }else{
            Ray *MouseRay = RayQueryer->GetMouseRay(5000);
            // *MouseRay *= 1000;
            //Ray *MouseRay = new Ray(Vector3(500.0, 0.0, 0.0),Vector3(-500.0, 0.0, 0.0));

            Vector3WActor *ClickOnActor = RayQueryer->GetFirstActorOnRayByPolygon(*MouseRay,Mezzanine::WSO_ActorRigid);
            //ActorBase *temp = ClickOnActor->Actor;

            bool firstframe=false;
            if (0 == ClickOnActor || 0 == ClickOnActor->Actor)
            {
                #ifdef MEZZDEBUG
                TheWorld->Log("No Actor Clicked on");
                #endif
            }else{
                #ifdef MEZZDEBUG
                TheWorld->Log("Actor Clicked on"); TheWorld->Log(*ClickOnActor);
                TheWorld->Log("MouseRay"); TheWorld->Log(*MouseRay);
                TheWorld->Log("PlaneOfPlay"); TheWorld->Log(PlaneOfPlay);
                TheWorld->Log("ClickOnActor"); TheWorld->Log(*ClickOnActor);
                #endif
                if(!(ClickOnActor->Actor->IsStaticOrKinematic()))
                {
                    if(!Dragger) //If we have a dragger, then this is dragging, not clicking
                    {
                        if(ClickOnActor->Actor->GetType()==Mezzanine::WSO_ActorRigid) //This is Dragging let's do some checks for sanity
                        {
                            Vector3 LocalPivot = ClickOnActor->Vector;
                            ActorRigid* rigid = static_cast<ActorRigid*>(ClickOnActor->Actor);
                            rigid->GetPhysicsSettings()->SetActivationState(Physics::WOAS_DisableDeactivation);
                            //Dragger = new Generic6DofConstraint(rigid, LocalPivot, Quaternion(0,0,0,1), false);
                            Dragger = new Physics::Point2PointConstraint(rigid, LocalPivot);
                            Dragger->SetTAU(0.001);
                            TheWorld->GetPhysicsManager()->AddConstraint(Dragger);
                            Dragger->SetParam(Physics::Con_Stop_CFM,0.8,0); Dragger->SetParam(Physics::Con_Stop_CFM,0.8,1); Dragger->SetParam(Physics::Con_Stop_CFM,0.8,2);
                            Dragger->SetParam(Physics::Con_Stop_ERP,0.1,0); Dragger->SetParam(Physics::Con_Stop_ERP,0.1,1); Dragger->SetParam(Physics::Con_Stop_ERP,0.1,2);
                            firstframe=true;
                        }else{  // since we don't
                            #ifdef MEZZDEBUG
                            TheWorld->Log("Actor is not an ActorRigid.  Aborting.");
                            #endif
                        }
                    }
                }else{
                    #ifdef MEZZDEBUG
                    TheWorld->Log("Actor is Static/Kinematic.  Aborting.");
                    #endif
                }
            }

            // This chunk of code calculates the 3d point that the actor needs to be dragged to
            Vector3 *DragTo = RayQueryer->RayPlaneIntersection(*MouseRay, PlaneOfPlay);
            if (0 == DragTo)
            {
                #ifdef MEZZDEBUG
                TheWorld->Log("PlaneOfPlay Not Clicked on");
                #endif
            }else{
                if(Dragger && !firstframe)
                {
                    #ifdef MEZZDEBUG
                    TheWorld->Log("Dragged To");
                    TheWorld->Log(*DragTo);
                    #endif
                    //Dragger->SetOffsetALocation(*DragTo);
                    Dragger->SetPivotBLocation(*DragTo);
                }
            }

            // Here we cleanup everything we needed for the clicking/dragging
            delete DragTo;
            delete MouseRay;
        }

    }else{  //Since we are no longer clicking we need to setup for the next clicking
        if(Dragger)
        {
            ActorRigid* Act = Dragger->GetActorA();
            TheWorld->GetPhysicsManager()->RemoveConstraint(Dragger);
            delete Dragger;
            Dragger=NULL;
            Act->GetPhysicsSettings()->SetActivationState(Physics::WOAS_Active);
        }
    }
    return true;
}

///////////////////
//Non-Callbacks
bool CheckForStuff()
{
    //this will either set the pointer to 0 or return a valid pointer to work with.
    EventUserInput* OneInput = TheWorld->GetEventManager()->PopNextUserInputEvent();

    //We check each Event
    while(0 != OneInput)
    {
        #ifdef MEZZDEBUG
        TheWorld->LogStream << "Input Events Processed (Escape is " << MetaCode::KEY_ESCAPE << ") : " << std::endl;
        #endif

        if(OneInput->GetType()!=EventBase::UserInput)
            { MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Trying to process a non-EventUserInput as an EventUserInput."); }

        #ifdef MEZZDEBUG
        TheWorld->Log(*OneInput);
        EventUserInput ASecondInput;
        StringStream UserInputXML;
        UserInputXML << *OneInput;
        UserInputXML >> ASecondInput;
        TheWorld->Log(ASecondInput);
        #endif

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

    #ifdef MEZZDEBUG
    TheWorld->Log("All Game Window Changes This Frame");
    #endif
    EventGameWindow* OneWindowEvent = TheWorld->GetEventManager()->PopNextGameWindowEvent();
    while(0 != OneWindowEvent)
    {
        if(OneWindowEvent->GetType()!=EventBase::GameWindow)
            { MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Trying to process a non-EventGameWindow as an EventGameWindow."); }

        if(!OneWindowEvent->IsEventIDValid())
        {
            StringStream ExceptionStream;
            ExceptionStream << "Invalid EventID on GameWindow Event: " << OneWindowEvent->GetEventID() << std::endl;
            MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,ExceptionStream.str());
        }

        TheWorld->Log(*OneWindowEvent);
        TheWorld->Log(EventGameWindow::GameWindowEventIDToString(OneWindowEvent->GetEventID()));
        StringStream eventXML;
        eventXML << *OneWindowEvent;    // Test XML conversion and reconstruction
        EventGameWindow AnotherWindowEvent(EventGameWindow::GAME_WINDOW_NONE,0,0);
        eventXML >> AnotherWindowEvent;
        TheWorld->Log(AnotherWindowEvent);

        if (OneWindowEvent->GetEventID()==EventGameWindow::GAME_WINDOW_MINIMIZED)
        {
            Audio::Sound* Welcome = NULL;
            Welcome = TheWorld->GetAudioManager()->GetSoundByName("Welcome");
            if(Welcome)
            {
                Welcome->Play2d(false);
            }
        }

        delete OneWindowEvent;
        OneWindowEvent = TheWorld->GetEventManager()->PopNextGameWindowEvent();
    }

    #ifdef MEZZDEBUG
    TheWorld->Log("All Collisions This Frame");
    #endif
    EventCollision* OneCollision = TheWorld->GetEventManager()->PopNextCollisionEvent();
    while(0 != OneCollision)
    {
        if(OneCollision->GetType() != EventBase::Collision)
            { MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Trying to process a non-EventCollision as an EventCollision."); }
        delete OneCollision;
        OneCollision = TheWorld->GetEventManager()->PopNextCollisionEvent();
    }

    return true;
}

void LoadContent()
{
    TheWorld->GetSceneManager()->SetAmbientLight(0.10,0.10,0.10,0.10);

    ActorRigid *object1, *object2, *object3, *object4;
    ActorRigid *object5, *object6;
    ActorRigid *object7;
    //Ogre Setup Code
    String groupname ("Group1");
    String filerobot ("robot.mesh");
    String robotprefix ("Robot");

    Real mass=15.0;
    TheWorld->GetResourceManager()->AddAssetLocation("data/common", "FileSystem", groupname, false);
    TheWorld->GetResourceManager()->AddAssetLocation("data/common/Music", "FileSystem", groupname, false);
    TheWorld->GetResourceManager()->AddAssetLocation("data/common/Sounds", "FileSystem", groupname, false);
    //TheWorld->GetResourceManager()->AddAssetLocation(zipname.str(), "Zip", groupname, false);
    TheWorld->GetResourceManager()->AddAssetLocation("", "FileSystem", groupname, false);
    TheWorld->GetResourceManager()->InitAssetGroup(groupname);

    ParticleEffect *GreenPart = TheWorld->GetSceneManager()->CreateParticleEffect("GreenParticles", "Examples/GreenyNimbus");
    GreenPart->SetLocation(Vector3(-70,70,-100));

    CollisionShapeManager* CSMan = World::GetWorldPointer()->GetCollisionShapeManager();
    Physics::CollisionShape* RobitCH = CSMan->GenerateConvexHull("RobitConvexHull",filerobot,groupname);
    Physics::CollisionShape* RobitCD = CSMan->PerformConvexDecomposition("RobitConvexDecomp",filerobot,groupname,5.0,5.0,10.0);
    Physics::CollisionShape* PlaneStatic = CSMan->GenerateStaticTriMesh("PlaneShape","Plane.mesh",groupname);
    Physics::CollisionShape* WoodenSphere = new Physics::SphereCollisionShape("WoodSphere",250.0);
    Physics::CollisionShape* MetalSphere = new Physics::SphereCollisionShape("MetalSphere",250.0);
    Physics::CollisionShape* MetalSphere2 = new Physics::SphereCollisionShape("MetalSphere2",250.0);

    // Now Lets make some bowling pins
    Real PinSpacing=75.0;           //This is how far apart we want the pins
    ActorRigid* ActRig = NULL;
    for(unsigned int c=0; c<4; c++)     //make the back row
    {
        std::stringstream namestream;
        namestream << robotprefix << c;
        ActRig = new ActorRigid(mass,namestream.str(),filerobot,groupname);
        ActRig->GetPhysicsSettings()->SetCollisionShape(RobitCD);
        //TheWorld->GetResourceManager()->ImportShapeData(ActRig, "data/common/RobotDecomp3.bullet");
        ActRig->SetLocation(Vector3( (-2.0*PinSpacing)+(c*PinSpacing), -90.0, 0));
        TheWorld->GetActorManager()->AddActor( ActRig );
    }

    for(unsigned int c=0; c<3; c++)     //the row with three pins
    {
        std::stringstream namestream;
        namestream << robotprefix << (c+4);
        ActRig = new ActorRigid(mass,namestream.str(),filerobot,groupname);
        ActRig->GetPhysicsSettings()->SetCollisionShape(RobitCD);
        //TheWorld->GetResourceManager()->ImportShapeData(ActRig, "data/common/RobotDecomp3.bullet");
        ActRig->SetLocation(Vector3( (-1.5*PinSpacing)+(c*PinSpacing), -66.0, -PinSpacing));
        TheWorld->GetActorManager()->AddActor( ActRig );
    }
    //TheWorld->Resources->ImportShapeData(ActRig, "RobotDecomp3.bullet");

    for(unsigned int c=0; c<2; c++)     //the row with 2 pins
    {
        std::stringstream namestream;
        namestream << robotprefix << (c+7);
        ActRig = new ActorRigid(mass,namestream.str(),filerobot,groupname);
        ActRig->GetPhysicsSettings()->SetCollisionShape(RobitCH);
        ActRig->SetLocation(Vector3( (-PinSpacing)+(c*PinSpacing), -30.0, -PinSpacing*2));
        TheWorld->GetActorManager()->AddActor( ActRig );
        if (c+7==7)
            {Robot7=ActRig;}
        if (c+7==8)
            {Robot8=ActRig;}
    }

    std::stringstream namestream;           //make the front pin
    namestream << robotprefix << 9;
    ActRig = new ActorRigid (mass,namestream.str(),filerobot,groupname);
    ActRig->GetPhysicsSettings()->SetCollisionShape(RobitCH);
    ActRig->SetLocation(Vector3( (-0.5*PinSpacing), 0.0, -PinSpacing*3));
    TheWorld->GetActorManager()->AddActor( ActRig );

    //// The simulations soft body, to be used once a suitable mesh is found/created.
    /*MeshGenerator::CreateSphereMesh("SoftTest","SphereWood",70);
    ActorSoft* Act9 = new ActorSoft (500,"spheresoft","SoftTest",groupname);
    Act9->SetInitLocation(Vector3( -35, 100, -50));// */

    object5 = new ActorRigid(0,"Plane","Plane.mesh",groupname);
    object5->GetPhysicsSettings()->SetCollisionShape(PlaneStatic);
    object5->SetLocation(Vector3(0.0,-100,-300.0));

    object6 = new ActorRigid(0,"Ramp","Plane.mesh",groupname);
    object6->GetPhysicsSettings()->SetCollisionShape(PlaneStatic);
    object6->SetLocation(Vector3(00.0,300.0,-1100.0));
    object6->SetOrientation(Quaternion(0.5, 0.0, 0.0, -0.25));

    object1 = new ActorRigid(mass,"RobotWayUpFrontRight",filerobot,groupname);
    object1->GetPhysicsSettings()->SetCollisionShape(RobitCH);
    object1->SetLocation(Vector3(400,70,100));
    object1->SetOrientation(Quaternion(0.5, 0.5, 0.0, 0.9));
    object1->SetAnimation("Idle", true);
    object1->EnableAnimation(true);

    object2 = new ActorRigid(150.0f,"WoodSphere","Sphere_Wood.mesh",groupname);
    object2->GetPhysicsSettings()->SetCollisionShape(WoodenSphere);
    object2->SetScaling(Vector3(0.5,0.5,0.5));
    object2->SetLocation(Vector3(-140.0,2800.0,-1150.0));

    object3 = new ActorRigid(200.0f,"MetalSphere","Sphere_Metal.mesh",groupname);
    object3->GetPhysicsSettings()->SetCollisionShape(MetalSphere);
    object3->SetScaling(Vector3(0.7,0.7,0.7));
    object3->SetLocation(Vector3(150.0,1800.0,-1300.0));

    object4 = new ActorRigid(mass,"RobotWayUpFrontLeft",filerobot,groupname);
    object4->GetPhysicsSettings()->SetCollisionShape(RobitCD);
    object4->SetLocation(Vector3(-400,10, 100));
    object4->SetOrientation(Quaternion(0.5, 0.5, 0.0, 0.9));
    object4->SetAnimation("Idle", true);
    object4->EnableAnimation(true);

    object7 = new ActorRigid(800.0f,"MetalSphere2","Sphere_Metal.mesh",groupname);
    object7->GetPhysicsSettings()->SetCollisionShape(MetalSphere2);
    object7->SetScaling(Vector3(0.3,0.3,0.3));
    object7->SetLocation(Vector3(10.0,25000.0,-1300.0));
    object7->GetPhysicsSettings()->SetDamping(0.3,0.0);

    //Final Steps
    Vector3 grav;
    grav.X=0.0;
    grav.Y=-400.0;
    grav.Z=0.0;

    TheWorld->GetActorManager()->AddActor(object1);
    TheWorld->GetActorManager()->AddActor(object2);
    TheWorld->GetActorManager()->AddActor(object3);
    TheWorld->GetActorManager()->AddActor(object4);
    TheWorld->GetActorManager()->AddActor(object5);
    TheWorld->GetActorManager()->AddActor(object6);
    TheWorld->GetActorManager()->AddActor(object7);
    //TheWorld->GetActorManager()->AddActor(Act9);

    /*GravityField* Reverse = new GravityField(String("UpField"), Vector3(0.0,-100.0,0.0));
    Reverse->CreateCylinderShapeY(Vector3(100.0,200.0,100));
    Reverse->SetLocation(Vector3(200,50,-5.0));
    TheWorld->GetPhysicsManager()->AddAreaEffect(Reverse); // Now that we have passed it, we can forget about it*/

    /*GravityWell* BlackHole = new GravityWell("BlackHole", Vector3(0.0,200.0,-300.0));
    BlackHole->CreateSphereShape(750.0);
    BlackHole->SetAllowWorldGravity(false);
    BlackHole->SetFieldStrength(100000.0);
    BlackHole->SetAttenuation(85.0,GravityWell::GW_Att_Linear);
    BlackHole->CreateGraphicsSphere(ColourValue(0.9,0.7,0.7,0.55));
    TheWorld->GetPhysicsManager()->AddAreaEffect(BlackHole);// */

    Audio::Sound *sound1, *music1, *music2;
    AudioManager* AudioMan = TheWorld->GetAudioManager();
    AudioMan->CreateSoundSet("Announcer");
    sound1 = AudioMan->CreateDialogSound("Welcome", "welcomefun-1.ogg", groupname);
    AudioMan->AddSoundToSoundSet("Announcer", sound1);

    AudioMan->CreateSoundSet("SoundTrack");
    music1 = AudioMan->CreateMusicSound("Theme1", "cAudioTheme1.ogg", groupname);
    AudioMan->AddSoundToSoundSet("SoundTrack", music1);
    music2 = AudioMan->CreateMusicSound("Theme2", "cAudioTheme2.ogg", groupname);
    AudioMan->AddSoundToSoundSet("SoundTrack", music2);

    TheWorld->Log("Actor Count");
    TheWorld->Log( TheWorld->GetActorManager()->GetNumActors() );

    TheWorld->GetPhysicsManager()->SetGravity(grav);
    TheWorld->GetPhysicsManager()->SetSoftGravity(grav);
}

void MakeGUI()
{
    String DefaultScreen = "DefaultScreen";
    UIManager* GUI = TheWorld->GetUIManager();
    Viewport* UIViewport = TheWorld->GetGraphicsManager()->GetGameWindow(0)->GetViewport(0);
    GUI->LoadMTA("dejavu");
    UI::Screen* DScreen = GUI->CreateScreen(DefaultScreen, "dejavu", UIViewport);

    ColourValue Transparent(0.0,0.0,0.0,0.0);
    ColourValue Black(0.0,0.0,0.0,1.0);
    ColourValue TransBlack(0.0,0.0,0.0,0.3);

    ColourValue MenuButtonColour(0.3,0.4,0.8,1.0);
    ColourValue MenuColour(0.4,0.8,0.3,1.0);
    ColourValue ReturnColour(0.6,0.2,0.2,1.0);
    ColourValue ExitColour(0.6,0.2,0.2,1.0);

    //Build the HUD
    UI::OpenRenderableContainerWidget* HUDContainer = DScreen->CreateOpenRenderableContainerWidget("D_HUD");
    UI::Button* MenuButton = HUDContainer->CreateButton( "D_MenuAccess", UI::RenderableRect(Vector2(0.008, 0.935), Vector2(0.16, 0.065), true),(Whole)14,"Menu");
    MenuButton->GetClickable()->SetBackgroundColour(MenuButtonColour);
    UI::Caption* CurFPS = HUDContainer->CreateCaption( "D_CurFPS", UI::RenderableRect(Vector2(0.16, 0.06), Vector2(0.06, 0.065), true), (Whole)14, "0.0");
    CurFPS->SetBackgroundColour(Transparent);
    CurFPS->HorizontallyAlign(UI::Txt_Left);
    UI::Caption* CurFPSText = HUDContainer->CreateCaption( "D_CurFPSText", UI::RenderableRect(Vector2(0.008, 0.06), Vector2(0.15, 0.065), true), (Whole)14, "Current FPS: ");
    CurFPSText->SetBackgroundColour(Transparent);
    CurFPSText->HorizontallyAlign(UI::Txt_Left);
    UI::Caption* AvFPS = HUDContainer->CreateCaption( "D_AvFPS", UI::RenderableRect(Vector2(0.16, 0.105), Vector2(0.06, 0.065), true), (Whole)14, "0.0");
    AvFPS->SetBackgroundColour(Transparent);
    AvFPS->HorizontallyAlign(UI::Txt_Left);
    UI::Caption* AvFPSText = HUDContainer->CreateCaption( "D_AvFPSText", UI::RenderableRect(Vector2(0.008, 0.105), Vector2(0.15, 0.065), true), (Whole)14, "Average FPS: ");
    AvFPSText->SetBackgroundColour(Transparent);
    AvFPSText->HorizontallyAlign(UI::Txt_Left);
    UI::Caption* IMPos = HUDContainer->CreateCaption( "D_IMPos", UI::RenderableRect(Vector2(0.16, 0.15), Vector2(0.12, 0.065), true), (Whole)14, "0,0");
    IMPos->SetBackgroundColour(Transparent);
    IMPos->HorizontallyAlign(UI::Txt_Left);
    UI::Caption* IMPosText = HUDContainer->CreateCaption( "D_IMPosText", UI::RenderableRect(Vector2(0.008, 0.15), Vector2(0.15, 0.065), true), (Whole)14, "Mouse Pos: ");
    IMPosText->SetBackgroundColour(Transparent);
    IMPosText->HorizontallyAlign(UI::Txt_Left);
    DScreen->AddRootWidget(0,HUDContainer);
    //End of HUD
    //Build the Menu
    UI::Menu* DemoMenu = DScreen->CreateMenu( "D_Menu", UI::RenderableRect(Vector2(0.35, 0.27), Vector2(0.3, 0.45), true));
    DemoMenu->GetRootWindow()->GetWindowBack()->SetBackgroundColour(MenuColour);
    UI::Button* ReturnButton = DemoMenu->GetRootWindow()->CreateButton( "D_Return", UI::RenderableRect(Vector2(0.38, 0.56), Vector2(0.24, 0.05), true), (Whole)14, "Return To Demo");
    ReturnButton->GetClickable()->SetBackgroundColour(ReturnColour);
    UI::Button* GameExitButton = DemoMenu->GetRootWindow()->CreateButton( "D_Exit", UI::RenderableRect(Vector2(0.38, 0.64), Vector2(0.24, 0.05), true), (Whole)14, "Exit Demo");
    GameExitButton->GetClickable()->SetBackgroundColour(ExitColour);
    DScreen->AddRootWidget(1,DemoMenu);
    DemoMenu->Hide();
    //End of Menu
}

void TestsToSave()
{
    /*
    ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
    //testing the directory listing feature
    //please don;t delete this, this should be turned into a unit test.
    World::GetWorldPointer()->LogStream << std::endl << "Checking content of data directory:" << "data/common" << std::endl;
    std::set<String>* Listing = ResourceMan->GetDirContents("data/common");
    for (std::set<String>::iterator Iter = Listing->begin(); Iter!=Listing->end(); ++Iter)
    {
        World::GetWorldPointer()->LogStream << *Iter << std::endl;
    }
    delete Listing;

    World::GetWorldPointer()->LogStream << std::endl << "Checking content of working directory:" << ResourceMan->GetWorkingDirectory() << std::endl;
    Listing = ResourceMan->GetDirContents(ResourceMan->GetWorkingDirectory());
    for (std::set<String>::iterator Iter = Listing->begin(); Iter!=Listing->end(); ++Iter)
    {
        World::GetWorldPointer()->LogStream << *Iter << std::endl;
    }
    delete Listing;

    World::GetWorldPointer()->LogStream << std::endl << "Checking content of current directory:" << "." << std::endl;
    Listing = ResourceMan->GetDirContents(".");
    for (std::set<String>::iterator Iter = Listing->begin(); Iter!=Listing->end(); ++Iter)
    {
        World::GetWorldPointer()->LogStream << *Iter << std::endl;
    }
    delete Listing;

    World::GetWorldPointer()->LogStream << std::endl << "Checking content of parent directory:" << ".." << std::endl;
    Listing = ResourceMan->GetDirContents("..");
    for (std::set<String>::iterator Iter = Listing->begin(); Iter!=Listing->end(); ++Iter)
    {
        World::GetWorldPointer()->LogStream << *Iter << std::endl;
    }
    delete Listing;

    World::GetWorldPointer()->LogStream << std::endl << "Checking content of '/'" << std::endl;
    Listing = ResourceMan->GetDirContents("/");
    if (NULL == Listing)
    {
        World::GetWorldPointer()->LogStream << "/ not a valid directory" << std::endl;
    }else{
        for (std::set<String>::iterator Iter = Listing->begin(); Iter!=Listing->end(); ++Iter)
        {
            World::GetWorldPointer()->LogStream << *Iter << std::endl;
        }
        delete Listing;
    }

    World::GetWorldPointer()->LogStream << std::endl << "Checking content of 'c:/'" << std::endl;
    Listing = ResourceMan->GetDirContents("c:/");
    if (NULL == Listing)
    {
        World::GetWorldPointer()->LogStream << "c:/ not a valid directory" << std::endl;
    }else{
        for (std::set<String>::iterator Iter = Listing->begin(); Iter!=Listing->end(); ++Iter)
        {
            World::GetWorldPointer()->LogStream << *Iter << std::endl;
        }
        delete Listing;
    }

    {
        ResourceInputStream* XMLptr = ResourceMan->GetResourceStream("test.XML");
        ResourceInputStream* Zippedptr = ResourceMan->GetResourceStream("test.txt");

        // Test reading by character
        Whole Howmany=10000;
        char chararray[Howmany+1];
        for (Whole c=0; c<Howmany+1; c++)
            { chararray[c]='\0'; }
        XMLptr->read(chararray, Howmany);
        String ShouldHaveXML( chararray );

        for (Whole c=0; c<Howmany+1; c++)
            { chararray[c]='\0'; }
        Zippedptr->read(chararray, Howmany);
        String ZippedFileContents( chararray );

        char filearray[Howmany+1];
        for (Whole c=0; c<Howmany+1; c++)
            { filearray[c]='\0'; }

        std::ifstream TestFile("data/common/test.txt");
        TestFile.read(filearray, Howmany);
        String TestFileString(filearray);

        String StreamExtractionTest;
        TestFile >> StreamExtractionTest;

        #ifdef MEZZDEBUG
        TheWorld->LogStream << "ShouldHaveXML: " << std::endl << ShouldHaveXML << std::endl
                            << "ZippedFileContents: " << std::endl << ZippedFileContents << std::endl
                            << "File read from fstream: " << std::endl << TestFileString << std::endl ;
        #endif

        //if the above stuff was logged this should just work
        if( !Zippedptr->good() )
            { TheWorld->Log("Zippedptr corrupted/broken by read call"); }

        //this should just work
        Zippedptr->unget();
        if( !Zippedptr->good() )
            { TheWorld->Log("Zippedptr corrupted/broken by unget() call"); }
        else
        {
            Whole howfew = 500;
            char temp[howfew+1];
            for (Whole c=0; c<=howfew+1; c++)
                { temp[c]='\0'; }
            for (Whole c=0; c<howfew-1; c++)      //we already did one above -(1), we do not want to pad anything with a \0 (no +1), and we want exactly this many (< instead of <=)
                { Zippedptr->unget(); }
            Zippedptr->read(temp, howfew);
            TheWorld->LogStream << howfew << " ungets and " << howfew << " characters :" << temp;
        }

        TheWorld->LogStream << std::endl << "End first streaming test" << std::endl;

        try
            { XMLptr->putback('X'); }// as in :X
        catch (...)
            { throw; }

        delete XMLptr;
        delete Zippedptr;
    }

    {
        #ifdef MEZZDEBUG
        TheWorld->Log("Testing streaming on test.XML and zipped test.txt, Making fresh set of stream pointers");
        #endif

        ResourceInputStream* XMLptr = TheWorld->GetResourceManager()->GetResourceStream("test.XML");
        ResourceInputStream* Zippedptr = TheWorld->GetResourceManager()->GetResourceStream("test.txt");

        String Content("");
        (*XMLptr) >> Content;
        TheWorld->LogStream << "Extracted: " << Content << std::endl;

        if( !XMLptr->good() )
            { TheWorld->Log("XMLptr corrupted/broken by >> call"); }
        else
        {
            TheWorld->Log("XMLptr fine after >> call");
            TheWorld->Log("Getting some words with >> from Zippedptr:");
            for (Whole c=0; c<=5; c++)
            {
                String ZippedContent;
                (*Zippedptr) >> ZippedContent;
                TheWorld->LogStream << "Word " << c << ": " << ZippedContent;
                TheWorld->Log();
            }
        }

        delete XMLptr;
        delete Zippedptr;

        #ifdef MEZZDEBUG
        TheWorld->Log("End streaming test2");
        #endif
    }

    {
        #ifdef MEZZDEBUG
        TheWorld->Log("Testing streaming on test.XML with Mezzanine::XML::Document");
        #endif

        ResourceInputStream* XMLptr = TheWorld->GetResourceManager()->GetResourceStream("test.XML");
        XML::Document TestDoc;
        XML::ParseResult ParsedXML = TestDoc.Load( *XMLptr );

        TheWorld->LogStream << "XML::StatusOk :" << XML::StatusOk << std::endl
                            << "XML::StatusFileNotFound :" << XML::StatusFileNotFound << std::endl
                            << "XML::StatusIOError :" << XML::StatusIOError << std::endl
                            << "XML::StatusOutOfMemory :" << XML::StatusOutOfMemory << std::endl
                            << "XML::StatusInternalError :" << XML::StatusInternalError << std::endl
                            << "XML::StatusUnrecognizedTag :" << XML::StatusUnrecognizedTag << std::endl
                            << "XML::StatusBadPi :" << XML::StatusBadPi << std::endl
                            << "XML::StatusBadComment :" << XML::StatusBadComment << std::endl
                            << "XML::StatusBadCdata :" << XML::StatusBadCdata << std::endl
                            << "XML::StatusBadDoctype :" << XML::StatusBadDocType << std::endl
                            << "XML::StatusBadPcdata :" << XML::StatusBadPcdata << std::endl
                            << "XML::StatusBadStartElement :" << XML::StatusBadStartElement << std::endl
                            << "XML::StatusBadAttribute :" << XML::StatusBadAttribute << std::endl
                            << "XML::StatusBadEndElement :" << XML::StatusBadEndElement << std::endl
                            << "XML::StatusEndElementMismatch :" << XML::StatusEndElementMismatch << std::endl
                            ;



        if(ParsedXML.Status)
        {
            TheWorld->Log("Successfully loaded and parsed test.XML");
        }else{
            TheWorld->LogStream << "Failed to load and parse test.XML. With Status :" << ParsedXML.Status;
        }

        TheWorld->Log("XML Streaming Test");

        Vector2 ASinglePoint(1,2);
        TheWorld->Log("ASinglePoint:");
        TheWorld->Log(ASinglePoint);

        TheWorld->Log("Streaming ASinglePoint to String Stream:");
        StringStream XMLStringStream;
        XMLStringStream << ASinglePoint;
        TheWorld->Log(XMLStringStream.str());

        Vector2 ASecondPoint(0,0);
        TheWorld->Log("ASecondPoint:");
        TheWorld->Log(ASecondPoint);

        TheWorld->Log("ReStreaming ASinglePoint from stringStream, to ASecondPoint:");
        XMLStringStream >> ASecondPoint;
        TheWorld->Log(ASecondPoint);

        Plane ASinglePlane(Vector3(1.34,23,1.004),4.5);
        TheWorld->Log("ASinglePlane:");
        TheWorld->Log(ASinglePlane);

        TheWorld->Log("Streaming ASinglePlane to String Stream:");
        StringStream XMLStringStream2;
        XMLStringStream2 << ASinglePlane;
        TheWorld->Log(XMLStringStream2.str());

        Plane ASecondPlane(Vector3(0,0,0),0);
        TheWorld->Log("ASecondPlane:");
        TheWorld->Log(ASecondPlane);

        TheWorld->Log("ReStreaming ASinglePlane from stringStream, to ASecondPlane:");
        XMLStringStream2 >> ASecondPlane;
        TheWorld->Log(ASecondPlane);

        MetaCode ASingleCode(MetaCode(1,MetaCode::MOUSEBUTTON));
        TheWorld->Log("ASingleCode:");
        TheWorld->Log(ASingleCode);

        TheWorld->Log("Streaming ASingleCode to String Stream:");
        StringStream XMLStringStream3;
        XMLStringStream3 << ASingleCode;
        TheWorld->Log(XMLStringStream3.str());

        MetaCode ASecondCode(MetaCode(0,MetaCode::KEY_FIRST));
        TheWorld->Log("ASecondCode:");
        TheWorld->Log(ASecondCode);

        EventQuit AndOneEventToRuleThemAll;
        TheWorld->Log("An EventQuit AndOneEventToRuleThemAll:");
        TheWorld->Log(AndOneEventToRuleThemAll);
        StringStream XMLStringStream4;
        XMLStringStream4 << AndOneEventToRuleThemAll;
        XMLStringStream4 >> AndOneEventToRuleThemAll;

        EventRenderTime TimingEvent(55);
        EventRenderTime TimingEvent2(0);
        TheWorld->Log("EventRenderTime TimingEvent and TimingEvent2:");
        TheWorld->Log(TimingEvent);
        TheWorld->Log(TimingEvent2);
        StringStream XMLStringStream5;
        XMLStringStream5 << TimingEvent;
        XMLStringStream5 >> TimingEvent2;
        TheWorld->Log("EventRenderTime TimingEvent and TimingEvent2 After streaming:");
        TheWorld->Log(TimingEvent);
        TheWorld->Log(TimingEvent2);

        TheWorld->Log("ReStreaming ASingleCode from stringStream, to ASingleCode:");
        XMLStringStream3 >> ASecondCode;
        TheWorld->Log(ASecondCode);

        ColourValue AColour(1.0,1.0,1.0,1.0);
        ColourValue AnotherColour(0.5,0.5,0.5,0.5);
        TheWorld->Log("A colour:");
        TheWorld->Log(AColour);
        TheWorld->Log("Another colour:");
        TheWorld->Log(AnotherColour);
        StringStream XMLStringStream7;
        XMLStringStream7 << AColour;
        XMLStringStream7 >> AnotherColour;
        TheWorld->Log("Another colour after streaming A colour to it:");
        TheWorld->Log(AColour);


        #ifdef MEZZDEBUG
        TheWorld->Log("End of testing XML and Streaming test 3");
        #endif
    }
    //*/
}
//*/
#endif
