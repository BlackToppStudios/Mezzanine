// © Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _eventpublisher_h
#define _eventpublisher_h

#include "event.h"
#include "exception.h"

namespace Mezzanine
{
	class Event;
	///////////////////////////////////////////////////////////////////////////////
    /// @class EventPublisher
    /// @headerfile eventpublisher.h
    /// @brief This is the base class for any class that generates and publishes events to subscribers.
    /// @details
    ///////////////////////////////////////
	class MEZZ_LIB EventPublisher
	{
    public:
        /// @brief Basic container type for @ref Event storage by this class.
        typedef std::map<String,Event*>         EventContainer;
        /// @brief Iterator type for @ref Event instances stored by this class.
        typedef EventContainer::iterator        EventIterator;
        /// @brief Const Iterator type for @ref Event instances stored by this class.
        typedef EventContainer::const_iterator  ConstEventIterator;
    protected:
        /// @internal
        /// @brief A container storing all the Events published by this class by name.
        EventContainer Events;

        /// @internal
        /// @brief Creates a new event this Publisher can fire.
        /// @note If the event already exists, this will return the created event instead.
        /// @param EventName The name to be given to the new event.
        /// @return Returns a pointer to the created or existing event.
        Event* AddEvent(const String& EventName);
        /// @internal
        /// @brief Fires an event.
        /// @param Args The arguments/event specific data related to this event.
        void FireEvent(const EventArguments& Args);
        /// @internal
        /// @brief Removes an existing event in this Publisher.
        /// @param EventName The name of the event to be removed.
        void RemoveEvent(const String& EventName);
        /// @internal
        /// @brief Removes all events in this Publisher.
        void RemoveAllEvents();
    public:
        /// @brief Class constructor.
        EventPublisher();
        /// @brief Class destructor.
        virtual ~EventPublisher();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets an event in this publisher.
        /// @param EventName The name of the event to retrieve.
        /// @return Returns a pointer to the requested event.
        Event* GetEvent(const String& EventName) const;
        /// @brief Gets an event in this publisher.
        /// @exception This version differs from the non-except version in that if it fails to find the event specified
        /// it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION".
        /// @param EventName The name of the event to retrieve.
        /// @return Returns a pointer to the requested event.
        Event* GetEventExcept(const String& EventName) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Subscribe Methods

        /// @brief Adds a subscriber to this event.
        /// @param EventName The name of the event to subscribe to.
        /// @param Sub The custom event subscriber.
        /// @return Returns a pointer to the created Subscriber slot for the provided subscriber.
        EventSubscriberSlot* Subscribe(const String& EventName, EventSubscriber* Sub);
        /// @brief Subscribes a functor object to this event.
        /// @param EventName The name of the event to subscribe to.
        /// @param Funct The functor to call when the event is fired.
        /// @param CleanUpAfter Whether or not to delete the functor when this subscriber is no longer subscribed to any events.
        /// @return Returns a pointer to the created Subscriber slot for the provided subscriber.
        EventSubscriberSlot* Subscribe(const String& EventName, FunctorSubscriberSlot::FunctorDefinition* Funct, Bool CleanUpAfter);
        /// @brief Subscribes a C-style function to this event.
        /// @param EventName The name of the event to subscribe to.
        /// @param CFunct The C-style function to call when the event is fired.
        /// @return Returns a pointer to the created Subscriber slot for the provided subscriber.
        EventSubscriberSlot* Subscribe(const String& EventName, CFunctionSubscriberSlot::SubscriberFunction* CFunct);
        /// @brief Subscribes a script to this event.
        /// @param EventName The name of the event to subscribe to.
        /// @param SubScript The subscribed script to execute when the event is fired.
        /// @return Returns a pointer to the created Subscriber slot for the provided subscriber.
        EventSubscriberSlot* Subscribe(const String& EventName, Scripting::iScript* SubScript);

        ///////////////////////////////////////////////////////////////////////////////
        // Unsubscribe Methods

        /// @brief Unsubscribes a single subscriber all events in this publisher.
        /// @param Subscriber The EventSubscriberSlot (and the subscriber it is holding) to be removed.
        void Unsubscribe(EventSubscriber* Subscriber);
        /// @brief Unsubscribes a single subscriber all events in this publisher.
        /// @param Funct The functor to be removed.
        void Unsubscribe(FunctorSubscriberSlot::FunctorDefinition* Funct);
        /// @brief Unsubscribes a single subscriber from all events in this publisher.
        /// @param CFunct The function to be removed.
        void Unsubscribe(CFunctionSubscriberSlot::SubscriberFunction* CFunct);
        /// @brief Unsubscribes a single subscriber from all events in this publisher.
        /// @param SubScript The Script to be removed.
        void Unsubscribe(Scripting::iScript* SubScript);
        /// @brief Unsubscribes a single subscriber from all events in this publisher.
        /// @param SubSlot The EventSubscriberSlot (and the subscriber it is holding) to be removed.
        void Unsubscribe(EventSubscriberSlot* SubSlot);
        /// @brief Unsubscribes all subscribers from all events in this publisher.
        /// @return Returns the number of subscribers removed.
        Whole UnsubscribeAll();

        /// @brief Unsubscribes a single subscriber from the named event.
        /// @param EventName The name of the event to unsubscribe from.
        /// @param Subscriber The EventSubscriberSlot (and the subscriber it is holding) to be removed.
        void Unsubscribe(const String& EventName, EventSubscriber* Subscriber);
        /// @brief Unsubscribes a single subscriber from the named event.
        /// @param EventName The name of the event to unsubscribe from.
        /// @param Funct The functor to be removed.
        void Unsubscribe(const String& EventName, FunctorSubscriberSlot::FunctorDefinition* Funct);
        /// @brief Unsubscribes a single subscriber from the named event.
        /// @param EventName The name of the event to unsubscribe from.
        /// @param CFunct The function to be removed.
        void Unsubscribe(const String& EventName, CFunctionSubscriberSlot::SubscriberFunction* CFunct);
        /// @brief Unsubscribes a single subscriber from the named event.
        /// @param EventName The name of the event to unsubscribe from.
        /// @param SubScript The Script to be removed.
        void Unsubscribe(const String& EventName, Scripting::iScript* SubScript);
        /// @brief Unsubscribes a single subscriber from the named event.
        /// @param EventName The name of the event to unsubscribe from.
        /// @param SubSlot The EventSubscriberSlot (and the subscriber it is holding) to be removed.
        void Unsubscribe(const String& EventName, EventSubscriberSlot* SubSlot);
        /// @brief Unsubscribes all subscribers from the named Event.
        /// @param EventName The name of the event to unsubscribe from.
        /// @return Returns the number of subscribers removed.
        Whole UnsubscribeAll(const String& EventName);
	};//EventPublisher
}//Mezzanine

#endif
