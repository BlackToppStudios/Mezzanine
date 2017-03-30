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
#ifndef _eventsubscriberbinding_cpp
#define _eventsubscriberbinding_cpp

#include "eventsubscriberbinding.h"
#include "eventpublisher.h"

namespace Mezzanine
{
    EventSubscriberBinding::EventSubscriberBinding(EventSubscriberID ID, const CallbackType& Delegate, EventPublisher* Pub, const EventHashType Hash) :
        Callback(Delegate),
        SubID(ID),
        Publisher(Pub),
        NameHash(Hash)
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Operators

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    const EventSubscriberBinding::CallbackType& EventSubscriberBinding::GetCallback() const
        { return this->Callback; }

    EventSubscriberID EventSubscriberBinding::GetSubID() const
        { return this->SubID; }

    EventPublisher* EventSubscriberBinding::GetPublisher() const
        { return this->Publisher; }

    EventHashType EventSubscriberBinding::GetEventHash() const
        { return this->NameHash; }

    Boole EventSubscriberBinding::IsSubscribed() const
        { return ( this->Publisher != NULL ); }

    void EventSubscriberBinding::Unsubscribe()
    {
        this->Publisher->GetEventTable(this->NameHash)->Unsubscribe(this->SubID);
        this->Unbind();
    }

    void EventSubscriberBinding::Unbind()
    {
        this->Publisher = NULL;
        this->NameHash = EventNameType::EmptyHash;
    }

    void EventSubscriberBinding::DispatchEvent(EventPtr Args) const
    {
        if( this->Callback ) {
            this->Callback(Args);
        }
    }
}//Mezzanine

#endif
