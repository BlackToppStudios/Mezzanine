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
#include <world.h>

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

std::vector<Audio::iSound*> Announcer;
std::vector<Audio::iSound*> Soundtrack;

RigidDebris* Robot7 = NULL;
RigidDebris* Robot8 = NULL;

DemoPreEventWorkUnit*       DemoPreEventWork     = NULL;
DemoPostInputWorkUnit*      DemoPostInputWork    = NULL;
DemoPostRenderWorkUnit*     DemoPostRenderWork   = NULL;
DemoPostPhysicsWorkUnit*    DemoPostPhysicsWork  = NULL;

AppEventDispatcher* DemoAppEvents = NULL;
World* DemoWorld = NULL;

Graphics::GameWindow* FirstWindow = NULL;
Graphics::CameraProxy* MainCam = NULL;

CameraController* CamControl = NULL;

TrackLooped< LinearInterpolator< Transform > > CameraTrackTest;

void CreateDemoWorld();
void DestroyDemoWorld();

class DemoPostInputWorkUnit : public Threading::DefaultWorkUnit
{
protected:
    World* OneWorld;
    Input::InputManager* InputMan;
    MousePicker Picker;
public:
    DemoPostInputWorkUnit(World* TheWorld, Input::InputManager* Inputs) :
        OneWorld(TheWorld),
        InputMan(Inputs)
    {
        //Plane DragPlane(Vector3(0.0,0.0,1.0),Vector3(0.0,0.0,-5.0));
        Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( TheWorld->GetManager(ManagerBase::MT_SceneManager) );
        //Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( TheWorld->GetManager(ManagerBase::MT_PhysicsManager) );
        this->Picker.Initialize( Inputs->GetSystemMouse(),
                                 new Graphics::RenderableRayQuery(SceneMan),
                                 //new Physics::CollidableRayQuery(PhysMan),
                                 new DistanceDragger(30.0) );
    }

    virtual ~DemoPostInputWorkUnit()
    {
        delete this->Picker.GetQuery();
        delete this->Picker.GetDragger();
        //this->Picker.Deinitialize(true);
    }

