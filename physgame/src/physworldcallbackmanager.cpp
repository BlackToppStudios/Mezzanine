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
physworldCallBackManager::physworldCallBackManager(physworld* _Parent)
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
		return (*PreInputCallback);
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

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////
bool physworldCallBackManager::PrePhysics()
{return true;}

void physworldCallBackManager::ErasePrePhysics()
{}

void physworldCallBackManager::SetPrePhysics(bool (*Callback)())
{}

bool physworldCallBackManager::PreRender()
{return true;}

void physworldCallBackManager::ErasePreRender()
{}

void physworldCallBackManager::SetPreRender(bool (*Callback)())
{}

bool physworldCallBackManager::PostRender()
{return true;}

void physworldCallBackManager::ErasePostRender()
{}

void physworldCallBackManager::SetPostRender(bool (*Callback)())
{}

#endif
