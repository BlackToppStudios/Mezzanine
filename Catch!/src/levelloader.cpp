#ifndef _levelloader_cpp
#define _levelloader_cpp

#include "levelloader.h"
#include "levelscorer.h"
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
    PhysicsManager* PhysMan = PhysicsManager::GetSingletonPtr();
    SceneManager* SceneMan = SceneManager::GetSingletonPtr();
    ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
    ActorManager* ActMan = ActorManager::GetSingletonPtr();
    CollisionShapeManager* CShapeMan = CollisionShapeManager::GetSingletonPtr();
    MeshManager* MeshMan = MeshManager::GetSingletonPtr();

    // Init Resources
    String CommonGroup("Common");
    String FerrisGroup("Ferris");
    String datadir = "Levels/";
    ResourceMan->AddResourceLocation(datadir+"Ferris.lvl", "Zip", FerrisGroup, false);
    ResourceMan->InitResourceGroup(FerrisGroup);

    // Scoring and Shop Setup
    LevelScorer* Scorer = CatchApp::GetCatchAppPointer()->GetLevelScorer();
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Scorer->SetLevelTargetTime(30);
    Shop->SetLevelCash(100);

    // Camera Setup
	Camera* DefCamera = CameraManager::GetSingletonPtr()->GetDefaultCamera();
	DefCamera->SetLocation(Vector3(0,0,425));
	DefCamera->LookAt(Vector3(0,0,0));

	// Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Light* DLight = SceneMan->CreateLight("SceneLight");
    DLight->SetLightType(Light::Directional);
    Vector3 Loc(150,250,-200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));

    // Physics Setup
    PhysMan->SetGravity(Vector3(0,-1000,0));

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,120),Vector3(15,-10,120),Vector3(0,10,120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Setup and Create the shapes that will be used.
    CShapeMan->LoadAllShapesFromFile("Ferris.bullet",FerrisGroup);

    //----------
    /*std::set<CollisionShape*>& Unnamed = CShapeMan->GetUnnamedShapes();
    for( std::set<CollisionShape*>::iterator CSit = Unnamed.begin() ; CSit != Unnamed.end() ; CSit++ )
    {
        CollisionShape* ToChange = (*CSit);
        if(CollisionShape::ST_Compound==ToChange->GetType())
        {
            if(10 > ((CompoundCollisionShape*)ToChange)->GetNumChildren())
            {
                CShapeMan->SetNameForUnnamedShape("Tray",ToChange);
            }else{
                CShapeMan->SetNameForUnnamedShape("Wheel",ToChange);
            }
        }
    }
    Unnamed.clear();
    CShapeMan->SaveAllStoredShapesToFile("Ferris2.bullet");// */
    //----------

    // Create the Wheel
    ActorRigid* FerrisWheel = new ActorRigid(100.0,"FerrisWheel","wheel.mesh",FerrisGroup);
    FerrisWheel->SetLocation(100,0,0);
    FerrisWheel->GetPhysicsSettings()->SetDamping(0,0.1);
    FerrisWheel->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Wheel")); // The names aren't being loaded as we expected
    ActMan->AddActor(FerrisWheel);

    // Create the trayz
    Real TrayMass = 50.0;
    ActorRigid* Tray1 = new ActorRigid(TrayMass,"Tray1","bluetray.mesh",FerrisGroup);
    Tray1->SetLocation(30.4,14.3,0);
    Tray1->GetPhysicsSettings()->SetDamping(0,0.1);
    Tray1->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Tray"));
    ActMan->AddActor(Tray1);
    ActorRigid* Tray2 = new ActorRigid(TrayMass,"Tray2","purpletray.mesh",FerrisGroup);
    Tray2->SetLocation(169.6,14.3,0);
    Tray2->GetPhysicsSettings()->SetDamping(0,0.1);
    Tray2->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Tray"));
    ActMan->AddActor(Tray2);
    ActorRigid* Tray3 = new ActorRigid(TrayMass,"Tray3","purpletray.mesh",FerrisGroup);
    Tray3->SetLocation(30.4,-42.7,0);
    Tray3->GetPhysicsSettings()->SetDamping(0,0.1);
    Tray3->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Tray"));
    ActMan->AddActor(Tray3);
    ActorRigid* Tray4 = new ActorRigid(TrayMass,"Tray4","bluetray.mesh",FerrisGroup);
    Tray4->SetLocation(169.6,-42.7,0);
    Tray4->GetPhysicsSettings()->SetDamping(0,0.1);
    Tray4->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Tray"));
    ActMan->AddActor(Tray4);
    ActorRigid* Tray5 = new ActorRigid(TrayMass,"Tray5","purpletray.mesh",FerrisGroup);
    Tray5->SetLocation(71.5,55.4,0);
    Tray5->GetPhysicsSettings()->SetDamping(0,0.1);
    Tray5->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Tray"));
    ActMan->AddActor(Tray5);
    ActorRigid* Tray6 = new ActorRigid(TrayMass,"Tray6","bluetray.mesh",FerrisGroup);
    Tray6->SetLocation(128.5,55.4,0);
    Tray6->GetPhysicsSettings()->SetDamping(0,0.1);
    Tray6->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Tray"));
    ActMan->AddActor(Tray6);
    ActorRigid* Tray7 = new ActorRigid(TrayMass,"Tray7","bluetray.mesh",FerrisGroup);
    Tray7->SetLocation(71.5,-83.8,0);
    Tray7->GetPhysicsSettings()->SetDamping(0,0.1);
    Tray7->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Tray"));
    ActMan->AddActor(Tray7);
    ActorRigid* Tray8 = new ActorRigid(TrayMass,"Tray8","purpletray.mesh",FerrisGroup);
    Tray8->SetLocation(128.5,-83.8,0);
    Tray8->GetPhysicsSettings()->SetDamping(0,0.1);
    Tray8->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Tray"));
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
    ThrowableData* PyriteData = ThrowableGenerator::GetThrowableData("Pyrite");
    CollisionShape* ClayCS = CShapeMan->GenerateConvexHull("ClayCS",ClayData->MeshName,CommonGroup);
    CollisionShape* LeadCS = CShapeMan->GenerateConvexHull("LeadCS",LeadData->MeshName,CommonGroup);
    CollisionShape* UraniumCS = CShapeMan->GenerateConvexHull("UraniumCS",UraniumData->MeshName,CommonGroup);
    CollisionShape* PyriteCS = CShapeMan->GenerateConvexHull("PyriteCS",PyriteData->MeshName,CommonGroup);
    ActorRigid* Uranium1 = ThrowableGenerator::CreateThrowable("Uranium");
    Uranium1->GetPhysicsSettings()->SetCollisionShape(UraniumCS);
    //Uranium1->GetPhysicsSettings()->SetStickyData(1);
    Uranium1->SetLocation(-145,60,0);
    ActMan->AddActor(Uranium1);
    GameApp->AddThrowable(Uranium1);
    ActorRigid* Uranium2 = ThrowableGenerator::CreateThrowable("Uranium");
    Uranium2->GetPhysicsSettings()->SetCollisionShape(UraniumCS);
    //Uranium2->GetPhysicsSettings()->SetStickyData(1);
    Uranium2->SetLocation(-195,60,0);
    ActMan->AddActor(Uranium2);
    GameApp->AddThrowable(Uranium2);// */
    ActorRigid* Lead1 = ThrowableGenerator::CreateThrowable("Lead");
    Lead1->GetPhysicsSettings()->SetCollisionShape(LeadCS);
    Lead1->SetLocation(-145,20,0);
    ActMan->AddActor(Lead1);
    GameApp->AddThrowable(Lead1);
    ActorRigid* Lead2 = ThrowableGenerator::CreateThrowable("Lead");
    Lead2->GetPhysicsSettings()->SetCollisionShape(LeadCS);
    Lead2->SetLocation(-195,20,0);
    ActMan->AddActor(Lead2);
    GameApp->AddThrowable(Lead2);// */
    ActorRigid* Clay1 = ThrowableGenerator::CreateThrowable("Clay");
    Clay1->GetPhysicsSettings()->SetCollisionShape(ClayCS);
    Clay1->SetLocation(-145,-20,0);
    ActMan->AddActor(Clay1);
    GameApp->AddThrowable(Clay1);
    ActorRigid* Clay2 = ThrowableGenerator::CreateThrowable("Clay");
    Clay2->GetPhysicsSettings()->SetCollisionShape(ClayCS);
    Clay2->SetLocation(-195,-20,0);
    ActMan->AddActor(Clay2);
    GameApp->AddThrowable(Clay2);// */
    ActorRigid* Pyrite1 = ThrowableGenerator::CreateThrowable("Pyrite");
    Pyrite1->GetPhysicsSettings()->SetCollisionShape(PyriteCS);
    Pyrite1->SetLocation(-145,-60,0);
    ActMan->AddActor(Pyrite1);
    GameApp->AddThrowable(Pyrite1);
    ActorRigid* Pyrite2 = ThrowableGenerator::CreateThrowable("Pyrite");
    Pyrite2->GetPhysicsSettings()->SetCollisionShape(PyriteCS);
    Pyrite2->SetLocation(-195,-60,0);
    ActMan->AddActor(Pyrite2);
    GameApp->AddThrowable(Pyrite2);// */

    // Create the zones
    /*AreaOfPlay* PlayZone = new AreaOfPlay("PlayArea",Vector3(0,0,0));
    PlayZone->SetFieldShape(new BoxCollisionShape("PlayAreaShape",Vector3(280,280,35)));
    PhysMan->AddAreaEffect(PlayZone);
    GameApp->SetPlayArea(PlayZone);// */

    Vector3 StartSize(50,130,25);
    StartArea* StartZone = new StartArea("StartArea",Vector3(-170,0,0));
    StartZone->GetPhysicsSettings()->SetCollisionShape(new BoxCollisionShape("StartAreaShape",StartSize));
    StartZone->GetGraphicsSettings()->SetMesh(MeshMan->CreateBoxCornerMesh("StartAreaMesh",ColourValue(0.1,0.8,0.1,0.8),StartSize,4.0));
    PhysMan->AddAreaEffect(StartZone);
    GameApp->RegisterStartArea(StartZone);// */
    ParticleEffect* StartParticle = SceneMan->CreateParticleEffect("StartParticle","StartVortex");
    StartParticle->GetEmitter(0)->SetCustomParam("width",StringTool::ConvertToString(StartSize.X * (Real)1.8));
    StartParticle->GetEmitter(0)->SetCustomParam("height",StringTool::ConvertToString(StartSize.Y * (Real)0.6));
    StartParticle->GetEmitter(0)->SetCustomParam("depth",StringTool::ConvertToString(StartSize.Z * (Real)1.8));
    StartParticle->GetEmitter(0)->SetCustomParam("time_to_live_min",StringTool::ConvertToString((Real)11.0));
    StartParticle->GetEmitter(0)->SetCustomParam("time_to_live_max",StringTool::ConvertToString((Real)15.0));
    StartParticle->SetLocation(Vector3(-170,-80,0));
    StartParticle->AddToWorld();// */

    Vector3 ScoreSize(100,100,30);
    ScoreArea* ScoreZone = new ScoreArea("ScoreArea",Vector3(100,0,0));
    ScoreZone->GetPhysicsSettings()->SetCollisionShape(new BoxCollisionShape("ScoreAreaShape",ScoreSize));
    ScoreZone->GetGraphicsSettings()->SetMesh(MeshMan->CreateBoxCornerMesh("ScoreAreaMesh",ColourValue(0.2,0.2,0.8,0.8),ScoreSize,4.0));
    PhysMan->AddAreaEffect(ScoreZone);
    GameApp->RegisterScoreArea(ScoreZone);// */

    Vector3 BonusScoreSize(18,18,30);
    CollisionShape* BonusScoreShape = new BoxCollisionShape("BonusAreaShape",BonusScoreSize);
    Mesh* BonusScoreMesh = MeshMan->CreateBoxCornerMesh("BonusAreaMesh",ColourValue(0.50,0.15,0.65,0.8),BonusScoreSize,4.0);
    ScoreArea* BonusZone1 = new ScoreArea("BonusArea1",Vector3(169.6,14.3,0));
    BonusZone1->GetPhysicsSettings()->SetCollisionShape(BonusScoreShape);
    BonusZone1->GetGraphicsSettings()->SetMesh(BonusScoreMesh);
    BonusZone1->SetScoreMultiplier(2.0);
    PhysMan->AddAreaEffect(BonusZone1);
    GameApp->RegisterScoreArea(BonusZone1);
    Tray2->AttachObject(BonusZone1);// */
    ScoreArea* BonusZone2 = new ScoreArea("BonusArea2",Vector3(30.4,-42.7,0));
    BonusZone2->GetPhysicsSettings()->SetCollisionShape(BonusScoreShape);
    BonusZone2->GetGraphicsSettings()->SetMesh(BonusScoreMesh);
    BonusZone2->SetScoreMultiplier(2.0);
    PhysMan->AddAreaEffect(BonusZone2);
    GameApp->RegisterScoreArea(BonusZone2);
    Tray3->AttachObject(BonusZone2);// */
    ScoreArea* BonusZone3 = new ScoreArea("BonusArea3",Vector3(71.5,55.4,0));
    BonusZone3->GetPhysicsSettings()->SetCollisionShape(BonusScoreShape);
    BonusZone3->GetGraphicsSettings()->SetMesh(BonusScoreMesh);
    BonusZone3->SetScoreMultiplier(2.0);
    PhysMan->AddAreaEffect(BonusZone3);
    GameApp->RegisterScoreArea(BonusZone3);
    Tray5->AttachObject(BonusZone3);// */
    ScoreArea* BonusZone4 = new ScoreArea("BonusArea4",Vector3(128.5,-83.8,0));
    BonusZone4->GetPhysicsSettings()->SetCollisionShape(BonusScoreShape);
    BonusZone4->GetGraphicsSettings()->SetMesh(BonusScoreMesh);
    BonusZone4->SetScoreMultiplier(2.0);
    PhysMan->AddAreaEffect(BonusZone4);
    GameApp->RegisterScoreArea(BonusZone4);
    Tray8->AttachObject(BonusZone4);// */
}

