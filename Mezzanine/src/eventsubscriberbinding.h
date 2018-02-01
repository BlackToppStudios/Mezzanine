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
#ifndef _eventsubscriberbinding_h
#define _eventsubscriberbinding_h

#include <functional>
#include <memory>

#include "event.h"
#include "eventhelper.h"
#include "eventsubscriberid.h"

namespace Mezzanine
{
    /// @addtogroup Events
    /// @{

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This class represents a given event that can be subscribed to and/or fired.
    /// @tparam Interface The interface that this binding will track the event subscription lifetime of.
    /// @pre Interface is required to have a type labeled "IDType" defined.  This type should be equality comparable(==,!=).
    ///////////////////////////////////////
    template<typename Interface>
    class EventSubscriberBinding
    {
    public:
        /// @brief Retrievable type for querying the type of callable interface this table works with.
        using SubscriberType = Interface;
        /// @brief The type to use for uniquely identifying instances of subscribers.
        using SubscriberIDType = typename Interface::IDType;
        /// @brief Convenience type for retrieving the subscriber.
        using SubscriberRet = typename std::conditional<std::is_pointer<SubscriberType>::value,SubscriberType,SubscriberType&>::type;
    protected:
        /// @brief The delegate that will be called (if valid) when a desired event is fired.
        SubscriberType Sub;
    public:
        /// @brief Descriptive constructor.
        /// @param BoundSub The observer to dispatch the event to.
        EventSubscriberBinding(const SubscriberType BoundSub) :
            Sub(BoundSub)
            {  }
        /// @brief Copy constructor.
        /// @param Other The other binding to not be copied.
        EventSubscriberBinding(const EventSubscriberBinding& Other) = delete;
        /// @brief Move constructor.
        /// @param Other The other binding to be moved.
        EventSubscriberBinding(EventSubscriberBinding&& Other) = default;
        /// @brief Class destructor.
        virtual ~EventSubscriberBinding() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Copy Assignment Operator.
        /// @param Other The other binding to not be copied.
        /// @return Returns a reference to this.
        EventSubscriberBinding& operator=(const EventSubscriberBinding& Other) = delete;
        /// @brief Move Assignment Operator.
        /// @param Other The other binding to be moved.
        /// @return Returns a reference to this.
        EventSubscriberBinding& operator=(EventSubscriberBinding&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the delegate that is called when the subscribed event is dispatched.
        /// @return Returns a pointer or reference (depending on implementation) of the stored subscriber.
        SubscriberRet GetSubscriber()
            { return this->Sub; }
        /// @brief Gets the unique identifier of the subscriber.
        /// @return Returns an ID that uniquely identifies the subscriber in the subscription table.
        SubscriberIDType GetID() const
            { return EventHelper::ToPointer( this->Sub )->GetID(); }

        /// @brief Check if this binding is still valid.
        /// @return Returns true of the subscriber can still get events from the publisher, false otherwise.
        virtual Boole IsSubscribed() const = 0;
        /// @brief Removes the subscriber from the list of interested recipients on the publisher.
        virtual void Unsubscribe() = 0;
        /// @brief Removes all references to an Event and/or Publisher from this binding.
        /// @remarks This method is called by Unsubscribe, and should never need to be called manually.
        /// This method also makes zero attempt to notify the publisher of it's changed state.  For that
        /// you should call Unsubscribe.
        virtual void Unbind() = 0;

        /// @brief Dispatches an event to the appropriate subscriber in this binding.
        /// @param Args The arguments and extra data related to this event.
        template<class MemberFunct, class... ArgTypes>
        void DispatchEvent(MemberFunct Funct, ArgTypes&&... Args) const
            { (EventHelper::ToPointer(this->Sub)->*Funct)( std::forward<ArgTypes>(Args)... ); }
    };//EventSubscriberBinding

