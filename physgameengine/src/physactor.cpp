#ifndef _physactor_cpp
#define _physactor_cpp

#include <Ogre.h>
#include "physactor.h"

///////////////////////////////////
// Actorbase class fuctions

actorbase::~actorbase ()
{
}

actorbase::actorbase (PhysReal x=0, PhysReal y=0, PhysReal z=0, PhysReal c=0, PhysReal w=0, PhysReal d=0)
{
    vector.X = x;
    vector.Y = y;
    vector.Z = z;
    coefficient = c;
    weight = w;
    density = d;
}

PhysVector3 actorbase::GetLocation ()
{
    return (vector);
}

PhysReal actorbase::GetCoefficient ()
{
	return (coefficient);
}

PhysReal actorbase::GetWeight ()
{
	return (weight);
}

PhysReal actorbase::GetDensity ()
{
	return (density);
}

void actorbase::CreateEntity (PhysString name, PhysString file, PhysString group)
{
    entity = this->physscenemanager->createEntity(name, file, group);
}

void actorbase::CreateSceneNode ()
{
    node = this->physscenemanager->createSceneNode();
}

void actorbase::SetOgreLocation (PhysReal x, PhysReal y, PhysReal z)
{
    this->node->setPosition(x, y, z);
}

//void actorbase::SetOgreOrientation ()
//{
//    this->node->setOrientation();
//}

///////////////////////////////////
// Actordyn class functions

actordyn::actordyn ()
{
    move=true;
}

actordyn::~actordyn ()
{
}

PhysVector3 actordyn::GetSpeed ()
{
	return (speed);
}

PhysVector3 actordyn::GetAccel ()
{
	return (accel);
}

void actordyn::Halt ()
{
    accel.X = 0;
    accel.Y = 0;
    accel.Z = 0;
    speed.X = 0;
    speed.Y = 0;
    speed.Z = 0;
}

///////////////////////////////////
// Actorsta class functions

actorsta::actorsta ()
{
    move=false;
}

actorsta::~actorsta ()
{
}

#endif
