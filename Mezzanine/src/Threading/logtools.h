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
#ifndef _logtools_h
#define _logtools_h

#include "datatypes.h"
#include "systemcalls.h"

#include <ostream>

/// @file
/// @brief The definitions of the logging tools

namespace Mezzanine
{
    namespace Threading
    {
        #ifndef SCOPEDTIMER
            /// @def SCOPEDTIMER
            /// @brief The easiest way to time the given scope
            /// @param Stream
            #ifdef __FUNCTION__
                #define SCOPEDTIMER(Stream) ScopedTimer TimeThisScope( Stream, __FUNCTION__, __FILE__, __LINE__ );
            #else
                #define SCOPEDTIMER(Stream) ScopedTimer TimeThisScope( Stream, __func__, __FILE__, __LINE__ );
            #endif
        #endif

        /// @brief Used to time fromthis objects creation to its desctruction
        class ScopedTimer
        {
            private:
                /// @brief The place to send the data.
                std::ostream& LogTarget;

                /// @brief Name of the function this was created in.
                String ScopeName;

                /// @brief Either empty of the name of the file the scope is in.
                String FileName;

                /// @brief A timestamp in microseconds set on creation.
                MaxInt StartTime;

                /// @brief The line this was instantiated on or 0.
                Whole Line;

            public:
                /// @brief In a ScopedTimerBegin xml element this logs every parameter and the current timestamp.
                /// @param LogSink The target to send log messages to.
                /// @param ScopeToMeasure The name of the current function or a user provided name.
                /// @param ScopeFile Defaults to an empty string, should be set to __FILE__ or similar.
                /// @param CurrentLine Defaults to 0, should be set to __LINE__ or similar.
                ScopedTimer(std::ostream& LogSink,
                            String ScopeToMeasure,
                            String ScopeFile = String(),
                            Whole CurrentLine = 0
                            );

                /// @brief Destructor, logs duration and current timestamp in ScopedTimerEnd xml element
                ~ScopedTimer();
        };
    }//Threading
}//Mezzanine
#endif
