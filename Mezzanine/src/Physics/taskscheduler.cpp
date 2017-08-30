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
#ifndef _physicstaskscheduler_cpp
#define _physicstaskscheduler_cpp

#include "Physics/taskscheduler.h.cpp"

namespace Mezzanine
{
    namespace Physics
    {
        ParallelForScheduler::ParallelForScheduler(int numThreads) :
            btITaskScheduler( "Mzzanine" ),
            StartingLine( numThreads + 1 ),
            FinishLine( numThreads + 1 ),
            Body( nullptr ),
            IndexStart( 0 ),
            IndexEnd( 0 ),
            StillRunning( true )
            { this->CreatePool(numThreads); }

        ParallelForScheduler::~ParallelForScheduler()
            { this->JoinAllThreads(); }

        void ParallelForScheduler::CreatePool(int NumThreads)
        {
            this->Threads.reserve(NumThreads);
            for( int CurrThread = 0 ; CurrThread < NumThreads ; ++CurrThread )
            {
                auto ThreadFunct = [this]() {
                    const unsigned int MyThreadIndex{ btGetCurrentThreadIndex() - 1 };
                    while(11)
                    {
                        StartingLine.Wait(); // Add error checking, this should be false
                        if(!StillRunning)
                            { break; }
                        const int RangeCount = IndexEnd - IndexStart;
                        const int PerThread = ( RangeCount / this->getNumThreads() ) + 1;
                        const int MyStart = std::min(PerThread * MyThreadIndex, static_cast<unsigned int>(IndexEnd));
                        const int MyEnd = std::min(MyStart + PerThread, IndexEnd);
                        Body->forLoop(MyStart, MyEnd);
                        FinishLine.Wait();
                    }
                };
                this->Threads.emplace_back(ThreadFunct);
            }
        }

        void ParallelForScheduler::JoinAllThreads()
        {
            StillRunning = false;
            StartingLine.Wait(); // Should be true here
            for( std::thread& CurrThread : this->Threads )
            {
                if( CurrThread.joinable() ) {
                    CurrThread.join();
                }
            }
        }

        int ParallelForScheduler::getMaxNumThreads() const
            { return std::thread::hardware_concurrency(); }

        int ParallelForScheduler::getNumThreads() const
            { return this->Threads.size(); }

        void ParallelForScheduler::setNumThreads(int numThreads)
        {
            this->JoinAllThreads();
            this->Threads.clear();
            this->CreatePool(numThreads);
        }

        void ParallelForScheduler::parallelFor(int iBegin, int iEnd, int grainSize, const btIParallelForBody& body)
        {
            const int RangeCount = iEnd - iBegin;
            if( RangeCount < 1 ) {
                return;
            }

            btPushThreadsAreRunning();
            this->Body = &body;
            this->IndexStart = iBegin;
            this->IndexEnd = iEnd;
            this->StartingLine.Wait(); // Check that this is true.
            // all the threads are working, we promise!
            this->FinishLine.Wait(); // Don't care about return value
            btPopThreadsAreRunning();
        }
    } //Physics
} //Mezzanine

#endif
