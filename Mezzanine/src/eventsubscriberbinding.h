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
#include "eventsubscriberid.h"

namespace Mezzanine
{
    /// @addtogroup Events
    /// @{

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This class represents a given event that can be subscribed to and/or fired.
    ///////////////////////////////////////
    template<typename InterfaceType>
    class MEZZ_LIB EventSubscriberBinding
    {
    public:
        /// @brief Convenience type for the callbacks that will be called when events are fired.
        using CallbackType = std::function< void(EventPtr) >;
    protected:
        /// @brief The delegate that will be called (if valid) when a desired event is fired.
        InterfaceType Callable;
        /// @brief The unique identifier for the subscriber/delegate.
        EventSubscriberID SubID;
        /// @brief The hash of the event name this binding is subscribed to.
        EventHashType NameHash;
    public:
        /// @brief Descriptive constructor.
        /// @param ID The unique identifier for the subscriber/delegate.
        /// @param Delegate The callback to dispatch the event to.
        /// @param Hash The hash of the event name this binding is subscribed to.
        EventSubscriberBinding(EventSubscriberID ID, const InterfaceType Delegate, const EventHashType Hash) :
            Callable(Delegate),
            SubID(ID),
            NameHash(Hash)
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
        /// @return Returns a const reference to the delegate bound to the subscribed event.
        const InterfaceType GetCallable() const
            { return this->Callable; }
        /// @brief Gets the unique identifier of the subscriber.
        /// @return Returns a SubscriberID that uniquely identifies the subscription in the publisher.
        EventSubscriberID GetSubID() const
            { return this->SubID; }
        /// @brief Gets the hash of the event this binding is bound to.
        /// @return Returns the hash for the event name this binding is subscribed to.
        EventHashType GetEventHash() const
            { return this->NameHash; }

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
        void DispatchEvent(EventPtr Args) const
            { this->Callable(Args); }
    };//EventSubscriberBinding

    /// @brief Convenience type for passing around EventSubscriberBindings.
    template<typename Interface>
    using EventSubscriberBindingPtr = std::shared_ptr< EventSubscriberBinding<Interface> >;

    /// @}
}//Mezzanine

#endif
