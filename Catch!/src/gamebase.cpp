#ifndef _gamebase_cpp
#define _gamebase_cpp
///////////////////////////////////////////////////////////////////////////////
// Gamewide Logic misc Features go here

///////////////////////////////////////////////////////////////////////////////
// Includes
//Game Include
#include "gamebase.h"

//Physengine include
#include "physworld.h"
#include "physeventmanager.h"
#include "physdatatypes.h"
#include "physeventrendertime.h"

//Create the World Globally!
physworld TheWorld;

int main(int argc, char **argv)
{

    //give the World our function to execute after rendering
    TheWorld.CallBacks->SetPreRender(&preRenderCallback);

    //Set the world in motion
	TheWorld.GameInit();

	return 0;
}

bool preRenderCallback()
{
	//Lets set a variable for the time
	static PhysWhole gametime = 0;

	TheWorld.Log("---------- Starting CallBack -------------");
	//getting a message from the event manager
	PhysEventRenderTime* CurrentTime = TheWorld.Events->GetNextRenderTimeEvent();

    TheWorld.Log("Time since last frame ");
    TheWorld.Log(CurrentTime->getMilliSecondsSinceLastFrame());
    TheWorld.Log("Current Game Time ");
    TheWorld.Log(gametime);
	gametime+=CurrentTime->getMilliSecondsSinceLastFrame();

	//since we got the event out of the manager, we are now responsible for deleting
	delete CurrentTime;

	if (3000<gametime)
	{
		return false;
	}

    return true;
}

#endif
