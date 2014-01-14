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
#ifndef _readwritespinlock_h
#define _readwritespinlock_h

#include "datatypes.h"
#include "lockguard.h"
#include "spinlock.h"

/// @file
/// @brief Declares a Mutex, Mutex tools, and at least one MutexLike object.

namespace Mezzanine
{
    namespace Threading
    {
        /// @brief A mutex like construct that supports multiple readsingle writer semantics and never makes a system call and uses CPU instructions instead.
        /// @details This should be used when delay is likely to be measured in CPUs cycles and almost
        /// certainly a short while. For pauses of unknown length use a Mutex so that the OS is informed it
        /// could schedule another thread.
        /// @n @n
        /// Provided readers and writers call the correct functions this will only allow access to a single writer
        /// or a group of readers at one time
        class MEZZ_LIB ReadWriteSpinLock
        {
            private:
                /// @brief Used to synchronize access to Locked, to indicate either the current count of Read locks or the existence of write locks
                SpinLock CountGaurd;

                /// @internal
                /// @brief 0 if unlocked, A positive amount is amount of locking readers and a negative value is writed locked.
                Int32 Locked;

            public:
                ///@brief Constructor, creates an unlocked mutex
                ReadWriteSpinLock();
                ///@brief Destructor.
                ~ReadWriteSpinLock();

                /// @brief Lock the ReadWriteSpinLock for Reading
                /// @details The method will block the calling thread until a read lock
                /// be obtained. This remains locked until @c UnlockRead() is called.
                /// @n @n
                /// Multiple read locks can be obtained. If this is locked multiple this
                /// will keep count until @c UnlockRead() decrements that count.
                void LockForRead();
                /// @brief Try to lock this for reading.
                /// @details The method will try to lock the ReadWriteSpinLock for Reading. If
                /// it fails, the function will return immediately (non-blocking).
                /// @return @c true if the lock was acquired, or @c false if the lock could
                /// not be acquired.
                bool TryLockForRead();
                /// @brief Unlock this .
                /// @details Decrement the reader count and If zero any threads that are waiting
                /// for the LockWrite method on this mutex, one of them will be unblocked. If
                /// locked for write, this does nothing.
                void UnlockRead();

                /// @brief Lock the ReadWriteSpinLock for Writing.
                /// @details The method will block the calling thread until a write lock
                /// be obtained. This remains locked until @c UnlockWrite() is called.
                /// @n @n
                /// Only a single write lock can exist and it can co-exist with no read locks.
                void LockForWrite();
                /// @brief Try to lock this for writing.
                /// @details The method will try to lock the ReadWriteSpinLock for writing. If
                /// it fails, the function will return immediately (non-blocking).
                /// @return @c true if the lock was acquired, or @c false if the lock could
                /// not be acquired.
                bool TryLockForWrite();
                /// @brief Unlock this for writing.
                /// @details If any threads are waiting for the read or write lock on this,
                /// one of them willbe unblocked. If locked for read this doe nothing.
                void UnlockWrite();

                /// @brief Simply calls LockForWrite() for compatibility with lock_guard
                void lock()
                    { LockForWrite(); }
                /// @brief Simply calls UnlockWrite() for compatibility with lock_guard
                void unlock()
                    { UnlockWrite(); }
        };//Mutex



        /// @brief Read only lock guard.
        /// @details The constructor locks the mutex, and the destructor unlocks the mutex, so
        /// the mutex will automatically be unlocked when the lock guard goes out of
        /// scope. Example usage:
        /// @code
        /// ReadWriteSpinLock m;
        /// ClassThatIsWrittenInAnotherThread n;
        ///
        /// ClassThatIsWrittenInAnotherThread GetCurrent()
        /// {
        ///   ReadOnlyLockGuard<ReadWriteSpinLock> guard(m);
        ///   return n; // returns a copy guaranteed to be valid because
        ///             // the lock (which is attempted in the constructor
        ///             // of the guard) could not be acquired until only
        ///             // readers want it.
        /// }
        /// @endcode
        template <class T>
        class ReadOnlyLockGuard
        {
            public:
                /// @brief This allows other code to use the type of this mutex in a more safe way.
                typedef T mutex_type;

            private:
                /// @internal
                /// @brief A non-owning pointer to the mutex.
                mutex_type* mMutex;

            public:
                /// @brief The constructor locks the mutex.
                /// @param aMutex Any mutex which implements lock() and unlock().
                explicit ReadOnlyLockGuard(mutex_type& aMutex)
                    : mMutex(&aMutex)
                    { mMutex->LockForRead(); }

                /// @brief The destructor unlocks the mutex.
                ~ReadOnlyLockGuard()
                    { mMutex->UnlockRead(); }
        };//lock_guard

        /// @brief ReadWrite lock guard.
        /// @details The constructor locks the mutex, and the destructor unlocks the mutex, so
        /// the mutex will automatically be unlocked when the lock guard goes out of
        /// scope. Example usage:
        /// @code
        /// ReadWriteSpinLock m;
        /// int counter;
        ///
        /// int Increment()
        /// {
        ///   ReadWriteLockGuard<ReadWriteSpinLock> guard(m);
        ///   return counter++;
        /// }
        /// @endcode
        template <class T>
        class ReadWriteLockGuard
        {
            public:
                /// @brief This allows other code to use the type of this mutex in a more safe way.
                typedef T mutex_type;

            private:
                /// @internal
                /// @brief A non-owning pointer to the mutex.
                mutex_type* mMutex;

            public:
                /// @brief The constructor locks the mutex.
                /// @param aMutex Any mutex which implements lock() and unlock().
                explicit ReadWriteLockGuard(mutex_type& aMutex)
                    : mMutex(&aMutex)
                    { mMutex->LockForWrite(); }

                /// @brief The destructor unlocks the mutex.
                ~ReadWriteLockGuard()
                    { mMutex->UnlockWrite(); }
        };//lock_guard

    }//Threading
}//Mezzanine

#endif
