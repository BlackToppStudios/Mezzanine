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
#ifndef _sizestests_h
#define _sizestests_h

#include "mezztest.h"
#include "dagrandomtests.h"

#include "dagframescheduler.h"

#include <vector>

/// @file
/// @brief implementation of a premade group of sizeof calls

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;


/// @brief A simple pair of speeds for a given set
struct VecMapTestSpeed
{
    MaxInt VectorTime;
    MaxInt MapTime;
    Whole Insertions;

    /// @brief Initializing constructor
    /// @param VTime Defaults to 0, initial time for vector rum
    /// @param STime Defaults to 0, initial time for set rum
    VecMapTestSpeed(MaxInt VTime=0, MaxInt STime=0, Whole Inserts = 0)
        : VectorTime(VTime), MapTime(STime), Insertions(Inserts)
    {}

    double GetTimescale()
    {
        if(VectorTime!=0 && MapTime!=0)
            { return double(MapTime)/double(VectorTime); }
        return 0.0;
    }
};

/// @brief Performa a creation test and a read test for a Map and Vector
/// @param Insertions how many entries inserted on creation
/// @param Output Should anything be printed, where should it be printed
/// @return data on the test results
VecMapTestSpeed VecMapTest(Whole Insertions, ostream& Output = cout)
{
    VecMapTestSpeed Results(0,0,Insertions);
    MaxInt StartTime = GetTimeStamp();
    Whole DefeatCompilerOptimizer=0;

    Output << "Trying test with " << Insertions << " in each container." << endl;

    vector<Whole> RandomVec;
    StartTime = GetTimeStamp();
    for(Whole Counter=0; Counter<Insertions; Counter++)
        { RandomVec.insert(RandomVec.begin(), Dice2D3()); }
    Results.VectorTime = GetTimeStamp()-StartTime;
    Output << "Vector worst case create time: " << Results.VectorTime << endl;

    StartTime = GetTimeStamp();
    for(Whole Counter=0; Counter<Insertions; Counter++)
        { DefeatCompilerOptimizer=RandomVec[rand()%Insertions]; }
    Whole VecLookTime = GetTimeStamp()-StartTime;
    Results.VectorTime += VecLookTime;
    Output << "Vector rand lookup time: " << VecLookTime << endl;
    Output << "Output to defeat compiler optimization: " << DefeatCompilerOptimizer << endl;

    StartTime = GetTimeStamp();
    map<Whole,Whole> RandomMap;
    for(Whole Counter=0; Counter<Insertions; Counter++)
        { RandomMap[Counter] = Dice2D3(); }
    Results.MapTime = GetTimeStamp()-StartTime;
    DefeatCompilerOptimizer=RandomVec[rand()%Insertions];
    Output << "Map creation time: " << Results.MapTime << endl;
    Output << "Output to defeat compiler optimization: " << DefeatCompilerOptimizer << endl;

    StartTime = GetTimeStamp();
    for(Whole Counter=0; Counter<Insertions; Counter++)
        { DefeatCompilerOptimizer=RandomMap[rand()*Insertions]; }
    MaxInt MapLookTime = GetTimeStamp()-StartTime;
    Results.MapTime += MapLookTime;
    Output << "Map rand lookup time: " << MapLookTime << endl;
    Output << "Output to defeat compiler optimization: " << DefeatCompilerOptimizer << endl;

    Output << "Time Totals - Vector: " << Results.VectorTime << "   Map: " << Results.MapTime << endl << endl;

    return Results;
}

/// @brief heuristically searches for point where vectors with their lineat searches but cache friendliness slow to the point where a Maps scalabity catch up to it
/// @param Output Should anything be printed, where should it be printed
/// @return A whole near the cut off point.
Whole WhenIsVectorSlowerThanSet(ostream& Output = cout)
{
    double LastTestScale = 2.0;
    const double Delta = 0.01;
    Whole TestCount = 10;
    Whole FailAt = 500;
    Whole CloseToDelta = 3; // To prevent early false positives due to tiny spike in times.

    while( ((1.0+Delta)<LastTestScale || LastTestScale<(1.0-Delta)) || 0!=CloseToDelta)
    {
        if( ((1.0+Delta)<LastTestScale || LastTestScale<(1.0-Delta)) && 0!=CloseToDelta)
            { CloseToDelta--; }

        FailAt--;
        if (0==FailAt)
        {
            cout << "Could not find the break even point." << endl;
            return 0;
        }

        VecMapTestSpeed Current = VecMapTest(TestCount,Output);
        LastTestScale = Current.GetTimescale();
        if(LastTestScale >4.0)
            { TestCount *= 2; }
        if(LastTestScale >2.0)
            { TestCount *= 1.5; }

        if(LastTestScale >1.5)
            { TestCount += 1001; }
        if(0.8>LastTestScale )
            { TestCount *= 0.8; }

        if(LastTestScale >1.1)
            { TestCount += 98; }
        if(0.9>LastTestScale )
            { TestCount *= .9; }

        if(LastTestScale >(1.0+Delta))
            { TestCount += 9; }
        if((1.0+Delta)>LastTestScale )
            { TestCount *=.99; }

        if(TestCount<0)
            { return 0; }
    }
    Output << "Break even point where Maps become faster than Vectors is around " << TestCount << " elements on this system." << std::endl;
    return TestCount;
}

