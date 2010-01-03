#ifndef _physeventmanager_h
#define _physeventmanager_h
///////////////////////////////////////////////////////////////////////////////
//This will capture all event and store them for easy dispatching.
//
//There will be an instance of this class in the physworld.
///////////////////////////////////////
#include <vector>

using namespace std;

#include "physevent.h"
#include "physeventrendertime.h"
#include "physeventkeyboardinput.h"

class PhysEventManager
{
	private:
		vector< PhysEvent* > EventQueue;



	public:
		PhysEventManager();

        //These functions will give you the next event or help you manage the events
        unsigned int GetRemainingEventCount();
		PhysEvent* GetNextEvent();
		PhysEventRenderTime* GetNextRenderTimeEvent();

        //By and large the Game won't use this, but there is no reason it shouldn't
		void AddEvent(PhysEvent* EventToAdd);
};

#endif


