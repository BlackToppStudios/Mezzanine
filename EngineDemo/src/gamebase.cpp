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

class DemoPreEventWorkUnit;
class DemoPostInputWorkUnit;
class DemoPostRenderWorkUnit;
class DemoPostPhysicsWorkUnit;

// ©reate the Entresol.... Globally! and set it to hold some actors
Entresol* TheEntresol = NULL;

const Plane PlaneOfPlay( Vector3(2.0,1.0,-5.0), Vector3(1.0,2.0,-5.0), Vector3(1.0,1.0,-5.0));

std::vector<Audio::iSound*>* Announcer = NULL;
std::vector<Audio::iSound*>* Soundtrack = NULL;

RigidDebris* Robot7 = NULL;
RigidDebris* Robot8 = NULL;

DemoPreEventWorkUnit* DemoPreEventWork = NULL;
DemoPostInputWorkUnit* DemoPostInputWork = NULL;
DemoPostRenderWorkUnit* DemoPostRenderWork = NULL;
DemoPostPhysicsWorkUnit* DemoPostPhysicsWork = NULL;

class DemoPreEventWorkUnit : public Threading::DefaultWorkUnit
{
public:
    DemoPreEventWorkUnit() {  }
    virtual ~DemoPreEventWorkUnit() {  }

    void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
    {
        //this will either set the pointer to 0 or return a valid pointer to work with.
        EventUserInput* OneInput = TheEntresol->GetEventManager()->PopNextUserInputEvent();

        //We check each Event
        while(0 != OneInput)
        {
            if(OneInput->GetType()!=EventBase::UserInput)
                { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Trying to process a non-EventUserInput as an EventUserInput."); }

            //we check each MetaCode in each Event
            for (unsigned int c=0; c<OneInput->GetMetaCodeCount(); c++ )
            {
                //Is the key we just pushed ESCAPE
                if(Input::KEY_ESCAPE == OneInput->GetMetaCode(c).GetCode() && Input::BUTTON_PRESSING == OneInput->GetMetaCode(c).GetMetaValue())
                    { TheEntresol->BreakMainLoop(); }
            }

            delete OneInput;
            OneInput = TheEntresol->GetEventManager()->PopNextUserInputEvent();
        }

        EventGameWindow* OneWindowEvent = TheEntresol->GetEventManager()->PopNextGameWindowEvent();
        while(0 != OneWindowEvent)
        {
            if(OneWindowEvent->GetType()!=EventBase::GameWindow)
                { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Trying to process a non-EventGameWindow as an EventGameWindow."); }

            if(!OneWindowEvent->IsEventIDValid())
            {
                StringStream ExceptionStream;
                ExceptionStream << "Invalid EventID on GameWindow Event: " << OneWindowEvent->GetEventID() << std::endl;
                MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,ExceptionStream.str());
            }

            if (OneWindowEvent->GetEventID()==EventGameWindow::GAME_WINDOW_MINIMIZED)
            {
                Audio::iSound* Welcome = NULL;
                Welcome = Announcer->front();
                if(Welcome)
                    { Welcome->Play(); }
            }

            delete OneWindowEvent;
            OneWindowEvent = TheEntresol->GetEventManager()->PopNextGameWindowEvent();
        }
    }
};//DemoPreEventWorkUnit

class DemoPostInputWorkUnit : public Threading::DefaultWorkUnit
{
        RayQueryTool RayCaster;
    public:
        DemoPostInputWorkUnit() {  }
        virtual ~DemoPostInputWorkUnit() {  }