void LevelLoader::LoadBigCurve()
{
    // Get managers
    PhysicsManager* PhysMan = PhysicsManager::GetSingletonPtr();
    SceneManager* SceneMan = SceneManager::GetSingletonPtr();
    ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
    ActorManager* ActMan = ActorManager::GetSingletonPtr();
    CollisionShapeManager* CShapeMan = CollisionShapeManager::GetSingletonPtr();
    MeshManager* MeshMan = MeshManager::GetSingletonPtr();

    // Init Resources
    String CommonGroup("Common");
    String BigCurveGroup("BigCurve");
    String datadir = "Levels/";
    ResourceMan->AddResourceLocation(datadir+"BigCurve.lvl", "Zip", BigCurveGroup, false);
    ResourceMan->InitResourceGroup(BigCurveGroup);

    // Scoring and Shop Setup
    LevelScorer* Scorer = CatchApp::GetCatchAppPointer()->GetLevelScorer();
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Scorer->SetLevelTargetTime(30);
    Shop->SetLevelCash(100);

    // Camera Setup
	Camera* DefCamera = CameraManager::GetSingletonPtr()->GetDefaultCamera();
	DefCamera->SetLocation(Vector3(0,0,425));
	DefCamera->LookAt(Vector3(0,0,0));

	// Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Light* DLight = SceneMan->CreateLight("SceneLight");
    DLight->SetLightType(Light::Directional);
    Vector3 Loc(150,200,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));

    // Physics Setup
    PhysMan->SetGravity(Vector3(0,-1000,0));

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
    CurveCave->SetScaling(Vector3(1.05,1.05,1.05));
    ActMan->AddActor(CurveCave);

    // Create some throwable objects
    ThrowableData* PyriteData = ThrowableGenerator::GetThrowableData("Pyrite");
    ThrowableData* RubberData = ThrowableGenerator::GetThrowableData("Rubber");
    CollisionShape* PyriteCS = CShapeMan->GenerateConvexHull("PyriteCS",PyriteData->MeshName,CommonGroup);
    CollisionShape* RubberCS = CShapeMan->GenerateConvexHull("RubberCS",RubberData->MeshName,CommonGroup);
    ActorRigid* Pyrite1 = ThrowableGenerator::CreateThrowable("Pyrite");
    Pyrite1->GetPhysicsSettings()->SetCollisionShape(PyriteCS);
    Pyrite1->SetLocation(-155,-20,0);
    ActMan->AddActor(Pyrite1);
    GameApp->AddThrowable(Pyrite1);
    ActorRigid* Pyrite2 = ThrowableGenerator::CreateThrowable("Pyrite");
    Pyrite2->GetPhysicsSettings()->SetCollisionShape(PyriteCS);
    Pyrite2->SetLocation(-205,-20,0);
    ActMan->AddActor(Pyrite2);
    GameApp->AddThrowable(Pyrite2);// */
    ActorRigid* Rubber1 = ThrowableGenerator::CreateThrowable("Rubber");
    Rubber1->GetPhysicsSettings()->SetCollisionShape(RubberCS);
    Rubber1->SetLocation(-155,-60,0);
    ActMan->AddActor(Rubber1);
    GameApp->AddThrowable(Rubber1);
    ActorRigid* Rubber2 = ThrowableGenerator::CreateThrowable("Rubber");
    Rubber2->GetPhysicsSettings()->SetCollisionShape(RubberCS);
    Rubber2->SetLocation(-205,-60,0);
    ActMan->AddActor(Rubber2);
    GameApp->AddThrowable(Rubber2);
    ActorRigid* Rubber3 = ThrowableGenerator::CreateThrowable("Rubber");
    Rubber3->GetPhysicsSettings()->SetCollisionShape(RubberCS);
    Rubber3->SetLocation(-155,-100,0);
    ActMan->AddActor(Rubber3);
    GameApp->AddThrowable(Rubber3);
    ActorRigid* Rubber4 = ThrowableGenerator::CreateThrowable("Rubber");
    Rubber4->GetPhysicsSettings()->SetCollisionShape(RubberCS);
    Rubber4->SetLocation(-205,-100,0);
    ActMan->AddActor(Rubber4);
    GameApp->AddThrowable(Rubber4);// */

    // Create the zones
    Vector3 StartSize(50,80,20);
    StartArea* StartZone = new StartArea("StartArea",Vector3(-180,-60,0));
    StartZone->GetPhysicsSettings()->SetCollisionShape(new BoxCollisionShape("StartAreaShape",StartSize));
    StartZone->GetGraphicsSettings()->SetMesh(MeshMan->CreateBoxCornerMesh("StartAreaMesh",ColourValue(0.1,0.8,0.1,0.8),StartSize,4.0));
    PhysMan->AddAreaEffect(StartZone);
    GameApp->RegisterStartArea(StartZone);// */
    ParticleEffect* StartParticle = SceneMan->CreateParticleEffect("StartParticle","StartVortex");
    StartParticle->GetEmitter(0)->SetCustomParam("width",StringTool::ConvertToString(StartSize.X * (Real)1.8));
    StartParticle->GetEmitter(0)->SetCustomParam("height",StringTool::ConvertToString(StartSize.Y * (Real)0.6));
    StartParticle->GetEmitter(0)->SetCustomParam("depth",StringTool::ConvertToString(StartSize.Z * (Real)1.8));
    StartParticle->GetEmitter(0)->SetCustomParam("time_to_live_min",StringTool::ConvertToString((Real)8.0));
    StartParticle->GetEmitter(0)->SetCustomParam("time_to_live_max",StringTool::ConvertToString((Real)10.0));
    StartParticle->SetLocation(Vector3(-180,-110,0));
    StartParticle->AddToWorld();// */

    Vector3 ScoreSize(50,40,50);
    ScoreArea* ScoreZone = new ScoreArea("ScoreArea",Vector3(120,-20,-10));
    ScoreZone->GetPhysicsSettings()->SetCollisionShape(new BoxCollisionShape("ScoreAreaShape",ScoreSize));
    ScoreZone->GetGraphicsSettings()->SetMesh(MeshMan->CreateBoxCornerMesh("ScoreAreaMesh",ColourValue(0.2,0.2,0.8,0.8),ScoreSize,4.0));
    PhysMan->AddAreaEffect(ScoreZone);
    GameApp->RegisterScoreArea(ScoreZone);// */
}

