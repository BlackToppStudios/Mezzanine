// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
    ///////////////////////////////////////////////////////////////////////////////
    // EventPublisherMethods

    EventPublisher::EventPublisher() :
        MuteEvents(false)
        {  }

    EventPublisher::EventPublisher(const Whole EventCount) :
        MuteEvents(false)
        { this->EventTables.reserve(EventCount); }

    ///////////////////////////////////////////////////////////////////////////////
    // Operators

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void EventPublisher::SetMuteEvents(const Boole Mute)
        { this->MuteEvents = Mute; }

    Boole EventPublisher::GetMuteEvents() const
        { return this->MuteEvents; }

    void EventPublisher::DispatchEvent(EventPtr Args) const
    {
        if( !this->MuteEvents ) {
            this->GetEventTable(Args->EventName)->_DispatchEvent(Args);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Event Table Management

    EventPublisher::EventTableIterator EventPublisher::AddEventTable(const HashedString32& EventName)
    {
        EventTableIterator EvIt = this->EventTables.find(EventName);
        if( EvIt == this->EventTables.end() ) {
            return this->EventTables.add_emplace([](const EventSubscriberTable& EvTable, const HashedString32& EventName) {
                return EvTable.GetName() < EventName;
            }, EventName);
        }else{
            return EvIt;
        }
    }

    Boole EventPublisher::HasEventTable(const HashedString32& EventName) const
    {
        ConstEventTableIterator EvIt = this->EventTables.find(EventName);
        return EvIt != this->EventTables.end();
    }

    Boole EventPublisher::HasEventTable(const Int32 EventHash) const
    {
        for( const EventSubscriberTable& CurrTable : this->EventTables )
        {
            if( CurrTable.GetName().GetHash() == EventHash ) {
                return true;
            }
        }
        return false;
    }

    EventPublisher::EventTableIterator EventPublisher::GetEventTable(const HashedString32& EventName)
    {
        EventTableIterator EvIt = this->EventTables.find(EventName);
        if( EvIt != this->EventTables.end() ) {
            return EvIt;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"Event name \"" + EventName + "\" not found in publisher.");
        }
        return this->EventTables.end();
    }

    EventPublisher::EventTableIterator EventPublisher::GetEventTable(const Int32 EventHash)
    {
        for( EventTableIterator TableIt = this->EventTables.begin() ; TableIt != this->EventTables.end() ; ++TableIt )
        {
            if( (*TableIt).GetName().GetHash() == EventHash ) {
                return TableIt;
            }
        }
        StringStream ExceptionStream;
        ExceptionStream << "Event hash \"" << EventHash << "\" not found in publisher.";
        MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
    }

    EventPublisher::ConstEventTableIterator EventPublisher::GetEventTable(const HashedString32& EventName) const
    {
        ConstEventTableIterator EvIt = this->EventTables.find(EventName);
        if( EvIt != this->EventTables.end() ) {
            return EvIt;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"Event name \"" + EventName + "\" not found in publisher.");
        }
        return this->EventTables.end();
    }

    EventPublisher::ConstEventTableIterator EventPublisher::GetEventTable(const Int32 EventHash) const
    {
        for( ConstEventTableIterator TableIt = this->EventTables.begin() ; TableIt != this->EventTables.end() ; ++TableIt )
        {
            if( (*TableIt).GetName().GetHash() == EventHash ) {
                return TableIt;
            }
        }
        StringStream ExceptionStream;
        ExceptionStream << "Event hash \"" << EventHash << "\" not found in publisher.";
        MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
    }

    void EventPublisher::RemoveEventTable(const HashedString32& EventName)
    {
        EventTableIterator EvIt = this->EventTables.find(EventName);
        if( EvIt != this->EventTables.end() ) {
            this->EventTables.erase(EvIt);
        }
    }

    void EventPublisher::RemoveAllEventTables()
        { this->EventTables.clear(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscription Management

    EventSubscriberBindingPtr EventPublisher::Subscribe(const HashedString32& EventName, SubscriberID ID, const CallbackType& Delegate)
        { return this->GetEventTable(EventName)->Subscribe(ID,Delegate,this); }

    void EventPublisher::Unsubscribe(const HashedString32& EventName, SubscriberID ID)
        { this->GetEventTable(EventName)->Unsubscribe(ID); }

    Whole EventPublisher::UnsubscribeAll(const HashedString32& EventName)
        { return this->GetEventTable(EventName)->UnsubscribeAll(); }

    void EventPublisher::Unsubscribe(SubscriberID ID)
    {
        for( EventSubscriberTable& CurrTable : this->EventTables )
            { CurrTable.Unsubscribe(ID); }
    }

    Whole EventPublisher::UnsubscribeAll()
    {
        Whole Ret = 0;
        for( EventSubscriberTable& CurrTable : this->EventTables )
            { Ret += CurrTable.UnsubscribeAll(); }
        return Ret;
    }
}//Mezzanine

#endif
