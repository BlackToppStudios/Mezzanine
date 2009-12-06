#ifndef PHYSWORLDFRAMELISTENER_CPP
#define PHYSWORLDFRAMELISTENER_CPP
///////////////////////////////////////////////////////////////////////////////
//Ogre Frame rendering callbacks are implemented here
///////////////////////////////////////////////////////////////////////////////

//Includes
#include "physworldframelistener.h"

using namespace std;

// physworldFrameListener
///////////////////////////////////////////////////////////////////////////////
// Implementation of physworldFrameListener
///////////////////////////////////////////////////////////////////////////////
physworldFrameListener::physworldFrameListener(physworld* _Parent) : mTime(0)
{
	TheWorldIListenTo = _Parent;
}

//This function gets called when we start rendering a newframe
bool physworldFrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
	mTime += evt.timeSinceLastFrame;
	if (mTime > 3)
	{
	    TheWorldIListenTo->Log("Quitting after 3 sec.");
		return false;
	}
	return true;
}

//This gets called everytime a new frame finishes
bool physworldFrameListener::frameEnded(const Ogre::FrameEvent& evt)
{
	return true;
}
#endif
