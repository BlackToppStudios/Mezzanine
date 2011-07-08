#ifndef _levelloader_cpp
#define _levelloader_cpp

#include "levelloader.h"
#include "catchapp.h"
#include "throwablegenerator.h"

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
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));

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
    ThrowableData* ClayData = ThrowableGenerator::GetThrowableData("Clay");
    ThrowableData* LeadData = ThrowableGenerator::GetThrowableData("Lead");
    ThrowableData* UraniumData = ThrowableGenerator::GetThrowableData("Uranium");
    ActorRigid* Uranium1 = new ActorRigid(UraniumData->Mass,"Uranium1",UraniumData->MeshName,CommonGroup);
    Uranium1->CreateShapeFromMeshDynamic(1);
    Uranium1->SetLocation(-145,40,0);
    Uranium1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Uranium1->LimitMovementOnAxis(true,true,false);
    ActMan->AddActor(Uranium1);
    GameApp->AddThrowable(Uranium1);
    ActorRigid* Uranium2 = new ActorRigid(UraniumData->Mass,"Uranium2",UraniumData->MeshName,CommonGroup);
    Uranium2->CreateShapeFromMeshDynamic(1);
    Uranium2->SetLocation(-195,40,0);
    Uranium2->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Uranium2->LimitMovementOnAxis(true,true,false);
    ActMan->AddActor(Uranium2);
    GameApp->AddThrowable(Uranium2);// */
    ActorRigid* Lead1 = new ActorRigid(LeadData->Mass,"Lead1",LeadData->MeshName,CommonGroup);
    Lead1->CreateShapeFromMeshDynamic(1);
    Lead1->SetLocation(-145,0,0);
    Lead1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Lead1->LimitMovementOnAxis(true,true,false);
    ActMan->AddActor(Lead1);
    GameApp->AddThrowable(Lead1);
    ActorRigid* Lead2 = new ActorRigid(LeadData->Mass,"Lead2",LeadData->MeshName,CommonGroup);
    Lead2->CreateShapeFromMeshDynamic(1);
    Lead2->SetLocation(-195,0,0);
    Lead2->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Lead2->LimitMovementOnAxis(true,true,false);
    ActMan->AddActor(Lead2);
    GameApp->AddThrowable(Lead2);// */
    ActorRigid* Clay1 = new ActorRigid(ClayData->Mass,"Clay1",ClayData->MeshName,CommonGroup);
    Clay1->CreateShapeFromMeshDynamic(1);
    Clay1->SetLocation(-145,-40,0);
    Clay1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Clay1->LimitMovementOnAxis(true,true,false);
    ActMan->AddActor(Clay1);
    GameApp->AddThrowable(Clay1);
    ActorRigid* Clay2 = new ActorRigid(ClayData->Mass,"Clay2",ClayData->MeshName,CommonGroup);
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
    Vector3 Loc(150,200,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));

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
    ThrowableData* UraniumData = ThrowableGenerator::GetThrowableData("Uranium");
    ActorRigid* Uranium1 = new ActorRigid(UraniumData->Mass,"Uranium1",UraniumData->MeshName,CommonGroup);
    Uranium1->CreateShapeFromMeshDynamic(1);
    Uranium1->SetLocation(-155,0,0);
    Uranium1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Uranium1->LimitMovementOnAxis(true,true,false);
    ActMan->AddActor(Uranium1);
    GameApp->AddThrowable(Uranium1);
    ActorRigid* Uranium2 = new ActorRigid(UraniumData->Mass,"Uranium2",UraniumData->MeshName,CommonGroup);
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
    Vector3 Loc(-150,200,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));

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
    ActorRigid* Fan = new ActorRigid(25,"Fan","fan.mesh",BlowsNotSucksGroup);
    Fan->CreateShapeFromMeshDynamic(1);
    Fan->SetLocation(Vector3(0,0,0));
    Fan->SetOrientation(Quaternion(MathTool::GetHalfPi() * 0.5,Vector3(0,0,1)));
    Fan->GetPhysicsSettings()->SetActivationState(phys::AAS_DisableDeactivation);
    ActMan->AddActor(Fan);

    ActorRigid* FanBody = new ActorRigid(0,"FanBody","body.mesh",BlowsNotSucksGroup);
    FanBody->CreateShapeFromMeshStatic();
    FanBody->SetLocation(Vector3(132.5,-70,25));
    ActMan->AddActor(FanBody);

    ActorRigid* FanButton = new ActorRigid(5,"FanButton","button.mesh",BlowsNotSucksGroup);
    FanButton->CreateShapeFromMeshDynamic(1);
    FanButton->SetLocation(Vector3(0,0,0));
    ActMan->AddActor(FanButton);

    // Create the series of constraints for assembling the fan
    HingeConstraint* FanToBody = new HingeConstraint(FanBody,Fan,Vector3(-12,-6,-17.48),Vector3(0,-24.2,0),Vector3(-1,1,0).Normalize(),Vector3(0,1,0),false);
    PhysMan->AddConstraint(FanToBody,true);
    FanToBody->EnableAngularMotor(true,2000.f,200.0f);
    FanToBody->SetLimit(1.0,-1.0);

    Generic6DofSpringConstraint* ButtonToBody = new Generic6DofSpringConstraint(FanBody,FanButton,Vector3(-128,-33.5,-25.5),Vector3(0,0,0),Quaternion(0,0,0,1),Quaternion(0,0,0,1),true);
    ButtonToBody->SetLinearUpperLimit(Vector3(0,0,0));
    ButtonToBody->SetLinearLowerLimit(Vector3(0,-10,0));
    ButtonToBody->SetAngularUpperLimit(Vector3(0,0,0));
    ButtonToBody->SetAngularLowerLimit(Vector3(0,0,0));
    ButtonToBody->EnableSpring(1,true);
    ButtonToBody->SetStiffness(1,0.1f);
    ButtonToBody->SetEquilibriumPoint(1);
    PhysMan->AddConstraint(ButtonToBody,true);

    // Create the goal tray

    // Create some throwable objects
    ThrowableData* UraniumData = ThrowableGenerator::GetThrowableData("Uranium");
    ActorRigid* Uranium1 = new ActorRigid(UraniumData->Mass,"Uranium1",UraniumData->MeshName,CommonGroup);
    Uranium1->CreateShapeFromMeshDynamic(1);
    Uranium1->SetLocation(-155,50,0);
    Uranium1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Uranium1->LimitMovementOnAxis(true,true,false);
    ActMan->AddActor(Uranium1);
    GameApp->AddThrowable(Uranium1);

    // Create the zones
    AreaOfPlay* PlayZone = new AreaOfPlay("PlayArea",Vector3(0,0,0));
    PlayZone->CreateBoxShape(Vector3(280,280,35));
    PhysMan->AddAreaEffect(PlayZone);
    GameApp->SetPlayArea(PlayZone);// */

    StartingArea* StartZone = new StartingArea("StartArea",Vector3(-170,80,0));
    StartZone->CreateBoxShape(Vector3(50,60,15));
    StartZone->CreateGraphicsBox(ColourValue(0.1,0.8,0.1,0.2));
    PhysMan->AddAreaEffect(StartZone);
    GameApp->RegisterStartArea(StartZone);// */
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
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));

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

    // Create Lady Justice
    ActorRigid* LadyJustice = new ActorRigid(0,"LadyJustice","lady.mesh",JusticeGroup);
    LadyJustice->CreateShapeFromMeshStatic();
    LadyJustice->SetLocation(Vector3(70,-40,-103.35));
    ActMan->AddActor(LadyJustice);

    // Create the scale object
    Real ScaleMass = 50.0;
    ActorRigid* JusticeScale = new ActorRigid(ScaleMass,"JusticeScale","scale.mesh",JusticeGroup);
    ResourceMan->ImportShapeData(JusticeScale,"justice_scale.bullet");
    JusticeScale->SetLocation(Vector3(58,47.4,0));
    ActMan->AddActor(JusticeScale);

    // Create the union objects
    Real UnionMass = 5.0;
    ActorRigid* Union1 = new ActorRigid(UnionMass,"Union1","union.mesh",JusticeGroup);
    Union1->CreateShapeFromMeshDynamic(1);
    Union1->SetLocation(Vector3(-80.9,2.4,-16.4));
    Union1->GetPhysicsSettings()->SetCollisionGroupAndMask(phys::CF_UserFilter1,phys::CF_AllFilter^phys::CF_UserFilter1);
    ActMan->AddActor(Union1);
    ActorRigid* Union2 = new ActorRigid(UnionMass,"Union2","union.mesh",JusticeGroup);
    Union2->CreateShapeFromMeshDynamic(1);
    Union2->SetLocation(Vector3(-29.1,2.4,-16.4));// -42.9,2.4,-16.4
    Union2->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Union2->GetPhysicsSettings()->SetCollisionGroupAndMask(phys::CF_UserFilter1,phys::CF_AllFilter^phys::CF_UserFilter1);
    ActMan->AddActor(Union2);
    ActorRigid* Union3 = new ActorRigid(UnionMass,"Union3","union.mesh",JusticeGroup);
    Union3->CreateShapeFromMeshDynamic(1);
    Union3->SetLocation(Vector3(-80.9,2.4,16.4));
    Union3->GetPhysicsSettings()->SetCollisionGroupAndMask(phys::CF_UserFilter1,phys::CF_AllFilter^phys::CF_UserFilter1);
    ActMan->AddActor(Union3);
    ActorRigid* Union4 = new ActorRigid(UnionMass,"Union4","union.mesh",JusticeGroup);
    Union4->CreateShapeFromMeshDynamic(1);
    Union4->SetLocation(Vector3(-29.1,2.4,16.4));// -42.9,2.4,16.4
    Union4->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Union4->GetPhysicsSettings()->SetCollisionGroupAndMask(phys::CF_UserFilter1,phys::CF_AllFilter^phys::CF_UserFilter1);
    ActMan->AddActor(Union4);
    ActorRigid* Union5 = new ActorRigid(UnionMass,"Union5","union.mesh",JusticeGroup);
    Union5->CreateShapeFromMeshDynamic(1);
    Union5->SetLocation(Vector3(196.9,2.4,-16.4));// 181.1,2.4,-16.4
    Union5->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Union5->GetPhysicsSettings()->SetCollisionGroupAndMask(phys::CF_UserFilter1,phys::CF_AllFilter^phys::CF_UserFilter1);
    ActMan->AddActor(Union5);
    ActorRigid* Union6 = new ActorRigid(UnionMass,"Union6","union.mesh",JusticeGroup);
    Union6->CreateShapeFromMeshDynamic(1);
    Union6->SetLocation(Vector3(145.1,2.4,-16.4));
    Union6->GetPhysicsSettings()->SetCollisionGroupAndMask(phys::CF_UserFilter1,phys::CF_AllFilter^phys::CF_UserFilter1);
    ActMan->AddActor(Union6);
    ActorRigid* Union7 = new ActorRigid(UnionMass,"Union7","union.mesh",JusticeGroup);
    Union7->CreateShapeFromMeshDynamic(1);
    Union7->SetLocation(Vector3(196.9,2.4,16.4));//181.1,2.4,16.4
    Union7->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Union7->GetPhysicsSettings()->SetCollisionGroupAndMask(phys::CF_UserFilter1,phys::CF_AllFilter^phys::CF_UserFilter1);
    ActMan->AddActor(Union7);
    ActorRigid* Union8 = new ActorRigid(UnionMass,"Union8","union.mesh",JusticeGroup);
    Union8->CreateShapeFromMeshDynamic(1);
    Union8->SetLocation(Vector3(145.1,2.4,16.4));
    Union8->GetPhysicsSettings()->SetCollisionGroupAndMask(phys::CF_UserFilter1,phys::CF_AllFilter^phys::CF_UserFilter1);
    ActMan->AddActor(Union8);

    // Create the trays
    Real TrayMass = 25.0;
    ActorRigid* Tray1 = new ActorRigid(TrayMass,"Tray1","tray.mesh",JusticeGroup);
    ResourceMan->ImportShapeData(Tray1,"justice_tray.bullet");
    Tray1->SetLocation(Vector3(-55,-61.3,0));
    ActMan->AddActor(Tray1);
    ActorRigid* Tray2 = new ActorRigid(TrayMass,"Tray2","tray.mesh",JusticeGroup);
    ResourceMan->ImportShapeData(Tray2,"justice_tray.bullet");
    Tray2->SetLocation(Vector3(171,-61.3,0));
    ActMan->AddActor(Tray2);

    // Create the series of constraints for connecting all the pieces of the scale together
    // Starting with creating the anchor for the scale
    HingeConstraint* ScaleAnchor = new HingeConstraint(LadyJustice,JusticeScale,Vector3(-12,127.4,103.35),Vector3(0,40,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(ScaleAnchor,true);

    // Original X distance from pivot on scale is 112.7, but space was needed to prevent collsions.
    // Create the scale-to-union constraints
    Point2PointConstraint* U1S = new Point2PointConstraint(JusticeScale,Union1,Vector3(-116.0,-8.4,-16.4),Vector3(22.9,45,0));
    PhysMan->AddConstraint(U1S,true);
    Point2PointConstraint* U2S = new Point2PointConstraint(JusticeScale,Union2,Vector3(-110.0,-8.4,-16.4),Vector3(22.9,45,0));
    PhysMan->AddConstraint(U2S,true);
    Point2PointConstraint* U3S = new Point2PointConstraint(JusticeScale,Union3,Vector3(-116.0,-8.4,16.4),Vector3(22.9,45,0));
    PhysMan->AddConstraint(U3S,true);
    Point2PointConstraint* U4S = new Point2PointConstraint(JusticeScale,Union4,Vector3(-110.0,-8.4,16.4),Vector3(22.9,45,0));
    PhysMan->AddConstraint(U4S,true);
    Point2PointConstraint* U5S = new Point2PointConstraint(JusticeScale,Union5,Vector3(116.0,-8.4,-16.4),Vector3(22.9,45,0));
    PhysMan->AddConstraint(U5S,true);
    Point2PointConstraint* U6S = new Point2PointConstraint(JusticeScale,Union6,Vector3(110.0,-8.4,-16.4),Vector3(22.9,45,0));
    PhysMan->AddConstraint(U6S,true);
    Point2PointConstraint* U7S = new Point2PointConstraint(JusticeScale,Union7,Vector3(116.0,-8.4,16.4),Vector3(22.9,45,0));
    PhysMan->AddConstraint(U7S,true);
    Point2PointConstraint* U8S = new Point2PointConstraint(JusticeScale,Union8,Vector3(110.0,-8.4,16.4),Vector3(22.9,45,0));
    PhysMan->AddConstraint(U8S,true);

    // Create the union-to-tray constraints
    Point2PointConstraint* U1T = new Point2PointConstraint(Union1,Tray1,Vector3(-22.9,-45,0),Vector3(-46.6,18.7,-16.4));
    PhysMan->AddConstraint(U1T,true);
    Point2PointConstraint* U2T = new Point2PointConstraint(Union2,Tray1,Vector3(-22.9,-45,0),Vector3(46.6,18.7,-16.4));
    PhysMan->AddConstraint(U2T,true);
    Point2PointConstraint* U3T = new Point2PointConstraint(Union3,Tray1,Vector3(-22.9,-45,0),Vector3(-46.6,18.7,16.4));
    PhysMan->AddConstraint(U3T,true);
    Point2PointConstraint* U4T = new Point2PointConstraint(Union4,Tray1,Vector3(-22.9,-45,0),Vector3(46.6,18.7,16.4));
    PhysMan->AddConstraint(U4T,true);
    Point2PointConstraint* U5T = new Point2PointConstraint(Union5,Tray2,Vector3(-22.9,-45,0),Vector3(46.6,18.7,-16.4));
    PhysMan->AddConstraint(U5T,true);
    Point2PointConstraint* U6T = new Point2PointConstraint(Union6,Tray2,Vector3(-22.9,-45,0),Vector3(-46.6,18.7,-16.4));
    PhysMan->AddConstraint(U6T,true);
    Point2PointConstraint* U7T = new Point2PointConstraint(Union7,Tray2,Vector3(-22.9,-45,0),Vector3(46.6,18.7,16.4));
    PhysMan->AddConstraint(U7T,true);
    Point2PointConstraint* U8T = new Point2PointConstraint(Union8,Tray2,Vector3(-22.9,-45,0),Vector3(-46.6,18.7,16.4));
    PhysMan->AddConstraint(U8T,true);

    // Create some throwable objects
    ThrowableData* ClayData = ThrowableGenerator::GetThrowableData("Clay");
    ThrowableData* LeadData = ThrowableGenerator::GetThrowableData("Lead");
    ThrowableData* UraniumData = ThrowableGenerator::GetThrowableData("Uranium");
    ActorRigid* Uranium1 = new ActorRigid(UraniumData->Mass,"Uranium1",UraniumData->MeshName,CommonGroup);
    Uranium1->CreateShapeFromMeshDynamic(1);
    Uranium1->SetLocation(-155,50,0);
    Uranium1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Uranium1->LimitMovementOnAxis(true,true,false);
    ActMan->AddActor(Uranium1);
    GameApp->AddThrowable(Uranium1);

    // Create the zones
    AreaOfPlay* PlayZone = new AreaOfPlay("PlayArea",Vector3(0,0,0));
    PlayZone->CreateBoxShape(Vector3(280,280,35));
    PhysMan->AddAreaEffect(PlayZone);
    GameApp->SetPlayArea(PlayZone);// */

    StartingArea* StartZone = new StartingArea("StartArea",Vector3(-170,80,0));
    StartZone->CreateBoxShape(Vector3(50,60,15));
    StartZone->CreateGraphicsBox(ColourValue(0.1,0.8,0.1,0.2));
    PhysMan->AddAreaEffect(StartZone);
    GameApp->RegisterStartArea(StartZone);// */

    ScoreArea* ScoreZone1 = new ScoreArea("ScoreArea1",Vector3(-55,-66.3,0));
    ScoreZone1->CreateBoxShape(Vector3(52,25,18));
    ScoreZone1->CreateGraphicsBox(ColourValue(0.2,0.2,0.8,0.2));
    PhysMan->AddAreaEffect(ScoreZone1);
    GameApp->RegisterScoreArea(ScoreZone1);// */
    ScoreArea* ScoreZone2 = new ScoreArea("ScoreArea2",Vector3(171,-66.3,0));
    ScoreZone2->CreateBoxShape(Vector3(52,25,18));
    ScoreZone2->CreateGraphicsBox(ColourValue(0.2,0.2,0.8,0.2));
    PhysMan->AddAreaEffect(ScoreZone2);
    GameApp->RegisterScoreArea(ScoreZone2);// */
}

void LevelLoader::LoadRollers()
{

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
    else if("Rollers" == LevelToLoad)
        LoadRollers();

    CurrentLevel = LevelToLoad;
    LevelToLoad = "";
}

#endif
