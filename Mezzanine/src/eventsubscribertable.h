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
#ifndef _eventsubscribertable_h
#define _eventsubscribertable_h

#include "hashedstring.h"
#include "event.h"

namespace Mezzanine
{
    class Event;
    class EventSubscriber;
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This class represents a given event that can be subscribed to and/or fired.
    ///////////////////////////////////////
	class MEZZ_LIB EventSubscriberTable
	{
    public:
        /// @brief Convenience type for the callbacks that will be called when events are fired.
        using CallbackType = std::function< Boole(EventPtr) >;
        /// @brief Container for the storage of subscribers interested in an event.
        using SubscriberContainer = std::vector<EventSubscriber*>;
        /// @brief Iterator type for subscribers stored by this table.
        using SubscriberIterator = SubscriberContainer::iterator;
        /// @brief Const Iterator type for subscribers stored by this table.
        using ConstSubscriberIterator = SubscriberContainer::const_iterator;
    protected:
        /// @brief The name of the Event the subscribers in this table are subscribed to.
        HashedString32 EventName;
    public:
        /// @brief Blank constructor.
        EventSubscriberTable() = delete;
        /// @brief Copy constructor.
        /// @param Other The other table to be copied.
        EventSubscriberTable(const EventSubscriberTable& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other table to be moved.
        EventSubscriberTable(EventSubscriberTable&& Other) = default;
        /// @brief Class constructor.
        /// @param Name The name to be given to this event.
        EventSubscriberTable(const HashedString32& Name);
        /// @brief Class destructor.
        ~EventSubscriberTable() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the name of the event associated with this table.
        /// @return Returns a const reference of a hashed string containing the name of this event.
        const HashedString32& GetName() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Subscription Management

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other table to be copied.
        /// @return Returns a reference to this.
        EventSubscriberTable& operator=(const EventSubscriberTable& Other) = default;
        /// @brief Move assignment operator.
        /// @param Other The other table to be moved.
        /// @return Returns a reference to this.
        EventSubscriberTable& operator=(EventSubscriberTable&& Other) = default;

        /// @brief Less-than operator.
        /// @param Other The other table to be compared to.
        /// @return Returns true if this table is considered less than the other, or should be sorted before the other table.
        Boole operator<(const EventSubscriberTable& Other) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @internal
        /// @brief Notifies all subscribers of this event that this event is firing.
        /// @param Args The arguments and extra data related to this event.
        void _FireEvent(EventArgumentsPtr Args) const;
	};//EventSubscriberTable
}//Mezzanine

#endif
