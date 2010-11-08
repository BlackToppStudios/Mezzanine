#ifndef _startingarea_cpp
#define _startingarea_cpp

#include "startingarea.h"
#include "physgame.h"

StartingArea::StartingArea(const String &name, Vector3 Location) : AreaEffect(name, Location)
{
    Grav.X = 0;
    Grav.Y = 0;
    Grav.Z = 0;
}

StartingArea::~StartingArea()
{

}

void StartingArea::Initialize()
{
    ActorBase* Act = NULL;
    for( std::list<ActorBase*>::iterator it = OverlappingActors.begin() ; it != OverlappingActors.end() ; it++ )
    {
        Act = (*it);
        Act->DisableCollisionResponse();
        TheWorld->GetPhysicsManager()->SetIndividualGravity(Act, Grav);
    }
}

void StartingArea::ApplyEffect()
{
    ActorBase* Act = NULL;
    PhysicsManager* PhysMan = TheWorld->GetPhysicsManager();
    for( std::vector<ActorBase*>::iterator it = RemovedActors.begin() ; it != RemovedActors.end() ; it++ )
    {
        Act = (*it);
        Act->EnableCollisionResponse();
        PhysMan->SetIndividualGravity(Act, PhysMan->GetGravity());
    }
}

bool StartingArea::IsEmpty()
{
    return OverlappingActors.empty();
}

#endif
