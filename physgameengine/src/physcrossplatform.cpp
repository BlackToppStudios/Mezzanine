#ifndef _crossplatform_cpp
#define _crossplatform_cpp
///////////////////////////////////////////////////////////////////////////////
//this is the place where ALL of the non-multiplatform code goes.

///////////////////////////////////////////////////////////////////////////////
//Additional Includes

#include "physcrossplatform.h"
#include <Ogre.h>

//Selective Includes
#ifdef WINDOWS
	#include <cstdlib>//for sleep
	#include "SDL_syswm.h" //for the needed commands
	#include "SDL.h"
#else
	#include <unistd.h>//for sleep
#endif


///////////////////////////////////////////////////////////////////////////////
//returns: The appropriate string, which contains a path, to the correct
//"plugins.cfg" file
string GetPluginsDotCFG()
{
	#ifdef LINUX
		return "data/linux/plugins.cfg";
	#endif
	#ifdef WINDOWS
		return "data/windows/plugins.cfg";
	#endif

}

///////////////////////////////////////////////////////////////////////////////
//returns: The appropriate string, which contains a path, to the correct
//"settings.cfg" file
string GetSettingsDotCFG()
{
	#ifdef LINUX
		return "data/linux/settings.cfg";
	#endif
	#ifdef WINDOWS
		return "data/windows/settings.cfg";
	#endif
}

string GetDataDirectory()
{
	return "data/common/";
}

///////////////////////////////////////////////////////////////////////////////
//This returns a named parameter list with valid settings to use Ogre rendering
//on a pre-existing SDL context
//This has not been tested on windows
Ogre::NameValuePairList GetSDLOgreBinder()
{
	Ogre::NameValuePairList misc;
	#ifdef WINDOWS
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWMInfo(&wmInfo);

		size_t winHandle = reinterpret_cast<size_t>(wmInfo.window);
		size_t winGlContext = reinterpret_cast<size_t>(wmInfo.hglrc);

		misc["externalWindowHandle"] = Ogre::StringConverter::toString(winHandle);
		misc["externalGLContext"] = Ogre::StringConverter::toString(winGlContext);
		misc["externalGLControl"] = Ogre::String("True");
	#else
		misc["currentGLContext"] = Ogre::String("True");
	#endif

	return misc;
}

void WaitMilliseconds(PhysWhole WaitTime)
{
	#ifdef WINDOWS
		Sleep(WaitTime);
	#else
		usleep(1000*WaitTime);
	#endif
}



#endif
