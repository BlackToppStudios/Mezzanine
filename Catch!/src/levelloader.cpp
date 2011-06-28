#ifndef _levelloader_cpp
#define _levelloader_cpp

#include "levelloader.h"
#include "catchapp.h"

LevelLoader::LevelLoader()
    : LevelToLoad("")
{
    TheWorld = World::GetWorldPointer();
    GameApp = CatchApp::GetCatchAppPointer();
}

LevelLoader::~LevelLoader()
{
}

void LevelLoader::LoadFerris()
{
    // Get managers
    PhysicsManager* PhysMan = TheWorld->GetPhysicsManager();
    SceneManager* SceneMan = TheWorld->GetSceneManager();
    ResourceManager* ResourceMan = TheWorld->GetResourceManager();
    ActorManager* ActMan = TheWorld->GetActorManager();

    String CommonGroup("Common");
    String FerrisGroup("Ferris");
    String datadir = "Levels/";
    ResourceMan->AddResourceLocation(datadir+"Ferris.lvl", "Zip", FerrisGroup, false);
    ResourceMan->InitResourceGroup(FerrisGroup);

    // Camera Setup
	Camera* DefCamera = TheWorld->GetCameraManager()->GetDefaultCamera();
	DefCamera->SetLocation(Vector3(0,0,425));
	DefCamera->LookAt(Vector3(0,0,0));

	// Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Light* DLight = SceneMan->CreateLight("SceneLight");
    DLight->SetType(Light::Directional);
    Vector3 Loc(150,250,-200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(1,1,1,1));
    DLight->SetSpecularColour(ColourValue(1,1,1,1));

    // Physics Setup
    PhysMan->SetGravity(Vector3(0,-1000,0));

    //Configure the wireframe Drawer
    //PhysMan->SetDebugPhysicsWireCount(2);
    //PhysMan->SetDebugPhysicsRendering(1);

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,120),Vector3(15,-10,120),Vector3(0,10,120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Create the Wheel
    ActorRigid* FerrisWheel = new ActorRigid(100.0,"FerrisWheel","wheel.mesh",FerrisGroup);
    ResourceMan->ImportShapeData(FerrisWheel,"ferris_wheel.bullet");
    FerrisWheel->SetLocation(100,0,0);
    FerrisWheel->GetPhysicsSettings()->SetDamping(0,0.1);
    ActMan->AddActor(FerrisWheel);

    // Create the trayz
    Real TrayMass = 50.0;
    ActorRigid* Tray1 = new ActorRigid(TrayMass,"Tray1","tray.mesh",FerrisGroup);
    TheWorld->GetResourceManager()->ImportShapeData(Tray1,"ferris_tray.bullet");
    PhysMan->StorePhysicsShape(Tray1,"TrayShape");
    Tray1->SetLocation(30.4,14.3,0);
    Tray1->GetPhysicsSettings()->SetDamping(0,0.1);
    ActMan->AddActor(Tray1);
    ActorRigid* Tray2 = new ActorRigid(TrayMass,"Tray2","tray.mesh",FerrisGroup);
    PhysMan->ApplyPhysicsShape(Tray2,"TrayShape");
    Tray2->SetLocation(169.6,14.3,0);
    Tray2->GetPhysicsSettings()->SetDamping(0,0.1);
    ActMan->AddActor(Tray2);
    ActorRigid* Tray3 = new ActorRigid(TrayMass,"Tray3","tray.mesh",FerrisGroup);
    PhysMan->ApplyPhysicsShape(Tray3,"TrayShape");
    Tray3->SetLocation(30.4,-42.7,0);
    Tray3->GetPhysicsSettings()->SetDamping(0,0.1);
    ActMan->AddActor(Tray3);
    ActorRigid* Tray4 = new ActorRigid(TrayMass,"Tray4","tray.mesh",FerrisGroup);
    PhysMan->ApplyPhysicsShape(Tray4,"TrayShape");
    Tray4->SetLocation(169.6,-42.7,0);
    Tray4->GetPhysicsSettings()->SetDamping(0,0.1);
    ActMan->AddActor(Tray4);
    ActorRigid* Tray5 = new ActorRigid(TrayMass,"Tray5","tray.mesh",FerrisGroup);
    PhysMan->ApplyPhysicsShape(Tray5,"TrayShape");
    Tray5->SetLocation(71.5,55.4,0);
    Tray5->GetPhysicsSettings()->SetDamping(0,0.1);
    ActMan->AddActor(Tray5);
    ActorRigid* Tray6 = new ActorRigid(TrayMass,"Tray6","tray.mesh",FerrisGroup);
    PhysMan->ApplyPhysicsShape(Tray6,"TrayShape");
    Tray6->SetLocation(128.5,55.4,0);
    Tray6->GetPhysicsSettings()->SetDamping(0,0.1);
    ActMan->AddActor(Tray6);
    ActorRigid* Tray7 = new ActorRigid(TrayMass,"Tray7","tray.mesh",FerrisGroup);
    PhysMan->ApplyPhysicsShape(Tray7,"TrayShape");
    Tray7->SetLocation(71.5,-83.8,0);
    Tray7->GetPhysicsSettings()->SetDamping(0,0.1);
    ActMan->AddActor(Tray7);
    ActorRigid* Tray8 = new ActorRigid(TrayMass,"Tray8","tray.mesh",FerrisGroup);
    PhysMan->ApplyPhysicsShape(Tray8,"TrayShape");
    Tray8->SetLocation(128.5,-83.8,0);
    Tray8->GetPhysicsSettings()->SetDamping(0,0.1);
    ActMan->AddActor(Tray8);// */

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
    Real UraniumMass = 10.0;
    Real GoldMass = 15.0;
    Real LeadMass = 12.0;
    Real ClayMass = 6.0;
    ActorRigid* Uranium1 = new ActorRigid(UraniumMass,"Uranium1","uranium.mesh",CommonGroup);
    Uranium1->CreateShapeFromMeshDynamic(1);
    Uranium1->SetLocation(-145,40,0);
    Uranium1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Uranium1->LimitMovementOnAxis(true,true,false);
    ActMan->AddActor(Uranium1);
    GameApp->AddThrowable(Uranium1);
    ActorRigid* Uranium2 = new ActorRigid(UraniumMass,"Uranium2","uranium.mesh",CommonGroup);
    Uranium2->CreateShapeFromMeshDynamic(1);
    Uranium2->SetLocation(-195,40,0);
    Uranium2->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Uranium2->LimitMovementOnAxis(true,true,false);
    ActMan->AddActor(Uranium2);
    GameApp->AddThrowable(Uranium2);// */
    ActorRigid* Lead1 = new ActorRigid(LeadMass,"Lead1","lead.mesh",CommonGroup);
    Lead1->CreateShapeFromMeshDynamic(1);
    Lead1->SetLocation(-145,0,0);
    Lead1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Lead1->LimitMovementOnAxis(true,true,false);
    ActMan->AddActor(Lead1);
    GameApp->AddThrowable(Lead1);
    ActorRigid* Lead2 = new ActorRigid(LeadMass,"Lead2","lead.mesh",CommonGroup);
    Lead2->CreateShapeFromMeshDynamic(1);
    Lead2->SetLocation(-195,0,0);
    Lead2->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Lead2->LimitMovementOnAxis(true,true,false);
    ActMan->AddActor(Lead2);
    GameApp->AddThrowable(Lead2);// */
    ActorRigid* Clay1 = new ActorRigid(ClayMass,"Clay1","clay.mesh",CommonGroup);
    Clay1->CreateShapeFromMeshDynamic(1);
    Clay1->SetLocation(-145,-40,0);
    Clay1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Clay1->LimitMovementOnAxis(true,true,false);
    ActMan->AddActor(Clay1);
    GameApp->AddThrowable(Clay1);
    ActorRigid* Clay2 = new ActorRigid(ClayMass,"Clay2","clay.mesh",CommonGroup);
    Clay2->CreateShapeFromMeshDynamic(1);
    Clay2->SetLocation(-195,-40,0);
    Clay2->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Clay2->LimitMovementOnAxis(true,true,false);
    ActMan->AddActor(Clay2);
    GameApp->AddThrowable(Clay2);// */

    // Create the zones
    AreaOfPlay* PlayZone = new AreaOfPlay("PlayArea",Vector3(0,0,0));
    PlayZone->CreateBoxShape(Vector3(280,280,35));
    PhysMan->AddAreaEffect(PlayZone);
    GameApp->SetPlayArea(PlayZone);// */

    StartingArea* StartZone = new StartingArea("StartArea",Vector3(-170,0,0));
    StartZone->CreateBoxShape(Vector3(50,130,15));
    StartZone->CreateGraphicsBox(ColourValue(0.1,0.8,0.1,0.2));
    PhysMan->AddAreaEffect(StartZone);
    GameApp->RegisterStartArea(StartZone);// */

    ScoreArea* ScoreZone = new ScoreArea("ScoreArea",Vector3(100,0,0));
    ScoreZone->CreateBoxShape(Vector3(100,100,30));
    ScoreZone->CreateGraphicsBox(ColourValue(0.2,0.2,0.8,0.2));
    PhysMan->AddAreaEffect(ScoreZone);
    GameApp->RegisterScoreArea(ScoreZone);// */
}

