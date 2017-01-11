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


class StateTransitionActionChecker : public StateTransitionAction
{
        Boole& Transitioned;
    public:

        // Dammit I hate out parameters
        StateTransitionActionChecker(Boole& BooleToWrite) : Transitioned(BooleToWrite)
            {}

        virtual Boole operator ()()
            { return Transitioned = true;}

        virtual StateTransitionActionChecker* clone()
            { return new StateTransitionActionChecker(Transitioned); }
};


class StateTransitionActionFail : public StateTransitionAction
{
    public:
        virtual Boole operator ()()
            { return false;}

        virtual StateTransitionActionFail* clone()
            { return new StateTransitionActionFail; }
};


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

            StopLight.AddStateTransition("Green","Yellow");
            TEST( StopLight.GetStateTransitionCount() == 1, "TransitionAdd1");
            StopLight.AddStateTransition(HashedString32("Yellow"),HashedString32("Red"));
            StopLight.AddStateTransition("Red","Green");
            StopLight.AddStateTransition("Red","LeftArrow");
            StopLight.AddStateTransition("LeftArrow","Green");
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

            TEST( StopLight.SetPendingState(HashedString32("Red")) == false,
                  "SetPendingInvalidHash");
            TEST( StopLight.SetPendingState("Red") == false, "SetPendingInvalidSTD");
            TEST( StopLight.SetPendingState(HashedString32("Yellow")) == true,
                  "SetPendingValidHash");
            TEST( StopLight.GetPendingState() == HashedString32("Yellow"), "PendingSetHash")
            TEST( StopLight.SetPendingState("Yellow") == true, "SetPendingValidSTD");
            TEST( StopLight.GetPendingState() == HashedString32("Yellow"), "PendingSetSTD")


            TEST( StopLight.ChangeState("Red") == false, "WontStupidChange")
            TEST( StopLight.GetCurrentState() == HashedString32("Green"), "StateUnchanged");
            TEST( StopLight.GetPendingState() == HashedString32("Yellow"), "PendingUnchanged");

            TEST( StopLight.ChangeState("Yellow") == true, "ChangedStateToYellow")
            TEST( StopLight.GetCurrentState() == HashedString32("Yellow"), "ActuallyChangeState");
            TEST( StopLight.GetPendingState() == HashedString32(""), "PendingClearedAfterChange")

            TEST( StopLight.SetPendingState("Red") == true, "SetPendingValidForUse");
            StopLight.DoPendingStateChange();
            TEST( StopLight.GetCurrentState() == HashedString32("Red"), "ChangeToPendingState");
            TEST( StopLight.GetPendingState() == HashedString32(""),
                "PendingClearedAfterPendingChange");

        }

        {
            StateMachine windowsMachine;
            windowsMachine.AddState("Off");
            windowsMachine.AddState("Booting");
            windowsMachine.AddState("Running");
            windowsMachine.AddState("Shutdown");
            windowsMachine.AddState("Crashing");

            TEST(windowsMachine.GetCurrentState() == HashedString32("Off"), "StartsOff");

            Boole OffToBootTransitioned = false;
            StateTransitionActionChecker* OffToBoot =
                    new StateTransitionActionChecker(OffToBootTransitioned);
            Boole BootToRunningTransitioned = false;
            StateTransitionActionChecker* BootToRunning =
                    new StateTransitionActionChecker(BootToRunningTransitioned);
            Boole RunningToShutdownTransitioned = false;
            StateTransitionActionChecker* RunningToShutdown =
                    new StateTransitionActionChecker(RunningToShutdownTransitioned);
            Boole ShutdownToOffTransitioned = false;
            StateTransitionActionChecker* ShutdownToOff =
                    new StateTransitionActionChecker(ShutdownToOffTransitioned);

            windowsMachine.AddStateTransition(HashedString32("Off"),
                                                HashedString32("Booting"),
                                                OffToBoot);

            windowsMachine.AddStateTransition("Booting","Running",BootToRunning);
            windowsMachine.AddStateTransition(HashedString32("Booting"),
                                                HashedString32("Crashing"));

            windowsMachine.AddStateTransition("Running","Crashing");
            windowsMachine.AddStateTransition(HashedString32("Running"),
                                                HashedString32("Shutdown"),
                                                RunningToShutdown);

            windowsMachine.AddStateTransition("Shutdown","Crashing");
            windowsMachine.AddStateTransition(HashedString32("Shutdown"),
                                                HashedString32("Off"),
                                                ShutdownToOff);

            windowsMachine.AddStateTransition("Crashing","Off");

            TEST(OffToBootTransitioned == false && BootToRunningTransitioned == false &&
                 RunningToShutdownTransitioned == false && ShutdownToOffTransitioned == false,
                 "TransitionSanityCheck");

            TEST(windowsMachine.ChangeState("Booting") == true, "BootMachine");
            TEST(OffToBootTransitioned == true, "BootingFunctorCalled");

            TEST(BootToRunningTransitioned == false &&
                 RunningToShutdownTransitioned == false && ShutdownToOffTransitioned == false,
                 "OnlyOneStateRun");

            TEST(windowsMachine.SetPendingState("Running") == true, "SetToRunMachine");
            windowsMachine.DoPendingStateChange();
            TEST(BootToRunningTransitioned== true, "StartedRunningFunctorCalled");
            TEST(RunningToShutdownTransitioned == false && ShutdownToOffTransitioned == false,
                 "OnlyTheSecondStateRun");

        }

        {
            StateMachine UnderTest(HashedString32("Running"));
            UnderTest.AddState("CantStop");

            UnderTest.AddStateTransition( HashedString32("Running"),
                                          HashedString32("CantStop"),
                                          new StateTransitionActionFail);

            TEST(UnderTest.CanChangeState(HashedString32("CantStop")), "TransitionAppearsValid");
            TEST(UnderTest.ChangeState(HashedString32("CantStop")) == false,
                  "FailingTransitionInterupts");
            TEST(UnderTest.GetCurrentState() == HashedString32("Running"), "StillRunning");
            TEST(UnderTest.GetPendingState() == HashedString32(""), "NoPendingAfterFail");

            TEST(UnderTest.SetPendingState(HashedString32("CantStop")), "FutureAppearsValid");
            TEST(UnderTest.DoPendingStateChange()==false, "PendingCanFail");
            TEST(UnderTest.ChangeState(HashedString32("CantStop")) == false,
                  "FailingTransitionInteruptsFuture");
            TEST(UnderTest.GetPendingState() == HashedString32(""), "NoPendingAfterFutureFail");

        }

    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//spheretests

#endif

