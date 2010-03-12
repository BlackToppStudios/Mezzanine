#ifndef _physmisc_h
#define _physmisc_h

#include "physdatatypes.h"

union SDL_Event;


//For internal use only
//this returns a string which contains the name of the SDL passed into it.
PhysString GetNameOfEventFrom(SDL_Event event);

#endif
