#ifndef _physeventmanager_h
#define _physeventmanager_h
///////////////////////////////////////////////////////////////////////////////
//This will capture all event and store them for easy dispatching.
//
//There will be an instance of this class in the physworld.
///////////////////////////////////////
#include <list>

using namespace std;

#include "physevent.h"
#include "physeventrendertime.h"
#include "physeventuserinput.h"

class PhysEventManager
{
	private:
		list<PhysEvent*> EventQueue;
        static bool IgnoreSDLQuitEvents;

	public:
		PhysEventManager();

        //These functions will give you the next event or help you manage the events
        //Whenever and event is gotten it is removed form the event queue
        //any getfunction cannot find an appropriate event it returns a pointer to 0
        unsigned int GetRemainingEventCount();
		PhysEvent* GetNextEvent();

		//This gets the first/next available PhysEventRenderTime* in the Queue, then removes it.
		PhysEventRenderTime* GetNextRenderTimeEvent();
        PhysEventUserInput* GetNextUserInputEvent();

        //By and large the Game won't use this, but there is no reason it shouldn't
		void AddEvent(PhysEvent* EventToAdd);

        //different platforms treat exiting the application differently, to work around that
        //we can use this to help identify if it comes accross as a normal event, or if we must
        //quit without generating one
        bool DoQuitMessagesExist();
        static bool IgnoreQuitEvents();                //if true, don't exit
        static void SetIgnoreQuitEvents(bool Ignore);  //if false exit when x is clicked.

};

#endif