void LevelLoader::LoadBigCurve()
{
    // Get managers
    PhysicsManager* PhysMan = TheWorld->GetPhysicsManager();
    SceneManager* SceneMan = TheWorld->GetSceneManager();
    ResourceManager* ResourceMan = TheWorld->GetResourceManager();
    ActorManager* ActMan = TheWorld->GetActorManager();

    String CommonGroup("Common");
    String BigCurveGroup("BigCurve");
    String datadir = "Levels/";
    ResourceMan->AddResourceLocation(datadir+"BigCurve.lvl", "Zip", BigCurveGroup, false);
    ResourceMan->InitResourceGroup(BigCurveGroup);

    // Camera Setup
	Camera* DefCamera = TheWorld->GetCameraManager()->GetDefaultCamera();
	DefCamera->SetLocation(Vector3(0,0,425));
	DefCamera->LookAt(Vector3(0,0,0));

	// Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Light* DLight = SceneMan->CreateLight("SceneLight");
    DLight->SetType(Light::Directional);
    Vector3 Loc(150,200,-200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(1,1,1,1));
    DLight->SetSpecularColour(ColourValue(1,1,1,1));

    // Physics Setup
    PhysMan->SetGravity(Vector3(0,-1000,0));

    //Configure the wireframe Drawer
    //PhysMan->SetDebugPhysicsWireCount(2);
    //PhysMan->SetDebugPhysicsRendering(1);

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,120),Vector3(15,-10,120),Vector3(0,10,120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Create the Cave
    ActorRigid* CurveCave = new ActorRigid(0,"CurveCave","cave.mesh",BigCurveGroup);
    CurveCave->CreateShapeFromMeshStatic();
    CurveCave->SetLocation(Vector3(130,0,-10));
    CurveCave->SetActorScaling(Vector3(1.05,1.05,1.05));
    ActMan->AddActor(CurveCave);

    // Create some throwable objects
    Real UraniumMass = 10.0;
    ActorRigid* Uranium1 = new ActorRigid(UraniumMass,"Uranium1","uranium.mesh",CommonGroup);
    Uranium1->CreateShapeFromMeshDynamic(1);
    Uranium1->SetLocation(-155,0,0);
    Uranium1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Uranium1->LimitMovementOnAxis(true,true,false);
    ActMan->AddActor(Uranium1);
    GameApp->AddThrowable(Uranium1);
    ActorRigid* Uranium2 = new ActorRigid(UraniumMass,"Uranium2","uranium.mesh",CommonGroup);
    Uranium2->CreateShapeFromMeshDynamic(1);
    Uranium2->SetLocation(-205,0,0);
    Uranium2->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Uranium2->LimitMovementOnAxis(true,true,false);
    ActMan->AddActor(Uranium2);
    GameApp->AddThrowable(Uranium2);// */

    // Create the zones
    AreaOfPlay* PlayZone = new AreaOfPlay("PlayArea",Vector3(0,0,0));
    PlayZone->CreateBoxShape(Vector3(280,280,35));
    PhysMan->AddAreaEffect(PlayZone);
    GameApp->SetPlayArea(PlayZone);// */

    StartingArea* StartZone = new StartingArea("StartArea",Vector3(-180,-60,0));
    StartZone->CreateBoxShape(Vector3(50,80,15));
    StartZone->CreateGraphicsBox(ColourValue(0.1,0.8,0.1,0.2));
    PhysMan->AddAreaEffect(StartZone);
    GameApp->RegisterStartArea(StartZone);// */

    ScoreArea* ScoreZone = new ScoreArea("ScoreArea",Vector3(120,-20,-10));
    ScoreZone->CreateBoxShape(Vector3(50,40,35));
    ScoreZone->CreateGraphicsBox(ColourValue(0.2,0.2,0.8,0.2));
    PhysMan->AddAreaEffect(ScoreZone);
    GameApp->RegisterScoreArea(ScoreZone);// */
}

