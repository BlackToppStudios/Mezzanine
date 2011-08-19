#ifndef _levelloader_cpp
#define _levelloader_cpp

#include "levelloader.h"
#include "catchapp.h"
#include "throwablegenerator.h"
#include "leveltriggers.h"

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
    CollisionShapeManager* CShapeMan = TheWorld->GetCollisionShapeManager();
    MeshManager* MeshMan = TheWorld->GetMeshManager();

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

    // Setup and Create the shapes that will be used.
    CShapeMan->LoadAllShapesFromFile("Ferris.bullet",FerrisGroup);

    // Create the Wheel
    ActorRigid* FerrisWheel = new ActorRigid(100.0,"FerrisWheel","wheel.mesh",FerrisGroup);
    FerrisWheel->SetLocation(100,0,0);
    FerrisWheel->GetPhysicsSettings()->SetDamping(0,0.1);
    FerrisWheel->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("wheel")); // The names aren't being loaded as we expected
    ActMan->AddActor(FerrisWheel);

    // Create the trayz
    Real TrayMass = 50.0;
    ActorRigid* Tray1 = new ActorRigid(TrayMass,"Tray1","tray.mesh",FerrisGroup);
    Tray1->SetLocation(30.4,14.3,0);
    Tray1->GetPhysicsSettings()->SetDamping(0,0.1);
    Tray1->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("tray"));
    ActMan->AddActor(Tray1);
    ActorRigid* Tray2 = new ActorRigid(TrayMass,"Tray2","tray.mesh",FerrisGroup);
    Tray2->SetLocation(169.6,14.3,0);
    Tray2->GetPhysicsSettings()->SetDamping(0,0.1);
    Tray2->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("tray"));
    ActMan->AddActor(Tray2);
    ActorRigid* Tray3 = new ActorRigid(TrayMass,"Tray3","tray.mesh",FerrisGroup);
    Tray3->SetLocation(30.4,-42.7,0);
    Tray3->GetPhysicsSettings()->SetDamping(0,0.1);
    Tray3->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("tray"));
    ActMan->AddActor(Tray3);
    ActorRigid* Tray4 = new ActorRigid(TrayMass,"Tray4","tray.mesh",FerrisGroup);
    Tray4->SetLocation(169.6,-42.7,0);
    Tray4->GetPhysicsSettings()->SetDamping(0,0.1);
    Tray4->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("tray"));
    ActMan->AddActor(Tray4);
    ActorRigid* Tray5 = new ActorRigid(TrayMass,"Tray5","tray.mesh",FerrisGroup);
    Tray5->SetLocation(71.5,55.4,0);
    Tray5->GetPhysicsSettings()->SetDamping(0,0.1);
    Tray5->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("tray"));
    ActMan->AddActor(Tray5);
    ActorRigid* Tray6 = new ActorRigid(TrayMass,"Tray6","tray.mesh",FerrisGroup);
    Tray6->SetLocation(128.5,55.4,0);
    Tray6->GetPhysicsSettings()->SetDamping(0,0.1);
    Tray6->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("tray"));
    ActMan->AddActor(Tray6);
    ActorRigid* Tray7 = new ActorRigid(TrayMass,"Tray7","tray.mesh",FerrisGroup);
    Tray7->SetLocation(71.5,-83.8,0);
    Tray7->GetPhysicsSettings()->SetDamping(0,0.1);
    Tray7->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("tray"));
    ActMan->AddActor(Tray7);
    ActorRigid* Tray8 = new ActorRigid(TrayMass,"Tray8","tray.mesh",FerrisGroup);
    Tray8->SetLocation(128.5,-83.8,0);
    Tray8->GetPhysicsSettings()->SetDamping(0,0.1);
    Tray8->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("tray"));
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
    CollisionShape* ClayCS = CShapeMan->GenerateConvexHull("ClayCS",ClayData->MeshName,CommonGroup);
    CollisionShape* LeadCS = CShapeMan->GenerateConvexHull("LeadCS",LeadData->MeshName,CommonGroup);
    CollisionShape* UraniumCS = CShapeMan->GenerateConvexHull("UraniumCS",UraniumData->MeshName,CommonGroup);
    ActorRigid* Uranium1 = new ActorRigid(UraniumData->Mass,"Uranium1",UraniumData->MeshName,CommonGroup);
    Uranium1->GetPhysicsSettings()->SetCollisionShape(UraniumCS);
    Uranium1->SetLocation(-145,40,0);
    Uranium1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Uranium1->SetLinearMovementFactor(Vector3(1,1,0));
    ActMan->AddActor(Uranium1);
    GameApp->AddThrowable(Uranium1);
    ActorRigid* Uranium2 = new ActorRigid(UraniumData->Mass,"Uranium2",UraniumData->MeshName,CommonGroup);
    Uranium2->GetPhysicsSettings()->SetCollisionShape(UraniumCS);
    Uranium2->SetLocation(-195,40,0);
    Uranium2->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Uranium2->SetLinearMovementFactor(Vector3(1,1,0));
    ActMan->AddActor(Uranium2);
    GameApp->AddThrowable(Uranium2);// */
    ActorRigid* Lead1 = new ActorRigid(LeadData->Mass,"Lead1",LeadData->MeshName,CommonGroup);
    Lead1->GetPhysicsSettings()->SetCollisionShape(LeadCS);
    Lead1->SetLocation(-145,0,0);
    Lead1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Lead1->SetLinearMovementFactor(Vector3(1,1,0));
    ActMan->AddActor(Lead1);
    GameApp->AddThrowable(Lead1);
    ActorRigid* Lead2 = new ActorRigid(LeadData->Mass,"Lead2",LeadData->MeshName,CommonGroup);
    Lead2->GetPhysicsSettings()->SetCollisionShape(LeadCS);
    Lead2->SetLocation(-195,0,0);
    Lead2->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Lead2->SetLinearMovementFactor(Vector3(1,1,0));
    ActMan->AddActor(Lead2);
    GameApp->AddThrowable(Lead2);// */
    ActorRigid* Clay1 = new ActorRigid(ClayData->Mass,"Clay1",ClayData->MeshName,CommonGroup);
    Clay1->GetPhysicsSettings()->SetCollisionShape(ClayCS);
    Clay1->SetLocation(-145,-40,0);
    Clay1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Clay1->SetLinearMovementFactor(Vector3(1,1,0));
    ActMan->AddActor(Clay1);
    GameApp->AddThrowable(Clay1);
    ActorRigid* Clay2 = new ActorRigid(ClayData->Mass,"Clay2",ClayData->MeshName,CommonGroup);
    Clay2->GetPhysicsSettings()->SetCollisionShape(ClayCS);
    Clay2->SetLocation(-195,-40,0);
    Clay2->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Clay2->SetLinearMovementFactor(Vector3(1,1,0));
    ActMan->AddActor(Clay2);
    GameApp->AddThrowable(Clay2);// */

    // Create the zones
    AreaOfPlay* PlayZone = new AreaOfPlay("PlayArea",Vector3(0,0,0));
    PlayZone->SetFieldShape(new BoxCollisionShape("PlayAreaShape",Vector3(280,280,35)));
    PhysMan->AddAreaEffect(PlayZone);
    GameApp->SetPlayArea(PlayZone);// */

    Vector3 StartSize(50,130,15);
    StartingArea* StartZone = new StartingArea("StartArea",Vector3(-170,0,0));
    StartZone->SetFieldShape(new BoxCollisionShape("StartAreaShape",StartSize));
    StartZone->SetFieldMesh(MeshMan->CreateBoxMesh("StartAreaMesh",ColourValue(0.1,0.8,0.1,0.2),StartSize));
    PhysMan->AddAreaEffect(StartZone);
    GameApp->RegisterStartArea(StartZone);// */

    Vector3 ScoreSize(100,100,30);
    ScoreArea* ScoreZone = new ScoreArea("ScoreArea",Vector3(100,0,0));
    ScoreZone->SetFieldShape(new BoxCollisionShape("ScoreAreaShape",ScoreSize));
    ScoreZone->SetFieldMesh(MeshMan->CreateBoxMesh("ScoreAreaMesh",ColourValue(0.2,0.2,0.8,0.2),ScoreSize));
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
    CollisionShapeManager* CShapeMan = TheWorld->GetCollisionShapeManager();
    MeshManager* MeshMan = TheWorld->GetMeshManager();

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

    // Setup and Create the shapes that will be used.
    // -no files to be loaded for this level-

    // Create the Cave
    ActorRigid* CurveCave = new ActorRigid(0,"CurveCave","cave.mesh",BigCurveGroup);
    CurveCave->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GenerateStaticTriMesh("Cave","cave.mesh",BigCurveGroup));
    CurveCave->SetLocation(Vector3(130,0,-10));
    CurveCave->SetActorScaling(Vector3(1.05,1.05,1.05));
    ActMan->AddActor(CurveCave);

    // Create some throwable objects
    ThrowableData* UraniumData = ThrowableGenerator::GetThrowableData("Uranium");
    ThrowableData* PyriteData = ThrowableGenerator::GetThrowableData("Pyrite");
    ThrowableData* RubberData = ThrowableGenerator::GetThrowableData("Rubber");
    CollisionShape* UraniumCS = CShapeMan->GenerateConvexHull("UraniumCS",UraniumData->MeshName,CommonGroup);
    CollisionShape* PyriteCS = CShapeMan->GenerateConvexHull("PyriteCS",PyriteData->MeshName,CommonGroup);
    //CollisionShape* RubberCS = CShapeMan->GenerateConvexHull("RubberCS",RubberData->MeshName,CommonGroup);
    ActorRigid* Uranium1 = new ActorRigid(UraniumData->Mass,"Uranium1",UraniumData->MeshName,CommonGroup);
    Uranium1->GetPhysicsSettings()->SetCollisionShape(UraniumCS);
    Uranium1->SetLocation(-155,-20,0);
    Uranium1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Uranium1->SetLinearMovementFactor(Vector3(1,1,0));
    ActMan->AddActor(Uranium1);
    GameApp->AddThrowable(Uranium1);
    ActorRigid* Uranium2 = new ActorRigid(UraniumData->Mass,"Uranium2",UraniumData->MeshName,CommonGroup);
    Uranium2->GetPhysicsSettings()->SetCollisionShape(UraniumCS);
    Uranium2->SetLocation(-205,-20,0);
    Uranium2->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Uranium2->SetLinearMovementFactor(Vector3(1,1,0));
    ActMan->AddActor(Uranium2);
    GameApp->AddThrowable(Uranium2);// */
    ActorRigid* Pyrite1 = new ActorRigid(PyriteData->Mass,"Pyrite1",PyriteData->MeshName,CommonGroup);
    Pyrite1->GetPhysicsSettings()->SetCollisionShape(PyriteCS);
    Pyrite1->SetLocation(-155,-60,0);
    Pyrite1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Pyrite1->SetLinearMovementFactor(Vector3(1,1,0));
    ActMan->AddActor(Pyrite1);
    GameApp->AddThrowable(Pyrite1);
    ActorRigid* Pyrite2 = new ActorRigid(PyriteData->Mass,"Pyrite2",PyriteData->MeshName,CommonGroup);
    Pyrite2->GetPhysicsSettings()->SetCollisionShape(PyriteCS);
    Pyrite2->SetLocation(-205,-60,0);
    Pyrite2->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Pyrite2->SetLinearMovementFactor(Vector3(1,1,0));
    ActMan->AddActor(Pyrite2);
    GameApp->AddThrowable(Pyrite2);// */

    // Create the zones
    AreaOfPlay* PlayZone = new AreaOfPlay("PlayArea",Vector3(0,0,0));
    PlayZone->SetFieldShape(new BoxCollisionShape("PlayAreaShape",Vector3(280,280,35)));
    PhysMan->AddAreaEffect(PlayZone);
    GameApp->SetPlayArea(PlayZone);// */

    Vector3 StartSize(50,80,15);
    StartingArea* StartZone = new StartingArea("StartArea",Vector3(-180,-60,0));
    StartZone->SetFieldShape(new BoxCollisionShape("StartAreaShape",StartSize));
    StartZone->SetFieldMesh(MeshMan->CreateBoxMesh("StartAreaMesh",ColourValue(0.1,0.8,0.1,0.2),StartSize));
    PhysMan->AddAreaEffect(StartZone);
    GameApp->RegisterStartArea(StartZone);// */

    Vector3 ScoreSize(50,40,35);
    ScoreArea* ScoreZone = new ScoreArea("ScoreArea",Vector3(120,-20,-10));
    ScoreZone->SetFieldShape(new BoxCollisionShape("ScoreAreaShape",ScoreSize));
    ScoreZone->SetFieldMesh(MeshMan->CreateBoxMesh("ScoreAreaMesh",ColourValue(0.2,0.2,0.8,0.2),ScoreSize));
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
    CollisionShapeManager* CShapeMan = TheWorld->GetCollisionShapeManager();
    MeshManager* MeshMan = TheWorld->GetMeshManager();

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

    // Setup and Create the shapes that will be used.
    CShapeMan->LoadAllShapesFromFile("BlowsNotSucks.bullet",BlowsNotSucksGroup);

    // Create the fan
    ActorRigid* Fan = new ActorRigid(25,"Fan","fan.mesh",BlowsNotSucksGroup);
    Fan->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Fan"));
    Fan->SetLocation(Vector3(0,0,0));
    Fan->SetOrientation(Quaternion(MathTool::GetHalfPi() * 0.5,Vector3(0,0,1)));
    Fan->GetPhysicsSettings()->SetActivationState(phys::AAS_DisableDeactivation);
    ActMan->AddActor(Fan);

    ActorRigid* FanBody = new ActorRigid(0,"FanBody","body.mesh",BlowsNotSucksGroup);
    FanBody->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GenerateStaticTriMesh("FanBody","body.mesh",BlowsNotSucksGroup));
    FanBody->SetLocation(Vector3(132.5,-70,25));
    ActMan->AddActor(FanBody);

    ActorRigid* FanButton = new ActorRigid(2.5,"FanButton","button.mesh",BlowsNotSucksGroup);
    FanButton->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GenerateConvexHull("Button","button.mesh",BlowsNotSucksGroup));
    FanButton->SetLocation(Vector3(-10.5,-107.0,-0.5));
    FanButton->SetAngularMovementFactor(Vector3(0,0,0));
    ActMan->AddActor(FanButton);
    PhysMan->SetIndividualGravity(FanButton,Vector3(0,0,0));

    // Create the series of constraints for assembling the fan
    HingeConstraint* FanToBody = new HingeConstraint(FanBody,Fan,Vector3(-12,-6,-17.5),Vector3(0,-24.2,0),Vector3(-1,1,0).Normalize(),Vector3(0,1,0),false);
    PhysMan->AddConstraint(FanToBody,true);
    FanToBody->SetLimit(1.0,-1.0);

    Generic6DofSpringConstraint* ButtonToBody = new Generic6DofSpringConstraint(FanBody,FanButton,Vector3(-143,-52,-25.5),Vector3(0,0,0),Quaternion(0,0,0,1),Quaternion(0,0,0,1),true);
    ButtonToBody->SetLinearUpperLimit(Vector3(0,15,0));
    ButtonToBody->SetLinearLowerLimit(Vector3(0,0,0));
    ButtonToBody->SetAngularUpperLimit(Vector3(0,0,0));
    ButtonToBody->SetAngularLowerLimit(Vector3(0,0,0));
    ButtonToBody->EnableSpring(1,true);
    ButtonToBody->SetStiffness(1,500.f);
    ButtonToBody->SetEquilibriumPoint(1);
    PhysMan->AddConstraint(ButtonToBody,true);

    // Create the field of force for the wind generated by the fan
    Vector3 FanWindSize(60,115,60);
    FieldOfForce* FanWind = new FieldOfForce("FanWind",Vector3(5,40,7.5));
    FanWind->SetFieldShape(new CylinderCollisionShape("FanWindShape",60,115,Vector3::Unit_Y()));
    FanWind->SetFieldMesh(MeshMan->CreateCylinderMesh("FanWindMesh",ColourValue(0.8,0.1,0.1,0.2),FanWindSize,Vector3::Unit_Y()));
    FanWind->SetOrientation(Quaternion(MathTool::GetHalfPi() * 0.5,Vector3(0,0,1)));
    FanWind->SetDirectionOfForce(Vector3(-1,1,0));
    FanWind->SetAttenuation(0.2f,phys::Att_Linear,Vector3(-125.5,-81,7.5));
    FanWind->SetFieldStrength(1000.f);
    PhysMan->AddAreaEffect(FanWind);

    // Create the trigger for the fan
    BNS_Fan* FanTrigger = new BNS_Fan("FanTigger",FanToBody,FanButton,Fan,FanWind);
    PhysMan->AddWorldTrigger(FanTrigger);

    // Create the goal tray
    ActorRigid* GoalTray = new ActorRigid(0,"GoalTray","tray.mesh",BlowsNotSucksGroup);
    GoalTray->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GenerateStaticTriMesh("GoalShape","tray.mesh",BlowsNotSucksGroup));
    GoalTray->SetLocation(Vector3(-170,100,0));
    ActMan->AddActor(GoalTray);

    // Create some throwable objects
    ThrowableData* UraniumData = ThrowableGenerator::GetThrowableData("Uranium");
    ThrowableData* StyrofoamData = ThrowableGenerator::GetThrowableData("Styrofoam");
    CollisionShape* UraniumCS = CShapeMan->GenerateConvexHull("UraniumCS",UraniumData->MeshName,CommonGroup);
    CollisionShape* StyrofoamCS = CShapeMan->GenerateConvexHull("StyrofoamCS",StyrofoamData->MeshName,CommonGroup);
    ActorRigid* Uranium1 = new ActorRigid(UraniumData->Mass,"Uranium1",UraniumData->MeshName,CommonGroup);
    Uranium1->GetPhysicsSettings()->SetCollisionShape(UraniumCS);
    Uranium1->SetLocation(-155,-20,0);
    Uranium1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Uranium1->SetLinearMovementFactor(Vector3(1,1,0));
    ActMan->AddActor(Uranium1);
    GameApp->AddThrowable(Uranium1);
    ActorRigid* Uranium2 = new ActorRigid(UraniumData->Mass,"Uranium2",UraniumData->MeshName,CommonGroup);
    Uranium2->GetPhysicsSettings()->SetCollisionShape(UraniumCS);
    Uranium2->SetLocation(-185,-20,0);
    Uranium2->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Uranium2->SetLinearMovementFactor(Vector3(1,1,0));
    ActMan->AddActor(Uranium2);
    GameApp->AddThrowable(Uranium2);// */
    ActorRigid* Styrofoam1 = new ActorRigid(StyrofoamData->Mass,"Styrofoam1",StyrofoamData->MeshName,CommonGroup);
    Styrofoam1->GetPhysicsSettings()->SetCollisionShape(StyrofoamCS);
    Styrofoam1->SetLocation(-155,-60,0);
    Styrofoam1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Styrofoam1->SetLinearMovementFactor(Vector3(1,1,0));
    ActMan->AddActor(Styrofoam1);
    GameApp->AddThrowable(Styrofoam1);
    ActorRigid* Styrofoam2 = new ActorRigid(StyrofoamData->Mass,"Styrofoam2",StyrofoamData->MeshName,CommonGroup);
    Styrofoam2->GetPhysicsSettings()->SetCollisionShape(StyrofoamCS);
    Styrofoam2->SetLocation(-185,-60,0);
    Styrofoam2->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Styrofoam2->SetLinearMovementFactor(Vector3(1,1,0));
    ActMan->AddActor(Styrofoam2);
    GameApp->AddThrowable(Styrofoam2);// */

    // Create the zones
    AreaOfPlay* PlayZone = new AreaOfPlay("PlayArea",Vector3(0,0,0));
    PlayZone->SetFieldShape(new BoxCollisionShape("PlayAreaShape",Vector3(280,280,35)));
    PhysMan->AddAreaEffect(PlayZone);
    GameApp->SetPlayArea(PlayZone);// */

    Vector3 StartSize(50,70,15);
    StartingArea* StartZone = new StartingArea("StartArea",Vector3(-170,-70,0));
    StartZone->SetFieldShape(new BoxCollisionShape("StartAreaShape",StartSize));
    StartZone->SetFieldMesh(MeshMan->CreateBoxMesh("StartAreaMesh",ColourValue(0.1,0.8,0.1,0.2),StartSize));
    PhysMan->AddAreaEffect(StartZone);
    GameApp->RegisterStartArea(StartZone);// */

    Vector3 Score1Size(25,21,25);
    CollisionShape* ScoreAreaShape = new BoxCollisionShape("ScoreAreaShape",Score1Size);
    Mesh* ScoreAreaMesh = MeshMan->CreateBoxMesh("ScoreAreaMesh",ColourValue(0.2,0.2,0.8,0.2),Score1Size);
    ScoreArea* ScoreZone1 = new ScoreArea("ScoreArea1",Vector3(-10.5,-100.0,0.0));
    ScoreZone1->SetFieldShape(ScoreAreaShape);
    ScoreZone1->SetFieldMesh(ScoreAreaMesh);
    PhysMan->AddAreaEffect(ScoreZone1);
    GameApp->RegisterScoreArea(ScoreZone1);
    Vector3 Score2Size(55,48,15);
    ScoreArea* ScoreZone2 = new ScoreArea("ScoreArea2",Vector3(-170,100,0));
    ScoreZone2->SetFieldShape(ScoreAreaShape);
    ScoreZone2->SetFieldMesh(ScoreAreaMesh);
    PhysMan->AddAreaEffect(ScoreZone2);
    GameApp->RegisterScoreArea(ScoreZone2);// */
}

