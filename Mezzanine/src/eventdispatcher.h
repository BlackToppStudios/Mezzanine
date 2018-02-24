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
#ifndef _eventdispatcher_h
#define _eventdispatcher_h

#include "event.h"
#include "eventhelper.h"
#include "eventsubscriptioncontainer.h"
#include "iteratorrange.h"

namespace Mezzanine
{
    /// @addtogroup Events
    /// @{

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is an empty dispatcher that is the basis from which dispatchers are specialized.
    ///////////////////////////////////////
    template<class TableType, class Traits, EventDispatcherType DispatcherType>
    class EventDispatcher
        {  };

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a basic event dispatcher that does simple mindless dispatch.
    /// @tparam TableType The type of subscription table using this dispatcher.
    /// @tparam Traits A collection of types that describes the operation of the subscription table.
    /// @pre TableType is expected to be a derived type of this dispatcher.  It is also expected to have an
    /// iterator type "StorageIterator" that can be dereferenced and converted to "SubscriberType", which
    /// also must be present and defined on the subscription table.
    /// @pre Traits is not expected to have anything by this class and is not used.
    ///////////////////////////////////////
    template<class TableType, class Traits>
    class EventDispatcher<TableType,Traits,EventDispatcherType::EDT_Empty>
    {
    public:
        /// @brief Convenience type for the dispatcher to use when retrieving the subscriber during event dispatch.
        using DispatchGet = typename Traits::DispatchGet;
    protected:
        /// @brief Gets an iterator range for the subscribers to dispatch to.
        /// @remarks The use of "auto" return type here is due to the circular reference that occurs when trying
        /// to access anything in the scope of TableType.  TableType will inherit from this.  The "completeness"
        /// of a class is different based on where it is accessed.  Inside the body of a function is ok.  Just
        /// about anywhere else it is not.  So accessing types on TableType in the function declaration won't
        /// compile.
        /// @return Returns an IteratorRange of the subscribers stored by the subscription table.
        auto GetRange() const
        {
            TableType* CastedTable = const_cast<TableType*>( static_cast<const TableType*>(this) );
            typename TableType::StorageIterator RangeBegin = CastedTable->begin();
            typename TableType::StorageIterator RangeEnd = CastedTable->end();
            return IteratorRange<typename TableType::StorageIterator>(RangeBegin,RangeEnd);
        }

        /// @brief Class destructor.
        ~EventDispatcher() = default;
    public:
        /// @brief Class constructor.
        EventDispatcher() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Invokes a function on each subscriber.
        /// @tparam MemberFunct The class member function to call for the event.
        /// @tparam ArgTypes A parameter pack of the arguments to pass into MemberFunct.
        /// @pre MemberFunct is expected to be a member function on "SubscriberType" defined on TableType.
        /// @pre ArgTypes is expected to be a series of variables that match the signature of MemberFunct.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        template<class MemberFunct, class... ArgTypes>
        void DispatchEvent(MemberFunct Funct, ArgTypes&&... Args) const
        {
            IteratorRange<typename TableType::StorageIterator> Subs = this->GetRange();
            while( Subs.RangeStart != Subs.RangeEnd )
            {
                DispatchGet CurrSub = (*Subs.RangeStart);
                (EventHelper::ToPointer(CurrSub)->*Funct)(Args...);
                ++Subs.RangeStart;
            }
        }

        /// @brief Invokes a function on one specific subscriber.
        /// @tparam SubIDType The type used to identify the subscriber to dispatch to.
        /// @tparam MemberFunct The class member function to call for the event.
        /// @tparam ArgTypes A parameter pack of the arguments to pass into MemberFunct.
        /// @pre SubIDType is expected to be equality comparable to TableType::SubscriberType::IDType.
        /// @pre MemberFunct is expected to be a member function on "SubscriberType" defined on TableType.
        /// @pre ArgTypes is expected to be a series of variables that match the signature of MemberFunct.
        /// @param ID The ID of the specific subscriber to dispatch to.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        template<class SubIDType, class MemberFunct, class... ArgTypes>
        void DispatchEventSingle(const SubIDType ID, MemberFunct Funct, ArgTypes&&... Args) const
        {
            IteratorRange<typename TableType::StorageIterator> Subs = this->GetRange();
            while( Subs.RangeStart != Subs.RangeEnd )
            {
                if( EventHelper::ToPointer( *Subs.RangeStart )->GetID() == ID ) {
                    DispatchGet CurrSub = (*Subs.RangeStart);
                    (EventHelper::ToPointer(CurrSub)->*Funct)(Args...);
                    return;
                }
                ++Subs.RangeStart;
            }
        }

