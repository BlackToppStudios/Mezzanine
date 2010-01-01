#ifndef PHYSEVENTRENDERTIME_H
#define PHYSEVENTRENDERTIME_H
///////////////////////////////////////////////////////////////////////////////
// This will store the amount of time since rendering events that occured recently
///////////////////////////////////////

#include "physevent.h"

#include <string>

using namespace std;


class PhysEventRenderTime : PhysEvent
{
	public:
		virtual string getEventType();
		PhysReal getTimeSinceLastFrame();

};

#endif
