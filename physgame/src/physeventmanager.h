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
#include "physeventkeyboardinput.h"

class PhysEventManager
{
	private:
		vector< PhysEvent* > EventQueue;

	public:
		PhysEventManager();

};

#endif


