#ifndef _gamebase_cpp
#define _gamebase_cpp
///////////////////////////////////////////////////////////////////////////////
// Gamewide Logic misc Features go here
///////////////////////////////////////////////////////////////////////////////

#include "gamebase.h"       //Game Include
#include <physgame.h>       //Physgame include
#include <sstream>          //STL includes

using namespace phys;

World TheWorld;                     //Create the World Globally! and we a place to hold some actors

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

    //IF the game has gone on for 60 or more seconds close it.
	if (60000<gametime || (TheWorld.Events->GetNextQuitEvent()!=0) )
        { return false; }

    return true;
}

bool PrePhysics()
{
    TheWorld.Log("Object Locations");
    //Replace this with something that uses the actor container and logs the location of everything
    //TheWorld.Log(object1->GetLocation());
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
    ActorRigid *object1, *object2, *object3, *object4, *object5, *object6;
    //Ogre Setup Code
    String groupname="Group1";
    String filerobot="robot.mesh";

    Real mass=5.0;
    TheWorld.AddResourceLocation(crossplatform::GetDataDirectory(), "FileSystem", groupname, false);
    TheWorld.DeclareResource(filerobot, "Mesh", groupname);
    TheWorld.DeclareResource("Examples.material", "Material", groupname);
    TheWorld.DeclareResource("plane.material", "Material", groupname);
    TheWorld.DeclareResource("Sphere_Wood.material", "Material", groupname);
    TheWorld.DeclareResource("Sphere_Metal.material", "Material", groupname);

    TheWorld.InitResourceGroup(groupname);

    object1 = new ActorRigid (mass,groupname,filerobot,groupname,&TheWorld);
    object1->CreateShapeFromMeshDynamic(4);
    object1->SetInitLocation(Vector3(-25.0,10,0));
    //object1->LimitMovementOnAxis(false,true,true);

    object2 = new ActorRigid (80.0f,"Sphere_Wood","Sphere_Wood.mesh",groupname,&TheWorld);
    object2->CreateSphereShapeFromMesh();
    object2->SetActorScaling(Vector3(0.1,0.1,0.1));
    object2->SetInitLocation(Vector3(50,800,-1000));

    object3 = new ActorRigid (100.0f,"Sphere_Metal","Sphere_Metal.mesh",groupname,&TheWorld);
    object3->CreateSphereShapeFromMesh();
    object2->SetActorScaling(Vector3(0.1,0.1,0.1));
    object3->SetInitLocation(Vector3(-50.0,600.0,-50.0));

    object4 = new ActorRigid (mass,"Robot_2",filerobot,groupname,&TheWorld);
    object4->CreateShapeFromMeshDynamic(1);
    object4->SetInitLocation(Vector3(25,10,0));
    object4->SetInitOrientation(Quaternion(0.5, 0.5, 0.0, 0.9));

    object5 = new ActorRigid (0,"Plane","plane.mesh",groupname,&TheWorld);
    object5->CreateShapeFromMeshStatic();
    object5->SetInitLocation(Vector3(0.0,-100,-300.0));

    object6 = new ActorRigid (0,"Ramp","plane.mesh",groupname,&TheWorld);
    object6->CreateShapeFromMeshDynamic(1);
    object6->SetInitLocation(Vector3(00.0,100,-900.0));
    object6->SetInitOrientation(Quaternion(0.5, 0.0, 0.0, 0.75));

    //Final Steps
    Vector3 grav;
    grav.X=0.0;
    grav.Y=-10000.0;
    grav.Z=0.0;
    TheWorld.AddActor(object1);
    TheWorld.AddActor(object2);
    TheWorld.AddActor(object3);
    TheWorld.AddActor(object4);
    TheWorld.AddActor(object5);
    TheWorld.AddActor(object6);

    TheWorld.SetGravity(grav);
}
#endif