        /// @brief Invokes a function on each subscriber and returns one result.
        /// @tparam MemberFunct The class member function to call for the event.
        /// @tparam ArgTypes A parameter pack of the arguments to pass into MemberFunct.
        /// @pre MemberFunct is expected to be a member function on "SubscriberType" defined on TableType.
        /// @pre ArgTypes is expected to be a series of variables that match the signature of MemberFunct.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        /// @return Returns the value returned by the last subscriber an event was dispatched to.
        template<class MemberFunct, class... ArgTypes>
        auto DispatchQuery(MemberFunct Funct, ArgTypes&&... Args) const
        {
            using ReturnType = std::vector< EventHelper::DeduceRetType<MemberFunct> >;
            ReturnType Ret;
            IteratorRange<typename TableType::StorageIterator> Subs = this->GetRange();
            while( Subs.RangeStart != Subs.RangeEnd )
            {
                DispatchGet CurrSub = (*Subs.RangeStart);
                Ret.push_back( (EventHelper::ToPointer(CurrSub)->*Funct)(Args...) );
                ++Subs.RangeStart;
            }
            return Ret;
        }

        /// @brief Invokes a function on one specific subscriber and returns one result.
        /// @tparam SubIDType The type used to identify the subscriber to dispatch to.
        /// @tparam MemberFunct The class member function to call for the event.
        /// @tparam ArgTypes A parameter pack of the arguments to pass into MemberFunct.
        /// @pre SubIDType is expected to be equality comparable to TableType::SubscriberType::IDType.
        /// @pre MemberFunct is expected to be a member function on "SubscriberType" defined on TableType.
        /// @pre ArgTypes is expected to be a series of variables that match the signature of MemberFunct.
        /// @param ID The ID of the specific subscriber to dispatch to.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        /// @return Returns the value returned by the last subscriber an event was dispatched to.
        template<class SubIDType, class MemberFunct, class... ArgTypes>
        auto DispatchQuerySingle(const SubIDType ID, MemberFunct Funct, ArgTypes&&... Args) const
        {
            using ReturnType = std::vector< EventHelper::DeduceRetType<MemberFunct> >;
            ReturnType Ret;
            IteratorRange<typename TableType::StorageIterator> Subs = this->GetRange();
            while( Subs.RangeStart != Subs.RangeEnd )
            {
                if( EventHelper::ToPointer( *Subs.RangeStart )->GetID() == ID ) {
                    DispatchGet CurrSub = (*Subs.RangeStart);
                    Ret.push_back( (EventHelper::ToPointer(CurrSub)->*Funct)(Args...) );
                }
                ++Subs.RangeStart;
            }
            return Ret;
        }
    };//EventDispatcher

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is an event dispatcher capable of blocking the dispatch of events.
    /// @tparam TableType The type of subscription table using this dispatcher.
    /// @tparam Traits A collection of types that describes the operation of the subscription table.
    /// @pre TableType is expected to be a derived type of this dispatcher.  It is also expected to have an
    /// iterator type "StorageIterator" that can be dereferenced and converted to "SubscriberType", which
    /// also must be present and defined on the subscription table.
    /// @pre Traits is not expected to have anything by this class and is not used.
    ///////////////////////////////////////
    template<class TableType, class Traits>
    class EventDispatcher<TableType,Traits,EventDispatcherType::EDT_Silencable>
    {
    public:
        /// @brief Convenience type for the dispatcher to use when retrieving the subscriber during event dispatch.
        using DispatchGet = typename Traits::DispatchGet;
    protected:
        /// @brief Controls whether or not the firing of events should be suppressed.
        Boole MuteEvents;

