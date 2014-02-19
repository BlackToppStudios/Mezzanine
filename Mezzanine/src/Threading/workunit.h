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
#ifndef _workunit_h
#define _workunit_h

#include "datatypes.h"

#if !defined(SWIG) || defined(SWIG_THREADING) // Do not read when in swig and not in the threading module
#include "framescheduler.h"
#include "mutex.h"
#include "rollingaverage.h"
#include "threadingenumerations.h"
#include "thread.h"
#include "workunitkey.h"
#endif

/// @file
/// @brief This file has the definition of the workunit.

namespace Mezzanine
{
    namespace Threading
    {
        /// @brief Interface of a WorkUnit. This represents on piece of work through time.
        /// @details This has no real substance, use the DefaultWorkUnit class is you
        /// want to inherit from something ready for easy integration. Implement this
        /// if you want to make heavy changes to the algorithm or maximize performance.
        class MEZZ_LIB iWorkUnit
        {
            public:
                /////////////////////////////////////////////////////////////////////////////////////////////
                // Work with the dependents as in what WorkUnits must not start until this finishes.

                /// @brief This returns the count workunits that depend on this work unit.
                /// @param SchedulerToCount The @ref FrameScheduler has a cache of this data, it is impossible to calculate without the complete list of WorkUnits on it either.
                /// @details Because Dependents are not tracked this iterates over entry in the FrameScheduler it is passed.
                /// @return A Whole is returned containing the count.
                virtual Whole GetDependentCount(FrameScheduler &SchedulerToCount) = 0;


                /////////////////////////////////////////////////////////////////////////////////////////////
                // Work with the dependencies as in what must finish before we can run this work unit.

                /// @brief This is used to iterate of all the dependencies.
                /// @param Index The 0 based index of the dependency you are trying to retrieve. No ordering is guaranteed, just that counting from 0 to @ref GetImmediateDependencyCount() with access each iWorkUnit once.
                /// @return A pointer to an iWorkUnit.
                virtual iWorkUnit* GetDependency(Whole Index) const = 0;

                /// @brief Get the amount of dependencies that directly count on this for access purposes.
                /// @return The amount of accessable WorkUnit from this WorkUnits dependency list.
                virtual Whole GetImmediateDependencyCount() const = 0;

                /// @brief How many other WorkUnits does this one depend on?
                /// @details This is what the @ref FrameScheduler uses to calculate the
                /// sorting order of WorkUnits. In the default implmentation it counts
                /// some units twice if they depend on this WorkUnit through Multiple
                /// dependency chains.
                /// @return A Whole containing the answer.
                virtual Whole GetDependencyCount() const = 0;

                /// @brief Force this WorkUnit to Start after another has completed.
                /// @param NewDependency The WorkUnit that must start after this one has completed.
                /// @warning Changing this outside the schedule, once the scheduler has started can cause undefined behavior.
                virtual void AddDependency(iWorkUnit* NewDependency) = 0;

                /// @brief Remove a dependency.
                /// @param RemoveDependency A pointer to the WorkUnit to remove as a dependency.
                virtual void RemoveDependency(iWorkUnit* RemoveDependency) = 0;

                /// @brief Drop any information about what work units this one depends on.
                virtual void ClearDependencies() = 0;

                /// @brief Check if this WorkUnit could concievably run right now.
                /// @return This returns true if all of this WorkUnits dependencies have completed execution and false otherwise.
                virtual bool IsEveryDependencyComplete() = 0;


                /////////////////////////////////////////////////////////////////////////////////////////////
                // Work with the ownership and RunningState

                /// @brief Attempts to atomically start the work unit in the current thread.
                /// @return Returns RunningState::Starting if this thread was able to gain ownership and start the workunit, returns RunningState::NotStarted otherwise.
                virtual RunningState TakeOwnerShip() = 0;

                /// @brief Retrieves the current RunningState of the thread.
                /// @return A RunningState that indicates if the thread is running, started, etc... This information can be changed at any time and should be considered stale immediately after retrieval.
                virtual RunningState GetRunningState() const = 0;

                /// @brief This resets the running state and takes any further action required to use the WorkUnit again.
                virtual void PrepareForNextFrame() = 0;


                /////////////////////////////////////////////////////////////////////////////////////////////
                // Work with the preformance log

                /// @brief Get the internal rolling average for querying.
                /// @return A whole that represents this WorkUnits Performance.
                virtual Whole GetPerformance() const = 0;


