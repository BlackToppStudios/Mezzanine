#ifndef PHYSEVENTRENDERTIME_CPP
#define PHYSEVENTRENDERTIME_CPP
///////////////////////////////////////////////////////////////////////////////
// This will store the amount of
///////////////////////////////////////

#include <string>

#include "physdatatypes.h"
#include "physeventrendertime.h"

using namespace std;

string PhysEventRenderTime::getEventType()
{
	return "PhysEventRenderTime";
}

PhysReal PhysEventRenderTime::getTimeSinceLastFrame()
{
	return 0;
}


#endif
