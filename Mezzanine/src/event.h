//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _event_h
#define _event_h

#include "eventconnection.h"
#include "eventsubscriber.h"
#include <map>

namespace Mezzanine
{
	///////////////////////////////////////////////////////////////////////////////
    /// @class Event
    /// @headerfile event.h
    /// @brief This class represents a given event that can be subscribed to and/or fired.
    /// @details
    ///////////////////////////////////////
	class MEZZ_LIB Event
	{
    public:
        /// @brief Basic container type for @ref EventConnection storage by this class.
        typedef std::multimap<UInt8,EventConnectionPtr>  ConnectionMap;
        /// @brief Iterator type for @ref EventConnection instances stored by this class.
        typedef ConnectionMap::iterator                  ConnectionIterator;
        /// @brief Const Iterator type for @ref EventConnection instances stored by this class.
        typedef ConnectionMap::const_iterator            ConstConnectionIterator;
        /// @brief An std::pair type for working with stored @ref EventConnection instances.
        typedef std::pair<UInt8,EventConnectionPtr>      ConnectionPair;
    protected:
        /// @internal
        /// @brief The name of this Event.
        const String EventName;
        /// @internal
        /// @brief A container storing all the EventConnection instances to subscribers.
        ConnectionMap Connections;
    public:
        /// @brief Class constructor.
        /// @param Name The name to be given to this event.
        Event(const String& Name);
        /// @brief Class destructor.
        ~Event();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the name of this event.
        /// @return Returns a const string reference containing the name of this event.
        const String& GetName() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Subscriber Slot Creation

