// The DAGFrameScheduler is a Multi-Threaded lock free and wait free scheduling library.
// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _framescheduler_h
#define _framescheduler_h

#include "datatypes.h"
#include "doublebufferedresource.h"
#include "thread.h"
#include "workunitkey.h"
#include "systemcalls.h"
#ifdef MEZZ_USEBARRIERSEACHFRAME
    #include "barrier.h"
#endif


#include <map>
#include <set>
#include <fstream>

/// @file
/// @brief This file has the Declarations for the main FrameScheduler class.

namespace Mezzanine
{
    namespace Threading
    {
        class MonopolyWorkUnit;
        class iWorkUnit;
        class LogAggregator;
        class LogBufferSwapper;
        class FrameScheduler;
        class WorkSorter;

        /// @brief This is central object in this algorithm, it is responsible for spawning threads and managing the order that work units are executed.
        /// @details For a detailed description of the @ref algorithm_sec "Algorithm" this implements see the @ref algorithm_sec "Algorithm" section on
        /// the Main page.
        class MEZZ_LIB FrameScheduler
        {
            friend class LogAggregator;
            friend class LogBufferSwapper;
            friend class WorkSorter;

            protected:

				////////////////////////////////////////////////////////////////////////////////
				// Data Members

                /// @brief A collection of all the work units that are not Monopolies and do not have affinity for a given thread.
                /// @details This stores a sorted listing(currently a vector) of @ref Mezzanine::Threading::WorkUnitKey "WorkUnitKey" instances.
                /// These include just the metadata required for sorting @ref Mezzanine::Threading::iWorkUnit "iWorkUnit"s. Higher priority
                /// @ref Mezzanine::Threading::iWorkUnit "iWorkUnit"s are higher/later in the collection. This list is sorted by calls
                /// to @ref Mezzanine::Threading::FrameScheduler::SortWorkUnitsMain "SortWorkUnitsMain" or @ref Mezzanine::Threading::FrameScheduler::SortWorkUnitsAll "SortWorkUnitsAll".
                std::vector<WorkUnitKey> WorkUnitsMain;

                /// @brief An iterator suitable for iterating over the main pool of work units.
                typedef std::vector<WorkUnitKey>::iterator IteratorMain;

                /// @brief A const iterator suitable for iterating over the main pool of work units.
                typedef std::vector<WorkUnitKey>::const_iterator ConstIteratorMain;

                /// @brief A collection of @ref Mezzanine::Threading::iWorkUnit "iWorkUnit"s that must be run on the main thread.
                /// @details This is very similar to @ref WorkUnitsMain except that the @ref Mezzanine::Threading::iWorkUnit "iWorkUnit"s
                /// are only run in the main thread and are sorted by calls to @ref SortWorkUnitsAll or @ref SortWorkUnitsAffinity .
                std::vector<WorkUnitKey> WorkUnitsAffinity;

                /// @brief An iterator suitable for iterating over the main pool of work units.
                typedef std::vector<WorkUnitKey>::iterator IteratorAffinity;

                /// @brief A const iterator suitable for iterating over the main pool of work units.
                typedef std::vector<WorkUnitKey>::const_iterator ConstIteratorAffinity;

                /// @brief A structure designed to minimalistically represent Dependency and Reverse Dependency Graphs in work units.
                typedef std::map<
                                iWorkUnit*,
                                std::set<iWorkUnit*>
                            > DependentGraphType;

                /// @brief This structure allows reverse lookup of dependencies.
                /// @details This is is a key part of the workunit sorting algorithm. This is calculated during calls to generate
                /// @ref Mezzanine::Threading::WorkUnitKey "WorkUnitKey"s,
                /// @warning
                /// @todo write this warning, it is important, but not easy to lay out.
                DependentGraphType DependentGraph;

                /// @brief This maintains ownership of all the thread specific resources.
                /// @note There should be the same amount or more of these than entries in the Threads vector.
                std::vector<DefaultThreadSpecificStorage::Type*> Resources;

                /// @brief A way to track an arbitrary number of threads.
                /// @note There should never be more of these than Resources, and if there are more at the beginning of a frame the resources will be created in CreateThreads().
                std::vector<Thread*> Threads;

