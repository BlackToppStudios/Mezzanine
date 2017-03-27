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
#ifndef _eventsubscriptiontable_cpp
#define _eventsubscriptiontable_cpp

#include "eventsubscriptiontable.h"
#include "exception.h"

namespace Mezzanine
{
    EventSubscriptionTable::EventSubscriptionTable(const HashedString32& Name) :
        EventName(Name)
        {  }

    EventSubscriptionTable::~EventSubscriptionTable()
        { this->UnsubscribeAll(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Operators

    Boole EventSubscriptionTable::operator<(const EventSubscriptionTable& Other) const
        { return this->EventName < Other.EventName; }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    const HashedString32& EventSubscriptionTable::GetName() const
        { return this->EventName; }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscription Management

    EventSubscriberBindingPtr EventSubscriptionTable::Subscribe(SubscriberID ID, const CallbackType& Delegate, EventPublisher* Pub)
    {
        EventSubscriberBindingPtr NewBinding = this->GetBinding(ID);
        if( NewBinding.use_count() > 0 /* != NULL */ ) {
            MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,"A subscriber with that ID already exists!");
        }
        NewBinding.reset( new EventSubscriberBinding(ID,Delegate,Pub,this->EventName.GetHash()) );
        this->Bindings.push_back(NewBinding);
        return NewBinding;
    }

    EventSubscriberBindingPtr EventSubscriptionTable::GetBinding(SubscriberID ID)
    {
        for( BindingIterator BindIt = this->Bindings.begin() ; BindIt != this->Bindings.end() ; ++BindIt )
        {
            if( (*BindIt)->GetSubID() == ID ) {
                return (*BindIt);
            }
        }
        return NULL;
    }

    void EventSubscriptionTable::Unsubscribe(SubscriberID ID)
    {
        for( BindingIterator BindIt = this->Bindings.begin() ; BindIt != this->Bindings.end() ; ++BindIt )
        {
            if( (*BindIt)->GetSubID() == ID ) {
                (*BindIt)->Unbind();
                this->Bindings.erase(BindIt);
                return;
            }
        }
    }

    Whole EventSubscriptionTable::UnsubscribeAll()
    {
        Whole RemoveCount = this->Bindings.size();
        for( BindingIterator BindIt = this->Bindings.begin() ; BindIt != this->Bindings.end() ; ++BindIt )
        {
            (*BindIt)->Unbind();
        }
        this->Bindings.clear();
        return RemoveCount;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void EventSubscriptionTable::_DispatchEvent(EventPtr Args) const
    {
        for( ConstBindingIterator BindIt = this->Bindings.begin() ; BindIt != this->Bindings.end() ; ++BindIt )
            { (*BindIt)->DispatchEvent(Args); }
    }
}//Mezzanine

#endif