        /// @brief Creates an event subscriber slot from normal subscriber.
        /// @details This function exists for custom subscribers that inherit from the EventSubscriber base class.  As
        /// such this function will not perform any cleanup when the object is removed from all events.
        /// @param Ev The event being subscribed to.
        /// @param Sub The custom event subscriber.
        /// @return Returns a ready-to-be-inserted event subscriber slot.
        static EventConnectionPtr CreateConnection(Event* Ev, EventSubscriber* Sub);
        /// @brief Creates an event subscriber slot from a Functor.
        /// @param Ev The event being subscribed to.
        /// @param Funct The functor to call when the event is fired.
        /// @param CleanUpAfter Whether or not to delete the functor when this subscriber is no longer subscribed to any events.
        /// @return Returns a ready-to-be-inserted event subscriber slot.
        static EventConnectionPtr CreateConnection(Event* Ev, FunctorSubscriber::FunctorDefinition* Funct, bool CleanUpAfter);
        /// @brief Creates an event subscriber slot from a C-style function.
        /// @param Ev The event being subscribed to.
        /// @param CFunct The C-style function to call when the event is fired.
        /// @return Returns a ready-to-be-inserted event subscriber slot.
        static EventConnectionPtr CreateConnection(Event* Ev, CFunctionSubscriber::SubscriberFunction* CFunct);
        /// @brief Creates an event subscriber slot that executes a provided script.
        /// @param Ev The event being subscribed to.
        /// @param SubScript The subscribed script to execute when the event is fired.
        /// @return Returns a ready-to-be-inserted event subscriber slot.
        static EventConnectionPtr CreateConnection(Event* Ev, Scripting::iScript* SubScript);
        /// @brief Creates an event subscriber slot that calls a member function of a particular object.
        /// @param Ev The event being subscribed to.
        /// @param ObjFunction The member function to call when the event is fired.
        /// @param Obj The object to call the member function on.
        /// @return Returns a ready-to-be-inserted event subscriber slot.
        template<typename T>
        static EventConnectionPtr CreateConnection(Event* Ev, void (T::*ObjFunction)(const EventArguments&), T* Obj)
        {
            MemberFunctionSubscriber<T>* Sub = new MemberFunctionSubscriber<T>(ObjFunction,Obj);
            EventConnectionPtr NewConnection(new VolitileEventConnection(Ev,Sub));
            return NewConnection;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Subscribe Methods

        /// @brief Adds a subscriber to this event.
        /// @param Subscriber The event subscriber slot to subscribe to this event.
        void Subscribe(EventConnectionPtr Subscriber);
        /// @brief Adds a subscriber to this event.
        /// @param Group The group "mask" to add this subscriber to.  Subscribers will be called in group order.
        /// @param Subscriber The event subscriber slot to subscribe to this event.
        void Subscribe(const UInt8 Group, EventConnectionPtr Subscriber);
        /// @brief Adds a subscriber to this event.
        /// @param Sub The custom event subscriber.
        /// @return Returns an EventConnectionPtr that can be inserted into other events.
        EventConnectionPtr Subscribe(EventSubscriber* Sub);
        /// @brief Adds a subscriber to this event.
        /// @param Group The group "mask" to add this subscriber to.  Subscribers will be called in group order.
        /// @param Sub The custom event subscriber.
        /// @return Returns an EventConnectionPtr that can be inserted into other events.
        EventConnectionPtr Subscribe(const UInt8 Group, EventSubscriber* Sub);
        /// @brief Subscribes a functor object to this event.
        /// @param Funct The functor to call when the event is fired.
        /// @param CleanUpAfter Whether or not to delete the functor when this subscriber is no longer subscribed to any events.
        /// @return Returns an EventConnectionPtr that can be inserted into other events.
        EventConnectionPtr Subscribe(FunctorSubscriber::FunctorDefinition* Funct, bool CleanUpAfter);
        /// @brief Subscribes a functor object to this event.
        /// @param Group The group "mask" to add this subscriber to.  Subscribers will be called in group order.
        /// @param Funct The functor to call when the event is fired.
        /// @param CleanUpAfter Whether or not to delete the functor when this subscriber is no longer subscribed to any events.
        /// @return Returns an EventConnectionPtr that can be inserted into other events.
        EventConnectionPtr Subscribe(const UInt8 Group, FunctorSubscriber::FunctorDefinition* Funct, bool CleanUpAfter);
        /// @brief Subscribes a C-style function to this event.
        /// @param CFunct The C-style function to call when the event is fired.
        /// @return Returns an EventConnectionPtr that can be inserted into other events.
        EventConnectionPtr Subscribe(CFunctionSubscriber::SubscriberFunction* CFunct);
        /// @brief Subscribes a C-style function to this event.
        /// @param Group The group "mask" to add this subscriber to.  Subscribers will be called in group order.
        /// @param CFunct The C-style function to call when the event is fired.
        /// @return Returns an EventConnectionPtr that can be inserted into other events.
        EventConnectionPtr Subscribe(const UInt8 Group, CFunctionSubscriber::SubscriberFunction* CFunct);
        /// @brief Subscribes a script to this event.
        /// @param SubScript The subscribed script to execute when the event is fired.
        /// @return Returns an EventConnectionPtr that can be inserted into other events.
        EventConnectionPtr Subscribe(Scripting::iScript* SubScript);
        /// @brief Subscribes a script to this event.
        /// @param Group The group "mask" to add this subscriber to.  Subscribers will be called in group order.
        /// @param SubScript The subscribed script to execute when the event is fired.
        /// @return Returns an EventConnectionPtr that can be inserted into other events.
        EventConnectionPtr Subscribe(const UInt8 Group, Scripting::iScript* SubScript);
        /// @brief Subscribes an Object and a member function on that object to this event.
        /// @param ObjFunction The member function to call when the event is fired.
        /// @param Obj The object to call the member function on.
        /// @return Returns an EventConnectionPtr that can be inserted into other events.
        template<typename T>
        EventConnectionPtr Subscribe(void (T::*ObjFunction)(const EventArguments&), T* Obj)
        {
            return Subscribe(255,ObjFunction,Obj);
        }
        /// @brief Subscribes an Object and a member function on that object to this event.
        /// @param Group The group "mask" to add this subscriber to.  Subscribers will be called in group order.
        /// @param ObjFunction The member function to call when the event is fired.
        /// @param Obj The object to call the member function on.
        /// @return Returns an EventConnectionPtr that can be inserted into other events.
        template<typename T>
        EventConnectionPtr Subscribe(const UInt8 Group, void (T::*ObjFunction)(const EventArguments&), T* Obj)
        {
            EventConnectionPtr EventSub = Event::CreateConnection(this,ObjFunction,Obj);
            Subscribe(Group,EventSub);
            return EventSub;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Unsubscribe Methods

        /// @brief Unsubscribes a single subscriber from this event.
        /// @param Subscriber Pointer to the subscriber to be removed.
        void Unsubscribe(EventConnection* Subscriber);
        /// @brief Unsubscribes a single subscriber from this event.
        /// @param Subscriber Shared pointer to the subscriber to be removed.
        void Unsubscribe(EventConnectionPtr Subscriber);
        /// @brief Unsubscribes a single subscriber from this event.
        /// @param Subscriber Iterator to the subscriber to be removed.
        void Unsubscribe(ConnectionIterator Subscriber);
        /// @brief Unsubscribes an entire group from this event.
        /// @param Group The mask for the group of subscribers to remove.
        /// @return Returns the number of subscribers removed.
        Whole UnsubscribeGroup(const UInt8 Group);
        /// @brief Unsubscribes all subscribers from this Event.
        /// @return Returns the number of subscribers removed.
        Whole UnsubscribeAll();

        ///////////////////////////////////////////////////////////////////////////////
        // Subscriber Access Methods

        /// @brief Gets an iterator to the first subscriber subscribed to this event.
        /// @return Returns an iterator to the first subscriber.
        ConnectionIterator SubscriberBegin();
        /// @brief Gets an iterator to one passed the last subscriber subscribed to this event.
        /// @return Returns an iterator to one passed the last subscriber.
        ConnectionIterator SubscriberEnd();
        /// @brief Gets an iterator to the first subscriber subscribed to this event.
        /// @return Returns a const iterator to the first subscriber.
        ConstConnectionIterator SubscriberBegin() const;
        /// @brief Gets an iterator to one passed the last subscriber subscribed to this event.
        /// @return Returns a const iterator to one passed the last subscriber.
        ConstConnectionIterator SubscriberEnd() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @internal
        /// @brief Notifies all subscribers of this event that this event is firing.
        /// @param Args The arguments and extra data related to this event.
        void _FireEvent(const EventArguments& Args);
        /// @internal
        /// @brief Notifies all subscribers in the specified group of this event that this event is firing.
        /// @param Group The group of subcribers to be notified.
        /// @param Args The arguments and extra data related to this event.
        void _FireGroupEvent(const UInt8 Group, const EventArguments& Args);
	};//Event
}//Mezzanine

#endif
