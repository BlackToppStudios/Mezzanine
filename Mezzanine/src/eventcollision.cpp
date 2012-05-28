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
#ifndef eventcollision_cpp
#define eventcollision_cpp

#include "eventcollision.h"
#include "actorcontainerbase.h"
#include "actormanager.h"
#include "actorbase.h"
#include "stringtool.h"

#include <btBulletDynamicsCommon.h>
#include <memory>

namespace Mezzanine
{
    EventCollision::EventCollision()
    {
        ActorA=NULL;
        ActorB=NULL;
        ObjectCollision=NULL;
    }

    EventCollision::EventCollision(ActorBase* actora, ActorBase* actorb, Mezzanine::Collision* Col)
    {
        ActorA=actora;
        ActorB=actorb;
        ObjectCollision=Col;
    }

    EventCollision::EventCollision(const EventCollision& Other)
    {
        ActorA=Other.ActorA;
        ActorB=Other.ActorB;
    }

    EventCollision::~EventCollision()
    {
    }

    void EventCollision::SetActorA(ActorBase* A)
    {
        if(ActorA)
        {
            World::GetWorldPointer()->Log("Attepting to change Actor pointer Member in EventCollision.  This is not permitted.");
        }else{
            ActorA = A;
        }
    }

    ActorBase* EventCollision::GetActorA() const
    {
        return ActorA;
    }

    void EventCollision::SetActorB(ActorBase* B)
    {
        if(ActorB)
        {
            World::GetWorldPointer()->Log("Attepting to change Actor pointer Member in EventCollision.  This is not permitted.");
        }else{
            ActorB = B;
        }
    }

    ActorBase* EventCollision::GetActorB() const
    {
        return ActorB;
    }

    EventBase::EventType EventCollision::GetType() const
    {
        return EventBase::Collision;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef MEZZXML
std::ostream& operator << (std::ostream& stream, const Mezzanine::EventCollision& Ev)
{
    stream  << "<EventCollision Version=\"1" //Impulse=\"" << Ev.Impulse
            << "\" ActorA=\"" << Ev.GetActorA()->GetName()
            << "\" ActorB=\"" << Ev.GetActorB()->GetName()
            << "\" >"
            //<<  Ev.WorldLocation
            << "</EventCollision>";
    return stream;
}

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::EventCollision& Ev)
{
    Mezzanine::String OneTag( Mezzanine::XML::GetOneTag(stream) );
    std::auto_ptr<Mezzanine::XML::Document> Doc( Mezzanine::XML::PreParseClassFromSingleTag("Mezzanine::", "EventCollision", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

/// @todo This whole thing needs to be updated, and have a serializer added.

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::EventCollision& Ev)
{
    if ( Mezzanine::String(OneNode.Name())==Mezzanine::String("EventCollision"))
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {

            Ev.SetActorA(Mezzanine::ActorManager::GetSingletonPtr()->GetActor(OneNode.GetAttribute("ActorA").AsString()));
            Ev.SetActorB(Mezzanine::ActorManager::GetSingletonPtr()->GetActor(OneNode.GetAttribute("ActorB").AsString()));
            //Ev.Impulse=OneNode.GetAttribute("Impulse").AsReal();

            //if(OneNode.GetFirstChild())
            //{
            //    OneNode.GetFirstChild() >> Ev.WorldLocation;
            //}else{
            //    throw(Mezzanine::Exception("Normal not found while parsing Mezzanine::EventCollision"));
            //}

        }else{
            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for EventCollision: Not Version 1");
        }
    }else{
        MEZZ_EXCEPTION(Mezzanine::Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a EventCollision, found a " + Mezzanine::String(OneNode.Name()));
    }
}
#endif // \MEZZXML

#endif
