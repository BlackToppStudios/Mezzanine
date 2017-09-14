// © Copyright 2010 - 2017 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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
#ifndef _physicstaskscheduler_h_cpp
#define _physicstaskscheduler_h_cpp

#include "crossplatformexport.h"
#include "Threading/spinbarrier.h"

#include <LinearMath/btThreads.h>

#include <thread>
#include <vector>

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a Mezzanine::Threading::iWorkUnit for the updating of the physics debug drawer.
        ///////////////////////////////////////
        class MEZZ_LIB ParallelForScheduler : public btITaskScheduler
        {
        public:
            /// @brief Convenience type for the storage of threads to use.
            using ThreadContainer = std::vector< std::thread >;
        private:
            /// @brief This is where all threads begin each iteration.
            Threading::SpinBarrier StartingLine;
            /// @brief This is where all threads end each iteration.
            Threading::SpinBarrier FinishLine;
            /// @brief Container storing all of the threads to be used by this scheduler.
            ThreadContainer Threads;
            /// @brief The work of the loop to be done.
            const btIParallelForBody* Body;
            /// @brief Cache of the start of the work to be done on the "Body" member.
            int IndexStart;
            /// @brief Cache of the end of the work to be done on the "Body" member.
            int IndexEnd;
            /// @brief Storage necessary for post construction activation, stores the number of threads to create.
            unsigned int ThreadCount;
            /// @brief Whether or not the the Scheduler is still running.
            std::atomic_bool StillRunning;
        protected:
            /// @brief Creates a re-usable pool of threads.
            /// @param NumThreads How many threads are needed
            void CreatePool(int NumThreads);
            /// @brief Joins all the threads for cleanup.
            void JoinAllThreads();
        public:
            /// @brief Class constructor.
            /// @param numThreads The number of threads to run when processing a group of objects.
            ParallelForScheduler(int numThreads);
            /// @brief Class destructor.
            virtual ~ParallelForScheduler();

            /// @brief Enables this scheduler and prepares it for use.
            virtual void activate();
            /// @brief Disables this scheduler and cleans up it's resources.
            virtual void deactivate();

            /// @brief Gets the maximum number of threads this scheduler allows to run.
            /// @remarks The interface means for this to possibly change based on the underlying threading implementation.
            /// In our case we're just using c++11 threads.
            /// @return Returns the maximum number of threads supported by this threading solution.
            virtual int getMaxNumThreads() const override;
            /// @brief Gets how many threads that will run when processing a group of objects.
            /// @return Returns the number of threads that will be used to process a group of objects.
            virtual int getNumThreads() const override;
            /// @brief Sets how many threads that will run when processing a group of objects.
            /// @param numThreads The number of threads that will be used to process a group of objects.
            /// @warning This is probably slow, this destroys all the old threads and recreates them.
            virtual void setNumThreads(int numThreads) override;
            /// @brief Processes an array in parallel.
            /// @param iBegin The index of the first object of the array segment to process.
            /// @param iEnd The index of one-passed-the-last object of the array segment to process.
            /// @param grainSize The amount of objects to process per segment/the size of the segment to process.
            /// @param body The "body" of the for loop processing the objects.
            virtual void parallelFor(int iBegin, int iEnd, int grainSize, const btIParallelForBody& body) override;
        };//ParallelForScheduler
    }//Physics
}//Mezzanine

#endif