                /////////////////////////////////////////////////////////////////////////////////////////////
                // Deciding when to and doing the work

                /// @brief This tracks work unit metadata, then calls DoWork.
                /// @param CurrentThreadStorage The @ref ThreadSpecificStorage that this WorkUnit will operate with when executing.
                virtual void operator() (DefaultThreadSpecificStorage::Type& CurrentThreadStorage) = 0;

                /// @brief Get the sorting metadata.
                /// @param SchedulerToCount This uses the metadata on the @ref FrameScheduler to generate the Dependency listing required.
                /// @return A WorkUnitKey suitable for sorting this workunit.
                virtual WorkUnitKey GetSortingKey(FrameScheduler &SchedulerToCount) = 0;

                /// @brief WorkUnits Must implement these to do the work.
                virtual void DoWork(DefaultThreadSpecificStorage::Type& CurrentThreadStorage) = 0;

                /// @brief Virtual destructor.
                virtual ~iWorkUnit(){}
        };//iWorkUnit


        /// @brief Default implementation of WorkUnit. This represents on piece of work through time.
        class MEZZ_LIB DefaultWorkUnit : public iWorkUnit
        {
                /////////////////////////////////////////////////////////////////////////////////////////////
                // Data Members
            protected:
                /// @brief A rolling average of execution times.
                DefaultRollingAverage<Whole>::Type PerformanceLog;

                /// @brief A collection of of workunits that must be complete before this one can start.
                std::vector<iWorkUnit*> Dependencies;

                /// @brief This controls do work with this after it has.
                Int32 CurrentRunningState;

                /////////////////////////////////////////////////////////////////////////////////////////////
                // The Simple Stuff
            private:
                /// @brief Remove Copy constructor, Copying a work unit does not make sense, it breaks scheduling, made private.
                DefaultWorkUnit(const DefaultWorkUnit&);

                /// @brief Assignment operator, Assignment on a WorkUnit Does Not make sense, it breaks scheduling, made private.
                DefaultWorkUnit& operator=(DefaultWorkUnit& Unused);

            public:
#ifndef SWIG
                /// @brief Simple constructor.
                DefaultWorkUnit();
#endif
                /// @brief Virtual destructor, doesn't actually do much.
                virtual ~DefaultWorkUnit();

                /////////////////////////////////////////////////////////////////////////////////////////////
                // Work with the dependents as in what must not start until this finishes.
            public:
                virtual Whole GetDependentCount(FrameScheduler &SchedulerToCount);

                /////////////////////////////////////////////////////////////////////////////////////////////
                // Work with the dependents as in what WorkUnits must not start until this finishes.
            public:
                virtual iWorkUnit* GetDependency(Whole Index) const;

                virtual Whole GetImmediateDependencyCount() const;

                virtual Whole GetDependencyCount() const;

            public:
                virtual void AddDependency(iWorkUnit* NewDependency);

                virtual void RemoveDependency(iWorkUnit* RemoveDependency);

                virtual void ClearDependencies();

                virtual bool IsEveryDependencyComplete();

                /////////////////////////////////////////////////////////////////////////////////////////////
                // Work with the ownership and RunningState
            public:
                virtual RunningState TakeOwnerShip();

                virtual RunningState GetRunningState() const;

                virtual void PrepareForNextFrame();

                /////////////////////////////////////////////////////////////////////////////////////////////
                // Work with the performance log
            public:
                virtual Whole GetPerformance() const;

                /// @brief Get the internal rolling average for querying.
                /// @return A const reference to the internal Rolling Average.
                virtual RollingAverage<Whole>& GetPerformanceLog();

                /////////////////////////////////////////////////////////////////////////////////////////////
                // Deciding when to and doing the work
            public:
                /// @brief This does everything required to track metadata and log work
                /// @param CurrentThreadStorage The FrameScheduler passes this in
                /// @details This wraps the log output in a WorkUnit element with a unique ID
                /// as and an attribute. This causes all the log output to be valid xml as long
                /// no '<' or '>' are emitted.
                /// @n @n
                ///
                virtual void operator() (DefaultThreadSpecificStorage::Type& CurrentThreadStorage);

                virtual WorkUnitKey GetSortingKey(FrameScheduler &SchedulerToCount);

        };//DefaultWorkUnit
    }//Threading
}//Mezzanine

#endif
