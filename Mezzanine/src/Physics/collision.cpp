//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _collision_cpp
#define _collision_cpp

#include "actorbase.h"
#include "actormanager.h"
#include "collision.h"
#include "stringtool.h"

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
            btManifoldArray Manifolds;
        };

        Collision::Collision(WorldObject* A, WorldObject* B, btCollisionAlgorithm* PhysicsAlgo)
        {
            ObjectA = A;
            ObjectB = B;
            InternalAlgo = PhysicsAlgo;
            InternalData = new CollisionInternalData();

            ObjectA->_NotifyCollisionState(this,Collision::Col_Begin);
            ObjectB->_NotifyCollisionState(this,Collision::Col_Begin);

            InternalAlgo->getAllContactManifolds(InternalData->Manifolds);
        }

        Collision::Collision()
        {
            ObjectA = NULL;
            ObjectB = NULL;
            InternalAlgo = NULL;
            InternalData = new CollisionInternalData();
        }

        Collision::Collision(const Collision& Other)
        {
            ObjectA = Other.ObjectA;
            ObjectB = Other.ObjectB;
            InternalAlgo = Other.InternalAlgo;
            for( Whole X = 0 ; X < Other.InternalData->Manifolds.size() ; ++X )
                InternalData->Manifolds.push_back(Other.InternalData->Manifolds[X]);

            // Double notifies seems like a bad idea.
            //ObjectA->_NotifyCollisionState(this,Collision::Col_Begin);
            //ObjectB->_NotifyCollisionState(this,Collision::Col_Begin);
        }

        Collision::~Collision()
        {
            ObjectA->_NotifyCollisionState(this,Collision::Col_End);
            ObjectB->_NotifyCollisionState(this,Collision::Col_End);
            delete InternalData;
        }

        btManifoldPoint& Collision::GetManifoldPoint(const Whole& Index)
        {
            if(Index >= InternalData->Manifolds.size())
                MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Attempting to access invalid index in Collision.");

            if(Index > 3)
            {
                Whole SuperIndex = Index/4;
                Whole SubIndex = Index%4;
                return (InternalData->Manifolds.at(SuperIndex)->getContactPoint(SubIndex));
            }else{
                return (InternalData->Manifolds.at(0)->getContactPoint(Index));
            }
        }

        void Collision::UpdatePenetrationDistances()
        {
            if( InternalData->Manifolds.size() > PenetrationDistances.size() )
                PenetrationDistances.resize(InternalData->Manifolds.size());
            PenetrationDistances.clear();
            for( Whole X = 0 ; X < InternalData->Manifolds.size() ; ++X )
            {
                PenetrationDistances[X] = GetManifoldPoint(X).m_distance1;
            }
        }

        void Collision::SetObjectA(WorldObject* A)
        {
            if(ObjectA)
            {
                Entresol::GetSingletonPtr()->Log("Attepting to change Object pointer Member in Collision.  This is not permitted.");
            }else{
                ObjectA = A;
                ObjectA->_NotifyCollisionState(this,Collision::Col_Begin);
            }
        }

        WorldObject* Collision::GetObjectA() const
        {
            return ObjectA;
        }

        void Collision::SetObjectB(WorldObject* B)
        {
            if(ObjectB)
            {
                Entresol::GetSingletonPtr()->Log("Attepting to change Object pointer Member in Collision.  This is not permitted.");
            }else{
                ObjectB = B;
                ObjectB->_NotifyCollisionState(this,Collision::Col_Begin);
            }
        }

        WorldObject* Collision::GetObjectB() const
        {
            return ObjectB;
        }

        Whole Collision::GetNumContactPoints()
        {
            return (Whole)InternalData->Manifolds.size();
        }

        Vector3 Collision::GetWorldLocation(const Whole& Point)
        {
            btVector3 PointA = GetManifoldPoint(Point).m_localPointA;
            btVector3 PointB = GetManifoldPoint(Point).m_localPointB;
            return Vector3((PointA+PointB) /= 2);
        }

        Vector3 Collision::GetLocalALocation(const Whole& Point)
        {
            return Vector3(GetManifoldPoint(Point).m_localPointA);
        }

        Vector3 Collision::GetLocalBLocation(const Whole& Point)
        {
            return Vector3(GetManifoldPoint(Point).m_localPointB);
        }

        Vector3 Collision::GetNormal(const Whole& Point)
        {
            return Vector3(GetManifoldPoint(Point).m_normalWorldOnB);
        }

        Real Collision::GetAppliedImpulse(const Whole& Point)
        {
            return GetManifoldPoint(Point).m_appliedImpulse;
        }

        Real Collision::GetDistance(const Whole& Point)
        {
            return GetManifoldPoint(Point).m_distance1;
        }

        Whole Collision::GetAge(const Whole& Point)
        {
            return (Whole)GetManifoldPoint(Point).m_lifeTime;
        }

        bool Collision::PairsMatch(WorldObject* A, WorldObject* B) const
        {
            bool ContainsA = (A == ObjectA) || (A == ObjectB);
            bool ContainsB = (B == ObjectA) || (B == ObjectB);
            return (ContainsA && ContainsB);
        }

        void Collision::Update()
        {
            InternalAlgo->getAllContactManifolds(InternalData->Manifolds);
            Whole NumManifolds = InternalData->Manifolds.size();
            if( PenetrationDistances.size() != NumManifolds )
            {
                UpdatePenetrationDistances();
                ObjectA->_NotifyCollisionState(this,Collision::Col_Contacts_Updated);
                ObjectB->_NotifyCollisionState(this,Collision::Col_Contacts_Updated);
                return;
            }
            for( Whole X = 0 ; X < NumManifolds ; ++X )
            {
                if( PenetrationDistances[X] != GetDistance(X) )
                {
                    UpdatePenetrationDistances();
                    ObjectA->_NotifyCollisionState(this,Collision::Col_Contacts_Updated);
                    ObjectB->_NotifyCollisionState(this,Collision::Col_Contacts_Updated);
                    return;
                }
            }
        }
    }//Physics
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef MEZZXML
std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::Collision& Col)
{
    stream  << "<EventCollision Version=\"1" //Impulse=\"" << Ev.Impulse
            << "\" ObjectA=\"" << Col.GetObjectA()->GetName()
            << "\" ObjectB=\"" << Col.GetObjectB()->GetName()
            << "\" >"
            //<<  Ev.WorldLocation
            << "</EventCollision>";
    return stream;
}

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Physics::Collision& Col)
{
    Mezzanine::String OneTag( Mezzanine::XML::GetOneTag(stream) );
    std::auto_ptr<Mezzanine::XML::Document> Doc( Mezzanine::XML::PreParseClassFromSingleTag("Mezzanine::", "EventCollision", OneTag) );

    Doc->GetFirstChild() >> Col;

    return stream;
}

/// @todo This whole thing needs to be updated, and have a serializer added.

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::Collision& Col)
{
    if ( Mezzanine::String(OneNode.Name())==Mezzanine::String("EventCollision"))
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {

            //Col.SetObjectA(Mezzanine::ActorManager::GetSingletonPtr()->GetActor(OneNode.GetAttribute("ObjectA").AsString()));
            //Col.SetObjectB(Mezzanine::ActorManager::GetSingletonPtr()->GetActor(OneNode.GetAttribute("ObjectB").AsString()));
            //Col.Impulse=OneNode.GetAttribute("Impulse").AsReal();

            //if(OneNode.GetFirstChild())
            //{
            //    OneNode.GetFirstChild() >> Col.WorldLocation;
            //}else{
            //    throw(Mezzanine::Exception("Normal not found while parsing Mezzanine::Collision"));
            //}

        }else{
            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for Collision: Not Version 1");
        }
    }else{
        MEZZ_EXCEPTION(Mezzanine::Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a Collision, found a " + Mezzanine::String(OneNode.Name()));
    }
}
#endif // \MEZZXML

#endif
