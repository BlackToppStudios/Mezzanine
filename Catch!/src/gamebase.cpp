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

    //Give the world functions to run before and after input
    TheWorld.CallBacks->SetPreInput(&PreInput);
    TheWorld.CallBacks->SetPostInput(&PostInput);

    //give the World our function to execute after rendering
    TheWorld.CallBacks->SetPostRender(&PostRender);

    //Set the world in motion
	TheWorld.GameInit();

	return 0;
}

bool PostRender()
{


	//Lets set a variable for the time
	static PhysWhole gametime = 0;

	TheWorld.Log("---------- Starting CallBack -------------");
    TheWorld.Log("Current Game Time ");


	//getting a message from the event manager
	PhysEventRenderTime* CurrentTime = TheWorld.Events->GetNextRenderTimeEvent();

    // Is currentTime a valid event?
    while(0 != CurrentTime)
    {
        //TheWorld.Log("Time since last frame ");
        //TheWorld.Log(CurrentTime->getMilliSecondsSinceLastFrame());
        TheWorld.Log(gametime);
        gametime+=CurrentTime->getMilliSecondsSinceLastFrame();

        delete CurrentTime;
        CurrentTime = TheWorld.Events->GetNextRenderTimeEvent();
    }

    //IF the game has gone on for 10 or more seconds close it.
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

bool PreInput()
{
    return true;
}

bool PostInput()
{
    //Do nothing this just guarantees that the main loop will run checks for user input.
    if( !CheckForEsc() )
        return false;
    return true;
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
        TheWorld.Log("Input Events Processed");

        //we check each MetaCode in each Event
        for (int c=0; c<OneInput->GetMetaCodeCount(); c++ )
        {
            TheWorld.Log(OneInput->GetMetaCode(c));
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
