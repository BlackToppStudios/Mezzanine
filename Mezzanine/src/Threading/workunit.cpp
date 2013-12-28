// The DAGFrameScheduler is a Multi-Threaded lock free and wait free scheduling library.
// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _workunit_cpp
#define _workunit_cpp

/// @file
/// @brief This file has the implementation of the @ref Mezzanine::Threading::DefaultWorkUnit.

#include "workunit.h"
#include "systemcalls.h"
#include "atomicoperations.h"
#include "doublebufferedresource.h"

#ifdef MEZZ_DEBUG
#include <cassert>
#endif

namespace Mezzanine
{
    namespace Threading
    {
        /////////////////////////////////////////////////////////////////////////////////////////////
        // The Simple Stuff

        DefaultWorkUnit::DefaultWorkUnit(const DefaultWorkUnit&)
            {}

        DefaultWorkUnit& DefaultWorkUnit::operator=(DefaultWorkUnit& Unused)
            { return Unused; }

        DefaultWorkUnit::DefaultWorkUnit() : CurrentRunningState(NotStarted)
            {}

        DefaultWorkUnit::~DefaultWorkUnit()
            {}

        /////////////////////////////////////////////////////////////////////////////////////////////
        // Work with the dependents as in what must not start until this finishes.

        Whole DefaultWorkUnit::GetDependentCount(FrameScheduler &SchedulerToCount)
            { return SchedulerToCount.GetDependentCountOf(this); }

        /////////////////////////////////////////////////////////////////////////////////////////////
        // Work with the dependencies as in what must finish before we can run this work unit.

        iWorkUnit* DefaultWorkUnit::GetDependency(Whole Index) const
            { return Dependencies.at(Index); }

        Whole DefaultWorkUnit::GetImmediateDependencyCount() const
            { return Dependencies.size(); }

        Whole DefaultWorkUnit::GetDependencyCount() const
        {
            Whole Results = Dependencies.size();
            for(std::vector<iWorkUnit*>::const_iterator Iter=Dependencies.begin(); Iter!=Dependencies.end(); ++Iter)
                { Results += (*Iter)->GetDependencyCount(); }
            return Results;

        }

        void DefaultWorkUnit::AddDependency(iWorkUnit* NewDependency)
            { Dependencies.push_back(NewDependency); }

        void DefaultWorkUnit::RemoveDependency(iWorkUnit* RemoveDependency)
        {
            Dependencies.erase(
                        std::remove(Dependencies.begin(),Dependencies.end(),RemoveDependency),
                        Dependencies.end()
                    );
        }

        void DefaultWorkUnit::ClearDependencies()
            { Dependencies.clear(); }

        bool DefaultWorkUnit::IsEveryDependencyComplete()
        {
            for (std::vector<iWorkUnit*>::iterator Iter = Dependencies.begin(); Iter!=Dependencies.end(); ++Iter)
            {
                if( Complete != (*Iter)->GetRunningState() )
                    { return false; }
            }
            return true;
        }

        /////////////////////////////////////////////////////////////////////////////////////////////
        // Work with the ownership and RunningState
        RunningState DefaultWorkUnit::TakeOwnerShip()
        {
            if(!IsEveryDependencyComplete())
                { return NotStarted; }

            if(NotStarted ==  AtomicCompareAndSwap32(&CurrentRunningState, NotStarted, Running) )
                { return Starting; } // This is the only place a starting should be generated, and it is never placed in CurrentRunningState

            return NotStarted;
        }

        RunningState DefaultWorkUnit::GetRunningState() const
            { return (RunningState)CurrentRunningState; } // This only works because we set all of in RunningState to be unsigned.

        void DefaultWorkUnit::PrepareForNextFrame()
            //{ while(CurrentRunningState!=AtomicCompareAndSwap(&CurrentRunningState,CurrentRunningState,NotStarted)); }
            { CurrentRunningState=NotStarted; }

        /////////////////////////////////////////////////////////////////////////////////////////////
        // Work with the performance log
        Whole DefaultWorkUnit::GetPerformance() const
            { return PerformanceLog.GetAverage(); }

        RollingAverage<Whole>& DefaultWorkUnit::GetPerformanceLog()
            { return PerformanceLog; }

        /////////////////////////////////////////////////////////////////////////////////////////////
        // Deciding when to and doing the work

        void DefaultWorkUnit::operator() (DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
        {
            MaxInt Begin = Mezzanine::GetTimeStamp();

            std::ostream& Out = CurrentThreadStorage.GetUsableLogger();
            Out << "<WorkUnit id=\"" << std::hex << this << std::dec << "\">" << std::endl;
            #ifdef MEZZ_DEBUG
            Out << "<WorkUnitStart BeginTimeStamp=\"" << Begin << "\" ThreadID=\"" << Mezzanine::Threading::this_thread::get_id() << "\" />" << std::endl;
            #endif

            this->DoWork(CurrentThreadStorage);
            MaxInt End = Mezzanine::GetTimeStamp();
            this->GetPerformanceLog().Insert( Whole(End-Begin)); // A whole is usually a 32 bit type, which is fine unless a single workunit runs for 35 minutes.
            CurrentRunningState = Complete;

            #ifdef MEZZ_DEBUG
            Out << "<WorkUnitEnd EndTimeStamp=\"" << End << "\" Duration=\"" << (End-Begin) << "\" DurationStored=\"" << Whole(End-Begin) << "\" />" << std::endl;
            #endif
            Out << "</WorkUnit>" << std::endl;
        }

        WorkUnitKey DefaultWorkUnit::GetSortingKey(FrameScheduler& SchedulerToCount)
            { return WorkUnitKey( this->GetDependentCount(SchedulerToCount), GetPerformanceLog().GetAverage(), this); }

    }//Threading
}//Mezzanine

#endif
