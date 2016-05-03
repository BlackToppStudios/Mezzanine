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
#ifndef statemachine_h
#define statemachine_h

/// @file
/// @brief Describe file here

#include "datatypes.h"
#include "statetransition.h"
#include "sortedvector.h"

namespace Mezzanine
{
    /// @brief This is intended to replace sophisticated if structure and complex control flow
    /// shenanigans with the simpler model of switching between states in a simple and deterministic
    /// way.
    /// @details If you not familar with the concept of StateMachines start by thinking about a
    /// traffic light. Common traffic lights have 3 valid states Green, Yellow and Red, ignoring all
    /// those arrows for now. A traffic light and a StateMachine can only be in one state at a time
    /// and they can only transition to and from states according to predetermined rules. A Green
    /// light must become Yellow before Red.
    /// @n @n
    /// In the same way a StateMachine must transition to and from predetermined states. These
    /// states can be added with the @ref AddState method. By default there are no transitions
    /// allow between states, transition can be added with @ref AddStateTransition.
    /// @n @n
    /// By default nothing special happens during when transitioning from one state to another. Each
    /// @ref StateTransition calls an @ref StateTransitionAction and the default one is a no-op
    /// (it actually just returns true. Any
    /// class inheriting from @ref StateTransitionAction implenting operator() can be used. When
    /// transitioning states the state will not be changed unless that function returns true.
    /// @n @n
    /// StateMachine instances have a notion of "Current State". They track this for when
    /// transitions between states occur. The current state is the From state on am
    /// @ref StateTransition and the To state is whatever is passed into @ref ChangeState or
    /// @ref SetPendingState methods.
    class MEZZ_LIB StateMachine
    {
        private:
            /// @internal
            /// @brief This is the storage for States.
            typedef SortedVector<HashedString32> StateContainerType;
            /// @internal
            /// @brief A mutable iterator type over the container of states.
            typedef StateContainerType::iterator StateIterator;
            /// @internal
            /// @brief A IMmutable iterator type over the container of states.
            typedef StateContainerType::const_iterator ConstStateIterator;

            /// @internal
            /// @brief A container for StateTransitions.
            //typedef SortedVector<StateTransition*, StateTransitionPointerSorter>
            typedef SortedVector<StateTransition>
                TranstionContainerType;
            /// @internal
            /// @brief A mutable iterator type over the container of Transitions.
            typedef TranstionContainerType::iterator TranstionIterator;
            /// @internal
            /// @brief A IMmutable iterator type over the container of Transitions.
            typedef TranstionContainerType::const_iterator ConstTranstionIterator;

            /// @internal
            /// @brief The actual container used to store states
            StateContainerType States;
            /// @internal
            /// @brief The actual container used to store StateTransitions
            TranstionContainerType Transitions;

            /// @internal
            /// @brief An immutable handle indicating the "Current" state.
            ConstStateIterator CurrentState;

            /// @internal
            /// @brief Used to prevent one seek, when the Future state is sent this is set so that
            /// when the state is transitioned it can be done in constanttime.
            ConstStateIterator FutureState;
            /// @internal
            /// @brief When wanting to set a change for this to change to in the future, this is
            /// used to store the possible future state.
            TranstionIterator FutureStateTransition;

        public:
            /// @brief Construct and empty state machine.
            StateMachine();

            /// @brief Good State adding constructor
            /// @details This adds a state and makes it the current state.
            /// @param InitialState The first state added to the State machine and the current state
            /// immediately after construction
            /// @note This is the preferred constructor because it can copy the hashes from the
            /// HashString32 and not recalculate it.
            explicit StateMachine(const HashedString32& InitialState);

            /// @brief Easy State adding constructor
            /// @details This adds a state and makes it the current state.
            /// @param InitialState The first state added to the State machine and the current state
            /// immediately after construction
            /// @warning  This needlessly forces recalcuation of the hash for the state, try to use
            /// the constructor accepting A HashString32.
            explicit StateMachine(const String& InitialState);

            /// @brief Empty destructor
            ~StateMachine();

