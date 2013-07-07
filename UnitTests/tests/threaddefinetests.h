//©opyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _threaddefinetests_h
#define _threaddefinetests_h

#include "main.h"


/// @brief A simple test to see if the threading compiler flag are set correctly
class threaddefinetests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "threaddefine"
        virtual String Name()
            { return String("threaddefine"); }

        /// @copydoc Mezzanine::Testing::UnitTestGroup::RunTests
        /// @detail A series on interactive tests allowing inspection and verification of CMake and compiler options.
        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            RunAutomaticTests = false; // unsused, this line prevents compiler warnings

            if (RunInteractiveTests)
            {
                #ifdef MEZZ_USEBARRIERSEACHFRAME
                                AddTestResult("DAGFrameScheduler::Threading::MEZZ_USEBARRIERSEACHFRAME", GetTestAnswerFromStdin( "Barriers used to absolutely minimize thread creation. Is this corrent?"));
                #else
                                AddTestResult("DAGFrameScheduler::Threading::MEZZ_USEBARRIERSEACHFRAME", GetTestAnswerFromStdin("Threads created and joined each frame. Barriers not set for use, is that correct?"));
                #endif

                #ifdef MEZZ_DEBUG
                                AddTestResult("DAGFrameScheduler::Threading::MEZZDEBUG", GetTestAnswerFromStdin( "Was MEZZDEBUG set to True when this was configured with CMake(where it is called Mezz_BuildTypeDebug) or other config tool? "));
                #else
                                AddTestResult("DAGFrameScheduler::Threading::MEZZDEBUG", GetTestAnswerFromStdin( "Was MEZZDEBUG set to False when this was configured with CMake(where it is called Mezz_BuildTypeDebug) or other config tool? "));
                #endif

            }else{
                AddTestResult("DAGFrameScheduler::Threading::MEZZ_DEBUG", Skipped);
                AddTestResult("DAGFrameScheduler::Threading::MEZZ_USEBARRIERSEACHFRAME", Skipped);
            }

        }
};
#endif