void LevelLoader::LoadBlowsNotSucks()
{
    // Get managers
    PhysicsManager* PhysMan = PhysicsManager::GetSingletonPtr();
    SceneManager* SceneMan = SceneManager::GetSingletonPtr();
    ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
    ActorManager* ActMan = ActorManager::GetSingletonPtr();
    CollisionShapeManager* CShapeMan = CollisionShapeManager::GetSingletonPtr();
    MeshManager* MeshMan = MeshManager::GetSingletonPtr();

    // Init Resources
    String CommonGroup("Common");
    String BlowsNotSucksGroup("BlowsNotSucks");
    String datadir = "Levels/";
    ResourceMan->AddResourceLocation(datadir+"BlowsNotSucks.lvl", "Zip", BlowsNotSucksGroup, false);
    ResourceMan->InitResourceGroup(BlowsNotSucksGroup);

    // Scoring and Shop Setup
    LevelScorer* Scorer = CatchApp::GetCatchAppPointer()->GetLevelScorer();
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Scorer->SetLevelTargetTime(30);
    Shop->SetLevelCash(100);

    // Camera Setup
	Camera* DefCamera = CameraManager::GetSingletonPtr()->GetDefaultCamera();
	DefCamera->SetLocation(Vector3(0,0,425));
	DefCamera->LookAt(Vector3(0,0,0));

	// Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Light* DLight = SceneMan->CreateLight("SceneLight");
    DLight->SetLightType(Light::Directional);
    Vector3 Loc(-150,200,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));

    // Physics Setup
    PhysMan->SetGravity(Vector3(0,-1000,0));

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,120),Vector3(15,-10,120),Vector3(0,10,120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Setup and Create the shapes that will be used.
    CShapeMan->LoadAllShapesFromFile("BlowsNotSucks.bullet",BlowsNotSucksGroup);

    //----------
    /*std::set<CollisionShape*>& Unnamed = CShapeMan->GetUnnamedShapes();
    for( std::set<CollisionShape*>::iterator CSit = Unnamed.begin() ; CSit != Unnamed.end() ; CSit++ )
    {
        CollisionShape* ToChange = (*CSit);
        if(CollisionShape::ST_Compound==ToChange->GetType())
        {
            CShapeMan->SetNameForUnnamedShape("Fan",ToChange);
        }
    }
    Unnamed.clear();
    CShapeMan->SaveAllStoredShapesToFile("BlowsNotSucks.bullet");// */
    //----------

    // Create the fan
    ActorRigid* Fan = new ActorRigid(25,"Fan","fan.mesh",BlowsNotSucksGroup);
    Fan->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Fan"));
    Fan->SetLocation(Vector3(103.388,-58.888,7.5));
    Fan->SetOrientation(Quaternion(MathTool::GetHalfPi() * 0.5,Vector3(0,0,1)));
    Fan->GetPhysicsSettings()->SetActivationState(Mezzanine::WOAS_DisableDeactivation);
    ActMan->AddActor(Fan);

    ActorRigid* FanBody = new ActorRigid(0,"FanBody","body.mesh",BlowsNotSucksGroup);
    FanBody->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GenerateStaticTriMesh("Body","body.mesh",BlowsNotSucksGroup));
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
    ButtonToBody->SetLinearLimitUpper(Vector3(0,15,0));
    ButtonToBody->SetLinearLimitLower(Vector3(0,0,0));
    ButtonToBody->SetAngularLimitUpper(Vector3(0,0,0));
    ButtonToBody->SetAngularLimitLower(Vector3(0,0,0));
    ButtonToBody->SetSpringEnabled(1,true);
    ButtonToBody->SetSpringStiffness(1,500.f);
    ButtonToBody->CalculateSpringEquilibriumPoint(1);
    PhysMan->AddConstraint(ButtonToBody,true);

    // Create the field of force for the wind generated by the fan
    Vector3 FanWindSize(60,115,60);
    Quaternion FanWindOrientation(MathTool::GetHalfPi() * 0.5,Vector3(0,0,1));
    FieldOfForce* FanWind = new FieldOfForce("FanWind",Vector3(5,40,7.5));
    FanWind->GetPhysicsSettings()->SetCollisionShape(new CylinderCollisionShape("FanWindShape",60,115,Vector3::Unit_Y()));
    //FanWind->GetGraphicsSettings()->SetMesh(MeshMan->CreateCylinderMesh("FanWindMesh",ColourValue(0.8,0.1,0.1,0.2),FanWindSize,Vector3::Unit_Y()));
    FanWind->SetOrientation(FanWindOrientation);
    FanWind->SetDirectionOfForce(Vector3(-1,1,0));
    FanWind->SetAttenuation(0.2f,Mezzanine::Att_Linear,Vector3(-125.5,-81,7.5));
    FanWind->SetFieldStrength(1000.f);
    PhysMan->AddAreaEffect(FanWind);
    ParticleEffect* WindParticle = SceneMan->CreateParticleEffect("WindParticle","WindTunnel");
    WindParticle->GetEmitter(0)->SetCustomParam("width",StringTool::ConvertToString(FanWindSize.X * (Real)1.8));
    WindParticle->GetEmitter(0)->SetCustomParam("height",StringTool::ConvertToString(FanWindSize.Y * (Real)0.6));
    WindParticle->GetEmitter(0)->SetCustomParam("depth",StringTool::ConvertToString(FanWindSize.Z * (Real)1.8));
    WindParticle->GetEmitter(0)->SetCustomParam("position","0 -60.0 0");
    WindParticle->SetLocation(Vector3(5,40,7.5));
    WindParticle->SetOrientation(FanWindOrientation);
    //WindParticle->AddToWorld();// */

    // Create the trigger for the fan
    BNS_Fan* FanTrigger = new BNS_Fan("FanTigger",FanToBody,FanButton,Fan,FanWind,WindParticle);
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
    ActorRigid* Uranium1 = ThrowableGenerator::CreateThrowable("Uranium");
    Uranium1->GetPhysicsSettings()->SetCollisionShape(UraniumCS);
    Uranium1->SetLocation(-155,-20,0);
    ActMan->AddActor(Uranium1);
    GameApp->AddThrowable(Uranium1);
    ActorRigid* Uranium2 = ThrowableGenerator::CreateThrowable("Uranium");
    Uranium2->GetPhysicsSettings()->SetCollisionShape(UraniumCS);
    Uranium2->SetLocation(-185,-20,0);
    ActMan->AddActor(Uranium2);
    GameApp->AddThrowable(Uranium2);// */
    ActorRigid* Styrofoam1 = ThrowableGenerator::CreateThrowable("Styrofoam");
    Styrofoam1->GetPhysicsSettings()->SetCollisionShape(StyrofoamCS);
    Styrofoam1->SetLocation(-155,-60,0);
    ActMan->AddActor(Styrofoam1);
    GameApp->AddThrowable(Styrofoam1);
    ActorRigid* Styrofoam2 = ThrowableGenerator::CreateThrowable("Styrofoam");
    Styrofoam2->GetPhysicsSettings()->SetCollisionShape(StyrofoamCS);
    Styrofoam2->SetLocation(-185,-60,0);
    ActMan->AddActor(Styrofoam2);
    GameApp->AddThrowable(Styrofoam2);
    ActorRigid* Styrofoam3 = ThrowableGenerator::CreateThrowable("Styrofoam");
    Styrofoam3->GetPhysicsSettings()->SetCollisionShape(StyrofoamCS);
    Styrofoam3->SetLocation(-155,-100,0);
    ActMan->AddActor(Styrofoam3);
    GameApp->AddThrowable(Styrofoam3);
    ActorRigid* Styrofoam4 = ThrowableGenerator::CreateThrowable("Styrofoam");
    Styrofoam4->GetPhysicsSettings()->SetCollisionShape(StyrofoamCS);
    Styrofoam4->SetLocation(-185,-100,0);
    ActMan->AddActor(Styrofoam4);
    GameApp->AddThrowable(Styrofoam4);// */

    // Create the zones
    Vector3 StartSize(50,70,20);
    StartArea* StartZone = new StartArea("StartArea",Vector3(-170,-70,0));
    StartZone->GetPhysicsSettings()->SetCollisionShape(new BoxCollisionShape("StartAreaShape",StartSize));
    StartZone->GetGraphicsSettings()->SetMesh(MeshMan->CreateBoxCornerMesh("StartAreaMesh",ColourValue(0.1,0.8,0.1,0.8),StartSize,4.0));
    PhysMan->AddAreaEffect(StartZone);
    GameApp->RegisterStartArea(StartZone);// */
    ParticleEffect* StartParticle = SceneMan->CreateParticleEffect("StartParticle","StartVortex");
    StartParticle->GetEmitter(0)->SetCustomParam("width",StringTool::ConvertToString(StartSize.X * (Real)1.8));
    StartParticle->GetEmitter(0)->SetCustomParam("height",StringTool::ConvertToString(StartSize.Y * (Real)0.6));
    StartParticle->GetEmitter(0)->SetCustomParam("depth",StringTool::ConvertToString(StartSize.Z * (Real)1.8));
    StartParticle->GetEmitter(0)->SetCustomParam("time_to_live_min",StringTool::ConvertToString((Real)6.0));
    StartParticle->GetEmitter(0)->SetCustomParam("time_to_live_max",StringTool::ConvertToString((Real)9.0));
    StartParticle->SetLocation(Vector3(-170,-115,0));
    StartParticle->AddToWorld();// */

    Vector3 Score1Size(25,21,25);
    CollisionShape* ScoreArea1Shape = new BoxCollisionShape("ScoreArea1Shape",Score1Size);
    Mesh* ScoreArea1Mesh = MeshMan->CreateBoxCornerMesh("ScoreArea1Mesh",ColourValue(0.2,0.2,0.8,0.8),Score1Size,4.0);
    ScoreArea* ScoreZone1 = new ScoreArea("ScoreArea1",Vector3(-10.5,-100.0,0.0));
    ScoreZone1->GetPhysicsSettings()->SetCollisionShape(ScoreArea1Shape);
    ScoreZone1->GetGraphicsSettings()->SetMesh(ScoreArea1Mesh);
    PhysMan->AddAreaEffect(ScoreZone1);
    GameApp->RegisterScoreArea(ScoreZone1);
    Vector3 Score2Size(55,48,15);
    CollisionShape* ScoreArea2Shape = new BoxCollisionShape("ScoreArea2Shape",Score2Size);
    Mesh* ScoreArea2Mesh = MeshMan->CreateBoxCornerMesh("ScoreArea2Mesh",ColourValue(0.2,0.2,0.8,0.8),Score2Size,4.0);
    ScoreArea* ScoreZone2 = new ScoreArea("ScoreArea2",Vector3(-170,100,0));
    ScoreZone2->GetPhysicsSettings()->SetCollisionShape(ScoreArea2Shape);
    ScoreZone2->GetGraphicsSettings()->SetMesh(ScoreArea2Mesh);
    PhysMan->AddAreaEffect(ScoreZone2);
    GameApp->RegisterScoreArea(ScoreZone2);// */
}

