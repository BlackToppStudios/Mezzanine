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
#ifndef _physicscollidablerayquery_cpp
#define _physicscollidablerayquery_cpp

#include "Physics/collidablerayquery.h"
#include "Physics/collidableproxy.h"
#include "Physics/physicsmanager.h"

#include "MathTools/intersections.h"

#include "entresol.h"
#include "world.h"
#include "managerbase.h"

#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>

#include <limits>

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        // BroadphaseOnlyCallback Methods

        class MEZZ_LIB BroadphaseOnlyCallback : public btBroadphaseRayCallback
        {
        public:
            btVector3       m_rayFromWorld;
            btVector3       m_rayToWorld;
            btTransform     m_rayFromTrans;
            btTransform     m_rayToTrans;
            btVector3       m_hitNormal;

            const btCollisionWorld* m_world;
            btCollisionWorld::RayResultCallback& m_resultCallback;

            BroadphaseOnlyCallback(const btVector3& rayFromWorld,const btVector3& rayToWorld,const btCollisionWorld* world,btCollisionWorld::RayResultCallback& resultCallback) :
                m_rayFromWorld(rayFromWorld),
                m_rayToWorld(rayToWorld),
                m_world(world),
                m_resultCallback(resultCallback)
            {
                m_rayFromTrans.setIdentity();
                m_rayFromTrans.setOrigin(m_rayFromWorld);
                m_rayToTrans.setIdentity();
                m_rayToTrans.setOrigin(m_rayToWorld);

                btVector3 rayDir = (rayToWorld-rayFromWorld);

                rayDir.normalize ();
                m_rayDirectionInverse[0] = rayDir[0] == btScalar(0.0) ? btScalar(BT_LARGE_FLOAT) : btScalar(1.0) / rayDir[0];
                m_rayDirectionInverse[1] = rayDir[1] == btScalar(0.0) ? btScalar(BT_LARGE_FLOAT) : btScalar(1.0) / rayDir[1];
                m_rayDirectionInverse[2] = rayDir[2] == btScalar(0.0) ? btScalar(BT_LARGE_FLOAT) : btScalar(1.0) / rayDir[2];
                m_signs[0] = m_rayDirectionInverse[0] < 0.0;
                m_signs[1] = m_rayDirectionInverse[1] < 0.0;
                m_signs[2] = m_rayDirectionInverse[2] < 0.0;

                m_lambda_max = rayDir.dot(m_rayToWorld-m_rayFromWorld);
            }

            virtual bool process(const btBroadphaseProxy* proxy)
            {
                if( m_resultCallback.m_closestHitFraction == btScalar(0.f) )
                    return false;

                btCollisionObject* collisionObject = (btCollisionObject*)proxy->m_clientObject;
                btVector3 HitNormal = ( this->m_rayToWorld - this->m_rayFromWorld ).normalize();
                btCollisionWorld::LocalShapeInfo ShapeInfo;
                ShapeInfo.m_shapePart = -1;
                ShapeInfo.m_triangleIndex = -1;

                AxisAlignedBox ProxyAABB( Vector3(proxy->m_aabbMin), Vector3(proxy->m_aabbMax) );
                Ray ToProxyRay( Vector3(this->m_rayFromWorld), Vector3(this->m_rayFromWorld).GetDirection( Vector3(this->m_rayToWorld) ) );
                MathTools::GeometryRayTestResult Result = MathTools::Intersects(ProxyAABB,ToProxyRay);
                btScalar BtHitFraction = ( this->m_rayFromWorld.distance( Result.second.PointA.GetBulletVector3() ) /
                                           this->m_rayFromWorld.distance( this->m_rayToWorld ) );

                btCollisionWorld::LocalRayResult RayResult( collisionObject,
                                                            &ShapeInfo,
                                                            HitNormal,
                                                            BtHitFraction );
                m_resultCallback.addSingleResult(RayResult,true);
                return true;
            }
        };//BroadphaseOnlyCallback

        ///////////////////////////////////////////////////////////////////////////////
        // SingleHitRayCallback Methods

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A custom AABB callback for Bullet ray world queries that returns the closest result.
        ///////////////////////////////////////
        class MEZZ_LIB SingleHitRayCallback : public btCollisionWorld::ClosestRayResultCallback
        {
        protected:
            /// @brief A custom filter for potential ray query hits.
            RayQuery::FilterFunction FilterFunct;
        public:
            /// @brief Class constructor.
            /// @param Start The point in world space where the ray originates.
            /// @param End The point in world space where the ray ends.
            /// @param Filter The function that will filter hit results.
            SingleHitRayCallback(const btVector3& Start, const btVector3& End, const RayQuery::FilterFunction Filter) :
                btCollisionWorld::ClosestRayResultCallback(Start,End),
                FilterFunct(Filter)
                {  }
            /// @brief Class destructor.
            ~SingleHitRayCallback() = default;

            /// @brief Callback method for when an object is found on the ray.
            /// @param rayResult Result containing the hit information.
            /// @param normalInWorldSpace Whether or not the normal provided is in world space.
            /// @return Returns the hit fraction, which is the interpolated distance between the ray start and end where the hit occurred.
            btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
            {
                CollidableProxy* Prox = CollidableProxy::_Upcast(rayResult.m_collisionObject);
                Boole FilterResult = ( this->FilterFunct ? this->FilterFunct(Prox) : true );
                if( FilterResult && Prox != nullptr ) {
                    return this->btCollisionWorld::ClosestRayResultCallback::addSingleResult(rayResult,normalInWorldSpace);
                }
                return rayResult.m_hitFraction;
            }
        };//SingleHitRayCallback

        ///////////////////////////////////////////////////////////////////////////////
        // MultiHitRayCallback Methods

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A custom AABB callback for Bullet ray world queries that dumps the results we want into a usable container.
        ///////////////////////////////////////
        class MEZZ_LIB MultiHitRayCallback : public btCollisionWorld::RayResultCallback
        {
        protected:
            /// @brief A container of the results to be returned.
            RayQuery::ResultContainer Results;
            /// @brief The point in world space where the ray originates.
            btVector3 RayStart;
            /// @brief The point in world space where the ray ends.
            btVector3 RayEnd;
            /// @brief A custom filter for potential ray query hits.
            RayQuery::FilterFunction FilterFunct;
        public:
            /// @brief Class constructor.
            /// @param Start The point in world space where the ray originates.
            /// @param End The point in world space where the ray ends.
            /// @param Filter The function that will filter hit results.
            MultiHitRayCallback(const btVector3& Start, const btVector3& End, const RayQuery::FilterFunction Filter) :
                RayStart(Start),
                RayEnd(End),
                FilterFunct(Filter)
                {  }
            /// @brief Class destructor.
            ~MultiHitRayCallback() = default;

            /// @brief Callback method for when an object is found on the ray.
            /// @param rayResult Result containing the hit information.
            /// @param normalInWorldSpace Whether or not the normal provided is in world space.
            /// @return Returns the hit fraction, which is the interpolated distance between the ray start and end where the hit occurred.
            btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
            {
                this->m_collisionObject = rayResult.m_collisionObject;
                CollidableProxy* Prox = CollidableProxy::_Upcast(rayResult.m_collisionObject);
                Boole FilterResult = ( this->FilterFunct ? this->FilterFunct(Prox) : true );
                if( FilterResult && Prox != nullptr ) {
                    btVector3 BtHitLoc;
                    BtHitLoc.setInterpolate3(this->RayStart,this->RayEnd,rayResult.m_hitFraction);

                    RayQueryHit NewHit;
                    NewHit.Object = Prox;
                    NewHit.Distance = this->RayStart.distance(BtHitLoc);
                    NewHit.HitLocation.ExtractBulletVector3(BtHitLoc);
                    NewHit.SubObject = -1;
                    if( normalInWorldSpace ) {
                        NewHit.HitNormal.ExtractBulletVector3(rayResult.m_hitNormalLocal);
                    }else{
                        NewHit.HitNormal.ExtractBulletVector3(this->m_collisionObject->getWorldTransform().getBasis() *
                                                              rayResult.m_hitNormalLocal);
                    }
                    this->Results.push_back(NewHit);
                }
                return rayResult.m_hitFraction;
            }

            /// @brief Sets the custom function that can filter ray query results.
            /// @param Filter The function that will filter hit results.
            void SetFilter(const RayQuery::FilterFunction Filter)
                { this->FilterFunct = Filter; }
            /// @brief Gets the custom function that will filter ray query results.
            /// @return Returns the function object currently filtering results from this callback.
            RayQuery::FilterFunction GetFilter() const
                { return this->FilterFunct; }
            /// @brief Gets the current results of the query.
            /// @return Returns the results of the query.
            const RayQuery::ResultContainer& GetResults() const
                { return this->Results; }
            /// @brief Sorts the results by distance.
            void SortResults()
                { std::sort(this->Results.begin(),this->Results.end()); }
            /// @brief Resizes the results container
            /// @param Keep The number of results to shrink down to.
            void TruncateResults(const Whole Keep)
                { if( Keep < this->Results.size() ) this->Results.resize(Keep); }
            /// @brief Clears the results in this listener.
            void ClearResults()
                { this->Results.clear(); }
        };//MultiHitRayCallback

        ///////////////////////////////////////////////////////////////////////////////
        // CollidableRayQuery Methods

        CollidableRayQuery::CollidableRayQuery(PhysicsManager* ToQuery) :
            PhysicsMan(ToQuery),
            RayCastLength(15000),
            ColFilter(std::numeric_limits<UInt32>::max())
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration

        void CollidableRayQuery::SetWorld(World* ToQuery)
            { this->PhysicsMan = static_cast<PhysicsManager*>( ToQuery->GetManager(ManagerBase::MT_PhysicsManager) ); }

        World* CollidableRayQuery::GetWorld() const
            { return this->PhysicsMan->GetWorld(); }

        void CollidableRayQuery::SetFilterFunction(const RayQuery::FilterFunction Filter)
            { this->FilterFunct = Filter; }

        RayQuery::FilterFunction CollidableRayQuery::GetFilterFunction() const
            { return this->FilterFunct; }

        void CollidableRayQuery::SetQueryFilter(const UInt32 Filter)
            { this->ColFilter = Filter; }

        UInt32 CollidableRayQuery::GetQueryFilter() const
            { return this->ColFilter; }

        void CollidableRayQuery::SetRayLength(const Real Length)
            { this->RayCastLength = Length; }

        Real CollidableRayQuery::GetRayLength() const
            { return this->RayCastLength; }

        void CollidableRayQuery::SetManager(PhysicsManager* Manager)
            { this->PhysicsMan = Manager; }

        PhysicsManager* CollidableRayQuery::GetManager() const
            { return this->PhysicsMan; }

        ///////////////////////////////////////////////////////////////////////////////
        // Fast Query

        RayQueryHit CollidableRayQuery::GetFirstAABBResult(const Ray& Cast) const
        {
            RayQueryHit Ret;
            btVector3 Start = Cast.Origin.GetBulletVector3();
            btVector3 End = Cast.GetPointAtDistance(this->RayCastLength).GetBulletVector3();

            btCollisionWorld* InternalWorld = this->PhysicsMan->_GetPhysicsWorldPointer();
            btBroadphaseInterface* Broadphase = InternalWorld->getBroadphase();
            SingleHitRayCallback RayCallback(Start,End,this->FilterFunct);
            RayCallback.m_collisionFilterGroup = btBroadphaseProxy::AllFilter;
            RayCallback.m_collisionFilterMask = this->ColFilter;
            BroadphaseOnlyCallback BroadphaseCallback(Start,End,InternalWorld,RayCallback);

            Broadphase->rayTest(Start,End,BroadphaseCallback);
            if( RayCallback.hasHit() ) {
                Ret.Distance = RayCallback.m_rayFromWorld.distance(RayCallback.m_hitPointWorld);
                Ret.HitLocation.ExtractBulletVector3(RayCallback.m_hitPointWorld);
                Ret.HitNormal.ExtractBulletVector3(RayCallback.m_hitNormalWorld);
                Ret.Object = CollidableProxy::_Upcast(RayCallback.m_collisionObject);
                Ret.SubObject = -1;
                return Ret;
            }
            return Ret.Reset();
        }

        RayQuery::ResultContainer CollidableRayQuery::GetAllAABBResults(const Ray& Cast, const Whole Limit) const
        {
            RayQueryHit Ret;
            btVector3 Start = Cast.Origin.GetBulletVector3();
            btVector3 End = Cast.GetPointAtDistance(this->RayCastLength).GetBulletVector3();

            btCollisionWorld* InternalWorld = this->PhysicsMan->_GetPhysicsWorldPointer();
            btBroadphaseInterface* Broadphase = InternalWorld->getBroadphase();
            MultiHitRayCallback RayCallback(Start,End,this->FilterFunct);
            RayCallback.m_collisionFilterGroup = btBroadphaseProxy::AllFilter;
            RayCallback.m_collisionFilterMask = this->ColFilter;
            BroadphaseOnlyCallback BroadphaseCallback(Start,End,InternalWorld,RayCallback);

            Broadphase->rayTest(Start,End,BroadphaseCallback);
            RayCallback.SortResults();
            if( Limit > 0 ) {
                RayCallback.TruncateResults(Limit);
            }
            return RayCallback.GetResults();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Precise Query

        RayQueryHit CollidableRayQuery::GetFirstShapeResult(const Ray& Cast) const
        {
            RayQueryHit Ret;
            btVector3 Start = Cast.Origin.GetBulletVector3();
            btVector3 End = Cast.GetPointAtDistance(this->RayCastLength).GetBulletVector3();

            btCollisionWorld* InternalWorld = this->PhysicsMan->_GetPhysicsWorldPointer();
            SingleHitRayCallback RayCallback(Start,End,this->FilterFunct);
            RayCallback.m_collisionFilterGroup = btBroadphaseProxy::AllFilter;
            RayCallback.m_collisionFilterMask = this->ColFilter;

            InternalWorld->rayTest(Start,End,RayCallback);
            if( RayCallback.m_collisionObject != nullptr ) {
                Ret.Distance = RayCallback.m_rayFromWorld.distance(RayCallback.m_hitPointWorld);
                Ret.HitLocation.ExtractBulletVector3(RayCallback.m_hitPointWorld);
                Ret.HitNormal.ExtractBulletVector3(RayCallback.m_hitNormalWorld);
                Ret.Object = CollidableProxy::_Upcast(RayCallback.m_collisionObject);
                Ret.SubObject = -1;
                return Ret;
            }
            return Ret.Reset();
        }

        RayQuery::ResultContainer CollidableRayQuery::GetAllShapeResults(const Ray& Cast, const Whole Limit) const
        {
            btVector3 Start = Cast.Origin.GetBulletVector3();
            btVector3 End = Cast.GetPointAtDistance(this->RayCastLength).GetBulletVector3();

            btCollisionWorld* InternalWorld = this->PhysicsMan->_GetPhysicsWorldPointer();
            MultiHitRayCallback RayCallback(Start,End,this->FilterFunct);
            RayCallback.m_collisionFilterGroup = btBroadphaseProxy::AllFilter;
            RayCallback.m_collisionFilterMask = this->ColFilter;

            InternalWorld->rayTest(Start,End,RayCallback);
            RayCallback.SortResults();
            if( Limit > 0 ) {
                RayCallback.TruncateResults(Limit);
            }
            return RayCallback.GetResults();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void CollidableRayQuery::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild( CollidableRayQuery::GetSerializableName() );

            if( SelfRoot.AppendAttribute("Version").SetValue("1") &&
                SelfRoot.AppendAttribute("WorldName").SetValue( this->GetWorld()->GetName() ) &&
                SelfRoot.AppendAttribute("RayCastLength").SetValue( this->GetRayLength() ) &&
                SelfRoot.AppendAttribute("QueryFilter").SetValue( this->GetQueryFilter() ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",CollidableRayQuery::GetSerializableName(),true);
            }
        }

        void CollidableRayQuery::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;

            if( SelfRoot.Name() == CollidableRayQuery::GetSerializableName() && !SelfRoot.Empty() ) {
                if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = SelfRoot.GetAttribute("WorldName");
                    if( !CurrAttrib.Empty() )
                        this->SetWorld( Entresol::GetSingletonPtr()->GetWorld( CurrAttrib.AsString() ) );

                    CurrAttrib = SelfRoot.GetAttribute("RayCastLength");
                    if( !CurrAttrib.Empty() )
                        this->SetRayLength( CurrAttrib.AsReal() );

                    CurrAttrib = SelfRoot.GetAttribute("QueryFilter");
                    if( !CurrAttrib.Empty() )
                        this->SetQueryFilter( CurrAttrib.AsUint() );
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + ( CollidableRayQuery::GetSerializableName() ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,CollidableRayQuery::GetSerializableName() + " was not found in the provided XML node, which was expected.");
            }
        }

        String CollidableRayQuery::GetSerializableName()
            { return "CollidableRayQuery"; }
    }//Physics
}//Mezzanine

#endif