/// @brief Get sizes of common data structures becuase this information is so often required for  optimization
class dagsizestests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "DAGSizes"
        virtual String Name()
            { return String("DAGSizes"); }

        /// @brief Performs only basic Sanity tests on the assumptions the scheduler is built on.
        virtual void RunAutomaticTests()
        {
            cout << "Determining sizeof() important types that are used throughout:" << endl
                 << "iWorkUnit: " << sizeof(iWorkUnit) << endl
                 << "DefaultWorkUnit: " << sizeof(DefaultWorkUnit) << endl
                 << "LogAggregator: " << sizeof(LogAggregator) << endl
                 << "LogBufferSwapper: " << sizeof(LogBufferSwapper) << endl
                 << "WorkUnitKey: " << sizeof(WorkUnitKey) << endl
                 << "DefaultRollingAverage<Whole>::Type: " << sizeof(DefaultRollingAverage<Whole>::Type) << endl
                 << "WeightedRollingAverage<Whole,Whole>: " << sizeof(WeightedRollingAverage<Whole,Whole>) << endl
                 << "BufferedRollingAverage<Whole>: " << sizeof(BufferedRollingAverage<Whole>) << endl
                 << "WorkUnitMonpoly: " << sizeof(MonopolyWorkUnit) << endl
                 << "DefaultThreadSpecificStorage::Type: " << sizeof(DefaultThreadSpecificStorage::Type) << endl
                 << "FrameScheduler: " << sizeof(FrameScheduler) << endl
                 << "WorkSorter: " << sizeof(WorkSorter) << endl
                 << "thread: " << sizeof(Thread) << endl
                 << "mutex: " << sizeof(Mutex) << endl
                 << "Barrier: " << sizeof(Barrier) << endl
                 << "vector<Whole>: " << sizeof(vector<Whole>) << endl
                 << "vector<WorkUnit*>: " << sizeof(vector<Whole*>) << endl
                 << "set<WorkUnit*>: " << sizeof(set<iWorkUnit*>) << endl
                 << "std::vector<WorkUnitKey>::reverse_iterator: " << sizeof(std::vector<WorkUnitKey>::reverse_iterator) << endl
                 << "ostream*: " << sizeof(ostream*) << endl
                 << "MaxInt: " << sizeof(MaxInt) << endl
                 << "Whole: " << sizeof(Whole) << endl << endl;

            cout << "CPU cache size: " << GetCacheSize() << endl;
            Whole CachLineSize = GetCachelineSize();
            cout << "CPU cache line size: " << CachLineSize << endl;
            cout << "Checking Line size against frequently used types." << endl;
            TEST_WARN(sizeof(iWorkUnit)<CachLineSize,"iWorkUnitFitsInCacheLine");
            TEST_WARN(sizeof(DefaultWorkUnit)<CachLineSize,"DefaultWorkUnitFitsInCacheLine");
            TEST_WARN(sizeof(WorkUnitKey)<CachLineSize,"WorkUnitKeyFitsInCacheLine");
            TEST_WARN(sizeof(DefaultRollingAverage<Whole>::Type)<CachLineSize,"DefaultRollingAverageFitsInCacheLine");

            cout << "This algorithm presumes that vectors under a certain size are just faster than sets/trees/maps below that size. This checks where that stops being true." << endl;
            stringstream trash;
            Whole Count1 = WhenIsVectorSlowerThanSet(trash);
            cout << "Elemement count 1: " << Count1 << endl;
            Whole Count2 = WhenIsVectorSlowerThanSet(trash);
            cout << "Elemement count 2: " << Count2 << endl;
            Whole Count3 = WhenIsVectorSlowerThanSet(trash);
            cout << "Elemement count 3: " << Count3 << endl;
            cout << "Are These higher than 800? " << ((Count1+Count2+Count3)/3>800) << " - If not the DAG scheduling algorithm may need to be revised" << endl;
            TEST_WARN((Count1+Count2+Count3)/3>800,"VectorSpeed");
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }
};

#endif

