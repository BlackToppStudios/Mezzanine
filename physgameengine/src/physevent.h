#ifndef PHYSEVENT_H
#define PHYSEVENT_H
///////////////////////////////////////////////////////////////////////////////
// This is a parent class that all events must derive from. This exists solely
//to normalize the output of the event manager class.
///////////////////////////////////////

enum EventType{
    RenderTime
};

#include <string>

using namespace std;

class PhysEvent
{
	public:
		virtual EventType getEventType() = 0;

};

#endif
