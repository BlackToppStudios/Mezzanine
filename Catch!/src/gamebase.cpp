#ifndef _gamebase_cpp
#define _gamebase_cpp
///////////////////////////////////////////////////////////////////////////////
// Gamewide Logic misc Features go here
///////////////////////////////////////////////////////////////////////////////
// Includes
//Game Include
#include "gamebase.h"

//Physengine include
#include <actorrigid.h>
#include <world.h>
#include <eventmanager.h>
#include <datatypes.h>
#include <eventrendertime.h>
#include <eventuserinput.h>
#include <actorcontainervector.h>
#include <metacode.h>

#include <sstream>

using namespace phys;

//Create the World Globally!
World TheWorld;
ActorRigid *object1, *object2, *object3, *object4;

int main(int argc, char **argv)
{
    TheWorld.SetWindowName("Catch!... The Game!");
    TheWorld.SetTargetFrameRate(40);

    //Give the world functions to run before and after input and physics
    TheWorld.CallBacks->SetPreInput(&PreInput);
    TheWorld.CallBacks->SetPostInput(&PostInput);
    TheWorld.CallBacks->SetPrePhysics(&PrePhysics);


    //give the World our function to execute after renderingg
    TheWorld.CallBacks->SetPostRender(&PostRender);

    //Set the Make the RenderWindow and load system stuff
	TheWorld.GameInit(false);

    //Set up polling for the letter Q and middle mouse button, and the mouse X and Y locations
    MetaCode PollForQ(0, 1, MetaCode::KEY_q);
    MetaCode PollForRightClick(0, 3, MetaCode::MOUSEBUTTON);
    MetaCode PollForX(0,0, MetaCode::MOUSEABSOLUTEHORIZONTAL);
    MetaCode PollForY(0,0, MetaCode::MOUSEABSOLUTEVERTICAL);
    TheWorld.Events->AddPollingCheck(PollForQ);
    TheWorld.Events->AddPollingCheck(PollForRightClick);
    TheWorld.Events->AddPollingCheck(PollForX);
    TheWorld.Events->AddPollingCheck(PollForY);

    LoadContent();

    TheWorld.SetDebugPhysicsWireCount(2);
    TheWorld.SetDebugPhysicsRendering(0);

	//Start the Main Loop
	TheWorld.MainLoop();

	return 0;
}

bool PostRender()
{
	//Lets set a variable for the time
	static Whole gametime = 0;

	TheWorld.Log("---------- Starting CallBack -------------");
    TheWorld.Log("Current Game Time ");

    std::stringstream timestream;
    timestream << "Catch!... " << gametime;
    TheWorld.SetWindowName( timestream.str() );

	//getting a message from the event manager
	EventRenderTime* CurrentTime = TheWorld.Events->PopNextRenderTimeEvent();

    // Is CurrentTime a valid event?
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
	if (10000<gametime || (TheWorld.Events->GetNextQuitEvent()!=0) )
	{
		return false;
	}

    if (2000<gametime)
    {
        TheWorld.SetDebugPhysicsWireCount(2);
    }

    return true;
}

bool PrePhysics()
{
    TheWorld.Log("Object Locations");
    TheWorld.Log(object1->GetLocation());
    TheWorld.Log(object2->GetLocation());
    TheWorld.Log(object3->GetLocation());
    TheWorld.Log(object4->GetLocation());
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
    EventUserInput* OneInput = TheWorld.Events->PopNextUserInputEvent();

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
        OneInput = TheWorld.Events->PopNextUserInputEvent();
    }

    return true;
}

void LoadContent()
{
    //Ogre Setup Code
    String groupname="Robot";
    String filename="robot.mesh";
    Real mass=5.0;
    TheWorld.AddResourceLocation(crossplatform::GetDataDirectory(), "FileSystem", groupname, false);
    TheWorld.DeclareResource(filename, "Mesh", groupname);
    TheWorld.DeclareResource("Examples.material", "Material", groupname);
    TheWorld.InitResourceGroup(groupname);

    //Actor Init Code
    ActorContainerVector box (&TheWorld);
    for(box.clear(); box.size()<7; box.CursorToLast())
    {
        std::stringstream namemaker;
        namemaker << groupname << box.size();
        box.AddActor(new ActorRigid (mass,namemaker.str(),filename,groupname,&TheWorld));
        box.LastActorAdded()->CreateShapeFromMeshDynamic(4);
        box.LastActorAdded()->SetInitLocation(Vector3( ((float)(34*box.size())-182), 160, 0));
        box.LastActorAdded()->SetInitOrientation(Quaternion(0,-0.5,0,0.5));
        TheWorld.AddActor(box.LastActorAdded());
    }

    object1 = new ActorRigid (mass,groupname,filename,groupname,&TheWorld);
    object1->CreateShapeFromMeshDynamic(4);
    object1->SetInitLocation(Vector3(0.0,10,0));
    object1->LimitMovementOnAxis(false,true,true);

    object2 = new ActorRigid (mass,"Robot_2",filename,groupname,&TheWorld);
    object2->CreateShapeFromMeshDynamic(4);
    object2->SetInitLocation(Vector3(0.0,10,0));
    object2->SetInitOrientation(Quaternion(0.5, 0.5, 0.0, 0.5));

    object3 = new ActorRigid (0,"Robot_3",filename,groupname,&TheWorld);
    object3->CreateShapeFromMeshStatic();
    object3->SetInitLocation(Vector3(-130,0,0));
    object4 = new ActorRigid (0,"Robot_4",filename,groupname,&TheWorld);
    object4->CreateShapeFromMeshStatic();
    object4->SetInitLocation(Vector3(130,0,0));

    //Final Steps
    Vector3 grav;
    grav.X=0.0;
    grav.Y=-1000.0;
    grav.Z=0.0;
    TheWorld.AddActor(object1);
    TheWorld.AddActor(object2);
    TheWorld.AddActor(object3);
    TheWorld.AddActor(object4);

    TheWorld.SetGravity(grav);
}

#endif