                /// @brief A collection of all the monopolies this scheduler must run and keep ownership of.
                std::vector<MonopolyWorkUnit*> WorkUnitMonopolies;

                /// @brief When the logs are aggregated, this is where they are sent
                std::ostream* LogDestination;

                /// @brief What time did the current Frame Start at.
                MaxInt CurrentFrameStart;

                /// @brief If this pointer is non-zero then the @ref WorkSorter it points at will be used to sort WorkUnits.
                WorkSorter* Sorter;

                #ifdef MEZZ_USEBARRIERSEACHFRAME
            public:
                /// @brief Used to synchronize the starting an stopping of all threads before the frame starts.
                Barrier StartFrameSync;

                /// @brief Used to synchronize the starting and stopping of all threads after work is done before the frame ends.
                Barrier EndFrameSync;

                /// @brief When using barriers instead of thread creation for synchronization this is what tells the threads to end.
                Int32 LastFrame;
            protected:
                #endif

                #ifdef MEZZ_USEATOMICSTODECACHECOMPLETEWORK
                /// @brief Indicates the beginning of work that must be searched when starting a fresh search for work in WorkUnitsMain.
                Int32 DecacheMain;

                /// @brief Indicates the beginning of work that must be searched when starting a fresh search for work in WorkUnitsAffinity.
                Int32 DecacheAffinity;
                #endif

                /// @brief How many threads will this try to execute with in the next frame.
                Whole CurrentThreadCount;

                /// @brief Used to store a count of frames from the begining of game execution.
                /// @warning At 60 Frames per second this loops in 2 years, 3 months, 6 days and around 18 hours, this may not be suitable for high uptime servers. Using a MaxInt fixes this.
                Whole FrameCount;

                /// @brief The Maximum frame rate this algorithm should run at.
                Whole TargetFrameLength;

                /// @brief To prevent frame time drift this many microseconds is subtracted from the wait period to allow time for calculations.
                Integer TimingCostAllowance;

                /// @brief Set based on which constructor is called, and only used during destruction.
                bool LoggingToAnOwnedFileStream;

				////////////////////////////////////////////////////////////////////////////////
				// Protected Methods

                /// @brief Get the endpoint for the logs.
                /// @return An std:ostream reference which can be streamed to commit log entries.
                std::ostream& GetLog();

                /// @brief Used in destruction to tear down threads.
                void CleanUpThreads();

                /// @brief Simply iterates over and deletes everything in Threads.
                void DeleteThreads();

                /// @brief Adds the dependencies of the @ref Mezzanine::Threading::iWorkUnit "iWorkUnit"s in the passed containter to the the internal reverse dependency graph.
                /// @param Units The container to examine for dependency relationships.
                void UpdateDependentGraph(const std::vector<WorkUnitKey> &Units);

                /// @brief Iterate over the passed container of @ref WorkUnitKey "WorkUnitKey"s and refresh them with the correct data from their respective @ref Mezzanine::Threading::iWorkUnit "iWorkUnit"s
                /// @param Units The container to examine for @ref WorkUnitKey "WorkUnitKey" metadata.
                void UpdateWorkUnitKeys(std::vector<WorkUnitKey> &Units);

            public:

				////////////////////////////////////////////////////////////////////////////////
				// Construction and Destruction
			
                /// @brief Create a Framescheduler that owns a filestream for logging.
                /// @param _LogDestination An fstream that will be closed and deleted when this framescheduler is destroyed. Defaults to a new Filestream Logging to 'Log.txt'.
                /// @param StartingThreadCount How many threads. Defaults to the value returned by @ref Mezzanine::GetCPUCount "GetCPUCount()".
                /// @warning This must be constructed from the Main(only) thread for any features with thread affinity to work correctly.
                FrameScheduler(
                        std::fstream* _LogDestination = new std::fstream("Log.txt", std::ios::out | std::ios::app),
                        Whole StartingThreadCount = GetCPUCount()
                    );

