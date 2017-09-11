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
    /// @brief This class represents a given event that can be subscribed to and/or fired.
    ///////////////////////////////////////
    template<class Interface, Boole UseBinding>
    class MEZZ_LIB EventSubscriptionTable
    {
    public:
        /// @brief Convenience type for describing the type of this.
        using SelfType = EventSubscriptionTable<Interface,UseBinding>;
        /// @brief Retrievable type for querying the type of callable interface this table works with.
        using InterfaceType = Interface;
        /// @brief Convenience type for passing the subscriber as an argument to the Subscribe method.
        using SubscribeArg = typename std::conditional<std::is_pointer<Interface>::value,Interface,Interface&>::type;
        /// @brief Convenience type for the return value of the Subscribe method.
        using SubscribeRet = typename std::conditional<UseBinding,EventSubscriberBindingPtr<Interface>,void>::type;
        /// @brief Convenience type and check for what exactly will be stored by this subscription table.
        using StoredType = typename std::conditional<UseBinding,EventSubscriberBindingPtr<Interface>,Interface>::type;
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
    public:
        /// @brief Blank constructor.
        EventSubscriptionTable() = delete;
        /// @brief Copy constructor.
        /// @param Other The other table to be copied.
        EventSubscriptionTable(const SelfType& Other) = default;
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
        /// @param Other The other table to be copied.
        /// @return Returns a reference to this.
        SelfType& operator=(const SelfType& Other) = default;
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
        /// @exception If ID provided is already being used by a binding/subscriber this will throw a "II_DUPLICATE_IDENTITY_EXCEPTION".
        /// @param ID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        /// @param Sub The subscriber to be called when the interested event is fired.
        /// @return Returns a pointer to the created Subscriber slot for the provided subscriber.
        template<class PublisherType>
        SubscribeRet Subscribe(EventSubscriberID ID, const SubscribeArg Sub, PublisherType* Pub)
        {
            if( UseBinding ) {
                EventSubscriberBindingPtr<Interface> NewSubscriber = this->GetBinding(ID);
                if( NewSubscriber.use_count() > 0 ) {
                    MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,"A subscriber with that ID already exists!");
                }
                NewSubscriber = std::make_shared< PublisherType::BindingImplType >( ID,Sub,Pub,this->EventHash );
                this->Subscribers.push_back(NewSubscriber);
                return NewSubscriber;
            }else{
                for( StorageIterator StorIt : this->Subscribers )
                {
                    if( (*StorIt)->GetSubId() == ID ) {
                        MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,"A subscriber with that ID already exists!");
                    }
                }
                this->Subscribers.push_back(Sub);
                return;
            }
        }
        /// @brief Gets a binding by the subscriber ID.
        /// @param ID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        /// @return Returns the binding with the specified ID, or nullptr of none exists.
        EventSubscriberBindingPtr<Interface> GetBinding(EventSubscriberID ID) const
        {
            if( UseBinding ) {
                for( ConstStorageIterator SubIt = this->Subscribers.begin() ; SubIt != this->Subscribers.end() ; ++SubIt )
                {
                    if( (*SubIt)->GetSubID() == ID ) {
                        return (*SubIt);
                    }
                }
            }
            return nullptr;
        }

        /// @brief Removes a single subscriber from this event table.
        /// @param ID The unique ID of the subscriber.  Must be unique among the IDs of this publisher.
        void Unsubscribe(EventSubscriberID ID)
        {
            for( StorageIterator SubIt = this->Subscribers.begin() ; SubIt != this->Subscribers.end() ; ++SubIt )
            {
                if( (*SubIt)->GetSubID() == ID ) {
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
    };//EventSubscriptionTable

    /// @}
}//Mezzanine

#endif
