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

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a basic event dispatcher that does simple mindless forwarding.
    ///////////////////////////////////////
    template<class IteratorType>
    class MEZZ_LIB EmptyEventDispatcher
    {
    public:
        /*
        /// @brief Convenience type for the element that is stored by the table we are dispatching for.
        using StoredType = typename TableType::StoredType;
        /// @brief Convenience type for the container of elements used by the table we are dispatching for.
        using ContainerType = typename TableType::ContainerType;
        */
    public:
        /// @brief Class constructor.
        EmptyEventDispatcher() = default;
        /// @brief Class destructor.
        ~EmptyEventDispatcher() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Fires an event.
        /// @tparam MemberFunt The class member function to call for the event.
        /// @tparam ArgTypes A parameter pack of the arguments to pass into MemberFunct.
        /// @param RangeBegin An iterator to the start of the range to dispatch the event to..
        /// @param RangeEnd An iterator to the end of the range to dispatch the event to.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        template<class MemberFunct, class... ArgTypes>
        void DispatchEvent(IteratorType RangeBegin, IteratorType RangeEnd, MemberFunct Funct, ArgTypes&&... Args)
        {
            while( RangeBegin != RangeEnd )
            {
               (EventHelper::ToPointer(*RangeBegin)->*Funct)(Args...);
                ++RangeBegin;
            }
        }
    };//EmptyEventDispatcher

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is an event dispatcher capable of blocking the dispatch of events.
    ///////////////////////////////////////
    template<class IteratorType>
    class MEZZ_LIB SilenceableEventDispatcher
    {
    public:
    protected:
        /// @brief Controls whether or not the firing of events should be suppressed.
        Boole MuteEvents;
    public:
        /// @brief Class constructor.
        SilenceableEventDispatcher() :
            MuteEvents(false)
            {  }
        /// @brief Class destructor.
        ~SilenceableEventDispatcher() = default;

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

        /// @brief Fires an event.
        /// @tparam MemberFunt The class member function to call for the event.
        /// @tparam ArgTypes A parameter pack of the arguments to pass into MemberFunct.
        /// @param RangeBegin An iterator to the start of the range to dispatch the event to..
        /// @param RangeEnd An iterator to the end of the range to dispatch the event to.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        template<class MemberFunct, class... ArgTypes>
        void DispatchEvent(IteratorType RangeBegin, IteratorType RangeEnd, MemberFunct Funct, ArgTypes&&... Args) const
        {
            if( !this->MuteEvents ) {
                while( RangeBegin != RangeEnd )
                {
                    (EventHelper::ToPointer(*RangeBegin)->*Funct)(Args...);
                    ++RangeBegin;
                }
            }
        }
    };//SilenceableEventDispatcher

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is an event dispatcher that queues up events for future dispatch.
    ///////////////////////////////////////
    template<class IteratorType>
    class MEZZ_LIB QueuedEventDispatcher
    {
    public:
        /// @brief The type that an event call will be bound to for later dispatch.
        using QueuedEventType = std::function<void(typename IteratorType::ReferenceType)>;
        /// @brief The type of container that is storing all of the events that are queued.
        using QueuedEventContainer = std::vector<QueuedEventType>;
    protected:
        /// @brief A container of the events being held on to for future dispatch.
        QueuedEventContainer QueuedEvents;
    public:
        /// @brief Class constructor.
        QueuedEventDispatcher()
            {  }
        /// @brief Class destructor.
        ~QueuedEventDispatcher() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Queues an event for later dispatch.
        /// @tparam MemberFunt The class member function to call for the event.
        /// @tparam ArgTypes A parameter pack of the arguments to pass into MemberFunct.
        /// @param RangeBegin An iterator to the start of the range to dispatch the event to.  Ignored.
        /// @param RangeEnd An iterator to the end of the range to dispatch the event to.  Ignored.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments/event specific data related to this event.
        template<class MemberFunct, class... ArgTypes>
        void DispatchEvent(IteratorType RangeBegin, IteratorType RangeEnd, MemberFunct Funct, ArgTypes&&... Args) const
        {
            this->QueuedEvents.emplace_back( std::bind(Funct,std::placeholders::_1,Args...) );
        }

        /// @brief Fires all queued events to the provided subscribers.
        /// @param RangeBegin An iterator to the start of the range to dispatch the event to..
        /// @param RangeEnd An iterator to the end of the range to dispatch the event to.
        void FlushEvents(IteratorType RangeBegin, IteratorType RangeEnd)
        {
            for( QueuedEventType Queued : this->QueuedEvents )
            {
                while( RangeBegin != RangeEnd )
                {
                    Queued(*RangeBegin);
                    ++RangeBegin;
                }
            }
            this->QueuedEvents.clear();
        }
    };//QueuedEventDispatcher
}//Mezzanine

#endif
