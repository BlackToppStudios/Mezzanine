// The DAGFrameScheduler is a Multi-Threaded lock free and wait free scheduling library.
// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _framescheduler_cpp
#define _framescheduler_cpp

#include "framescheduler.h"
#include "doublebufferedresource.h"
#include "monopoly.h"
#include "frameschedulerworkunits.h"
#include "lockguard.h"
#ifdef MEZZ_USEATOMICSTODECACHECOMPLETEWORK
    #include "atomicoperations.h"
#endif


#include <exception>
#include <iostream>
#include <algorithm>

using namespace std;

/// @file
/// @brief This is the core object implementation for this algorithm

#ifdef _MEZZ_THREAD_WIN32_
    #ifdef _MSC_VER
        #pragma warning( disable : 4706) // Disable Legitimate assignment in a WorkUnit acquisition loops
    #endif
#endif


namespace Mezzanine
{
    namespace Threading
    {
        /// @cond false

        // Initializing static members
        SpinLock FrameScheduler::FrameSchedulersLock;
        std::vector<FrameScheduler*> FrameScheduler::FrameSchedulers;

        /// @brief The function Frameschedulers have std::terminate() call.
        /// @details This will iterate over every frame scheduler, finds and activate its log aggregator, and flush its log.
        void TerminateHandler()
        {
            lock_guard<SpinLock> g(FrameScheduler::FrameSchedulersLock);
            for(std::vector<FrameScheduler*>::iterator Iter = FrameScheduler::FrameSchedulers.begin();
                Iter != FrameScheduler::FrameSchedulers.end();
                Iter++)
            {
                FrameScheduler* CurrentFrameScheduler = *Iter;
                CurrentFrameScheduler->ForceLogFlush();
            }
            exit(1);
        }

        /// @brief This is the function that all threads will run, except the main.
        /// @param ThreadStorage A pointer to a ThreadSpecificStorage that has the required data for a thread after it launches.
        void ThreadWork(void* ThreadStorage)
        {
            DefaultThreadSpecificStorage::Type& Storage = *((DefaultThreadSpecificStorage::Type*)ThreadStorage);
            FrameScheduler& FS = *(Storage.GetFrameScheduler());
            iWorkUnit* CurrentUnit;

            do
            {
                while( (CurrentUnit = FS.GetNextWorkUnit()) ) /// @todo needs to skip ahead a unit instead of spinning
                {
                    if(Starting==CurrentUnit->TakeOwnerShip())
                        { CurrentUnit->operator()(Storage); }
                }
            } while(!FS.AreAllWorkUnitsComplete());
        }

        /// @brief This is the function that the main thread runs.
        /// @param ThreadStorage A pointer to a ThreadSpecificStorage that has the required data for a thread after it launches.
        void ThreadWorkAffinity(void* ThreadStorage)
        {
            DefaultThreadSpecificStorage::Type& Storage = *((DefaultThreadSpecificStorage::Type*)ThreadStorage);
            FrameScheduler& FS = *(Storage.GetFrameScheduler());
            iWorkUnit* CurrentUnit;
            do
            {
                while( (CurrentUnit = FS.GetNextWorkUnitAffinity()) ) /// @todo needs to skip ahead a unit instead of spinning
                {
                    if(Starting==CurrentUnit->TakeOwnerShip())
                        { CurrentUnit->operator()(Storage); }
                }
            }
            while(!FS.AreAllWorkUnitsComplete());
        }
        /// @endcond

        ////////////////////////////////////////////////////////////////////////////////
        // Protected Methods

        void FrameScheduler::AddErrorScheduler(FrameScheduler* SchedulerToAdd)
        {
            lock_guard<SpinLock> g(FrameSchedulersLock);
            FrameSchedulers.push_back(SchedulerToAdd);
        }

        void FrameScheduler::RemoveErrorScheduler(FrameScheduler* SchedulerToRemove)
        {
            lock_guard<SpinLock> g(FrameSchedulersLock);
            FrameSchedulers.erase( std::remove( FrameSchedulers.begin(), FrameSchedulers.end(), SchedulerToRemove ), FrameSchedulers.end() );
        }


        void FrameScheduler::CleanUpThreads()
            { JoinAllThreads(); }

