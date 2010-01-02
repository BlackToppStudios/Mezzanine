#ifndef _physgamesettings_cpp
#define _physgamesettings_cpp

#include "physworld.h"
#include "physeventmanager.h"
#include "physgamesettings.h"

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
