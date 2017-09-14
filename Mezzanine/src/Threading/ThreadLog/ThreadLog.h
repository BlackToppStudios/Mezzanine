// The DAGFrameScheduler is a Multi-Threaded lock free and wait free scheduling library.
// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef ThreadLog_H
#define ThreadLog_H

#include "ThreadIndex.h"
#include "SingleThreadLogEntry.h"
#include "AggregatedLogEntry.h"

namespace Mezzanine {
namespace Threading {

    /// @brief This class is intended to assist in troublshooting threading issues with low overhead.
    /// @details Race conditions can be difficult to troublshoot. If you have found this then you likely know what a
    /// race condition is and can skip past this to the Overview section a few paragraphs below.
    /// @n @n
    /// Race conditions are subtle bugs that occur when two threads try to access the same resource without correctly
    /// ensuring that changes to the resource happen while no other thread is accessing that resource. The most common
    /// kind of race is when two or more threads that might reference the same variable and only occasionally change it.
    /// If that variable isn't properly synchronized between threads then a huge variety of failures can happen. The
    /// variable might be read half in an old state and half in a newer state, the value may be stale even though that
    /// appears impossible, the variable may do dynamic allocation internally and leave incorrect pointers in other
    /// threads. Even when it looks like code is synchronized it is possible for the Compiler, the operating or the CPU
    /// to re-order many kinds of instructions such that events during the actual execution of threads doesn't happen in
    /// the order of the source code.
    /// @n @n
    /// If a programmer is carefully following the C++ memory model or follows simple multithreading patterns these
    /// classes of bugs can often be avoided. This is not always as simple as it appears. When someone tries to get
    /// "clever" without understanding the memory model or when someone tries to create their own synchronization tools
    /// without knowing best practices used in the industry this can lead to subtle bugs that occur only occasionally.
    /// @n @n
    /// The real change can be something as simple a new optimization added to the compiler. If this optimization
    /// presumed your program didn't rely on undefined behavior but unknowingly did then there could be a change in
    /// thread behavior. The change could be as simply as running the program on a machine with more or fewer cores (or
    /// more or less load from other processes). This could change the timing of when resources are accessed, and an the
    /// accesses to an improperly synchronized resource might be move all to time in a batch of work making an error
    /// happen more frequently.
    /// @n @n
    /// Whatever the details, the code might have worked 10,000 times and seemed flawless until one day something subtle
    /// changed and you are left holding a program that was bug free yesterday but crashes today and there were no code
    /// changes.
    /// @n @n
    /// @b Overview
    /// @n This is a replacement for another common set of tools when fixing thread bugs, cout and printf. Many
    /// threading issues can be isolated by inserting some kind of output statement into the code and capturing that
    /// output from a failure. Often a bug will be expressed 1 in a 1,000 runs so the debugging developer will make
    /// simple script to run the program thousands of times and go through the logs attempting to recreate the failure.
    /// This often works but can just as often mask the bug. Output to the console, file or other OS level construct
    /// will invoke some kind of synchronization mechanism. Inside the OS there is only the one buffer to the console or
    /// contents of the file, so the OS must have mutexes, memory fences or some other kind of synchronization to
    /// prevent race conditions in calling code. Sometimes this is enough to hide a race conditions. Even tools that do
    /// no synchronization intentionally may do some by accident as they log to these simplistic resources across
    /// threads or simply take too much time marshalling log messages.
    /// @n
    /// This works in a similar way to those but with the lowest overhead possible. Each thread gets its own binary log,
    /// and into that it writes one pointer at a time. There is never any synchronization called just a call to get the
    /// time and copy a pointer into the log (If your system synchronizes on the time that is really rough, this won't
    /// help you much).
    /// @n
    /// For you to get the full use out of the ThreadLog you will need to insert calls to it in your code and follow the
    /// 4 stages to its use. First, you must initialize it. Second each thread must be registered. Third, Each thread
    /// logs its messages. Fourth and Finally, the contents of the log are aggregated and sorted into chronological
    /// order. This will give you a step by step log of what your application did regardless of however many threads
    /// it is doing that work on.
    /// @n @n
    /// @b Preparation
    /// @n
    /// @todo Flesh this out
    /// Use @ref ThreadLog::PrepareLogGroup to create a log for each thread and optionally to preallocate space for each
    /// threads log. After setting this more threads cannot be added, the amount passed for the thread count must be
    /// equaly or larger than the actual number of threads. This is because it is impossible to change the layout of the
    /// logging structures without including some kind of synchronization primitive that could alter the behavior of
    /// your code and multiple threads are using this structure to get access to their logs.
    /// @n
    /// The starting space for each thread is much more forgiving. It defaults to 2,000 log entries. Each thread gets
    /// its own std::vector that will store the log. This will expand as more log entries are added. This is possible
    /// because each thread is the only thread that will access its own log. The memory allocations could still cause
    /// accidental synchronizations, to minimize this try to set the log size larger than the amount any single thread
    /// will log.
    ///
    /// @code{.cpp}
    /// ThreadLog::PrepareLogGroup(1); // Reserve space for 1 thread (with memory reserved for 2000 entries in each)
    /// ThreadLog::PrepareLogGroup(1200); // Reserve space 1200 threads (with memory reserved for 2000 entries in each)
    /// ThreadLog::PrepareLogGroup(10, 1000); // Reserve space for 10 logs with 1000 entries in each
    /// @endcode
    ///
    /// @n @n
    /// @b Registration
    /// @n
    ///
    /// Each thread needs to get its own unique thread number that starts at 0 and counts up. If each thread had this
    /// number it could perform a constant time lookup into an array or similar construct and retrieve its log and do
    /// anything with that log with fear of threading issues. This is exactly the purpose of the @ref ThreadIndex class.
    ///
    /// A thread can register at any time to get its own ThreadIndex with the @ref ThreadLog::RegisterThread function.
    /// It must do this before calling any of the loggingfunctions. It doesn't matter if any other thread has started
    /// logging or not, as long as each thread registers before it starts logging.
    ///
    /// The ThreadIndex is stored in thread_local storage and will not need to be handled directly by call code.
    ///
    /// @code{.cpp}
    /// ThreadLog::RegisterThread(); // Do this in each thread before that thread logs.
    /// @endcode
    ///
    /// @n @n
    /// @b Logging
    /// @n
    /// To insure this is a constant time operation as often as possible the loggin
    ///
    /// @n @n
    /// @b Aggregation
    /// @n
    class MEZZ_LIB ThreadLog
    {
    public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////

