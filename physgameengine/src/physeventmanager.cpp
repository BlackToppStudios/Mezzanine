#ifndef _physeventmanager_cpp
#define _physeventmanager_cpp
///////////////////////////////////////////////////////////////////////////////
//This will capture all event and store them for easy dispatching.
//
//There will be an instance of this class in the physworld.
///////////////////////////////////////

#include "physeventmanager.h"
#include "SDL.h"

int PhysSDLFilter( const RawEvent *event );

bool PhysEventManager::IgnoreSDLQuitEvents;

PhysEventManager::PhysEventManager()
{
    SetIgnoreQuitEvents(false);
    SDL_SetEventFilter( PhysSDLFilter );
}

//These functions will give you the next event or help you manage the events
unsigned int PhysEventManager::GetRemainingEventCount()
{
    return EventQueue.size();
}

PhysEvent* PhysEventManager::GetNextEvent()
{
    if(EventQueue.size()==0)
    {
            return 0;
    }
    PhysEvent* results = EventQueue.front();
    EventQueue.pop_front();
    return results;
}

PhysEventRenderTime* PhysEventManager::GetNextRenderTimeEvent()
{
    PhysEventRenderTime* results = 0;
    for(list<PhysEvent*>::iterator Iter = EventQueue.begin(); Iter!=EventQueue.end(); Iter++)
    {
        if((*Iter)->getEventType()==RenderTime)
        {
            results = dynamic_cast<PhysEventRenderTime*> (*Iter);
            EventQueue.erase(Iter);
            return results;
        }
    }
    return results;
}

PhysEventUserInput* PhysEventManager::GetNextUserInputEvent()
{
    PhysEventUserInput* results = 0;
    for(list<PhysEvent*>::iterator Iter = EventQueue.begin(); Iter!=EventQueue.end(); Iter++)
    {
        if((*Iter)->getEventType()==UserInput)
        {
            results = dynamic_cast<PhysEventUserInput*> (*Iter);
            EventQueue.erase(Iter);
            return results;
        }
    }
    return results;
}

void PhysEventManager::AddEvent(PhysEvent* EventToAdd)
{
    EventQueue.push_back(EventToAdd);
}

bool PhysEventManager::DoQuitMessagesExist()
{
        return false;//This is system dependent.
}

bool PhysEventManager::IgnoreQuitEvents()
{
    return IgnoreSDLQuitEvents;
}

void PhysEventManager::SetIgnoreQuitEvents(bool Ignore)
{
    IgnoreSDLQuitEvents=Ignore;
}

int PhysSDLFilter( const RawEvent *event )
{
     if ( !PhysEventManager::IgnoreQuitEvents() && event->type == SDL_QUIT)
        { return 0; }
     return 1;
}


#endif
