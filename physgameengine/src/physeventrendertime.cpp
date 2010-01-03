#ifndef PHYSEVENTRENDERTIME_CPP
#define PHYSEVENTRENDERTIME_CPP
///////////////////////////////////////////////////////////////////////////////
// This will store the amount of time  since key rendering events have occurred
///////////////////////////////////////

#include "physdatatypes.h"
#include "physeventrendertime.h"

using namespace std;

PhysEventRenderTime::PhysEventRenderTime (unsigned long Milliseconds)
{
    Rendertime=Milliseconds;
}

EventType PhysEventRenderTime::getEventType()
{
	return RenderTime;
}

unsigned long PhysEventRenderTime::getMilliSecondsSinceLastFrame()
{
	return Rendertime;
}


#endif
