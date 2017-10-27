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
    ///////////////////////////////////////
    template<typename Interface>
    class MEZZ_LIB EventSubscriberBinding
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
        SubscriberType Callable;
    public:
        /// @brief Descriptive constructor.
        /// @param ID The unique identifier for the subscriber/delegate.
        /// @param Sub The observer to dispatch the event to.
        EventSubscriberBinding(const SubscriberType Sub) :
            Callable(Sub)
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

        /// @brief Copy constructor.
        /// @param Other The other binding to not be copied.
        /// @return Returns a reference to this.
        EventSubscriberBinding& operator=(const EventSubscriberBinding& Other) = delete;
        /// @brief Move assignment operator.
        /// @param Other The other binding to be moved.
        /// @return Returns a reference to this.
        EventSubscriberBinding& operator=(EventSubscriberBinding&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the delegate that is called when the subscribed event is dispatched.
        /// @return Returns a pointer or reference (depending on implementation) of the stored subscriber.
        SubscriberRet GetCallable()
            { return this->Callable; }
        /// @brief Gets the unique identifier of the subscriber.
        /// @return Returns an ID that uniquely identifies the subscriber in the subscription table.
        SubscriberIDType GetID() const
            { return EventHelper::ToPointer( this->Callable )->GetID(); }

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
            { ( EventHelper::ToPointer(this->Callable)->*Funct)( std::forward<ArgTypes>(Args)... ); }
    };//EventSubscriberBinding

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
        using SubscriberType = Interface;
        /// @brief The type to use for uniquely identifying instances of subscribers.
        using SubscriberIDType = typename Interface::IDType;
    protected:
        /// @brief A pointer to the EventSubscriberTable we are subscribed to.
        TableType* EventTable;
    public:
        /// @brief Class constructor.
        /// @param ID The unique identifier for the subscriber/delegate.
        /// @param Observer The callback to dispatch the event to.
        /// @param Table A pointer to the table dispatching the interested event.
        EventSubscriberBindingImpl(const SubscriberType Observer, TableType* Table) :
            EventSubscriberBinding<SubscriberType>(Observer),
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
            { this->EventTable = nullptr; }
        /// @brief Notifies this binding of an updated address for the table.
        /// @param ToUpdate An updated pointer to the subscription table that can be used.
        void UpdateTable(TableType* ToUpdate)
            { if( this->IsSubscribed() ) this->EventTable = ToUpdate; }
    };//EventSubscriberBindingImpl

    /// @brief Convenience type for passing around EventSubscriberBindings.
    template<typename Interface>
    using EventSubscriberBindingPtr = std::shared_ptr< EventSubscriberBinding<Interface> >;

    /// @}
}//Mezzanine

#endif
