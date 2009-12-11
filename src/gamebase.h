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

		//these refer to the render window
		int RenderHeight;
		int RenderWidth;

	public:
		Settings();
		bool getFullscreen();
		bool setFullscreen(bool _Fullscreen);

		//For the renderwindow
		int getRenderHeight();
		int getRenderWidth();
		bool setRenderHeight(int Height);
		bool setRenderWidth(int Width);

};

///////////////////////////////////////////////////////////////////////////////
//entry point to make a phyworld world and start it.
//will return true if it works
bool StartGame(UiFrame* MenuWindow);

#endif