        void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
        {
            //User Input through a WorldQueryTool
            Input::InputManager* InputMan = Input::InputManager::GetSingletonPtr();
            Input::Mouse* SysMouse = InputMan->GetSystemMouse();
            Input::Keyboard* SysKeyboard = InputMan->GetSystemKeyboard();
            Input::Controller* Controller1 = NULL;
            if( InputMan->GetNumControllers() > 0 )
                Controller1 = InputMan->GetController(0);

            CameraController* DefaultControl = TheEntresol->GetCameraManager()->GetOrCreateCameraController(TheEntresol->GetCameraManager()->GetCamera(0));
            if( SysKeyboard->IsButtonPressed(Input::KEY_LEFT) || (Controller1 ? Controller1->IsHatPushedInDirection(1,Input::CONTROLLERHAT_LEFT) : false) )
                { DefaultControl->StrafeLeft(300 * (TheEntresol->GetLastFrameTimeMilliseconds() * 0.001)); }

            if( SysKeyboard->IsButtonPressed(Input::KEY_RIGHT) || (Controller1 ? Controller1->IsHatPushedInDirection(1,Input::CONTROLLERHAT_RIGHT) : false) )
                { DefaultControl->StrafeRight(300 * (TheEntresol->GetLastFrameTimeMilliseconds() * 0.001)); }

            if( SysKeyboard->IsButtonPressed(Input::KEY_UP) || (Controller1 ? Controller1->IsHatPushedInDirection(1,Input::CONTROLLERHAT_UP) : false) )
                { DefaultControl->MoveForward(300 * (TheEntresol->GetLastFrameTimeMilliseconds() * 0.001)); }

            if( SysKeyboard->IsButtonPressed(Input::KEY_DOWN)  || (Controller1 ? Controller1->IsHatPushedInDirection(1,Input::CONTROLLERHAT_DOWN) : false) )
                { DefaultControl->MoveBackward(300 * (TheEntresol->GetLastFrameTimeMilliseconds() * 0.001)); }

            static bool MouseCam = false;
            if( SysKeyboard->IsButtonPressed(Input::KEY_HOME) )
                { MouseCam = true; }

            if( SysKeyboard->IsButtonPressed(Input::KEY_END))
                { MouseCam = false; }


            if( SysKeyboard->IsButtonPressed(Input::KEY_BACKSLASH) )
                { Entresol::GetSingletonPtr()->GetPhysicsManager()->SetTimeMultiplier(1.0); }

            if( SysKeyboard->IsButtonPressed(Input::KEY_RIGHTBRACKET) )
            {
                Real Factor = Entresol::GetSingletonPtr()->GetPhysicsManager()->GetTimeMultiplier() * 1.5;
                if(Factor>2.0)
                    { Factor = 2.0; }
                Entresol::GetSingletonPtr()->GetPhysicsManager()->SetTimeMultiplier(Factor);
            }

            if( SysKeyboard->IsButtonPressed(Input::KEY_LEFTBRACKET) )
            {
                Real Factor = Entresol::GetSingletonPtr()->GetPhysicsManager()->GetTimeMultiplier() * .75;
                if(Factor<0.01)
                    { Factor = 0.01; }
                Entresol::GetSingletonPtr()->GetPhysicsManager()->SetTimeMultiplier(Factor);
            }

            Vector2 Offset = SysMouse->GetMouseDelta();
            if( MouseCam && Vector2(0,0) != Offset )
                DefaultControl->Rotate(Offset.X * 0.01,Offset.Y * 0.01,0);

            if( SysKeyboard->IsButtonPressed(Input::KEY_M) || (Controller1 ? Controller1->IsButtonPressed(1) : false) )
            {
                Audio::iSound* Theme = Soundtrack->at(1);
                if(!Theme->IsPlaying())
                    { Theme->Play(); }
            }

            if( SysKeyboard->IsButtonPressed(Input::KEY_N) || (Controller1 ? Controller1->IsButtonPressed(2) : false) )
            {
                Audio::iSound* Theme = Soundtrack->at(1);
                if(Theme->IsPlaying())
                    { Theme->Stop(); }
            }

            // Make a declaration for a static constrain so it survives the function lifetime
            static Physics::Point2PointConstraint* Dragger=NULL;

            if( SysMouse->IsButtonPressed(1) )
            {
                UI::UIManager* UIMan = UI::UIManager::GetSingletonPtr();
                if(UIMan->MouseIsInUISystem())
                {
                    //UI::Screen* DScreen = UIMan->GetScreen("DefaultScreen");
                    UI::Widget* Hover = UIMan->GetHoveredWidget();
                    if(Hover) {
                        Hover = Hover->GetBottomMostHoveredWidget();
                        if( "D_Exit" == Hover->GetName() ) {
                            TheEntresol->BreakMainLoop();
                        }
                    }
                }else{
                    Ray MouseRay = RayQueryTool::GetMouseRay(5000);
                    RayCaster.GetFirstObjectOnRayByPolygon(MouseRay,Mezzanine::WO_DebrisRigid);

                    bool firstframe=false;
                    if (0 == RayCaster.LastQueryResultsObjectPtr())
                    {
                        #ifdef MEZZDEBUG
                        //TheEntresol->Log("No Object Clicked on");
                        #endif
                    }else{
                        #ifdef MEZZDEBUG
                        //TheEntresol->Log("Object Clicked on"); TheEntresol->Log(*ClickOnActor);
                        //TheEntresol->Log("MouseRay"); TheEntresol->Log(*MouseRay);
                        //TheEntresol->Log("PlaneOfPlay"); TheEntresol->Log(PlaneOfPlay);
                        //TheEntresol->Log("ClickOnActor"); TheEntresol->Log(*ClickOnActor);
                        #endif
                        if( !( RayCaster.LastQueryResultsObjectPtr()->IsStatic() || RayCaster.LastQueryResultsObjectPtr()->IsKinematic() ) )
                        {
                            if(!Dragger) //If we have a dragger, then this is dragging, not clicking
                            {
                                if(RayCaster.LastQueryResultsObjectPtr()->GetType() == Mezzanine::WO_DebrisRigid) //This is Dragging let's do some checks for sanity
                                {
                                    Vector3 LocalPivot = RayCaster.LastQueryResultsOffset();
                                    RigidDebris* rigid = static_cast<RigidDebris*>(RayCaster.LastQueryResultsObjectPtr());
                                    rigid->GetRigidProxy()->SetActivationState(Physics::AS_DisableDeactivation);
                                    //Dragger = new Generic6DofConstraint(rigid, LocalPivot, Quaternion(0,0,0,1), false);
                                    Dragger = new Physics::Point2PointConstraint(rigid->GetRigidProxy(), LocalPivot);
                                    Dragger->SetTAU(0.001);
                                    TheEntresol->GetPhysicsManager()->AddConstraint(Dragger);
                                    Dragger->SetParam(Physics::Con_Stop_CFM,0.8,-1); Dragger->SetParam(Physics::Con_Stop_CFM,0.8,-1); Dragger->SetParam(Physics::Con_Stop_CFM,0.8,-1);
                                    Dragger->SetParam(Physics::Con_Stop_ERP,0.1,-1); Dragger->SetParam(Physics::Con_Stop_ERP,0.1,-1); Dragger->SetParam(Physics::Con_Stop_ERP,0.1,-1);
                                    firstframe=true;
                                }else{  // since we don't
                                    #ifdef MEZZDEBUG
                                    //TheEntresol->Log("Object is not an ActorRigid.  Aborting.");
                                    #endif
                                }
                            }
                        }else{
                            #ifdef MEZZDEBUG
                            //TheEntresol->Log("Object is Static/Kinematic.  Aborting.");
                            #endif
                        }
                    }

                    // This chunk of code calculates the 3d point that the actor needs to be dragged to
                    if (RayCaster.RayPlaneIntersection(MouseRay, PlaneOfPlay))
                    {
                        if(Dragger&&!firstframe)
                            { Dragger->SetPivotBLocation(RayCaster.LastQueryResultsOffset()); }
                    }
                }

            }else{  //Since we are no longer clicking we need to setup for the next clicking
                if(Dragger)
                {
                    Physics::RigidProxy* Prox = Dragger->GetProxyA();
                    TheEntresol->GetPhysicsManager()->RemoveConstraint(Dragger);
                    delete Dragger;
                    Dragger = NULL;
                    Prox->SetActivationState(Physics::AS_Active);
                }
            }
        }
};//DemoPostInputWorkUnit

class DemoPostRenderWorkUnit : public Threading::DefaultWorkUnit
{
public:
    DemoPostRenderWorkUnit() {  }
    virtual ~DemoPostRenderWorkUnit() {  }