void LevelLoader::LoadJustice()
{
    // Get managers
    PhysicsManager* PhysMan = TheWorld->GetPhysicsManager();
    SceneManager* SceneMan = TheWorld->GetSceneManager();
    ResourceManager* ResourceMan = TheWorld->GetResourceManager();
    ActorManager* ActMan = TheWorld->GetActorManager();
    CollisionShapeManager* CShapeMan = TheWorld->GetCollisionShapeManager();
    MeshManager* MeshMan = TheWorld->GetMeshManager();

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

    // Setup and Create the shapes that will be used.
    CShapeMan->LoadAllShapesFromFile("Justice.bullet",JusticeGroup);

    // Create Lady Justice
    ActorRigid* LadyJustice = new ActorRigid(0,"LadyJustice","lady.mesh",JusticeGroup);
    LadyJustice->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GenerateStaticTriMesh("LadyShape","lady.mesh",JusticeGroup));
    LadyJustice->SetLocation(Vector3(12,-60,-103.35));
    ActMan->AddActor(LadyJustice);

    // Create the scale object
    Real ScaleMass = 50.0;
    ActorRigid* JusticeScale = new ActorRigid(ScaleMass,"JusticeScale","scale.mesh",JusticeGroup);
    JusticeScale->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Scale"));
    JusticeScale->SetLocation(Vector3(0,37.4,0));
    JusticeScale->GetPhysicsSettings()->SetDamping(0.0,0.6);
    ActMan->AddActor(JusticeScale);

    // Create the union objects
    Real UnionMass = 5.0;
    ActorRigid* Union1 = new ActorRigid(UnionMass,"Union1","union.mesh",JusticeGroup);
    Union1->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Union"));
    Union1->SetLocation(Vector3(-112,0.2,-16.4));
    ActMan->AddActor(Union1);
    ActorRigid* Union2 = new ActorRigid(UnionMass,"Union2","union.mesh",JusticeGroup);
    Union2->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Union"));
    Union2->SetLocation(Vector3(-112,0.2,16.4));
    ActMan->AddActor(Union2);
    ActorRigid* Union3 = new ActorRigid(UnionMass,"Union3","union.mesh",JusticeGroup);
    Union3->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Union"));
    Union3->SetLocation(Vector3(112,0.2,-16.4));
    ActMan->AddActor(Union3);
    ActorRigid* Union4 = new ActorRigid(UnionMass,"Union4","union.mesh",JusticeGroup);
    Union4->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Union"));
    Union4->SetLocation(Vector3(112,0.2,16.4));
    ActMan->AddActor(Union4);

    // Create the trays
    Real TrayMass = 25.0;
    ActorRigid* Tray1 = new ActorRigid(TrayMass,"Tray1","tray.mesh",JusticeGroup);
    Tray1->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Tray"));
    Tray1->SetLocation(Vector3(-112,-35,0));
    Tray1->SetLinearMovementFactor(Vector3(1,1,0));
    Tray1->SetAngularMovementFactor(Vector3(1,0,1));
    Tray1->GetPhysicsSettings()->SetDamping(0.0,0.3);
    ActMan->AddActor(Tray1);
    ActorRigid* Tray2 = new ActorRigid(TrayMass,"Tray2","tray.mesh",JusticeGroup);
    Tray2->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Tray"));
    Tray2->SetLocation(Vector3(112,-35,0));
    Tray2->SetLinearMovementFactor(Vector3(1,1,0));
    Tray2->SetAngularMovementFactor(Vector3(1,0,1));
    Tray2->GetPhysicsSettings()->SetDamping(0.0,0.3);
    ActMan->AddActor(Tray2);

    // Create the series of constraints for connecting all the pieces of the scale together
    // Starting with creating the anchor for the scale
    HingeConstraint* ScaleAnchor = new HingeConstraint(LadyJustice,JusticeScale,Vector3(-12,127.4,103.35),Vector3(0,30,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(ScaleAnchor,true);

    // Original X distance from pivot on scale is 112.7, but space was needed to prevent collsions.
    // Create the scale-to-union constraints
    Point2PointConstraint* U1S = new Point2PointConstraint(JusticeScale,Union1,Vector3(-112.0,-20,-16.4),Vector3(0,17.2,0));//58,47.4,0 // -54,27.4,-16.4
    PhysMan->AddConstraint(U1S,true);
    Point2PointConstraint* U2S = new Point2PointConstraint(JusticeScale,Union2,Vector3(-112.0,-20,16.4),Vector3(0,17.2,0));//58,47.4,0 // -54,27.4,16.4
    PhysMan->AddConstraint(U2S,true);
    Point2PointConstraint* U3S = new Point2PointConstraint(JusticeScale,Union3,Vector3(112.0,-20,-16.4),Vector3(0,17.2,0));//58,47.4,0 // 170,27.4,-16.4
    PhysMan->AddConstraint(U3S,true);
    Point2PointConstraint* U4S = new Point2PointConstraint(JusticeScale,Union4,Vector3(112.0,-20,16.4),Vector3(0,17.2,0));//58,47.4,0 // 170,27.4,16.4
    PhysMan->AddConstraint(U4S,true);

    // Create the union-to-tray constraints
    Point2PointConstraint* U1T = new Point2PointConstraint(Union1,Tray1,Vector3(0,-17.2,0),Vector3(0,18,-16.4));// -54,10.2,-16.4 // -54,-25,0
    PhysMan->AddConstraint(U1T,true);
    Point2PointConstraint* U2T = new Point2PointConstraint(Union2,Tray1,Vector3(0,-17.2,0),Vector3(0,18,16.4));// -54,10.2,16.4 // -54,-25,0
    PhysMan->AddConstraint(U2T,true);
    Point2PointConstraint* U3T = new Point2PointConstraint(Union3,Tray2,Vector3(0,-17.2,0),Vector3(0,18,-16.4));// 170,10.2,-16.4 // 170,-25,0
    PhysMan->AddConstraint(U3T,true);
    Point2PointConstraint* U4T = new Point2PointConstraint(Union4,Tray2,Vector3(0,-17.2,0),Vector3(0,18,16.4));// 170,10.2,16.4 // 170,-25,0
    PhysMan->AddConstraint(U4T,true);

    // Create some throwable objects
    ThrowableData* ClayData = ThrowableGenerator::GetThrowableData("Clay");
    ThrowableData* LeadData = ThrowableGenerator::GetThrowableData("Lead");
    ThrowableData* UraniumData = ThrowableGenerator::GetThrowableData("Uranium");
    //CollisionShape* ClayCS = CShapeMan->GenerateConvexHull("ClayCS",ClayData->MeshName,CommonGroup);
    //CollisionShape* LeadCS = CShapeMan->GenerateConvexHull("LeadCS",LeadData->MeshName,CommonGroup);
    CollisionShape* UraniumCS = CShapeMan->GenerateConvexHull("UraniumCS",UraniumData->MeshName,CommonGroup);
    ActorRigid* Uranium1 = new ActorRigid(UraniumData->Mass,"Uranium1",UraniumData->MeshName,CommonGroup);
    Uranium1->GetPhysicsSettings()->SetCollisionShape(UraniumCS);
    Uranium1->SetLocation(-155,110,0);
    Uranium1->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Uranium1->SetLinearMovementFactor(Vector3(1,1,0));
    ActMan->AddActor(Uranium1);
    GameApp->AddThrowable(Uranium1);
    ActorRigid* Uranium2 = new ActorRigid(UraniumData->Mass,"Uranium2",UraniumData->MeshName,CommonGroup);
    Uranium2->GetPhysicsSettings()->SetCollisionShape(UraniumCS);
    Uranium2->SetLocation(-185,110,0);
    Uranium2->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    Uranium2->SetLinearMovementFactor(Vector3(1,1,0));
    ActMan->AddActor(Uranium2);
    GameApp->AddThrowable(Uranium2);

    // Create the zones
    AreaOfPlay* PlayZone = new AreaOfPlay("PlayArea",Vector3(0,0,0));
    PlayZone->SetFieldShape(new BoxCollisionShape("PlayAreaShape",Vector3(280,280,35)));
    PhysMan->AddAreaEffect(PlayZone);
    GameApp->SetPlayArea(PlayZone);// */

    Vector3 StartSize(60,30,15);
    CollisionShape* StartAreaShape = new BoxCollisionShape("StartArea1Shape",StartSize);
    Mesh* StartAreaMesh = MeshMan->CreateBoxMesh("StartAreaMesh",ColourValue(0.1,0.8,0.1,0.2),StartSize);
    StartingArea* StartZone1 = new StartingArea("StartArea1",Vector3(-160,120,0));
    StartZone1->SetFieldShape(StartAreaShape);
    StartZone1->SetFieldMesh(StartAreaMesh);
    PhysMan->AddAreaEffect(StartZone1);
    GameApp->RegisterStartArea(StartZone1);
    StartingArea* StartZone2 = new StartingArea("StartArea2",Vector3(160,120,0));
    StartZone2->SetFieldShape(StartAreaShape);
    StartZone2->SetFieldMesh(StartAreaMesh);
    PhysMan->AddAreaEffect(StartZone2);
    GameApp->RegisterStartArea(StartZone2);// */

    Vector3 ScoreSize(52,25,18);
    CollisionShape* ScoreAreaShape = new BoxCollisionShape("ScoreAreaShape",ScoreSize);
    Mesh* ScoreAreaMesh = MeshMan->CreateBoxMesh("ScoreAreaMesh",ColourValue(0.2,0.2,0.8,0.2),ScoreSize);
    ScoreArea* ScoreZone1 = new ScoreArea("ScoreArea1",Vector3(-113,-40,0));
    ScoreZone1->SetFieldShape(ScoreAreaShape);
    ScoreZone1->SetFieldMesh(ScoreAreaMesh);
    PhysMan->AddAreaEffect(ScoreZone1);
    GameApp->RegisterScoreArea(ScoreZone1);// */
    ScoreArea* ScoreZone2 = new ScoreArea("ScoreArea2",Vector3(113,-40,0));
    ScoreZone2->SetFieldShape(ScoreAreaShape);
    ScoreZone2->SetFieldMesh(ScoreAreaMesh);
    PhysMan->AddAreaEffect(ScoreZone2);
    GameApp->RegisterScoreArea(ScoreZone2);// */
}

void LevelLoader::LoadRollers()
{
    // Get managers
    PhysicsManager* PhysMan = TheWorld->GetPhysicsManager();
    SceneManager* SceneMan = TheWorld->GetSceneManager();
    ResourceManager* ResourceMan = TheWorld->GetResourceManager();
    ActorManager* ActMan = TheWorld->GetActorManager();
    CollisionShapeManager* CShapeMan = TheWorld->GetCollisionShapeManager();
    MeshManager* MeshMan = TheWorld->GetMeshManager();

    String CommonGroup("Common");
    String RollersGroup("Rollers");
    String datadir = "Levels/";
    ResourceMan->AddResourceLocation(datadir+"Rollers.lvl", "Zip", RollersGroup, false);
    ResourceMan->InitResourceGroup(RollersGroup);

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

    // Setup and Create the shapes that will be used.

    // Create the basic terrain that will be used
    ActorRigid* RollersFrame = new ActorRigid(0,"RollersFrame","frame.mesh",RollersGroup);
    RollersFrame->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GenerateStaticTriMesh("FrameShape","frame.mesh",RollersGroup));
    RollersFrame->SetLocation(Vector3(0,-25,0));
    ActMan->AddActor(RollersFrame);

    // Create the individual Rollers
    ActorRigid* Roller1 = new ActorRigid(10,"Roller1","rubberroller.mesh",RollersGroup);

    ActorRigid* Roller2 = new ActorRigid(10,"Roller2","rubberroller.mesh",RollersGroup);

    ActorRigid* Roller3 = new ActorRigid(10,"Roller3","rubberroller.mesh",RollersGroup);

    // Create the constraints to place the Rollers

    // Create some throwable objects

    // Create the zones
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
