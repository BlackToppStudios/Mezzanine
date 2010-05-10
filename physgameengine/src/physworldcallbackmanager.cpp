//© Copyright 2010 Joseph Toppi and John Blackwood
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
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
#include "datatypes.h"
#include "physworld.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////////
// PhysWorldFrameListener
///////////////////////////////////////
PhysWorldCallBackManager::PhysWorldCallBackManager(PhysWorld* _Parent): PreInputCallback(NULL)
{
	TheWorldIListenTo = _Parent;

	PreInputCallback = NULL;
	PostInputCallback = NULL;

	PrePhysicsCallback = NULL;
	PostPhysicsCallback = NULL;

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
//Postinput callback will be called in the main loop right after input
///////////////////////////////////////
bool PhysWorldCallBackManager::PostInput()
{
	//If a call back has been set then we use it, otherwise we can
	//assume that we should just keep execute the main loop
	if(NULL!=PostInputCallback)
	{
		return (*PostInputCallback)();
	}
	return true;
}

void PhysWorldCallBackManager::ErasePostInput()
{
	this->PostInputCallback = NULL;
}

void PhysWorldCallBackManager::SetPostInput(bool (*Callback)())
{
	this->PostInputCallback = Callback;
}

bool PhysWorldCallBackManager::IsPostInputCallbackSet()
{
	if(NULL==PostInputCallback)
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
// These functions manage the Post physics functions
///////////////////////////////////////
bool PhysWorldCallBackManager::PostPhysics()
{
	if(NULL!=PostPhysicsCallback)
	{
		return (*PostPhysicsCallback)();
	}
	return true;
}

void PhysWorldCallBackManager::ErasePostPhysics()
{
	this->PostPhysicsCallback = NULL;
}

void PhysWorldCallBackManager::SetPostPhysics(bool (*Callback)())
{
	this->PostPhysicsCallback = Callback;
}

bool PhysWorldCallBackManager::IsPostPhysicsCallbackSet()
{
	if(NULL==PostPhysicsCallback)
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
