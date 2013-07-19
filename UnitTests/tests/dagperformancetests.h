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
#ifndef _dagperformancetests_h
#define _dagperformancetests_h

#include "mezztest.h"

#include "dagframescheduler.h"
#include "dagrandomtests.h"

#include "workunittests.h"

#include <limits>
#include <numeric>

/// @file
/// @brief Basic tests for the performance of DAGFrameScheduler components

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;

/// @brief Tests the performance of specific components
class dagperformancetests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "DAGPerformance"
        virtual String Name()
            { return String("DAGPerformance"); }

        /// @brief Converts anything streamable to a Mezzanine::String
        /// @details Used in @ref ThreadCreate
        /// @param Datum the item to convert
        /// @return a string containing the lexographically casted data
        template <typename T>
        Mezzanine::String ToString(T Datum)
        {
            stringstream Converter;
            Converter << Datum;
            return Converter.str();
        }

        /// @brief This is called when Automatic tests are run
        void RunAutomaticTests()
        {
            {// \Variance tests
                cout << "Creating a few Schedulers with work units and testing a variety of framerates timing accuracies." << endl;
                vector<Whole> Rates;
                Rates.push_back(10);
                Rates.push_back(25);
                Rates.push_back(28);
                Rates.push_back(30);
                Rates.push_back(60);
                Rates.push_back(100);
                BufferedRollingAverage<double> VarianceTotal(Rates.size());

                for(vector<Whole>::iterator Iter = Rates.begin(); Iter!=Rates.end(); ++Iter)
                {
                    stringstream LogCache;
                    cout << "Creating a Scheduler with only one work unit " << *Iter << " Frame Per Second running " << *Iter << " frames. " << endl;
                    FrameScheduler TimingTest(&LogCache,1);
                    PiMakerWorkUnit* WorkUnitTT1 = new PiMakerWorkUnit(50,"ForeverAlone",false);
                    TimingTest.AddWorkUnitMain(WorkUnitTT1);
                    TimingTest.SetFrameRate(*Iter);
                    MaxInt TimingTestStart = GetTimeStamp();
                    for(Whole Counter=0; Counter<*Iter; ++Counter)
                    {
                        TimingTest.DoOneFrame();
                    }
                    MaxInt TimingTestEnd = GetTimeStamp();
                    Whole TestLength = TimingTestEnd-TimingTestStart;
                    cout << "  " << *Iter << " Frames took " << TestLength << " microseconds to run, should be around 1000000 (one million)." << endl;
                    Integer Error = TestLength - 1000000;
                    Error = (Error>0.0) ? Error : -Error;
                    double Variance = (double(Error))/double(1000000) * 100;
                    cout << "  " << "This is a variance of " << Error << " Frames or " << Variance << "%. Which is " << endl;
                    VarianceTotal.Insert(Variance);
                    TEST_WARN(3>Variance,"TimingVariance3%");       // Allow a 3% variance - Inconsistent - achievable even on even on winxp with its crappy 3500 microsecond timer
                    TEST_WARN(0.1>Variance,"TimingVariance0.1%");   // Allow a .1% variance - This is very achievable with an accurate microsecond timer
                }
                cout << "Average Variance: " << VarianceTotal.GetAverage() << "%" << endl;
            } // \Variance tests

            { //Max framerate
                cout << "|Testing the FrameScheduler with a framrate of 0 to see max performance in a fixed number of frames: " << endl;
                vector<Whole> Durations;
                Durations.push_back(10);
                Durations.push_back(25);
                Durations.push_back(28);
                Durations.push_back(30);
                Durations.push_back(60);
                Durations.push_back(100);
                Durations.push_back(200);
                Durations.push_back(300);
                Durations.push_back(400);
                Durations.push_back(500);
                Durations.push_back(600);
                Durations.push_back(700);
                Durations.push_back(800);
                Durations.push_back(900);
                Durations.push_back(1000);
                Durations.push_back(2000);
                Durations.push_back(3000);
                Durations.push_back(4000);
                Durations.push_back(5000);
                Durations.push_back(6000);
                Durations.push_back(7000);
                Durations.push_back(8000);
                Durations.push_back(9000);
                Durations.push_back(10000);
                Durations.push_back(20000);
                Durations.push_back(30000);
                Durations.push_back(40000);
                Durations.push_back(50000);
                Durations.push_back(60000);
                Durations.push_back(70000);
                Durations.push_back(80000);
                Durations.push_back(90000);
                Durations.push_back(100000);
                Durations.push_back(200000);
                Durations.push_back(300000);
                Durations.push_back(400000);
                Durations.push_back(500000);
                Durations.push_back(600000);
                Durations.push_back(700000);
                Durations.push_back(800000);
                Durations.push_back(900000);
                Durations.push_back(1000000);
                Durations.push_back(10000000);
                Durations.push_back(100000000);

                Whole EmptyMaxFR = 0;
                Whole EmptyMinFR = std::numeric_limits<Whole>::max();
                std::vector<Whole> EmptyResults;

                Whole OneMaxFR = 0;
                Whole OneMinFR = std::numeric_limits<Whole>::max();
                std::vector<Whole> OneResults;

                Whole ChainMaxFR = 0;
                Whole ChainMinFR = std::numeric_limits<Whole>::max();
                std::vector<Whole> ChainResults;

                for(vector<Whole>::iterator Iter = Durations.begin(); Iter!=Durations.end(); ++Iter)
                {
                    stringstream LogCache;
                    cout << "Creating a Scheduler with a variety of WorkUnits running at full speed. " << endl;
                    FrameScheduler TimingTest(&LogCache,1);
                    TimingTest.SetFrameRate(0);
                    MaxInt TimingTestStart = GetTimeStamp();
                    for(Whole Counter=0; Counter<*Iter; ++Counter)
                        { TimingTest.DoOneFrame(); }
                    MaxInt TimingTestEnd = GetTimeStamp();
                    Whole TestLength = TimingTestEnd-TimingTestStart;
                    Whole FrameRate = double(*Iter)/(double(TestLength)/double(1000000));
                    cout << "  " << *Iter << " Empty Frames took " << TestLength << " microseconds to run, which is " << FrameRate << " frames per second." << endl;
                    if(FrameRate>EmptyMaxFR)
                        { EmptyMaxFR = FrameRate; }
                    if(FrameRate<EmptyMinFR)
                        { EmptyMinFR = FrameRate; }
                    EmptyResults.push_back(FrameRate);
                    if(3000000<TestLength)
                        { cout << "Single Test longer than three seconds, bailing from other performace tests" << endl; break; }

                    //WorkUnit* WorkUnitTT2 = new PausesWorkUnit(0,"ForeverAlone");
                    iWorkUnit* WorkUnitTT2 = new PiMakerWorkUnit(0,"ForeverAlone",false);
                    TimingTest.AddWorkUnitMain(WorkUnitTT2);
                    TimingTestStart = GetTimeStamp();
                    for(Whole Counter=0; Counter<*Iter; ++Counter)
                        { TimingTest.DoOneFrame(); }
                    TimingTestEnd = GetTimeStamp();
                    TestLength = TimingTestEnd-TimingTestStart;
                    FrameRate = double(*Iter)/(double(TestLength)/double(1000000));
                    cout << "  " << *Iter << " Single WorkUnit Frames took " << TestLength << " microseconds to run, which is " << FrameRate << " frames per second." << endl;
                    if(FrameRate>OneMaxFR)
                        { OneMaxFR = FrameRate; }
                    if(FrameRate<OneMinFR)
                        { OneMinFR = FrameRate; }
                    OneResults.push_back(FrameRate);
                    if(3000000<TestLength)
                        { cout << "Single Test longer than three seconds, bailing from other performace tests" << endl; break; }

                    //WorkUnit* WorkUnitTT2A = new PausesWorkUnit(0,"ForeverAlone");
                    iWorkUnit* WorkUnitTT2A = new PiMakerWorkUnit(0,"A",false);
                    iWorkUnit* WorkUnitTT2B = new PiMakerWorkUnit(0,"B",false);
                    iWorkUnit* WorkUnitTT2C = new PiMakerWorkUnit(0,"C",false);
                    TimingTest.AddWorkUnitMain(WorkUnitTT2A);
                    TimingTest.AddWorkUnitMain(WorkUnitTT2B);
                    TimingTest.AddWorkUnitMain(WorkUnitTT2C);
                    WorkUnitTT2C->AddDependency(WorkUnitTT2B);
                    WorkUnitTT2B->AddDependency(WorkUnitTT2A);
                    TimingTest.SortWorkUnitsAll();
                    TimingTestStart = GetTimeStamp();
                    for(Whole Counter=0; Counter<*Iter; ++Counter)
                        { TimingTest.DoOneFrame(); }
                    TimingTestEnd = GetTimeStamp();
                    TestLength = TimingTestEnd-TimingTestStart;
                    FrameRate = double(*Iter)/(double(TestLength)/double(1000000));
                    cout << "  " << *Iter << " Frames with the previous and an extra dependency set (A->B->C) took " << TestLength << " microseconds to run, which is " << FrameRate << " frames per second." << endl;
                    if(FrameRate>ChainMaxFR)
                        { ChainMaxFR = FrameRate; }
                    if(FrameRate<ChainMinFR)
                        { ChainMinFR = FrameRate; }
                    ChainResults.push_back(FrameRate);
                    if(3000000<TestLength)
                        { cout << "Single Test longer than three seconds, bailing from other performance tests" << endl; break; }
                    cout << endl;
                }

                std::vector<String> Output;
                Output.push_back("|");
                Output.push_back("Min");
                Output.push_back("Mean");
                Output.push_back("Max");
                Output.push_back("|Empty");
                Output.push_back(ToString(EmptyMinFR));
                Output.push_back(ToString(std::accumulate<>(EmptyResults.begin(),EmptyResults.end(),0)/EmptyResults.size()));
                Output.push_back(ToString(EmptyMaxFR));
                Output.push_back("|One");
                Output.push_back(ToString(OneMinFR));
                Output.push_back(ToString(std::accumulate<>(OneResults.begin(),OneResults.end(),0)/OneResults.size()));
                Output.push_back(ToString(OneMaxFR));
                Output.push_back("|Chain");
                Output.push_back(ToString(ChainMinFR));
                Output.push_back(ToString(std::accumulate<>(ChainResults.begin(),ChainResults.end(),0)/ChainResults.size()));
                Output.push_back(ToString(ChainMaxFR));

                cout << "Scheduler timings for X frames in any second:" << endl;

                Whole ColumnWidth=14;
                Whole ColumnCount=4;
                Whole WhichColumn=0;
                String CurrentOutput;
                for(std::vector<String>::iterator Iter=Output.begin(); Iter!=Output.end(); ++Iter)
                {
                    if(!(WhichColumn % ColumnCount))
                    {
                        WhichColumn=0;
                        cout << endl << "  ";
                    }
                    WhichColumn++;

                    for(CurrentOutput = *Iter; CurrentOutput.size()<ColumnWidth; CurrentOutput += " ") {}
                    cout << CurrentOutput;
                }
                cout << endl << endl;

            } // \Max framerate

        }

        /// @brief Since RunAutomaticTests implements no tests this does not true
        /// @return false
        virtual bool HasAutomaticTests() const
            { return false; }
};

#endif

