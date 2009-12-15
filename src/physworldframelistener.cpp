#ifndef PHYSWORLDFRAMELISTENER_CPP
#define PHYSWORLDFRAMELISTENER_CPP
///////////////////////////////////////////////////////////////////////////////
//Ogre Frame rendering callbacks are implemented here
//
//Since Ogre handles the main loop, most of our interactions for the game logic
//need to interact with this, and the most consistent interface is the ogre
//frame listener
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

//Called when a frame is about to begin rendering.
bool physworldFrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
	//DoMainLoopAllItems();


	mTime += evt.timeSinceLastFrame;
	if (mTime > 10)
	{
	    TheWorldIListenTo->Log("Quitting after 10 sec.");
		return false;
	}

	return true;
}

//Called after all render targets have had their rendering commands issued, but before render windows have been asked to flip their buffers over.
bool physworldFrameListener::frameRenderingQueued (const Ogre::FrameEvent &evt)
{
	return true;
}


//Called just after a frame has been rendered.
bool physworldFrameListener::frameEnded(const Ogre::FrameEvent& evt)
{
	return true;
}




#endif
