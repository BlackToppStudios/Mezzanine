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
#ifndef _randomtests_h
#define _randomtests_h

#include "mezztest.h"

#include "dagframescheduler.h"

#include <cstdlib>
#include <iostream>

/// @file
/// @brief Random number are used to aid in the testing of some heuristics, this tests those random number facilities

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief Simulate a 20 sided die
/// @details Used in @ref RandomTests
/// @return A Mezzanine::Whole containing a a random number between 1 and 20 inclusive with equal chance.
Mezzanine::Whole DiceD20()
    { return rand()%20+1; }

/// @brief Simulate 2x 3 sided die being added
/// @details Used in @ref RandomTests
/// @return A Mezzanine::Whole containing a a random number between 2 and 6 inclusive with a bell curve probability. This actually generates 2 numbers between 1 and 3 inclusive adds them, then returns that.
Mezzanine::Whole Dice2D3()
    { return ((rand()%3) + (rand()%3) + 2); }

/// @brief The 'random' Test.
/// @details This tests nothing in the library itself, but good quality random numbers are required for stressing the Scheduler in other tests
class dagrandomtests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "DAGRandom"
        virtual String Name()
            { return String("DAGRandom"); }

        /// @brief Tests the random number facility used in other tests.
        void RunAutomaticTests()
        {
            cout << endl << "Starting random number generation tests. Not part of the library, but required for testing." << endl;
            Mezzanine::MaxInt Timestamp1 = Mezzanine::GetTimeStamp();
            srand((int)Timestamp1);
            Whole TestRuns = 10000000;
            vector<Whole> D20Rolls;
            vector<Whole> D2d3Rolls;
            vector<Whole> D20Histogram;
            vector<Whole> D2d3Histogram;

            cout <<  "Doing " << TestRuns << " iterations of generatining numbers with each function, and initializing random seed with " << Timestamp1 << "." << endl;
            for (Whole Counter=0; Counter<TestRuns; ++Counter)
            {
                D20Rolls.push_back(DiceD20());
                D2d3Rolls.push_back(Dice2D3());
            }

            cout << "Preparing counts of the rolls." << endl;
            // init some 0s
            for (Whole Counter=0; Counter<=21; ++Counter) //providing a small amount of buffer space on either side in case dice algorithm has off by one issue.
                { D20Histogram.push_back(0); }
            for (Whole Counter=0; Counter<=7; ++Counter)
                { D2d3Histogram.push_back(0); }

            // count the actual rolls
            for(vector<Whole>::iterator Iter=D20Rolls.begin(); Iter!=D20Rolls.end(); ++Iter)
                { D20Histogram[*Iter]++; }
            for(vector<Whole>::iterator Iter=D2d3Rolls.begin(); Iter!=D2d3Rolls.end(); ++Iter)
                { D2d3Histogram[*Iter]++; }

            bool EvenDist = true;
            bool HistDist = true;

            // display chart
            cout << "D20 rolls should be distributed evenly between 1 and 20 inclusive:" << endl;
            for (Whole Counter=1; Counter<=20; ++Counter) //providing a small amount of buffer space on either side in case dice algorithm has off by one issue.
            {
                double Output = (((double)D20Histogram[Counter])/((double)TestRuns))*100;
                cout << Counter << ": " << D20Histogram[Counter] << " \t" << Output << "% " << endl;
                if(Output<4.9 || 5.1<Output )
                    { EvenDist = false; }
            }

            cout << "2d3 rolls should be distributed on a bell curve 2 and 6 inclusive:" << endl;
            for (Whole Counter=0; Counter<=7; ++Counter)
                { cout << Counter << ": " << D2d3Histogram[Counter] << " \t" << (((double)D2d3Histogram[Counter])/((double)TestRuns))*100 << "% " <<  endl; }
            if(D2d3Histogram[2]<1100000 || 1200000<D20Histogram[2] )
                { HistDist = false; }
            if(D2d3Histogram[3]<2200000 || 2300000<D20Histogram[3] )
                { HistDist = false; }
            if(D2d3Histogram[4]<3300000 || 3400000<D20Histogram[4] )
                { HistDist = false; }
            if(D2d3Histogram[5]<2200000 || 2300000<D20Histogram[5] )
                { HistDist = false; }
            if(D2d3Histogram[6]<1100000 || 1200000<D20Histogram[6] )
                { HistDist = false; }

            TEST_WARN(EvenDist, "DAGFrameScheduler::Random::D20");
            TEST_WARN(HistDist, "DAGFrameScheduler::Random::2D3");


        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }
};

#endif

