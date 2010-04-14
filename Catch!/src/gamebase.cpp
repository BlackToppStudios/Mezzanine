#ifndef _gamebase_cpp
#define _gamebase_cpp
///////////////////////////////////////////////////////////////////////////////
// Gamewide Logic misc Features go here

///////////////////////////////////////////////////////////////////////////////
// Includes
//Game Include
#include "gamebase.h"

//Physengine include
#include "physactor.h"
#include "physworld.h"
#include "physeventmanager.h"
#include "physdatatypes.h"
#include "physeventrendertime.h"
#include "physeventuserinput.h"

//Create the World Globally!
PhysWorld TheWorld;
ActorDynRigid* object1;
PhysReal x=0;
PhysReal y=0;
PhysReal z=0;

int main(int argc, char **argv)
{
    TheWorld.SetWindowName("Catch!... The Game!");
    TheWorld.SetTargetFrameRate(60);

    //Give the world functions to run before and after input
    TheWorld.CallBacks->SetPreInput(&PreInput);
    TheWorld.CallBacks->SetPostInput(&PostInput);

    //give the World our function to execute after rendering
    TheWorld.CallBacks->SetPostRender(&PostRender);

    //Set the Make the RenderWindow and load system stuff
	TheWorld.GameInit(false);

    LoadContent();

	//Start the Main Loop
	TheWorld.MainLoop();


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

    x+=0.5;
    object1->SetLocation(x,y,z);

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

void LoadContent()
{
    //Ogre Setup Code
    PhysString groupname="Robot";
    PhysString filename="robot.mesh";
    PhysReal mass=0.001;
    TheWorld.AddResourceLocation(GetDataDirectory(), "FileSystem", groupname, false);
    TheWorld.DeclareResource(filename, "Mesh", groupname);
    TheWorld.DeclareResource("Examples.material", "Material", groupname);
    TheWorld.InitResourceGroup(groupname);

    //Actor Init Code
    object1 = new ActorDynRigid (mass,groupname,filename,groupname,&TheWorld);
    object1->CreateShapeFromMesh();

    //Final Steps
    TheWorld.AddActor(object1);

	//Ogre::Entity *ent1 = this->OgreSceneManager->createEntity( "Robot", "robot.mesh" );
	//Ogre::SceneNode *node1 = this->OgreSceneManager->getRootSceneNode()->createChildSceneNode( "RobotNode" );
	//node1->attachObject( ent1 );
}

#endif
