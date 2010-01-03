#ifndef PHYSEVENTRENDERTIME_H
#define PHYSEVENTRENDERTIME_H
///////////////////////////////////////////////////////////////////////////////
// This will store the amount of time since rendering events that occured recently
///////////////////////////////////////

#include "physevent.h"

using namespace std;


class PhysEventRenderTime : PhysEvent
{
    private:
        unsigned long Rendertime;
	public:
        PhysEventRenderTime (unsigned long Milliseconds);
		virtual EventType getEventType();
		unsigned long getMilliSecondsSinceLastFrame();
};

#endif
