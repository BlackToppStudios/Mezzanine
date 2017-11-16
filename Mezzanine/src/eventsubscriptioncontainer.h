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
#ifndef _eventsubscriptioncontainer_h
#define _eventsubscriptioncontainer_h

#include "eventsubscriberbinding.h"
#include "sortedvector.h"
#include "managedarray.h"
#include "sortedmanagedarray.h"

namespace Mezzanine
{
    /// @addtogroup Events
    /// @{

    /// @brief An enum describing which container template specialization to use.
    enum class SubscriberContainerType
    {
        //SCT_Single,         ///< A container that stores only one element/subscriber.
        SCT_Unsorted,       ///< A container that stores N-elements/subscribers.
        SCT_Unsorted_Fixed, ///< A container that stores a specific amount of elements/subscribers.
        SCT_Sorted,         ///< A container that stores N-elements/subscribers and keeps them sorted by a given predicate.
        SCT_Sorted_Fixed    ///< A container that stores a specific amount of elements/subscribers and keeps them sorted by a given predicate.
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a convenience iterator class used by the EventBindingTable.
    /// @tparam ContainerType The type of subscriber container that this iterator is being used by.
    ///////////////////////////////////////
    template<class ContainerType>
    class MEZZ_LIB BindingIterator
    {
    public:
        /// @brief Convenience type for the BindingIterator implementation being used.
        using SelfType = BindingIterator<ContainerType>;
        /// @brief Convenience type to the binding pointed to by the iterator.
        using StoredType = typename ContainerType::StoredType;
        /// @brief Convenience type for the interface/subscriber this iterator will return on dereference.
        using SubscriberType = typename ContainerType::SubscriberType;
    protected:
        /// @brief A pointer to the binding that will be used on dereference.
        StoredType* Value;
    public:
        /// @brief Blank constructor.
        BindingIterator() :
            Value(nullptr)
            {  }
        /// @brief Iterator constructor.
        /// @param It The iterator to dereference and use to initialize this iterator.
        template<class IteratorType>
        BindingIterator(const IteratorType It) :
            Value( std::addressof(*It) )
            {  }
        /// @brief Descriptive constructor.
        /// @param Val A pointer to the value that will be used by this iterator.
        BindingIterator(StoredType* Val) :
            Value(Val)
            {  }
        /// @brief Copy constructor.
        /// @param Other The other iterator to be copied.
        BindingIterator(const SelfType& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other iterator to be moved.
        BindingIterator(SelfType&& Other) = default;
        /// @brief Class destructor.
        ~BindingIterator() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction Operators

        /// @brief Copy assignment operator.
        /// @param Other The other iterator to be copied.
        /// @return Returns a reference to this.
        SelfType& operator=(const SelfType& Other) = default;
        /// @brief Move assignment operator.
        /// @param Other The other iterator to be moved.
        /// @return Returns a reference to this.
        SelfType& operator=(SelfType&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Comparison Operators

        /// @brief Equality comparison operator.
        /// @param Other The other iterator to be compared to.
        /// @return Returns true if the two iterators are pointing to the same element, false otherwise.
        Boole operator==(const SelfType& Other)
            { return this->Value = Other.Value; }
        /// @brief Inequality comparison operator.
        /// @param Other The other iterator to be compared to.
        /// @return Returns true if the two iterators are pointing to different elements, false otherwise.
        Boole operator!=(const SelfType& Other)
            { return this->Value != Other.Value; }

        ///////////////////////////////////////////////////////////////////////////////
        // Access Operators

        /// @brief Reference dereference operator.
        /// @return Returns a reference to the interface in the binding pointed to by this iterator.
        SubscriberType& operator*()
            { return EventHelper::ToReference( (*this->Value)->GetSubscriber() ); }
        /// @brief Pointer dereference operator.
        /// @return Returns a pointer to the interface in the binding pointed to by this iterator.
        SubscriberType* operator->()
            { return EventHelper::ToPointer( (*this->Value)->GetSubscriber() ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Manipulation Operators

        /// @brief Pre-increment operator.
        void operator++()
            { this->Value++; }
        /// @brief Pre-decrement operator.
        void operator--()
            { this->Value--; }
        /// @brief Post-increment operator.
        /// @return Returns an iterator to the value held prior to this operation.
        SelfType operator++(int)
        {
            SelfType Ret(this);
            ++(this->Value);
            return Ret;
        }
        /// @brief Post-decrement operator.
        /// @return Returns an iterator to the value held prior to this operation.
        SelfType operator--(int)
        {
            SelfType Ret(this);
            --(this->Value);
            return Ret;
        }
    };//BindingIterator

    template<class TableType, SubscriberContainerType ContainerType>
    class EventSubscriptionContainer
    {  };

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a container class for the storage of multiple sorted subscribers.
    ///////////////////////////////////////
    template<class TableType, SubscriberContainerType::SCT_Unsorted>
    class MEZZ_LIB EventSubscriptionContainer
    {
    public:
        /// @brief Convenience type for the EventSubscriptionContainer implementation being used.
        using SelfType = EventSubscriptionContainer<TableType,SubscriberContainerType::SCT_Unsorted>;
        /// @brief Convenience type and check for what exactly will be stored by this container.
        using StoredType = typename TableType::StoredType;
        /// @brief Convenience type for the internal container storing the subscriptions.
        using InternalContainerType = std::vector<StoredType>;
    public:
        /// @brief Blank constructor.
        EventSubscriptionContainer() = default;
        /// @brief Copy constructor.
        /// @param Other The other table to NOT be copied.
        EventSubscriptionContainer(const EventSubscriptionContainer& Other) = delete;
        /// @brief Move constructor.
        /// @param Other The other table to be moved.
        EventSubscriptionContainer(EventSubscriptionContainer&& Other) = default;
        /// @brief Class destructor.
        virtual ~EventSubscriptionContainer() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other table to NOT be copied.
        /// @return Returns a reference to this.
        EventSubscriptionContainer& operator=(const EventSubscriptionContainer& Other) = delete;
        /// @brief Move assignment operator.
        /// @param Other The other table to be moved.
        /// @return Returns a reference to this.
        EventSubscriptionContainer& operator=(EventSubscriptionContainer&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility
    };//EventSubscriptionContainer

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a container class for the storage of multiple sorted subscribers.
    ///////////////////////////////////////
    template<class TableType, SubscriberContainerType::SCT_Unsorted_Fixed>
    class MEZZ_LIB EventSubscriptionContainer
    {
    public:
        /// @brief Convenience type for the EventSubscriptionContainer implementation being used.
        using SelfType = EventSubscriptionContainer<TableType,SubscriberContainerType::SCT_Unsorted_Fixed>;
        /// @brief Convenience type and check for what exactly will be stored by this container.
        using StoredType = typename TableType::StoredType;
        /// @brief Convenience type for the internal container storing the subscriptions.
        using InternalContainerType = ManagedArray<StoredType,typename TableType::TableTraits::StorageCount>;
    public:
        /// @brief Blank constructor.
        EventSubscriptionContainer() = default;
        /// @brief Copy constructor.
        /// @param Other The other table to NOT be copied.
        EventSubscriptionContainer(const EventSubscriptionContainer& Other) = delete;
        /// @brief Move constructor.
        /// @param Other The other table to be moved.
        EventSubscriptionContainer(EventSubscriptionContainer&& Other) = default;
        /// @brief Class destructor.
        virtual ~EventSubscriptionContainer() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other table to NOT be copied.
        /// @return Returns a reference to this.
        EventSubscriptionContainer& operator=(const EventSubscriptionContainer& Other) = delete;
        /// @brief Move assignment operator.
        /// @param Other The other table to be moved.
        /// @return Returns a reference to this.
        EventSubscriptionContainer& operator=(EventSubscriptionContainer&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility
    };//EventSubscriptionContainer

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a container class for the storage of multiple unsorted subscribers.
    ///////////////////////////////////////
    template<class TableType, SubscriberContainerType::SCT_Sorted>
    class MEZZ_LIB EventSubscriptionContainer
    {
    public:
        /// @brief Convenience type for the EventSubscriptionContainer implementation being used.
        using SelfType = EventSubscriptionContainer<TableType,SubscriberContainerType::SCT_Sorted>;
        /// @brief Convenience type and check for what exactly will be stored by this container.
        using StoredType = typename TableType::StoredType;
        /// @brief The function to use for sorting this container.
        using StoragePredicate = typename TableType::TableTraits::StoragePredicate;
        /// @brief Convenience type for the internal container storing the subscriptions.
        using InternalContainerType = SortedVector<StoredType,StoragePredicate>;
    public:
        /// @brief Blank constructor.
        EventSubscriptionContainer() = default;
        /// @brief Copy constructor.
        /// @param Other The other table to NOT be copied.
        EventSubscriptionContainer(const EventSubscriptionContainer& Other) = delete;
        /// @brief Move constructor.
        /// @param Other The other table to be moved.
        EventSubscriptionContainer(EventSubscriptionContainer&& Other) = default;
        /// @brief Class destructor.
        virtual ~EventSubscriptionContainer() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other table to NOT be copied.
        /// @return Returns a reference to this.
        EventSubscriptionContainer& operator=(const EventSubscriptionContainer& Other) = delete;
        /// @brief Move assignment operator.
        /// @param Other The other table to be moved.
        /// @return Returns a reference to this.
        EventSubscriptionContainer& operator=(EventSubscriptionContainer&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility
    };//EventSubscriptionContainer

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a container class for the storage of multiple sorted subscribers.
    ///////////////////////////////////////
    template<class TableType, SubscriberContainerType::SCT_Sorted_Fixed>
    class MEZZ_LIB EventSubscriptionContainer
    {
    public:
        /// @brief Convenience type for the EventSubscriptionContainer implementation being used.
        using SelfType = EventSubscriptionContainer<TableType,SubscriberContainerType::SCT_Sorted_Fixed>;
        /// @brief Convenience type and check for what exactly will be stored by this container.
        using StoredType = typename TableType::StoredType;
        /// @brief The function to use for sorting this container.
        using StoragePredicate = typename TableType::TableTraits::StoragePredicate;
        /// @brief Convenience type for the internal container storing the subscriptions.
        using InternalContainerType = SortedManagedArray<StoredType,typename TableType::TableTraits::StorageCount,StoragePredicate>;
    public:
        /// @brief Blank constructor.
        EventSubscriptionContainer() = default;
        /// @brief Copy constructor.
        /// @param Other The other table to NOT be copied.
        EventSubscriptionContainer(const EventSubscriptionContainer& Other) = delete;
        /// @brief Move constructor.
        /// @param Other The other table to be moved.
        EventSubscriptionContainer(EventSubscriptionContainer&& Other) = default;
        /// @brief Class destructor.
        virtual ~EventSubscriptionContainer() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other table to NOT be copied.
        /// @return Returns a reference to this.
        EventSubscriptionContainer& operator=(const EventSubscriptionContainer& Other) = delete;
        /// @brief Move assignment operator.
        /// @param Other The other table to be moved.
        /// @return Returns a reference to this.
        EventSubscriptionContainer& operator=(EventSubscriptionContainer&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility
    };//EventSubscriptionContainer

    /// @}
}//Mezzanine

#endif
