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
#ifndef _physicscollision_cpp
#define _physicscollision_cpp

#include "collision.h"
#include "worldobject.h"

#include "entresol.h"
#include "stringtool.h"

#include "Physics/physicsenumerations.h"
#include "Physics/collidableproxy.h"

#include <btBulletDynamicsCommon.h>
#include <memory>

namespace Mezzanine
{
    namespace Physics
    {
        /// @internal
        /// @brief Used to insulate the interface from the implementation details for bullet.
        struct CollisionInternalData
        {
            /// @brief Wrapped Data, An array of Collision Manifolds
            btManifoldArray Manifolds;
        };

        Collision::Collision(CollidableProxy* A, CollidableProxy* B, btCollisionAlgorithm* PhysicsAlgo) :
            InternalAlgo(PhysicsAlgo),
            ProxyA(A),
            ProxyB(B)
        {
            this->InternalData = new CollisionInternalData();

            WorldObject* ObjectA = this->ProxyA->GetParentObject();
            if( ObjectA ) {
                ObjectA->_NotifyCollisionState(this,Physics::Col_Begin);
            }

            WorldObject* ObjectB = this->ProxyB->GetParentObject();
            if( ObjectB ) {
                ObjectB->_NotifyCollisionState(this,Physics::Col_Begin);
            }

            this->InternalAlgo->getAllContactManifolds(this->InternalData->Manifolds);
        }

        Collision::Collision() :
            InternalAlgo(NULL),
            ProxyA(NULL),
            ProxyB(NULL)
        {
            this->InternalData = new CollisionInternalData();
        }

        Collision::Collision(const Collision& Other) :
            InternalAlgo(Other.InternalAlgo),
            ProxyA(Other.ProxyA),
            ProxyB(Other.ProxyB)
        {
            for( Whole X = 0 ; X < Other.InternalData->Manifolds.size() ; ++X )
                this->InternalData->Manifolds.push_back(Other.InternalData->Manifolds[X]);

            // Double notifies seems like a bad idea.
            //this->ProxyA->_NotifyCollisionState(this,Physics::Col_Begin);
            //this->ProxyB->_NotifyCollisionState(this,Physics::Col_Begin);
        }

        Collision::~Collision()
        {
            WorldObject* ObjectA = this->ProxyA->GetParentObject();
            if( ObjectA ) {
                ObjectA->_NotifyCollisionState(this,Physics::Col_End);
            }

            WorldObject* ObjectB = this->ProxyB->GetParentObject();
            if( ObjectB ) {
                ObjectB->_NotifyCollisionState(this,Physics::Col_End);
            }

            delete this->InternalData;
        }

        btManifoldPoint& Collision::GetManifoldPoint(const Whole& Index)
        {
            if( static_cast<Integer>( Index ) >= this->InternalData->Manifolds.size() )
                MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Attempting to access invalid index in Collision.");

            if( Index > 3 ) {
                Whole SuperIndex = Index/4;
                Whole SubIndex = Index%4;
                return (this->InternalData->Manifolds.at(SuperIndex)->getContactPoint(SubIndex));
            }else{
                return (this->InternalData->Manifolds.at(0)->getContactPoint(Index));
            }
        }

