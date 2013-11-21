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
#ifndef _eventconnection_h
#define _eventconnection_h

#include "countedptr.h"

namespace Mezzanine
{
    class Event;
    class EventSubscriber;
	///////////////////////////////////////////////////////////////////////////////
    /// @brief This class wraps a subscriber being subscribed to an event.
    /// @details This base class is useful for subscribers that do not need or want to be
	/// destroyed when no longer subscribed to any events.  For that see VolitileEventSunscriberSlot.
    ///////////////////////////////////////
	class MEZZ_LIB EventConnection
	{
    protected:
        /// @internal
        /// @brief A pointer to the connected subscriber.
        EventSubscriber* Sub;
        /// @internal
        /// @brief A pointer to the connected event.
        Event* Ev;
    public:
        /// @brief Class constructor.
        /// @param TheEvent The event being subscribed to.
        /// @param Subscriber The subscriber to wrap.
        EventConnection(Event* TheEvent, EventSubscriber* Subscriber);
        /// @brief Class destructor.
        virtual ~EventConnection();
    //public:
        /// @brief Gets the wrapped subscriber.
        /// @return Returns a pointer to the subscriber being wrapped.
        EventSubscriber* GetSubscriber() const;
        /// @brief Gets the set of events the subscriber is subscribed to.
        /// @return Returns a reference to the set of events this is subscribed to.
        Event* GetEvent() const;

        /// @brief Checks to see if there is currently a valid connection between event and subscriber.
        /// @return Returns true if there is a connection, false otherwise.
        bool IsConnected() const;

        /// @internal
        /// @brief Disconnects the existing subscriber from this slot, allowing it to be removed.
        /// @param Subscriber The subscriber calling this method.
        void Disconnect(EventSubscriber* Subscriber);
        /// @internal
        /// @brief Disconnects the existing subscriber from this slot, allowing it to be removed.
        /// @param TheEvent The event calling this method.
        void Disconnect(Event* TheEvent);
	};//EventConnection

	///////////////////////////////////////////////////////////////////////////////
    /// @brief This class wraps a subscriber being subscribed to an event.
    /// @details This class will destroy it's underlying subscriber when it is destroyed(and no longer
	/// subscribed to anything).  For any object that you want to last longer then that use the base class
	/// instead.
    ///////////////////////////////////////
    class MEZZ_LIB VolatileEventConnection : public EventConnection
	{
    //protected:
    public:
        /// @brief Class constructor.
        /// @param TheEvent The event being subscribed to.
        /// @param Subscriber The subscriber to wrap.
        VolatileEventConnection(Event* TheEvent, EventSubscriber* Subscriber);
        /// @brief Class destructor.
        virtual ~VolatileEventConnection();
	};//VolitileEventConnection

	/// @typedef EventConnectionPtr
	/// @brief Typedef for working with EventConnections in a shared pointer.
	typedef CountedPtr<EventConnection> EventConnectionPtr;
}//Mezzanine

#endif
