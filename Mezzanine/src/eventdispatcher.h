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
    protected:
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
            TableType* CastedTable = const_cast<TableType*>( static_cast<const TableType*>(this) );
            typename TableType::StorageIterator RangeBegin = CastedTable->begin();
            typename TableType::StorageIterator RangeEnd = CastedTable->end();
            while( RangeBegin != RangeEnd )
            {
                typename TableType::SubscriberType CurrSub = (*RangeBegin);
                (EventHelper::ToPointer(CurrSub)->*Funct)(Args...);
                ++RangeBegin;
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
        auto DispatchEventReturn(MemberFunct Funct, ArgTypes&&... Args) const -> decltype(Funct(Args...))
        {
            using ReturnType = decltype(Funct(Args...));
            ReturnType Ret;
            TableType* CastedTable = const_cast<TableType*>( static_cast<const TableType*>(this) );
            typename TableType::StorageIterator RangeBegin = CastedTable->begin();
            typename TableType::StorageIterator RangeEnd = CastedTable->end();
            while( RangeBegin != RangeEnd )
            {
                typename TableType::SubscriberType CurrSub = (*RangeBegin);
                Ret = (EventHelper::ToPointer(CurrSub)->*Funct)(Args...);
                ++RangeBegin;
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
    protected:
        /// @brief Controls whether or not the firing of events should be suppressed.
        Boole MuteEvents;

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
                TableType* CastedTable = const_cast<TableType*>( static_cast<const TableType*>(this) );
                typename TableType::StorageIterator RangeBegin = CastedTable->begin();
                typename TableType::StorageIterator RangeEnd = CastedTable->end();
                while( RangeBegin != RangeEnd )
                {
                    typename TableType::SubscriberType CurrSub = (*RangeBegin);
                    (EventHelper::ToPointer(CurrSub)->*Funct)(Args...);
                    ++RangeBegin;
                }
            }
        }
    };//EventDispatcher

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
        using QueuedEventType = std::function<void(typename Traits::SubscribeArg)>;
        /// @brief The type of container that is storing all of the events that are queued.
        using QueuedEventContainer = std::vector<QueuedEventType>;
    protected:
        /// @brief A container of the events being held on to for future dispatch.
        QueuedEventContainer QueuedEvents;

        /// @brief Class destructor.
        ~EventDispatcher() = default;
    public:
        /// @brief Class constructor.
        EventDispatcher()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Queues an event for later dispatch.
        /// @tparam MemberFunct The class member function to call for the event.
        /// @tparam ArgTypes A parameter pack of the arguments to pass into MemberFunct.
        /// @pre MemberFunct is expected to be a member function on "SubscriberType" defined on TableType.
        /// @pre ArgTypes is expected to be a series of variables that match the signature of MemberFunct.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        template<class MemberFunct, class... ArgTypes>
        void DispatchEvent(MemberFunct Funct, ArgTypes&&... Args) const
        {
            this->QueuedEvents.emplace_back( std::bind(Funct,std::placeholders::_1,std::forward<ArgTypes>(Args)...) );
        }

        /// @brief Fires all queued events to the provided subscribers.
        void FlushEvents()
        {
            for( QueuedEventType Queued : this->QueuedEvents )
            {
                TableType* CastedTable = static_cast<TableType*>(this);
                typename TableType::StorageIterator RangeBegin = CastedTable->begin();
                typename TableType::StorageIterator RangeEnd = CastedTable->end();
                while( RangeBegin != RangeEnd )
                {
                    typename TableType::SubscriberType CurrSub = (*RangeBegin);
                    Queued(CurrSub);
                    ++RangeBegin;
                }
            }
            this->QueuedEvents.clear();
        }
    };//EventDispatcher

    /// @}
}//Mezzanine

#endif
