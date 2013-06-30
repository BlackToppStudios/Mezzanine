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

#ifndef _frameschedulerworkunits_h
#define _frameschedulerworkunits_h

#include "datatypes.h"
#include "workunit.h"
#include "workunitkey.h"

#include <vector>

/// @file
/// @brief This defines a number of workunits that are required for doing some tasks that the Framescheduler requires.

namespace Mezzanine
{
    namespace Threading
    {
        /// @brief Gather all the thread specific logs and commit them to the main log.
        /// @details All the logs are double buffered. This reads from the commitable buffer
        /// which should otherwise be unused. This task was separated.
        class MEZZ_LIB LogAggregator : public DefaultWorkUnit
        {
            public:
                /// @brief This does the actual work of log aggregation.
                /// @param CurrentThreadStorage Ignored, this workunits goals requires this.
                virtual void DoWork(DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
        };//LogAggregator

        /// @brief Swaps all of the Logs so the usable logs can be commited, and the usable commitable.
        /// @note If other doublebuffered resources are used then this or another work unit like it should be adjusted/created to swap those buffers.
        /// @details This needs to have every other workunit that uses the logger as a dependency. If you use the log in a work unit
        /// that work unit needs to depend on this one, or this one needs to depend on that one.
        class MEZZ_LIB LogBufferSwapper : public DefaultWorkUnit
        {
            public:
                /// @brief This does the swapping of buffers.
                /// @param CurrentThreadStorage Just to get a reference to the framescheduler.
                virtual void DoWork(DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
        };//LogBufferSwapper

        /// @brief Sorts all of the WorkUnits in the @ref FrameScheduler.
        class MEZZ_LIB WorkSorter : public DefaultWorkUnit
        {
                friend class FrameScheduler;
            protected:
                /// @brief 1 in every this many frames Sorting happens.
                Whole SortingFrequency;

                /// @brief How long since the last sort?
                Whole FramesSinceLastSort;

                /// @brief A freshly sorted WorkUnitsMain or an empty vector.
                std::vector<WorkUnitKey> WorkUnitsMain;

                /// @brief A freshly sorted WorkUnitsAffinity or an empty vector.
                std::vector<WorkUnitKey> WorkUnitsAffinity;

            public:
                /// @brief Default constructor.
                WorkSorter();

                /// @brief This usually does nothing, but sometimes it will do a whole bunch of work sorting.
                /// @param CurrentThreadStorage Just to get a reference to the framescheduler. So the WorkUnits can sorted and passed back to the Scheduler outisde of scheduling.
                virtual void DoWork(DefaultThreadSpecificStorage::Type& CurrentThreadStorage);

                /// @brief Set how often this actually does work
                /// @param FramesBetweenSorts 1 in every this many frame this will sort the work units.
                virtual void SetSortingFrequency(Whole FramesBetweenSorts);

                /// @brief Check how often this sorts.
                /// @return A Whole containing the sorting frequency.
                virtual Whole GetSortingFrequency();
        };//WorkSorter
    }//Threading
}//Mezzanine

#endif
