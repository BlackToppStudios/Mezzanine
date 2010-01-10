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
    TheWorld.CallBacks->SetPreInput(&postRenderCallback);
    //TheWorld.CallBacks->SetPostRender(&postRenderCallback);

    //Set the world in motion
	TheWorld.GameInit();

	return 0;
}

bool postRenderCallback()
{
	static PhysReal gametime = 0;
	PhysEventRenderTime* CurrentTime = TheWorld.Events->GetNextRenderTimeEvent();

	delete CurrentTime;
    return false;
}

#endif