        /// @brief Gets an iterator range for the subscribers to dispatch to.
        /// @remarks The use of "auto" return type here is due to the circular reference that occurs when trying
        /// to access anything in the scope of TableType.  TableType will inherit from this.  The "completeness"
        /// of a class is different based on where it is accessed.  Inside the body of a function is ok.  Just
        /// about anywhere else it is not.  So accessing types on TableType in the function declaration won't
        /// compile.
        /// @return Returns an IteratorRange of the subscribers stored by the subscription table.
        auto GetRange() const
        {
            TableType* CastedTable = const_cast<TableType*>( static_cast<const TableType*>(this) );
            typename TableType::StorageIterator RangeBegin = CastedTable->begin();
            typename TableType::StorageIterator RangeEnd = CastedTable->end();
            return IteratorRange<typename TableType::StorageIterator>(RangeBegin,RangeEnd);
        }

        /// @brief Class destructor.
        ~EventDispatcher() = default;
    public:
        /// @brief Class constructor.
        EventDispatcher() :
            MuteEvents(false)
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Sets whether or not event firings by this publisher will be suppressed.
        /// @param Mute True to prevent events from firing, false for normal operation.
        void SetMuteEvents(const Boole Mute)
            { this->MuteEvents = Mute; }
        /// @brief Gets whether or not event firings by this publisher will be suppressed.
        /// @return Returns true if events are being suppressed, false if this publisher is operating normally.
        Boole GetMuteEvents() const
            { return this->MuteEvents; }

        /// @brief Invokes a function on each subscriber.
        /// @tparam MemberFunct The class member function to call for the event.
        /// @tparam ArgTypes A parameter pack of the arguments to pass into MemberFunct.
        /// @pre MemberFunct is expected to be a member function on "SubscriberType" defined on TableType.
        /// @pre ArgTypes is expected to be a series of variables that match the signature of MemberFunct.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        template<class MemberFunct, class... ArgTypes>
        void DispatchEvent(MemberFunct Funct, ArgTypes&&... Args) const
        {
            if( !this->MuteEvents ) {
                IteratorRange<typename TableType::StorageIterator> Subs = this->GetRange();
                while( Subs.RangeStart != Subs.RangeEnd )
                {
                    DispatchGet CurrSub = (*Subs.RangeStart);
                    (EventHelper::ToPointer(CurrSub)->*Funct)(Args...);
                    ++Subs.RangeStart;
                }
            }
        }

        /// @brief Invokes a function on one specific subscriber.
        /// @tparam SubIDType The type used to identify the subscriber to dispatch to.
        /// @tparam MemberFunct The class member function to call for the event.
        /// @tparam ArgTypes A parameter pack of the arguments to pass into MemberFunct.
        /// @pre SubIDType is expected to be equality comparable to TableType::SubscriberType::IDType.
        /// @pre MemberFunct is expected to be a member function on "SubscriberType" defined on TableType.
        /// @pre ArgTypes is expected to be a series of variables that match the signature of MemberFunct.
        /// @param ID The ID of the specific subscriber to dispatch to.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        template<class SubIDType, class MemberFunct, class... ArgTypes>
        void DispatchEventSingle(const SubIDType ID, MemberFunct Funct, ArgTypes&&... Args) const
        {
            if( !this->MuteEvents ) {
                IteratorRange<typename TableType::StorageIterator> Subs = this->GetRange();
                while( Subs.RangeStart != Subs.RangeEnd )
                {
                    if( EventHelper::ToPointer( *Subs.RangeStart )->GetID() == ID ) {
                        DispatchGet CurrSub = (*Subs.RangeStart);
                        (EventHelper::ToPointer(CurrSub)->*Funct)(Args...);
                        return;
                    }
                    ++Subs.RangeStart;
                }
            }
        }

        /// @brief Invokes a function on each subscriber and returns one result.
        /// @tparam MemberFunct The class member function to call for the event.
        /// @tparam ArgTypes A parameter pack of the arguments to pass into MemberFunct.
        /// @pre MemberFunct is expected to be a member function on "SubscriberType" defined on TableType.
        /// @pre ArgTypes is expected to be a series of variables that match the signature of MemberFunct.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        /// @return Returns the value returned by the last subscriber an event was dispatched to.
        template<class MemberFunct, class... ArgTypes>
        auto DispatchQuery(MemberFunct Funct, ArgTypes&&... Args) const
        {
            using ReturnType = std::vector< EventHelper::DeduceRetType<MemberFunct> >;
            ReturnType Ret;
            if( !this->MuteEvents ) {
                IteratorRange<typename TableType::StorageIterator> Subs = this->GetRange();
                while( Subs.RangeStart != Subs.RangeEnd )
                {
                    DispatchGet CurrSub = (*Subs.RangeStart);
                    Ret.push_back( (EventHelper::ToPointer(CurrSub)->*Funct)(Args...) );
                    ++Subs.RangeStart;
                }
            }
            return Ret;
        }