void LevelLoader::LoadJustice()
{
    // Get managers
    PhysicsManager* PhysMan = PhysicsManager::GetSingletonPtr();
    SceneManager* SceneMan = SceneManager::GetSingletonPtr();
    ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
    ActorManager* ActMan = ActorManager::GetSingletonPtr();
    CollisionShapeManager* CShapeMan = CollisionShapeManager::GetSingletonPtr();
    MeshManager* MeshMan = MeshManager::GetSingletonPtr();

    // Init Resources
    String CommonGroup("Common");
    String JusticeGroup("Justice");
    String datadir = "Levels/";
    ResourceMan->AddResourceLocation(datadir+"Justice.lvl", "Zip", JusticeGroup, false);
    ResourceMan->InitResourceGroup(JusticeGroup);

    // Scoring and Shop Setup
    LevelScorer* Scorer = CatchApp::GetCatchAppPointer()->GetLevelScorer();
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Scorer->SetLevelTargetTime(30);
    Shop->SetLevelCash(100);

    // Camera Setup
	Camera* DefCamera = CameraManager::GetSingletonPtr()->GetDefaultCamera();
	DefCamera->SetLocation(Vector3(0,0,425));
	DefCamera->LookAt(Vector3(0,0,0));

	// Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Light* DLight = SceneMan->CreateLight("SceneLight");
    DLight->SetLightType(Light::Directional);
    Vector3 Loc(-150,100,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));

    // Physics Setup
    PhysMan->SetGravity(Vector3(0,-1000,0));

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,120),Vector3(15,-10,120),Vector3(0,10,120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Setup and Create the shapes that will be used.
    CShapeMan->LoadAllShapesFromFile("Justice.bullet",JusticeGroup);

    //----------
    /*std::set<CollisionShape*>& Unnamed = CShapeMan->GetUnnamedShapes();
    for( std::set<CollisionShape*>::iterator CSit = Unnamed.begin() ; CSit != Unnamed.end() ; CSit++ )
    {
        CollisionShape* ToChange = (*CSit);
        if(CollisionShape::ST_Compound==ToChange->GetType())
        {
            if(1 == ((CompoundCollisionShape*)ToChange)->GetNumChildren())
                CShapeMan->SetNameForUnnamedShape("Union",ToChange);
            else if(8 == ((CompoundCollisionShape*)ToChange)->GetNumChildren())
                CShapeMan->SetNameForUnnamedShape("Scale",ToChange);
            else if(9 == ((CompoundCollisionShape*)ToChange)->GetNumChildren())
                CShapeMan->SetNameForUnnamedShape("Tray",ToChange);
        }
    }
    Unnamed.clear();
    CShapeMan->SaveAllStoredShapesToFile("Justice.bullet");// */
    //----------

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
    JusticeScale->GetPhysicsSettings()->SetDamping(0.5,0.80);
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
    ScaleAnchor->SetLimit( -(MathTool::GetPi() * 0.20),(MathTool::GetPi() * 0.20) );
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
    ActorRigid* Uranium1 = ThrowableGenerator::CreateThrowable("Uranium");
    Uranium1->GetPhysicsSettings()->SetCollisionShape(UraniumCS);
    Uranium1->SetLocation(-155,110,0);
    ActMan->AddActor(Uranium1);
    GameApp->AddThrowable(Uranium1);
    ActorRigid* Uranium2 = ThrowableGenerator::CreateThrowable("Uranium");
    Uranium2->GetPhysicsSettings()->SetCollisionShape(UraniumCS);
    Uranium2->SetLocation(-185,110,0);
    ActMan->AddActor(Uranium2);
    GameApp->AddThrowable(Uranium2);

    // Create the zones
    Vector3 StartSize(60,30,20);
    CollisionShape* StartAreaShape = new BoxCollisionShape("StartArea1Shape",StartSize);
    Mesh* StartAreaMesh = MeshMan->CreateBoxCornerMesh("StartAreaMesh",ColourValue(0.1,0.8,0.1,0.8),StartSize,4.0);
    StartArea* StartZone1 = new StartArea("StartArea1",Vector3(-160,120,0));
    StartZone1->GetPhysicsSettings()->SetCollisionShape(StartAreaShape);
    StartZone1->GetGraphicsSettings()->SetMesh(StartAreaMesh);
    PhysMan->AddAreaEffect(StartZone1);
    GameApp->RegisterStartArea(StartZone1);
    StartArea* StartZone2 = new StartArea("StartArea2",Vector3(160,120,0));
    StartZone2->GetPhysicsSettings()->SetCollisionShape(StartAreaShape);
    StartZone2->GetGraphicsSettings()->SetMesh(StartAreaMesh);
    PhysMan->AddAreaEffect(StartZone2);
    GameApp->RegisterStartArea(StartZone2);// */
    ParticleEffect* StartParticle1 = SceneMan->CreateParticleEffect("StartParticle1","StartVortex");
    StartParticle1->GetEmitter(0)->SetCustomParam("width",StringTool::ConvertToString(StartSize.X * (Real)1.8));
    StartParticle1->GetEmitter(0)->SetCustomParam("height",StringTool::ConvertToString(StartSize.Y * (Real)0.6));
    StartParticle1->GetEmitter(0)->SetCustomParam("depth",StringTool::ConvertToString(StartSize.Z * (Real)1.8));
    StartParticle1->GetEmitter(0)->SetCustomParam("time_to_live_min",StringTool::ConvertToString((Real)3.0));
    StartParticle1->GetEmitter(0)->SetCustomParam("time_to_live_max",StringTool::ConvertToString((Real)4.0));
    StartParticle1->SetLocation(Vector3(-160,100,0));
    StartParticle1->AddToWorld();// */
    ParticleEffect* StartParticle2 = SceneMan->CreateParticleEffect("StartParticle2","StartVortex");
    StartParticle2->GetEmitter(0)->SetCustomParam("width",StringTool::ConvertToString(StartSize.X * (Real)1.8));
    StartParticle2->GetEmitter(0)->SetCustomParam("height",StringTool::ConvertToString(StartSize.Y * (Real)0.6));
    StartParticle2->GetEmitter(0)->SetCustomParam("depth",StringTool::ConvertToString(StartSize.Z * (Real)1.8));
    StartParticle2->GetEmitter(0)->SetCustomParam("time_to_live_min",StringTool::ConvertToString((Real)3.0));
    StartParticle2->GetEmitter(0)->SetCustomParam("time_to_live_max",StringTool::ConvertToString((Real)4.0));
    StartParticle2->SetLocation(Vector3(160,100,0));
    StartParticle2->AddToWorld();// */

    Vector3 ScoreSize(52,25,18);
    CollisionShape* ScoreAreaShape = new BoxCollisionShape("ScoreAreaShape",ScoreSize);
    Mesh* ScoreAreaMesh = MeshMan->CreateBoxCornerMesh("ScoreAreaMesh",ColourValue(0.2,0.2,0.8,0.8),ScoreSize,4.0);
    ScoreArea* ScoreZone1 = new ScoreArea("ScoreArea1",Vector3(-113,-40,0));
    ScoreZone1->GetPhysicsSettings()->SetCollisionShape(ScoreAreaShape);
    ScoreZone1->GetGraphicsSettings()->SetMesh(ScoreAreaMesh);
    PhysMan->AddAreaEffect(ScoreZone1);
    GameApp->RegisterScoreArea(ScoreZone1);// */
    ScoreArea* ScoreZone2 = new ScoreArea("ScoreArea2",Vector3(113,-40,0));
    ScoreZone2->GetPhysicsSettings()->SetCollisionShape(ScoreAreaShape);
    ScoreZone2->GetGraphicsSettings()->SetMesh(ScoreAreaMesh);
    PhysMan->AddAreaEffect(ScoreZone2);
    GameApp->RegisterScoreArea(ScoreZone2);// */
}

