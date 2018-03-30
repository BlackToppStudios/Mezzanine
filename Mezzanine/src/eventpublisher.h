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
#include "eventid.h"
#include "eventsubscriber.h"
#include "eventsubscriberid.h"
#include "eventsubscriptiontable.h"

namespace Mezzanine
{
    /// @addtogroup Events
    /// @{

    /// @page Mezzanine Event System
    /// @section EvtOverview Overview
    /// Despite the name of the primary classes involved, the Mezzanine Event system more closely resembles the
    /// Observer Pattern for messaging.  It was designed to be as flexible as possible in common game development
    /// tasks with a number of considerations for speed and flexibility.  The original version of the Mezzanine
    /// Event system was already an extremely flexible system, but made assumptions about the needs of subscription
    /// lifetime management that were forced to always be on.  As such event dispatches often required 5 or 6 pointer
    /// indirections and required overhead, even if you weren't using it.  In the transition to the new system both
    /// of these details were refactored, and much more.  Depending on your configuration an event dispatch can be
    /// done with as few as 2 pointer indirections, or like the old Event system, as many as 6.  Overhead of
    /// subscription lifetime management is entirely optional.  This allowed the Event system to move from being
    /// suitable for communication between subsystems (such as Physics, Graphics, and UI) to something that can be
    /// used in any context, even when the scope is entirely within a single subsystem.
    /// @n @n
    /// The primary criteria for the design of the Event System are: @n
    /// @subsection EvtScalable Scalable
    /// No forced monolithic design or master publisher.  Objects can arbitrarily use the event
    /// system classes where they want, be it a simulation object, an application object (such as a
    /// window), a manager for a subsystem, or (ironically) a monolithic singleton.  Although I don't
    /// recommend that last one.
    /// @subsection EvtModular Modular
    /// No EventPublisher needs to be aware of any other EventPublisher.  Each EventPublisher is it's own
    /// contained event ecosystem.
    /// @subsection EvtSubSpecific Subscribe to specific events
    /// In many event systems you subscribe to a publisher and receive all of it's events.  With the
    /// Mezzanine Event System you can subscribe to just one, or however many specific events you are
    /// interested in.  In some cases this means fewer checks in the callback logic, but it also means
    /// less time is wasted on uninterested subscribers during event dispatch.
    /// @subsection EvtFlexPub Flexible Publisher usage
    /// Publishers are not interfaces and have no virtual methods.  You are free to inherit from it or
    /// store it as a data member, whichever is more appropriate.  In fact, the event system is designed
    /// without inheritance or any virtual methods.  It is entirely possible to end up with virtual
    /// method calls when dealing with subscribers though.  It is not expected or enforced by the system,
    /// but is a sane way to extend the flexibility of how subscribers can interact with the system further.
    /// @subsection EvtBookOptional No forced binding bookkeeping
    /// To help with corner cases where both the publisher and subscriber have finite lifetimes you can
    /// optionally create bindings to represent the subscriptions which are returned upon subscription.  They
    /// are stored in shared_ptr's and interested classes may hold on to them if needed to check if a
    /// publisher exists, or can ignore them entirely.  Alternatively, these can be disabled completely for
    /// a given table or publisher.
    /// @subsection EvtFlexIDs Flexible Event IDs
    /// Enums are an intuitive way to identify specific events efficiently, however since publishers are
    /// their own event ecosystems, and nothing stops a subscriber from subscribing to multiple publishers
    /// it would be possible for event enums between publishers to have the same value, which can be
    /// difficult or impossible to sort out in the subscriber logic.  Event IDs can be whatever type you wish
    /// as they are a type set in a config class that can be overridden/replaced.  We provide a sane default
    /// EventID class that uses hashed strings to define our events which are potentially more
    /// descriptive as well as easier to distinguish and extend than enums.  It can also facilitate
    /// integration in data driven systems.
    /// @subsection EvtSubIDs Flexible Subscriber IDs
    /// Subscriber IDs can be whatever type you wish, as it is a configurable type in a config class.  The
    /// Mezzanine event system does provide a default EventSubscriberID class that is set by default in the
    /// base config class, but this can be overridden/replaced.  Further, the EventPublisher class will not
    /// generate IDs for you.  Ultimately IDs are stored as uintptr_t specifically to allow pointers to be
    /// used as unique IDs, but this isn't required and doesn't match the requirements in all cases.  So you
    /// are free to specify whatever identification scheme you wish, the only requirement that exists is
    /// that the ID be unique to the event subscribed to.
    /// @subsection EvtFlexSubs Flexible Subscribers
    /// Subscribers have only two requirements mandated by the system; have an ID class defined, and a method to
    /// get the ID of an instance.  The method to get the ID, nor anything else on the subscriber, needs to be
    /// polymorphic.  Subscribers can be stored as either values, or pointers.  Any method on the subscriber
    /// can be invoked during event dispatch.  This allows the same system to have an std::function subscriber
    /// (which is provided as FunctionSubscriber) or an interface as a subscriber where each method on the
    /// interface allows more granular event dispatch.
    /// @n @n
    /// @subsection EvtLimits Limits Overview
    /// The Mezzanine Event system is extremely flexible and was designed with several configurations in mind.  The
    /// one caveat to this configuration is that it is in templates and thus must be configured at compile time
    /// rather than runtime.  Additionally, responsibilities in the system are encapsulated in such a way that
    /// functionality can be added to the system to address more specialized use cases as we discover them.
    /// @n @n
    /// @section EvtTableConfig Table Configuration
    /// Configuring the event system begins with the @ref EventSubscriptionTableConfig class.  It contains all the
    /// basic information needed to create an EventSubscriptionTable instance.  If settings need to be changed it's
    /// easiest (and expected) to create a new config class that inherits from EventSubscriptionTableConfig and
    /// redefines the members you want changed with different values.  A very small config class is provided that
    /// does this with a different factory type, the @ref EventBindingTableConfig.  The EventBindingTableConfig
    /// inherits from EventSubscriptionTableConfig and redefines FactoryType with the value of
    /// "SubscriptionFactoryType::SFT_Binding", leaving all other parts of the configuration the same as it was in
    /// EventSubscriptionTableConfig.
    /// @n @n
    /// @section EvtConfigSubs Subscribers
    /// The template parameter "Interface" on EventSubscriptionTableConfig is the subscriber to be set and will be
    /// invoked on event dispatch.  This subscriber can be any value or pointer type.  It can even be an abstract
    /// base/interface class.  If you choose to have a pointer subscriber type, you need to include the "*" in the
    /// template parameter as such:
    /// @n
    /// @code
    /// using TableConfig = EventSubscriptionTableConfig<SubscriberPtr*>;
    /// @endcode
    /// @n
    /// Not including the asterisk will set the system up to store your subscriber type by value instead.  @n
    /// There are two requirements for the subscriber type: @n
    /// 1) A type with the name "IDType" must be defined and it must be equality comparable (==).  @n
    /// 2) A method that returns "IDType" named "GetID()" and is const must be present (it can be pure virtual). @n
    /// No other requirements for the subscriber exist.  It is expected there would be more methods on the subscriber
    /// otherwise event dispatches to that subscriber type would be useless.  There are no expectations or requirements
    /// for those methods.
    /// @n @n
    /// @section EvtConfigFactories FactoryType Config
    /// The EventSubscriptionTableConfig::FactoryType member is an enum value describing the template specialization
    /// that will be used to create subscriptions for the EventSubscriptionTable.  The @ref SubscriptionFactoryType
    /// enum lists the possible values that can be set.
    /// @n
    /// SFT_Subscriber is the default setting and uses the SubscriberType for the subscription.  No extra or special
    /// bookkeeping is attempted and it is ideal for minimal setups or situations where you don't need to track
    /// subscription lifetime.
    /// @n
    /// SFT_Binding will use a factory that will store and return @ref EventSubscriberBinding and/or
    /// @ref EventSubscriberBindingImpl instances, which are designed to help track the lifetime of the subscription
    /// table and subscriber and will invalidate the subscription if either are destroyed.  This process is not entirely
    /// automatic, however.  On the subscriber side it is expected to call @ref EventSubscriberBinding::Unsubscribe
    /// when the subscriber is destroyed or otherwise done listening to events.  If object lifetime is a consideration
    /// for the system using event dispatch, then using SubscriptionFactoryType::SFT_Binding is likely appropriate.  A
    /// convenience struct is provided that sets this (and only this) named @ref EventBindingTableConfig.
    /// @n @n
    /// @section EvtConfigContainers ContainerType Config
    /// The EventSubscriptionTableConfig::ContainerType member is an enum value describing the template specialization
    /// that will be used to store the subscriptions created by the FactoryType.  The @ref SubscriptionContainerType
    /// enum lists the possible values that can be set.
    /// @n
    /// SCT_Single is intended to store one, and only one, subscriber IF it is a pointer type.  This restriction was
    /// imposed to prevent the need to supply a null or invalid value for the SubscriberType, which may be non-sensical
    /// or inappropriate for many subscriber types.  A separate data member was considered as an alternative to
    /// restricting this container to pointer types, but then it would be too similar to other container specializations
    /// such as the Fixed-Size containers, which are detailed below.
    /// @n
    /// SCT_Unsorted is a sane default container for most configurations, as it doesn't impose a maximum number of
    /// subscribers and makes no attempt to sort them (although they can still be manually sorted using the iterator
    /// access methods available).  It will do it's best to keep it's memory footprint as low as it can given the
    /// subscriber count.
    /// @n
    /// SCT_Unsorted_Fixed will allocate all the space to store subscribers once on construction.  If you know the exact
    /// or close to exact amount of subscribers your system will be using, then this container may improve performance
    /// by reducing the number of allocations the system will make as well as removing one pointer indirection during
    /// event dispatch.  The number of subscribers to allocate for is based on a value specified on the config struct,
    /// which is detailed below.
    /// @n
    /// SCT_Sorted has no maximum subscriber count that it can store, like SCT_Unsorted, but it will sort the
    /// subscriptions based on a provided predicate.  The predicate should be provided as a different member on the
    /// config struct, which is detailed below.  The sorting is intended to control the order in which subscribers
    /// will receive event dispatches.  Direct iterator access is provided, but the ordering should not be tampered with
    /// or assumptions about the ordering made by container may cause it to behave improperly.
    /// @n
    /// SCT_Sorted_Fixed will allocate all the space to store subscribers once on construction, and will keep them sorted
    /// throughout the lifetime of the container.  It has all the same considerations as SCT_Unsorted_Fixed and
    /// SCT_Sorted.
    /// @n @n
    /// @section EvtConfigContainerCount Container Storage Count
    /// The EventSubscriptionTableConfig::StorageCount member is used by Fixed-Size containers to allocate fixed storage
    /// on initialization.  This number defaults to 4, but it is important to keep in mind the intended usage when
    /// determining what is an appropriate storage count as it will allocate space for that many subscribers per table
    /// of that type/configuration.
    /// @n @n
    /// @section EvtConfigContainerPred Container Predicate
    /// The EventSubscriptionTableConfig::StoragePredicate member is used by Sorted containers to keep their stored
    /// subscriptions sorted.  The predicate must be a functor type that can be default constructed, which unfortunately
    /// eliminates lambdas as a possibility.  It's operator() member must take two instances of SubscriberType and
    /// return a bool; true if first should be sorted before second or false otherwise.
    /// @n @n
    /// @section EvtConfigDispatchers DispatcherType Config
    /// The EventSubscriptionTableConfig::DispatcherType member is an enum value describing the template specialization
    /// that will be used to dispatch the events to the subscriptions stored in the subscription container.  The @ref
    /// EventDispatcherType enum lists the possible values that can be set.
    /// @n
    /// EDT_Empty is very basic class with no members or special logic.  It retrieves a range of subscriptions and
    /// dispatches the event to each from beginning to end.  This is a sane default as far as dispatchers go as it has
    /// no memory footprint and has no special features.
    /// @n
    /// EDT_Silencable is almost identical to EDT_Empty, but with the additional feature of having a single bool member
    /// that is checked on each dispatch to allow the dispatch to continue.  By calling SetMuteEvents(const bool) passing
    /// in true on that template specialization it will silence any and all event dispatches until that same method is
    /// called again passing in false.
    /// @n
    /// EDT_Queued is a fairly complex dispatcher and may be required in some multi-threaded configurations.  When an
    /// event is dispatched from the EDT_Queued template specialization it doesn't go directly to the subscriber.  Instead
    /// the information related to that dispatch is stored until a flush method is called.  Only then is the subscriber
    /// notified of the event.  Because of the need to store the dispatch, this is a fairly heavyweight dispatcher,
    /// however this may be required to avoid race conditions that may be otherwise unavoidable.
    /// @n @n
    /// @section EvtConfigDispatchID Dispatch ID
    /// The EventSubscriptionTableConfig::DispatchIDType member is the type used to uniquely identify EventSubscriptionTable
    /// instances.  This type must be less-than comparable (<).  No other requirements exist for this type.
    /// @n @n
    /// @section EvtTableTraits EventSubscriptionTableTraits
    /// EventSubscriptionTableConfig is used by EventSubscriptionTableTraits to generate types and communicate those
    /// types to other classes in the event ecosystem.  You as an end user should never need to interact directly with
    /// EventSubscriptionTableTraits and should only need to be acutely aware of it's existence.  Config classes exist
    /// solely to separate the types that are safe to edit from the types that aren't.
    /// @n @n
    /// @section EvtSubLimits Subscriber Details and Limitations
    /// Subscriber types are given an extreme amount of flexibility in how they are built and what they do.  The
    /// @ref FunctionSubscriber class is provided as a convenience subscriber type that wraps an std::function and provides
    /// the necessary IDType requirements that exist for a subscriber.  This is also provided to help compatibility with
    /// the old Mezzanine Event system.  The FunctionSubscriber can be set up to use any function signature that
    /// std::function can support.
    /// @n
    /// Another versatile setup is to use interface Subscriber types.  That is, to have a abstract base class pointer be
    /// set as the SubscriberType.  The event system expects you to specify the method to be invoked during dispatch
    /// explicitly, so the method can be anything matching the SubscriberType.  This allows you to group up similar events
    /// on either the publisher level by creating multiple subscription tables with IDs corresponding to the event, or
    /// on the subscriber level by creating an interface class with multiple methods that is the grouping of similar events,
    /// or both.  Neither of these options forces the exclusion of the other.  Pointer subscriber types do not need to be
    /// to abstract classes and nothing expects them to be, so you are free to use a pointer to anything if you wish.  Any
    /// method called through the event system cannot be overloaded.
    /// @n @n
    /// @section EvtPub Event Publisher
    /// The EventPublisher class is essentially a glorified EventSubscriptionTable container.  It doesn't perform any
    /// mandatory operation for event dispatch.  If you need multiple tables in one place, it's handy.  However if you have
    /// a setup that doesn't require one, or even requires a different special way of storing your EventSubscriptionTable
    /// instances, you can do that without worry to the rest of the system.  Currently there is only one EventPublisher,
    /// which supports the storage of only one EventSubscriptionTable type, but future plans for the event system involve
    /// expanding the number and versatility of these table containers.
    /// @n @n
    /// @section EvtMultiThread Multi-threading considerations.
    /// The Event system was made with minimal code additions designed to facilitate multi-threaded situations.  Ideally
    /// threading dependencies can be set up to avoid these issues, or event dispatches can be confined to one subsystem
    /// where multi-threading isn't a consideration.  For other situations you will likely need some manner of queueing
    /// for your events.  Queueing can be done either on the EventSubscriptionTable or on the EventSubscriber.  As mentioned
    /// above, a EventDispatcher template specialization is provided to help with EventSubscriptionTable queueing.
    /// EventSubscriber queueing is not provided out-of-box at the time of this writing but is a plan for future expansion
    /// of the Event system.  Creating your own QueuedSubscriber should be as straightforward as saving the parameters of
    /// the event dispatch until it is ready to process it.  The specifics will depend on the chosen design of your
    /// subscriber.
    /// @n @n
    /// @section EvtUsage Using the Event system
    /// Usage of the Mezzanine Event system requires some preparation.  First you need a subscriber type.  Next you need a
    /// config that determines the type of EventSubscriptionTable.  You'll also need some constants that will identify the
    /// EventSubscriptionTable instances you'll be creating.  Keep in mind that unique IDs for EventSubscriptionTable
    /// instances is only enforced within a given EventPublisher.  If you are subscribing to tables within multiple
    /// EventPublishers it's possible to have different tables with the same IDs.  If you pass the table ID as a part of
    /// the event information (as some systems in the Mezzanine do) then you could end up with duplicate IDs if you are
    /// not careful.  This is part of why HashedStrings are used in conjunction with the Event system throughout the Mezzanine,
    /// although this is not required.
    /// @n
    /// If you are using an EventPublisher start by constructing it and calling "AddSubscriptionTable" for each table you
    /// want created.  For simple setups it's fairly intuitive to create one table per event.  If you aren't using an
    /// EventPublisher, then you can just make however many tables you want/need.  Once the tables are made, calling "Subscribe"
    /// passing in an instance of a subscriber to the relevant tables is the last step you need before dispatching.
    /// @n
    /// @code
    /// // Subscriber type
    /// class MySubscriberBase
    /// {
    /// public:
    ///     using IDType = EventSubscriberID;
    ///
    ///     virtual void OnEventStr(const String& Str) = 0;
    ///     virtual int OnEventInt(const int Num) = 0;
    ///     virtual IDType GetID() const = 0;
    /// };
    /// // Subscriber Implementation
    /// class MySubscriber : public MySubscriberBase
    /// {
    /// protected:
    ///     IDType MyID;
    /// public:
    ///     MySubscriber(IDType ID) :
    ///         MyID(ID)
    ///         {  }
    ///     virtual void OnEventStr(const String& Str)
    ///         { std::cout << "Event String: " << Str << "\n"; }
    ///     virtual int OnEventInt(const int Num)
    ///         { std::cout << "Event Int: " << Num << "\n";  return Num; }
    ///     virtual IDType GetID() const
    ///         { return MyID; }
    /// };
    /// // EventSubscriptionTable IDs
    /// const HashedString32 EventOne = "EventOne";
    /// const HashedString32 EventTwo = "EventTwo";
    /// // Config type
    /// using ConfigType = EventSubscriptionTableConfig<MySubscriberBase*>;
    /// // Actual Setup
    /// MySubscriber SubOne(12345);
    /// MySubscriber SubTwo(54321);
    /// EventPublisher<ConfigType> MyPublisher;
    /// MyPublisher.AddSubscriptionTable(EventOne);
    /// MyPublisher.AddSubscriptionTable(EventTwo);
    /// MyPublisher.Subscribe(EventOne,&SubOne);
    /// MyPublisher.Subscribe(EventTwo,&SubTwo);
    /// @endcode
    /// @n
    /// If you aren't using an EventPublisher the steps are mostly the same:
    /// @n
    /// @code
    /// // Subscriber type
    /// class MySubscriberBase
    /// {
    /// public:
    ///     using IDType = EventSubscriberID;
    ///
    ///     virtual void OnEventStr(const String& Str) = 0;
    ///     virtual int OnEventInt(const int Num) = 0;
    ///     virtual IDType GetID() const = 0;
    /// };
    /// // Subscriber Implementation
    /// class MySubscriber : public MySubscriberBase
    /// {
    /// protected:
    ///     IDType MyID;
    /// public:
    ///     MySubscriber(IDType ID) :
    ///         MyID(ID)
    ///         {  }
    ///     virtual void OnEventStr(const String& Str)
    ///         { std::cout << "Event String: " << Str << "\n"; }
    ///     virtual int OnEventInt(const int Num)
    ///         { std::cout << "Event Int: " << Num << "\n";  return Num; }
    ///     virtual IDType GetID() const
    ///         { return MyID; }
    /// };
    /// // EventSubscriptionTable IDs
    /// const HashedString32 EventOne = "EventOne";
    /// const HashedString32 EventTwo = "EventTwo";
    /// // Config type
    /// using ConfigType = EventSubscriptionTableConfig<MySubscriberBase*>;
    /// // Actual Setup
    /// MySubscriber SubOne(12345);
    /// MySubscriber SubTwo(54321);
    /// EventSubscriptionTable<ConfigType> TableOne(EventOne);
    /// EventSubscriptionTable<ConfigType> TableTwo(EventTwo);
    /// TableOne.Subscribe(&SubOne);
    /// TableTwo.Subscribe(&SubTwo);
    /// @endcode
    /// @n
    /// Once set up event dispatch is done by either calling "DispatchEvent" or "DispatchQuery" passing in a member
    /// function pointer to the member function to be invoked followed by any and all arguments needed for the
    /// function.  The function invoked cannot be overloaded.  If the function doesn't take any parameters, only the
    /// member function pointer is needed.  Calling "DispatchEvent" is for when you do not care for and return
    /// parameters that may be returned.  "DispatchQuery" is for when you DO want the return value.  Calling
    /// "DispatchEvent" when invoking a function with a return value is safe.  Calling "DispatchQuery" when invoking
    /// a function that returns void is not and will generate a compiler error.
    /// @n
    /// If you wish to dispatch to only one subscriber in a table, there are also a second pair of "DispatchEvent" and
    /// "DispatchQuery" methods called "DispatchEventSingle" and "DispatchQuerySingle" that accept a SubscriberIDType
    /// to uniquely identify the subscriber you wish to dispatch two.  No other subscriber will be notified during that
    /// dispatch, but is otherwise identical to the non-Single variants.
    /// @n
    /// More code examples.  With EventPublisher:
    /// @n
    /// @code
    /// String EventName = "ImAnEventWoo!";
    /// int EventNum = 42;
    /// // Invokes "OnEventStr" for each subscriber in the "EventOne" table.
    /// MyPublisher.DispatchEvent(EventOne,&MySubscriberBase::OnEventStr,EventName);
    /// // Invokes "OnEventStr" on the subscriber identified as "12345" in the "EventOne" table.
    /// MyPublisher.DispatchEventSingle(EventOne,12345,&MySubscriberBase::OnEventStr,EventName);
    /// // Invokes "OnEventInt" for each subscriber in the "EventTwo" and returns a vector of those results.
    /// std::vector<int> PotentiallyManyResults = MyPublisher.DispatchQuery(EventTwo,&MySubscriberBase::OnEventInt,EventNum);
    /// // Invokes "OnEventInt" on the subscriber identified as "54321" in the "EventTwo" table and returns a vector with that result.
    /// std::vector<int> PotentiallyOneResult = MyPublisher.DispatchQuerySingle(EventTwo,54321,&MySubscriberBase::OnEventInt,EventNum);
    /// @endcode
    /// @n
    /// Without EventPublisher:
    /// @n
    /// @code
    /// String EventName = "ImAnEventWoo!";
    /// int EventNum = 42;
    /// // Invokes "OnEventStr" for each subscriber in the table.
    /// TableOne.DispatchEvent(&MySubscriberBase::OnEventStr,EventName);
    /// // Invokes "OnEventStr" on the subscriber identified as "12345" in the table.
    /// TableOne.DispatchEventSingle(12345,&MySubscriberBase::OnEventStr,EventName);
    /// // Invokes "OnEventInt" for each subscriber in the table and returns a vector of those results.
    /// std::vector<int> PotentiallyManyResults = TableTwo.DispatchQuery(&MySubscriberBase::OnEventInt,EventNum);
    /// // Invokes "OnEventInt" on the subscriber identified as "54321" in the table and returns a vector with that result.
    /// std::vector<int> PotentiallyOneResult = TableTwo.DispatchQuerySingle(54321,&MySubscriberBase::OnEventInt,EventNum);
    /// @endcode
    /// @n
    /// The return results of a query are different if you are using the EDT_Queued dispatcher template specialization,
    /// which will return a std::shared_ptr pointing to an instance of @ref QueuedQueryResults, which can be used to check
    /// if the container is ready to be read.
    /// @n
    /// In addition to all the dont's that have already been covered, you should also avoid removing a subscriber during
    /// the dispatch operation.  It may end up working, it may not.  That feature is not officially supported so if you
    /// try you will be on your own.

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is the base class for any class that generates and publishes events to subscribers.
    /// @tparam Config A struct of types and values to use for the configuration of the tables used by this publisher.
    /// @pre Config is not used by the EventPublisher directly, but instead is used to configure the tables stored by
    /// the publisher.  As such, it should be a valid configuration usable by EventSubscriptionTable.  Check the
    /// EventSubscriptionTable documentation for more information.
    ///////////////////////////////////////
    template<class Config>
    class EventPublisher
    {
    public:
        /// @brief Convenience type for describing the type of "this".
        using SelfType = EventPublisher<Config>;
        /// @brief Retrievable type for the table used by this publisher.
        using TableType = EventSubscriptionTable<Config>;
        /// @brief Retrievable type for querying the type of callable interface the table works with.
        using SubscriberType = typename TableType::SubscriberType;
        /// @brief The type to use for uniquely identifying instances of subscribers.
        using SubscriberIDType = typename TableType::SubscriberIDType;
        /// @brief Convenience type for passing the subscriber as an argument to the Subscribe method.
        using SubscribeArg = typename TableType::SubscribeArg;
        /// @brief Convenience type for getting a subscription stored in a table.
        using SubscriptionGet = typename TableType::SubscriptionGet;
        /// @brief Basic container type for TableType storage by this class.
        using TableContainer = SortedVector<TableType>;
        /// @brief Iterator type for TableType instances stored by this class.
        using TableIterator = typename TableContainer::iterator;
        /// @brief Const Iterator type for TableType instances stored by this class.
        using ConstTableIterator = typename TableContainer::const_iterator;
        /// @brief The type to use for uniquely identifying events that can be dispatched.
        using DispatchIDType = typename TableType::DispatchIDType;
    protected:
        /// @brief A container storing all the Events published by this class by name.
        TableContainer SubscriptionTables;
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Construction, Destruction, and Assignment

