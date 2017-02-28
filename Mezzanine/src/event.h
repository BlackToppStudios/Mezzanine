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
#ifndef _event_h
#define _event_h

#include "eventsubscriber.h"
#include "eventsubscriberslot.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a common class to represent all possible arguments for a given event that is fired.
    ///////////////////////////////////////
	class MEZZ_LIB Event
	{
    private:
        /// @brief This is count of the number of references to this object.
        Whole RefCount;
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief The name of the event being fired.
        const HashedString32 EventName;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Class constructor.
        /// @param Name The name of the event being fired.
        Event(const HashedString32& Name) :
            EventName(Name)
            {  }
        /// @brief Class destructor.
        virtual ~Event() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // CountedPtr Functionality

        /// @brief Increase the reference count by one and return the updated count.
        /// @return Returns a Whole representing the updated count.
        Whole IncrementReferenceCount()
            { return ++this->RefCount; }
        /// @brief Decrease the reference count by one and return the updated count.
        /// @return Returns a Whole representing the updated count.
        Whole DecrementReferenceCount()
            { return --this->RefCount; }
        /// @brief Get the current amount of references.
        /// @return A Whole with the current reference count
        Whole GetReferenceCount() const
            { return this->RefCount; }

        /// @brief Gets the actual pointer to the target of the base type.
        /// @return Returns a pointer of the targeted type to the object being managed.
        Event* GetReferenceCountTargetAsPointer()
            { return this; }
        /// @brief Get a pointer to the most Derived type of this instance.
        /// @return Returns a pointer of the most derived type of this.
        virtual Event* GetMostDerived()
            { return this; }
	};//Event

	///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a metaprogramming traits class used by Events.
    /// @details This is needed for an intrusive CountedPtr implementation.  Should a working external reference count be made this
    /// could be dropped in favor of a leaner implementation.
    ///////////////////////////////////////
	template <>
    class ReferenceCountTraits<Event>
    {
    public:
        /// @brief Typedef communicating the reference count type to be used.
        typedef Event RefCountType;

        /// @brief Method responsible for creating a reference count for a CountedPtr of the templated type.
        /// @param Target A pointer to the target class that is to be reference counted.
        /// @return Returns a pointer to a new reference counter for the templated type.
        static RefCountType* ConstructionPointer(RefCountType* Target)
            { return Target; }

        /// @brief Enum used to decide the type of casting to be used by a reference counter of the templated type.
        enum { IsCastable = CastStatic };
    };//ReferenceCountTraits<Event>

	/// @brief Convenience typedef for passing around EventArguments.
	typedef CountedPtr<Event> EventPtr;

    /*
	///////////////////////////////////////////////////////////////////////////////
    /// @brief This class represents a given event that can be subscribed to and/or fired.
    ///////////////////////////////////////
	class MEZZ_LIB Event
	{
    public:
        /// @brief Basic container type for @ref EventSubscriberSlot storage by this class.
        typedef std::vector<EventSubscriberSlot*>           SlotContainer;
        /// @brief Iterator type for @ref EventSubscriberSlot instances stored by this class.
        typedef SlotContainer::iterator                     SlotIterator;
        /// @brief Const Iterator type for @ref EventSubscriberSlot instances stored by this class.
        typedef SlotContainer::const_iterator               ConstSlotIterator;
        /// @brief An std::pair type for working with stored @ref EventSubscriberSlot instances.
        typedef std::pair<UInt8,EventSubscriberSlot*>       SlotPair;
    protected:
        /// @brief The name of this Event.
        const HashedString32 EventName;
        /// @brief A container storing all the EventSubscriberSlot instances to subscribers.
        SlotContainer Slots;
    public:
        /// @brief Class constructor.
        /// @param Name The name to be given to this event.
        Event(const HashedString32& Name);
        /// @brief Class destructor.
        ~Event();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the name of this event.
        /// @return Returns a const reference of a hashed string containing the name of this event.
        const HashedString32& GetName() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Subscribe Methods

        /// @brief Adds a subscriber to this event.
        /// @param Subscriber The custom event subscriber.
        /// @return Returns a pointer to the created Subscriber slot for the provided subscriber.
        EventSubscriberSlot* Subscribe(EventSubscriber* Subscriber);
        /// @brief Subscribes a functor object to this event.
        /// @param Funct The functor to call when the event is fired.
        /// @param CleanUpAfter Whether or not to delete the functor when this subscriber is no longer subscribed to any events.
        /// @return Returns a pointer to the created Subscriber slot for the provided subscriber.
        EventSubscriberSlot* Subscribe(FunctorEventSubscriber* Funct, Boole CleanUpAfter);
        /// @brief Subscribes a C-style function to this event.
        /// @param CFunct The C-style function to call when the event is fired.
        /// @return Returns a pointer to the created Subscriber slot for the provided subscriber.
        EventSubscriberSlot* Subscribe(CFunctionSubscriberSlot::SubscriberFunction* CFunct);
        /// @brief Subscribes a script to this event.
        /// @param SubScript The subscribed script to execute when the event is fired.
        /// @return Returns a pointer to the created Subscriber slot for the provided subscriber.
        EventSubscriberSlot* Subscribe(Scripting::iScript* SubScript);

        ///////////////////////////////////////////////////////////////////////////////
        // Unsubscribe Methods

        /// @brief Unsubscribes a single subscriber from this event.
        /// @param Subscriber The EventSubscriberSlot (and the subscriber it is holding) to be removed.
        void Unsubscribe(EventSubscriber* Subscriber);
        /// @brief Unsubscribes a single subscriber from this event.
        /// @param Funct The functor to be removed.
        void Unsubscribe(FunctorEventSubscriber* Funct);
        /// @brief Unsubscribes a single subscriber from this event.
        /// @param CFunct The function to be removed.
        void Unsubscribe(CFunctionSubscriberSlot::SubscriberFunction* CFunct);
        /// @brief Unsubscribes a single subscriber from this event.
        /// @param SubScript The Script to be removed.
        void Unsubscribe(Scripting::iScript* SubScript);
        /// @brief Unsubscribes a single subscriber from this event.
        /// @param SubSlot The EventSubscriberSlot (and the subscriber it is holding) to be removed.
        void Unsubscribe(EventSubscriberSlot* SubSlot);
        /// @brief Unsubscribes all subscribers from this Event.
        /// @return Returns the number of subscribers removed.
        Whole UnsubscribeAll();

        ///////////////////////////////////////////////////////////////////////////////
        // Subscriber Access Methods

        /// @brief Gets an iterator to the first subscriber slot in this event.
        /// @return Returns an iterator to the first subscriber slot.
        SlotIterator SubscriberSlotBegin();
        /// @brief Gets an iterator to one passed the last subscriber slot in this event.
        /// @return Returns an iterator to one passed the last subscriber slot.
        SlotIterator SubscriberSlotEnd();
#ifndef SWIG // Since these functions differ only by constness, they make no sense to most(all?) scripting languages
        /// @brief Gets an iterator to the first subscriber slot in this event.
        /// @return Returns a const iterator to the first subscriber slot.
        ConstSlotIterator SubscriberSlotBegin() const;
        /// @brief Gets an iterator to one passed the last subscriber slot in this event.
        /// @return Returns a const iterator to one passed the last subscriber slot.
        ConstSlotIterator SubscriberSlotEnd() const;
#endif

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @internal
        /// @brief Notifies all subscribers of this event that this event is firing.
        /// @param Args The arguments and extra data related to this event.
        void _FireEvent(EventArgumentsPtr Args);
	};//Event //*/
}//Mezzanine

#endif