    void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
    {
        //User Input through a WorldQueryTool
        Input::Mouse* SysMouse = InputMan->GetSystemMouse();
        Input::Keyboard* SysKeyboard = InputMan->GetSystemKeyboard();
        Input::Joystick* Joystick1 = NULL;
        if( InputMan->GetNumJoysticks() > 0 )
            Joystick1 = InputMan->GetJoystick(0);

        if( SysKeyboard->IsButtonPressed(Input::KEY_LEFT) || (Joystick1 ? Joystick1->IsHatPushedInDirection(1,Input::JOYSTICKHAT_LEFT) : false) )
            { CamControl->StrafeLeft( 300 * ( TheEntresol->GetLastFrameTimeMilliseconds() * 0.001 ) ); }

        if( SysKeyboard->IsButtonPressed(Input::KEY_RIGHT) || (Joystick1 ? Joystick1->IsHatPushedInDirection(1,Input::JOYSTICKHAT_RIGHT) : false) )
            { CamControl->StrafeRight( 300 * ( TheEntresol->GetLastFrameTimeMilliseconds() * 0.001 ) ); }

        if( SysKeyboard->IsButtonPressed(Input::KEY_UP) || (Joystick1 ? Joystick1->IsHatPushedInDirection(1,Input::JOYSTICKHAT_UP) : false) )
            { CamControl->MoveForward( 300 * ( TheEntresol->GetLastFrameTimeMilliseconds() * 0.001 ) ); }

        if( SysKeyboard->IsButtonPressed(Input::KEY_DOWN) || (Joystick1 ? Joystick1->IsHatPushedInDirection(1,Input::JOYSTICKHAT_DOWN) : false) )
            { CamControl->MoveBackward( 300 * ( TheEntresol->GetLastFrameTimeMilliseconds() * 0.001 ) ); }

        static Real TrackPos = 0.0;

        if( SysKeyboard->IsButtonPressed(Input::KEY_A) ) {
            TrackPos = ( TrackPos + 0.001 > 1.0 ? ( TrackPos + 0.001 ) - 1.0 : TrackPos + 0.001 );
            MainCam->SetTransform( CameraTrackTest.GetInterpolated( TrackPos ) );
        }

        if( SysKeyboard->IsButtonPressed(Input::KEY_D) ) {
            TrackPos = ( TrackPos - 0.001 < 0.0 ? ( TrackPos - 0.001 ) + 1.0 : TrackPos - 0.001 );
            MainCam->SetTransform( CameraTrackTest.GetInterpolated( TrackPos ) );
        }

        static bool MouseCam = false;
        if( SysKeyboard->IsButtonPressed(Input::KEY_HOME) )
            { MouseCam = true; }

        if( SysKeyboard->IsButtonPressed(Input::KEY_END))
            { MouseCam = false; }

        Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( this->OneWorld->GetManager(ManagerBase::MT_PhysicsManager) );

        static Boole DrawToggle = true;
        if( SysKeyboard->IsButtonPressing(Input::KEY_C ) ) {
            if( DrawToggle ) PhysMan->SetDebugRenderingMode(Physics::DDM_DrawWireframe);
            else PhysMan->SetDebugRenderingMode(Physics::DDM_NoDebug);
            DrawToggle = !DrawToggle;
        }

        if( SysKeyboard->IsButtonPressed(Input::KEY_BACKSLASH) )
            { PhysMan->SetTimeMultiplier(1.0); }

        if( SysKeyboard->IsButtonPressed(Input::KEY_RIGHTBRACKET) ) {
            Real Factor = PhysMan->GetTimeMultiplier() * 1.5;
            if( Factor > 2.0 )
                { Factor = 2.0; }
            PhysMan->SetTimeMultiplier(Factor);
        }

        if( SysKeyboard->IsButtonPressed(Input::KEY_LEFTBRACKET) ) {
            Real Factor = PhysMan->GetTimeMultiplier() * .75;
            if( Factor < 0.01 )
                { Factor = 0.01; }
            PhysMan->SetTimeMultiplier(Factor);
        }

        Vector2 Offset = SysMouse->GetMouseDelta();
        if( MouseCam && Vector2(0,0) != Offset )
            CamControl->Rotate(Offset.X * 0.01,Offset.Y * 0.01,0);

        if( SysKeyboard->IsButtonPressing(Input::KEY_M) || (Joystick1 ? Joystick1->IsButtonPressed(1) : false) ) {
            Audio::iSound* Theme = Soundtrack.at(1);
            if( !Theme->IsPlaying() ) {
                Theme->Play();
            }
        }

        if( SysKeyboard->IsButtonPressing(Input::KEY_N) || (Joystick1 ? Joystick1->IsButtonPressed(2) : false) ) {
            Audio::iSound* Theme = Soundtrack.at(1);
            if( Theme->IsPlaying() ) {
                Theme->Stop();
            }
        }

        // Audio test fun
        if( SysKeyboard->IsButtonPressing(Input::KEY_T) ) {
            static Boole Toggle = true;
            Audio::iSound* Welcome = ( Announcer.size() >= 2 ? ( Toggle ? Announcer[1] : Announcer[0] ) : NULL );
            if( Welcome != NULL ) {
                if( Welcome->IsPlaying() ) {
                    Welcome->Stop();
                }
                Welcome->Play();
            }
            Toggle = !Toggle;
        }

        UI::UIManager* UIMan = UI::UIManager::GetSingletonPtr();
        Boole MouseInUI = UIMan->MouseIsInUISystem();
        if( SysMouse->IsButtonPressed(1) ) {
            if( MouseInUI ) {
                UI::Widget* Hover = UIMan->GetHoveredWidget();
                if(Hover) {
                    Hover = Hover->GetBottomMostHoveredWidget();
                    if( "D_Exit" == Hover->GetName() ) {
                        TheEntresol->BreakMainLoop();
                    }
                }
            }
        }

        if( !MouseInUI ) {
            this->Picker.Execute(nullptr);
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
        }// */

        // Update Stat information
        Graphics::GameWindow* MainWindow = static_cast<Graphics::GraphicsManager*>( TheEntresol->GetManager(ManagerBase::MT_GraphicsManager) )->GetGameWindow(0);
        UI::Screen* DScreen = static_cast<UI::UIManager*>( TheEntresol->GetManager(ManagerBase::MT_UIManager) )->GetScreen("DefaultScreen");
        UI::Widget* CurFPS = static_cast<UI::Widget*>(DScreen->GetWidget("D_CurFPS"));
        UI::Widget* AvFPS = static_cast<UI::Widget*>(DScreen->GetWidget("D_AvFPS"));
        StringStream CFPSstream;
        StringStream AFPSstream;
        CFPSstream << MainWindow->GetLastFPS();
        AFPSstream << MainWindow->GetAverageFPS();
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
    #ifdef MEZZ_MACOSX
    String ExeDir = Mezzanine::Resource::GetExecutableDirFromArg(argc,argv);
    Mezzanine::Resource::ChangeWorkingDirectory(ExeDir);
    #endif

    Physics::ManagerConstructionInfo Info;
    Info.PhysicsFlags = Physics::ManagerConstructionInfo::PCF_LimitlessWorld | Physics::ManagerConstructionInfo::PCF_SoftRigidWorld | Physics::ManagerConstructionInfo::PCF_Multithreaded;

    try
    {
        TheEntresol = new Entresol( "data/common/", "EngineDemoLog.txt");
        TheEntresol->SetTargetFrameRate(60);
        DemoAppEvents = new AppEventDispatcher();
        DemoAppEvents->Subscribe(AppEventDispatcher::EventAppQuit,1,[=](EventPtr Args){ TheEntresol->BreakMainLoop(); });

        TheEntresol->Initialize(false);

        CreateDemoWorld();
        DestroyDemoWorld();
        CreateDemoWorld();

        //Generate the UI
        MakeGUI();

        // Greet our viewers
        Audio::iSound* Welcome = Announcer.at(0);
        if( Welcome ) {
            Welcome->Play();
        }
        #ifdef MEZZDEBUG
        TheEntresol->_Log("Played Welcome Fun.");
        #endif

        //Start the Main Loop
        TheEntresol->MainLoop();

        DestroyDemoWorld();
        delete DemoAppEvents;
        delete TheEntresol;
    }catch(std::exception& excep){
        std::cerr << "Exception: " << excep.what() << std::endl;
        return 1;
    }catch(...){
        std::cerr << "Unknown exception" << std::endl;
        return 1;
        // Could not create the perfect world ending program
    }
    return 0;
}

void CreateDemoWorld()
{
    Physics::ManagerConstructionInfo Info;
    Info.PhysicsFlags = Physics::ManagerConstructionInfo::PCF_LimitlessWorld | Physics::ManagerConstructionInfo::PCF_SoftRigidWorld | Physics::ManagerConstructionInfo::PCF_Multithreaded;

    // Create World and all it's lovely Managers
    DemoWorld = TheEntresol->CreateWorld( "DemoWorld", Info, "DefaultSceneManager" );

    if( DemoWorld == NULL ) {
        throw RuntimeAssertionException(
            "Failed to Create New Demo World",
            __func__,
            __FILE__,
            __LINE__
        );
    }

    DemoWorld->Initialize();

    AreaEffectManager* AreaEffectMan = static_cast<AreaEffectManager*>( DemoWorld->GetManager(ManagerBase::MT_AreaEffectManager) );
    Graphics::GraphicsManager* GraphMan = static_cast<Graphics::GraphicsManager*>( TheEntresol->GetManager(ManagerBase::MT_GraphicsManager) );
    Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( DemoWorld->GetManager(ManagerBase::MT_SceneManager) );
    Input::InputManager* InputMan = static_cast<Input::InputManager*>( TheEntresol->GetManager(ManagerBase::MT_InputManager) );
    Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( DemoWorld->GetManager(ManagerBase::MT_PhysicsManager) );

    MainCam = SceneMan->CreateCamera();
    //MainCam->SetLocation(Vector3(0.0,200.0,1000.0));
    //MainCam->LookAt(Vector3(0,0,0));
    CamControl = new CameraController(MainCam);
    //CamControl->SetMovementMode(CameraController::CCM_Walk);
    //CamControl->SetHoverHeight(75);

    CameraTrackTest.Add( Transform( Vector3(0.0,200.0,1000.0),
                         Quaternion(MathTools::DegreesToRadians(-10),Vector3::Unit_X()) * Quaternion(MathTools::DegreesToRadians(360),Vector3::Unit_Y()) ) );
    CameraTrackTest.Add( Transform( Vector3(-1000.0,200.0,-100.0),
                         Quaternion(MathTools::DegreesToRadians(-10),Vector3::Unit_X()) * Quaternion(MathTools::DegreesToRadians(270),Vector3::Unit_Y()) ) );
    CameraTrackTest.Add( Transform( Vector3(0.0,200.0,-1200.0),
                         Quaternion(MathTools::DegreesToRadians(-10),Vector3::Unit_X()) * Quaternion(MathTools::DegreesToRadians(180),Vector3::Unit_Y()) ) );
    CameraTrackTest.Add( Transform( Vector3(1000.0,200.0,-100.0),
                         Quaternion(MathTools::DegreesToRadians(-10),Vector3::Unit_X()) * Quaternion(MathTools::DegreesToRadians(90),Vector3::Unit_Y()) ) );
    CameraTrackTest.Add( Transform( Vector3(1000.0,200.0,-100.0),
                         Quaternion(MathTools::DegreesToRadians(-10),Vector3::Unit_X()) * Quaternion(MathTools::DegreesToRadians(0),Vector3::Unit_Y()) ) );//*/
    //CameraTrackTest.EnforceLoop();
    MainCam->SetTransform( CameraTrackTest.GetInterpolated(0.0) );

    // Create the window(s)!
    FirstWindow = GraphMan->CreateGameWindow("First",1024,768,0);
    FirstWindow->CreateViewport(MainCam,0);

    // Setup our workunits
    DemoPostInputWork = new DemoPostInputWorkUnit(DemoWorld,InputMan);
    DemoPostInputWork->AddDependency( InputMan->GetDeviceUpdateWork() );
    TheEntresol->GetScheduler().AddWorkUnitMain( DemoPostInputWork, "DemoPostInputWork" );

    DemoPostRenderWork = new DemoPostRenderWorkUnit();
    DemoPostRenderWork->AddDependency( GraphMan->GetRenderWork() );
    DemoPostRenderWork->AddDependency( AreaEffectMan->GetAreaEffectUpdateWork() );
    TheEntresol->GetScheduler().AddWorkUnitMain( DemoPostRenderWork, "DemoPostRenderWork" );

    DemoPostPhysicsWork = new DemoPostPhysicsWorkUnit();
    DemoPostPhysicsWork->AddDependency( PhysMan->GetSimulationWork() );
    TheEntresol->GetScheduler().AddWorkUnitMain( DemoPostPhysicsWork, "DemoPostPhysicsWork" );

    // Configure Shadows
    SceneMan->SetSceneShadowTechnique(Graphics::SceneManager::SST_Stencil_Additive);
    SceneMan->SetShadowFarDistance(3000);

    //Actually Load the game stuff
    LoadContent();

    // Configure the wireframe Drawer
    //TheEntresol->GetPhysicsManager()->SetDebugRenderingMode(Physics::DDM_DrawWireframe);
    //TheEntresol->GetPhysicsManager()->SetSimulationSubstepModifier(3);

    Graphics::LightProxy* Headlight = SceneMan->CreateLightProxy(Graphics::LT_Directional);
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
    // CameraNode->AttachObject(Headlight);

    std::cerr << "Finished Creating World\n";
}

void DestroyDemoWorld()
{
    // Make sure everything is dead before starting again

    // Check if Threaded Workunits are still alive
    // RemoveWorkUnitMain
    if( DemoPostInputWork ) {
        TheEntresol->GetScheduler().RemoveWorkUnitMain( DemoPostInputWork );
        DemoPostInputWork = NULL;
    }
    if( DemoPostRenderWork ) {
        TheEntresol->GetScheduler().RemoveWorkUnitMain( DemoPostRenderWork );
        DemoPostRenderWork = NULL;
    }
    if( DemoPostPhysicsWork ) {
        TheEntresol->GetScheduler().RemoveWorkUnitMain( DemoPostPhysicsWork );
        DemoPostPhysicsWork = NULL;
    }

    MainCam = NULL;

    // Destory the World Last, ya dumbby!
    if( DemoWorld ) {
        TheEntresol->DestroyWorld( DemoWorld );
        DemoWorld = NULL;
    }

    static_cast<Physics::CollisionShapeManager*>( TheEntresol->GetManager(ManagerBase::MT_CollisionShapeManager) )->DestroyAllShapes();

    if( FirstWindow ) {
        static_cast<Graphics::GraphicsManager*>( TheEntresol->GetManager(ManagerBase::MT_GraphicsManager) )->DestroyGameWindow( FirstWindow );
        FirstWindow = NULL;
    }
}

void LoadContent()
{
    DebrisManager* DebrisMan = static_cast<DebrisManager*>( DemoWorld->GetManager(ManagerBase::MT_DebrisManager) );
    Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( DemoWorld->GetManager(ManagerBase::MT_SceneManager) );
    Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( DemoWorld->GetManager(ManagerBase::MT_PhysicsManager) );
    Resource::ResourceManager* ResourceMan = static_cast<Resource::ResourceManager*>( TheEntresol->GetManager(ManagerBase::MT_ResourceManager) );

    SceneMan->SetAmbientLight(0.10,0.10,0.10,0.10);

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
    ResourceMan->AddAssetLocation("data/common", Resource::AT_FileSystem, groupname, false);
    ResourceMan->AddAssetLocation("data/common/music", Resource::AT_FileSystem, groupname, false);
    ResourceMan->AddAssetLocation("data/common/sounds", Resource::AT_FileSystem, groupname, false);
    //ResourceMan->AddAssetLocation(zipname.str(), "Zip", groupname, false);
    ResourceMan->AddAssetLocation("", Resource::AT_FileSystem, groupname, false);
    ResourceMan->InitAssetGroup(groupname);

    Vector3 grav( 0.0, -400.0, 0.0 );
    PhysMan->SetWorldGravity(grav);
    PhysMan->SetWorldSoftGravity(grav);

    //ParticleEffect *GreenPart = SceneMan->CreateParticleEffect("GreenParticles", "Examples/GreenyNimbus");
    //GreenPart->SetLocation(Vector3(-70,70,-100));

    Physics::CollisionShapeManager* CSMan = static_cast<Physics::CollisionShapeManager*>( TheEntresol->GetManager(ManagerBase::MT_CollisionShapeManager) );
    Physics::CollisionShape* RobitCH = CSMan->GenerateConvexHull("RobitConvexHull",filerobot,groupname);
    //Physics::CollisionShape* RobitCD = CSMan->PerformConvexDecomposition("RobitConvexDecomp",filerobot,groupname,5.0,5.0,10.0);
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
        RigDeb = DebrisMan->CreateRigidDebris(namestream.str(),mass);
        RigDeb->GetRigidProxy()->SetCollisionShape(RobitCH);
        RigDeb->GetEntityProxy()->SetMesh(filerobot,groupname);
        //TheEntresol->GetResourceManager()->ImportShapeData(RigDeb, "data/common/RobotDecomp3.bullet");
        RigDeb->SetLocation(Vector3( (-2.0*PinSpacing)+(c*PinSpacing), -90.0, 0));
        RigDeb->AddToWorld();
    }

