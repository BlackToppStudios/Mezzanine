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
#ifndef _DAGFrameScheduler_h
#define _DAGFrameScheduler_h

/// @file
/// @brief This is the file that code using this library should include. It includes all the required components.

#include "datatypes.h"

#ifndef MEZZANINE_CORE
    #ifdef SWIG
        // Tell SWIG to create a module that scripting languages can use called "mezzanine"
        // and insert a minimum of documentation into the bindingfile
        %{
            // code to be inserted verbatim into the swig file goes here
            #ifdef GetCurrentTime
            #undef GetCurrentTime
            #endif

            using namespace Mezzanine;
            using namespace Mezzanine::Threading;
        %}

        %include stl.i
        %include stdint.i
        %include std_except.i
        %include std_common.i
        //%include std_container.i
        %include std_deque.i
        %include std_except.i
        //%include std_list.i
        %include std_map.i
        %include std_pair.i
        %include std_string.i
        %include std_vector.i
    #endif
#else
	#include "swig.h"
#endif


#ifdef SWIG_THREADING
    #ifdef SWIG_UNSAFE
        %module MezzanineThreading
    #else
        #define SWIG_SAFE
        %module MezzanineThreadingSafe
    #endif
    #define SWIG_MODULE_SET
#endif

#include "asynchronousfileloadingworkunit.h"
#include "asynchronousworkunit.h"
#include "atomicoperations.h"
#include "barrier.h"
//#include "crossplatformincludes.h" // This is omitted because windows.h include a ton of macros that break clean code, so this vile file's scope must be minimized
#include "crossplatformexport.h"
#include "datatypes.h"
#include "doublebufferedresource.h"
#include "framescheduler.h"
#include "frameschedulerworkunits.h"
#include "logtools.h"
#include "monopoly.h"
#include "mutex.h"
#include "rollingaverage.h"
#include "systemcalls.h"
#include "thread.h"
#include "threadingenumerations.h"
#include "workunit.h"
#include "workunitkey.h"


/// @def MEZZ_DAGFRAMESCHEDULER_MAJOR_VERSION
/// @brief The Major version number of the library. (The front/left number)
#define MEZZ_DAGFRAMESCHEDULER_MAJOR_VERSION 1

/// @def MEZZ_DAGFRAMESCHEDULER_MINOR_VERSION
/// @brief The Minor version number of the library. (The middle number)
#define MEZZ_DAGFRAMESCHEDULER_MINOR_VERSION 11

/// @def MEZZ_DAGFRAMESCHEDULER_REVISION_VERSION
/// @brief The revision version number of the library. (This right/back number)
#define MEZZ_DAGFRAMESCHEDULER_REVISION_VERSION 0



#ifndef MEZZANINE_CORE
/// @mainpage Directed Acyclic Graph Frame Scheduler.
/// For an in depth description please see the @ref ThreadingManual
#endif