void LevelLoader::LoadRollers()
{
    // Get managers
    PhysicsManager* PhysMan = PhysicsManager::GetSingletonPtr();
    SceneManager* SceneMan = SceneManager::GetSingletonPtr();
    ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
    ActorManager* ActMan = ActorManager::GetSingletonPtr();
    CollisionShapeManager* CShapeMan = CollisionShapeManager::GetSingletonPtr();
    MeshManager* MeshMan = MeshManager::GetSingletonPtr();

    // Init Resources
    String CommonGroup("Common");
    String RollersGroup("Rollers");
    String datadir = "Levels/";
    ResourceMan->AddResourceLocation(datadir+"Rollers.lvl", "Zip", RollersGroup, false);
    ResourceMan->InitResourceGroup(RollersGroup);

    // Scoring and Shop Setup
    LevelScorer* Scorer = CatchApp::GetCatchAppPointer()->GetLevelScorer();
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Scorer->SetLevelTargetTime(30);
    Shop->SetLevelCash(100);

    // Camera Setup
	Camera* DefCamera = CameraManager::GetSingletonPtr()->GetDefaultCamera();
	DefCamera->SetLocation(Vector3(0,0,425));
	DefCamera->LookAt(Vector3(0,0,0));

	// Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Light* DLight = SceneMan->CreateLight("SceneLight");
    DLight->SetLightType(Light::Directional);
    Vector3 Loc(-150,100,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));

    // Physics Setup
    PhysMan->SetGravity(Vector3(0,-1000,0));

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,120),Vector3(15,-10,120),Vector3(0,10,120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Setup and Create the shapes that will be used.

    // Create the basic terrain that will be used
    ActorRigid* RollersFrame = new ActorRigid(0,"RollersFrame","frame.mesh",RollersGroup);
    RollersFrame->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GenerateStaticTriMesh("FrameShape","frame.mesh",RollersGroup));
    RollersFrame->GetPhysicsSettings()->SetFriction(1.0);
    RollersFrame->SetLocation(Vector3(0,-25,0));
    ActMan->AddActor(RollersFrame);

    // Create the individual Rollers and their constraints
    std::vector<HingeConstraint*> TheRollers;
    Real XStride = 14.5;
    CollisionShape* RollerShape = new CylinderCollisionShape("RollerShape",6.51,3.36,Vector3::Unit_Z());
    for( Whole X = 0 ; X < 7 ; ++X )
    {
        Vector3 Location(112.4+(XStride*X),-20,0);
        std::stringstream Namestream;
        Namestream << "Roller" << (X+1);
        ActorRigid* Roller = new ActorRigid(10,Namestream.str(),"rubberroller.mesh",RollersGroup);
        Roller->GetPhysicsSettings()->SetCollisionShape(RollerShape);
        Roller->GetPhysicsSettings()->SetFriction(1.0);
        Roller->SetLocation(Location);
        ActMan->AddActor(Roller);

        HingeConstraint* RollerAnchor = new HingeConstraint(Roller,Vector3(0,0,0),Vector3(0,0,1),true);
        RollerAnchor->SetLimit(1.0,-1.0);
        RollerAnchor->SetMaxMotorImpulse(1500.0);
        RollerAnchor->EnableMotor(true);
        PhysMan->AddConstraint(RollerAnchor,false);

        TheRollers.push_back(RollerAnchor);
    }

    Roll_Roll* RollerTrigger = new Roll_Roll("RollerTrigger",TheRollers);
    PhysMan->AddWorldTrigger(RollerTrigger);

    // Create some throwable objects
    ThrowableData* PyriteData = ThrowableGenerator::GetThrowableData("Pyrite");
    ThrowableData* LeadData = ThrowableGenerator::GetThrowableData("Lead");
    CollisionShape* PyriteCS = CShapeMan->GenerateConvexHull("PyriteCS",PyriteData->MeshName,CommonGroup);
    CollisionShape* LeadCS = CShapeMan->GenerateConvexHull("LesdCS",LeadData->MeshName,CommonGroup);
    ActorRigid* Pyrite1 = ThrowableGenerator::CreateThrowable("Pyrite");
    Pyrite1->GetPhysicsSettings()->SetCollisionShape(PyriteCS);
    Pyrite1->SetLocation(-160,80,0);
    ActMan->AddActor(Pyrite1);
    GameApp->AddThrowable(Pyrite1);
    ActorRigid* Pyrite2 = ThrowableGenerator::CreateThrowable("Pyrite");
    Pyrite2->GetPhysicsSettings()->SetCollisionShape(PyriteCS);
    Pyrite2->SetLocation(-120,80,0);
    ActMan->AddActor(Pyrite2);
    GameApp->AddThrowable(Pyrite2);// */
    ActorRigid* Lead1 = ThrowableGenerator::CreateThrowable("Lead");
    Lead1->GetPhysicsSettings()->SetCollisionShape(LeadCS);
    Lead1->SetLocation(-160,30,0);
    ActMan->AddActor(Lead1);
    GameApp->AddThrowable(Lead1);
    ActorRigid* Lead2 = ThrowableGenerator::CreateThrowable("Lead");
    Lead2->GetPhysicsSettings()->SetCollisionShape(LeadCS);
    Lead2->SetLocation(-120,30,0);
    ActMan->AddActor(Lead2);
    GameApp->AddThrowable(Lead2);// */

    // Create the zones
    Vector3 StartSize(50,50,20);
    StartArea* StartZone = new StartArea("StartArea",Vector3(-140,60,0));
    StartZone->GetPhysicsSettings()->SetCollisionShape(new BoxCollisionShape("StartAreaShape",StartSize));
    StartZone->GetGraphicsSettings()->SetMesh(MeshMan->CreateBoxCornerMesh("StartAreaMesh",ColourValue(0.1,0.8,0.1,0.8),StartSize,4.0));
    PhysMan->AddAreaEffect(StartZone);
    GameApp->RegisterStartArea(StartZone);// */
    ParticleEffect* StartParticle = SceneMan->CreateParticleEffect("StartParticle","StartVortex");
    StartParticle->GetEmitter(0)->SetCustomParam("width",StringTool::ConvertToString(StartSize.X * (Real)1.8));
    StartParticle->GetEmitter(0)->SetCustomParam("height",StringTool::ConvertToString(StartSize.Y * (Real)0.6));
    StartParticle->GetEmitter(0)->SetCustomParam("depth",StringTool::ConvertToString(StartSize.Z * (Real)1.8));
    StartParticle->GetEmitter(0)->SetCustomParam("time_to_live_min",StringTool::ConvertToString((Real)5.0));
    StartParticle->GetEmitter(0)->SetCustomParam("time_to_live_max",StringTool::ConvertToString((Real)6.0));
    StartParticle->SetLocation(Vector3(-140,25,0));
    StartParticle->AddToWorld();// */

    Vector3 ScoreSize(70,40,20);
    ScoreArea* ScoreZone = new ScoreArea("ScoreArea",Vector3(140,20,0));
    ScoreZone->GetPhysicsSettings()->SetCollisionShape(new BoxCollisionShape("ScoreAreaShape",ScoreSize));
    ScoreZone->GetGraphicsSettings()->SetMesh(MeshMan->CreateBoxCornerMesh("ScoreAreaMesh",ColourValue(0.2,0.2,0.8,0.8),ScoreSize,4.0));
    PhysMan->AddAreaEffect(ScoreZone);
    GameApp->RegisterScoreArea(ScoreZone);// */
}

