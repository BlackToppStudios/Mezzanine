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
#ifndef _eventsubscriptiontabletraits_h
#define _eventsubscriptiontabletraits_h

#include "eventid.h"
#include "eventsubscriberbinding.h"

namespace Mezzanine
{
    /// @addtogroup Events
    /// @{

    /// @brief An enum describing which factory template specialization to use.
    enum class SubscriptionFactoryType
    {
        SFT_Subscriber,     ///< A factory type that uses the subscriber as the subscription type.  It's simple with no extra constructs.
        SFT_Binding,        ///< A factory type that creates bindings as the subscription to help track/managed lifetimes of objects.
    };

    /// @brief An enum describing which container template specialization to use.
    enum class SubscriptionContainerType
    {
        SCT_Single,         ///< A container that stores only one element/subscriber.
        SCT_Unsorted,       ///< A container that stores N-elements/subscribers.
        SCT_Unsorted_Fixed, ///< A container that stores a specific amount of elements/subscribers.
        SCT_Sorted,         ///< A container that stores N-elements/subscribers and keeps them sorted by a given predicate.
        SCT_Sorted_Fixed    ///< A container that stores a specific amount of elements/subscribers and keeps them sorted by a given predicate.
    };

    /// @brief An enum describing which dispatcher template specialization to use.
    enum class EventDispatcherType
    {
        EDT_Empty,          ///< A simple dispatcher with no special features other than its size (which is zero!).
        EDT_Silencable,     ///< A dispatcher that can be set to ignore/discard all requests to dispatch events, allowing it to be silenced.
        EDT_Queued,         ///< A dispatcher that will cache dispatches using std::bind for later dispatch.  Cached events must be flushed manually.
    };

    // Forward Declare for EventSubscriptionFactory
    template<class TableType, class Traits, SubscriptionFactoryType FactoryType>
    class EventSubscriptionFactory;
    // Forward Declare for EventSubscriptionContainer
    template<class TableType, class Traits, SubscriptionContainerType ContainerType>
    class EventSubscriptionContainer;
    // Forward Declare for EventDispatcher
    template<class TableType, class Traits, EventDispatcherType DispatcherType>
    class EventDispatcher;

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a default configuration class of types to use for an EventSubscriptionTable.
    /// @tparam Interface The interface that will subscribe to events in the table using these traits.
    /// @pre Interface is required to have a type "IDType" on it as well as a method named "GetID" which is const, accepts no parameters and returns IDType.
    ///////////////////////////////////////
    template<class Interface>
    struct EventSubscriptionTableConfig
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Subscriber Traits

        /// @brief Retrievable type for querying the type of callable interface this table works with.
        using SubscriberType = Interface;

        ///////////////////////////////////////////////////////////////////////////////
        // Factory Traits

        /// @brief The type of subscription factory to use.
        static const SubscriptionFactoryType FactoryType = SubscriptionFactoryType::SFT_Subscriber;

        ///////////////////////////////////////////////////////////////////////////////
        // Storage Traits

        /// @brief The type of subscription container to use.
        static const SubscriptionContainerType ContainerType = SubscriptionContainerType::SCT_Unsorted;
        /// @brief The amount of subscribers to allocate for when using fixed size subscription containers.
        /// @remarks This is ignored if ContainerType is not SCT_Unsorted_Fixed or SCT_Sorted_Fixed.
        static const size_t StorageCount = 4;
        /// @brief The function to use for sorting the subscriber container.
        /// @remarks This is ignored if ContainerType is not SCT_Sorted or SCT_Sorted_Fixed.
        using StoragePredicate = std::less<SubscriberType>;

        ///////////////////////////////////////////////////////////////////////////////
        // Dispatch Traits

