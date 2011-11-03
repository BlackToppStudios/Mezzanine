//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef eventcollision_cpp
#define eventcollision_cpp

#include "actorcontainerbase.h"
#include "actormanager.h"
#include "eventcollision.h"
#include "world.h"

#include <btBulletDynamicsCommon.h>
#include <memory>

namespace phys {
    EventCollision::EventCollision()
    {
        ActorA=NULL;
        ActorB=NULL;
    }

    EventCollision::EventCollision(ActorBase* actora, ActorBase* actorb)
    {
        ActorA=actora;
        ActorB=actorb;

        ActorA->GetCurrentCollisions().insert(this);
        ActorB->GetCurrentCollisions().insert(this);
    }

    EventCollision::EventCollision(const EventCollision& Other)
    {
        ActorA=Other.ActorA;
        ActorB=Other.ActorB;
        Manifold=Other.Manifold;

        ActorA->GetCurrentCollisions().insert(this);
        ActorB->GetCurrentCollisions().insert(this);
    }

    EventCollision::~EventCollision()
    {
        std::set<EventCollision*>& CollisionsA = ActorA->GetCurrentCollisions();
        std::set<EventCollision*>& CollisionsB = ActorB->GetCurrentCollisions();
        CollisionsA.erase(CollisionsA.find(this));
        CollisionsB.erase(CollisionsB.find(this));
    }

    void EventCollision::SetActorA(ActorBase* A)
    {
        if(ActorA)
        {
            World::GetWorldPointer()->Log("Attepting to change Actor pointer Member in EventCollision.  This is not permitted.");
        }else{
            ActorA = A;
            ActorA->GetCurrentCollisions().insert(this);
        }
    }

    ActorBase* EventCollision::GetActorA() const
    {
        return ActorB;
    }

    void EventCollision::SetActorB(ActorBase* B)
    {
        if(ActorB)
        {
            World::GetWorldPointer()->Log("Attepting to change Actor pointer Member in EventCollision.  This is not permitted.");
        }else{
            ActorB = B;
            ActorB->GetCurrentCollisions().insert(this);
        }
    }

    ActorBase* EventCollision::GetActorB() const
    {
        return ActorB;
    }

    Whole EventCollision::GetNumContactPoints()
    {
        return (Whole)Manifold->getNumContacts();
    }

    Vector3 EventCollision::GetWorldLocation(const Whole& Point)
    {
        btVector3 PointA = Manifold->getContactPoint(Point).m_localPointA;
        btVector3 PointB = Manifold->getContactPoint(Point).m_localPointB;
        return Vector3((PointA+PointB) /= 2);
    }

    Vector3 EventCollision::GetLocalALocation(const Whole& Point)
    {
        return Vector3(Manifold->getContactPoint(Point).m_localPointA);
    }

    Vector3 EventCollision::GetLocalBLocation(const Whole& Point)
    {
        return Vector3(Manifold->getContactPoint(Point).m_localPointB);
    }

    Vector3 EventCollision::GetNormal(const Whole& Point)
    {
        return Vector3(Manifold->getContactPoint(Point).m_normalWorldOnB);
    }

    Real EventCollision::GetAppliedImpulse(const Whole& Point)
    {
        return Manifold->getContactPoint(Point).m_appliedImpulse;
    }

    Whole EventCollision::GetAge(const Whole& Point)
    {
        return (Whole)Manifold->getContactPoint(Point).m_lifeTime;
    }

    EventBase::EventType EventCollision::GetType() const
    {
        return EventBase::Collision;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::EventCollision& Ev)
{
    stream  << "<EventCollision Version=\"1" //Impulse=\"" << Ev.Impulse
            << "\" ActorA=\"" << Ev.GetActorA()->GetName()
            << "\" ActorB=\"" << Ev.GetActorB()->GetName()
            << "\" >"
            //<<  Ev.WorldLocation
            << "</EventCollision>";
    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::EventCollision& Ev)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "EventCollision", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

/// @todo This whole thing needs to be updated, and have a serializer added.

void operator >> (const phys::xml::Node& OneNode, phys::EventCollision& Ev)
{
    if ( phys::String(OneNode.Name())==phys::String("EventCollision"))
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {

            Ev.SetActorA(phys::World::GetWorldPointer()->GetActorManager()->GetActor(OneNode.GetAttribute("ActorA").AsString()));
            Ev.SetActorB(phys::World::GetWorldPointer()->GetActorManager()->GetActor(OneNode.GetAttribute("ActorB").AsString()));
            //Ev.Impulse=OneNode.GetAttribute("Impulse").AsReal();

            //if(OneNode.GetFirstChild())
            //{
            //    OneNode.GetFirstChild() >> Ev.WorldLocation;
            //}else{
            //    throw(phys::Exception("Normal not found while parsing phys::EventCollision"));
            //}

        }else{
            throw( phys::Exception("Incompatible XML Version for EventCollision: Not Version 1"));
        }
    }else{
        throw( phys::Exception(phys::StringCat("Attempting to deserialize a EventCollision, found a ", OneNode.Name())));
    }
}
#endif // \PHYSXML

#endif
