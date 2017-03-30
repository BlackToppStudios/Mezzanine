// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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

    class EventPublisher;
	///////////////////////////////////////////////////////////////////////////////
    /// @brief This class represents a given event that can be subscribed to and/or fired.
    ///////////////////////////////////////
    class MEZZ_LIB EventSubscriberBinding
    {
    public:
        /// @brief Convenience type for the callbacks that will be called when events are fired.
        using CallbackType = std::function< void(EventPtr) >;
    protected:
        /// @brief The delegate that will be called (if valid) when a desired event is fired.
        CallbackType Callback;
        /// @brief The unique identifier for the subscriber/delegate.
        EventSubscriberID SubID;
        /// @brief A pointer to the publisher dispatching the interested event.
        EventPublisher* Publisher;
        /// @brief The hash of the event name this binding is subscribed to.
        EventHashType NameHash;
    public:
        /// @brief Descriptive constructor.
        /// @param ID The unique identifier for the subscriber/delegate.
        /// @param Delegate The callback to dispatch the event to.
        /// @param Pub A pointer to the publisher dispatching the interested event.
        /// @param Hash The hash of the event name this binding is subscribed to.
        EventSubscriberBinding(EventSubscriberID ID, const CallbackType& Delegate, EventPublisher* Pub, const EventHashType Hash);
        /// @brief Copy constructor.
        /// @param Other The other binding to not be copied.
        EventSubscriberBinding(const EventSubscriberBinding& Other) = delete;
        /// @brief Move constructor.
        /// @param Other The other binding to be moved.
        EventSubscriberBinding(EventSubscriberBinding&& Other) = default;
        /// @brief Class destructor.
        ~EventSubscriberBinding() = default;

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
        const CallbackType& GetCallback() const;
        /// @brief Gets the unique identifier of the subscriber.
        /// @return Returns a SubscriberID that uniquely identifies the subscription in the publisher.
        EventSubscriberID GetSubID() const;
        /// @brief Gets the publisher that has been subscribed to.
        /// @return Returns a pointer to the EventPublisher that the subscriber will receive events from.
        EventPublisher* GetPublisher() const;
        /// @brief Gets the hash of the event this binding is bound to.
        /// @return Returns the hash for the event name this binding is subscribed to.
        EventHashType GetEventHash() const;

        /// @brief Check if this binding is still valid.
        /// @return Returns true of the subscriber can still get events from the publisher, false otherwise.
        Boole IsSubscribed() const;

        /// @brief Removes the subscriber from the list of interested recipients on the publisher.
        void Unsubscribe();
        /// @brief Removes all references to an Event and/or Publisher from this binding.
        /// @remarks This method is called by Unsubscribe, and should never need to be called manually.
        /// This method also makes zero attempt to notify the publisher of it's changed state.  For that
        /// you should call Unsubscribe.
        void Unbind();

        /// @brief Dispatches an event to the appropriate subscriber in this binding.
        /// @param Args The arguments and extra data related to this event.
        void DispatchEvent(EventPtr Args) const;
    };//EventSubscriberBinding

	/// @brief Convenience type for passing around EventSubscriberBindings.
	using EventSubscriberBindingPtr = std::shared_ptr<EventSubscriberBinding>;

    /// @}
}//Mezzanine

#endif
