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
#ifndef _frameschedulerworkunits_cpp
#define _frameschedulerworkunits_cpp

#include "frameschedulerworkunits.h"
#include "doublebufferedresource.h"

/// @file
/// @brief The implementation of any workunits the framescheduler needs to work correctly

namespace Mezzanine
{
    namespace Threading
    {

        LogAggregator::LogAggregator() : AggregationTarget(NULL)
        {}

        LogAggregator::~LogAggregator()
        {}

        void LogAggregator::DoWork(DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
        {
            if(!AggregationTarget)
                { AggregationTarget = CurrentThreadStorage.GetFrameScheduler(); }
            AggregationTarget->LogResources.Lock();

            std::ostream& Log = AggregationTarget->GetLog();
            Log << "<Frame Count=\"" << AggregationTarget->GetFrameCount() << "\">" << std::endl;
            for(std::vector<DefaultThreadSpecificStorage::Type*>::const_iterator Iter=AggregationTarget->Resources.begin();
                Iter!=AggregationTarget->Resources.end();
                ++Iter)
            {
                Log << "<Thread Main=\"" << (AggregationTarget->Resources.begin()==Iter?1:0)<< "\">" << std::endl
                    << (*Iter)->GetResource<DoubleBufferedLogger>(DBRLogger).GetCommittable().str()
                    << "</Thread>" << std::endl;
                (*Iter)->GetResource<DoubleBufferedLogger>(DBRLogger).GetCommittable().str("");
            }
            AggregationTarget->LogResources.Unlock();
            Log << "</Frame>" << std::endl;
        }


        FrameScheduler* LogAggregator::GetAggregationTarget() const
            { return AggregationTarget; }

        void LogAggregator::SetAggregationTarget(FrameScheduler* NewTarget)
            { AggregationTarget = NewTarget; }

        WorkSorter::WorkSorter() :
            SortingFrequency(MEZZ_FRAMESTOTRACK*2),
            FramesSinceLastSort(0)
        {}

        void WorkSorter::DoWork(DefaultThreadSpecificStorage::Type &CurrentThreadStorage)
        {
            FramesSinceLastSort++;
            if(FramesSinceLastSort==SortingFrequency)
            {
                #ifdef MEZZ_DEBUG
                CurrentThreadStorage.GetResource<DoubleBufferedLogger>(DBRLogger).GetUsable() << "<SortWork sorting=\"0\" ThreadID=\"" << Mezzanine::Threading::this_thread::get_id() << "\" />" << std::endl;
                #endif
                FramesSinceLastSort=0;
                FrameScheduler& FS = *CurrentThreadStorage.GetFrameScheduler();
                //WorkUnitsMain.clear(); // should be cleared by the framescheduler or whatever copies this into the framscheduler
                //WorkUnitsAffinity.clear();
                WorkUnitsMain.clear();
                WorkUnitsAffinity.clear();
                WorkUnitsMain.insert(WorkUnitsMain.end(),FS.WorkUnitsMain.begin(),FS.WorkUnitsMain.end());
                FS.UpdateWorkUnitKeys(WorkUnitsMain);
                WorkUnitsAffinity.insert(WorkUnitsAffinity.end(),FS.WorkUnitsAffinity.begin(),FS.WorkUnitsAffinity.end());
                FS.UpdateWorkUnitKeys(WorkUnitsAffinity);
                FS.Sorter = this;
            }else{
                #ifdef MEZZ_DEBUG
                CurrentThreadStorage.GetResource<DoubleBufferedLogger>(DBRLogger).GetUsable() << "<SortWork sorting=\"1\" ThreadID=\"" << Mezzanine::Threading::this_thread::get_id() << "\" />" << std::endl;
                #endif
            }
        }

        void WorkSorter::SetSortingFrequency(Whole FramesBetweenSorts)
            { SortingFrequency = FramesBetweenSorts; }

        Whole WorkSorter::GetSortingFrequency()
            { return SortingFrequency; }

    }//Threading
}//Mezzanine


#endif