            /// @brief This adds a State to the StateMachine.
            /// @param NewState The State to be added.
            /// @details This is possibly the most expensive
            /// operation in the StateMachine because it sorts all the States and potentially
            /// allocates memory. Try to get all of this done up front.
            /// @note This is the preferred metohod because it can copy the hashes from the
            /// HashString32 and not recalculate it.
            void AddState(const HashedString32& NewState);

            /// @brief This adds a State to the StateMachine.
            /// @param NewState The State to be added.
            /// @details This is possibly the most expensive
            /// operation in the StateMachine because it sorts all the States and potentially
            /// allocates memory and then it also needs to calculate the hash of the string.
            /// @warning  This needlessly forces recalcuation of the hash for the state, try to use
            /// the AddState method accepting A HashString32.
            void AddState(const String& NewState);

            /// Expensive does insertion, hashing, might allocate and does sort work
            /// @brief Add a transition between two states.
            /// @param From The state that this needs to be in to go to the To state.
            /// @param To The new state that can be accessed from the From state.
            /// @param PossibleAction An owning Pointer to an @ref StateTransitionAction that is
            /// executed during the transition and determines success or failure. Defaults to an
            /// @details This is moderately expensive because it sorts all the StateTransitions and
            /// potentially allocates memory to store them. Try to get all of this done up front.
            /// @ref StateTransitionNoAction instance. Once passed in this will delete the pointer.
            /// @note This is the preferred method because it can copy the hashes from the
            /// HashString32 and not recalculate it.
            void AddStateTransitation(  const HashedString32& From,
                                        const HashedString32& To,
                                        StateTransitionAction* PossibleAction
                                            = new StateTransitionNoAction());

            /// Expensive does insertion, hashing, might allocate and does sort work
            /// @brief Add a transition between two states.
            /// @param From The state that this needs to be in to go to the To state.
            /// @param To The new state that can be accessed from the From state.
            /// @param PossibleAction An owning Pointer to an @ref StateTransitionAction that is
            /// executed during the transition and determines success or failure. Defaults to an
            /// @ref StateTransitionNoAction instance. Once passed in this will delete the pointer.
            /// @details This is moderately expensive because it sorts all the StateTransitions and
            /// potentially allocates memory to store them. Try to get all of this done up front.
            /// @warning This needlessly forces recalcuation of the hash for the state, try to use
            /// the AddStateTransitation method accepting A pair of HashString32.
            void AddStateTransitation(  const String& From,
                                        const String& To,
                                        StateTransitionAction* PossibleAction
                                            = new StateTransitionNoAction());

            /// @brief Does this StateMachine have the passed state?
            /// @param PossibleState The state to search for.
            /// @details This runs in log(NumberOfStates) time by binary searching over the the
            /// states. So it should be fast, but not free.
            /// @return True if the state is present, false otherwise.
            /// @note This is the preferred method because it can copy the hashes from the
            /// HashString32 and not recalculate it.
            Boole HasState(const HashedString32& PossibleState) const;

            /// @brief Does this StateMachine have the passed state?
            /// @param PossibleState The state to search for.
            /// @details This runs in log(NumberOfStates) time by binary searching over the the
            /// states. So it should be fast, but not free, also this wastes time calculating a
            /// hash for the String passed in.
            /// @return True if the state is present, false otherwise.
            /// @warning  This needlessly forces recalcuation of the hash for the state, try to use
            /// the HasState method accepting A HashString32.
            Boole HasState(const String& PossibleState) const;

            Boole HasStateTransition(const HashedString32& FromState,
                                     const HashedString32& ToState) const;
            Boole HasStateTransition(const String& FromState, const String& ToState) const;


            Boole CanChangeState(const HashedString32& ToState);
            Boole CanChangeState(const String& ToState);

            Boole ChangeState(const HashedString32& ToState);
            Boole ChangeState(const String& ToState);

            Boole SetPendingState(const HashedString32& ToState);
            Boole SetPendingState(const String& ToState);

            void ClearPendingState();

            Boole DoPendingStateChange();


            /// Cheap One if
            const HashedString32& GetCurrentState() const;
            /// Cheap One if
            const HashedString32& GetPendingState() const;
            /// Cheap
            Whole GetStateCount() const;
            /// Cheap
            Whole GetStateTransitionCount() const;
    };
} // /namespace Mezzanine

#endif // Include guard
