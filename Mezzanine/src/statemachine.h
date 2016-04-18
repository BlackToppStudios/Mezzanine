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

    // Document stuff about default state being first state added.
    class MEZZ_LIB StateMachine
    {
        private:
            typedef SortedVector<HashedString32> StateContainerType;
            typedef StateContainerType::iterator StateIterator;
            typedef StateContainerType::const_iterator ConstStateIterator;

            typedef SortedVector<StateTransition> TranstionContainerType;
            typedef TranstionContainerType::iterator TranstionIterator;
            typedef TranstionContainerType::const_iterator ConstTranstionIterator;

            StateContainerType States;
            TranstionContainerType Transitions;
            ConstStateIterator CurrentState;
            ConstStateIterator FutureState;

        public:
            /// Cheap
            StateMachine();
            /// Cheap
            explicit StateMachine(const HashedString32& InitialState);
            /// Cheap
            explicit StateMachine(const String& InitialState);


            /// Less Cheap does insertion, might allocate and does sort work
            void AddState(const HashedString32& NewState);
            void AddState(const String& NewState);

            /// Expensive does insertion, hashing, might allocate and does sort work
            void AddStateTransitation(  const HashedString32& From,
                                        const HashedString32& To,
                                        StateTransitionAction* PossibleAction
                                            = new StateTransitionNoAction());
            void AddStateTransitation(  const String& From,
                                        const String& To,
                                        StateTransitionAction* PossibleAction
                                            = new StateTransitionNoAction());

            // costs search log time on container, ought to be pretty cheap
            Boole HasState(const HashedString32& PossibleState) const;
            Boole HasState(const String& PossibleState) const;

            Boole HasStateTransition(const HashedString32& FromState,
                                     const HashedString32& ToState) const;
            Boole HasStateTransition(const String& FromState, const String& ToState) const;


            Boole CanChangeState(const HashedString32& ToState);
            Boole CanChangeState(const String& ToState);

            Boole ChangeState(const HashedString32& ToState);
            Boole SetPendingState(const HashedString32& ToState);
            void DoPendingStateChange();


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
