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

#include <atomic>
#include <vector>

namespace Mezzanine { namespace Threading {

    /// @brief This class is intended to assiste in troublshooting threading issues with low overhead.
    /// @details Race conditions can be difficult to troublshoot. If you have found this then you likely know what a
    /// race condition is and can skip past this to the Usage section a few paragraphs below.
    /// @n @n
    /// Race conditions are subtle bugs that occur when two thread try to access the same resource without correctly
    /// ensuring that changes to the resource happen while no other thread is accessing that resource. The most common
    /// kind of race is when two or more threads that might reference the same variable and only occasionally change it.
    /// If that variable isn't properly synchronized between threads then a huge variety of failures can happen. The
    /// variable might be read half in an old state and half in a newer state, the value may be stale even though that
    /// appears impossible, the variable may do dynamic allocation internally and leave incorrect pointers in other
    /// threads. Even when it looks like code is synchronized it is possible for the Compiler, the operating or the CPU
    /// to re-order many kinds of instructions such that events during the actual execution of threads doesn't happen in
    /// the order of the source code.
    /// @n @n
    /// If a programmer is carefully following the C++ memory model or sticking to simple multithreading patterns these
    /// classes of bugs can often be avoided. This is not always as simple as it appears. When someone tries to get
    /// "clever" without understanding the memory model or when someone tries to creat their own synchronization tools
    /// with knowing best practices used in the industry this can lead to subtle bugs that occur only occasionally.
    /// @n @n
    /// The real change can be something as simple a new optimization added to the compiler. If this optimization
    /// presumed your program didn't rely on undefined behavior but unknowing did then there could be a change in thread
    /// behavior. The change could be as simply as running the program on a machine with more or fewer cores (or more or
    /// less load from other processes). This could change the timing of when resources are accessed, and an the accesses
    /// to an improperly synchronized resource might be move all to time in a batch of work.
    /// @n @n
    /// Whatever the details, the code might have worked 10,000 times and seemed flawless until one day something subtle
    /// changed and you are left holding a program that was bug free yesterday but crashes today and there were no code
    /// changes.
    /// @n @n
    /// @b Usage
    /// @n This is a replacement for another common tool when fixing bugs, cout. Many threading issues can be isolated
    /// by inserting some kind of output statement into the code and capturing that output from a failure. Often a bug
    /// will be expressed 1 in a 1,000 runs so the debugging developer will make simple script to run the program 10,000
    /// times and go through the logs attempting to recreate the failure. This often works but can just as oftend mask
    /// the bug. output to the console, file or other OS level construct will invoke some kind of synchronization
    /// mechanism. thead mutex that is in many cout implementations is sometimes enough to hide a race condition is
    /// reasonably fast code. Even tools that do no synchronization intentionally may do some by accident as they log
    /// across threads or simplytake too much time marshalling log messages.
    /// @n
    /// This works in a similar way to those but with the lowest overhead possible. Each thread gets its own binary log,
    /// and into that it writes one pointer at a time.
    class ThreadLog
    {
    public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////

        /// @brief Thread Indexes are intended to uniquely and sequentially ID threads
        using ThreadIndexType = ThreadIndex;

        /// @brief This type is what is actaully logged from each thread
        using ThreadLogEntryType = SingleThreadLogEntry;

        /// @brief Each thread will get unrestricted access to one of these
        using ThreadLogType = std::vector<ThreadLogEntryType>;

        /// @brief The type of the log used in each thread.
        using ThreadLogGroupType = std::vector<ThreadLogType>;

        /// @brief Look into using an allocator to carefully align memory to mitigate false sharing.
        //using ThreadLogGroupType = std::vector<ThreadLogType, AlignedAllocator<ThreadLogType, 64>>;

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

    private:
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
    // Register Threads for fast lookup
    ////////////////////////////////////////

        /// @brief This prepares a thread for low impact logging and must be called in each thead that will be logging.
        /// @details This is safe to call mulitple times and the the threads ID will not change.
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
        /// @warning This must be called before any thread logs.
        /// @warning When called MaxThreadCount must be larger than the amount of threads logging.
        static void PrepareLogGroup(const ThreadLogGroupType::size_type& MaxThreadCount);

        /// @brief Record a timestamped message in this thread's log.
        /// @param A pointer to a C-String, preferably a string literal that isn't going anywhere.
        static void Log(const char* Message);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Log Viewing
    ////////////////////////////////////////

        /// @brief Convience method to retrieve log with ints
        /// @param An integer which indicates the thread index that is compatible with the size_type of vector.
        /// @return An immutable reference to the threads logs.
        static const ThreadLogType& GetLog(const ThreadLogGroupType::size_type& Index);

        /// @brief Convience method to retrieve log with ints
        /// @param An integer which indicates the thread index that is compatible with the size_type of vector.
        /// @return An immutable reference to the threads logs.
        static const ThreadLogType& GetLog(const ThreadIndexType& Index);

        /// @brief Aggregate All of the Thread logs into one Chronologically sorted log.
        /// @warning No logging can happen while this occurs (and we can't put a mutex into the logging functions
        /// because that would change their timing).
        /// @return A chronologically sorted group of log entries with the ThreadIndex, Log Message and Time stamps.
        static AggregatedLogType AggregateLogs();

        // print single thread log

        // print multi thread log

        // Add macros.
    };

} }
#endif