/// @page ThreadingManual Directed Acyclic Graph Frame Scheduler
/// @section dag_header The DAG Frame Scheduler Manual.
/// This page will explain what the DAG FrameScheduler than explain how to use it.
/// @section dag_contents Contents
///     - @ref dag_goal_sec - A description of this algorithm and how/why it is different from other threading algorithms.
///     - @ref dag_usage - Examples, code snippets, caveats and best practices with the scheduler. *Incomplete*
/// @subsection dag_goal_sec Goals
/// This library tries to make writing multithreaded software easier by changing the kinds of primitives that
/// multithreaded software is built upon. Several libraries before this have attempted this already.
/// This library is different becuse it focuses on a specific kind of workload and provides the kinds of
/// guarantees that the workload needs while sacrificing other guarantees that the workload does not need.
/// @n @n
/// This attempts to provide a multithreading solution for workloads that must be run in many iterations in
/// a given amount of realtime. Games are an ideal example. Every frame a video game must update physics
/// simulations, make AI decisions, accept/interpret user input, enforce game rules, perform dynamic I/O
/// and render it to the screen all while maintaining a smooth FrameRate and do that while minimizing drain
/// to batteries on portable devices (sometimes without even knowing if the device is portable).
/// @n @n
/// This library accomplishes those goals by removing the conventional mutlithreading primitives that so many
/// developers have come to fear, loathe, or misunderstand. Mutexes, threads, memory fences, thread_local
/// storage, atomic variables, and all the pitfalls that come with them are replaced by a small set of
/// of primitives that provide all the required sophistication a typical multi-threaded application
/// requires. It does this using a new kind of @ref Mezzanine::Threading::iWorkUnit "iWorkUnit",
/// @ref Mezzanine::Threading::DoubleBufferedResource "Double Buffering", a strong concept of
/// dependencies and a @ref Mezzanine::Threading::FrameScheduler "FrameScheduler" that uses heuristics
/// to decide how to run it all without exposing needless complexity to the application developer.
///
/// @subsection overview_sec Algorithm Overview
/// The DAGFrameScheduler is a variation on a common multithreaded work queue. It seeks to avoid its pitfalls,
/// such as non-determinism, thread contention, and lackluster scalability while keeping its advantages
/// including simplicity, understandiblity, and low overhead.
/// @n @n
/// With this algorithm very few, if any,
/// calls will need to be made to the underlying system for synchronization of the actual work to be performed.
/// Instead, this library will provide limited (not always, but for constistent workloads)
/// deterministic ordering of @ref Mezzanine::Threading::iWorkUnit "iWorkUnit" execution through a dependency
/// feature. Having the knowledge that one @ref Mezzanine::Threading::iWorkUnit "iWorkUnit" will complete after
/// another allows for resources to be used without using expensive and complex synchronization mechansisms
/// like @ref Mezzanine::Threading::Mutex "mutexes", semaphores, or even an
/// @ref Mezzanine::Threading::AtomicCompareAndSwap32 "Atomic Compare And Swaps". These primitives are provided
/// to allow use of this library in advanced ways for developers who are already familiar with
/// multithreaded systems.
/// @n @n
/// The internal work queue is not changed while a frame is executing. Because it is only read, each
/// thread can pick its own work. Synchronization still needs to occur, but it has been moved onto each
/// @ref Mezzanine::Threading::iWorkUnit "iWorkUnit" amd is managed with atomic CPU operations. Like this,
/// contention is less frequent occurring only when threads simultaneously attempt to start the same
/// @ref Mezzanine::Threading::iWorkUnit "iWorkUnit".  This consumes far less time because atomic operations
/// are CPU instructions instead of Operating System calls. This is managed by the library, so individual
/// @ref Mezzanine::Threading::iWorkUnit "iWorkUnit"s do not need to worry synchronization beyond telling
/// each @ref Mezzanine::Threading::iWorkUnit "iWorkUnit" about its data dependencies and making sure
/// all the @ref Mezzanine::Threading::iWorkUnit "iWorkUnit"s are added to a
/// @ref Mezzanine::Threading::FrameScheduler "FrameScheduler".
///
/// @subsection broken_sec Broken Algorithms
/// To understand why a new multithreading system is needed, it is helpful to look at other methods
/// of threading that have been used in the past. This can give us an understanding of what they lack
///  or how they aren't ideal for the kinds of work this algorithm is intended for. This overview is
/// intentionally simplified. There are variations on many of these algorithms that can fix some of
/// the problems presented. Despite these workarounds there are fundamental limitations that prevent
/// these algorithms from being ideal for video games, simulations and similar tasks.
/// These threading models aren't necessarily broken, as some of these clearly have a place in software
/// development. Many of these require complex algorithms, require subtle knowledge, or simply aren't
/// performant enough for realtime environments.
/// @n @n
/// I will use charts that plot possible resource use of a computer across time. Generally time will
/// run across the top as resources, usually CPUs, will run down one side. Most of these algorithms have a
/// concept of tasks or workunits, which are just pieces of work with a distinct begining and end. The
/// width of a piece of work loosely represents the execution time (the names are just for show and not
/// related to anything real).
/// @subsubsection broken_Single Single Threaded
/// An application using this threading model is not actually multithreaded at all. However, It has been shown
/// that software can run in a single and get good perfomance. This is the benchmark all other threading models
/// get compared too.
/// @n @n
/// There is a term, Speedup ( http://en.wikipedia.org/wiki/Speedup ), which is simply a
/// comparison of the single threaded performance of an algorithm to the mutlithreaded performance. You simply
/// determine how many times more work the multithreaded algorithm does in the same time, or how many times
/// longer the single threaded algorithm takes to the same work. Ideally two threads will be twice as fast
/// (speedup of 2x), and three thread would be three times as fast (3x speedup), and so; this is called linear
/// speedup. In practice there is always some overhead in creating and synchronizing threads, so achieving
/// linear speedup is difficult.
/// @image html Single.png "Single Threaded Execution - Fig 1."
/// @image latex Single.png "Single Threaded Execution - Fig 1."
/// @image rtf Single.png "Single Threaded Execution - Fig 1."
/// @n @n
/// The DAGFrameScheduler library tries to tailor the threading model to a specific problem to minimize that
/// overhead. With a single threaded application one thread does all the work and always wastes every other
/// thread, but there is no overhead if the system only has one thread.
/// @n @n
/// @subsubsection broken_Unplanned Unplanned Thread
/// Sometimes someone means well and tries to increase the performance of a single threaded program and tries
/// to add extra threads to increase performance. Sometimes this works really well and sometimes there is a
/// marginal increase in performance or a significant increase in bugs. If that someone has a good plan
/// then they can usually achieve close to the best speedup possible in the given situation. This is not easy
/// and many cannot do this or do not want to invest the time it would take. If not carefully planned
/// bugs like deadlock ( http://en.wikipedia.org/wiki/Deadlock ) and race conditions
/// ( http://stackoverflow.com/questions/34510/what-is-a-race-condition )
/// can be introduced. Unfortunately no amount of testing can replace this careful planning. Without a
/// complete understanding of how multithreaded software is assembled (a plan) it is not possible to prove
/// that multithreaded software will not hang/freeze or that it will produce the correct results.
/// @n @n
/// Software with no multithreading plan could have just about any kind of execution behavior. Usually
/// unplanned software performs at least slightly better than single threaded versions of the software, but
/// frequently does not utilize all the available resources. Generally performance does not scale well as
/// unplanned software is run on more processors. Frequently, there is contention for a specific resource and
/// a thread will wait for that resource longer than is actually needed.
/// @image html Unplanned.png "Unplanned Threaded Execution - Fig 2."
/// @image latex Unplanned.png "Unplanned Threaded Execution - Fig 2."
/// @image rtf Unplanned.png "Unplanned Threaded Execution - Fig 2."
/// @n @n
/// The DAGFrameScheduler is carefully planned and completely avoids costly synchronization
/// mechanisms in favor of less costly minimalistic ones. Marking one @ref Mezzanine::Threading::iWorkUnit "iWorkUnit"
/// as dependent on another allows the reordering of @ref Mezzanine::Threading::iWorkUnit "iWorkUnits" so that
/// some @ref Mezzanine::Threading::iWorkUnit "iWorkUnit" can be executed with no thread waiting or blocking.
/// @n @n
/// @subsubsubsection broken_TaskPerThread One Task Per Thread
/// A common example of poor planning is the creation of one thread for each task in a game. Despite
/// being conceptually simple, performance of systems designed this way is poor due to synchronization
/// and complexities that synchronization requires.
/// @subsection broken_ConventionWorkQueue Conventional Work Queue/Thread Pools
/// Conventional work queues and thread pools are a well known and robust way to increase the throughput of
/// of an application. These are ideal solutions for many systems, but not games.
/// @n @n
/// In conventional workqueues all of the work is broken into a number of small thread-safe
/// units. As these units are created they are stuffed into a queue and threads pull out units of work
/// as it completes other units it has started. This simple plan has many advantages. If there is work
/// to do then at least one thread will be doing some, and usually more threads will be working. This is
/// good for games and the DAGFrameScheduler mimics it. If the kind of work is unknown when the software is
/// written heuristics and runtime decisions can create the kind of units of work that are required. This
/// is not the case with games and the others kinds of software this library caters to, so changes can
/// be made that remove the problems this causes. One such drawback is that a given unit of work never
/// knows if another is running or has completed, and must therefor make some pessimistic assumptions.
/// @image html Threadpool.png "Convention Work Queue/ThreadPools - Fig 3."
/// @image latex Threadpool.png "Convention Work Queue/ThreadPools - Fig 3."
/// @image rtf Threadpool.png "Convention Work Queue/ThreadPools - Fig 3."
/// @n @n
/// Common synchronization mechanisms like mutexes or semaphores block the thread for an unknown
/// amount of time, and are required by the design of workqueues. There are two times this is required.
/// The first time is whenever a work unit is acquired by a thread, a mutex (or similar) must be used
/// to prevent other threads from modifying the queue as well. This impacts scalability, but can be
/// circumvented to a point. Common ways to work around this try to split up the work queue
/// pre-emptively, or feed the threads work units from varying points in the queue. The
/// DAGFrameScheduler moves the synchronizantion onto each work unit to greatly reduce the contention as
/// more work units are added.
/// @n @n
/// The other, and less obvious, point of contention that has not be circumvented in a
/// satisfactory way for games is the large of amount of synchronization required between units of
/// work that must communicate. For example, there may be hundreds of thousands of pieces of data
/// that must be passed from a system into a 3d rendering system. Applying mutexes to each would slow
/// execution an impossibly long time (if it didn't introduce deadlock), while more coarse grained
/// lock would prevent large portions of physics and rendering from occurring at the time causing
/// one or both of them to wait/block. A simple solution would be to run physics before graphics,
/// but common work queues do not provide good guarantees in this regard.
/// @n @n
/// The DAGFrameScheduler was explicitly designed to provide exactly this guarantee. If the
/// physics @ref Mezzanine::Threading::iWorkUnit "iWorkUnit" is added to the graphics
/// @ref Mezzanine::Threading::iWorkUnit "iWorkUnit" with
/// @ref Mezzanine::Threading::iWorkUnit::AddDependency() "AddDependency(WorkUnit*)" then it will
/// always be run before the graphics workunit in a given frame. The drawback of this is that it
/// is more difficult to make runtime creation of workunits (It is possible but it cannot be done
/// during any frame execution), but completely removes the locking
/// mechanisms of conventional work queues. The DAGFrameScheduler has traded one useless feature
/// for a useful guarantee.
///
/// @subsection algorithm_sec The Algorithm
/// When first creating the DAGFrameScheduler it was called it "Dagma-CP". When describing it the
/// phrase "Directed Acyclic Graph Minimal Assembly of Critical Path" was used. If you are lucky
/// enough to knows what all those terms mean when assembled this way they are very descriptive. For
/// rest of us the algorithm tries to determine what is the shortest way to execute the work in a
/// minimalistic way using a mathematical graph. The graph is based on what work must done
/// before other work each frame and executing it. All the work in this graph will have a
/// location somewhere between the beginning and end, and will never circle around back so it can
/// be called acyclic.
/// @n @n
/// This algorithm was designed with practicality as the first priority. To accomodate and integrate
/// with a variety of other algorithms and system a variety of Work Units are provided. New classes
/// can be created that inherit from these to allow them to be in the scheduler where they will work
/// best.
///     @li @ref Mezzanine::Threading::iWorkUnit "iWorkUnit" - The interface for a common workunit.
/// These work units will be executed once each frame after all their dependencies have completed.
/// These are also expected to complete execution in a relatively brief period of time compared to
/// the length of a frame, and create no threads while doing so.
///     @li @ref Mezzanine::Threading::DefaultWorkUnit "DefaultWorkUnit" - A simple implementation
/// of an @ref Mezzanine::Threading::iWorkUnit "iWorkUnit". This may not be suitable for every
/// use case, but it should be suitable for most. Just one function for derived classes to
/// implement, the one that does actual work.
///     @li @ref Mezzanine::Threading::iAsynchronousWorkUnit "iAsynchronousWorkUnit" - Intended to
/// allow loading of files and streams even after the framescheduler has paused. Work units are
/// to spawn one thread and manage it without interfering with other execution. DMA, and other
/// hardware coprocessors are expected to be utilized to their fullest to help accomplish this.
///     @li @ref Mezzanine::Threading::MonopolyWorkUnit "MonopolyWorkUnit" - These are expected
/// to monopolize cpu resources at the beginning of each frame. This is ideal when working with
/// other systems. For example a phsyics system like Bullet3D. If the calls to a physics system are
/// wrapped in a @ref Mezzanine::Threading::MonopolyWorkUnit "MonopolyWorkUnit" then it will be
/// given full opportunity to run before other work units.
///
/// Once all the @ref Mezzanine::Threading::MonopolyWorkUnit "MonopolyWorkUnit"s are done then the
/// @ref Mezzanine::Threading::FrameScheduler "FrameScheduler" class instance spawns or activates
/// a number of threads based on a simple heuristic. This heuristic is the way work units are sorted
/// in preparation for execution. To understand how these are sorted, the dependency system needs to
/// be understood.
/// @n @n
/// Most other work queues do not provide any guarantee about the order work will be executed in.
/// This means that each piece of work must ensure its own data integrity using synchronization
/// primitives like mutexes and semaphores to protect from being corrupted by multithread access. In
/// most cases these should be removed and one of any two work units that must read/write the data
/// must depend on the other. This allows the code in the workunits to be very simple even if it
/// needs to use a great deal of data other work units may also consume or produce.
/// @n @n
/// Once all the dependencies are in place for any synchronization that has been removed, a
/// @ref Mezzanine::Threading::FrameScheduler "FrameScheduler" can be created and started. At
/// runtime this creates a reverse dependency graph, a
/// @ref Mezzanine::Threading::FrameScheduler::DependentGraph "DependentGraph". This is used to determine
/// which work units are the most depended upon. For each work unit a simple count of how many work
/// units cannot start until has been completed is generated. The higher this number the earlier
/// the work unit will be executed in a frame. Additionally workunits that take longer to execute
/// will be prioritized ahead of work units that are faster.
/// @n @n
/// Here is a chart that provides an example of this re-factoring and the runtime sorting process:
/// @image html DAGSorting.png "DAG WorkSorting - Fig 4."
/// @image latex DAGSorting.png "DAG WorkSorting - Fig 4."
/// @image rtf DAGSorting.png "DAG WorkSorting - Fig 4."
/// @n @n
/// There are several advantages this sorting provides that are not immediately obvious. It separates
/// the scheduling from the execution allowing the relatively costly sorting process to be executed
/// only when work units are added, removed, or changed. Prioritizing Workunits that
/// take longer to run should help insure the shortest critical path is found by minimizing how often
/// dependencies cause threads to wait for more work.
/// @n @n
/// Sorting the work can be done by a manual call to
/// @ref Mezzanine::Threading::FrameScheduler::SortWorkUnitsAll "FrameScheduler::SortWorkUnitsAll()",
/// @ref Mezzanine::Threading::FrameScheduler::SortWorkUnitsMain "FrameScheduler::SortWorkUnitsMain()",
/// @ref Mezzanine::Threading::FrameScheduler::SortWorkUnitsAffinity "FrameScheduler::SortWorkUnitsAffinity()"
/// or by adding a @ref Mezzanine::Threading::WorkSorter "WorkSorter" WorkUnit to the
/// @ref Mezzanine::Threading::FrameScheduler "FrameScheduler". This only needs to be done when work
/// units have been added, removed, or their times are likely to have changed.
/// @n @n
/// Each thread queries the @ref Mezzanine::Threading::FrameScheduler "FrameScheduler" for the next
/// piece of work. The @ref Mezzanine::Threading::FrameScheduler "FrameScheduler" maintains the
/// list of work units and the next available piece of work can be retrieved with
/// @ref Mezzanine::Threading::FrameScheduler::GetNextWorkUnit "FrameScheduler::GetNextWorkUnit()" or
/// @ref Mezzanine::Threading::FrameScheduler::GetNextWorkUnitAffinity "FrameScheduler::GetNextWorkUnitAffinity()".
/// This by itself is not the atomic operation that allows the thread to execute the workunit, instead
/// @ref Mezzanine::Threading::iWorkUnit::TakeOwnerShip "iWorkUnit::TakeOwnerShip()" can grant that.
/// Internally this uses an @ref Mezzanine::Threading::AtomicCompareAndSwap32 "Atomic Compare And Swap"
/// operation to maximize performance.
/// By having the workunit manage the right to execute it removes the work queue as the primary source
/// of contention that would prevent scaling. This does add another potential point of slowdowns though;
/// threads must iterate over each other workunit until they reach the work to be executed. If atomic
/// operations are used to maintain an iterator that keeps track of where to start searching for work,
/// in a waitfree way, then we can trade the cost of this iteration for a number of atomic operations.
/// On some systems this is a great idea, on others a terrible idea, so it is a
/// @ref MEZZ_USEATOMICSTODECACHECOMPLETEWORK "CMake option called DecachingWork". Because this update
/// can be skipped if it work incur a wait, it does not recreate a central workqueue's primary point of
/// contention while providing all the benefits.
/// @image html DAGThreads.gif "DAG threads - Fig 5."
/// @n @n
/// Some work must be run on specific threads, such as calls to underlying devices (for example,
/// graphics cards using Directx or OpenGL). These @ref Mezzanine::Threading::iWorkUnit "iWorkUnit"s
/// are put into a different listing where only the main thread will attempt to execute them. Other
/// than running these, and running these first, the behavior of the main thread is very similar to
/// other threads.
/// @n @n
/// Even much of the @ref Mezzanine::Threading::FrameScheduler "FrameScheduler"'s work is performed
/// in @ref Mezzanine::Threading::iWorkUnit "iWorkUnit"s, such as log aggregation and
/// @ref Mezzanine::Threading::WorkSorter "Sorting the work listing".
/// @ref Mezzanine::Threading::iAsynchronousWorkUnit "iAsynchronousWorkUnit"s continue to run in a thread
/// beyond normal scheduling and are intended to consume fewer CPU resources and more IO resources.
/// For example loading a large file or listening for network traffic. These will be normal
/// @ref Mezzanine::Threading::iWorkUnit "iWorkUnit"s in most regards and will check on the asynchronous
/// tasks they manage each frame when they run as a normally scheduled.
/// @n @n
/// If a thread runs out of work because all the work is completed the frame will pause until it
/// should start again the next frame. This pause length is calulated using a runtime configurable value
/// on the @ref Mezzanine::Threading::FrameScheduler "FrameScheduler". If a thread has checked every
/// @ref Mezzanine::Threading::iWorkUnit "iWorkUnit" and some are still not executing, but could not
/// be started because of incomplete dependencies the thread will simply iterate over every
/// @ref Mezzanine::Threading::iWorkUnit "iWorkUnit" in the
/// @ref Mezzanine::Threading::FrameScheduler "FrameScheduler" until the dependencies of one are
/// met and allows one to be executed. This implicitly guarantees that at least one thread will
/// always do work, and if dependencies chains are kept short then it is more likely that several
/// threads will advance.
/// @subsection algorithmintegrate_sec Integrating with the Algorithm
/// When
/// @ref Mezzanine::Threading::FrameScheduler::DoOneFrame() "FrameScheduler::DoOneFrame()"
/// is called several things happen. All work units are executed, all threads are paused until the
/// this frame has consumed the amount of time it should, and the timer is restarted for the next
/// frame.
/// @n @n
/// This process is actually divided into six steps. The function
/// @ref Mezzanine::Threading::FrameScheduler::DoOneFrame() "FrameScheduler::DoOneFrame()"
/// simply calls the following functions:
/// @subsubsection integrate1 Step 1 - Run All the Monopolies
/// The function
/// @ref Mezzanine::Threading::FrameScheduler::RunAllMonopolies() "FrameScheduler::RunAllMonopolies()"
/// simply iterates through all the @ref Mezzanine::Threading::MonopolyWorkUnit "MonopolyWorkUnit"
/// that have been added with
/// @ref Mezzanine::Threading::FrameScheduler::AddWorkUnitMonopoly() "FrameScheduler::AddWorkUnitMonopoly()".
/// It does this in no specific order.
/// @n @n
/// In general @ref Mezzanine::Threading::MonopolyWorkUnit "MonopolyWorkUnit"s can be expected to use
/// all available CPU resources. Other threads should not be executed in general.
/// @subsubsection integrate2 Step 2 - Create and Start Threads
/// The function
/// @ref Mezzanine::Threading::FrameScheduler::CreateThreads() "FrameScheduler::CreateThreads()"
/// Creates enough threads to get to the amount set by
/// @ref Mezzanine::Threading::FrameScheduler::SetThreadCount() "FrameScheduler::SetThreadCount()"
/// . Depending on how this library is configured this could mean either creating that many threads each
/// frame or creating additional threads only if this changed.
/// @n @n
/// Regardless of the amount of threads created, all but one of them will start executing work units as
/// described in the section @ref algorithm_sec "The Algorithm". This will execute as much work as possible
/// (work units with affinity can affect how much work can be done with out waiting) that was added by
/// @ref Mezzanine::Threading::FrameScheduler::AddWorkUnitMain(iWorkUnit *, const String&) "FrameScheduler::AddWorkUnitMain".
/// If there is only one thread, the main thread, then this will return immediately and no work will be done.
/// @subsubsection integrate3 Step 3 - Main Thread Work
/// The call to
/// @ref Mezzanine::Threading::FrameScheduler::RunMainThreadWork() "FrameScheduler::RunMainThreadWork()"
/// will start the main thread executing work units. This is the call that executes work units added with
/// @ref Mezzanine::Threading::FrameScheduler::AddWorkUnitAffinity(iWorkUnit *, const String&) "FrameScheduler::AddWorkUnitAffinity".
/// @n @n
/// If you have single thread work that is not part of a work unit and will not interfere with and work
/// units execution then you can run it before calling this. Be careful when doing this, if there are any
/// work units that depend on work units with affinity then they will not be able to start until some
/// point after this is called.
/// @n @n
/// Once every work unit has started this call can return. This does not mean every work unit is complete,
/// though every work unit with affinity will be complete. There could be work in other threads still
/// executing. This is another good point to run work that is single threaded and won't interfere with
/// workunits that could be executing.
/// @subsubsection integrate4 Step 4 - Clean Up Threads
/// If you must execute something that could interfere (write to anything they could read or write)
/// with work units, you should do that after
/// @ref Mezzanine::Threading::FrameScheduler::JoinAllThreads() "FrameScheduler::JoinAllThreads()" is
/// called. This joins, destroys, or otherwise cleans up the threads the scheduler has used depending
/// on how this library is configured.
/// @subsubsection integrate5 Step 5 - Prepare for the next frame.
/// All the work units are marked as complete and need to be reset with
/// @ref Mezzanine::Threading::FrameScheduler::ResetAllWorkUnits() "FrameScheduler::ResetAllWorkUnits()"
/// to be used by the next frame. This simply iterates over each work unit resetting their status. A
/// potential future optimization could run this as a multithreaded monopoly instead.
/// @subsubsection integrate6 Step 6 - Wait for next frame.
/// The final step is to wait until the next frame should begin. To do this tracking the begining of
/// of each frame is required. The value in
/// @ref Mezzanine::Threading::FrameScheduler::CurrentFrameStart "FrameScheduler::CurrentFrameStart"
/// is on @ref Mezzanine::Threading::FrameScheduler "FrameScheduler" construction to the current time,
/// and reset every to the current time when
/// @ref Mezzanine::Threading::FrameScheduler::WaitUntilNextFrame() "FrameScheduler::WaitUntilNextFrame()"
/// is called. The value set by
/// @ref Mezzanine::Threading::FrameScheduler::SetFrameRate() "FrameScheduler::SetFrameRate()"
/// is used to calculate the desired length of a frame in microseconds. If the begining of the next
/// frame has not been reached, then this function will sleep the scheduler until the next frame should
/// begin. To compensate for systems with an imprecise sleep mechanism (or timing mechanism) an internal
/// @ref Mezzanine::Threading::FrameScheduler::TimingCostAllowance "FrameScheduler::TimingCostAllowance"
/// is tracked that averages the effects of imprecision across multiple frames to prevent roundings
/// errors from consistently lengthening of shortening frames.
/// @section dag_usage Using the DAG FrameScheduler
/// Still in progress. In the meantime please peruse the unit test source directory for examples.


/// @brief All of the Mezzanine game library components reside in this namespace.
/// @details The DAG Frame Scheduler is just one part of many in the Mezzanine. The Mezzanine as a
/// whole is intended to tie a complex collection of libraries into one cohesive library.
namespace Mezzanine
{
    /// @brief This is where game specific threading algorithms and a minimalistic subset of the std threading library a held
    /// @details This implements all of the multithreaded algorithms from BlackTopp Studios, parts of std::thread,
    /// std::this_thread, std:: mutex, and maybe a few others. In general only the specialized gaming algorithms store here are
    /// intended to be used in game code.
    namespace Threading
    {

    }//Threading
}//Mezzanine

#endif
