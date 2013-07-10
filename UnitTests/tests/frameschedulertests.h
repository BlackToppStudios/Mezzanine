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

#include "main.h"

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
        /// @return Returns a String containing "framescheduler"
        virtual String Name()
            { return String("framescheduler"); }

        /// @copydoc Mezzanine::Testing::UnitTestGroup::RunTests
        /// @detail Test if the Framescheduler works correctly
        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            RunInteractiveTests = false; //prevent warnings

            if (RunAutomaticTests)
            {
                TestResult temp;

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
                SchedulingTest1.AddWorkUnit(WorkUnitK1); // no deletes required the Scheduler takes ownership
                SchedulingTest1.AddWorkUnit(WorkUnitK2);
                SchedulingTest1.AddWorkUnit(WorkUnitK3);
                SchedulingTest1.AddWorkUnit(WorkUnitK4);
                SchedulingTest1.SortWorkUnitsMain();

                iWorkUnit* Counter = SchedulingTest1.GetNextWorkUnit();
                cout << "Getting the WorkUnit Named " << ((PiMakerWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                if(((PiMakerWorkUnit*)Counter)->Name == String("First"))
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::Sorting::First", temp);
                Counter->operator()(Storage1);
                Counter = SchedulingTest1.GetNextWorkUnit();
                cout << "Getting the WorkUnit Named " << ((PiMakerWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                if(((PiMakerWorkUnit*)Counter)->Name == String("Second"))
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::Sorting::Second", temp);
                Counter->operator()(Storage1);
                Counter = SchedulingTest1.GetNextWorkUnit();
                cout << "Getting the WorkUnit Named " << ((PiMakerWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                if(((PiMakerWorkUnit*)Counter)->Name == String("Third"))
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::Sorting::Third", temp);
                Counter->operator()(Storage1);
                Counter = SchedulingTest1.GetNextWorkUnit();
                cout << "Getting the WorkUnit Named " << ((PiMakerWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                if(((PiMakerWorkUnit*)Counter)->Name == String("Fourth"))
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::Sorting::Fourth", temp);
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
                SchedulingTest2.AddWorkUnit(FiveHundred);
                SchedulingTest2.AddWorkUnit(FiftyThousand);
                SchedulingTest2.AddWorkUnit(FiveThousand);
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
                if(((PausesWorkUnit*)Counter)->Name == String("FiftyThousand-ms"))
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::Sorting::FiftyThousand-ms", temp);
                Counter->operator()(Storage2);
                Counter = SchedulingTest2.GetNextWorkUnit();
                cout << "Getting the WorkUnit Named " << ((PausesWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                if(((PausesWorkUnit*)Counter)->Name == String("FiveThousand-ms"))
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::Sorting::FiveThousand-ms", temp);
                Counter->operator()(Storage2);
                Counter = SchedulingTest2.GetNextWorkUnit();
                cout << "Getting the WorkUnit Named " << ((PausesWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                if(((PausesWorkUnit*)Counter)->Name == String("FiveHundred-ms"))
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::Sorting::FiveHundred-ms", temp);
                Counter->operator()(Storage2);
            }else{
                AddTestResult("DAGFrameScheduler::Sorting::First", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::Sorting::Second", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::Sorting::Third", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::Sorting::Fourth", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::Sorting::FiftyThousand-ms", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::Sorting::FiveThousand-ms", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::Sorting::FiveHundred-ms", Testing::Skipped);
            }

            if (RunAutomaticTests)
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
                ThreadCreationTest1.AddWorkUnit(WorkUnitR1);
                ThreadCreationTest1.AddWorkUnit(WorkUnitR2);
                ThreadCreationTest1.AddWorkUnit(WorkUnitR3);
                ThreadCreationTest1.AddWorkUnit(WorkUnitR4);

                cout << "Thread count on initial creation: " << ThreadCreationTest1.GetThreadCount() << endl;
                cout << "Running One Frame." << endl;
                ThreadCreationTest1.DoOneFrame();
                Swapper2(SwapResource2);
                Agg2(SwapResource2);
                cout << "Emitting log:" << endl;
                cout << LogCache.str() << endl;
                CheckSchedulerLog(LogCache,1,4,"DAGFrameScheduler::FrameScheduler::SingleThread");
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
                CheckSchedulerLog(LogCache,2,4,"DAGFrameScheduler::FrameScheduler::DualThread");
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
                CheckSchedulerLog(LogCache,3,4,"DAGFrameScheduler::FrameScheduler::TripleThread");
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
                CheckSchedulerLog(LogCache,4,4,"DAGFrameScheduler::FrameScheduler::QuadThread");
                cout << "It ran correctly." << endl;
                LogCache.str("");

                //Whole Work = 8;
                Whole Work = 1000;
                cout << endl << "Leaving thread count alone and adding " << Work << " WorkUnits to the test scheduler" << endl;
                cout << "Running One Frame." << endl;
                for (Whole Counter=0; Counter<Work; ++Counter)
                    { ThreadCreationTest1.AddWorkUnit( new PiMakerWorkUnit(50000,"Dyn"+ToString(Counter),false) ); }
                ThreadCreationTest1.DoOneFrame();
                Swapper2(SwapResource2);
                Agg2(SwapResource2);
                //CheckSchedulerLog(LogCache,4,12);
                //cout << LogCache.str() << endl;
                CheckSchedulerLog(LogCache,4,1004,"DAGFrameScheduler::FrameScheduler::ThousandUnit");
                cout << "It ran correctly." << endl;
                LogCache.str("");

            }else{
                AddTestResult("DAGFrameScheduler::FrameScheduler::DualThread::LogcheckNames", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::DualThread::LogcheckSizes", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::DualThread::TestLog", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::DualThread::ThreadCount", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::QuadThread::LogcheckNames", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::QuadThread::LogcheckSizes", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::QuadThread::TestLog", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::QuadThread::ThreadCount", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::SingleThread::LogcheckNames", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::SingleThread::LogcheckSizes", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::SingleThread::TestLog", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::SingleThread::ThreadCount", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::ThousandUnit::LogcheckNames", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::ThousandUnit::LogcheckSizes", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::ThousandUnit::TestLog", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::ThousandUnit::ThreadCount", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::TripleThread::LogcheckNames", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::TripleThread::LogcheckSizes", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::TripleThread::TestLog", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::TripleThread::ThreadCount", Testing::Skipped);
            }

            if (RunAutomaticTests)
            {
                TestResult temp;
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
                RestartScheduler1.AddWorkUnit(RestartA);
                RestartScheduler1.AddWorkUnit(RestartB);
                RestartScheduler1.AddWorkUnit(RestartC);
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
                if(Thread1Node)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::FrameScheduler::LogNode1", temp);
                if(Thread2Node)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::FrameScheduler::LogNode2", temp);

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
                    if(ToWhatever<MaxInt>(AEnd)<=(ToWhatever<MaxInt>(BStart)+GetTimeStampResolution()))
                        { temp=Testing::Success; }
                    else
                        { temp=Testing::Failed; }
                    AddTestResult("DAGFrameScheduler::FrameScheduler::ABeforeB", temp);
                    cout << "Was A complete before C started: " << (ToWhatever<MaxInt>(AEnd)<=ToWhatever<MaxInt>(CStart)) << endl;
                    cout << "Was A complete before C started if the clock resolution could cause error: " << (ToWhatever<MaxInt>(AEnd)<=(ToWhatever<MaxInt>(CStart)+GetTimeStampResolution())) << endl;
                    if(ToWhatever<MaxInt>(AEnd)<=(ToWhatever<MaxInt>(CStart)+GetTimeStampResolution()))
                        { temp=Testing::Success; }
                    else
                        { temp=Testing::Failed; }
                    AddTestResult("DAGFrameScheduler::FrameScheduler::ABeforeC", temp);
                    cout << "Were B and C run in different threads: " << (BThread!=CThread) << endl;
                    if(BThread!=CThread)
                        { temp=Testing::Success; }
                    else
                        { temp=Testing::Failed; }
                    AddTestResult("DAGFrameScheduler::FrameScheduler::BNotInCThread", temp);
                #else
                    cout << "This test does not validate when not in debug mode. The log is missing much meta data.";
                    // can still do some tests here
                #endif
            }else{
                AddTestResult("DAGFrameScheduler::FrameScheduler::LogNode1", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::LogNode2", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::ABeforeB", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::ABeforeC", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::BNotInCThread", Testing::Skipped);
            }

            if(RunAutomaticTests)
            {
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
                /*PiMakerWorkUnit *AffinityA = new PiMakerWorkUnit(100000,"A",false);
                PiMakerWorkUnit *AffinityB = new PiMakerWorkUnit(100000,"B",false);
                PiMakerWorkUnit *AffinityAffinity = new PiMakerWorkUnit(10000,"Affinity",false);
                PiMakerWorkUnit *AffinityC = new PiMakerWorkUnit(100000,"C",false);
                PiMakerWorkUnit *AffinityD = new PiMakerWorkUnit(100000,"D",false);*/
                AffinityAffinity->AddDependency(AffinityA);
                AffinityAffinity->AddDependency(AffinityB);
                AffinityC->AddDependency(AffinityAffinity);
                AffinityD->AddDependency(AffinityAffinity);

                FrameScheduler Scheduler1(&LogCache,2);
                LogBufferSwapper Swapper1;
                LogAggregator Agg1;
                DefaultThreadSpecificStorage::Type SwapResource(&Scheduler1);
                Scheduler1.AddWorkUnit(AffinityA);
                Scheduler1.AddWorkUnit(AffinityB);
                Scheduler1.AddWorkUnitAffinity(AffinityAffinity);
                Scheduler1.AddWorkUnit(AffinityC);
                Scheduler1.AddWorkUnit(AffinityD);
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


                /*if(true)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Warning; }
                AddTestResult("DAGFrameScheduler::temp", temp);*/
            }else{
                //AddTestResult("DAGFrameScheduler::temp", Testing::Skipped);

            }

            if(RunAutomaticTests)
            {
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
                EraseE->AddDependency(EraseD);
                EraseD->AddDependency(EraseC);
                EraseC->AddDependency(EraseB);
                EraseB->AddDependency(EraseA);

                cout << "Stuffing all 6 into a test frame scheduler and preparing it for a run" << endl;
                RemovalScheduler.AddWorkUnit(EraseA);
                RemovalScheduler.AddWorkUnit(EraseB);
                RemovalScheduler.AddWorkUnit(EraseC);
                RemovalScheduler.AddWorkUnit(EraseD);
                RemovalScheduler.AddWorkUnit(EraseE);
                RemovalScheduler.AddWorkUnit(EraseF);

                RemovalScheduler.SortWorkUnitsMain();

                cout << "Checking the order of the workunits (Should be \"A B C D E F\" or \"A B C D F E\"): ";
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
                cout << Next->Name << " " << endl; //E or F Whichever was not EitherOr
                Next->operator() (RemovalResource);
                if( (Next->Name=="E" || Next->Name=="F") && OtherEF!= Next) { PrepCount++; }
                Test((6==PrepCount),"DAGFrameScheduler::FrameScheduler::Erase::OrderingPreTest");

                cout << endl << "Removing F from Scheduler then resorting and resetting it (New order should be \"A B C D E\"): ";
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
                cout << Next->Name << " " << endl; //E
                Next->operator() (RemovalResource);
                if(Next->Name=="E") { PrepCount++; }
                Test(5==PrepCount, "DAGFrameScheduler::FrameScheduler::Erase::Simple");




            }else{
                AddTestResult("DAGFrameScheduler::FrameScheduler::Erase::OrderingPreTest", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::FrameScheduler::Erase::Simple", Testing::Skipped);
            }


        }
};

#endif

