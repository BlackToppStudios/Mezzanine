#ifndef PHYSEVENT_H
#define PHYSEVENT_H
///////////////////////////////////////////////////////////////////////////////
// This is a parent class that all events must derive from. This exists solely
//to normalize the output of the event manager class.
///////////////////////////////////////

enum EventType{
    RenderTime,
    UserInput,
    SystemMessage,
    Other
};


#include <string>
#include <ostream>

using namespace std;

class PhysEvent
{
	public:
		virtual EventType getEventType() = 0;
};

// Allow the PhysEvent to be sent to a stream, and there sent directly to the log
//std::ostream& operator<<(std::ostream& stream, const PhysEvent& x);

#endif
