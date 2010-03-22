#ifndef _crossplatform_cpp
#define _crossplatform_cpp
///////////////////////////////////////////////////////////////////////////////
//this is the place where ALL of the non-multiplatform code goes.

//////////////////////
// If it cannot go here, at least describe it here
// 1: log(size_t) in the phys world is windows only there are items related to this in physworld.cpp and .h
//



///////////////////////////////////////////////////////////////////////////////
//Additional Includes

#include "physcrossplatform.h"
#include "physworld.h"
#include <Ogre.h>
#include "SDL.h"

//Selective Includes
#ifdef WINDOWS
	#include <cstdlib>//for sleep
	#include "SDL_syswm.h" //for the needed commands
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
//void* is always an ogre NameValuePairList
void* GetSDLOgreBinder()
{
	Ogre::NameValuePairList *misc = new Ogre::NameValuePairList();
	#ifdef WINDOWS
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWMInfo(&wmInfo);

		size_t winHandle = reinterpret_cast<size_t>(wmInfo.window);
		size_t winGlContext = reinterpret_cast<size_t>(wmInfo.hglrc);

		(*misc)["externalWindowHandle"] = Ogre::StringConverter::toString(winHandle);
		(*misc)["externalGLContext"] = Ogre::StringConverter::toString(winGlContext);
		(*misc)["externalGLControl"] = Ogre::String("True");
	#else
		(*misc)["currentGLContext"] = Ogre::String("True");
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

void RenderPhysWorld(PhysWorld *TheWorld)
{
    //TheWorld->OgreRoot->renderOneFrame();
    #ifndef WINDOWS
       TheWorld->OgreGameWindow->update(true);
    #else
       TheWorld->OgreRoot->renderOneFrame();
       SDL_GL_SwapBuffers();
    #endif
}

#endif
