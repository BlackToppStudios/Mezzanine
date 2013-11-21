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
#ifndef _event_cpp
#define _event_cpp

#include "event.h"

namespace Mezzanine
{
    Event::Event(const String& Name) :
        EventName(Name)
        {  }

    Event::~Event()
    {
        this->UnsubscribeAll();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    const String& Event::GetName() const
    {
        return this->EventName;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscriber Slot Creation

    EventConnectionPtr Event::CreateConnection(Event* Ev, EventSubscriber* Sub)
    {
        return EventConnectionPtr(new EventConnection(Ev,Sub));
    }

    EventConnectionPtr Event::CreateConnection(Event* Ev, FunctorSubscriber::FunctorDefinition* Funct, bool CleanUpAfter)
    {
        FunctorSubscriber* Sub = new FunctorSubscriber(Funct,CleanUpAfter);
        EventConnectionPtr NewConnection(new VolatileEventConnection(Ev,Sub));
        return NewConnection;
    }

    EventConnectionPtr Event::CreateConnection(Event* Ev, CFunctionSubscriber::SubscriberFunction* CFunct)
    {
        CFunctionSubscriber* Sub = new CFunctionSubscriber(CFunct);
        EventConnectionPtr NewConnection(new VolatileEventConnection(Ev,Sub));
        return NewConnection;
    }

    EventConnectionPtr Event::CreateConnection(Event* Ev, Scripting::iScript* SubScript)
    {
        ScriptSubscriber* Sub = new ScriptSubscriber(SubScript);
        EventConnectionPtr NewConnection(new VolatileEventConnection(Ev,Sub));
        return NewConnection;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscription Methods

    void Event::Subscribe(EventConnectionPtr Subscriber)
    {
        this->Connections.insert(ConnectionPair(255,Subscriber));
    }

    void Event::Subscribe(const UInt8 Group, EventConnectionPtr Subscriber)
    {
        this->Connections.insert(ConnectionPair(Group,Subscriber));
    }

    EventConnectionPtr Event::Subscribe(EventSubscriber* Sub)
    {
        this->Subscribe(255,Sub);
    }

    EventConnectionPtr Event::Subscribe(const UInt8 Group, EventSubscriber* Sub)
    {
        EventConnectionPtr EventSub = Event::CreateConnection(this,Sub);
        this->Subscribe(Group,EventSub);
        return EventSub;
    }

    EventConnectionPtr Event::Subscribe(FunctorSubscriber::FunctorDefinition* Funct, bool CleanUpAfter)
    {
        this->Subscribe(255,Funct,CleanUpAfter);
    }

    EventConnectionPtr Event::Subscribe(const UInt8 Group, FunctorSubscriber::FunctorDefinition* Funct, bool CleanUpAfter)
    {
        EventConnectionPtr EventSub = Event::CreateConnection(this,Funct,CleanUpAfter);
        this->Subscribe(Group,EventSub);
        return EventSub;
    }

    EventConnectionPtr Event::Subscribe(CFunctionSubscriber::SubscriberFunction* CFunct)
    {
        this->Subscribe(255,CFunct);
    }

    EventConnectionPtr Event::Subscribe(const UInt8 Group, CFunctionSubscriber::SubscriberFunction* CFunct)
    {
        EventConnectionPtr EventSub = Event::CreateConnection(this,CFunct);
        this->Subscribe(Group,EventSub);
        return EventSub;
    }

    EventConnectionPtr Event::Subscribe(Scripting::iScript* SubScript)
    {
        this->Subscribe(255,SubScript);
    }

    EventConnectionPtr Event::Subscribe(const UInt8 Group, Scripting::iScript* SubScript)
    {
        EventConnectionPtr EventSub = Event::CreateConnection(this,SubScript);
        this->Subscribe(Group,EventSub);
        return EventSub;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Unsubscribe Methods

    void Event::Unsubscribe(EventConnection* Subscriber)
    {
        Subscriber->Disconnect(this);
        for( ConnectionIterator SubIt = this->Connections.begin() ; SubIt != this->Connections.end() ; ++SubIt )
        {
            if( Subscriber == (*SubIt).second.get() )
            {
                this->Connections.erase(SubIt);
                return;
            }
        }
    }

    void Event::Unsubscribe(EventConnectionPtr Subscriber)
    {
        Subscriber->Disconnect(this);
        for( ConnectionIterator SubIt = this->Connections.begin() ; SubIt != this->Connections.end() ; ++SubIt )
        {
            if( Subscriber == (*SubIt).second )
            {
                this->Connections.erase(SubIt);
                return;
            }
        }
    }

    void Event::Unsubscribe(ConnectionIterator Subscriber)
    {
        (*Subscriber).second->Disconnect(this);
        this->Connections.erase(Subscriber);
    }

    Whole Event::UnsubscribeGroup(const UInt8 Group)
    {
        ConnectionIterator Begin = this->Connections.lower_bound(Group);
        ConnectionIterator End = this->Connections.upper_bound(Group);
        for( ConnectionIterator Current = Begin ; Current != End ; ++Current )
        {
            (*Current).second->Disconnect(this);
        }
        return this->Connections.erase(Group);
    }

    Whole Event::UnsubscribeAll()
    {
        Whole PrevSize = this->Connections.size();
        ConnectionIterator Begin = this->Connections.begin();
        ConnectionIterator End = this->Connections.end();
        for( ConnectionIterator Current = Begin ; Current != End ; ++Current )
        {
            (*Current).second->Disconnect(this);
        }
        this->Connections.clear();
        return PrevSize;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscriber Access Methods

    Event::ConnectionIterator Event::SubscriberBegin()
    {
        return this->Connections.begin();
    }

    Event::ConnectionIterator Event::SubscriberEnd()
    {
        return this->Connections.end();
    }

    Event::ConstConnectionIterator Event::SubscriberBegin() const
    {
        return this->Connections.begin();
    }

    Event::ConstConnectionIterator Event::SubscriberEnd() const
    {
        return this->Connections.end();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void Event::_FireEvent(const EventArguments& Args)
    {
        for( ConnectionIterator SubIt = this->Connections.begin() ; SubIt != this->Connections.end() ; ++SubIt )
        {
            if( (*SubIt).second->IsConnected() ) {
                (*SubIt).second->GetSubscriber()->_NotifyEvent(Args);
                ++SubIt;
            }else{
                ConnectionIterator Remove = SubIt++;
                this->Connections.erase(Remove);
            }
        }
    }

    void Event::_FireGroupEvent(const UInt8 Group, const EventArguments& Args)
    {
        ConnectionIterator Begin = this->Connections.lower_bound(Group);
        ConnectionIterator End = this->Connections.upper_bound(Group);
        for( ConnectionIterator Current = Begin ; Current != End ; ++Current )
        {
            if( (*Current).second->IsConnected() ) {
                (*Current).second->GetSubscriber()->_NotifyEvent(Args);
                ++Current;
            }else{
                ConnectionIterator Remove = Current++;
                this->Connections.erase(Remove);
            }
        }
    }
}

#endif
