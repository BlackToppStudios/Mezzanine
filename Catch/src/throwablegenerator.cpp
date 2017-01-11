#ifndef _throwablegenerator_cpp
#define _throwablegenerator_cpp

#include "throwablegenerator.h"
#include "catchapp.h"
#include <cassert>

ThrowableGenerator* ThrowableGenerator::TheRealThrowableGenerator = 0;
std::vector<ThrowableData*> ThrowableGenerator::Throwables;

ThrowableGenerator::ThrowableGenerator()
{
    assert(0==ThrowableGenerator::TheRealThrowableGenerator);
    TheRealThrowableGenerator = this;
}

ThrowableGenerator::~ThrowableGenerator()
{

}

ThrowableGenerator* ThrowableGenerator::GetThrowableGenerator()
{
    return TheRealThrowableGenerator;
}

void ThrowableGenerator::ParseThrowables(const String& FileName)
{
    /// @todo Currently this has the values hardcoded, but ideally we could add them to an XML file to be parsed.
    /// Also note that some info, such as the mass is something that can and will be changed in some levels from
    /// the base value, all that we need to provide here is that base value.
    ThrowableData* ClayData = new ThrowableData();
    ClayData->ScoreValue = 10;
    ClayData->ThrowableCount = 0;
    ClayData->ThrowableName = "Clay";
    ClayData->MeshName = "clay.mesh";
    ClayData->GroupName = "Common";
    ClayData->Mass = 2.25;//1.74
    ClayData->Friction = 0.6;
    ClayData->Restitution = 0.15;
    Throwables.push_back(ClayData);

    ThrowableData* GoldData = new ThrowableData();
    GoldData->ScoreValue = 100;
    GoldData->ThrowableCount = 0;
    GoldData->ThrowableName = "Gold";
    GoldData->MeshName = "gold.mesh";
    GoldData->GroupName = "Common";
    GoldData->Mass = 12.5;//17.8
    GoldData->Friction = 0.7;
    GoldData->Restitution = 0.1;
    Throwables.push_back(GoldData);

    ThrowableData* LeadData = new ThrowableData();
    LeadData->ScoreValue = 30;
    LeadData->ThrowableCount = 0;
    LeadData->ThrowableName = "Lead";
    LeadData->MeshName = "lead.mesh";
    LeadData->GroupName = "Common";
    LeadData->Mass = 14.0;//11.3
    LeadData->Friction = 0.43;//0.43
    LeadData->Restitution = 0.1;
    Throwables.push_back(LeadData);

    ThrowableData* PyriteData = new ThrowableData();
    PyriteData->ScoreValue = 50;
    PyriteData->ThrowableCount = 0;
    PyriteData->ThrowableName = "Pyrite";
    PyriteData->MeshName = "pyrite.mesh";
    PyriteData->GroupName = "Common";
    PyriteData->Mass = 7.0;//
    PyriteData->Friction = MathTools::Sqrt(0.8);
    PyriteData->Restitution = 0.1;
    Throwables.push_back(PyriteData);

    ThrowableData* RubberData = new ThrowableData();
    RubberData->ScoreValue = 30;
    RubberData->ThrowableCount = 0;
    RubberData->ThrowableName = "Rubber";
    RubberData->MeshName = "rubber.mesh";
    RubberData->GroupName = "Common";
    RubberData->Mass = 3.0;//1.52
    RubberData->Friction = 0.6;// 0.5 - 0.9
    RubberData->Restitution = 0.8;
    Throwables.push_back(RubberData);

    ThrowableData* StyrofoamData = new ThrowableData();
    StyrofoamData->ScoreValue = 10;
    StyrofoamData->ThrowableCount = 0;
    StyrofoamData->ThrowableName = "Styrofoam";
    StyrofoamData->MeshName = "styrofoam.mesh";
    StyrofoamData->GroupName = "Common";
    StyrofoamData->Mass = 0.15;//0.1
    StyrofoamData->Friction = 0.55;
    StyrofoamData->Restitution = 0.2;
    Throwables.push_back(StyrofoamData);

    ThrowableData* UraniumData = new ThrowableData();
    UraniumData->ScoreValue = 100;
    UraniumData->ThrowableCount = 0;
    UraniumData->ThrowableName = "Uranium";
    UraniumData->MeshName = "uranium.mesh";
    UraniumData->GroupName = "Common";
    UraniumData->Mass = 15.5;//18.74
    UraniumData->Friction = 0.6;
    UraniumData->Restitution = 0.1;
    Throwables.push_back(UraniumData);

    ThrowableData* WoodData = new ThrowableData();
    WoodData->ScoreValue = 20;
    WoodData->ThrowableCount = 0;
    WoodData->ThrowableName = "Wood";
    WoodData->MeshName = "woodpin.mesh";
    WoodData->GroupName = "Common";
    WoodData->Mass = 4.5;//
    WoodData->Friction = 0.3;// 0.25 - 0.5
    WoodData->Restitution = 0.15;
    Throwables.push_back(WoodData);

    LevelScorer* Scorer = CatchApp::GetCatchAppPointer()->GetLevelScorer();
    for( Whole X = 0 ; X < Throwables.size() ; ++X )
    {
        Scorer->SetThrowableScore(Throwables[X]->ThrowableName,Throwables[X]->ScoreValue);
    }
}

ThrowableData* ThrowableGenerator::GetThrowableData(const String& Throwable)
{
    for(Whole X = 0 ; X < Throwables.size() ; X++ )
    {
        if(Throwable == Throwables[X]->ThrowableName)
            return Throwables[X];
    }
    return NULL;
}

Debris* ThrowableGenerator::CreateThrowable(const String& Throwable)
{
    DebrisManager* DebMan = static_cast<DebrisManager*>( Entresol::GetSingletonPtr()->GetWorld(0)->GetManager(ManagerBase::MT_DebrisManager) );
    ThrowableData* ToBeCreated = GetThrowableData(Throwable);
    if(!ToBeCreated)
        return NULL;
    std::stringstream NameGen;
    (ToBeCreated->ThrowableCount)++;
    NameGen << ToBeCreated->ThrowableName << ToBeCreated->ThrowableCount;

    RigidDebris* Created = DebMan->CreateRigidDebris(NameGen.str(),ToBeCreated->Mass);
    Created->GetRigidProxy()->SetLinearMovementFactor(Vector3(1,1,0));
    Created->GetRigidProxy()->SetFriction(ToBeCreated->Friction);
    Created->GetRigidProxy()->SetRestitution(ToBeCreated->Restitution);
    // ©reated->GetRigidProxy()->SetActivationState(Physics::WOAS_DisableDeactivation);
    Created->GetEntityProxy()->SetMesh(ToBeCreated->MeshName,ToBeCreated->GroupName);
    Created->SetOrientation(Quaternion(MathTools::GetPi(),Vector3(0,1,0)));

    /*if("Rubber"==Throwable)
    {
        //generate sphere shape
    }else{
        //generate convex hull
        //probably make more if's for cylinders and such
    }// */
    return Created;
}

#endif
