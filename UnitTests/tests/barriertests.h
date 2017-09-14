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
#ifndef _barriertests_h
#define _barriertests_h

#include "mezztest.h"

#include "dagframescheduler.h"

/// @file
/// @brief Basic tests of the barrier synchronization primitive

using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;

using Clock = std::chrono::high_resolution_clock;

#include <thread>





/// @brief Tests for the WorkUnit class
class barriertests : public UnitTestGroup
{
    private:

        /// @brief A template version of the test to test multiple classes identically.
        /// @param ClassName Since we don't have simple introspection, passing the class name is an easy workaround
        /// @tparam BarrierType The barrier to be passed.
        template <typename BarrierType>
        Clock::duration TestBarrierClass(String ClassName)
        {
            Clock::time_point StartTime{Clock::now()};

            // Basic synchronization
            {
                TestOutput << "Testing Basic Barrier functionality on " << ClassName << "." << endl
                     << "This Thread's id: " <<  Mezzanine::Threading::this_thread::get_id() << endl
                     << "A group of data has been populated with 5,10,15 and 20, this should be doubled and then the "
                     << "total of this place into a new field of data and will be done by 4 threads." << endl;

                BarrierType TestBarrier(4);
                vector<Whole> BarrierData1 = {5, 10, 15, 20};
                vector<Whole> BarrierData2 = {0, 0, 0, 0};

                auto BarrierTestHelper = [&](Int32 Position)
                {
                    BarrierData1[Position] *= 2;
                    TestBarrier.Wait();
                    BarrierData2[Position] = BarrierData1[0] + BarrierData1[1] + BarrierData1[2] + BarrierData1[3];
                };

                std::thread T1(BarrierTestHelper, 0);
                std::thread T2(BarrierTestHelper, 1);
                std::thread T3(BarrierTestHelper, 2);
                std::thread T4(BarrierTestHelper, 3);
                T1.join();
                T2.join();
                T3.join();
                T4.join();

                TestOutput  << "Each thread should have 100, here is what they have:" << endl
                            << "1 - " << BarrierData2[0] << std::endl
                            << "2 - " << BarrierData2[1] << std::endl
                            << "3 - " << BarrierData2[2] << std::endl
                            << "4 - " << BarrierData2[3] << std::endl;

                TEST(100==BarrierData2[0], ClassName + "Thread1")
                TEST(100==BarrierData2[1], ClassName + "Thread2")
                TEST(100==BarrierData2[2], ClassName + "Thread3")
                TEST(100==BarrierData2[3], ClassName + "Thread4")
            }

            // Multi Iteration stress test.
            {
                const Int32 TestCount = 1000;
                BarrierType StartStressBarrier(5); // One more than threads launch because the main thread
                BarrierType EndStressBarrier(5);
                Int32 DataToCopy = 0;
                auto BarrierStressor = [&StartStressBarrier, &EndStressBarrier, &DataToCopy, TestCount]
                                       (std::vector<Int32>& Storage)
                {
                    for(Int32 Counter=0; Counter < TestCount; Counter++)
                    {
                        StartStressBarrier.Wait();
                        // Main Thread Changes data here
                        EndStressBarrier.Wait();
                        Storage.push_back(DataToCopy);
                    }
                };

                std::vector<Int32> StorageForZero;
                std::vector<Int32> StorageForOne;
                std::vector<Int32> StorageForTwo;
                std::vector<Int32> StorageForThree;

                std::thread T1(BarrierStressor, std::ref(StorageForZero));
                std::thread T2(BarrierStressor, std::ref(StorageForOne));
                std::thread T3(BarrierStressor, std::ref(StorageForTwo));
                std::thread T4(BarrierStressor, std::ref(StorageForThree));

                for(Int32 Counter=0; Counter < TestCount; Counter++)
                {
                    StartStressBarrier.Wait();
                    DataToCopy = 2 * Counter;
                    EndStressBarrier.Wait();
                    // Worker threads copy data after end barrier and before next start barrier
                }

                T1.join();
                T2.join();
                T3.join();
                T4.join();

                auto CheckResults = [TestCount] (std::vector<Int32>& StorageToCheck)
                {
                    Boole Passing = true;
                    Int32 ExpectedCountByTwo = 0;
                    for(Int32 Index=0; Index < TestCount; Index++)
                    {
                        Passing = Passing && (ExpectedCountByTwo == StorageToCheck[Index]);
                        //TestOutput << ExpectedCountByTwo << " == " << StorageForZero[Index] << std::endl;
                        ExpectedCountByTwo += 2;
                    }
                    return Passing;
                };

                TEST(CheckResults(StorageForZero),  ClassName + "ThreadCounterOffInThread0");
                TEST(CheckResults(StorageForOne),   ClassName + "ThreadCounterOffInThread1");
                TEST(CheckResults(StorageForTwo),   ClassName + "ThreadCounterOffInThread2");
                TEST(CheckResults(StorageForThree), ClassName + "ThreadCounterOffInThread3");
            }

            Clock::time_point EndTime{Clock::now()};
            return EndTime - StartTime;
        }

    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "Barrier"
        virtual String Name()
            { return String("Barrier"); }

        /// @brief Test if the barrier works properly
        void RunAutomaticTests()
        {
            std::chrono::nanoseconds SpinTime = TestBarrierClass<SpinBarrier>("SpinBarrier");
            std::chrono::nanoseconds BackgroundTime = TestBarrierClass<BackgroundBarrier>("BackgroundBarrier");

            TestOutput << '\n'
                       << "Spin Barrier Took       " << SpinTime.count() << "  ns.\n"
                       << "Background Barrier Took " << BackgroundTime.count() << "  ns.\n"
                       ;

        /// @todo Add test for changing thread count and possible another for a feature for checking the amoutn of
        /// waiting threads.
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }


};

#endif

