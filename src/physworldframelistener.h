#ifndef PHYSWORLDFRAMELISTENER_H
#define PHYSWORLDFRAMELISTENER_H
///////////////////////////////////////////////////////////////////////////////
//Framelisteners are classes which contain callback functions for when
//certain events happen with the Rendering engine
///////////////////////////////////////////////////////////////////////////////

#include "physworld.h"
//Forward declaring a class to interface with the callbacks from the rendering
//engine
class physworldPrivateFrameListener;

///////////////////////////////////////////////////////////////////////////////
// Interface of physworldFrameListener
class physworldFrameListener
{
	private:
		physworldPrivateFrameListener* PrivateListen;
		physworld* TheWorldIListenTo;

	public:
	//To build a frame listener we need a world to listen too
  		physworldFrameListener(physworld* _Parent);
  		~physworldFrameListener();

  		//Called when a frame is about to begin rendering.
		bool frameStarted();

		//Called after all render targets have had their rendering commands issued, but before render windows have been asked to flip their buffers over.
		bool frameRenderingQueued ();

		//Called just after a frame has been rendered.
		bool frameEnded();

		friend void physworld::GameInit();
};

#endif
