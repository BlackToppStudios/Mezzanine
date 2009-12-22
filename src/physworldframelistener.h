#ifndef PHYSWORLDFRAMELISTENER_H
#define PHYSWORLDFRAMELISTENER_H
///////////////////////////////////////////////////////////////////////////////
//Framelisteners are classes which contain callback functions for when
//certain events happen with the Rendering engine
///////////////////////////////////////////////////////////////////////////////

//#include "physworld.h"
class physworld;
//Forward declaring a class to interface with the callbacks from the rendering
//engine
class physworldPrivateFrameListener;

//#include "physdatatypes.h"
#include "Ogre.h"

///////////////////////////////////////////////////////////////////////////////
// Interface of physworldFrameListener
class physworldFrameListener
{
	private:
		//pointers to internal structures
		physworldPrivateFrameListener* PrivateListen;
		physworld* TheWorldIListenTo;

		//pointers to callback functions
 		bool (*FrameStartCallback)();
 		bool (*FrameQueuedCallback)();
 		bool (*FrameEndedCallback)();

	public:
	//To build a frame listener we need a world to listen too
  		physworldFrameListener(physworld* _Parent);
  		~physworldFrameListener();

  		//Called when a frame is about to begin rendering. so is the
  		//Function you passe in with the setFrameStartCallback
		bool FrameStarted();
		void EraseFrameStartedCallback();
		void SetFrameStartedCallback(bool (*Callback)());

		//Called after all render targets have had their rendering commands issued, but before render windows have been asked to flip their buffers over.
		bool FrameRenderingQueued ();
		void EraseFrameRenderingQueuedCallback();
		void SetFrameRenderingQueuedCallback(bool (*Callback)());

		//Called just after a frame has been rendered.
		bool FrameEnded();
		void EraseFrameEndedCallback();
		void SetFrameEndedCallback(bool (*Callback)());

		Ogre::FrameListener* temp;

		friend class physworld;
};

#endif