        /// @brief Class constructor.
        EventPublisher() = default;
        /// @brief Reserving constructor.
        /// @param EventCapacity The number of events to expect this publisher is expected to be populated with.
        EventPublisher(const Whole EventCapacity)
            { this->SubscriptionTables.reserve(EventCapacity); }
        /// @brief Copy constructor.
        /// @param Other The other publisher to NOT be copied.
        EventPublisher(const SelfType& Other) = delete;
        /// @brief Move constructor.
        /// @param Other The other publisher to be moved.
        EventPublisher(SelfType&& Other) = default;
        /// @brief Class destructor.
        ~EventPublisher() = default;

        /// @brief Assignment operator.
        /// @param Other The other publisher to NOT be copied.
        /// @return Returns a reference to this.
        SelfType& operator=(const SelfType& Other) = delete;
        /// @brief Move assignment operator.
        /// @param Other The other publisher to be moved.
        /// @return Returns a reference to this.
        SelfType& operator=(SelfType&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets an iterator to the first table in the publisher.
        /// @return Returns an iterator to the start of the table range.
        TableIterator begin()
            { return this->SubscriptionTables.begin(); }
        /// @brief Gets a const iterator to the first table in the publisher.
        /// @return Returns a const iterator to the start of the table range.
        ConstTableIterator begin() const
            { return this->SubscriptionTables.begin(); }
        /// @brief Gets an iterator to one-passed-the-last table in the publisher.
        /// @return Returns an iterator to the end of the table range.
        TableIterator end()
            { return this->SubscriptionTables.end(); }
        /// @brief Gets a const iterator to one-passed-the-last table in the publisher.
        /// @return Returns a const iterator to the end of the table range.
        ConstTableIterator end() const
            { return this->SubscriptionTables.end(); }

        /// @brief Dispatches an event to each subscriber in a specific table.
        /// @tparam MemberFunct The deduced type of member function pointer that will be invoked.
        /// @tparam ArgTypes A variadic template of arguments to be passed to the member function.
        /// @pre MemberFunct must be a member of the SubscriberType specified in the EventSubscriptionTableConfig
        /// class used by the EventSubscriptionTable instantiation.
        /// @pre ArgTypes is expected to match the needed parameters of the provided member function.
        /// @param DisID The ID of which group of subscribers to dispatch to.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        template<class MemberFunct, class... ArgTypes>
        void DispatchEvent(const DispatchIDType DisID, MemberFunct Funct, ArgTypes&&... Args)
        {
            TableIterator TableIt = this->GetSubscriptionTable(DisID);
            (*TableIt).DispatchEvent(Funct,std::forward<ArgTypes>(Args)...);
        }

        /// @brief Dispatches an event to one subscriber in a specific table.
        /// @tparam MemberFunct The deduced type of member function pointer that will be invoked.
        /// @tparam ArgTypes A variadic template of arguments to be passed to the member function.
        /// @pre MemberFunct must be a member of the SubscriberType specified in the EventSubscriptionTableConfig
        /// class used by the EventSubscriptionTable instantiation.
        /// @pre ArgTypes is expected to match the needed parameters of the provided member function.
        /// @param DisID The ID of which group of subscribers to dispatch to.
        /// @param SubID The ID of the specific subscriber to dispatch to.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        template<class MemberFunct, class... ArgTypes>
        void DispatchEventSingle(const DispatchIDType DisID, const SubscriberIDType SubID, MemberFunct Funct, ArgTypes&&... Args)
        {
            TableIterator TableIt = this->GetSubscriptionTable(DisID);
            (*TableIt).DispatchEventSingle(SubID,Funct,std::forward<ArgTypes>(Args)...);
        }

        /// @brief Dispatches a query to each subscriber in a specific table.
        /// @tparam MemberFunct The deduced type of member function pointer that will be invoked.
        /// @tparam ArgTypes A variadic template of arguments to be passed to the member function.
        /// @pre MemberFunct must be a member of the SubscriberType specified in the EventSubscriptionTableConfig
        /// class used by the EventSubscriptionTable instantiation.
        /// @pre ArgTypes is expected to match the needed parameters of the provided member function.
        /// @param DisID The ID of which group of subscribers to dispatch to.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        /// @return Returns a container (vector) of return values from the subscribers dispatched to.
        template<class MemberFunct, class... ArgTypes>
        auto DispatchQuery(const DispatchIDType DisID, MemberFunct Funct, ArgTypes&&... Args)
        {
            TableIterator TableIt = this->GetSubscriptionTable(DisID);
            return (*TableIt).DispatchQuery(Funct,std::forward<ArgTypes>(Args)...);
        }

        /// @brief Dispatches a query to one subscriber in a specific table.
        /// @tparam MemberFunct The deduced type of member function pointer that will be invoked.
        /// @tparam ArgTypes A variadic template of arguments to be passed to the member function.
        /// @pre MemberFunct must be a member of the SubscriberType specified in the EventSubscriptionTableConfig
        /// class used by the EventSubscriptionTable instantiation.
        /// @pre ArgTypes is expected to match the needed parameters of the provided member function.
        /// @param DisID The ID of which group of subscribers to dispatch to.
        /// @param SubID The ID of the specific subscriber to dispatch to.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        /// @return Returns a container (vector) of return values from the subscriber dispatched to.
        template<class MemberFunct, class... ArgTypes>
        auto DispatchQuerySingle(const DispatchIDType DisID, const SubscriberIDType SubID, MemberFunct Funct, ArgTypes&&... Args)
        {
            TableIterator TableIt = this->GetSubscriptionTable(DisID);
            return (*TableIt).DispatchQuerySingle(SubID,Funct,std::forward<ArgTypes>(Args)...);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Subscription Table Management

        /// @brief Creates a new table representing a group of subscribers an event can be dispatched to.
        /// @exception If a subscription table with the names hash already exists it will throw a "II_DUPLICATE_IDENTITY_EXCEPTION".
        /// @param ID The unique ID to use to identify the new table.
        /// @return Returns an iterator to the created table.
        TableIterator AddSubscriptionTable(const DispatchIDType ID)
        {
            TableIterator TableIt = this->SubscriptionTables.find_if([ID](const TableType& SubTable) -> Boole {
                return SubTable.GetID() == ID;
            });
            if( TableIt == this->SubscriptionTables.end() ) {
                return this->SubscriptionTables.add_emplace([](const TableType& EvTable, DispatchIDType ID) {
                    return EvTable.GetID() < ID;
                }, ID);
            }else{
                StringStream ExceptionStream;
                ExceptionStream << "An Event Table with the ID \"" << ID << "\" already exists!" << std::endl;
                MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,ExceptionStream.str());
            }
        }
        /// @brief Checks to see if a table is registered with and has a subscriber table in this publisher.
        /// @remarks The EventNameType overload of this method should be used instead where possible.
        /// @param ID The unique ID for the table name to check for.
        /// @return Returns true of the specified table is present in this publisher.
        Boole HasSubscriptionTable(const DispatchIDType ID) const
        {
            ConstTableIterator TableIt = this->SubscriptionTables.find_if([ID](const TableType& SubTable) -> Boole {
                return SubTable.GetID() == ID;
            });
            return TableIt != this->SubscriptionTables.end();
        }
        /// @brief Gets a table in this publisher.
        /// @remarks The EventNameType overload of this method should be used instead where possible.
        /// @exception If this fails to find the table specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param ID The unique ID for the table to retrieve.
        /// @return Returns an iterator to the requested table or throws an exception if it was not found.
        TableIterator GetSubscriptionTable(const DispatchIDType ID)
        {
            TableIterator TableIt = this->SubscriptionTables.find_if([ID](const TableType& SubTable) -> Boole {
                return SubTable.GetID() == ID;
            });
            if( TableIt != this->SubscriptionTables.end() ) {
                return TableIt;
            }else{
                StringStream ExceptionStream;
                ExceptionStream << "Dispatch ID \"" << ID << "\" not found in publisher." << std::endl;
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
            }
            return this->SubscriptionTables.end();
        }
        /// @brief Gets a table in this publisher.
        /// @exception If this fails to find the table specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param ID The unique ID for the table to retrieve.
        /// @return Returns a const iterator to the requested table or throws an exception if it was not found.
        ConstTableIterator GetSubscriptionTable(const DispatchIDType ID) const
        {
            ConstTableIterator TableIt = this->SubscriptionTables.find_if([ID](const TableType& SubTable) -> Boole {
                return SubTable.GetID() == ID;
            });
            if( TableIt != this->SubscriptionTables.end() ) {
                return TableIt;
            }else{
                StringStream ExceptionStream;
                ExceptionStream << "Dispatch ID \"" << ID << "\" not found in publisher." << std::endl;
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
            }
            return this->SubscriptionTables.end();
        }
        /// @brief Gets the number of subscription tables in this publisher.
        /// @return Returns the amount of subscription tables being managed by this publisher.
        Whole GetNumSubscriptionTables() const
        {
            return this->SubscriptionTables.size();
        }
        /// @brief Removes an existing table in this Publisher.
        /// @param ID The unique ID for the table to remove.
        void RemoveSubscriptionTable(const DispatchIDType ID)
        {
            TableIterator TableIt = this->SubscriptionTables.find_if([ID](const TableType& SubTable) -> Boole {
                return SubTable.GetID() == ID;
            });
            if( TableIt != this->SubscriptionTables.end() ) {
                this->SubscriptionTables.erase(TableIt);
            }
        }
        /// @brief Removes all events in this Publisher.
        /// @return Returns the number of tables removed.
        Whole RemoveAllSubscriptionTables()
        {
            Whole Ret = this->SubscriptionTables.size();
            this->SubscriptionTables.clear();
            return Ret;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Table Specific Subscription Management

        /// @brief Adds a subscriber to a table.
        /// @remarks The ID of the subscriber must be unique among the IDs in the table it is subscribed to.
        /// @exception If this fails to find the table specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @exception If the ID of the Sub already exists on the specified table, this will throw a "II_DUPLICATE_IDENTITY_EXCEPTION".
        /// @param ID The unique ID of the table to subscribe to.
        /// @param Callable The callback to be called when the interested event is fired.
        /// @return Returns a pointer to the created Subscriber slot for the provided subscriber.
        SubscriptionGet Subscribe(const DispatchIDType ID, SubscribeArg Callable)
            { return this->GetSubscriptionTable(ID)->Subscribe(Callable); }
        /// @brief Removes a single subscriber from the specified table.
        /// @exception If this fails to find the table specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param ID The unique ID of the table to unsubscribe from.
        /// @param SubID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        void Unsubscribe(const DispatchIDType ID, const SubscriberIDType SubID)
            { this->GetSubscriptionTable(ID)->Unsubscribe(SubID); }
        /// @brief Removes all subscribers from the named Event.
        /// @exception If this fails to find the table specified it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param ID The unique ID of the table to unsubscribe from.
        /// @return Returns the number of subscribers removed.
        Whole UnsubscribeAll(const DispatchIDType ID)
            { return this->GetSubscriptionTable(ID)->UnsubscribeAll(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Table Agnostic Subscription Management

        /// @brief Removes a single subscriber from all tables in this publisher.
        /// @param ID The unique ID of the subscriber.
        void Unsubscribe(const SubscriberIDType ID)
        {
            for( TableType& CurrTable : this->SubscriptionTables )
                { CurrTable.Unsubscribe(ID); }
        }
        /// @brief Removes all subscribers from all tables in this publisher.
        /// @return Returns the number of subscribers removed.
        Whole UnsubscribeAll()
        {
            Whole Ret = 0;
            for( TableType& CurrTable : this->SubscriptionTables )
                { Ret += CurrTable.UnsubscribeAll(); }
            return Ret;
        }
    };//EventPublisher

    template<>
    class EventPublisher<void>
    {
    public:
        /// @brief Convenience type for describing the type of "this".
        using SelfType = EventPublisher<void>;
    protected:
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Construction, Destruction, and Assignment

        /// @brief Class constructor.
        EventPublisher() = default;
        /// @brief Copy constructor.
        /// @param Other The other publisher to be copied.
        EventPublisher(const SelfType& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other publisher to be moved.
        EventPublisher(SelfType&& Other) = default;
        /// @brief Class destructor.
        ~EventPublisher() = default;

        /// @brief Assignment operator.
        /// @param Other The other publisher to be copied.
        /// @return Returns a reference to this.
        SelfType& operator=(const SelfType& Other) = default;
        /// @brief Move assignment operator.
        /// @param Other The other publisher to be moved.
        /// @return Returns a reference to this.
        SelfType& operator=(SelfType&& Other) = default;
    };//EventPublisher

    /// @}
}//Mezzanine

#endif