        /// @brief The type of subscription dispatcher to use.
        static const EventDispatcherType DispatcherType = EventDispatcherType::EDT_Empty;
        /// @brief The type to use for uniquely identifying this table among a group of like tables.
        using DispatchIDType = EventID;
    };//EventSubscriptionTableConfig

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a default configuration class of types to use for an EventBindingTable.
    /// @tparam Interface The interface that will subscribe to events in the table using these traits.
    /// @pre Interface is required to have a type "IDType" on it as well as a method named "GetID" which is const, accepts no parameters and returns IDType.
    ///////////////////////////////////////
    template<class Interface>
    struct EventBindingTableConfig : public EventSubscriptionTableConfig<Interface>
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Factory Traits

        /// @brief The type of subscription factory to use.
        static const SubscriptionFactoryType FactoryType = SubscriptionFactoryType::SFT_Binding;
    };//EventBindingTableConfig

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a default configuration class of types to use for an EventBindingTable.
    /// @tparam Table The type of table using these traits.
    /// @tparam Interface The interface that will subscribe to events in the table using these traits.
    /// @pre Table is not expected to have anything by this class, however other classes used here will have their own expectations.
    /// @pre Interface is required to have a type "IDType" on it as well as a method named "GetID" which is const, accepts no parameters and returns IDType.
    ///////////////////////////////////////
    template<class Table, class Config>
    struct EventSubscriptionTableTraits
    {
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Error Checking

        /// @brief Sanity check to make sure we're not attempting to store subscribers by reference.
        static_assert(!std::is_reference<typename Config::SubscriberType>::value,"The event system does not permit reference subscriber types.");

        ///////////////////////////////////////////////////////////////////////////////
        // Subscriber Traits

        /// @brief Retrievable type for querying the type of callable interface this table works with.
        using SubscriberType = typename Config::SubscriberType;
        /// @brief Guaranteed to be a non-pointer, non-reference SubscriberType.
        using SubscriberValue = typename std::remove_pointer<SubscriberType>::type;
        /// @brief A pointer to the underlying Subscriber type.
        using SubscriberPtr = typename std::conditional<std::is_pointer<SubscriberType>::value,SubscriberType,SubscriberType*>::type;
        /// @brief The type to use for uniquely identifying instances of subscribers.
        using SubscriberIDType = typename SubscriberValue::IDType;

        ///////////////////////////////////////////////////////////////////////////////
        // Major Types

        /// @brief Convenience type for this traits type.
        using SelfType = EventSubscriptionTableTraits<Table,Config>;
        /// @brief Retrievable type for the config used to generate these traits.
        using ConfigType = Config;
        /// @brief Convenience type for the table using these traits.
        using TableType = Table;
        /// @brief Factory type used to construct subscriptions tracked by the table.
        using ActualFactoryType = EventSubscriptionFactory<TableType,SubscriberType,Config::FactoryType>;

        ///////////////////////////////////////////////////////////////////////////////
        // Other Types

        /// @brief Type stored in subscription containers representing the subscriptions.
        using StoredType = typename ActualFactoryType::StoredType;
        /// @brief Convenience type for what is passed back to the user for tracking the subscription.
        /// @remarks This is allowed to be different from StoredType, but StoredType must be implicitly convertible to this type.
        using SubscribeRet = typename ActualFactoryType::SubscribeRet;
        /// @brief Convenience type for passing the subscriber as an argument to the Subscribe method.
        using SubscribeArg = typename ActualFactoryType::SubscribeArg;
        /// @brief Convenience type for passing the subscriber as an argument to the Subscribe method.
        using SubscriptionGet = typename ActualFactoryType::SubscriptionGet;

        ///////////////////////////////////////////////////////////////////////////////
        // Conditional Types

        /// @brief The amount of subscribers to allocate for when using fixed size subscription containers.
        /// @remarks This is ignored if ContainerType is not SCT_Unsorted_Fixed or SCT_Sorted_Fixed.
        static const size_t StorageCount = Config::StorageCount;
        /// @brief The function to use for sorting the subscriber container.
        /// @remarks This is ignored if ContainerType is not SCT_Sorted or SCT_Sorted_Fixed.
        using StoragePredicate = typename Config::StoragePredicate;
    };//EventSubscriptionTableTraits

    /// @}
}//Mezzanine

#endif
