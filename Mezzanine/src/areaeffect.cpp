//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#include "Graphics/scenemanager.h"

#include "serialization.h"
#include "exception.h"
#include "entresol.h"

#include <algorithm>

#ifdef GetObject
#undef GetObject
#endif

namespace Mezzanine
{
    AreaEffect::AreaEffect(World* TheWorld) :
        WorldObject(TheWorld),
        Ghost(NULL)
        { this->CreateAreaEffect(); }

    AreaEffect::AreaEffect(const String& Name, World* TheWorld) :
        WorldObject(Name,TheWorld),
        Ghost(NULL)
        { this->CreateAreaEffect(); }

    AreaEffect::~AreaEffect()
        { this->DestroyAreaEffect(); }

    void AreaEffect::CreateAreaEffect()
    {
        Physics::PhysicsManager* PhysMan = Entresol::GetSingletonPtr()->GetPhysicsManager();
        if( PhysMan != NULL ) {
            this->Ghost = PhysMan->CreateGhostProxy();

            UInt16 ColGroup = Physics::CF_SensorFilter;
            UInt16 ColMask = Physics::CF_AllFilter & ~(Physics::CF_SensorFilter | Physics::CF_StaticFilter);
            this->Ghost->SetCollisionGroupAndMask(ColGroup,ColMask);
        }
    }