    void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
    {
        /*RigidDebris* Deb1 = TheEntresol->GetActorManager()->GetActor("RobotWayUpFrontLeft");
        RigidDebris* Deb2 = TheEntresol->GetActorManager()->GetActor("RobotWayUpFrontRight");
        if( Deb1->IsAnimated() )
        {
            Deb1->AdvanceAnimation((Real)0.001 * TheEntresol->GetLastFrameTimeMilliseconds() );
        }

        if( Deb2->IsAnimated() )
        {
            Deb2->AdvanceAnimation((Real)0.0001 * TheEntresol->GetLastFrameTimeMilliseconds() );
        }//*/

        static bool notplayed = true;
        //if (1000<gametime && notplayed)
        if( notplayed )
        {
            notplayed = false;
            Audio::iSound* Welcome = NULL;
            Welcome = Announcer->front();
            if(Welcome)
                { Welcome->Play(); }
            #ifdef MEZZDEBUG
            //TheEntresol->Log("Played Welcome Fun:");
            #endif

        }

        // Update Stat information
        UI::Screen* DScreen = TheEntresol->GetUIManager()->GetScreen("DefaultScreen");
        UI::Widget* CurFPS = static_cast<UI::Widget*>(DScreen->GetWidget("D_CurFPS"));
        UI::Widget* AvFPS = static_cast<UI::Widget*>(DScreen->GetWidget("D_AvFPS"));
        StringStream CFPSstream;
        StringStream AFPSstream;
        CFPSstream << TheEntresol->GetGraphicsManager()->GetGameWindow(0)->GetLastFPS();
        AFPSstream << TheEntresol->GetGraphicsManager()->GetGameWindow(0)->GetAverageFPS();
        String CFPS = CFPSstream.str();
        String AFPS = AFPSstream.str();
        static_cast<UI::SingleLineTextLayer*>(CurFPS->GetRenderLayer(0))->SetText(CFPS);
        static_cast<UI::SingleLineTextLayer*>(AvFPS->GetRenderLayer(0))->SetText(AFPS);
    }
};//DemoPostRenderWorkUnit

class DemoPostPhysicsWorkUnit : public Threading::DefaultWorkUnit
{
public:
    DemoPostPhysicsWorkUnit() {  }
    virtual ~DemoPostPhysicsWorkUnit() {  }

