#ifndef _physactor_h
#define _physactor_h

#include "physvector.h"
#include "physworld.h"

///////////////////////////////////
// Forward Declares

class btMotionState;
class btQuaternion;
class btRigidBody;
class btSoftBody;
class btTransform;
class btDiscreteDynamicsWorld;
class btDefaultMotionState;
class btCollisionShape;

namespace Ogre
{
    class Entity;
    class SceneManager;
    class SceneNode;
}

///////////////////////////////////
// Actual code

class ActorBase {
    private:
        friend class PhysWorld;

	protected:
        //PhysQuaternion orientation;

        //abstraction for other libraries
        Ogre::Entity* entity;
        Ogre::SceneManager* physscenemanager;
        Ogre::SceneNode* node;

        //btQuaternion* physorientation;
        //btTransform* physlocation;
        btDefaultMotionState* MotionState;
        btCollisionShape* Shape;

        virtual void AddObjectToWorld (PhysWorld *TargetWorld, btDiscreteDynamicsWorld* TargetPhysicsWorld) = 0;


        //Ogre Management Functions
        void CreateEntity(PhysString name, PhysString file, PhysString group);
        void CreateSceneNode();
        void SetOgreLocation(PhysVector3 Place);
        void SetOgreOrientation(PhysReal x, PhysReal y, PhysReal z, PhysReal w);

        //Bullet Management
        void SetBulletLocation (PhysVector3 Location);

	public:
        ~ActorBase ();
        ActorBase (PhysString name, PhysString file);

        void SetLocation(PhysReal x, PhysReal y, PhysReal z);
        void SetLocation(PhysVector3 Place);

        void SetOrientation(PhysReal x, PhysReal y, PhysReal z, PhysReal w);
        void SetOrientation(PhysQuaternion Rotation);

        void AttachToGraphics();
};

class ActorDynRigid: public ActorBase {

	protected:
        btRigidBody* physrigidbody;
        btMotionState* physmotionstate;
        void AddObjectToWorld (PhysWorld *TargetWorld, btDiscreteDynamicsWorld* TargetPhysicsWorld);

	public:
        ActorDynRigid();
        ~ActorDynRigid();
        void CreateRigidObject ();
};

class ActorDynSoft: public ActorBase {

	protected:
        btSoftBody* physoftbody;
        btMotionState* physmotionstate;
        void AddObjectToWorld (PhysWorld *TargetWorld, btDiscreteDynamicsWorld* TargetPhysicsWorld);

	public:
        ActorDynSoft();
        ~ActorDynSoft();
        void CreateSoftObject ();
};

class ActorSta: public ActorBase {

    protected:
        btRigidBody* physrigidbody;
        void AddObjectToWorld (PhysWorld *TargetWorld, btDiscreteDynamicsWorld* TargetPhysicsWorld);

    public:
        ActorSta();
        ~ActorSta();
        void CreateRigidObject ();
};

#endif
