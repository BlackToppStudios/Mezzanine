#ifndef PHYSWORLDCALLBACKMANAGER_CPP
#define PHYSWORLDCALLBACKMANAGER_CPP
///////////////////////////////////////////////////////////////////////////////
//Ogre Frame rendering callbacks are implemented here
//
//Since Ogre handles the main loop, most of our interactions for the game logic
//need to interact with this, and the most consistent interface is the ogre
//frame listener, so we created this wrapper around that.
///////////////////////////////////////////////////////////////////////////////

//Includes
#include "physworldcallbackmanager.h"
#include "physdatatypes.h"
#include "physworld.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////////
// physworldFrameListener
///////////////////////////////////////
physworldCallBackManager::physworldCallBackManager(physworld* _Parent): PreInputCallback(NULL)
{
	TheWorldIListenTo = _Parent;

	PreInputCallback = NULL;
	PrePhysicsCallback = NULL;
	PreRenderCallback = NULL;
 	PostRenderCallback = NULL;
}

physworldCallBackManager::~physworldCallBackManager()
{

}

///////////////////////////////////////////////////////////////////////////////
//Preinput callback will be called in the main loop first
///////////////////////////////////////
bool physworldCallBackManager::PreInput()
{
	//If a call back has been set then we use it, otherwise we can
	//assume that we should just keep execute the main loop
	if(NULL!=PreInputCallback)
	{
		return (*PreInputCallback)();
	}
	return true;
}

void physworldCallBackManager::ErasePreInput()
{
	this->PreInputCallback = NULL;
}

void physworldCallBackManager::SetPreInput(bool (*Callback)())
{
	this->PreInputCallback = Callback;
}

bool physworldCallBackManager::IsPreInputCallbackSet()
{
	if(NULL==PreInputCallback)
	{
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// These functions manage the pre physics functions
///////////////////////////////////////
bool physworldCallBackManager::PrePhysics()
{
	if(NULL!=PrePhysicsCallback)
	{
		return (*PrePhysicsCallback)();
	}
	return true;
}

void physworldCallBackManager::ErasePrePhysics()
{
	this->PrePhysicsCallback = NULL;
}

void physworldCallBackManager::SetPrePhysics(bool (*Callback)())
{
	this->PrePhysicsCallback = Callback;
}

bool physworldCallBackManager::IsPrePhysicsCallbackSet()
{
	if(NULL==PrePhysicsCallback)
	{
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// These functions manage the pre render functions
///////////////////////////////////////
bool physworldCallBackManager::PreRender()
{
	if(NULL!=PreRenderCallback)
	{
		return (*PreRenderCallback)();
	}
	return true;
}

void physworldCallBackManager::ErasePreRender()
{
	this->PreRenderCallback = NULL;
}

void physworldCallBackManager::SetPreRender(bool (*Callback)())
{
	this->PreRenderCallback = Callback;
}

bool physworldCallBackManager::IsPreRenderCallbackSet()
{
	if(NULL==PreRenderCallback)
	{
		return false;
	}
	return true;
}



///////////////////////////////////////////////////////////////////////////////
// These functions manage the posy render functions
/////////////////////////////////////// PostRenderCallback
bool physworldCallBackManager::PostRender()
{
	if(NULL!=PostRenderCallback)
	{
		return (*PostRenderCallback)();
	}
	return true;
}

void physworldCallBackManager::ErasePostRender()
{
	this->PostRenderCallback = NULL;
}

void physworldCallBackManager::SetPostRender(bool (*Callback)())
{
	this->PostRenderCallback = Callback;
}

bool physworldCallBackManager::IsPostRenderCallbackSet()
{
	if(NULL==PostRenderCallback)
	{
		return false;
	}
	return true;
}

#endif