    void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
    {
        //TheEntresol->Log("Object Locations");
        //Replace this with something that uses the actor container and logs the location of everything
        //TheEntresol->Log(TheEntresol->GetActorManager()->GetActor("MetalSphere2")->GetLocation());

        // Updating functions to be used when a suitable mesh is found/created.
        /*ActorSoft* ActS = static_cast< ActorSoft* > (TheEntresol->GetActorManager()->FindActor("spheresoft"));
        ActS->UpdateSoftBody();
        TheEntresol->Log("Soft Locations");
        TheEntresol->Log(TheEntresol->GetActorManager()->FindActor("spheresoft")->GetLocation());
        if(ActS->IsInWorld())
            TheEntresol->Log("Softbody is in World");
        else
            TheEntresol->Log("Softbody is not in World");// */
    }
};//DemoPostPhysicsWorkUnit


int main(int argc, char **argv)
{
    // Temporary Hack
    #ifdef MACOSX
    String ExeDir = Mezzanine::ResourceManager::GetExecutableDirFromArg(argc,argv);
    Mezzanine::ResourceManager::ChangeDirectory(ExeDir);
    #endif

    try
    {
        Physics::ManagerConstructionInfo Info;
        Info.PhysicsFlags = Physics::ManagerConstructionInfo::PCF_SoftRigidWorld;
        Info.GeographyLowerBounds = Vector3(-30000.0,-30000.0,-30000.0);
        Info.GeographyUpperBounds = Vector3(30000.0,30000.0,30000.0);
        Info.MaxProxies = 60;
        TheEntresol = new Entresol( Info, "DefaultSceneManager", "data/common/");
    }catch(...){
        return 1;
        // ©ould not create the perfect worldending program
    }
    TheEntresol->SetTargetFrameRate(60);

    //TheEntresol->SetLoggingFrequency(Entresol::LogOncePerXFrames,250); //Every 250 frames should be once every 5 seconds or so.
    //TheEntresol->SetLoggingFrequency(Entresol::LogOncePerXSeconds,5);
    //TheEntresol->SetLoggingFrequency(Entresol::LogNever);

    Graphics::GraphicsManager* GraphMan = TheEntresol->GetGraphicsManager();

    // ©reate the windows!
    Graphics::GameWindow* FirstWindow = GraphMan->CreateGameWindow("First",1024,768,0);
    Graphics::CameraProxy* FirstCam = TheEntresol->GetCameraManager()->CreateCamera("FirstCam");
    //Graphics::Viewport* FirstViewport = FirstWindow->CreateViewport(FirstCam);
    FirstWindow->CreateViewport(FirstCam,0);
    FirstCam->SetLocation( Vector3(0,50,900) );
    FirstCam->LookAt( Vector3(0,0,0) );

    /*Graphics::GameWindow* SecondWindow = GraphMan->CreateGameWindow("Second",640,480,0);
    Camera* SecondCam = TheEntresol->GetCameraManager()->CreateCamera("SecondCam");
    Graphics::Viewport* SecondViewport = SecondWindow->CreateViewport(SecondCam);
    SecondCam->SetLocation( Vector3(-300,50,-50) );
    SecondCam->LookAt( Vector3(0,0,0) );//*/

    // Setup our workunits
    DemoPreEventWork = new DemoPreEventWorkUnit();
    TheEntresol->GetEventManager()->GetEventPumpWork()->AddDependency( DemoPreEventWork );
    TheEntresol->GetScheduler().AddWorkUnitMain( DemoPreEventWork, "DemoPreEventWork" );

    DemoPostInputWork = new DemoPostInputWorkUnit();
    DemoPostInputWork->AddDependency( TheEntresol->GetInputManager()->GetDeviceUpdateWork() );
    TheEntresol->GetScheduler().AddWorkUnitMain( DemoPostInputWork, "DemoPostInputWork" );

    DemoPostRenderWork = new DemoPostRenderWorkUnit();
    DemoPostRenderWork->AddDependency( TheEntresol->GetGraphicsManager()->GetRenderWork() );
    DemoPostRenderWork->AddDependency( TheEntresol->GetAreaEffectManager()->GetAreaEffectUpdateWork() );
    TheEntresol->GetScheduler().AddWorkUnitMain( DemoPostRenderWork, "DemoPostRenderWork" );

    DemoPostPhysicsWork = new DemoPostPhysicsWorkUnit();
    DemoPostPhysicsWork->AddDependency( TheEntresol->GetPhysicsManager()->GetSimulationWork() );
    TheEntresol->GetScheduler().AddWorkUnitMain( DemoPostPhysicsWork, "DemoPostPhysicsWork" );

    // Init
    TheEntresol->EngineInit(false);

    // Configure Shadows
    TheEntresol->GetSceneManager()->SetSceneShadowTechnique(Graphics::SceneManager::SST_Stencil_Additive);
    TheEntresol->GetSceneManager()->SetShadowFarDistance(3000);

    //Set up polling for the letter Q
    TheEntresol->GetEventManager()->AddPollingCheck( Input::MetaCode(0, Input::KEY_Q) );

    //Actually Load the game stuff
    LoadContent();

    //Generate the UI
    MakeGUI();

    // ©onfigure the wireframe Drawer
    //TheEntresol->GetPhysicsManager()->SetDebugRenderingMode(Physics::DDM_DrawWireframe);
    TheEntresol->GetPhysicsManager()->SetSimulationSubstepModifier(3);

    //Setup some camera tricks
    TheEntresol->GetCameraManager()->GetCamera(0)->SetLocation(Vector3(0.0,200.0,450.0));
    // ©ameraController* DefaultControl = TheEntresol->GetCameraManager()->GetOrCreateCameraController(TheEntresol->GetCameraManager()->GetCamera(0));
    //DefaultControl->SetMovementMode(CameraController::CCM_Walk);
    //DefaultControl->SetHoverHeight(75);
    Graphics::LightProxy* Headlight = TheEntresol->GetSceneManager()->CreateLightProxy(Graphics::LT_Directional);
    Vector3 LightLoc(200,300,0);
    Headlight->SetLocation(LightLoc);
    LightLoc.X = -LightLoc.X;
    LightLoc.Y = -LightLoc.Y;
    LightLoc.Normalize();
    Headlight->SetDirection(LightLoc);
    Headlight->SetDiffuseColour(ColourValue(0.7,0.7,0.7,1.0));
    Headlight->SetSpecularColour(ColourValue(0.7,0.7,0.7,1.0));
    Headlight->SetLocation(Vector3(0,150,0));
    Headlight->AddToWorld();
    //Headlight->SetAttenuation(1000.0, 0.0, 1.0, 0.0);         //I couldn't get these to work
    // ©ameraNode->AttachObject(Headlight);

    //Start the Main Loop
    TheEntresol->MainLoop();

    delete Soundtrack;
    delete Announcer;
    delete TheEntresol;
    //delete DemoPostInputWork;
    //delete DemoPostPhysicsWork;
    //delete DemoPostRenderWork;
    //delete DemoPreEventWork;
    return 0;
}

void LoadContent()
{
    TheEntresol->GetSceneManager()->SetAmbientLight(0.10,0.10,0.10,0.10);

    RigidDebris* object1;
    RigidDebris* object2;
    RigidDebris* object3;
    RigidDebris* object4;
    RigidDebris* object5;
    RigidDebris* object6;
    RigidDebris* object7;
    //Ogre Setup Code
    String groupname("Group1");
    String filerobot("robot.mesh");
    String robotprefix("Robot");

    Real mass = 15.0;
    /// @todo Figure why the EngineDemo fails on Linux when trying to find items in the
    TheEntresol->GetResourceManager()->AddAssetLocation("data/common", AT_FileSystem, groupname, false);
    TheEntresol->GetResourceManager()->AddAssetLocation("data/common/music", AT_FileSystem, groupname, false);
    TheEntresol->GetResourceManager()->AddAssetLocation("data/common/sounds", AT_FileSystem, groupname, false);
    //TheEntresol->GetResourceManager()->AddAssetLocation(zipname.str(), "Zip", groupname, false);
    TheEntresol->GetResourceManager()->AddAssetLocation("", AT_FileSystem, groupname, false);
    TheEntresol->GetResourceManager()->InitAssetGroup(groupname);

    Vector3 grav( 0.0, -400.0, 0.0 );
    TheEntresol->GetPhysicsManager()->SetWorldGravity(grav);
    TheEntresol->GetPhysicsManager()->SetWorldSoftGravity(grav);

    //ParticleEffect *GreenPart = TheEntresol->GetSceneManager()->CreateParticleEffect("GreenParticles", "Examples/GreenyNimbus");
    //GreenPart->SetLocation(Vector3(-70,70,-100));

    Physics::CollisionShapeManager* CSMan = Entresol::GetSingletonPtr()->GetCollisionShapeManager();
    Physics::CollisionShape* RobitCH = CSMan->GenerateConvexHull("RobitConvexHull",filerobot,groupname);
    Physics::CollisionShape* RobitCD = CSMan->PerformConvexDecomposition("RobitConvexDecomp",filerobot,groupname,5.0,5.0,10.0);
    Physics::CollisionShape* PlaneStatic = CSMan->GenerateStaticTriMesh("PlaneShape","Plane.mesh",groupname);
    Physics::CollisionShape* WoodenSphere = new Physics::SphereCollisionShape("WoodSphere",250.0);
    Physics::CollisionShape* MetalSphere = new Physics::SphereCollisionShape("MetalSphere",250.0);
    Physics::CollisionShape* MetalSphere2 = new Physics::SphereCollisionShape("MetalSphere2",250.0);

    // Now Lets make some bowling pins
    Real PinSpacing=75.0;           //This is how far apart we want the pins
    RigidDebris* RigDeb = NULL;
    for(unsigned int c=0; c<4; c++)     //make the back row
    {
        std::stringstream namestream;
        namestream << robotprefix << c;
        RigDeb = TheEntresol->GetDebrisManager()->CreateRigidDebris(namestream.str(),mass);
        RigDeb->GetRigidProxy()->SetCollisionShape(RobitCD);
        RigDeb->GetEntityProxy()->SetMesh(filerobot,groupname);
        //TheEntresol->GetResourceManager()->ImportShapeData(RigDeb, "data/common/RobotDecomp3.bullet");
        RigDeb->SetLocation(Vector3( (-2.0*PinSpacing)+(c*PinSpacing), -90.0, 0));
        RigDeb->AddToWorld();
    }

    for(unsigned int c=0; c<3; c++)     //the row with three pins
    {
        std::stringstream namestream;
        namestream << robotprefix << (c+4);
        RigDeb = TheEntresol->GetDebrisManager()->CreateRigidDebris(namestream.str(),mass);
        RigDeb->GetRigidProxy()->SetCollisionShape(RobitCD);
        RigDeb->GetEntityProxy()->SetMesh(filerobot,groupname);
        //TheEntresol->GetResourceManager()->ImportShapeData(RigDeb, "data/common/RobotDecomp3.bullet");
        RigDeb->SetLocation(Vector3( (-1.5*PinSpacing)+(c*PinSpacing), -66.0, -PinSpacing));
        RigDeb->AddToWorld();
    }
    //TheEntresol->Resources->ImportShapeData(RigDeb, "RobotDecomp3.bullet");

    for(unsigned int c=0; c<2; c++)     //the row with 2 pins
    {
        std::stringstream namestream;
        namestream << robotprefix << (c+7);
        RigDeb = TheEntresol->GetDebrisManager()->CreateRigidDebris(namestream.str(),mass);
        RigDeb->GetRigidProxy()->SetCollisionShape(RobitCH);
        RigDeb->GetEntityProxy()->SetMesh(filerobot,groupname);
        RigDeb->SetLocation(Vector3( (-PinSpacing)+(c*PinSpacing), -30.0, -PinSpacing*2));
        if (c+7==7)
            {Robot7=RigDeb;}
        if (c+7==8)
            {Robot8=RigDeb;}
        RigDeb->AddToWorld();
    }

    std::stringstream namestream;           //make the front pin
    namestream << robotprefix << 9;
    RigDeb = TheEntresol->GetDebrisManager()->CreateRigidDebris(namestream.str(),mass);
    RigDeb->GetRigidProxy()->SetCollisionShape(RobitCH);
    RigDeb->GetEntityProxy()->SetMesh(filerobot,groupname);
    RigDeb->SetLocation(Vector3( (-0.5*PinSpacing), 0.0, -PinSpacing*3));
    RigDeb->AddToWorld();

    object5 = TheEntresol->GetDebrisManager()->CreateRigidDebris("Plane",0);
    object5->GetRigidProxy()->SetCollisionShape(PlaneStatic);
    object5->GetEntityProxy()->SetMesh("Plane.mesh",groupname);
    object5->SetLocation(Vector3(0.0,-100,-300.0));
    object5->AddToWorld();

    object6 = TheEntresol->GetDebrisManager()->CreateRigidDebris("Ramp",0);
    object6->GetRigidProxy()->SetCollisionShape(PlaneStatic);
    object6->GetEntityProxy()->SetMesh("Plane.mesh",groupname);
    object6->SetLocation(Vector3(00.0,300.0,-1100.0));
    object6->SetOrientation(Quaternion(0.5, 0.0, 0.0, -0.25));
    object6->AddToWorld();

    object1 = TheEntresol->GetDebrisManager()->CreateRigidDebris("RobotWayUpFrontRight",mass);
    object1->GetRigidProxy()->SetCollisionShape(RobitCH);
    object1->GetEntityProxy()->SetMesh(filerobot,groupname);
    object1->SetLocation(Vector3(400,70,100));
    object1->SetOrientation(Quaternion(0.5, 0.5, 0.0, 0.9));
    object1->AddToWorld();

    object2 = TheEntresol->GetDebrisManager()->CreateRigidDebris("WoodSphere",150.0);
    object2->GetRigidProxy()->SetCollisionShape(WoodenSphere);
    object2->GetEntityProxy()->SetMesh("Sphere_Wood.mesh",groupname);
    object2->SetScale(Vector3(0.5,0.5,0.5));
    object2->SetLocation(Vector3(-140.0,2800.0,-1150.0));
    object2->AddToWorld();

    object3 = TheEntresol->GetDebrisManager()->CreateRigidDebris("MetalSphere",200.0);
    object3->GetRigidProxy()->SetCollisionShape(MetalSphere);
    object3->GetEntityProxy()->SetMesh("Sphere_Metal.mesh",groupname);
    object3->SetScale(Vector3(0.7,0.7,0.7));
    object3->SetLocation(Vector3(150.0,1800.0,-1300.0));
    object3->AddToWorld();

    object4 = TheEntresol->GetDebrisManager()->CreateRigidDebris("RobotWayUpFrontLeft",mass);
    object4->GetRigidProxy()->SetCollisionShape(RobitCD);
    object4->GetEntityProxy()->SetMesh(filerobot,groupname);
    object4->SetLocation(Vector3(-400,10, 100));
    object4->SetOrientation(Quaternion(0.5, 0.5, 0.0, 0.9));
    object4->AddToWorld();

    object7 = TheEntresol->GetDebrisManager()->CreateRigidDebris("MetalSphere2",800.0);
    object7->GetRigidProxy()->SetCollisionShape(MetalSphere2);
    object7->GetEntityProxy()->SetMesh("Sphere_Metal.mesh",groupname);
    object7->SetScale(Vector3(0.3,0.3,0.3));
    object7->SetLocation(Vector3(10.0,25000.0,-1300.0));
    object7->GetRigidProxy()->SetDamping(0.3,0.0);
    object7->AddToWorld();

    //GravityField
    /*GravityField* Reverse = new GravityField(String("UpField"), Vector3(0.0,-100.0,0.0));
    Reverse->CreateCylinderShapeY(Vector3(100.0,200.0,100));
    Reverse->SetLocation(Vector3(200,50,-5.0));
    TheEntresol->GetPhysicsManager()->AddAreaEffect(Reverse); // Now that we have passed it, we can forget about it*/

    //GravityWell
    /*GravityWell* BlackHole = new GravityWell("BlackHole", Vector3(0.0,200.0,-300.0));
    BlackHole->GetPhysicsSettings()->SetCollisionShape(new Physics::SphereCollisionShape("GravWellShape",750.0));
    BlackHole->SetAllowWorldGravity(false);
    BlackHole->SetFieldStrength(1000.0);
    BlackHole->SetAttenuation(3.000,Mezzanine::Att_Linear);
    //BlackHole->GetGraphicsSettings()->SetMesh(MeshManager::GetSingletonPtr()->CreateSphereMesh("GravWellMesh",ColourValue(0.8,0.1,0.1,0.15),750.0));
    TheEntresol->GetPhysicsManager()->AddAreaEffect(BlackHole);// */

    //Final Steps
    Audio::iSound *sound1 = NULL, *music1 = NULL, *music2 = NULL;
    Announcer = new std::vector<Audio::iSound*>();
    Soundtrack = new std::vector<Audio::iSound*>();
    Audio::AudioManager* AudioMan = TheEntresol->GetAudioManager();
    sound1 = AudioMan->CreateDialogSound("welcomefun-1.ogg", groupname);
    Announcer->push_back(sound1);

    music1 = AudioMan->CreateMusicSound("cAudioTheme1.ogg", groupname);
    Soundtrack->push_back(music1);
    music2 = AudioMan->CreateMusicSound("cAudioTheme2.ogg", groupname);
    Soundtrack->push_back(music2);

    TheEntresol->Log("Debris Count ");
    TheEntresol->Log( TheEntresol->GetDebrisManager()->GetNumDebris() );
}

void MakeGUI()
{
    String DefaultScreen = "DefaultScreen";
    UI::UIManager* GUI = TheEntresol->GetUIManager();
    Graphics::Viewport* UIViewport = TheEntresol->GetGraphicsManager()->GetGameWindow(0)->GetViewport(0);
    GUI->LoadMTA("EngineDemo_Menu.mta","Group1");
    GUI->AddAutoRegisterCode(Input::MetaCode(Input::BUTTON_PRESSING,Input::MOUSEBUTTON_1));
    GUI->EnableButtonAutoRegister(true);
    UI::Screen* DScreen = GUI->CreateScreen(DefaultScreen, "EngineDemo_Menu", UIViewport,0);

    ColourValue Transparent(0.0,0.0,0.0,0.0);
    ColourValue Black(0.0,0.0,0.0,1.0);
    ColourValue TransBlack(0.0,0.0,0.0,0.3);

    ColourValue ButtonColour(0.6,0.2,0.2,1.0);
    ColourValue HoveredButtonColour(0.8,0.7,0.05,1.0);
    ColourValue MenuColour(0.4,0.8,0.3,1.0);

    //Build the HUD
    UI::StackButton* MenuAccess = DScreen->CreateStackButton("D_MenuAccess",UI::UnifiedRect(0.008,0.932,0.14,0.06));
    UI::SingleImageLayer* AccessBackground = MenuAccess->CreateSingleImageLayer(0,"Normal");
    AccessBackground->SetColour(ButtonColour);
    UI::SingleImageLayer* AccessHoveredBackground = MenuAccess->CreateSingleImageLayer(0,"Hovered");
    AccessHoveredBackground->SetColour(HoveredButtonColour);
    UI::SingleLineTextLayer* AccessText = MenuAccess->CreateSingleLineTextLayer();
    AccessText->HorizontallyAlign(UI::LA_Center);
    AccessText->VerticallyAlign(UI::LA_Center);
    AccessText->SetDefaultFont("Ubuntu-14");
    AccessText->SetText("Menu");
    MenuAccess->AddLayerToGroup(AccessText,1,"Normal");
    MenuAccess->AddLayerToGroup(AccessText,1,"Hovered");
    DScreen->AddChild(MenuAccess,1);

    UI::Widget* CurFPS = DScreen->CreateWidget("D_CurFPS",UI::UnifiedRect(0.16,0.008,0.06,0.065));
    UI::SingleLineTextLayer* CurFPSText = CurFPS->CreateSingleLineTextLayer();
    CurFPSText->HorizontallyAlign(UI::LA_TopLeft);
    CurFPSText->VerticallyAlign(UI::LA_Center);
    CurFPSText->SetDefaultFont("Ubuntu-14");
    CurFPSText->SetText("0.0");
    CurFPS->AddLayerToGroup(CurFPSText,1,"Normal");
    CurFPS->AddLayerToGroup(CurFPSText,1,"Hovered");
    DScreen->AddChild(CurFPS,2);

    UI::Widget* CurFPSStat = DScreen->CreateWidget("D_CurFPSStat",UI::UnifiedRect(0.008,0.008,0.15,0.065));
    UI::SingleLineTextLayer* CurFPSStatText = CurFPSStat->CreateSingleLineTextLayer();
    CurFPSStatText->HorizontallyAlign(UI::LA_TopLeft);
    CurFPSStatText->VerticallyAlign(UI::LA_Center);
    CurFPSStatText->SetDefaultFont("Ubuntu-14");
    CurFPSStatText->SetText("Current FPS: ");
    CurFPSStat->AddLayerToGroup(CurFPSStatText,1,"Normal");
    CurFPSStat->AddLayerToGroup(CurFPSStatText,1,"Hovered");
    DScreen->AddChild(CurFPSStat,3);

    UI::Widget* AvFPS = DScreen->CreateWidget("D_AvFPS",UI::UnifiedRect(0.16,0.081,0.06,0.065));
    UI::SingleLineTextLayer* AvFPSText = AvFPS->CreateSingleLineTextLayer();
    AvFPSText->HorizontallyAlign(UI::LA_TopLeft);
    AvFPSText->VerticallyAlign(UI::LA_Center);
    AvFPSText->SetDefaultFont("Ubuntu-14");
    AvFPSText->SetText("0.0");
    AvFPS->AddLayerToGroup(AvFPSText,1,"Normal");
    AvFPS->AddLayerToGroup(AvFPSText,1,"Hovered");
    DScreen->AddChild(AvFPS,4);

    UI::Widget* AvFPSStat = DScreen->CreateWidget("D_AvFPSStat",UI::UnifiedRect(0.008,0.081,0.15,0.065));
    UI::SingleLineTextLayer* AvFPSStatText = AvFPSStat->CreateSingleLineTextLayer();
    AvFPSStatText->HorizontallyAlign(UI::LA_TopLeft);
    AvFPSStatText->VerticallyAlign(UI::LA_Center);
    AvFPSStatText->SetDefaultFont("Ubuntu-14");
    AvFPSStatText->SetText("Average FPS: ");
    AvFPSStat->AddLayerToGroup(AvFPSStatText,1,"Normal");
    AvFPSStat->AddLayerToGroup(AvFPSStatText,1,"Hovered");
    DScreen->AddChild(AvFPSStat,5);
    //End of HUD
    //Build the Menu
    UI::MenuEntry* DemoMenu = DScreen->CreateMenuEntry("D_Menu",UI::UnifiedRect(0.35,0.27,0.3,0.45));
    UI::SingleImageLayer* DemoMenuBackground = DemoMenu->CreateSingleImageLayer();
    DemoMenuBackground->SetColour(MenuColour);
    DemoMenu->AddLayerToGroup(DemoMenuBackground,0,"Normal");
    DemoMenu->AddLayerToGroup(DemoMenuBackground,0,"Hovered");
    DScreen->AddChild(DemoMenu,6);

    UI::StackButton* ReturnButton = DScreen->CreateStackButton("D_Return",UI::UnifiedRect(0.10,0.56,0.80,0.16));
    UI::SingleImageLayer* ReturnBackground = ReturnButton->CreateSingleImageLayer(0,"Normal");
    ReturnBackground->SetColour(ButtonColour);
    UI::SingleImageLayer* ReturnHoveredBackground = ReturnButton->CreateSingleImageLayer(0,"Hovered");
    ReturnHoveredBackground->SetColour(HoveredButtonColour);
    UI::SingleLineTextLayer* ReturnText = ReturnButton->CreateSingleLineTextLayer();
    ReturnText->HorizontallyAlign(UI::LA_Center);
    ReturnText->VerticallyAlign(UI::LA_Center);
    ReturnText->SetDefaultFont("Ubuntu-14");
    ReturnText->SetText("Return To Demo");
    ReturnButton->AddLayerToGroup(ReturnText,1,"Normal");
    ReturnButton->AddLayerToGroup(ReturnText,1,"Hovered");
    DemoMenu->AddChild(ReturnButton,1);

    UI::Button* GameExitButton = DScreen->CreateButton("D_Exit",UI::UnifiedRect(0.10,0.78,0.80,0.16));
    UI::SingleImageLayer* GameExitBackground = GameExitButton->CreateSingleImageLayer(0,"Normal");
    GameExitBackground->SetColour(ButtonColour);
    UI::SingleImageLayer* GameExitHoveredBackground = GameExitButton->CreateSingleImageLayer(0,"Hovered");
    GameExitHoveredBackground->SetColour(HoveredButtonColour);
    UI::SingleLineTextLayer* GameExitText = GameExitButton->CreateSingleLineTextLayer();
    GameExitText->HorizontallyAlign(UI::LA_Center);
    GameExitText->VerticallyAlign(UI::LA_Center);
    GameExitText->SetDefaultFont("Ubuntu-14");
    GameExitText->SetText("Exit Demo");
    GameExitButton->AddLayerToGroup(GameExitText,1,"Normal");
    GameExitButton->AddLayerToGroup(GameExitText,1,"Hovered");
    DemoMenu->AddChild(GameExitButton,2);

    DemoMenu->SetPushButton(MenuAccess);
    DemoMenu->SetPopButton(ReturnButton);
    //End of Menu
}

void TestsToSave()
{
    /*
    ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
    //testing the directory listing feature
    //please don;t delete this, this should be turned into a unit test.
    Entresol::GetSingletonPtr()->LogStream << std::endl << "Checking content of data directory:" << "data/common" << std::endl;
    std::set<String>* Listing = ResourceMan->GetDirContents("data/common");
    for (std::set<String>::iterator Iter = Listing->begin(); Iter!=Listing->end(); ++Iter)
    {
        Entresol::GetSingletonPtr()->LogStream << *Iter << std::endl;
    }
    delete Listing;

    Entresol::GetSingletonPtr()->LogStream << std::endl << "Checking content of working directory:" << ResourceMan->GetWorkingDirectory() << std::endl;
    Listing = ResourceMan->GetDirContents(ResourceMan->GetWorkingDirectory());
    for (std::set<String>::iterator Iter = Listing->begin(); Iter!=Listing->end(); ++Iter)
    {
        Entresol::GetSingletonPtr()->LogStream << *Iter << std::endl;
    }
    delete Listing;

    Entresol::GetSingletonPtr()->LogStream << std::endl << "Checking content of current directory:" << "." << std::endl;
    Listing = ResourceMan->GetDirContents(".");
    for (std::set<String>::iterator Iter = Listing->begin(); Iter!=Listing->end(); ++Iter)
    {
        Entresol::GetSingletonPtr()->LogStream << *Iter << std::endl;
    }
    delete Listing;

    Entresol::GetSingletonPtr()->LogStream << std::endl << "Checking content of parent directory:" << ".." << std::endl;
    Listing = ResourceMan->GetDirContents("..");
    for (std::set<String>::iterator Iter = Listing->begin(); Iter!=Listing->end(); ++Iter)
    {
        Entresol::GetSingletonPtr()->LogStream << *Iter << std::endl;
    }
    delete Listing;

    Entresol::GetSingletonPtr()->LogStream << std::endl << "Checking content of '/'" << std::endl;
    Listing = ResourceMan->GetDirContents("/");
    if (NULL == Listing)
    {
        Entresol::GetSingletonPtr()->LogStream << "/ not a valid directory" << std::endl;
    }else{
        for (std::set<String>::iterator Iter = Listing->begin(); Iter!=Listing->end(); ++Iter)
        {
            Entresol::GetSingletonPtr()->LogStream << *Iter << std::endl;
        }
        delete Listing;
    }

    Entresol::GetSingletonPtr()->LogStream << std::endl << "Checking content of 'c:/'" << std::endl;
    Listing = ResourceMan->GetDirContents("c:/");
    if (NULL == Listing)
    {
        Entresol::GetSingletonPtr()->LogStream << "c:/ not a valid directory" << std::endl;
    }else{
        for (std::set<String>::iterator Iter = Listing->begin(); Iter!=Listing->end(); ++Iter)
        {
            Entresol::GetSingletonPtr()->LogStream << *Iter << std::endl;
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
        TheEntresol->LogStream << "ShouldHaveXML: " << std::endl << ShouldHaveXML << std::endl
                            << "ZippedFileContents: " << std::endl << ZippedFileContents << std::endl
                            << "File read from fstream: " << std::endl << TestFileString << std::endl ;
        #endif

        //if the above stuff was logged this should just work
        if( !Zippedptr->good() )
            { TheEntresol->Log("Zippedptr corrupted/broken by read call"); }

        //this should just work
        Zippedptr->unget();
        if( !Zippedptr->good() )
            { TheEntresol->Log("Zippedptr corrupted/broken by unget() call"); }
        else
        {
            Whole howfew = 500;
            char temp[howfew+1];
            for (Whole c=0; c<=howfew+1; c++)
                { temp[c]='\0'; }
            for (Whole c=0; c<howfew-1; c++)      //we already did one above -(1), we do not want to pad anything with a \0 (no +1), and we want exactly this many (< instead of <=)
                { Zippedptr->unget(); }
            Zippedptr->read(temp, howfew);
            TheEntresol->LogStream << howfew << " ungets and " << howfew << " characters :" << temp;
        }

        TheEntresol->LogStream << std::endl << "End first streaming test" << std::endl;

        try
            { XMLptr->putback('X'); }// as in :X
        catch (...)
            { throw; }

        delete XMLptr;
        delete Zippedptr;
    }

    {
        #ifdef MEZZDEBUG
        TheEntresol->Log("Testing streaming on test.XML and zipped test.txt, Making fresh set of stream pointers");
        #endif

        ResourceInputStream* XMLptr = TheEntresol->GetResourceManager()->GetResourceStream("test.XML");
        ResourceInputStream* Zippedptr = TheEntresol->GetResourceManager()->GetResourceStream("test.txt");

        String Content("");
        (*XMLptr) >> Content;
        TheEntresol->LogStream << "Extracted: " << Content << std::endl;

        if( !XMLptr->good() )
            { TheEntresol->Log("XMLptr corrupted/broken by >> call"); }
        else
        {
            TheEntresol->Log("XMLptr fine after >> call");
            TheEntresol->Log("Getting some words with >> from Zippedptr:");
            for (Whole c=0; c<=5; c++)
            {
                String ZippedContent;
                (*Zippedptr) >> ZippedContent;
                TheEntresol->LogStream << "Word " << c << ": " << ZippedContent;
                TheEntresol->Log();
            }
        }

        delete XMLptr;
        delete Zippedptr;

        #ifdef MEZZDEBUG
        TheEntresol->Log("End streaming test2");
        #endif
    }

    {
        #ifdef MEZZDEBUG
        TheEntresol->Log("Testing streaming on test.XML with Mezzanine::XML::Document");
        #endif

        ResourceInputStream* XMLptr = TheEntresol->GetResourceManager()->GetResourceStream("test.XML");
        XML::Document TestDoc;
        XML::ParseResult ParsedXML = TestDoc.Load( *XMLptr );

        TheEntresol->LogStream << "XML::StatusOk :" << XML::StatusOk << std::endl
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
            TheEntresol->Log("Successfully loaded and parsed test.XML");
        }else{
            TheEntresol->LogStream << "Failed to load and parse test.XML. With Status :" << ParsedXML.Status;
        }

        TheEntresol->Log("XML Streaming Test");

        Vector2 ASinglePoint(1,2);
        TheEntresol->Log("ASinglePoint:");
        TheEntresol->Log(ASinglePoint);

        TheEntresol->Log("Streaming ASinglePoint to String Stream:");
        StringStream XMLStringStream;
        XMLStringStream << ASinglePoint;
        TheEntresol->Log(XMLStringStream.str());

        Vector2 ASecondPoint(0,0);
        TheEntresol->Log("ASecondPoint:");
        TheEntresol->Log(ASecondPoint);

        TheEntresol->Log("ReStreaming ASinglePoint from stringStream, to ASecondPoint:");
        XMLStringStream >> ASecondPoint;
        TheEntresol->Log(ASecondPoint);

        Plane ASinglePlane(Vector3(1.34,23,1.004),4.5);
        TheEntresol->Log("ASinglePlane:");
        TheEntresol->Log(ASinglePlane);

        TheEntresol->Log("Streaming ASinglePlane to String Stream:");
        StringStream XMLStringStream2;
        XMLStringStream2 << ASinglePlane;
        TheEntresol->Log(XMLStringStream2.str());

        Plane ASecondPlane(Vector3(0,0,0),0);
        TheEntresol->Log("ASecondPlane:");
        TheEntresol->Log(ASecondPlane);

        TheEntresol->Log("ReStreaming ASinglePlane from stringStream, to ASecondPlane:");
        XMLStringStream2 >> ASecondPlane;
        TheEntresol->Log(ASecondPlane);

        MetaCode ASingleCode(MetaCode(1,Input::MOUSEBUTTON));
        TheEntresol->Log("ASingleCode:");
        TheEntresol->Log(ASingleCode);

        TheEntresol->Log("Streaming ASingleCode to String Stream:");
        StringStream XMLStringStream3;
        XMLStringStream3 << ASingleCode;
        TheEntresol->Log(XMLStringStream3.str());

        MetaCode ASecondCode(MetaCode(0,Input::KEY_FIRST));
        TheEntresol->Log("ASecondCode:");
        TheEntresol->Log(ASecondCode);

        EventQuit AndOneEventToRuleThemAll;
        TheEntresol->Log("An EventQuit AndOneEventToRuleThemAll:");
        TheEntresol->Log(AndOneEventToRuleThemAll);
        StringStream XMLStringStream4;
        XMLStringStream4 << AndOneEventToRuleThemAll;
        XMLStringStream4 >> AndOneEventToRuleThemAll;

        EventRenderTime TimingEvent(55);
        EventRenderTime TimingEvent2(0);
        TheEntresol->Log("EventRenderTime TimingEvent and TimingEvent2:");
        TheEntresol->Log(TimingEvent);
        TheEntresol->Log(TimingEvent2);
        StringStream XMLStringStream5;
        XMLStringStream5 << TimingEvent;
        XMLStringStream5 >> TimingEvent2;
        TheEntresol->Log("EventRenderTime TimingEvent and TimingEvent2 After streaming:");
        TheEntresol->Log(TimingEvent);
        TheEntresol->Log(TimingEvent2);

        TheEntresol->Log("ReStreaming ASingleCode from stringStream, to ASingleCode:");
        XMLStringStream3 >> ASecondCode;
        TheEntresol->Log(ASecondCode);

        ColourValue AColour(1.0,1.0,1.0,1.0);
        ColourValue AnotherColour(0.5,0.5,0.5,0.5);
        TheEntresol->Log("A colour:");
        TheEntresol->Log(AColour);
        TheEntresol->Log("Another colour:");
        TheEntresol->Log(AnotherColour);
        StringStream XMLStringStream7;
        XMLStringStream7 << AColour;
        XMLStringStream7 >> AnotherColour;
        TheEntresol->Log("Another colour after streaming A colour to it:");
        TheEntresol->Log(AColour);


        #ifdef MEZZDEBUG
        TheEntresol->Log("End of testing XML and Streaming test 3");
        #endif
    }
    //*/
}
//*/
#endif
