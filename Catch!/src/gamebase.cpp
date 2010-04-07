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
#include "physeventuserinput.h"

//Create the World Globally!
PhysWorld TheWorld;

int main(int argc, char **argv)
{

    //Give the world a function to run before user input
    TheWorld.CallBacks->SetPreInput(&preInput);

    //give the World our function to execute after rendering
    TheWorld.CallBacks->SetPreRender(&preRender);

    //Set the world in motion
	TheWorld.GameInit();

	return 0;
}

bool preRender()
{

    if( !CheckForEsc() )
        return false;

	//Lets set a variable for the time
	static PhysWhole gametime = 0;

	TheWorld.Log("---------- Starting CallBack -------------");
	//getting a message from the event manager
	PhysEventRenderTime* CurrentTime = TheWorld.Events->GetNextRenderTimeEvent();

    //TheWorld.Log("Time since last frame ");
    //TheWorld.Log(CurrentTime->getMilliSecondsSinceLastFrame());
    TheWorld.Log("Current Game Time ");
    TheWorld.Log(gametime);
	gametime+=CurrentTime->getMilliSecondsSinceLastFrame();

	//since we got the event out of the manager, we are now responsible for deleting
	delete CurrentTime;

	if (10000<gametime)
	{
		return false;
	}

    //If we wanted to we could unremark the following line and call all the main loop items right here, but
    //that is not needed, nor is it the prefered way to do things. All these Items will be called automatically
    //if the callbacks for the exists.
    //TheWorld.DoMainLoopAllItems();

    return true;
}

bool preInput()
{
    //Do nothing this just guarantees that the main loop will run checks for user input.

}

///////////////////
//Non-Callbacks
bool CheckForEsc()
{
    //this will either set the pointer to 0 or return a valid pointer to work with.
    PhysEventUserInput* OneInput = TheWorld.Events->GetNextUserInputEvent();

    //We check each Event
    while(0 != OneInput)
    {
        //we check each MetaCode in each Event
        for (int c=0; c<OneInput->GetMetaCodeCount(); c++ )
        {
            //Is the key we just pulled ESCAPE
            if(MetaCode::KEY_ESCAPE == OneInput->GetMetaCode(c).GetCode())
            {
                return false;
            }
        }

        delete OneInput;
        OneInput = TheWorld.Events->GetNextUserInputEvent();
    }

    return true;
}



#endif
