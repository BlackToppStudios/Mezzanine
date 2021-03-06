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
#ifndef _eventsubscriber_h
#define _eventsubscriber_h

#include "event.h"
#include <queue>

namespace Mezzanine
{
    /// @addtogroup Events
    /// @{

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is an example class that satisfies all the preconditions expected by other event classes.
    /// @tparam SubIDType The type to use for the unique identification of subscriber instances.
    ///////////////////////////////////////
    template<typename SubIDType>
    class EventSubscriber
    {
    public:
        /// @brief The type to use for the unique identification of subscriber instances.
        using IDType = SubIDType;

        /// @brief Gets the ID of this subscriber.
        /// @return Returns an instance of IDType identifying this subscriber.
        virtual IDType GetID() const = 0;
    };//EventSubscriber

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is an example class that satisfies all the preconditions expected by other event classes.
    /// @tparam SubIDType The type to use for the unique identification of subscriber instances.
    /// @tparam ReturnType The type to be returned by the function called when an event is dispatched.
    /// @tparam ArgTypes A variadic template of types that are the parameters of the function to call.
    /// @pre SubIDType is required to have assignment operators, equality comparison operators and a less-than operator(=,==,!=,<).
    /// @pre ReturnType has no preconditions.  It's just fed into an std::function.
    /// @pre ArgTypes cannot contain any non-const references or move references.  Also fed into an std::function.
    ///////////////////////////////////////
    template<class SubIDType, typename ReturnType, typename... ArgTypes>
    class FunctionSubscriber
    {
    public:
        /// @brief The type to use for the unique identification of subscriber instances.
        using IDType = SubIDType;
        /// @brief The type of standard function/function signature to call.
        using FunctionType = std::function<ReturnType(ArgTypes...)>;
        /// @brief The type to be returned after the function is invoked.
        using RetType = ReturnType;
        /// @brief Convenience type
        using SelfType = FunctionSubscriber<IDType,ReturnType,ArgTypes...>;
    protected:
        /// @brief The function to be called when the event is fired.
        FunctionType SubFunct;
        /// @brief The ID to be associated with the function set.
        IDType SubID;
    public:
        /// @brief Class constructor.
        FunctionSubscriber() = delete;
        /// @brief Descriptive constructor.
        /// @param ID The ID to be associated with the function set.
        /// @param Funct The function to be called when the event is fired.
        FunctionSubscriber(const IDType ID, const FunctionType& Funct) :
            SubFunct(Funct),
            SubID(ID)
            {  }
        /// @brief Copy constructor.
        /// @param Other The other subscriber to NOT be copied.
        FunctionSubscriber(const SelfType& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other subscriber to be moved.
        FunctionSubscriber(SelfType&& Other) = default;
        /// @brief Class destructor.
        ~FunctionSubscriber() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Copy constructor.
        /// @param Other The other subscriber to NOT be copied.
        /// @return Returns a reference to this.
        SelfType& operator=(const SelfType& Other) = default;
        /// @brief Move assignment operator.
        /// @param Other The other subscriber to be moved.
        /// @return Returns a reference to this.
        SelfType& operator=(SelfType&& Other) = default;

        /// @brief Less-Than operator.
        /// @param Other The other subscriber to compare with.
        /// @return Returns true if this should be placed before the other
        Boole operator<(const SelfType& Other) const
            { return this->SubID < Other.SubID; }

        ///////////////////////////////////////////////////////////////////////////////
        // ID

        /// @brief Gets the ID of this subscriber.
        /// @return Returns an instance of IDType identifying this subscriber.
        constexpr IDType GetID() const
            { return this->SubID; }

        ///////////////////////////////////////////////////////////////////////////////
        // Callable

        /// @brief Callable method for this subscriber.
        /// @param Params The parameters of the function to be called.
        RetType operator()(ArgTypes... Params) const
            { return this->SubFunct( std::forward<ArgTypes>(Params)... ); }
    };//FunctionSubscriber

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a simple subscriber that will queue up the events it receives for later processing.
    /// @remarks This is not expected to be a complete solution for queuing events subscriber-side.
    /// @tparam SubIDType The type to use for the unique identification of subscriber instances.
    /// @pre SubIDType is expected to be able to convert 0 into an ID.
    ///////////////////////////////////////
    template<typename SubIDType>
    class QueuedSubscriber : public EventSubscriber<SubIDType>
    {
    public:
        /// @brief The type to use for the unique identification of subscriber instances.
        using IDType = SubIDType;
        /// @brief Basic container type for Events stored by this class.
        using QueueType = std::queue<EventPtr>;
    protected:
        /// @brief The container storing the queued up events to be processed.
        QueueType EventQueue;
    public:
        /// @copydoc EventSubscriber::GetID() const
        virtual IDType GetID() const
            { return 0; }
    };//QueuedSubscriber

    /// @}
}//Mezzanine

#endif
