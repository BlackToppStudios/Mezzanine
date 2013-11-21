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
#ifndef _eventpublisher_cpp
#define _eventpublisher_cpp

#include "eventpublisher.h"

namespace Mezzanine
{
    EventPublisher::EventPublisher()
        {  }

    EventPublisher::~EventPublisher()
        { this->RemoveAllEvents(); }

    Event* EventPublisher::AddEvent(const String& EventName)
    {
        Event* Ev = this->GetEvent(EventName);
        if( Ev != NULL ) {
            return Ev;
        }else{
            Ev = new Event(EventName);
            this->Events[EventName] = Ev;
            return Ev;
        }
    }

    void EventPublisher::FireEvent(const EventArguments& Args)
        { this->GetEventExcept(Args.EventName)->_FireEvent(Args); }

    void EventPublisher::RemoveEvent(const String& EventName)
    {
        EventIterator EvIt = this->Events.find(EventName);
        if( EvIt != this->Events.end() ) {
            delete (*EvIt).second;
            this->Events.erase(EvIt);
        }
    }

    void EventPublisher::RemoveAllEvents()
    {
        for( EventIterator EvIt = this->Events.begin() ; EvIt != this->Events.end() ; ++EvIt )
        {
            delete (*EvIt).second;
        }
        this->Events.clear();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Event* EventPublisher::GetEvent(const String& EventName) const
    {
        ConstEventIterator EvIt = this->Events.find(EventName);
        if( EvIt != this->Events.end() ) return (*EvIt).second;
        else return NULL;
    }

    Event* EventPublisher::GetEventExcept(const String& EventName) const
    {
        ConstEventIterator EvIt = this->Events.find(EventName);
        if( EvIt != this->Events.end() ) {
            return (*EvIt).second;
        }else{
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,"Event name \"" + EventName + "\" not found in publisher.");
        }
        return NULL;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscribe Methods

    EventSubscriberSlot* EventPublisher::Subscribe(const String& EventName, EventSubscriber* Sub)
        { return this->GetEventExcept(EventName)->Subscribe(Sub); }

    EventSubscriberSlot* EventPublisher::Subscribe(const String& EventName, const UInt8 Group, EventSubscriber* Sub)
        { return this->GetEventExcept(EventName)->Subscribe(Group,Sub); }

    EventSubscriberSlot* EventPublisher::Subscribe(const String& EventName, FunctorSubscriberSlot::FunctorDefinition* Funct, Bool CleanUpAfter)
        { return this->GetEventExcept(EventName)->Subscribe(Funct,CleanUpAfter); }

    EventSubscriberSlot* EventPublisher::Subscribe(const String& EventName, const UInt8 Group, FunctorSubscriberSlot::FunctorDefinition* Funct, Bool CleanUpAfter)
        { return this->GetEventExcept(EventName)->Subscribe(Group,Funct,CleanUpAfter); }

    EventSubscriberSlot* EventPublisher::Subscribe(const String& EventName, CFunctionSubscriberSlot::SubscriberFunction* CFunct)
        { return this->GetEventExcept(EventName)->Subscribe(CFunct); }

    EventSubscriberSlot* EventPublisher::Subscribe(const String& EventName, const UInt8 Group, CFunctionSubscriberSlot::SubscriberFunction* CFunct)
        { return this->GetEventExcept(EventName)->Subscribe(Group,CFunct); }

    EventSubscriberSlot* EventPublisher::Subscribe(const String& EventName, Scripting::iScript* SubScript)
        { return this->GetEventExcept(EventName)->Subscribe(SubScript); }

    EventSubscriberSlot* EventPublisher::Subscribe(const String& EventName, const UInt8 Group, Scripting::iScript* SubScript)
        { return this->GetEventExcept(EventName)->Subscribe(Group,SubScript); }

    ///////////////////////////////////////////////////////////////////////////////
    // Unsubscribe Methods

    void EventPublisher::Unsubscribe(EventSubscriber* Subscriber)
    {
        for( EventIterator EventIt = this->Events.begin() ; EventIt != this->Events.begin() ; ++EventIt )
            { (*EventIt).second->Unsubscribe(Subscriber); }
    }

    void EventPublisher::Unsubscribe(FunctorSubscriberSlot::FunctorDefinition* Funct)
    {
        for( EventIterator EventIt = this->Events.begin() ; EventIt != this->Events.begin() ; ++EventIt )
            { (*EventIt).second->Unsubscribe(Funct); }
    }

    void EventPublisher::Unsubscribe(CFunctionSubscriberSlot::SubscriberFunction* CFunct)
    {
        for( EventIterator EventIt = this->Events.begin() ; EventIt != this->Events.begin() ; ++EventIt )
            { (*EventIt).second->Unsubscribe(CFunct); }
    }

    void EventPublisher::Unsubscribe(Scripting::iScript* SubScript)
    {
        for( EventIterator EventIt = this->Events.begin() ; EventIt != this->Events.begin() ; ++EventIt )
            { (*EventIt).second->Unsubscribe(SubScript); }
    }

    void EventPublisher::Unsubscribe(EventSubscriberSlot* SubSlot)
    {
        for( EventIterator EventIt = this->Events.begin() ; EventIt != this->Events.begin() ; ++EventIt )
            { (*EventIt).second->Unsubscribe(SubSlot); }
    }

    Whole EventPublisher::UnsubscribeGroup(const UInt8 Group)
    {
        for( EventIterator EventIt = this->Events.begin() ; EventIt != this->Events.begin() ; ++EventIt )
            { (*EventIt).second->UnsubscribeGroup(Group); }
    }

    Whole EventPublisher::UnsubscribeAll()
    {
        for( EventIterator EventIt = this->Events.begin() ; EventIt != this->Events.begin() ; ++EventIt )
            { (*EventIt).second->UnsubscribeAll(); }
    }

    void EventPublisher::Unsubscribe(const String& EventName, EventSubscriber* Subscriber)
        { this->GetEventExcept(EventName)->Unsubscribe(Subscriber); }

    void EventPublisher::Unsubscribe(const String& EventName, FunctorSubscriberSlot::FunctorDefinition* Funct)
        { this->GetEventExcept(EventName)->Unsubscribe(Funct); }

    void EventPublisher::Unsubscribe(const String& EventName, CFunctionSubscriberSlot::SubscriberFunction* CFunct)
        { this->GetEventExcept(EventName)->Unsubscribe(CFunct); }

    void EventPublisher::Unsubscribe(const String& EventName, Scripting::iScript* SubScript)
        { this->GetEventExcept(EventName)->Unsubscribe(SubScript); }

    void EventPublisher::Unsubscribe(const String& EventName, EventSubscriberSlot* SubSlot)
        { this->GetEventExcept(EventName)->Unsubscribe(SubSlot); }

    Whole EventPublisher::UnsubscribeGroup(const String& EventName, const UInt8 Group)
        { this->GetEventExcept(EventName)->UnsubscribeGroup(Group); }

    Whole EventPublisher::UnsubscribeAll(const String& EventName)
        { this->GetEventExcept(EventName)->UnsubscribeAll(); }
}//Mezzanine

#endif
