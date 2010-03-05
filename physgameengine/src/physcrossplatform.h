#ifndef _crossplatform_h
#define _crossplatform_h
///////////////////////////////////////////////////////////////////////////////
// If we did our jobs right this section should be the only place that you
//need to change to compile this on a supported platform. Just remark all the
//lines that are not your platform using "//" and unremark your platform.
//
// Should you choose to port this to your platform, make sure that all the
//required libraries are installed, then make sure to write an implementation
//for each of the functions in crossplatform.cpp
///////////////////////////////////////////////////////////////////////////////

#define LINUX
//#define MACOSX
//#define WINDOWS


///////////////////////////////////////////////////////////////////////////////
//End Choose Your Platform
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
#include "physdatatypes.h"

#include <string>

using namespace std;

//Forward Declarations
class physworld;

///////////////////////////////////////////////////////////////////////////////
// Platform Specific Functions get Declared here

//gets thePlugins.cfg filename and location
string GetPluginsDotCFG();

//Gets the Settings.cfg filename and location
string GetSettingsDotCFG();

//Gets the default locatio for Game Data
string GetDataDirectory();

//This returns a named parameter list with valid settings to use Ogre rendering
// on a pre-existing SDL context
void* GetSDLOgreBinder();

//A simple crossplatform sleep function, unused, prefer SDLsleep instead
void WaitMilliseconds(PhysWhole WaitTime);

//Handles the actual cross platform swapping of buffers.
void RenderPhysWorld(physworld *TheWorld);

#endif