                /// @brief Create a Framescheduler, that logs to an unowned stream.
                /// @param _LogDestination Any stream, other than an fstream, and it will be closed (not deleted) when this frame scheduler is destroyed.
                /// @param StartingThreadCount How many threads. Defaults to the value returned by @ref Mezzanine::GetCPUCount "GetCPUCount()".
                /// @warning This must be constructed from the Main(only) thread for any features with thread affinity to work correctly.
                FrameScheduler(
                        std::ostream* _LogDestination,
                        Whole StartingThreadCount = GetCPUCount()
                    );

                /// @brief Destructor
                /// @details Deletes all std::fstream, WorkUnit, MonopolyWorkUnit and ThreadSpecificStorage objects that this was passed or created during its lifetime.
                virtual ~FrameScheduler();

				////////////////////////////////////////////////////////////////////////////////
				// WorkUnit management
			
                /// @brief Add a normal @ref iWorkUnit to this For fcheduling.
                /// @param MoreWork A pointer the the WorkUnit, that the FrameScheduler will take ownership of, and schedule for work.
                virtual void AddWorkUnitMain(iWorkUnit* MoreWork);

                /// @brief Add a normal @ref iWorkUnit to this For scheduling.
                /// @param MoreWork A pointer the the WorkUnit, that the FrameScheduler will take ownership of, and schedule for work.
                virtual void AddWorkUnitAffinity(iWorkUnit* MoreWork);

                /// @brief Add a @ref MonopolyWorkUnit for execution at the beginning of the frame.
                /// @param MoreWork A pointer to the @ref MonopolyWorkUnit to add.
                virtual void AddWorkUnitMonopoly(MonopolyWorkUnit* MoreWork);

                /// @brief Sort the the main pool of WorkUnits to allow them to be used more efficiently in the next frame executed.
                /// @param UpdateDependentGraph_ Should the internal cache of reverse dependents be updated.
                /// @details See @ref Mezzanine::Threading::FrameScheduler::DependentGraph "DependentGraph"
                /// for the appropriate times to use this.
                virtual void SortWorkUnitsMain(bool UpdateDependentGraph_ = true);

                /// @brief Sort the WorkUnits that must run on the main thread to allow them to be used more efficiently in the next frame executed.
                /// @param UpdateDependentGraph_ Should the internal cache of reverse dependents be updated.
                /// @details See @ref Mezzanine::Threading::FrameScheduler::DependentGraph "DependentGraph"
                /// for the appropriate times to use this.
                virtual void SortWorkUnitsAffinity(bool UpdateDependentGraph_ = true);

                /// @brief Sort all the WorkUnits that must run on the main thread to allow them to be used more efficiently in the next frame executed.
                /// @param UpdateDependentGraph_ Should the internal cache of reverse dependents be updated.
                /// @details See @ref Mezzanine::Threading::FrameScheduler::DependentGraph "DependentGraph"
                /// for the appropriate times to use this.
                virtual void SortWorkUnitsAll(bool UpdateDependentGraph_ = true);

                // @brief Remove a WorkUnit regardless of type, and caller regains ownership of it.
                // @param LessWork A pointer to a WorkUnit that should no longer be scheduled.
                // @details This is relative slow compared to adding or finding a working unit, this works in linear time relative to the number of WorkUnits in the scheduler.
                //virtual void RemoveWorkUnit(iWorkUnit* LessWork);

                /// @brief Remove a WorkUnit from the main pool of WorkUnits (and not from the groups of Affinity or MonpolyWorkUnits).
                /// @param LessWork A pointer to the workunit the calling coding will reclaim ownership of and will no longer be scheduled, and have its dependencies removed.
                virtual void RemoveWorkUnitMain(iWorkUnit* LessWork);

                /// @brief Remove a WorkUnit from the Affinity pool of WorkUnits (and not from the Main group or MonpolyWorkUnits).
                /// @param LessWork A pointer to the workunit the calling coding will reclaim ownership of and will no longer be scheduled, and have its dependencies removed.
                virtual void RemoveWorkUnitAffinity(iWorkUnit* LessWork);

                /// @brief Remove a WorkUnit from the Monopoly pool of WorkUnits (and not from the Main or Affinity group).
                /// @param LessWork A pointer to the MonopolyWorkUnit the calling coding will reclaim ownership of and will no longer be scheduled, and have its dependencies removed.
                virtual void RemoveWorkUnitMonopoly(MonopolyWorkUnit* LessWork);

