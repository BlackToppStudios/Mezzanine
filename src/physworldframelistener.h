#ifndef PHYSWORLDFRAMELISTENER_H
#define PHYSWORLDFRAMELISTENER_H
///////////////////////////////////////////////////////////////////////////////
//Ogre framelisteners are classes which contain callback functions for when
//certain events happen with frame render associated with a specific Ogre root.
///////////////////////////////////////////////////////////////////////////////


//Includes
#include <Ogre.h>
#include "physworld.h"


///////////////////////////////////////////////////////////////////////////////
// Interface of physworldFrameListener

class physworldFrameListener : public Ogre::FrameListener
{
	private:
		Ogre::Real mTime;

		physworld* TheWorldIListenTo;
		//Ogre::InputReader* OgreInputReader;
	public:
  		physworldFrameListener(physworld* _Parent);

  		//Called when a frame is about to begin rendering.
		virtual bool frameStarted(const Ogre::FrameEvent& evt);

		//Called after all render targets have had their rendering commands issued, but before render windows have been asked to flip their buffers over.
		virtual bool frameRenderingQueued (const Ogre::FrameEvent &evt);

		//Called just after a frame has been rendered.
		virtual bool frameEnded(const Ogre::FrameEvent& evt);
};

#endif


