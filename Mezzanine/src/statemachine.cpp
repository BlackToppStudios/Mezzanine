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
#ifndef statemachine_cpp
#define statemachine_cpp

/// @file
/// @brief Describe file here

#include "statemachine.h"

namespace
{
    Mezzanine::HashedString32 NoState("");
}

namespace Mezzanine
{
    StateMachine::StateMachine()
    {
        CurrentState = States.end();
        FutureStateTransition = Transitions.end();
    }

    StateMachine::~StateMachine()
    {
        for(TranstionIterator tran = Transitions.begin(); tran != Transitions.end(); tran++)
        { delete *tran; }
    }

    StateMachine::StateMachine(const HashedString32& InitialState) //: CurrentState(InitialState)
    {
        States.add(InitialState);
        CurrentState = States.begin();
        FutureStateTransition = Transitions.end();
    }

    StateMachine::StateMachine(const String& InitialState) //: CurrentState(InitialState)
    {
        States.add(InitialState);
        CurrentState = States.begin();
        FutureStateTransition = Transitions.end();
    }

    void StateMachine::AddState(const HashedString32& NewState)
    {
        HashedString32 LastCurrentState = *CurrentState;
        States.add(NewState);
        if(LastCurrentState.IsEmpty())
            { CurrentState = States.begin(); }
        else
            { CurrentState = States.find(LastCurrentState); }

    }

    void StateMachine::AddState(const String& NewState)
        { AddState(HashedString32(NewState));}

    void StateMachine::AddStateTransitation(const HashedString32& From,
                                            const HashedString32& To,
                                            StateTransitionAction* PossibleAction)
    {
        Transitions.add(new StateTransition(From, To, PossibleAction));
    }

    void StateMachine::AddStateTransitation(const String& From,
                                            const String& To,
                                            StateTransitionAction* PossibleAction)
    { AddStateTransitation(HashedString32(From), HashedString32(To), PossibleAction); }

    Boole StateMachine::HasState(const HashedString32& PossibleState) const
        { return States.find(PossibleState) != States.end(); }

    Boole StateMachine::HasState(const String& PossibleState) const
        { return HasState(HashedString32(PossibleState)); }

    Boole StateMachine::HasStateTransition(const HashedString32& FromState,
                                           const HashedString32& ToState) const
    {
        StateTransition Needle(FromState,ToState);
        return Transitions.find(&Needle) != Transitions.end();
    }

    Boole StateMachine::HasStateTransition(const String& FromState, const String& ToState) const
        { return HasStateTransition(HashedString32(FromState),HashedString32(ToState)); }

    Boole StateMachine::CanChangeState(const HashedString32& ToState)
        { return HasStateTransition(*CurrentState, ToState); }

    Boole StateMachine::CanChangeState(const String& ToState)
        { return CanChangeState(HashedString32(ToState)); }

    Boole StateMachine::ChangeState(const HashedString32& ToState)
    {
        ConstStateIterator FoundState = States.find(ToState);
        if(FoundState == States.end())
            { return false; }

        StateTransition Needle(*CurrentState, ToState);
        TranstionIterator FoundTransition
                = Transitions.find(&Needle);
        if(FoundTransition == Transitions.end())
            { return false; }

        (*FoundTransition)->operator()();
        CurrentState = FoundState;
        ClearPendingState();
        return true;
    }

    Boole StateMachine::ChangeState(const String& ToState)
        { return ChangeState(HashedString32(ToState)); }

    Boole StateMachine::SetPendingState(const HashedString32& ToState)
    {
        if(!HasState(ToState))
            { return false; }

        StateTransition Needle(*CurrentState, ToState);
        TranstionIterator FoundTransition
                = Transitions.find(&Needle);
        if(FoundTransition == Transitions.end())
            { return false; }
        FutureStateTransition = FoundTransition;
        return true;
    }

    Boole StateMachine::SetPendingState(const String& ToState)
        { return SetPendingState(HashedString32(ToState)); }

    void StateMachine::ClearPendingState()
        { FutureStateTransition = Transitions.end(); }

    void StateMachine::DoPendingStateChange()
    {

    }

    const HashedString32& StateMachine::GetCurrentState() const
    {
        if(CurrentState!=States.end())
            { return *CurrentState; }
        else
            { return ::NoState; }
    }

    const HashedString32& StateMachine::GetPendingState() const
    {
        if(FutureStateTransition!=Transitions.end())
            { return (*FutureStateTransition)->ToState(); }
        else
            { return ::NoState; }
    }

    Whole StateMachine::GetStateCount() const
        { return States.size(); }

    Whole StateMachine::GetStateTransitionCount() const
        { return Transitions.size(); }

} // /namespace Mezzanine

#endif // Include guard

