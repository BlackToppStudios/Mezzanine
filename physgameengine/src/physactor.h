#ifndef _physactor_h
#define _physactor_h

#include "physvector.h"

///////////////////////////////////
// Forward Declares

class btMotionState;
class btQuaternion;
class btRigidBody;
class btSoftBody;
class btTransform;

namespace Ogre
{
    class Entity;
    class SceneManager;
    class SceneNode;
}

///////////////////////////////////
// Actual code

class ActorBase {

	protected:

	//abstraction for other libraries
	Ogre::Entity* entity;
    Ogre::SceneManager* physscenemanager;
    Ogre::SceneNode* node;

    btQuaternion* physorientation;
    btTransform* physlocation;

	public:
	~ActorBase ();
	ActorBase (PhysReal x, PhysReal y, PhysReal z);
	void CreateEntity(PhysString name, PhysString file, PhysString group);
	void CreateSceneNode();
    void SetOgreLocation(PhysReal x, PhysReal y, PhysReal z);
    //void SetOgreOrientation();
};

class ActorDynRigid: public ActorBase {

	private:

	btRigidBody* physrigidbody;
	btMotionState* physmotionstate;

	public:
	ActorDynRigid();
	~ActorDynRigid();
	void CreateRigidObject ();
	void AddObjectToWorld ();
};

class ActorDynSoft: public ActorBase {

	private:

    btSoftBody* physoftbody;
    btMotionState* physmotionstate;

	public:
	ActorDynSoft();
	~ActorDynSoft();
    void CreateSoftObject ();
	void AddObjectToWorld ();
};

class ActorSta: public ActorBase {

    private:

    btRigidBody* physrigidbody;

    public:
    ActorSta();
    ~ActorSta();
    void CreateRigidObject ();
	void AddObjectToWorld ();
};

#endif
