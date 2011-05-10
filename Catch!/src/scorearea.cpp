#ifndef _scorearea_cpp
#define _scorearea_cpp

#include "scorearea.h"
#include "physgame.h"

ScoreArea::ScoreArea(const String &name, Vector3 Location) : AreaEffect(name, Location)
{

}

ScoreArea::~ScoreArea()
{

}

void ScoreArea::ApplyEffect()
{

}

bool ScoreArea::IsInside(ActorBase* Actor)
{
    for( std::list<ActorBase*>::iterator it = OverlappingActors.begin() ; it != OverlappingActors.end() ; it++ )
    {
        if( Actor == (*it) )
        {
            return true;
        }
    }
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

#endif
