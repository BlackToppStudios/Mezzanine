// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _threadlogtests_h
#define _threadlogtests_h

#include "mezztest.h"

#include "dagframescheduler.h"

#include <random>
#include <thread>

/// @file
/// @brief Basic tests of the ThreadLog and directly associated classes

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;


/// @brief Tests for the mutex class
class threadlogtests : public UnitTestGroup
{
    public:        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Tools for this test
        ////////////////////////////////////////

        /// @brief To insure that random components that show bugs can be duplicated.
        const Integer RandomSeed;

        /// @brief Needed to construct a const random seed.
        threadlogtests() : RandomSeed{ToInteger(time(0))}
            {}

        /// @brief Generate a random numer between Min and Max inclusively.
        /// @param Min The lower bound
        /// @param Max The upper bound
        /// @return a thread safe random number.
        Integer RandomInt(Integer Min, Integer Max) const
        {
            static thread_local std::mt19937 Generator(RandomSeed);
            std::uniform_int_distribution<int> Distribution(Min, Max);
            return Distribution(Generator);
        }

        /// @brief Creates a threadsafe random true or false
        /// @return True or false.
        Boole RandomBool() const
            { return static_cast<bool>(RandomInt(0,1)); }

        /// @brief 50% of the time pause the current thread for d10 nanoseconds.
        void RandomSleep() const
        {
            if(RandomBool()) // Some threads sleep between certain actions and some don't.
                { std::this_thread::sleep_for(std::chrono::nanoseconds(RandomInt(0,10))); }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Actual Tests
        ////////////////////////////////////////

        /// @brief Test that registering threads and retrieving ThreadIndexes work on the first thread
        void TestRegisterFirstThread()
        {
            // Test id is predictable when registering just one thread.
            const ThreadLog::ThreadIndexType FirstIndex = ThreadLog::RegisterThread();
            const ThreadLog::ThreadIndexType ExpectedFirstIndex{0};
            TEST(ExpectedFirstIndex == FirstIndex, "FirstThreadIndexRegistered");

            // Test registering a thread again
            const ThreadLog::ThreadIndexType FirstIndexAgain = ThreadLog::RegisterThread();
            TEST(ExpectedFirstIndex == FirstIndexAgain, "DoubleRegisterSafe");

            // Test RaceLog::GetThreadIndex()
            const ThreadLog::ThreadIndexType FirstIndexViaGet = ThreadLog::GetThreadIndex();
            TEST(ExpectedFirstIndex == FirstIndexViaGet, "GetIndex");

            // Leave no mess for the next test
            ThreadLog::ResetRegistry();
        }

        /// @brief Test that registering threads and retrieving ThreadIndexes work on multiple threads
        void TestRegisterThread()
        {
            // Now lets make a bunch of thrads thats that race to call The register function
            std::vector<std::thread> Threads; // Replace this with Threading::Thread when it doesn't require a rewrite.

            // A place for answers and a mutex to protect it.
            std::vector<ThreadLog::ThreadIndexType> IndexCheck;
            SpinLock CheckMutex;

            // Do some number of tests, because just one proves nothing with race conditions at risk.
            for(int TrialCounter = 0; TrialCounter < 100; TrialCounter++)
            {
                const int ThreadUpperLimitThisTest = TrialCounter+1;
                Threads.reserve(ThreadUpperLimitThisTest);
                IndexCheck.reserve(ThreadUpperLimitThisTest);

                // Creating a few threads, and one more each test to increase the variablilty of the timing.
                for(int ThreadCounter = 0; ThreadCounter < ThreadUpperLimitThisTest; ThreadCounter++)
                {
                    Threads.emplace_back([&CheckMutex, &IndexCheck, this]{
                        RandomSleep(); // Some threads sleep, some don't before registering
                        const ThreadLog::ThreadIndexType MyIndex = ThreadLog::RegisterThread();
                        RandomSleep(); // Some threads sleep, some don't before storing answer.

                        Threading::lock_guard<SpinLock> Gourd(CheckMutex); // Protect my threads with pumpkin helmets.
                        IndexCheck.push_back(MyIndex); // Store each threads Index in the index checker.
                    });
                }

                // Stop all the threads and clean up every but the index checking stuff.
                for(std:: thread& Thread : Threads)
                {
                    Thread.join();
                }
                Threads.clear();
                ThreadLog::ResetRegistry();

                // Sort the indexes and verify they are unique and sequential
                std::sort(IndexCheck.begin(), IndexCheck.end());
                ThreadIndex Expected{0};
                for(const ThreadLog::ThreadIndexType& OneIndex : IndexCheck)
                {
                    TEST(Expected == OneIndex, "ThreadIndexSeriality");
                    ++Expected;
                }
                IndexCheck.clear();
            }
        }

        /// @brief Verify that the various operator< work for LogEntry classes
        void TesLogEntrySorting()
        {
            {
                SingleThreadLogEntry First("First Sample Entry");
                std::this_thread::sleep_for(std::chrono::nanoseconds(1));
                SingleThreadLogEntry Second("Second Sample Entry");
                TEST(First < Second, "SortingSingleThreadLogEntry");


                AggregatedLogEntry DerivedSecond(ThreadIndex{0}, Second);
                TEST(Second.What == DerivedSecond.What, "CreateAggregatedFromSingleLogEntryWhat");
                TEST(Second.When == DerivedSecond.When, "CreateAggregatedFromSingleLogEntryWhen");
            }

            {
                AggregatedLogEntry First(ThreadIndex{0},"First Sample Entry");
                std::this_thread::sleep_for(std::chrono::nanoseconds(1));
                AggregatedLogEntry Second(ThreadIndex{0}, "Second Sample Entry");
                TEST(First < Second, "SortingAggregatedLogEntry");
            }
        }

        /// @brief Test the RaceLog::Log method directly skipping the intended API.
        void TestLoggingWithoutMacros()
        {
            // Prepare the logger, some test values and register the main thread.
            const Integer TestThreadCount = 10;
            ThreadLog::RegisterThread();
            ThreadLog::PrepareLogGroup(TestThreadCount + 1); // +1 for the main thread
            std::vector<std::thread> Threads;

            const char* Third = "Third";

            auto LoggingLambda = [&Third, this]{
                RandomSleep();
                ThreadLog::RegisterThread();
                RandomSleep();
                ThreadLog::Log("First");
                RandomSleep();
                ThreadLog::Log("Second");
                RandomSleep();
                ThreadLog::Log(Third);
                RandomSleep();
                ThreadLog::Log(__FILE__); // Fourth and File both start with an F

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
                TEST(String("First") == String(ThreadLog::GetLog(ThreadCounter)[0].What),
                     "FirstMessage");
                TEST(ThreadLog::GetLog(ThreadCounter)[0].When < ThreadLog::GetLog(ThreadCounter)[1].When,
                     "FirstBeforeSecond");

                TEST(String("Second") == String(ThreadLog::GetLog(ThreadCounter)[1].What),
                     "SecondMessage");
                TEST(ThreadLog::GetLog(ThreadCounter)[1].When < ThreadLog::GetLog(ThreadCounter)[2].When,
                     "SecondBeforeThird");

                TEST(String("Third") == String(ThreadLog::GetLog(ThreadCounter)[2].What),
                     "ThirdMessage");
                TEST(ThreadLog::GetLog(ThreadCounter)[2].When < ThreadLog::GetLog(ThreadCounter)[3].When,
                     "ThirdBeforeFourth");

                TEST(String(__FILE__) == String(ThreadLog::GetLog(ThreadCounter)[3].What),
                     "FourthMessage");
            }

            // Leave no mess for the next test
            ThreadLog::ResetRegistry();
        }

        /// @brief Test the log through a normal workflow
        /// @note This work is abnormal in that all the thread access is serialized.
        void TestLoggingAggregation()
        {
            // Prepare the log with the amount of threads we expect.
            ThreadLog::PrepareLogGroup(3);

            // This is the 0th thread and like any that logs it needs to be registered. This test uses the ThreadIndex
            // but your code can simply ignore it capture it as needed.
            const ThreadIndex CurrentThreadIndex{ThreadLog::RegisterThread()};
            const ThreadIndex FirstThreadIndex{CurrentThreadIndex + 1};
            const ThreadIndex SecondThreadIndex{CurrentThreadIndex + 2};

            ThreadLog::Log("First Entry");

            // We make a thread to log
            std::thread FirstThread([]{
                ThreadLog::RegisterThread();
                ThreadLog::Log("Second Entry");
                // Outside of simple tests your threads could be arbitrarily long and call to logs until they are
                // clear is called.
            });
            FirstThread.join();

            ThreadLog::Log("Third Entry");

            // Another thread to insure the Racelog is expandable to N threads.
            std::thread SecondThread([]{
                ThreadLog::RegisterThread();
                ThreadLog::Log("Fourth Entry");
            });
            SecondThread.join();
            ThreadLog::Log("Fifth Entry");

            // All logging must be complete before aggregating the logs from multiple threads into one log.
            const ThreadLog::AggregatedLogType Results{ThreadLog::AggregateLogs()};

            // Once you have the logs you can analyze them however you like, They will be sorted by time so you can see
            // how thread behaved with their connent interleaved.
            TEST(CurrentThreadIndex == Results[0].Where, "TrialFirst");
            TEST(String("First Entry") == String(Results[0].What), "TrialFirstAccuracy");

            TEST(FirstThreadIndex == Results[1].Where, "TrialSecond");
            TEST(String("Second Entry") == String(Results[1].What), "TrialSecongAccuracy");

            TEST(CurrentThreadIndex == Results[2].Where, "TrialThird");
            TEST(String("Third Entry") == String(Results[2].What), "TrialThirdAccuracy");

            TEST(SecondThreadIndex == Results[3].Where, "TrialFourth");
            TEST(String("Fourth Entry") == String(Results[3].What), "TrialFourthAccuracy");

            TEST(CurrentThreadIndex == Results[4].Where, "TrialFifth");
            TEST(String("Fifth Entry")== String(Results[4].What), "TrialFifthAccuracy");

            ThreadLog::ResetRegistry();
        }

        /// @brief Stress test the ThreadLog and check it for false sharing (must be done manually with results)
        void StressTest()
        {
            const int TestLogIterations{10000000};
            ThreadLog::RegisterThread();

            auto LoggingLambda = [&TestLogIterations]{
                ThreadLog::RegisterThread();
                for(int Counter{0}; Counter < TestLogIterations; Counter++)
                    { ThreadLog::Log("First"); }
            };

            using Clock = std::chrono::high_resolution_clock;

            // Lets capture a baseline
            ThreadLog::PrepareLogGroup(1);

            const std::chrono::time_point<Clock> StartBaseline{Clock::now()};
            LoggingLambda();
            const std::chrono::time_point<Clock> StopBaseline{Clock::now()};

            auto BaseLineDuration = StopBaseline - StartBaseline;
            std::cout << "Baseline time to execute: " << BaseLineDuration.count() << "ns" << std::endl;

            ThreadLog::ResetRegistry();

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
                    ThreadLog::PrepareLogGroup(TryItOnNThreads + 1);

                    // Start the timer and start launching threads.
                    const std::chrono::time_point<Clock> StartThreadedTest{Clock::now()};
                    for(unsigned int CreationCount = 1; CreationCount < TryItOnNThreads; CreationCount++)
                        { Threads.emplace_back(LoggingLambda); }
                    LoggingLambda();
                    for(std:: thread& Thread : Threads)
                        { Thread.join(); }
                    const std::chrono::time_point<Clock> StopThreadedTest{Clock::now()};
                    // After doing work on a few threads, including this one, synchronize everything again and stop the
                    // timer.

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
                    ThreadLog::ResetRegistry();
                }
            } else {
                std::cerr << "Hardware Concurrency not detected, skipping Stress test.\n";
            }
        }

        /// @brief Test that the streaming function work
        void TestStreaming()
        {

            // Let's do some simple logging
            ThreadLog::PrepareLogGroup(2);
            ThreadLog::RegisterThread();

            ThreadLog::Log("Frist Post");

            std::thread FirstThread([]{
                ThreadLog::RegisterThread();
                ThreadLog::Log("Second Post");
            });
            FirstThread.join();

            {
                // let's try printing those logs to a string stream
                std::stringstream TestStream;

                ThreadLog::PrintOneThreadsLog(TestStream, ThreadIndex{0});
                TEST(TestStream.str().find(String("Frist Post")) != std::string::npos, "LogPrinting");
                TestStream.str("");

                ThreadLog::PrintOneThreadsLog(TestStream);
                TEST(TestStream.str().find(String("Frist Post")) != std::string::npos, "LogPrintingDefaultsCorrectly");
                TestStream.str("");

                ThreadLog::PrintOneThreadsLog(TestStream, ThreadIndex{1});
                TEST(TestStream.str().find(String("Second Post")) != std::string::npos, "LogPrintingOtherThreads");
                TestStream.str("");
            }

            {
                // Try Printing the aggregated logs
                std::stringstream TestStream;

                ThreadLog::PrintAggregatedLog(TestStream);

                String::size_type Frist = TestStream.str().find(String("Frist Post"));
                String::size_type Second = TestStream.str().find(String("Second Post"));

                TEST( Frist != std::string::npos, "LogPrintingAggregatedFirst");
                TEST( Second != std::string::npos, "LogPrintingAggregatedSecond");
                TEST( Frist < Second, "LogPrintingAggregatedOrderingPreserved");
            }

            ThreadLog::ResetRegistry();
        }

        void TestMacros()
        {
            ThreadLog::PrepareLogGroup(1,1); // Forcibly erase logs

            // Let's do some the same simple logging other tests do, but with MACROS
            PREPARE_THREAD_LOG(2,10);
            REGISTER_THREAD;
            THREAD_LOG;

            // Log Aggregation and size check
            const ThreadLog::AggregatedLogType Results{ThreadLog::AggregateLogs()};

            #ifdef MEZZTHREADLOG
                TEST(Results.size() > 0, "LogsWhenEnabled");
            #else
                TEST(Results.size() == 0, "SkipsWhenDisabled");
            #endif
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Stuff the test suite needs
        ////////////////////////////////////////

        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "Mutex"
        virtual String Name()
            { return String("ThreadLog"); }

        /// @brief Launch a bunch of other tests
        virtual void RunAutomaticTests()
        {
            TestRegisterFirstThread();
            TestRegisterThread();
            TesLogEntrySorting();
            TestLoggingWithoutMacros();
            TestLoggingAggregation();
            StressTest();
            TestStreaming();
            TestMacros();
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }

        /// @todo Figure out to test printing of log on signal handlers on signal handlers.

};

#endif

