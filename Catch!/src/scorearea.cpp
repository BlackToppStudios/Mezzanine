#ifndef _scorearea_cpp
#define _scorearea_cpp

#include "scorearea.h"
#include "physgame.h"

ScoreArea::ScoreArea(const String &name, Vector3 Location, World* world) : AreaEffect(name, Location, world)
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
        if( Act->CheckActivation() )
            return false;
    }
    return true;
}

#endif
