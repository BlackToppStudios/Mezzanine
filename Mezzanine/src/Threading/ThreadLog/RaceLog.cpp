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
#include "RaceLog.h"

#include <numeric>
#include <algorithm>

namespace Mezzanine { namespace Threading {

    std::atomic<ThreadLog::ThreadIndexType::InternalType> ThreadLog::ThreadCount{-1};
    thread_local ThreadLog::ThreadIndexType ThreadLog::CurrentThreadIndex;
    ThreadLog::ThreadLogGroupType ThreadLog::ThreadLogGroup;

    ThreadLog::ThreadIndexType ThreadLog::RegisterThread()
    {
        if(ThreadIndexType{} == CurrentThreadIndex)
        {
            CurrentThreadIndex = ++ThreadCount;
        }
        return CurrentThreadIndex;
    }

    ThreadLog::ThreadIndexType ThreadLog::GetThreadIndex()
    {
        return CurrentThreadIndex;
    }

    void ThreadLog::ResetRegistry()
    {
        ThreadCount = -1;
        CurrentThreadIndex = ThreadIndexType{-1};
    }

    void ThreadLog::PrepareLogGroup(const ThreadLogGroupType::size_type& MaxThreadCount)
    {
        ThreadLogGroup.clear();
        ThreadLogGroup.reserve(MaxThreadCount);
        for(ThreadLogGroupType::size_type logCreationCounter = 0;
            logCreationCounter < MaxThreadCount;
            logCreationCounter++)
        {
            ThreadLogGroup.push_back(ThreadLogType{});
        }
    }

    void ThreadLog::Log(const char* Message)
    {
        ThreadLogGroup[CurrentThreadIndex.AsRaw()].push_back(ThreadLogEntryType(Message));
    }

    const ThreadLog::ThreadLogType& ThreadLog::GetLog(const ThreadLog::ThreadLogGroupType::size_type& Index)
    {
        return GetLog(ThreadIndexType{Index}); // @todo Do something about this narrowing.
    }

    const ThreadLog::ThreadLogType& ThreadLog::GetLog(const ThreadLog::ThreadIndexType& Index)
    {
        return ThreadLogGroup[Index.AsRaw()];
    }

    ThreadLog::AggregatedLogType ThreadLog::AggregateLogs()
    {
        // to Assemble the log
        // get iterators to each begin.
        // find the lowest. Copy it into the assembled witht he ID
        // keep going until we hit the end of one thread
        //or
        // Add everything to one record then sort it.

        // Adding everyhing and then sorting:
        const ThreadLog::AggregatedLogType::size_type TotalCount{
            std::accumulate(ThreadLogGroup.begin(),
                            ThreadLogGroup.end(),
                            ThreadLog::AggregatedLogType::size_type{0},
                            [](ThreadLog::AggregatedLogType::size_type RunningTotal, const ThreadLogType& OneThreadLog)
                                { return RunningTotal + OneThreadLog.size(); }
                           )
        };
        const ThreadLog::AggregatedLogType::size_type ThreadCount{ThreadLogGroup.size()};

        ThreadLog::AggregatedLogType Results;
        Results.reserve(TotalCount);

        for(ThreadIndexType OneThreadIndex{0};
            OneThreadIndex < ThreadCount;
            ++OneThreadIndex)
        {
            const ThreadLogType& ThisLog = GetLog(OneThreadIndex);
            for(const SingleThreadLogEntry& OneEntry : ThisLog)
            {
                Results.emplace_back(OneThreadIndex, OneEntry);
            }
        }
        std::sort(Results.begin(), Results.end());

        return Results;
    }

} }

