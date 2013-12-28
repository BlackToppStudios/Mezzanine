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

        ThreadId InThread;

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
        void CheckSchedulerLog(Mezzanine::Logger& Log, Whole TargetThreadCount_, Whole WorkUnitCount_, String TestName)
        {
            pugi::xml_document Doc;
            Doc.load(Log);
            pugi::xml_node TestFrame = Doc.child("MezzanineLog").child("Frame");
            TEST(TestFrame, TestName+"::TestLog")

            pugi::xpath_node_set Results = Doc.select_nodes("MezzanineLog/Frame/Thread/WorkUnit/MakePi");
            TestOutput << "Found " << Results.size() << " WorkUnits expected " << WorkUnitCount_ << "." << endl;
            TEST(WorkUnitCount_==Results.size(),TestName+"::WorkUnitCount");

            Results = Doc.select_nodes("MezzanineLog/Frame[1]/Thread");
            TestOutput << "Found " << Results.size() << " threads expected " << TargetThreadCount_ << "." << endl;
            TEST(TargetThreadCount_==Results.size(),TestName+"::ThreadCount");

            Results = Doc.select_nodes("MezzanineLog/Frame/Thread/WorkUnit/MakePi/@WorkUnitName");
            TestOutput << "Found " << Results.size() << " WorkUnitNames expected " << WorkUnitCount_ << ", with the following names: ";
            for(pugi::xpath_node_set::const_iterator Iter = Results.begin();
                Results.end()!=Iter;
                ++Iter)
            { TestOutput << "\t" <<Iter->attribute().as_string(); }
            TestOutput << endl << endl;
            TEST(WorkUnitCount_==Results.size(),TestName+"::NameCount");

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
                TestOutput <<  "Creating a simple dependency chain in 4 WorkUnits and inserting them into a Test FrameScheduler. Then they will be pulled out one at a time and mark them as completed: " << endl;

                PiMakerWorkUnit *WorkUnitK1 = new PiMakerWorkUnit(500,"First",false);
                PiMakerWorkUnit *WorkUnitK2 = new PiMakerWorkUnit(500,"Second",false);
                PiMakerWorkUnit *WorkUnitK3 = new PiMakerWorkUnit(500,"Third",false);
                PiMakerWorkUnit *WorkUnitK4 = new PiMakerWorkUnit(500,"Fourth",false);

                FrameScheduler SchedulingTest1(&TestOutput,1);
                DefaultThreadSpecificStorage::Type Storage1(&SchedulingTest1);
                WorkUnitK4->AddDependency(WorkUnitK3);
                WorkUnitK3->AddDependency(WorkUnitK2);
                WorkUnitK2->AddDependency(WorkUnitK1);
                SchedulingTest1.AddWorkUnitMain(WorkUnitK1, WorkUnitK1->Name); // no deletes required the Scheduler takes ownership
                SchedulingTest1.AddWorkUnitMain(WorkUnitK2, WorkUnitK2->Name);
                SchedulingTest1.AddWorkUnitMain(WorkUnitK3, WorkUnitK3->Name);
                SchedulingTest1.AddWorkUnitMain(WorkUnitK4, WorkUnitK4->Name);
                SchedulingTest1.SortWorkUnitsMain();

                iWorkUnit* Counter = SchedulingTest1.GetNextWorkUnit();
                TestOutput << "Getting the WorkUnit Named " << ((PiMakerWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                TEST(((PiMakerWorkUnit*)Counter)->Name == String("First"),"BasicSorting::First");

                Counter->operator()(Storage1);
                Counter = SchedulingTest1.GetNextWorkUnit();
                TestOutput << "Getting the WorkUnit Named " << ((PiMakerWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                TEST(((PiMakerWorkUnit*)Counter)->Name == String("Second"),"BasicSorting::Second");

                Counter->operator()(Storage1);
                Counter = SchedulingTest1.GetNextWorkUnit();
                TestOutput << "Getting the WorkUnit Named " << ((PiMakerWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                TEST(((PiMakerWorkUnit*)Counter)->Name == String("Third"),"BasicSorting::Third");

                Counter->operator()(Storage1);
                Counter = SchedulingTest1.GetNextWorkUnit();
                TestOutput << "Getting the WorkUnit Named " << ((PiMakerWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                TEST(((PiMakerWorkUnit*)Counter)->Name == String("Fourth"),"BasicSorting::Fourth");

                Counter->operator()(Storage1);

                TestOutput << endl << "Creating 3 WorkUnits with precise runtimes and inserting them into a Test FrameScheduler. Then they will be pulled out one at a time and mark them as completed: " << endl;
                FrameScheduler SchedulingTest2(&TestOutput,1);
                DefaultThreadSpecificStorage::Type Storage2(&SchedulingTest2);

                PausesWorkUnit *FiveHundred = new PausesWorkUnit(500,"FiveHundred-ms");
                PausesWorkUnit *FiveThousand = new PausesWorkUnit(5000,"FiveThousand-ms");
                PausesWorkUnit *FiftyThousand = new PausesWorkUnit(50000,"FiftyThousand-ms");
                //PausesWorkUnit *FiveHundredThousand = new PausesWorkUnit(500000,"FiveHundredThousand-ms");
                TestOutput << "Work Units (FiveHundred-ms, FiveThousand-ms, FiftyThousand-ms)[ms is microseconds in this context] Created, executing each ten times: " << endl;
                for(Int8 Counter = 0; Counter <10; ++Counter)
                {
                    FiveHundred->operator()(Storage2);
                    FiveThousand->operator()(Storage2);
                    FiftyThousand->operator()(Storage2);
                    //FiveHundredThousand->operator()(Storage2,SchedulingTest2);
                }
                SchedulingTest2.AddWorkUnitMain(FiveHundred, FiveHundred->Name);
                SchedulingTest2.AddWorkUnitMain(FiftyThousand, FiftyThousand->Name);
                SchedulingTest2.AddWorkUnitMain(FiveThousand, FiftyThousand->Name);
                //SchedulingTest2.AddWorkUnit(FiveHundredThousand);

                TestOutput << "FiveHundred-ms   : " << FiveHundred->GetPerformanceLog().GetAverage() << endl;
                TestOutput << "FiveThousand-ms  : " << FiveThousand->GetPerformanceLog().GetAverage() << endl;
                TestOutput << "FiftyThousand-ms : " << FiftyThousand->GetPerformanceLog().GetAverage() << endl;
                //TestOutput << "FiveHundredThousand-ms  : " << FiveHundredThousand->GetPerformanceLog().GetAverage() << endl;
                TestOutput << "Marking each WorkUnit as usable for the next frame:" << endl;
                FiveHundred->PrepareForNextFrame();
                FiveThousand->PrepareForNextFrame();
                FiftyThousand->PrepareForNextFrame();
                //FiveHundredThousand->PrepareForNextFrame();
                SchedulingTest2.SortWorkUnitsMain();

                TestOutput << "Extracting WorkUnits with the scheduling mechanism: " << endl;
                // ©ounter = SchedulingTest2.GetNextWorkUnit();
                //TestOutput << "Getting the WorkUnit Named " << ((PausesWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                //assert( ((PausesWorkUnit*)Counter)->Name == String("FiveHundredThousand-ms") );
                // ©ounter->operator()(Storage2, SchedulingTest2);
                Counter = SchedulingTest2.GetNextWorkUnit();
                TestOutput << "Getting the WorkUnit Named " << ((PausesWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                TEST(((PausesWorkUnit*)Counter)->Name == String("FiftyThousand-ms"),"TimedSort::FiftyThousand-ms")

                Counter->operator()(Storage2);
                Counter = SchedulingTest2.GetNextWorkUnit();
                TestOutput << "Getting the WorkUnit Named " << ((PausesWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                TEST(((PausesWorkUnit*)Counter)->Name == String("FiveThousand-ms"),"TimedSort::FiveThousand-ms")

                Counter->operator()(Storage2);
                Counter = SchedulingTest2.GetNextWorkUnit();
                TestOutput << "Getting the WorkUnit Named " << ((PausesWorkUnit*)Counter)->Name << " and marking it as complete." << endl;
                TEST(((PausesWorkUnit*)Counter)->Name == String("FiveHundred-ms"),"TimedSort::FiveHundred-ms")
                Counter->operator()(Storage2);
            } // \Basic Sorting

            {
                TestOutput << std::endl << "Creating a FrameScheduler with 4 WorkUnits and a LogAggregator WorkUnit an Running one frame then redoing it with different thread counts: " << endl;

                PiMakerWorkUnit WorkUnitR1(50000,"Run1",false);
                PiMakerWorkUnit WorkUnitR2(50000,"Run2",false);
                PiMakerWorkUnit WorkUnitR3(50000,"Run3",false);
                PiMakerWorkUnit WorkUnitR4(50000,"Run4",false);
                LogAggregator Agg;

                stringstream LogCache;
                {
                    FrameScheduler ThreadCreationTest1(&LogCache,1);
                    ThreadCreationTest1.AddWorkUnitMain(&WorkUnitR1, WorkUnitR1.Name);
                    ThreadCreationTest1.AddWorkUnitMain(&WorkUnitR2, WorkUnitR2.Name);
                    ThreadCreationTest1.AddWorkUnitMain(&WorkUnitR3, WorkUnitR3.Name);
                    ThreadCreationTest1.AddWorkUnitMain(&WorkUnitR4, WorkUnitR4.Name);
                    ThreadCreationTest1.AddWorkUnitMain(&Agg,"Agg");
                    TestOutput << "Thread count on initial creation: " << ThreadCreationTest1.GetThreadCount() << endl;
                    TestOutput << "Running One Frame." << endl;
                    ThreadCreationTest1.DoOneFrame(); // Do the work
                    ThreadCreationTest1.RemoveWorkUnitMain(&WorkUnitR1);
                    ThreadCreationTest1.RemoveWorkUnitMain(&WorkUnitR2);
                    ThreadCreationTest1.RemoveWorkUnitMain(&WorkUnitR3);
                    ThreadCreationTest1.RemoveWorkUnitMain(&WorkUnitR4);
                    ThreadCreationTest1.DoOneFrame(); // Remove the work, but swap the log buffers.
                    ThreadCreationTest1.RemoveWorkUnitMain(&Agg);
                }
                TestOutput << "Emitting log:" << endl;
                TestOutput << LogCache.str() << endl;
                CheckSchedulerLog(LogCache,1,4,"ThreadTests::SingleThread");
                TestOutput << endl << endl;
                LogCache.str("");

                {
                    FrameScheduler ThreadCreationTest1(&LogCache,2);
                    ThreadCreationTest1.AddWorkUnitMain(&WorkUnitR1, WorkUnitR1.Name);
                    ThreadCreationTest1.AddWorkUnitMain(&WorkUnitR2, WorkUnitR2.Name);
                    ThreadCreationTest1.AddWorkUnitMain(&WorkUnitR3, WorkUnitR3.Name);
                    ThreadCreationTest1.AddWorkUnitMain(&WorkUnitR4, WorkUnitR4.Name);
                    ThreadCreationTest1.AddWorkUnitMain(&Agg,"Agg");
                    TestOutput << "Thread count on initial creation: " << ThreadCreationTest1.GetThreadCount() << endl;
                    TestOutput << "Running One Frame." << endl;
                    ThreadCreationTest1.DoOneFrame(); // Do the work
                    ThreadCreationTest1.RemoveWorkUnitMain(&WorkUnitR1);
                    ThreadCreationTest1.RemoveWorkUnitMain(&WorkUnitR2);
                    ThreadCreationTest1.RemoveWorkUnitMain(&WorkUnitR3);
                    ThreadCreationTest1.RemoveWorkUnitMain(&WorkUnitR4);
                    ThreadCreationTest1.DoOneFrame(); // Remove the work, but swap the log buffers.
                    ThreadCreationTest1.RemoveWorkUnitMain(&Agg);
                }
                TestOutput << "Emitting log:" << endl;
                TestOutput << LogCache.str() << endl;
                CheckSchedulerLog(LogCache,2,4,"ThreadTests::DualThread");
                TestOutput << endl << endl;
                LogCache.str("");

                {
                    FrameScheduler ThreadCreationTest1(&LogCache,3);
                    ThreadCreationTest1.AddWorkUnitMain(&WorkUnitR1, WorkUnitR1.Name);
                    ThreadCreationTest1.AddWorkUnitMain(&WorkUnitR2, WorkUnitR2.Name);
                    ThreadCreationTest1.AddWorkUnitMain(&WorkUnitR3, WorkUnitR3.Name);
                    ThreadCreationTest1.AddWorkUnitMain(&WorkUnitR4, WorkUnitR4.Name);
                    ThreadCreationTest1.AddWorkUnitMain(&Agg,"Agg");
                    TestOutput << "Thread count on initial creation: " << ThreadCreationTest1.GetThreadCount() << endl;
                    TestOutput << "Running One Frame." << endl;
                    ThreadCreationTest1.DoOneFrame(); // Do the work
                    ThreadCreationTest1.RemoveWorkUnitMain(&WorkUnitR1);
                    ThreadCreationTest1.RemoveWorkUnitMain(&WorkUnitR2);
                    ThreadCreationTest1.RemoveWorkUnitMain(&WorkUnitR3);
                    ThreadCreationTest1.RemoveWorkUnitMain(&WorkUnitR4);
                    ThreadCreationTest1.DoOneFrame(); // Remove the work, but swap the log buffers.
                    ThreadCreationTest1.RemoveWorkUnitMain(&Agg);
                }
                TestOutput << "Emitting log:" << endl;
                TestOutput << LogCache.str() << endl;
                CheckSchedulerLog(LogCache,3,4,"ThreadTests::TripleThread");
                TestOutput << endl << endl;
                LogCache.str("");

                {
                    FrameScheduler ThreadCreationTest1(&LogCache,4);
                    ThreadCreationTest1.AddWorkUnitMain(&WorkUnitR1, WorkUnitR1.Name);
                    ThreadCreationTest1.AddWorkUnitMain(&WorkUnitR2, WorkUnitR2.Name);
                    ThreadCreationTest1.AddWorkUnitMain(&WorkUnitR3, WorkUnitR3.Name);
                    ThreadCreationTest1.AddWorkUnitMain(&WorkUnitR4, WorkUnitR4.Name);
                    ThreadCreationTest1.AddWorkUnitMain(&Agg,"Agg");
                    TestOutput << "Thread count on initial creation: " << ThreadCreationTest1.GetThreadCount() << endl;
                    TestOutput << "Running One Frame." << endl;
                    ThreadCreationTest1.DoOneFrame(); // Do the work
                    ThreadCreationTest1.RemoveWorkUnitMain(&WorkUnitR1);
                    ThreadCreationTest1.RemoveWorkUnitMain(&WorkUnitR2);
                    ThreadCreationTest1.RemoveWorkUnitMain(&WorkUnitR3);
                    ThreadCreationTest1.RemoveWorkUnitMain(&WorkUnitR4);
                    ThreadCreationTest1.DoOneFrame(); // Remove the work, but swap the log buffers.
                    ThreadCreationTest1.RemoveWorkUnitMain(&Agg);
                }
                TestOutput << "Emitting log:" << endl;
                TestOutput << LogCache.str() << endl;
                CheckSchedulerLog(LogCache,4,4,"ThreadTests::DualThread");
                TestOutput << endl << endl;
                LogCache.str("");

            } // \threading tests

            { // Dependency
                TestOutput << "Creating a few WorkUnits with a " << endl;
                stringstream LogCache;
                TestOutput << "Creating WorkUnits a Dependency chain as follows: "
                        << endl << "    +--->B"
                        << endl << "    |"
                        << endl << "A---+"
                        << endl << "    |"
                        << endl << "    +--->C"
                        << endl;
                PausesWorkUnit RestartA(100000,"A");
                PausesWorkUnit RestartB(100000,"B");
                PausesWorkUnit RestartC(100000,"C");
                LogAggregator Agg3;
                RestartB.AddDependency(&RestartA);
                RestartC.AddDependency(&RestartA);
                LogCache.str("");

                {
                    FrameScheduler RestartScheduler1(&LogCache,2);
                    RestartScheduler1.AddWorkUnitMain(&RestartA, RestartA.Name);
                    RestartScheduler1.AddWorkUnitMain(&RestartB, RestartB.Name);
                    RestartScheduler1.AddWorkUnitMain(&RestartC, RestartC.Name);
                    RestartScheduler1.SortWorkUnitsMain();
                    RestartScheduler1.DoOneFrame();
                    RestartScheduler1.RemoveWorkUnitMain(&RestartA); // Remove work to prevent multiple execution
                    RestartScheduler1.RemoveWorkUnitMain(&RestartB);
                    RestartScheduler1.RemoveWorkUnitMain(&RestartC);
                    RestartScheduler1.AddWorkUnitMain(&Agg3,"Agg");        // To get the logs
                    RestartScheduler1.DoOneFrame();
                    RestartScheduler1.RemoveWorkUnitMain(&Agg3);
                }
                TestOutput << LogCache.str() << endl << "Parsing log to determine if everything happened correctly" << endl;
                pugi::xml_document Doc;
                Doc.load(LogCache);

                pugi::xml_node Thread1Node = Doc.child("MezzanineLog").child("Frame").first_child();
                pugi::xml_node Thread2Node = Doc.child("MezzanineLog").child("Frame").last_child();
                TEST(Thread1Node,"BasicDependency::LogNode1");
                TEST(Thread2Node,"BasicDependency::LogNode2");


                pugi::xpath_node_set Results;
                Results = Doc.select_nodes("MezzanineLog/Frame/Thread/WorkUnit/Pause[@WorkUnitName='B']/@ThreadID");
                String BThread(Results.first().attribute().as_string());
                Results = Doc.select_nodes("MezzanineLog/Frame/Thread/WorkUnit/Pause[@WorkUnitName='C']/@ThreadID");
                String CThread(Results.first().attribute().as_string());
                //String AEnd;
                //String BStart;
                //String CStart;

                //TestOutput << "The timer cannot resolve times less then: " << GetTimeStampResolution() << endl;
                //TestOutput << "Was A complete before B started: " << (ToWhatever<MaxInt>(AEnd)<=ToWhatever<MaxInt>(BStart)+ToWhatever<MaxInt>(GetTimeStampResolution())) << endl;
                //TestOutput << "Was A complete before B started if the clock resolution could cause error: " << (ToWhatever<MaxInt>(AEnd)<=(ToWhatever<MaxInt>(BStart)+ToWhatever<MaxInt>(GetTimeStampResolution()))) << endl;
                //TEST(ToWhatever<MaxInt>(AEnd)<=(ToWhatever<MaxInt>(BStart)+ToWhatever<MaxInt>(GetTimeStampResolution())),"BasicSorting::ABeforeB");
                //TestOutput << "Was A complete before C started: " << (ToWhatever<MaxInt>(AEnd)<=ToWhatever<MaxInt>(CStart)) << endl;
                //TestOutput << "Was A complete before C started if the clock resolution could cause error: " << (ToWhatever<MaxInt>(AEnd)<=(ToWhatever<MaxInt>(CStart)+ToWhatever<MaxInt>(GetTimeStampResolution()))) << endl;
                //TEST(ToWhatever<MaxInt>(AEnd)<=(ToWhatever<MaxInt>(CStart)+ToWhatever<MaxInt>(GetTimeStampResolution())),"BasicSorting::ABeforeC");
                TestOutput << "Were B and C run in different threads: " << (BThread!=CThread) << endl;
                TEST(BThread!=CThread,"BasicSorting::BNotInCThread");
            }

            { // Removal
                stringstream LogCache;
                FrameScheduler RemovalScheduler(&LogCache,1);
                ThreadSpecificStorage RemovalResource(&RemovalScheduler);

                TestOutput << "Creating 5 workunits each depending on the next: A -> B -> C -> D -> E" << endl
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

                TestOutput << "Stuffing all 6 into a test frame scheduler and preparing it for a run" << endl;
                RemovalScheduler.AddWorkUnitMain(EraseA, EraseA->Name);
                RemovalScheduler.AddWorkUnitMain(EraseB, EraseB->Name);
                RemovalScheduler.AddWorkUnitMain(EraseC, EraseC->Name);
                RemovalScheduler.AddWorkUnitMain(EraseD, EraseD->Name);
                RemovalScheduler.AddWorkUnitMain(EraseE, EraseE->Name);
                RemovalScheduler.AddWorkUnitMain(EraseF, EraseF->Name);

                RemovalScheduler.SortWorkUnitsMain();

                TestOutput << "Checking the order of the workunits (Should be \"A B C D E F NULL\" or \"A B C D F E NULL\"): ";
                Whole PrepCount = 0;

                PausesWorkUnit* Next;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; //A
                Next->operator() (RemovalResource);
                if(Next->Name=="A") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; // ©
                Next->operator() (RemovalResource);
                if(Next->Name=="C") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; //E or F
                Next->operator() (RemovalResource);
                PausesWorkUnit* OtherEF = Next;
                if(Next->Name=="E" || Next->Name=="F") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; //E or F Whichever was not EitherOr
                Next->operator() (RemovalResource);
                if( (Next->Name=="E" || Next->Name=="F") && OtherEF!= Next) { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnit())
                    { TestOutput << Next->Name << " "; }
                else
                    { TestOutput << "NULL "; PrepCount++; }
                TestOutput << endl;
                TEST((7==PrepCount),"RemoveMain::OrderingPreTest");

                TestOutput << endl << "Removing F from Scheduler then resorting and resetting it (New order should be \"A B C D E NULL\"): ";
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.RemoveWorkUnitMain(EraseF);
                RemovalScheduler.SortWorkUnitsMain();
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; //A
                Next->operator() (RemovalResource);
                if(Next->Name=="A") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; // ©
                Next->operator() (RemovalResource);
                if(Next->Name=="C") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; //E
                Next->operator() (RemovalResource);
                if(Next->Name=="E") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnit())
                    { TestOutput << Next->Name << " "; }
                else
                    { TestOutput << "NULL "; PrepCount++; }
                TestOutput << endl;
                TEST(6==PrepCount, "RemoveMain::Simple");

                TestOutput << endl << "Removing E from Scheduler then resorting and resetting it (New order should be \"A B C D NULL\"): ";
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.RemoveWorkUnitMain(EraseE);
                RemovalScheduler.SortWorkUnitsMain();
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; //A
                Next->operator() (RemovalResource);
                if(Next->Name=="A") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; // ©
                Next->operator() (RemovalResource);
                if(Next->Name=="C") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnit())
                    { TestOutput << Next->Name << " "; }
                else
                    { TestOutput << "NULL "; PrepCount++; }
                TestOutput << endl;
                TEST(5==PrepCount, "RemoveMain::WithDep");

                TestOutput << endl << "Removing A from Scheduler then resorting and resetting it (New order should be \"B C D NULL\"): ";
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.RemoveWorkUnitMain(EraseA);
                RemovalScheduler.SortWorkUnitsMain();
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; // ©
                Next->operator() (RemovalResource);
                if(Next->Name=="C") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnit())
                    { TestOutput << Next->Name << " "; }
                else
                    { TestOutput << "NULL "; PrepCount++; }
                TestOutput << endl;
                TEST(4==PrepCount, "RemoveMain::AsDep");

                TestOutput << endl << "Removing C from Scheduler then resorting and resetting it (New order should be \"B D NULL\" or \"D B NULL\"): ";
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.RemoveWorkUnitMain(EraseC);
                RemovalScheduler.SortWorkUnitsMain();
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                PausesWorkUnit* BorD = Next;
                TestOutput << Next->Name << " "; //B or D
                Next->operator() (RemovalResource);
                if(Next->Name=="B"||Next->Name=="D") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnit());
                TestOutput << Next->Name << " "; // The other
                Next->operator() (RemovalResource);
                if( (Next->Name=="B"||Next->Name=="D") && Next!=BorD) { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnit())
                    { TestOutput << Next->Name << " "; }
                else
                    { TestOutput << "NULL "; PrepCount++; }
                TestOutput << endl;
                TEST(3==PrepCount, "RemoveMain::WithAndAsDep");


                TestOutput << endl << "Adding an Affinity WorkUnit Z and making it depend on B (new order should be B, Z, D, NULL): ";
                RemovalScheduler.AddWorkUnitAffinity(EraseZ, EraseZ->Name);
                EraseZ->AddDependency(EraseB);
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.SortWorkUnitsAll(); //Sorting all on this test
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //Z
                Next->operator() (RemovalResource);
                if(Next->Name=="Z") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { TestOutput << Next->Name << " "; }
                else
                    { TestOutput << "NULL "; PrepCount++; }
                TestOutput << endl;
                TEST(4==PrepCount, "RemoveMain::MainPreTest");

                TestOutput << endl << "Removing B and verifying no infinite loops are added(new order should be Z, D, NULL): ";
                RemovalScheduler.RemoveWorkUnitMain(EraseB);
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.SortWorkUnitsAll(); //Sorting all on this test
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //Z
                Next->operator() (RemovalResource);
                if(Next->Name=="Z") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { TestOutput << Next->Name << " "; }
                else
                    { TestOutput << "NULL "; PrepCount++; }
                TestOutput << endl;
                TEST(3==PrepCount, "RemoveMain::MainCleanup");

                delete EraseA; delete EraseB; delete EraseC; delete EraseE; delete EraseF;
            } // \Removal

            { // Affinty
                stringstream LogCache;
                TestOutput << "Creating WorkUnits a Dependency chain as follows: "
                        << endl << "A---+                  +--->B"
                        << endl << "    |                  |"
                        << endl << "    +-->AffinityUnit---+"
                        << endl << "    |                  |"
                        << endl << "B---+                  +--->C"
                        << endl;
                PausesWorkUnit AffinityA(10000,"A");
                PausesWorkUnit AffinityB(10000,"B");
                PausesWorkUnit AffinityAffinity(10000,"Affinity");
                PausesWorkUnit AffinityC(10000,"C");
                PausesWorkUnit AffinityD(10000,"D");
                AffinityAffinity.AddDependency(&AffinityA);
                AffinityAffinity.AddDependency(&AffinityB);
                AffinityC.AddDependency(&AffinityAffinity);
                AffinityD.AddDependency(&AffinityAffinity);

                FrameScheduler Scheduler1(&LogCache,2);
                LogAggregator Agg1;

                Scheduler1.AddWorkUnitMain(&AffinityA, AffinityA.Name);
                Scheduler1.AddWorkUnitMain(&AffinityB, AffinityB.Name);
                Scheduler1.AddWorkUnitAffinity(&AffinityAffinity, AffinityAffinity.Name);
                Scheduler1.AddWorkUnitMain(&AffinityC, AffinityC.Name);
                Scheduler1.AddWorkUnitMain(&AffinityD, AffinityD.Name);
                Scheduler1.SortWorkUnitsMain();
                Scheduler1.DoOneFrame();
                Scheduler1.AddWorkUnitMain(&Agg1,"Agg1");
                Scheduler1.RemoveWorkUnitMain(&AffinityA);
                Scheduler1.RemoveWorkUnitMain(&AffinityB);
                Scheduler1.RemoveWorkUnitAffinity(&AffinityAffinity);
                Scheduler1.RemoveWorkUnitMain(&AffinityC);
                Scheduler1.RemoveWorkUnitMain(&AffinityD);
                Scheduler1.DoOneFrame();
                Scheduler1.RemoveWorkUnitMain(&Agg1);

                // Check that two threads exist and that B and C run in different thread, and after A finished
                TestOutput << "Affinity should run in this This thread and this thread has id: " << Mezzanine::Threading::this_thread::get_id() << endl;
                TestOutput << LogCache.str() << endl << "Parsing log to determine if everything happened correctly" << endl;

                pugi::xml_document Doc;
                Doc.load(LogCache);

                pugi::xpath_node_set MakePiNodes = Doc.select_nodes("MezzanineLog/Frame/Thread/WorkUnit/MakePi");
                //TestOutput << "Found " << MakePiNodes.size() << " MakePI workUnits executed, expected " << Expected << "." << endl;
                //TEST(MakePiNodes.size()==Expected,"WorkLogged");


            } // \Affinity

            { // \ Removal Affinity
                stringstream LogCache;
                FrameScheduler RemovalScheduler(&LogCache,1);
                ThreadSpecificStorage RemovalResource(&RemovalScheduler);

                TestOutput << endl << "Creating 5 workunits each depending on the next: A -> B -> C -> D -> E" << endl
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

                TestOutput << "Stuffing all 6 into a test frame scheduler as affinity workunit and preparing it for a run" << endl;
                RemovalScheduler.AddWorkUnitAffinity(EraseA, EraseA->Name);
                RemovalScheduler.AddWorkUnitAffinity(EraseB, EraseB->Name);
                RemovalScheduler.AddWorkUnitAffinity(EraseC, EraseC->Name);
                RemovalScheduler.AddWorkUnitAffinity(EraseD, EraseD->Name);
                RemovalScheduler.AddWorkUnitAffinity(EraseE, EraseE->Name);
                RemovalScheduler.AddWorkUnitAffinity(EraseF, EraseF->Name);

                RemovalScheduler.SortWorkUnitsAffinity();

                TestOutput << "Checking the order of the workunits (Should be \"A B C D E F NULL\" or \"A B C D F E NULL\"): ";
                Whole PrepCount = 0;

                PausesWorkUnit* Next;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //A
                Next->operator() (RemovalResource);
                if(Next->Name=="A") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; // ©
                Next->operator() (RemovalResource);
                if(Next->Name=="C") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //E or F
                Next->operator() (RemovalResource);
                PausesWorkUnit* OtherEF = Next;
                if(Next->Name=="E" || Next->Name=="F") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //E or F Whichever was not EitherOr
                Next->operator() (RemovalResource);
                if( (Next->Name=="E" || Next->Name=="F") && OtherEF!= Next) { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { TestOutput << Next->Name << " "; }
                else
                    { TestOutput << "NULL "; PrepCount++; }
                TestOutput << endl;
                TEST((7==PrepCount),"RemoveAffinity::OrderingPreTest");

                TestOutput << endl << "Removing F from Scheduler then resorting and resetting it (New order should be \"A B C D E NULL\"): ";
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.RemoveWorkUnitAffinity(EraseF);
                RemovalScheduler.SortWorkUnitsAffinity();
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //A
                Next->operator() (RemovalResource);
                if(Next->Name=="A") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; // ©
                Next->operator() (RemovalResource);
                if(Next->Name=="C") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //E
                Next->operator() (RemovalResource);
                if(Next->Name=="E") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { TestOutput << Next->Name << " "; }
                else
                    { TestOutput << "NULL "; PrepCount++; }
                TestOutput << endl;
                TEST((6==PrepCount),"RemoveAffinity::Simple");

                TestOutput << endl << "Removing E from Scheduler then resorting and resetting it (New order should be \"A B C D NULL\"): ";
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.RemoveWorkUnitAffinity(EraseE);
                RemovalScheduler.SortWorkUnitsAffinity();
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //A
                Next->operator() (RemovalResource);
                if(Next->Name=="A") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; // ©
                Next->operator() (RemovalResource);
                if(Next->Name=="C") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { TestOutput << Next->Name << " "; }
                else
                    { TestOutput << "NULL "; PrepCount++; }
                TestOutput << endl;
                TEST((5==PrepCount),"RemoveAffinity::WithDep");

                TestOutput << endl << "Removing A from Scheduler then resorting and resetting it (New order should be \"B C D NULL\"): ";
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.RemoveWorkUnitAffinity(EraseA);
                RemovalScheduler.SortWorkUnitsAffinity();
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; // ©
                Next->operator() (RemovalResource);
                if(Next->Name=="C") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { TestOutput << Next->Name << " "; }
                else
                    { TestOutput << "NULL "; PrepCount++; }
                TestOutput << endl;
                TEST((4==PrepCount),"RemoveAffinity::AsDep");

                TestOutput << endl << "Removing C from Scheduler then resorting and resetting it (New order should be \"B D NULL\" or \"D B NULL\"): ";
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.RemoveWorkUnitAffinity(EraseC);
                RemovalScheduler.SortWorkUnitsAffinity();
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                PausesWorkUnit* BorD = Next;
                TestOutput << Next->Name << " "; //B or D
                Next->operator() (RemovalResource);
                if(Next->Name=="B"||Next->Name=="D") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; // The other
                Next->operator() (RemovalResource);
                if( (Next->Name=="B"||Next->Name=="D") && Next!=BorD) { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { TestOutput << Next->Name << " "; }
                else
                    { TestOutput << "NULL "; PrepCount++; }
                TestOutput << endl;
                TEST((3==PrepCount),"RemoveAffinity::WithAndAsDep");

                TestOutput << endl << "Adding a non-affinity/Main WorkUnit Z and making it depend on B (new order should be B, D, Z, NULL): ";
                RemovalScheduler.AddWorkUnitMain(EraseZ, EraseZ->Name);
                EraseZ->AddDependency(EraseB);
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.SortWorkUnitsAll(); //Sorting all on this test
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //B
                Next->operator() (RemovalResource);
                if(Next->Name=="B") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //Z
                Next->operator() (RemovalResource);
                if(Next->Name=="Z") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { TestOutput << Next->Name << " "; }
                else
                    { TestOutput << "NULL "; PrepCount++; }
                TestOutput << endl;
                TEST((4==PrepCount),"RemoveAffinity::MainPreTest");

                TestOutput << endl << "Removing B and verifying no infinite loops are added(new order should be D, Z, NULL): ";
                RemovalScheduler.RemoveWorkUnitAffinity(EraseB);
                RemovalScheduler.ResetAllWorkUnits();
                RemovalScheduler.SortWorkUnitsAll(); //Sorting all on this test
                PrepCount = 0;
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //D
                Next->operator() (RemovalResource);
                if(Next->Name=="D") { PrepCount++; }
                Next = static_cast<PausesWorkUnit*>(RemovalScheduler.GetNextWorkUnitAffinity());
                TestOutput << Next->Name << " "; //Z
                Next->operator() (RemovalResource);
                if(Next->Name=="Z") { PrepCount++; }
                if(RemovalScheduler.GetNextWorkUnitAffinity())
                    { TestOutput << Next->Name << " "; }
                else
                    { TestOutput << "NULL "; PrepCount++; }
                TestOutput << endl;
                TEST((3==PrepCount),"RemoveAffinity::MainCleanup");

                delete EraseA; delete EraseB; delete EraseC; delete EraseE; delete EraseF;
            }// \ Removal Affinity

            { // Removal Monopoly
                stringstream LogCache;
                FrameScheduler RemovalScheduler(&LogCache,1);
                //ThreadSpecificStorage RemovalResource(&RemovalScheduler);

                TestOutput << endl << "Creating 3 Monopoly Workunits and an affinity Workunit and a normal/Main workunit" << endl;
                PauseMonopoly *EraseMonoA = new PauseMonopoly(10,"MonoA");
                PauseMonopoly *EraseMonoB = new PauseMonopoly(10,"MonoB");
                PauseMonopoly *EraseMonoC = new PauseMonopoly(10,"MonoC");

                PausesWorkUnit *EraseA = new PausesWorkUnit(10,"NeedsA");
                PausesWorkUnit *EraseB = new PausesWorkUnit(10,"NeedsB");

                EraseA->AddDependency(EraseMonoA);
                EraseB->AddDependency(EraseMonoB);

                RemovalScheduler.AddWorkUnitAffinity(EraseA, EraseA->Name);
                RemovalScheduler.AddWorkUnitMain(EraseB, EraseB->Name);
                RemovalScheduler.AddWorkUnitMonopoly(EraseMonoA, EraseMonoA->Name);
                RemovalScheduler.AddWorkUnitMonopoly(EraseMonoB, EraseMonoB->Name);
                RemovalScheduler.AddWorkUnitMonopoly(EraseMonoC, EraseMonoC->Name);

                TestOutput << "Test Scheduler has " << RemovalScheduler.GetWorkUnitMonopolyCount() << " WorkUnits, and the affinity unit has " << EraseA->GetDependencyCount()
                     << " Dependencies and the main workunit has " << EraseB->GetDependencyCount() << "." << endl;
                Test(EraseA->GetDependencyCount()==1 &&
                     EraseB->GetDependencyCount()==1 &&
                     RemovalScheduler.GetWorkUnitMonopolyCount()==3, "DAGFrameScheduler::FrameScheduler::RemoveMopoly::PreTest");

                RemovalScheduler.RemoveWorkUnitMonopoly(EraseMonoC);
                TestOutput << "Removing Monopoly is no dependent, now the are " << RemovalScheduler.GetWorkUnitMonopolyCount() << " Monopolies (should be 2)." << endl << endl;
                Test(RemovalScheduler.GetWorkUnitMonopolyCount()==2, "DAGFrameScheduler::FrameScheduler::RemoveMopoly::Simple");

                RemovalScheduler.RemoveWorkUnitMonopoly(EraseMonoA);
                TestOutput << "Removing Monopoly is a dependent for an Affinity unit, now the are " << RemovalScheduler.GetWorkUnitMonopolyCount() << " Monopolies (should be 1)" << endl
                     << "and the affinity unit has " << EraseA->GetDependencyCount() << " deps (should be 0)." << endl << endl;
                Test(RemovalScheduler.GetWorkUnitMonopolyCount()==1 &&
                     EraseA->GetDependencyCount()==0, "DAGFrameScheduler::FrameScheduler::RemoveMopoly::AffinityDep");

                RemovalScheduler.RemoveWorkUnitMonopoly(EraseMonoB);
                TestOutput << "Removing Monopoly is a dependent for an main unit, now the are " << RemovalScheduler.GetWorkUnitMonopolyCount() << " Monopolies (should be 0)" << endl
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
                Scheduler1.AddWorkUnitMain(Checker1, "Checker1"); //scheduler should delete workunit
                Scheduler1.AddWorkUnitMain(Checker2, "Checker2");
                Scheduler1.AddWorkUnitMain(Checker3, "Checker3"); // There is no guarantee these will run in different threads, but unless there is
                Scheduler1.AddWorkUnitMain(Checker4, "Checker4"); // a huge delay in starting these workunits the FS really should

                Scheduler1.SortWorkUnitsAll();
                Scheduler1.DoOneFrame();

                TestOutput << "Checking that FrameScheduler::GetThreadResource produces the same results as what is passed into WorkUnit::DoWork: "
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

                TestOutput << "Checking that FrameScheduler::GetThreadUsableLogger produces the same results as what is passed into WorkUnit::DoWork: "
                     << endl
                     << dec << "from thread :" << Checker1->InThread << " - " << hex
                        << (Checker1->LogFromArgs) << " == " << (Checker1->LogFromGet) << " : " << (Checker1->LogFromArgs==Checker1->LogFromGet) << endl
                     << dec << "from thread :" << Checker2->InThread << " - " << hex
                        << (Checker2->LogFromArgs) << " == " << (Checker2->LogFromGet) << " : " << (Checker2->LogFromArgs==Checker2->LogFromGet) << endl
                     << dec << "from thread :" << Checker3->InThread << " - " << hex
                        << (Checker3->LogFromArgs) << " == " << (Checker3->LogFromGet) << " : " << (Checker3->LogFromArgs==Checker3->LogFromGet) << endl
                     << dec << "from thread :" << Checker4->InThread << " - " << hex
                        << (Checker4->LogFromArgs) << " == " << (Checker4->LogFromGet) << " : " << (Checker4->LogFromArgs==Checker4->LogFromGet) << endl
                     << endl << fixed << dec ;
                TEST(Checker1->LogFromArgs==Checker1->LogFromGet &&
                     Checker2->LogFromArgs==Checker2->LogFromGet &&
                     Checker3->LogFromArgs==Checker3->LogFromGet &&
                     Checker4->LogFromArgs==Checker4->LogFromGet
                     ,"GetThreadUsableLogger");
            }

            {
                stringstream LogCache;
                FrameScheduler Scheduler1(&LogCache);
                MaxInt FrameLength=10000;
                MaxInt FrameCount=100;
                MaxInt TargetLength = FrameCount*FrameLength;
                Scheduler1.SetFrameLength(FrameLength);

                TestOutput << "Creating a Framescheduler and running it empty for brief period to measure its accuracy." << endl;
                TEST_TIMED (
                                for(MaxInt Counter = 0; Counter<FrameCount; Counter++)
                                    { Scheduler1.DoOneFrame(); }
                            ,TargetLength, 0.02, "MultiFrameTiming"
                            );

                // get the last inserted frame length
                MaxInt ActualLength = Scheduler1.GetFrameTimeRollingAverage()[Scheduler1.GetFrameTimeRollingAverage().RecordCapacity()-1];
                TestOutput << "Expected last frame to be " << FrameLength << " microseconds and it was " << ActualLength << " microseconds." << endl;
                TEST( (FrameLength-200<ActualLength) && (ActualLength<FrameLength+200),"LastFrameData" );

                MaxInt PauseLength = Scheduler1.GetPauseTimeRollingAverage()[Scheduler1.GetPauseTimeRollingAverage().RecordCapacity()-1];
                TestOutput << "Without know the performance of this machine ahead of time, knowing the size of the pause is impossible, how it can be tested for sane values, it is: "
                     << PauseLength<< "  microseconds." << endl;
                TEST( (0<=PauseLength) && (PauseLength<=FrameLength+1),"LastPauseData" );

            }


        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }
};

#endif

