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
#ifndef _threadtests_h
#define _threadtests_h

#include "main.h"

#include "dagframescheduler.h"

/// @file
/// @brief Basic tests of the thread class

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;

/// @brief used in some basic threading tests
Thread::id HelloID;
/// @brief Used to test the thread class in some Basic Threading test
void PrintHello(void*)
{
    cout << "Hello from thread T1 with id: " << Mezzanine::Threading::this_thread::get_id() << endl;
    HelloID = Mezzanine::Threading::this_thread::get_id();
    Mezzanine::Threading::this_thread::sleep_for(200000);
}

/// @brief Used to Basic Threading Passing
static Mezzanine::Integer ThreadPassTest=0;
/// @brief Used to Basic Threading Passing
static Mezzanine::Threading::Mutex ThreadPassLock;
/// @brief Used to Basic Threading Passing
/// @param Value The functionality being tested is that is passed correctly
void SquareInThread(void* Value)
{
    cout << "Thread T3 waiting for lock on mutex ThreadPassLock, thread has id: " << Mezzanine::Threading::this_thread::get_id() << endl;
    ThreadPassLock.Lock();
    cout << "Thread T3 locked mutex: " << endl;
    ThreadPassTest = *(Mezzanine::Integer*)Value * *(Mezzanine::Integer*)Value;
    cout << "Thread T3 work complete unlocking mutex: " << endl;
    ThreadPassLock.Unlock();
}


/// @brief Tests for the Thread class
class threadtests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "thread"
        virtual String Name()
            { return String("thread"); }

        /// @copydoc Mezzanine::Testing::UnitTestGroup::RunTests
        /// @detail Test if the thread works correctly were possible
        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            RunInteractiveTests = false; //prevent warnings

            if (RunAutomaticTests)
            {
                TestResult temp;
                HelloID = 0;
                Thread::id ActualID = 0;

                cout << "Testing Basic Thread functionality." << endl;
                cout << "This Threads id: " <<  Mezzanine::Threading::this_thread::get_id() << endl;

                cout << "Creating a thread with identifier T1 and unkown id." << endl;
                Mezzanine::Threading::Thread T1(PrintHello);
                cout << "T1 should have an id of: " << T1.get_id() << endl;
                ActualID = T1.get_id();

                cout << "Is T1 joinable: " << T1.joinable() << endl;
                if(T1.joinable())
                    { temp=Success; }
                else
                    { temp=Testing::Failed; } // In theory this could spuriosly fail but it seems unlikely with the 2/10 second pause in the thread
                AddTestResult("DAGFrameScheduler::Thread::joinable", temp);

                MaxInt BeginSleep = GetTimeStamp();
                cout << "Sleeping main thread for 300ms." << endl;
                Mezzanine::Threading::this_thread::sleep_for(300000);
                MaxInt SleepDuration = GetTimeStamp() - BeginSleep;
                if(SleepDuration>=300000)
                    { temp=Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::Thread::sleep_for", temp);

                cout << "Joining T1" << endl;
                T1.join();
                cout << "Is T1 joinable: " << T1.joinable() << endl;


                if(HelloID==ActualID)
                    { temp=Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::Thread::get_id", temp);

                cout << "Yielding thread to OS scheduler." << endl;
                Mezzanine::Threading::this_thread::yield();


            }else{
                AddTestResult("DAGFrameScheduler::Thread::get_id", Skipped);
                AddTestResult("DAGFrameScheduler::Thread::joinable", Skipped);
                AddTestResult("DAGFrameScheduler::Thread::sleep_for", Skipped);
            }


            if (RunAutomaticTests)
            {
                TestResult temp;

                cout << "Testing passing to thread functionality" << endl;
                cout << "Locking ThreadPassLock in thread: " << Mezzanine::Threading::this_thread::get_id() << endl;
                ThreadPassLock.Lock();

                cout << "Creating a thread with identifier T3 and unkown id." << endl;
                Mezzanine::Integer Value = 9;
                cout << "Passing " << Value << " into thread T3." << endl;
                Mezzanine::Threading::Thread T3(SquareInThread, &Value);

                cout << "Unlocking ThreadPassLock from main and sleeping for 300 ms." << endl;
                ThreadPassLock.Unlock();
                Mezzanine::Threading::this_thread::sleep_for(300000);

                ThreadPassLock.Lock();
                cout << "Thread gives us: " << ThreadPassTest << endl;
                cout << "Does the thread give us the square of what we passed it: " << (Value*Value == ThreadPassTest) << endl;
                if(Value*Value == ThreadPassTest)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::Thread::Passing", temp);
                ThreadPassLock.Unlock();

                cout << "Joining T3" << endl;
                T3.join();
            }else{
                AddTestResult("DAGFrameScheduler::Thread::Passing", Skipped);
            }


            /*if (RunInteractiveTests)
            {   
                AddTestResult("Threading::Interactive", Success
                              );
            }else{
                AddTestResult("Threading::Interactive", Skipped);
            }*/
        }
};

#endif