    /// @brief Convenience type for passing around EventSubscriberBinding instances wrapped in a shared ptr.
    template<typename Interface>
    using EventSubscriberBindingPtr = std::shared_ptr< EventSubscriberBinding<Interface> >;

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is the base class for any class that generates and publishes events to subscribers.
    /// @tparam TableType The type of table this binding will be bound to.
    /// @tparam Interface The type of interface/subscriber this binding will be bound to.
    /// @pre TableType is required to have a method with the signature "Unsubscribe(Interface::IDType)".
    /// @pre Interface is required to have a type labeled "IDType" defined.  This type should be equality comparable(==,!=).
    ///////////////////////////////////////
    template<class TableType, class Interface>
    class MEZZ_LIB EventSubscriberBindingImpl : public EventSubscriberBinding<Interface>
    {
    public:
        /// @brief Convenience type for describing the type of "this".
        using SelfType = EventSubscriberBindingImpl<TableType,Interface>;
        /// @brief Retrievable type for the binding base class this binding inherits from.
        using BaseBindingType = EventSubscriberBinding<Interface>;
        /// @brief Retrievable type for the binding base class wrapped in a shared_ptr.
        using BaseBindingPtrType = EventSubscriberBindingPtr<Interface>;
        /// @brief Retrievable type for querying the type of callable interface this table works with.
        using SubscriberType = Interface;
        /// @brief The type to use for uniquely identifying instances of subscribers.
        using SubscriberIDType = typename Interface::IDType;
    protected:
        /// @brief A pointer to the EventSubscriberTable we are subscribed to.
        TableType* EventTable;
    public:
        /// @brief Class constructor.
        /// @param Sub The subscriber to dispatch the event to.
        /// @param Table A pointer to the table dispatching the interested event.
        EventSubscriberBindingImpl(const SubscriberType BoundSub, TableType* Table) :
            EventSubscriberBinding<SubscriberType>(BoundSub),
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

        /// @brief Gets the table this binding is bound to.
        /// @return Returns a pointer to the table this binding was created by.
        TableType* GetTable() const
            { return this->EventTable; }

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
            { this->EventTable = nullptr; }
        /// @brief Notifies this binding of an updated address for the table.
        /// @param ToUpdate An updated pointer to the subscription table that can be used.
        void UpdateTable(TableType* ToUpdate)
            { this->EventTable = ToUpdate; }
    };//EventSubscriberBindingImpl

    /// @brief Convenience type for passing around EventSubscriberBindingImpl instances wrapped in a shared ptr.
    template<typename TableType, typename Interface>
    using EventSubscriberBindingImplPtr = std::shared_ptr< EventSubscriberBindingImpl<TableType,Interface> >;

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a convenience struct designed to create conversion compatibility between bindings and the subscription containers.
    /// @tparam BindingPtrType The type of binding this storable will provide storage and conversions for.
    /// @pre BindingPtrType is required to have a type with "SubscribeRet" type specified and a "GetSubscriber()" method returning "SubscribeRet".
    ///////////////////////////////////////
    template<class BindingPtrType>
    struct MEZZ_LIB EventSubscriberBindingStorable
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Types Used

        /// @brief Convenience type for this storage type.
        using SelfType = EventSubscriberBindingStorable<BindingPtrType>;
        /// @brief Convenience type for the binding behind the shared_ptr.
        using BindingType = typename BindingPtrType::element_type;
        /// @brief Convenience type for the base class the binding inherits from wrapped in a shared_ptr.
        using BaseBindingPtrType = typename BindingType::BaseBindingPtrType;
        /// @brief The type to use for uniquely identifying instances of subscribers.
        using SubscriberIDType = typename BindingType::SubscriberIDType;
        /// @brief Convenience type for what is returned when retrieving the subscriber.
        using SubscriberRet = typename BindingType::SubscriberRet;

        ///////////////////////////////////////////////////////////////////////////////
        // Data Members

        /// @brief The binding this storable wraps.
        BindingPtrType StoredBinding;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Blank constructor.
        EventSubscriberBindingStorable() = default;
        /// @brief Binding init constructor.
        EventSubscriberBindingStorable(BindingPtrType Bind) :
            StoredBinding(Bind)
            {  }
        /// @brief Copy constructor.
        /// @param Other The other binding storable to copy.
        EventSubscriberBindingStorable(const SelfType& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other binding storable to move.
        EventSubscriberBindingStorable(SelfType&& Other) = default;
        /// @brief Class destructor.
        ~EventSubscriberBindingStorable() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Copy assignment operator.
        /// @param Other The other binding storable to copy.
        /// @return Returns a reference to this.
        SelfType& operator=(const SelfType& Other) = default;
        /// @brief Move assignment operator.
        /// @param Other The other binding storable to move.
        /// @return Returns a reference to this.
        SelfType& operator=(SelfType&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the unique identifier of the subscriber.
        /// @return Returns an ID that uniquely identifies the subscriber in the subscription table.
        SubscriberIDType GetID() const
            { return this->StoredBinding->GetID(); }

        /// @brief Conversion to base binding operator.
        /// @return Returns the binding base of the subscription.
        operator BaseBindingPtrType() const
            { return this->StoredBinding; }
        /// @brief Conversion to binding operator.
        /// @return Returns the binding implementation of the subscription.
        operator BindingPtrType() const
            { return this->StoredBinding; }
        /// @brief Conversion to subscriber operator.
        /// @return Returns the subscriber according to the SubscribeRet type expected on the binding type.
        operator SubscriberRet() const
            { return this->StoredBinding->GetSubscriber(); }
    };//EventSubscriberBindingStorable

    /// @}
}//Mezzanine

#endif
