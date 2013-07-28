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
#ifndef _frameschedulertests_h
#define _frameschedulertests_h

#include "mezztest.h"

#include "dagframescheduler.h"
#include "workunittests.h"
#include "pugixml.h"

/// @file
/// @brief Test the core Framescheduler

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;

/// @brief A samplework unit that that just block the thread it is in
/// @details Used in @ref FrameSchedulerGetNext and other tests
/// @warning Everything on these samples has a public access specifier, for production code that is poor form, encapsulate your stuff.
class PausesWorkUnit : public DefaultWorkUnit
{
    public:
        /// @brief How many milliseconds should this thread block for.
        Mezzanine::Whole Length;

        /// @brief Name used for clarifying output and enhancing logging.
        Mezzanine::String Name;

        /// @brief Create one of these workunit
        /// @param Length_ Defaults to 50 and is the number of Milliseconds to wait
        /// @param Name Defaults to "Default" only used in output logs
        PausesWorkUnit(Mezzanine::Whole Length_ = 50, Mezzanine::String Name_ = "Default")
            : Length(Length_), Name(Name_)
            { }

        /// @brief Empty Virtual Deconstructor
        virtual ~PausesWorkUnit()
            { }

        /// @brief Wait and log it.
        /// @param CurrentThreadStorage used to retrieve a valid logger.
        /// @brief CurrentFrameScheduler ignored
        virtual void DoWork(DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
        {
            DoubleBufferedLogger& CurrentLogger = CurrentThreadStorage.GetResource<DoubleBufferedLogger>(DBRLogger);
            CurrentLogger.GetUsable() << "<Pause PauseLength=\"" << Length << "\" WorkUnitName=\"" << Name << "\" ThreadID=\"" << Mezzanine::Threading::this_thread::get_id() << "\" />" << endl;
            Mezzanine::Threading::this_thread::sleep_for(Length);
        }
};

/// @brief a monopoly version of the pausing workunit
class PauseMonopoly : public MonopolyWorkUnit
{
    public:
        /// @brief How many milliseconds should this thread block for.
        Mezzanine::Whole Length;

        /// @brief Name used for clarifying output and enhancing logging.
        Mezzanine::String Name;

        /// @brief Create one of these workunit
        /// @param Length_ Defaults to 50 and is the number of Milliseconds to wait
        /// @param Name Defaults to "Default" only used in output logs
        PauseMonopoly(Mezzanine::Whole Length_ = 50, Mezzanine::String Name_ = "Default")
            : Length(Length_), Name(Name_)
            { }

        /// @brief Virtual Deconstructor
        virtual ~PauseMonopoly()
            {}

        /// @brief Swallows whatever value is passed
        virtual void UseThreads(const Whole&)
            {}

        /// @brief Sleeping does not require any extra threads.
        /// @return 1
        virtual Whole UsingThreadCount()
            { return 1; }

        /// @brief Wait and log it.
        /// @param CurrentThreadStorage used to retrieve a valid logger.
        /// @brief CurrentFrameScheduler ignored
        virtual void DoWork(DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
        {
            DoubleBufferedLogger& CurrentLogger = CurrentThreadStorage.GetResource<DoubleBufferedLogger>(DBRLogger);
            CurrentLogger.GetUsable() << "<Pause PauseLength=\"" << Length << "\" WorkUnitName=\"" << Name << "\" ThreadID=\"" << Mezzanine::Threading::this_thread::get_id() << "\" />" << endl;
            Mezzanine::Threading::this_thread::sleep_for(Length);
        }
};

/// @brief Used in @ref ThreadRestart
class RestartMetric
{
    public:
        /// @brief when the given unit started
        String UnitStart;

        /// @brief when the given unit completed
        String UnitEnd;

        /// @brief name of the given unit
        String Name;

        /// @brief The thread the unit ran in.
        String Threadid;
};

/// @brief Used to easily output a work unit metrics
/// @details Used in @ref ThreadCreate
ostream& operator<< (ostream& out, RestartMetric Lhs)
{
    out << "Name: " << Lhs.Name << " \tStarted: " << Lhs.UnitStart << " \tEnded: " << Lhs.UnitEnd << " \tThread: " << Lhs.Threadid;
    return out;
}


/// @brief Perform Basic tests of the functions to acquire resources
class LoggerCheckWorkUnit : public DefaultWorkUnit
{
    public:
        /// @brief Empty Virtual Deconstructor
        virtual ~LoggerCheckWorkUnit()
            { }

        DefaultThreadSpecificStorage::Type* FromGetResource;
        DefaultThreadSpecificStorage::Type* FromArgs;

        Logger* LogFromGet;
        Logger* LogFromArgs;

        Thread::id InThread;

