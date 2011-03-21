//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _physactorbase_cpp
#define _physactorbase_cpp

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"

#include "actorbase.h"
#include "actorrigid.h"
#include "actorterrain.h"
#include "actorsoft.h"
#include "actorgraphicssettings.h"
#include "world.h"
#include "internalmotionstate.h.cpp"
#include "internalmeshtools.h.cpp"

namespace phys{
    ///////////////////////////////////
    // ActorBase class fuctions
    ActorBase::ActorBase (String name, String file, String group)
        : GraphicsObject(NULL),
          MotionState(NULL),
          ActorSounds(NULL),
          Animation(NULL),
          ShapeIsSaved(false),
          ActorType(ActorBase::Actorbase)
    {
        this->GameWorld = World::GetWorldPointer();
        this->GraphicsNode = this->GameWorld->GetSceneManager()->GetGraphicsWorldPointer()->createSceneNode();
        this->GameWorld->GetSceneManager()->GetGraphicsWorldPointer()->getRootSceneNode()->addChild(this->GraphicsNode);
        this->Shape = new btEmptyShape();
        this->GraphicsSettings = new ActorGraphicsSettings(this,GraphicsObject);
    }

    ActorBase::~ActorBase ()
    {
        DetachFromGraphics();
        delete MotionState;
        if(!ShapeIsSaved)
        {
            delete Shape;
        }
        //delete GraphicsObject;
        this->GameWorld->GetSceneManager()->GetGraphicsWorldPointer()->destroyEntity(GraphicsObject);
        //delete GraphicsNode;
        this->GameWorld->GetSceneManager()->GetGraphicsWorldPointer()->destroySceneNode(GraphicsNode);
        if(CollisionObject)
        {
            delete CollisionObject;
        }
    }

    ///////////////////////////////////
    // ActorBase Private Location Functions

    void ActorBase::SetOgreLocation (Vector3 Location)
    {
        this->GraphicsNode->setPosition(Location.GetOgreVector3());
    }

    Vector3 ActorBase::GetOgreLocation() const
    {
        Vector3 temp(this->GraphicsNode->getPosition());
        return temp;
    }

    void ActorBase::SetBulletLocation (Vector3 Location)
    {
        //btTransform* temp = this->CollisionObject->getWorldTransform();
        this->CollisionObject->getWorldTransform().setOrigin(Location.GetBulletVector3());
    }

    Vector3 ActorBase::GetBulletLocation() const
    {
        Vector3 temp(this->CollisionObject->getWorldTransform().getOrigin());
        return temp;
    }

    ///////////////////////////////////
    // ActorBase Private Orientation functions

    void ActorBase::SetOgreOrientation (Quaternion Rotation)
    {
        this->GraphicsNode->setOrientation(Rotation.GetOgreQuaternion());
    }

    void ActorBase::SetBulletOrientation (Quaternion Rotation)
    {
        this->CollisionObject->getWorldTransform().setRotation(Rotation.GetBulletQuaternion(true));
    }

    ///////////////////////////////////
    // ActorBase Public Location functions

    void ActorBase::SetLocation (Real x, Real y, Real z)
    {
        Vector3 temp(x,y,z);
        this->SetLocation(temp);
    }

    void ActorBase::SetLocation (Vector3 Place)
    {
        this->SetBulletLocation(Place);
        this->SetOgreLocation(Place);
    }

    Vector3 ActorBase::GetLocation() const
    {
        return this->GetBulletLocation();
    }

    void ActorBase::SetInitLocation(Vector3 Location)
    {
        this->SetBulletLocation(Location);
        this->SetOgreLocation(Location);
    }

    ///////////////////////////////////
    // ActorBase Public Orientation functions

    void ActorBase::SetInitOrientation(Quaternion Orientation)
    {
        this->SetBulletOrientation(Orientation);
    }

    void ActorBase::SetOrientation (Real x, Real y, Real z, Real w)
    {
        Quaternion temp(x,y,z,w);
        this->SetOrientation(temp);
    }

    void ActorBase::SetOrientation (Quaternion Rotation)
    {
        this->SetBulletOrientation(Rotation);
        this->SetOgreOrientation(Rotation);
    }

    ///////////////////////////////////
    // ActorBase Public Misc Functions

    void ActorBase::AttachToGraphics ()
    {
        Vector3 tempv(CollisionObject->getWorldTransform().getOrigin());
        Quaternion tempq(CollisionObject->getWorldTransform().getRotation());
        this->GraphicsNode->setPosition(tempv.GetOgreVector3());
        this->GraphicsNode->setOrientation(tempq.GetOgreQuaternion());
        this->GraphicsNode->attachObject(this->GraphicsObject);
    }

    void ActorBase::DetachFromGraphics ()
    {
        this->GraphicsNode->detachObject(this->GraphicsObject);
    }

    void ActorBase::SetActorScaling(Vector3 scaling)
    {
        this->GraphicsNode->setScale(scaling.GetOgreVector3());
        this->Shape->setLocalScaling(scaling.GetBulletVector3());
    }

    int ActorBase::GetType()
    {
        return this->ActorType;
    }

    const bool ActorBase::GetShapeIsSaved()
    {
        return ShapeIsSaved;
    }

