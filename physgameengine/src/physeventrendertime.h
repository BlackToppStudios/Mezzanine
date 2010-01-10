#ifndef PHYSEVENTRENDERTIME_H
#define PHYSEVENTRENDERTIME_H
///////////////////////////////////////////////////////////////////////////////
// This will store the amount of time since rendering events that occured recently
///////////////////////////////////////

#include "physevent.h"
#include "physeventrendertime.h"
#include "physdatatypes.h"

using namespace std;


class PhysEventRenderTime : public PhysEvent
{
    private:
        PhysWhole Rendertime;
	public:
        PhysEventRenderTime (PhysWhole Milliseconds);
		virtual EventType getEventType();
		PhysWhole getMilliSecondsSinceLastFrame();
};

#endif
