#ifndef _gamebase_h
#define _gamebase_h
///////////////////////////////////////////////////////////////////////////////
// This is where all the gamewide features will go. If it has no better home
//Somewhere else we will stuff it here until it is time to refactor again.
///////////////////////////////////////////////////////////////////////////////

// Includes
#include "menuinterface.h"

///////////////////////////////////////////////////////////////////////////////
// This class will store settings about player preferences, The current plan
//plan is to use only one, but we can make more for multiplayer situations
class Settings
{
	private:
		bool Fullscreen;

	public:
		Settings();
		bool getFullscreen();
		bool setFullscreen(bool _Fullscreen);
};

///////////////////////////////////////////////////////////////////////////////
//entry point to make a phyworld world and start it.
//will return true if it works
bool StartGame(UiFrame* MenuWindow);

#endif