        void Collision::UpdatePenetrationDistances()
        {
            if( this->InternalData->Manifolds.size() > this->PenetrationDistances.size() )
                this->PenetrationDistances.resize( this->InternalData->Manifolds.size() );
            this->PenetrationDistances.clear();
            for( Integer X = 0 ; X < this->InternalData->Manifolds.size() ; ++X )
            {
                this->PenetrationDistances[X] = this->GetManifoldPoint(X).m_distance1;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        CollidableProxy* Collision::GetProxyA() const
            { return this->ProxyA; }

        CollidableProxy* Collision::GetProxyB() const
            { return this->ProxyB; }

        WorldObject* Collision::GetObjectA() const
            { return this->ProxyA->GetParentObject(); }

        WorldObject* Collision::GetObjectB() const
            { return this->ProxyB->GetParentObject(); }

        bool Collision::PairsMatch(WorldObject* A, WorldObject* B) const
        {
            WorldObject* ObjA = this->GetObjectA();
            WorldObject* ObjB = this->GetObjectB();
            if( ObjA && ObjB ) {
                bool ContainsA = ( A == ObjA ) || ( A == ObjB );
                bool ContainsB = ( B == ObjA ) || ( B == ObjB );
                return (ContainsA && ContainsB);
            }else{
                return false;
            }
        }

        bool Collision::PairsMatch(CollidableProxy* A, CollidableProxy* B) const
        {
            bool ContainsA = ( A == this->GetProxyA() ) || ( A == this->GetProxyB() );
            bool ContainsB = ( B == this->GetProxyA() ) || ( B == this->GetProxyB() );
            return (ContainsA && ContainsB);
        }

        void Collision::Update()
        {
            this->InternalAlgo->getAllContactManifolds(this->InternalData->Manifolds);
            Whole NumManifolds = this->InternalData->Manifolds.size();
            if( this->PenetrationDistances.size() != NumManifolds )
            {
                this->UpdatePenetrationDistances();

                WorldObject* ObjectA = this->ProxyA->GetParentObject();
                if( ObjectA ) {
                    ObjectA->_NotifyCollisionState(this,Physics::Col_Contacts_Updated);
                }

                WorldObject* ObjectB = this->ProxyB->GetParentObject();
                if( ObjectB ) {
                    ObjectB->_NotifyCollisionState(this,Physics::Col_Contacts_Updated);
                }

                return;
            }
            for( Whole X = 0 ; X < NumManifolds ; ++X )
            {
                if( this->PenetrationDistances[X] != this->GetDistance(X) )
                {
                    this->UpdatePenetrationDistances();

                    WorldObject* ObjectA = this->ProxyA->GetParentObject();
                    if( ObjectA ) {
                        ObjectA->_NotifyCollisionState(this,Physics::Col_Contacts_Updated);
                    }

                    WorldObject* ObjectB = this->ProxyB->GetParentObject();
                    if( ObjectB ) {
                        ObjectB->_NotifyCollisionState(this,Physics::Col_Contacts_Updated);
                    }

                    return;
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Contact Query

        Whole Collision::GetNumContactPoints()
            { return (Whole)this->InternalData->Manifolds.size(); }

        Vector3 Collision::GetWorldLocation(const Whole& Point)
        {
            btVector3 PointA = this->GetManifoldPoint(Point).m_localPointA;
            btVector3 PointB = this->GetManifoldPoint(Point).m_localPointB;
            return Vector3((PointA+PointB) /= 2);
        }

        Vector3 Collision::GetLocalALocation(const Whole& Point)
            { return Vector3(this->GetManifoldPoint(Point).m_localPointA); }

        Vector3 Collision::GetLocalBLocation(const Whole& Point)
            { return Vector3(this->GetManifoldPoint(Point).m_localPointB); }

        Vector3 Collision::GetNormal(const Whole& Point)
            { return Vector3(this->GetManifoldPoint(Point).m_normalWorldOnB); }

        Real Collision::GetAppliedImpulse(const Whole& Point)
            { return this->GetManifoldPoint(Point).m_appliedImpulse; }

        Real Collision::GetDistance(const Whole& Point)
            { return this->GetManifoldPoint(Point).m_distance1; }

        Whole Collision::GetAge(const Whole& Point)
            { return (Whole)this->GetManifoldPoint(Point).m_lifeTime; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void Collision::_SetProxyA(CollidableProxy* A)
        {
            if( this->ProxyA ) {
                Entresol::GetSingletonPtr()->Log("Attepting to change Proxy pointer Member in Collision.  This is not permitted.");
            }else{
                this->ProxyA = A;
                WorldObject* ObjectA = this->ProxyA->GetParentObject();
                if( ObjectA ) {
                    ObjectA->_NotifyCollisionState(this,Physics::Col_Begin);
                }
            }
        }

        void Collision::_SetProxyB(CollidableProxy* B)
        {
            if( this->ProxyB ) {
                Entresol::GetSingletonPtr()->Log("Attepting to change Proxy pointer Member in Collision.  This is not permitted.");
            }else{
                this->ProxyB = B;
                WorldObject* ObjectB = this->ProxyB->GetParentObject();
                if( ObjectB ) {
                    ObjectB->_NotifyCollisionState(this,Physics::Col_Begin);
                }
            }
        }
    }//Physics
}//Mezzanine

#endif
