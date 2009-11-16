//This is where all the gamewide features will go
//this file is also where all dirty hacks shall reside.

#ifndef _gamebase_h
#define _gamebase_h

#include <wx/wx.h>
#include "menuinterface.h"

//entry point to make a phyworld world and start it.
//will return true if it
bool SpawnGameThread(UiFrame* GameWindow);

#endif
