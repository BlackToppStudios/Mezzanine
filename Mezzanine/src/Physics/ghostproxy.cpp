// © Copyright 2010 - 2017 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

The Mezzanine Engine is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The Mezzanine Engine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with The Mezzanine Engine. If not, see <http://www.gnu.org/licenses/>.
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
#ifndef _ghostproxy_cpp
#define _ghostproxy_cpp

#include "Physics/ghostproxy.h"
#include "Physics/physicsmanager.h"

#include "enumerations.h"
#include "stringtool.h"

#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

namespace Mezzanine
{
    namespace Physics
    {
        GhostProxy::GhostProxy(const UInt32 ID, PhysicsManager* Creator) :
            CollidableProxy(ID,Creator),
            PhysicsGhostBody(NULL)
        {
            this->CreateGhostObject();
        }

        GhostProxy::GhostProxy(const UInt32 ID, CollisionShape* Shape, PhysicsManager* Creator) :
            CollidableProxy(ID,Creator),
            PhysicsGhostBody(NULL)
        {
            this->CreateGhostObject();
            this->SetCollisionShape(Shape);
        }

        GhostProxy::GhostProxy(const XML::Node& SelfRoot, PhysicsManager* Creator) :
            CollidableProxy(Creator),
            PhysicsGhostBody(NULL)
        {
            this->CreateGhostObject();
            this->ProtoDeSerialize(SelfRoot);
        }

        GhostProxy::~GhostProxy()
        {
            if( this->IsActivated() ) {
                this->Deactivate();
            }

            delete this->PhysicsGhostBody;
        }

        void GhostProxy::CreateGhostObject()
        {
            this->PhysicsGhostBody = new btPairCachingGhostObject();
            this->PhysicsGhostBody->setCollisionFlags( this->PhysicsGhostBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE );
            this->PhysicsGhostBody->setUserPointer( this );
            this->CollisionGroup = Physics::CF_SensorFilter;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Mezzanine::ComponentType GhostProxy::GetComponentType() const
        {
            return Mezzanine::CT_Physics_GhostProxy;
        }

        void GhostProxy::Activate()
        {
            if( !this->IsActivated() ) {
                this->Manager->_GetPhysicsWorldPointer()->addCollisionObject( this->PhysicsGhostBody, this->CollisionGroup, this->CollisionMask );
            }
        }

        void GhostProxy::Deactivate()
        {
            if( this->IsActivated() ) {
                this->Manager->_GetPhysicsWorldPointer()->removeCollisionObject( this->PhysicsGhostBody );
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Overlapping Proxy Access

        CollidableProxy* GhostProxy::GetAABBOverlappingProxy(const UInt32 Index)
        {
            btCollisionObject* OverlappingObject = this->PhysicsGhostBody->getOverlappingObject( static_cast<int>( Index ) );
            return static_cast<CollidableProxy*>( OverlappingObject->getUserPointer() );
        }

        UInt32 GhostProxy::GetNumAABBOverlappingProxies() const
        {
            return static_cast<UInt32>( this->PhysicsGhostBody->getNumOverlappingObjects() );
        }

        CollidableProxy* GhostProxy::GetShapeOverlappingProxy(const UInt32 Index)
        {
            btCollisionWorld* PhysWorld = this->Manager->_GetPhysicsWorldPointer();

            btBroadphasePairArray& PairArray = this->PhysicsGhostBody->getOverlappingPairCache()->getOverlappingPairArray();
            const btBroadphasePair& CachePair = PairArray[Index];
            btBroadphasePair* WorldPair = PhysWorld->getPairCache()->findPair(CachePair.m_pProxy0,CachePair.m_pProxy1);

            if( WorldPair != NULL )
            {
                btManifoldArray ManifoldArray;
                ManifoldArray.clear();

                if( WorldPair->m_algorithm )
                    WorldPair->m_algorithm->getAllContactManifolds(ManifoldArray);

                for( int ManifoldIndex = 0 ; ManifoldIndex < ManifoldArray.size() ; ++ManifoldIndex )
                {
                    btPersistentManifold* Manifold = ManifoldArray[ManifoldIndex];
                    for( int ContactIndex = 0 ; ContactIndex < Manifold->getNumContacts() ; ++ContactIndex )
                    {
                        // const btManifoldPoint& pt = Manifold->getContactPoint(ContactIndex);
                        //if( pt.m_distance1 > 0 )
                        //    return NULL;

                        btCollisionObject* ColObj = ( Manifold->getBody0() != this->PhysicsGhostBody ? (btCollisionObject*)(Manifold->getBody0()) : (btCollisionObject*)(Manifold->getBody1()) );
                        return static_cast<CollidableProxy*>( ColObj->getUserPointer() );
                    }
                }
            }
            return NULL;
        }

        UInt32 GhostProxy::GetNumShapeOverlappingProxies() const
        {
            btBroadphasePairArray& PairArray = this->PhysicsGhostBody->getOverlappingPairCache()->getOverlappingPairArray();
            return static_cast<UInt32>( PairArray.size() );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void GhostProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
            { this->CollidableProxy::ProtoSerializeProperties(SelfRoot); }

        void GhostProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
            { this->CollidableProxy::ProtoDeSerializeProperties(SelfRoot); }

        String GhostProxy::GetDerivedSerializableName() const
            { return GhostProxy::GetSerializableName(); }

        String GhostProxy::GetSerializableName()
            { return "GhostProxy"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        btPairCachingGhostObject* GhostProxy::_GetPhysicsObject() const
            { return this->PhysicsGhostBody; }

        btCollisionObject* GhostProxy::_GetBasePhysicsObject() const
            { return this->PhysicsGhostBody; }
    }//Physics
}//Mezzanine

#endif
