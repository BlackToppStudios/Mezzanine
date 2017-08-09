// The DAGFrameScheduler is a Multi-Threaded lock free and wait free scheduling library.
// © Copyright 2010 - 2016 BlackTopp Studios Inc.
/* This file is part of The DAGFrameScheduler.

    The DAGFrameScheduler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The DAGFrameScheduler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The DAGFrameScheduler.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'doc' folder. See 'gpl.txt'
*/
/* We welcome the use of the DAGFrameScheduler to anyone, including companies who wish to
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
#include "AggregatedLogEntry.h"
#include "SingleThreadLogEntry.h"
#include "ThreadIndex.h"
#include "RaceLog.h"

#include <cstdlib>
#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <random>
#include <mutex>
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Declarations
////////////////////////////////////////

// Types
using String = std::string;

// Globals... Ick.
int RandomSeed; // This should be const, it is set in main and only read after that.

// Some colors for making output slightly prettier
const std::string Red       ("\x1b[31m");   // A red suitable for ANSI console output
const std::string Green     ("\x1b[32m");   // A green suitable for ANSI console output
const std::string Reset     ("\x1b[0m");    // Reset a console back to its defaults

// Prototypes for "test suite" and tools
void Test(bool Condition, String Message);
template<typename Type> void TestEqual(Type Expected, Type Actual, String Message);

// Random Numbers
int RandomInt(int Min, int Max);
bool RandomBool();
void RandomSleep();

// Prototypes for tests
void TestRegisterFirstThread();
void TestRegisterThread();
void TesLogEntrySorting();
void TestLoggingWithoutMacros();
void TestLoggingAggregation(); // most realistic test
void StressTest();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The test runner accepts no arguments and just tests.
////////////////////////////////////////
int main()
{
    try
    {
        // Some simple setup
        std::cout << std::boolalpha;
        RandomSeed = time(0); //To use a previous random seed changes this.
        std::cout << "Random seed is: " << RandomSeed << std::endl;

        // List of tests to run.
        TestRegisterFirstThread();
        TestRegisterThread();
        TesLogEntrySorting();
        TestLoggingWithoutMacros();
        TestLoggingAggregation();
        StressTest();

        // Cleanup should be pretty minimal.
        std::cout << "All Tests " << Green << "Passed" << Reset << "!" << std::endl;
        return EXIT_SUCCESS;
    }
    catch(std::exception& e) // Test failures are signaled by exception with error message in the #what.
    {
        std::cout << "At least some Tests " << Red << "Failed" << Reset << "!" << std::endl;
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implementation of test suite.
////////////////////////////////////////
void Test(bool Condition, std::string Message)
{
    if(!Condition)
    {
        throw std::runtime_error(Message.c_str());
    }
}

template<typename Type> void TestEqual(Type Expected, Type Actual, String Message)
{
    std::stringstream StringMaker;
    StringMaker << Message << "\nExpected \"" << Expected << "\" to equal \"" << Actual << "\", but it didn't.\n";

    Test(Expected == Actual, StringMaker.str() );
}

int RandomInt(int Min, int Max)
{
    static thread_local std::mt19937 Generator(RandomSeed);
    std::uniform_int_distribution<int> Distribution(Min, Max);
    return Distribution(Generator);
}

bool RandomBool()
{
    return static_cast<bool>(RandomInt(0,1));
}

void RandomSleep()
{
    if(RandomBool()) // Some threads sleep between certain actions and some don't.
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(RandomInt(0,10)));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implementation of tests
////////////////////////////////////////
void TestRegisterFirstThread()
{
    std::cout << "Starting TestRegisterFirstThread" << std::endl;

    // Test id is predictable when registering just one thread.
    const RaceLog::ThreadIndexType FirstIndex = RaceLog::RegisterThread();
    const RaceLog::ThreadIndexType ExpectedFirstIndex{0};
    TestEqual(ExpectedFirstIndex, FirstIndex, "The first thread index was wrong.");

    // Test registering a thread again
    const RaceLog::ThreadIndexType FirstIndexAgain = RaceLog::RegisterThread();
    TestEqual(ExpectedFirstIndex, FirstIndexAgain, "Registering First thread again had unexpected results.");

    // Test RaceLog::GetThreadIndex()
    const RaceLog::ThreadIndexType FirstIndexViaGet = RaceLog::GetThreadIndex();
    TestEqual(ExpectedFirstIndex, FirstIndexViaGet, "Getting first thread ID and it was wrong.");

    // Leave no mess for the next test
    RaceLog::ResetRegistry();
    std::cout << "TestRegisterFirstThread " << Green << "Passed" << Reset << "." << std::endl;
}

void TestRegisterThread()
{
    std::cout << "Starting TestRegisterThread" << std::endl;

    // Now lets make a bunch of thrads thats that race to call The register function
    std::vector<std::thread> Threads;

    // A place for answers and a mutex to protect it.
    std::vector<RaceLog::ThreadIndexType> IndexCheck;
    std::mutex CheckMutex;

    // Do some number of tests, because just one proves nothing with race conditions at risk.
    for(int TrialCounter = 0; TrialCounter < 100; TrialCounter++)
    {
        const int ThreadUpperLimitThisTest = TrialCounter+1;
        Threads.reserve(ThreadUpperLimitThisTest);
        IndexCheck.reserve(ThreadUpperLimitThisTest);

        // Creating a few threads, and one more each test to increase the variablilty of the timing.
        for(int ThreadCounter = 0; ThreadCounter < ThreadUpperLimitThisTest; ThreadCounter++)
        {
            Threads.emplace_back([&CheckMutex, &IndexCheck]{
                RandomSleep(); // Some threads sleep, some don't before registering
                const RaceLog::ThreadIndexType MyIndex = RaceLog::RegisterThread();
                RandomSleep(); // Some threads sleep, some don't before storing answer.

                std::lock_guard<std::mutex> Gourd(CheckMutex); // I protect my threads with pumpkin helmets.
                IndexCheck.push_back(MyIndex); // Store each threads Index in the index checker.
            });
        }

        // Stop all the threads and clean up every but the index checking stuff.
        for(std:: thread& Thread : Threads)
        {
            Thread.join();
        }
        Threads.clear();
        RaceLog::ResetRegistry();

        // Sort the indexes and verify they are unique and sequential
        std::sort(IndexCheck.begin(), IndexCheck.end());
        ThreadIndex Expected{0};
        for(const RaceLog::ThreadIndexType& OneIndex : IndexCheck)
        {
            TestEqual(Expected, OneIndex, "Error with Thread Index uniqueness or Seriality.");
            ++Expected;
        }
        IndexCheck.clear();
    }

    std::cout << "TestRegisterThread " << Green << "Passed" << Reset << "." << std::endl;
}

void TesLogEntrySorting()
{
    std::cout << "Starting TesLogEntrySorting" << std::endl;

    {
        SingleThreadLogEntry First("First Sample Entry");
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        SingleThreadLogEntry Second("Second Sample Entry");
        Test(First < Second, "Default time capture or comparison failed for SingleThreadLogEntry");


        AggregatedLogEntry DerivedSecond(ThreadIndex{0}, Second);
        Test(Second.What == DerivedSecond.What, "Copying error in message pointer");
        Test(Second.When == DerivedSecond.When, "Copying error in timestamp");
    }

    {
        AggregatedLogEntry First(ThreadIndex{0},"First Sample Entry");
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        AggregatedLogEntry Second(ThreadIndex{0}, "Second Sample Entry");
        Test(First < Second, "Default time capture or comparison failed for AggregatedLogEntry");
    }

    std::cout << "TesLogEntrySorting " << Green << "Passed" << Reset << "." << std::endl;
}

void TestLoggingWithoutMacros()
{
    std::cout << "Starting TestLoggingWithoutMacros" << std::endl;

    // Prepare the logger, some test values and register the main thread.
    const int TestThreadCount = 10;
    RaceLog::RegisterThread();
    RaceLog::PrepareLogGroup(TestThreadCount + 1); // +1 for the main thread
    std::vector<std::thread> Threads;

    const char* Third = "Third";

    auto LoggingLambda = [&Third]{
        RandomSleep();
        RaceLog::RegisterThread();
        RandomSleep();
        RaceLog::Log("First");
        RandomSleep();
        RaceLog::Log("Second");
        RandomSleep();
        RaceLog::Log(Third);
        RandomSleep();
        RaceLog::Log(__FILE__); // Fourth and File both start with an F

    };

    // On each thread including this one, log all the things and do it all together.
    for(int ThreadCounter = 0; ThreadCounter < TestThreadCount; ThreadCounter++)
        { Threads.emplace_back(LoggingLambda); }
    LoggingLambda();

    // Clean up our threads.
    for(std:: thread& Thread : Threads)
        { Thread.join(); }

    // Test that each log contains what it should in the order it should
    for(int ThreadCounter = 0; ThreadCounter < TestThreadCount; ThreadCounter++)
    {
        TestEqual(String("First"),
                  String(RaceLog::GetLog(ThreadCounter)[0].What),
                 "First message on thread incorrect");
        Test(RaceLog::GetLog(ThreadCounter)[0].When < RaceLog::GetLog(ThreadCounter)[1].When,
             "First and second items logged for thread are not in chronoligical order.");

        TestEqual(String("Second"),
                  String(RaceLog::GetLog(ThreadCounter)[1].What),
                 "Second message on thread incorrect");
        Test(RaceLog::GetLog(ThreadCounter)[1].When < RaceLog::GetLog(ThreadCounter)[2].When,
             "Second and Third items logged for thread are not in chronoligical order.");

        TestEqual(String("Third"),
                  String(RaceLog::GetLog(ThreadCounter)[2].What),
                 "Third message on thread incorrect");
        Test(RaceLog::GetLog(ThreadCounter)[2].When < RaceLog::GetLog(ThreadCounter)[3].When,
             "Third and fourth items logged for thread are not in chronoligical order.");

        TestEqual(String(__FILE__),
                  String(RaceLog::GetLog(ThreadCounter)[3].What),
                 "Fourth message on thread incorrect");
    }

    // Leave no mess for the next test
    RaceLog::ResetRegistry();
    std::cout << "TestLoggingWithoutMacros " << Green << "Passed" << Reset << "." << std::endl;
}

void TestLoggingAggregation()
{
    std::cout << "Starting TestLoggingAggregation" << std::endl;

    // Prepare the log with the amount of threads we expect.
    RaceLog::PrepareLogGroup(3);

    // This is the 0th thread and like any that logs it needs to be registered. This test uses the ThreadIndex but
    // your code can simply ignore it capture it as needed.
    const ThreadIndex CurrentThreadIndex{RaceLog::RegisterThread()};
    const ThreadIndex FirstThreadIndex{CurrentThreadIndex + 1};
    const ThreadIndex SecondThreadIndex{CurrentThreadIndex + 2};

    RaceLog::Log("First Entry");

    // We make a thread to log
    std::thread FirstThread([]{
        RaceLog::RegisterThread();
        RaceLog::Log("Second Entry");
        // Outside of simple tests your threads  could be arbitrarily long and call to logs until they are cleared.
    });
    FirstThread.join(); // This test happens to be serialized, but the log is safe to call from multiple threads.

    RaceLog::Log("Third Entry");

    // Another thread to insure the Racelog is expandable to N threads.
    std::thread SecondThread([]{
        RaceLog::RegisterThread();
        RaceLog::Log("Fourth Entry");
    });
    SecondThread.join();
    RaceLog::Log("Fifth Entry");

    // All logging must be complete before aggregating the logs from multiple threads into one log.
    const RaceLog::AggregatedLogType Results{RaceLog::AggregateLogs()};

    // Once you have the logs you can analyze them however you like, They will be sorted by time so you can see how
    // thread behaved with their connent interleaved.
    TestEqual(CurrentThreadIndex, Results[0].Where, "First message did not happen on main thread?!");
    TestEqual(String("First Entry"), String(Results[0].What), "First message was not accurate.");

    TestEqual(FirstThreadIndex, Results[1].Where, "Second message did not happen on First thread?!");
    TestEqual(String("Second Entry"), String(Results[1].What), "First message was not accurate.");

    TestEqual(CurrentThreadIndex, Results[2].Where, "Third message did not happen on First thread?!");
    TestEqual(String("Third Entry"), String(Results[2].What), "Third message was not accurate.");

    TestEqual(SecondThreadIndex, Results[3].Where, "Fourth message did not happen on First thread?!");
    TestEqual(String("Fourth Entry"), String(Results[3].What), "Fourth message was not accurate.");

    TestEqual(CurrentThreadIndex, Results[4].Where, "Fifth message did not happen on First thread?!");
    TestEqual(String("Fifth Entry"), String(Results[4].What), "Fifth message was not accurate.");

    RaceLog::ResetRegistry();
    std::cout << "TestLoggingAggregation " << Green << "Passed" << Reset << "." << std::endl;
}

void StressTest()
{
    std::cout << "Starting StressTest" << std::endl;
    const int TestLogIterations{10000000};
    RaceLog::RegisterThread();

    auto LoggingLambda = [&TestLogIterations]{
        RaceLog::RegisterThread();
        for(int Counter{0}; Counter < TestLogIterations; Counter++)
            { RaceLog::Log("First"); }

    };

    using Clock = std::chrono::high_resolution_clock;

    // Lets capture a baseline
    RaceLog::PrepareLogGroup(1);

    const std::chrono::time_point<Clock> StartBaseline{Clock::now()};
    LoggingLambda();
    const std::chrono::time_point<Clock> StopBaseline{Clock::now()};

    auto BaseLineDuration = StopBaseline - StartBaseline;
    std::cout << "Baseline time to execute: " << BaseLineDuration.count() << "ns" << std::endl;


    RaceLog::ResetRegistry();

    // Lets try it on as many threads as the hardware supports
    const unsigned int ThreadCount{std::thread::hardware_concurrency()};
    std::cout << "Detected " << ThreadCount << " Hardware threads, filling them with work." << std::endl;

    // Do it on several theads to see how bad it starts slowing down
    if(1 < ThreadCount)
    {
        // Try the stress test at increasing amounts of threads up to what the hardware claims it can supprt
        for(unsigned int TryItOnNThreads{0}; TryItOnNThreads < ThreadCount; TryItOnNThreads++)
        {
            // setup to prepare for threads.
            std::vector<std::thread> Threads;
            Threads.reserve(TryItOnNThreads);
            RaceLog::PrepareLogGroup(TryItOnNThreads + 1);

            // Start the timer and start launching threads.
            const std::chrono::time_point<Clock> StartThreadedTest{Clock::now()};
            for(unsigned int CreationCount = 1; CreationCount < TryItOnNThreads; CreationCount++)
                { Threads.emplace_back(LoggingLambda); }
            LoggingLambda();
            for(std:: thread& Thread : Threads)
                { Thread.join(); }
            const std::chrono::time_point<Clock> StopThreadedTest{Clock::now()};
            // After doing work on a few threads, including this one, synchronize everything again and stop the timer.

            // Lets figure out how many times slower this was that single threaded despite doing 2x or 3x or Nx
            // as much work. Then lets display it.
            auto ThreadedTestDuration = StopThreadedTest - StartThreadedTest;
            std::cout << "Threaded(" << (TryItOnNThreads+1) << ") test time execute: "
                      << ThreadedTestDuration.count() << "ns" << std::endl;

            const auto SlowDown = static_cast<long double>(ThreadedTestDuration.count())
                                    / static_cast<long double>(BaseLineDuration.count());
            //const auto SpeedUp = static_cast<long double>(ThreadCount) / SlowDown;
            std::cout << "\tSlowdown compared to one thread: " << SlowDown << std::endl;
            //std::cout << "\tSpeedup: " << SpeedUp << std::endl;

            // Reset for the next run
            RaceLog::ResetRegistry();
        }
    } else {
        std::cerr << "Hardware Concurrency not detected, skipping Stress test.\n";
    }

    std::cout << "StressTest " << Green << "Passed" << Reset << "." << std::endl;
}
