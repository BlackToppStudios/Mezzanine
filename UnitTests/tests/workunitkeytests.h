//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _workunitkeytests_h
#define _workunitkeytests_h

#include "main.h"

#include "dagframescheduler.h"
#include "workunittests.h"

/// @file
/// @brief Basic tests of the workunitkeys class

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;

/// @brief Tests for the WorkUnitket class, the sort of workunits
class workunitkeytests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "workunitkey"
        virtual String Name()
            { return String("workunitkey"); }

        /// @copydoc Mezzanine::Testing::UnitTestGroup::RunTests
        /// @detail Test if the WorkUnit
        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            RunInteractiveTests = false; //prevent warnings

            if (RunAutomaticTests)
            {
                TestResult temp;

                cout << "Creating a number of WorkUnitKeys and tesing their ability to sort: " << endl;
                cout << "\t" << "                   Deps, Time, Ptr" << endl;
                cout << "\t" << "WorkUnitKey First(   10, 500,  0 ); //Units with the most dependants get priority" << endl;
                cout << "\t" << "WorkUnitKey Second(   5, 600,  0 ); //Then Units that take more time" << endl;
                cout << "\t" << "WorkUnitKey Third(    5, 500,  0 ); " << endl;
                cout << "\t" << "WorkUnitKey Fourth(   3, 500,  1 ); //In case of a tie use higher pointer" << endl;
                cout << "\t" << "WorkUnitKey Fifth(    3, 500,  0 );" << endl;
                WorkUnitKey First(10,500,0);
                WorkUnitKey Second(5,600,0);
                WorkUnitKey Third(5,500,0);
                WorkUnitKey Fourth(3,500,(iWorkUnit*)1);
                WorkUnitKey Fifth(3,500,0);
                cout << "Second<First: " << (Second < First) << "\t Third<First: " << (Third < First) << "\t Fourth<First: " << (Fourth < First) << "\t Fifth<First: " << (Fifth < First) << endl;
                if(Second < First)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Second<First", temp);
                if(Third < First)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Third<First", temp);
                if(Fourth < First)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Fourth<First", temp);
                if(Fifth < First)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Fifth<First", temp);
                cout << "Third<Second: " << (Third < Second) << "\t Fourth<Second: " << (Fourth < Second) << "\t Fifth<Second: " << (Third < Second) << endl;
                if(Third < Second)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Third<Second", temp);
                if(Fourth < Second)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Fourth<Second", temp);
                if(Fifth < Second)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Fifth<Second", temp);
                cout << "Fourth<Third: " << (Fourth < Third) << "\t Fifth<Third: " << (Fifth < Third) << endl;
                if(Fourth < Third)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Fourth<Third", temp);
                if(Fifth < Third)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Fifth<Third", temp);
                if(Fifth < Fourth)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Fifth<Fourth", temp);

                cout << "First<Second: " << (First < Second) << "\t First<Third: " << (First < Third) << "\t First<Fourth: " << (First < Fourth) << "\t First<Fifth: " << (First < Fifth) << endl;
                if(!(First < Second))
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(First<Second)", temp);
                if(!(First < Third))
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(First<Third)", temp);
                if(!(First < Fourth))
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(First<Fourth)", temp);
                if(!(First < Fifth))
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(First<Fifth)", temp);
                cout << "Second<Third: " << (Second < Third) << "\t Second<Fourth: " << (Second < Fourth) << "\t Second<Fifth: " << (Second < Fifth) << endl;
                if(!(Second < Third))
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(Second<Third)", temp);
                if(!(Second < Fourth))
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(Second<Fourth)", temp);
                if(!(Second < Fifth))
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(Second<Fifth)", temp);
                cout << "Third<Fourth: " << (Third < Fourth) << "\t Third<Fifth: " << (Third < Fifth) << endl;
                if(!(Third < Fourth))
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(Third<Fourth)", temp);
                if(!(Third < Fifth))
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(Third<Fifth)", temp);
                cout << "Fourth<Fifth: " << (Fourth<Fifth) << endl;
                if(!(Fourth < Fifth))
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(Fourth<Fifth)", temp);

                cout << "Creating 4 WorkUnits for a sorting test with an set (be the only differrence between fourth and fifth was the address of the workunit, and we cannot control that.):" << endl;
                PiMakerWorkUnit *WorkUnitK1 = new PiMakerWorkUnit(500,"First",false);
                PiMakerWorkUnit *WorkUnitK2 = new PiMakerWorkUnit(500,"Second",false);
                PiMakerWorkUnit *WorkUnitK3 = new PiMakerWorkUnit(500,"Third",false);
                PiMakerWorkUnit *WorkUnitK4 = new PiMakerWorkUnit(500,"Fourth",false);
                First.Unit=WorkUnitK1;
                Second.Unit=WorkUnitK2;
                Third.Unit=WorkUnitK3;
                Fourth.Unit=WorkUnitK4;
                set<WorkUnitKey> WorkUnitKeyTest;
                WorkUnitKeyTest.insert(Second);
                WorkUnitKeyTest.insert(Fourth);
                WorkUnitKeyTest.insert(Third);
                WorkUnitKeyTest.insert(First);
                set<WorkUnitKey>::reverse_iterator Iter=WorkUnitKeyTest.rbegin();
                cout << ((PiMakerWorkUnit*)(Iter->Unit))->Name << " ";
                if(((PiMakerWorkUnit*)(Iter->Unit))->Name == String("First"), "Unit.Name==First")
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::First", temp);
                Iter++;
                cout << ((PiMakerWorkUnit*)(Iter->Unit))->Name << " ";
                if(((PiMakerWorkUnit*)(Iter->Unit))->Name == String("Second"), "Unit.Name==Second")
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Second", temp);
                Iter++;
                cout << ((PiMakerWorkUnit*)(Iter->Unit))->Name << " ";
                if(((PiMakerWorkUnit*)(Iter->Unit))->Name == String("Third"), "Unit.Name==Third")
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Third", temp);
                Iter++;
                cout << ((PiMakerWorkUnit*)(Iter->Unit))->Name << " ";
                if(((PiMakerWorkUnit*)(Iter->Unit))->Name == String("Fourth"), "Unit.Name==Fourth")
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Fourth", temp);
                Iter++;
                cout << endl;

                delete WorkUnitK1; delete WorkUnitK2; delete WorkUnitK3; delete WorkUnitK4;

            }else{
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Second<First", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Third<First", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Fourth<First", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Fifth<First", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Third<Second", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Fourth<Second", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Fifth<Second", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Fourth<Third", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Fifth<Third", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Fifth<Fourth", Testing::Skipped);

                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(First<Second)", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(First<Third)", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(First<Fourth)", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(First<Fifth)", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(Second<Third)", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(Second<Fourth)", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(Second<Fifth)", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(Third<Fourth)", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(Third<Fifth)", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::!(Fourth<Fifth)", Testing::Skipped);

                AddTestResult("DAGFrameScheduler::WorkUnitKey::First", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Second", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Third", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::WorkUnitKey::Fourth", Testing::Skipped);

            }


        }
};

#endif