    bool ActorBase::IsInWorld()
    {
        return CollisionObject->getBroadphaseHandle() != 0;
    }

    void ActorBase::SetBasicCollisionParams(Real Friction, Real Restitution)
    {
        this->CollisionObject->setFriction(Friction);
        this->CollisionObject->setRestitution(Restitution);
    }

    void ActorBase::SetAnimation(ConstString &AnimationName, bool Loop)
    {
        if(this->IsAnimated())
        {
            Animation->setEnabled(false);
        }
        Animation = GraphicsObject->getAnimationState(AnimationName);
        Animation->setLoop(Loop);
    }

    void ActorBase::EnableAnimation(bool Enable)
    {
        if(Animation)
        {
            Animation->setEnabled(Enable);
        }
    }

    bool ActorBase::IsAnimated()
    {
        if(Animation)
        {
            return Animation->getEnabled();
        }else{
            return false;
        }
    }

    void ActorBase::AdvanceAnimation(Real Time)
    {
        if(Animation)
        {
            Animation->addTime(Time);
        }
    }

    void ActorBase::RemoveSetAnimation()
    {
        if(Animation)
        {
            Animation->setEnabled(false);
            Animation = NULL;
        }
    }

    ActorGraphicsSettings* ActorBase::GetGraphicsSettings()
    {
        return GraphicsSettings;
    }

    ///////////////////////////////////
    // ActorBase Public Collision flag functions
    void ActorBase::SetKinematic()
    {
        this->CollisionObject->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
    }

    void ActorBase::SetStatic()
    {
        this->CollisionObject->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    }

    bool ActorBase::IsStaticOrKinematic()
    {
        return this->CollisionObject->isStaticOrKinematicObject();
    }

    void ActorBase::EnableCollisionResponse()
    {
        switch (ActorType)
        {
            case ActorBase::Actorrigid:
            {
                ActorRigid* Rigid = static_cast<ActorRigid*> (this);
                Rigid->RemoveObjectFromWorld(GameWorld);
                this->CollisionObject->setCollisionFlags(CollisionObject->getCollisionFlags() - btCollisionObject::CF_NO_CONTACT_RESPONSE);
                Rigid->AddObjectToWorld(GameWorld);
                break;
            }
            case ActorBase::Actorsoft:
            {
                ActorSoft* Soft = static_cast<ActorSoft*> (this);
                Soft->RemoveObjectFromWorld(GameWorld);
                this->CollisionObject->setCollisionFlags(CollisionObject->getCollisionFlags() - btCollisionObject::CF_NO_CONTACT_RESPONSE);
                Soft->AddObjectToWorld(GameWorld);
                break;
            }
            case ActorBase::Actorterrain:
            {
                ActorTerrain* Terrain = static_cast<ActorTerrain*> (this);
                Terrain->RemoveObjectFromWorld(GameWorld);
                this->CollisionObject->setCollisionFlags(CollisionObject->getCollisionFlags() - btCollisionObject::CF_NO_CONTACT_RESPONSE);
                Terrain->AddObjectToWorld(GameWorld);
                break;
            }
            case ActorBase::Actorragdoll:
            {
                return;
                break;
            }
            default:
                return;
        }
    }

    void ActorBase::DisableCollisionResponse()
    {
        switch (ActorType)
        {
            case ActorBase::Actorrigid:
            {
                ActorRigid* Rigid = static_cast<ActorRigid*> (this);
                Rigid->RemoveObjectFromWorld(GameWorld);
                this->CollisionObject->setCollisionFlags(CollisionObject->getCollisionFlags() + btCollisionObject::CF_NO_CONTACT_RESPONSE);
                Rigid->AddObjectToWorld(GameWorld);
                break;
            }
            case ActorBase::Actorsoft:
            {
                ActorSoft* Soft = static_cast<ActorSoft*> (this);
                Soft->RemoveObjectFromWorld(GameWorld);
                this->CollisionObject->setCollisionFlags(CollisionObject->getCollisionFlags() + btCollisionObject::CF_NO_CONTACT_RESPONSE);
                Soft->AddObjectToWorld(GameWorld);
                break;
            }
            case ActorBase::Actorterrain:
            {
                ActorTerrain* Terrain = static_cast<ActorTerrain*> (this);
                Terrain->RemoveObjectFromWorld(GameWorld);
                this->CollisionObject->setCollisionFlags(CollisionObject->getCollisionFlags() + btCollisionObject::CF_NO_CONTACT_RESPONSE);
                Terrain->AddObjectToWorld(GameWorld);
                break;
            }
            case ActorBase::Actorragdoll:
            {
                return;
                break;
            }
            default:
                return;
        }
    }

    bool ActorBase::CheckActivation()
    {
        int Activation = this->CollisionObject->getActivationState();
        if( ACTIVE_TAG == Activation )
        {
            return true;
        }else{
            return false;
        }
    }

    Ogre::Entity* ActorBase::GetOgreObject()
    {
        return GraphicsObject;
    }
}// /phys

std::ostream& operator << (std::ostream& stream, const phys::ActorBase& x)
{
    stream << "[" << x.GetName() << " at:" << x.GetLocation() << "]";
    return stream;
}

#endif
