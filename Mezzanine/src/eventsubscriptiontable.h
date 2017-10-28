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
#include "eventhelper.h"
#include "eventid.h"
#include "eventsubscriptiontablebase.h"
#include "eventdispatcher.h"

namespace Mezzanine
{
    /// @addtogroup Events
    /// @{

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a default configuration class of types to use for an EventSubscriptionTable.
    /// @tparam Interface The interface that will subscribe to events in the table using these traits.
    ///////////////////////////////////////
    template<class Interface>
    struct MEZZ_LIB EventSubscriptionTableTraits
    {
        /// @brief Retrievable type for querying the type of callable interface this table works with.
        using SubscriberType = Interface;
        /// @brief Convenience type and check for what exactly will be stored by this subscription table.
        using StoredType = Interface;
        /// @brief Container for the storage of bindings between subscribers and the events they are interested in.
        using StorageContainer = std::vector<StoredType>;
        /// @brief Iterator type for subscriber bindings stored by this table.
        using StorageIterator = typename StorageContainer::iterator;
        /// @brief Const Iterator type for subscriber bindings stored by this table.
        using ConstStorageIterator = typename StorageContainer::const_iterator;
        /// @brief The type to use for uniquely identifying this table among a group of like tables.
        using DispatchIDType = EventID;
        /// @brief The type of iterator that will be passed to the dispatcher when an event is dispatched.
        using DispatchIterator = StorageIterator;
        /// @brief The type to use for the actual dispatch logic for events.
        using DispatcherType = EmptyEventDispatcher<DispatchIterator>;
    };//EventSubscriptionTableTraits

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This class represents a given event that can be subscribed to and/or fired.
    /// @tparam Traits A struct of types to use for the operation of this table.
    ///////////////////////////////////////
    template<class Traits>
    class MEZZ_LIB EventSubscriptionTable : public EventSubscriptionTableBase
    {
    public:
        /// @brief Convenience type for describing the type of this.
        using SelfType = EventSubscriptionTable<Traits>;
        /// @brief Retrievable type for querying the type of callable interface this table works with.
        using SubscriberType = typename Traits::SubscriberType;
        /// @brief The type to use for uniquely identifying instances of subscribers.
        using SubscriberIDType = typename SubscriberType::IDType;
        /// @brief Convenience type for passing the subscriber as an argument to the Subscribe method.
        using SubscribeArg = typename std::conditional<std::is_pointer<SubscriberType>::value,SubscriberType,const SubscriberType&>::type;
        /// @brief Convenience type for the return value of the Subscribe method.
        using SubscribeRet = void;
        /// @brief Convenience type and check for what exactly will be stored by this subscription table.
        using StoredType = typename Traits::StoredType;
        /// @brief Container for the storage of bindings between subscribers and the events they are interested in.
        using StorageContainer = typename Traits::StorageContainer;
        /// @brief Iterator type for subscriber bindings stored by this table.
        using StorageIterator = typename Traits::StorageIterator;
        /// @brief Const Iterator type for subscriber bindings stored by this table.
        using ConstStorageIterator = typename Traits::ConstStorageIterator;
        /// @brief The type to use for uniquely identifying this table among a group of like tables.
        using DispatchIDType = typename Traits::DispatchIDType;
        /// @brief The type of iterator that will be passed to the dispatcher when an event is dispatched.
        using DispatchIterator = typename Traits::DispatchIterator;
        /// @brief The type to use for the actual dispatch logic for events.
        using DispatcherType = typename Traits::DispatcherType;
    protected:
        /// @brief A container of all the subscriber bindings to this event table.
        StorageContainer Subscribers;
        /// @brief The dispatcher that will determine how/if the event should be routed to subscribers.
        DispatcherType Dispatcher;
        /// @brief The hash of the Event the subscribers in this table are subscribed to.
        DispatchIDType DisID;
    public:
        /// @brief Blank constructor.
        EventSubscriptionTable() = delete;
        /// @brief Copy constructor.
        /// @param Other The other table to NOT be copied.
        EventSubscriptionTable(const SelfType& Other) = delete;
        /// @brief Move constructor.
        /// @param Other The other table to be moved.
        EventSubscriptionTable(SelfType&& Other) = default;
        /// @brief Identifier constructor.
        /// @param ID The unique ID of the event this table will store subscriptions for.
        EventSubscriptionTable(const DispatchIDType ID) :
            DisID(ID)
            {  }
        /// @brief Class destructor.
        ~EventSubscriptionTable()
            { this->UnsubscribeAll(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other table to NOT be copied.
        /// @return Returns a reference to this.
        SelfType& operator=(const SelfType& Other) = delete;
        /// @brief Move assignment operator.
        /// @param Other The other table to be moved.
        /// @return Returns a reference to this.
        SelfType& operator=(SelfType&& Other) = default;

        /// @brief Less-than operator.
        /// @param Other The other table to be compared to.
        /// @return Returns true if this table is considered less than the other, or should be sorted before the other table.
        Boole operator<(const SelfType& Other) const
            { return this->DisID < Other.DisID; }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the event dispatcher.
        /// @return Returns a reference to the event dispatcher being used by this EventSubscriptionTable.
        DispatcherType& GetDispatcher()
            { return this->Dispatcher; }
        /// @brief Gets the event dispatcher.
        /// @return Returns a const reference to the event dispatcher being used by this EventSubscriptionTable.
        const DispatcherType& GetDispatcher() const
            { return this->Dispatcher; }

        /// @brief Gets the ID of the event associated with this table.
        /// @return Returns the unique ID of this event.
        DispatchIDType GetID() const
            { return this->DisID; }

        ///////////////////////////////////////////////////////////////////////////////
        // Subscription Management

        /// @brief Adds a subscriber to this event table.
        /// @exception If the ID of the Sub is already being used by another binding/subscriber this will throw a "II_DUPLICATE_IDENTITY_EXCEPTION".
        /// @param Sub The subscriber to be called when the interested event is fired.
        /// @return Returns a pointer to the created Subscriber slot for the provided subscriber.
        SubscribeRet Subscribe(SubscribeArg Sub)
        {
            for( StorageIterator StorIt : this->Subscribers )
            {
                if( (*StorIt)->GetID() == EventHelper::ToPointer(Sub)->GetID() ) {
                    MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,"A subscriber with that ID already exists!");
                }
            }
            this->Subscribers.push_back(Sub);
            return;
        }
        /// @brief Gets a binding by the subscriber ID.
        /// @param ID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        /// @return Returns the binding with the specified ID, or nullptr of none exists.
        SubscriberType GetSubscriber(const SubscriberIDType ID) const
        {
            for( ConstStorageIterator SubIt = this->Subscribers.begin() ; SubIt != this->Subscribers.end() ; ++SubIt )
            {
                if( (*SubIt)->GetID() == ID ) {
                    return (*SubIt);
                }
            }
            return nullptr;
        }

        /// @brief Removes a single subscriber from this event table.
        /// @param ID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        void Unsubscribe(const SubscriberIDType ID)
        {
            for( StorageIterator SubIt = this->Subscribers.begin() ; SubIt != this->Subscribers.end() ; ++SubIt )
            {
                if( (*SubIt)->GetSubID() == ID ) {
                    this->Subscribers.erase(SubIt);
                    return;
                }
            }
        }
        /// @brief Removes all subscribers from all events in this publisher.
        /// @return Returns the number of subscribers removed.
        Whole UnsubscribeAll()
        {
            Whole RemoveCount = this->Subscribers.size();
            this->Subscribers.clear();
            return RemoveCount;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @internal
        /// @brief Notifies all subscribers of this event that this event is firing.
        /// @param Funct The function on the subscriber to call.
        /// @param Args The arguments and extra data related to this event.
        template<class MemberFunct, class... ArgTypes>
        void DispatchEvent(MemberFunct Funct, ArgTypes&&... Args)
        {
            //for( ConstStorageIterator SubIt = this->Subscribers.begin() ; SubIt != this->Subscribers.end() ; ++SubIt )
            //    { ((*SubIt)->*Funct)(Args...); }
            DispatchIterator Begin = this->Subscribers.begin();
            DispatchIterator End = this->Subscribers.end();
            this->Dispatcher.DispatchEvent(Begin,End,Funct,std::forward<ArgTypes>(Args)...);
        }
    };//EventSubscriptionTable

    /// @brief Convenience type for an EventSubscriptionTable using default traits.
    template<class Interface>
    using DefaultEventSubscriptionTable = EventSubscriptionTable< EventSubscriptionTableTraits< Interface > >;

    /// @}
}//Mezzanine

#endif
