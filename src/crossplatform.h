#ifndef _crossplatform_h
#define _crossplatform_h
///////////////////////////////////////////////////////////////////////////////
// If we did our jobs right this section should be the only place that you
//need to change to compile this on a supported platform. just remark all the
//lines that do not have you platform using "//" and unremark your platform.
//
// Should you choose to port this to your platform, make sure that all the
//required libraries are installed, then make sure to write an implementation
//for each of the functions in crossplatform.cpp
///////////////////////////////////////////////////////////////////////////////

//#define MACOSX
#define UBUNTU
//#define WINDOWS

///////////////////////////////////////////////////////////////////////////////
//End Choose Your Platform
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes

#include <Ogre.h>
#include <string>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Platform Specific Functions get Declared here

//gets thePlugins.cfg filename and location
string GetPluginsDotCFG();

//Gets the Settings.cfg filename and location
string GetSettingsDotCFG();

//This returns a named parameter list with valid settings to use Ogre rendering
// on a pre-existing SDL context
Ogre::NameValuePairList GetSDLOgreBinder();


#endif
