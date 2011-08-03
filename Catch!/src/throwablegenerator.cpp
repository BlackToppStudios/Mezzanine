#ifndef _throwablegenerator_cpp
#define _throwablegenerator_cpp

#include "throwablegenerator.h"
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
    ClayData->ThrowableCount = 0;
    ClayData->ThrowableName = "Clay";
    ClayData->MeshName = "clay.mesh";
    ClayData->GroupName = "Common";
    ClayData->Mass = 2.25;//1.74
    Throwables.push_back(ClayData);

    ThrowableData* GoldData = new ThrowableData();
    GoldData->ThrowableCount = 0;
    GoldData->ThrowableName = "Gold";
    GoldData->MeshName = "gold.mesh";
    GoldData->GroupName = "Common";
    GoldData->Mass = 12.5;//17.8
    Throwables.push_back(GoldData);

    ThrowableData* LeadData = new ThrowableData();
    LeadData->ThrowableCount = 0;
    LeadData->ThrowableName = "Lead";
    LeadData->MeshName = "lead.mesh";
    LeadData->GroupName = "Common";
    LeadData->Mass = 14.0;//11.3
    Throwables.push_back(LeadData);

    ThrowableData* PyriteData = new ThrowableData();
    PyriteData->ThrowableCount = 0;
    PyriteData->ThrowableName = "Pyrite";
    PyriteData->MeshName = "pyrite.mesh";
    PyriteData->GroupName = "Common";
    PyriteData->Mass = 7.0;//
    Throwables.push_back(PyriteData);

    ThrowableData* RubberData = new ThrowableData();
    RubberData->ThrowableCount = 0;
    RubberData->ThrowableName = "Rubber";
    RubberData->MeshName = "rubber.mesh";
    RubberData->GroupName = "Common";
    RubberData->Mass = 3.0;//1.52
    Throwables.push_back(RubberData);

    ThrowableData* StyrofoamData = new ThrowableData();
    StyrofoamData->ThrowableCount = 0;
    StyrofoamData->ThrowableName = "Styrofoam";
    StyrofoamData->MeshName = "styrofoam.mesh";
    StyrofoamData->GroupName = "Common";
    StyrofoamData->Mass = 0.15;//0.1
    Throwables.push_back(StyrofoamData);

    ThrowableData* UraniumData = new ThrowableData();
    UraniumData->ThrowableCount = 0;
    UraniumData->ThrowableName = "Uranium";
    UraniumData->MeshName = "uranium.mesh";
    UraniumData->GroupName = "Common";
    UraniumData->Mass = 15.5;//18.74
    Throwables.push_back(UraniumData);

    ThrowableData* WoodData = new ThrowableData();
    WoodData->ThrowableCount = 0;
    WoodData->ThrowableName = "Wood";
    WoodData->MeshName = "woodpin.mesh";
    WoodData->GroupName = "Common";
    WoodData->Mass = 4.5;//18.74
    Throwables.push_back(UraniumData);
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

ActorRigid* ThrowableGenerator::CreateThrowable(const String& Throwable)
{
    ThrowableData* ToBeCreated = GetThrowableData(Throwable);
    if(!ToBeCreated)
        return NULL;
    std::stringstream NameGen;
    (ToBeCreated->ThrowableCount)++;
    NameGen << ToBeCreated->ThrowableName << ToBeCreated->ThrowableCount;
    ActorRigid* Created = new ActorRigid(ToBeCreated->Mass,NameGen.str(),ToBeCreated->MeshName,ToBeCreated->GroupName);
    Created->SetLinearMovementFactor(Vector3(1,1,0));
    Created->SetOrientation(Quaternion(MathTool::GetPi(),Vector3(0,1,0)));
    return Created;
}

#endif
