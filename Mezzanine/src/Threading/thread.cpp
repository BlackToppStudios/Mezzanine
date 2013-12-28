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
#ifndef _btsthread_cpp
#define _btsthread_cpp

#include "crossplatformincludes.h"

/// @file
/// @brief This file implements a minimalistic cross-platform thread that the scheduler uses to schedule tasks.

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

#include "thread.h"
#include "crossplatformincludes.h"
#include "lockguard.h"

#if defined(_MEZZ_THREAD_POSIX_)
    #include <map>
#endif


namespace Mezzanine
{
    namespace Threading
    {

        //------------------------------------------------------------------------------
        // POSIX pthread_t to unique thread::id mapping logic.
        // Note: Here we use a global thread safe std::map to convert instances of
        // pthread_t to small thread identifier numbers (unique within one process).
        // This method should be portable across different POSIX implementations.
        //------------------------------------------------------------------------------

        /// @cond false
        #if defined(_MEZZ_THREAD_POSIX_)
        /// @internal
        /// @brief This converts the id in thread::id into a native thread handle
        /// @param aHandle An OS specific handle that will be used to lookup the thread::id
        /// @return The Corresponding thread::id
        /// @todo Investigate if this is a point of contention in the algorithm and try to remove it if possible
        static ThreadId _pthread_t_to_ID(const pthread_t &aHandle)
        {
          static Mutex idMapLock;
          static std::map<pthread_t, unsigned long int> idMap;
          static unsigned long int idCount(1);

          lock_guard<Mutex> guard(idMapLock);
          if(idMap.find(aHandle) == idMap.end())
            idMap[aHandle] = idCount ++;
          return ThreadId(idMap[aHandle]);
        }
        #endif // _MEZZ_POSIX_
        /// @endcond

        //------------------------------------------------------------------------------
        // thread
        //------------------------------------------------------------------------------

        /// @brief Information to pass to the new thread (what to run).
        struct _thread_start_info {
          void (*mFunction)(void *); ///< Pointer to the function to be executed.
          void * mArg;               ///< Function argument for the thread function.
          Thread * mThread;          ///< Pointer to the thread object.
        };

        // Thread wrapper function.
        #if defined(_MEZZ_THREAD_WIN32_)
        unsigned WINAPI Thread::wrapper_function(void * aArg)
        #elif defined(_MEZZ_THREAD_POSIX_)
        void * Thread::wrapper_function(void * aArg)
        #endif
        {
          // Get thread startup information
          _thread_start_info * ti = (_thread_start_info *) aArg;

          try
          {
            // Call the actual client thread function
            ti->mFunction(ti->mArg);
          }
          catch(...)
          {
            // Uncaught exceptions will terminate the application (default behavior
            // according to C++11)
            std::terminate();
          }

          // The thread is no longer executing
          lock_guard<Mutex> guard(ti->mThread->mDataMutex);
          ti->mThread->mNotAThread = true;

          // The thread is responsible for freeing the startup information if this can't, then failed threads
          // will clean it up in the threads constructor.
          delete ti;

          return 0;
        }

        Thread::Thread(void (*aFunction)(void *))
        {
            // Serialize access to this thread structure
            lock_guard<Mutex> guard(mDataMutex);

            // Fill out the thread startup information (passed to the thread wrapper,
            // which will eventually free it)
            _thread_start_info * ti = new _thread_start_info;
            ti->mFunction = aFunction;
            ti->mArg = 0;
            ti->mThread = this;

            // The thread is now alive
            mNotAThread = false;

            // Create the thread
        #if defined(_MEZZ_THREAD_WIN32_)
            mHandle = (HANDLE) _beginthreadex(0, 0, wrapper_function, (void *) ti, 0, &mWin32ThreadID);
        #elif defined(_MEZZ_THREAD_POSIX_)
            if(pthread_create(&mHandle, NULL, wrapper_function, (void *) ti) != 0)
                mHandle = 0;
        #endif

            // Did we fail to create the thread?
            if(!mHandle)
            {
                mNotAThread = true;
                delete ti; // Since there is no thread wrapper_function was never called, so it never deleted this
            }
        }

