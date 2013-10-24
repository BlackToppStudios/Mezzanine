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
#ifndef _eventsubscriber_h
#define _eventsubscriber_h

#include "eventarguments.h"
#include "eventconnection.h"

namespace Mezzanine
{
    namespace Scripting
    {
        class iScript;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /// @class EventSubscriber
    /// @headerfile eventsubscriber.h
    /// @brief This is a base class for all classes that subscribe to events.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB EventSubscriber
    {
    public:
        /// @brief Basic container type for @ref EventConnection storage by this class.
        typedef std::vector<EventConnectionPtr>      ConnectionContainer;
        /// @brief Iterator type for @ref EventConnection instances stored by this class.
        typedef ConnectionContainer::iterator        ConnectionIterator;
        /// @brief Const Iterator type for @ref EventConnection instances stored by this class.
        typedef ConnectionContainer::const_iterator  ConstConnectionIterator;
    protected:
        friend class EventConnection;
        friend class VolitileEventConnection;
        /// @internal
        /// @brief A container storing all the EventConnection instances to events.
        ConnectionContainer Connections;
    //public:
        /// @brief Class constructor.
        EventSubscriber();
        /// @brief Class destructor.
        virtual ~EventSubscriber();
    public:
        /// @internal
        /// @brief Notifies this subscriber of an event being fired.
        /// @param Args The arguments containing specific information regarding this event.
        virtual void _NotifyEvent(const EventArguments& Args) = 0;
    };//EventSubscriber

    ///////////////////////////////////////////////////////////////////////////////
    /// @class FunctorSubscriber
    /// @headerfile eventsubscriber.h
    /// @brief This is a subscriber class that makes the appropriate call on a functor.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB FunctorSubscriber : public EventSubscriber
    {
    public:
        class FunctorDefinition
        {
        public:
            /// @brief Class destructor.
            virtual ~FunctorDefinition() {  };
            /// @brief Executes subscriber specific functionality when the event is fired.
            /// @param Args The arguments that describe the fired event.
            virtual void operator()(const EventArguments& Args) = 0;
        };
    protected:
        friend class Event;
        /// @internal
        /// @brief A pointer to the functor to be called when the event is fired.
        FunctorDefinition* Functor;
        /// @internal
        /// @brief Stores whether or not the functor is to be deleted when this subscriber is destructed.
        bool CleanUp;
    //public:
        /// @brief Class constructor.
        /// @param Funct The Functor object to be called when the event is fired.
        /// @param CleanUpAfter Whether or not to delete the functor when this object is deleted.
        FunctorSubscriber(FunctorDefinition* Funct, bool CleanUpAfter);
        /// @brief Class destructor.
        virtual ~FunctorSubscriber();
    public:
        /// @copydoc EventSubscriber::_NotifyEvent(const EventArguments& Args)
        virtual void _NotifyEvent(const EventArguments& Args);
    };//FunctorSubscriber

    ///////////////////////////////////////////////////////////////////////////////
    /// @class CFunctionSubscriber
    /// @headerfile eventsubscriber.h
    /// @brief This is a subscriber class that triggers a Free/C-style function.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB CFunctionSubscriber : public EventSubscriber
    {
    public:
        typedef void (SubscriberFunction)(const EventArguments& Args);
    protected:
        friend class Event;
        /// @internal
        /// @brief A pointer to the c-style function to be called when the event is fired.
        SubscriberFunction* CFunction;
    //public:
        /// @brief Class constructor.
        /// @param Funct The C-style function to be called when the event is fired.
        CFunctionSubscriber(SubscriberFunction* CFunct);
        /// @brief Class destructor.
        virtual ~CFunctionSubscriber();
    public:
        /// @copydoc EventSubscriber::_NotifyEvent(const EventArguments& Args)
        virtual void _NotifyEvent(const EventArguments& Args);
    };//CFunctionSubscriber

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ScriptSubscriber
    /// @headerfile eventsubscriber.h
    /// @brief This is a subscriber class that makes that triggers a specified script.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB ScriptSubscriber : public EventSubscriber
    {
    protected:
        friend class Event;
        /// @internal
        /// @brief A pointer to the script to be executed when the event is fired.
        Scripting::iScript* SubscriberScript;
    //public:
        /// @brief Class constructor.
        /// @param SubScript The subscriber script to be called when the event is fired.
        ScriptSubscriber(Scripting::iScript* SubScript);
        /// @brief Class destructor.
        virtual ~ScriptSubscriber();
    public:
        /// @copydoc EventSubscriber::_NotifyEvent(const EventArguments& Args)
        virtual void _NotifyEvent(const EventArguments& Args);
    };//ScriptSubscriber

    ///////////////////////////////////////////////////////////////////////////////
    /// @class MemberFunctionSubscriber
    /// @headerfile eventsubscriber.h
    /// @brief This is a subscriber class that triggers a member function of a specified instance.
    /// @details
    ///////////////////////////////////////
    template<typename T>
    class MEZZ_LIB MemberFunctionSubscriber : public EventSubscriber
    {
    public:
        typedef void (T::*MemberFunctionType)(const EventArguments& Args);
    protected:
        friend class Event;
        /// @internal
        /// @brief A pointer to the object to have it's member function called.
        T* Object;
        /// @internal
        /// @brief A pointer to the class method to be called.
        MemberFunctionType ObjectFunction;
    //public:
        /// @brief Class constructor.
        /// @param (T::*function) The member function accepting "const EventArguments& Args" to call.
        /// @param Obj The object to call the specified function on.
        MemberFunctionSubscriber(MemberFunctionType ObjFunction, T* Obj) :
            ObjectFunction(ObjFunction),
            Object(Obj)
            {  }
        /// @brief Class destructor.
        virtual ~MemberFunctionSubscriber()
            {  }
    public:
        /// @copydoc EventSubscriber::_NotifyEvent(const EventArguments& Args)
        virtual void _NotifyEvent(const EventArguments& Args)
        {
            (Object->*ObjectFunction)(Args);
        }
    };//MemberFunctionSubscriber
}//Mezzanine

#endif