        /// @brief Invokes a function on one specific subscriber and returns one result.
        /// @tparam SubIDType The type used to identify the subscriber to dispatch to.
        /// @tparam MemberFunct The class member function to call for the event.
        /// @tparam ArgTypes A parameter pack of the arguments to pass into MemberFunct.
        /// @pre SubIDType is expected to be equality comparable to TableType::SubscriberType::IDType.
        /// @pre MemberFunct is expected to be a member function on "SubscriberType" defined on TableType.
        /// @pre ArgTypes is expected to be a series of variables that match the signature of MemberFunct.
        /// @param ID The ID of the specific subscriber to dispatch to.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        /// @return Returns the value returned by the last subscriber an event was dispatched to.
        template<class SubIDType, class MemberFunct, class... ArgTypes>
        auto DispatchQuerySingle(const SubIDType ID, MemberFunct Funct, ArgTypes&&... Args) const
        {
            using ReturnType = std::vector< EventHelper::DeduceRetType<MemberFunct> >;
            ReturnType Ret;
            if( !this->MuteEvents ) {
                IteratorRange<typename TableType::StorageIterator> Subs = this->GetRange();
                while( Subs.RangeStart != Subs.RangeEnd )
                {
                    if( EventHelper::ToPointer( *Subs.RangeStart )->GetID() == ID ) {
                        DispatchGet CurrSub = (*Subs.RangeStart);
                        Ret.push_back( (EventHelper::ToPointer(CurrSub)->*Funct)(Args...) );
                    }
                    ++Subs.RangeStart;
                }
            }
            return Ret;
        }
    };//EventDispatcher

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a helper class that will store extra data that may be needed for a queued event dispatch.
    /// @tparam Traits A collection of types that describes the operation of the subscription table.
    ///////////////////////////////////////
    template<class Traits>
    class QueuedEvent
    {
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Public Types

        /// @brief Convenience type for the ID type used by subscribers.
        using SubscriberIDType = typename Traits::SubscriberIDType;
        /// @brief Storage type that will keep the stored ID uninitialized until we need it.
        using SubscriberIDStorage = std::aligned_storage_t<sizeof(SubscriberIDType),alignof(SubscriberIDType)>;
        /// @brief Function type that will store the event to be dispatched.
        using DispatchFunction = std::function<void(typename Traits::SubscribeArg,const Boole)>;

        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief The event to be dispatched to subscriber(s).
        DispatchFunction Funct;
    protected:
        /// @brief Whether or not we're sending this event to every subscriber.
        Boole Broadcast;
        /// @brief The ID of the one subscriber the event is bound for, if initialized.
        SubscriberIDStorage DispatchID;

        /// @brief Creates a new instance of a subscriber ID in an already existing chunk of memory.
        /// @remarks This does not allocate.  Expects to work with already allocated array memory.
        /// @tparam ArgTypes A variadic template of types for the parameters of construction.
        /// @param Args A collection of arguments to pass into the constructor.
        template<class... ArgTypes>
        void CreateID(ArgTypes&&... Args)
        {
            ::new (static_cast<void*>(&DispatchID)) SubscriberIDType(std::forward<ArgTypes>(Args)...);
        }
        /// @brief Destroys the ID if it was initialized.
        void DestroyID()
        {
            if( !this->Broadcast ) {
                reinterpret_cast<SubscriberIDType*>(&DispatchID)->~SubscriberIDType();
            }
        }
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Construction, Destruction, and Assignment

