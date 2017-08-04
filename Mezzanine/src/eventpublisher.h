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
#ifndef _eventpublisher_h
#define _eventpublisher_h

#include "hashedstring.h"
#include "exception.h"
#include "sortedvector.h"
#include "eventsubscriptiontable.h"

namespace Mezzanine
{
    /// @addtogroup Events
    /// @{

    /// @page Mezzanine Event System
    /// Despite the name of the primary classes involved, the Mezzanine Event system more closely resembles the
    /// Observer Pattern for messaging.  It was designed to be as flexible as possible in common game development
    /// tasks with a number of considerations for speed (although at the time of this writing the system prioritizes
    /// flexibility over speed in situations where one has to be chosen over the other).
    /// @n @n
    /// The primary criteria for the design of the Event System are: @n
    ///     - Scalable. @n
    ///       No forced monolithic design or master publisher.  Objects can arbitrarily use the event
    ///       system classes where they want, be it a simulation object, an application object (such as a
    ///       window), a manager for a subsystem, or (ironically) a monolithic singleton.  Although I don't
    ///       recommend that last one.
    ///     - Modular. @n
    ///       No EventPublisher needs to be aware of any other EventPublisher.  Each EventPublisher is it's own
    ///       contained event ecosystem.
    ///     - Subscribe to specific events. @n
    ///       In most event systems you subscribe to a publisher and receive all of it's events.  With the
    ///       Mezzanine Event System you can subscribe to just one, or however many specific events you are
    ///       interested in.  In some cases this means fewer checks in the callback logic, but it also means
    ///       less time is wasted on uninterested subscribers during event dispatch.
    ///     - Non-Polymorphic Subscribers. @n
    ///       We use templated delegates for event dispatch, not a polymorphic base class.  To be specific, at
    ///       the time of this writing we use std::function.  This allows an extreme amount of versatility in
    ///       subscribers with minimal performance overhead provided you make use of it's small function
    ///       optimization.  Subscribers need only match the signature "void(EventPtr)".
    ///     - Flexible Publisher usage. @n
    ///       Publishers are not interfaces and have no virtual methods.  You are free to inherit from it or
    ///       store it as a data member, whichever is more appropriate.  In fact most of the event system is
    ///       designed without inheritance or virtual methods.  The dispatched events themselves are the one
    ///       exception.
    ///     - No forced binding bookkeeping. @n
    ///       To help with corner cases where both the publisher and subscriber have finite lifetimes we create
    ///       bindings to represent the subscriptions which are returned upon subscription.  This are stored in
    ///       shared_ptr's and your subscribers may hold on to them if needed to check if a publisher exists, or
    ///       can ignore them entirely.
    ///     - Non-Enum Event IDs. @n
    ///       Enums are great, however since publishers are their own event ecosystems, and nothing stops a
    ///       subscriber from subscribing to multiple publishers it would be possible for event enums between
    ///       publishers to have the same value, which can be difficult to sort out in the subscriber logic.
    ///       Instead we use hashed strings (EventNameType) to define our events which are potentially more
    ///       descriptive as well as easier to distinguish and extend than enums.  It can also facilitate
    ///       integration in data driven systems.
    ///     - Flexible Subscriber IDs. @n
    ///       The EventPublisher class will not generate IDs for you.  Ultimately IDs are stored as uintptr_t
    ///       specifically to allow pointers to be used as unique IDs, but this isn't required and doesn't match
    ///       the requirements in all cases.  So you are free to specify whatever identification scheme you wish,
    ///       the only requirement that exists is that the ID be unique to the event subscribed to.
    /// @n @n
    /// As far as messaging schemes go Event/Publisher and Observer are some of the slower ones out there.  There are
    /// many situations where direct messaging or listeners may be more appropriate.  That said, this Event system
    /// excels primarily when you are connecting multiple systems together.  Different systems will have different
    /// requirements for what is passed around and how that is passed into the system.  This Event systems offers that
    /// flexibility.  This IS NOT an answer for all of your messaging needs.  Just the more demanding ones.
    /// @n @n
    /// The usage of the Mezzanine Event System is pretty straightforward.  First you set up the events in the publisher
    /// and then subscribe objects to those events, and when appropriate fire events that occur.
    /// @n @n
    /// Once an EventPublisher is created you should add subscriber tables to it for each event you want it to dispatch.
    /// You do this by calling EventPublisher::AddEventTable(const EventNameType&).  If the named event already exists
    /// then this will return an iterator to the pre-existing table.  Otherwise you'll get an iterator to the created
    /// table, either way you get a valid table.  The underlying implementation does sort the table based on the hash
    /// of the event name AND it does so in a vector.  So there's two reasons to not expect the iterator you are given
    /// to remain valid after multiple insertions.  Once a subscriber table is made via an event name, you are good to
    /// subscribe to that event with the same name.
    /// @n @n
    /// You subscribe to an event by providing the name of the event you want to subscribe to, an ID for the delegate,
    /// and the delegate itself.  If the publisher doesn't have a subscriber table for the named event, an exception
    /// will be thrown.  Otherwise you'll get back a binding you can use or ignore.
    /// @n @n
    /// Firing events is the most manual part.  Some events are simple enough that the event name is descriptive enough.
    /// For those events you just create an EventPtr with the name of the event and call DispatchEvent(EventPtr).  Simple.
    /// If you need to pass additional parameters you are expected to inherit from the Event base class and add the
    /// parameters as data members.  Your subscribers will also need to be aware of the event and perform a cast on
    /// its end to access the parameters.  The steps are mostly identical otherwise.  Construct your custom Event
    /// with the relevant parameters and call DispatchEvent(EventPtr).
    /// @n @n
    /// In general it is expected that either the subscriber or the system the subscriber belongs to will manage the
    /// subscription lifetime.  For some things lifetime isn't a consideration, such as subscribing a non-member
    /// function.  For the situations where it does matter and it's possible the publisher may disappear before
    /// the subscriber, it's a VERY good idea to keep track of the returned binding.  It's also possible to retrieve
    /// a binding from a subscription table using the subscriber ID if you don't want to take it immediately when it's
    /// returned from the Subscribe method.  The binding has a method that will safely check if it is still bound to
    /// publisher.  It also has a convenience method that will unsubscribe it from the bound publisher.
    /// @n @n
    /// There are a few dont's to keep in mind while using the Mezzanine Event System:
    ///     - Due to the use of pointers to publishers (in bindings), it is advised to avoid storing EventPublishers
    ///       where their address may become invalidated.
    ///     - Do not attempt to add/remove subscribers or destroy the publisher/subscription table during the dispatch
    ///       sequence.  The system offers no guarantees that it'll work, although it might.  Robust support for this
    ///       may be added in the future but does not exist at the time of this writing.
    ///     - When using the event system to communicate across multiple threads (or in the context of the Mezzanine,
    ///       across WorkUnits) it is STRONGLY recommended that you store the dispatched Events in a thread-safe Queue
    ///       and process the Events later, instead of trying to process them immediately.  It's far to easy to create
    ///       stalls and/or race conditions otherwise.

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is the base class for any class that generates and publishes events to subscribers.
    ///////////////////////////////////////
    class MEZZ_LIB EventPublisher
    {
    public:
        /// @brief Convenience type for the callbacks that will be called when events are fired.
        using CallbackType = EventSubscriptionTable::CallbackType;
        /// @brief Basic container type for @ref EventSubscriptionTable storage by this class.
        using SubscriptionTableContainer = SortedVector<EventSubscriptionTable>;
        /// @brief Iterator type for @ref EventSubscriptionTable instances stored by this class.
        using SubscriptionTableIterator = SubscriptionTableContainer::iterator;
        /// @brief Const Iterator type for @ref EventSubscriptionTable instances stored by this class.
        using ConstSubscriptionTableIterator = SubscriptionTableContainer::const_iterator;
    protected:
        /// @brief A container storing all the Events published by this class by name.
        SubscriptionTableContainer SubscriptionTables;
        /// @brief Stores whether or not events will actually be fired when requested.
        Boole MuteEvents;
    public:
        /// @brief Class constructor.
        EventPublisher();
        /// @brief Reserving constructor.
        /// @param EventCapacity The number of events to expect this publisher is expected to be populated with.
        EventPublisher(const Whole EventCapacity);
        /// @brief Copy constructor.
        /// @param Other The other publisher to be copied.
        EventPublisher(const EventPublisher& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other publisher to be moved.
        EventPublisher(EventPublisher&& Other) = default;
        /// @brief Class destructor.
        ~EventPublisher() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other publisher to be copied.
        /// @return Returns a reference to this.
        EventPublisher& operator=(const EventPublisher& Other) = default;
        /// @brief Move assignment operator.
        /// @param Other The other publisher to be moved.
        /// @return Returns a reference to this.
        EventPublisher& operator=(EventPublisher&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Sets whether or not event firings by this publisher will be suppressed.
        /// @param Mute True to prevent events from firing, false for normal operation.
        void SetMuteEvents(const Boole Mute);
        /// @brief Gets whether or not event firings by this publisher will be suppressed.
        /// @return Returns true if events are being suppressed, false if this publisher is operating normally.
        Boole GetMuteEvents() const;

        /// @brief Fires an event.
        /// @param Args The arguments/event specific data related to this event.
        void DispatchEvent(EventPtr Args) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Subscription Table Management - Via Name

        /// @brief Creates a new event table representing an event that can be subscribed to.
        /// @exception If a subscription table with the names hash already exists it will throw a "II_DUPLICATE_IDENTITY_EXCEPTION".
        /// @param EventName The name to be given to the new event subscription table.
        /// @return Returns an iterator to the created event table.
        SubscriptionTableIterator AddSubscriptionTable(const EventNameType& EventName);
        /// @brief Checks to see if an event table is registered with and has a subscriber table in this publisher.
        /// @param EventName The name of the table to check for.
        /// @return Returns true of the named event table is present in this publisher.
        Boole HasSubscriptionTable(const EventNameType& EventName) const;
        /// @brief Gets an event table in this publisher.
        /// @exception If this fails to find the table specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param EventName The name of the table to retrieve.
        /// @return Returns an iterator to the requested event table or throws an exception if it was not found.
        SubscriptionTableIterator GetSubscriptionTable(const EventNameType& EventName);
        /// @brief Gets an event table in this publisher.
        /// @exception If this fails to find the event specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param EventName The name of the event to retrieve.
        /// @return Returns a const iterator to the requested event table or throws an exception if it was not found.
        ConstSubscriptionTableIterator GetSubscriptionTable(const EventNameType& EventName) const;
        /// @brief Removes an existing event in this Publisher.
        /// @param EventName The name of the event to be removed.
        void RemoveSubscriptionTable(const EventNameType& EventName);

        ///////////////////////////////////////////////////////////////////////////////
        // Subscription Table Management - Via Hash

        /// @brief Creates a new event table representing an event that can be subscribed to.
        /// @exception If a subscription table with the names hash already exists it will throw a "II_DUPLICATE_IDENTITY_EXCEPTION".
        /// @param EventName The generated hash to use to identify the new event subscription table.
        /// @return Returns an iterator to the created event table.
        SubscriptionTableIterator AddSubscriptionTable(const EventHashType EventHash);
        /// @brief Checks to see if an event table is registered with and has a subscriber table in this publisher.
        /// @remarks The EventNameType overload of this method should be used instead where possible.
        /// @param EventHash The generated hash for the table name to check for.
        /// @return Returns true of the named event table is present in this publisher.
        Boole HasSubscriptionTable(const EventHashType EventHash) const;
        /// @brief Gets an event table in this publisher.
        /// @remarks The EventNameType overload of this method should be used instead where possible.
        /// @exception If this fails to find the event specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param EventHash The generated hash for the event name to retrieve.
        /// @return Returns an iterator to the requested event table or throws an exception if it was not found.
        SubscriptionTableIterator GetSubscriptionTable(const EventHashType EventHash);
        /// @brief Gets an event table in this publisher.
        /// @remarks The EventNameType overload of this method should be used instead where possible.
        /// @exception If this fails to find the event specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param EventHash The generated hash for the event name to retrieve.
        /// @return Returns a const iterator to the requested event table or throws an exception if it was not found.
        ConstSubscriptionTableIterator GetSubscriptionTable(const EventHashType EventHash) const;
        /// @brief Removes an existing event in this Publisher.
        /// @param EventHash The generated hash for the event name to remove.
        void RemoveSubscriptionTable(const EventHashType EventHash);

        ///////////////////////////////////////////////////////////////////////////////
        // Subscription Table Management

        /// @brief Removes all events in this Publisher.
        void RemoveAllSubscriptionTables();

        ///////////////////////////////////////////////////////////////////////////////
        // Subscription Management - Via Name

        /// @brief Adds a subscriber to this event.
        /// @exception If this fails to find the event specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param EventName The name of the event to subscribe to.
        /// @param ID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        /// @param Delegate The callback to be called when the interested event is fired.
        /// @return Returns a pointer to the created Subscriber slot for the provided subscriber.
        EventSubscriberBindingPtr Subscribe(const EventNameType& EventName, const EventSubscriberID ID, const CallbackType& Delegate);
        /// @brief Removes a single subscriber from the named event.
        /// @exception If this fails to find the event specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param EventName The name of the event to unsubscribe from.
        /// @param ID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        void Unsubscribe(const EventNameType& EventName, const EventSubscriberID ID);
        /// @brief Removes all subscribers from the named Event.
        /// @exception If this fails to find the event specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param EventName The name of the event to unsubscribe from.
        /// @return Returns the number of subscribers removed.
        Whole UnsubscribeAll(const EventNameType& EventName);

        ///////////////////////////////////////////////////////////////////////////////
        // Subscription Management - Via Hash

        /// @brief Adds a subscriber to this event.
        /// @exception If this fails to find the event specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param EventHash The generated hash identifying the event to subscribe to.
        /// @param ID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        /// @param Delegate The callback to be called when the interested event is fired.
        /// @return Returns a pointer to the created Subscriber slot for the provided subscriber.
        EventSubscriberBindingPtr Subscribe(const EventHashType EventHash, const EventSubscriberID ID, const CallbackType& Delegate);
        /// @brief Removes a single subscriber from the named event.
        /// @exception If this fails to find the event specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param EventHash The generated hash identifying the event to unsubscribe from.
        /// @param ID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        void Unsubscribe(const EventHashType EventHash, const EventSubscriberID ID);
        /// @brief Removes all subscribers from the named Event.
        /// @exception If this fails to find the event specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param EventHash The generated hash identifying the event to unsubscribe from.
        /// @return Returns the number of subscribers removed.
        Whole UnsubscribeAll(const EventHashType EventHash);

        ///////////////////////////////////////////////////////////////////////////////
        // Subscription Management

        /// @brief Removes a single subscriber from all events in this publisher.
        /// @param ID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        void Unsubscribe(const EventSubscriberID ID);
        /// @brief Removes all subscribers from all events in this publisher.
        /// @return Returns the number of subscribers removed.
        Whole UnsubscribeAll();
    };//EventPublisher

    /// @}
}//Mezzanine

#endif