        virtual void DoWork(DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
        {
            FromGetResource = CurrentThreadStorage.GetFrameScheduler()->GetThreadResource();
            FromArgs = &CurrentThreadStorage;

            LogFromGet = &CurrentThreadStorage.GetFrameScheduler()->GetThreadResource()->GetUsableLogger();
            LogFromArgs = &CurrentThreadStorage.GetUsableLogger();

            InThread = this_thread::get_id();

            this_thread::sleep_for(100000); // wait 1/10th of a second.
        }
};


/// @brief Tests for the Framescheduler class
class frameschedulertests : public UnitTestGroup
{
    public:
        /// @brief Performs basic checks on the logs
        /// @details Used in @ref ThreadCreate
        /// @param Log The Log to check
        /// @param TargetThreadCount there better be this many threads.
        /// @param WorkUnitCount There better be this many uniquely named WorkUnits.
        /// @return A Set of WorkUnit names in case extra work needs to be performed on it.
        set<String> CheckSchedulerLog(Mezzanine::Logger& Log, Whole TargetThreadCount_, Whole WorkUnitCount_, String TestName)
        {
            TestResult temp;
            pugi::xml_document Doc;
            Doc.load(Log);
            pugi::xml_node TestLog = Doc.child("Frame");
            if(TestLog)
                { temp=Testing::Success; }
            else
                { temp=Testing::Failed; }
            AddTestResult(TestName+"::TestLog", temp);
            Whole ThreadCount = 0;
            Whole WorkUnitCount = 0;
            pugi::xml_node LogCommit = TestLog.child("Thread");

            set<String> WorkUnitNames;
            while(LogCommit)
            {
                pugi::xml_node OneUnit = LogCommit.child("MakePi");
                while(OneUnit)
                {
                    pugi::xml_attribute CurrentName = OneUnit.attribute("WorkUnitName");
                    WorkUnitNames.insert(CurrentName.value());

                    WorkUnitCount++;
                    OneUnit = OneUnit.next_sibling("MakePi");
                }
                ThreadCount++;
                LogCommit = LogCommit.next_sibling("Thread");
            }
            cout << "Log inspection results: " << endl
                 << "\t Found " << ThreadCount << " threads, expected " << TargetThreadCount_ << "." << endl
                 << "\t Found " << WorkUnitNames.size() << " total WorkUnits run with " << WorkUnitCount << " different names and expected " << WorkUnitCount_ << "." << endl
                 << "WorkUnit Names:" << endl;
            //sort(WorkUnitNames.begin(),WorkUnitNames.end());
            for(set<String>::iterator Iter=WorkUnitNames.begin(); Iter!=WorkUnitNames.end(); Iter++)
                { cout << *Iter << "\t"; }
            //ThrowOnFalse(ThreadCount==TargetThreadCount_, "Thread count wrong");
            if(ThreadCount==TargetThreadCount_)
                { temp=Testing::Success; }
            else
                { temp=Testing::Failed; }
            AddTestResult(TestName+"::ThreadCount", temp);
            //ThrowOnFalse(WorkUnitCount_==WorkUnitNames.size(),"Wrong number of Unique WorkUnit Names");
            if(WorkUnitCount_==WorkUnitNames.size())
                { temp=Testing::Success; }
            else
                { temp=Testing::Failed; }
            AddTestResult(TestName+"::LogcheckSizes", temp);
            //ThrowOnFalse(WorkUnitCount_==WorkUnitCount,"Wrong number of WorkUnit Names");
            if(WorkUnitCount_==WorkUnitCount)
                { temp=Testing::Success; }
            else
                { temp=Testing::Failed; }
            AddTestResult(TestName+"::LogcheckNames", temp);
            return WorkUnitNames;
        }

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

        /// @brief Converts anything streamable to anything streamable Mezzanine::String
        /// @details Used in @ref ThreadRestart
        /// @param Datum the item to convert
        /// @return a string containing the lexographically casted data
        template <typename To, typename From>
        To ToWhatever(From Datum)
        {
            stringstream Converter;
            Converter << Datum;
            To Results;
            Converter >> Results;
            return Results;
        }

        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "FrameScheduler"
        virtual String Name()
            { return String("FrameScheduler"); }

