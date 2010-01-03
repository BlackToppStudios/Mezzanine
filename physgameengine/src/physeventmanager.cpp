#ifndef _physeventmanager_cpp
#define _physeventmanager_cpp
///////////////////////////////////////////////////////////////////////////////
//This will capture all event and store them for easy dispatching.
//
//There will be an instance of this class in the physworld.
///////////////////////////////////////

#include "physeventmanager.h"



PhysEventManager::PhysEventManager()
{

}

//These functions will give you the next event or help you manage the events
unsigned int PhysEventManager::GetRemainingEventCount()
{

}

PhysEvent* PhysEventManager::GetNextEvent()
{

}

PhysEventRenderTime* PhysEventManager::GetNextRenderTimeEvent()
{

}

void AddEvent(PhysEvent* EventToAdd)
{

}

#endif
