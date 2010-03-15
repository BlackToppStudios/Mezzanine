#ifndef _physactor_h
#define _physactor_h

#include "physvector.h"

namespace Ogre
{
    class Entity;
    class SceneManager;
    class SceneNode;
}

class actorbase {

	protected:
	PhysVector3 vector;
	PhysReal coefficient;
	PhysReal weight;
	PhysReal density;
    bool move;

	//abstraction for other libraries
	Ogre::Entity* entity;
    Ogre::SceneManager* physscenemanager;
    Ogre::SceneNode* node;

	public:
	~actorbase ();
	actorbase (PhysReal x, PhysReal y, PhysReal z, PhysReal c, PhysReal w, PhysReal d);
	PhysVector3 GetLocation ();
	PhysReal GetCoefficient ();
	PhysReal GetWeight ();
	PhysReal GetDensity ();
	void CreateEntity(PhysString name, PhysString file, PhysString group);
	void CreateSceneNode();
    void SetOgreLocation(PhysReal x, PhysReal y, PhysReal z);
    //void SetOgreOrientation();
};

class actordyn: public actorbase {

	private:
	PhysVector3 speed;
	PhysVector3 accel;

	public:
	actordyn();
	~actordyn();
	PhysVector3 GetSpeed ();
	PhysVector3 GetAccel ();
	void Halt ();
};

class actorsta: public actorbase {

    public:
    actorsta();
    ~actorsta();
};

#endif
