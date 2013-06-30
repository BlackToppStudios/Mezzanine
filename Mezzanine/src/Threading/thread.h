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
#ifndef _btsthread_h
#define _btsthread_h

/// @file
/// @brief This file defines a minimalistic cross-platform thread that the scheduler uses to schedule tasks.

// Parts of this library use the TinyThread++ libary and those parts are also covered by the following license
/*
Copyright (c) 2010-2012 Marcus Geelnard

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

#include "compilerthreadcompat.h"
#include "systemcalls.h"
#include "mutex.h"

#include <ostream>
#include <vector>

namespace Mezzanine
{
    namespace Threading
    {
        /// @brief A small wrapper around the system thread.
        /// @details In general game code should not be creating this if they are using the
        /// DAG Frame Scheduler, as it tries to maintain control over the threads created
        /// by a game. This tries to keep the names the same as the standard thread, and might
        /// at some point be replace by some template machinery that wraps the minor difference
        /// that currently exist.
        class MEZZ_LIB Thread
        {
            public:
            /// @typedef native_handle_type
            /// @brief The native handle type, made available with requiring specific knowledge of whether it is a posix or win32 handle.
            #if defined(_MEZZ_THREAD_WIN32_)
                typedef HANDLE native_handle_type;
            #else
                typedef pthread_t native_handle_type;
            #endif

                // Forward Declaration
                class id;

                /// @brief Default constructor.
                /// @details Construct a @c thread object without an associated thread of execution
                /// (i.e. non-joinable).
                Thread() : mHandle(0), mNotAThread(true)
            #if defined(_MEZZ_WIN32_)
                , mWin32ThreadID(0)
            #endif
                {}

                /// @brief Thread starting constructor with no parameters.
                /// @note May work in templates that do not attempt to pass values to threads, but not standard conformant.
                /// @param[in] aFunction A function pointer to a function of type:
                ///          @code void fun(void * arg) @endcode
                explicit Thread(void (*aFunction)(void *));

                /// @brief Thread starting constructor.
                /// @details Construct a @c thread object with a new thread of execution.
                /// @param[in] aFunction A function pointer to a function of type:
                ///          @code void fun(void * arg) @endcode
                /// @param[in] aArg Argument to the thread function.
                /// @note This constructor is not fully compatible with the standard C++
                /// thread class. It is more similar to the pthread_create() (POSIX) and
                /// CreateThread() (Windows) functions.
                Thread(void (*aFunction)(void *), void * aArg);

                /// @brief Destructor.
                /// @note If the thread is joinable upon destruction, @c std::terminate()
                /// will be called, which terminates the process. It is always wise to do
                /// @c join() before deleting a thread object.
                ~Thread();

                /// @brief Wait for the thread to finish (join execution flows).
                /// @details After calling @c join(), the thread object is no longer associated with
                /// a thread of execution (i.e. it is not joinable, and you may not join
                /// with it nor detach from it).
                void join();

                /// @brief Check if the thread is joinable.
                /// @details A thread object is joinable if it has an associated thread of execution.
                /// @return A bool, false if the thread has been detached, joined or otherwise made unjoinable by exceptional circumstances, true otherwise.
                bool joinable() const;

                /// @brief Detach from the thread.
                /// @details After calling @c detach(), the thread object is no longer assicated with
                /// a thread of execution (i.e. it is not joinable). The thread continues
                /// execution without the calling thread blocking, and when the thread
                /// ends execution, any owned resources are released.
                void detach();

                /// @brief Return the thread ID of a thread object.
                /// @return A platform specific handle for the thread.
                id get_id() const;

                /// @brief Get the native handle for this thread.
                /// @note Under Windows, this is a @c HANDLE, and under POSIX systems, this is a @c pthread_t.
                /// @return The native handle for the thread in case it is desired to use it with OS APIs.
                inline native_handle_type native_handle()
                    { return mHandle; }

                /// @brief Determine the number of threads which can possibly execute concurrently.
                /// @details This function is useful for determining the optimal number of threads to
                /// use for a task.
                /// @return The number of hardware thread contexts in the system.
                /// @note If this value is not defined, the function returns zero (0).
                static unsigned hardware_concurrency();

            private:
                /// @brief Deleted assignment operator.
                #ifdef _MEZZ_CPP11_PARTIAL_
                    Thread& operator=(const Thread&) = delete;
                #else
                    Thread& operator=(const Thread&);
                #endif

                /// @brief Deleted assignment operator copy constructor.
                #ifdef _MEZZ_CPP11_PARTIAL_
                    Thread(const Thread&) = delete;
                #else
                    Thread(const Thread&);
                #endif


                native_handle_type mHandle;   ///< Thread handle.
                mutable Mutex mDataMutex;     ///< Serializer for access to the thread private data.
                bool mNotAThread;             ///< True if this object is not a thread of execution.
            #if defined(_MEZZ_THREAD_WIN32_)
                unsigned int mWin32ThreadID;  ///< Unique thread ID (filled out by _beginthreadex).
            #endif

            #if defined(_MEZZ_THREAD_WIN32_)
                /// @brief This is the internal thread wrapper function.
                static unsigned WINAPI wrapper_function(void * aArg);
            #else
                /// @brief This is the internal thread wrapper function.
                static void * wrapper_function(void * aArg);
            #endif
        };//Thread

        /// @brief The thread ID is a unique identifier for each thread.
        /// @see thread::get_id()
        class MEZZ_LIB Thread::id
        {
            public:
                /// @brief Default constructor.
                /// @details The default constructed ID is that of thread without a thread of
                /// execution.
                id() : mId(0)
                    {}

                /// @brief Creation from long.
                /// @param aId The OS provided long to create an ID from.
                id(unsigned long int aId) : mId(aId)
                    {}

                /// @brief Copy constructor.
                /// @param aId The other id to create a copy of.
                id(const id& aId) : mId(aId.mId)
                    {}

                /// @brief Assignment Operator.
                /// @param aId The right hand operand during assignment.
                /// @return A reference to the left hand operand after it has been assigned.
                inline id & operator=(const id &aId)
                {
                      mId = aId.mId;
                      return *this;
                }

                /// @brief Equality Comparison.
                /// @param aId1 Left hand operand.
                /// @param aId2 Right hand operand.
                /// @returns True boolean value if the OS specific IDs match and false otherwise.
                inline friend bool operator==(const id &aId1, const id &aId2)
                    { return (aId1.mId == aId2.mId); }

                /// @brief Inequality Comparison.
                /// @param aId1 Left hand operand.
                /// @param aId2 Right hand operand.
                /// @returns False boolean value if the OS specific IDs match and true otherwise.
                inline friend bool operator!=(const id &aId1, const id &aId2)
                    { return (aId1.mId != aId2.mId); }

                /// @brief Greater than or equal to Comparison.
                /// @param aId1 Left hand operand.
                /// @param aId2 Right hand operand.
                /// @returns A consistent value to allow for sorting/ordering of threads.
                inline friend bool operator<=(const id &aId1, const id &aId2)
                    { return (aId1.mId <= aId2.mId); }

                /// @brief Greater than Comparison.
                /// @param aId1 Left hand operand.
                /// @param aId2 Right hand operand.
                /// @returns A consistent value to allow for sorting/ordering of threads.
                inline friend bool operator<(const id &aId1, const id &aId2)
                    { return (aId1.mId < aId2.mId); }

                /// @brief Less than or equal to Comparison.
                /// @param aId1 Left hand operand.
                /// @param aId2 Right hand operand.
                /// @returns A consistent value to allow for sorting/ordering of threads.
                inline friend bool operator>=(const id &aId1, const id &aId2)
                    { return (aId1.mId >= aId2.mId); }

                /// @brief Less than Comparison.
                /// @param aId1 Left hand operand.
                /// @param aId2 Right hand operand.
                /// @returns A consistent value to allow for sorting/ordering of threads.
                inline friend bool operator>(const id &aId1, const id &aId2)
                    { return (aId1.mId > aId2.mId); }

                /// @brief Output Streaming operator.
                /// @param os The output stream to send the id into.
                /// @param obj the streamed id.
                /// @returns A reference to the output stream to allow for operator chaining.
                inline friend std::ostream& operator <<(std::ostream &os, const id &obj)
                {
                      os << obj.mId;
                      return os;
                }

            private:
				/// @internal
                /// @brief The ID value
                unsigned long int mId;
        };//Thread::id

        /// @brief A partial implementation of std::thread::this_thread.
        namespace this_thread
        {
            /// @brief Return the thread ID of the calling thread.
            /// @return A thread::id unique to this thread.
            Thread::id MEZZ_LIB get_id();

            /// @brief Yield execution to another thread.
            /// @details Offers the operating system the opportunity to schedule another thread
            /// that is ready to run on the current processor.
            void MEZZ_LIB yield();

            /// @brief Blocks the calling thread for a period of time.
            /// @param MicroSeconds Minimum time to put the thread to sleep.
            void MEZZ_LIB sleep_for(UInt32 MicroSeconds);

            // At some point I would like an overload of this function
            /*
            // Blocks the calling thread for a period of time.
            // @param aTime Minimum time to put the thread to sleep.
            // Example usage:
            // @code
            // // Sleep for 100 milliseconds
            // this_thread::sleep_for(chrono::milliseconds(100));
            // @endcode
            // @note Supported duration types are: nanoseconds, microseconds,
            // milliseconds, seconds, minutes and hours.
            template <class _Rep, class _Period> void sleep_for(const chrono::duration<_Rep, _Period>& aTime)
            {
            #if defined(_MEZZ_THREAD_WIN32_)
                Sleep(int(double(aTime.count()) * (1000.0 * _Period::_as_double()) + 0.5));
            #else
                usleep(int(double(aTime.count()) * (1000000.0 * _Period::_as_double()) + 0.5));
            #endif
            }
            */
        } // namespace this_thread
    }//Threading
}//Mezzanine

#endif