void LevelLoader::LoadBlowsNotSucks()
{
    // Get managers
    PhysicsManager* PhysMan = TheWorld->GetPhysicsManager();
    SceneManager* SceneMan = TheWorld->GetSceneManager();
    ResourceManager* ResourceMan = TheWorld->GetResourceManager();
    ActorManager* ActMan = TheWorld->GetActorManager();

    String CommonGroup("Common");
    String BlowsNotSucksGroup("BlowsNotSucks");
    String datadir = "Levels/";
    ResourceMan->AddResourceLocation(datadir+"BlowsNotSucks.lvl", "Zip", BlowsNotSucksGroup, false);
    ResourceMan->InitResourceGroup(BlowsNotSucksGroup);

    // Camera Setup
	Camera* DefCamera = TheWorld->GetCameraManager()->GetDefaultCamera();
	DefCamera->SetLocation(Vector3(0,0,425));
	DefCamera->LookAt(Vector3(0,0,0));

	// Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Light* DLight = SceneMan->CreateLight("SceneLight");
    DLight->SetType(Light::Directional);
    Vector3 Loc(150,200,-200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(1,1,1,1));
    DLight->SetSpecularColour(ColourValue(1,1,1,1));

    // Physics Setup
    PhysMan->SetGravity(Vector3(0,-1000,0));

    //Configure the wireframe Drawer
    //PhysMan->SetDebugPhysicsWireCount(2);
    //PhysMan->SetDebugPhysicsRendering(1);

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,120),Vector3(15,-10,120),Vector3(0,10,120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Create the fan
    ActorRigid* Fan = new ActorRigid(0,"Fan","fan.mesh",BlowsNotSucksGroup);
    Fan->CreateShapeFromMeshDynamic(3);
    Fan->SetLocation(Vector3(110,0,0));
    ActMan->AddActor(Fan);

    ActorRigid* FanBody = new ActorRigid(0,"FanBody","fan.mesh",BlowsNotSucksGroup);
    FanBody->CreateShapeFromMeshDynamic(3);
    FanBody->SetLocation(Vector3(110,0,0));
    ActMan->AddActor(FanBody);

    ActorRigid* FanButton = new ActorRigid(0,"FanButton","button.mesh",BlowsNotSucksGroup);
    FanButton->CreateShapeFromMeshDynamic(3);
    FanButton->SetLocation(Vector3(110,0,0));
    ActMan->AddActor(FanButton);

    // Create the goal tray

    // Create some throwable objects

    // Create the zones
}

