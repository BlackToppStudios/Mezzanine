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
// PhysWorldFrameListener
///////////////////////////////////////
PhysWorldCallBackManager::PhysWorldCallBackManager(PhysWorld* _Parent): PreInputCallback(NULL)
{
	TheWorldIListenTo = _Parent;

	PreInputCallback = NULL;
	PrePhysicsCallback = NULL;
	PreRenderCallback = NULL;
 	PostRenderCallback = NULL;
}

PhysWorldCallBackManager::~PhysWorldCallBackManager()
{

}

///////////////////////////////////////////////////////////////////////////////
//Preinput callback will be called in the main loop first
///////////////////////////////////////
bool PhysWorldCallBackManager::PreInput()
{
	//If a call back has been set then we use it, otherwise we can
	//assume that we should just keep execute the main loop
	if(NULL!=PreInputCallback)
	{
		return (*PreInputCallback)();
	}
	return true;
}

void PhysWorldCallBackManager::ErasePreInput()
{
	this->PreInputCallback = NULL;
}

void PhysWorldCallBackManager::SetPreInput(bool (*Callback)())
{
	this->PreInputCallback = Callback;
}

bool PhysWorldCallBackManager::IsPreInputCallbackSet()
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
bool PhysWorldCallBackManager::PrePhysics()
{
	if(NULL!=PrePhysicsCallback)
	{
		return (*PrePhysicsCallback)();
	}
	return true;
}

void PhysWorldCallBackManager::ErasePrePhysics()
{
	this->PrePhysicsCallback = NULL;
}

void PhysWorldCallBackManager::SetPrePhysics(bool (*Callback)())
{
	this->PrePhysicsCallback = Callback;
}

bool PhysWorldCallBackManager::IsPrePhysicsCallbackSet()
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
bool PhysWorldCallBackManager::PreRender()
{
	if(NULL!=PreRenderCallback)
	{
		return (*PreRenderCallback)();
	}
	return true;
}

void PhysWorldCallBackManager::ErasePreRender()
{
	this->PreRenderCallback = NULL;
}

void PhysWorldCallBackManager::SetPreRender(bool (*Callback)())
{
	this->PreRenderCallback = Callback;
}

bool PhysWorldCallBackManager::IsPreRenderCallbackSet()
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
bool PhysWorldCallBackManager::PostRender()
{
	if(NULL!=PostRenderCallback)
	{
		return (*PostRenderCallback)();
	}
	return true;
}

void PhysWorldCallBackManager::ErasePostRender()
{
	this->PostRenderCallback = NULL;
}

void PhysWorldCallBackManager::SetPostRender(bool (*Callback)())
{
	this->PostRenderCallback = Callback;
}

bool PhysWorldCallBackManager::IsPostRenderCallbackSet()
{
	if(NULL==PostRenderCallback)
	{
		return false;
	}
	return true;
}

#endif
