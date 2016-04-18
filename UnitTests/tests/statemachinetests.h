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
#ifndef statemachinetests_h
#define statemachinetests_h

#include "mezztest.h"

#include "statemachine.h"

/// @file
/// @brief Test the statemachine works

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief Test
class statemachinetests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "StateMachine"
    virtual String Name()
        { return String("StateMachine"); }

    /// @brief Test most features on the StateMachine.
    void RunAutomaticTests()
    {
        {
            StateMachine UnderTest;
            TEST( UnderTest.GetStateCount() == 0, "NoStartingStates");
            TEST( UnderTest.GetCurrentState().IsEmpty(), "NoDefaultState");
            TEST( UnderTest.GetPendingState().IsEmpty(), "NoDefaultPendingState");
        }

        {
            StateMachine UnderTest("starting");
            TEST( UnderTest.GetStateCount() == 1, "OneStartingStateSTD");
            TEST( UnderTest.GetCurrentState() == HashedString32("starting"), "StartingStateSTD");
            TEST( UnderTest.GetPendingState().IsEmpty(), "NoDefaultPendingStateSTD");
        }

        {
            StateMachine UnderTest(HashedString32("starting"));
            TEST( UnderTest.GetStateCount() == 1, "OneStartingStateHash");
            TEST( UnderTest.GetCurrentState() == HashedString32("starting"), "StartingStateHash");
            TEST( UnderTest.GetPendingState().IsEmpty(), "NoDefaultPendingStateHash");
        }

        {
            StateMachine UnderTest(HashedString32("starting"));
            TEST( UnderTest.GetStateCount() == 1, "OneStartingState");
            UnderTest.AddState("started");
            TEST( UnderTest.GetStateCount() == 2, "AddStateIncrementsSomething1");
            UnderTest.AddState("stopping");
            TEST( UnderTest.GetStateCount() == 3, "AddStateIncrementsSomething2");
        }

        {
            StateMachine StopLight("Green");
            TEST( StopLight.GetStateTransitionCount() == 0, "NoStartingTransitions1");
            StopLight.AddState("Yellow");
            StopLight.AddState("Red");
            StopLight.AddState("LeftArrow");
            TEST( StopLight.GetStateTransitionCount() == 0, "NoStartingTransitions2");

            StopLight.AddStateTransitation("Green","Yellow");
            TEST( StopLight.GetStateTransitionCount() == 1, "TransitionAdd1");
            StopLight.AddStateTransitation("Yellow","Red");
            StopLight.AddStateTransitation("Red","Green");
            StopLight.AddStateTransitation("Red","LeftArrow");
            StopLight.AddStateTransitation("LeftArrow","Green");
            TEST( StopLight.GetStateTransitionCount() == 5, "TransitionAdd2");

            TEST( StopLight.HasState("") == false, "DoesntHaveStateNilSTD");
            TEST( StopLight.HasState(HashedString32("")) == false, "DoesntHaveStateNilHash");
            TEST( StopLight.HasState("RightArrow") == false, "DoesntHaveStateRightArrowSTD");
            TEST( StopLight.HasState(HashedString32("RightArrow")) == false,
                  "DoesntHaveStateRightArrowHash");
            TEST( StopLight.HasState("LeftArrow") == true, "HasStateSTD");
            TEST( StopLight.HasState(HashedString32("LeftArrow")) == true, "HasStateHash");

            TEST( StopLight.HasStateTransition("Green","Red") == false,
                  "HasInvalidTransition1");
            TEST( StopLight.HasStateTransition("Yellow","Green") == false,
                  "HasInvalidTransition2");
            TEST( StopLight.HasStateTransition("Green","LeftArrow") == false,
                  "HasInvalidTransition3");
            TEST( StopLight.HasStateTransition(HashedString32("Green"),
                                               HashedString32("LeftArrow") ) == false,
                  "HasInvalidTransition4");
            TEST( StopLight.HasStateTransition("Green","Yellow") == true,
                  "HasValidTransition1");
            TEST( StopLight.HasStateTransition("Red","Green") == true,
                  "HasValidTransition2");
            TEST( StopLight.HasStateTransition("LeftArrow","Green") == true,
                  "HasValidTransition3");
            TEST( StopLight.HasStateTransition(HashedString32("LeftArrow"),
                                               HashedString32("Green") ) == true,
                  "HasValidTransition4");

            TEST( StopLight.GetCurrentState() == HashedString32("Green"), "QueryDoesntChangeState");

            TEST( StopLight.CanChangeState("Green") == false,
                  "CanChangeFail1");
            TEST( StopLight.CanChangeState(HashedString32("Green")) == false,
                  "CanChangeFail2");
            TEST( StopLight.CanChangeState("Red") == false,
                  "CanChangeFail3");
            TEST( StopLight.CanChangeState("LeftArrow") == false,
                  "CanChangeFail4");
            TEST( StopLight.CanChangeState("Yellow") == true,
                  "CanChangeWinSTD");
            TEST( StopLight.CanChangeState(HashedString32("Yellow")) == true,
                  "CanChangeWinHash");


            //TEST_THROW();

        }

        {
            StateMachine foo;
        }
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//spheretests

#endif
