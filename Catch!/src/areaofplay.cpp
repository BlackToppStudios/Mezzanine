#ifndef _areaofplay_cpp
#define _areaofplay_cpp

#include "areaofplay.h"
#include <physgame.h>

AreaOfPlay::AreaOfPlay(ConstString &name, Vector3 Location) : AreaEffect(name, Location)
{

}

AreaOfPlay::~AreaOfPlay()
{

}

void AreaOfPlay::ApplyEffect()
{

}

bool AreaOfPlay::IsInside(ActorBase* Actor)
{
    for( std::list<ActorBase*>::iterator it = OverlappingActors.begin() ; it != OverlappingActors.end() ; it++ )
    {
        if( Actor == (*it) )
        {
            return true;
        }
    }
}

bool AreaOfPlay::AllObjectsAtRest()
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