				////////////////////////////////////////////////////////////////////////////////
				// Algorithm essentials
			
                /// @brief How many other WorkUnit instances must wait on this one.
                /// @param Work The WorkUnit to get the updated count of.
                /// @param UsedCachedDepedentGraph If the cache is already up to date leaving this false, and not updating it can save significant time.
                /// @return A Whole Number representing the amount of WorkUnit instances that cannot start until this finishes.
                virtual Whole GetDependentCountOf(iWorkUnit *Work, bool UsedCachedDepedentGraph=false);

                /// @brief Gets the next available workunit for execution.
                /// @details This finds the next available WorkUnit which has not started execution, has no dependencies that have
                /// not complete, has the most WorkUnits that depend on it (has the highest runtime in the case of a tie).
                /// @return A pointer to the WorkUnit that could be executed or a null pointer if that could not be acquired. This does not give ownership of that WorkUnit.
                virtual iWorkUnit* GetNextWorkUnit();

                /// @brief Just like @ref GetNextWorkUnit except that it also searches through and prioritizes work units with affinity too.
                /// @return A pointer to the WorkUnit that could be executed *in the main thread* or a null pointer if that could not be acquired. This does not give ownership of that WorkUnit.
                virtual iWorkUnit* GetNextWorkUnitAffinity();

                /// @brief Is the work of the frame done?
                /// @return This returns true if all the WorkUnit instances are complete, and false otherwise.
                virtual bool AreAllWorkUnitsComplete();

                /// @brief Create a reverse depedent graph that can be used for sorting @ref iWorkUnit "iWorkUnit"s to optimize execution each frame.
                /// @details This can be called automatically from any of several places that make sense by passing a boolean true value.
                /// These place include create a @ref WorkUnitKey or Sorting the work units in a framescheduler.
                virtual void UpdateDependentGraph();

				////////////////////////////////////////////////////////////////////////////////
				// Algorithm Configuration and Introspection

                /// @brief Get the current number of frames that have elapsed
                /// @return A Whole containing the frame count.
                virtual Whole GetFrameCount() const;

                /// @brief Get the desired length of a frame.
                /// @return The desired frame length as a Whole in Microseconds
                virtual Whole GetFrameLength() const;

                /// @brief Set the desired Frate rate.
                /// @param FrameRate in frames per second
                /// @details Defaults to 60, to maximize smoothmess of execution (no human can see that fast),
                /// while not killing battery life. This is a maximum framerate, if WorkUnits take too long
                /// to execute this will not make them finish faster. This controls a delay to prevent the
                /// machine's resources from being completely tapped. @n @n Set this to 0 to never pause
                /// and run as fast as possible.
                /// @return A Whole containing the Target frame rate.
                virtual void SetFrameRate(const Whole& FrameRate);

                /// @brief Set the Desired length of a frame in microseconds.
                /// @param FrameLength The desired minimum length of the frame. Use 0 for no pause.
                virtual void SetFrameLength(const Whole& FrameLength);

                /// @brief Get the amount of threads that will be used to execute WorkUnits a the start of the next frame.
                /// @return A Whole with the current desired thread count.
                virtual Whole GetThreadCount();

                /// @brief Set the amount of thread to use.
                /// @param NewThreadCount The amount of threads to use starting at the begining of the next frame.
                /// @note Currently the thread count cannot be reduced if Mezz_MinimizeThreadsEachFrame is selected in cmake configuration.
                virtual void SetThreadCount(const Whole& NewThreadCount);

                /// @brief When did this frame start?
                /// @return A MaxInt with the timestamp corresponding to when this frame started.
                virtual MaxInt GetCurrentFrameStart() const;

				////////////////////////////////////////////////////////////////////////////////
				// Executing a Frame

