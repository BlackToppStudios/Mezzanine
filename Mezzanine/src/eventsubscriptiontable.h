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
#ifndef _eventsubscriptiontable_h
#define _eventsubscriptiontable_h

#include "hashedstring.h"
#include "eventsubscriberbinding.h"

namespace Mezzanine
{
    /// @addtogroup Events
    /// @{

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This class represents a given event that can be subscribed to and/or fired.
    ///////////////////////////////////////
	class MEZZ_LIB EventSubscriptionTable
	{
    public:
        /// @brief Convenience type for the callbacks that will be called when events are fired.
        using CallbackType = EventSubscriberBinding::CallbackType;
        /// @brief Container for the storage of bindings between subscribers and the events they are interested in.
        using BindingContainer = std::vector<EventSubscriberBindingPtr>;
        /// @brief Iterator type for subscriber bindings stored by this table.
        using BindingIterator = BindingContainer::iterator;
        /// @brief Const Iterator type for subscriber bindings stored by this table.
        using ConstBindingIterator = BindingContainer::const_iterator;
    protected:
        /// @brief A container of all the subscriber bindings to this event table.
        BindingContainer Bindings;
        /// @brief The name of the Event the subscribers in this table are subscribed to.
        EventNameType EventName;
    public:
        /// @brief Blank constructor.
        EventSubscriptionTable() = delete;
        /// @brief Copy constructor.
        /// @param Other The other table to be copied.
        EventSubscriptionTable(const EventSubscriptionTable& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other table to be moved.
        EventSubscriptionTable(EventSubscriptionTable&& Other) = default;
        /// @brief Class constructor.
        /// @param Name The name to be given to this event.
        EventSubscriptionTable(const EventNameType& Name);
        /// @brief Class destructor.
        ~EventSubscriptionTable();

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other table to be copied.
        /// @return Returns a reference to this.
        EventSubscriptionTable& operator=(const EventSubscriptionTable& Other) = default;
        /// @brief Move assignment operator.
        /// @param Other The other table to be moved.
        /// @return Returns a reference to this.
        EventSubscriptionTable& operator=(EventSubscriptionTable&& Other) = default;

        /// @brief Less-than operator.
        /// @param Other The other table to be compared to.
        /// @return Returns true if this table is considered less than the other, or should be sorted before the other table.
        Boole operator<(const EventSubscriptionTable& Other) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the name of the event associated with this table.
        /// @return Returns a const reference of a hashed string containing the name of this event.
        const EventNameType& GetName() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Subscription Management

        /// @brief Adds a subscriber to this event table.
        /// @exception If ID provided is already being used by a binding/subscriber this will throw a "II_DUPLICATE_IDENTITY_EXCEPTION".
        /// @param ID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        /// @param Delegate The callback to be called when the interested event is fired.
        /// @return Returns a pointer to the created Subscriber slot for the provided subscriber.
        EventSubscriberBindingPtr Subscribe(EventSubscriberID ID, const CallbackType& Delegate, EventPublisher* Pub);
        /// @brief Gets a binding by the subscriber ID.
        /// @param ID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        /// @return Returns the binding with the specified ID, or NULL of none exists.
        EventSubscriberBindingPtr GetBinding(EventSubscriberID ID);

        /// @brief Removes a single subscriber from this event table.
        /// @param ID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        void Unsubscribe(EventSubscriberID ID);
        /// @brief Removes all subscribers from all events in this publisher.
        /// @return Returns the number of subscribers removed.
        Whole UnsubscribeAll();

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @internal
        /// @brief Notifies all subscribers of this event that this event is firing.
        /// @param Args The arguments and extra data related to this event.
        void DispatchEvent(EventPtr Args) const;
	};//EventSubscriptionTable

    /// @}
}//Mezzanine

#endif
