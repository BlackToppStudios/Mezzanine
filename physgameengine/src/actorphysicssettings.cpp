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
#ifndef _actorphysicssettings_cpp
#define _actorphysicssettings_cpp

#include "actorphysicssettings.h"
#include "actorbase.h"
#include "world.h"

#include "btBulletDynamicsCommon.h"

namespace phys
{
    ActorBasePhysicsSettings::ActorBasePhysicsSettings(ActorBase* Actor, btCollisionObject* PhysicsObject)
        : Parent(Actor),
          ActorCO(PhysicsObject)
    {
    }

    ActorBasePhysicsSettings::~ActorBasePhysicsSettings()
    {
    }

    void ActorBasePhysicsSettings::SetKinematic()
    {
        ActorCO->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
    }

    void ActorBasePhysicsSettings::SetStatic()
    {
        ActorCO->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    }

    bool ActorBasePhysicsSettings::IsStaticOrKinematic()
    {
        return ActorCO->isStaticOrKinematicObject();
    }

    void ActorBasePhysicsSettings::EnableCollisionResponse()
    {
        World* GameWorld = World::GetWorldPointer();
        switch (Parent->GetType())
        {
            case ActorBase::Actorrigid:
            /*{
                ActorRigid* Rigid = static_cast<ActorRigid*> (Parent);
                Rigid->RemoveObjectFromWorld(GameWorld);
                ActorCO->setCollisionFlags(CollisionObject->getCollisionFlags() - btCollisionObject::CF_NO_CONTACT_RESPONSE);
                Rigid->AddObjectToWorld(GameWorld);
                break;
            }// */
            case ActorBase::Actorsoft:
            /*{
                ActorSoft* Soft = static_cast<ActorSoft*> (Parent);
                Soft->RemoveObjectFromWorld(GameWorld);
                ActorCO->setCollisionFlags(CollisionObject->getCollisionFlags() - btCollisionObject::CF_NO_CONTACT_RESPONSE);
                Soft->AddObjectToWorld(GameWorld);
                break;
            }// */
            case ActorBase::Actorterrain:
            {
                //ActorTerrain* Terrain = static_cast<ActorTerrain*> (Parent);
                Parent->RemoveObjectFromWorld(GameWorld);
                ActorCO->setCollisionFlags(ActorCO->getCollisionFlags() - btCollisionObject::CF_NO_CONTACT_RESPONSE);
                Parent->AddObjectToWorld(GameWorld);
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

    void ActorBasePhysicsSettings::DisableCollisionResponse()
    {
        World* GameWorld = World::GetWorldPointer();
        switch (Parent->GetType())
        {
            case ActorBase::Actorrigid:
            /*{
                ActorRigid* Rigid = static_cast<ActorRigid*> (Parent);
                Rigid->RemoveObjectFromWorld(GameWorld);
                ActorCO->setCollisionFlags(CollisionObject->getCollisionFlags() + btCollisionObject::CF_NO_CONTACT_RESPONSE);
                Rigid->AddObjectToWorld(GameWorld);
                break;
            }// */
            case ActorBase::Actorsoft:
            /*{
                ActorSoft* Soft = static_cast<ActorSoft*> (Parent);
                Soft->RemoveObjectFromWorld(GameWorld);
                ActorCO->setCollisionFlags(CollisionObject->getCollisionFlags() + btCollisionObject::CF_NO_CONTACT_RESPONSE);
                Soft->AddObjectToWorld(GameWorld);
                break;
            }// */
            case ActorBase::Actorterrain:
            {
                //ActorTerrain* Terrain = static_cast<ActorTerrain*> (Parent);
                Parent->RemoveObjectFromWorld(GameWorld);
                ActorCO->setCollisionFlags(ActorCO->getCollisionFlags() + btCollisionObject::CF_NO_CONTACT_RESPONSE);
                Parent->AddObjectToWorld(GameWorld);
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

    bool ActorBasePhysicsSettings::CheckActivation()
    {
        int Activation = ActorCO->getActivationState();
        if( ACTIVE_TAG == Activation )
        {
            return true;
        }else{
            return false;
        }
    }
}

#endif