        /// @brief Thread Indexes are intended to uniquely and sequentially ID threads
        using ThreadIndexType = ThreadIndex;

        /// @brief This type is what is actually logged from each thread
        using ThreadLogEntryType = SingleThreadLogEntry;

        /// @brief Each thread will get unrestricted access to one of these
        using ThreadLogType = std::vector<ThreadLogEntryType>;

        /// @brief The type of the log used in each thread.
        using ThreadLogGroupType = std::vector<ThreadLogType>;

        /// @brief A log that stores logs from multiple threads
        using AggregatedLogType = std::vector<AggregatedLogEntry>;

    public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Copy, Move and other value and class semantics
    ////////////////////////////////////////

        /// @brief Deleted constructor, to prevent construction.
        ThreadLog() = delete;

        /// @brief Deleted destructor, to further discourage attempts to construct.
        ~ThreadLog() = delete;

        /// @brief Deleted copy constructor, because copying something that can never be construction is nonsensical.
        ThreadLog(const ThreadLog&) = delete;

        /// @brief Delete move constructor.
        ThreadLog(ThreadLog&&) = delete;

    protected:
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Data Members
    ////////////////////////////////////////

        /// @brief Used to count the amount of threads that are registered
        /// @note This starts counting at -1 so that the threads start at 0 and this has no relation to the
        /// value in InvalidThreadIndex.
        static std::atomic<ThreadIndexType::InternalType> ThreadCount;

        /// @brief The storage for this threads Index.
        /// @details This is used to index into storage for this thread.
        static thread_local ThreadIndexType CurrentThreadIndex;

        /// @brief The collection of logs for each thread.
        static ThreadLogGroupType ThreadLogGroup;

    public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Register Threads
    ////////////////////////////////////////

        /// @brief This prepares a thread for low impact logging and must be called in each thread that will be logging.
        /// @details This is safe to call multiple times and the the threads ID will not change.
        /// @returns A copy of this threads index.
        static ThreadIndexType RegisterThread();

        /// @brief Check what thread we are on.
        static ThreadIndexType GetThreadIndex();

        /// @brief This restarts the thread counter prepares for a new group of threads.
        /// @warning Any threads registered before calling this must not log further, Those threads cannot even
        /// re-register because their ID will not reset and will conflict with future thread claiming those Indexes.
        static void ResetRegistry();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Basic Logging
    ////////////////////////////////////////

