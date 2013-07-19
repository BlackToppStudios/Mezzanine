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
#ifndef _barriertests_h
#define _barriertests_h

#include "mezztest.h"

#include "dagframescheduler.h"

/// @file
/// @brief Basic tests of the barrier synchronization primitive

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;

/// @brief The Barrier instance to be used in the test 'barrier'
Barrier TestBarrier(4);

/// @brief Stores data for a set of barrier tests
vector<Whole> BarrierData1;
/// @brief Stores data for another set of barrier tests
vector<Whole> BarrierData2;


/// @brief A simple function to synchronize in the 'barrier' test
void BarrierTestHelper(void* ThreadID)
{
    Int32 Position = *((Int32*)ThreadID);
    stringstream Output;
    Output << "-------------------" << endl
           << "This is the thread with id: " <<  Mezzanine::Threading::this_thread::get_id() << endl
           << "For this test it requires the data in position: " << Position-1 << endl
           << "doubling data in position: " << Position%4 << endl;
    BarrierData1[Position-1] *= 2;
    cout << Output.str();
    Output.str("");


    //Mezzanine::Threading::this_thread::sleep_for(10000*DiceD20()); // just because the standard says streams need to synchronize output does not mean they actually output data in correct order, this lets cout catchup and makes guessing which thread makes it to the barrier last impossible
    if(TestBarrier.Wait())
    {
        Output << "-------------------" << endl
               << "This is the thread with id: " <<  Mezzanine::Threading::this_thread::get_id() << endl
               << "This thread broke the barrier" << endl
               << "Copy data in position:" << Position%4 << endl;
    }else{
        Output << "-------------------" << endl
               << "This is the thread with id: " <<  Mezzanine::Threading::this_thread::get_id() << endl
               << "This thread waited for another to break it." << endl
               << "Copy data in position: " << Position%4 << endl;
    }
    BarrierData2[Position%4]=BarrierData1[Position%4];
    Output << "Data: " << BarrierData2[Position%4] << endl;

    cout << Output.str();
    Output.str("");
}

/// @brief Tests for the WorkUnit class
class barriertests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "Barrier"
        virtual String Name()
            { return String("Barrier"); }

        /// @copydoc Mezzanine::Testing::UnitTestGroup::RunTests
        /// @detail Test if the barrier works properly
        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            RunInteractiveTests = false; //prevent warnings

            if (RunAutomaticTests)
            {
                TestResult temp;

                cout << "Testing Basic Thread Barrier functionality." << endl
                     << "This Threads id: " <<  Mezzanine::Threading::this_thread::get_id() << endl
                     << "A group of data has been populated with 5,10,15 and 20, this should be doubled and copied into a new field of data and will be done by 4 threads. Each thread will be indexed, and will adjust the data from some other thread then synchronize and copy its own data." << endl;

                Int32 One = 1;
                Int32 Two = 2;
                Int32 Three = 3;
                Int32 Four = 4;
                BarrierData1.push_back(5);
                BarrierData1.push_back(10);
                BarrierData1.push_back(15);
                BarrierData1.push_back(20);
                BarrierData2.push_back(0);
                BarrierData2.push_back(0);
                BarrierData2.push_back(0);
                BarrierData2.push_back(0);

                Mezzanine::Threading::Thread T1(BarrierTestHelper,&One);
                Mezzanine::Threading::Thread T2(BarrierTestHelper,&Two);
                Mezzanine::Threading::Thread T3(BarrierTestHelper,&Three);
                Mezzanine::Threading::Thread T4(BarrierTestHelper,&Four);
                T1.join();
                T2.join();
                T3.join();
                T4.join();

                if(10==BarrierData2[0])
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::BarrierThread1", temp);
                if(20==BarrierData2[1])
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::BarrierThread2", temp);
                if(30==BarrierData2[2])
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::BarrierThread3", temp);
                if(40==BarrierData2[3])
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::BarrierThread4", temp);

            }else{
                AddTestResult("DAGFrameScheduler::BarrierThread1", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::BarrierThread2", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::BarrierThread3", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::BarrierThread4", Testing::Skipped);
            }


        }
};

#endif

