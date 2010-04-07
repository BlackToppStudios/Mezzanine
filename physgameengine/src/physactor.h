//© Copyright 2010 Joseph Toppi and John Blackwood
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _physactor_h
#define _physactor_h

#include "physvector.h"
#include "physquaternion.h"
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

class PhysMotionState;

///////////////////////////////////
// Actual code

class ActorBase {
    private:
        friend class PhysWorld;

	protected:
        //abstraction for other libraries
        Ogre::Entity* entity;
        Ogre::SceneManager* physscenemanager;
        Ogre::SceneNode* node;

        btCollisionShape* Shape;

        PhysMotionState* MotionState;

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
        //btMotionState* physmotionstate;
        void AddObjectToWorld (PhysWorld *TargetWorld, btDiscreteDynamicsWorld* TargetPhysicsWorld);

	public:
        ActorDynRigid(PhysReal mass, PhysString name, PhysString file);
        virtual ~ActorDynRigid();
        void CreateRigidObject (PhysReal pmass);
};

class ActorDynSoft: public ActorBase {

	protected:
        btSoftBody* physoftbody;
        //btMotionState* physmotionstate;
        void AddObjectToWorld (PhysWorld *TargetWorld, btDiscreteDynamicsWorld* TargetPhysicsWorld);

	public:
        ActorDynSoft();
        virtual ~ActorDynSoft();
        void CreateSoftObject ();
};

class ActorSta: public ActorBase {

    protected:
        btRigidBody* physrigidbody;
        void AddObjectToWorld (PhysWorld *TargetWorld, btDiscreteDynamicsWorld* TargetPhysicsWorld);

    public:
        ActorSta(PhysString name, PhysString file);
        virtual ~ActorSta();
        void CreateRigidObject ();
};

#endif
