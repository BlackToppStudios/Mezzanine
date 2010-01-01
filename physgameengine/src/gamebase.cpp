#ifndef _gamebase_cpp
#define _gamebase_cpp
///////////////////////////////////////////////////////////////////////////////
// Gamewide Logic misc Features go here

///////////////////////////////////////////////////////////////////////////////
// Includes
//Game Include
#include "gamebase.h"

//Physengine include
#include "physworld.h"
#include "physeventmanager.h"


int main(int argc, char **argv)
{

	PhysEventManager TheWorldsEvents;

	//Set the world in motion
	physworld TheWorld;
	TheWorld.GameInit();


	return 0;
}


/////////////////////////////////////
// Functions for managing settings
////
Settings::Settings()
{
	this->Fullscreen=false;
	this->RenderHeight = 680;
	this->RenderWidth = 480;
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

//Returns the height of the render window
int Settings::getRenderHeight()
{
	return this->RenderHeight;
}

//Returns the width of the render window
int Settings::getRenderWidth()
{
	return this->RenderWidth;
}
//returns: false if changes could not be made
bool Settings::setRenderHeight(int Height)
{
	this->RenderHeight = Height;
	return true;
}

//returns: false if changes could not be made
bool Settings::setRenderWidth(int Width)
{
	this->RenderWidth = Width;
	return true;
}

#endif
