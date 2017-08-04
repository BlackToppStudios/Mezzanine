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

    EventPublisher::EventPublisher(const Whole EventCapacity) :
        MuteEvents(false)
        { this->SubscriptionTables.reserve(EventCapacity); }

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
            this->GetSubscriptionTable(Args->EventName)->DispatchEvent(Args);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscription Table Management


    ///////////////////////////////////////////////////////////////////////////////
    // Subscription Table Management - Via Name

    EventPublisher::SubscriptionTableIterator EventPublisher::AddSubscriptionTable(const EventNameType& EventName)
        { return this->AddSubscriptionTable(EventName.GetHash()); }

    Boole EventPublisher::HasSubscriptionTable(const EventNameType& EventName) const
        { return this->HasSubscriptionTable(EventName.GetHash()); }

    EventPublisher::SubscriptionTableIterator EventPublisher::GetSubscriptionTable(const EventNameType& EventName)
        { return this->GetSubscriptionTable(EventName.GetHash()); }

    EventPublisher::ConstSubscriptionTableIterator EventPublisher::GetSubscriptionTable(const EventNameType& EventName) const
        { return this->GetSubscriptionTable(EventName.GetHash()); }

    void EventPublisher::RemoveSubscriptionTable(const EventNameType& EventName)
        { this->RemoveSubscriptionTable(EventName.GetHash()); }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscription Table Management - Via Hash

    EventPublisher::SubscriptionTableIterator EventPublisher::AddSubscriptionTable(const EventHashType EventHash)
    {
        SubscriptionTableIterator TableIt = this->SubscriptionTables.find(EventHash);
        if( TableIt == this->SubscriptionTables.end() ) {
            return this->SubscriptionTables.add_emplace([](const EventSubscriptionTable& EvTable, EventHashType EventHash) {
                return EvTable.GetHash() < EventHash;
            }, EventHash);
        }else{
            StringStream ExceptionStream;
            ExceptionStream << "An EventSubscriptionTable with the hash \"" <<  EventHash << "\" already exists!" << std::endl;
            MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,ExceptionStream.str());
        }
    }

    Boole EventPublisher::HasSubscriptionTable(const EventHashType EventHash) const
    {
        ConstSubscriptionTableIterator TableIt = this->SubscriptionTables.find(EventHash);
        return TableIt != this->SubscriptionTables.end();
    }

    EventPublisher::SubscriptionTableIterator EventPublisher::GetSubscriptionTable(const EventHashType EventHash)
    {
        SubscriptionTableIterator TableIt = this->SubscriptionTables.find(EventHash);
        if( TableIt != this->SubscriptionTables.end() ) {
            return TableIt;
        }else{
            StringStream ExceptionStream;
            ExceptionStream << "Event hash \"" << EventHash << "\" not found in publisher." << std::endl;
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
        }
        return this->SubscriptionTables.end();
    }

    EventPublisher::ConstSubscriptionTableIterator EventPublisher::GetSubscriptionTable(const EventHashType EventHash) const
    {
        ConstSubscriptionTableIterator TableIt = this->SubscriptionTables.find(EventHash);
        if( TableIt != this->SubscriptionTables.end() ) {
            return TableIt;
        }else{
            StringStream ExceptionStream;
            ExceptionStream << "Event hash \"" << EventHash << "\" not found in publisher." << std::endl;
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
        }
        return this->SubscriptionTables.end();
    }

    void EventPublisher::RemoveSubscriptionTable(const EventHashType EventHash)
    {
        SubscriptionTableIterator TableIt = this->SubscriptionTables.find(EventHash);
        if( TableIt != this->SubscriptionTables.end() ) {
            this->SubscriptionTables.erase(TableIt);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscription Table Management

    void EventPublisher::RemoveAllSubscriptionTables()
        { this->SubscriptionTables.clear(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscription Management - Via Name

    EventSubscriberBindingPtr EventPublisher::Subscribe(const EventNameType& EventName, const EventSubscriberID ID, const CallbackType& Delegate)
        { return this->Subscribe(EventName.GetHash(),ID,Delegate); }

    void EventPublisher::Unsubscribe(const EventNameType& EventName, const EventSubscriberID ID)
        { this->Unsubscribe(EventName.GetHash(),ID); }

    Whole EventPublisher::UnsubscribeAll(const EventNameType& EventName)
        { return this->UnsubscribeAll(EventName.GetHash()); }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscription Management - Via Hash

    EventSubscriberBindingPtr EventPublisher::Subscribe(const EventHashType EventHash, const EventSubscriberID ID, const CallbackType& Delegate)
        { return this->GetSubscriptionTable(EventHash)->Subscribe(ID,Delegate,this); }

    void EventPublisher::Unsubscribe(const EventHashType EventHash, const EventSubscriberID ID)
        { this->GetSubscriptionTable(EventHash)->Unsubscribe(ID); }

    Whole EventPublisher::UnsubscribeAll(const EventHashType EventHash)
        { return this->GetSubscriptionTable(EventHash)->UnsubscribeAll(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscription Management

    void EventPublisher::Unsubscribe(const EventSubscriberID ID)
    {
        for( EventSubscriptionTable& CurrTable : this->SubscriptionTables )
            { CurrTable.Unsubscribe(ID); }
    }

    Whole EventPublisher::UnsubscribeAll()
    {
        Whole Ret = 0;
        for( EventSubscriptionTable& CurrTable : this->SubscriptionTables )
            { Ret += CurrTable.UnsubscribeAll(); }
        return Ret;
    }
}//Mezzanine

#endif
