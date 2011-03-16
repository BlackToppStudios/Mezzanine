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
    {
        this->GameWorld = World::GetWorldPointer();
        this->node = this->GameWorld->GetSceneManager()->GetGraphicsWorldPointer()->createSceneNode();
        this->GameWorld->GetSceneManager()->GetGraphicsWorldPointer()->getRootSceneNode()->addChild(this->node);
        this->Shape = new btEmptyShape();
        this->CreateEntity(name, file, group);
        this->GraphicsSettings = new ActorGraphicsSettings(this,entity);
        MotionState = NULL;
        ActorSounds = NULL;
        Animation = NULL;
        ShapeIsSaved = false;
        ActorType = ActorBase::Actorbase;
    }

    ActorBase::~ActorBase ()
    {
        DetachFromGraphics();
        delete MotionState;
        if(!ShapeIsSaved)
        {
            delete Shape;
        }
        //delete entity;
        this->GameWorld->GetSceneManager()->GetGraphicsWorldPointer()->destroyEntity(entity);
        //delete node;
        this->GameWorld->GetSceneManager()->GetGraphicsWorldPointer()->destroySceneNode(node);
        if(CollisionObject)
        {
            delete CollisionObject;
        }
    }

    ///////////////////////////////////
    // ActorBase Private misc functions
    Ogre::ResourceGroupManager* ActorBase::GetOgreResourceManager()
    {
        return Ogre::ResourceGroupManager::getSingletonPtr();
    }

    ///////////////////////////////////
    // ActorBase Constructor functions

    void ActorBase::CreateEntity (String name, String file, String group)
    {
        this->entity = this->GameWorld->GetSceneManager()->GetGraphicsWorldPointer()->createEntity(name, file, group);
    }

    void ActorBase::CreateSceneNode ()
    {
        this->node = this->GameWorld->GetSceneManager()->GetGraphicsWorldPointer()->createSceneNode();
    }

    void ActorBase::CreateMotionState(Ogre::SceneNode* Node)
    {
        this->MotionState = new internal::PhysMotionState(Node);
    }

    void ActorBase::CreateMotionState(Ogre::SceneNode* Node, Vector3 InitPosition)
    {
        this->MotionState = new internal::PhysMotionState(Node);
        this->MotionState->SetPosition(InitPosition);
    }

    void ActorBase::CreateMotionState(Ogre::SceneNode* Node, Vector3 InitPosition, Quaternion InitOrientation)
    {
        this->MotionState = new internal::PhysMotionState(Node);
        this->MotionState->SetPosition(InitPosition);
        this->MotionState->SetOrientation(InitOrientation);
    }

    ///////////////////////////////////
    // ActorBase Private Location Functions

    void ActorBase::SetOgreLocation (Vector3 Location)
    {
        this->node->setPosition(Location.GetOgreVector3());
    }

    Vector3 ActorBase::GetOgreLocation() const
    {
        Vector3 temp;
        temp.ExtractOgreVector3(this->node->getPosition());
        return temp;
    }

    void ActorBase::SetBulletLocation (Vector3 Location)
    {
        //btTransform* temp = this->CollisionObject->getWorldTransform();
        this->CollisionObject->getWorldTransform().setOrigin(Location.GetBulletVector3());
    }

    Vector3 ActorBase::GetBulletLocation() const
    {
        Vector3 temp;
        //btTransform trans = this->CollisionObject->getWorldTransform();
        temp.ExtractBulletVector3(this->CollisionObject->getWorldTransform().getOrigin());
        return temp;
    }

    ///////////////////////////////////
    // ActorBase Private Orientation functions

    void ActorBase::SetOgreOrientation (Quaternion Rotation)
    {
        this->node->setOrientation(Rotation.GetOgreQuaternion());
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
        Vector3 tempv;
        Quaternion tempq;
        tempv = GetBulletLocation();
        tempq.ExtractBulletQuaternion(CollisionObject->getWorldTransform().getRotation());
        this->node->setPosition(tempv.GetOgreVector3());
        this->node->setOrientation(tempq.GetOgreQuaternion());
        this->node->attachObject(this->entity);
    }

    void ActorBase::DetachFromGraphics ()
    {
        this->node->detachObject(this->entity);
    }

    void ActorBase::SetActorScaling(Vector3 scaling)
    {
        this->node->setScale(scaling.GetOgreVector3());
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
        Animation = entity->getAnimationState(AnimationName);
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
        //int x=2;
        this->CollisionObject->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
    }

    void ActorBase::SetStatic()
    {
        //int x=1;
        this->CollisionObject->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    }

    bool ActorBase::IsStaticOrKinematic()
    {
        //bool result = this->CollisionObject->isStaticOrKinematicObject();
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
        return entity;
    }
}// /phys

std::ostream& operator << (std::ostream& stream, const phys::ActorBase& x)
{
    stream << "[" << x.GetName() << " at:" << x.GetLocation() << "]";
    return stream;
}

#endif
