#ifndef _gamebase_cpp
#define _gamebase_cpp
///////////////////////////////////////////////////////////////////////////////
// Gamewide misc features go here

///////////////////////////////////////////////////////////////////////////////
// Additional Includes that are not required by the header
#include <wx/wx.h>
#include "gamebase.h"
#include "physworld.h"
#include "Ogre.h"

///////////////////////////////////////////////////////////////////////////////
// This function will Hide the game window and launch the appropriate 3d render
//window and start the game based on the users settings.
// returns: True if succeeds, False on Error
bool StartGame(UiFrame* MenuWindow)
{
	//Hide the old window
	MenuWindow->Hide();
	MenuWindow->Yield();

	//Set the world in motion
	physworld TheWorld;
	TheWorld.GameInit();

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

//returns: True if user wants fullscreen
bool Settings::getFullscreen()
{
	return this->Fullscreen;
}

//returns: false if changes could not be made
bool Settings::setFullscreen(bool _Fullscreen)
{
	//wierd checks go here to make sure it worked
	this->Fullscreen = _Fullscreen;
	return true;
}
#endif
