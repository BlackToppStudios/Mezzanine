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
#ifndef _DAGenumerations_h
#define _DAGenumerations_h

#include "datatypes.h"

/// @file
/// @brief Any enumerations the threading library requires are all declared here.

namespace Mezzanine
{
    namespace Threading
    {
        /// @brief Used to track whether a thread has started, completed, etc...
        enum RunningState
        {
            NotStarted=0,   ///< Task is not yet started this frame, this can change without notice.
            Starting=1,     ///< Only used when a thread successfully attempts to gain ownership of a task, or some other tasks successfully starts
            Running=2,      ///< Task is running when the value was check, it could become Complete or Failed with no notice.
            Complete=3,     ///< Thread has completed all work this from frame, will not change until this frame ends.
            Failed=4        ///< Indicates an abnormal termination of a Workunit or other failure, Likely the whole application will need to stop.
        };//RunningState
    }//Threading
}//Mezzanine
#endif