        /// @brief Default constructor (NO TOUCH!).
        QueuedEvent() = delete;
        /// @brief Broadcast dispatch constructor.
        /// @param Queued The event packaged into a std::function to be dispatched.
        QueuedEvent(DispatchFunction&& Queued) :
            Funct( std::move(Queued) ),
            Broadcast(true)
            {  }
        /// @brief Single subscriber dispatch constructor.
        /// @param ID The ID of the single subscriber to dispatch to.
        /// @param Queued The event packaged into a std::function to be dispatched.
        QueuedEvent(const SubscriberIDType& ID, DispatchFunction&& Queued) :
            Funct( std::move(Queued) ),
            Broadcast(false)
            { this->CreateID(ID); }
        /// @brief Copy constructor.
        /// @param Other The other event to NOT be copied.
        QueuedEvent(const QueuedEvent& Other) = delete;
        /// @brief Move constructor.
        /// @param Other The other event to be moved.
        QueuedEvent(QueuedEvent&& Other) = default;
        /// @brief Class destructor.
        ~QueuedEvent()
            { this->DestroyID(); }

        /// @brief Copy assignment operator.
        /// @param Other The other event to NOT be copied.
        /// @return Returns a reference to this.
        QueuedEvent& operator=(const QueuedEvent& Other) = delete;
        /// @brief Move assignment operator.
        /// @param Other The other event to be moved.
        /// @return Returns a reference to this.
        QueuedEvent& operator=(QueuedEvent&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets whether or not this is dispatching to every subscriber.
        /// @return Returns true if the event will be dispatched to every subscriber.
        Boole IsBroadcasting() const
            { return this->Broadcast; }
        /// @brief Gets whether or not this is dispatching to just one subscriber.
        /// @return Returns true if the ID is initialized and the event will be dispatched to only one subscriber, false otherwise.
        Boole IsDispatchingToID() const
            { return !this->Broadcast; }
        /// @brief Retrieves the ID of the only subscriber that will be dispatched to.
        /// @warning This is hella dangerous and undefined if this event is meant to broadcast, as the ID returned will be uninitialized.
        /// @return Returns a const reference to the ID of the subscriber to dispatch to.
        const SubscriberIDType& GetID() const
            { return *(reinterpret_cast<const SubscriberIDType*>(&DispatchID)); }
    };//QueuedEvent

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a helper class that will store the results and ready state for a given query dispatch.
    /// @tparam ResultsType The type returned by the function being queried.
    ///////////////////////////////////////
    template <class ResultsType>
    class QueuedQueryResults
    {
    public:
        /// @brief A container of results from the method queried.
        std::vector<ResultsType> Results;
        /// @brief An atomic flag storing whether or not the results are ready.
        std::atomic_bool ReadyState;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction, Destruction, and Assignment

        /// @brief Default constructor.
        QueuedQueryResults() :
            ReadyState(false)
            {  }
        /// @brief Copy constructor.
        /// @param Other The other results to NOT be copied.
        QueuedQueryResults(const QueuedQueryResults& Other) = delete;
        /// @brief Move constructor.
        /// @param Other The other results to be moved.
        QueuedQueryResults(QueuedQueryResults&& Other) = default;
        /// @brief Class destructor.
        ~QueuedQueryResults() = default;

        /// @brief Copy assignment operator.
        /// @param Other The other results to NOT be copied.
        /// @return Returns a reference to this.
        QueuedQueryResults& operator=(const QueuedQueryResults& Other) = delete;
        /// @brief Move assignment operator.
        /// @param Other The other results to be moved.
        /// @return Returns a reference to this.
        QueuedQueryResults& operator=(QueuedQueryResults&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief A non-blocking check to see if the results are ready to be accessed/used.
        /// @return Returns true if the Results member is ready to be used, false otherwise.
        Boole IsReady() const
            { return this->ReadyState.load(); }
    };//QueuedQueryResults

    /// @brief Convenience type for QueuedQueryResults managed by a shared_ptr.
    template<class ResultsType>
    using QueuedQueryResultsPtr = std::shared_ptr< QueuedQueryResults<ResultsType> >;

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is an event dispatcher that queues up events for future dispatch.
    /// @tparam TableType The type of subscription table using this dispatcher.
    /// @tparam Traits A collection of types that describes the operation of the subscription table.
    ///////////////////////////////////////
    template<class TableType, class Traits>
    class EventDispatcher<TableType,Traits,EventDispatcherType::EDT_Queued>
    {
    public:
        /// @brief The type that an event call will be bound to for later dispatch.
        using QueuedEventType = QueuedEvent<Traits>;
        /// @brief The type of container that is storing all of the events that are queued.
        using QueuedEventContainer = std::vector<QueuedEventType>;
        /// @brief Convenience type for the dispatcher to use when retrieving the subscriber during event dispatch.
        using DispatchGet = typename Traits::DispatchGet;
    protected:
        /// @brief A container of the events being held on to for future dispatch.
        QueuedEventContainer QueuedEvents;
        /// @brief A container of the queries being held on to for future dispatch.
        QueuedEventContainer QueuedQueries;