    for(unsigned int c=0; c<3; c++)     //the row with three pins
    {
        std::stringstream namestream;
        namestream << robotprefix << (c+4);
        RigDeb = DebrisMan->CreateRigidDebris(namestream.str(),mass);
        RigDeb->GetRigidProxy()->SetCollisionShape(RobitCH);
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
        RigDeb = DebrisMan->CreateRigidDebris(namestream.str(),mass);
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
    RigDeb = DebrisMan->CreateRigidDebris(namestream.str(),mass);
    RigDeb->GetRigidProxy()->SetCollisionShape(RobitCH);
    RigDeb->GetEntityProxy()->SetMesh(filerobot,groupname);
    RigDeb->SetLocation(Vector3( (-0.5*PinSpacing), 0.0, -PinSpacing*3));
    RigDeb->AddToWorld();

    object5 = DebrisMan->CreateRigidDebris("Plane",0);
    object5->GetRigidProxy()->SetCollisionShape(PlaneStatic);
    object5->GetEntityProxy()->SetMesh("Plane.mesh",groupname);
    object5->SetLocation(Vector3(0.0,-100,-300.0));
    object5->AddToWorld();

    object6 = DebrisMan->CreateRigidDebris("Ramp",0);
    object6->GetRigidProxy()->SetCollisionShape(PlaneStatic);
    object6->GetEntityProxy()->SetMesh("Plane.mesh",groupname);
    object6->SetLocation(Vector3(00.0,300.0,-1100.0));
    object6->SetOrientation(Quaternion(0.5, 0.0, 0.0, -0.25));
    object6->AddToWorld();

    object1 = DebrisMan->CreateRigidDebris("RobotWayUpFrontRight",mass);
    object1->GetRigidProxy()->SetCollisionShape(RobitCH);
    object1->GetEntityProxy()->SetMesh(filerobot,groupname);
    object1->SetLocation(Vector3(400,70,100));
    object1->SetOrientation(Quaternion(0.5, 0.5, 0.0, 0.9));
    object1->AddToWorld();

    object2 = DebrisMan->CreateRigidDebris("WoodSphere",150.0);
    object2->GetRigidProxy()->SetCollisionShape(WoodenSphere);
    object2->GetEntityProxy()->SetMesh("Sphere_Wood.mesh",groupname);
    object2->SetScale(Vector3(0.5,0.5,0.5));
    object2->SetLocation(Vector3(-140.0,2800.0,-1150.0));
    object2->AddToWorld();

    object3 = DebrisMan->CreateRigidDebris("MetalSphere",200.0);
    object3->GetRigidProxy()->SetCollisionShape(MetalSphere);
    object3->GetEntityProxy()->SetMesh("Sphere_Metal.mesh",groupname);
    object3->SetScale(Vector3(0.7,0.7,0.7));
    object3->SetLocation(Vector3(150.0,1800.0,-1300.0));
    object3->AddToWorld();

    object4 = DebrisMan->CreateRigidDebris("RobotWayUpFrontLeft",mass);
    object4->GetRigidProxy()->SetCollisionShape(RobitCH);
    object4->GetEntityProxy()->SetMesh(filerobot,groupname);
    object4->SetLocation(Vector3(-400,10, 100));
    object4->SetOrientation(Quaternion(0.5, 0.5, 0.0, 0.9));
    object4->AddToWorld();

    object7 = DebrisMan->CreateRigidDebris("MetalSphere2",800.0);
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
    PhysMan->AddAreaEffect(Reverse); // Now that we have passed it, we can forget about it*/

    //GravityWell
    /*GravityWell* BlackHole = new GravityWell("BlackHole", Vector3(0.0,200.0,-300.0));
    BlackHole->GetPhysicsSettings()->SetCollisionShape(new Physics::SphereCollisionShape("GravWellShape",750.0));
    BlackHole->SetAllowWorldGravity(false);
    BlackHole->SetFieldStrength(1000.0);
    BlackHole->SetAttenuation(3.000,Mezzanine::Att_Linear);
    //BlackHole->GetGraphicsSettings()->SetMesh(MeshManager::GetSingletonPtr()->CreateSphereMesh("GravWellMesh",ColourValue(0.8,0.1,0.1,0.15),750.0));
    PhysMan->AddAreaEffect(BlackHole);// */

    //Physics::RigidProxy* InvisFloor = PhysMan->CreateRigidProxy(0,NULL);
    //InvisFloor->SetCollisionShape( new Physics::PlaneCollisionShape("InvisFloor",Plane(Vector3::Unit_Y(),Vector3(0,-300,0))) );
    //InvisFloor->AddToWorld();

    //Final Steps
    Audio::iSound* Sound = NULL;
    Audio::AudioManager* AudioMan = static_cast<Audio::AudioManager*>( TheEntresol->GetManager(ManagerBase::MT_AudioManager) );
    Sound = AudioMan->CreateDialogSound("welcomefun-1-edit.ogg", groupname);
    Announcer.push_back(Sound);
    Sound = AudioMan->CreateDialogSound("welcomefun-1-edit.ogg", groupname);
    Announcer.push_back(Sound);

    Sound = AudioMan->CreateMusicSound("cAudioTheme1.ogg", groupname);
    Soundtrack.push_back(Sound);
    Sound = AudioMan->CreateMusicSound("cAudioTheme2.ogg", groupname);
    Soundtrack.push_back(Sound);

    TheEntresol->_Log("Debris Count ");
    TheEntresol->_Log( DebrisMan->GetNumDebris() );
}

void MakeGUI()
{
    String DefaultScreen = "DefaultScreen";
    UI::UIManager* GUI = static_cast<UI::UIManager*>( TheEntresol->GetManager(ManagerBase::MT_UIManager) );
    Graphics::Viewport* UIViewport = static_cast<Graphics::GraphicsManager*>( TheEntresol->GetManager(ManagerBase::MT_GraphicsManager) )->GetGameWindow(0)->GetViewport(0);
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
    UI::SingleImageLayer* AccessBackground = MenuAccess->CreateSingleImageLayer(UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    AccessBackground->SetColour(ButtonColour);
    UI::SingleImageLayer* AccessHoveredBackground = MenuAccess->CreateSingleImageLayer(UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    AccessHoveredBackground->SetColour(HoveredButtonColour);
    UI::SingleLineTextLayer* AccessText = MenuAccess->CreateSingleLineTextLayer(1,1);
    AccessText->HorizontallyAlign(UI::LA_Center);
    AccessText->VerticallyAlign(UI::LA_Center);
    AccessText->SetDefaultFont("Ubuntu-14");
    AccessText->SetText("Menu");
    DScreen->AddChild(MenuAccess,1);

    UI::Widget* CurFPS = DScreen->CreateWidget("D_CurFPS",UI::UnifiedRect(0.16,0.008,0.06,0.065));
    UI::SingleLineTextLayer* CurFPSText = CurFPS->CreateSingleLineTextLayer(1,1);
    CurFPSText->HorizontallyAlign(UI::LA_TopLeft);
    CurFPSText->VerticallyAlign(UI::LA_Center);
    CurFPSText->SetDefaultFont("Ubuntu-14");
    CurFPSText->SetText("0.0");
    DScreen->AddChild(CurFPS,2);

    UI::Widget* CurFPSStat = DScreen->CreateWidget("D_CurFPSStat",UI::UnifiedRect(0.008,0.008,0.15,0.065));
    UI::SingleLineTextLayer* CurFPSStatText = CurFPSStat->CreateSingleLineTextLayer(1,1);
    CurFPSStatText->HorizontallyAlign(UI::LA_TopLeft);
    CurFPSStatText->VerticallyAlign(UI::LA_Center);
    CurFPSStatText->SetDefaultFont("Ubuntu-14");
    CurFPSStatText->SetText("Current FPS: ");
    DScreen->AddChild(CurFPSStat,3);

    UI::Widget* AvFPS = DScreen->CreateWidget("D_AvFPS",UI::UnifiedRect(0.16,0.081,0.06,0.065));
    UI::SingleLineTextLayer* AvFPSText = AvFPS->CreateSingleLineTextLayer(1,1);
    AvFPSText->HorizontallyAlign(UI::LA_TopLeft);
    AvFPSText->VerticallyAlign(UI::LA_Center);
    AvFPSText->SetDefaultFont("Ubuntu-14");
    AvFPSText->SetText("0.0");
    DScreen->AddChild(AvFPS,4);

    UI::Widget* AvFPSStat = DScreen->CreateWidget("D_AvFPSStat",UI::UnifiedRect(0.008,0.081,0.15,0.065));
    UI::SingleLineTextLayer* AvFPSStatText = AvFPSStat->CreateSingleLineTextLayer(1,1);
    AvFPSStatText->HorizontallyAlign(UI::LA_TopLeft);
    AvFPSStatText->VerticallyAlign(UI::LA_Center);
    AvFPSStatText->SetDefaultFont("Ubuntu-14");
    AvFPSStatText->SetText("Average FPS: ");
    DScreen->AddChild(AvFPSStat,5);
    //End of HUD
    //Build the Menu
    UI::MenuEntry* DemoMenu = DScreen->CreateMenuEntry("D_Menu",UI::UnifiedRect(0.35,0.27,0.3,0.45));
    UI::SingleImageLayer* DemoMenuBackground = DemoMenu->CreateSingleImageLayer(1,1);
    DemoMenuBackground->SetColour(MenuColour);
    DScreen->AddChild(DemoMenu,6);

    UI::StackButton* ReturnButton = DScreen->CreateStackButton("D_Return",UI::UnifiedRect(0.10,0.56,0.80,0.16));
    UI::SingleImageLayer* ReturnBackground = ReturnButton->CreateSingleImageLayer(UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    ReturnBackground->SetColour(ButtonColour);
    UI::SingleImageLayer* ReturnHoveredBackground = ReturnButton->CreateSingleImageLayer(UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    ReturnHoveredBackground->SetColour(HoveredButtonColour);
    UI::SingleLineTextLayer* ReturnText = ReturnButton->CreateSingleLineTextLayer(1,1);
    ReturnText->HorizontallyAlign(UI::LA_Center);
    ReturnText->VerticallyAlign(UI::LA_Center);
    ReturnText->SetDefaultFont("Ubuntu-14");
    ReturnText->SetText("Return To Demo");
    DemoMenu->AddChild(ReturnButton,1);

    UI::Button* GameExitButton = DScreen->CreateButton("D_Exit",UI::UnifiedRect(0.10,0.78,0.80,0.16));
    UI::SingleImageLayer* GameExitBackground = GameExitButton->CreateSingleImageLayer(UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GameExitBackground->SetColour(ButtonColour);
    UI::SingleImageLayer* GameExitHoveredBackground = GameExitButton->CreateSingleImageLayer(UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    GameExitHoveredBackground->SetColour(HoveredButtonColour);
    UI::SingleLineTextLayer* GameExitText = GameExitButton->CreateSingleLineTextLayer(1,1);
    GameExitText->HorizontallyAlign(UI::LA_Center);
    GameExitText->VerticallyAlign(UI::LA_Center);
    GameExitText->SetDefaultFont("Ubuntu-14");
    GameExitText->SetText("Exit Demo");
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
    // */
}
// */
#endif