        /// @brief Reserve space and resources for the Group of logs.
        /// @details This clears old logs and creates space for new logs. This must be called with an amount larger than
        /// the possible amount of threads or it could fail because there is no log for a given thread.
        /// @param MaxThreadCount The largest amount of threads that logs will be captured for.
        /// @param StartingLogReservation How many log entries should be stored before allocations are required?
        /// Defaults to 2000 log entries, Set this higher or lower based on how many log entries you think a single
        /// thread might make at maximum. This will expand automatically, but could mask some threading errors.
        /// @warning This must be called before any thread logs.
        /// @warning When called MaxThreadCount must be larger than the amount of threads logging.
        static void PrepareLogGroup(const ThreadIndex::InternalType& MaxThreadCount,
                                    const ThreadLogGroupType::size_type& StartingLogReservation = 2000);

        /// @brief Record a timestamped message in this thread's log.
        /// @param A pointer to a C-String, preferably a string literal that isn't going anywhere.
        static void Log(const char* Message);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Log Viewing
    ////////////////////////////////////////

        /// @brief Convenience method to retrieve log with ints
        /// @param An integer which indicates the thread index that is compatible with the size_type of vector.
        /// @return An immutable reference to the threads logs.
        static const ThreadLogType& GetLog(const ThreadIndex::InternalType& Index);

        /// @brief Convenience method to retrieve log with ints
        /// @param An integer which indicates the thread index that is compatible with the size_type of vector.
        /// @return An immutable reference to the threads logs.
        static const ThreadLogType& GetLog(const ThreadIndexType& Index);

        /// @brief Aggregate All of the Thread logs into one Chronologically sorted log.
        /// @warning No logging can happen while this occurs (and we can't put a mutex into the logging functions
        /// because that would change their timing).
        /// @return A chronologically sorted group of log entries with the ThreadIndex, Log Message and Time stamps.
        static AggregatedLogType AggregateLogs();


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Log Viewing Helpers
    ////////////////////////////////////////

        /// @brief Print the Log from One Thread to any std::ostream
        /// @param Stream the std::ostream to print to.
        /// @param Index the thread index for the log to print
        static void PrintOneThreadsLog(std::ostream& Stream,
                                       const ThreadIndex& Index = ThreadLog::GetThreadIndex());

        /// @brief Print the log from all the threads to a given std::ostream
        /// @warning By default this calls @ref AggregateLogs and has all the same restrictions
        /// @param Stream the std::ostream to print to.
        /// @param Logs Defaults to calling @ref AggregateLogs and using its results but can work with any instance of
        /// an AggregatedLogType,
        static void PrintAggregatedLog(std::ostream& Stream,
                                       const AggregatedLogType& Logs = AggregateLogs());

        /// @brief Attempt to have the ThreadLog Emit logs to std::cerr when an OS signal occurs.
        /// @param Signal The signal to attempt to handle from the csignal header.
        /// @details This will configure a function that prints the aggregated logs as the signal handler for the passed
        /// signal.
        /// @warning This invokes several kinds of undefined behavior and exposes race conditions. This relies on the
        /// notion that most segfaults stop other execution and undefined behavior often works as expected. Try to log
        /// normally before using this.
        static void PrintLogOnSignal(int Signal);

        /// @brief Call PrintLogOnSignal for SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV and SIGTERM.
        static void PrintLogOnAllSignals();


    }; //ThreadLog

    /// @macro PREPARE_THREAD_LOG
    /// @brief When the CMake option Mezz_Profile is enabled this calls @ref Mezzanine::ThreadLog::PrepareLogGroup
    /// @param ThreadCount The highest possible amount of threads.
    /// @param StartingEntries The starting entry count.

    /// @macro REGISTER_THREAD
    /// @brief When the CMake option Mezz_Profile is enabled this calls @ref Mezzanine::ThreadLog::RegisterThread

    /// @macro THREAD_LOG
    /// @brief When the CMake option Mezz_Profile is enabled this calls @ref Mezzanine::ThreadLog::ThreadLog passing
    /// the file and line number.

    #ifdef MEZZTHREADLOG
        #define PREPARE_THREAD_LOG(ThreadCount, StartingEntries) \
            Mezzanine::Threading::ThreadLog::PrepareLogGroup(ThreadCount, StartingEntries);
        #define REGISTER_THREAD Mezzanine::Threading::ThreadLog::RegisterThread();
        #define THREAD_LOG Mezzanine::Threading::ThreadLog::Log(__FILE__ ":" Stringify(__LINE__));
    #else
        #define PREPARE_THREAD_LOG(ThreadCount, StartingEntries)
        #define REGISTER_THREAD
        #define THREAD_LOG
    #endif

} //Threading
} //Mezzanine
#endif
