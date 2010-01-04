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


int main(int argc, char **argv)
{

    //create the world
	physworld TheWorld;

    //give the World our function to execute after rendering
    TheWorld.CallBacks->SetPostRender(&postRenderCallback);

    //Set the world in motion
	TheWorld.GameInit();


	return 0;
}

bool postRenderCallback()
{
    return false;
}

#endif