                /// @brief Do one frame worth of work.
                /// @details This just calls the following functions in the order presented:
                /// @code
                /// void FrameScheduler::DoOneFrame()
                /// {
                ///   RunAllMonopolies();
                ///   CreateThreads();
                ///   RunMainThreadWork();
                ///   JoinAllThreads();
                ///   ResetAllWorkUnits();
                ///   WaitUntilNextFrame();
                /// }
                /// @endcode
                /// This can be replaced calling these functions in this order. You can add any other calls you like between the various stages. This can be done to
                /// allow maximum integration with existing projects. It can also be used to prevent a giant migration and replace it with a piecemeal upgrade.
                /// @warning Do not call this on an unsorted set of WorkUnits. Use @ref FrameScheduler::SortWorkUnitsAll() and the @ref DependentGraph to sort WorkUnits after
                /// they are inserted into the frame scheduler for the first time. This doesn't need to happen each frame, just after any new work units are added or removed
                /// (except Monopolies).
                virtual void DoOneFrame();

                /// @brief This is the 1st step (of 6) in a frame.
                /// @details This iterates over the listing of @ref MonopolyWorkUnit "MonopolyWorkUnit"s and executes each one in the order
                /// it was added. This should be considered as consuming all available CPU time until it returns. This call blocks until execution
                /// of monopolies is complete.
                virtual void RunAllMonopolies();

                /// @brief This is the 2nd step (of 6) in a frame.
                /// @details This starts all the threads on their work. Until @ref JoinAllThreads() is called some thread may still be working.
                /// This thread starts the man @ref algorithm_sec "scheduling algorithm" working on every thread except the calling thread. This
                /// call does not block and tends to return very quickly.
                /// @n @n
                /// This checks the amount of threads as set by @ref SetFrameLength "SetFrameLength". It creates any
                /// @ref ThreadSpecificStorage instances required and creates threads if they are required. If the build option
                /// @ref MEZZ_USEBARRIERSEACHFRAME Mezz_MinimizeThreadsEachFrame was enabled then this will reuse threads from previous frames,
                /// otherwise this will re-use thread specific resources and create a new set of threads. Re-use of threads is synchronized
                /// with the @ref Barrier StartFrameSync member variable. It is unclear, and likely platform specific, which option has
                /// better performance characteristics.
                /// @warning While this is running any changes to the @ref FrameScheduler be made with an atomic operation like the
                /// @ref AtomicCompareAndSwap32 "AtomicCompareAndSwap32" or @ref AtomicAdd "AtomicAdd". Any other threads
                /// @ref ThreadSpecificStorage or workunit may be accessed as long as they are protected by some kind of synchronization
                /// mechanism.
                virtual void CreateThreads();

                /// @brief This is the 3rd step (of 6) in a frame.
                /// @details This runs the main portion of the @ref algorithm_sec "scheduling algorithm" on the main thread. This call
                /// blocks until the execution of all workunits with main thread affinity are complete and all other work units have at
                /// least started. This could return and other threads could still be working.
                virtual void RunMainThreadWork();

                /// @brief This is the 4th step (of 6) in a frame.
                /// @details Used when completing the work of a frame, to cleaning end the execution of the threads. This function will
                /// only return when all the work started by @ref CreateThreads() and @ref RunMainThreadWork() have completed. This call
                /// blocks until all threads executing. If a thread takes too long then this simply waits for it to finish. No attempt is
                /// made to timeout or interupt a work unit before it finishes.
                void JoinAllThreads();

                /// @brief This is the 5th step (of 6) in a frame.
                /// @details Take any steps required to prepare all owned WorkUnits for execution next frame. This usually includes reseting
                /// all the work units running state to @ref NotStarted "NotStarted". This can cause work units to be executed multiple times
                /// if a thread is still executing.
                virtual void ResetAllWorkUnits();

                /// @brief This is the final step (of 6) in a frame.
                /// @details Wait until this frame has consumed its fair share of a second. This uses the value passed in
                /// @ref SetFrameRate "SetFrameRate" to determine what portion of a second each frame should
                /// use. If a frame took too long to execute this calculates that and returns.
                /// @n @n
                /// Wait 1/TargetFrame seconds, minus time already run. This also starts the timer for the next frame so
                /// any other logic that needs to run after the frame does not interfere with frame timing.
                void WaitUntilNextFrame();

        };//FrameScheduler
    } // \Threading
}// \Mezanine


#endif
