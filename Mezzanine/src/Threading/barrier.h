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
#ifndef _barrier_h
#define _barrier_h

#include "datatypes.h"
#include "atomicoperations.h"



/// @file
/// @brief The declaration of the @ref Mezzanine::Threading::Barrier Barrier synchronization primitive

namespace Mezzanine
{
    namespace Threading
    {
        /// @brief A synchronization primitive that causes a predefined number of threads to all wait before continuing.
        class MEZZ_LIB Barrier
        {
            protected:
                /// @brief The number of threads to have wait.
                Int32 ThreadGoal;

                /// @brief The number of threads currently waiting.
                Int32 ThreadCurrent;

            public:
                /// @brief Constructor
                /// @param SynchThreadCount The amount of threads that this should wait for. If 0 is passed all threads waiting advance.
                Barrier (const Int32& SynchThreadCount);

                /// @brief Wait until the specified number of threads reach this point.
                /// @return The last thread to reach this point gets true, the others are returned false.
                bool Wait ();

                /// @brief Set the Thread count Atomically.
                /// @param NewCount The new amounf threads to sync.
                void SetThreadSyncCount(Int32 NewCount);

        };//Barrier
    }//Threading
}//Mezzanine
#endif
