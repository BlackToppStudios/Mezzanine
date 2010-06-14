#ifndef _gamebase_cpp
#define _gamebase_cpp
///////////////////////////////////////////////////////////////////////////////
// Gamewide Logic misc Features go here
///////////////////////////////////////////////////////////////////////////////

//Game Includes
#include "gamebase.h"

//Physgame includes
#include <physgame.h>

//STL includes
#include <sstream>

using namespace phys;

World TheWorld;                     //Create the World Globally! and we a place to hold some actors
ActorRigid *object1, *object2, *object3, *object4;

int main(int argc, char **argv)
{
    TheWorld.Log("Vector Math Test");
    Vector3 Temp1(1,2,1);
    Vector3 Temp2(1,1,1);
    TheWorld.Log("Temp1");
    TheWorld.Log(Temp1);
    TheWorld.Log("Temp2");
    TheWorld.Log(Temp2);
    TheWorld.Log("Temp1-Temp2");
    TheWorld.Log(Temp1-Temp2);
    TheWorld.Log("Temp1");
    TheWorld.Log(Temp1);
    TheWorld.Log("Temp2");
    TheWorld.Log(Temp2);

    Vector3 Temp3(1,2,1);
    Vector3 Temp4(1,1,1);
    TheWorld.Log("Temp3");
    TheWorld.Log(Temp3);
    TheWorld.Log("Temp4");
    TheWorld.Log(Temp4);
    TheWorld.Log("Temp3+Temp4");
    TheWorld.Log(Temp3+Temp4);
    TheWorld.Log("Temp3");
    TheWorld.Log(Temp3);
    TheWorld.Log("Temp4");
    TheWorld.Log(Temp4);

    Vector3 Temp5(1,2,1);
    Vector3 Temp6(1,1,1);
    TheWorld.Log("Temp5");
    TheWorld.Log(Temp5);
    TheWorld.Log("Temp6");
    TheWorld.Log(Temp6);
    TheWorld.Log("Temp5*Temp6");
    TheWorld.Log(Temp5*Temp6);
    TheWorld.Log("Temp5");
    TheWorld.Log(Temp5);
    TheWorld.Log("Temp6");
    TheWorld.Log(Temp6);

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
    TheWorld.Events->AddPollingCheck( MetaCode(0, 1, MetaCode::KEY_q) );
    TheWorld.Events->AddPollingCheck( MetaCode(0, 3, MetaCode::MOUSEBUTTON) );
    TheWorld.Events->AddPollingCheck( MetaCode(0,0, MetaCode::MOUSEABSOLUTEHORIZONTAL) );

    //Actually Load the game stuff
    LoadContent();

    TheWorld.SetDebugPhysicsWireCount(2);
    TheWorld.SetDebugPhysicsRendering(1);

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

	//getting a message from the event manager
	EventRenderTime* CurrentTime = TheWorld.Events->PopNextRenderTimeEvent();

    // Is CurrentTime a valid event?
    while(0 != CurrentTime)
    {

        TheWorld.Log(gametime);
        gametime+=CurrentTime->getMilliSecondsSinceLastFrame();

        delete CurrentTime;
        CurrentTime = TheWorld.Events->GetNextRenderTimeEvent();
    }

    //Play around with the title bar
    std::stringstream timestream;
    timestream << "Catch!... " << gametime;
    TheWorld.SetWindowName( timestream.str() );

    //IF the game has gone on for 10 or more seconds close it.
	if (10000<gametime || (TheWorld.Events->GetNextQuitEvent()!=0) )
        { return false; }

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
    //User Input through a WorldQueryTool
    static WorldQueryTool Queryer(&TheWorld);

    Queryer.GatherEvents();
    TheWorld.Log("Mouse location From WorldQueryTool X/Y");
    TheWorld.Log(Queryer.GetMouseX());
    TheWorld.Log(Queryer.GetMouseY());

    // using the Raw Event Manager, and deleting the events
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
            //Is the key we just pushed ESCAPE
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
        box.LastActorAdded()->SetInitLocation(Vector3( ((float)(48*box.size())-182), 160, 0));
        box.LastActorAdded()->SetInitOrientation(Quaternion(0,-0.5,0,0.5));
        TheWorld.AddActor(box.LastActorAdded());
    }

    object1 = new ActorRigid (mass,groupname,filename,groupname,&TheWorld);
    object1->CreateShapeFromMeshDynamic(4);
    object1->SetInitLocation(Vector3(-5.0,10,0));
    object1->LimitMovementOnAxis(false,true,true);

    object2 = new ActorRigid (mass,"Robot_2",filename,groupname,&TheWorld);
    object2->CreateShapeFromMeshDynamic(4);
    object2->SetInitLocation(Vector3(5,10,0));
    object2->SetInitOrientation(Quaternion(0.5, 0.5, 0.0, 0.9));

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
