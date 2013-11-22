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
        { this->UnsubscribeAll(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    const String& Event::GetName() const
        { return this->EventName; }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscription Methods

    EventSubscriberSlot* Event::Subscribe(EventSubscriber* Sub)
    {
        return this->Subscribe(255,Sub);
    }

    EventSubscriberSlot* Event::Subscribe(const UInt8 Group, EventSubscriber* Sub)
    {
        CustomSubscriberSlot* NewSlot = new CustomSubscriberSlot(this,Sub);
        this->Slots.insert( SlotPair(Group,NewSlot) );
        return NewSlot;
    }

    EventSubscriberSlot* Event::Subscribe(FunctorSubscriberSlot::FunctorDefinition* Funct, Bool CleanUpAfter)
    {
        return this->Subscribe(255,Funct,CleanUpAfter);
    }

    EventSubscriberSlot* Event::Subscribe(const UInt8 Group, FunctorSubscriberSlot::FunctorDefinition* Funct, Bool CleanUpAfter)
    {
        FunctorSubscriberSlot* NewSlot = new FunctorSubscriberSlot(this,Funct,CleanUpAfter);
        this->Slots.insert( SlotPair(Group,NewSlot) );
        return NewSlot;
    }

    EventSubscriberSlot* Event::Subscribe(CFunctionSubscriberSlot::SubscriberFunction* CFunct)
    {
        return this->Subscribe(255,CFunct);
    }

    EventSubscriberSlot* Event::Subscribe(const UInt8 Group, CFunctionSubscriberSlot::SubscriberFunction* CFunct)
    {
        CFunctionSubscriberSlot* NewSlot = new CFunctionSubscriberSlot(this,CFunct);
        this->Slots.insert( SlotPair(Group,NewSlot) );
        return NewSlot;
    }

    EventSubscriberSlot* Event::Subscribe(Scripting::iScript* SubScript)
    {
        return this->Subscribe(255,SubScript);
    }

    EventSubscriberSlot* Event::Subscribe(const UInt8 Group, Scripting::iScript* SubScript)
    {
        ScriptSubscriberSlot* NewSlot = new ScriptSubscriberSlot(this,SubScript);
        this->Slots.insert( SlotPair(Group,NewSlot) );
        return NewSlot;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Unsubscribe Methods

    void Event::Unsubscribe(EventSubscriber* Subscriber)
    {
        SlotIterator Current = this->Slots.begin();
        while( Current != this->Slots.end() )
        {
            if( (*Current).second->GetType() == EventSubscriberSlot::ST_Custom &&
                static_cast<CustomSubscriberSlot*>( (*Current).second )->GetSubscriber() == Subscriber )
            {
                SlotIterator Prev = Current++;
                delete (*Prev).second;
                this->Slots.erase(Prev);
            }else{
                ++Current;
            }
        }
    }

    void Event::Unsubscribe(FunctorSubscriberSlot::FunctorDefinition* Funct)
    {
        SlotIterator Current = this->Slots.begin();
        while( Current != this->Slots.end() )
        {
            if( (*Current).second->GetType() == EventSubscriberSlot::ST_Functor &&
                static_cast<FunctorSubscriberSlot*>( (*Current).second )->GetFunctor() == Funct )
            {
                SlotIterator Prev = Current++;
                delete (*Prev).second;
                this->Slots.erase(Prev);
            }else{
                ++Current;
            }
        }
    }

    void Event::Unsubscribe(CFunctionSubscriberSlot::SubscriberFunction* CFunct)
    {
        SlotIterator Current = this->Slots.begin();
        while( Current != this->Slots.end() )
        {
            if( (*Current).second->GetType() == EventSubscriberSlot::ST_CFunction &&
                static_cast<CFunctionSubscriberSlot*>( (*Current).second )->GetFunction() == CFunct )
            {
                SlotIterator Prev = Current++;
                delete (*Prev).second;
                this->Slots.erase(Prev);
            }else{
                ++Current;
            }
        }
    }

    void Event::Unsubscribe(Scripting::iScript* SubScript)
    {
        SlotIterator Current = this->Slots.begin();
        while( Current != this->Slots.end() )
        {
            if( (*Current).second->GetType() == EventSubscriberSlot::ST_Script &&
                static_cast<ScriptSubscriberSlot*>( (*Current).second )->GetScript() == SubScript )
            {
                SlotIterator Prev = Current++;
                delete (*Prev).second;
                this->Slots.erase(Prev);
            }else{
                ++Current;
            }
        }
    }

    void Event::Unsubscribe(EventSubscriberSlot* SubSlot)
    {
        SlotIterator Current = this->Slots.begin();
        while( Current != this->Slots.end() )
        {
            if( (*Current).second == SubSlot ) {
                SlotIterator Prev = Current++;
                delete (*Prev).second;
                this->Slots.erase(Prev);
            }else{
                ++Current;
            }
        }
    }

    Whole Event::UnsubscribeGroup(const UInt8 Group)
    {
        SlotIterator Begin = this->Slots.lower_bound(Group);
        SlotIterator End = this->Slots.upper_bound(Group);
        for( SlotIterator Current = Begin ; Current != End ; ++Current )
        {
            delete (*Current).second;
        }
        return this->Slots.erase(Group);
    }

    Whole Event::UnsubscribeAll()
    {
        Whole PrevSize = this->Slots.size();
        SlotIterator Begin = this->Slots.begin();
        SlotIterator End = this->Slots.end();
        for( SlotIterator Current = Begin ; Current != End ; ++Current )
        {
            delete (*Current).second;
        }
        this->Slots.clear();
        return PrevSize;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscriber Access Methods

    Event::SlotIterator Event::SubscriberSlotBegin()
        { return this->Slots.begin(); }

    Event::SlotIterator Event::SubscriberSlotEnd()
        { return this->Slots.end(); }

    Event::ConstSlotIterator Event::SubscriberSlotBegin() const
        { return this->Slots.begin(); }

    Event::ConstSlotIterator Event::SubscriberSlotEnd() const
        { return this->Slots.end(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void Event::_FireEvent(const EventArguments& Args)
    {
        for( SlotIterator SlotIt = this->Slots.begin() ; SlotIt != this->Slots.end() ; ++SlotIt )
        {
            (*SlotIt).second->_NotifyEvent(Args);
        }
    }

    void Event::_FireGroupEvent(const UInt8 Group, const EventArguments& Args)
    {
        SlotIterator Begin = this->Slots.lower_bound(Group);
        SlotIterator End = this->Slots.upper_bound(Group);
        for( SlotIterator Current = Begin ; Current != End ; ++Current )
        {
            (*Current).second->_NotifyEvent(Args);
        }
    }
}

#endif