        /// @brief Performs a number of tests on the Framescheduler and the core algorithm
        virtual void RunAutomaticTests()
        {
            { // Basic sorting
                cout <<  "Creating a simple dependency chain in 4 WorkUnits and inserting them into a Test FrameScheduler. Then they will be pulled out one at a time and mark them as completed: " << endl;

                PiMakerWorkUnit *WorkUnitK1 = new PiMakerWorkUnit(500,"First",false);
                PiMakerWorkUnit *WorkUnitK2 = new PiMakerWorkUnit(500,"Second",false);
                PiMakerWorkUnit *WorkUnitK3 = new PiMakerWorkUnit(500,"Third",false);
                PiMakerWorkUnit *WorkUnitK4 = new PiMakerWorkUnit(500,"Fourth",false);

                FrameScheduler SchedulingTest1(&cout,1);
                DefaultThreadSpecificStorage::Type Storage1(&SchedulingTest1);
                WorkUnitK4->AddDependency(WorkUnitK3);
                WorkUnitK3->AddDependency(WorkUnitK2);
                WorkUnitK2->AddDependency(WorkUnitK1);
                SchedulingTest1.AddWorkUnitMain(WorkUnitK1); // no deletes required the Scheduler takes ownership
                SchedulingTest1.AddWorkUnitMain(WorkUnitK2);
                SchedulingTest1.AddWorkUnitMain(WorkUnitK3);
                SchedulingTest1.AddWorkUnitMain(WorkUnitK4);
                SchedulingTest1.SortWorkUnitsMain();

                iWorkUnit* Counter = SchedulingTest1.GetNextWorkUnit();
                cout << "Getting the WorkUnit Named " << ((PiMakerWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                TEST(((PiMakerWorkUnit*)Counter)->Name == String("First"),"BasicSorting::First");

                Counter->operator()(Storage1);
                Counter = SchedulingTest1.GetNextWorkUnit();
                cout << "Getting the WorkUnit Named " << ((PiMakerWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                TEST(((PiMakerWorkUnit*)Counter)->Name == String("Second"),"BasicSorting::Second");

                Counter->operator()(Storage1);
                Counter = SchedulingTest1.GetNextWorkUnit();
                cout << "Getting the WorkUnit Named " << ((PiMakerWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                TEST(((PiMakerWorkUnit*)Counter)->Name == String("Third"),"BasicSorting::Third");

                Counter->operator()(Storage1);
                Counter = SchedulingTest1.GetNextWorkUnit();
                cout << "Getting the WorkUnit Named " << ((PiMakerWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                TEST(((PiMakerWorkUnit*)Counter)->Name == String("Fourth"),"BasicSorting::Fourth");

                Counter->operator()(Storage1);

                cout << endl << "Creating 3 WorkUnits with precise runtimes and inserting them into a Test FrameScheduler. Then they will be pulled out one at a time and mark them as completed: " << endl;
                FrameScheduler SchedulingTest2(&cout,1);
                DefaultThreadSpecificStorage::Type Storage2(&SchedulingTest2);

                PausesWorkUnit *FiveHundred = new PausesWorkUnit(500,"FiveHundred-ms");
                PausesWorkUnit *FiveThousand = new PausesWorkUnit(5000,"FiveThousand-ms");
                PausesWorkUnit *FiftyThousand = new PausesWorkUnit(50000,"FiftyThousand-ms");
                //PausesWorkUnit *FiveHundredThousand = new PausesWorkUnit(500000,"FiveHundredThousand-ms");
                cout << "Work Units (FiveHundred-ms, FiveThousand-ms, FiftyThousand-ms)[ms is microseconds in this context] Created, executing each ten times: " << endl;
                for(Int8 Counter = 0; Counter <10; ++Counter)
                {
                    FiveHundred->operator()(Storage2);
                    FiveThousand->operator()(Storage2);
                    FiftyThousand->operator()(Storage2);
                    //FiveHundredThousand->operator()(Storage2,SchedulingTest2);
                }
                SchedulingTest2.AddWorkUnitMain(FiveHundred);
                SchedulingTest2.AddWorkUnitMain(FiftyThousand);
                SchedulingTest2.AddWorkUnitMain(FiveThousand);
                //SchedulingTest2.AddWorkUnit(FiveHundredThousand);

                cout << "FiveHundred-ms   : " << FiveHundred->GetPerformanceLog().GetAverage() << endl;
                cout << "FiveThousand-ms  : " << FiveThousand->GetPerformanceLog().GetAverage() << endl;
                cout << "FiftyThousand-ms : " << FiftyThousand->GetPerformanceLog().GetAverage() << endl;
                //cout << "FiveHundredThousand-ms  : " << FiveHundredThousand->GetPerformanceLog().GetAverage() << endl;
                cout << "Marking each WorkUnit as usable for the next frame:" << endl;
                FiveHundred->PrepareForNextFrame();
                FiveThousand->PrepareForNextFrame();
                FiftyThousand->PrepareForNextFrame();
                //FiveHundredThousand->PrepareForNextFrame();
                SchedulingTest2.SortWorkUnitsMain();

                cout << "Extracting WorkUnits with the scheduling mechanism: " << endl;
                //Counter = SchedulingTest2.GetNextWorkUnit();
                //cout << "Getting the WorkUnit Named " << ((PausesWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                //assert( ((PausesWorkUnit*)Counter)->Name == String("FiveHundredThousand-ms") );
                //Counter->operator()(Storage2, SchedulingTest2);
                Counter = SchedulingTest2.GetNextWorkUnit();
                cout << "Getting the WorkUnit Named " << ((PausesWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                TEST(((PausesWorkUnit*)Counter)->Name == String("FiftyThousand-ms"),"TimedSort::FiftyThousand-ms")

                Counter->operator()(Storage2);
                Counter = SchedulingTest2.GetNextWorkUnit();
                cout << "Getting the WorkUnit Named " << ((PausesWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                TEST(((PausesWorkUnit*)Counter)->Name == String("FiveThousand-ms"),"TimedSort::FiveThousand-ms")

                Counter->operator()(Storage2);
                Counter = SchedulingTest2.GetNextWorkUnit();
                cout << "Getting the WorkUnit Named " << ((PausesWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                TEST(((PausesWorkUnit*)Counter)->Name == String("FiveHundred-ms"),"TimedSort::FiveHundred-ms")
                Counter->operator()(Storage2);
            } // \Basic Sorting

            {
                //TestResult temp;

                cout << "Creating a FrameScheduler with 4 WorkUnits Running one frame with different thread counts: " << endl;
                stringstream LogCache;
                FrameScheduler ThreadCreationTest1(&LogCache,1);
                PiMakerWorkUnit* WorkUnitR1 = new PiMakerWorkUnit(50000,"Run1",false);
                PiMakerWorkUnit* WorkUnitR2 = new PiMakerWorkUnit(50000,"Run2",false);
                PiMakerWorkUnit* WorkUnitR3 = new PiMakerWorkUnit(50000,"Run3",false);
                PiMakerWorkUnit* WorkUnitR4 = new PiMakerWorkUnit(50000,"Run4",false);
                LogBufferSwapper Swapper2;
                LogAggregator Agg2;
                DefaultThreadSpecificStorage::Type SwapResource2(&ThreadCreationTest1);
                ThreadCreationTest1.AddWorkUnitMain(WorkUnitR1);
                ThreadCreationTest1.AddWorkUnitMain(WorkUnitR2);
                ThreadCreationTest1.AddWorkUnitMain(WorkUnitR3);
                ThreadCreationTest1.AddWorkUnitMain(WorkUnitR4);

                cout << "Thread count on initial creation: " << ThreadCreationTest1.GetThreadCount() << endl;
                cout << "Running One Frame." << endl;
                ThreadCreationTest1.DoOneFrame();
                Swapper2(SwapResource2);
                Agg2(SwapResource2);
                cout << "Emitting log:" << endl;
                cout << LogCache.str() << endl;
                CheckSchedulerLog(LogCache,1,4,"ThreadTests::SingleThread");
                cout << "It ran correctly." << endl;
                LogCache.str("");

                ThreadCreationTest1.SetThreadCount(2);
                cout << endl << "Thread count after setting to: " << ThreadCreationTest1.GetThreadCount() << endl;
                cout << "Running One Frame." << endl;
                ThreadCreationTest1.DoOneFrame();
                Swapper2(SwapResource2);
                Agg2(SwapResource2);
                cout << "Emitting log:" << endl;
                cout << LogCache.str() << endl;
                CheckSchedulerLog(LogCache,2,4,"ThreadTests::DualThread");
                cout << "It ran correctly." << endl;
                LogCache.str("");

                ThreadCreationTest1.SetThreadCount(3);
                cout << endl << "Thread count after setting to: " << ThreadCreationTest1.GetThreadCount() << endl;
                cout << "Running One Frame." << endl;
                ThreadCreationTest1.DoOneFrame();
                Swapper2(SwapResource2);
                Agg2(SwapResource2);
                cout << "Emitting log:" << endl;
                cout << LogCache.str() << endl;
                CheckSchedulerLog(LogCache,3,4,"ThreadTests::TripleThread");
                cout << "It ran correctly." << endl;
                LogCache.str("");

                ThreadCreationTest1.SetThreadCount(4);
                cout << endl << "Thread count after setting to: " << ThreadCreationTest1.GetThreadCount() << endl;
                cout << "Running One Frame." << endl;
                ThreadCreationTest1.DoOneFrame();
                Swapper2(SwapResource2);
                Agg2(SwapResource2);
                cout << "Emitting log:" << endl;
                cout << LogCache.str() << endl;
                CheckSchedulerLog(LogCache,4,4,"ThreadTests::QuadThread");
                cout << "It ran correctly." << endl;
                LogCache.str("");

                //Whole Work = 8;
                Whole Work = 1000;
                cout << endl << "Leaving thread count alone and adding " << Work << " WorkUnits to the test scheduler" << endl;
                cout << "Running One Frame." << endl;
                for (Whole Counter=0; Counter<Work; ++Counter)
                    { ThreadCreationTest1.AddWorkUnitMain( new PiMakerWorkUnit(50000,"Dyn"+ToString(Counter),false) ); }
                ThreadCreationTest1.DoOneFrame();
                Swapper2(SwapResource2);
                Agg2(SwapResource2);
                //CheckSchedulerLog(LogCache,4,12);
                //cout << LogCache.str() << endl;
                CheckSchedulerLog(LogCache,4,1004,"ThousandUnitStress");
                cout << "It ran correctly." << endl;
                LogCache.str("");
            } // \threading tests

            { // Dependency
                cout << "Creating a few WorkUnits with a " << endl;
                stringstream LogCache;
                cout << "Creating WorkUnits a Dependency chain as follows: "
                        << endl << "    +--->B"
                        << endl << "    |"
                        << endl << "A---+"
                        << endl << "    |"
                        << endl << "    +--->C"
                        << endl;
                PausesWorkUnit *RestartA = new PausesWorkUnit(100000,"A");
                PausesWorkUnit *RestartB = new PausesWorkUnit(100000,"B");
                PausesWorkUnit *RestartC = new PausesWorkUnit(100000,"C");
                RestartB->AddDependency(RestartA);
                RestartC->AddDependency(RestartA);
                LogCache.str("");
                FrameScheduler RestartScheduler1(&LogCache,2);
                LogBufferSwapper Swapper3;
                LogAggregator Agg3;
                DefaultThreadSpecificStorage::Type SwapResource3(&RestartScheduler1);
                RestartScheduler1.AddWorkUnitMain(RestartA);
                RestartScheduler1.AddWorkUnitMain(RestartB);
                RestartScheduler1.AddWorkUnitMain(RestartC);
                RestartScheduler1.SortWorkUnitsMain();
                RestartScheduler1.DoOneFrame();
                Swapper3(SwapResource3);
                Agg3(SwapResource3);
                // Check that two threads exist and that B and C run in different thread, and after A finished

                cout << LogCache.str() << "Parsing log to determine if everything happened correctly" << endl;
                pugi::xml_document Doc;
                Doc.load(LogCache);

                pugi::xml_node Thread1Node = Doc.child("Frame").first_child();
                pugi::xml_node Thread2Node = Doc.child("Frame").last_child();
                TEST(Thread1Node,"BasicDependency::LogNode1");
                TEST(Thread2Node,"BasicDependency::LogNode2");

                #ifdef MEZZ_DEBUG
                    vector<RestartMetric> UnitTracking;
                    UnitTracking.push_back(RestartMetric());
                    UnitTracking.push_back(RestartMetric());
                    UnitTracking.push_back(RestartMetric());
                    UnitTracking.push_back(RestartMetric());


                    // gather all the data that might be useful in this test.
                    UnitTracking[0].UnitStart = String(Thread1Node.child("WorkunitStart").attribute("BeginTimeStamp").as_string());
                    UnitTracking[0].Name = String(Thread1Node.child("WorkunitStart").next_sibling().attribute("WorkUnitName").as_string());
                    UnitTracking[0].Threadid = String(Thread1Node.child("WorkunitStart").next_sibling().attribute("ThreadID").as_string());
                    UnitTracking[0].UnitEnd = String(Thread1Node.child("WorkunitStart").next_sibling().next_sibling().attribute("EndTimeStamp").as_string());
                    cout << UnitTracking[0] << endl;
                    UnitTracking[1].UnitStart = String(Thread1Node.child("WorkunitEnd").next_sibling().attribute("BeginTimeStamp").as_string());
                    UnitTracking[1].Name = String(Thread1Node.child("WorkunitEnd").next_sibling().next_sibling().attribute("WorkUnitName").as_string());
                    UnitTracking[1].Threadid = String(Thread1Node.child("WorkunitEnd").next_sibling().next_sibling().attribute("ThreadID").as_string());
                    UnitTracking[1].UnitEnd = String(Thread1Node.child("WorkunitEnd").next_sibling().next_sibling().next_sibling().attribute("EndTimeStamp").as_string());
                    cout << UnitTracking[1] << endl;
                    UnitTracking[2].UnitStart = String(Thread2Node.child("WorkunitStart").attribute("BeginTimeStamp").as_string());
                    UnitTracking[2].Name = String(Thread2Node.child("WorkunitStart").next_sibling().attribute("WorkUnitName").as_string());
                    UnitTracking[2].Threadid = String(Thread2Node.child("WorkunitStart").next_sibling().attribute("ThreadID").as_string());
                    UnitTracking[2].UnitEnd = String(Thread2Node.child("WorkunitStart").next_sibling().next_sibling().attribute("EndTimeStamp").as_string());
                    cout << UnitTracking[2] << endl;
                    UnitTracking[3].UnitStart = String(Thread2Node.child("WorkunitEnd").next_sibling().attribute("BeginTimeStamp").as_string());
                    UnitTracking[3].Name = String(Thread2Node.child("WorkunitEnd").next_sibling().next_sibling().attribute("WorkUnitName").as_string());
                    UnitTracking[3].Threadid = String(Thread2Node.child("WorkunitEnd").next_sibling().next_sibling().attribute("ThreadID").as_string());
                    UnitTracking[3].UnitEnd = String(Thread2Node.child("WorkunitEnd").next_sibling().next_sibling().next_sibling().attribute("EndTimeStamp").as_string());
                    cout << UnitTracking[3] << endl;

                    // Get exactly what we need.
                    String BThread;
                    String CThread;
                    String AEnd;
                    String BStart;
                    String CStart;
                    for(vector<RestartMetric>::iterator Iter = UnitTracking.begin(); Iter != UnitTracking.end(); ++Iter)
                    {
                        if(Iter->Name=="A")
                        {
                            AEnd = Iter->UnitEnd;
                        }
                        if(Iter->Name=="B")
                        {
                            BStart = Iter->UnitStart;
                            BThread = Iter->Threadid;
                        }
                        if(Iter->Name=="C")
                        {
                            CStart = Iter->UnitStart;
                            CThread = Iter->Threadid;
                        }
                    }

                    cout << "The timer cannot resolve times less then: " << GetTimeStampResolution() << endl;
                    cout << "Was A complete before B started: " << (ToWhatever<MaxInt>(AEnd)<=ToWhatever<MaxInt>(BStart)+GetTimeStampResolution()) << endl;
                    cout << "Was A complete before B started if the clock resolution could cause error: " << (ToWhatever<MaxInt>(AEnd)<=(ToWhatever<MaxInt>(BStart)+GetTimeStampResolution())) << endl;
                    TEST(ToWhatever<MaxInt>(AEnd)<=(ToWhatever<MaxInt>(BStart)+GetTimeStampResolution()),"BasicSorting::ABeforeB");
                    cout << "Was A complete before C started: " << (ToWhatever<MaxInt>(AEnd)<=ToWhatever<MaxInt>(CStart)) << endl;
                    cout << "Was A complete before C started if the clock resolution could cause error: " << (ToWhatever<MaxInt>(AEnd)<=(ToWhatever<MaxInt>(CStart)+GetTimeStampResolution())) << endl;
                    TEST(ToWhatever<MaxInt>(AEnd)<=(ToWhatever<MaxInt>(CStart)+GetTimeStampResolution()),"BasicSorting::ABeforeC");
                    cout << "Were B and C run in different threads: " << (BThread!=CThread) << endl;
                    TEST(BThread!=CThread,"BasicSorting::BNotInCThread");
                #else
                    cout << "This test does not validate when not in debug mode. The log is missing much meta data.";
                    // can still do some tests here
                #endif
            }

            { // Removal
                stringstream LogCache;
                FrameScheduler RemovalScheduler(&LogCache,1);
                ThreadSpecificStorage RemovalResource(&RemovalScheduler);

                cout << "Creating 5 workunits each depending on the next: A -> B -> C -> D -> E" << endl
                     << "And a 6th that has no dependency relations: F" << endl;
                PausesWorkUnit *EraseA = new PausesWorkUnit(10,"A");
                PausesWorkUnit *EraseB = new PausesWorkUnit(10,"B");
                PausesWorkUnit *EraseC = new PausesWorkUnit(10,"C");
                PausesWorkUnit *EraseD = new PausesWorkUnit(10,"D");
                PausesWorkUnit *EraseE = new PausesWorkUnit(10,"E");
                PausesWorkUnit *EraseF = new PausesWorkUnit(10,"F");
                PausesWorkUnit *EraseZ = new PausesWorkUnit(10,"Z");
                EraseE->AddDependency(EraseD);
                EraseD->AddDependency(EraseC);
                EraseC->AddDependency(EraseB);
                EraseB->AddDependency(EraseA);

                cout << "Stuffing all 6 into a test frame scheduler and preparing it for a run" << endl;
                RemovalScheduler.AddWorkUnitMain(EraseA);
                RemovalScheduler.AddWorkUnitMain(EraseB);
                RemovalScheduler.AddWorkUnitMain(EraseC);
                RemovalScheduler.AddWorkUnitMain(EraseD);
                RemovalScheduler.AddWorkUnitMain(EraseE);
                RemovalScheduler.AddWorkUnitMain(EraseF);

                RemovalScheduler.SortWorkUnitsMain();

                cout << "Checking the order of the workunits (Should be \"A B C D E F NULL\" or \"A B C D F E NULL\"): ";
                Whole PrepCount = 0;

                PausesWorkUnit* Next;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //A
                Next->operator() (RemovalResource);
                if(Next->Name=="A") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //C
                Next->operator() (RemovalResource);
                if(Next->Name=="C") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //E or F
                Next->operator() (RemovalResource);
                PausesWorkUnit* OtherEF = Next;
                if(Next->Name=="E" || Next->Name=="F") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //E or F Whichever was not EitherOr
                Next->operator() (RemovalResource);
                if( (Next->Name=="E" || Next->Name=="F") && OtherEF!= Next) { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnit())
                    { cout << Next->Name << " "; }
                else
                    { cout << "NULL "; PrepCount++; }
                cout << endl;
                TEST((7==PrepCount),"RemoveMain::OrderingPreTest");

                cout << endl << "Removing F from Scheduler then resorting and resetting it (New order should be \"A B C D E NULL\"): ";
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.RemoveWorkUnitMain(EraseF);
                RemovalScheduler.SortWorkUnitsMain();
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //A
                Next->operator() (RemovalResource);
                if(Next->Name=="A") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //C
                Next->operator() (RemovalResource);
                if(Next->Name=="C") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //E
                Next->operator() (RemovalResource);
                if(Next->Name=="E") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnit())
                    { cout << Next->Name << " "; }
                else
                    { cout << "NULL "; PrepCount++; }
                cout << endl;
                TEST(6==PrepCount, "RemoveMain::Simple");

                cout << endl << "Removing E from Scheduler then resorting and resetting it (New order should be \"A B C D NULL\"): ";
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.RemoveWorkUnitMain(EraseE);
                RemovalScheduler.SortWorkUnitsMain();
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //A
                Next->operator() (RemovalResource);
                if(Next->Name=="A") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //C
                Next->operator() (RemovalResource);
                if(Next->Name=="C") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnit())
                    { cout << Next->Name << " "; }
                else
                    { cout << "NULL "; PrepCount++; }
                cout << endl;
                TEST(5==PrepCount, "RemoveMain::WithDep");

                cout << endl << "Removing A from Scheduler then resorting and resetting it (New order should be \"B C D NULL\"): ";
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.RemoveWorkUnitMain(EraseA);
                RemovalScheduler.SortWorkUnitsMain();
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //C
                Next->operator() (RemovalResource);
                if(Next->Name=="C") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnit())
                    { cout << Next->Name << " "; }
                else
                    { cout << "NULL "; PrepCount++; }
                cout << endl;
                TEST(4==PrepCount, "RemoveMain::AsDep");

                cout << endl << "Removing C from Scheduler then resorting and resetting it (New order should be \"B D NULL\" or \"D B NULL\"): ";
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.RemoveWorkUnitMain(EraseC);
                RemovalScheduler.SortWorkUnitsMain();
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                PausesWorkUnit* BorD = Next;
                cout << Next->Name << " "; //B or D
                Next->operator() (RemovalResource);
                if(Next->Name=="B"||Next->Name=="D") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                cout << Next->Name << " "; // The other
                Next->operator() (RemovalResource);
                if( (Next->Name=="B"||Next->Name=="D") && Next!=BorD) { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnit())
                    { cout << Next->Name << " "; }
                else
                    { cout << "NULL "; PrepCount++; }
                cout << endl;
                TEST(3==PrepCount, "RemoveMain::WithAndAsDep");


                cout << endl << "Adding an Affinity WorkUnit Z and making it depend on B (new order should be B, Z, D, NULL): ";
                RemovalScheduler.AddWorkUnitAffinity(EraseZ);
                EraseZ->AddDependency(EraseB);
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.SortWorkUnitsAll(); //Sorting all on this test
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //Z
                Next->operator() (RemovalResource);
                if(Next->Name=="Z") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { cout << Next->Name << " "; }
                else
                    { cout << "NULL "; PrepCount++; }
                cout << endl;
                TEST(4==PrepCount, "RemoveMain::MainPreTest");

                cout << endl << "Removing B and verifying no infinite loops are added(new order should be Z, D, NULL): ";
                RemovalScheduler.RemoveWorkUnitMain(EraseB);
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.SortWorkUnitsAll(); //Sorting all on this test
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //Z
                Next->operator() (RemovalResource);
                if(Next->Name=="Z") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { cout << Next->Name << " "; }
                else
                    { cout << "NULL "; PrepCount++; }
                cout << endl;
                TEST(3==PrepCount, "RemoveMain::MainCleanup");

                delete EraseA; delete EraseB; delete EraseC; delete EraseE; delete EraseF;
            } // \Removal

            { // Affinty
                stringstream LogCache;
                cout << "Creating WorkUnits a Dependency chain as follows: "
                        << endl << "A---+                  +--->B"
                        << endl << "    |                  |"
                        << endl << "    +-->AffinityUnit---+"
                        << endl << "    |                  |"
                        << endl << "B---+                  +--->C"
                        << endl;
                PausesWorkUnit *AffinityA = new PausesWorkUnit(10000,"A");
                PausesWorkUnit *AffinityB = new PausesWorkUnit(10000,"B");
                PausesWorkUnit *AffinityAffinity = new PausesWorkUnit(10000,"Affinity");
                PausesWorkUnit *AffinityC = new PausesWorkUnit(10000,"C");
                PausesWorkUnit *AffinityD = new PausesWorkUnit(10000,"D");
                AffinityAffinity->AddDependency(AffinityA);
                AffinityAffinity->AddDependency(AffinityB);
                AffinityC->AddDependency(AffinityAffinity);
                AffinityD->AddDependency(AffinityAffinity);

                FrameScheduler Scheduler1(&LogCache,2);
                LogBufferSwapper Swapper1;
                LogAggregator Agg1;
                DefaultThreadSpecificStorage::Type SwapResource(&Scheduler1);
                Scheduler1.AddWorkUnitMain(AffinityA);
                Scheduler1.AddWorkUnitMain(AffinityB);
                Scheduler1.AddWorkUnitAffinity(AffinityAffinity);
                Scheduler1.AddWorkUnitMain(AffinityC);
                Scheduler1.AddWorkUnitMain(AffinityD);
                Scheduler1.SortWorkUnitsMain();
                Scheduler1.DoOneFrame();
                Swapper1(SwapResource);
                Agg1(SwapResource);
                // Check that two threads exist and that B and C run in different thread, and after A finished
                cout << "Affinity should run in this This thread and this thread has id: " << Mezzanine::Threading::this_thread::get_id() << endl;
                cout << LogCache.str() << "Parsing log to determine if everything happened correctly" << endl;

                #ifdef MEZZ_DEBUG
                    /// @todo make a function that checks the order works units ran in returns that as a vector
                #else
                    cout << "This test does not validate when not in debug mode. The log is missing much meta data.";
                    // can still do some tests here
                #endif
            } // \Affinity

            { // \ Removal Affinity
                stringstream LogCache;
                FrameScheduler RemovalScheduler(&LogCache,1);
                ThreadSpecificStorage RemovalResource(&RemovalScheduler);

                cout << endl << "Creating 5 workunits each depending on the next: A -> B -> C -> D -> E" << endl
                     << "And a 6th that has no dependency relations: F" << endl;
                PausesWorkUnit *EraseA = new PausesWorkUnit(10,"A");
                PausesWorkUnit *EraseB = new PausesWorkUnit(10,"B");
                PausesWorkUnit *EraseC = new PausesWorkUnit(10,"C");
                PausesWorkUnit *EraseD = new PausesWorkUnit(10,"D");
                PausesWorkUnit *EraseE = new PausesWorkUnit(10,"E");
                PausesWorkUnit *EraseF = new PausesWorkUnit(10,"F");
                PausesWorkUnit *EraseZ = new PausesWorkUnit(10,"Z");
                EraseE->AddDependency(EraseD);
                EraseD->AddDependency(EraseC);
                EraseC->AddDependency(EraseB);
                EraseB->AddDependency(EraseA);

                cout << "Stuffing all 6 into a test frame scheduler as affinity workunit and preparing it for a run" << endl;
                RemovalScheduler.AddWorkUnitAffinity(EraseA);
                RemovalScheduler.AddWorkUnitAffinity(EraseB);
                RemovalScheduler.AddWorkUnitAffinity(EraseC);
                RemovalScheduler.AddWorkUnitAffinity(EraseD);
                RemovalScheduler.AddWorkUnitAffinity(EraseE);
                RemovalScheduler.AddWorkUnitAffinity(EraseF);

                RemovalScheduler.SortWorkUnitsAffinity();

                cout << "Checking the order of the workunits (Should be \"A B C D E F NULL\" or \"A B C D F E NULL\"): ";
                Whole PrepCount = 0;

                PausesWorkUnit* Next;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //A
                Next->operator() (RemovalResource);
                if(Next->Name=="A") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //C
                Next->operator() (RemovalResource);
                if(Next->Name=="C") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //E or F
                Next->operator() (RemovalResource);
                PausesWorkUnit* OtherEF = Next;
                if(Next->Name=="E" || Next->Name=="F") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //E or F Whichever was not EitherOr
                Next->operator() (RemovalResource);
                if( (Next->Name=="E" || Next->Name=="F") && OtherEF!= Next) { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { cout << Next->Name << " "; }
                else
                    { cout << "NULL "; PrepCount++; }
                cout << endl;
                TEST((7==PrepCount),"RemoveAffinity::OrderingPreTest");

                cout << endl << "Removing F from Scheduler then resorting and resetting it (New order should be \"A B C D E NULL\"): ";
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.RemoveWorkUnitAffinity(EraseF);
                RemovalScheduler.SortWorkUnitsAffinity();
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //A
                Next->operator() (RemovalResource);
                if(Next->Name=="A") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //C
                Next->operator() (RemovalResource);
                if(Next->Name=="C") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //E
                Next->operator() (RemovalResource);
                if(Next->Name=="E") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { cout << Next->Name << " "; }
                else
                    { cout << "NULL "; PrepCount++; }
                cout << endl;
                TEST((6==PrepCount),"RemoveAffinity::Simple");

                cout << endl << "Removing E from Scheduler then resorting and resetting it (New order should be \"A B C D NULL\"): ";
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.RemoveWorkUnitAffinity(EraseE);
                RemovalScheduler.SortWorkUnitsAffinity();
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //A
                Next->operator() (RemovalResource);
                if(Next->Name=="A") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //C
                Next->operator() (RemovalResource);
                if(Next->Name=="C") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { cout << Next->Name << " "; }
                else
                    { cout << "NULL "; PrepCount++; }
                cout << endl;
                TEST((5==PrepCount),"RemoveAffinity::WithDep");

                cout << endl << "Removing A from Scheduler then resorting and resetting it (New order should be \"B C D NULL\"): ";
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.RemoveWorkUnitAffinity(EraseA);
                RemovalScheduler.SortWorkUnitsAffinity();
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //C
                Next->operator() (RemovalResource);
                if(Next->Name=="C") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { cout << Next->Name << " "; }
                else
                    { cout << "NULL "; PrepCount++; }
                cout << endl;
                TEST((4==PrepCount),"RemoveAffinity::AsDep");

                cout << endl << "Removing C from Scheduler then resorting and resetting it (New order should be \"B D NULL\" or \"D B NULL\"): ";
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.RemoveWorkUnitAffinity(EraseC);
                RemovalScheduler.SortWorkUnitsAffinity();
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                PausesWorkUnit* BorD = Next;
                cout << Next->Name << " "; //B or D
                Next->operator() (RemovalResource);
                if(Next->Name=="B"||Next->Name=="D") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; // The other
                Next->operator() (RemovalResource);
                if( (Next->Name=="B"||Next->Name=="D") && Next!=BorD) { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { cout << Next->Name << " "; }
                else
                    { cout << "NULL "; PrepCount++; }
                cout << endl;
                TEST((3==PrepCount),"RemoveAffinity::WithAndAsDep");

                cout << endl << "Adding a non-affinity/Main WorkUnit Z and making it depend on B (new order should be B, D, Z, NULL): ";
                RemovalScheduler.AddWorkUnitMain(EraseZ);
                EraseZ->AddDependency(EraseB);
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.SortWorkUnitsAll(); //Sorting all on this test
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //Z
                Next->operator() (RemovalResource);
                if(Next->Name=="Z") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { cout << Next->Name << " "; }
                else
                    { cout << "NULL "; PrepCount++; }
                cout << endl;
                TEST((4==PrepCount),"RemoveAffinity::MainPreTest");

                cout << endl << "Removing B and verifying no infinite loops are added(new order should be D, Z, NULL): ";
                RemovalScheduler.RemoveWorkUnitAffinity(EraseB);
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.SortWorkUnitsAll(); //Sorting all on this test
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                cout << Next->Name << " "; //Z
                Next->operator() (RemovalResource);
                if(Next->Name=="Z") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { cout << Next->Name << " "; }
                else
                    { cout << "NULL "; PrepCount++; }
                cout << endl;
                TEST((3==PrepCount),"RemoveAffinity::MainCleanup");

                delete EraseA; delete EraseB; delete EraseC; delete EraseE; delete EraseF;
            }// \ Removal Affinity

            { // Removal Monopoly
                stringstream LogCache;
                FrameScheduler RemovalScheduler(&LogCache,1);
                //ThreadSpecificStorage RemovalResource(&RemovalScheduler);

                cout << endl << "Creating 3 Monopoly Workunits and an affinity Workunit and a normal/Main workunit" << endl;
                PauseMonopoly *EraseMonoA = new PauseMonopoly(10,"MonoA");
                PauseMonopoly *EraseMonoB = new PauseMonopoly(10,"MonoB");
                PauseMonopoly *EraseMonoC = new PauseMonopoly(10,"MonoC");

                PausesWorkUnit *EraseA = new PausesWorkUnit(10,"NeedsA");
                PausesWorkUnit *EraseB = new PausesWorkUnit(10,"NeedsB");

                EraseA->AddDependency(EraseMonoA);
                EraseB->AddDependency(EraseMonoB);

                RemovalScheduler.AddWorkUnitAffinity(EraseA);
                RemovalScheduler.AddWorkUnitMain(EraseB);
                RemovalScheduler.AddWorkUnitMonopoly(EraseMonoA);
                RemovalScheduler.AddWorkUnitMonopoly(EraseMonoB);
                RemovalScheduler.AddWorkUnitMonopoly(EraseMonoC);

                cout << "Test Scheduler has " << RemovalScheduler.GetWorkUnitMonopolyCount() << " WorkUnits, and the affinity unit has " << EraseA->GetDependencyCount()
                     << " Dependencies and the main workunit has " << EraseB->GetDependencyCount() << "." << endl;
                Test(EraseA->GetDependencyCount()==1 &&
                     EraseB->GetDependencyCount()==1 &&
                     RemovalScheduler.GetWorkUnitMonopolyCount()==3, "DAGFrameScheduler::FrameScheduler::RemoveMopoly::PreTest");

                RemovalScheduler.RemoveWorkUnitMonopoly(EraseMonoC);
                cout << "Removing Monopoly is no dependent, now the are " << RemovalScheduler.GetWorkUnitMonopolyCount() << " Monopolies (should be 2)." << endl << endl;
                Test(RemovalScheduler.GetWorkUnitMonopolyCount()==2, "DAGFrameScheduler::FrameScheduler::RemoveMopoly::Simple");

                RemovalScheduler.RemoveWorkUnitMonopoly(EraseMonoA);
                cout << "Removing Monopoly is a dependent for an Affinity unit, now the are " << RemovalScheduler.GetWorkUnitMonopolyCount() << " Monopolies (should be 1)" << endl
                     << "and the affinity unit has " << EraseA->GetDependencyCount() << " deps (should be 0)." << endl << endl;
                Test(RemovalScheduler.GetWorkUnitMonopolyCount()==1 &&
                     EraseA->GetDependencyCount()==0, "DAGFrameScheduler::FrameScheduler::RemoveMopoly::AffinityDep");

                RemovalScheduler.RemoveWorkUnitMonopoly(EraseMonoB);
                cout << "Removing Monopoly is a dependent for an main unit, now the are " << RemovalScheduler.GetWorkUnitMonopolyCount() << " Monopolies (should be 0)" << endl
                     << "and the main unit has " << EraseB->GetDependencyCount() << " deps (should be 0)." << endl << endl;
                Test(RemovalScheduler.GetWorkUnitMonopolyCount()==0 &&
                     EraseB->GetDependencyCount()==0, "DAGFrameScheduler::FrameScheduler::RemoveMopoly::AffinityDep");

                delete EraseMonoA; delete EraseMonoB; delete EraseMonoC;
            } // \Removal Monopoly

            {
                stringstream LogCache;
                FrameScheduler Scheduler1(&LogCache,4);
                LoggerCheckWorkUnit* Checker1 = new LoggerCheckWorkUnit;
                LoggerCheckWorkUnit* Checker2 = new LoggerCheckWorkUnit;
                LoggerCheckWorkUnit* Checker3 = new LoggerCheckWorkUnit;
                LoggerCheckWorkUnit* Checker4 = new LoggerCheckWorkUnit;
                Scheduler1.AddWorkUnitMain(Checker1); //scheduler should delete workunit
                Scheduler1.AddWorkUnitMain(Checker2);
                Scheduler1.AddWorkUnitMain(Checker3); // There is no guarantee these will run in different threads, but unless there is
                Scheduler1.AddWorkUnitMain(Checker4); // a huge delay in starting these workunits the FS really should

                Scheduler1.SortWorkUnitsAll();
                Scheduler1.DoOneFrame();

                cout << "Checking that FrameScheduler::GetThreadResource produces the same results as what is passed into WorkUnit::DoWork: "
                     << endl
                     << dec << "from thread :" << Checker1->InThread << " - " << hex
                        << (Checker1->FromArgs) << " == " << (Checker1->FromGetResource) << " : " << (Checker1->FromArgs==Checker1->FromGetResource) << endl
                     << dec << "from thread :" << Checker2->InThread << " - " << hex
                        << (Checker2->FromArgs) << " == " << (Checker2->FromGetResource) << " : " << (Checker2->FromArgs==Checker2->FromGetResource) << endl
                     << dec << "from thread :" << Checker3->InThread << " - " << hex
                        << (Checker3->FromArgs) << " == " << (Checker3->FromGetResource) << " : " << (Checker3->FromArgs==Checker3->FromGetResource) << endl
                     << dec << "from thread :" << Checker4->InThread << " - " << hex
                        << (Checker4->FromArgs) << " == " << (Checker4->FromGetResource) << " : " << (Checker4->FromArgs==Checker4->FromGetResource) << endl
                     << endl;
                TEST(Checker1->FromArgs==Checker1->FromGetResource &&
                     Checker2->FromArgs==Checker2->FromGetResource &&
                     Checker3->FromArgs==Checker3->FromGetResource &&
                     Checker4->FromArgs==Checker4->FromGetResource
                     ,"GetThreadResource");

                cout << "Checking that FrameScheduler::GetThreadUsableLogger produces the same results as what is passed into WorkUnit::DoWork: "
                     << endl
                     << dec << "from thread :" << Checker1->InThread << " - " << hex
                        << (Checker1->LogFromArgs) << " == " << (Checker1->LogFromGet) << " : " << (Checker1->LogFromArgs==Checker1->LogFromGet) << endl
                     << dec << "from thread :" << Checker2->InThread << " - " << hex
                        << (Checker2->LogFromArgs) << " == " << (Checker2->LogFromGet) << " : " << (Checker2->LogFromArgs==Checker2->LogFromGet) << endl
                     << dec << "from thread :" << Checker3->InThread << " - " << hex
                        << (Checker3->LogFromArgs) << " == " << (Checker3->LogFromGet) << " : " << (Checker3->LogFromArgs==Checker3->LogFromGet) << endl
                     << dec << "from thread :" << Checker4->InThread << " - " << hex
                        << (Checker4->LogFromArgs) << " == " << (Checker4->LogFromGet) << " : " << (Checker4->LogFromArgs==Checker4->LogFromGet) << endl
                     << endl;
                TEST(Checker1->LogFromArgs==Checker1->LogFromGet &&
                     Checker2->LogFromArgs==Checker2->LogFromGet &&
                     Checker3->LogFromArgs==Checker3->LogFromGet &&
                     Checker4->LogFromArgs==Checker4->LogFromGet
                     ,"GetThreadUsableLogger");

            }
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }
};

#endif