        /// @brief Gets an iterator range for the subscribers to dispatch to.
        /// @remarks The use of "auto" return type here is due to the circular reference that occurs when trying
        /// to access anything in the scope of TableType.  TableType will inherit from this.  The "completeness"
        /// of a class is different based on where it is accessed.  Inside the body of a function is ok.  Just
        /// about anywhere else it is not.  So accessing types on TableType in the function declaration won't
        /// compile.
        /// @return Returns an IteratorRange of the subscribers stored by the subscription table.
        auto GetRange() const
        {
            TableType* CastedTable = const_cast<TableType*>( static_cast<const TableType*>(this) );
            typename TableType::StorageIterator RangeBegin = CastedTable->begin();
            typename TableType::StorageIterator RangeEnd = CastedTable->end();
            return IteratorRange<typename TableType::StorageIterator>(RangeBegin,RangeEnd);
        }

        /// @brief Class destructor.
        ~EventDispatcher() = default;
    public:
        /// @brief Class constructor.
        EventDispatcher()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Queues an event for later dispatch on all subscribers.
        /// @tparam MemberFunct The class member function to call for the event.
        /// @tparam ArgTypes A parameter pack of the arguments to pass into MemberFunct.
        /// @pre MemberFunct is expected to be a member function on "SubscriberType" defined on TableType.
        /// @pre ArgTypes is expected to be a series of variables that match the signature of MemberFunct.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        template<class MemberFunct, class... ArgTypes>
        void DispatchEvent(MemberFunct Funct, ArgTypes&&... Args)
        {
            this->QueuedEvents.emplace_back( std::bind(Funct,std::placeholders::_1,std::forward<ArgTypes>(Args)...) );
        }

        /// @brief Queues an event for later dispatch on one subscriber.
        /// @tparam SubIDType The type used to identify the subscriber to dispatch to.
        /// @tparam MemberFunct The class member function to call for the event.
        /// @tparam ArgTypes A parameter pack of the arguments to pass into MemberFunct.
        /// @pre SubIDType is expected to be equality comparable to TableType::SubscriberType::IDType.
        /// @pre MemberFunct is expected to be a member function on "SubscriberType" defined on TableType.
        /// @pre ArgTypes is expected to be a series of variables that match the signature of MemberFunct.
        /// @param ID The ID of the specific subscriber to dispatch to.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        template<class SubIDType, class MemberFunct, class... ArgTypes>
        void DispatchEventSingle(const SubIDType ID, MemberFunct Funct, ArgTypes&&... Args)
        {
            this->QueuedEvents.emplace_back( ID, std::bind(Funct,std::placeholders::_1,std::forward<ArgTypes>(Args)...) );
        }

        /// @brief Queues a query for later dispatch on all subscribers.
        /// @tparam MemberFunct The class member function to call for the event.
        /// @tparam ArgTypes A parameter pack of the arguments to pass into MemberFunct.
        /// @pre MemberFunct is expected to be a member function on "SubscriberType" defined on TableType.
        /// @pre ArgTypes is expected to be a series of variables that match the signature of MemberFunct.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        template<class MemberFunct, class... ArgTypes>
        auto DispatchQuery(MemberFunct Funct, ArgTypes&&... Args)
        {
            using ReturnType = EventHelper::DeduceRetType<MemberFunct>;
            QueuedQueryResultsPtr<ReturnType> QueryResults = std::make_shared< QueuedQueryResults<ReturnType> >();
            this->QueuedQueries.emplace_back( [QueryResults,Funct,Args...](typename Traits::SubscribeArg Subscriber, const Boole Final) {
                QueryResults->Results.push_back( std::move( (EventHelper::ToPointer(Subscriber)->*Funct)(Args...) ) );
                if( Final ) {
                    QueryResults->ReadyState.store(true);
                }
            } );
            return QueryResults;
        }

