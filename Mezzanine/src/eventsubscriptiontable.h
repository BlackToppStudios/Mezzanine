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
    /// @brief This is the base class for any class that generates and publishes events to subscribers.
    /// @tparam TableType The type of table this binding will be bound to.
    /// @tparam Interface The type of interface/subscriber this binding will be bound to.
    ///////////////////////////////////////
    template<class TableType, class Interface>
    class MEZZ_LIB EventSubscriberBindingImpl : public EventSubscriberBinding<Interface>
    {
    public:
        /// @brief Convenience type for describing the type of "this".
        using SelfType = EventSubscriberBindingImpl<TableType,Interface>;
        /// @brief Retrievable type for querying the type of callable interface this table works with.
        using InterfaceType = Interface;
        /// @brief The type to use for uniquely identifying instances of subscribers.
        using InterfaceID = typename Interface::IDType;
    protected:
        /// @brief A pointer to the EventSubscriberTable we are subscribed to.
        TableType* EventTable;
    public:
        /// @brief Class constructor.
        /// @param ID The unique identifier for the subscriber/delegate.
        /// @param Observer The callback to dispatch the event to.
        /// @param Table A pointer to the table dispatching the interested event.
        /// @param Hash The hash of the event name this binding is subscribed to.
        EventSubscriberBindingImpl(InterfaceID ID, const InterfaceType Observer, TableType* Table, const EventHashType Hash) :
            EventSubscriberBinding<InterfaceType>(ID,Observer,Hash),
            EventTable(Table)
            {  }
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
        virtual Boole IsSubscribed() const
            { return ( this->EventTable != nullptr ); }
        /// @brief Removes the subscriber from the list of interested recipients on the publisher.
        virtual void Unsubscribe()
        {
            this->EventTable->Unsubscribe( this->GetID() );
            this->Unbind();
        }
        /// @brief Removes all references to an Event and/or Publisher from this binding.
        /// @remarks This method is called by Unsubscribe, and should never need to be called manually.
        /// This method also makes zero attempt to notify the publisher of it's changed state.  For that
        /// you should call Unsubscribe.
        virtual void Unbind()
        {
            this->EventTable = nullptr;
            this->NameHash = EventNameType::EmptyHash;
        }
        /// @brief Notifies this binding of an updated address for the table.
        /// @param ToUpdate An updated pointer to the subscription table that can be used.
        void UpdateTable(TableType* ToUpdate)
            { this->EventTable = ToUpdate; }
    };//EventSubscriberBindingImpl

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This class represents a given event that can be subscribed to and/or fired.
    ///////////////////////////////////////
    template<class Interface>
    class MEZZ_LIB EventBindingTable
    {
    public:
        /// @brief Convenience type for describing the type of this.
        using SelfType = EventBindingTable<Interface>;
        /// @brief Retrievable type for querying the type of callable interface this table works with.
        using InterfaceType = Interface;
        /// @brief The type to use for uniquely identifying instances of subscribers.
        using InterfaceID = typename Interface::IDType;
        /// @brief Convenience type for the base binding class that is returned.
        using BindingType = EventSubscriberBindingPtr<Interface>;
        /// @brief Convenience type for the actual binding implementation used by this publisher.
        using BindingImplType = EventSubscriberBindingImpl<SelfType,InterfaceType>;
        /// @brief Convenience type for passing the subscriber as an argument to the Subscribe method.
        using SubscribeArg = typename std::conditional<std::is_pointer<Interface>::value,Interface,Interface&>::type;
        /// @brief Convenience type for the return value of the Subscribe method.
        using SubscribeRet = BindingType;
        /// @brief Convenience type and check for what exactly will be stored by this subscription table.
        using StoredType = BindingType;
        /// @brief Container for the storage of bindings between subscribers and the events they are interested in.
        using StorageContainer = std::vector<StoredType>;
        /// @brief Iterator type for subscriber bindings stored by this table.
        using StorageIterator = typename StorageContainer::iterator;
        /// @brief Const Iterator type for subscriber bindings stored by this table.
        using ConstStorageIterator = typename StorageContainer::const_iterator;
    protected:
        /// @brief A container of all the subscriber bindings to this event table.
        StorageContainer Subscribers;
        /// @brief The hash of the Event the subscribers in this table are subscribed to.
        EventHashType EventHash;

        /// @brief One of two helper functions to ensure a common means of dereference.
        /// @param ToConvert The reference to convert.
        /// @return Returns a pointer of the templated type.
        template<class AnyType>
        static AnyType* ToPointer(AnyType& ToConvert)
            { return &ToConvert; }
        /// @brief One of two helper functions to ensure a common means of dereference.
        /// @param ToConvert The reference to convert.
        /// @return Returns a pointer of the templated type.
        template<class AnyType>
        static AnyType* ToPointer(AnyType* ToConvert)
            { return ToConvert; }

        /// @brief Updates the address of this table in each subscriber this table is tracking.
        void UpdateTableAddresses()
        {
            for( StoredType CurrBinding : this->Subscribers )
            {
                std::shared_ptr<BindingImplType> Casted = std::static_pointer_cast<BindingImplType>(CurrBinding);
                Casted->UpdateTable(this);
            }
        }
    public:
        /// @brief Blank constructor.
        EventBindingTable() = delete;
        /// @brief Copy constructor.
        /// @param Other The other table to NOT be copied.
        EventBindingTable(const SelfType& Other) = delete;
        /// @brief Move constructor.
        /// @param Other The other table to be moved.
        EventBindingTable(SelfType&& Other)
        {
            this->Subscribers = std::move(Other.Subscribers);
            this->EventHash = std::move(Other.EventHash);
            this->UpdateTableAddresses();
        }
        /// @brief Class constructor.
        /// @param Hash The generated hash to use to identify this event.
        EventBindingTable(const EventHashType Hash) :
            EventHash(Hash)
            {  }
        /// @brief Class destructor.
        ~EventBindingTable()
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
        SelfType& operator=(SelfType&& Other)
        {
            this->Subscribers = std::move(Other.Subscribers);
            this->EventHash = std::move(Other.EventHash);
            this->UpdateTableAddresses();
            return *this;
        }

        /// @brief Less-than operator.
        /// @param Other The other table to be compared to.
        /// @return Returns true if this table is considered less than the other, or should be sorted before the other table.
        Boole operator<(const SelfType& Other) const
            { return this->EventHash < Other.EventHash; }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the hash of the event associated with this table.
        /// @return Returns the hash identifying this event.
        EventHashType GetHash() const
            { return this->EventHash; }

        ///////////////////////////////////////////////////////////////////////////////
        // Subscription Management

        /// @brief Adds a subscriber to this event table.
        /// @exception If the ID of the Sub is already being used by another binding/subscriber this will throw a "II_DUPLICATE_IDENTITY_EXCEPTION".
        /// @param Sub The subscriber to be called when the interested event is fired.
        /// @return Returns a pointer to the created Subscriber slot for the provided subscriber.
        SubscribeRet Subscribe(const SubscribeArg Sub)
        {
            EventSubscriberBindingPtr<Interface> NewSubscriber = this->GetBinding( ToPointer(Sub)->GetID() );
            if( NewSubscriber.use_count() > 0 ) {
                MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,"A subscriber with that ID already exists!");
            }
            NewSubscriber = std::make_shared<BindingImplType>(Sub,this);
            this->Subscribers.push_back(NewSubscriber);
            return NewSubscriber;
        }
        /// @brief Gets a binding by the subscriber ID.
        /// @param ID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        /// @return Returns the binding with the specified ID, or nullptr of none exists.
        EventSubscriberBindingPtr<Interface> GetBinding(InterfaceID ID) const
        {
            for( ConstStorageIterator SubIt = this->Subscribers.begin() ; SubIt != this->Subscribers.end() ; ++SubIt )
            {
                if( (*SubIt)->GetSubID() == ID ) {
                    return (*SubIt);
                }
            }
            return nullptr;
        }

        /// @brief Removes a single subscriber from this event table.
        /// @param ID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        void Unsubscribe(InterfaceID ID)
        {
            for( StorageIterator SubIt = this->Subscribers.begin() ; SubIt != this->Subscribers.end() ; ++SubIt )
            {
                if( (*SubIt)->GetID() == ID ) {
                    (*SubIt)->Unbind();
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
            for( StorageIterator SubIt = this->Subscribers.begin() ; SubIt != this->Subscribers.end() ; ++SubIt )
                { (*SubIt)->Unbind(); }
            this->Subscribers.clear();
            return RemoveCount;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @internal
        /// @brief Notifies all subscribers of this event that this event is firing.
        /// @param Args The arguments and extra data related to this event.
        void DispatchEvent(EventPtr Args) const
        {
            for( ConstStorageIterator SubIt = this->Subscribers.begin() ; SubIt != this->Subscribers.end() ; ++SubIt )
                { (*SubIt)->DispatchEvent(Args); }
        }
    };//EventBindingTable

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This class represents a given event that can be subscribed to and/or fired.
    ///////////////////////////////////////
    template<class Interface>
    class MEZZ_LIB EventSubscriptionTable
    {
    public:
        /// @brief Convenience type for describing the type of this.
        using SelfType = EventSubscriptionTable<Interface>;
        /// @brief Retrievable type for querying the type of callable interface this table works with.
        using InterfaceType = Interface;
        /// @brief The type to use for uniquely identifying instances of subscribers.
        using InterfaceID = typename Interface::IDType;
        /// @brief Convenience type for passing the subscriber as an argument to the Subscribe method.
        using SubscribeArg = typename std::conditional<std::is_pointer<Interface>::value,Interface,Interface&>::type;
        /// @brief Convenience type for the return value of the Subscribe method.
        using SubscribeRet = void;
        /// @brief Convenience type and check for what exactly will be stored by this subscription table.
        using StoredType = Interface;
        /// @brief Container for the storage of bindings between subscribers and the events they are interested in.
        using StorageContainer = std::vector<StoredType>;
        /// @brief Iterator type for subscriber bindings stored by this table.
        using StorageIterator = typename StorageContainer::iterator;
        /// @brief Const Iterator type for subscriber bindings stored by this table.
        using ConstStorageIterator = typename StorageContainer::const_iterator;
    protected:
        /// @brief A container of all the subscriber bindings to this event table.
        StorageContainer Subscribers;
        /// @brief The hash of the Event the subscribers in this table are subscribed to.
        EventHashType EventHash;

        /// @brief One of two helper functions to ensure a common means of dereference.
        /// @param ToConvert The reference to convert.
        /// @return Returns a pointer of the templated type.
        template<class AnyType>
        static AnyType* ToPointer(AnyType& ToConvert)
            { return &ToConvert; }
        /// @brief One of two helper functions to ensure a common means of dereference.
        /// @param ToConvert The reference to convert.
        /// @return Returns a pointer of the templated type.
        template<class AnyType>
        static AnyType* ToPointer(AnyType* ToConvert)
            { return ToConvert; }
    public:
        /// @brief Blank constructor.
        EventSubscriptionTable() = delete;
        /// @brief Copy constructor.
        /// @param Other The other table to NOT be copied.
        EventSubscriptionTable(const SelfType& Other) = delete;
        /// @brief Move constructor.
        /// @param Other The other table to be moved.
        EventSubscriptionTable(SelfType&& Other) = default;
        /// @brief Class constructor.
        /// @param Hash The generated hash to use to identify this event.
        EventSubscriptionTable(const EventHashType Hash) :
            EventHash(Hash)
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
            { return this->EventHash < Other.EventHash; }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the hash of the event associated with this table.
        /// @return Returns the hash identifying this event.
        EventHashType GetHash() const
            { return this->EventHash; }

        ///////////////////////////////////////////////////////////////////////////////
        // Subscription Management

        /// @brief Adds a subscriber to this event table.
        /// @exception If the ID of the Sub is already being used by another binding/subscriber this will throw a "II_DUPLICATE_IDENTITY_EXCEPTION".
        /// @param Sub The subscriber to be called when the interested event is fired.
        /// @return Returns a pointer to the created Subscriber slot for the provided subscriber.
        SubscribeRet Subscribe(const SubscribeArg Sub)
        {

            for( StorageIterator StorIt : this->Subscribers )
            {
                if( (*StorIt)->GetId() == ToPointer(Sub)->GetID() ) {
                    MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,"A subscriber with that ID already exists!");
                }
            }
            this->Subscribers.push_back(Sub);
            return;
        }
        /// @brief Gets a binding by the subscriber ID.
        /// @param ID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        /// @return Returns the binding with the specified ID, or nullptr of none exists.
        InterfaceType GetSubscriber(InterfaceID ID) const
        {
            for( ConstStorageIterator SubIt = this->Subscribers.begin() ; SubIt != this->Subscribers.end() ; ++SubIt )
            {
                if( (*SubIt)->GetSubID() == ID ) {
                    return (*SubIt);
                }
            }
            return nullptr;
        }

        /// @brief Removes a single subscriber from this event table.
        /// @param ID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        void Unsubscribe(InterfaceID ID)
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
        /// @param Args The arguments and extra data related to this event.
        void DispatchEvent(EventPtr Args) const
        {
            for( ConstStorageIterator SubIt = this->Subscribers.begin() ; SubIt != this->Subscribers.end() ; ++SubIt )
                { (*SubIt)->DispatchEvent(Args); }
        }
    };//EventSubscriptionTable

    /// @}
}//Mezzanine

#endif
