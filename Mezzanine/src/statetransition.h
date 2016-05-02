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
#ifndef statetransition_h
#define statetransition_h

/// @file
/// @brief Describe file here

#include "datatypes.h"
#include "hashedstring.h"
#include "statetransitionaction.h"

namespace Mezzanine
{
    /// @brief When a @ref StateMachine changes state how is the knowledge that a transition
    /// @b From one state and @b To another stored and referenced, and what @b Action is performed.
    /// @details This is not intended to be a base for inheritance, this is intended to be stored
    /// directly in a container inside an @ref StateMachine. That said if this is inherited from
    /// it ought to work as long as no new data members are added.
    class MEZZ_LIB StateTransition
    {
        private:
            /// @internal
            /// @brief The state this transition is coming from.
            HashedString32 From;
            /// @internal
            /// @brief The State this transition is going to.
            HashedString32 To;
            /// @internal
            /// @brief The action it takes to get @b From here @b To there.
            StateTransitionAction* Action;

        public:
            /// @brief State Transition Stealing constructor
            /// @param FromState Sets the state this transition is coming from.
            /// @param ToState Sets the state this transition is going to.
            /// @param OwningActionPointer An owning pointer (meaning this pointer will be deleted)
            /// and the calling code should forget about because this class will delete it) to any
            /// instance of a class derived from a @ref StateTransitionAction. This defaults to a
            /// new @ref StateTransitionNoAction, which does no action exactly like it sounds.
            StateTransition(const HashedString32& FromState,
                            const HashedString32& ToState,
                            StateTransitionAction* OwningActionPointer
                                = new StateTransitionNoAction());
            /// @brief Copy constructor
            /// @param CopiedTransition The other StateTransition to copy.
            StateTransition(const StateTransition& CopiedTransition);

            /// @brief Assignment Operator
            /// @param CopiedTransition The other StateTransition to copy.
            /// @return A reference to this StateTransition to allow operator chaining.
            StateTransition& operator= (const StateTransition& CopiedTransition);

            /// @brief A virtual destructor to not totally screw inheritance, because this is
            /// intended to be stored in a container directly in an @ref StateMachine, so any
            /// inheritance needs to avoid adding any new data members and preserve
            virtual ~StateTransition();

            /// @brief Determines sorting order, not useful for human readable sorting.
            /// @param Other the other StateTransition to work with.
            /// @details This compares the Hashes in the Other StateTransition to this one. It
            /// starts by comparing the value of the From State, then in the event of a tie it
            /// compares the values of the To state.
            Boole operator< (const StateTransition& Other) const;

            /// @brief Execute the StateTransitionAction.
            /// @return True if it executed completely.
            Boole operator()();

            /// @brief What is the From State?
            /// @return An immutable Reference to the From state.
            const HashedString32& FromState() const;
            /// @brief What is the To State?
            /// @return An immutable Reference to the To state.
            const HashedString32& ToState() const;
    };

    /// @brief If pointers to this must be stored, then this comparator can be used to get the same
    /// sorting results as without pointers.
    class MEZZ_LIB StateTransitionPointerSorter
    {
        public:
            Boole operator()(StateTransition* Left, StateTransition* Right);
    };
} // /namespace Mezzanine

#endif // Include guard
