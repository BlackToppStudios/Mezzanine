#ifndef _levelzones_cpp
#define _levelzones_cpp

#include "levelzones.h"
#include "mezzanine.h"

LevelZone::LevelZone(const String& name, const Vector3& Location)
    : AreaEffect(name,Location)
{
    /*MeshManager* MeshMan = MeshManager::GetSingltonPtr();
    this->SetFieldShape(new BoxCollisionShape(Name+"Shape",Size));
    this->SetFieldMesh(MeshMan->CreateBoxMesh(Name+"Mesh",Colour,Size));// */
}

LevelZone::~LevelZone()
{
}

bool LevelZone::IsInside(ActorBase* Actor)
{
    for( std::list<ActorBase*>::iterator it = OverlappingActors.begin() ; it != OverlappingActors.end() ; it++ )
    {
        if( Actor == (*it) )
        {
            return true;
        }
    }
    return false;
}

bool LevelZone::IsEmpty()
{
    return OverlappingActors.empty();
}

Mezzanine::WorldAndSceneObjectType LevelZone::GetType() const
{
    return Mezzanine::WSO_AEUnknown;
}

StartArea::StartArea(const String& name, const Vector3& Location)
    : LevelZone(name,Location)
{
}

StartArea::~StartArea()
{
}

void StartArea::ApplyEffect()
{
    Vector3 Grav(0,0,0);
    ActorBase* Act = NULL;
    PhysicsManager* PhysMan = PhysicsManager::GetSingletonPtr();
    if(!AddedActors.empty())
    {
        for( std::vector<ActorBase*>::iterator it = AddedActors.begin() ; it != AddedActors.end() ; it++ )
        {
            Act = (*it);
            ((ActorBasePhysicsSettings*)Act->GetPhysicsSettings())->SetCollisionResponse(false);
            PhysMan->SetIndividualGravity(Act, Grav);
        }
    }
    if(!RemovedActors.empty())
    {
        for( std::vector<ActorBase*>::iterator it = RemovedActors.begin() ; it != RemovedActors.end() ; it++ )
        {
            Act = (*it);
            ((ActorBasePhysicsSettings*)Act->GetPhysicsSettings())->SetCollisionResponse(true);
            PhysMan->SetIndividualGravity(Act, PhysMan->GetGravity());
        }
    }
}

ScoreArea::ScoreArea(const String& name, const Vector3& Location)
    : LevelZone(name,Location),
      ScoreMultiplier(1.0)
{
}

ScoreArea::~ScoreArea()
{
}

void ScoreArea::ApplyEffect()
{
}

bool ScoreArea::AllObjectsAtRest()
{
    ActorBase* Act = NULL;
    if(OverlappingActors.empty())
        return false;
    for ( std::list<ActorBase*>::iterator it = OverlappingActors.begin() ; it != OverlappingActors.end() ; it++ )
    {
        Act = (*it);
        if( Act->GetPhysicsSettings()->IsActive() )
            return false;
    }
    return true;
}

void ScoreArea::SetScoreMultiplier(const Real& Multiplier)
{
    ScoreMultiplier = Multiplier;
}

Real ScoreArea::GetScoreMultiplier() const
{
    return ScoreMultiplier;
}

#endif
