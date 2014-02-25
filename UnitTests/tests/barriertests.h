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
    Int32 Position = (*((Int32*)ThreadID)); // The place to get data from
    BarrierData1[Position] *= 2;
    TestBarrier.Wait();
    BarrierData2[Position]=BarrierData1[0]+BarrierData1[1]+BarrierData1[2]+BarrierData1[3];
}

/// @brief Tests for the WorkUnit class
class barriertests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "Barrier"
        virtual String Name()
            { return String("Barrier"); }

        /// @brief Test if the barrier works properly
        void RunAutomaticTests()
        {
            TestOutput << "Testing Basic Thread Barrier functionality." << endl
                 << "This Threads id: " <<  Mezzanine::Threading::this_thread::get_id() << endl
                 << "A group of data has been populated with 5,10,15 and 20, this should be doubled and then the total of this place into a new field of data and will be done by 4 threads." << endl;

            Int32 One = 0;
            Int32 Two = 1;
            Int32 Three = 2;
            Int32 Four = 3;
            BarrierData1.push_back(5);
            BarrierData1.push_back(10);
            BarrierData1.push_back(15);
            BarrierData1.push_back(20);
            BarrierData2.push_back(0);
            BarrierData2.push_back(0);
            BarrierData2.push_back(0);
            BarrierData2.push_back(0);

            Mezzanine::Threading::Thread T1(BarrierTestHelper, &One);
            Mezzanine::Threading::Thread T2(BarrierTestHelper, &Two);
            Mezzanine::Threading::Thread T3(BarrierTestHelper, &Three);
            Mezzanine::Threading::Thread T4(BarrierTestHelper, &Four);
            T1.join();
            T2.join();
            T3.join();
            T4.join();

            TestOutput << "Each thread should have 100, here is what they have:" << endl
                       << "1 - " << BarrierData2[0] << endl
                       << "2 - " << BarrierData2[1] << endl
                       << "3 - " << BarrierData2[2] << endl
                       << "4 - " << BarrierData2[3] << endl;

            TEST(100==BarrierData2[0], "BarrierThread1")
            TEST(100==BarrierData2[1], "BarrierThread2")
            TEST(100==BarrierData2[2], "BarrierThread3")
            TEST(100==BarrierData2[3], "BarrierThread4")
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }


};

#endif

