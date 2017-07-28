// © Copyright 2010 - 2017 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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
#ifndef _areaeffect_cpp
#define _areaeffect_cpp

#include "areaeffect.h"

#include "Physics/physicsmanager.h"
#include "Physics/collisionshape.h"
#include "Physics/collisionshapemanager.h"
#include "Physics/ghostproxy.h"

#include "Graphics/mesh.h"
#include "Graphics/meshmanager.h"
#include "Graphics/itemproxy.h"
#include "Graphics/particlesystemproxy.h"
#include "Graphics/scenemanager.h"

#include "serialization.h"
#include "exception.h"
#include "entresol.h"
#include "world.h"

#include <algorithm>

#ifdef GetObject
#undef GetObject
#endif

namespace Mezzanine
{
    AreaEffect::AreaEffect(World* TheWorld) :
        Entity(TheWorld),
        SyncTarget(nullptr)
        { this->CreateAreaEffect(nullptr); }

    AreaEffect::AreaEffect(const EntityID& EntID, World* TheWorld) :
        Entity(EntID,TheWorld),
        SyncTarget(nullptr)
        { this->CreateAreaEffect(nullptr); }

    AreaEffect::AreaEffect(const EntityID& EntID, Physics::CollisionShape* Shape, World* TheWorld) :
        Entity(EntID,TheWorld),
        SyncTarget(nullptr)
        { this->CreateAreaEffect(Shape); }

    AreaEffect::~AreaEffect()
        { this->DestroyAreaEffect(); }

    void AreaEffect::CreateAreaEffect(Physics::CollisionShape* Shape)
    {
        Physics::GhostProxy* Ghost = nullptr;
        Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( this->ParentWorld->GetManager(ManagerBase::MT_PhysicsManager) );
        if( PhysMan != nullptr ) {
            if( Shape == nullptr ) {
                Ghost = PhysMan->CreateGhostProxy();
            }else{
                Ghost = PhysMan->CreateGhostProxy(Shape);
            }
            this->AddComponent(Ghost);
            this->SetPrimaryProxy(Ghost);

            UInt16 ColGroup = Physics::CF_SensorFilter;
            UInt16 ColMask = Physics::CF_AllFilter & ~(Physics::CF_SensorFilter | Physics::CF_StaticFilter);
            Ghost->SetCollisionGroupAndMask(ColGroup,ColMask);
            Ghost->_Bind(this);
        }
    }

    void AreaEffect::DestroyAreaEffect()
    {
        this->RemoveFromWorld();
        this->DestroyAllComponents();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Mezzanine::EntityType AreaEffect::GetType() const
        { return Mezzanine::ET_AreaEffectUnknown; }

    Physics::GhostProxy* AreaEffect::GetGhostProxy() const
        { return static_cast<Physics::GhostProxy*>( this->GetComponent(Mezzanine::CT_Physics_GhostProxy,0) ); }

    void AreaEffect::ApplyEffect()
    {
        if( this->SyncTarget != nullptr ) {
            this->SetTransform( this->SyncTarget->GetTransform() );
        }
    }

    void AreaEffect::SetSyncTarget(Entity* ToSync)
        { this->SyncTarget = ToSync; }

    Entity* AreaEffect::GetSyncTarget() const
        { return this->SyncTarget; }

    ///////////////////////////////////////////////////////////////////////////////
    // Overlapping Object Management

    UInt32 AreaEffect::GetNumOverlappingObjects() const
        { return this->OverlappingObjects.size(); }

    UInt32 AreaEffect::GetNumAddedObjects() const
        { return this->AddedObjects.size(); }

    UInt32 AreaEffect::GetNumRemovedObjects() const
        { return this->RemovedObjects.size(); }

    AreaEffect::ObjectContainer& AreaEffect::GetOverlappingObjects()
        { return this->OverlappingObjects; }

    AreaEffect::ObjectContainer& AreaEffect::GetAddedObjects()
        { return this->AddedObjects; }

    AreaEffect::ObjectContainer& AreaEffect::GetRemovedObjects()
        { return this->RemovedObjects; }

    ///////////////////////////////////////////////////////////////////////////////
    // AreaEffect Properties

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void AreaEffect::ProtoSerializeProperties(XML::Node& SelfRoot) const
    {
        this->Entity::ProtoSerializeProperties(SelfRoot);
    }

    void AreaEffect::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
    {
        this->Entity::ProtoDeSerializeProperties(SelfRoot);
    }

    String AreaEffect::GetDerivedSerializableName() const
        { return AreaEffect::GetSerializableName(); }

    String AreaEffect::GetSerializableName()
        { return "AreaEffect"; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void AreaEffect::_Update(const Whole Delta)
    {
        if( this->PrimaryProxy->GetComponentType() != Mezzanine::CT_Physics_GhostProxy )
            return;

        this->AddedObjects.clear();
        this->RemovedObjects.clear();

        Physics::GhostProxy* Ghost = static_cast<Physics::GhostProxy*>( this->PrimaryProxy );
        ObjectContainer PrevOverlapping;
        PrevOverlapping.swap( this->OverlappingObjects );

        const UInt32 NumProxies = Ghost->GetNumShapeOverlappingProxies();
        for( UInt32 ProxIndex = 0 ; ProxIndex < NumProxies ; ++ProxIndex )
        {
            Physics::CollidableProxy* CurrProxy = Ghost->GetShapeOverlappingProxy( ProxIndex );
            Entity* CurrObject = ( CurrProxy ? CurrProxy->GetParentEntity() : nullptr );
            if( CurrObject != nullptr ) {
                // We need to check for unique world objects just in case a world object contains multiple collidable proxies
                ObjectIterator UniqueCheck = std::find( this->OverlappingObjects.begin(), this->OverlappingObjects.end(), CurrObject );
                if( UniqueCheck == this->OverlappingObjects.end() ) {
                    // We've established that this object is unique, so lets try to remove it from our temporary vector
                    ObjectIterator PrevCheck = std::find( PrevOverlapping.begin(), PrevOverlapping.end(), CurrObject );
                    if( PrevCheck == PrevOverlapping.end() ) {
                        // If we've failed to find it, then it's new
                        this->OverlappingObjects.push_back( CurrObject );
                        this->AddedObjects.push_back( CurrObject );
                    }else{
                        // If it was in the previous frame it isn't new, but it may be removed
                        this->OverlappingObjects.push_back( CurrObject );
                        PrevOverlapping.erase( PrevCheck );
                    }
                }
            }
        }

        // If we have anything left over in our prev-overlapping, it's not inside this anymore so put them in the removed vector
        if( !PrevOverlapping.empty() ) {
            this->RemovedObjects.swap( PrevOverlapping );
        }
    }
}

#endif
