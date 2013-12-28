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
#include "eventsubscriberslot.h"

#include <algorithm>

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

    EventSubscriberSlot* Event::Subscribe(EventSubscriber* Subscriber)
    {
        for( SlotIterator Current = this->Slots.begin() ; Current != this->Slots.end() ; ++Current )
        {
            if( (*Current)->GetType() == EventSubscriberSlot::ST_Custom &&
                static_cast<CustomSubscriberSlot*>( (*Current) )->GetSubscriber() == Subscriber )
            {
                // Just escape from here, returning the pre-existing slot
                return (*Current);
            }
        }

        CustomSubscriberSlot* NewSlot = new CustomSubscriberSlot(this,Subscriber);
        this->Slots.push_back( NewSlot );
        return NewSlot;
    }

    EventSubscriberSlot* Event::Subscribe(FunctorSubscriberSlot::FunctorDefinition* Funct, Boolean CleanUpAfter)
    {
        for( SlotIterator Current = this->Slots.begin() ; Current != this->Slots.end() ; ++Current )
        {
            if( (*Current)->GetType() == EventSubscriberSlot::ST_Functor &&
                static_cast<FunctorSubscriberSlot*>( (*Current) )->GetFunctor() == Funct )
            {
                // Just escape from here, returning the pre-existing slot
                return (*Current);
            }
        }

        FunctorSubscriberSlot* NewSlot = new FunctorSubscriberSlot(this,Funct,CleanUpAfter);
        this->Slots.push_back( NewSlot );
        return NewSlot;
    }

    EventSubscriberSlot* Event::Subscribe(CFunctionSubscriberSlot::SubscriberFunction* CFunct)
    {
        for( SlotIterator Current = this->Slots.begin() ; Current != this->Slots.end() ; ++Current )
        {
            if( (*Current)->GetType() == EventSubscriberSlot::ST_CFunction &&
                static_cast<CFunctionSubscriberSlot*>( (*Current) )->GetFunction() == CFunct )
            {
                // Just escape from here, returning the pre-existing slot
                return (*Current);
            }
        }

        CFunctionSubscriberSlot* NewSlot = new CFunctionSubscriberSlot(this,CFunct);
        this->Slots.push_back( NewSlot );
        return NewSlot;
    }

    EventSubscriberSlot* Event::Subscribe(Scripting::iScript* SubScript)
    {
        for( SlotIterator Current = this->Slots.begin() ; Current != this->Slots.end() ; ++Current )
        {
            if( (*Current)->GetType() == EventSubscriberSlot::ST_Script &&
                static_cast<ScriptSubscriberSlot*>( (*Current) )->GetScript() == SubScript )
            {
                // Just escape from here, returning the pre-existing slot
                return (*Current);
            }
        }

        ScriptSubscriberSlot* NewSlot = new ScriptSubscriberSlot(this,SubScript);
        this->Slots.push_back( NewSlot );
        return NewSlot;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Unsubscribe Methods

    void Event::Unsubscribe(EventSubscriber* Subscriber)
    {
        for( SlotIterator Current = this->Slots.begin() ; Current != this->Slots.end() ; ++Current )
        {
            if( (*Current)->GetType() == EventSubscriberSlot::ST_Custom &&
                static_cast<CustomSubscriberSlot*>( (*Current) )->GetSubscriber() == Subscriber )
            {
                delete (*Current);
                this->Slots.erase(Current);
                return;
            }
        }
    }

    void Event::Unsubscribe(FunctorSubscriberSlot::FunctorDefinition* Funct)
    {
        for( SlotIterator Current = this->Slots.begin() ; Current != this->Slots.end() ; ++Current )
        {
            if( (*Current)->GetType() == EventSubscriberSlot::ST_Functor &&
                static_cast<FunctorSubscriberSlot*>( (*Current) )->GetFunctor() == Funct )
            {
                delete (*Current);
                this->Slots.erase(Current);
                return;
            }
        }
    }

    void Event::Unsubscribe(CFunctionSubscriberSlot::SubscriberFunction* CFunct)
    {
        for( SlotIterator Current = this->Slots.begin() ; Current != this->Slots.end() ; ++Current )
        {
            if( (*Current)->GetType() == EventSubscriberSlot::ST_CFunction &&
                static_cast<CFunctionSubscriberSlot*>( (*Current) )->GetFunction() == CFunct )
            {
                delete (*Current);
                this->Slots.erase(Current);
                return;
            }
        }
    }

    void Event::Unsubscribe(Scripting::iScript* SubScript)
    {
        for( SlotIterator Current = this->Slots.begin() ; Current != this->Slots.end() ; ++Current )
        {
            if( (*Current)->GetType() == EventSubscriberSlot::ST_Script &&
                static_cast<ScriptSubscriberSlot*>( (*Current) )->GetScript() == SubScript )
            {
                delete (*Current);
                this->Slots.erase(Current);
                return;
            }
        }
    }

    void Event::Unsubscribe(EventSubscriberSlot* SubSlot)
    {
        for( SlotIterator Current = this->Slots.begin() ; Current != this->Slots.end() ; ++Current )
        {
            if( (*Current) == SubSlot ) {
                delete (*Current);
                this->Slots.erase(Current);
                return;
            }
        }
    }

    Whole Event::UnsubscribeAll()
    {
        Whole PrevSize = this->Slots.size();
        for( SlotIterator Current = this->Slots.begin() ; Current != this->Slots.end() ; ++Current )
            { delete (*Current); }
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
            (*SlotIt)->_NotifyEvent(Args);
        }
    }
}

#endif