void LevelLoader::LoadJustice()
{
    // Get managers
    PhysicsManager* PhysMan = TheWorld->GetPhysicsManager();
    SceneManager* SceneMan = TheWorld->GetSceneManager();
    ResourceManager* ResourceMan = TheWorld->GetResourceManager();
    ActorManager* ActMan = TheWorld->GetActorManager();

    String CommonGroup("Common");
    String JusticeGroup("Justice");
    String datadir = "Levels/";
    ResourceMan->AddResourceLocation(datadir+"Justice.lvl", "Zip", JusticeGroup, false);
    ResourceMan->InitResourceGroup(JusticeGroup);

    // Camera Setup
	Camera* DefCamera = TheWorld->GetCameraManager()->GetDefaultCamera();
	DefCamera->SetLocation(Vector3(0,0,425));
	DefCamera->LookAt(Vector3(0,0,0));

	// Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Light* DLight = SceneMan->CreateLight("SceneLight");
    DLight->SetType(Light::Directional);
    Vector3 Loc(-150,100,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(1,1,1,1));
    DLight->SetSpecularColour(ColourValue(1,1,1,1));

    // Physics Setup
    PhysMan->SetGravity(Vector3(0,-1000,0));

    //Configure the wireframe Drawer
    PhysMan->SetDebugPhysicsWireCount(2);
    PhysMan->SetDebugPhysicsRendering(1);

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,120),Vector3(15,-10,120),Vector3(0,10,120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Create Lady Justice
    ActorRigid* LadyJustice = new ActorRigid(0,"LadyJustice","lady.mesh",JusticeGroup);
    LadyJustice->CreateShapeFromMeshStatic();
    LadyJustice->SetLocation(Vector3(100,-40,-103.35));
    ActMan->AddActor(LadyJustice);

    // Create the scale objects
    ActorRigid* JusticeScale = new ActorRigid(50,"JusticeScale","scale.mesh",JusticeGroup);
    ResourceMan->ImportShapeData(JusticeScale,"justice_scale.bullet");
    JusticeScale->SetLocation(Vector3(88,47.4,0));
    ActMan->AddActor(JusticeScale);

    // Create the series of constraints for connecting all the pieces of the scale together
    HingeConstraint* ScaleAnchor = new HingeConstraint(LadyJustice,JusticeScale,Vector3(-12,127.4,103.35),Vector3(0,40,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(ScaleAnchor,true);

    // Create some throwable objects

    // Create the zones
    AreaOfPlay* PlayZone = new AreaOfPlay("PlayArea",Vector3(0,0,0));
    PlayZone->CreateBoxShape(Vector3(280,280,35));
    PhysMan->AddAreaEffect(PlayZone);
    GameApp->SetPlayArea(PlayZone);// */

    StartingArea* StartZone = new StartingArea("StartArea",Vector3(-180,-40,0));
    StartZone->CreateBoxShape(Vector3(50,80,15));
    StartZone->CreateGraphicsBox(ColourValue(0.1,0.8,0.1,0.2));
    PhysMan->AddAreaEffect(StartZone);
    GameApp->RegisterStartArea(StartZone);// */

    /*ScoreArea* ScoreZone = new ScoreArea("ScoreArea",Vector3(120,-20,-10));
    ScoreZone->CreateBoxShape(Vector3(50,40,35));
    ScoreZone->CreateGraphicsBox(ColourValue(0.2,0.2,0.8,0.2));
    PhysMan->AddAreaEffect(ScoreZone);
    GameApp->RegisterScoreArea(ScoreZone);// */
}

bool LevelLoader::HasALevelToLoad()
{
    return "" != LevelToLoad;
}

void LevelLoader::SetNextLevel(const String& LevelName)
{
    LevelToLoad = LevelName;
}

String LevelLoader::GetNextLevel()
{
    return LevelToLoad;
}

String LevelLoader::GetCurrentLevel()
{
    return CurrentLevel;
}

void LevelLoader::LoadLevel()
{
    /// @todo The if(Level) statements need to removed in favor of something that will parse an XML file of the level name.
    if("Ferris" == LevelToLoad)
        LoadFerris();
    else if("BigCurve" == LevelToLoad)
        LoadBigCurve();
    else if("BlowsNotSucks" == LevelToLoad)
        LoadBlowsNotSucks();
    else if("Justice" == LevelToLoad)
        LoadJustice();

    CurrentLevel = LevelToLoad;
    LevelToLoad = "";
}

#endif
