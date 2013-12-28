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
#ifndef _spinlock_h
#define _spinlock_h

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

#include "datatypes.h"

/// @file
/// @brief Declares a Mutex, Mutex tools, and at least one MutexLike object.

namespace Mezzanine
{
    namespace Threading
    {
        /// @brief A mutex like construct that never makes a system call and uses CPU instructions instead.
        /// @details This should be used when delay is likely to be measured in CPUs cycles and almost
        /// certainly a short while. For pauses of unknown length use a Mutex so that the OS is informed it
        /// could schedule another thread.
        /// @n @n
        /// This is compatible with the lock_guard class.
        class MEZZ_LIB SpinLock
        {
            private:
                /// @internal
                /// 0 if unlocked, any other value if locked.
                Int32 Locked;

            public:
                ///@brief Constructor, creates an unlocked mutex
                SpinLock();

                ///@brief Destructor.
                ~SpinLock();

                /// @brief Lock the SpinLock.
                /// @details The method will block the calling thread until a lock on the SpinLock can
                /// be obtained. The SpinLock remains locked until @c unlock() is called.
                /// @see lock_guard
                void Lock();

                /// @brief Try to lock the spinlock.
                /// @details The method will try to lock the SpinLock. If it fails, the function will
                /// return immediately (non-blocking).
                /// @return @c true if the lock was acquired, or @c false if the lock could
                /// not be acquired.
                bool TryLock();

                /// @brief Unlock the spinlock.
                /// @details If any threads are waiting for the lock on this mutex, one of them will
                /// be unblocked.
                void Unlock();
        };
    }//Threading
}//Mezzanine

#endif
