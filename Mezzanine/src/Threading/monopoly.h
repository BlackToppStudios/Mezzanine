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
#ifndef _threadmonopoly_h
#define _threadmonopoly_h

#include "datatypes.h"

#if !defined(SWIG) || defined(SWIG_THREADING) // Do not read when in swig and not in the threading module
#include "doublebufferedresource.h"
#include "workunit.h"
#endif

/// @file
/// @brief Contains an interface for a kind of WorkUnit that consumes time on multiple thread

namespace Mezzanine
{
    namespace Threading
    {
        class FrameScheduler;

        /// @brief A kind of workunit given exclusive runtime so it can consume time on multiple threads
        /// @details A @ref FrameScheduler executes monopolies before other and will allow them to run with on every thread.
        class MEZZ_LIB MonopolyWorkUnit : public DefaultWorkUnit
        {
            public:
                /// @brief Provides a hint to the monopoly as to how many threads it should use.
                /// @param AmountToUse The amount of threads you would like the monopoly to consume.
                virtual void UseThreads(const Whole& AmountToUse) = 0;

                /// @brief Retrieves the amount of threads that the monopoly will actually use.
                /// @details The monopoly is free to ignore any value passed @ref UseThreads and
                /// use any heuristic or threading model instead.
                /// @return A whole with the amount of threads to be used.
                virtual Whole UsingThreadCount() = 0;

                /// @brief A virtual destructor, currently empty.
                virtual ~MonopolyWorkUnit();
        };//MonopolyWorkUnit
    }//Threading
}//Mezzanine
#endif