        /// @brief Queues a query for later dispatch on one subscriber.
        /// @tparam SubIDType The type used to identify the subscriber to dispatch to.
        /// @tparam MemberFunct The class member function to call for the event.
        /// @tparam ArgTypes A parameter pack of the arguments to pass into MemberFunct.
        /// @pre SubIDType is expected to be equality comparable to TableType::SubscriberType::IDType.
        /// @pre MemberFunct is expected to be a member function on "SubscriberType" defined on TableType.
        /// @pre ArgTypes is expected to be a series of variables that match the signature of MemberFunct.
        /// @param ID The ID of the specific subscriber to dispatch to.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        template<class SubIDType, class MemberFunct, class... ArgTypes>
        auto DispatchQuerySingle(const SubIDType ID, MemberFunct Funct, ArgTypes&&... Args)
        {
            using ReturnType = EventHelper::DeduceRetType<MemberFunct>;
            QueuedQueryResultsPtr<ReturnType> QueryResults = std::make_shared< QueuedQueryResults<ReturnType> >();
            this->QueuedQueries.emplace_back( ID, [QueryResults,Funct,Args...](typename Traits::SubscribeArg Subscriber, const Boole Final) {
                QueryResults->Results.push_back( std::move( (EventHelper::ToPointer(Subscriber)->*Funct)(Args...) ) );
                QueryResults->ReadyState.store(true);
            } );
            return QueryResults;
        }

        /// @brief Fires all queued events to the provided subscribers.
        void FlushAllEvents()
        {
            using DispatchIterator = typename TableType::StorageIterator;
            IteratorRange<DispatchIterator> Subs = this->GetRange();
            for( QueuedEventType& Queued : this->QueuedEvents )
            {
                DispatchIterator RangeBegin = Subs.begin();
                const DispatchIterator RangeEnd = Subs.end();
                //const DispatchIterator RangeLast = RangeEnd - 1;
                if( Queued.IsBroadcasting() ) {
                    while( RangeBegin != RangeEnd )
                    {
                        DispatchGet CurrSub = (*RangeBegin);
                        Queued.Funct(CurrSub,false);
                        ++RangeBegin;
                    }
                }else if( Queued.IsDispatchingToID() ) {
                    auto FindPredicate = [&Queued](typename TableType::StoredType PredSub) -> Boole {
                        DispatchGet CurrSub = PredSub;
                        return ( EventHelper::ToPointer(CurrSub)->GetID() == Queued.GetID() );
                    };
                    DispatchIterator DisIt = std::find_if(RangeBegin,RangeEnd,FindPredicate);
                    if( DisIt != RangeEnd ) {
                        DispatchGet CurrSub = (*DisIt);
                        Queued.Funct(CurrSub,true);
                    }
                }
            }
            this->QueuedEvents.clear();
        }
        /// @brief Fires all queued queries to the provided subscribers.
        void FlushAllQueries()
        {
            using DispatchIterator = typename TableType::StorageIterator;
            IteratorRange<DispatchIterator> Subs = this->GetRange();
            for( QueuedEventType& Queued : this->QueuedQueries )
            {
                DispatchIterator RangeBegin = Subs.begin();
                const DispatchIterator RangeEnd = Subs.end();
                const DispatchIterator RangeLast = RangeEnd - 1;
                if( Queued.IsBroadcasting() ) {
                    while( RangeBegin != RangeEnd )
                    {
                        DispatchGet CurrSub = (*RangeBegin);
                        Queued.Funct(CurrSub,RangeBegin == RangeLast);
                        ++RangeBegin;
                    }
                }else if( Queued.IsDispatchingToID() ) {
                    auto FindPredicate = [&Queued](typename TableType::StoredType PredSub) -> Boole {
                        DispatchGet CurrSub = PredSub;
                        return ( EventHelper::ToPointer(CurrSub)->GetID() == Queued.GetID() );
                    };
                    DispatchIterator DisIt = std::find_if(RangeBegin,RangeEnd,FindPredicate);
                    if( DisIt != RangeEnd ) {
                        DispatchGet CurrSub = (*DisIt);
                        Queued.Funct(CurrSub,true);
                    }
                }
            }
            this->QueuedQueries.clear();
        }
        /// @brief Flushes all Events and Queries.
        void FlushAll()
        {
            this->FlushAllEvents();
            this->FlushAllQueries();
        }
    };//EventDispatcher

    /// @}
}//Mezzanine

#endif
