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
#include "eventdispatcher.h"
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
    /// @brief This is responsible for taking a dispatch request and forwarding it to the appropriate subscription table.
    ///////////////////////////////////////
    class MEZZ_LIB DefaultEventPublisherTraits
    {
    public:
        /// @brief The subscriber type that will be invoked when an Event is fired.
        using InterfaceType = std::function<Boole(EventPtr)>;
        /// @brief The Event type that will contain Event information.
        using EventIDType = EventHashType;
        /// @brief The type that will be used to provide more verbose identification of the Event.
        using EventDescriptorType = EventNameType;
        /// @brief The type that will be used to uniquely identify each subscriber.
        using SubscriberIDType = EventSubscriberID;
        /// @brief The type of binding that will be used (if at all) to help track subscription lifetime.
        using BindingType = EventSubscriberBinding<InterfaceType>;
        /// @brief The type of table storing that will be used to store all of the subscribers for a single event.
        using TableType = EventSubscriptionTable<InterfaceType,true>;
        /// @brief The type of dispatcher that will be used to forward events being fired to the subscription tables.
        using DispatcherType = DefaultEventDispatcher<TableType>;
    };//DefaultPublisherTraits

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is the base class for any class that generates and publishes events to subscribers.
    /// @tparam PublisherType The type of publisher this binding will be bound to.
    /// @tparam InterfaceType The type of interface/subscriber this binding will be bound to.
    ///////////////////////////////////////
    template<class PublisherType, class InterfaceType>
    class MEZZ_LIB EventSubscriberBindingImpl : public EventSubscriberBinding<InterfaceType>
    {
    public:
        /// @brief Convenience type for describing the type of "this".
        using SelfType = EventSubscriberBindingImpl<PublisherType,InterfaceType>;
    protected:
        /// @brief A pointer to the EventPublisher we are subscribed to.
        PublisherType* EventPub;
    public:
        /// @brief Class constructor.
        /// @param ID The unique identifier for the subscriber/delegate.
        /// @param Delegate The callback to dispatch the event to.
        /// @param Pub A pointer to the publisher dispatching the interested event.
        /// @param Hash The hash of the event name this binding is subscribed to.
        EventSubscriberBindingImpl(EventSubscriberID ID, const InterfaceType Delegate, PublisherType* Pub, const EventHashType Hash);
        /// @brief Copy constructor.
        /// @param Other The other binding to not be copied.
        EventSubscriberBindingImpl(const SelfType& Other) = delete;
        /// @brief Move constructor.
        /// @param Other The other binding to be moved.
        EventSubscriberBindingImpl(SelfType&& Other) = default;
        /// @brief Class destructor.
        virtual ~EventSubscriberBindingImpl() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Copy constructor.
        /// @param Other The other binding to not be copied.
        /// @return Returns a reference to this.
        SelfType& operator=(const SelfType& Other) = delete;
        /// @brief Move assignment operator.
        /// @param Other The other binding to be moved.
        /// @return Returns a reference to this.
        SelfType& operator=(SelfType&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Check if this binding is still valid.
        /// @return Returns true of the subscriber can still get events from the publisher, false otherwise.
        virtual Boole IsSubscribed() const;
        /// @brief Removes the subscriber from the list of interested recipients on the publisher.
        virtual void Unsubscribe();
        /// @brief Removes all references to an Event and/or Publisher from this binding.
        /// @remarks This method is called by Unsubscribe, and should never need to be called manually.
        /// This method also makes zero attempt to notify the publisher of it's changed state.  For that
        /// you should call Unsubscribe.
        virtual void Unbind();
    };//EventSubscriberBindingImpl

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is the base class for any class that generates and publishes events to subscribers.
    /// @tparam TableType The type to be used to store subscriptions that events will be dispatched to.
    /// @tparam DispatchType The type to be used to dispatch the event to the Subscription table.
    ///////////////////////////////////////
    template<class TableType, class DispatcherType>
    class MEZZ_LIB EventPublisher
    {
    public:
        /// @brief Convenience type for describing the type of "this".
        using SelfType = EventPublisher<TableType,DispatcherType>;
        /// @brief Retrievable type for querying the type of callable interface the table works with.
        using InterfaceType = typename TableType::InterfaceType;
        /// @brief Convenience type for passing the subscriber as an argument to the Subscribe method.
        using SubscribeArg = typename TableType::SubscribeArg;
        /// @brief Convenience type for the return value of the Subscribe method.
        using SubscribeRet = typename TableType::SubscribeRet;
        /// @brief Convenience type for the actual binding implementation used by this publisher.
        using BindingImplType = EventSubscriberBindingImpl<SelfType,InterfaceType>;
        /// @brief Basic container type for TableType storage by this class.
        using TableContainer = SortedVector<TableType>;
        /// @brief Iterator type for TableType instances stored by this class.
        using TableIterator = typename TableContainer::iterator;
        /// @brief Const Iterator type for TableType instances stored by this class.
        using ConstTableIterator = typename TableContainer::const_iterator;
    protected:
        /// @brief A container storing all the Events published by this class by name.
        TableContainer SubscriptionTables;
        /// @brief The dispatcher that will determine how/if the event should be routed to the subscription table.
        DispatcherType Dispatcher;
    public:
        /// @brief Class constructor.
        EventPublisher() = default;
        /// @brief Reserving constructor.
        /// @param EventCapacity The number of events to expect this publisher is expected to be populated with.
        EventPublisher(const Whole EventCapacity);
        /// @brief Copy constructor.
        /// @param Other The other publisher to be copied.
        EventPublisher(const SelfType& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other publisher to be moved.
        EventPublisher(SelfType&& Other) = default;
        /// @brief Class destructor.
        ~EventPublisher() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other publisher to be copied.
        /// @return Returns a reference to this.
        SelfType& operator=(const SelfType& Other) = default;
        /// @brief Move assignment operator.
        /// @param Other The other publisher to be moved.
        /// @return Returns a reference to this.
        SelfType& operator=(SelfType&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the event dispatcher.
        /// @return Returns a reference to the event dispatcher being used by this EventPublisher.
        DispatcherType& GetDispatcher();
        /// @brief Gets the event dispatcher.
        /// @return Returns a const reference to the event dispatcher being used by this EventPublisher.
        const DispatcherType& GetDispatcher() const;

        /// @brief Fires an event.
        /// @param Args The arguments/event specific data related to this event.
        void DispatchEvent(EventPtr Args) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Subscription Table Management - Via Name

        /// @brief Creates a new event table representing an event that can be subscribed to.
        /// @exception If a subscription table with the names hash already exists it will throw a "II_DUPLICATE_IDENTITY_EXCEPTION".
        /// @param EventName The name to be given to the new event subscription table.
        /// @return Returns an iterator to the created event table.
        TableIterator AddSubscriptionTable(const EventNameType& EventName);
        /// @brief Checks to see if an event table is registered with and has a subscriber table in this publisher.
        /// @param EventName The name of the table to check for.
        /// @return Returns true of the named event table is present in this publisher.
        Boole HasSubscriptionTable(const EventNameType& EventName) const;
        /// @brief Gets an event table in this publisher.
        /// @exception If this fails to find the table specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param EventName The name of the table to retrieve.
        /// @return Returns an iterator to the requested event table or throws an exception if it was not found.
        TableIterator GetSubscriptionTable(const EventNameType& EventName);
        /// @brief Gets an event table in this publisher.
        /// @exception If this fails to find the event specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param EventName The name of the event to retrieve.
        /// @return Returns a const iterator to the requested event table or throws an exception if it was not found.
        ConstTableIterator GetSubscriptionTable(const EventNameType& EventName) const;
        /// @brief Removes an existing event in this Publisher.
        /// @param EventName The name of the event to be removed.
        void RemoveSubscriptionTable(const EventNameType& EventName);

        ///////////////////////////////////////////////////////////////////////////////
        // Subscription Table Management - Via Hash

        /// @brief Creates a new event table representing an event that can be subscribed to.
        /// @exception If a subscription table with the names hash already exists it will throw a "II_DUPLICATE_IDENTITY_EXCEPTION".
        /// @param EventName The generated hash to use to identify the new event subscription table.
        /// @return Returns an iterator to the created event table.
        TableIterator AddSubscriptionTable(const EventHashType EventHash);
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
        TableIterator GetSubscriptionTable(const EventHashType EventHash);
        /// @brief Gets an event table in this publisher.
        /// @remarks The EventNameType overload of this method should be used instead where possible.
        /// @exception If this fails to find the event specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param EventHash The generated hash for the event name to retrieve.
        /// @return Returns a const iterator to the requested event table or throws an exception if it was not found.
        ConstTableIterator GetSubscriptionTable(const EventHashType EventHash) const;
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
        SubscribeRet Subscribe(const EventNameType& EventName, const EventSubscriberID ID, const SubscribeArg Delegate);
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
        SubscribeRet Subscribe(const EventHashType EventHash, const EventSubscriberID ID, const SubscribeArg Delegate);
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

    ///////////////////////////////////////////////////////////////////////////////
    // EventSubscriberBindingImpl Method Implementations

    ///////////////////////////////////////////////////////////////////////////////
    // Construction and Destruction

    template<class PublisherType, class InterfaceType>
    EventSubscriberBindingImpl<PublisherType,InterfaceType>::EventSubscriberBindingImpl(EventSubscriberID ID, const InterfaceType Delegate, PublisherType* Pub, const EventHashType Hash) :
        EventSubscriberBinding<InterfaceType>(ID,Delegate,Hash),
        EventPub(Pub)
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    template<class PublisherType, class InterfaceType>
    Boole EventSubscriberBindingImpl<PublisherType,InterfaceType>::IsSubscribed() const
        { return ( this->EventPub != nullptr ); }

    template<class PublisherType, class InterfaceType>
    void EventSubscriberBindingImpl<PublisherType,InterfaceType>::Unsubscribe()
    {
        this->EventPub->GetSubscriptionTable(this->NameHash)->Unsubscribe(this->SubID);
        this->Unbind();
    }

    template<class PublisherType, class InterfaceType>
    void EventSubscriberBindingImpl<PublisherType,InterfaceType>::Unbind()
    {
        this->EventPub = nullptr;
        this->NameHash = EventNameType::EmptyHash;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // EventPublisher Method Implementations

    ///////////////////////////////////////////////////////////////////////////////
    // Construction and Destruction

    template<class TableType, class DispatcherType>
    EventPublisher<TableType,DispatcherType>::EventPublisher(const Whole EventCapacity)
        { this->SubscriptionTables.reserve(EventCapacity); }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    template<class TableType, class DispatcherType>
    DispatcherType& EventPublisher<TableType,DispatcherType>::GetDispatcher()
        { return this->Dispatcher; }

    template<class TableType, class DispatcherType>
    const DispatcherType& EventPublisher<TableType,DispatcherType>::GetDispatcher() const
        { return this->Dispatcher; }

    template<class TableType, class DispatcherType>
    void EventPublisher<TableType,DispatcherType>::DispatchEvent(EventPtr Args) const
    {
        ConstTableIterator TableIt = this->GetSubscriptionTable(Args->EventName);
        this->Dispatcher.DispatchEvent((*TableIt),Args);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscription Table Management - Via Name

    template<class TableType, class DispatcherType>
    typename EventPublisher<TableType,DispatcherType>::TableIterator
    EventPublisher<TableType,DispatcherType>::AddSubscriptionTable(const EventNameType& EventName)
        { return this->AddSubscriptionTable(EventName.GetHash()); }

    template<class TableType, class DispatcherType>
    Boole EventPublisher<TableType,DispatcherType>::HasSubscriptionTable(const EventNameType& EventName) const
        { return this->HasSubscriptionTable(EventName.GetHash()); }

    template<class TableType, class DispatcherType>
    typename EventPublisher<TableType,DispatcherType>::TableIterator
    EventPublisher<TableType,DispatcherType>::GetSubscriptionTable(const EventNameType& EventName)
        { return this->GetSubscriptionTable(EventName.GetHash()); }

    template<class TableType, class DispatcherType>
    typename EventPublisher<TableType,DispatcherType>::ConstTableIterator
    EventPublisher<TableType,DispatcherType>::GetSubscriptionTable(const EventNameType& EventName) const
        { return this->GetSubscriptionTable(EventName.GetHash()); }

    template<class TableType, class DispatcherType>
    void EventPublisher<TableType,DispatcherType>::RemoveSubscriptionTable(const EventNameType& EventName)
        { this->RemoveSubscriptionTable(EventName.GetHash()); }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscription Table Management - Via Hash

    template<class TableType, class DispatcherType>
    typename EventPublisher<TableType,DispatcherType>::TableIterator
    EventPublisher<TableType,DispatcherType>::AddSubscriptionTable(const EventHashType EventHash)
    {
        TableIterator TableIt = this->SubscriptionTables.find(EventHash);
        if( TableIt == this->SubscriptionTables.end() ) {
            return this->SubscriptionTables.add_emplace([](const TableType& EvTable, EventHashType EventHash) {
                return EvTable.GetHash() < EventHash;
            }, EventHash);
        }else{
            StringStream ExceptionStream;
            ExceptionStream << "An EventSubscriptionTable with the hash \"" <<  EventHash << "\" already exists!" << std::endl;
            MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,ExceptionStream.str());
        }
    }

    template<class TableType, class DispatcherType>
    Boole EventPublisher<TableType,DispatcherType>::HasSubscriptionTable(const EventHashType EventHash) const
    {
        ConstTableIterator TableIt = this->SubscriptionTables.find(EventHash);
        return TableIt != this->SubscriptionTables.end();
    }

    template<class TableType, class DispatcherType>
    typename EventPublisher<TableType,DispatcherType>::TableIterator
    EventPublisher<TableType,DispatcherType>::GetSubscriptionTable(const EventHashType EventHash)
    {
        TableIterator TableIt = this->SubscriptionTables.find(EventHash);
        if( TableIt != this->SubscriptionTables.end() ) {
            return TableIt;
        }else{
            StringStream ExceptionStream;
            ExceptionStream << "Event hash \"" << EventHash << "\" not found in publisher." << std::endl;
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
        }
        return this->SubscriptionTables.end();
    }

    template<class TableType, class DispatcherType>
    typename EventPublisher<TableType,DispatcherType>::ConstTableIterator
    EventPublisher<TableType,DispatcherType>::GetSubscriptionTable(const EventHashType EventHash) const
    {
        ConstTableIterator TableIt = this->SubscriptionTables.find(EventHash);
        if( TableIt != this->SubscriptionTables.end() ) {
            return TableIt;
        }else{
            StringStream ExceptionStream;
            ExceptionStream << "Event hash \"" << EventHash << "\" not found in publisher." << std::endl;
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
        }
        return this->SubscriptionTables.end();
    }

    template<class TableType, class DispatcherType>
    void EventPublisher<TableType,DispatcherType>::RemoveSubscriptionTable(const EventHashType EventHash)
    {
        TableIterator TableIt = this->SubscriptionTables.find(EventHash);
        if( TableIt != this->SubscriptionTables.end() ) {
            this->SubscriptionTables.erase(TableIt);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscription Table Management

    template<class TableType, class DispatcherType>
    void EventPublisher<TableType,DispatcherType>::RemoveAllSubscriptionTables()
        { this->SubscriptionTables.clear(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscription Management - Via Name

    template<class TableType, class DispatcherType>
    typename EventPublisher<TableType,DispatcherType>::SubscribeRet
    EventPublisher<TableType,DispatcherType>::Subscribe(const EventNameType& EventName, const EventSubscriberID ID, const SubscribeArg Delegate)
        { return this->Subscribe(EventName.GetHash(),ID,Delegate); }

    template<class TableType, class DispatcherType>
    void EventPublisher<TableType,DispatcherType>::Unsubscribe(const EventNameType& EventName, const EventSubscriberID ID)
        { this->Unsubscribe(EventName.GetHash(),ID); }

    template<class TableType, class DispatcherType>
    Whole EventPublisher<TableType,DispatcherType>::UnsubscribeAll(const EventNameType& EventName)
        { return this->UnsubscribeAll(EventName.GetHash()); }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscription Management - Via Hash

    template<class TableType, class DispatcherType>
    typename EventPublisher<TableType,DispatcherType>::SubscribeRet
    EventPublisher<TableType,DispatcherType>::Subscribe(const EventHashType EventHash, const EventSubscriberID ID, const SubscribeArg Delegate)
        { return this->GetSubscriptionTable(EventHash)->Subscribe(ID,Delegate,this); }

    template<class TableType, class DispatcherType>
    void EventPublisher<TableType,DispatcherType>::Unsubscribe(const EventHashType EventHash, const EventSubscriberID ID)
        { this->GetSubscriptionTable(EventHash)->Unsubscribe(ID); }

    template<class TableType, class DispatcherType>
    Whole EventPublisher<TableType,DispatcherType>::UnsubscribeAll(const EventHashType EventHash)
        { return this->GetSubscriptionTable(EventHash)->UnsubscribeAll(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Subscription Management

    template<class TableType, class DispatcherType>
    void EventPublisher<TableType,DispatcherType>::Unsubscribe(const EventSubscriberID ID)
    {
        for( TableType& CurrTable : this->SubscriptionTables )
            { CurrTable.Unsubscribe(ID); }
    }

    template<class TableType, class DispatcherType>
    Whole EventPublisher<TableType,DispatcherType>::UnsubscribeAll()
    {
        Whole Ret = 0;
        for( TableType& CurrTable : this->SubscriptionTables )
            { Ret += CurrTable.UnsubscribeAll(); }
        return Ret;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Default Implementation Types

    /// @brief Convenience Type that will repeat the TableType so you don't have to!
    /// @tparam TableType The type to be used to store subscriptions that events will be dispatched to.
    /// @tparam DispatchType The type to be used to dispatch the event to the Subscription table.
    template< class TableType,template <typename...> class DispatcherType >
    using EventPublisherAlias = EventPublisher< TableType,DispatcherType<TableType> >;
    /// @brief Default/Convenience implementation of an EventSubscriptionTable.
    using DefaultEventSubscriptionTable = EventSubscriptionTable<std::function<void(EventPtr)>,true>;
    /// @brief Default/Convenience implementation of an EventPublisher.
    //using DefaultEventPublisher = EventPublisher<DefaultEventSubscriptionTable,DefaultEventDispatcher<DefaultEventSubscriptionTable>>;
    using DefaultEventPublisher = EventPublisherAlias<DefaultEventSubscriptionTable,DefaultEventDispatcher>;

    /// @}
}//Mezzanine

#endif
