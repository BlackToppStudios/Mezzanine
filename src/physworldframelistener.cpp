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
#include "physdatatypes.h"

#include "Ogre.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// physworldPrivateFrameListener
///////////////////////////////////////
//This Class is internal to the phys world and should be access by users
// of the library
class physworldPrivateFrameListener : public Ogre::FrameListener
{
	private:
		PhysReal mTime;

		//used to abstract away the rendering library
		physworldFrameListener* Translator;

		physworld* TheWorldIListenTo;
		//Ogre::InputReader* OgreInputReader;
	public:
  		physworldPrivateFrameListener(physworld* _Parent, physworldFrameListener* _Translator);

  		//Called when a frame is about to begin rendering.
		virtual bool frameStarted(const Ogre::FrameEvent& evt);

		//Called after all render targets have had their rendering commands issued, but before render windows have been asked to flip their buffers over.
		virtual bool frameRenderingQueued (const Ogre::FrameEvent &evt);

		//Called just after a frame has been rendered.
		virtual bool frameEnded(const Ogre::FrameEvent& evt);
};

///////////////////////////////////////////////////////////////////////////////
// Implementation of physworldFrameListener
///////////////////////////////////////////////////////////////////////////////
physworldPrivateFrameListener::physworldPrivateFrameListener(physworld* _Parent, physworldFrameListener* _Translator) : mTime(0)
{
	TheWorldIListenTo = _Parent;
	Translator = _Translator;
}

//Called when a frame is about to begin rendering.
bool physworldPrivateFrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
	mTime += evt.timeSinceLastFrame;
	if (mTime > 10)
	{
	    TheWorldIListenTo->Log("Quitting after 10 sec.");
		return false;
	}

	return Translator->frameStarted();
}

//Called after all render targets have had their rendering commands issued, but before render windows have been asked to flip their buffers over.
bool physworldPrivateFrameListener::frameRenderingQueued (const Ogre::FrameEvent &evt)
{
	return Translator->frameStarted();
}

//Called just after a frame has been rendered.
bool physworldPrivateFrameListener::frameEnded(const Ogre::FrameEvent& evt)
{
	return Translator->frameStarted();
}

///////////////////////////////////////////////////////////////////////////////
// physworldFrameListener
///////////////////////////////////////
physworldFrameListener::physworldFrameListener(physworld* _Parent)
{
	PrivateListen = new physworldPrivateFrameListener(_Parent,this);
}

physworldFrameListener::~physworldFrameListener()
{
	delete PrivateListen;
}

//Called when a frame is about to begin rendering.
bool physworldFrameListener::frameStarted()
{
	//TODO execute callback functions
	TheWorldIListenTo->DoMainLoopAllItems();
	return true;
}

//Called after all render targets have had their rendering commands issued, but before render windows have been asked to flip their buffers over.
bool physworldFrameListener::frameRenderingQueued ()
{
	//TODO execute callback functions
	return true;
}

//Called just after a frame has been rendered.
bool physworldFrameListener::frameEnded()
{
	//TODO execute callback functions
	return true;
}


#endif