    void AreaEffect::DestroyAreaEffect()
    {
        this->RemoveFromWorld();
        Physics::PhysicsManager* PhysMan = Entresol::GetSingletonPtr()->GetPhysicsManager();
        if( this->Ghost != NULL && PhysMan != NULL ) {
            PhysMan->DestroyProxy( this->Ghost );
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Mezzanine::WorldObjectType AreaEffect::GetType() const
        { return Mezzanine::WO_AreaEffectUnknown; }

    Bool AreaEffect::IsInWorld() const
        { return this->Ghost->IsInWorld(); }

    Bool AreaEffect::IsStatic() const
        { return this->Ghost->IsStatic(); }

    Bool AreaEffect::IsKinematic() const
        { return this->Ghost->IsKinematic(); }

    void AreaEffect::GetProxies(ProxyContainer& Proxies)
    {
        Proxies.clear();
        Proxies.push_back( this->Ghost );
    }

    void AreaEffect::GetProxies(const UInt32 Types, ProxyContainer& Proxies)
    {
        Proxies.clear();
        if( Types & Mezzanine::PT_Physics_GhostProxy ) {
            Proxies.push_back( this->Ghost );
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Working with the World

    void AreaEffect::AddToWorld()
    {
        this->Ghost->AddToWorld();
    }

    void AreaEffect::RemoveFromWorld()
    {
        this->Ghost->RemoveFromWorld();
    }

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
    // Transform Methods

    void AreaEffect::SetLocation(const Vector3& Loc)
    {
        this->Ghost->SetLocation(Loc);
    }

    void AreaEffect::SetLocation(const Real X, const Real Y, const Real Z)
    {
        this->Ghost->SetLocation(X,Y,Z);
    }

    Vector3 AreaEffect::GetLocation() const
    {
        return this->Ghost->GetLocation();
    }

    void AreaEffect::SetOrientation(const Quaternion& Ori)
    {
        this->Ghost->SetOrientation(Ori);
    }

    void AreaEffect::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
    {
        this->Ghost->SetOrientation(X,Y,Z,W);
    }

    Quaternion AreaEffect::GetOrientation() const
    {
        return this->Ghost->GetOrientation();
    }

    void AreaEffect::SetScale(const Vector3& Sc)
    {
        this->Ghost->SetScale(Sc);
    }

    void AreaEffect::SetScale(const Real X, const Real Y, const Real Z)
    {
        this->Ghost->SetScale(X,Y,Z);
    }

    Vector3 AreaEffect::GetScale() const
    {
        return this->Ghost->GetScale();
    }

    void AreaEffect::Translate(const Vector3& Trans)
    {
        this->Ghost->Translate(Trans);
    }

    void AreaEffect::Translate(const Real X, const Real Y, const Real Z)
    {
        this->Ghost->Translate(X,Y,Z);
    }

    void AreaEffect::Yaw(const Real Angle)
    {
        this->Ghost->Yaw(Angle);
    }

    void AreaEffect::Pitch(const Real Angle)
    {
        this->Ghost->Pitch(Angle);
    }

    void AreaEffect::Roll(const Real Angle)
    {
        this->Ghost->Roll(Angle);
    }

    void AreaEffect::Rotate(const Vector3& Axis, const Real Angle)
    {
        this->Ghost->Rotate(Axis,Angle);
    }

    void AreaEffect::Rotate(const Quaternion& Rotation)
    {
        this->Ghost->Rotate(Rotation);
    }

    void AreaEffect::Scale(const Vector3& Scale)
    {
        this->Ghost->Scale(Scale);
    }

    void AreaEffect::Scale(const Real X, const Real Y, const Real Z)
    {
        this->Ghost->Scale(X,Y,Z);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void AreaEffect::ProtoSerializeProperties(XML::Node& SelfRoot) const
    {
        this->WorldObject::ProtoSerializeProperties(SelfRoot);
    }

    void AreaEffect::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
    {
        this->WorldObject::ProtoDeSerializeProperties(SelfRoot);
    }

    String AreaEffect::GetDerivedSerializableName() const
        { return AreaEffect::GetSerializableName(); }

    String AreaEffect::GetSerializableName()
        { return "AreaEffect"; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void AreaEffect::_Update()
    {
        this->AddedObjects.clear();
        this->RemovedObjects.clear();

        ObjectContainer PrevOverlapping;
        PrevOverlapping.swap( this->OverlappingObjects );

        const UInt32 NumProxies = this->Ghost->GetNumShapeOverlappingProxies();
        for( UInt32 ProxIndex = 0 ; ProxIndex < NumProxies ; ++ProxIndex )
        {
            Physics::CollidableProxy* CurrProxy = this->Ghost->GetShapeOverlappingProxy( ProxIndex );
            if( CurrProxy != NULL ) {
                WorldObject* CurrObject = CurrProxy->GetParentObject();
                if( CurrObject != NULL ) {
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
        }

        // If we have anything left over in our prev-overlapping, it's not inside this anymore so put them in the removed vector
        if( !PrevOverlapping.empty() ) {
            this->RemovedObjects.swap( PrevOverlapping );
        }
    }

    void AreaEffect::_NotifyProxyDestroyed(WorldProxy* ToBeDestroyed)
    {
        if( ToBeDestroyed == NULL )
            return;

        if( this->Ghost == ToBeDestroyed ) {
            Physics::PhysicsManager* PhysMan = Entresol::GetSingletonPtr()->GetPhysicsManager();
            if( PhysMan != NULL ) {
                PhysMan->DestroyProxy( this->Ghost );
                this->Ghost = NULL;
            }
        }
    }

    ///////////////////////////////////
    // TestAE functions

    TestAE::TestAE(World* TheWorld) :
        AreaEffect(TheWorld)
        {  }

    TestAE::TestAE(const String& Name, World* TheWorld) :
        AreaEffect(Name,TheWorld)
        {  }

    TestAE::~TestAE()
        {  }

    void TestAE::ApplyEffect()
    {
        Entresol* TheEntresol = Entresol::GetSingletonPtr();
        ObjectIterator ObjIt;

        if ( !AddedObjects.empty() ) {
            TheEntresol->Log("Objects Added to field this frame:");
            for ( ObjIt = AddedObjects.begin() ; ObjIt != AddedObjects.end() ; ObjIt++ )
                { TheEntresol->Log( (*ObjIt) ); }
        }

        if ( !RemovedObjects.empty() ) {
            TheEntresol->Log("Objects Removed from field this frame:");
            for ( ObjIt = RemovedObjects.begin() ; ObjIt != RemovedObjects.end() ; ObjIt++ )
                { TheEntresol->Log( (*ObjIt) ); }
        }

        if ( !OverlappingObjects.empty() ) {
            TheEntresol->Log("Objects Currently in field this frame:");
            for ( ObjIt = OverlappingObjects.begin() ; ObjIt != OverlappingObjects.end() ; ObjIt++ )
                { TheEntresol->Log( (*ObjIt) ); }
        }
    }
}

#endif