        Thread::Thread(void (*aFunction)(void *), void * aArg)
        {
            // Serialize access to this thread structure
            lock_guard<Mutex> guard(mDataMutex);

            // Fill out the thread startup information (passed to the thread wrapper,
            // which will eventually free it)
            _thread_start_info * ti = new _thread_start_info;
            ti->mFunction = aFunction;
            ti->mArg = aArg;
            ti->mThread = this;

            // The thread is now alive
            mNotAThread = false;

            // Create the thread
        #if defined(_MEZZ_THREAD_WIN32_)
            mHandle = (HANDLE) _beginthreadex(0, 0, wrapper_function, (void *) ti, 0, &mWin32ThreadID);
        #elif defined(_MEZZ_THREAD_POSIX_)
            if(pthread_create(&mHandle, NULL, wrapper_function, (void *) ti) != 0)
                mHandle = 0;
        #endif

            // Did we fail to create the thread?
            if(!mHandle)
            {
                mNotAThread = true;
                delete ti;
            }
        }

        Thread::~Thread()
        {
            if(joinable())
                std::terminate(); // prevents undefined behavior, and hopefully helps identifies bugs early
        }

        void Thread::join()
        {
            if(joinable())
            {
            #if defined(_MEZZ_THREAD_WIN32_)
                WaitForSingleObject(mHandle, INFINITE);
                CloseHandle(mHandle);
            #elif defined(_MEZZ_THREAD_POSIX_)
                pthread_join(mHandle, NULL);
            #endif
            }
        }

        bool Thread::joinable() const
        {
          mDataMutex.Lock();
          bool result = !mNotAThread;
          mDataMutex.Unlock();
          return result;
        }

        void Thread::detach()
        {
          mDataMutex.Lock();
          if(!mNotAThread)
          {
        #if defined(_MEZZ_THREAD_WIN32_)
            CloseHandle(mHandle);
        #elif defined(_TTHREAD_POSIX_)
            pthread_detach(mHandle);
        #endif
            mNotAThread = true;
          }
          mDataMutex.Unlock();
        }

        ThreadId Thread::get_id() const
        {
          if(!joinable())
            return ThreadId();
        #if defined(_MEZZ_THREAD_WIN32_)
          return ThreadId((unsigned long int) mWin32ThreadID);
        #elif defined(_MEZZ_THREAD_POSIX_)
          return _pthread_t_to_ID(mHandle);
        #endif
        }

        unsigned Thread::hardware_concurrency()
        {
        #if defined(_MEZZ_THREAD_WIN32_)
          SYSTEM_INFO si;
          GetSystemInfo(&si);
          return (int) si.dwNumberOfProcessors;
        #elif defined(_SC_NPROCESSORS_ONLN)
          return (int) sysconf(_SC_NPROCESSORS_ONLN);
        #elif defined(_SC_NPROC_ONLN)
          return (int) sysconf(_SC_NPROC_ONLN);
        #else
          // The standard requires this function to return zero if the number of
          // hardware cores could not be determined.
          return 0;
        #endif
        }


        //------------------------------------------------------------------------------
        // this_thread
        //------------------------------------------------------------------------------

        ThreadId this_thread::get_id()
        {
        #if defined(_MEZZ_THREAD_WIN32_)
          return ThreadId((unsigned long int) GetCurrentThreadId());
        #elif defined(_MEZZ_THREAD_POSIX_)
          return _pthread_t_to_ID(pthread_self());
        #endif
        }

        void MEZZ_LIB this_thread::yield()
        {
        #if defined(_MEZZ_THREAD_WIN32_)
            Sleep(0);
        #else
            sched_yield();
        #endif
        }

        void this_thread::sleep_for(UInt32 MicroSeconds)
        {
        #if defined(_MEZZ_THREAD_WIN32_)
            Sleep(MicroSeconds/1000);
        #else
            usleep(MicroSeconds);
        #endif
        }
    }//Threading
}//Mezzanine


#endif
