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
#ifndef _readwritespinlock_cpp
#define _readwritespinlock_cpp

#include "atomicoperations.h"
#include "readwritespinlock.h"
#include "crossplatformincludes.h"
#include <limits>
#include <cassert>

/// @file
/// @brief Contains the implementation for the @ref Mezzanine::Threading::Mutex Mutex synchronization object.

namespace Mezzanine
{
    namespace Threading
    {
        ReadWriteSpinLock::ReadWriteSpinLock() : Locked(0)
        {}

        ReadWriteSpinLock::~ReadWriteSpinLock()
        {}


        void ReadWriteSpinLock::LockForRead()
            { while(!TryLockForRead()){} }

        bool ReadWriteSpinLock::TryLockForRead()
        {
            if(CountGaurd.TryLock())
            {
                if(0<=Locked)
                {
                    assert(0<=Locked); // fail because of timing bug in this lock
                    Locked++;
                    CountGaurd.Unlock();
                    return true;
                }else{
                    CountGaurd.Unlock();
                    return false;
                }
            }else{
                return false;
            }
        }

        void ReadWriteSpinLock::UnlockRead()
        {
            CountGaurd.Lock();
            assert(!(0>Locked));  // fail because of writelock
            assert(!(0==Locked)); // fail because not locked
            Locked--;
            CountGaurd.Unlock();
        }

        void ReadWriteSpinLock::LockForWrite()
            { while(!TryLockForWrite()){} }

        bool ReadWriteSpinLock::TryLockForWrite()
        {
            if(CountGaurd.TryLock())
            {
                if(0==Locked)
                {
                    Locked=std::numeric_limits<Int32>::min();
                    CountGaurd.Unlock();
                    return true;
                }else{
                    CountGaurd.Unlock();
                    return false;
                }
            }else{
                return false;
            }
        }

        void ReadWriteSpinLock::UnlockWrite()
        {
            CountGaurd.Lock();
            assert(!(0<Locked));  // fail because of Readlock
            assert(!(0==Locked)); // fail because not locked
            assert(!(std::numeric_limits<Int32>::min()!=Locked)); // failed because cannot unlocked if already unlocked
            Locked=0;
            CountGaurd.Unlock();
        }



    } // \Threading namespace
} // \Mezzanine namespace
#endif
