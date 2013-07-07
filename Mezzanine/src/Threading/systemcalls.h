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
#ifndef _systemcalls_h
#define _systemcalls_h

/// @file
/// @brief This file defines a minimalistic cross-platform thread that the scheduler uses to schedule tasks.

#include "datatypes.h"

namespace Mezzanine
{
    /// @brief Get a timestamp, in microseconds. This will generally be some multiple of the GetTimeStampResolution return value.
    /// @warning On some platforms this requires a static initialization, an can cause undefined behavior if called before static initializations are complete
    /// @return The largest size integer containing a timestamp that can be compared to other timestamps, but hads no guarantees for external value.
    MaxInt MEZZ_LIB GetTimeStamp();

    /// @brief Get the resolution of the timestamp in microseconds. This is the smallest amount of time that the GetTimeStamp can accurately track.
    /// @return A Whole which returns in millionths of a second the smallest unit of time that GetTimeStamp can measure.
    Whole MEZZ_LIB GetTimeStampResolution();

    /// @brief Get the amount of logical processors, a reasononable amount to use for thread creation.
    /// @details This returns whatever your OS thinks is the count of CPUs. This could include
    /// Hyperthreading unit on Intel's chip, or it might not, it could include the threads from
    /// Niagra CPUs or it might not, it could return just about any value on a given piece of
    /// and should return a reasonable value for how many threads should be used.
    /// @return A Whole containing the amount of processors.
    Whole MEZZ_LIB GetCPUCount();

    // @brief How much storage can be used before RAM must be used.
    // @details In many modern system cache (of all levels) is signifigantly faster than main
    // memory. For this reason the most useful cache value is the largest cache. This attempts
    // to get the largest value of cache on the CPU. If there are multiple physical CPUs (as
    // opposed to multiple cores) this will likely return the amount of cache on each CPU
    // package. This is useful for tuning alogrithms in a specific places and only in a single
    // thread.
    // @return The size of the largest CPU cache in bytes.
    //Whole MEZZ_LIB GetCacheSize();

    // @brief Get the size of one unit of storage on the CPU cache for purposes of tuning alogrithms.
    // @details This gets the smallest line size amongst available caches on the current system, it
    // assumes that L1 cache lines are smaller than L2 and those are smaller than L3 and so if required.
    // @return The size of a single cache line/cache unit
    //Whole MEZZ_LIB GetCachelineSize();

}//Mezzanine

#endif
