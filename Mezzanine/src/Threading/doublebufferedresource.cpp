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
#ifndef _doublebufferedresource_cpp
#define _doublebufferedresource_cpp

#include "datatypes.h"
#include "doublebufferedresource.h"
#include "framescheduler.h"

/// @file
/// @brief This file defines the template double buffered resources that can be attached to a thread.

namespace Mezzanine
{
    namespace Threading
    {
        ThreadSpecificStorage::ThreadSpecificStorage(FrameScheduler* Scheduler_) :
            Scheduler(Scheduler_)
        {
            this->ThreadResources.push_back((ConvertiblePointer)new DoubleBufferedLogger);  // must comes first so it lands in spot 0
        }

        Logger& ThreadSpecificStorage::GetUsableLogger()
            { return GetResource<DoubleBufferedLogger>(DBRLogger).GetUsable(); }

        FrameScheduler* ThreadSpecificStorage::GetFrameScheduler()
            { return Scheduler; }

        Whole ThreadSpecificStorage::GetLastFrameTime() const
            { return Scheduler->GetLastFrameTime() ; }

        Whole ThreadSpecificStorage::GetLastPauseTime() const
            { return Scheduler->GetLastPauseTime(); }

        MaxInt ThreadSpecificStorage::GetCurrentFrameStart() const
            { return Scheduler->GetCurrentFrameStart(); }

        void ThreadSpecificStorage::SwapAllBufferedResources()
            { GetResource<DoubleBufferedLogger>(DBRLogger).SwapUsableAndCommitable(); }

        ThreadSpecificStorage::~ThreadSpecificStorage()
        {
            delete (DoubleBufferedLogger*)this->ThreadResources[DBRLogger]; // Items must wind up in the correct spot in the vector for this to work. Be careful to get the order right if you overload this.
        }

    }//Threading
}//Mezzanine

#endif
