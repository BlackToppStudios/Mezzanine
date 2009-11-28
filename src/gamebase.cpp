
//This is where all the gamewide features will go
//this file is also where all dirty hacks shall reside.

#ifndef _gamebase_cpp
#define _gamebase_cpp

#include <wx/wx.h>
#include "gamebase.h"
#include "physworld.h"
#include "Ogre.h"

bool StartGame(UiFrame* MenuWindow)
{
	//Hide the old window
	MenuWindow->Hide();
	MenuWindow->Yield();

	//make the physworld and set it in motion
	physworld TestOne;
	TestOne.GameInit();

	//return to the menu;
	MenuWindow->Show();

	return true;
}

/////////////////////////////////////
// Functions for managing settings
////
Settings::Settings()
{
	this->Fullscreen=false;
}

bool Settings::getFullscreen()
{
	return this->Fullscreen;
}

void Settings::setFullscreen(bool _Fullscreen)
{
	//wierd checks go here
	this->Fullscreen = _Fullscreen;
}
#endif