        void FrameScheduler::DeleteThreads()
        {
            for(std::vector<Thread*>::iterator Iter = Threads.begin(); Iter!=Threads.end(); ++Iter)
                { delete *Iter; }
        }

        void FrameScheduler::UpdateDependentGraph(const std::vector<WorkUnitKey>& Units)
        {
            // for each WorkUnit
            for(std::vector<WorkUnitKey>::const_iterator Iter=Units.begin(); Iter!=Units.end(); ++Iter)
            {
                // For each Dependent of that Workunit
                size_t Max = Iter->Unit->GetImmediateDependencyCount();
                for(size_t Counter=0; Counter<Max; ++Counter)
                    { DependentGraph[Iter->Unit->GetDependency(Counter)].insert(Iter->Unit); } // Make a record of the reverse association
            }
        }

        void FrameScheduler::UpdateWorkUnitKeys(std::vector<WorkUnitKey> &Units)
        {
            for(std::vector<WorkUnitKey>::iterator Iter=Units.begin(); Iter!=Units.end(); ++Iter)
                { *Iter = Iter->Unit->GetSortingKey(*this); }
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction
        FrameScheduler::FrameScheduler(std::fstream *_LogDestination, Whole StartingThreadCount) :
            FrameTimeLog(MEZZ_FRAMESTOTRACK),
            PauseTimeLog(MEZZ_FRAMESTOTRACK),
            CurrentFrameStart(GetTimeStamp()),
            CurrentPauseStart(GetTimeStamp()),
            Sorter(0),
            #ifdef MEZZ_USEATOMICSTODECACHECOMPLETEWORK
            DecacheMain(0),
            DecacheAffinity(0),
            #endif
            CurrentThreadCount(StartingThreadCount),
            FrameCount(0), TargetFrameLength(16666),
            TimingCostAllowance(0),
            MainThreadID(this_thread::get_id()),
            LoggingToAnOwnedFileStream(true),
            NeedToLogDeps(true)
        {
            Resources.push_back(new DefaultThreadSpecificStorage::Type(this));
            if(_LogDestination)
                { InstallLog(_LogDestination); }
            else
                { InstallLog(new std::fstream("Mezzanine.log", std::ios::out | std::ios::trunc)); }
            AddErrorScheduler(this);
            SetErrorHandler();
        }

        FrameScheduler::FrameScheduler(std::ostream *_LogDestination, Whole StartingThreadCount) :
            FrameTimeLog(MEZZ_FRAMESTOTRACK),
            PauseTimeLog(MEZZ_FRAMESTOTRACK),
            CurrentFrameStart(GetTimeStamp()),
            CurrentPauseStart(GetTimeStamp()),

            Sorter(0),
            #ifdef MEZZ_USEATOMICSTODECACHECOMPLETEWORK
            DecacheMain(0),
            DecacheAffinity(0),
            #endif
            CurrentThreadCount(StartingThreadCount),
            FrameCount(0), TargetFrameLength(16666),
            TimingCostAllowance(0),
            MainThreadID(this_thread::get_id()),
            LoggingToAnOwnedFileStream(false),
            NeedToLogDeps(true)
        {
            Resources.push_back(new DefaultThreadSpecificStorage::Type(this));
            InstallLog(_LogDestination);
            AddErrorScheduler(this);
            SetErrorHandler();
        }

        void FrameScheduler::SetErrorHandler()
        {
            std::set_terminate(TerminateHandler);
        }

        FrameScheduler::~FrameScheduler()
        {
            RemoveErrorScheduler(this);
            CleanUpThreads();
            RemoveLog();
            for(std::vector<WorkUnitKey>::iterator Iter=WorkUnitsMain.begin(); Iter!=WorkUnitsMain.end(); ++Iter)
                { delete Iter->Unit; }
            for(std::vector<MonopolyWorkUnit*>::iterator Iter = WorkUnitsMonopolies.begin(); Iter!=WorkUnitsMonopolies.end(); ++Iter)
                { delete *Iter; }
            for(std::vector<DefaultThreadSpecificStorage::Type*>::iterator Iter = Resources.begin(); Iter!=Resources.end(); ++Iter)
                { delete *Iter; }
            DeleteThreads();
        }

        ////////////////////////////////////////////////////////////////////////////////
        // WorkUnit management
        void FrameScheduler::AddWorkUnitMain(iWorkUnit* MoreWork, const String& WorkUnitName)
        {
            DependenciesChanged();
            this->WorkUnitsMain.push_back(MoreWork->GetSortingKey(*this));
            (*this->LogDestination) << "<WorkUnitMainInsertion ID=\"" << hex << MoreWork << "\" Name=\"" << WorkUnitName << "\" />" << endl;
        }

        void FrameScheduler::AddWorkUnitAffinity(iWorkUnit* MoreWork, const String& WorkUnitName)
        {
            DependenciesChanged();
            this->WorkUnitsAffinity.push_back(MoreWork->GetSortingKey(*this));
            (*this->LogDestination) << "<WorkUnitAffinityInsertion ID=\"" << hex << MoreWork << "\" Name=\"" << WorkUnitName << "\" />" << endl;
        }

        void FrameScheduler::AddWorkUnitMonopoly(MonopolyWorkUnit* MoreWork, const String& WorkUnitName)
        {
            DependenciesChanged();
            this->WorkUnitsMonopolies.push_back(MoreWork);
            (*this->LogDestination) << "<WorkUnitMonopolyInsertion ID=\"" << hex << MoreWork << "\" Name=\"" << WorkUnitName << "\" />" << endl;
        }

        void FrameScheduler::SortWorkUnitsMain(bool UpdateDependentGraph_)
        {
            if(UpdateDependentGraph_)
                { UpdateDependentGraph(); }
            if(WorkUnitsMain.size())
            {
                UpdateWorkUnitKeys(WorkUnitsMain);
                std::sort(WorkUnitsMain.begin(),WorkUnitsMain.end(),std::less<WorkUnitKey>() );
            }
        }

        void FrameScheduler::SortWorkUnitsAffinity(bool UpdateDependentGraph_)
        {
            if(UpdateDependentGraph_)
                { UpdateDependentGraph(); }
            if(WorkUnitsAffinity.size())
            {
                UpdateWorkUnitKeys(WorkUnitsAffinity);
                std::sort(WorkUnitsAffinity.begin(),WorkUnitsAffinity.end(),std::less<WorkUnitKey>() );
            }
        }

        void FrameScheduler::SortWorkUnitsAll(bool UpdateDependentGraph_)
        {
            SortWorkUnitsAffinity(UpdateDependentGraph_);
            SortWorkUnitsMain(false);
        }

        void FrameScheduler::RemoveWorkUnitMain(iWorkUnit* LessWork)
        {
            if(WorkUnitsMain.size())
            {
                IteratorMain RemovalTarget = WorkUnitsMain.end();
                for(IteratorMain Iter = WorkUnitsMain.begin(); Iter!=WorkUnitsMain.end(); Iter++)
                {
                    if(Iter->Unit == LessWork)
                    {
                        if(Iter+1 == WorkUnitsMain.end())   // once we find it, push it to the back linearly.
                            { RemovalTarget = Iter;}        // This way we can erase from the back where it is cheap
                        else                                // to do so and still make just on pass through the list
                            { std::swap (*Iter,*(Iter+1)); }
                    }
                    Iter->Unit->RemoveDependency(LessWork); // This has a ton of cache miss potential I am curious what it benchmarks like?!
                }
                if(RemovalTarget!=WorkUnitsMain.end())
                    { WorkUnitsMain.erase(RemovalTarget); }
            }
            if(WorkUnitsAffinity.size())
            {
                for(IteratorAffinity Iter = WorkUnitsAffinity.begin(); Iter!=WorkUnitsAffinity.end(); Iter++)
                    { Iter->Unit->RemoveDependency(LessWork); }
            }
        }

        void FrameScheduler::RemoveWorkUnitAffinity(iWorkUnit* LessWork)
        {
            if(WorkUnitsAffinity.size())
            {
                IteratorAffinity RemovalTarget = WorkUnitsMain.end();
                for(IteratorAffinity Iter = WorkUnitsAffinity.begin(); Iter!=WorkUnitsAffinity.end(); Iter++)
                {
                    if(Iter->Unit == LessWork)
                    {
                        if(Iter+1 == WorkUnitsAffinity.end())
                            { RemovalTarget = Iter;}
                        else
                            { std::swap (*Iter,*(Iter+1)); }
                    }
                    Iter->Unit->RemoveDependency(LessWork);
                }
                if(RemovalTarget!=WorkUnitsAffinity.end())
                    { WorkUnitsAffinity.erase(RemovalTarget); }
            }
            if(WorkUnitsMain.size())
            {
                for(IteratorMain Iter = WorkUnitsMain.begin(); Iter!=WorkUnitsMain.end(); Iter++)
                    { Iter->Unit->RemoveDependency(LessWork); }
            }
        }

        void FrameScheduler::RemoveWorkUnitMonopoly(MonopolyWorkUnit* LessWork)
        {
            if(WorkUnitsMain.size())
            {
                for(IteratorMain Iter = WorkUnitsMain.begin(); Iter!=WorkUnitsMain.end(); Iter++)
                    { Iter->Unit->RemoveDependency(LessWork); }
            }
            if(WorkUnitsAffinity.size())
            {
                for(IteratorAffinity Iter = WorkUnitsAffinity.begin(); Iter!=WorkUnitsAffinity.end(); Iter++)
                    { Iter->Unit->RemoveDependency(LessWork); }
            }
            if(WorkUnitsMonopolies.size())
            {
                for(IteratorMonoply Iter = WorkUnitsMonopolies.begin(); Iter!=WorkUnitsMonopolies.end(); Iter++)
                {
                    if(*Iter==LessWork)
                    {
                        WorkUnitsMonopolies.erase(Iter);
                        return;
                    }
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Algorithm essentials

        Whole FrameScheduler::GetDependentCountOf(iWorkUnit* Work, bool UsedCachedDepedentGraph)
        {
            if(UsedCachedDepedentGraph)
                { UpdateDependentGraph(); }
            Whole Results = DependentGraph[Work].size();
            for(std::set<iWorkUnit*>::iterator Iter=DependentGraph[Work].begin(); Iter!=DependentGraph[Work].end(); ++Iter)
            {
                Results+=GetDependentCountOf(*Iter);
            }
            return Results;
        }
        //DecacheMain(0),
        //DecacheAffinity(0),
        iWorkUnit* FrameScheduler::GetNextWorkUnit()
        {
            #ifdef MEZZ_USEATOMICSTODECACHECOMPLETEWORK
            bool CompleteSoFar = true;
            Int32 CurrentRun = DecacheMain;
            for(std::vector<WorkUnitKey>::reverse_iterator Iter = WorkUnitsMain.rbegin()+CurrentRun; Iter!=WorkUnitsMain.rend(); ++Iter)
            {
                if (NotStarted==Iter->Unit->GetRunningState())
                {
                    if(Iter->Unit->IsEveryDependencyComplete())
                        { return Iter->Unit; }
                    CompleteSoFar=false;
                }

                if(CompleteSoFar)
                {
                    CurrentRun++;
                    if(Complete==Iter->Unit->GetRunningState())
                        { AtomicCompareAndSwap32(&DecacheMain,DecacheMain,CurrentRun); }
                }
            }
            #else
            for(std::vector<WorkUnitKey>::reverse_iterator Iter = WorkUnitsMain.rbegin(); Iter!=WorkUnitsMain.rend(); ++Iter)
            {
                if (NotStarted==Iter->Unit->GetRunningState())
                {
                    if(Iter->Unit->IsEveryDependencyComplete())
                        { return Iter->Unit; }
                }
            }
            #endif

            return 0;
        }

        iWorkUnit* FrameScheduler::GetNextWorkUnitAffinity()
        {
            #ifdef MEZZ_USEATOMICSTODECACHECOMPLETEWORK
            bool CompleteSoFar = true;
            Int32 CurrentRun = DecacheAffinity;
            for(std::vector<WorkUnitKey>::reverse_iterator Iter = WorkUnitsAffinity.rbegin()+CurrentRun; Iter!=WorkUnitsAffinity.rend(); ++Iter)
            {
                if (NotStarted==Iter->Unit->GetRunningState())
                {
                    if(Iter->Unit->IsEveryDependencyComplete())
                        { return Iter->Unit; }
                    CompleteSoFar=false;
                }

                if(CompleteSoFar)
                {
                    CurrentRun++;
                    if(Complete==Iter->Unit->GetRunningState())
                        { AtomicCompareAndSwap32(&DecacheAffinity,DecacheAffinity,CurrentRun); }
                }
            }
            #else
            for(std::vector<WorkUnitKey>::reverse_iterator Iter = WorkUnitsAffinity.rbegin(); Iter!=WorkUnitsAffinity.rend(); ++Iter)
            {
                if (NotStarted==Iter->Unit->GetRunningState())
                {
                    if(Iter->Unit->IsEveryDependencyComplete())
                        { return Iter->Unit; }
                }
            }
            #endif

            return GetNextWorkUnit();
        }

        bool FrameScheduler::AreAllWorkUnitsComplete()
        {
            // start reading from units likely to be executed last.
            for(IteratorMain Iter = WorkUnitsMain.begin(); Iter!=WorkUnitsMain.end(); ++Iter)
            {
                if(Complete!=Iter->Unit->GetRunningState())
                    { return false; }
            }

            for(IteratorAffinity Iter = WorkUnitsAffinity.begin(); Iter!=WorkUnitsAffinity.end(); ++Iter)
            {
                if(Complete!=Iter->Unit->GetRunningState())
                    { return false; }
            }

            return true;
        }

        void FrameScheduler::UpdateDependentGraph()
        {
            DependentGraph.clear();
            UpdateDependentGraph(WorkUnitsMain);
            UpdateDependentGraph(WorkUnitsAffinity);
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Algorithm Configuration and Introspection

        Whole FrameScheduler::GetFrameCount() const
            { return FrameCount; }

        Whole FrameScheduler::GetFrameLength() const
            { return TargetFrameLength; }

        void FrameScheduler::SetFrameRate(const Whole& FrameRate)
        {
            if(FrameRate)
                { TargetFrameLength = 1000000/FrameRate; }
            else
                { TargetFrameLength = 0; }
        }

        void FrameScheduler::SetFrameLength(const Whole& FrameLength)
            { TargetFrameLength = FrameLength; }

        Whole FrameScheduler::GetThreadCount()
            { return CurrentThreadCount; }

        void FrameScheduler::SetThreadCount(const Whole& NewThreadCount)
            { CurrentThreadCount = NewThreadCount; }

        MaxInt FrameScheduler::GetCurrentFrameStart() const
            { return CurrentFrameStart; }

        DefaultRollingAverage<Whole>::Type& FrameScheduler::GetPauseTimeRollingAverage()
            { return this->PauseTimeLog; }

        Whole FrameScheduler::GetLastPauseTime() const
            { return this->PauseTimeLog[PauseTimeLog.RecordCapacity()-1]; }

        DefaultRollingAverage<Whole>::Type& FrameScheduler::GetFrameTimeRollingAverage()
            { return this->FrameTimeLog; }

        Whole FrameScheduler::GetLastFrameTime() const
            { return this->FrameTimeLog[FrameTimeLog.RecordCapacity()-1]; }

        ////////////////////////////////////////////////////////////////////////////////
        // Executing a Frame

        void FrameScheduler::DoOneFrame()
        {
            RunAllMonopolies();
            CreateThreads();
            RunMainThreadWork();
            JoinAllThreads();
            ResetAllWorkUnits();
            WaitUntilNextFrame();
        }

        void FrameScheduler::RunAllMonopolies()
        {
            for(IteratorMonoply Iter = WorkUnitsMonopolies.begin(); Iter!=WorkUnitsMonopolies.end(); ++Iter)
                { (*Iter)->operator()(*(Resources.at(0))); }
        }

        void FrameScheduler::CreateThreads()
        {
            LogResources.Lock(); //Unlocks in FrameScheduler::RunMainThreadWork() after last resource is swapped
            SwapBufferedResources();
        }

        void FrameScheduler::RunMainThreadWork()
        {
            LogDependencies();
            LogResources.Unlock();
            ThreadWorkAffinity(Resources[0]); // Do work in this thread and get the units with affinity

        }

        void FrameScheduler::JoinAllThreads()
        {
            for(std::vector<Thread*>::iterator Iter=Threads.begin(); Iter!=Threads.end(); ++Iter)
            {
                (*Iter)->join();
                delete *Iter;
            }
            Threads.clear();
            Threads.reserve(CurrentThreadCount);

            if(Sorter)
            {
                WorkUnitsAffinity = Sorter->WorkUnitsAffinity;
                WorkUnitsMain = Sorter->WorkUnitsMain;
                Sorter=0;
            }

            CurrentPauseStart=GetTimeStamp();
        }

        void FrameScheduler::ResetAllWorkUnits()
        {
            /// @todo could be replace with a parallel for, or a monopoly
            for(std::vector<WorkUnitKey>::reverse_iterator Iter = WorkUnitsMain.rbegin(); Iter!=WorkUnitsMain.rend(); ++Iter)
                { Iter->Unit->PrepareForNextFrame(); }
            for(std::vector<WorkUnitKey>::reverse_iterator Iter = WorkUnitsAffinity.rbegin(); Iter!=WorkUnitsAffinity.rend(); ++Iter)
                { Iter->Unit->PrepareForNextFrame(); }
            #ifdef MEZZ_USEATOMICSTODECACHECOMPLETEWORK
            DecacheMain=0;
            DecacheAffinity=0;
            #endif
        }

        void FrameScheduler::WaitUntilNextFrame()
        {
            FrameCount++;
            Whole TargetFrameEnd=0;
            if(TargetFrameLength)
            {
                TargetFrameEnd = TargetFrameLength + CurrentFrameStart;
                Whole WaitTime = Whole(TargetFrameEnd - GetTimeStamp()) + TimingCostAllowance;
                if(WaitTime>1000000) /// @todo Replace hard-code timeout with compiler/define/cmake_option
                    { WaitTime = 0; }
                Mezzanine::Threading::this_thread::sleep_for( WaitTime );
            }
            MaxInt Now = GetTimeStamp();
            FrameTimeLog.Insert(Now-CurrentFrameStart); //Track Frame Time for the past while
            PauseTimeLog.Insert(Now-CurrentPauseStart); //Track Pause Time for the past while
            (*LogDestination) << dec << "<FrameTimes Frame=\"" << (FrameCount-1) << "\" PauseTimeLog=\"" << (Now-CurrentPauseStart) << "\" FrameLength=\"" << (Now-CurrentFrameStart) << "\" />" << endl;
            CurrentFrameStart=Now;
            TimingCostAllowance -= (CurrentFrameStart-TargetFrameEnd);
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Basic container features

        Whole FrameScheduler::GetWorkUnitMonopolyCount() const
            { return this->WorkUnitsMonopolies.size(); }

        Whole FrameScheduler::GetWorkUnitAffinityCount() const
            { return this->WorkUnitsAffinity.size(); }

        Whole FrameScheduler::GetWorkUnitMainCount() const
            { return this->WorkUnitsMain.size(); }

        ////////////////////////////////////////////////////////////////////////////////
        // Other Utility Features

        FrameScheduler::Resource* FrameScheduler::GetThreadResource(ThreadId ID)
        {
            std::vector<Resource*>::iterator Results = Resources.begin();
            if(ID == MainThreadID)
                { return *Results; } // Main Thread Resources are stored in slot 0

            for(std::vector<Thread*>::iterator Iter=Threads.begin(); Iter!=Threads.end(); ++Iter)
            {
                Results++;
                if ( (*Iter)->get_id()==ID)
                    { return *Results; }
            }
            return NULL;
        }

        Logger* FrameScheduler::GetThreadUsableLogger(ThreadId ID)
        {
            Resource* AlmostResults = GetThreadResource(ID);
            if(AlmostResults)
                { return &AlmostResults->GetUsableLogger(); }
            return 0;
        }

        void FrameScheduler::DependenciesChanged(bool Changed)
            { this->NeedToLogDeps = Changed; }

        void FrameScheduler::LogDependencies()
        {
            if(this->NeedToLogDeps)
            {
                this->NeedToLogDeps = false;
                for(IteratorMain Iter = WorkUnitsMain.begin(); Iter!=WorkUnitsMain.end(); ++Iter)
                {
                    Whole MainCount = Iter->Unit->GetImmediateDependencyCount();
                    for(Whole Counter = 0; Counter<MainCount; Counter++)
                    {
                        *(this->LogDestination) << "<WorkUnitDependency "
                                                   "Unit=\"" << hex << Iter->Unit
                                                << "\" DependsOn=\"" << Iter->Unit->GetDependency(Counter) << "\" "
                                                   "/>" << endl;
                    }
                }

                for(IteratorAffinity Iter = WorkUnitsAffinity.begin(); Iter!=WorkUnitsAffinity.end(); ++Iter)
                {
                    Whole AffinityCount = Iter->Unit->GetImmediateDependencyCount();
                    for(Whole Counter = 0; Counter<AffinityCount; Counter++)
                    {
                        *(this->LogDestination) << "<WorkUnitDependency "
                                                   "Unit=\"" << hex << Iter->Unit
                                                << "\" DependsOn=\"" << Iter->Unit->GetDependency(Counter) << "\" "
                                                   "/>" << endl;
                    }
                }

                for(IteratorMonoply Iter = WorkUnitsMonopolies.begin(); Iter!=WorkUnitsMonopolies.end(); ++Iter)
                {
                    Whole MonopolyCount = (*Iter)->GetImmediateDependencyCount();
                    for(Whole Counter = 0; Counter<MonopolyCount; Counter++)
                    {
                        *(this->LogDestination) << "<WorkUnitDependency "
                                                   "Unit=\"" << hex << (*Iter)
                                                << "\" DependsOn=\"" << (*Iter)->GetDependency(Counter) << "\" "
                                                   "/>" << endl;
                    }
                }
            }
        }

        std::ostream& FrameScheduler::GetLog()
            { return *LogDestination; }

        void FrameScheduler::ChangeLogTarget(std::ostream* LogTarget)
        {
            RemoveLog();
            InstallLog(LogTarget);
        }

        void FrameScheduler::InstallLog(std::ostream* LogTarget)
        {
            LogDestination = LogTarget;
            (*LogDestination)<< "<MezzanineLog>" << std::endl;
            LogDestination->flush();
        }

        void FrameScheduler::RemoveLog()
        {
            (*LogDestination) << "</MezzanineLog>" << std::endl;
            LogDestination->flush();

            if(LoggingToAnOwnedFileStream)
            {
                ((std::fstream*)LogDestination)->close();
                delete LogDestination;
            }
        }

        LogAggregator* FrameScheduler::GetLogAggregator()
        {
            LogAggregator* Results = NULL;
            for(IteratorMain Iter = WorkUnitsMain.begin(); Iter!=WorkUnitsMain.end(); ++Iter)
            {
                Results = dynamic_cast<LogAggregator*>(Iter->Unit);
                if(Results)
                    { return Results; }
            }

            for(IteratorAffinity Iter = WorkUnitsAffinity.begin(); Iter!=WorkUnitsAffinity.end(); ++Iter)
            {
                Results = dynamic_cast<LogAggregator*>(Iter->Unit);
                if(Results)
                    { return Results; }
            }

            for(IteratorMonoply Iter = WorkUnitsMonopolies.begin(); Iter!=WorkUnitsMonopolies.end(); ++Iter)
            {
                Results = dynamic_cast<LogAggregator*>(*Iter);
                if(Results)
                    { return Results; }
            }
            return NULL;

        }

        Boole FrameScheduler::ForceLogFlush()
        {
            LogAggregator* Pointer = GetLogAggregator();
            if(Pointer)
            {
                DefaultThreadSpecificStorage::Type Storage(this);
                Pointer->NextFlushForced();
                Pointer->DoWork(Storage);
                this->SwapBufferedResources();
                Pointer->NextFlushForced();
                Pointer->DoWork(Storage);
                GetLog().flush();
                return true;
            }
            return false;
        }

        void FrameScheduler::SwapBufferedResources()
        {
            Resources[0]->SwapAllBufferedResources();
            for(Whole Count = 1; Count<CurrentThreadCount; ++Count)
            {
                if(Count+1>Resources.size())
                    { Resources.push_back(new DefaultThreadSpecificStorage::Type(this)); }
                Resources[Count]->SwapAllBufferedResources();
                Threads.push_back(new Thread(ThreadWork, Resources[Count]));
            }
        }

    } // \FrameScheduler
}// \Mezanine


#endif
