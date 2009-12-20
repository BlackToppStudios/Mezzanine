#ifndef PHYSWORLDFRAMELISTENER_CPP
#define PHYSWORLDFRAMELISTENER_CPP
///////////////////////////////////////////////////////////////////////////////
//Ogre Frame rendering callbacks are implemented here
//
//Since Ogre handles the main loop, most of our interactions for the game logic
//need to interact with this, and the most consistent interface is the ogre
//frame listener, so we created this wrapper around that.
///////////////////////////////////////////////////////////////////////////////

//Includes
#include "physworldframelistener.h"
#include "physdatatypes.h"
#include "physworld.h"

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
		virtual bool FrameStarted(const Ogre::FrameEvent& evt);

		//Called after all render targets have had their rendering commands issued, but before render windows have been asked to flip their buffers over.
		virtual bool FrameRenderingQueued (const Ogre::FrameEvent &evt);

		//Called just after a frame has been rendered.
		virtual bool FrameEnded(const Ogre::FrameEvent& evt);
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
bool physworldPrivateFrameListener::FrameStarted(const Ogre::FrameEvent& evt)
{
	mTime += evt.timeSinceLastFrame;
	if (mTime > 1)
	{
	    TheWorldIListenTo->Log("Quitting after 10 sec.");
		return false;
	}

	return Translator->FrameStarted();
}

//Called after all render targets have had their rendering commands issued, but before render windows have been asked to flip their buffers over.
bool physworldPrivateFrameListener::FrameRenderingQueued (const Ogre::FrameEvent &evt)
{
	return Translator->FrameStarted();
}

//Called just after a frame has been rendered.
bool physworldPrivateFrameListener::FrameEnded(const Ogre::FrameEvent& evt)
{
	return Translator->FrameStarted();
}

///////////////////////////////////////////////////////////////////////////////
// physworldFrameListener
///////////////////////////////////////
physworldFrameListener::physworldFrameListener(physworld* _Parent)
{
	PrivateListen = new physworldPrivateFrameListener(_Parent,this);
	FrameStartCallback = NULL;
	FrameQueuedCallback = NULL;
	FrameEndedCallback = NULL;
}

physworldFrameListener::~physworldFrameListener()
{
	delete PrivateListen;
}

//Called when a frame is about to begin rendering.
bool physworldFrameListener::FrameStarted()
{
	//Lets do the bulk; of the work before we render
	//TheWorldIListenTo->DoMainLoopAllItems();

	//If a call back has been set then we use it, otherwise we can
	//assume that we should just keep execute the main loop
	if(NULL!=FrameStartCallback)
	{
		return (*FrameStartCallback);
	}
	return true;
}

void physworldFrameListener::EraseFrameStartedCallback()
{
	this->FrameStartCallback = NULL;
}

void physworldFrameListener::SetFrameStartedCallback(bool (*Callback)())
{
	this->FrameStartCallback = Callback;
}
///////////////////////////////////////////////////////////////////////////////
//Called after all render targets have had their rendering commands issued, but before render windows have been asked to flip their buffers over.
bool physworldFrameListener::FrameRenderingQueued ()
{
	if(NULL!=FrameQueuedCallback)
	{
		return (*FrameQueuedCallback);
	}
	return true;
}

void physworldFrameListener::EraseFrameRenderingQueuedCallback()
{
	this->FrameQueuedCallback = NULL;
}

void physworldFrameListener::SetFrameRenderingQueuedCallback(bool (*Callback)())
{
	this->FrameQueuedCallback = Callback;
}
///////////////////////////////////////////////////////////////////////////////
//Called just after a frame has been rendered.
bool physworldFrameListener::FrameEnded()
{
	if(NULL!=FrameEndedCallback)
	{
		return (*FrameEndedCallback);
	}

	return true;
}

void physworldFrameListener::EraseFrameEndedCallback()
{
	this->FrameEndedCallback = NULL;
}


void physworldFrameListener::SetFrameEndedCallback(bool (*Callback)())
{
	this->FrameEndedCallback = Callback;
}

#endif
