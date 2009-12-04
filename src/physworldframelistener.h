#ifndef PHYSWORLDFRAMELISTENER_H
#define PHYSWORLDFRAMELISTENER_H
///////////////////////////////////////////////////////////////////////////////
//Ogre framelisteners are classes which contain callback functions for when
//certain events happen with frame render associated with a specific Ogre root.
//
///////////////////////////////////////////////////////////////////////////////


//Includes
//#include <Ogre.h>
#include "physworld.h"


///////////////////////////////////////////////////////////////////////////////
// Interface of physworldFrameListener

class physworldFrameListener : public Ogre::FrameListener
{
	private:
		Ogre::Real mTime;
		physworld* TheWorldIListenTo;
	public:
  		physworldFrameListener(physworld* _Parent);

  		//this is called whenever we start rendering a frame
		virtual bool frameStarted(const Ogre::FrameEvent& evt);

		//Whenever a frame has finished rendering this is called
		virtual bool frameEnded(const Ogre::FrameEvent& evt);
};

#endif


