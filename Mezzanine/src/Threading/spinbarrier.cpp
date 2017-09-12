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
#ifndef _spin_barrier_cpp
#define _spin_barrier_cpp

#include "spinbarrier.h"

#include <cassert>

/// @file
/// @brief Contains the implementation for the @ref Mezzanine::Threading::Barrier Barrier synchronization object.

namespace Mezzanine
{
    namespace Threading
    {

        const SpinBarrier::NonAtomicInt SpinBarrier::Entering = 0;
        const SpinBarrier::NonAtomicInt SpinBarrier::Exiting = 1;

        SpinBarrier::SpinBarrier (const Int32& SynchThreadCount)
            : ThreadGoal (SynchThreadCount),
              BlockingState(Entering),
              ThreadCurrent (0)
        {}

        Boole SpinBarrier::Wait()
        {
            // If a thread is so fast it hits the barrier while others are exiting wait for the barrier to reset.
            while(BlockingState.load() == Exiting);

            const Int32 PriorAmountOfThreads = ThreadCurrent.fetch_add(1);
            const Int32 AmountNeedToBreak = ThreadGoal - 1;
            if(PriorAmountOfThreads == AmountNeedToBreak)
            {
                // This thread is breaking the barrier
                BlockingState.store(Exiting);       // Tell the other threads
                while(ThreadCurrent.load() != 1);   // Wait for other threads to leave wait function
                ThreadCurrent.fetch_sub(1);         // We are the last thread to leave so this sets it to 0
                BlockingState.store(Entering);      // Let threads start the next use of this barrier
                return true;
            }
            else
            {
                // This thread is just piling on
                while(BlockingState.load() != Exiting); // Wait for all the other threads
                ThreadCurrent.fetch_sub(1);             // Decrement to let breaking thread know
                return false;
            }
        }

        void SpinBarrier::SetThreadSyncCount(Int32 NewCount)
            { ThreadGoal.store(NewCount); }

    } // \Threading namespace
} // \Mezzanine namespace
#endif
