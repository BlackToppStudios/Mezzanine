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
    this->TheEntresol = Entresol::GetSingletonPtr();
    this->GameApp = CatchApp::GetCatchAppPointer();
}

LevelLoader::~LevelLoader()
{
}

void LevelLoader::LoadFerris()
{
    // Get managers
    ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
    Physics::CollisionShapeManager* CShapeMan = Physics::CollisionShapeManager::GetSingletonPtr();
    Graphics::MeshManager* MeshMan = Graphics::MeshManager::GetSingletonPtr();
    Physics::PhysicsManager* PhysMan = this->TheEntresol->GetPhysicsManager();
    Graphics::SceneManager* SceneMan = this->TheEntresol->GetSceneManager();
    AreaEffectManager* AreaEffectMan = this->TheEntresol->GetAreaEffectManager();
    DebrisManager* DebrisMan = this->TheEntresol->GetDebrisManager();

    // Init Resources
    String CommonGroup("Common");
    String FerrisGroup("Ferris");
    String datadir = "Levels/";
    ResourceMan->AddAssetLocation(datadir+"Ferris.lvl", AT_Zip, FerrisGroup, false);
    ResourceMan->InitAssetGroup(FerrisGroup);

    // Scoring and Shop Setup
    LevelScorer* Scorer = CatchApp::GetCatchAppPointer()->GetLevelScorer();
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Scorer->SetLevelTargetTime(30);
    Shop->SetLevelCash(100);

    // Camera Setup
	Graphics::CameraProxy* DefCamera = this->TheEntresol->GetCameraManager()->GetCamera(0);
	DefCamera->SetLocation(Vector3(0,0,425));
	DefCamera->LookAt(Vector3(0,0,0));

	// Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Graphics::LightProxy* DLight = SceneMan->CreateLightProxy(Graphics::LT_Directional);
    Vector3 Loc(150,250,-200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));
    DLight->AddToWorld();

    // Physics Setup
    PhysMan->SetWorldGravity(Vector3(0,-1000,0));

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,120),Vector3(15,-10,120),Vector3(0,10,120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Setup and Create the shapes that will be used.
    CShapeMan->LoadAllShapesFromBinaryFile("Ferris.bullet",FerrisGroup);

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
    RigidDebris* FerrisWheel = DebrisMan->CreateRigidDebris("FerrisWheel",100.0);
    FerrisWheel->GetRigidProxy()->SetDamping(0,0.1);
    FerrisWheel->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Wheel") ); // The names aren't being loaded as we expected
    FerrisWheel->GetEntityProxy()->SetMesh("wheel.mesh",FerrisGroup);
    FerrisWheel->SetLocation(100,0,0);
    FerrisWheel->AddToWorld();

    // Create the trayz
    Real TrayMass = 50.0;
    RigidDebris* Tray1 = DebrisMan->CreateRigidDebris("Tray1",TrayMass);
    Tray1->GetRigidProxy()->SetDamping(0,0.1);
    Tray1->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray1->GetEntityProxy()->SetMesh("bluetray.mesh",FerrisGroup);
    Tray1->SetLocation(30.4,14.3,0);
    Tray1->AddToWorld();
    RigidDebris* Tray2 = DebrisMan->CreateRigidDebris("Tray2",TrayMass);
    Tray2->GetRigidProxy()->SetDamping(0,0.1);
    Tray2->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray2->GetEntityProxy()->SetMesh("purpletray.mesh",FerrisGroup);
    Tray2->SetLocation(169.6,14.3,0);
    Tray2->AddToWorld();
    RigidDebris* Tray3 = DebrisMan->CreateRigidDebris("Tray3",TrayMass);
    Tray3->GetRigidProxy()->SetDamping(0,0.1);
    Tray3->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray3->GetEntityProxy()->SetMesh("purpletray.mesh",FerrisGroup);
    Tray3->SetLocation(30.4,-42.7,0);
    Tray3->AddToWorld();
    RigidDebris* Tray4 = DebrisMan->CreateRigidDebris("Tray4",TrayMass);
    Tray4->GetRigidProxy()->SetDamping(0,0.1);
    Tray4->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray4->GetEntityProxy()->SetMesh("bluetray.mesh",FerrisGroup);
    Tray4->SetLocation(169.6,-42.7,0);
    Tray4->AddToWorld();
    RigidDebris* Tray5 = DebrisMan->CreateRigidDebris("Tray5",TrayMass);
    Tray5->GetRigidProxy()->SetDamping(0,0.1);
    Tray5->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray5->GetEntityProxy()->SetMesh("purpletray.mesh",FerrisGroup);
    Tray5->SetLocation(71.5,55.4,0);
    Tray5->AddToWorld();
    RigidDebris* Tray6 = DebrisMan->CreateRigidDebris("Tray6",TrayMass);
    Tray6->GetRigidProxy()->SetDamping(0,0.1);
    Tray6->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray6->GetEntityProxy()->SetMesh("bluetray.mesh",FerrisGroup);
    Tray6->SetLocation(128.5,55.4,0);
    Tray6->AddToWorld();
    RigidDebris* Tray7 = DebrisMan->CreateRigidDebris("Tray7",TrayMass);
    Tray7->GetRigidProxy()->SetDamping(0,0.1);
    Tray7->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray7->GetEntityProxy()->SetMesh("bluetray.mesh",FerrisGroup);
    Tray7->SetLocation(71.5,-83.8,0);
    Tray7->AddToWorld();
    RigidDebris* Tray8 = DebrisMan->CreateRigidDebris("Tray8",TrayMass);
    Tray8->GetRigidProxy()->SetDamping(0,0.1);
    Tray8->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray8->GetEntityProxy()->SetMesh("purpletray.mesh",FerrisGroup);
    Tray8->SetLocation(128.5,-83.8,0);
    Tray8->AddToWorld();// */

    // Create world anchor for the wheel, which will allow it to spin.
    Physics::HingeConstraint* WheelAnchor = new Physics::HingeConstraint(FerrisWheel->GetRigidProxy(),Vector3(0,0,0),Vector3(0,0,1),true);
    PhysMan->AddConstraint(WheelAnchor,false);// */

    // Create the series of hinges for connecting the 8 trays to the wheel
    Physics::HingeConstraint* Tray1Anchor = new Physics::HingeConstraint(FerrisWheel->GetRigidProxy(),Tray1->GetRigidProxy(),Vector3(-69.6,28.5,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(Tray1Anchor,true);
    Physics::HingeConstraint* Tray2Anchor = new Physics::HingeConstraint(FerrisWheel->GetRigidProxy(),Tray2->GetRigidProxy(),Vector3(69.6,28.5,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(Tray2Anchor,true);
    Physics::HingeConstraint* Tray3Anchor = new Physics::HingeConstraint(FerrisWheel->GetRigidProxy(),Tray3->GetRigidProxy(),Vector3(-69.6,-28.5,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(Tray3Anchor,true);
    Physics::HingeConstraint* Tray4Anchor = new Physics::HingeConstraint(FerrisWheel->GetRigidProxy(),Tray4->GetRigidProxy(),Vector3(69.6,-28.5,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(Tray4Anchor,true);
    Physics::HingeConstraint* Tray5Anchor = new Physics::HingeConstraint(FerrisWheel->GetRigidProxy(),Tray5->GetRigidProxy(),Vector3(-28.5,69.6,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(Tray5Anchor,true);
    Physics::HingeConstraint* Tray6Anchor = new Physics::HingeConstraint(FerrisWheel->GetRigidProxy(),Tray6->GetRigidProxy(),Vector3(28.5,69.6,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(Tray6Anchor,true);
    Physics::HingeConstraint* Tray7Anchor = new Physics::HingeConstraint(FerrisWheel->GetRigidProxy(),Tray7->GetRigidProxy(),Vector3(-28.5,-69.6,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(Tray7Anchor,true);
    Physics::HingeConstraint* Tray8Anchor = new Physics::HingeConstraint(FerrisWheel->GetRigidProxy(),Tray8->GetRigidProxy(),Vector3(28.5,-69.6,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1),false);
    PhysMan->AddConstraint(Tray8Anchor,true);// */

    // Create some throwable objects
    ThrowableData* ClayData = ThrowableGenerator::GetThrowableData("Clay");
    ThrowableData* LeadData = ThrowableGenerator::GetThrowableData("Lead");
    ThrowableData* UraniumData = ThrowableGenerator::GetThrowableData("Uranium");
    ThrowableData* PyriteData = ThrowableGenerator::GetThrowableData("Pyrite");
    Physics::CollisionShape* ClayCS = CShapeMan->GenerateConvexHull("ClayCS",ClayData->MeshName,CommonGroup);
    Physics::CollisionShape* LeadCS = CShapeMan->GenerateConvexHull("LeadCS",LeadData->MeshName,CommonGroup);
    Physics::CollisionShape* UraniumCS = CShapeMan->GenerateConvexHull("UraniumCS",UraniumData->MeshName,CommonGroup);
    Physics::CollisionShape* PyriteCS = CShapeMan->GenerateConvexHull("PyriteCS",PyriteData->MeshName,CommonGroup);

    RigidDebris* Uranium1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Uranium") );
    Uranium1->GetRigidProxy()->SetCollisionShape(UraniumCS);
    //Uranium1->GetRigidProxy()->SetStickyData(1);
    Uranium1->SetLocation(-145,60,0);
    Uranium1->AddToWorld();
    GameApp->AddThrowable(Uranium1);
    RigidDebris* Uranium2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Uranium") );
    Uranium2->GetRigidProxy()->SetCollisionShape(UraniumCS);
    //Uranium2->GetRigidProxy()->SetStickyData(1);
    Uranium2->SetLocation(-195,60,0);
    Uranium2->AddToWorld();
    GameApp->AddThrowable(Uranium2);// */
    RigidDebris* Lead1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Lead") );
    Lead1->GetRigidProxy()->SetCollisionShape(LeadCS);
    Lead1->SetLocation(-145,20,0);
    Lead1->AddToWorld();
    GameApp->AddThrowable(Lead1);
    RigidDebris* Lead2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Lead") );
    Lead2->GetRigidProxy()->SetCollisionShape(LeadCS);
    Lead2->SetLocation(-195,20,0);
    Lead2->AddToWorld();
    GameApp->AddThrowable(Lead2);// */
    RigidDebris* Clay1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Clay") );
    Clay1->GetRigidProxy()->SetCollisionShape(ClayCS);
    Clay1->SetLocation(-145,-20,0);
    Clay1->AddToWorld();
    GameApp->AddThrowable(Clay1);
    RigidDebris* Clay2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Clay") );
    Clay2->GetRigidProxy()->SetCollisionShape(ClayCS);
    Clay2->SetLocation(-195,-20,0);
    Clay2->AddToWorld();
    GameApp->AddThrowable(Clay2);// */
    RigidDebris* Pyrite1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Pyrite") );
    Pyrite1->GetRigidProxy()->SetCollisionShape(PyriteCS);
    Pyrite1->SetLocation(-145,-60,0);
    Pyrite1->AddToWorld();
    GameApp->AddThrowable(Pyrite1);
    RigidDebris* Pyrite2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Pyrite") );
    Pyrite2->GetRigidProxy()->SetCollisionShape(PyriteCS);
    Pyrite2->SetLocation(-195,-60,0);
    Pyrite2->AddToWorld();
    GameApp->AddThrowable(Pyrite2);// */

    // Create the zones
    Vector3 StartSize(50,130,25);
    NameValuePairMap StartZoneParams;
    StartZoneParams["SizeX"] = StringTools::ConvertToString(StartSize.X);
    StartZoneParams["SizeY"] = StringTools::ConvertToString(StartSize.Y);
    StartZoneParams["SizeZ"] = StringTools::ConvertToString(StartSize.Z);

    StartArea* StartZone = static_cast<StartArea*>( AreaEffectMan->CreateAreaEffect("StartArea","StartArea1",StartZoneParams) );
    StartZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("StartAreaShape",StartSize) );
    StartZone->GetEntityProxy()->SetMesh( MeshMan->CreateBoxCornerMesh("StartAreaMesh",ColourValue(0.1,0.8,0.1,0.8),StartSize,4.0) );
    StartZone->SetParticleMinimumTimeToLive(11.0);
    StartZone->SetParticleMaximumTimeToLive(15.0);
    StartZone->SetLocation(-170,0,0);
    StartZone->AddToWorld();// */

    Vector3 ScoreSize(100,100,30);
    NameValuePairMap ScoreZoneParams;
    ScoreZoneParams["SizeX"] = StringTools::ConvertToString(ScoreSize.X);
    ScoreZoneParams["SizeY"] = StringTools::ConvertToString(ScoreSize.Y);
    ScoreZoneParams["SizeZ"] = StringTools::ConvertToString(ScoreSize.Z);

    ScoreArea* ScoreZone = static_cast<ScoreArea*>( AreaEffectMan->CreateAreaEffect("ScoreArea","ScoreArea1",ScoreZoneParams) );
    ScoreZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("ScoreAreaShape",ScoreSize) );
    ScoreZone->GetEntityProxy()->SetMesh(MeshMan->CreateBoxCornerMesh("ScoreAreaMesh",ColourValue(0.2,0.2,0.8,0.8),ScoreSize,4.0));
    ScoreZone->SetLocation(100,0,0);
    ScoreZone->AddToWorld();// */

    Vector3 BonusScoreSize(18,18,30);
    NameValuePairMap BonusZoneParams;
    BonusZoneParams["SizeX"] = StringTools::ConvertToString(BonusScoreSize.X);
    BonusZoneParams["SizeY"] = StringTools::ConvertToString(BonusScoreSize.Y);
    BonusZoneParams["SizeZ"] = StringTools::ConvertToString(BonusScoreSize.Z);
    Physics::CollisionShape* BonusScoreShape = new Physics::BoxCollisionShape("BonusAreaShape",BonusScoreSize);
    Graphics::Mesh* BonusScoreMesh = MeshMan->CreateBoxCornerMesh("BonusAreaMesh",ColourValue(0.50,0.15,0.65,0.8),BonusScoreSize,4.0);

    ScoreArea* BonusZone1 = static_cast<ScoreArea*>( AreaEffectMan->CreateAreaEffect("ScoreArea","BonusArea1",BonusZoneParams) );
    BonusZone1->GetGhostProxy()->SetCollisionShape(BonusScoreShape);
    BonusZone1->GetEntityProxy()->SetMesh(BonusScoreMesh);
    BonusZone1->SetScoreMultiplier(2.0);
    BonusZone1->SetLocation(169.6,14.3,0);
    BonusZone1->AddToWorld();
    Tray2->GetRigidProxy()->AddSyncObject(BonusZone1);// */
    ScoreArea* BonusZone2 = static_cast<ScoreArea*>( AreaEffectMan->CreateAreaEffect("ScoreArea","BonusArea2",BonusZoneParams) );
    BonusZone2->GetGhostProxy()->SetCollisionShape(BonusScoreShape);
    BonusZone2->GetEntityProxy()->SetMesh(BonusScoreMesh);
    BonusZone2->SetScoreMultiplier(2.0);
    BonusZone2->SetLocation(30.4,-42.7,0);
    BonusZone2->AddToWorld();
    Tray3->GetRigidProxy()->AddSyncObject(BonusZone2);// */
    ScoreArea* BonusZone3 = static_cast<ScoreArea*>( AreaEffectMan->CreateAreaEffect("ScoreArea","BonusArea3",BonusZoneParams) );
    BonusZone3->GetGhostProxy()->SetCollisionShape(BonusScoreShape);
    BonusZone3->GetEntityProxy()->SetMesh(BonusScoreMesh);
    BonusZone3->SetScoreMultiplier(2.0);
    BonusZone3->SetLocation(71.5,55.4,0);
    BonusZone3->AddToWorld();
    Tray5->GetRigidProxy()->AddSyncObject(BonusZone3);// */
    ScoreArea* BonusZone4 = static_cast<ScoreArea*>( AreaEffectMan->CreateAreaEffect("ScoreArea","BonusArea4",BonusZoneParams) );
    BonusZone4->GetGhostProxy()->SetCollisionShape(BonusScoreShape);
    BonusZone4->GetEntityProxy()->SetMesh(BonusScoreMesh);
    BonusZone4->SetScoreMultiplier(2.0);
    BonusZone4->SetLocation(128.5,-83.8,0);
    BonusZone4->AddToWorld();
    Tray8->GetRigidProxy()->AddSyncObject(BonusZone4);// */
}

void LevelLoader::LoadBigCurve()
{
    // Get managers
    ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
    Physics::CollisionShapeManager* CShapeMan = Physics::CollisionShapeManager::GetSingletonPtr();
    Graphics::MeshManager* MeshMan = Graphics::MeshManager::GetSingletonPtr();
    Physics::PhysicsManager* PhysMan = this->TheEntresol->GetPhysicsManager();
    Graphics::SceneManager* SceneMan = this->TheEntresol->GetSceneManager();
    AreaEffectManager* AreaEffectMan = this->TheEntresol->GetAreaEffectManager();
    DebrisManager* DebrisMan = this->TheEntresol->GetDebrisManager();

    // Init Resources
    String CommonGroup("Common");
    String BigCurveGroup("BigCurve");
    String datadir = "Levels/";
    ResourceMan->AddAssetLocation(datadir+"BigCurve.lvl", AT_Zip, BigCurveGroup, false);
    ResourceMan->InitAssetGroup(BigCurveGroup);

    // Scoring and Shop Setup
    LevelScorer* Scorer = CatchApp::GetCatchAppPointer()->GetLevelScorer();
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Scorer->SetLevelTargetTime(30);
    Shop->SetLevelCash(100);

    // Camera Setup
	Graphics::CameraProxy* DefCamera = this->TheEntresol->GetCameraManager()->GetCamera(0);
	DefCamera->SetLocation(Vector3(0,0,425));
	DefCamera->LookAt(Vector3(0,0,0));

	// Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Graphics::LightProxy* DLight = SceneMan->CreateLightProxy(Graphics::LT_Directional);
    Vector3 Loc(150,200,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));
    DLight->AddToWorld();

    // Physics Setup
    PhysMan->SetWorldGravity(Vector3(0,-1000,0));

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,120),Vector3(15,-10,120),Vector3(0,10,120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Setup and Create the shapes that will be used.
    // -no files to be loaded for this level-

    // Create the Cave
    RigidDebris* CurveCave = DebrisMan->CreateRigidDebris("CurveCave",0);
    CurveCave->GetRigidProxy()->SetCollisionShape(CShapeMan->GenerateStaticTriMesh("Cave","cave.mesh",BigCurveGroup));
    CurveCave->GetEntityProxy()->SetMesh("cave.mesh",BigCurveGroup);
    CurveCave->SetLocation(130,0,-10);
    CurveCave->SetScale(1.05,1.05,1.05);
    CurveCave->AddToWorld();

    // Create some throwable objects
    ThrowableData* PyriteData = ThrowableGenerator::GetThrowableData("Pyrite");
    ThrowableData* RubberData = ThrowableGenerator::GetThrowableData("Rubber");
    Physics::CollisionShape* PyriteCS = CShapeMan->GenerateConvexHull("PyriteCS",PyriteData->MeshName,CommonGroup);
    Physics::CollisionShape* RubberCS = CShapeMan->GenerateConvexHull("RubberCS",RubberData->MeshName,CommonGroup);

    RigidDebris* Pyrite1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Pyrite") );
    Pyrite1->GetRigidProxy()->SetCollisionShape( PyriteCS );
    Pyrite1->SetLocation(-155,-20,0);
    Pyrite1->AddToWorld();
    GameApp->AddThrowable(Pyrite1);
    RigidDebris* Pyrite2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Pyrite") );
    Pyrite2->GetRigidProxy()->SetCollisionShape( PyriteCS );
    Pyrite2->SetLocation(-205,-20,0);
    Pyrite2->AddToWorld();
    GameApp->AddThrowable(Pyrite2);// */
    RigidDebris* Rubber1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Rubber") );
    Rubber1->GetRigidProxy()->SetCollisionShape( RubberCS );
    Rubber1->SetLocation(-155,-60,0);
    Rubber1->AddToWorld();
    GameApp->AddThrowable(Rubber1);
    RigidDebris* Rubber2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Rubber") );
    Rubber2->GetRigidProxy()->SetCollisionShape( RubberCS );
    Rubber2->SetLocation(-205,-60,0);
    Rubber2->AddToWorld();
    GameApp->AddThrowable(Rubber2);
    RigidDebris* Rubber3 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Rubber") );
    Rubber3->GetRigidProxy()->SetCollisionShape( RubberCS );
    Rubber3->SetLocation(-155,-100,0);
    Rubber3->AddToWorld();
    GameApp->AddThrowable(Rubber3);
    RigidDebris* Rubber4 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Rubber") );
    Rubber4->GetRigidProxy()->SetCollisionShape( RubberCS );
    Rubber4->SetLocation(-205,-100,0);
    Rubber4->AddToWorld();
    GameApp->AddThrowable(Rubber4);// */

    // Create the zones
    Vector3 StartSize(50,80,20);
    NameValuePairMap StartZoneParams;
    StartZoneParams["SizeX"] = StringTools::ConvertToString(StartSize.X);
    StartZoneParams["SizeY"] = StringTools::ConvertToString(StartSize.Y);
    StartZoneParams["SizeZ"] = StringTools::ConvertToString(StartSize.Z);

    StartArea* StartZone = static_cast<StartArea*>( AreaEffectMan->CreateAreaEffect("StartArea","StartArea1",StartZoneParams) );
    StartZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("StartAreaShape",StartSize) );
    StartZone->GetEntityProxy()->SetMesh( MeshMan->CreateBoxCornerMesh("StartAreaMesh",ColourValue(0.1,0.8,0.1,0.8),StartSize,4.0) );
    StartZone->SetParticleMinimumTimeToLive(8.0);
    StartZone->SetParticleMaximumTimeToLive(10.0);
    StartZone->SetLocation(-180,-60,0);
    StartZone->AddToWorld();// */

    Vector3 ScoreSize(50,40,50);
    NameValuePairMap ScoreZoneParams;
    ScoreZoneParams["SizeX"] = StringTools::ConvertToString(ScoreSize.X);
    ScoreZoneParams["SizeY"] = StringTools::ConvertToString(ScoreSize.Y);
    ScoreZoneParams["SizeZ"] = StringTools::ConvertToString(ScoreSize.Z);

    ScoreArea* ScoreZone = static_cast<ScoreArea*>( AreaEffectMan->CreateAreaEffect("ScoreArea","ScoreArea1",ScoreZoneParams) );
    ScoreZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("ScoreAreaShape",ScoreSize) );
    ScoreZone->GetEntityProxy()->SetMesh(MeshMan->CreateBoxCornerMesh("ScoreAreaMesh",ColourValue(0.2,0.2,0.8,0.8),ScoreSize,4.0));
    ScoreZone->SetLocation(120,-20,-10);
    ScoreZone->AddToWorld();// */
}

void LevelLoader::LoadBlowsNotSucks()
{
    // Get managers
    ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
    Physics::CollisionShapeManager* CShapeMan = Physics::CollisionShapeManager::GetSingletonPtr();
    Graphics::MeshManager* MeshMan = Graphics::MeshManager::GetSingletonPtr();
    Physics::PhysicsManager* PhysMan = this->TheEntresol->GetPhysicsManager();
    Graphics::SceneManager* SceneMan = this->TheEntresol->GetSceneManager();
    AreaEffectManager* AreaEffectMan = this->TheEntresol->GetAreaEffectManager();
    DebrisManager* DebrisMan = this->TheEntresol->GetDebrisManager();

    // Init Resources
    String CommonGroup("Common");
    String BlowsNotSucksGroup("BlowsNotSucks");
    String datadir = "Levels/";
    ResourceMan->AddAssetLocation(datadir+"BlowsNotSucks.lvl", AT_Zip, BlowsNotSucksGroup, false);
    ResourceMan->InitAssetGroup(BlowsNotSucksGroup);

    // Scoring and Shop Setup
    LevelScorer* Scorer = CatchApp::GetCatchAppPointer()->GetLevelScorer();
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Scorer->SetLevelTargetTime(30);
    Shop->SetLevelCash(100);

    // Camera Setup
	Graphics::CameraProxy* DefCamera = this->TheEntresol->GetCameraManager()->GetCamera(0);
	DefCamera->SetLocation(Vector3(0,0,425));
	DefCamera->LookAt(Vector3(0,0,0));

	// Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Graphics::LightProxy* DLight = SceneMan->CreateLightProxy(Graphics::LT_Directional);
    Vector3 Loc(-150,200,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));
    DLight->AddToWorld();

    // Physics Setup
    PhysMan->SetWorldGravity(Vector3(0,-1000,0));

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,120),Vector3(15,-10,120),Vector3(0,10,120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Setup and Create the shapes that will be used.
    CShapeMan->LoadAllShapesFromBinaryFile("BlowsNotSucks.bullet",BlowsNotSucksGroup);

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
    RigidDebris* Fan = DebrisMan->CreateRigidDebris("Fan",25);
    Fan->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Fan") );
    Fan->GetRigidProxy()->SetActivationState(Mezzanine::Physics::AS_DisableDeactivation);
    Fan->GetEntityProxy()->SetMesh("fan.mesh",BlowsNotSucksGroup);
    Fan->SetLocation(103.388,-58.888,7.5);
    Fan->SetOrientation(Quaternion(MathTools::GetQuarterPi(),Vector3(0,0,1)));
    Fan->AddToWorld();

    RigidDebris* FanBody = DebrisMan->CreateRigidDebris("FanBody",0);
    FanBody->GetRigidProxy()->SetCollisionShape( CShapeMan->GenerateStaticTriMesh("Body","body.mesh",BlowsNotSucksGroup) );
    FanBody->GetEntityProxy()->SetMesh("body.mesh",BlowsNotSucksGroup);
    FanBody->SetLocation(132.5,-70,25);
    FanBody->AddToWorld();

    RigidDebris* FanButton = DebrisMan->CreateRigidDebris("FanButton",2.5);
    FanButton->GetRigidProxy()->SetCollisionShape( CShapeMan->GenerateConvexHull("Button","button.mesh",BlowsNotSucksGroup) );
    FanButton->GetRigidProxy()->SetGravity( Vector3(0,0,0) );
    FanButton->GetRigidProxy()->SetAngularMovementFactor(Vector3(0,0,0));
    FanButton->GetEntityProxy()->SetMesh("button.mesh",BlowsNotSucksGroup);
    FanButton->SetLocation(-10.5,-107.0,-0.5);
    FanButton->AddToWorld();

    // Create the series of constraints for assembling the fan
    Physics::HingeConstraint* FanToBody = new Physics::HingeConstraint(FanBody->GetRigidProxy(),Fan->GetRigidProxy(),Vector3(-12,-6,-17.5),Vector3(0,-24.2,0),Vector3(-1,1,0).Normalize(),Vector3(0,1,0),false);
    PhysMan->AddConstraint(FanToBody,true);
    FanToBody->SetLimit(1.0,-1.0);

    Physics::Generic6DofSpringConstraint* ButtonToBody = new Physics::Generic6DofSpringConstraint(FanBody->GetRigidProxy(),FanButton->GetRigidProxy(),Vector3(-143,-52,-25.5),Vector3(0,0,0),Quaternion(0,0,0,1),Quaternion(0,0,0,1),true);
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
    Quaternion FanWindOrientation(MathTools::GetQuarterPi(),Vector3(0,0,1));
    FieldOfForce* FanWind = AreaEffectMan->CreateFieldOfForce("FanWind");
    FanWind->GetGhostProxy()->SetCollisionShape( new Physics::CylinderCollisionShape("FanWindShape",60,115,Vector3::Unit_Y()) );
    FanWind->SetOrientation(FanWindOrientation);
    FanWind->SetDirectionOfForce(Vector3(-1,1,0));
    FanWind->SetAttenuation(0.2f,Mezzanine::Att_Linear,Vector3(-125.5,-81,7.5));
    FanWind->SetFieldStrength(1000.f);
    FanWind->SetLocation(5,40,7.5);
    FanWind->AddToWorld();
    Graphics::ParticleSystemProxy* WindParticle = SceneMan->CreateParticleSystemProxy("WindTunnel");
    WindParticle->GetEmitter(0)->SetCustomParam("width",StringTools::ConvertToString(FanWindSize.X * (Real)1.8));
    WindParticle->GetEmitter(0)->SetCustomParam("height",StringTools::ConvertToString(FanWindSize.Y * (Real)0.6));
    WindParticle->GetEmitter(0)->SetCustomParam("depth",StringTools::ConvertToString(FanWindSize.Z * (Real)1.8));
    WindParticle->GetEmitter(0)->SetCustomParam("position","0 -60.0 0");
    WindParticle->SetLocation(5,40,7.5);
    WindParticle->SetOrientation(FanWindOrientation);
    WindParticle->AddToWorld();// */

    // Create the trigger for the fan
    BNS_Fan* FanTrigger = new BNS_Fan("FanTigger",FanToBody,FanButton,Fan,FanWind,WindParticle);
    PhysMan->AddWorldTrigger(FanTrigger);

    // Create the goal tray
    RigidDebris* GoalTray = DebrisMan->CreateRigidDebris("GoalTray",0);
    GoalTray->GetRigidProxy()->SetCollisionShape( CShapeMan->GenerateStaticTriMesh("GoalShape","tray.mesh",BlowsNotSucksGroup) );
    GoalTray->GetEntityProxy()->SetMesh("tray.mesh",BlowsNotSucksGroup);
    GoalTray->SetLocation(-170,100,0);
    GoalTray->AddToWorld();

    // Create some throwable objects
    ThrowableData* UraniumData = ThrowableGenerator::GetThrowableData("Uranium");
    ThrowableData* StyrofoamData = ThrowableGenerator::GetThrowableData("Styrofoam");
    Physics::CollisionShape* UraniumCS = CShapeMan->GenerateConvexHull("UraniumCS",UraniumData->MeshName,CommonGroup);
    Physics::CollisionShape* StyrofoamCS = CShapeMan->GenerateConvexHull("StyrofoamCS",StyrofoamData->MeshName,CommonGroup);

    RigidDebris* Uranium1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Uranium") );
    Uranium1->GetRigidProxy()->SetCollisionShape(UraniumCS);
    Uranium1->SetLocation(-155,-20,0);
    Uranium1->AddToWorld();
    GameApp->AddThrowable(Uranium1);
    RigidDebris* Uranium2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Uranium") );
    Uranium2->GetRigidProxy()->SetCollisionShape(UraniumCS);
    Uranium2->SetLocation(-185,-20,0);
    Uranium2->AddToWorld();
    GameApp->AddThrowable(Uranium2);// */
    RigidDebris* Styrofoam1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Styrofoam") );
    Styrofoam1->GetRigidProxy()->SetCollisionShape(StyrofoamCS);
    Styrofoam1->SetLocation(-155,-60,0);
    Styrofoam1->AddToWorld();
    GameApp->AddThrowable(Styrofoam1);
    RigidDebris* Styrofoam2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Styrofoam") );
    Styrofoam2->GetRigidProxy()->SetCollisionShape(StyrofoamCS);
    Styrofoam2->SetLocation(-185,-60,0);
    Styrofoam2->AddToWorld();
    GameApp->AddThrowable(Styrofoam2);
    RigidDebris* Styrofoam3 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Styrofoam") );
    Styrofoam3->GetRigidProxy()->SetCollisionShape(StyrofoamCS);
    Styrofoam3->SetLocation(-155,-100,0);
    Styrofoam3->AddToWorld();
    GameApp->AddThrowable(Styrofoam3);
    RigidDebris* Styrofoam4 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Styrofoam") );
    Styrofoam4->GetRigidProxy()->SetCollisionShape(StyrofoamCS);
    Styrofoam4->SetLocation(-185,-100,0);
    Styrofoam4->AddToWorld();
    GameApp->AddThrowable(Styrofoam4);// */

    // Create the zones
    Vector3 StartSize(50,70,20);
    NameValuePairMap StartZoneParams;
    StartZoneParams["SizeX"] = StringTools::ConvertToString(StartSize.X);
    StartZoneParams["SizeY"] = StringTools::ConvertToString(StartSize.Y);
    StartZoneParams["SizeZ"] = StringTools::ConvertToString(StartSize.Z);

    StartArea* StartZone = static_cast<StartArea*>( AreaEffectMan->CreateAreaEffect("StartArea","StartArea1",StartZoneParams) );
    StartZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("StartAreaShape",StartSize) );
    StartZone->GetEntityProxy()->SetMesh( MeshMan->CreateBoxCornerMesh("StartAreaMesh",ColourValue(0.1,0.8,0.1,0.8),StartSize,4.0) );
    StartZone->SetParticleMinimumTimeToLive(6.0);
    StartZone->SetParticleMaximumTimeToLive(9.0);
    StartZone->SetLocation(-170,-70,0);
    StartZone->AddToWorld();// */

    Vector3 Score1Size(25,21,25);
    NameValuePairMap ScoreZone1Params;
    ScoreZone1Params["SizeX"] = StringTools::ConvertToString(Score1Size.X);
    ScoreZone1Params["SizeY"] = StringTools::ConvertToString(Score1Size.Y);
    ScoreZone1Params["SizeZ"] = StringTools::ConvertToString(Score1Size.Z);
    Physics::CollisionShape* ScoreArea1Shape = new Physics::BoxCollisionShape("ScoreArea1Shape",Score1Size);
    Graphics::Mesh* ScoreArea1Mesh = MeshMan->CreateBoxCornerMesh("ScoreArea1Mesh",ColourValue(0.2,0.2,0.8,0.8),Score1Size,4.0);

    ScoreArea* ScoreZone1 = static_cast<ScoreArea*>( AreaEffectMan->CreateAreaEffect("ScoreArea","ScoreArea1",ScoreZone1Params) );
    ScoreZone1->GetGhostProxy()->SetCollisionShape(ScoreArea1Shape);
    ScoreZone1->GetEntityProxy()->SetMesh(ScoreArea1Mesh);
    ScoreZone1->SetLocation(-10.5,-100.0,0.0);
    ScoreZone1->AddToWorld();

    Vector3 Score2Size(55,48,15);
    NameValuePairMap ScoreZone2Params;
    ScoreZone2Params["SizeX"] = StringTools::ConvertToString(Score2Size.X);
    ScoreZone2Params["SizeY"] = StringTools::ConvertToString(Score2Size.Y);
    ScoreZone2Params["SizeZ"] = StringTools::ConvertToString(Score2Size.Z);
    Physics::CollisionShape* ScoreArea2Shape = new Physics::BoxCollisionShape("ScoreArea2Shape",Score2Size);
    Graphics::Mesh* ScoreArea2Mesh = MeshMan->CreateBoxCornerMesh("ScoreArea2Mesh",ColourValue(0.2,0.2,0.8,0.8),Score2Size,4.0);

    ScoreArea* ScoreZone2 = static_cast<ScoreArea*>( AreaEffectMan->CreateAreaEffect("ScoreArea","ScoreArea2",ScoreZone2Params) );
    ScoreZone2->GetGhostProxy()->SetCollisionShape(ScoreArea2Shape);
    ScoreZone2->GetEntityProxy()->SetMesh(ScoreArea2Mesh);
    ScoreZone2->SetLocation(-170,100,0);
    ScoreZone2->AddToWorld();// */
}

void LevelLoader::LoadJustice()
{
    // Get managers
    ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
    Physics::CollisionShapeManager* CShapeMan = Physics::CollisionShapeManager::GetSingletonPtr();
    Graphics::MeshManager* MeshMan = Graphics::MeshManager::GetSingletonPtr();
    Physics::PhysicsManager* PhysMan = this->TheEntresol->GetPhysicsManager();
    Graphics::SceneManager* SceneMan = this->TheEntresol->GetSceneManager();
    AreaEffectManager* AreaEffectMan = this->TheEntresol->GetAreaEffectManager();
    DebrisManager* DebrisMan = this->TheEntresol->GetDebrisManager();

    // Init Resources
    String CommonGroup("Common");
    String JusticeGroup("Justice");
    String datadir = "Levels/";
    ResourceMan->AddAssetLocation(datadir+"Justice.lvl", AT_Zip, JusticeGroup, false);
    ResourceMan->InitAssetGroup(JusticeGroup);

    // Scoring and Shop Setup
    LevelScorer* Scorer = CatchApp::GetCatchAppPointer()->GetLevelScorer();
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Scorer->SetLevelTargetTime(30);
    Shop->SetLevelCash(100);

    // Camera Setup
	Graphics::CameraProxy* DefCamera = this->TheEntresol->GetCameraManager()->GetCamera(0);
	DefCamera->SetLocation(Vector3(0,0,425));
	DefCamera->LookAt(Vector3(0,0,0));

	// Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Graphics::LightProxy* DLight = SceneMan->CreateLightProxy(Graphics::LT_Directional);
    Vector3 Loc(-150,100,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));
    DLight->AddToWorld();

    // Physics Setup
    PhysMan->SetWorldGravity(Vector3(0,-1000,0));

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,120),Vector3(15,-10,120),Vector3(0,10,120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Setup and Create the shapes that will be used.
    CShapeMan->LoadAllShapesFromBinaryFile("Justice.bullet",JusticeGroup);

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
    RigidDebris* LadyJustice = DebrisMan->CreateRigidDebris("LadyJustice",0);
    LadyJustice->GetRigidProxy()->SetCollisionShape( CShapeMan->GenerateStaticTriMesh("LadyShape","lady.mesh",JusticeGroup) );
    LadyJustice->GetEntityProxy()->SetMesh("lady.mesh",JusticeGroup);
    LadyJustice->SetLocation(12,-60,-103.35);
    LadyJustice->AddToWorld();

    // Create the scale object
    Real ScaleMass = 50.0;
    RigidDebris* JusticeScale = DebrisMan->CreateRigidDebris("JusticeScale",ScaleMass);
    JusticeScale->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Scale") );
    JusticeScale->GetRigidProxy()->SetDamping(0.5,0.80);
    JusticeScale->GetEntityProxy()->SetMesh("scale.mesh",JusticeGroup);
    JusticeScale->SetLocation(0,37.4,0);
    JusticeScale->AddToWorld();

    // Create the union objects
    Real UnionMass = 5.0;
    RigidDebris* Union1 = DebrisMan->CreateRigidDebris("Union1",UnionMass);
    Union1->GetRigidProxy()->SetCollisionShape(CShapeMan->GetShape("Union"));
    Union1->GetEntityProxy()->SetMesh("union.mesh",JusticeGroup);
    Union1->SetLocation(-112,0.2,-16.4);
    Union1->AddToWorld();
    RigidDebris* Union2 = DebrisMan->CreateRigidDebris("Union2",UnionMass);
    Union2->GetRigidProxy()->SetCollisionShape(CShapeMan->GetShape("Union"));
    Union2->GetEntityProxy()->SetMesh("union.mesh",JusticeGroup);
    Union2->SetLocation(-112,0.2,16.4);
    Union2->AddToWorld();
    RigidDebris* Union3 = DebrisMan->CreateRigidDebris("Union3",UnionMass);
    Union3->GetRigidProxy()->SetCollisionShape(CShapeMan->GetShape("Union"));
    Union3->GetEntityProxy()->SetMesh("union.mesh",JusticeGroup);
    Union3->SetLocation(112,0.2,-16.4);
    Union3->AddToWorld();
    RigidDebris* Union4 = DebrisMan->CreateRigidDebris("Union4",UnionMass);
    Union4->GetRigidProxy()->SetCollisionShape(CShapeMan->GetShape("Union"));
    Union4->GetEntityProxy()->SetMesh("union.mesh",JusticeGroup);
    Union4->SetLocation(112,0.2,16.4);
    Union4->AddToWorld();

    // Create the trays
    Real TrayMass = 25.0;
    RigidDebris* Tray1 = DebrisMan->CreateRigidDebris("Tray1",TrayMass);
    Tray1->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray1->GetRigidProxy()->SetLinearMovementFactor(Vector3(1,1,0));
    Tray1->GetRigidProxy()->SetAngularMovementFactor(Vector3(1,0,1));
    Tray1->GetRigidProxy()->SetDamping(0.0,0.3);
    Tray1->GetEntityProxy()->SetMesh("tray.mesh",JusticeGroup);
    Tray1->SetLocation(-112,-35,0);
    Tray1->AddToWorld();
    RigidDebris* Tray2 = DebrisMan->CreateRigidDebris("Tray2",TrayMass);
    Tray2->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray2->GetRigidProxy()->SetLinearMovementFactor(Vector3(1,1,0));
    Tray2->GetRigidProxy()->SetAngularMovementFactor(Vector3(1,0,1));
    Tray2->GetRigidProxy()->SetDamping(0.0,0.3);
    Tray2->GetEntityProxy()->SetMesh("tray.mesh",JusticeGroup);
    Tray2->SetLocation(112,-35,0);
    Tray2->AddToWorld();

    // Create the series of constraints for connecting all the pieces of the scale together
    // Starting with creating the anchor for the scale
    Physics::HingeConstraint* ScaleAnchor = new Physics::HingeConstraint(LadyJustice->GetRigidProxy(),JusticeScale->GetRigidProxy(),Vector3(-12,127.4,103.35),Vector3(0,30,0),Vector3(0,0,1),Vector3(0,0,1),false);
    ScaleAnchor->SetLimit( -(MathTools::GetPi() * 0.20),(MathTools::GetPi() * 0.20) );
    PhysMan->AddConstraint(ScaleAnchor,true);

    // Original X distance from pivot on scale is 112.7, but space was needed to prevent collsions.
    // Create the scale-to-union constraints
    Physics::Point2PointConstraint* U1S = new Physics::Point2PointConstraint(JusticeScale->GetRigidProxy(),Union1->GetRigidProxy(),Vector3(-112.0,-20,-16.4),Vector3(0,17.2,0));//58,47.4,0 // -54,27.4,-16.4
    PhysMan->AddConstraint(U1S,true);
    Physics::Point2PointConstraint* U2S = new Physics::Point2PointConstraint(JusticeScale->GetRigidProxy(),Union2->GetRigidProxy(),Vector3(-112.0,-20,16.4),Vector3(0,17.2,0));//58,47.4,0 // -54,27.4,16.4
    PhysMan->AddConstraint(U2S,true);
    Physics::Point2PointConstraint* U3S = new Physics::Point2PointConstraint(JusticeScale->GetRigidProxy(),Union3->GetRigidProxy(),Vector3(112.0,-20,-16.4),Vector3(0,17.2,0));//58,47.4,0 // 170,27.4,-16.4
    PhysMan->AddConstraint(U3S,true);
    Physics::Point2PointConstraint* U4S = new Physics::Point2PointConstraint(JusticeScale->GetRigidProxy(),Union4->GetRigidProxy(),Vector3(112.0,-20,16.4),Vector3(0,17.2,0));//58,47.4,0 // 170,27.4,16.4
    PhysMan->AddConstraint(U4S,true);

    // Create the union-to-tray constraints
    Physics::Point2PointConstraint* U1T = new Physics::Point2PointConstraint(Union1->GetRigidProxy(),Tray1->GetRigidProxy(),Vector3(0,-17.2,0),Vector3(0,18,-16.4));// -54,10.2,-16.4 // -54,-25,0
    PhysMan->AddConstraint(U1T,true);
    Physics::Point2PointConstraint* U2T = new Physics::Point2PointConstraint(Union2->GetRigidProxy(),Tray1->GetRigidProxy(),Vector3(0,-17.2,0),Vector3(0,18,16.4));// -54,10.2,16.4 // -54,-25,0
    PhysMan->AddConstraint(U2T,true);
    Physics::Point2PointConstraint* U3T = new Physics::Point2PointConstraint(Union3->GetRigidProxy(),Tray2->GetRigidProxy(),Vector3(0,-17.2,0),Vector3(0,18,-16.4));// 170,10.2,-16.4 // 170,-25,0
    PhysMan->AddConstraint(U3T,true);
    Physics::Point2PointConstraint* U4T = new Physics::Point2PointConstraint(Union4->GetRigidProxy(),Tray2->GetRigidProxy(),Vector3(0,-17.2,0),Vector3(0,18,16.4));// 170,10.2,16.4 // 170,-25,0
    PhysMan->AddConstraint(U4T,true);

    // Create some throwable objects
    //ThrowableData* ClayData = ThrowableGenerator::GetThrowableData("Clay");
    //ThrowableData* LeadData = ThrowableGenerator::GetThrowableData("Lead");
    ThrowableData* UraniumData = ThrowableGenerator::GetThrowableData("Uranium");
    //Physics::CollisionShape* ClayCS = CShapeMan->GenerateConvexHull("ClayCS",ClayData->MeshName,CommonGroup);
    //Physics::CollisionShape* LeadCS = CShapeMan->GenerateConvexHull("LeadCS",LeadData->MeshName,CommonGroup);
    Physics::CollisionShape* UraniumCS = CShapeMan->GenerateConvexHull("UraniumCS",UraniumData->MeshName,CommonGroup);

    RigidDebris* Uranium1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Uranium") );
    Uranium1->GetRigidProxy()->SetCollisionShape(UraniumCS);
    Uranium1->SetLocation(-155,110,0);
    Uranium1->AddToWorld();
    GameApp->AddThrowable(Uranium1);
    RigidDebris* Uranium2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Uranium") );
    Uranium2->GetRigidProxy()->SetCollisionShape(UraniumCS);
    Uranium2->SetLocation(-185,110,0);
    Uranium2->AddToWorld();
    GameApp->AddThrowable(Uranium2);

    // Create the zones
    Vector3 StartSize(60,30,20);
    NameValuePairMap StartZoneParams;
    StartZoneParams["SizeX"] = StringTools::ConvertToString(StartSize.X);
    StartZoneParams["SizeY"] = StringTools::ConvertToString(StartSize.Y);
    StartZoneParams["SizeZ"] = StringTools::ConvertToString(StartSize.Z);
    Physics::CollisionShape* StartAreaShape = new Physics::BoxCollisionShape("StartArea1Shape",StartSize);
    Graphics::Mesh* StartAreaMesh = MeshMan->CreateBoxCornerMesh("StartAreaMesh",ColourValue(0.1,0.8,0.1,0.8),StartSize,4.0);

    StartArea* StartZone1 = static_cast<StartArea*>( AreaEffectMan->CreateAreaEffect("StartArea","StartArea1",StartZoneParams) );
    StartZone1->GetGhostProxy()->SetCollisionShape(StartAreaShape);
    StartZone1->GetEntityProxy()->SetMesh(StartAreaMesh);
    StartZone1->SetParticleMinimumTimeToLive(3.0);
    StartZone1->SetParticleMaximumTimeToLive(4.0);
    StartZone1->SetLocation(-160,120,0);
    StartZone1->AddToWorld();
    StartArea* StartZone2 = static_cast<StartArea*>( AreaEffectMan->CreateAreaEffect("StartArea","StartArea2",StartZoneParams) );
    StartZone2->GetGhostProxy()->SetCollisionShape(StartAreaShape);
    StartZone2->GetEntityProxy()->SetMesh(StartAreaMesh);
    StartZone2->SetParticleMinimumTimeToLive(3.0);
    StartZone2->SetParticleMaximumTimeToLive(4.0);
    StartZone2->SetLocation(160,120,0);
    StartZone2->AddToWorld();// */

    Vector3 ScoreSize(52,25,18);
    NameValuePairMap ScoreZoneParams;
    ScoreZoneParams["SizeX"] = StringTools::ConvertToString(ScoreSize.X);
    ScoreZoneParams["SizeY"] = StringTools::ConvertToString(ScoreSize.Y);
    ScoreZoneParams["SizeZ"] = StringTools::ConvertToString(ScoreSize.Z);
    Physics::CollisionShape* ScoreAreaShape = new Physics::BoxCollisionShape("ScoreAreaShape",ScoreSize);
    Graphics::Mesh* ScoreAreaMesh = MeshMan->CreateBoxCornerMesh("ScoreAreaMesh",ColourValue(0.2,0.2,0.8,0.8),ScoreSize,4.0);

    ScoreArea* ScoreZone1 = static_cast<ScoreArea*>( AreaEffectMan->CreateAreaEffect("ScoreArea","ScoreArea1",ScoreZoneParams) );
    ScoreZone1->GetGhostProxy()->SetCollisionShape(ScoreAreaShape);
    ScoreZone1->GetEntityProxy()->SetMesh(ScoreAreaMesh);
    ScoreZone1->SetLocation(-113,-40,0);
    ScoreZone1->AddToWorld();// */
    ScoreArea* ScoreZone2 = static_cast<ScoreArea*>( AreaEffectMan->CreateAreaEffect("ScoreArea","ScoreArea2",ScoreZoneParams) );
    ScoreZone2->GetGhostProxy()->SetCollisionShape(ScoreAreaShape);
    ScoreZone2->GetEntityProxy()->SetMesh(ScoreAreaMesh);
    ScoreZone2->SetLocation(113,-40,0);
    ScoreZone2->AddToWorld();// */
}

void LevelLoader::LoadRollers()
{
    // Get managers
    ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
    Physics::CollisionShapeManager* CShapeMan = Physics::CollisionShapeManager::GetSingletonPtr();
    Graphics::MeshManager* MeshMan = Graphics::MeshManager::GetSingletonPtr();
    Physics::PhysicsManager* PhysMan = this->TheEntresol->GetPhysicsManager();
    Graphics::SceneManager* SceneMan = this->TheEntresol->GetSceneManager();
    AreaEffectManager* AreaEffectMan = this->TheEntresol->GetAreaEffectManager();
    DebrisManager* DebrisMan = this->TheEntresol->GetDebrisManager();

    // Init Resources
    String CommonGroup("Common");
    String RollersGroup("Rollers");
    String datadir = "Levels/";
    ResourceMan->AddAssetLocation(datadir+"Rollers.lvl", AT_Zip, RollersGroup, false);
    ResourceMan->InitAssetGroup(RollersGroup);

    // Scoring and Shop Setup
    LevelScorer* Scorer = CatchApp::GetCatchAppPointer()->GetLevelScorer();
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Scorer->SetLevelTargetTime(30);
    Shop->SetLevelCash(100);

    // Camera Setup
	Graphics::CameraProxy* DefCamera = this->TheEntresol->GetCameraManager()->GetCamera(0);
	DefCamera->SetLocation(Vector3(0,0,425));
	DefCamera->LookAt(Vector3(0,0,0));

	// Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Graphics::LightProxy* DLight = SceneMan->CreateLightProxy(Graphics::LT_Directional);
    Vector3 Loc(-150,100,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));
    DLight->AddToWorld();

    // Physics Setup
    PhysMan->SetWorldGravity(Vector3(0,-1000,0));

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,120),Vector3(15,-10,120),Vector3(0,10,120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Setup and Create the shapes that will be used.

    // Create the basic terrain that will be used
    RigidDebris* RollersFrame = DebrisMan->CreateRigidDebris("RollersFrame",0);
    RollersFrame->GetRigidProxy()->SetCollisionShape( CShapeMan->GenerateStaticTriMesh("FrameShape","frame.mesh",RollersGroup) );
    RollersFrame->GetRigidProxy()->SetFriction(1.0);
    RollersFrame->GetEntityProxy()->SetMesh("frame.mesh",RollersGroup);
    RollersFrame->SetLocation(0,-25,0);
    RollersFrame->AddToWorld();

    // Create the individual Rollers and their constraints
    std::vector<Physics::HingeConstraint*> TheRollers;
    Real XStride = 14.5;
    Physics::CollisionShape* RollerShape = new Physics::CylinderCollisionShape("RollerShape",6.51,3.36,Vector3::Unit_Z());
    for( Whole X = 0 ; X < 7 ; ++X )
    {
        Vector3 Location(112.4+(XStride*X),-20,0);
        std::stringstream Namestream;
        Namestream << "Roller" << (X+1);
        RigidDebris* Roller = DebrisMan->CreateRigidDebris(Namestream.str(),10);
        Roller->GetRigidProxy()->SetCollisionShape(RollerShape);
        Roller->GetRigidProxy()->SetFriction(1.0);
        Roller->GetEntityProxy()->SetMesh("rubberroller.mesh",RollersGroup);
        Roller->SetLocation(Location);
        Roller->AddToWorld();

        Physics::HingeConstraint* RollerAnchor = new Physics::HingeConstraint(Roller->GetRigidProxy(),Vector3(0,0,0),Vector3(0,0,1),true);
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
    Physics::CollisionShape* PyriteCS = CShapeMan->GenerateConvexHull("PyriteCS",PyriteData->MeshName,CommonGroup);
    Physics::CollisionShape* LeadCS = CShapeMan->GenerateConvexHull("LesdCS",LeadData->MeshName,CommonGroup);

    RigidDebris* Pyrite1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Pyrite") );
    Pyrite1->GetRigidProxy()->SetCollisionShape(PyriteCS);
    Pyrite1->SetLocation(-160,80,0);
    Pyrite1->AddToWorld();
    GameApp->AddThrowable(Pyrite1);
    RigidDebris* Pyrite2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Pyrite") );
    Pyrite2->GetRigidProxy()->SetCollisionShape(PyriteCS);
    Pyrite2->SetLocation(-120,80,0);
    Pyrite2->AddToWorld();
    GameApp->AddThrowable(Pyrite2);// */
    RigidDebris* Lead1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Lead") );
    Lead1->GetRigidProxy()->SetCollisionShape(LeadCS);
    Lead1->SetLocation(-160,30,0);
    Lead1->AddToWorld();
    GameApp->AddThrowable(Lead1);
    RigidDebris* Lead2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Lead") );
    Lead2->GetRigidProxy()->SetCollisionShape(LeadCS);
    Lead2->SetLocation(-120,30,0);
    Lead2->AddToWorld();
    GameApp->AddThrowable(Lead2);// */

    // Create the zones
    Vector3 StartSize(50,50,20);
    NameValuePairMap StartZoneParams;
    StartZoneParams["SizeX"] = StringTools::ConvertToString(StartSize.X);
    StartZoneParams["SizeY"] = StringTools::ConvertToString(StartSize.Y);
    StartZoneParams["SizeZ"] = StringTools::ConvertToString(StartSize.Z);

    StartArea* StartZone = static_cast<StartArea*>( AreaEffectMan->CreateAreaEffect("StartArea","ScoreArea1",StartZoneParams) );
    StartZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("StartAreaShape",StartSize) );
    StartZone->GetEntityProxy()->SetMesh( MeshMan->CreateBoxCornerMesh("StartAreaMesh",ColourValue(0.1,0.8,0.1,0.8),StartSize,4.0) );
    StartZone->SetParticleMinimumTimeToLive(5.0);
    StartZone->SetParticleMaximumTimeToLive(6.0);
    StartZone->SetLocation(-140,60,0);
    StartZone->AddToWorld();// */

    Vector3 ScoreSize(70,40,20);
    NameValuePairMap ScoreZoneParams;
    ScoreZoneParams["SizeX"] = StringTools::ConvertToString(ScoreSize.X);
    ScoreZoneParams["SizeY"] = StringTools::ConvertToString(ScoreSize.Y);
    ScoreZoneParams["SizeZ"] = StringTools::ConvertToString(ScoreSize.Z);

    ScoreArea* ScoreZone = static_cast<ScoreArea*>( AreaEffectMan->CreateAreaEffect("ScoreArea","ScoreArea1",ScoreZoneParams) );
    ScoreZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("ScoreAreaShape",ScoreSize) );
    ScoreZone->GetEntityProxy()->SetMesh(MeshMan->CreateBoxCornerMesh("ScoreAreaMesh",ColourValue(0.2,0.2,0.8,0.8),ScoreSize,4.0));
    ScoreZone->SetLocation(140,20,0);
    ScoreZone->AddToWorld();// */
}

void LevelLoader::LoadJustBounce()
{
    // Get managers
    ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
    Physics::CollisionShapeManager* CShapeMan = Physics::CollisionShapeManager::GetSingletonPtr();
    Graphics::MeshManager* MeshMan = Graphics::MeshManager::GetSingletonPtr();
    Physics::PhysicsManager* PhysMan = this->TheEntresol->GetPhysicsManager();
    Graphics::SceneManager* SceneMan = this->TheEntresol->GetSceneManager();
    AreaEffectManager* AreaEffectMan = this->TheEntresol->GetAreaEffectManager();
    DebrisManager* DebrisMan = this->TheEntresol->GetDebrisManager();

    // Init Resources
    String CommonGroup("Common");
    String JustBounceGroup("JustBounce");
    String datadir = "Levels/";
    ResourceMan->AddAssetLocation(datadir+"JustBounce.lvl", AT_Zip, JustBounceGroup, false);
    ResourceMan->InitAssetGroup(JustBounceGroup);

    // Scoring and Shop Setup
    LevelScorer* Scorer = CatchApp::GetCatchAppPointer()->GetLevelScorer();
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Scorer->SetLevelTargetTime(30);
    Shop->SetLevelCash(100);

    // Camera Setup
	Graphics::CameraProxy* DefCamera = this->TheEntresol->GetCameraManager()->GetCamera(0);
	DefCamera->SetLocation(Vector3(0,0,425));
	DefCamera->LookAt(Vector3(0,0,0));

	// Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Graphics::LightProxy* DLight = SceneMan->CreateLightProxy(Graphics::LT_Directional);
    Vector3 Loc(-150,100,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));
    DLight->AddToWorld();

    // Physics Setup
    PhysMan->SetWorldGravity(Vector3(0,-1000,0));

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,120),Vector3(15,-10,120),Vector3(0,10,120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Setup and Create the shapes that will be used.
    CShapeMan->LoadAllShapesFromBinaryFile("JustBounce.bullet",JustBounceGroup);

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
    RigidDebris* TopWall = DebrisMan->CreateRigidDebris("TopWall",0);
    TopWall->GetRigidProxy()->SetCollisionShape( CShapeMan->GenerateStaticTriMesh("WallShape","wall.mesh",JustBounceGroup) );
    TopWall->GetRigidProxy()->SetRestitution(0.5);
    TopWall->GetEntityProxy()->SetMesh("wall.mesh",JustBounceGroup);
    TopWall->SetLocation(0,85,0);
    TopWall->AddToWorld();

    RigidDebris* Basket = DebrisMan->CreateRigidDebris("Basket",0);
    Basket->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Basket") );
    Basket->GetEntityProxy()->SetMesh("basket.mesh",JustBounceGroup);
    Basket->SetLocation(165,-25,0);
    Basket->AddToWorld();

    RigidDebris* ElasticFloor = DebrisMan->CreateRigidDebris("ElasticFloor",0);
    ElasticFloor->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("ElasticBed") );//CShapeMan->GenerateStaticTriMesh("ElasticShape","elastic_bed.mesh",JustBounceGroup));
    ElasticFloor->GetRigidProxy()->SetRestitution(1.0);
    //ElasticFloor->GetRigidProxy()->SetFriction(0.4);
    ElasticFloor->GetEntityProxy()->SetMesh("elastic_bed.mesh",JustBounceGroup);
    ElasticFloor->SetLocation(-20,-70,0);
    ElasticFloor->AddToWorld();

    RigidDebris* ElasticCase = DebrisMan->CreateRigidDebris("ElasticCase",0);
    ElasticCase->GetRigidProxy()->SetCollisionShape( CShapeMan->GenerateStaticTriMesh("CaseShape","case.mesh",JustBounceGroup) );
    ElasticCase->GetEntityProxy()->SetMesh("case.mesh",JustBounceGroup);
    ElasticCase->SetLocation(-20,-71,0);
    ElasticCase->AddToWorld();

    // Create some throwable objects
    //ThrowableData* RubberData = ThrowableGenerator::GetThrowableData("Rubber");
    ThrowableData* WoodData = ThrowableGenerator::GetThrowableData("Wood");
    Physics::CollisionShape* RubberCS = new Physics::SphereCollisionShape("RubberCS",11.4);//CShapeMan->GenerateConvexHull("RubberCS",RubberData->MeshName,CommonGroup);
    Physics::CollisionShape* WoodCS = CShapeMan->GenerateConvexHull("WoodCS",WoodData->MeshName,CommonGroup);// */

    RigidDebris* Rubber1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Rubber") );
    Rubber1->GetRigidProxy()->SetCollisionShape(RubberCS);
    Rubber1->SetLocation(-185,110,0);
    Rubber1->AddToWorld();
    GameApp->AddThrowable(Rubber1);
    RigidDebris* Rubber2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Rubber") );
    Rubber2->GetRigidProxy()->SetCollisionShape(RubberCS);
    Rubber2->SetLocation(-155,110,0);
    Rubber2->AddToWorld();
    GameApp->AddThrowable(Rubber2);// */
    RigidDebris* Wood1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Wood") );
    Wood1->GetRigidProxy()->SetCollisionShape(WoodCS);
    Wood1->SetLocation(-185,70,0);
    Wood1->AddToWorld();
    GameApp->AddThrowable(Wood1);
    RigidDebris* Wood2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Wood") );
    Wood2->GetRigidProxy()->SetCollisionShape(WoodCS);
    Wood2->SetLocation(-155,70,0);
    Wood2->AddToWorld();
    GameApp->AddThrowable(Wood2);// */


    // Create the zones
    Vector3 StartSize(50,50,20);
    NameValuePairMap StartZoneParams;
    StartZoneParams["SizeX"] = StringTools::ConvertToString(StartSize.X);
    StartZoneParams["SizeY"] = StringTools::ConvertToString(StartSize.Y);
    StartZoneParams["SizeZ"] = StringTools::ConvertToString(StartSize.Z);

    StartArea* StartZone = static_cast<StartArea*>( AreaEffectMan->CreateAreaEffect("StartArea","StartArea1",StartZoneParams) );
    StartZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("StartAreaShape",StartSize) );
    StartZone->GetEntityProxy()->SetMesh( MeshMan->CreateBoxCornerMesh("StartAreaMesh",ColourValue(0.1,0.8,0.1,0.8),StartSize,4.0) );
    StartZone->SetParticleMinimumTimeToLive(5.0);
    StartZone->SetParticleMaximumTimeToLive(6.0);
    StartZone->SetLocation(-170,90,0);
    StartZone->AddToWorld();// */

    Vector3 ScoreSize(42,50,35);
    NameValuePairMap ScoreZoneParams;
    ScoreZoneParams["SizeX"] = StringTools::ConvertToString(ScoreSize.X);
    ScoreZoneParams["SizeY"] = StringTools::ConvertToString(ScoreSize.Y);
    ScoreZoneParams["SizeZ"] = StringTools::ConvertToString(ScoreSize.Z);

    ScoreArea* ScoreZone = static_cast<ScoreArea*>( AreaEffectMan->CreateAreaEffect("ScoreArea","ScoreArea1",ScoreZoneParams) );
    ScoreZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("ScoreAreaShape",ScoreSize) );
    ScoreZone->GetEntityProxy()->SetMesh(MeshMan->CreateBoxCornerMesh("ScoreAreaMesh",ColourValue(0.2,0.2,0.8,0.8),ScoreSize,4.0));
    ScoreZone->SetLocation(158,-25,0);
    ScoreZone->AddToWorld();// */
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
