// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _eventsubscriberslot_h
#define _eventsubscriberslot_h

#include "eventarguments.h"

namespace Mezzanine
{
    class Event;
    class EventSubscriber;
    namespace Scripting
    {
        class iScript;
    }
	///////////////////////////////////////////////////////////////////////////////
    /// @brief This class represents a slot in an event that can be subscribed to via subscribers, functors, or methods.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB EventSubscriberSlot
    {
    public:
        /// @brief This enum is used to describe the type of SubscriberSlot an instance is, to be used for casting.
        enum SlotType
        {
            ST_Custom         = 1,
            ST_Functor        = 2,
            ST_CFunction      = 3,
            ST_Script         = 4,
            ST_MemberFunction = 5
        };
    protected:
        /// @internal
        /// @brief A pointer to the connected event.
        Event* SubbedEvent;
    public:
        /// @brief Class constructor.
        /// @param Ev The event this subscriber slot belongs to.
        EventSubscriberSlot(Event* Ev);
        /// @brief Class destructor.
        virtual ~EventSubscriberSlot();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the set of events the subscriber is subscribed to.
        /// @return Returns a reference to the set of events this is subscribed to.
        Event* GetEvent() const;
        /// @brief Gets the type of subscriber slot this is.
        /// @return Returns a SlotType enum value representing the type of subscriber slot this is.
        virtual SlotType GetType() const = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @internal
        /// @brief Notifies this subscriber of an event being fired.
        /// @param Args The arguments containing specific information regarding this event.
        virtual void _NotifyEvent(const EventArguments& Args) = 0;
    };//EventSubscriberSlot

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a subscriber slot class that passes on the event firing to a custom subscriber class.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB CustomSubscriberSlot : public EventSubscriberSlot
    {
    protected:
        /// @internal
        /// @brief A pointer to the custom subscriber that the event will be passed on to.
        EventSubscriber* Subscriber;
    public:
        /// @brief Class constructor.
        /// @param Ev The event this subscriber slot belongs to.
        /// @param SubScript The subscriber script to be called when the event is fired.
        CustomSubscriberSlot(Event* Ev, EventSubscriber* Sub);
        /// @brief Class destructor.
        virtual ~CustomSubscriberSlot();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets a pointer to the subscriber used by this subscriber slot.
        /// @return Returns a pointer to the custom subscriber being used in this subscriber slot.
        EventSubscriber* GetSubscriber() const;
        /// @copydoc EventSubscriberSlot::GetType() const
        virtual SlotType GetType() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @copydoc EventSubscriberSlot::_NotifyEvent(const EventArguments& Args)
        virtual void _NotifyEvent(const EventArguments& Args);
    };// CustomSubscriberSlot

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a subscriber slot class that makes the appropriate call on a functor.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB FunctorSubscriberSlot : public EventSubscriberSlot
    {
    public:
        /// @brief Basic class definition for functors used by this subscriber slot.
        class FunctorDefinition
        {
        public:
            /// @brief Class destructor.
            virtual ~FunctorDefinition() {  };
            /// @brief Executes subscriber specific functionality when the event is fired.
            /// @param Args The arguments that describe the fired event.
            virtual void operator()(const EventArguments& Args) = 0;
        };//FunctorDefinition
    protected:
        /// @internal
        /// @brief A pointer to the functor to be called when the event is fired.
        FunctorDefinition* Functor;
        /// @internal
        /// @brief Stores whether or not the functor is to be deleted when this subscriber is destructed.
        Boolean CleanUp;
    public:
        /// @brief Class constructor.
        /// @param Ev The event this subscriber slot belongs to.
        /// @param Funct The Functor object to be called when the event is fired.
        /// @param CleanUpAfter Whether or not to delete the functor when this object is deleted.
        FunctorSubscriberSlot(Event* Ev, FunctorDefinition* Funct, bool CleanUpAfter);
        /// @brief Class destructor.
        virtual ~FunctorSubscriberSlot();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets a pointer to the functor used by this subscriber slot.
        /// @return Returns a pointer to the functor subscriber being used in this subscriber slot.
        FunctorDefinition* GetFunctor() const;
        /// @copydoc EventSubscriberSlot::GetType() const
        virtual SlotType GetType() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @copydoc EventSubscriberSlot::_NotifyEvent(const EventArguments& Args)
        virtual void _NotifyEvent(const EventArguments& Args);
    };//FunctorSubscriberSlot

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a subscriber slot class that triggers a Free/C-style function.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB CFunctionSubscriberSlot : public EventSubscriberSlot
    {
    public:
        /// @brief This is a convenience typedef for a c-style method that accepts EventArguments.
        typedef void (SubscriberFunction)(const EventArguments& Args);
    protected:
        /// @internal
        /// @brief A pointer to the c-style function to be called when the event is fired.
        SubscriberFunction* Function;
    public:
        /// @brief Class constructor.
        /// @param Ev The event this subscriber slot belongs to.
        /// @param Funct The C-style function to be called when the event is fired.
        CFunctionSubscriberSlot(Event* Ev, SubscriberFunction* Funct);
        /// @brief Class destructor.
        virtual ~CFunctionSubscriberSlot();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets a pointer to the function used by this subscriber slot.
        /// @return Returns a pointer to the function subscriber being used in this subscriber slot.
        SubscriberFunction* GetFunction() const;
        /// @copydoc EventSubscriberSlot::GetType() const
        virtual SlotType GetType() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @copydoc EventSubscriberSlot::_NotifyEvent(const EventArguments& Args)
        virtual void _NotifyEvent(const EventArguments& Args);
    };// CFunctionSubscriberSlot

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a subscriber slot class that triggers a provided script.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB ScriptSubscriberSlot : public EventSubscriberSlot
    {
    protected:
        /// @internal
        /// @brief A pointer to the script to be executed when the event is fired.
        Scripting::iScript* SubscriberScript;
    public:
        /// @brief Class constructor.
        /// @param Ev The event this subscriber slot belongs to.
        /// @param SubScript The subscriber script to be called when the event is fired.
        ScriptSubscriberSlot(Event* Ev, Scripting::iScript* SubScript);
        /// @brief Class destructor.
        virtual ~ScriptSubscriberSlot();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets a pointer to the function used by this subscriber slot.
        /// @return Returns a pointer to the function subscriber being used in this subscriber slot.
        Scripting::iScript* GetScript() const;
        /// @copydoc EventSubscriberSlot::GetType() const
        virtual SlotType GetType() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @copydoc EventSubscriberSlot::_NotifyEvent(const EventArguments& Args)
        virtual void _NotifyEvent(const EventArguments& Args);
    };//ScriptSubscriberSlot
}//Mezzanine

#endif