void LevelLoader::LoadJustBounce()
{
    // Get managers
    PhysicsManager* PhysMan = PhysicsManager::GetSingletonPtr();
    SceneManager* SceneMan = SceneManager::GetSingletonPtr();
    ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
    ActorManager* ActMan = ActorManager::GetSingletonPtr();
    CollisionShapeManager* CShapeMan = CollisionShapeManager::GetSingletonPtr();
    MeshManager* MeshMan = MeshManager::GetSingletonPtr();

    // Init Resources
    String CommonGroup("Common");
    String JustBounceGroup("JustBounce");
    String datadir = "Levels/";
    ResourceMan->AddResourceLocation(datadir+"JustBounce.lvl", "Zip", JustBounceGroup, false);
    ResourceMan->InitResourceGroup(JustBounceGroup);

    // Scoring and Shop Setup
    LevelScorer* Scorer = CatchApp::GetCatchAppPointer()->GetLevelScorer();
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Scorer->SetLevelTargetTime(30);
    Shop->SetLevelCash(100);

    // Camera Setup
	Camera* DefCamera = CameraManager::GetSingletonPtr()->GetDefaultCamera();
	DefCamera->SetLocation(Vector3(0,0,425));
	DefCamera->LookAt(Vector3(0,0,0));

	// Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Light* DLight = SceneMan->CreateLight("SceneLight");
    DLight->SetLightType(Light::Directional);
    Vector3 Loc(-150,100,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));

    // Physics Setup
    PhysMan->SetGravity(Vector3(0,-1000,0));

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,120),Vector3(15,-10,120),Vector3(0,10,120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Setup and Create the shapes that will be used.
    CShapeMan->LoadAllShapesFromFile("JustBounce.bullet",JustBounceGroup);

    //----------
    /*std::set<CollisionShape*>& Unnamed = CShapeMan->GetUnnamedShapes();
    for( std::set<CollisionShape*>::iterator CSit = Unnamed.begin() ; CSit != Unnamed.end() ; CSit++ )
    {
        CollisionShape* ToChange = (*CSit);
        if(CollisionShape::ST_Compound==ToChange->GetType())
        {
            if(7 == ((CompoundCollisionShape*)ToChange)->GetNumChildren())
                CShapeMan->SetNameForUnnamedShape("Basket",ToChange);
            else if(2 == ((CompoundCollisionShape*)ToChange)->GetNumChildren())
                CShapeMan->SetNameForUnnamedShape("ElasticBed",ToChange);
        }
    }
    Unnamed.clear();
    CShapeMan->SaveAllStoredShapesToFile("JustBounce.bullet");// */
    //----------

    // Create the basic terrain that will be used
    ActorRigid* TopWall = new ActorRigid(0,"TopWall","wall.mesh",JustBounceGroup);
    TopWall->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GenerateStaticTriMesh("WallShape","wall.mesh",JustBounceGroup));
    TopWall->GetPhysicsSettings()->SetRestitution(0.5);
    TopWall->SetLocation(Vector3(0,85,0));
    ActMan->AddActor(TopWall);

    ActorRigid* Basket = new ActorRigid(0,"Basket","basket.mesh",JustBounceGroup);
    Basket->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("Basket"));
    Basket->SetLocation(Vector3(165,-25,0));
    ActMan->AddActor(Basket);

    ActorRigid* ElasticFloor = new ActorRigid(0,"ElasticFloor","elastic_bed.mesh",JustBounceGroup);
    ElasticFloor->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GetShape("ElasticBed"));//CShapeMan->GenerateStaticTriMesh("ElasticShape","elastic_bed.mesh",JustBounceGroup));
    ElasticFloor->GetPhysicsSettings()->SetRestitution(1.0);
    //ElasticFloor->GetPhysicsSettings()->SetFriction(0.4);
    ElasticFloor->SetLocation(Vector3(-20,-70,0));
    ActMan->AddActor(ElasticFloor);

    ActorRigid* ElasticCase = new ActorRigid(0,"ElasticCase","case.mesh",JustBounceGroup);
    ElasticCase->GetPhysicsSettings()->SetCollisionShape(CShapeMan->GenerateStaticTriMesh("CaseShape","case.mesh",JustBounceGroup));
    ElasticCase->SetLocation(Vector3(-20,-71,0));
    ActMan->AddActor(ElasticCase);

    // Create some throwable objects
    ThrowableData* RubberData = ThrowableGenerator::GetThrowableData("Rubber");
    ThrowableData* WoodData = ThrowableGenerator::GetThrowableData("Wood");
    CollisionShape* RubberCS = new SphereCollisionShape("RubberCS",11.4);//CShapeMan->GenerateConvexHull("RubberCS",RubberData->MeshName,CommonGroup);
    CollisionShape* WoodCS = CShapeMan->GenerateConvexHull("WoodCS",WoodData->MeshName,CommonGroup);// */
    ActorRigid* Rubber1 = ThrowableGenerator::CreateThrowable("Rubber");
    Rubber1->GetPhysicsSettings()->SetCollisionShape(RubberCS);
    Rubber1->SetLocation(-185,110,0);
    ActMan->AddActor(Rubber1);
    GameApp->AddThrowable(Rubber1);
    ActorRigid* Rubber2 = ThrowableGenerator::CreateThrowable("Rubber");
    Rubber2->GetPhysicsSettings()->SetCollisionShape(RubberCS);
    Rubber2->SetLocation(-155,110,0);
    ActMan->AddActor(Rubber2);
    GameApp->AddThrowable(Rubber2);// */
    ActorRigid* Wood1 = ThrowableGenerator::CreateThrowable("Wood");
    Wood1->GetPhysicsSettings()->SetCollisionShape(WoodCS);
    Wood1->SetLocation(-185,70,0);
    ActMan->AddActor(Wood1);
    GameApp->AddThrowable(Wood1);
    ActorRigid* Wood2 = ThrowableGenerator::CreateThrowable("Wood");
    Wood2->GetPhysicsSettings()->SetCollisionShape(WoodCS);
    Wood2->SetLocation(-155,70,0);
    ActMan->AddActor(Wood2);
    GameApp->AddThrowable(Wood2);// */


    // Create the zones
    Vector3 StartSize(50,50,20);
    StartArea* StartZone = new StartArea("StartArea",Vector3(-170,90,0));
    StartZone->GetPhysicsSettings()->SetCollisionShape(new BoxCollisionShape("StartAreaShape",StartSize));
    StartZone->GetGraphicsSettings()->SetMesh(MeshMan->CreateBoxCornerMesh("StartAreaMesh",ColourValue(0.1,0.8,0.1,0.8),StartSize,4.0));
    PhysMan->AddAreaEffect(StartZone);
    GameApp->RegisterStartArea(StartZone);// */
    ParticleEffect* StartParticle = SceneMan->CreateParticleEffect("StartParticle","StartVortex");
    StartParticle->GetEmitter(0)->SetCustomParam("width",StringTool::ConvertToString(StartSize.X * (Real)1.8));
    StartParticle->GetEmitter(0)->SetCustomParam("height",StringTool::ConvertToString(StartSize.Y * (Real)0.6));
    StartParticle->GetEmitter(0)->SetCustomParam("depth",StringTool::ConvertToString(StartSize.Z * (Real)1.8));
    StartParticle->GetEmitter(0)->SetCustomParam("time_to_live_min",StringTool::ConvertToString((Real)5.0));
    StartParticle->GetEmitter(0)->SetCustomParam("time_to_live_max",StringTool::ConvertToString((Real)6.0));
    StartParticle->SetLocation(Vector3(-170,55,0));
    StartParticle->AddToWorld();// */

    Vector3 ScoreSize(42,50,35);
    ScoreArea* ScoreZone = new ScoreArea("ScoreArea",Vector3(158,-25,0));
    ScoreZone->GetPhysicsSettings()->SetCollisionShape(new BoxCollisionShape("ScoreAreaShape",ScoreSize));
    ScoreZone->GetGraphicsSettings()->SetMesh(MeshMan->CreateBoxCornerMesh("ScoreAreaMesh",ColourValue(0.2,0.2,0.8,0.8),ScoreSize,4.0));
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
    else if("Rollers" == LevelToLoad)
        LoadRollers();
    else if("JustBounce" == LevelToLoad)
        LoadJustBounce();

    CurrentLevel = LevelToLoad;
    LevelToLoad = "";
}

#endif
